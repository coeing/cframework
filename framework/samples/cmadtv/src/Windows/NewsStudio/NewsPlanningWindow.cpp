#include "cmadtv/Windows/NewsStudio/NewsPlanningWindow.h"

#include "cfoundation/Math/Utils.h"

#include "cgui/DragDrop/DragDropSystem.h"

#include "cgui/Gui/Button.h"
#include "cgui/Gui/TextBox.h"

#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/News/News.h"
#include "cmadtv/Data/News/NewsChannel.h"
#include "cmadtv/Data/News/NewsInstance.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerCompNews.h"
#include "cmadtv/Data/Player/PlayerContext.h"

#include "cmadtv/DragDrop/BlockTrashDropController.h"
#include "cmadtv/DragDrop/NewsDragController.h"
#include "cmadtv/DragDrop/NewsDropController.h"
#include "cmadtv/Input/InputTranslator.h"
#include "cmadtv/Windows/NewsBlock.h"

using namespace CMadTV;
        
CFoundation::String NewsPlanningWindow::GetXMLFileName()
{
    return "data/windows/NewsPlanningWindow.xml";
}

NewsPlanningWindow::NewsPlanningWindow()
:   m_u32FirstNewsIdx( MAX_U32 ),
    m_u32NumNews( MAX_U32 )
{
    // Create delegates
    m_spDelegateChannelSelectionChanged = CCtrl::ButtonGroupButtonEventDelegate3Ptr( new CCtrl::ButtonGroupButtonEventDelegate3( this, &NewsPlanningWindow::OnChannelSelectionChanged ) );
    m_spDelegateButtonClicked = CGui::ObjectInputEventDelegate1Ptr( new CGui::ObjectInputEventDelegate1( this, &NewsPlanningWindow::OnButtonClicked ) );
    m_spDelegateNewsAdded = PlayerCompNewsEventNewsDelegate1Ptr( new PlayerCompNewsEventNewsDelegate1( this, &NewsPlanningWindow::OnNewsAdded ) );
    m_spDelegateNewsRemoved = PlayerCompNewsEventNewsDelegate1Ptr( new PlayerCompNewsEventNewsDelegate1( this, &NewsPlanningWindow::OnNewsRemoved ) );
    m_spDelegatePlannedNewsChanged = PlayerCompNewsEventU32NewsNewsDelegate2Ptr( new PlayerCompNewsEventU32NewsNewsDelegate2( this, &NewsPlanningWindow::OnPlannedNewsChanged ) );
}

NewsPlanningWindow::~NewsPlanningWindow()
{
}

