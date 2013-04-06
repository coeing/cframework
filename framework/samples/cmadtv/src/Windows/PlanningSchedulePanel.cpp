#include "cmadtv/Windows/PlanningSchedulePanel.h"

#include "cfoundation/Debug/Makros.h"

#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/Image.h"

#include "cgui/DragDrop/DragDropSystem.h"
#include "cgui/DragDrop/FreeDragController.h"
#include "cgui/Tooltip/TooltipSystem.h"

#include "cmadtv/Data/Advert.h"

#include "cmadtv/Data/Game/Game.h"

#include "cmadtv/Data/Player/AdvertContract.h"
#include "cmadtv/Data/Player/BlockStat.h"
#include "cmadtv/Data/Player/DayStat.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerCompStats.h"
#include "cmadtv/Data/Player/PlayerContext.h"
#include "cmadtv/Data/Player/Schedule.h"
#include "cmadtv/Data/Player/Scheduling.h"

#include "cmadtv/Logic/PlayerInput.h"
#include "cmadtv/Logic/Commands/ScheduleAdvertCmd.h"
#include "cmadtv/Logic/Commands/ScheduleBroadcastCmd.h"

#include "cmadtv/DragDrop/AdvertDragController.h"
#include "cmadtv/DragDrop/AdvertDropController.h"
#include "cmadtv/DragDrop/BroadcastDragController.h"
#include "cmadtv/DragDrop/BroadcastDropController.h"
#include "cmadtv/DragDrop/BroadcastBlockDropController.h"

#include "cmadtv/Tooltip/AdvertTooltipController.h"
#include "cmadtv/Tooltip/BroadcastTooltipController.h"

#include "cmadtv/Windows/AdvertBlock.h"
#include "cmadtv/Windows/BroadcastBlock.h"

using namespace CMadTV;

PlanningSchedulePanel::PlanningSchedulePanel()
:   m_eScheduleDay( SCHEDULE_DAY_END )
{
    // Create delegates
    m_spDelegateBroadcastAdded = ScheduleBroadcastEventDelegatePtr( new ScheduleBroadcastEventDelegate( this, &PlanningSchedulePanel::OnBroadcastAdded ) );
    m_spDelegateBroadcastRemoved = ScheduleBroadcastEventDelegatePtr( new ScheduleBroadcastEventDelegate( this, &PlanningSchedulePanel::OnBroadcastRemoved ) );
    m_spDelegateBroadcastDragStarted = CGui::DragEventDelegatePtr( new CGui::DragEventDelegate( this, &PlanningSchedulePanel::OnBroadcastDragStarted ) );
    m_spDelegateBroadcastDragUndone = CGui::DragEventDelegatePtr( new CGui::DragEventDelegate( this, &PlanningSchedulePanel::OnBroadcastDragUndone ) );
    m_spDelegateBroadcastDragEnded = CGui::DragEventDelegatePtr( new CGui::DragEventDelegate( this, &PlanningSchedulePanel::OnBroadcastDragEnded ) );

    m_spDelegateAdvertAdded = ScheduleAdvertEventDelegatePtr( new ScheduleAdvertEventDelegate( this, &PlanningSchedulePanel::OnAdvertAdded ) );
    m_spDelegateAdvertRemoved = ScheduleAdvertEventDelegatePtr( new ScheduleAdvertEventDelegate( this, &PlanningSchedulePanel::OnAdvertRemoved ) );
    m_spDelegateAdvertDragStarted = CGui::DragEventDelegatePtr( new CGui::DragEventDelegate( this, &PlanningSchedulePanel::OnAdvertDragStarted ) );
    m_spDelegateAdvertDragUndone = CGui::DragEventDelegatePtr( new CGui::DragEventDelegate( this, &PlanningSchedulePanel::OnAdvertDragUndone ) );
    m_spDelegateAdvertDragEnded = CGui::DragEventDelegatePtr( new CGui::DragEventDelegate( this, &PlanningSchedulePanel::OnAdvertDragEnded ) );
}

PlanningSchedulePanel::~PlanningSchedulePanel()
{
}

