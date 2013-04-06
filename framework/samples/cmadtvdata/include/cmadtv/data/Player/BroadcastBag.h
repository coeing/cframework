#ifndef CMADTV_BROADCAST_BAG_H
#define CMADTV_BROADCAST_BAG_H

#include "cfoundation/Base/Types.h"
#include "cfoundation/Events/DelegateInvoker.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CFoundation
{
    class Serializer;
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Broadcast );

    enum BroadcastBagEvent
    {
        BROADCAST_BAG_EVENT_NONE,

        BROADCAST_BAG_EVENT_BROADCAST_ADDED,
        BROADCAST_BAG_EVENT_BROADCAST_REMOVED,

        BROADCAST_BAG_EVENT_END
    };
    TYPEDEF_DELEGATE1( BroadcastBagEvent, BroadcastPtr );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// BroadcastBag
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class BroadcastBag : public CFoundation::Delegate1Invoker< BroadcastBagEvent, BroadcastPtr >
    {
    public:
        DELEGATE1_INVOKER( BroadcastBagEvent, BroadcastPtr );

        static const Unsigned32 MAX_NUM_BROADCASTS = 10;

        /// Assignment operator
        void            operator=( const BroadcastBag& _rhs );

        /// Serialization
        void            Serialize( CFoundation::Serializer& _Serializer );

        /// Tries to add a broadcast to the bag, returns if successful
        /// If bag is full this operation would fail.
        bool            AddBroadcast( BroadcastPtr _spBroadcast );

        /// Removes a broadcast
        void            RemoveBroadcast( BroadcastPtr _spBroadcast );

        /// Returns the current number of broadcasts
        Unsigned32      GetNumBroadcasts() const;

        /// Returns the broadcast with the passed index
        BroadcastPtr    GetBroadcast( Unsigned32 _u32Idx ) const;

        /// Returns the broadcasts
        const std::vector< BroadcastPtr >&  GetBroadcasts() const;

        /// Indicates if the passed broadcast is in the bag
        bool            HasBroadcast( BroadcastPtr _spBroadcast ) const;

    private:
        typedef std::vector< BroadcastPtr > BroadcastVec;

        /// Broadcasts
        BroadcastVec    m_aspBroadcasts;
    };
}

#endif
