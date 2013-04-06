#include "cmadtv/Data/Player/PlayerContext.h"

#include "cmadtv/Data/Game/Game.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// PlayerContext
///
////////////////////////////////////////////////////////////////////////////////////////////
        
PlayerContext::PlayerContext()
:   m_u32PlayerIdx( 0 )
{
}
        
void PlayerContext::SetDatabase( DatabasePtr _spDatabase )
{
    m_spDatabase = _spDatabase;
}

DatabasePtr PlayerContext::GetDatabase() const
{
    return m_spDatabase;
}
        
void PlayerContext::SetGame( GamePtr _spGame )
{
    m_spGame = _spGame;
}

GamePtr PlayerContext::GetGame() const
{
    return m_spGame;
}
        
void PlayerContext::SetPlayerIdx( Unsigned32 _u32PlayerIdx )
{
    m_u32PlayerIdx = _u32PlayerIdx;
}
        
Unsigned32 PlayerContext::GetPlayerIdx() const
{
    return m_u32PlayerIdx;
}

PlayerPtr PlayerContext::GetPlayer() const
{
    return m_spGame != NULL ? m_spGame->GetPlayer( m_u32PlayerIdx ) : PlayerPtr();
}
       
void PlayerContext::SetInput( PlayerInputPtr _spPlayerInput )
{
    m_spPlayerInput = _spPlayerInput;
}

PlayerInputPtr PlayerContext::GetInput() const
{
    return m_spPlayerInput;
}
