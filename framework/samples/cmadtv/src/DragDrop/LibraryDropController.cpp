#include "cmadtv/DragDrop/LibraryDropController.h"

#include "cmadtv/Data/Broadcast.h"
#include "cmadtv/Data/Genre.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Player/BroadcastBag.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerContext.h"
#include "cmadtv/Data/Player/Scheduling.h"
#include "cmadtv/Data/World/AdvertDealer.h"
#include "cmadtv/Data/World/BroadcastDealer.h"

#include "cmadtv/Logic/PlayerInput.h"
#include "cmadtv/Logic/Commands/MoveBroadcastCmd.h"

#include "cmadtv/DragDrop/AdvertDragController.h"
#include "cmadtv/DragDrop/DealerBroadcastDragController.h"

using namespace CMadTV;

LibraryDropController::LibraryDropController( PlayerContextPtr _spContext, bool _bBag )
:   m_spContext( _spContext ),
    m_bBag( _bBag )
{
}

LibraryDropController::~LibraryDropController()
{
}

bool LibraryDropController::Execute( CGui::GuiObjectPtr _spDragObject, CGui::DragDataPtr _spDragData, const CFoundation::Vector2Di& /*_vDragPosition*/, const CFoundation::Vector2Di& /*_vDropPosition*/ ) 
{
    // Broadcast or advert drag data expected
    DealerBroadcastDragDataPtr spBroadcastDragData = dynamic_pointer_cast< DealerBroadcastDragData >( _spDragData );
    if ( spBroadcastDragData != NULL )
    {
        return ExecuteBroadcastDrop( spBroadcastDragData->m_spBroadcast );
    }
    else
    {
        return false;
    }
}
        
void LibraryDropController::SetContext( PlayerContextPtr _spContext )
{
    m_spContext = _spContext;
}

bool LibraryDropController::ExecuteBroadcastDrop( BroadcastPtr _spBroadcast )
{
    if ( m_spContext == NULL )
    {
        return false;
    }

    Unsigned32 u32ReturnValue = 0;
    if ( m_bBag )
    {
        MoveBroadcastCmdPtr spCmd( new MoveBroadcastCmd( m_spContext, _spBroadcast, MoveBroadcastCmd::LOCATION_RACK, MoveBroadcastCmd::LOCATION_BAG ) );
        u32ReturnValue = m_spContext->GetInput()->Execute( spCmd );
        switch ( u32ReturnValue )
        {
        case MoveBroadcastCmd::ERROR_BAG_FULL:
            {
                DispatchEvent( LIBRARY_DROP_EVENT_BAG_FULL, _spBroadcast );
            }
            break;
        case MoveBroadcastCmd::ERROR_IS_PLANNED:
            {
                //DispatchEvent( LIBRARY_DROP_EVENT_BROADCAST_PLANNED, _spBroadcast );
            }
            break;
        }
    }
    else
    {
        MoveBroadcastCmdPtr spCmd( new MoveBroadcastCmd( m_spContext, _spBroadcast, MoveBroadcastCmd::LOCATION_BAG, MoveBroadcastCmd::LOCATION_RACK ) );
        u32ReturnValue = m_spContext->GetInput()->Execute( spCmd );
        switch ( u32ReturnValue )
        {
        case MoveBroadcastCmd::ERROR_RACK_SHELF_FULL:
            {
                DispatchEvent( LIBRARY_DROP_EVENT_BAG_FULL, _spBroadcast );
            }
            break;
        }
    }
        
    return u32ReturnValue == 0;
}
