#include "cmadtv/Application/Application.h"

#include "csystem/Files/File.h"

#include "cgui/Gui/GuiStyle.h"

#include "cmadtv/Windows/InfoWindow.h"
#include "cmadtv/Windows/NavigationWindow.h"

// Main windows
#include "cmadtv/Windows/AdvertDealerWindow.h"
#include "cmadtv/Windows/Betty/BettyWindow.h"
#include "cmadtv/Windows/Boss/BossWindow.h"
#include "cmadtv/Windows/BroadcastDealerWindow.h"
#include "cmadtv/Windows/GameOver/GameOverWindow.h"
#include "cmadtv/Windows/LoadSaveWindow.h"
#include "cmadtv/Windows/MainMenuWindow.h"
#include "cmadtv/Windows/Library/LibraryWindow.h"
#include "cmadtv/Windows/MissionMenu/MissionMenuWindow.h"
#include "cmadtv/Windows/NewsStudio/NewsStudioWindow.h"
#include "cmadtv/Windows/Spectators/SpectatorsWindow.h"
#include "cmadtv/Windows/Supermarket/SupermarketPresentsWindow.h"
#include "cmadtv/Windows/PlanningWindow.h"

#include "cmadtv/Input/InputTranslator.h"
#include "cmadtv/Tooltip/AdvertTooltipController.h"
#include "cmadtv/Tooltip/BroadcastTooltipController.h"
#include "cmadtv/Tooltip/StatsRatingTooltipController.h"

#include "cmadtv/Data/Advert.h"
#include "cmadtv/Data/Broadcast.h"
#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/Schedule.h"
#include "cmadtv/Data/World/World.h"

using namespace CMadTV;

Application::Application()
:   m_eRoom( ROOM_NONE ),
    m_u32ActivePlayer( 0 )
{
    m_spDelegateGameStarted = WorldEventGameDelegate1Ptr( new WorldEventGameDelegate1( this, &Application::OnGameStarted ) );
    m_spDelegateGameLost = WorldEventGameDelegate1Ptr( new WorldEventGameDelegate1( this, &Application::OnGameLost ) );
    m_spDelegateGameWon = WorldEventGameDelegate1Ptr( new WorldEventGameDelegate1( this, &Application::OnGameWon ) );
}

Application::~Application()
{
}

