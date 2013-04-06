#include "cmadtv/Windows/BroadcastDealerWindow.h"

#include "cgui/DragDrop/DragDropSystem.h"
#include "cgui/Gui/Image.h"
#include "cgui/Tooltip/TooltipSystem.h"

#include "cmadtv/Data/Broadcast.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Player/BroadcastBag.h"
#include "cmadtv/Data/Player/BroadcastRack.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerContext.h"

#include "cmadtv/Logic/PlayerInput.h"
#include "cmadtv/Logic/Commands/MoveBroadcastCmd.h"

#include "cmadtv/DragDrop/DealerDropController.h"
#include "cmadtv/DragDrop/DealerBroadcastDragController.h"
#include "cmadtv/Tooltip/BroadcastTooltipController.h"
#include "cmadtv/Windows/BroadcastDealer/BroadcastBagPanel.h"

using namespace CMadTV;
        
CFoundation::String BroadcastDealerWindow::GetXMLFileName()
{
    return "data/windows/BroadcastDealerWindow.xml";
}

BroadcastDealerWindow::BroadcastDealerWindow()
{
    // Create delegates
    m_spDelegateBroadcastAdded = BroadcastDealerEventDelegate2Ptr( new BroadcastDealerEventDelegate2( this, &BroadcastDealerWindow::OnBroadcastAdded ) );
    m_spDelegateBroadcastRemoved = BroadcastDealerEventDelegate2Ptr( new BroadcastDealerEventDelegate2( this, &BroadcastDealerWindow::OnBroadcastRemoved ) );

    m_spBagDropController = DealerDropControllerPtr( new DealerDropController( m_spContext, true ) );
}

BroadcastDealerWindow::~BroadcastDealerWindow()
{
}

void BroadcastDealerWindow::InitWindow()
{
    // Get dealer
    m_spImgDealer = GetImage( "ImgDealer" );

    // Create bag
    CGui::PanelPtr spPnlBag = GetPanel( "PnlBag" );
    CGui::DragDropSystem::GetInstance().SetDropController( spPnlBag, m_spBagDropController );
    m_spBag = BroadcastBagPanelPtr( new BroadcastBagPanel( spPnlBag ) );

    WindowController::InitWindow();
}
       
void BroadcastDealerWindow::Deinit()
{
    CGui::DragDropSystem::GetInstance().RemoveDropController( m_spImgDealer );
    CGui::DragDropSystem::GetInstance().RemoveDropController( m_spBag->GetObject() );

    // Remove delegates
    m_spDealer->RemoveDelegate( BROADCAST_DEALER_EVENT_BROADCAST_ADDED, m_spDelegateBroadcastAdded );
    m_spDealer->RemoveDelegate( BROADCAST_DEALER_EVENT_BROADCAST_REMOVED, m_spDelegateBroadcastRemoved );
}
		
void BroadcastDealerWindow::DeinitWindow()
{
    // Deinitialization
    Deinit();

    WindowController::DeinitWindow();
}
		
void BroadcastDealerWindow::Close( Unsigned32 _u32ReturnValue )
{
    if ( m_spPlayer != NULL )
    {
        // Move broadcasts from bag to rack
        BroadcastBag& clBag = m_spPlayer->GetBroadcastBag();
        std::vector< BroadcastPtr > aspBroadcasts = clBag.GetBroadcasts();
        Unsigned32 u32NumBroadcasts = aspBroadcasts.size();
        for ( Unsigned32 u32Idx = 0; u32Idx != u32NumBroadcasts; ++u32Idx )
        {
            BroadcastPtr spBroadcast = aspBroadcasts[ u32Idx ];

            MoveBroadcastCmdPtr spCmd( new MoveBroadcastCmd( m_spContext, spBroadcast, MoveBroadcastCmd::LOCATION_BAG, MoveBroadcastCmd::LOCATION_RACK ) );
            m_spContext->GetInput()->Execute( spCmd );
        }
    }

    WindowController::Close( _u32ReturnValue );
}

void BroadcastDealerWindow::OnContextChanged()
{
    // Update bag
    m_spBag->SetContext( m_spContext );
    m_spBagDropController->SetContext( m_spContext );

    SetGame( m_spContext != NULL ? m_spContext->GetGame() : GamePtr() );
    SetPlayer( m_spContext != NULL ? m_spContext->GetPlayer() : PlayerPtr() );
}
        
void BroadcastDealerWindow::SetGame( GamePtr _spGame )
{
    if ( _spGame == m_spGame )
    {
        return;
    }

    m_spGame = _spGame;

    // Set dealer
    SetDealer( m_spGame != NULL ? m_spGame->GetBroadcastDealer() : BroadcastDealerPtr() );
}
        
void BroadcastDealerWindow::SetPlayer( PlayerPtr _spPlayer )
{
    if ( _spPlayer == m_spPlayer )
    {
        return;
    }

    m_spPlayer = _spPlayer;

    SetGame( m_spPlayer != NULL ? m_spPlayer->GetGame() : GamePtr() );
}
        
