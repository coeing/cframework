#include "cmadtv/Data/Player/Schedule.h"

#include "cfoundation/Base/Defines.h"
#include "cfoundation/Debug/Makros.h"
#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/Serialization/SerializationContext.h"

#include "cmadtv/Data/Advert.h"
#include "cmadtv/Data/Broadcast.h"
#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/World/Hour.h"

using namespace CMadTV;

Schedule::Schedule()
{
    for ( Unsigned32 u32Slot = 0; u32Slot < HOUR_END; ++u32Slot )
    {
        m_abBroadcastStart[ u32Slot ] = false;
    }
}

Schedule::~Schedule()
{
}
        
void Schedule::operator=( const Schedule& _rhs )
{
    for ( Hour eHour = HOUR_BEGIN; eHour != HOUR_END; ++eHour )
    {
        BroadcastPtr spBroadcast = _rhs.m_aspBroadcasts[ eHour ];
        if ( spBroadcast != m_aspBroadcasts[ eHour ] &&
             ( spBroadcast == NULL ||
               _rhs.m_abBroadcastStart[ eHour ] ) )
        {
            SetBroadcast( eHour, spBroadcast );
        }
        if ( _rhs.m_aspAdverts[ eHour ] != m_aspAdverts[ eHour ] )
        {
            SetAdvert( eHour, _rhs.m_aspAdverts[ eHour ] );
        }
    }
}
        
void Schedule::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    _Serializer.SerializeArray< CFoundation::SerializeWrapper_Context >( m_aspBroadcasts, HOUR_END );
    _Serializer.SerializeArray< CFoundation::SerializeWrapper_Value >( m_abBroadcastStart, HOUR_END );
    _Serializer.SerializeArray< CFoundation::SerializeWrapper_Context >( m_aspAdverts, HOUR_END );
}
       
void Schedule::Clear()
{
    for ( Hour eHour = HOUR_BEGIN; eHour < HOUR_END; ++eHour )
    {
        RemoveBroadcast( eHour );
        RemoveAdvert( eHour );
    }
}