void Application::SetActiveRoom( Room _eRoom )
{
    if ( m_eRoom == _eRoom )
    {
        return;
    }

    // Serialize game
    if ( _eRoom != ROOM_NONE )
    {
        CFoundation::Serializer serializer( CFoundation::Serializer::MODE_SAVE );
        m_spWorld->Serialize( serializer );
        serializer.SaveFile( "LastGame.sav" );
    }

    // Close active window
    if ( m_spMainWindow != NULL )
    {
        m_spMainWindow->Close();
        m_spMainWindow.reset();
    }

    // Set new active window
    switch ( _eRoom )
    {
    case ROOM_NONE:
        {
            // No room
        }
        break;
    case ROOM_MAIN_MENU:
        {
            SharedPtr< MainMenuWindow > spWindow = CCtrl::WindowController::Create< MainMenuWindow >();
            spWindow->SetWorld( m_spWorld );
            m_spMainWindow = spWindow;
        }
        break;
    case ROOM_BOSS:
        {
            SharedPtr< BossWindow > spWindow = CCtrl::WindowController::Create< BossWindow >();
            spWindow->SetPlayer( GetActivePlayer() );
            m_spMainWindow = spWindow;
        }
        break;
    case ROOM_PLANNING:
        {
            SharedPtr< PlanningWindow > spWindow = CCtrl::WindowController::Create< PlanningWindow >();
            //spWindow->SetContext( m_spContext );
            m_spMainWindow = spWindow;
        }
        break;
    case ROOM_SPECTATORS:
        {
            SharedPtr< SpectatorsWindow > spWindow = CCtrl::WindowController::Create< SpectatorsWindow >();
            //spWindow->SetContext( m_spContext );
            m_spMainWindow = spWindow;
        }
        break;
    case ROOM_LOAD:
        {
            SharedPtr< LoadSaveWindow > spLoadSaveWindow = CCtrl::WindowController::Create< LoadSaveWindow >();
            spLoadSaveWindow->Init( LoadSaveWindow::MODE_LOAD, m_eRoom );
            m_spMainWindow = spLoadSaveWindow;
        }
        break;
    case ROOM_SAVE:
        {
            SharedPtr< LoadSaveWindow > spLoadSaveWindow = CCtrl::WindowController::Create< LoadSaveWindow >();
            spLoadSaveWindow->Init( LoadSaveWindow::MODE_SAVE, m_eRoom );
            m_spMainWindow = spLoadSaveWindow;
        }
        break;
    case ROOM_LOAD_SAVE:
        {
            SharedPtr< LoadSaveWindow > spLoadSaveWindow = CCtrl::WindowController::Create< LoadSaveWindow >();
            spLoadSaveWindow->Init( LoadSaveWindow::MODE_NONE, m_eRoom );
            m_spMainWindow = spLoadSaveWindow;
        }
        break;
    case ROOM_BROADCAST_DEALER:
        {
            // Fill up broadcasts
//            m_spGame->FillBroadcastDealer();

            SharedPtr< BroadcastDealerWindow > spWindow = CCtrl::WindowController::Create< BroadcastDealerWindow >();
//            spWindow->Init( m_spGame->GetBroadcastDealer(), m_spGame->GetActivePlayer() );
            m_spMainWindow = spWindow;
        }
        break;
    case ROOM_ADVERT_DEALER:
        {
            // Fill up adverts
//            m_spWorld->FillAdvertDealer();

            SharedPtr< AdvertDealerWindow > spWindow = CCtrl::WindowController::Create< AdvertDealerWindow >();
//            spWindow->Init( m_spGame->GetAdvertDealer(), m_spGame->GetActivePlayer() );
            m_spMainWindow = spWindow;
        }
        break;
    case ROOM_NEWS_STUDIO:
        {
            SharedPtr< NewsStudioWindow > spWindow = CCtrl::WindowController::Create< NewsStudioWindow >();
            m_spMainWindow = spWindow;
        }
        break;
    case ROOM_LIBRARY:
        {
            SharedPtr< LibraryWindow > spWindow = CCtrl::WindowController::Create< LibraryWindow >();
            m_spMainWindow = spWindow;
        }
        break;
    case ROOM_SUPERMARKET:
        {
            SharedPtr< SupermarketPresentsWindow > spWindow = CCtrl::WindowController::Create< SupermarketPresentsWindow >();
            m_spMainWindow = spWindow;
        }
        break;
    case ROOM_BETTY:
        {
            SharedPtr< BettyWindow > spWindow = CCtrl::WindowController::Create< BettyWindow >();
            m_spMainWindow = spWindow;
        }
        break;
    default:
        {
            CF_WARN( "Invalid room." );
        }
        break;
    }

    m_eRoom = _eRoom;
}

Room Application::GetActiveRoom() const
{
    return m_eRoom;
}

void Application::Exit()
{
    CCtrl::App::Exit();
}
		
bool Application::Init()
{
    if ( !CCtrl::App::Init() )
    {
        return false;
    }

    // Create world
    m_spWorld = WorldPtr( new World() );

    // Register delegates
//    m_spWorld->RegisterDelegate( WORLD_EVENT_GAME_STARTED, m_spDelegateGameStarted );
//    m_spWorld->RegisterDelegate( WORLD_EVENT_GAME_LOST, m_spDelegateGameLost );
//    m_spWorld->RegisterDelegate( WORLD_EVENT_GAME_WON, m_spDelegateGameWon );

    // Load database
    //Database::GetInstance()->LoadFromXML( CSystem::FilePtr( new CSystem::File( "data/database/madtv.xml" ) ) );

    return true;
}

