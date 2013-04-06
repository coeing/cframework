#ifndef CETRIS_GAME_H
#define CETRIS_GAME_H

#include "cfoundation/Utils/Singleton.h"
#include "cfoundation/Serialization/Serializable.h"

#include "cetris/HighscoreList.h"

namespace Cetris
{
	class OneGame;

	class Game : public CFoundation::Singleton<Game>, public CFoundation::Serializable
	{
	public:
		Game();

		bool IsGameRunning() const;
		OneGame*		GetRunningGame();
		const OneGame*	GetRunningGame() const;
		OneGame*		StartNewGame();

		HighscoreList&	GetHighscoreList();
		const HighscoreList&	GetHighscoreList() const;

		virtual void Save( CFoundation::Serializer &clSerializer ) const;
		virtual void Load( const CFoundation::Serializer &clSerializer );

	private:
		OneGame			*m_pRunningGame;

		HighscoreList	m_clHighscoreList;
	};
}

#endif