void PlanningSchedulePanel::Init( CGui::PanelPtr _spPanel )
{
    m_spPnlRoot = _spPanel;
    
    // Create time slots
    CGui::GuiObjectPtr spObjTimeSlotsAnchor = m_spPnlRoot->FindChild( "ObjTimeSlotsAnchor" );
    CGui::PanelPtr spPnlTimeSlotTemplate = static_pointer_cast< CGui::Panel >( m_spPnlRoot->FindChild( "PnlTimeSlotTemplate" ) );
    spPnlTimeSlotTemplate->RemoveFromParent();

    for ( Hour eHour = HOUR_BEGIN; eHour != HOUR_END; ++eHour )
    {
        // Create slot
        CGui::PanelPtr spPnlTimeSlot = CGui::GuiMain::GetInstance().CreateFromTemplate( "PnlTimeSlot", spPnlTimeSlotTemplate );

        // Get children
        CGui::ImagePtr spImgBackground = static_pointer_cast< CGui::Image >( spPnlTimeSlot->FindChild( "ImgBackground" ) );
        CGui::TextBoxPtr spTbTime = static_pointer_cast< CGui::TextBox >( spPnlTimeSlot->FindChild( "TbTime" ) );

        // Set time
        spTbTime->SetText( CFoundation::String::Build( "%2d:00", eHour - HOUR_BEGIN + 18 ) );

        // Add slot
        m_aspTimeSlot[ eHour ] = spImgBackground;
        spObjTimeSlotsAnchor->AddAfter( spPnlTimeSlot );
        spObjTimeSlotsAnchor = spPnlTimeSlot;
    }

    CGui::GuiMain::GetInstance().Delete( spPnlTimeSlotTemplate );
    
    // Create broadcast slots
    CGui::GuiObjectPtr spObjBroadcastSlotsAnchor = m_spPnlRoot->FindChild( "ObjBroadcastSlotsAnchor" );
    CGui::ImagePtr spImgBroadcastSlotTemplate = static_pointer_cast< CGui::Image >( m_spPnlRoot->FindChild( "ImgBroadcastSlotTemplate" ) );
    spImgBroadcastSlotTemplate->RemoveFromParent();

    for ( Hour eHour = HOUR_BEGIN; eHour != HOUR_END; ++eHour )
    {
        // Create slot
        CGui::ImagePtr spImgBroadcastSlot = CGui::GuiMain::GetInstance().CreateFromTemplate( "ImgBroadcastSlot", spImgBroadcastSlotTemplate );

        // Add slot
        m_aspBroadcastSlot[ eHour ] = spImgBroadcastSlot;
        spObjBroadcastSlotsAnchor->AddAfter( spImgBroadcastSlot );
        spObjBroadcastSlotsAnchor = spImgBroadcastSlot;
    }

    CGui::GuiMain::GetInstance().Delete( spImgBroadcastSlotTemplate );
    
    // Create advert slots
    CGui::GuiObjectPtr spObjAdvertSlotsAnchor = m_spPnlRoot->FindChild( "ObjAdvertSlotsAnchor" );
    CGui::ImagePtr spImgAdvertSlotTemplate = static_pointer_cast< CGui::Image >( m_spPnlRoot->FindChild( "ImgAdvertSlotTemplate" ) );
    spImgAdvertSlotTemplate->RemoveFromParent();

    for ( Hour eSlot = HOUR_BEGIN; eSlot != HOUR_END; ++eSlot )
    {
        // Create slot
        CGui::ImagePtr spImgAdvertSlot = CGui::GuiMain::GetInstance().CreateFromTemplate( "ImgAdvertSlot", spImgAdvertSlotTemplate );

        // Add slot
        m_aspAdvertSlot[ eSlot ] = spImgAdvertSlot;
        spObjAdvertSlotsAnchor->AddAfter( spImgAdvertSlot );
        spObjAdvertSlotsAnchor = spImgAdvertSlot;
    }

    CGui::GuiMain::GetInstance().Delete( spImgAdvertSlotTemplate );
}
        
void PlanningSchedulePanel::Deinit()
{
}
        
void PlanningSchedulePanel::SetContext( PlayerContextPtr _spContext )
{
    m_spContext = _spContext;

    SetGame( m_spContext != NULL ? m_spContext->GetGame() : GamePtr() );
    SetPlayer( m_spContext != NULL ? m_spContext->GetPlayer() : PlayerPtr() );

    // Update drop controller
    for ( Hour eSlot = HOUR_BEGIN; eSlot != HOUR_END; ++eSlot )
    {
        BroadcastDropControllerPtr spBroadcastDropController = m_aspBroadcastSlotDropController[ eSlot ];
        SAFE_CALL( spBroadcastDropController )->SetContext( m_spContext );
    }
}
       
