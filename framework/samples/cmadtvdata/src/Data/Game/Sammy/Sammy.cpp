#include "cmadtv/Data/Game/Sammy/Sammy.h"

#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/Text/String.h"

#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Game/Sammy/SammyCultural.h"
#include "cmadtv/Data/Game/Sammy/SammyNews.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// Sammy
///
////////////////////////////////////////////////////////////////////////////////////////////
        
SammyPtr Sammy::Create( Type _eType )
{
    switch ( _eType )
    {
    case TYPE_CULTURAL:
        return SammyPtr( new SammyCultural() );
    case TYPE_NEWS:
        return SammyPtr( new SammyNews() );
    }
    return SammyPtr();
}

Sammy::Sammy( Type _eType )
:   m_eType( _eType )
{
}

void Sammy::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    _Serializer.SerializeVector< CFoundation::SerializeWrapper_Value >( m_au32Points );
}
        
Sammy::Type Sammy::GetType() const
{
    return m_eType;
}
       
void Sammy::SetGame( GameCPtr _spGame )
{
    if ( _spGame == m_spGame )
    {
        return;
    }

    m_spGame = _spGame;

    // Resize point vector
    m_au32Points.clear();
    Unsigned32 u32NumPlayers = m_spGame != NULL ? m_spGame->GetNumPlayers() : 0;
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPlayers; ++u32Idx )
    {
        m_au32Points.push_back( 0 );
    }
}
        
GameCPtr Sammy::GetGame() const
{
    return m_spGame;
}
   
Unsigned32 Sammy::GetPoints( Unsigned32 _u32PlayerIdx ) const
{
    if ( _u32PlayerIdx < m_au32Points.size() )
    {
        return m_au32Points[ _u32PlayerIdx ];
    }
    else
    {
        return 0;
    }
}

Unsigned32 Sammy::GetMaxPoints() const
{
    Unsigned32 u32MaxPoints = 0;
    Unsigned32 u32NumPlayers = m_au32Points.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPlayers; ++u32Idx )
    {
        if ( m_au32Points[ u32Idx ] > u32MaxPoints )
        {
            u32MaxPoints = m_au32Points[ u32Idx ];
        }
    }
    return u32MaxPoints;
}
        
Unsigned32 Sammy::GetSummedPoints() const
{
    Unsigned32 u32SummedPoints = 0;
    Unsigned32 u32NumPlayers = m_au32Points.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPlayers; ++u32Idx )
    {
        u32SummedPoints += m_au32Points[ u32Idx ];
    }
    return u32SummedPoints;
}

void Sammy::OnNewsStarted( const CFoundation::Date& _Date, Hour _eHour )
{
}

void Sammy::OnBroadcastsStarted( const CFoundation::Date& _Date, Hour _eHour )
{
}

void Sammy::AddPoints( Unsigned32 _u32PlayerIdx, Unsigned32 _u32Points )
{
    if ( _u32Points == 0 )
    {
        return;
    }

    if ( _u32PlayerIdx < m_au32Points.size() )
    {
        m_au32Points[ _u32PlayerIdx ] += _u32Points;

        // Dispatch event
        DispatchEvent( EVENT_SAMMY_POINTS_CHANGED, m_spGame->GetPlayer( _u32PlayerIdx ) );
    }
}
