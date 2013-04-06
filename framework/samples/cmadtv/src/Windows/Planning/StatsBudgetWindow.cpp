#include "cmadtv/Windows/Planning/StatsBudgetWindow.h"

#include "csystem/Input/Input.h"

#include "cgui/Gui/Button.h"
#include "cgui/Gui/TextBox.h"

#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerContext.h"
#include "cmadtv/Data/Player/Budget.h"

using namespace CMadTV;
        
CFoundation::String StatsBudgetWindow::GetXMLFileName()
{
    return "data/windows/StatsBudgetWindow.xml";
}

StatsBudgetWindow::StatsBudgetWindow()
:   m_bDateToday( true )
{
    m_spDelegateDayPushed = CGui::StateEventDelegate0Ptr( new CGui::StateEventDelegate0( this, &StatsBudgetWindow::OnDayPushed ) );
}

StatsBudgetWindow::~StatsBudgetWindow()
{
}

void StatsBudgetWindow::InitWindow()
{    
    // Get account textboxes
    for ( Unsigned32 u32Account = 0; u32Account < ACCOUNT_END; ++u32Account )
    {
        AccountIdentifier eAccount = static_cast< AccountIdentifier >( u32Account );
        m_mapAccountTextBoxes[ eAccount ] = GetValueTextBox( CFoundation::String::Build( "PnlAccount%d", u32Account ) );
    }

    m_spTbExpensesTotal = GetValueTextBox( "PnlExpensesTotal" );
    m_spTbIncomeTotal = GetValueTextBox( "PnlIncomeTotal" );

    m_spTbOverviewPrev = GetValueTextBox( "PnlOverviewPrev" );
    m_spTbOverviewExpenses = GetValueTextBox( "PnlOverviewExpenses" );
    m_spTbOverviewIncome = GetValueTextBox( "PnlOverviewIncome" );
    m_spTbOverviewInterest = GetValueTextBox( "PnlOverviewInterest" );
    m_mapAccountTextBoxes[ ACCOUNT_INTEREST ] = m_spTbOverviewInterest;
    m_spTbOverviewCurrent = GetValueTextBox( "PnlOverviewCurrent" );
    
    m_spBtDay = GetButton( "BtDay" );
    m_spBtDay->RegisterDelegate( CGui::INPUT_EVENT_PUSHED, m_spDelegateDayPushed );

    WindowController::InitWindow();
}
        
void StatsBudgetWindow::DeinitWindow()
{
    // Remove delegates
    m_spBtDay->RemoveDelegate( CGui::INPUT_EVENT_PUSHED, m_spDelegateDayPushed );

    WindowController::DeinitWindow();
}

void StatsBudgetWindow::ProcessInput( const CSystem::InputState& _State )
{
    if ( _State.WasPressed( CSystem::MOUSE_RIGHT ) ||
         _State.WasPressed( CSystem::KEY_ESC ) )
    {
        Close();
    }
}
        
void StatsBudgetWindow::OnContextChanged()
{
    SetPlayer( m_spContext != NULL ? m_spContext->GetPlayer() : PlayerPtr() );
}
                
void StatsBudgetWindow::SetPlayer( PlayerPtr _spPlayer )
{
    if ( _spPlayer == m_spPlayer )
    {
        return;
    }
    
    m_spPlayer = _spPlayer;

    UpdateStats();
}
       
CGui::TextBoxPtr StatsBudgetWindow::GetValueTextBox( const CFoundation::String& _strPanelName ) const
{
    CGui::PanelCPtr spPanel = GetPanel( _strPanelName );
    if ( spPanel != NULL )
    {
        return static_pointer_cast< CGui::TextBox >( spPanel->FindChild( "TbAccountValue" ) );
    }
    else
    {
        return CGui::TextBoxPtr();
    }
}
        
void StatsBudgetWindow::UpdateStats()
{
    if ( m_spPlayer == NULL )
    {
        return;
    }

    // Get date to show account results for
    CFoundation::DateTime fromDateTime = m_spPlayer->GetGame()->GetDateTime();
    fromDateTime.SetTime( 0, 0, 0 );
    
    // Check if to show yesterdays results
    if ( !m_bDateToday )
    {
        fromDateTime.AddDays( -1 );
    }

    // Compute to date
    CFoundation::DateTime toDateTime = fromDateTime;
    toDateTime.AddDays( 1 );

    // Get budget
    CFoundation::Money totalResult;
    Budget& clBudget = m_spPlayer->GetBudget();
    for ( Unsigned32 u32Account = ACCOUNT_NONE; u32Account < ACCOUNT_END; ++u32Account )
    {
        AccountIdentifier eAccount = static_cast< AccountIdentifier >( u32Account );
        if ( eAccount == ACCOUNT_NONE )
        {
            continue;
        }

        CFoundation::Account& clAccount = clBudget.GetAccount( eAccount );
        const CFoundation::Money& result = clAccount.ComputeResult( fromDateTime, toDateTime );
        CFoundation::Money resultUnsigned = result.GetAmount() < 0 ? -result : result;

        CGui::TextBoxPtr spTbValue = m_mapAccountTextBoxes[ eAccount ];      
        if ( spTbValue != NULL )
        {
            spTbValue->SetText( resultUnsigned.ToString( CFoundation::CURRENCY_DOLLAR ) );
        }
        
        // Add to total result
        totalResult += result;

        // Check if end of expenses or income
        if ( eAccount == ACCOUNT_EXPENSES_END )
        {
            CFoundation::Money totalResultUnsigned = totalResult.GetAmount() < 0 ? -totalResult : totalResult;
            m_spTbExpensesTotal->SetText( totalResultUnsigned.ToString( CFoundation::CURRENCY_DOLLAR ) );
            m_spTbOverviewExpenses->SetText( totalResultUnsigned.ToString( CFoundation::CURRENCY_DOLLAR ) );
            totalResult.SetAmount( 0 );
        }
        else if ( eAccount == ACCOUNT_INCOME_END )
        {
            CFoundation::Money totalResultUnsigned = totalResult.GetAmount() < 0 ? -totalResult : totalResult;
            m_spTbIncomeTotal->SetText( totalResultUnsigned.ToString( CFoundation::CURRENCY_DOLLAR ) );
            m_spTbOverviewIncome->SetText( totalResultUnsigned.ToString( CFoundation::CURRENCY_DOLLAR ) );
            totalResult.SetAmount( 0 );
        }
    }
    
    // Get cash amount at start and end
    m_spTbOverviewPrev->SetText( clBudget.GetAccount( ACCOUNT_CASH ).ComputeBalance( fromDateTime ).ToString( CFoundation::CURRENCY_DOLLAR ) );   
    m_spTbOverviewCurrent->SetText( clBudget.GetAccount( ACCOUNT_CASH ).ComputeBalance( toDateTime ).ToString( CFoundation::CURRENCY_DOLLAR ) );    
}
               
void StatsBudgetWindow::OnDayPushed()
{
    if ( m_bDateToday )
    {
        // Check if at least one day played
        GamePtr spGame = m_spContext != NULL ? m_spContext->GetGame() : GamePtr();
        if ( spGame != NULL &&
             spGame->GetDays() == 0 )
        {
            return;
        }
    }

    m_bDateToday = !m_bDateToday;

    m_spBtDay->SetText( m_bDateToday ? "Today" : "Yesterday" );

    UpdateStats();
}
     