void PlanningSchedulePanel::SetGame( GamePtr _spGame )
{
    if ( _spGame == m_spGame )
    {
        return;
    }

    // Unregister for world events
    if ( m_spGame != NULL )
    {
        DISCONNECT_DELEGATE_CONNECTION( m_spDelegateHourStageChanged );
        DISCONNECT_DELEGATE_CONNECTION( m_spDelegateDateChanged );
    }

    m_spGame = _spGame;

    // Register for world events
    if ( m_spGame != NULL )
    {
        m_spDelegateHourStageChanged = m_spGame->RegisterDelegate( EVENT_GAME_HOUR_STAGE_CHANGED, CFoundation::CreateDelegate( this, &PlanningSchedulePanel::OnTimeChanged ) );
        m_spDelegateDateChanged = m_spGame->RegisterDelegate( EVENT_GAME_DATE_CHANGED, CFoundation::CreateDelegate( this, &PlanningSchedulePanel::OnDateChanged ) );
    }
}
       
void PlanningSchedulePanel::SetPlayer( PlayerPtr _spPlayer )
{
    if ( _spPlayer == m_spPlayer )
    {
        return;
    }

    m_spPlayer = _spPlayer;

    // Update schedule
    UpdateSchedule();
}

void PlanningSchedulePanel::SetSchedule( ScheduleDay _eScheduleDay )
{
    if ( _eScheduleDay == m_eScheduleDay )
    {
        return;
    }

    m_eScheduleDay = _eScheduleDay;

    // Update schedule
    UpdateSchedule();
}
        
void PlanningSchedulePanel::UpdateSchedule()
{
    // Update schedule
    if ( m_eScheduleDay == SCHEDULE_DAY_END ||
         m_spPlayer == NULL )
    {
        SetSchedule( SchedulePtr() );
    }
    else
    {
        SetSchedule( m_spPlayer->GetScheduling()->GetSchedule( m_eScheduleDay ) );
    }
}
       
CFoundation::Date PlanningSchedulePanel::GetShownDate() const
{
    if ( m_spContext == NULL )
    {
        return CFoundation::Date( 0, 0, 0 );
    }
    return ComputeDateFromScheduleDay( m_spContext->GetGame()->GetDate(), m_eScheduleDay );
}

