#include "cmadtv/Windows/WindowManager.h"

#include "cmadtv/Data/Room.h"

#include "cmadtv/Windows/InfoWindow.h"
#include "cmadtv/Windows/NavigationWindow.h"

#include "cmadtv/Windows/AdvertDealerWindow.h"
#include "cmadtv/Windows/Betty/BettyWindow.h"
#include "cmadtv/Windows/Boss/BossWindow.h"
#include "cmadtv/Windows/BroadcastDealerWindow.h"
#include "cmadtv/Windows/GameOver/GameOverWindow.h"
#include "cmadtv/Windows/LoadSaveWindow.h"
#include "cmadtv/Windows/MainMenuWindow.h"
#include "cmadtv/Windows/Library/LibraryWindow.h"
#include "cmadtv/Windows/LoginMenu/LoginMenuWindow.h"
#include "cmadtv/Windows/MissionMenu/MissionMenuWindow.h"
#include "cmadtv/Windows/NewsStudio/NewsStudioWindow.h"
#include "cmadtv/Windows/Spectators/SpectatorsWindow.h"
#include "cmadtv/Windows/Supermarket/SupermarketPresentsWindow.h"
#include "cmadtv/Windows/PlanningWindow.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// WindowManager
///
////////////////////////////////////////////////////////////////////////////////////////////
        
WindowManager::WindowManager()
:   m_eRoom( ROOM_NONE )
{
}

WindowManager::~WindowManager()
{
}
        
void WindowManager::Init()
{
    // Create info window
    m_spInfoWindow = CCtrl::WindowController::Create< InfoWindow >();
    m_spInfoWindow->SetContext( m_spContext );

    // Create navigation window
    m_spNavWindow = CCtrl::WindowController::Create< NavigationWindow >();
    m_spNavWindow->SetVisible( false );
    m_spDelegateRoomClicked = m_spNavWindow->RegisterDelegate( EVENT_NAVIGATION_WINDOW_ROOM_CLICKED, CFoundation::CreateDelegate( this, &WindowManager::OnRoomClicked ) );
}

void WindowManager::Deinit()
{
    // Close windows
    if ( m_spInfoWindow != NULL )
    {
        m_spInfoWindow->Close();
        m_spInfoWindow.reset();
    }

    if ( m_spNavWindow != NULL )
    {
        DISCONNECT_DELEGATE_CONNECTION( m_spDelegateRoomClicked );
        m_spNavWindow->Close();
        m_spNavWindow.reset();
    }

    SetActiveRoom( ROOM_NONE );
}
        
void WindowManager::SetContext( PlayerContextPtr _spContext )
{
    if ( _spContext == m_spContext )
    {
        return;
    }

    m_spContext = _spContext;

    // Setup members
    SAFE_CALL( m_spInfoWindow )->SetContext( m_spContext );
    SAFE_CALL( m_spWindow )->SetContext( m_spContext );
}
       
void WindowManager::OnContextChanged()
{
    SAFE_CALL( m_spInfoWindow )->OnContextChanged();
    SAFE_CALL( m_spWindow )->OnContextChanged();
}

void WindowManager::SetActiveRoom( Room _eRoom )
{
    if ( m_eRoom == _eRoom )
    {
        return;
    }

    // Close active window
    if ( m_spWindow != NULL )
    {
        m_spWindow->Close();
        m_spWindow->SetContext( PlayerContextPtr() );
        m_spWindow.reset();
    }

    // Set new active window
    WindowPtr spNewWindow;
    switch ( _eRoom )
    {
    case ROOM_NONE:
        {
            // No room
        }
        break;
    case ROOM_MAIN_MENU:
        {
            spNewWindow = CCtrl::WindowController::Create< LoginMenuWindow >();
        }
        break;
    case ROOM_GAME_OVER:
        {            
            spNewWindow = CCtrl::WindowController::Create< GameOverWindow >();
        }
        break;
    case ROOM_BOSS:
        {
            spNewWindow = CCtrl::WindowController::Create< BossWindow >();
        }
        break;
    case ROOM_PLANNING:
        {
           spNewWindow = CCtrl::WindowController::Create< PlanningWindow >();
        }
        break;
    case ROOM_SPECTATORS:
        {
            spNewWindow = CCtrl::WindowController::Create< SpectatorsWindow >();
        }
        break;
    case ROOM_LOAD:
        {
            SharedPtr< LoadSaveWindow > spLoadSaveWindow = CCtrl::WindowController::Create< LoadSaveWindow >();
            spLoadSaveWindow->Init( LoadSaveWindow::MODE_LOAD, m_eRoom );
            spNewWindow = spLoadSaveWindow;
        }
        break;
    case ROOM_SAVE:
        {
            SharedPtr< LoadSaveWindow > spLoadSaveWindow = CCtrl::WindowController::Create< LoadSaveWindow >();
            spLoadSaveWindow->Init( LoadSaveWindow::MODE_SAVE, m_eRoom );
            spNewWindow = spLoadSaveWindow;
        }
        break;
    case ROOM_LOAD_SAVE:
        {
            SharedPtr< LoadSaveWindow > spLoadSaveWindow = CCtrl::WindowController::Create< LoadSaveWindow >();
            spLoadSaveWindow->Init( LoadSaveWindow::MODE_NONE, m_eRoom );
            spNewWindow = spLoadSaveWindow;
        }
        break;
    case ROOM_BROADCAST_DEALER:
        {
            // Fill up broadcasts
//            m_spWorld->FillBroadcastDealer();

            spNewWindow = CCtrl::WindowController::Create< BroadcastDealerWindow >();
        }
        break;
    case ROOM_ADVERT_DEALER:
        {
            // Fill up adverts
//            m_spWorld->FillAdvertDealer();

            spNewWindow = CCtrl::WindowController::Create< AdvertDealerWindow >();
        }
        break;
    case ROOM_NEWS_STUDIO:
        {
            spNewWindow = CCtrl::WindowController::Create< NewsStudioWindow >();
        }
        break;
    case ROOM_LIBRARY:
        {
            spNewWindow = CCtrl::WindowController::Create< LibraryWindow >();
        }
        break;
    case ROOM_SUPERMARKET:
        {
            spNewWindow = CCtrl::WindowController::Create< SupermarketPresentsWindow >();
        }
        break;
    case ROOM_BETTY:
        {
            spNewWindow = CCtrl::WindowController::Create< BettyWindow >();
        }
        break;
    default:
        {
            CF_WARN( "Invalid room." );
        }
        break;
    }

    // Init new window
    if ( spNewWindow != NULL )
    {
        spNewWindow->SetContext( m_spContext );
    }
    m_spWindow = spNewWindow;

    m_eRoom = _eRoom;
}

Room WindowManager::GetActiveRoom() const
{
    return m_eRoom;
}

void WindowManager::ShowNavigationWindow( bool _bShow )
{
    SAFE_CALL( m_spNavWindow )->SetVisible( _bShow );
}
       
void WindowManager::OnRoomClicked( Room _eRoom )
{
    SetActiveRoom( _eRoom );
}

