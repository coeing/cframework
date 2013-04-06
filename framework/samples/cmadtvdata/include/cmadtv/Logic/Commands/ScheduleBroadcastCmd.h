#ifndef CMADTV_SCHEDULE_BROADCAST_CMD_H
#define CMADTV_SCHEDULE_BROADCAST_CMD_H

#include "cfoundation/DateTime/Date.h"

#include "cmadtv/Data/World/Hour.h"
#include "cmadtv/Logic/Commands/PlayerCommand.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Broadcast );
    INCLUDE_SHARED_CLASS( Schedule );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// ScheduleBroadcastCmd
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class ScheduleBroadcastCmd : public PlayerCommand
    {
    public:
        /// Error codes
        enum Error
        {
            ERROR_NONE,
            ERROR_INVALID_DATE,
            ERROR_BROADCAST_TOO_LONG,
        };

        /// Constructor
        ScheduleBroadcastCmd( PlayerContextPtr _spContext = PlayerContextPtr(), BroadcastPtr _spBroadcast = BroadcastPtr(), const CFoundation::Date& _Date = CFoundation::Date(), Hour _eSlot = HOUR_NONE );

        /// Sets the broadcast
        void                    SetBroadcast( BroadcastPtr _spBroadcast );

        /// Returns the broadcast
        BroadcastPtr            GetBroadcast() const;

        /// Serialization
        /// Should serialize the non-context dependent data
        virtual void            Serialize( CFoundation::Serializer& _Serializer );

        /// Verifies if the command can be executed without changing any data
        /// Returns 0 if everything is ok, otherwise a error code is returned
        virtual Unsigned32      Verify() const;

        /// Executes the command
        virtual void            Execute() const;

    protected:
        /// Called by base class when context changed
        virtual void            OnContextChanged();

    private:
        /// Returns the schedule that should be used
        SchedulePtr             GetSchedule() const;

        /// Broadcast ID    
        Unsigned32              m_u32BroadcastID;

        /// Broadcast
        BroadcastPtr            m_spBroadcast;

        /// Date
        CFoundation::Date       m_Date;

        /// Start slot
        Hour                    m_eSlot;

        TYPE_DATA
    };

    INCLUDE_SHARED_CLASS( ScheduleBroadcastCmd );
}

#endif
