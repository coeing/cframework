#include "cmadtv/Windows/PlanningWindow.h"

#include "cgui/DragDrop/DragDropSystem.h"

#include "cctrl/Controls/ButtonGroup.h"

#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerContext.h"
#include "cmadtv/Data/Player/Scheduling.h"

#include "cmadtv/DragDrop/BlockTrashDropController.h"
#include "cmadtv/Windows/PlanningAdvertContractsWindow.h"
#include "cmadtv/Windows/PlanningArchivWindow.h"
#include "cmadtv/Windows/PlanningSchedulePanel.h"
#include "cmadtv/Windows/Planning/ControlCenterWindow.h"
#include "cmadtv/Windows/Planning/StatsBudgetWindow.h"
#include "cmadtv/Windows/Planning/StatsImageWindow.h"
#include "cmadtv/Windows/Planning/StatsRatingsWindow.h"

using namespace CMadTV;
        
CFoundation::String PlanningWindow::GetXMLFileName()
{
    return "data/windows/PlanningWindow.xml";
}

PlanningWindow::PlanningWindow()
:   m_eScheduleDay( SCHEDULE_DAY_END ),
    m_spSchedulePanel( new PlanningSchedulePanel() )
{
    m_spDelegateClickedFunction = CGui::ObjectInputEventDelegate1Ptr( new CGui::ObjectInputEventDelegate1( this, &PlanningWindow::OnClickedFunction ) );

    m_spDelegateChangedSchedule = CCtrl::ButtonGroupButtonEventDelegate1Ptr( new CCtrl::ButtonGroupButtonEventDelegate1( this, &PlanningWindow::OnChangedSchedule ) );
    
    m_spTrashDropController = BlockTrashDropControllerPtr( new BlockTrashDropController( m_spContext ) );
}

PlanningWindow::~PlanningWindow()
{
}

void PlanningWindow::InitWindow()
{
    // Create button group
    m_spScheduleButtons = CCtrl::ButtonGroupPtr( new CCtrl::ButtonGroup() );
    m_spScheduleButtons->RegisterDelegate( CCtrl::BUTTON_GROUP_EVENT_SELECTION_CHANGED, m_spDelegateChangedSchedule );

    // Get schedule buttons
    CGui::ButtonPtr spBtToday = GetButton( "BtToday" );
    m_spScheduleButtons->AddButton( spBtToday );
    CGui::ButtonPtr spBtTomorrow = GetButton( "BtTomorrow" );
    m_spScheduleButtons->AddButton( spBtTomorrow );

    m_spImgControlCenter = GetImage( "ImgControlCenter" );
    m_spImgControlCenter->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedFunction );

    m_spImgAdvertArchiv = GetImage( "ImgAdvertArchiv" );
    m_spImgAdvertArchiv->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedFunction );

    m_spImgBroadcastArchiv = GetImage( "ImgBroadcastArchiv" );
    m_spImgBroadcastArchiv->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedFunction );

    m_spImgStatsBudget = GetImage( "ImgStatsBudget" );
    m_spImgStatsBudget->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedFunction );

    m_spImgStatsImage = GetImage( "ImgStatsImage" );
    m_spImgStatsImage->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedFunction );

    m_spImgTrash = GetImage( "ImgTrash" );
    CGui::DragDropSystem::GetInstance().SetDropController( m_spImgTrash, m_spTrashDropController );

    WindowController::InitWindow();

    // Initialize schedule panel
    m_spSchedulePanel->Init( GetPanel( "PnlPlanning" ) );
}
        