void PlanningSchedulePanel::SetSchedule( SchedulePtr _spSchedule )
{
    if ( m_spSchedule == _spSchedule )
    {
        return;
    }

    // Get date of schedule
    const CFoundation::Date& shownDate = GetShownDate();

    if ( m_spSchedule != NULL )
    {
        // Remove delegates from old schedule
        m_spSchedule->RemoveDelegate( SCHEDULE_EVENT_BROADCAST_ADDED, m_spDelegateBroadcastAdded );
        m_spSchedule->RemoveDelegate( SCHEDULE_EVENT_BROADCAST_REMOVED, m_spDelegateBroadcastRemoved );
        m_spSchedule->RemoveDelegate( SCHEDULE_EVENT_ADVERT_ADDED, m_spDelegateAdvertAdded );
        m_spSchedule->RemoveDelegate( SCHEDULE_EVENT_ADVERT_REMOVED, m_spDelegateAdvertRemoved );

        // Clear broadcasts
        for ( Hour eHour = HOUR_BEGIN; eHour != HOUR_END; ++eHour )
        {
            BroadcastBlockPtr spBlock = m_aspBroadcastBlocks[ eHour ];
            if ( spBlock != NULL )
            {
                RemoveBroadcastBlock( eHour );
            }
        }

        // Clear adverts
        for ( Hour eHour = HOUR_BEGIN; eHour != HOUR_END; ++eHour )
        {
            AdvertBlockPtr spBlock = m_aspAdvertBlocks[ eHour ];
            if ( spBlock != NULL )
            {
                RemoveAdvertBlock( eHour );
            }
        }
    }

    m_spSchedule = _spSchedule;

    if ( m_spSchedule != NULL )
    {
        // Register delegates
        m_spSchedule->RegisterDelegate( SCHEDULE_EVENT_BROADCAST_ADDED, m_spDelegateBroadcastAdded );
        m_spSchedule->RegisterDelegate( SCHEDULE_EVENT_BROADCAST_REMOVED, m_spDelegateBroadcastRemoved );
        m_spSchedule->RegisterDelegate( SCHEDULE_EVENT_ADVERT_ADDED, m_spDelegateAdvertAdded );
        m_spSchedule->RegisterDelegate( SCHEDULE_EVENT_ADVERT_REMOVED, m_spDelegateAdvertRemoved );

        // Add broadcasts
        for ( Hour eSlot = HOUR_BEGIN; eSlot != HOUR_END; ++eSlot )
        {
            bool bBroadcastStart = m_spSchedule->DoesBroadcastStart( eSlot );
            if ( !bBroadcastStart )
            {
                continue;
            }

            BroadcastPtr spBroadcast = m_spSchedule->GetBroadcast( eSlot );
            if ( spBroadcast == NULL )
            {
                continue;
            }

            // Add broadcast block
            AddBroadcastBlock( spBroadcast, eSlot );
        }

        // Add adverts
        for ( Hour eSlot = HOUR_BEGIN; eSlot != HOUR_END; ++eSlot )
        {
            AdvertPtr spAdvert = m_spSchedule->GetAdvert( eSlot );
            if ( spAdvert == NULL )
            {
                continue;
            }

            // Add advert block
            AddAdvertBlock( spAdvert, eSlot );
        }
    }
    
    // Set drop controller for broadcast and advert slots
    for ( Hour eSlot = HOUR_BEGIN; eSlot != HOUR_END; ++eSlot )
    {
        // Set drop controller
        CGui::ImagePtr spImgBroadcastSlot = m_aspBroadcastSlot[ eSlot ];
        BroadcastDropControllerPtr spBroadcastDropController( new BroadcastDropController( m_spContext, shownDate, eSlot ) );
        CGui::DragDropSystem::GetInstance().SetDropController( spImgBroadcastSlot, spBroadcastDropController );
        m_aspBroadcastSlotDropController[ eSlot ] = spBroadcastDropController;

        // Set drop controller
        CGui::ImagePtr spImgAdvertSlot = m_aspAdvertSlot[ eSlot ];
        AdvertDropControllerPtr spAdvertDropController( new AdvertDropController( m_spContext, shownDate, eSlot ) );
        CGui::DragDropSystem::GetInstance().SetDropController( spImgAdvertSlot, spAdvertDropController );
        m_aspAdvertSlotDropController[ eSlot ] = spAdvertDropController;
    }

    // Update active states
    UpdateActiveStates();
}
      
void PlanningSchedulePanel::UpdateActiveStates()
{
    if ( m_spGame == NULL )
    {
        return;
    }

    // Get current date and time
    const CFoundation::Date& today = m_spGame->GetDate();
    Hour eCurHour = m_spGame->GetHour();
    HourStage eCurHourStage = m_spGame->GetHourStage();
    
    // Get date of schedule
    const CFoundation::Date& shownDate = GetShownDate();

    bool bCurrentDay = shownDate == today;
    for ( Hour eHour = HOUR_BEGIN; eHour < HOUR_END; ++eHour )
    {
        // Handle time slots
        bool bTimeActive = bCurrentDay && eHour <= eCurHour;
        CGui::ImagePtr spImgTimeSlot = m_aspTimeSlot[ eHour ];
        if ( spImgTimeSlot != NULL )
        {
            spImgTimeSlot->SetActive( bTimeActive );
        }

        // Handle broadcast slots
        bool bBroadcastActive = bCurrentDay && 
                                ( eHour < eCurHour ||
                                  eHour == eCurHour && eCurHourStage >= HOUR_STAGE_BROADCAST );
        CGui::ImagePtr spImgBroadcastSlot = m_aspBroadcastSlot[ eHour ];
        if ( spImgBroadcastSlot != NULL )
        {
            spImgBroadcastSlot->SetActive( bBroadcastActive );
        }
        CGui::DropControllerPtr spBroadcastSlotDropController = m_aspBroadcastSlotDropController[ eHour ];
        if ( spBroadcastSlotDropController != NULL )
        {
            spBroadcastSlotDropController->SetEnabled( !bBroadcastActive );
        }

        // Handle broadcast block
        BroadcastBlockPtr spBroadcastBlock = m_aspBroadcastBlocks[ eHour ];
        if ( spBroadcastBlock != NULL )
        {
            spBroadcastBlock->SetActive( bBroadcastActive );

            if ( bBroadcastActive )
            {
                // Remove drag controller
                CGui::PanelPtr spPanel = spBroadcastBlock->GetPanel();
                spPanel->SetDraggable( false );
                CGui::DragDropSystem::GetInstance().RemoveDragController( spPanel );
            }
        }

        CGui::DropControllerPtr spBroadcastDropController = m_aspBroadcastDropController[ eHour ];
        if ( spBroadcastDropController != NULL )
        {
            spBroadcastDropController->SetEnabled( !bBroadcastActive );
        }

        // Handle advert slots
        bool bAdvertActive = bCurrentDay && 
                             ( eHour < eCurHour ||
                               eHour == eCurHour && eCurHourStage >= HOUR_STAGE_ADVERT );
        CGui::ImagePtr spImgAdvertSlot = m_aspAdvertSlot[ eHour ];
        spImgAdvertSlot->SetActive( bAdvertActive );

        CGui::DropControllerPtr spAdvertSlotDropController = m_aspAdvertSlotDropController[ eHour ];
        if ( spAdvertSlotDropController != NULL )
        {
            spAdvertSlotDropController->SetEnabled( !bAdvertActive );
        }

        // Handle advert block
        AdvertBlockPtr spAdvertBlock = m_aspAdvertBlocks[ eHour ];
        if ( spAdvertBlock != NULL )
        {
            spAdvertBlock->SetActive( bAdvertActive );

            if ( bAdvertActive )
            {
                // Remove drag controller
                CGui::PanelPtr spPanel = spAdvertBlock->GetPanel();
                spPanel->SetDraggable( false );
                CGui::DragDropSystem::GetInstance().RemoveDragController( spPanel );
            }
        }

        CGui::DropControllerPtr spAdvertDropController = m_aspAdvertDropController[ eHour ];
        if ( spAdvertDropController != NULL )
        {
            spAdvertDropController->SetEnabled( !bAdvertActive );
        }
    }
}
        
