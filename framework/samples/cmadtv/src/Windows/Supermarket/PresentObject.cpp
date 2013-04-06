#include "cmadtv/Windows/Supermarket/PresentObject.h"

#include "cgui/DragDrop/DragDropSystem.h"
#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/Image.h"
#include "cgui/Gui/TextBox.h"
#include "cgui/Tooltip/TooltipSystem.h"

#include "cmadtv/Data/Betty/Present.h"
#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Player/Player.h"

#include "cmadtv/DragDrop/PresentDragController.h"
#include "cmadtv/Tooltip/PresentTooltipController.h"

using namespace CMadTV;
    
////////////////////////////////////////////////////////////////////////////////////////////
///
/// PresentObject
///
////////////////////////////////////////////////////////////////////////////////////////////
        
PresentObjectPtr PresentObject::Create( PresentPtr _spPresent, bool _bTooltip, bool _bDrag )
{
    // Create panel
    CGui::PanelPtr spPnlRoot = CGui::GuiMain::GetInstance().CreateFromXML< CGui::Panel >( "data/windows/PresentObject.xml" );

    // Create object
    PresentObjectPtr spObject( new PresentObject( _bTooltip, _bDrag ) );
    spObject->Init( spPnlRoot );
    spObject->SetPresent( _spPresent );
    return spObject;
}

PresentObject::PresentObject( bool _bTooltip, bool _bDrag )
:   m_bVisibleDuringDrag( true )
{
    if ( _bDrag )
    {
        // Create delegates
        m_spDelegateDragStarted = CGui::DragEventDelegatePtr( new CGui::DragEventDelegate( this, &PresentObject::OnDragStarted ) );
        m_spDelegateDragUndone = CGui::DragEventDelegatePtr( new CGui::DragEventDelegate( this, &PresentObject::OnDragUndone ) );

        m_spDragController = PresentDragControllerPtr( new PresentDragController() );
        m_spDragController->RegisterDelegate( CGui::EVENT_DRAG_STARTED, m_spDelegateDragStarted );
        m_spDragController->RegisterDelegate( CGui::EVENT_DRAG_UNDONE, m_spDelegateDragUndone );
    }

    if ( _bTooltip )
    {
        m_spTooltipController = PresentTooltipControllerPtr( new PresentTooltipController() );
    }
}
        
PresentObject::~PresentObject()
{
    if ( m_spDragController != NULL )
    {
        m_spDragController->RemoveDelegate( CGui::EVENT_DRAG_UNDONE, m_spDelegateDragUndone );
        m_spDragController->RemoveDelegate( CGui::EVENT_DRAG_STARTED, m_spDelegateDragStarted );
    }
}

void PresentObject::Init( CGui::PanelPtr _spPnlRoot )
{
    if ( m_spPnlRoot != NULL )
    {
        //CGui::DragDropSystem::GetInstance().RemoveDragController( m_spObject );
        CGui::TooltipSystem::GetInstance().RemoveTooltipController( m_spPnlRoot );
    }

    m_spPnlRoot = _spPnlRoot;

    if ( m_spPnlRoot != NULL )
    {
        m_spImgImage = static_pointer_cast< CGui::Image >( m_spPnlRoot->FindChild( "ImgImage" ) );
        m_spTbPrice = static_pointer_cast< CGui::TextBox >( m_spPnlRoot->FindChild( "TbPrice" ) );
        m_spPnlRoot->SetVisible( false );
        m_spPnlRoot->SetDraggable( true );
        CGui::DragDropSystem::GetInstance().SetDragController( m_spPnlRoot, m_spDragController );
        CGui::TooltipSystem::GetInstance().SetTooltipController( m_spPnlRoot, m_spTooltipController );
    }
    else
    {
        m_spImgImage.reset();
        m_spTbPrice.reset();
    }
}
        
void PresentObject::SetPlayer( PlayerPtr _spPlayer )
{
    //m_spTooltipController->SetPlayer( _spPlayer );
    m_spDragController->SetPlayer( _spPlayer );
}

void PresentObject::SetPresent( PresentPtr _spPresent )
{
    if ( _spPresent != NULL )
    {
        SAFE_CALL( m_spPnlRoot )->SetVisible( true );
        SAFE_CALL( m_spImgImage )->SetTexture( _spPresent->GetImage() );
        SAFE_CALL( m_spTbPrice )->SetText( CFoundation::String::Build( "%.0f", _spPresent->GetPrice().GetAmount( CFoundation::CURRENCY_DOLLAR ) ) );
    }
    else
    {
        SAFE_CALL( m_spPnlRoot )->SetVisible( false );
    }
    SAFE_CALL( m_spDragController )->SetPresent( _spPresent );
    SAFE_CALL( m_spTooltipController )->SetPresent( _spPresent );
}
        
PresentPtr PresentObject::GetPresent() const
{
    return m_spTooltipController->GetPresent();
}
        
void PresentObject::ShowPrice( bool _bShow )
{
    SAFE_CALL( m_spTbPrice )->SetVisible( _bShow );
}
        
void PresentObject::SetVisibleDuringDrag( bool _bVisible )
{
    m_bVisibleDuringDrag = _bVisible;
}
        
CGui::GuiObjectPtr PresentObject::GetObject() const
{
    return m_spPnlRoot;
}

void PresentObject::OnDragStarted( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spGuiObject, CGui::DragDataPtr _spDragData )
{
    if ( !m_bVisibleDuringDrag )
    {
        m_spPnlRoot->SetVisible( false );
    }
}

void PresentObject::OnDragUndone( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spGuiObject, CGui::DragDataPtr _spDragData )
{
    m_spPnlRoot->SetVisible( true );
}