void Schedule::SetBroadcast( Hour _eStartSlot, BroadcastPtr _spBroadcast )
{
    if ( _spBroadcast == NULL )
    {
        RemoveBroadcast( _eStartSlot );
        return;
    }

    // Check that broadcast doesn't need more blocks than available
    Unsigned32 u32NumBlocks = _spBroadcast->GetNumBlocks();
    if ( _eStartSlot + u32NumBlocks - 1 >= HOUR_END )
    {
        CF_WARN( "Schedule::SetBroadcast: Broadcast has too many blocks to schedule in start slot %d", _eStartSlot );
        return;
    }

    for ( Unsigned32 u32Block = 0; u32Block < u32NumBlocks; ++u32Block )
    {
        Hour eSlot = static_cast< Hour >( _eStartSlot + u32Block );

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

void Schedule::RemoveBroadcast( Hour _eSlot )
{
    BroadcastPtr spBroadcast = m_aspBroadcasts[ _eSlot ];
    if ( spBroadcast == NULL )
    {
        return;
    }

    // Remove all pieces of this broadcast
    Hour eStartSlot = _eSlot;
    for ( Hour eSlot = _eSlot; eSlot > HOUR_NONE; --eSlot )
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

    Hour eSlot = _eSlot;
    ++eSlot;
    for ( ; eSlot < HOUR_END; ++eSlot )
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

BroadcastPtr Schedule::GetBroadcast( Hour _eSlot ) const
{
    if ( _eSlot < HOUR_END )
    {
        return m_aspBroadcasts[ _eSlot ];
    }
    else
    {
        CF_WARN( "Schedule::GetBroadcast: Invalid schedule slot" );
        return BroadcastPtr();
    }
}
       
Unsigned32 Schedule::GetBroadcastBlockNumber( Hour _eHour ) const
{
    BroadcastPtr spBroadcast = m_aspBroadcasts[ _eHour ];
    if ( spBroadcast == NULL )
    {
        return MAX_U32;
    }
    Unsigned32 u32NumBlocks = spBroadcast->GetNumBlocks();
    Unsigned32 u32BlockNumber = 0;
    Hour eHour = _eHour;
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumBlocks - 1; ++u32Idx )
    {
        if ( m_abBroadcastStart[ eHour ] ||
             eHour == HOUR_BEGIN )
        {
            break;
        }
        --eHour;
        ++u32BlockNumber;
    }
    return u32BlockNumber;
}
        
bool Schedule::DoesBroadcastStart( Hour _eSlot ) const
{
    if ( _eSlot < HOUR_END )
    {
        return m_abBroadcastStart[ _eSlot ];
    }
    else
    {
        CF_WARN( "Schedule::DoesBroadcastStart: Invalid schedule slot" );
        return false;
    }
}
        
bool Schedule::DoesBroadcastEnd( Hour _eHour ) const
{
    BroadcastPtr spBroadcast = m_aspBroadcasts[ _eHour ];
    if ( spBroadcast == NULL )
    {
        return false;
    }
    Unsigned32 u32NumBlocks = spBroadcast->GetNumBlocks();
    return GetBroadcastBlockNumber( _eHour ) == u32NumBlocks - 1;
}
       
Unsigned32 Schedule::GetNumBroadcasts( BroadcastPtr _spBroadcast, Hour _eHourBegin, Hour _eHourEnd ) const
{
    Unsigned32 u32NumBroadcasts = 0;
    for ( Hour eHour = _eHourBegin; eHour < _eHourEnd; ++eHour )
    {
        if ( m_abBroadcastStart[ eHour ] &&
             m_aspBroadcasts[ eHour ] == _spBroadcast )
        {
            ++u32NumBroadcasts;
        }
    }
    return u32NumBroadcasts;
}
        
bool Schedule::IsBroadcastPlanned( BroadcastPtr _spBroadcast, Hour _eHourBegin, Hour _eHourEnd ) const
{
    for ( Hour eHour = _eHourBegin; eHour < _eHourEnd; ++eHour )
    {
        if ( m_aspBroadcasts[ eHour ] == _spBroadcast )
        {
            return true;
        }
    }
    return false;
}

void Schedule::SetAdvert( Hour _eSlot, AdvertPtr _spAdvert )
{
    if ( _spAdvert == m_aspAdverts[ _eSlot ] )
    {
        return;
    }
    else if ( _spAdvert == NULL )
    {
        RemoveAdvert( _eSlot );
        return;
    }

    // Free slot
    RemoveAdvert( _eSlot );

    // Plan slot
    m_aspAdverts[ _eSlot ] = _spAdvert;

    // Dispatch event
    DispatchEvent( SCHEDULE_EVENT_ADVERT_ADDED, _spAdvert, _eSlot );
}

void Schedule::RemoveAdvert( Hour _eSlot )
{
    AdvertPtr spAdvert = m_aspAdverts[ _eSlot ];
    if ( spAdvert == NULL )
    {
        return;
    }

    // Reset slot
    m_aspAdverts[ _eSlot ].reset();

    // Dispatch event
    DispatchEvent( SCHEDULE_EVENT_ADVERT_REMOVED, spAdvert, _eSlot );
}
        
void Schedule::RemoveAdverts( AdvertPtr _spAdvert, Hour _eHourBegin, Hour _eHourEnd )
{
    for ( Hour eHour = _eHourBegin; eHour < _eHourEnd; ++eHour )
    {
        if ( m_aspAdverts[ eHour ] == _spAdvert )
        {
            RemoveAdvert( eHour );
        }
    }
}

AdvertPtr Schedule::GetAdvert( Hour _eSlot ) const
{
    if ( _eSlot < HOUR_END )
    {
        return m_aspAdverts[ _eSlot ];
    }
    else
    {
        return AdvertPtr();
    }
}

Unsigned32 Schedule::GetNumAdverts( AdvertPtr _spAdvert, Hour _eHourBegin, Hour _eHourEnd ) const
{
    Unsigned32 u32NumAdverts = 0;
    for ( Hour eHour = _eHourBegin; eHour < _eHourEnd; ++eHour )
    {
        if ( m_aspAdverts[ eHour ] == _spAdvert )
        {
            ++u32NumAdverts;
        }
    }
    return u32NumAdverts;
}

