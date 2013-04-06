#include "cmadtv/Data/Player/AdvertBag.h"

#include <algorithm>

#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/Serialization/SerializationContext.h"

#include "cmadtv/Data/Advert.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// AdvertBag
///
////////////////////////////////////////////////////////////////////////////////////////////

void AdvertBag::operator=( const AdvertBag& _rhs )
{
    // Remove non-existing adverts
    AdvertVec aspAdverts = m_aspAdverts;
    Unsigned32 u32NumAdverts = aspAdverts.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumAdverts; ++u32Idx )
    {
        AdvertPtr spAdvert = aspAdverts[ u32Idx ];
        if ( std::find( _rhs.m_aspAdverts.begin(), _rhs.m_aspAdverts.end(), spAdvert ) == _rhs.m_aspAdverts.end() )
        {
            RemoveAdvert( spAdvert );
        }
    }

    // Add new adverts
    u32NumAdverts = _rhs.m_aspAdverts.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumAdverts; ++u32Idx )
    {
        AdvertPtr spAdvert = _rhs.m_aspAdverts[ u32Idx ];
        if ( !HasAdvert( spAdvert ) )
        {
            AddAdvert( spAdvert );
        }
    }
}
        
void AdvertBag::Serialize( CFoundation::Serializer& _Serializer )
{
    _Serializer.SerializeVector< CFoundation::SerializeWrapper_Context >( m_aspAdverts );
}

bool AdvertBag::AddAdvert( AdvertPtr _spAdvert )
{
    if ( GetNumAdverts() >= MAX_NUM_ADVERTS )
    {
        return false;
    }

    // Check if already in bag
    AdvertVec::iterator it = std::find( m_aspAdverts.begin(), m_aspAdverts.end(), _spAdvert );
    if ( it != m_aspAdverts.end() )
    {
        return false;
    }

    m_aspAdverts.push_back( _spAdvert );

    // Dispatch event
    DispatchEvent( ADVERT_BAG_EVENT_ADVERT_ADDED, _spAdvert );

    return true;
}

void AdvertBag::RemoveAdvert( AdvertPtr _spAdvert )
{
    AdvertVec::iterator it = std::find( m_aspAdverts.begin(), m_aspAdverts.end(), _spAdvert );
    if ( it != m_aspAdverts.end() )
    {
        m_aspAdverts.erase( it );
        DispatchEvent( ADVERT_BAG_EVENT_ADVERT_REMOVED, _spAdvert );
    }
}
        
bool AdvertBag::HasAdvert( AdvertPtr _spAdvert ) const
{
    return std::find( m_aspAdverts.begin(), m_aspAdverts.end(), _spAdvert ) != m_aspAdverts.end();
}
        
Unsigned32 AdvertBag::GetNumAdverts() const
{
    return m_aspAdverts.size();
}
        
AdvertPtr AdvertBag::GetAdvert( Unsigned32 _u32Idx ) const
{
    if ( _u32Idx >= m_aspAdverts.size() )
    {
        return AdvertPtr();
    }
    return m_aspAdverts[ _u32Idx ];
}

const std::vector< AdvertPtr >& AdvertBag::GetAdverts() const
{
    return m_aspAdverts;
}
