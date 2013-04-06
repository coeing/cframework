#ifndef CMADTV_ADVERT_DEALER_H
#define CMADTV_ADVERT_DEALER_H

#include "cfoundation/Base/Types.h"
#include "cfoundation/Events/DelegateInvoker.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CFoundation
{
    class Serializer;
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Advert );

    enum AdvertDealerEvent
    {
        ADVERT_DEALER_EVENT_NONE,
        ADVERT_DEALER_EVENT_ADVERT_ADDED,
        ADVERT_DEALER_EVENT_ADVERT_REMOVED,
        ADVERT_DEALER_EVENT_END
    };
    TYPEDEF_DELEGATE2( AdvertDealerEvent, AdvertPtr, Unsigned32 );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// AdvertDealer
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class AdvertDealer : public CFoundation::Delegate2Invoker< AdvertDealerEvent, AdvertPtr, Unsigned32 >
    {
    public:
        DELEGATE2_INVOKER( AdvertDealerEvent, AdvertPtr, Unsigned32 );

        static const Unsigned32 NUM_SLOTS = 10;

        /// Assignment operator
        void            operator=( const AdvertDealer& _rhs );

        /// Serialization
        void            Serialize( CFoundation::Serializer& _Serializer );

        /// Sets the advert in the passed slot
        void            SetAdvert( Unsigned32 _u32Slot, AdvertPtr _spAdvert );

        /// Adds the advert to the first free slot
        /// Returns if adding was successful
        bool            AddAdvert( AdvertPtr _spAdvert );

        /// Removes the advert from the passed slot
        void            RemoveAdvert( Unsigned32 _u32Slot );

        /// Removes the past advert from the dealer
        void            RemoveAdvert( AdvertCPtr _spAdvert );

        /// Removes all adverts from the dealer
        void            RemoveAllAdverts();

        /// Returns the number of adverts
        Unsigned32      GetNumAdverts() const;

        /// Returns the advert in the passed slot
        AdvertPtr       GetAdvert( Unsigned32 _u32Slot ) const;

        /// Checks if the dealer owns the passed advert
        bool            HasAdvert( AdvertCPtr _spAdvert ) const;

        /// Returns the number of free slots
        Unsigned32      GetNumFreeSlots() const;

    private:
        /// Adverts
        AdvertPtr       m_aspAdverts[ NUM_SLOTS ];
    };
}

#endif
