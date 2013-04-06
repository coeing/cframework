#include "cmadtv/Data/World/BroadcastDealer.h"

#include <algorithm>

#include "cfoundation/Base/Defines.h"
#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/Serialization/SerializationContext.h"

#include "cmadtv/Data/Broadcast.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// BroadcastDealer
///
////////////////////////////////////////////////////////////////////////////////////////////
      
void BroadcastDealer::operator=( const BroadcastDealer& _rhs )
{
    for ( Unsigned32 u32Idx = 0; u32Idx < NUM_SLOTS; ++u32Idx )
    {
        SetBroadcast( u32Idx, _rhs.m_aspBroadcasts[ u32Idx ] );
    }
}

void BroadcastDealer::Serialize( CFoundation::Serializer& _Serializer )
{
    _Serializer.SerializeArray< CFoundation::SerializeWrapper_Context >( m_aspBroadcasts, NUM_SLOTS );
}

void BroadcastDealer::SetBroadcast( Unsigned32 _u32Slot, BroadcastPtr _spBroadcast )
{
    if ( _spBroadcast == NULL )
    {
        RemoveBroadcast( _u32Slot );
        return;
    }

    if ( _u32Slot >= NUM_SLOTS )
    {
        return;
    }

    if ( _spBroadcast == m_aspBroadcasts[ _u32Slot ] )
    {
        return;
    }

    // Remove old broadcast
    if ( m_aspBroadcasts[ _u32Slot ] != NULL )
    {
        RemoveBroadcast( _u32Slot );
    }

    m_aspBroadcasts[ _u32Slot ] = _spBroadcast;

    // Dispatch event
    DispatchEvent( BROADCAST_DEALER_EVENT_BROADCAST_ADDED, _spBroadcast, _u32Slot );
}
       
bool BroadcastDealer::AddBroadcast( BroadcastPtr _spBroadcast )
{
    if ( _spBroadcast == NULL )
    {
        return false;
    }

    for ( Unsigned32 u32Idx = 0; u32Idx < NUM_SLOTS; ++u32Idx )
    {
        if ( m_aspBroadcasts[ u32Idx ] == NULL )
        {
            SetBroadcast( u32Idx, _spBroadcast );
            return true;
        }
    }
    return false;
}
       
void BroadcastDealer::RemoveBroadcast( Unsigned32 _u32Slot )
{
    if ( _u32Slot >= NUM_SLOTS )
    {
        return;
    }

    BroadcastPtr spBroadcast = m_aspBroadcasts[ _u32Slot ];
    if ( spBroadcast == NULL )
    {
        return;
    }

    // Remove broadcast
    m_aspBroadcasts[ _u32Slot ].reset();

    // Dispatch event
    DispatchEvent( BROADCAST_DEALER_EVENT_BROADCAST_REMOVED, spBroadcast, _u32Slot );
}
        
void BroadcastDealer::RemoveBroadcast( BroadcastPtr _spBroadcast )
{
    for ( Unsigned32 u32Idx = 0; u32Idx < NUM_SLOTS; ++u32Idx )
    {
        if ( m_aspBroadcasts[ u32Idx ] == _spBroadcast )
        {
            RemoveBroadcast( u32Idx );
            break;
        }
    }
}
        
void BroadcastDealer::RemoveAllBroadcasts()
{
    for ( Unsigned32 u32Idx = 0; u32Idx < NUM_SLOTS; ++u32Idx )
    {
        RemoveBroadcast( u32Idx );
    }
}
       
Unsigned32 BroadcastDealer::GetNumBroadcasts() const
{
    Unsigned32 u32NumBroadcasts = 0;
    for ( Unsigned32 u32Idx = 0; u32Idx < NUM_SLOTS; ++u32Idx )
    {
        if ( m_aspBroadcasts[ u32Idx ] != NULL )
        {
            ++u32NumBroadcasts;
        }
    }
    return u32NumBroadcasts;
}

BroadcastPtr BroadcastDealer::GetBroadcast( Unsigned32 _u32Slot ) const
{
    if ( _u32Slot >= NUM_SLOTS )
    {
        return BroadcastPtr();
    }

    return m_aspBroadcasts[ _u32Slot ];
}

bool BroadcastDealer::HasBroadcast( BroadcastPtr _spBroadcast ) const
{
    return std::find( m_aspBroadcasts, m_aspBroadcasts + NUM_SLOTS, _spBroadcast ) != ( m_aspBroadcasts + NUM_SLOTS );
}
        
Unsigned32 BroadcastDealer::GetNumFreeSlots() const
{
    return NUM_SLOTS - GetNumBroadcasts();
}
