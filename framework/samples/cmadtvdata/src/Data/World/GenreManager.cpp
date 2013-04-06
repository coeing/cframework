#include "cmadtv/Data/GenreManager.h"

using namespace CMadTV;

GenreManager::GenreManager()
:   m_u32NextGenreID( 0 )
{
}

GenrePtr GenreManager::CreateGenre()
{
    GenrePtr spNewGenre( new Genre() );
    spNewGenre->SetID( m_u32NextGenreID++ );
    m_aspGenres.push_back( spNewGenre );
    return spNewGenre;
}

Unsigned32 GenreManager::GetNumGenres() const
{
    return m_aspGenres.size();
}

GenrePtr GenreManager::GetGenre( Unsigned32 _u32Idx ) const
{
    if ( _u32Idx < GetNumGenres() )
    {
        return m_aspGenres[ _u32Idx ];
    }
    else
    {
        return GenrePtr();
    }
}
