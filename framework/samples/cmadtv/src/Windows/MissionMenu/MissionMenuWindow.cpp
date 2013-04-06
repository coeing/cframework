#include "cmadtv/Windows/MissionMenu/MissionMenuWindow.h"

#include "cgui/Gui/Button.h"
#include "cgui/Gui/EditBox.h"
#include "cgui/Gui/ListBox.h"
#include "cgui/Gui/TextBox.h"

#include "cmadtv/Application/Application.h"
#include "cmadtv/Data/Game/GameSettings.h"
#include "cmadtv/Data/Game/Mission.h"
#include "cmadtv/Data/Game/Conditions/MissionConditionAdvertsSent.h"
#include "cmadtv/Data/World/World.h"

using namespace CMadTV;

CFoundation::String MissionMenuWindow::GetXMLFileName()
{
    return "data/windows/MissionMenuWindow.xml";
}

MissionMenuWindow::MissionMenuWindow()
{
    // Create delegates
    m_spDelegateClickedStart = CGui::ObjectInputEventDelegate0Ptr( new CGui::ObjectInputEventDelegate0( this, &MissionMenuWindow::OnStart ) );
    m_spDelegateClickedLoad = CGui::ObjectInputEventDelegate0Ptr( new CGui::ObjectInputEventDelegate0( this, &MissionMenuWindow::OnLoad ) );
    m_spDelegateClickedExit = CGui::ObjectInputEventDelegate0Ptr( new CGui::ObjectInputEventDelegate0( this, &MissionMenuWindow::OnExit ) );
    m_spDelegateSelectedMission = CGui::ListBoxEventDelegate1Ptr( new CGui::ListBoxEventDelegate1( this, &MissionMenuWindow::OnSelectedMission ) );

    // Create missions

    // Set mission
    MissionPtr spMission( new Mission() );

    // Create conditions
    SharedPtr< MissionConditionAdvertsSent > spCondition1( new MissionConditionAdvertsSent() );
    spCondition1->SetNumAdverts( 3 );
    spCondition1->SetAdvertSuccess( true );

    spMission->AddCondition( spCondition1, true );

    m_aspMissions.push_back( spMission );
}

MissionMenuWindow::~MissionMenuWindow()
{
}

void MissionMenuWindow::InitWindow()
{    
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

    // Get mission listbox
    m_spLbMissions = GetListBox( "LbMissions" );

    // Get mission description text box
    m_spTbMissionDesc = GetTextBox( "TbMissionDesc" );

    // Load buttons
    m_spBtStart = GetButton( "BtStart" );
    m_spBtStart->RegisterDelegate( CGui::INPUT_EVENT_PUSHED, m_spDelegateClickedStart );

    m_spBtLoad = GetButton( "BtLoad" );
    m_spBtLoad->SetDisabled( true );
    m_spBtLoad->RegisterDelegate( CGui::INPUT_EVENT_PUSHED, m_spDelegateClickedLoad );

    m_spBtExit = GetButton( "BtExit" );
    m_spBtExit->RegisterDelegate( CGui::INPUT_EVENT_PUSHED, m_spDelegateClickedExit );

    if ( m_spLbMissions != NULL )
    {
        m_spLbMissions->RegisterDelegate( CGui::EVENT_LISTBOX_SELECTED_CELL, m_spDelegateSelectedMission );

        // Fill missions
        Unsigned32 u32NumMissions = m_aspMissions.size();
        for ( Unsigned32 u32Idx = 0; u32Idx < u32NumMissions; ++u32Idx )
        {
            MissionPtr spMission = m_aspMissions[ u32Idx ];

            m_spLbMissions->AddColumnData( CFoundation::String::Build( "Mission %d", u32Idx + 1 ) );
            m_spLbMissions->AddRow();
        }
    }
}
        
void MissionMenuWindow::DeinitWindow()
{
    // Remove all buttons from button groups
    m_spDifficultyButtons->RemoveAllButtons();

    // Remove delegates
    m_spBtStart->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedStart );
    m_spBtLoad->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedLoad );
    m_spBtExit->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedExit );

    SAFE_CALL( m_spLbMissions )->RegisterDelegate( CGui::EVENT_LISTBOX_SELECTED_CELL, m_spDelegateSelectedMission );

    CCtrl::WindowController::DeinitWindow();
}
        
void MissionMenuWindow::Init( WorldPtr _spWorld )
{
    m_spWorld = _spWorld;
}
        
void MissionMenuWindow::OnStart()
{
    // Create game settings
    GameSettings settings;

    // Set player
    GameSettings::PlayerInfo info;
    CGui::EditBoxPtr spEbPlayerName = GetEditBox( "EbPlayer" );
    info.m_strName = spEbPlayerName->GetText();
    info.m_bHuman = true;
    info.m_colColor = spEbPlayerName->GetBgColor( CGui::STATE_NORMAL );
    settings.AddPlayer( info );

    // Get current selected mission
    if ( m_spLbMissions != NULL &&
         m_spLbMissions->GetSelectedRow() >= 0 )
    {
        settings.SetMission( m_aspMissions[ m_spLbMissions->GetSelectedRow() ] );
    }

    // Start game
    m_spWorld->StartGame( settings );
}
        
void MissionMenuWindow::OnLoad()
{
    Application::GetInstance().SetActiveRoom( ROOM_LOAD );
}

void MissionMenuWindow::OnExit()
{
    Application::GetInstance().Exit();
}
        
void MissionMenuWindow::OnSelectedMission( Unsigned32 _u32Row )
{
    MissionPtr spMission = m_aspMissions[ _u32Row ];

    // Update description
    const CFoundation::String& strDescription = spMission->GetDescription();
    m_spTbMissionDesc->SetText( strDescription );
}

void MissionMenuWindow::OnChangedPlayerSelection( CCtrl::ButtonGroupPtr _spButtonGroup, CGui::ButtonPtr _spNewActiveButton, CGui::ButtonPtr _spOldActiveButton )
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


