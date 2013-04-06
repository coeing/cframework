#ifndef CETRIS_ONEGAME_H
#define CETRIS_ONEGAME_H

#include <set>

#include "cfoundation/DateTime/Timer.h"

#include "cetris/Block.h"
#include "cetris/Figure.h"

namespace Cetris
{
	class OneGame
	{
	public:
		enum Direction
		{
			DIR_LEFT,
			DIR_RIGHT,
			DIR_DOWN
		};

		enum Rotation
		{
			ROT_CW,
			ROT_CCW
		};

		OneGame();
		~OneGame();

		void Start( Unsigned8 u8StartLevel = 0 );

		void Update();

		void MoveFigure( Direction eDir );
		void RotateFigure( Rotation eRot );

		void Pause();
		bool IsPaused() const;
		void Continue();

		const CFoundation::Time GetElapsedTime() const;

		Unsigned32	GetPoints() const;
		Unsigned32	GetNumCompletedRows() const;
		Unsigned8	GetLevel() const;

		bool GameOver() const;

	private:
		enum State
		{
			STATE_NOT_STARTED,
			STATE_RUNNING,
			STATE_ROWS_COMPLETE,
			STATE_FAST_DOWN,
			STATE_PAUSED,
			STATE_GAME_OVER,
			NUM_STATES
		};

		static const Unsigned16 BLINK_SPEED = 100;
		static const Unsigned16	BLINK_ENDURANCE = 1000;

		void SetState( State eNewState );

		void DoTurn();

		bool FigureDocked() const;
		void AddFigureToBlocks();

		bool CheckForCompleteRows();
		void DeleteCompleteRows();

		bool ValidFigurePosition();

		void CompletedRows( Unsigned32 _u32NumRows );
		void LevelUp();
		void SetLevel( Unsigned8 u8Level );

		// Control Data
		State			m_eState;

		CFoundation::Timer	m_clTimer;
		Unsigned64		m_u64LastTurnMs;
		Unsigned16		m_u16MsBetweenTurns;
		Unsigned16		m_u16CurMsBetweenTurns;

		typedef std::set<Unsigned8> U8Set;
		U8Set			m_u8CompleteRows;

		CFoundation::Timer	m_clBlinkTimer;
		Unsigned64		m_u64LastBlink;

		// Data
		static const Unsigned8	NUM_COLS = 11;
		static const Unsigned8	NUM_ROWS = 22;

		Figure			*m_pFigure;		
		Block			*m_pBlocks[ NUM_ROWS ][ NUM_COLS ];

		Unsigned8		m_u8Level;
		Unsigned32		m_u32CompletedRows;
		Unsigned32		m_u32Points;
	};
}

#endif
