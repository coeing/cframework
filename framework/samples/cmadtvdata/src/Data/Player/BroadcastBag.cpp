#include "cmadtv/Data/Player/BroadcastBag.h"

#include <algorithm>

#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/Serialization/SerializationContext.h"

#include "cmadtv/Data/Broadcast.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// BroadcastBag
///
////////////////////////////////////////////////////////////////////////////////////////////
        
void BroadcastBag::operator=( const BroadcastBag& _rhs )
{
    // Remove non-existing broadcasts
    BroadcastVec aspBroadcasts = m_aspBroadcasts;
    Unsigned32 u32NumBroadcasts = aspBroadcasts.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumBroadcasts; ++u32Idx )
    {
        BroadcastPtr spBroadcast = aspBroadcasts[ u32Idx ];
        if ( std::find( _rhs.m_aspBroadcasts.begin(), _rhs.m_aspBroadcasts.end(), spBroadcast ) == _rhs.m_aspBroadcasts.end() )
        {
            RemoveBroadcast( spBroadcast );
        }
    }

    // Add new broadcasts
    u32NumBroadcasts = _rhs.m_aspBroadcasts.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumBroadcasts; ++u32Idx )
    {
        BroadcastPtr spBroadcast = _rhs.m_aspBroadcasts[ u32Idx ];
        if ( !HasBroadcast( spBroadcast ) )
        {
            AddBroadcast( spBroadcast );
        }
    }
}
        
void BroadcastBag::Serialize( CFoundation::Serializer& _Serializer )
{
    _Serializer.SerializeVector< CFoundation::SerializeWrapper_Context >( m_aspBroadcasts );
}

bool BroadcastBag::AddBroadcast( BroadcastPtr _spBroadcast )
{
    if ( GetNumBroadcasts() >= MAX_NUM_BROADCASTS )
    {
        return false;
    }

    m_aspBroadcasts.push_back( _spBroadcast );

    // Dispatch event
    DispatchEvent( BROADCAST_BAG_EVENT_BROADCAST_ADDED, _spBroadcast );

    return true;
}

void BroadcastBag::RemoveBroadcast( BroadcastPtr _spBroadcast )
{
    BroadcastVec::iterator it = std::find( m_aspBroadcasts.begin(), m_aspBroadcasts.end(), _spBroadcast );
    if ( it != m_aspBroadcasts.end() )
    {
        m_aspBroadcasts.erase( it );

        // Dispatch event
        DispatchEvent( BROADCAST_BAG_EVENT_BROADCAST_REMOVED, _spBroadcast );
    }
}
        
Unsigned32 BroadcastBag::GetNumBroadcasts() const
{
    return m_aspBroadcasts.size();
}
        
BroadcastPtr BroadcastBag::GetBroadcast( Unsigned32 _u32Idx ) const
{
    if ( _u32Idx >= m_aspBroadcasts.size() )
    {
        return BroadcastPtr();
    }
    return m_aspBroadcasts[ _u32Idx ];
}

const std::vector< BroadcastPtr >& BroadcastBag::GetBroadcasts() const
{
    return m_aspBroadcasts;
}
        
bool BroadcastBag::HasBroadcast( BroadcastPtr _spBroadcast ) const
{
    return std::find( m_aspBroadcasts.begin(), m_aspBroadcasts.end(), _spBroadcast ) != m_aspBroadcasts.end();
}
