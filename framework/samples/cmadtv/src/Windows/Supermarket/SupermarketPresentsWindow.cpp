#include "cmadtv/Windows/Supermarket/SupermarketPresentsWindow.h"

#include "cgui/DragDrop/DragDropSystem.h"
#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/Image.h"
#include "cgui/Gui/TextBox.h"
#include "cgui/Tooltip/TooltipSystem.h"

#include "cmadtv/Data/Betty/Present.h"
#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerCompBetty.h"
#include "cmadtv/Data/Player/PlayerContext.h"

#include "cmadtv/DragDrop/PresentDragController.h"
#include "cmadtv/DragDrop/PresentDropController.h"
#include "cmadtv/Windows/Supermarket/PresentObject.h"

using namespace CMadTV;
        
CFoundation::String SupermarketPresentsWindow::GetXMLFileName()
{
    return "data/windows/SupermarketPresentsWindow.xml";
}

SupermarketPresentsWindow::SupermarketPresentsWindow()
{
    // Create delegates
    m_spDelegateBettyPresentChanged = PlayerCompBettyEventPresentDelegate1Ptr( new PlayerCompBettyEventPresentDelegate1( this, &SupermarketPresentsWindow::OnBettyPresentChanged ) );
    
    m_spBagDropController = PresentDropControllerPtr( new PresentDropController( PresentDropController::TYPE_BAG ) );
    m_spPresentsDropController = PresentDropControllerPtr( new PresentDropController( PresentDropController::TYPE_PRESENTS ) );
}

SupermarketPresentsWindow::~SupermarketPresentsWindow()
{
}

void SupermarketPresentsWindow::InitWindow()
{
    m_spPnlPresents = GetPanel( "PnlPresentsBox" );
    CGui::DragDropSystem::GetInstance().SetDropController( m_spPnlPresents, m_spPresentsDropController );

    // Get broadcast object template
    m_spObjPresentTemplate = static_pointer_cast< CGui::Panel >( m_spPnlPresents->FindChild( "ObjPresentTemplate" ) );
    m_spObjPresentTemplate->RemoveFromParent();
    m_spObjPresentAnchor1 = m_spPnlPresents->FindChild( "ObjPresentAnchor1" );
    m_spObjPresentAnchor2 = m_spPnlPresents->FindChild( "ObjPresentAnchor2" );

    // Init bag
    CGui::PanelPtr spPnlBag = GetPanel( "PnlBag" );
    CGui::DragDropSystem::GetInstance().SetDropController( spPnlBag, m_spBagDropController );
    m_spBagPresentObject = PresentObjectPtr( new PresentObject() );
    m_spBagPresentObject->Init( static_pointer_cast< CGui::Panel >( spPnlBag->FindChild( "ObjPresent" ) ) );
    m_spBagPresentObject->SetVisibleDuringDrag( false );

    WindowController::InitWindow();
}
		
void SupermarketPresentsWindow::DeinitWindow()
{
    // Deinit bag
    CGui::PanelPtr spPnlBag = GetPanel( "PnlBag" );
    CGui::DragDropSystem::GetInstance().RemoveDropController( spPnlBag );

    CGui::DragDropSystem::GetInstance().RemoveDropController( m_spPnlPresents );

    WindowController::DeinitWindow();
}
        
void SupermarketPresentsWindow::OnContextChanged()
{
    SetPlayer( m_spContext != NULL ? m_spContext->GetPlayer() : PlayerPtr() );

    // Init presents
    DatabasePtr spDatabase = m_spContext != NULL ? m_spContext->GetDatabase() : DatabasePtr();
    if ( spDatabase != NULL )
    {
        const PresentVec& aspPresents = spDatabase->GetPresents();
        Unsigned32 u32NumPresents = aspPresents.size();
        for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPresents; ++u32Idx )
        {
            PresentPtr spPresent = aspPresents[ u32Idx ];
            AddPresentObject( spPresent );
        }
    }
}
        