void NewsPlanningWindow::InitWindow()
{        
    // Get up/down button
    m_spBtUp = GetButton( "BtUp" );
    SAFE_CALL( m_spBtUp )->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateButtonClicked );
    m_spBtDown = GetButton( "BtDown" );
    SAFE_CALL( m_spBtDown )->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateButtonClicked );

    // Get news
    for ( Unsigned32 u32Idx = 0; u32Idx < NUM_NEWS_BLOCKS; ++u32Idx )
    {
        CGui::PanelPtr spPnlNewsBlock = GetPanel( CFoundation::String::Build( "News%d", u32Idx + 1 ) );
        SAFE_CALL( spPnlNewsBlock )->SetDraggable( true );

        // Create drag controller
        NewsDragControllerPtr spDragController( new NewsDragController( NewsInstancePtr(), m_spPlayer ) );
        CGui::DragDropSystem::GetInstance().SetDragController( spPnlNewsBlock, spDragController );

        m_aspNewsBlocks[ u32Idx ] = NewsBlockPtr( new NewsBlock( spPnlNewsBlock ) );
        m_aspNewsBlockDragControllers[ u32Idx ] = spDragController;
    }

    // Get news slots
    for ( Unsigned32 u32Idx = 0; u32Idx < PlayerCompNews::NUM_PLANNED_NEWS; ++u32Idx )
    {
        CGui::PanelPtr spPnlNewsSlot = GetPanel( CFoundation::String::Build( "NewsSlot%d", u32Idx + 1 ) );

        // Create drop controller
        NewsDropControllerPtr spDropController( new NewsDropController( m_spContext, u32Idx ) );
        CGui::DragDropSystem::GetInstance().SetDropController( spPnlNewsSlot, spDropController );

        m_aspNewsSlots[ u32Idx ] = spPnlNewsSlot;
        m_aspNewsSlotDropControllers[ u32Idx ] = spDropController;
    }

    // Create button group
    m_spChannelButtons = CCtrl::ButtonGroupPtr( new CCtrl::ButtonGroup() );

    // Register delegate
    m_spChannelButtons->RegisterDelegate( CCtrl::BUTTON_GROUP_EVENT_SELECTION_CHANGED, m_spDelegateChannelSelectionChanged );

    // Setup trash can
    CGui::GuiObjectPtr spTrash = GetGuiObject( "ObjTrash" );
    BlockTrashDropControllerPtr spTrashDropController( new BlockTrashDropController() );
    CGui::DragDropSystem::GetInstance().SetDropController( spTrash, spTrashDropController );

    // Get buttons
    for ( Unsigned32 u32Idx = 0; u32Idx < MAX_CHANNELS; ++u32Idx )
    {
        CGui::ButtonPtr spBtNewsChannel = GetButton( CFoundation::String::Build( "BtNewsChannel%d", u32Idx + 1 ) );
        
        // Store channel ID
        m_mapChannelIDs[ spBtNewsChannel ] = u32Idx;
        m_mapChannelObjects[ u32Idx ] = spBtNewsChannel;

        // Add to button group
        if ( spBtNewsChannel != NULL )
        {
            m_spChannelButtons->AddButton( spBtNewsChannel );
        }
    }

    WindowController::InitWindow();
}
        
void NewsPlanningWindow::DeinitWindow()
{
    // Remove delegate
    m_spChannelButtons->RemoveDelegate( CCtrl::BUTTON_GROUP_EVENT_SELECTION_CHANGED, m_spDelegateChannelSelectionChanged );

    // Remove all buttons from button groups
    m_spChannelButtons->RemoveAllButtons();

    // Remove delegates
    SAFE_CALL( m_spBtUp )->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateButtonClicked );
    SAFE_CALL( m_spBtDown )->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateButtonClicked );

    // Remove player
    SetPlayer( PlayerPtr() );

    WindowController::DeinitWindow();
}
        
void NewsPlanningWindow::ProcessInput( const CSystem::InputState& _state )
{
    // Check if the window should be closed
    if ( InputTranslator::GetInstance().WasExecuted( INPUT_COMMAND_PLANNING_BROADCAST_ARCHIV_WINDOW_CLOSE, _state ) )
    {
        Close();
    }
}
        
void NewsPlanningWindow::OnContextChanged()
{
    SetPlayer( m_spContext != NULL ? m_spContext->GetPlayer() : PlayerPtr() );

    // Update drop controller
    for ( Unsigned32 u32Idx = 0; u32Idx < PlayerCompNews::NUM_PLANNED_NEWS; ++u32Idx )
    {
        // Setup drop controller
        NewsDropControllerPtr spDropController = m_aspNewsSlotDropControllers[ u32Idx ];
        spDropController->SetContext( m_spContext );
    }

    // Set active channel
    NewsChannelPtr spChannel = FindChannel( m_spChannelButtons->GetActiveButton() );
    SetActiveChannel( spChannel );
}
        