void PlanningSchedulePanel::UpdateAdvertSubTexts()
{
    for ( Hour eHour = HOUR_BEGIN; eHour < HOUR_END; ++eHour )
    {
        AdvertBlockPtr spBlock = m_aspAdvertBlocks[ eHour ];
        if ( spBlock == NULL )
        {
            continue;
        }

        // Update sub text
        UpdateAdvertSubText( spBlock, eHour );
    }
}

void PlanningSchedulePanel::UpdateAdvertSubText( AdvertBlockPtr _spBlock, Hour _eHour )
{    
    if ( m_spGame == NULL )
    {
        return;
    }

    // Get current date and time
    const CFoundation::Date& today = m_spGame->GetDate();
    Hour eCurHour = m_spGame->GetHour();
    HourStage eCurHourStage = m_spGame->GetHourStage();
    
    // Get date of schedule
    const CFoundation::Date& shownDate = GetShownDate();

    // Get advert
    AdvertPtr spAdvert = _spBlock->GetAdvert();
    if ( spAdvert == NULL )
    {
        CF_WARN( "Advert block without advert." );
        return;
    }

    bool bCurrentDay = shownDate == today;
    bool bActive = bCurrentDay &&
                   ( _eHour < eCurHour ||
                     _eHour == eCurHour && eCurHourStage >= HOUR_STAGE_ADVERT );

    CFoundation::String strText;
    if ( bActive )
    {
        // Get block stat
        PlayerCompStatsPtr spCompStats = m_spPlayer->GetCompStats();
        BlockStatPtr spStat = spCompStats->GetDayStat( m_spPlayer->GetGame()->GetDate() )->GetBlockStat( _eHour );

        // Check if successful
        if ( spStat->WasAdvertSuccessful() )
        {
            if ( spStat->GetAdvertOrder() + 1 == spAdvert->GetNumSpots() )
            {
                strText = "Done!!!";
            }
            else
            {
                strText.Format( "%d/%d", spStat->GetAdvertOrder() + 1, spAdvert->GetNumSpots() );
            }
        }
        else
        {
            strText = "---";
        }
    }
    else
    {
        // Get contract
        AdvertContractPtr spContract = m_spPlayer->GetAdvertContracts().GetContract( spAdvert );
        CF_WARN_IF( spContract == NULL, "Advert without contract in schedule." );
        if ( spContract != NULL )
        {
            // Get advert order number
            Unsigned32 u32Order = spContract->GetNumSpotsSent();
            Hour eHourBegin = eCurHour;
            if ( eCurHourStage >= HOUR_STAGE_ADVERT )
            {
                ++eHourBegin;
            }
            ScheduleDay eDayEnd = m_eScheduleDay; ++eDayEnd;
            u32Order += m_spPlayer->GetScheduling()->GetNumAdverts( spAdvert, SCHEDULE_DAY_TODAY, eHourBegin, eDayEnd, _eHour );

            strText.Format( "%d/%d", u32Order + 1, spAdvert->GetNumSpots() );
        }
        else
        {
            strText = "???";
        }
    }

    // Set sub text
    _spBlock->SetSubText( strText );
}
        
