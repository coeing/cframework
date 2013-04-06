#include "cmadtv/DragDrop/DealerDropController.h"

#include "cmadtv/Data/Broadcast.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Player/BroadcastBag.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerContext.h"
#include "cmadtv/Data/World/AdvertDealer.h"
#include "cmadtv/Data/World/BroadcastDealer.h"

#include "cmadtv/DragDrop/AdvertDragController.h"
#include "cmadtv/DragDrop/DealerBroadcastDragController.h"

#include "cmadtv/Logic/PlayerInput.h"
#include "cmadtv/Logic/Commands/BuyBroadcastCmd.h"
#include "cmadtv/Logic/Commands/MoveAdvertCmd.h"
#include "cmadtv/Logic/Commands/SellBroadcastCmd.h"

using namespace CMadTV;

DealerDropController::DealerDropController( PlayerContextPtr _spContext, bool _bBag )
:   m_spContext( _spContext ),
    m_bBag( _bBag )
{
}

DealerDropController::~DealerDropController()
{
}

bool DealerDropController::Execute( CGui::GuiObjectPtr _spDragObject, CGui::DragDataPtr _spDragData, const CFoundation::Vector2Di& /*_vDragPosition*/, const CFoundation::Vector2Di& /*_vDropPosition*/ ) 
{
    // Broadcast or advert drag data expected
    DealerBroadcastDragDataPtr spBroadcastDragData = dynamic_pointer_cast< DealerBroadcastDragData >( _spDragData );
    AdvertDragDataPtr spAdvertDragData = dynamic_pointer_cast< AdvertDragData >( _spDragData );
    if ( spBroadcastDragData != NULL )
    {
        return ExecuteBroadcastDrop( spBroadcastDragData->m_spBroadcast );
    }
    else if ( spAdvertDragData != NULL )
    {
        return ExecuteAdvertDrop( spAdvertDragData->m_spAdvert );
    }
    else
    {
        return false;
    }
}
        
void DealerDropController::SetContext( PlayerContextPtr _spContext )
{
    m_spContext = _spContext;
}

bool DealerDropController::ExecuteBroadcastDrop( BroadcastPtr _spBroadcast )
{
    Unsigned32 u32ReturnValue = 0;
    if ( m_bBag )
    {
        // Buy broadcast
        BuyBroadcastCmdPtr spCmd( new BuyBroadcastCmd( m_spContext, _spBroadcast ) );
        u32ReturnValue = m_spContext->GetInput()->Execute( spCmd );
        switch ( u32ReturnValue )
        {
        case BuyBroadcastCmd::ERROR_BAG_FULL:
            {
                DispatchEvent( DEALER_DROP_EVENT_BAG_FULL, _spBroadcast );
            }
            break;
        case BuyBroadcastCmd::ERROR_INSUFFICIENT_MONEY:
            {
                DispatchEvent( DEALER_DROP_EVENT_INSUFFICIENT_MONEY, _spBroadcast );
            }
            break;
        }
    }
    else
    {
        // Sell broadcast
        SellBroadcastCmdPtr spCmd( new SellBroadcastCmd( m_spContext, _spBroadcast ) );
        u32ReturnValue = m_spContext->GetInput()->Execute( spCmd );
    }
        
    return u32ReturnValue == 0;
}

bool DealerDropController::ExecuteAdvertDrop( AdvertPtr _spAdvert )
{
    Unsigned32 u32ReturnValue = 0;
    if ( m_bBag )
    {
        // Take advert
        MoveAdvertCmdPtr spCmd( new MoveAdvertCmd( m_spContext, _spAdvert, MoveAdvertCmd::LOCATION_DEALER, MoveAdvertCmd::LOCATION_BAG ) );
        u32ReturnValue = m_spContext->GetInput()->Execute( spCmd );
    }
    else
    {
        // Return advert
        MoveAdvertCmdPtr spCmd( new MoveAdvertCmd( m_spContext, _spAdvert, MoveAdvertCmd::LOCATION_BAG, MoveAdvertCmd::LOCATION_DEALER ) );
        u32ReturnValue = m_spContext->GetInput()->Execute( spCmd );
    }
        
    return u32ReturnValue == 0;
}