void NewsPlanningWindow::SetPlayer( PlayerPtr _spPlayer )
{
    if ( _spPlayer == m_spPlayer )
    {
        return;
    }

    // Remove delegates
    if ( m_spPlayer != NULL )
    {
        PlayerCompNewsPtr spCompNews = m_spPlayer->GetCompNews();
        if ( spCompNews != NULL )
        {
            spCompNews->RemoveDelegate( EVENT_NEWS_ADDED, m_spDelegateNewsAdded );
            spCompNews->RemoveDelegate( EVENT_NEWS_REMOVED, m_spDelegateNewsRemoved );
            spCompNews->RemoveDelegate( EVENT_NEWS_PLANNED_CHANGED, m_spDelegatePlannedNewsChanged );
        }
    }

    m_spPlayer = _spPlayer;  

    // Register delegates
    if ( m_spPlayer != NULL )
    {
        PlayerCompNewsPtr spCompNews = m_spPlayer->GetCompNews();
        if ( spCompNews != NULL )
        {
            spCompNews->RegisterDelegate( EVENT_NEWS_ADDED, m_spDelegateNewsAdded );
            spCompNews->RegisterDelegate( EVENT_NEWS_REMOVED, m_spDelegateNewsRemoved );
            spCompNews->RegisterDelegate( EVENT_NEWS_PLANNED_CHANGED, m_spDelegatePlannedNewsChanged );
        }
    }

    // Update drag controller
    for ( Unsigned32 u32Idx = 0; u32Idx < NUM_NEWS_BLOCKS; ++u32Idx )
    {
        // Setup drag controller
        NewsDragControllerPtr spDragController = m_aspNewsBlockDragControllers[ u32Idx ];
        spDragController->SetPlayer( m_spPlayer );
    }

    // Update channels
    UpdateChannelButtons();

    // Update num news
    UpdateNumNews();

    // Update news blocks
    UpdateNewsBlocks();

    // Update planned news
    PlayerCompNewsPtr spCompNews = m_spPlayer != NULL ? m_spPlayer->GetCompNews() : PlayerCompNewsPtr();
    for ( Unsigned32 u32Idx = 0; u32Idx < PlayerCompNews::NUM_PLANNED_NEWS; ++u32Idx )
    {
        SetPlannedNews( u32Idx, spCompNews != NULL ? spCompNews->GetPlannedNews( u32Idx ) : NewsInstancePtr() );
    }
}
       
NewsChannelPtr NewsPlanningWindow::FindChannel( CGui::ButtonPtr _spObject ) const
{
    // Get channel id
    ObjectChannelIDMap::const_iterator it = m_mapChannelIDs.find( _spObject );
    if ( m_spContext == NULL || 
         it == m_mapChannelIDs.end() )
    {
        return NewsChannelPtr();
    }

    // Get channel
    Unsigned32 u32NewsChannelID = it->second;
    return m_spContext->GetDatabase()->GetNewsChannel( u32NewsChannelID );
}
        
void NewsPlanningWindow::SetActiveChannel( NewsChannelPtr _spChannel )
{
    if ( _spChannel == m_spActiveChannel )
    {
        return;
    }

    m_spActiveChannel = _spChannel;

    // Update num news
    UpdateNumNews();

    // Reset first index
    SetFirstNewsIdx( 0 );

    // Update news blocks
    UpdateNewsBlocks();
}
       
void NewsPlanningWindow::SetFirstNewsIdx( Unsigned32 _u32Idx )
{
    // Clamp new index
    CLAMP( _u32Idx, 0, m_u32NumNews > NUM_NEWS_BLOCKS ? m_u32NumNews - NUM_NEWS_BLOCKS : 0 );

    if ( _u32Idx == m_u32FirstNewsIdx )
    {
        return;
    }

    m_u32FirstNewsIdx = _u32Idx;

    UpdateScrollButtons();
    UpdateNewsBlocks();
}
      