void PlanningSchedulePanel::OnDateChanged()
{
    // Update schedule
    UpdateSchedule();
}
        
void PlanningSchedulePanel::OnTimeChanged()
{
    // Update active states
    UpdateActiveStates();

    // Update advert sub titles
    UpdateAdvertSubTexts();
}

void PlanningSchedulePanel::OnBroadcastAdded( BroadcastPtr _spBroadcast, Hour _eStartSlot )
{
    // Add broadcast block
    AddBroadcastBlock( _spBroadcast, _eStartSlot );
}

void PlanningSchedulePanel::OnBroadcastRemoved( BroadcastPtr _spBroadcast, Hour _eStartSlot )
{
    // Remove broadcast block
    RemoveBroadcastBlock( _eStartSlot );
}
        
void PlanningSchedulePanel::OnBroadcastDragStarted( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spObject, CGui::DragDataPtr _spDragData )
{
    // Broadcast drag data expected
    BroadcastDragDataPtr spDragData = dynamic_pointer_cast< BroadcastDragData >( _spDragData );
    if ( spDragData == NULL )
    {
        return;
    }

    // Remove broadcast from schedule
    ScheduleBroadcastCmdPtr spCmd( new ScheduleBroadcastCmd( m_spContext, BroadcastPtr(), spDragData->m_Date, spDragData->m_eHour ) );
    Unsigned32 u32ReturnValue = m_spContext->GetInput()->Execute( spCmd );
    switch ( u32ReturnValue )
    {
    case ScheduleBroadcastCmd::ERROR_INVALID_DATE:
    case ScheduleBroadcastCmd::ERROR_BROADCAST_TOO_LONG:
        {
        }
        break;
    }

    // Add delegate
    _spDragController->RegisterDelegate( CGui::EVENT_DRAG_UNDONE, m_spDelegateBroadcastDragUndone );
}
        
void PlanningSchedulePanel::OnBroadcastDragUndone( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spObject, CGui::DragDataPtr _spDragData )
{
    // Broadcast drag data expected
    BroadcastDragDataPtr spDragData = dynamic_pointer_cast< BroadcastDragData >( _spDragData );
    if ( spDragData == NULL )
    {
        return;
    }

    // Set broadcast back to schedule
    ScheduleBroadcastCmdPtr spCmd( new ScheduleBroadcastCmd( m_spContext, spDragData->m_spBroadcast, spDragData->m_Date, spDragData->m_eHour ) );
    Unsigned32 u32ReturnValue = m_spContext->GetInput()->Execute( spCmd );
    switch ( u32ReturnValue )
    {
    case ScheduleBroadcastCmd::ERROR_INVALID_DATE:
    case ScheduleBroadcastCmd::ERROR_BROADCAST_TOO_LONG:
        {
        }
        break;
    }

    // Remove delegate
    _spDragController->RemoveDelegate( CGui::EVENT_DRAG_UNDONE, m_spDelegateBroadcastDragUndone );
}
        
void PlanningSchedulePanel::OnBroadcastDragEnded( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spObject, CGui::DragDataPtr _spDragData )
{
    // Remove delegate
    _spDragController->RemoveDelegate( CGui::EVENT_DRAG_UNDONE, m_spDelegateBroadcastDragUndone );
}
        
void PlanningSchedulePanel::OnAdvertAdded( AdvertPtr _spAdvert, Hour _eSlot )
{
    // Add advert block
    AddAdvertBlock( _spAdvert, _eSlot );
}

void PlanningSchedulePanel::OnAdvertRemoved( AdvertPtr _spAdvert, Hour _eSlot )
{
    // Remove advert block
    RemoveAdvertBlock( _eSlot );
}

