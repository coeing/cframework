#include "cetris/windows/MainMenu.h"

#include "cgui/Gui/Button.h"

#include "cctrl/ScreenClosedMsg.h"

#include "cetris/CetrisApp.h"
#include "cetris/BlockManager.h"
#include "cetris/OneGame.h"

#include "cetris/windows/GameMain.h"
#include "cetris/windows/HighscoreScreen.h"

using namespace Cetris;

MainMenu::MainMenu()
{
    m_spDelegateClickedButton = CGui::ObjectInputEventDelegate1Ptr( new CGui::ObjectInputEventDelegate1( this, &MainMenu::OnClickedButton ) );
}

const CFoundation::String MainMenu::GetXMLFileName()
{
	return "data/windows/MainMenu.xml";
}

void MainMenu::InitWindow()
{
	m_pBtStart		= GetButton( "BtStart" );
    if ( m_pBtStart != NULL )
    {
        m_pBtStart->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedButton );
    }
	m_pBtHighscores	= GetButton( "BtHighscores" );
    if ( m_pBtHighscores != NULL )
    {
        m_pBtHighscores->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedButton );
    }
	m_pBtExit		= GetButton( "BtExit" );
    if ( m_pBtExit != NULL )
    {
        m_pBtExit->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedButton );
    }
}

void MainMenu::OnClickedButton( CGui::GuiObjectPtr _spObject )
{
    if ( _spObject == m_pBtStart )
	{
		m_pGameMainScreen = CreateWindow< GameMain >( GetSharedPtr() );
	}
    else if ( _spObject == m_pBtHighscores )
	{
		CreateWindow< HighscoreScreen >( GetSharedPtr() );
	}
    else if ( _spObject == m_pBtExit )
	{
		CetrisApp::GetInstance().Exit();
	}
}
		
void MainMenu::OnCloseScreen( const ScreenClosedMsg& clMessage )
{
	if( clMessage.IsReceiver( GetSharedPtr() ) )
	{
		if( m_pGameMainScreen != NULL &&
			clMessage.GetScreen() == m_pGameMainScreen )
		{
			SharedPtr< HighscoreScreen > spHighscoreScreen = CreateWindow<HighscoreScreen>( GetSharedPtr() );
			
			// check if new highscore
			const OneGame *pRunningGame = Game::GetInstance().GetRunningGame();
			if( pRunningGame != NULL )
			{
				const HighscoreList &clHighscoreList = Game::GetInstance().GetHighscoreList();

				if( clHighscoreList.GetNumHighscores() < 10 ||
					clHighscoreList.GetPoints( 9 ) < pRunningGame->GetPoints() )
				{
					spHighscoreScreen->NewHighscore( true );
				}
			}
		}
	}
}