void NewsPlanningWindow::SetNumNews( Unsigned32 _u32NumNews )
{
    if ( _u32NumNews == m_u32NumNews )
    {
        return;
    }

    m_u32NumNews = _u32NumNews;

    // Clamp new index
    Unsigned32 u32FirstNewsIdx = m_u32FirstNewsIdx; 
    CLAMP( u32FirstNewsIdx, 0, m_u32NumNews > NUM_NEWS_BLOCKS ? m_u32NumNews - NUM_NEWS_BLOCKS : 0 );
    SetFirstNewsIdx( u32FirstNewsIdx );

    // Update scroll buttons
    UpdateScrollButtons();
}
        
void NewsPlanningWindow::UpdateChannelButtons() const
{
    // Check news
    PlayerCompNewsPtr spCompNews = m_spPlayer != NULL ? m_spPlayer->GetCompNews() : PlayerCompNewsPtr();
    if ( spCompNews == NULL )
    {
        return;
    }

    // Check all channels
    Unsigned32 u32NumChannels = m_spContext->GetDatabase()->GetNumNewsChannels();
    CGui::ButtonPtr spNewActiveButton;
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumChannels; ++u32Idx )
    {
        NewsChannelPtr spChannel = m_spContext->GetDatabase()->GetNewsChannel( u32Idx );
        ChannelIDObjectMap::const_iterator it = m_mapChannelObjects.find( u32Idx );
        if ( it == m_mapChannelObjects.end() )
        {
            continue;
        }

        // Get news for channel
        std::vector< NewsInstancePtr > aspNews;
        spCompNews->GetNews( spChannel, aspNews );
        bool bHasNews = aspNews.size() > 0;

        CGui::ButtonPtr spBtChannel = it->second;
        spBtChannel->SetVisible( bHasNews );

        // Check if should be used as active button
        if ( bHasNews &&
             ( spNewActiveButton == NULL || spChannel == m_spActiveChannel ) )
        {
            spNewActiveButton = spBtChannel;
        }
    }

    m_spChannelButtons->SetActiveButton( spNewActiveButton );
}

void NewsPlanningWindow::UpdateScrollButtons() const
{
    SAFE_CALL( m_spBtUp )->SetVisible( m_u32FirstNewsIdx > 0 );
    SAFE_CALL( m_spBtDown )->SetVisible( m_u32NumNews > NUM_NEWS_BLOCKS && m_u32FirstNewsIdx < m_u32NumNews - NUM_NEWS_BLOCKS );
}
        
void NewsPlanningWindow::UpdateNewsBlocks() const
{
    // Get news
    std::vector< NewsInstancePtr > aspNews;
    PlayerCompNewsPtr spCompNews = m_spPlayer != NULL ? m_spPlayer->GetCompNews() : PlayerCompNewsPtr();
    if ( spCompNews != NULL )
    {
        spCompNews->GetNews( m_spActiveChannel, aspNews );
    }

    // Fill blocks
    for ( Unsigned32 u32Idx = m_u32FirstNewsIdx; u32Idx < m_u32FirstNewsIdx + NUM_NEWS_BLOCKS; ++u32Idx )
    {
        NewsInstancePtr spNews = u32Idx < aspNews.size() ? aspNews[ u32Idx ] : NewsInstancePtr();
        NewsBlockPtr spNewsBlock = m_aspNewsBlocks[ u32Idx - m_u32FirstNewsIdx ];

        // Setup news block
        if ( spNewsBlock != NULL )
        {
            spNewsBlock->SetVisible( spNews != NULL );
            spNewsBlock->SetNewsInstance( spNews );
        }

        // Setup drag controller        
        NewsDragControllerPtr spDragController = m_aspNewsBlockDragControllers[ u32Idx - m_u32FirstNewsIdx ];
        spDragController->SetNews( spNews );
    }   
}
       
void NewsPlanningWindow::UpdateNumNews()
{
    // Get news
    std::vector< NewsInstancePtr > aspNews;
    PlayerCompNewsPtr spCompNews = m_spPlayer != NULL ? m_spPlayer->GetCompNews() : PlayerCompNewsPtr();
    if ( spCompNews != NULL )
    {
        spCompNews->GetNews( m_spActiveChannel, aspNews );
    }

    // Set number of news
    Unsigned32 u32NumNews = aspNews.size();
    SetNumNews( u32NumNews );
}
       
