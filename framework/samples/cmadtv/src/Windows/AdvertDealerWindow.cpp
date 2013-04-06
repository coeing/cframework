#include "cmadtv/Windows/AdvertDealerWindow.h"

#include "cgui/DragDrop/DragDropSystem.h"
#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/Image.h"
#include "cgui/Tooltip/TooltipSystem.h"

#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Player/AdvertContracts.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerContext.h"

#include "cmadtv/Logic/PlayerInput.h"
#include "cmadtv/Logic/Commands/MoveAdvertCmd.h"

#include "cmadtv/DragDrop/DealerDropController.h"
#include "cmadtv/DragDrop/AdvertDragController.h"
#include "cmadtv/Tooltip/AdvertTooltipController.h"

using namespace CMadTV;
    
CFoundation::String AdvertDealerWindow::GetXMLFileName()
{
    return "data/windows/AdvertDealerWindow.xml";
}

AdvertDealerWindow::AdvertDealerWindow()
{
    m_spBagDropController = DealerDropControllerPtr( new DealerDropController( m_spContext, true ) );
    m_spDealerDropController = DealerDropControllerPtr( new DealerDropController( m_spContext, false ) );
}

AdvertDealerWindow::~AdvertDealerWindow()
{
}

void AdvertDealerWindow::InitWindow()
{
    // Get dealer
    m_spImgDealer = GetImage( "ImgDealer" );
    CGui::DragDropSystem::GetInstance().SetDropController( m_spImgDealer, m_spDealerDropController );

    // Get bag
    CGui::PanelPtr spPnlBag = GetPanel( "PnlBag" );
    CGui::DragDropSystem::GetInstance().SetDropController( spPnlBag, m_spBagDropController );
    m_spBag = AdvertBagPanelPtr( new AdvertBagPanel( spPnlBag ) );
    
    // Get advert objects
    for ( Unsigned32 u32Idx = 0; u32Idx < AdvertDealer::NUM_SLOTS; ++u32Idx )
    {
        // Get object
        CGui::GuiObjectPtr spObject = GetGuiObject( CFoundation::String::Build( "ImgAdvert%d", u32Idx + 1 ) );
        if ( spObject == NULL )
        {
            continue;
        }

        // Create Advert object
        AdvertObjectPtr spAdvertObject( new AdvertObject( spObject ) );
        m_aspAdvertObjects[ u32Idx ] = spAdvertObject;
    }

    WindowController::InitWindow();
}
		
void AdvertDealerWindow::DeinitWindow()
{
    CGui::DragDropSystem::GetInstance().RemoveDropController( m_spBag->GetObject() );

    CGui::DragDropSystem::GetInstance().RemoveDropController( m_spImgDealer );

    WindowController::DeinitWindow();
}
		
void AdvertDealerWindow::Close( Unsigned32 _u32ReturnValue )
{
    if ( m_spPlayer != NULL )
    {
        // Move Adverts from bag to rack
        AdvertBag& clBag = m_spPlayer->GetAdvertBag();
        std::vector< AdvertPtr > aspAdverts = clBag.GetAdverts();
        Unsigned32 u32NumAdverts = aspAdverts.size();
        for ( Unsigned32 u32Idx = 0; u32Idx < u32NumAdverts; ++u32Idx )
        {
            AdvertPtr spAdvert = aspAdverts[ u32Idx ];

            // Start new contract
            MoveAdvertCmdPtr spCmd( new MoveAdvertCmd( m_spContext, spAdvert, MoveAdvertCmd::LOCATION_BAG, MoveAdvertCmd::LOCATION_CONTRACTS ) );
            m_spContext->GetInput()->Execute( spCmd );
        }
    }

    WindowController::Close( _u32ReturnValue );
}
        
