#ifndef CMADTV_SCHEDULE_NEWS_CMD_H
#define CMADTV_SCHEDULE_NEWS_CMD_H

#include "cfoundation/Base/Defines.h"

#include "cmadtv/Logic/Commands/PlayerCommand.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( NewsInstance );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// ScheduleNewsCmd
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class ScheduleNewsCmd : public PlayerCommand
    {
    public:
        /// Error codes
        enum Error
        {
            ERROR_NONE,
            ERROR_INVALID_SLOT,
            ERROR_NEWS_NOT_AVAILABLE,
            ERROR_INSUFFICIENT_MONEY,
        };

        /// Constructor
        ScheduleNewsCmd( PlayerContextPtr _spContext = PlayerContextPtr(), NewsInstancePtr _spNews = NewsInstancePtr(), Unsigned32 _u32Slot = MAX_U32 );

        /// Serialization
        /// Should serialize the non-context dependent data
        virtual void            Serialize( CFoundation::Serializer& _Serializer );

        /// Verifies if the command can be executed without changing any data
        /// Returns 0 if everything is ok, otherwise a error code is returned
        virtual Unsigned32      Verify() const;

        /// Executes the command
        virtual void            Execute() const;

    private:
        /// News instance
        NewsInstancePtr         m_spNews;

        /// Slot to schedule the news in
        Unsigned32              m_u32Slot;

        TYPE_DATA
    };

    INCLUDE_SHARED_CLASS( ScheduleNewsCmd );
}

#endif
