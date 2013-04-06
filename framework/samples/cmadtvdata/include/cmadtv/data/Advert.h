#ifndef CMADTV_ADVERT_H
#define CMADTV_ADVERT_H

#include <list>
#include <set>
#include <vector>

#include "cfoundation/Base/Defines.h"
#include "CFoundation/Finance/Money.h"
#include "CFoundation/Text/String.h"
#include "cfoundation/RTTI/rtti.h"
#include "CFoundation/Utils/SharedPtr.h"

namespace CFoundation
{
    class Serializer;
}

namespace CMadTV
{
    /// Represents an advert
    /**
     *  \author Christian Oeing (oeing@gmx.at)
     *  \date 20.04.2009
     */
    class Advert
    {
    public:
        /// Constructor
        Advert( Unsigned32 _u32ID = MAX_U32 );

        /// Serialization
        void                            Serialize( CFoundation::Serializer& _Serializer );

        /// Sets the id
        virtual void                    SetID( Unsigned32 _u32ID );

        /// Returns the id
        virtual Unsigned32              GetID() const;

        /// Sets the name
        void                            SetName( const CFoundation::String& _strTitle );

        /// Returns the name
        const CFoundation::String&      GetName() const;

        /// Sets the slogan
        void                            SetSlogan( const CFoundation::String& _strSlogan );

        /// Returns the slogan
        const CFoundation::String&      GetSlogan() const;

        /// Sets the bonus
        void                            SetBonus( const CFoundation::Money& _clBonus );

        /// Returns the bonus
        const CFoundation::Money&       GetBonus() const;

        /// Sets the penalty
        void                            SetPenalty( const CFoundation::Money& _clPenalty );

        /// Returns the penalty
        const CFoundation::Money&       GetPenalty() const;

        /// Sets the minimum amount of spectators
        void                            SetMinSpectators( Unsigned32 _u32MinSpectators );

        /// Returns the minimum amount of spectators
        Unsigned32                      GetMinSpectators() const;

        /// Sets the number of spots
        void                            SetNumSpots( Unsigned32 _u32NumSpots );

        /// Returns the number of spots
        Unsigned32                      GetNumSpots() const;

        /// Sets the number of days
        void                            SetNumDays( Unsigned32 _u32NumDays );

        /// Returns the number of days
        Unsigned32                      GetNumDays() const;

    private:
        /// ID
        Unsigned32                      m_u32ID;

        /// Name
        CFoundation::String             m_strName;

        /// Description
        CFoundation::String             m_strSlogan;

        /// Bonus
        CFoundation::Money              m_clBonus;

        /// Penalty
        CFoundation::Money              m_clPenalty;

        /// Minimum amount of spectators
        Unsigned32                  m_u32MinSpectators;

        /// Number of spots
        Unsigned32                  m_u32NumSpots;

        /// Number of days
        Unsigned32                  m_u32NumDays;

        TYPE_DATA
    };

    /// Advert pointer
    typedef SharedPtr< Advert > AdvertPtr;
    typedef SharedPtr< const Advert > AdvertCPtr;

    /// Advert vector
    typedef std::vector< AdvertPtr > AdvertVec;

    /// Advert list
    typedef std::list< AdvertPtr > AdvertList;

    /// Advert set
    typedef std::set< AdvertPtr > AdvertSet;
}

#endif
