#include "cmadtv/Data/Player/Schedule.h"

#include "cofiles/Enum.h"
#include "cofiles/Makros.h"

#include "cmadtv/Data/Broadcast.h"

using namespace CMadTV;

ScheduleSlot& CMadTV::operator++( ScheduleSlot& _eSlot )
{
    return enum_increment< ScheduleSlot >( _eSlot, SCHEDULE_SLOT_NONE, SCHEDULE_SLOT_END );
}

ScheduleSlot& CMadTV::operator--( ScheduleSlot& _eSlot )
{
    return enum_decrement< ScheduleSlot >( _eSlot, SCHEDULE_SLOT_NONE, SCHEDULE_SLOT_END );
}  

Schedule::Schedule()
{
    for ( Unsigned32 u32Slot = 0; u32Slot < SCHEDULE_SLOT_END; ++u32Slot )
    {
        m_abBroadcastStart[ u32Slot ] = false;
    }
}

Schedule::~Schedule()
{
}

void Schedule::SetBroadcast( BroadcastPtr _spBroadcast, ScheduleSlot _eStartSlot )
{
    // Check that broadcast doesn't need more blocks than available
    Unsigned32 u32NumBlocks = _spBroadcast->GetNumBlocks();
    if ( _eStartSlot + u32NumBlocks - 1 >= SCHEDULE_SLOT_END )
    {
        CO_WARN( "Schedule::SetBroadcast: Broadcast has too many blocks to schedule in start slot %d", _eStartSlot );
        return;
    }

    for ( Unsigned32 u32Block = 0; u32Block < u32NumBlocks; ++u32Block )
    {
        ScheduleSlot eSlot = static_cast< ScheduleSlot >( _eStartSlot + u32Block );

        // Free slot
        RemoveBroadcast( eSlot );

        // Plan slot
        m_aspBroadcasts[ eSlot ] = _spBroadcast;
    }

    // Set start flag for start slot
    m_abBroadcastStart[ _eStartSlot ] = true;

    // Dispatch event
    DispatchEvent( SCHEDULE_EVENT_BROADCAST_ADDED, _spBroadcast, _eStartSlot );
}

void Schedule::RemoveBroadcast( ScheduleSlot _eSlot )
{
    BroadcastPtr spBroadcast = m_aspBroadcasts[ _eSlot ];
    if ( spBroadcast == NULL )
    {
        return;
    }

    // Remove all pieces of this broadcast
    ScheduleSlot eStartSlot = _eSlot;
    for ( ScheduleSlot eSlot = _eSlot; eSlot > SCHEDULE_SLOT_NONE; --eSlot )
    {
        bool bBroadcastStart = m_abBroadcastStart[ eSlot ];

        // Reset slot
        m_aspBroadcasts[ eSlot ].reset();
        m_abBroadcastStart[ eSlot ] = false;

        // Check if was start block
        if ( bBroadcastStart )
        {
            eStartSlot = eSlot;
            break;
        }
    }

    ScheduleSlot eSlot = _eSlot;
    ++eSlot;
    for ( ; eSlot < SCHEDULE_SLOT_END; ++eSlot )
    {
        // Check if start block of next broadcast
        if ( m_abBroadcastStart[ eSlot ] )
        {
            break;
        }

        // Reset slot
        m_aspBroadcasts[ eSlot ].reset();
        m_abBroadcastStart[ eSlot ] = false;
    }

    // Dispatch event
    DispatchEvent( SCHEDULE_EVENT_BROADCAST_REMOVED, spBroadcast, eStartSlot );
}

BroadcastPtr Schedule::GetBroadcast( ScheduleSlot _eSlot ) const
{
    if ( _eSlot < SCHEDULE_SLOT_END )
    {
        return m_aspBroadcasts[ _eSlot ];
    }
    else
    {
        CO_WARN( "Schedule::GetBroadcast: Invalid schedule slot" );
        return BroadcastPtr();
    }
}
        
bool Schedule::DoesBroadcastStart( ScheduleSlot _eSlot ) const
{
    if ( _eSlot < SCHEDULE_SLOT_END )
    {
        return m_abBroadcastStart[ _eSlot ];
    }
    else
    {
        CO_WARN( "Schedule::DoesBroadcastStart: Invalid schedule slot" );
        return false;
    }
}
