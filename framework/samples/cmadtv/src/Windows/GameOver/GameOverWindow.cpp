#include "cmadtv/Windows/GameOver/GameOverWindow.h"

#include "cgui/Gui/Button.h"
#include "cgui/Gui/EditBox.h"
#include "cgui/Gui/ListBox.h"
#include "cgui/Gui/TextBox.h"

#include "cmadtv/Application/Application.h"
#include "cmadtv/Data/Game/GameSettings.h"
#include "cmadtv/Data/Game/Mission.h"
#include "cmadtv/Data/Game/Conditions/MissionConditionAdvertsSent.h"

using namespace CMadTV;
 
CFoundation::String GameOverWindow::GetXMLFileName()
{
    return "data/windows/GameOverWindow.xml";
}

GameOverWindow::GameOverWindow()
{
    // Create delegates
    m_spDelegateClickedRestart = CGui::ObjectInputEventDelegate0Ptr( new CGui::ObjectInputEventDelegate0( this, &GameOverWindow::OnRestart ) );
    m_spDelegateClickedMainMenu = CGui::ObjectInputEventDelegate0Ptr( new CGui::ObjectInputEventDelegate0( this, &GameOverWindow::OnMainMenu ) );
    m_spDelegateClickedExit = CGui::ObjectInputEventDelegate0Ptr( new CGui::ObjectInputEventDelegate0( this, &GameOverWindow::OnExit ) );
}

GameOverWindow::~GameOverWindow()
{
}

void GameOverWindow::InitWindow()
{    
    // Get point listbox
    m_spLbPoints = GetListBox( "LbPoints" );

    // Load buttons
    m_spBtRestart = GetButton( "BtRestart" );
    m_spBtRestart->RegisterDelegate( CGui::INPUT_EVENT_PUSHED, m_spDelegateClickedRestart );

    m_spBtMainMenu = GetButton( "BtMainMenu" );
    m_spBtMainMenu->RegisterDelegate( CGui::INPUT_EVENT_PUSHED, m_spDelegateClickedMainMenu );

    m_spBtExit = GetButton( "BtExit" );
    m_spBtExit->RegisterDelegate( CGui::INPUT_EVENT_PUSHED, m_spDelegateClickedExit );

    if ( m_spLbPoints != NULL )
    {
        // Fill points
        m_spLbPoints->AddColumnData( "Mission" );
        m_spLbPoints->AddColumnData( 3000 );
        m_spLbPoints->AddRow();
        
        m_spLbPoints->AddColumnData( "Time" );
        m_spLbPoints->AddColumnData( 6000 );
        m_spLbPoints->AddRow();
        
        m_spLbPoints->AddColumnData( "Money" );
        m_spLbPoints->AddColumnData( 1000 );
        m_spLbPoints->AddRow();
        
        m_spLbPoints->AddColumnData( "Difficulty" );
        m_spLbPoints->AddColumnData( "x3" );
        m_spLbPoints->AddRow();

        m_spLbPoints->AddRow();       
        
        m_spLbPoints->AddColumnData( "Points" );
        m_spLbPoints->AddColumnData( 1000 );
        m_spLbPoints->AddRow();
    }
}
        
void GameOverWindow::DeinitWindow()
{
    // Remove delegates
    m_spBtRestart->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedRestart );
    m_spBtMainMenu->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedMainMenu );
    m_spBtExit->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedExit );

    CCtrl::WindowController::DeinitWindow();
}
        
void GameOverWindow::Init( GamePtr _spGame )
{
    m_spGame = _spGame;
}
        
void GameOverWindow::OnRestart()
{
    // Get game settings
    //const GameSettings& settings = m_spWorld->GetGame()->GetSettings();

    // Start new game
    //m_spWorld->StartGame( settings );
}
        
void GameOverWindow::OnMainMenu()
{
    Application::GetInstance().SetActiveRoom( ROOM_MAIN_MENU );
}

void GameOverWindow::OnExit()
{
    Application::GetInstance().Exit();
}
        