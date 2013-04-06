#include "cmadtv/Windows/InfoWindow.h"

#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/ProgressBar.h"
#include "cgui/Gui/TextBox.h"

#include "cgui/Tooltip/PointerTooltipController.h"
#include "cgui/Tooltip/TooltipSystem.h"

#include "cmadtv/Data/Advert.h"
#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Player/DayStat.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerCompBetty.h"
#include "cmadtv/Data/Player/PlayerCompSpectators.h"
#include "cmadtv/Data/Player/PlayerCompStats.h"
#include "cmadtv/Data/Player/PlayerContext.h"
#include "cmadtv/Data/Player/Schedule.h"
#include "cmadtv/Data/Player/Scheduling.h"

#include "cmadtv/Input/InputTranslator.h"

using namespace CMadTV;
        
CFoundation::String InfoWindow::GetXMLFileName()
{
    return "data/windows/InfoWindow.xml";
}

InfoWindow::InfoWindow()
{
    m_spDelegateLoveChanged = PlayerCompBettyEventU8U8Delegate2Ptr( new PlayerCompBettyEventU8U8Delegate2( this, &InfoWindow::OnLoveChanged ) );
}

void InfoWindow::InitWindow()
{
	m_spTbFrameRate = GetTextBox( "TbFrameRate" );
    m_spTbBudget = GetTextBox( "TbCash" );
	m_spTbSpectators = GetTextBox( "TbSpectators" );
    m_spPbLove = GetProgressBar( "PbLove" );
	m_spTbTime = GetTextBox( "TbTime" );

    // Create tooltips
    m_spPnlTooltipBudget = CGui::GuiMain::GetInstance().CreateFromXML< CGui::Panel >( "data/windows/BudgetTooltip.xml" );
    m_spTbCash = static_pointer_cast< CGui::TextBox >( m_spPnlTooltipBudget->FindDescendant( "TbCash" ) );
    m_spTbCredit = static_pointer_cast< CGui::TextBox >( m_spPnlTooltipBudget->FindDescendant( "TbCredit" ) );

    CGui::PointerTooltipControllerPtr spTooltipBudget( new CGui::PointerTooltipController() );
    spTooltipBudget->SetTooltip( m_spPnlTooltipBudget );
    CGui::TooltipSystem::GetInstance().SetTooltipController( m_spTbBudget, spTooltipBudget );

    m_spPnlTooltipSpectators = CGui::GuiMain::GetInstance().CreateFromXML< CGui::Panel >( "data/windows/SpectatorsTooltip.xml" );
    m_spTbMaxViewers = static_pointer_cast< CGui::TextBox >( m_spPnlTooltipSpectators->FindChild( "TbMaxViewers" ) );
    m_spTbCurrentRating = static_pointer_cast< CGui::TextBox >( m_spPnlTooltipSpectators->FindChild( "TbRating" ) );
    m_spTbNextAdvert = static_pointer_cast< CGui::TextBox >( m_spPnlTooltipSpectators->FindChild( "TbNextAdvert" ) );

    CGui::PointerTooltipControllerPtr spTooltipSpectators( new CGui::PointerTooltipController() );
    spTooltipSpectators->SetTooltip( m_spPnlTooltipSpectators );
    CGui::TooltipSystem::GetInstance().SetTooltipController( m_spTbSpectators, spTooltipSpectators );
    
    m_spPnlTooltipTime = CGui::GuiMain::GetInstance().CreateFromXML< CGui::Panel >( "data/windows/TimeTooltip.xml" );
    m_spTbDay = static_pointer_cast< CGui::TextBox >( m_spPnlTooltipTime->FindChild( "TbDay" ) );

    CGui::PointerTooltipControllerPtr spTooltipTime( new CGui::PointerTooltipController() );
    spTooltipTime->SetTooltip( m_spPnlTooltipTime );
    CGui::TooltipSystem::GetInstance().SetTooltipController( m_spTbTime, spTooltipTime );

    WindowController::InitWindow();
}
        
void InfoWindow::DeinitWindow()
{
    // Remove player
    SetPlayer( PlayerPtr() );

    // Remove tooltips
    CGui::TooltipSystem::GetInstance().RemoveTooltipController( m_spTbBudget ); 
    CGui::GuiMain::GetInstance().Delete( m_spPnlTooltipBudget );   
    CGui::TooltipSystem::GetInstance().RemoveTooltipController( m_spTbSpectators );
    CGui::GuiMain::GetInstance().Delete( m_spPnlTooltipSpectators );
    CGui::TooltipSystem::GetInstance().RemoveTooltipController( m_spTbTime ); 
    CGui::GuiMain::GetInstance().Delete( m_spPnlTooltipTime );   

    WindowController::DeinitWindow();
}
        