void PlanningWindow::DeinitWindow()
{
    // Close sub window that is currently open
    if ( m_spSubWindow != NULL )
    {
        m_spSubWindow->Close();
        m_spSubWindow.reset();
    }

    // Deinitialize schedule panel
    m_spSchedulePanel->Deinit();

    CGui::DragDropSystem::GetInstance().RemoveDropController( m_spImgTrash );

    m_spImgControlCenter->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedFunction );
    m_spImgAdvertArchiv->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedFunction );
    m_spImgBroadcastArchiv->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedFunction );
    m_spImgStatsBudget->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedFunction );
    m_spImgStatsImage->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedFunction );    

    // Clear player
    SetPlayer( PlayerPtr() );

    // Remove all buttons from button groups
    m_spScheduleButtons->RemoveDelegate( CCtrl::BUTTON_GROUP_EVENT_SELECTION_CHANGED, m_spDelegateChangedSchedule );
    m_spScheduleButtons->RemoveAllButtons();

    WindowController::DeinitWindow();
}
     		
void PlanningWindow::OnCloseScreen( const CCtrl::ScreenClosedMsg& _clMessage )
{
    if ( _clMessage.GetScreen() == m_spSubWindow )
    {
        m_spSubWindow.reset();
    }
}

void PlanningWindow::OnContextChanged()
{
    SetGame( m_spContext != NULL ? m_spContext->GetGame() : GamePtr() );
    SetPlayer( m_spContext != NULL ? m_spContext->GetPlayer() : PlayerPtr() );

    // Update members
    m_spSchedulePanel->SetContext( m_spContext );
    m_spTrashDropController->SetContext( m_spContext );
}
       
void PlanningWindow::SetGame( GamePtr _spGame )
{
    if ( _spGame == m_spGame )
    {
        return;
    }

    m_spGame = _spGame;
}

void PlanningWindow::SetPlayer( PlayerPtr _spPlayer )
{
    if ( _spPlayer == m_spPlayer )
    {
        return;
    }

    m_spPlayer = _spPlayer;

    // Set schedule
    SetSchedule( SCHEDULE_DAY_TODAY );
}
        
void PlanningWindow::SetSchedule( ScheduleDay _eScheduleDay )
{
    if ( _eScheduleDay == m_eScheduleDay )
    {
        return;
    }

    m_eScheduleDay = _eScheduleDay;

    m_spSchedulePanel->SetSchedule( m_eScheduleDay );
}
        
void PlanningWindow::OnChangedSchedule( CCtrl::ButtonGroupPtr _spButtonGroup )
{
    // Set current schedule
    ScheduleDay eDay = SCHEDULE_DAY_TODAY;
    if ( _spButtonGroup->GetActiveButtonIdx() != 0 )
    {
        eDay = SCHEDULE_DAY_TOMORROW;
    }

    SetSchedule( eDay );
}
           
void PlanningWindow::OnClickedFunction( CGui::GuiObjectPtr _spObject )
{
    WindowPtr spNewSubWindow;
    if ( _spObject == m_spImgControlCenter )
    {
        // Open control center window
        spNewSubWindow = Create< ControlCenterWindow >( GetSharedPtr() );
    }
    else if ( _spObject == m_spImgAdvertArchiv )
    {
        // Open advert contracts window
        spNewSubWindow = Create< PlanningAdvertContractsWindow >( GetSharedPtr() );
    }
    else if ( _spObject == m_spImgBroadcastArchiv )
    {
        // Open broadcast archiv window
        spNewSubWindow = Create< PlanningArchivWindow >( GetSharedPtr() );
    }
    else if ( _spObject == m_spImgStatsBudget )
    {
        // Open budget stats window
        spNewSubWindow = Create< StatsBudgetWindow >( GetSharedPtr() );
    }
    else if ( _spObject == m_spImgStatsImage )
    {
        // Open rating stats window
        spNewSubWindow = Create< StatsRatingsWindow >( GetSharedPtr() );
    }

    if ( spNewSubWindow == NULL )
    {
        return;
    }

    // Close old sub window
    if ( m_spSubWindow != NULL )
    {
        m_spSubWindow->Close();
        m_spSubWindow.reset();
    }

    // Init new window
    m_spSubWindow = spNewSubWindow;
    m_spSubWindow->SetContext( m_spContext );
}

