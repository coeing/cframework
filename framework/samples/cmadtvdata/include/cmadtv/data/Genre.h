#ifndef CMADTV_GENRE_H
#define CMADTV_GENRE_H

#include <list>
#include <set>
#include <vector>

#include "cfoundation/Base/Defines.h"
#include "cfoundation/RTTI/rtti.h"
#include "cfoundation/Text/String.h"
#include "cfoundation/Utils/SharedPtr.h"

#include "cmadtv/Data/World/Hour.h"

namespace CFoundation
{
    class Serializer;
}

namespace CMadTV
{
    /// Represents a genre
    /**
     *  \author Christian Oeing (oeing@gmx.at)
     *  \date 18.01.2009
     */
    class Genre
    {
    public:
        /// Constructor
        Genre( Unsigned32 _u32ID = MAX_U32 );

        /// Serialization
        void                    Serialize( CFoundation::Serializer& _Serializer );

        /// Returns the ID of the genre
        Unsigned32              GetID() const;

        /// Sets the name of the genre
        void                    SetName( const CFoundation::String& _strName );

        /// Returns the name of the genre
        const CFoundation::String&  GetName() const;

        /// Sets the rating for this genre at the passed hour
        void                    SetRating( Hour _eSlot, Unsigned8 _u8Rating );

        /// Returns the rating for this genre at the passed hour
        Unsigned8               GetRating( Hour _eSlot ) const;

    private:
        /// ID
        Unsigned32              m_u32ID;

        /// Name
        CFoundation::String     m_strName;

        /// Ratings
        Unsigned8               m_au8Ratings[ HOUR_END ];

        TYPE_DATA
    };

    /// Genre pointer
    typedef SharedPtr< Genre > GenrePtr;

    /// Genre vector
    typedef std::vector< GenrePtr > GenreVec;

    /// Genre list
    typedef std::list< GenrePtr > GenreList;

    /// Genre set
    typedef std::set< GenrePtr > GenreSet;
}

#endif