void PlanningSchedulePanel::OnAdvertDragStarted( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spObject, CGui::DragDataPtr _spDragData )
{
    // Advert drag data expected
    AdvertDragDataPtr spDragData = dynamic_pointer_cast< AdvertDragData >( _spDragData );
    if ( spDragData == NULL )
    {
        return;
    }

    // Remove advert from schedule
    ScheduleAdvertCmdPtr spCmd( new ScheduleAdvertCmd( m_spContext, AdvertPtr(), spDragData->m_Date, spDragData->m_eHour ) );
    Unsigned32 u32ReturnValue = m_spContext->GetInput()->Execute( spCmd );
    switch ( u32ReturnValue )
    {
    case ScheduleAdvertCmd::ERROR_INVALID_DATE:
        {
        }
        break;
    }

    // Add delegate
    _spDragController->RegisterDelegate( CGui::EVENT_DRAG_UNDONE, m_spDelegateAdvertDragUndone );
}

void PlanningSchedulePanel::OnAdvertDragUndone( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spObject, CGui::DragDataPtr _spDragData )
{
    // Advert drag data expected
    AdvertDragDataPtr spDragData = dynamic_pointer_cast< AdvertDragData >( _spDragData );
    if ( spDragData == NULL )
    {
        return;
    }

    // Set advert back to schedule
    ScheduleAdvertCmdPtr spCmd( new ScheduleAdvertCmd( m_spContext, spDragData->m_spAdvert, spDragData->m_Date, spDragData->m_eHour ) );
    Unsigned32 u32ReturnValue = m_spContext->GetInput()->Execute( spCmd );
    switch ( u32ReturnValue )
    {
    case ScheduleAdvertCmd::ERROR_INVALID_DATE:
        {
        }
        break;
    }

    // Remove delegate
    _spDragController->RemoveDelegate( CGui::EVENT_DRAG_UNDONE, m_spDelegateAdvertDragUndone );
}

void PlanningSchedulePanel::OnAdvertDragEnded( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spObject, CGui::DragDataPtr _spDragData )
{
    // Remove delegate
    _spDragController->RemoveDelegate( CGui::EVENT_DRAG_UNDONE, m_spDelegateAdvertDragUndone );
}
        
BroadcastBlockPtr PlanningSchedulePanel::AddBroadcastBlock( BroadcastPtr _spBroadcast, Hour _eStartSlot )
{
    // Create broadcast block
    BroadcastBlockPtr spBroadcastBlock = BroadcastBlock::Create( _spBroadcast );

    // Add tooltip controller
    CGui::PanelPtr spPanel = spBroadcastBlock->GetPanel();
    BroadcastTooltipControllerPtr spTooltipController( new BroadcastTooltipController( m_spContext, _spBroadcast ) );
    CGui::TooltipSystem::GetInstance().SetTooltipController( spPanel, spTooltipController );

    // Add as child
    m_spPnlRoot->AddChildLast( spPanel );
    
    // Set to right position
    spBroadcastBlock->GetPanel()->SetOffset( m_aspBroadcastSlot[ _eStartSlot ]->GetRelativePosition() );

    // Store
    m_aspBroadcastBlocks[ _eStartSlot ] = spBroadcastBlock;

    // Set drag controller
    spPanel->SetDraggable( true );
    BroadcastDragControllerPtr spDragController( new BroadcastDragController( spBroadcastBlock, GetShownDate(), _eStartSlot ) );
    CGui::DragDropSystem::GetInstance().SetDragController( spPanel, spDragController );

    // Add delegates
    spDragController->RegisterDelegate( CGui::EVENT_DRAG_STARTED, m_spDelegateBroadcastDragStarted );
    spDragController->RegisterDelegate( CGui::EVENT_DRAG_ENDED, m_spDelegateBroadcastDragEnded );

    // Set drop controller
    BroadcastBlockDropControllerPtr spDropController( new BroadcastBlockDropController( m_spContext, GetShownDate(), spBroadcastBlock, _eStartSlot ) );
    CGui::DragDropSystem::GetInstance().SetDropController( spBroadcastBlock->GetPanel(), spDropController );
    m_aspBroadcastDropController[ _eStartSlot ] = spDropController;

    return spBroadcastBlock;
}
        
