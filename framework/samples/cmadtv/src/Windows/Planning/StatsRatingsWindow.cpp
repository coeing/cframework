#include "cmadtv/Windows/Planning/StatsRatingsWindow.h"

#include <math.h>

#include "cfoundation/Enum/EnumIncrement.h"
#include "cfoundation/Math/Utils.h"

#include "csystem/Input/Input.h"

#include "cgui/Gui/Button.h"
#include "cgui/Gui/TextBox.h"
#include "cgui/Tooltip/TooltipSystem.h"

#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Player/DayStat.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerCompSpectators.h"
#include "cmadtv/Data/Player/PlayerCompStats.h"
#include "cmadtv/Data/Player/PlayerContext.h"
#include "cmadtv/Data/Game/Game.h"

#include "cmadtv/Tooltip/StatsRatingTooltipController.h"
#include "cmadtv/Windows/Planning/StatsImageWindow.h"

using namespace CMadTV;
        
CFoundation::String StatsRatingsWindow::GetXMLFileName()
{
    return "data/windows/StatsRatingsWindow.xml";
}

StatsRatingsWindow::StatsRatingsWindow()
:   m_u32Player( MAX_U32 ),
    m_ePeriod( PERIOD_NONE ),
    m_u32MaxBarHeight( 0 )
{
    m_spDelegatePeriodPushed = CGui::StateEventDelegate0Ptr( new CGui::StateEventDelegate0( this, &StatsRatingsWindow::OnPeriodPushed ) );
    m_spDelegateImagePushed = CGui::StateEventDelegate0Ptr( new CGui::StateEventDelegate0( this, &StatsRatingsWindow::OnImagePushed ) );
    m_spDelegateChangedPlayerSelection = CCtrl::ButtonGroupButtonEventDelegate3Ptr( new CCtrl::ButtonGroupButtonEventDelegate3( this, &StatsRatingsWindow::OnChangedPlayerSelection ) );
}

StatsRatingsWindow::~StatsRatingsWindow()
{
}

void StatsRatingsWindow::InitWindow()
{      
    // Create button group
    m_spPlayerButtons = CCtrl::ButtonGroupPtr( new CCtrl::ButtonGroup() );

    // Register delegate
    m_spPlayerButtons->RegisterDelegate( CCtrl::BUTTON_GROUP_EVENT_SELECTION_CHANGED, m_spDelegateChangedPlayerSelection );

    // Get player buttons
    CGui::ButtonPtr spBtPlayer1 = GetButton( "BtPlayer1" );
    m_mapButtonPlayers[ spBtPlayer1 ] = 0;
    CGui::ButtonPtr spBtPlayer2 = GetButton( "BtPlayer2" );
    m_mapButtonPlayers[ spBtPlayer2 ] = 1;
    CGui::ButtonPtr spBtPlayer3 = GetButton( "BtPlayer3" );
    m_mapButtonPlayers[ spBtPlayer3 ] = 2;

    // Add to button group
    m_spPlayerButtons->AddButton( spBtPlayer1 );
    m_spPlayerButtons->AddButton( spBtPlayer2 );
    m_spPlayerButtons->AddButton( spBtPlayer3 );

    // Get axis labels
    for ( Unsigned32 u32Idx = 0; u32Idx < NUM_AXIS_LABELS; ++u32Idx )
    {
        m_aspAxisLabels.push_back( GetTextBox( CFoundation::String::Build( "TbAxisLabel%d", u32Idx + 1 ) ) );
    }

    // Get bars
    for ( Unsigned32 u32Idx = HOUR_BEGIN; u32Idx < HOUR_END; ++u32Idx )
    {
        m_mapBars[ u32Idx ] = GetPanel( CFoundation::String::Build( "PnlBar%d", u32Idx ) );
    }

    // Get initial bar height
    CGui::PanelPtr spPnlBarPanel = GetPanel( "PnlBar1" );
    CGui::GuiObjectPtr spPnlBar = spPnlBarPanel != NULL ? spPnlBarPanel->FindChild( "PnlBar" ) : CGui::GuiObjectPtr();
    if ( spPnlBar != NULL )
    {
        m_u32MaxBarHeight = spPnlBar->GetHeight();
    }    

    m_spBtPeriod = GetButton( "BtPeriod" );
    m_spBtPeriod->RegisterDelegate( CGui::INPUT_EVENT_PUSHED, m_spDelegatePeriodPushed );

    m_spBtImage = GetButton( "BtRatings" );
    m_spBtImage->RegisterDelegate( CGui::INPUT_EVENT_PUSHED, m_spDelegateImagePushed );

    WindowController::InitWindow();
}
        
