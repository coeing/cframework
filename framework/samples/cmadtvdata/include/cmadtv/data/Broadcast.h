#ifndef CMADTV_BROADCAST_H
#define CMADTV_BROADCAST_H

#include <list>
#include <set>
#include <vector>

#include "cfoundation/Base/Defines.h"
#include "CFoundation/Finance/Money.h"
#include "cfoundation/RTTI/rtti.h"
#include "cfoundation/Serialization/Serializable.h"
#include "CFoundation/Text/String.h"
#include "CFoundation/Utils/SharedPtr.h"

namespace CFoundation
{
    class Serializer;
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Genre );

    enum Country;

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// Broadcast
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    /// Represents a broadcast
    /**
     *  \author Christian Oeing (oeing@gmx.at)
     *  \date 18.01.2009
     */
    class Broadcast : public CFoundation::Serializable
    {
    public:
        /// Constructor
        Broadcast( Unsigned32 _u32ID = MAX_U32 );

        /// Serialization
        virtual void                        Serialize( CFoundation::Serializer& _Serializer );

        /// Sets the id
        virtual void                        SetID( Unsigned32 _u32ID );

        /// Returns the id
        virtual Unsigned32                  GetID() const;

        /// Sets the title
        virtual void                        SetTitle( const CFoundation::String& _strTitle );

        /// Returns the title
        virtual const CFoundation::String&  GetTitle() const;

        /// Sets the genre
        virtual void                        SetGenre( GenrePtr _spGenre );

        /// Returns the genre
        virtual GenrePtr                    GetGenre() const;

        /// Sets the description
        virtual void                        SetDescription( const CFoundation::String& _strDescription );

        /// Returns the description
        virtual const CFoundation::String&  GetDescription() const;

        /// Sets the country
        void                                SetCountry( Country _eCountry );

        /// Returns the country
        Country                             GetCountry() const;

        /// Sets the year
        void                                SetYear( Unsigned16 _u16Year );

        /// Returns the year
        Unsigned16                          GetYear() const;

        /// Sets the number of blocks
        virtual void                        SetNumBlocks( Unsigned32 _u32NumBlocks );

        /// Returns the number of blocks
        virtual Unsigned32                  GetNumBlocks() const;

        /// Sets the price
        virtual void                        SetPrice( const CFoundation::Money& _clPrice );

        /// Returns the price
        virtual const CFoundation::Money&   GetPrice() const;

        /// Returns the current price
        /// Considers the up-to-dateness
        virtual CFoundation::Money          GetCurrentPrice() const;

        /// Sets the up-to-dateness
        virtual void                        SetUp2date( Unsigned8 _u8Up2date );

        /// Gets the up-to-dateness
        virtual Unsigned8                   GetUp2date() const;

    private:
        /// ID
        Unsigned32                      m_u32ID;

        /// Title
        CFoundation::String             m_strTitle;

        /// Genre
        GenrePtr                        m_spGenre;

        /// Description
        CFoundation::String             m_strDescription;

        /// Country
        Country                         m_eCountry;

        /// Year
        Unsigned8                       m_u8Year;

        /// Number of blocks
        Unsigned32                      m_u32NumBlocks;

        /// Price
        CFoundation::Money              m_clPrice;

        /// Up-to-dateness
        Unsigned8                       m_u8Up2date;

        TYPE_DATA
    };

    /// Broadcast pointer
    typedef SharedPtr< Broadcast > BroadcastPtr;

    /// Broadcast vector
    typedef std::vector< BroadcastPtr > BroadcastVec;

    /// Broadcast list
    typedef std::list< BroadcastPtr > BroadcastList;

    /// Broadcast set
    typedef std::set< BroadcastPtr > BroadcastSet;
}

#endif