void BroadcastDealerWindow::SetDealer( BroadcastDealerPtr _spDealer )
{
    if ( _spDealer == m_spDealer )
    {
        return;
    }

    m_spDealer = _spDealer;

    if ( m_spDealer != NULL )
    {
        // Register delegates
        m_spDealer->RegisterDelegate( BROADCAST_DEALER_EVENT_BROADCAST_ADDED, m_spDelegateBroadcastAdded );
        m_spDealer->RegisterDelegate( BROADCAST_DEALER_EVENT_BROADCAST_REMOVED, m_spDelegateBroadcastRemoved );

        // Get broadcast objects
        for ( Unsigned32 u32Idx = 0; u32Idx != BroadcastDealer::NUM_SLOTS; ++u32Idx )
        {
            // Get object
            CGui::GuiObjectPtr spObject = GetGuiObject( CFoundation::String::Build( "ImgBroadcast%d", u32Idx + 1 ) );
            if ( spObject == NULL )
            {
                continue;
            }

            // Create broadcast object
            BroadcastObjectPtr spBroadcastObject( new BroadcastObject() );
            spBroadcastObject->Init( spObject );

            // Set broadcast
            spBroadcastObject->SetBroadcast( m_spDealer->GetBroadcast( u32Idx ) );

            m_aspBroadcastObjects[ u32Idx ] = spBroadcastObject;
        }
    }

    // Set dealer drop controller
    CGui::DropControllerPtr spDealerDropController( new DealerDropController( m_spContext, false ) );
    CGui::DragDropSystem::GetInstance().SetDropController( m_spImgDealer, spDealerDropController );    
}

void BroadcastDealerWindow::OnBroadcastAdded( BroadcastPtr _spBroadcast, Unsigned32 _u32Slot )
{
    // Get broadcast object
    BroadcastObjectPtr spBroadcastObject = m_aspBroadcastObjects[ _u32Slot ];

    // Set broadcast
    spBroadcastObject->SetBroadcast( _spBroadcast );
}

void BroadcastDealerWindow::OnBroadcastRemoved( BroadcastPtr _spBroadcast, Unsigned32 _u32Slot )
{
    // Get broadcast object
    BroadcastObjectPtr spBroadcastObject = m_aspBroadcastObjects[ _u32Slot ];

    // Remove broadcast
    spBroadcastObject->SetBroadcast( BroadcastPtr() );
}

////////////////////////////////////////////////////////////////////////////////////////////
///
/// BroadcastDealerWindow::BroadcastObject
///
////////////////////////////////////////////////////////////////////////////////////////////

BroadcastDealerWindow::BroadcastObject::BroadcastObject()
{
    // Create delegates
    m_spDelegateDragStarted = CGui::DragEventDelegatePtr( new CGui::DragEventDelegate( this, &BroadcastDealerWindow::BroadcastObject::OnDragStarted ) );

    m_spBroadcastDragController = DealerBroadcastDragControllerPtr( new DealerBroadcastDragController() );
    m_spBroadcastDragController->RegisterDelegate( CGui::EVENT_DRAG_STARTED, m_spDelegateDragStarted );
    m_spDelegateDragUndone = m_spBroadcastDragController->RegisterDelegate( CGui::EVENT_DRAG_UNDONE, CFoundation::CreateDelegate( this, &BroadcastObject::OnDragUndone ) );
    m_spDelegateDragEnded = m_spBroadcastDragController->RegisterDelegate( CGui::EVENT_DRAG_ENDED, CFoundation::CreateDelegate( this, &BroadcastObject::OnDragEnded ) );
  
    m_spBroadcastTooltipController = BroadcastTooltipControllerPtr( new BroadcastTooltipController() );
}
        
BroadcastDealerWindow::BroadcastObject::~BroadcastObject()
{
    m_spBroadcastDragController->RemoveDelegate( CGui::EVENT_DRAG_STARTED, m_spDelegateDragStarted );
    DISCONNECT_DELEGATE_CONNECTION( m_spDelegateDragUndone );
    DISCONNECT_DELEGATE_CONNECTION( m_spDelegateDragEnded );
}

void BroadcastDealerWindow::BroadcastObject::Init( CGui::GuiObjectPtr _spObject )
{
    if ( m_spObject != NULL )
    {
        CGui::DragDropSystem::GetInstance().RemoveDragController( m_spObject );
        CGui::TooltipSystem::GetInstance().RemoveTooltipController( m_spObject );
    }

    m_spObject = _spObject;

    if ( m_spObject != NULL )
    {
        m_spObject->SetDraggable( true );
        CGui::DragDropSystem::GetInstance().SetDragController( m_spObject, m_spBroadcastDragController );
        CGui::TooltipSystem::GetInstance().SetTooltipController( m_spObject, m_spBroadcastTooltipController );
    }
}

void BroadcastDealerWindow::BroadcastObject::SetBroadcast( BroadcastPtr _spBroadcast )
{
    if ( m_spObject != NULL )
    {
        m_spObject->SetVisible( _spBroadcast != NULL );
    }
    m_spBroadcastDragController->SetBroadcast( _spBroadcast );
    m_spBroadcastTooltipController->SetBroadcast( _spBroadcast );
}

void BroadcastDealerWindow::BroadcastObject::OnDragStarted( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spGuiObject, CGui::DragDataPtr _spDragData )
{
    m_spObject->SetVisible( false );
}

void BroadcastDealerWindow::BroadcastObject::OnDragUndone()
{
    m_spObject->SetVisible( true );
}

void BroadcastDealerWindow::BroadcastObject::OnDragEnded()
{
}
