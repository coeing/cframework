#ifndef CMADTV_SCHEDULE_ADVERT_CMD_H
#define CMADTV_SCHEDULE_ADVERT_CMD_H

#include "cfoundation/DateTime/Date.h"

#include "cmadtv/Data/World/Hour.h"
#include "cmadtv/Logic/Commands/PlayerCommand.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Advert );
    INCLUDE_SHARED_CLASS( Schedule );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// ScheduleAdvertCmd
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class ScheduleAdvertCmd : public PlayerCommand
    {
    public:
        /// Error codes
        enum Error
        {
            ERROR_NONE,
            ERROR_INVALID_DATE,
        };

        /// Constructor
        ScheduleAdvertCmd( PlayerContextPtr _spContext = PlayerContextPtr(), AdvertPtr _spAdvert = AdvertPtr(), const CFoundation::Date& _Date = CFoundation::Date(), Hour _eSlot = HOUR_NONE );

        /// Sets the advert
        void                    SetAdvert( AdvertPtr _spAdvert );

        /// Returns the advert
        AdvertPtr            GetAdvert() const;

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

        /// Advert ID    
        Unsigned32              m_u32AdvertID;

        /// Advert
        AdvertPtr            m_spAdvert;

        /// Date
        CFoundation::Date       m_Date;

        /// Start slot
        Hour                    m_eSlot;

        TYPE_DATA
    };

    INCLUDE_SHARED_CLASS( ScheduleAdvertCmd );
}

#endif
