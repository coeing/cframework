#ifndef CMADTV_SCHEDULING_H
#define CMADTV_SCHEDULING_H

#include "cfoundation/Base/Types.h"
#include "cfoundation/Events/DelegateInvoker.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CFoundation
{
    class Date;
    class Serializer;
}

namespace CMadTV
{    
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// ScheduleDay
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    /// Available schedule days

    enum ScheduleDay
    {
        SCHEDULE_DAY_TODAY,
        SCHEDULE_DAY_TOMORROW,
        SCHEDULE_DAY_END
    };

    ScheduleDay& operator++( ScheduleDay& _eScheduleDay ); 
    ScheduleDay& operator--( ScheduleDay& _eScheduleDay );  

    /// Computes the schedule day out of the passed date
    ScheduleDay         ComputeScheduleDayFromDate( const CFoundation::Date& _DateToday, const CFoundation::Date& _Date );

    /// Computes the date out of the passed schedule day
    CFoundation::Date   ComputeDateFromScheduleDay( const CFoundation::Date& _DateToday, ScheduleDay _eScheduleDay );
        
    INCLUDE_SHARED_CLASS( Advert );
    INCLUDE_SHARED_CLASS( Broadcast );
    INCLUDE_SHARED_CLASS( Schedule );
    enum Hour;

    enum SchedulingAdvertEvent
    {
        SCHEDULING_EVENT_ADVERT_SENT,
    };
    TYPEDEF_DELEGATE2( SchedulingAdvertEvent, AdvertPtr, bool );
    
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// Scheduling
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class Scheduling : public CFoundation::Delegate2Invoker< SchedulingAdvertEvent, AdvertPtr, bool >
    {
    public:
        DELEGATE2_INVOKER( SchedulingAdvertEvent, AdvertPtr, bool );

        /// Constructor
        Scheduling();

        /// Assignment operator
        void                operator=( const Scheduling& _rhs );

        /// Serialization
        void                Serialize( CFoundation::Serializer& _Serializer );

        /// Sets the schedule
        void                SetSchedule( ScheduleDay _eDay, SchedulePtr _spSchedule );

        /// Returns the schedule
        SchedulePtr         GetSchedule( ScheduleDay _eDay ) const;

        /// Indicates if the passed broadcast is planned from the passed day and hour on
        bool                IsBroadcastPlanned( BroadcastPtr _spBroadcast, ScheduleDay _eDayBegin, Hour _eHourBegin ) const;

        /// Removes all adverts of the passed type from the passed day and hour on
        void                RemoveAdverts( AdvertPtr _spAdvert, ScheduleDay _eDayBegin, Hour _eHourBegin );

        /// Returns the number of planned adverts from the first passed till the second passed day/hour (excluding)
        Unsigned32          GetNumAdverts( AdvertPtr _spAdvert, ScheduleDay _eDayBegin, Hour _eHourBegin, ScheduleDay _eDayEnd, Hour _eHourEnd ) const;

    private:
        /// Schedules
        SchedulePtr         m_aspSchedules[ SCHEDULE_DAY_END ];
    };
}

#endif