void InfoWindow::ProcessInput( const CSystem::InputState& _state )
{
    if ( InputTranslator::GetInstance().WasExecuted( INPUT_COMMAND_DEBUG_SPEED_PAUSE, _state ) )
    {
        m_spPlayer->GetGame()->SetSpeed( 0.0f );
    }
    else if ( InputTranslator::GetInstance().WasExecuted( INPUT_COMMAND_DEBUG_SPEED_1, _state ) )
    {
        m_spPlayer->GetGame()->SetSpeed( 60.0f );
    }
    else if ( InputTranslator::GetInstance().WasExecuted( INPUT_COMMAND_DEBUG_SPEED_2, _state ) )
    {
        m_spPlayer->GetGame()->SetSpeed( 200.0f );
    }
    else if ( InputTranslator::GetInstance().WasExecuted( INPUT_COMMAND_DEBUG_SPEED_3, _state ) )
    {
        m_spPlayer->GetGame()->SetSpeed( 2000.0f );
    }
    if ( InputTranslator::GetInstance().WasExecuted( INPUT_COMMAND_DEBUG_GET_MONEY, _state ) )
    {
        m_spPlayer->GetBudget().Book( ACCOUNT_NONE, ACCOUNT_CASH, CFoundation::Money( 100000, CFoundation::CURRENCY_DOLLAR ), m_spPlayer->GetGame()->GetDateTime() );
    }
}
        
void InfoWindow::Update( Float32 /*_f32TimeDifference*/ )
{
    // Update spectators and time
    CFoundation::String strSpectators;
    if ( m_spPlayer != NULL )
    {
        // Create spectators string
        Hour eHour = m_spGame->GetHour();
        HourStage eHourStage = m_spGame->GetHourStage();
        PlayerCompStatsPtr spCompStats = m_spPlayer->GetCompStats();
        BlockStatPtr spStat = spCompStats->GetDayStat( m_spGame->GetDate() )->GetBlockStat( eHour );
        Unsigned32 u32NumViewers = eHourStage == HOUR_STAGE_NEWS ? spStat->GetNewsSpectators() : spStat->GetBroadcastSpectators();
        strSpectators.Format( "%.2f", u32NumViewers / 1000000.0f ); 

        // Set max viewers
        Unsigned32 u32MaxViewers = m_spPlayer->GetCompSpectators()->GetMaxNumSpectators( m_spContext->GetDatabase()->GetMap() );
        m_spTbMaxViewers->SetText( CFoundation::String::MakeMillionString( u32MaxViewers ) );

        // Set current rating
        CFoundation::String strTooltipSpectators;
        if ( eHour != HOUR_NONE )
        {
            strTooltipSpectators.Format( "%s (%.0f%%)", CFoundation::String::MakeMillionString( u32NumViewers ).c_str(), u32NumViewers * 100.0f / u32MaxViewers );
        }
        else
        {
            strTooltipSpectators = "N/A";
        }
        m_spTbCurrentRating->SetText( strTooltipSpectators );

        // Set next advert
        AdvertPtr spAdvert = m_spPlayer->GetScheduling()->GetSchedule( SCHEDULE_DAY_TODAY )->GetAdvert( eHour );
        CFoundation::String strText;
        bool bActive = false;
        if ( spAdvert != NULL )
        {
            Unsigned32 u32MinViewers = spAdvert->GetMinSpectators();
            bActive = u32NumViewers < u32MinViewers;
            strText = CFoundation::String::Build( "%s (%.0f%%)", CFoundation::String::MakeMillionString( u32MinViewers ).c_str(), u32MinViewers * 100.0f / u32MaxViewers );
        }
        else
        {
            bActive = eHour != HOUR_NONE;
            strText = eHour != HOUR_NONE ? "ERROR!" : "N/A";
        }
        m_spTbNextAdvert->SetActive( bActive );
        m_spTbNextAdvert->SetText( strText );
    }
    else
    {
        strSpectators = "---";
    }
    
    m_spTbSpectators->SetText( strSpectators );
}
        
void InfoWindow::OnContextChanged()
{
    SetGame( m_spContext != NULL ? m_spContext->GetGame() : GamePtr() );
    SetPlayer( m_spContext != NULL ? m_spContext->GetPlayer() : PlayerPtr() );
}

