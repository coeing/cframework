#include "cmadtv/Windows/MainMenuWindow.h"

#include "cgui/Gui/EditBox.h"

#include "cmadtv/Application/Application.h"
#include "cmadtv/Data/Game/GameSettings.h"
#include "cmadtv/Data/World/World.h"

using namespace CMadTV;

CFoundation::String MainMenuWindow::GetXMLFileName()
{
    return "data/windows/MainMenuWindow.xml";
}

MainMenuWindow::MainMenuWindow()
{
    m_spDelegateClickedStart = CGui::ObjectInputEventDelegate0Ptr( new CGui::ObjectInputEventDelegate0( this, &MainMenuWindow::OnStart ) );
    m_spDelegateClickedLoad = CGui::ObjectInputEventDelegate0Ptr( new CGui::ObjectInputEventDelegate0( this, &MainMenuWindow::OnLoad ) );
    m_spDelegateClickedExit = CGui::ObjectInputEventDelegate0Ptr( new CGui::ObjectInputEventDelegate0( this, &MainMenuWindow::OnExit ) );
    m_spDelegateChangedPlayerSelection = CCtrl::ButtonGroupButtonEventDelegate3Ptr( new CCtrl::ButtonGroupButtonEventDelegate3( this, &MainMenuWindow::OnChangedPlayerSelection ) );
}

MainMenuWindow::~MainMenuWindow()
{
}

void MainMenuWindow::InitWindow()
{    
    // Create button group
    m_spPlayerButtons = CCtrl::ButtonGroupPtr( new CCtrl::ButtonGroup() );

    // Register delegate
    m_spPlayerButtons->RegisterDelegate( CCtrl::BUTTON_GROUP_EVENT_SELECTION_CHANGED, m_spDelegateChangedPlayerSelection );

    // Get player buttons
    CGui::ButtonPtr spBtPlayer1 = GetButton( "BtPlayer1" );
    CGui::ButtonPtr spBtPlayer2 = GetButton( "BtPlayer2" );
    CGui::ButtonPtr spBtPlayer3 = GetButton( "BtPlayer3" );

    // Add to button group
    if ( spBtPlayer1 != NULL )
    {
        m_spPlayerButtons->AddButton( spBtPlayer1 );
    }
    if ( spBtPlayer2 != NULL )
    {
        m_spPlayerButtons->AddButton( spBtPlayer2 );
    }
    if ( spBtPlayer3 != NULL )
    {
        m_spPlayerButtons->AddButton( spBtPlayer3 );
    }

    // Create button group
    m_spDifficultyButtons = CCtrl::ButtonGroupPtr( new CCtrl::ButtonGroup() );

    // Get difficulty buttons
    CGui::ButtonPtr spBtEasy = GetButton( "BtEasy" );
    CGui::ButtonPtr spBtNormal = GetButton( "BtNormal" );
    CGui::ButtonPtr spBtDifficult = GetButton( "BtDifficult" );

    // Add to button group
    if ( spBtEasy != NULL )
    {
        m_spDifficultyButtons->AddButton( spBtEasy );
    }
    if ( spBtNormal != NULL )
    {
        m_spDifficultyButtons->AddButton( spBtNormal );
    }
    if ( spBtDifficult != NULL )
    {
        m_spDifficultyButtons->AddButton( spBtDifficult );
    }

    // Load buttons
    m_spBtStart = GetButton( "BtStart" );
    m_spBtStart->RegisterDelegate( CGui::INPUT_EVENT_PUSHED, m_spDelegateClickedStart );

    m_spBtLoad = GetButton( "BtLoad" );
    m_spBtLoad->SetDisabled( true );
    m_spBtLoad->RegisterDelegate( CGui::INPUT_EVENT_PUSHED, m_spDelegateClickedLoad );

    m_spBtExit = GetButton( "BtExit" );
    m_spBtExit->RegisterDelegate( CGui::INPUT_EVENT_PUSHED, m_spDelegateClickedExit );
}
        
void MainMenuWindow::DeinitWindow()
{
    // Remove delegate
    m_spPlayerButtons->RemoveDelegate( CCtrl::BUTTON_GROUP_EVENT_SELECTION_CHANGED, m_spDelegateChangedPlayerSelection );

    // Remove all buttons from button groups
    m_spPlayerButtons->RemoveAllButtons();
    m_spDifficultyButtons->RemoveAllButtons();

    // Remove delegates
    m_spBtStart->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedStart );
    m_spBtLoad->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedLoad );
    m_spBtExit->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedExit );

    CCtrl::WindowController::DeinitWindow();
}
        
void MainMenuWindow::SetWorld( WorldPtr _spWorld )
{
    m_spWorld = _spWorld;
}
        
void MainMenuWindow::OnStart()
{
    // Create game settings
    GameSettings settings;
    Unsigned32 u32NumPlayers = m_spPlayerButtons->GetNumButtons();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPlayers; ++u32Idx )
    {
        GameSettings::PlayerInfo info;

        // Get name
        CGui::EditBoxPtr spEbPlayerName = GetEditBox( CFoundation::String::Build( "EbPlayer%d", u32Idx + 1 ) );
        info.m_strName = spEbPlayerName->GetText();

        // Get color
        info.m_colColor = spEbPlayerName->GetBgColor( CGui::STATE_NORMAL );

        // Check if human
        CGui::ButtonPtr spPlayerButton = m_spPlayerButtons->GetButton( u32Idx );
        info.m_bHuman = spPlayerButton->IsActive();

        // Add player
        settings.AddPlayer( info );
    }

    // Start game
    m_spWorld->StartGame( settings );
}
        
void MainMenuWindow::OnLoad()
{
    Application::GetInstance().SetActiveRoom( ROOM_LOAD );
}

void MainMenuWindow::OnExit()
{
    Application::GetInstance().Exit();
}

void MainMenuWindow::OnChangedPlayerSelection( CCtrl::ButtonGroupPtr _spButtonGroup, CGui::ButtonPtr _spNewActiveButton, CGui::ButtonPtr _spOldActiveButton )
{
    if ( _spNewActiveButton != NULL )
    {
        _spNewActiveButton->SetText( "Spieler" );
    }
    if ( _spOldActiveButton != NULL )
    {
        _spOldActiveButton->SetText( "Computer" );
    }
}


