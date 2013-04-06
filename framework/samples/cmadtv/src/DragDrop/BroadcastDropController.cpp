#include "cmadtv/DragDrop/BroadcastDropController.h"

#include "cfoundation/DateTime/Date.h"

#include "cmadtv/Data/Broadcast.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Player/PlayerContext.h"
#include "cmadtv/Data/Player/Scheduling.h"

#include "cmadtv/DragDrop/BroadcastDragController.h"

#include "cmadtv/Logic/PlayerInput.h"
#include "cmadtv/Logic/Commands/ScheduleBroadcastCmd.h"

using namespace CMadTV;

BroadcastDropController::BroadcastDropController( PlayerContextPtr _spContext, const CFoundation::Date& _Date, Hour _eSlot )
:   m_spContext( _spContext ),
    m_Date( _Date ),
    m_eSlot( _eSlot )
{
}

BroadcastDropController::~BroadcastDropController()
{
}

bool BroadcastDropController::Execute( CGui::GuiObjectPtr _spDragObject, CGui::DragDataPtr _spDragData, const CFoundation::Vector2Di& /*_vDragPosition*/, const CFoundation::Vector2Di& /*_vDropPosition*/ ) 
{
    // Broadcast drag data expected
    BroadcastDragDataPtr spDragData = dynamic_pointer_cast< BroadcastDragData >( _spDragData );
    if ( spDragData == NULL )
    {
        return false;
    }

    if ( m_spContext == NULL )
    {
        return false;
    }

    BroadcastPtr spBroadcast = spDragData->m_spBroadcast;

    // Try to schedule broadcast at this slot
    ScheduleBroadcastCmdPtr spCmd( new ScheduleBroadcastCmd( m_spContext, spBroadcast, m_Date, m_eSlot ) );
    Unsigned32 u32ReturnValue = m_spContext->GetInput()->Execute( spCmd );
    switch ( u32ReturnValue )
    {
    case ScheduleBroadcastCmd::ERROR_INVALID_DATE:
        {
        }
        break;
    case ScheduleBroadcastCmd::ERROR_BROADCAST_TOO_LONG:
        {
        }
        break;
    }

    return u32ReturnValue == 0;
}
       
void BroadcastDropController::SetContext( PlayerContextPtr _spContext )
{
    m_spContext = _spContext;
}