void AdvertDealerWindow::OnContextChanged()
{
    SetPlayer( m_spContext != NULL ? m_spContext->GetPlayer() : PlayerPtr() );
    SetGame( m_spContext != NULL ? m_spContext->GetGame() : GamePtr() );

    m_spBagDropController->SetContext( m_spContext );
    m_spDealerDropController->SetContext( m_spContext );

    // Setup advert objects
    for ( Unsigned32 u32Idx = 0; u32Idx < AdvertDealer::NUM_SLOTS; ++u32Idx )
    {
        AdvertObjectPtr spAdvertObject = m_aspAdvertObjects[ u32Idx ];
        spAdvertObject->Set( m_spContext, m_spDealer != NULL ? m_spDealer->GetAdvert( u32Idx ) : AdvertPtr() );
    }

    m_spBag->SetContext( m_spContext );
}
        
void AdvertDealerWindow::SetGame( GamePtr _spGame )
{
    if ( _spGame == m_spGame )
    {
        return;
    }
    
    m_spGame = _spGame;

    SetDealer( m_spGame != NULL ? m_spGame->GetAdvertDealer() : AdvertDealerPtr() );
}

void AdvertDealerWindow::SetDealer( AdvertDealerPtr _spDealer )
{
    if ( _spDealer == m_spDealer )
    {
        return;
    }

    if ( m_spDealer != NULL )
    {
        // Remove delegates
        DISCONNECT_DELEGATE_CONNECTION( m_spDelegateAdvertAdded );
        DISCONNECT_DELEGATE_CONNECTION( m_spDelegateAdvertRemoved );
    }
    
    m_spDealer = _spDealer;

    if ( m_spDealer != NULL )
    {
        // Register delegates
        m_spDelegateAdvertAdded = m_spDealer->RegisterDelegate( ADVERT_DEALER_EVENT_ADVERT_ADDED, CreateDelegate( this, &AdvertDealerWindow::OnAdvertAdded ) );
        m_spDelegateAdvertRemoved = m_spDealer->RegisterDelegate( ADVERT_DEALER_EVENT_ADVERT_REMOVED, CreateDelegate( this, &AdvertDealerWindow::OnAdvertRemoved ) );
    }

    // Setup advert objects
    for ( Unsigned32 u32Idx = 0; u32Idx < AdvertDealer::NUM_SLOTS; ++u32Idx )
    {
        AdvertObjectPtr spAdvertObject = m_aspAdvertObjects[ u32Idx ];
        spAdvertObject->Set( m_spContext, m_spDealer != NULL ? m_spDealer->GetAdvert( u32Idx ) : AdvertPtr() );
    }
}

void AdvertDealerWindow::SetPlayer( PlayerPtr _spPlayer )
{
    if ( _spPlayer == m_spPlayer )
    {
        return;
    }
    
    m_spPlayer = _spPlayer;
}

void AdvertDealerWindow::OnAdvertAdded( AdvertPtr _spAdvert, Unsigned32 _u32Slot )
{
    // Get advert object
    AdvertObjectPtr spAdvertObject = m_aspAdvertObjects[ _u32Slot ];

    // Set advert
    spAdvertObject->Set( m_spContext, _spAdvert );
}

void AdvertDealerWindow::OnAdvertRemoved( AdvertPtr _spAdvert, Unsigned32 _u32Slot )
{
    // Get advert object
    AdvertObjectPtr spAdvertObject = m_aspAdvertObjects[ _u32Slot ];

    // Set advert
    spAdvertObject->Set( m_spContext, AdvertPtr() );
}

////////////////////////////////////////////////////////////////////////////////////////////
///
/// AdvertDealerWindow::AdvertObject
///
////////////////////////////////////////////////////////////////////////////////////////////

