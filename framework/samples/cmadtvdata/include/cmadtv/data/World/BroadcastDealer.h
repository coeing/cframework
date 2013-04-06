#ifndef CMADTV_BROADCAST_DEALER_H
#define CMADTV_BROADCAST_DEALER_H

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

    enum BroadcastDealerEvent
    {
        BROADCAST_DEALER_EVENT_NONE,
        BROADCAST_DEALER_EVENT_BROADCAST_ADDED,
        BROADCAST_DEALER_EVENT_BROADCAST_REMOVED,
        BROADCAST_DEALER_EVENT_END
    };
    TYPEDEF_DELEGATE2( BroadcastDealerEvent, BroadcastPtr, Unsigned32 );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// BroadcastDealer
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class BroadcastDealer : public CFoundation::Delegate2Invoker< BroadcastDealerEvent, BroadcastPtr, Unsigned32 >
    {
    public:
        DELEGATE2_INVOKER( BroadcastDealerEvent, BroadcastPtr, Unsigned32 );

        static const Unsigned32 NUM_SLOTS = 10;

        /// Assignment operator
        void            operator=( const BroadcastDealer& _rhs );

        /// Serialization
        virtual void    Serialize( CFoundation::Serializer& _Serializer );

        /// Sets the broadcast in the passed slot
        void            SetBroadcast( Unsigned32 _u32Slot, BroadcastPtr _spBroadcast );

        /// Adds the broadcast to the first free slot
        /// Returns if adding was successful
        bool            AddBroadcast( BroadcastPtr _spBroadcast );

        /// Removes the broadcast from the passed slot
        void            RemoveBroadcast( Unsigned32 _u32Slot );

        /// Removes the past broadcast from the dealer
        void            RemoveBroadcast( BroadcastPtr _spBroadcast );

        /// Removes all broadcasts
        void            RemoveAllBroadcasts();

        /// Returns the number of broadcasts
        Unsigned32      GetNumBroadcasts() const;

        /// Returns the broadcast in the passed slot
        BroadcastPtr    GetBroadcast( Unsigned32 _u32Slot ) const;

        /// Checks if the dealer owns the passed broadcast
        bool            HasBroadcast( BroadcastPtr _spBroadcast ) const;

        /// Returns the number of free slots
        Unsigned32      GetNumFreeSlots() const;

    private:
        /// Broadcasts
        BroadcastPtr    m_aspBroadcasts[ NUM_SLOTS ];
    };
}

#endif
