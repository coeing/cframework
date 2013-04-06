#include "cmadtv/Data/Player/AdvertContract.h"

#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/Serialization/SerializationContext.h"

#include "cmadtv/Data/Advert.h"

using namespace CMadTV;

AdvertContract::AdvertContract( AdvertPtr _spAdvert, const CFoundation::Date& _clStartDate )
:   m_spAdvert( _spAdvert ),
    m_clStartDate( _clStartDate ),
    m_u32NumSpotsSent( 0 )
{
}

AdvertContract::~AdvertContract()
{
}
        
void AdvertContract::Serialize( CFoundation::Serializer& _Serializer )
{
    _Serializer.SerializeProxy< CFoundation::SerializeWrapper_Context >( m_spAdvert );
    m_clStartDate.Serialize( _Serializer );
    _Serializer.Serialize( m_u32NumSpotsSent );
}

AdvertPtr AdvertContract::GetAdvert() const
{
    return m_spAdvert;
}
        
Unsigned32 AdvertContract::GetNumSpotsSent() const
{
    return m_u32NumSpotsSent;
}

Unsigned32 AdvertContract::GetNumSpotsLeft() const
{
    Unsigned32 u32NumSpots = m_spAdvert->GetNumSpots();
    if ( m_u32NumSpotsSent >= u32NumSpots )
    {
        return 0;
    }
    else
    {
        return u32NumSpots - m_u32NumSpotsSent;
    }
}

Unsigned32 AdvertContract::GetNumDaysLeft( const CFoundation::Date& _clToday ) const
{
    CFoundation::Date clEndDate = m_clStartDate + m_spAdvert->GetNumDays();
    if ( _clToday > clEndDate )
    {
        return 0;
    }
    else
    {
        return static_cast< Unsigned32 >( clEndDate - _clToday );
    }
}
        
Unsigned32 AdvertContract::RemoveSpot()
{
    Unsigned32 u32NumSpots = m_spAdvert->GetNumSpots();
    if ( m_u32NumSpotsSent < u32NumSpots )
    {
        ++m_u32NumSpotsSent;
    }
    return u32NumSpots - m_u32NumSpotsSent;
}

