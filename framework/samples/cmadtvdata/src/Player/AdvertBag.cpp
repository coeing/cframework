#include "cmadtv/Data/Player/AdvertBag.h"

#include <algorithm>

using namespace CMadTV;

bool AdvertBag::AddAdvert( AdvertPtr _spAdvert )
{
    if ( GetNumAdverts() >= MAX_NUM_ADVERTS )
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
