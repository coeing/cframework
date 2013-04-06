#include "cetris/OneGame.h"

#include "cfoundation/Math/Random.h"
#include "cfoundation/DateTime/Time.h"

#include "cetris/BlockManager.h"

using namespace Cetris;

OneGame::OneGame()
:	m_eState( STATE_NOT_STARTED ),
	m_pFigure( NULL ),
	m_u64LastTurnMs( 0 ),
	m_u16MsBetweenTurns( 250 ),
	m_u16CurMsBetweenTurns( 0 ),
	m_u64LastBlink( 0 ),
	m_u8Level( 0 ),
	m_u32CompletedRows( 0 ),
	m_u32Points( 0 )
{
	for( Unsigned8 u8Row = 0; u8Row < NUM_ROWS; ++u8Row )
	{
		for( Unsigned8 u8Col = 0; u8Col < NUM_COLS; ++u8Col )
		{
			m_pBlocks[ u8Row ][ u8Col ] = NULL;
		}
	}
}

OneGame::~OneGame()
{
	static BlockManager &gBlockManager = BlockManager::GetInstance();

	////free fallen blocks and figure
	//for( Unsigned16 u16Row = 0; u16Row < NUM_ROWS; ++u16Row )
	//{
	//	for( Unsigned16 u16Col = 0; u16Col < NUM_COLS; ++u16Col )
	//	{
	//		Block *pBlock = m_pBlocks[ u16Row ][ u16Col ];
	//		if( pBlock == NULL )
	//		{
	//			continue;
	//		}
	//		gBlockManager.FreeBlock( pBlock );
	//		m_pBlocks[ u16Row ][ u16Col ] = NULL;
	//	}
	//}

	//if( m_pFigure != NULL )
	//{
	//	gBlockManager.FreeFigure( m_pFigure );
	//}
}

void OneGame::Start( Unsigned8 u8StartLevel )
{
	m_u32CompletedRows = 0;
	m_u32Points = 0;
	SetLevel( u8StartLevel );
	SetState( STATE_RUNNING );
}

void OneGame::Update()
{
	if( m_eState == STATE_RUNNING ||
		m_eState == STATE_FAST_DOWN )
	{
		// Do a turn every second
		Unsigned64 u64OneGameMs = m_clTimer.GetElapsedMs();
		if ( u64OneGameMs > m_u64LastTurnMs + m_u16CurMsBetweenTurns )
		{
			DoTurn();
			m_u64LastTurnMs = u64OneGameMs;
		}
	}
	else if( m_eState == STATE_ROWS_COMPLETE )
	{
		if ( m_u64LastBlink + BLINK_SPEED < m_clBlinkTimer.GetElapsedMs() )
		{
			// Do a blink for all complete rows
			for( U8Set::iterator it = m_u8CompleteRows.begin(); it != m_u8CompleteRows.end(); ++it )
			{
				for( Unsigned8 u8Col = 0; u8Col < NUM_COLS; ++u8Col )
				{
					m_pBlocks[ *it ][ u8Col ]->SetVisible( !m_pBlocks[ *it ][ u8Col ]->IsVisible() );
				}	
			}

			m_u64LastBlink = m_clBlinkTimer.GetElapsedMs();
		}
		
		if ( m_clBlinkTimer.GetElapsedMs() > BLINK_ENDURANCE )
		{
			SetState( STATE_RUNNING );
		}
	}
}
		
void OneGame::MoveFigure( Direction eDir )
{
	if( m_pFigure == NULL ||
		( m_eState != STATE_RUNNING &&
		  m_eState != STATE_FAST_DOWN ) )
	{
		return;
	}

	switch( eDir )
	{
	case DIR_LEFT:
		{
			m_pFigure->Move( Figure::DIR_LEFT, 1 );
			if( !ValidFigurePosition() )
			{
				m_pFigure->Move( Figure::DIR_RIGHT, 1 );
			}
		}
		break;
	case DIR_RIGHT:
		{
			m_pFigure->Move( Figure::DIR_RIGHT, 1 );
			if( !ValidFigurePosition() )
			{
				m_pFigure->Move( Figure::DIR_LEFT, 1 );
			}
		}
		break;
	case DIR_DOWN:
		{
			SetState( STATE_FAST_DOWN );
		}
		break;
	};
}
		
void OneGame::RotateFigure( Rotation eRot )
{
	if( m_pFigure == NULL ||
		( m_eState != STATE_RUNNING &&
		  m_eState != STATE_FAST_DOWN ) )
	{
		return;
	}

	switch( eRot )
	{
	case ROT_CW:
		{
			m_pFigure->RotateCW();
			if( !ValidFigurePosition() )
			{
				m_pFigure->RotateCCW();
			}
		}
		break;
	case ROT_CCW:
		{
			m_pFigure->RotateCCW();
			if( !ValidFigurePosition() )
			{
				m_pFigure->RotateCW();
			}
		}
		break;
	};
}

void OneGame::Pause()
{
	if( !IsPaused() &&
        !GameOver() )
	{
		SetState( STATE_PAUSED );
	}
}

