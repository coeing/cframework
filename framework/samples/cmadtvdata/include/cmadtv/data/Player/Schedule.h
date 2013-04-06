#ifndef CMADTV_SCHEDULE_H
#define CMADTV_SCHEDULE_H

#include "cfoundation/Base/Types.h"
#include "cfoundation/Events/DelegateInvoker.h"
#include "cfoundation/Utils/SharedPtr.h"

#include "cmadtv/Data/World/Hour.h"

namespace CFoundation
{
    class Serializer;
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Advert );
    INCLUDE_SHARED_CLASS( Broadcast );

    /// Schedule events
    enum ScheduleBroadcastEvent
    {
        SCHEDULE_EVENT_BROADCAST_ADDED,
        SCHEDULE_EVENT_BROADCAST_REMOVED
    };
    enum ScheduleAdvertEvent
    {
        SCHEDULE_EVENT_ADVERT_ADDED,
        SCHEDULE_EVENT_ADVERT_REMOVED
    };

    /// Schedule delegates
    typedef Delegate2< BroadcastPtr, Hour > ScheduleBroadcastEventDelegate;
    typedef SharedPtr< ScheduleBroadcastEventDelegate > ScheduleBroadcastEventDelegatePtr;
    typedef Delegate2< AdvertPtr, Hour > ScheduleAdvertEventDelegate;
    typedef SharedPtr< ScheduleAdvertEventDelegate > ScheduleAdvertEventDelegatePtr;

    /// Schedule of one broadcast station for one day
    class Schedule : public CFoundation::Delegate2Invoker< ScheduleBroadcastEvent, BroadcastPtr, Hour >,
                     public CFoundation::Delegate2Invoker< ScheduleAdvertEvent, AdvertPtr, Hour >
    {
    public:
        DELEGATE2_INVOKER( ScheduleBroadcastEvent, BroadcastPtr, Hour );
        DELEGATE2_INVOKER( ScheduleAdvertEvent, AdvertPtr, Hour );

        /// Constructor
        Schedule();

        /// Destructor
        ~Schedule();

        /// Assignment operator
        void                operator=( const Schedule& _rhs );

        /// Serialization
        void                Serialize( CFoundation::Serializer& _Serializer );

        /// Removes all broadcasts and adverts from the schedule
        void                Clear();

        /// Sets the passed broadcast to start at the passed time
        /** Removes broadcasts that overlap
         */
        void                SetBroadcast( Hour _eStartSlot, BroadcastPtr _spBroadcast );

        /// Removes the broadcast from the passed slot and all slots that belong to it
        void                RemoveBroadcast( Hour _eSlot );

        /// Returns the broadcast that's planned at the passed time
        BroadcastPtr        GetBroadcast( Hour _eSlot ) const;

        /// Returns the block number of the broadcast at the passed time
        Unsigned32          GetBroadcastBlockNumber( Hour _eHour ) const;

        /// Returns if at the passed time a broadcast starts
        bool                DoesBroadcastStart( Hour _eHour ) const;

        /// Returns if at the passed time a broadcast ends
        bool                DoesBroadcastEnd( Hour _eHour ) const;

        /// Returns the number of planned broadcasts from the first passed till the second passed hour (excluding)
        Unsigned32          GetNumBroadcasts( BroadcastPtr _spBroadcast, Hour _eHourBegin, Hour _eHourEnd ) const;

        /// Indicates if the passed broadcast is planned between the first passed and the second passed hour (excluding)
        bool                IsBroadcastPlanned( BroadcastPtr _spBroadcast, Hour _eHourBegin, Hour _eHourEnd ) const;

        /// Sets the passed advert at the passed time
        void                SetAdvert( Hour _eSlot, AdvertPtr _spAdvert );

        /// Removes the advert from the passed slot and all slots that belong to it
        void                RemoveAdvert( Hour _eSlot );

        /// Removes all adverts of the passed type from the first passed till the second passed hour (excluding)
        void                RemoveAdverts( AdvertPtr _spAdvert, Hour _eHourBegin, Hour _eHourEnd );

        /// Returns the advert that's planned at the passed time
        AdvertPtr           GetAdvert( Hour _eSlot ) const;

        /// Returns the number of planned adverts from the first passed till the second passed hour (excluding)
        Unsigned32          GetNumAdverts( AdvertPtr _spAdvert, Hour _eHourBegin, Hour _eHourEnd ) const;
        
    private:
        /// Planned broadcasts
        BroadcastPtr        m_aspBroadcasts[ HOUR_END ];

        /// Indicates if a broadcast starts at this block
        bool                m_abBroadcastStart[ HOUR_END ];

        /// Planned adverts
        AdvertPtr           m_aspAdverts[ HOUR_END ];
    };

    /// Schedule pointer
    typedef SharedPtr< Schedule > SchedulePtr;
}

#endif
