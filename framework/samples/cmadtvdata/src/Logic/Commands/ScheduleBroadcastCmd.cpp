#include "cmadtv/Logic/Commands/ScheduleBroadcastCmd.h"

#include "cmadtv/Data/Broadcast.h"

#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/World/BroadcastDealer.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerContext.h"
#include "cmadtv/Data/Player/Schedule.h"
#include "cmadtv/Data/Player/Scheduling.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// ScheduleBroadcastCmd
///
////////////////////////////////////////////////////////////////////////////////////////////
RTTI_DEF1_INST( ScheduleBroadcastCmd, "CMadTV::ScheduleBroadcastCmd", PlayerCommand );
        
ScheduleBroadcastCmd::ScheduleBroadcastCmd( PlayerContextPtr _spContext, BroadcastPtr _spBroadcast, const CFoundation::Date& _Date, Hour _eSlot )
:   PlayerCommand( _spContext ),
    m_Date( _Date ),
    m_eSlot( _eSlot )
{
    SetBroadcast( _spBroadcast );
}

void ScheduleBroadcastCmd::SetBroadcast( BroadcastPtr _spBroadcast )
{
    m_spBroadcast = _spBroadcast;
    m_u32BroadcastID = m_spBroadcast != NULL ? m_spBroadcast->GetID() : MAX_U32;
}

BroadcastPtr ScheduleBroadcastCmd::GetBroadcast() const
{
    return m_spBroadcast;
}
       
void ScheduleBroadcastCmd::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    // Serialize broadcast ID
    _Serializer.Serialize( m_u32BroadcastID );
    m_Date.Serialize( _Serializer );
    _Serializer.SerializeEnum( m_eSlot );
}
        
Unsigned32 ScheduleBroadcastCmd::Verify() const
{
    if ( m_spContext == NULL ||
         m_spBroadcast == NULL )
    {
        return ERROR_NONE;
    }

    // Get schedule
    SchedulePtr spSchedule = GetSchedule();
    if ( spSchedule == NULL )
    {
        return ERROR_INVALID_DATE;
    }

    Unsigned32 u32NumBlocks = m_spBroadcast != NULL ? m_spBroadcast->GetNumBlocks() : 0;
    if ( m_eSlot + u32NumBlocks - 1 >= HOUR_END )
    {
        return ERROR_BROADCAST_TOO_LONG;
    }

    return ERROR_NONE;
}

void ScheduleBroadcastCmd::Execute() const
{
    // Get schedule
    SchedulePtr spSchedule = GetSchedule();
    if ( spSchedule == NULL )
    {
        return;
    }

    // Set dropped broadcast to slot
    spSchedule->SetBroadcast( m_eSlot, m_spBroadcast );
}
        
void ScheduleBroadcastCmd::OnContextChanged()
{
    if ( m_spContext != NULL )
    {
        DatabasePtr spDatabase = m_spContext->GetDatabase();

        // Get broadcast by ID
        m_spBroadcast = spDatabase->GetBroadcast( m_u32BroadcastID );
    }
    else
    {
        m_spBroadcast = BroadcastPtr();
    }
}

SchedulePtr ScheduleBroadcastCmd::GetSchedule() const
{
    if ( m_spContext == NULL )
    {
        return SchedulePtr();
    }

    // Get schedule day
    const CFoundation::Date& today = m_spContext->GetGame()->GetDate();
    ScheduleDay eScheduleDay = SCHEDULE_DAY_END;
    if ( m_Date == today )
    {
        eScheduleDay = SCHEDULE_DAY_TODAY;
    }
    else if ( m_Date == today + 1 )
    {
        eScheduleDay = SCHEDULE_DAY_TOMORROW;
    }

    PlayerPtr spPlayer = m_spContext->GetPlayer();
    if ( spPlayer == NULL )
    {
        return SchedulePtr();
    }
    
    // Get schedule
    SchedulingPtr spScheduling = spPlayer->GetScheduling();
    return spScheduling->GetSchedule( eScheduleDay );
}

