#ifndef CMADTV_PLAYER_COMP_STATS_H
#define CMADTV_PLAYER_COMP_STATS_H

#include "cfoundation/Base/Types.h"
#include "cfoundation/DateTime/Date.h"
#include "cfoundation/Events/DelegateInvoker.h"
#include "cfoundation/Utils/SharedPtr.h"

#include "cmadtv/Data/World/Hour.h"

namespace CFoundation
{
    class Serializer;
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( DayStat );
    INCLUDE_SHARED_CLASS( PlayerCompStats );
    INCLUDE_SHARED_CLASS( Player );

    /// PlayerCompStats of one broadcast station for one day
    class PlayerCompStats
    {
    public:
        /// Constructor
        PlayerCompStats();

        /// Destructor
        ~PlayerCompStats();

        /// Serialization
        virtual void        Serialize( CFoundation::Serializer& _Serializer );

        /// Returns the day stat for the passed date
        /// _bCreateIfNecessary indicates if it should be created if necessary
        DayStatPtr          GetDayStat( const CFoundation::Date& _Date, bool _bCreateIfNecessary = true );
        
    private:
        typedef std::map< CFoundation::Date, DayStatPtr > DateDayStatMap;

        /// Day stats
        DateDayStatMap      m_mapDayStats;
    };
}

#endif