void StatsRatingsWindow::DeinitWindow()
{
    // Remove delegates
    m_spPlayerButtons->RemoveDelegate( CCtrl::BUTTON_GROUP_EVENT_SELECTION_CHANGED, m_spDelegateChangedPlayerSelection );
    m_spBtPeriod->RemoveDelegate( CGui::INPUT_EVENT_PUSHED, m_spDelegatePeriodPushed );
    m_spBtImage->RemoveDelegate( CGui::INPUT_EVENT_PUSHED, m_spDelegateImagePushed );

    WindowController::DeinitWindow();
}

void StatsRatingsWindow::ProcessInput( const CSystem::InputState& _State )
{
    if ( _State.WasPressed( CSystem::MOUSE_RIGHT ) ||
         _State.WasPressed( CSystem::KEY_ESC ) )
    {
        Close();
    }
}
        
void StatsRatingsWindow::OnContextChanged()
{
    SetGame( m_spContext != NULL ? m_spContext->GetGame() : GamePtr() );
    SetPlayer( m_spContext != NULL ? m_spContext->GetPlayerIdx() : MAX_U32 );
}
        
void StatsRatingsWindow::SetGame( GamePtr _spGame )
{
    if ( _spGame == m_spGame )
    {
        return;
    }

    m_spGame = _spGame;

    if ( m_spGame != NULL )
    {
        // Set button texts
        Unsigned32 u32NumButtons = m_spPlayerButtons->GetNumButtons();
        for ( Unsigned32 u32Idx = 0; u32Idx < u32NumButtons; ++u32Idx )
        {
            CGui::ButtonPtr spButton = m_spPlayerButtons->GetButton( u32Idx );

            PlayerPtr spPlayer = m_spGame->GetPlayer( u32Idx );
            spButton->SetVisible( spPlayer != NULL && spPlayer->GetImage() > 0 );
            if ( spPlayer != NULL )
            {
                // TODO Set station name instead of player name
                spButton->SetText( spPlayer->GetName() );
            }
        }
    }
    
    // Update player
    UpdatePlayer();
}
        
void StatsRatingsWindow::SetPlayer( Unsigned32 _u32Player )
{
    if ( _u32Player == m_u32Player )
    {
        return;
    }
    
    m_u32Player = _u32Player;

    // Activate correct player button
    m_spPlayerButtons->SetActiveButton( m_u32Player );

    // Update player
    UpdatePlayer();
}
      
void StatsRatingsWindow::UpdatePlayer()
{
    // Get player pointer
    SetPlayer( m_spGame != NULL ? m_spGame->GetPlayer( m_u32Player ) : PlayerPtr() );
}
        
void StatsRatingsWindow::SetPlayer( PlayerPtr _spPlayer )
{
    if ( _spPlayer == m_spPlayer )
    {
        return;
    }

    m_spPlayer = _spPlayer;

    // Set period
    SetPeriod( PERIOD_TODAY );

    // Update stats
    UpdateStats();
}

void StatsRatingsWindow::SetPeriod( Period _ePeriod )
{
    if ( _ePeriod == m_ePeriod )
    {
        return;
    }

    m_ePeriod = _ePeriod;

    // Set button text
    CFoundation::String strText;
    switch ( m_ePeriod )
    {
    case PERIOD_TODAY:
        {
            strText = "Today";
        }
        break;
    case PERIOD_YESTERDAY:
        {
            strText = "Yesterday";
        }
        break;
    case PERIOD_WEEK:
        {
            strText = "Week";
        }
        break;
    }
    SAFE_CALL( m_spBtPeriod )->SetText( strText );

    // Update stats
    UpdateStats();
}
        