void Application::Deinit()
{
    // Close windows
    SetActiveRoom( ROOM_NONE );
    m_spInfoWindow->Close();
    m_spInfoWindow.reset();
    SAFE_CALL( m_spNavWindow )->Close();

    // Remove delegates
//    m_spWorld->RemoveDelegate( WORLD_EVENT_GAME_STARTED, m_spDelegateGameStarted );
//    m_spWorld->RemoveDelegate( WORLD_EVENT_GAME_LOST, m_spDelegateGameLost );
//    m_spWorld->RemoveDelegate( WORLD_EVENT_GAME_WON, m_spDelegateGameWon );

    // Remove world
    m_spWorld.reset();

    // Destroy singletons
    AdvertTooltipController::DestroyTooltip();
    BroadcastTooltipController::DestroyTooltip();
    StatsRatingTooltipController::DestroyTooltip();
    CFoundation::EnumConverter< Country >::DestroyInstance();
    CFoundation::EnumConverter< InputCommand >::DestroyInstance();
    InputTranslator::DestroyInstance();
//    Database::DestroyInstance();

    CCtrl::App::Deinit();
}

void Application::CreateStartWindows()
{    
    // Create info window
    m_spInfoWindow = CCtrl::WindowController::Create< InfoWindow >();
//    m_spInfoWindow->Init( m_spWorld );

    // Load game
    CFoundation::Serializer serializer( CFoundation::Serializer::MODE_LOAD );
    if ( serializer.LoadFile( "LastGame.sav" ) )
    {
        m_spWorld->Serialize( serializer );
        if ( m_spWorld->GetGame() != NULL )
        {
            OnGameStarted( m_spWorld->GetGame() );
        }
        else
        {
            // Create main menu window
            SetActiveRoom( ROOM_MAIN_MENU );
        }
    }
    else
    {
        // Create main menu window
        SetActiveRoom( ROOM_MAIN_MENU );
    }
}
        
CFoundation::Vector2Du Application::GetDimensions() const
{
    return CFoundation::Vector2Du( 640, 450 );
}

CGui::GuiStylePtr Application::GetDefaultStyle() const
{
    return CGui::GuiStylePtr( new CGui::GuiStyle( "data/ctrls/styles/gui_style.xml" ) );
}

void Application::Update( Float32 _f32TimeDifference )
{
    // Update world
    m_spWorld->Update( _f32TimeDifference );
}

const CFoundation::String Application::GetWindowCaption() const
{
	return "CMadTV";
}
        
PlayerPtr Application::GetActivePlayer() const
{
    GamePtr spGame = m_spWorld->GetGame();
    return spGame != NULL ? spGame->GetPlayer( m_u32ActivePlayer ) : PlayerPtr();
}
   
void Application::OnGameStarted( GamePtr _spGame )
{
    // Set active player
    //m_spInfoWindow->SetPlayer( GetActivePlayer() );

    // Create navigation window
    if ( m_spNavWindow == NULL )
    {
        m_spNavWindow = CCtrl::WindowController::Create< NavigationWindow >();
    }

    // Go to boss
    SetActiveRoom( ROOM_BOSS );
}

void Application::OnGameLost( GamePtr _spGame )
{
    OnGameEnded( _spGame, false );
}

void Application::OnGameWon( GamePtr _spGame )
{
    OnGameEnded( _spGame, true );
}
        
void Application::OnGameEnded( GamePtr _spGame, bool _bWon )
{
    // Set active player
//    m_spInfoWindow->SetPlayer( PlayerPtr() );

    // Remove navigation window
    if ( m_spNavWindow != NULL )
    {
        m_spNavWindow->Close();
        m_spNavWindow.reset();
    }

    // Leave room
    SetActiveRoom( ROOM_NONE );

    // Open game over window
    SharedPtr< GameOverWindow > spGameOverWindow = CCtrl::WindowController::Create< GameOverWindow >();
    spGameOverWindow->Init( _spGame );
    m_spMainWindow = spGameOverWindow;
}