bool OneGame::IsPaused() const
{
	return m_eState == STATE_PAUSED;
}

void OneGame::Continue()
{
	if( IsPaused() )
	{
		SetState( STATE_RUNNING );
	}
}

Unsigned32 OneGame::GetPoints() const
{
	return m_u32Points;
}

Unsigned32 OneGame::GetNumCompletedRows() const
{
	return m_u32CompletedRows;
}

Unsigned8 OneGame::GetLevel() const
{
	return m_u8Level;
}

const CFoundation::Time OneGame::GetElapsedTime() const
{
	return m_clTimer.GetElapsedTime();
}

bool OneGame::GameOver() const
{
	return m_eState == STATE_GAME_OVER;
}

void OneGame::SetState( State eNewState )
{
	static BlockManager &gBlockManager = BlockManager::GetInstance();

	switch( eNewState )
	{
	case STATE_NOT_STARTED:
		{
		}
		break;
	case STATE_RUNNING:
		{
			if( m_eState == STATE_NOT_STARTED )
			{				
				m_clTimer.Start();
				m_u64LastTurnMs = 0;
				m_u16CurMsBetweenTurns = m_u16MsBetweenTurns;
			}
			else if( m_eState == STATE_ROWS_COMPLETE )
			{
				// add rows and points
				CompletedRows( m_u8CompleteRows.size() );

				// delete completed rows
				DeleteCompleteRows();

				if( m_pFigure != NULL )
				{
					gBlockManager.FreeFigure( m_pFigure );
					m_pFigure = NULL;
				}

				m_clTimer.Continue();
				m_u64LastTurnMs = 0;
			}
			else if( m_eState == STATE_FAST_DOWN )
			{
				m_u16CurMsBetweenTurns = m_u16MsBetweenTurns;
			}
			else if( m_eState == STATE_PAUSED )
			{
				m_clTimer.Continue();
				m_u64LastTurnMs = 0;
			}
		}
		break;
	case STATE_ROWS_COMPLETE:
		{
			if( m_eState == STATE_RUNNING )
			{
				m_clTimer.Pause();
				m_u64LastTurnMs = 0;
				m_clBlinkTimer.Start();
                m_u64LastBlink = 0;
			}
		}
		break;
	case STATE_FAST_DOWN:
		{
			if( m_eState == STATE_RUNNING )
			{
				m_u16CurMsBetweenTurns = 50;
			}
		}
		break;
	case STATE_PAUSED:
		{
			if( m_eState == STATE_RUNNING )
			{
				m_clTimer.Pause();
			}
		}
		break;
	case STATE_GAME_OVER:
		{
			if( m_pFigure != NULL )
			{
				gBlockManager.FreeFigure( m_pFigure );
				m_pFigure = NULL;
			}
			m_clTimer.Stop();
		}
		break;
	}
	m_eState = eNewState;
}

void OneGame::DoTurn()
{
	static BlockManager &gBlockManager = BlockManager::GetInstance();
	if( m_pFigure != NULL )
	{
		if( FigureDocked() )
		{
			if( m_eState == STATE_FAST_DOWN )
			{
				SetState( STATE_RUNNING );
			}

			// check if lost OneGame
			if( !ValidFigurePosition() )
			{
				SetState( STATE_GAME_OVER );
				return;
			}

			// add the blocks of the figure to the fallen blocks
			AddFigureToBlocks();

			// check for complete rows
			if( CheckForCompleteRows() )
			{
				SetState( STATE_ROWS_COMPLETE );
			}
			else
			{
				gBlockManager.FreeFigure( m_pFigure );
				m_pFigure = NULL;
			}
		}
		else
		{
			m_pFigure->Move( Figure::DIR_DOWN, 1 );
		}
	}
	
	// if no active figure create one
	if( m_pFigure == NULL )
	{
		Unsigned8 u8RandType = CFoundation::Random::GetU8( 0, Figure::NUM_TYPES );
		m_pFigure = gBlockManager.CreateFigure( static_cast<Figure::Type>( u8RandType ) );

		// set new figure to start position
		m_pFigure->SetPosition( 4, 0 );
	}
}

bool OneGame::FigureDocked() const
{
	/*! The figure docks if one of its blocks is onto a
	 *  fallen block. Or if one hits the ground.*/
	for( Unsigned8 u8Idx = 0; u8Idx < 4; ++u8Idx )
	{
		const Block *pBlock = m_pFigure->GetBlock( u8Idx );

		if( pBlock == NULL )
		{
			continue;
		}

		Unsigned8 u8Row = pBlock->GetRow();
		Unsigned8 u8Col = pBlock->GetCol();

		if( u8Row == OneGame::NUM_ROWS - 1 )
		{
			return true;
		}

		if( u8Row >= 0 && u8Row < NUM_ROWS &&
			u8Col >= 0 && u8Col < NUM_COLS &&
			m_pBlocks[ u8Row + 1 ][ u8Col ] != NULL )
		{
			return true;
		}
	}

	return false;
}
	
