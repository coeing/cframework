#include "cetris/windows/GameMain.h"

#include "cfoundation/DateTime/Time.h"

#include "csystem/Input/Input.h"

#include "cgui/Gui/Button.h"
#include "cgui/Gui/TextBox.h"

#include "cetris/CetrisApp.h"
#include "cetris/BlockManager.h"
#include "cetris/Game.h"
#include "cetris/OneGame.h"
#include "cetris/Input/InputTranslator.h"

using namespace Cetris;

GameMain::GameMain()
:	m_pOneGame( NULL )
{
    m_spDelegatePause = Delegate0Ptr( new Delegate0<>( this, &GameMain::OnPause ) );
    m_spDelegateExit = Delegate0Ptr( new Delegate0<>( this, &GameMain::OnExit ) );
}

const CFoundation::String GameMain::GetXMLFileName()
{
	return "data/windows/game/GameMain.xml";
}

void GameMain::InitWindow()
{
	m_pMainPanel	= GetPanel( "MainPanel" );

	// Get block image template
    BlockManager::GetInstance().Init( GetImage( "ImgBlockTemplate" ) );

	m_pTbGameOver	= GetTextBox( "TbGameOver" );
	m_pTbGameOver->SetVisible( false );

	m_pTbTimer		= GetTextBox( "TbTimer" );
	m_pTbPoints		= GetTextBox( "TbPoints" );
	m_pTbRows		= GetTextBox( "TbRows" );
	m_pTbLevel		= GetTextBox( "TbLevel" );

	m_pBtPause		= GetButton( "BtPause" );
    m_pBtPause->RegisterDelegate( CGui::INPUT_EVENT_PUSHED, m_spDelegatePause );
	m_pBtExit		= GetButton( "BtExit" );
    m_pBtExit->RegisterDelegate( CGui::INPUT_EVENT_PUSHED, m_spDelegateExit );

	// start new game
	m_pOneGame = Game::GetInstance().StartNewGame();
}
        
void GameMain::ProcessInput( const CSystem::InputState& _state )
{
    // Check if the window should be closed
    if ( InputTranslator::GetInstance().WasExecuted( INPUT_COMMAND_GAME_PAUSE, _state ) )
    {
        OnPause();
    }
    // Check for left move
    else if ( InputTranslator::GetInstance().WasExecuted( INPUT_COMMAND_GAME_MOVE_LEFT, _state ) )
    {
		m_pOneGame->MoveFigure( OneGame::DIR_LEFT );
    }
    // Check for right move
    else if ( InputTranslator::GetInstance().WasExecuted( INPUT_COMMAND_GAME_MOVE_RIGHT, _state ) )
    {
		m_pOneGame->MoveFigure( OneGame::DIR_RIGHT );
    }
    // Check for down move
    else if ( InputTranslator::GetInstance().WasExecuted( INPUT_COMMAND_GAME_MOVE_DOWN, _state ) )
    {
        m_pOneGame->MoveFigure( OneGame::DIR_DOWN );
    }
    // Check for rotate left
    else if ( InputTranslator::GetInstance().WasExecuted( INPUT_COMMAND_GAME_ROT_CCW, _state ) )
    {
		m_pOneGame->RotateFigure( OneGame::ROT_CCW );
    }
    // Check for rotate right
    else if ( InputTranslator::GetInstance().WasExecuted( INPUT_COMMAND_GAME_ROT_CW, _state ) )
    {
		m_pOneGame->RotateFigure( OneGame::ROT_CW );
    }
}

void GameMain::OnPause()
{
	if ( m_pOneGame->IsPaused() )
	{
		m_pOneGame->Continue();
		m_pBtPause->SetText( "Pause" );
	}
	else
	{
		m_pOneGame->Pause();
		m_pBtPause->SetText( "Weiter" );
	}
}

void GameMain::OnExit()
{
	BlockManager::GetInstance().Clear();
	Close();
}

//		
//void GameMain::OnKeyPressed( const CGui::KeyboardEventMsg &clMessage )
//{
//	if( m_pOneGame->GameOver() )
//	{
//		BlockManager::GetInstance().Clear();
//		Close();
//	}
//}
//
//void GameMain::OnKeyReleased( const CGui::KeyboardEventMsg &clMessage )
//{
//	if( clMessage.GetTrigger() == CGui::KEY_DOWN )
//	{
//	}
//}
		
void GameMain::Update( Float32 /*_f32TimeDifference*/ )
{
	if( m_pOneGame->GameOver() )
	{
		m_pTbGameOver->SetVisible( true );
        m_pBtPause->SetDisabled( true );
	}
	else
	{
		m_pTbTimer->SetText( m_pOneGame->GetElapsedTime().ToString() );

		m_pOneGame->Update();

        m_pTbPoints->SetText( CFoundation::String( m_pOneGame->GetPoints() ) );
		m_pTbRows->SetText( CFoundation::String( m_pOneGame->GetNumCompletedRows() ) );
		m_pTbLevel->SetText( CFoundation::String( m_pOneGame->GetLevel() ) );
	}
}