void NewsPlanningWindow::SetPlannedNews( Unsigned32 _u32Slot, NewsInstancePtr _spNews )
{
    if ( _u32Slot >= PlayerCompNews::NUM_PLANNED_NEWS )
    {
        return;
    }

    NewsBlockPtr spNewsBlock = m_aspPlannedNewsBlocks[ _u32Slot ];
    NewsInstancePtr spNews = spNewsBlock != NULL ? spNewsBlock->GetNewsInstance() : NewsInstancePtr();
    if ( spNews == _spNews )
    {
        return;
    }

    if ( _spNews != NULL )
    {
        if ( spNewsBlock == NULL )
        {
            spNewsBlock = NewsBlock::Create( _spNews );
            m_aspPlannedNewsBlocks[ _u32Slot ] = spNewsBlock;
            m_aspNewsSlots[ _u32Slot ]->AddChildLast( spNewsBlock->GetPanel() );

            // Set drag controller
            NewsDragControllerPtr spDragController( new NewsDragController( _spNews, m_spPlayer, _u32Slot ) );
            CGui::DragDropSystem::GetInstance().SetDragController( spNewsBlock->GetPanel(), spDragController );            
            spNewsBlock->GetPanel()->SetDraggable( true );
            m_aspPlannedNewsBlockDragControllers[ _u32Slot ] = spDragController;
        }
    }

    NewsDragControllerPtr spDragController = m_aspPlannedNewsBlockDragControllers[ _u32Slot ];
    SAFE_CALL( spDragController )->SetNews( _spNews );

    SAFE_CALL( spNewsBlock )->SetVisible( _spNews != NULL );
    SAFE_CALL( spNewsBlock )->SetNewsInstance( _spNews );
}

void NewsPlanningWindow::OnChannelSelectionChanged( CCtrl::ButtonGroupPtr _spButtonGroup, CGui::ButtonPtr _spNewActiveButton, CGui::ButtonPtr _spOldActiveButton )
{
    NewsChannelPtr spChannel = FindChannel( _spNewActiveButton );
    SetActiveChannel( spChannel );
}
       
void NewsPlanningWindow::OnButtonClicked( CGui::GuiObjectPtr _spButton )
{
    if ( _spButton == m_spBtUp )
    {
        if ( m_u32FirstNewsIdx > 0 )
        {
            SetFirstNewsIdx( m_u32FirstNewsIdx - 1 );
        }
    }
    else if ( _spButton == m_spBtDown )
    {
        SetFirstNewsIdx( m_u32FirstNewsIdx + 1 );
    }
}

void NewsPlanningWindow::OnNewsAdded( NewsInstancePtr _spNews )
{
    if ( _spNews == NULL )
    {
        return;
    }

    // Check if of active channel
    NewsChannelPtr spChannel = _spNews->GetNews()->GetChannel();
    if ( spChannel == m_spActiveChannel )
    {
        // Update num news
        UpdateNumNews();

        UpdateNewsBlocks();
    }

    UpdateChannelButtons();
}

void NewsPlanningWindow::OnNewsRemoved( NewsInstancePtr _spNews )
{
    if ( _spNews == NULL )
    {
        return;
    }

    // Check if of active channel
    NewsChannelPtr spChannel = _spNews->GetNews()->GetChannel();
    if ( spChannel == m_spActiveChannel )
    {
        // Update num news
        UpdateNumNews();

        UpdateNewsBlocks();
    }
    
    UpdateChannelButtons();
}
        
void NewsPlanningWindow::OnPlannedNewsChanged( Unsigned32 _u32Slot, NewsInstancePtr _spNews )
{
    SetPlannedNews( _u32Slot, _spNews );
}
