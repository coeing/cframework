#include "cmadtv/Data/Player/PlayerCompStats.h"

#include "cfoundation/Base/Defines.h"
#include "cfoundation/Serialization/Serializer.h"

#include "cmadtv/Data/Player/DayStat.h"
#include "cmadtv/Data/Player/Player.h"

using namespace CMadTV;

PlayerCompStats::PlayerCompStats()
{
}

PlayerCompStats::~PlayerCompStats()
{
}
        
void PlayerCompStats::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    _Serializer.SerializeMap< CFoundation::SerializeWrapper_Simple, CFoundation::SerializeWrapper_SharedPtrSimple >( m_mapDayStats );
}

DayStatPtr PlayerCompStats::GetDayStat( const CFoundation::Date& _Date, bool _bCreateIfNecessary )
{
    DayStatPtr spDayStat;
    DateDayStatMap::const_iterator it = m_mapDayStats.find( _Date );
    if ( it != m_mapDayStats.end() )
    {
        spDayStat = it->second;
    }
    else if ( _bCreateIfNecessary )
    {
         spDayStat = DayStatPtr( new DayStat() );
         m_mapDayStats[ _Date ] = spDayStat;
    }
    return spDayStat;
}
