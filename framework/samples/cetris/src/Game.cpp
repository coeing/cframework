#include "cetris/Game.h"

#include "cetris/OneGame.h"

using namespace Cetris;

Game::Game()
:	m_pRunningGame( NULL )
{
}

bool Game::IsGameRunning() const
{
	return m_pRunningGame != NULL;
}

OneGame* Game::GetRunningGame() 
{
	return m_pRunningGame;
}

const OneGame* Game::GetRunningGame() const
{
	return m_pRunningGame;
}

OneGame*	Game::StartNewGame()
{
	if( m_pRunningGame != NULL )
	{
		delete m_pRunningGame;
	}
	m_pRunningGame = new OneGame();
	m_pRunningGame->Start( 0 );

	return m_pRunningGame;
}


HighscoreList&	Game::GetHighscoreList()
{
	return m_clHighscoreList;
}

const HighscoreList&	Game::GetHighscoreList() const
{
	return m_clHighscoreList;
}
		
void Game::Save( CFoundation::Serializer &clSerializer ) const
{
	m_clHighscoreList.Save( clSerializer );
}
		
void Game::Load( const CFoundation::Serializer &clSerializer )
{
	m_clHighscoreList.Load( clSerializer );
}