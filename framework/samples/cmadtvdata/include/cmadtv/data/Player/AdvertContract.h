#ifndef CMADTV_ADVERT_CONTRACT_H
#define CMADTV_ADVERT_CONTRACT_H

#include <vector>

#include "cfoundation/DateTime/Date.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CFoundation
{
    class Serializer;
}

namespace CMadTV
{
    class Advert;
    typedef SharedPtr< Advert > AdvertPtr;
  
    /**
	 * \brief
	 * An advert contract signed by a player.
	 * 
	 * 
	 * \remarks
	 * 
	 * \see
	 * Advert | AdvertDealer
	 */
	class AdvertContract
    {
    public:
        /// Constructor
        explicit AdvertContract( AdvertPtr _spAdvert = AdvertPtr(), const CFoundation::Date& _clStartDate = CFoundation::Date() );

        /// Destructor
        ~AdvertContract();

        /// Serialization
        virtual void        Serialize( CFoundation::Serializer& _Serializer );

        /// Returns the advert this contract is for
        AdvertPtr           GetAdvert() const;

        /// Returns the spots that were sent already
        Unsigned32          GetNumSpotsSent() const;

        /// Returns the left spots to send
        Unsigned32          GetNumSpotsLeft() const;

        /// Returns the left days at the passed date
        Unsigned32          GetNumDaysLeft( const CFoundation::Date& ) const;

        /// Removes one spot
        /// Returns the number of left spots
        Unsigned32          RemoveSpot();
        
    private:
        /// Advert
        AdvertPtr           m_spAdvert;

        /// Start date
        CFoundation::Date   m_clStartDate;

        /// Sent spots
        Unsigned32          m_u32NumSpotsSent;
    };

    /// AdvertContract pointer
    typedef SharedPtr< AdvertContract > AdvertContractPtr;
    typedef SharedPtr< const AdvertContract > AdvertContractCPtr;
    typedef std::vector< AdvertContractPtr > AdvertContractVec;
}

#endif
