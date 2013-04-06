#ifndef CMADTV_GENREMANAGER_H
#define CMADTV_GENREMANAGER_H

#include "CFoundation/Utils/Singleton.h"

#include "cmadtv/Data/Genre.h"

namespace CMadTV
{
    class GenreManager : public CFoundation::Singleton< GenreManager >
    {
    public:
        /// Constructor
        GenreManager();

        /// Creates a genre
        GenrePtr        CreateGenre();

        /// Returns the number of genres
        Unsigned32      GetNumGenres() const;

        /// Returns the genre with the passed index
        GenrePtr        GetGenre( Unsigned32 _u32Idx ) const;

    private:
        /// Next genre ID
        Unsigned32      m_u32NextGenreID;

        /// Genres
        GenreVec        m_aspGenres;
    };
}

#endif