AdvertDealerWindow::AdvertObject::AdvertObject( CGui::GuiObjectPtr _spObject )
:   m_spObject( _spObject )
{
    // Create delegates
    m_spDelegateDragStarted = CGui::DragEventDelegatePtr( new CGui::DragEventDelegate( this, &AdvertDealerWindow::AdvertObject::OnDragStarted ) );
    m_spDelegateDragUndone = CGui::DragEventDelegatePtr( new CGui::DragEventDelegate( this, &AdvertDealerWindow::AdvertObject::OnDragUndone ) );

    m_spAdvertDragController = AdvertDragControllerPtr( new AdvertDragController( ADVERT_DRAG_TYPE_DEALER ) );
    m_spAdvertDragController->RegisterDelegate( CGui::EVENT_DRAG_STARTED, m_spDelegateDragStarted );
    m_spAdvertDragController->RegisterDelegate( CGui::EVENT_DRAG_UNDONE, m_spDelegateDragUndone );

    m_spAdvertTooltipController = AdvertTooltipControllerPtr( new AdvertTooltipController() );

    if ( m_spObject != NULL )
    {
        m_spObject->SetDraggable( true );
        CGui::DragDropSystem::GetInstance().SetDragController( m_spObject, m_spAdvertDragController );
        CGui::TooltipSystem::GetInstance().SetTooltipController( m_spObject, m_spAdvertTooltipController );
    }
}
        
AdvertDealerWindow::AdvertObject::~AdvertObject()
{
    if ( m_spObject != NULL )
    {
        CGui::DragDropSystem::GetInstance().RemoveDragController( m_spObject );
        CGui::TooltipSystem::GetInstance().RemoveTooltipController( m_spObject );
    }

    m_spAdvertDragController->RemoveDelegate( CGui::EVENT_DRAG_UNDONE, m_spDelegateDragUndone );
    m_spAdvertDragController->RemoveDelegate( CGui::EVENT_DRAG_STARTED, m_spDelegateDragStarted );
}

void AdvertDealerWindow::AdvertObject::Set( PlayerContextPtr _spContext, AdvertPtr _spAdvert )
{
    if ( m_spObject != NULL )
    {
        m_spObject->SetVisible( _spAdvert != NULL );
    }
    m_spAdvertDragController->SetAdvert( _spAdvert );
    m_spAdvertTooltipController->Set( _spContext, _spAdvert );
}

void AdvertDealerWindow::AdvertObject::OnDragStarted( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spGuiObject, CGui::DragDataPtr _spDragData )
{
    m_spObject->SetVisible( false );
}

void AdvertDealerWindow::AdvertObject::OnDragUndone( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spGuiObject, CGui::DragDataPtr _spDragData )
{
    m_spObject->SetVisible( true );
}

////////////////////////////////////////////////////////////////////////////////////////////
///
/// AdvertBagPanel
///
////////////////////////////////////////////////////////////////////////////////////////////

AdvertBagPanel::AdvertBagPanel( CGui::PanelPtr _spPanel )
:   m_spPanel( _spPanel )
{
    // Get Advert object template
    m_spObjAdvertTemplate = m_spPanel->FindChild( "ObjAdvertTemplate" );
    m_spObjAdvertTemplate->RemoveFromParent();
    m_spObjAdvertAnchor1 = m_spPanel->FindChild( "ObjAdvertAnchor1" );
    m_spObjAdvertAnchor2 = m_spPanel->FindChild( "ObjAdvertAnchor2" );
}

AdvertBagPanel::~AdvertBagPanel()
{
    SetPlayer( PlayerPtr() );
}
        
CGui::GuiObjectPtr AdvertBagPanel::GetObject() const
{
    return m_spPanel;
}
       
void AdvertBagPanel::SetContext( PlayerContextPtr _spContext )
{
    m_spContext = _spContext;
    SetPlayer( m_spContext != NULL ? m_spContext->GetPlayer() : PlayerPtr() );
}
       