void InfoWindow::SetGame( GamePtr _spGame )
{
    if ( _spGame == m_spGame )
    {
        return;
    }

    // Unregister for world events
    if ( m_spGame != NULL )
    {
        SAFE_CALL( m_spDelegateTimeChanged )->Disconnect();
        SAFE_CALL( m_spDelegateDateChanged )->Disconnect();
    }

    m_spGame = _spGame;

    // Register for world events
    if ( m_spGame != NULL )
    {
        m_spDelegateTimeChanged = m_spGame->RegisterDelegate( EVENT_GAME_TIME_CHANGED, CFoundation::CreateDelegate( this, &InfoWindow::OnTimeChanged ) );
        m_spDelegateDateChanged = m_spGame->RegisterDelegate( EVENT_GAME_DATE_CHANGED, CFoundation::CreateDelegate( this, &InfoWindow::OnDateChanged ) );
    }

    // Update game based controls
    OnTimeChanged();
    OnDateChanged();
}
        
void InfoWindow::SetPlayer( PlayerPtr _spPlayer )
{
    if ( m_spPlayer == _spPlayer )
    {
        return;
    }

    if ( m_spPlayer != NULL )
    {
        // Remove delegates
        if ( m_spDelegateCashChanged != NULL )
        {
            m_spDelegateCashChanged->Disconnect();
            m_spDelegateCashChanged.reset();
        }

        PlayerCompBettyPtr spCompBetty = m_spPlayer->GetCompBetty();
        spCompBetty->RemoveDelegate( EVENT_BETTY_LOVE_CHANGED, m_spDelegateLoveChanged );

        OnCashChanged( CFoundation::Money( 0 ), CFoundation::Money( 0 ) );
        OnCreditChanged( CFoundation::Money( 0 ), CFoundation::Money( 0 ) );
    }

    m_spPlayer = _spPlayer;

    if ( m_spPlayer != NULL )
    {
        // Register delegates
        CFoundation::Account& clCashAccount = m_spPlayer->GetBudget().GetAccount( ACCOUNT_CASH );
        m_spDelegateCashChanged = clCashAccount.RegisterDelegate( CFoundation::ACCOUNT_EVENT_BALANCE_CHANGED, CreateDelegate( this, &InfoWindow::OnCashChanged ) );        

        PlayerCompBettyPtr spCompBetty = m_spPlayer->GetCompBetty();
        spCompBetty->RegisterDelegate( EVENT_BETTY_LOVE_CHANGED, m_spDelegateLoveChanged );

        OnCashChanged( CFoundation::Money( 0 ), clCashAccount.GetBalance() );
        OnCreditChanged( CFoundation::Money( 0 ), m_spPlayer->GetBudget().GetCredit() );
        OnLoveChanged( spCompBetty->GetLove(), 0 );
    }

    // Set game
    SetGame( m_spPlayer != NULL ? m_spPlayer->GetGame() : GamePtr() );
}
        
void InfoWindow::SetFPS( Float32 _f32FPS )
{
    // Update frame rate
    CFoundation::String strFrameRate;
    strFrameRate.Format( "%.2f fps", _f32FPS );
    m_spTbFrameRate->SetText( strFrameRate );
}
        
void InfoWindow::OnTimeChanged()
{
    if ( m_spGame != NULL )
    {
        const CFoundation::Time& clTime = m_spGame->GetTime();

        // Create time string
        CFoundation::String strTime;
        strTime.Format( "%02d:%02d", clTime.GetHour() % 24, clTime.GetMinute() );

        m_spTbTime->SetText( strTime );
    }
    else
    {
        m_spTbTime->SetText( "Start" );
    }
}
        
void InfoWindow::OnDateChanged()
{
    if ( m_spGame != NULL )
    {
        m_spTbDay->SetText( CFoundation::String::Build( "%d. Tag", m_spGame->GetDays() + 1 ) );
    }
    else
    {
        m_spTbDay->SetText( "---" );
    }
}
        
void InfoWindow::OnCashChanged( const CFoundation::Money& /*_clOldBalance*/, const CFoundation::Money& _clNewBalance )
{
    m_spTbBudget->SetText( CFoundation::String::Build( "%.0f", _clNewBalance.GetAmount( CFoundation::CURRENCY_DOLLAR ) / 1000.0f ) );

    m_spTbCash->SetText( _clNewBalance.ToString( CFoundation::CURRENCY_DOLLAR ) );
}
       
void InfoWindow::OnCreditChanged( const CFoundation::Money& _clOldCredit, const CFoundation::Money& _clNewCredit )
{
    m_spTbCredit->SetText( _clNewCredit.ToString( CFoundation::CURRENCY_DOLLAR ) );
}
        
void InfoWindow::OnLoveChanged( Unsigned8 _u8NewLove, Unsigned8 _u8OldLove )
{
    m_spPbLove->SetRatio( _u8NewLove / 100.0f );
}
