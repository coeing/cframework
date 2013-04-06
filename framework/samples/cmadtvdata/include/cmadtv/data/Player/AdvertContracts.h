#ifndef CMADTV_ADVERT_CONTRACTS_H
#define CMADTV_ADVERT_CONTRACTS_H

#include <vector>

#include "cfoundation/DateTime/Date.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CMadTV
{
    class Advert;
    typedef SharedPtr< Advert > AdvertPtr;
    class AdvertContract;
    typedef SharedPtr< AdvertContract > AdvertContractPtr;
    typedef std::vector< AdvertContractPtr > AdvertContractVec;

    /// Contains all adverts that were accepted by the player
    /** The advert contracts object of a player contains all accepted
     *  adverts.
     * 
     *  \author Christian Oeing (oeing@gmx.at)
     *  \date 04.04.2009
     */
    class AdvertContracts
    {
    public:
        /// Constructor
        AdvertContracts();

        /// Serialization
        virtual void        Serialize( CFoundation::Serializer& _Serializer );

        /// Starts a new contract of the passed advert
        void                StartContract( AdvertPtr _spAdvert, const CFoundation::Date& _clDate );

        /// Removes the passed contract
        void                RemoveContract( AdvertContractPtr _spContract );

        /// Returns the number of contracts
        Unsigned32          GetNumContracts() const;

        /// Returns the contract with the passed index
        AdvertContractPtr   GetContract( Unsigned32 _u32Idx ) const;

        /// Returns the contract for the passed advert
        AdvertContractPtr   GetContract( AdvertPtr _spAdvert ) const;

    private:
        /// Contracts
        AdvertContractVec   m_aspContracts;
    };
}

#endif