void AdvertBagPanel::SetPlayer( PlayerPtr _spPlayer )
{
    if ( _spPlayer == m_spPlayer )
    {
        return;
    }
    
    if ( m_spPlayer != NULL )
    {
        // Remove all adverts
        RemoveAllAdvertObjects();

        // Remove delegates
        DISCONNECT_DELEGATE_CONNECTION( m_spDelegateAdvertAdded );
        DISCONNECT_DELEGATE_CONNECTION( m_spDelegateAdvertRemoved );
    }

    m_spPlayer = _spPlayer;

    if ( m_spPlayer != NULL )
    {
        AdvertBag& clBag = m_spPlayer->GetAdvertBag();

        // Setup start Adverts
        Unsigned32 u32NumAdverts = clBag.GetNumAdverts();
        for ( Unsigned32 u32Idx = 0; u32Idx != u32NumAdverts; ++u32Idx )
        {
            AdvertPtr spAdvert = clBag.GetAdvert( u32Idx );
            AddAdvertObject( spAdvert );
        }

        // Register delegates
        m_spDelegateAdvertAdded = clBag.RegisterDelegate( ADVERT_BAG_EVENT_ADVERT_ADDED, CFoundation::CreateDelegate( this, &AdvertBagPanel::OnAdvertAdded ) );
        m_spDelegateAdvertRemoved = clBag.RegisterDelegate( ADVERT_BAG_EVENT_ADVERT_REMOVED, CFoundation::CreateDelegate( this, &AdvertBagPanel::OnAdvertRemoved ) );
    }
}
        
void AdvertBagPanel::AddAdvertObject( AdvertPtr _spAdvert )
{
    // Create gui object
    CGui::GuiObjectPtr spGuiObject = CGui::GuiMain::GetInstance().CreateFromTemplate( "ObjAdvert", m_spObjAdvertTemplate );
    
    // Connect to panel
    CGui::GuiObjectPtr spAdvertAnchor = GetAdvertObjectAnchor();
    m_spPanel->AddChildAfter( spGuiObject, spAdvertAnchor );

    // Create Advert object
    AdvertObjectPtr spAdvertObject( new AdvertObject( spGuiObject ) );
    spAdvertObject->Set( m_spContext, _spAdvert );
    m_aspAdvertObjects.push_back( spAdvertObject );
}
        
void AdvertBagPanel::RemoveAllAdvertObjects()
{
    Unsigned32 u32NumObjects = m_aspAdvertObjects.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumObjects; ++u32Idx )
    {        
        AdvertObjectPtr spAdvertObject = m_aspAdvertObjects[ u32Idx ];
        CGui::GuiObjectPtr spGuiObject = spAdvertObject->GetObject();
        m_spPanel->RemoveChild( spGuiObject );
    }
    m_aspAdvertObjects.clear();
}

void AdvertBagPanel::RemoveAdvertObject( AdvertPtr _spAdvert )
{
    Unsigned32 u32NumAdvertObjects = m_aspAdvertObjects.size();
    if ( u32NumAdvertObjects == 0 )
    {
        return;
    }

    // Move Advert after removed Advert one forward
    bool bAfterRemoved = false;
    for ( Unsigned32 u32Idx = 0; u32Idx != u32NumAdvertObjects; ++u32Idx )
    {
        AdvertObjectPtr spAdvertObject = m_aspAdvertObjects[ u32Idx ];

        // Get next Advert
        AdvertPtr spNextAdvert;
        if ( u32Idx < u32NumAdvertObjects - 1 )
        {
            spNextAdvert = m_aspAdvertObjects[ u32Idx + 1 ]->GetAdvert();
        }

        if ( bAfterRemoved )
        {
            spAdvertObject->Set( m_spContext, spNextAdvert );
        }
        else if ( spAdvertObject->GetAdvert() == _spAdvert )
        {
            bAfterRemoved = true;
            spAdvertObject->Set( m_spContext, spNextAdvert );
        }
    }

    // Remove last Advert
    if ( bAfterRemoved )
    {
        AdvertObjectVec::iterator it = m_aspAdvertObjects.end() - 1;
        AdvertObjectPtr spAdvertObject = *it;
        CGui::GuiObjectPtr spGuiObject = spAdvertObject->GetObject();
        m_spPanel->RemoveChild( spGuiObject );
        m_aspAdvertObjects.erase( it );
    }
}
        