void StatsRatingsWindow::UpdateStats()
{
    if ( m_spGame == NULL ||
         m_spPlayer == NULL )
    {
        return;
    }

    // Get maximum value
    Unsigned32 u32MaxSpectators = m_spPlayer->GetCompSpectators()->GetMaxNumSpectators( m_spContext->GetDatabase()->GetMap() );
    Unsigned32 u32Step = u32MaxSpectators / 5;
    u32Step = std::max( static_cast< Unsigned32 >( 1000000 ), u32Step );
    Unsigned32 u32NumDigits = CFoundation::Math::GetNumDigits( u32Step );
    Unsigned32 u32Multiplier = static_cast< Unsigned32 >( pow( 10.0f, static_cast< int >( u32NumDigits - 1 ) ) );
    u32Step /= u32Multiplier;
    u32Step *= u32Multiplier;
    Unsigned32 u32MaxValue = u32Step * 5;

    // Set axis labels
    Unsigned32 u32NumSteps = m_aspAxisLabels.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumSteps; ++u32Idx )
    {
        Unsigned32 u32Value = u32Step * ( u32Idx + 1 );

        // Get value in millions
        u32Value /= 1000000;

        // Set text
        m_aspAxisLabels[ u32Idx ]->SetText( CFoundation::String::Build( "%d", u32Value ) );
    }

    switch ( m_ePeriod )
    {
    case PERIOD_TODAY:
    case PERIOD_YESTERDAY:
        {
            // Check which day to show
            CFoundation::Date date = m_spGame->GetDate();

            if ( m_ePeriod == PERIOD_YESTERDAY )
            {
                date.AddDays( -1 );
            }
                
            // Get day stat
            PlayerCompStatsPtr spCompStats = m_spPlayer->GetCompStats();
            DayStatPtr spDayStat = spCompStats->GetDayStat( date );

            // Fill stats
            for ( Hour eHour = HOUR_BEGIN; eHour != HOUR_END; ++eHour )
            {
                CGui::PanelPtr spPnlBarPanel = m_mapBars[ eHour ];
                if ( spPnlBarPanel == NULL )
                {
                    continue;
                }

                // Get stat
                BlockStatPtr spBlockStat = spDayStat->GetBlockStat( eHour );

                // Setup news bar
                {
                    // Get spectators
                    Unsigned32 u32NumSpectators = spBlockStat->GetNewsSpectators();
                    Unsigned32 u32Height = m_u32MaxBarHeight * u32NumSpectators / u32MaxValue;

                    // Set bar and shadow height
                    CGui::PanelPtr spPnlBar = static_pointer_cast< CGui::Panel >( spPnlBarPanel->FindChild( "PnlBarNews" ) );
                    spPnlBar->SetVisible( true );
                    spPnlBar->SetCustomHeight( u32Height );
                    CGui::GuiObjectPtr spImgShadow = spPnlBarPanel->FindChild( "ImgShadowNews" );
                    spImgShadow->SetVisible( true );
                    spImgShadow->SetCustomHeight( u32Height );

                    // Set player color
                    CGui::GuiObjectPtr spImgColor = spPnlBar->FindChild( "ImgColor" );
                    spImgColor->SetBgColor( m_spPlayer->GetColorLight() );
                }

                // Setup broadcast bar
                {
                    // Get spectators
                    Unsigned32 u32NumSpectators = spBlockStat->GetBroadcastSpectators();
                    Unsigned32 u32Height = m_u32MaxBarHeight * u32NumSpectators / u32MaxValue;                
                    
                    // Set bar and shadow height
                    CGui::PanelPtr spPnlBar = static_pointer_cast< CGui::Panel >( spPnlBarPanel->FindChild( "PnlBar" ) );
                    spPnlBar->SetCustomHeight( u32Height );
                    CGui::GuiObjectPtr spImgShadow = spPnlBarPanel->FindChild( "ImgShadow" );
                    spImgShadow->SetCustomHeight( u32Height );

                    // Set player color
                    CGui::GuiObjectPtr spImgColor = spPnlBar->FindChild( "ImgColor" );
                    spImgColor->SetBgColor( m_spPlayer->GetColor() );
                }

                // Show/hide arrows
                CGui::GuiObjectPtr spImgArrowUp = spPnlBarPanel->FindChild( "ImgArrowUp" );
                spImgArrowUp->SetVisible( spBlockStat->GetImageEffect() == BlockStat::IMAGE_EFFECT_INCREASE );
                CGui::GuiObjectPtr spImgArrowDown = spPnlBarPanel->FindChild( "ImgArrowDown" );
                spImgArrowDown->SetVisible( spBlockStat->GetImageEffect() == BlockStat::IMAGE_EFFECT_DECREASE );

                // Set text
                CGui::TextBoxPtr spTbHour= static_pointer_cast< CGui::TextBox >( spPnlBarPanel->FindChild( "TbHour" ) );
                spTbHour->SetText( CFoundation::String::Build( "%d", 17 + eHour ) );

                // Set tooltip
                StatsRatingTooltipControllerPtr spTooltipController = StatsRatingTooltipControllerPtr( new StatsRatingTooltipController() );
                spTooltipController->SetBlockStat( spBlockStat );
                CGui::TooltipSystem::GetInstance().SetTooltipController( spTbHour, spTooltipController );
            }
        }
        break;
    case PERIOD_WEEK:
        {
            // Check which day to show
            CFoundation::Date today = m_spGame->GetDate();
            Unsigned32 u32PassedDays = m_spGame->GetDays();
                
            PlayerCompStatsPtr spCompStats = m_spPlayer->GetCompStats();

            // Fill stats
            Unsigned32 u32NumBars = m_mapBars.size();
            for ( Unsigned32 u32DaysAgo = 1; u32DaysAgo <= u32NumBars; ++u32DaysAgo )
            {
                CFoundation::Date date = today;
                Integer32 i32DaysSubstract = u32DaysAgo;
                date.AddDays( -i32DaysSubstract );

                // Get day stat
                DayStatPtr spDayStat = spCompStats->GetDayStat( date, false );

                CGui::PanelPtr spPnlBarPanel = m_mapBars[ u32NumBars - u32DaysAgo + 1 ];
                if ( spPnlBarPanel == NULL )
                {
                    continue;
                }

                // Hide news bar
                {
                    CGui::PanelPtr spPnlBar = static_pointer_cast< CGui::Panel >( spPnlBarPanel->FindChild( "PnlBarNews" ) );
                    spPnlBar->SetVisible( false );
                    CGui::GuiObjectPtr spImgShadow = spPnlBarPanel->FindChild( "ImgShadowNews" );
                    spImgShadow->SetVisible( false );
                }
                // Get spectators
                Unsigned32 u32NumSpectators = spDayStat != NULL ? spDayStat->GetAvgBroadcastSpectators() : 0;
                Unsigned32 u32Height = m_u32MaxBarHeight * u32NumSpectators / u32MaxValue;
                
                // Set bar and shadow height
                CGui::PanelPtr spPnlBar = static_pointer_cast< CGui::Panel >( spPnlBarPanel->FindChild( "PnlBar" ) );
                spPnlBar->SetCustomHeight( u32Height );
                CGui::GuiObjectPtr spImgShadow = spPnlBarPanel->FindChild( "ImgShadow" );
                spImgShadow->SetCustomHeight( u32Height );

                // Set player color
                CGui::GuiObjectPtr spImgColor = spPnlBar->FindChild( "ImgColor" );
                spImgColor->SetBgColor( m_spPlayer->GetColor() );

                // Hide arrows
                CGui::GuiObjectPtr spImgArrowUp = spPnlBarPanel->FindChild( "ImgArrowUp" );
                spImgArrowUp->SetVisible( false );
                CGui::GuiObjectPtr spImgArrowDown = spPnlBarPanel->FindChild( "ImgArrowDown" );
                spImgArrowDown->SetVisible( false );

                // Set text
                CGui::TextBoxPtr spTbHour= static_pointer_cast< CGui::TextBox >( spPnlBarPanel->FindChild( "TbHour" ) );
                if ( u32PassedDays >= u32DaysAgo )
                {
                    spTbHour->SetText( CFoundation::String::Build( "%d.", u32PassedDays - u32DaysAgo + 1 ) );
                }
                else
                {
                    spTbHour->SetText( "-" );
                }

                // Remove tooltip controller
                CGui::TooltipSystem::GetInstance().RemoveTooltipController( spTbHour );
            }
        }
        break;
    }
}
               
