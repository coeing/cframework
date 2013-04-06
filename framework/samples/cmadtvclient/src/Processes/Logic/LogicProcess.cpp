#include "cmadtvclient/Processes/Logic/LogicProcess.h"

#include "cfoundation/Enum/EnumConverter.h"

#include "csystem/Input/Input.h"

#include "cgui/Globals.h"
#include "cgui/GuiControls.h"
#include "cgui/Gui/GuiStyle.h"
#include "cgui/Input/Input.h"
#include "cgui/Tooltip/TooltipSystem.h"

#include "cctrl/WindowManager.h"

#include "cmadtv/Windows/WindowManager.h"

#include "cmadtv/Input/InputTranslator.h"
#include "cmadtv/Tooltip/AdvertTooltipController.h"
#include "cmadtv/Tooltip/BroadcastTooltipController.h"
#include "cmadtv/Tooltip/StatsRatingTooltipController.h"

#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Network/NetworkMessages.h"
#include "cmadtv/Data/Player/PlayerContext.h"
#include "cmadtv/Data/Room.h"
#include "cmadtv/Data/World/Country.h"

#include "cmadtv/Logic/PlayerInputRemote.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// LogicProcess
///
////////////////////////////////////////////////////////////////////////////////////////////

LogicProcess::LogicProcess( CSystem::ProcessManagerPtr _spProcessManager )
:   ProcessImpl( _spProcessManager ),
    m_u32Frames( 0 ),
    m_f32FPS( 0.0f ),
    m_spContext( new PlayerContext() ),
    m_spWindowManager( new WindowManager() )
{
    // Create remote input interface
    m_spContext->SetInput( PlayerInputPtr( new PlayerInputRemote() ) );

    m_spWindowManager->SetContext( m_spContext );
}
        
void LogicProcess::Init()
{
	// Init Gui
	if( !CGui::Init( "CMadTV", 640, 450 ) )
	{
		CF_WARN( L"Gui couldn't be initialized..." );
		return;
	}

    // Init Gui controls
    if ( !CGui::InitControls() )
    {
		CF_WARN( L"Gui controls couldn't be initialized..." );
		return;
    }

    // Set frame brake
    //SetMaxFrameTime( 30 );

    // Set default gui style
    CGui::GuiStylePtr spDefaultStyle( new CGui::GuiStyle( "data/ctrls/styles/gui_style.xml" ) );
    CGui::GuiMain::GetInstance().SetDefaultStyle( spDefaultStyle );
    
    // Register delegates
    RegisterDelegate( CFoundation::CreateDelegate( this, &LogicProcess::OnDatabaseUpdate ) );
    RegisterDelegate( CFoundation::CreateDelegate( this, &LogicProcess::OnStateUpdate ) );

    // Create main menu window
    m_spWindowManager->Init();
    m_spWindowManager->SetActiveRoom( ROOM_MAIN_MENU );

    // Start frame timer
    m_TimerFPS.Start();
}

void LogicProcess::Deinit()
{
    m_spWindowManager->Deinit();

    // Destroy singletons
    AdvertTooltipController::DestroyTooltip();
    BroadcastTooltipController::DestroyTooltip();
    StatsRatingTooltipController::DestroyTooltip();
    CFoundation::EnumConverter< Country >::DestroyInstance();
    CFoundation::EnumConverter< InputCommand >::DestroyInstance();
    InputTranslator::DestroyInstance();

	// Deinit gui
	CGui::Deinit();

    // Destroy window manager
    CCtrl::WindowManager::DestroyInstance();
}
        
void LogicProcess::Update( Float32 _f32TimeDifference )
{
    // Check frame timer
    ++m_u32Frames;
    Unsigned64 u64PassedTime = m_TimerFPS.GetElapsedMs();
    if ( u64PassedTime >= 1000 )
    {
        // Compute fps
        m_f32FPS = m_u32Frames * 1000.0f / u64PassedTime;
//        SAFE_CALL( m_spInfoWindow )->SetFPS( m_f32FPS );

        // Restart
        m_u32Frames = 0;
        m_TimerFPS.Start();
    }

    // Update the system input
    CSystem::Input::GetInstance().Update( _f32TimeDifference );

    // Update tooltip system
    CGui::TooltipSystem::GetInstance().Update( _f32TimeDifference );

    // Update gui input
	CGui::Input::GetInstance().Update( _f32TimeDifference );

    // Update GUI
	CGui::GuiMain::GetInstance().Update( _f32TimeDifference );

    // Let windows process input
    CCtrl::WindowManager::GetInstance().ProcessInput( CSystem::Input::GetInstance().GetState() );

    // Update Window Manager
    CCtrl::WindowManager::GetInstance().Update( _f32TimeDifference );

    // Render gui
	CGui::GuiMain::GetInstance().Render();
}     
        
void LogicProcess::SetGame( GamePtr _spGame )
{
    if ( _spGame == m_spContext->GetGame() )
    {
        return;
    }

    GamePtr spOldGame = m_spContext->GetGame();
    if ( spOldGame == NULL )
    {
        m_spContext->SetGame( _spGame );
        OnGameStarted( _spGame );
    }
    else if ( _spGame == NULL )
    {
        m_spContext->SetGame( _spGame );
        OnGameEnded( spOldGame, false );
    }
    else
    {
        // Just copy data instead of replace pointer
        *spOldGame = *_spGame;
    }

    // Set active player
    //m_spMainWindow->SetPlayer( GetActivePlayer() );
}

void LogicProcess::OnDatabaseUpdate( DatabaseUpdateMsgPtr _spMsg )
{
    m_spContext->SetDatabase( _spMsg->m_Database );
}
       
void LogicProcess::OnStateUpdate( StateMsgPtr _spMsg )
{
    SetGame( _spMsg->m_Game );
}

void LogicProcess::OnGameStarted( GamePtr _spGame )
{
    m_spWindowManager->OnContextChanged();

    // Show navigation window
    m_spWindowManager->ShowNavigationWindow( true );

    // Go to boss
    m_spWindowManager->SetActiveRoom( ROOM_BOSS );
}

void LogicProcess::OnGameLost( GamePtr _spGame )
{
    OnGameEnded( _spGame, false );
}

void LogicProcess::OnGameWon( GamePtr _spGame )
{
    OnGameEnded( _spGame, true );
}
        
void LogicProcess::OnGameEnded( GamePtr _spGame, bool _bWon )
{
    m_spWindowManager->SetContext( PlayerContextPtr() );
    m_spWindowManager->ShowNavigationWindow( false );

    // Open game over window
    m_spWindowManager->SetActiveRoom( ROOM_GAME_OVER );
}