void OneGame::AddFigureToBlocks()
{
	static BlockManager &gBlockManager = BlockManager::GetInstance();

	for( Unsigned8 u8Idx = 0; u8Idx < 4; ++u8Idx )
	{
		Block *pBlock = m_pFigure->GetBlock( u8Idx );

		if( pBlock == NULL )
		{
			continue;
		}

		Unsigned8 u8Row = pBlock->GetRow();
		Unsigned8 u8Col = pBlock->GetCol();

		if( u8Row < NUM_ROWS &&
			u8Col < NUM_COLS )
		{
			m_pBlocks[ u8Row ][ u8Col ] = pBlock;
		}
		else
		{
			gBlockManager.FreeBlock( pBlock );
		}
	}
}

bool OneGame::CheckForCompleteRows()
{
	for( Unsigned8 u8Idx = 0; u8Idx < 4; ++u8Idx )
	{
		Block *pBlock = m_pFigure->GetBlock( u8Idx );

		if( pBlock == NULL )
		{
			continue;
		}

		Unsigned8 u8Row = pBlock->GetRow();

		bool bCompleteRow = true;
		for( Unsigned8 u8Col = 0; u8Col < NUM_COLS; ++u8Col )
		{
			if( m_pBlocks[ u8Row ][ u8Col ] == NULL )
			{
				bCompleteRow = false;
				break;
			}
		}

		if( bCompleteRow )
		{
			m_u8CompleteRows.insert( u8Row );
		}
	}

	return m_u8CompleteRows.size() > 0;
}

void OneGame::DeleteCompleteRows()
{
	if( m_u8CompleteRows.empty() )
	{
		return;
	}

	static BlockManager &gBlockManager = BlockManager::GetInstance();

	// delete complete rows
	for( U8Set::iterator it = m_u8CompleteRows.begin(); it != m_u8CompleteRows.end(); ++it )
	{
		for( Unsigned8 u8Col = 0; u8Col < NUM_COLS; ++u8Col )
		{
			gBlockManager.FreeBlock( m_pBlocks[ *it ][ u8Col ] );
			m_pBlocks[ *it ][ u8Col ] = NULL;
		}	
	}

	// move other rows down
	for( Integer8 i8Row = NUM_ROWS - 1; i8Row >= 0; --i8Row )
	{
		Unsigned8 u8MoveDown = 0;
		for( U8Set::const_iterator it = m_u8CompleteRows.begin(); it != m_u8CompleteRows.end(); ++it )
		{
			if( *it == i8Row )
			{
				u8MoveDown = 0;
				break;
			}
			else if( *it > i8Row )
			{
				++u8MoveDown;
			}
		}

		if( u8MoveDown == 0 )
		{
			continue;
		}
		for( Unsigned8 u8Col = 0; u8Col < NUM_COLS; ++u8Col )
		{
			if( m_pBlocks[ i8Row ][ u8Col ] != NULL )
			{
				m_pBlocks[ i8Row ][ u8Col ]->MoveDown( u8MoveDown );
			}
			m_pBlocks[ i8Row + u8MoveDown ][ u8Col ] = m_pBlocks[ i8Row ][ u8Col ];
		}
	}

	// remove complete rows
	m_u8CompleteRows.clear();
}

bool OneGame::ValidFigurePosition()
{
	bool bValid = true;

	for( Unsigned8 u8Idx = 0; u8Idx < 4; ++u8Idx )
	{
		const Block *pBlock = m_pFigure->GetBlock( u8Idx );

		if( pBlock == NULL )
		{
			continue;
		}

		Unsigned8 u8Row = pBlock->GetRow();
		Unsigned8 u8Col = pBlock->GetCol();

		if( u8Col >= NUM_COLS ||
			u8Row >= NUM_ROWS ||
			m_pBlocks[ u8Row ][ u8Col ] != NULL )
		{
			bValid = false;
			break;
		}
	}

	return bValid;
}

void OneGame::CompletedRows( Unsigned32 _u32NumRows )
{
	m_u32CompletedRows += _u32NumRows;

	Unsigned16 u16BasePoints = 0;
	switch ( _u32NumRows )
	{
	case 1:
		{
			u16BasePoints = 40;
		}
		break;
	case 2:
		{
			u16BasePoints = 100;
		}
		break;
	case 3:
		{
			u16BasePoints = 300;
		}
		break;
	case 4:
		{
			u16BasePoints = 1200;
		}
		break;
	}

	m_u32Points += u16BasePoints * ( m_u8Level + 1 );

	if ( m_u32CompletedRows / 10 > GetLevel() )
	{
		LevelUp();
	}
}

void OneGame::LevelUp()
{
	if( GetLevel() < 20 )
	{
		SetLevel( GetLevel() + 1 );
	}
}

void OneGame::SetLevel( Unsigned8 u8Level )
{
	m_u8Level = u8Level;
	
	// set new time between turns
	m_u16MsBetweenTurns = m_u16CurMsBetweenTurns = 1000 - m_u8Level * 45;
}