void StatsRatingsWindow::OnPeriodPushed()
{
    // Get next period
    Period eNewPeriod = m_ePeriod;
    enum_increment( eNewPeriod, PERIOD_NONE, PERIOD_END );

    // Don't show yesterday's stats if no day played yet
    if ( eNewPeriod == PERIOD_YESTERDAY )
    {
        if ( m_spGame != NULL &&
             m_spGame->GetDays() == 0 )
        {
            enum_increment( eNewPeriod, PERIOD_NONE, PERIOD_END );
        }
    }

    if ( eNewPeriod == PERIOD_END )
    {
        eNewPeriod = PERIOD_TODAY;
    }

    SetPeriod( eNewPeriod );
}
               
void StatsRatingsWindow::OnImagePushed()
{
    StatsImageWindowPtr spStatsImage = Create< StatsImageWindow >( GetParent() );
    spStatsImage->SetContext( m_spContext );

    Close();
}
       
void StatsRatingsWindow::OnChangedPlayerSelection( CCtrl::ButtonGroupPtr _spButtonGroup, CGui::ButtonPtr _spNewActiveButton, CGui::ButtonPtr _spOldActiveButton )
{
    // Set new player
    SetPlayer( m_mapButtonPlayers[ _spNewActiveButton ] );
}
     