void PlanningSchedulePanel::RemoveBroadcastBlock( Hour _eStartSlot )
{
    // Get broadcast block
    BroadcastBlockPtr spBroadcastBlock = GetBroadcastBlock( _eStartSlot );
    if ( spBroadcastBlock == NULL )
    {
        CF_WARN( "PlanningSchedulePanel::OnBroadcastRemoved: Broadcast removed but no broadcast block found." );
        return;
    }

    // Remove drop controller
    CGui::DragDropSystem::GetInstance().RemoveDropController( spBroadcastBlock->GetPanel() );

    // Remove delegates
    CGui::DragControllerPtr spDragController = CGui::DragDropSystem::GetInstance().GetDragController( spBroadcastBlock->GetPanel() );
    spDragController->RemoveDelegate( CGui::EVENT_DRAG_ENDED, m_spDelegateBroadcastDragEnded );
    spDragController->RemoveDelegate( CGui::EVENT_DRAG_STARTED, m_spDelegateBroadcastDragStarted );

    // Remove drag controller
    CGui::DragDropSystem::GetInstance().RemoveDragController( spBroadcastBlock->GetPanel() );

    // Remove from panel
    m_spPnlRoot->RemoveChild( spBroadcastBlock->GetPanel() );

    // Remove broadcast block
    m_aspBroadcastBlocks[ _eStartSlot ].reset();
}
        
BroadcastBlockPtr PlanningSchedulePanel::GetBroadcastBlock( Hour _eSlot ) const
{
    if ( _eSlot < HOUR_END )
    {
        return m_aspBroadcastBlocks[ _eSlot ];
    }
    else
    {
        return BroadcastBlockPtr();
    }
}

void PlanningSchedulePanel::AddAdvertBlock( AdvertPtr _spAdvert, Hour _eSlot )
{
    // Create advert block
    AdvertBlockPtr spBlock = AdvertBlock::Create( _spAdvert );

    // Add tooltip controller
    CGui::PanelPtr spPanel = spBlock->GetPanel();
    AdvertTooltipControllerPtr spTooltipController( new AdvertTooltipController( m_spContext, _spAdvert ) );
    CGui::TooltipSystem::GetInstance().SetTooltipController( spPanel, spTooltipController );

    // Add as child
    m_spPnlRoot->AddChildLast( spPanel );
    
    // Set to right position
    spPanel->SetOffset( m_aspAdvertSlot[ _eSlot ]->GetRelativePosition() );

    // Store
    m_aspAdvertBlocks[ _eSlot ] = spBlock;

    // Set drag controller
    spPanel->SetDraggable( true );
    AdvertDragControllerPtr spDragController( new AdvertDragController( spBlock, GetShownDate(), _eSlot ) );
    CGui::DragDropSystem::GetInstance().SetDragController( spPanel, spDragController );

    // Add delegates
    spDragController->RegisterDelegate( CGui::EVENT_DRAG_STARTED, m_spDelegateAdvertDragStarted );
    spDragController->RegisterDelegate( CGui::EVENT_DRAG_ENDED, m_spDelegateAdvertDragEnded );

    // Set drop controller
    AdvertDropControllerPtr spDropController( new AdvertDropController( m_spContext, GetShownDate(), _eSlot ) );
    CGui::DragDropSystem::GetInstance().SetDropController( spPanel, spDropController );
    m_aspAdvertDropController[ _eSlot ] = spDropController;

    // Update sub texts
    UpdateAdvertSubTexts();
}

void PlanningSchedulePanel::RemoveAdvertBlock( Hour _eSlot )
{
    // Get advert block
    AdvertBlockPtr spBlock = GetAdvertBlock( _eSlot );
    if ( spBlock == NULL )
    {
        CF_WARN( "PlanningSchedulePanel::RemoveAdvertBlock: Advert removed but no advert block found." );
        return;
    }

    // Get panel
    CGui::PanelPtr spPanel = spBlock->GetPanel();

    // Remove drop controller
    CGui::DragDropSystem::GetInstance().RemoveDropController( spPanel );

    // Remove delegates
    CGui::DragControllerPtr spDragController = CGui::DragDropSystem::GetInstance().GetDragController( spPanel );
    spDragController->RemoveDelegate( CGui::EVENT_DRAG_ENDED, m_spDelegateAdvertDragEnded );
    spDragController->RemoveDelegate( CGui::EVENT_DRAG_STARTED, m_spDelegateAdvertDragStarted );

    // Remove drag controller
    CGui::DragDropSystem::GetInstance().RemoveDragController( spPanel );

    // Remove from panel
    m_spPnlRoot->RemoveChild( spPanel );

    // Remove broadcast block
    m_aspAdvertBlocks[ _eSlot ].reset();

    // Update sub texts
    UpdateAdvertSubTexts();
}
        
AdvertBlockPtr PlanningSchedulePanel::GetAdvertBlock( Hour _eSlot ) const
{
    return m_aspAdvertBlocks[ _eSlot ];
}
