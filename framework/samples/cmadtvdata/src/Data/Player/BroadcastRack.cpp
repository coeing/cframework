#include "cmadtv/Data/Player/BroadcastRack.h"

#include <algorithm>

#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/Serialization/SerializationContext.h"

#include "cmadtv/Data/Broadcast.h"
#include "cmadtv/Data/Genre.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// BroadcastRack
///
////////////////////////////////////////////////////////////////////////////////////////////

BroadcastRack::BroadcastRack()
{
}
        
void BroadcastRack::operator=( const BroadcastRack& _rhs )
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
        
void BroadcastRack::Serialize( CFoundation::Serializer& _Serializer )
{
    _Serializer.SerializeVector< CFoundation::SerializeWrapper_Context >( m_aspBroadcasts );
}

void BroadcastRack::AddBroadcast( BroadcastPtr _spBroadcast )
{
    m_aspBroadcasts.push_back( _spBroadcast );

    // Dispatch event
    DispatchEvent( BROADCAST_RACK_EVENT_BROADCAST_ADDED, _spBroadcast );
}

void BroadcastRack::RemoveBroadcast( BroadcastPtr _spBroadcast )
{
    BroadcastVec::iterator it = std::find( m_aspBroadcasts.begin(), m_aspBroadcasts.end(), _spBroadcast );
    if ( it != m_aspBroadcasts.end() )
    {
        m_aspBroadcasts.erase( it );

        // Dispatch event
        DispatchEvent( BROADCAST_RACK_EVENT_BROADCAST_REMOVED, _spBroadcast );
    }
}

Unsigned32 BroadcastRack::GetNumBroadcasts() const
{
    return m_aspBroadcasts.size();
}
        
bool BroadcastRack::HasBroadcast( BroadcastPtr _spBroadcast ) const
{
    return std::find( m_aspBroadcasts.begin(), m_aspBroadcasts.end(), _spBroadcast ) != m_aspBroadcasts.end();
}

Unsigned32 BroadcastRack::GetNumBroadcasts( Unsigned32 _u32GenreID ) const
{
    BroadcastVec apBroadcasts;
    GetBroadcasts( _u32GenreID, apBroadcasts );
    return apBroadcasts.size();
}

Unsigned32 BroadcastRack::GetNumBroadcasts( GenrePtr _spGenre ) const
{
    return GetNumBroadcasts( _spGenre->GetID() );
}

BroadcastPtr BroadcastRack::GetBroadcast( Unsigned32 _u32Idx ) const
{
    if ( _u32Idx < GetNumBroadcasts() )
    {
        return m_aspBroadcasts[ _u32Idx ];
    }
    else
    {
        return BroadcastPtr();
    }
}

void BroadcastRack::GetBroadcasts( Unsigned32 _u32GenreID, BroadcastVec& _apBroadcasts ) const
{
    for ( BroadcastVec::const_iterator it = m_aspBroadcasts.begin(); it != m_aspBroadcasts.end(); ++it )
    {
        BroadcastPtr spBroadcast = *it;
        if ( spBroadcast->GetGenre()->GetID() == _u32GenreID )
        {
            _apBroadcasts.push_back( spBroadcast );
        }
    }
}