void SupermarketPresentsWindow::SetPlayer( PlayerPtr _spPlayer )
{
    if ( _spPlayer == m_spPlayer )
    {
        return;
    }

    if ( m_spPlayer != NULL )
    {
        // Remove delegate
        PlayerCompBettyPtr spCompBetty = m_spPlayer->GetCompBetty();
        spCompBetty->RemoveDelegate( EVENT_BETTY_PRESENT_CHANGED, m_spDelegateBettyPresentChanged );

        SetBagPresent( PresentPtr() );

        // Disable present return
        spCompBetty->EnablePresentReturn( false );
    }

    m_spPlayer = _spPlayer;

    // Setup drop controllers
    m_spBagDropController->SetPlayer( m_spPlayer );
    m_spPresentsDropController->SetPlayer( m_spPlayer );

    m_spBagPresentObject->SetPlayer( m_spPlayer );

    if ( m_spPlayer != NULL )
    {
        // Register delegate
        PlayerCompBettyPtr spCompBetty = m_spPlayer->GetCompBetty();
        spCompBetty->RegisterDelegate( EVENT_BETTY_PRESENT_CHANGED, m_spDelegateBettyPresentChanged );

        // Init bag
        SetBagPresent( spCompBetty->GetPresent() );
    }
}
        
void SupermarketPresentsWindow::AddPresentObject( PresentPtr _spPresent )
{
    // Create gui object
    CGui::PanelPtr spGuiObject = CGui::GuiMain::GetInstance().CreateFromTemplate( "ObjPresent", m_spObjPresentTemplate );
    
    // Connect to panel
    CGui::GuiObjectPtr spPresentAnchor = GetPresentObjectAnchor();
    m_spPnlPresents->AddChildAfter( spGuiObject, spPresentAnchor );

    // Create object
    PresentObjectPtr spPresentObject( new PresentObject() );
    spPresentObject->Init( spGuiObject );
    spPresentObject->SetPresent( _spPresent );
    m_aspPresentObjects.push_back( spPresentObject );
}

void SupermarketPresentsWindow::RemovePresentObject( PresentPtr _spPresent )
{
    Unsigned32 u32NumPresentObjects = m_aspPresentObjects.size();
    if ( u32NumPresentObjects == 0 )
    {
        return;
    }

    // Move broadcast after removed broadcast one forward
    bool bAfterRemoved = false;
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPresentObjects; ++u32Idx )
    {
        PresentObjectPtr spPresentObject = m_aspPresentObjects[ u32Idx ];

        // Get next broadcast
        PresentPtr spNextPresent;
        if ( u32Idx < u32NumPresentObjects - 1 )
        {
            spNextPresent = m_aspPresentObjects[ u32Idx + 1 ]->GetPresent();
        }

        if ( bAfterRemoved )
        {
            spPresentObject->SetPresent( spNextPresent );
        }
        else if ( spPresentObject->GetPresent() == _spPresent )
        {
            bAfterRemoved = true;
            spPresentObject->SetPresent( spNextPresent );
        }
    }

    // Remove last broadcast
    if ( bAfterRemoved )
    {
        PresentObjectVec::iterator it = m_aspPresentObjects.end() - 1;
        PresentObjectPtr spPresentObject = *it;

        CGui::GuiObjectPtr spGuiObject = spPresentObject->GetObject();
        m_spPnlPresents->RemoveChild( spGuiObject );
        m_aspPresentObjects.erase( it );
    }
}
        
CGui::GuiObjectPtr SupermarketPresentsWindow::GetPresentObjectAnchor() const
{
    Unsigned32 u32NumPresentObjects = m_aspPresentObjects.size();
    if ( u32NumPresentObjects == 0 )
    {
        return m_spObjPresentAnchor1;
    }
    else if ( u32NumPresentObjects == 5 )
    {
        return m_spObjPresentAnchor2;
    }
    else
    {
        PresentObjectPtr spLastPresentObject = *( m_aspPresentObjects.end() - 1 );
        return spLastPresentObject->GetObject();
    }
}
        
void SupermarketPresentsWindow::SetBagPresent( PresentPtr _spPresent )
{
    m_spBagPresentObject->SetPresent( _spPresent );
}
        
void SupermarketPresentsWindow::OnBettyPresentChanged( PresentPtr _spPresent )
{
    SetBagPresent( _spPresent );
}
    