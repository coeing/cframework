#ifndef CMADTV_ADVERT_BAG_H
#define CMADTV_ADVERT_BAG_H

#include "CFoundation/Base/Types.h"
#include "CFoundation/Events/DelegateInvoker.h"
#include "CFoundation/Utils/SharedPtr.h"

namespace CFoundation
{
    class Serializer;
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Advert );

    enum AdvertBagEvent
    {
        ADVERT_BAG_EVENT_NONE,

        ADVERT_BAG_EVENT_ADVERT_ADDED,
        ADVERT_BAG_EVENT_ADVERT_REMOVED,

        ADVERT_BAG_EVENT_END
    };
    TYPEDEF_DELEGATE1( AdvertBagEvent, AdvertPtr );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// AdvertBag
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class AdvertBag : public CFoundation::Delegate1Invoker< AdvertBagEvent, AdvertPtr >
    {
    public:
        DELEGATE1_INVOKER( AdvertBagEvent, AdvertPtr );

        static const Unsigned32 MAX_NUM_ADVERTS = 10;

        /// Assignment operator
        void            operator=( const AdvertBag& _rhs );

        /// Serialization
        void            Serialize( CFoundation::Serializer& _Serializer );

        /// Tries to add a Advert to the bag, returns if successful
        /// If bag is full this operation would fail.
        bool            AddAdvert( AdvertPtr _spAdvert );

        /// Removes a Advert
        void            RemoveAdvert( AdvertPtr _spAdvert );

        /// Indicates if the passed advert is in the bag
        bool            HasAdvert( AdvertPtr _spAdvert ) const;

        /// Returns the current number of Adverts
        Unsigned32      GetNumAdverts() const;

        /// Returns the Advert with the passed index
        AdvertPtr       GetAdvert( Unsigned32 _u32Idx ) const;

        /// Returns the Adverts
        const std::vector< AdvertPtr >&  GetAdverts() const;

    private:
        typedef std::vector< AdvertPtr > AdvertVec;

        /// Adverts
        AdvertVec    m_aspAdverts;
    };
}

#endif