CGui::GuiObjectPtr AdvertBagPanel::GetAdvertObjectAnchor() const
{
    Unsigned32 u32NumAdvertObjects = m_aspAdvertObjects.size();
    if ( u32NumAdvertObjects == 0 )
    {
        return m_spObjAdvertAnchor1;
    }
    else if ( u32NumAdvertObjects == AdvertBag::MAX_NUM_ADVERTS / 2 )
    {
        return m_spObjAdvertAnchor2;
    }
    else
    {
        AdvertObjectPtr spLastAdvertObject = *( m_aspAdvertObjects.end() - 1 );
        return spLastAdvertObject->GetObject();
    }
}

void AdvertBagPanel::OnAdvertAdded( AdvertPtr _spAdvert )
{
    AddAdvertObject( _spAdvert );
}

void AdvertBagPanel::OnAdvertRemoved( AdvertPtr _spAdvert )
{
    RemoveAdvertObject( _spAdvert );
}

////////////////////////////////////////////////////////////////////////////////////////////
///
/// AdvertBagPanel::AdvertObject
///
////////////////////////////////////////////////////////////////////////////////////////////

AdvertBagPanel::AdvertObject::AdvertObject( CGui::GuiObjectPtr _spObject )
:   m_spObject( _spObject )
{
    // Create delegates
    m_spDelegateDragStarted = CGui::DragEventDelegatePtr( new CGui::DragEventDelegate( this, &AdvertBagPanel::AdvertObject::OnDragStarted ) );
    m_spDelegateDragUndone = CGui::DragEventDelegatePtr( new CGui::DragEventDelegate( this, &AdvertBagPanel::AdvertObject::OnDragUndone ) );
    m_spDelegateDragEnded = CGui::DragEventDelegatePtr( new CGui::DragEventDelegate( this, &AdvertBagPanel::AdvertObject::OnDragEnded ) );

    m_spDragController = AdvertDragControllerPtr( new AdvertDragController( ADVERT_DRAG_TYPE_DEALER ) );
    m_spDragController->RegisterDelegate( CGui::EVENT_DRAG_STARTED, m_spDelegateDragStarted );
    m_spDragController->RegisterDelegate( CGui::EVENT_DRAG_UNDONE, m_spDelegateDragUndone );
    m_spDragController->RegisterDelegate( CGui::EVENT_DRAG_ENDED, m_spDelegateDragEnded );

    m_spTooltipController = AdvertTooltipControllerPtr( new AdvertTooltipController() );

    if ( m_spObject != NULL )
    {
        m_spObject->SetDraggable( true );
        CGui::DragDropSystem::GetInstance().SetDragController( m_spObject, m_spDragController );
        CGui::TooltipSystem::GetInstance().SetTooltipController( m_spObject, m_spTooltipController );
    }
}

AdvertBagPanel::AdvertObject::~AdvertObject()
{
    // Remove delegates
    m_spDragController->RemoveDelegate( CGui::EVENT_DRAG_ENDED, m_spDelegateDragEnded );
    m_spDragController->RemoveDelegate( CGui::EVENT_DRAG_UNDONE, m_spDelegateDragUndone );
    m_spDragController->RemoveDelegate( CGui::EVENT_DRAG_STARTED, m_spDelegateDragStarted );
}
        
CGui::GuiObjectPtr AdvertBagPanel::AdvertObject::GetObject() const
{
    return m_spObject;
}

void AdvertBagPanel::AdvertObject::Set( PlayerContextPtr _spContext, AdvertPtr _spAdvert )
{
    m_spAdvert = _spAdvert;
    m_spDragController->SetAdvert( m_spAdvert );
    m_spTooltipController->Set( _spContext, m_spAdvert );
}
        
AdvertPtr AdvertBagPanel::AdvertObject::GetAdvert() const
{
    return m_spAdvert;
}

void AdvertBagPanel::AdvertObject::OnDragStarted( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spGuiObject, CGui::DragDataPtr _spDragData )
{
    m_spObject->SetVisible( false );
}

void AdvertBagPanel::AdvertObject::OnDragUndone( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spGuiObject, CGui::DragDataPtr _spDragData )
{
    m_spObject->SetVisible( true );
}
        
void AdvertBagPanel::AdvertObject::OnDragEnded( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spGuiObject, CGui::DragDataPtr _spDragData )
{
}
