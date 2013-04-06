#ifndef CMADTV_DAY_STAT_H
#define CMADTV_DAY_STAT_H

#include "cfoundation/Base/Types.h"
#include "cfoundation/Utils/SharedPtr.h"

#include "cmadtv/Data/World/Hour.h"

namespace CFoundation
{
    class Serializer;
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( BlockStat );

    /// Represents a statistic for one day of broadcasting
    /**
     *  \author Christian Oeing (oeing@gmx.at)
     *  \date 27.09.2009
     */
    class DayStat
    {
    public:
        /// Constructor
        DayStat();

        /// Destructor
        ~DayStat();

        /// Serialization
        virtual void            Serialize( CFoundation::Serializer& _Serializer );

        /// Returns the block statistic for the passed hour
        BlockStatPtr            GetBlockStat( Hour _eSlot );

        /// Returns the block statistic for the passed hour
        BlockStatCPtr           GetBlockStat( Hour _eSlot ) const;

        /// Returns the average number of spectators of the broadcast
        Unsigned32              GetAvgBroadcastSpectators() const;

    private:
        /// Block statistics
        BlockStatPtr            m_aspBlockStats[ HOUR_END ];
    };

    INCLUDE_SHARED_CLASS( DayStat );
}

#endif
