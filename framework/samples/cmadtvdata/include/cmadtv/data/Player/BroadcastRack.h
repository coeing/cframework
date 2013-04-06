#ifndef CMADTV_BROADCASTRACK_H
#define CMADTV_BROADCASTRACK_H

#include <vector>

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
    typedef std::vector< BroadcastPtr > BroadcastVec;
    INCLUDE_SHARED_CLASS( Genre );

    enum BroadcastRackEvent
    {
        BROADCAST_RACK_EVENT_NONE,

        BROADCAST_RACK_EVENT_BROADCAST_ADDED,
        BROADCAST_RACK_EVENT_BROADCAST_REMOVED,

        BROADCAST_RACK_EVENT_END
    };
    TYPEDEF_DELEGATE1( BroadcastRackEvent, BroadcastPtr );

    /// Contains all broadcasts that can be added to the planning
    /** There are three places where a broadcast can be when a 
     *  player owns it: The bag, the rack and the archiv. If
     *  he wants to add a broadcast to his planning the broadcast
     *  has first to be in the rack.
     * 
     *  \author Christian Oeing (oeing@gmx.at)
     *  \date 18.01.2009
     */
    class BroadcastRack : public CFoundation::Delegate1Invoker< BroadcastRackEvent, BroadcastPtr >
    {
    public:
        DELEGATE1_INVOKER( BroadcastRackEvent, BroadcastPtr );

        static const Unsigned32 MAX_BROADCASTS_PER_GENRE = 4;

        /// Constructor
        BroadcastRack();

        /// Assignment operator
        void            operator=( const BroadcastRack& _rhs );

        /// Serialization
        virtual void    Serialize( CFoundation::Serializer& _Serializer );

        /// Adds the broadcast to the rack
        void            AddBroadcast( BroadcastPtr _spBroadcast );

        /// Removes the broadcast from the rack
        void            RemoveBroadcast( BroadcastPtr _spBroadcast );

        /// Returns the number of broadcasts in the rack
        Unsigned32      GetNumBroadcasts() const;

        /// Indicates if the passed broadcast is in the rack
        bool            HasBroadcast( BroadcastPtr _spBroadcast ) const;

        /// Returns the broadcast with the passed index
        BroadcastPtr    GetBroadcast( Unsigned32 _u32Idx ) const;

        /// Returns the number of broadcasts of the passed genre in the rack
        Unsigned32      GetNumBroadcasts( Unsigned32 _u32GenreID ) const;

        /// Returns the number of broadcasts of the passed genre in the rack
        Unsigned32      GetNumBroadcasts( GenrePtr _spGenre ) const;

        /// Fills the passed vector with all broadcasts of the passed genre
        void            GetBroadcasts( Unsigned32 _u32GenreID, BroadcastVec& _apBroadcasts ) const;

    private:
        /// Broadcasts in the rack
        BroadcastVec    m_aspBroadcasts;
    };
}

#endif
