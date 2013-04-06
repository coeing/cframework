#include "cmadtv/Windows/PlanningRackShelfWindow.h"

#include "cgui/DragDrop/DragDropSystem.h"
#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/TextBox.h"
#include "cgui/Input/Input.h"
#include "cgui/Tooltip/TooltipSystem.h"

#include "cmadtv/Data/Broadcast.h"
#include "cmadtv/Data/Genre.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerContext.h"

#include "cmadtv/Input/InputTranslator.h"
#include "cmadtv/DragDrop/BroadcastDragController.h"
#include "cmadtv/Tooltip/BroadcastTooltipController.h"

using namespace CMadTV;
        
CFoundation::String PlanningRackShelfWindow::GetXMLFileName()
{
    return "data/windows/PlanningRackShelfWindow.xml";
}

PlanningRackShelfWindow::PlanningRackShelfWindow()
{
}

PlanningRackShelfWindow::~PlanningRackShelfWindow()
{
}

void PlanningRackShelfWindow::InitWindow()
{
    // Get controls
    m_spTbGenre = GetTextBox( "TbGenre" );

    m_spPnlTemplateBroadcast = GetPanel( "PnlTemplateBroadcast" );
    m_spPnlTemplateBroadcast->RemoveFromParent();

    m_spObjBroadcastsAnchor = GetGuiObject( "ObjBroadcastsAnchor" );

    WindowController::InitWindow();
}

void PlanningRackShelfWindow::Init( PlayerPtr _spPlayer, GenrePtr _spGenre )
{
    m_spPlayer = _spPlayer;

    // Set title
    m_spTbGenre->SetText( _spGenre->GetName() + ":" );

    // Get broadcasts of passed genre
    BroadcastRack& clBroadcastRack = m_spPlayer->GetBroadcastRack();
    BroadcastVec apBroadcasts;
    clBroadcastRack.GetBroadcasts( _spGenre->GetID(), apBroadcasts );

    // Create Broadcasts
    Unsigned32 u32NumBroadcasts = apBroadcasts.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumBroadcasts; ++u32Idx )
    {
        BroadcastPtr pBroadcast = apBroadcasts[ u32Idx ];

        BroadcastPanelPtr spBroadcastPanel = CreateBroadcastPanel();
        spBroadcastPanel->SetBroadcast( pBroadcast );
        m_aspBroadcastPanels.push_back( spBroadcastPanel );
    }
}
		
void PlanningRackShelfWindow::DeinitWindow()
{
    Unsigned32 u32NumPanels = m_aspBroadcastPanels.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPanels; ++u32Idx )
    {
        BroadcastPanelPtr spBroadcastPanel = m_aspBroadcastPanels[ u32Idx ];
        spBroadcastPanel->SetBroadcast( BroadcastPtr() );
    }
    m_aspBroadcastPanels.clear();

    // Delete template
    CGui::GuiMain::GetInstance().Delete( m_spPnlTemplateBroadcast );

    WindowController::DeinitWindow();
}
        
void PlanningRackShelfWindow::ProcessInput( const CSystem::InputState& _state )
{
    // Check if the window should be closed
    if ( InputTranslator::GetInstance().WasExecuted( INPUT_COMMAND_PLANNING_RACK_SHELF_WINDOW_CLOSE, _state ) )
    {
        Close();
    }
}
		
void PlanningRackShelfWindow::Close()
{
    // Make unmodal
    CCtrl::WindowManager::GetInstance().SetModalWindow( CCtrl::WindowControllerPtr() );

    WindowController::Close();
}
       
PlanningRackShelfWindow::BroadcastPanelPtr PlanningRackShelfWindow::CreateBroadcastPanel()
{
    // Create gui elements
    CGui::PanelPtr spGUIPanel = CGui::GuiMain::GetInstance().CreateFromTemplate( "PnlBroadcastX", m_spPnlTemplateBroadcast );
    m_spObjBroadcastsAnchor->GetParent()->AddChildAfter( spGUIPanel, m_spObjBroadcastsAnchor );
    m_spObjBroadcastsAnchor = spGUIPanel;

    BroadcastPanelPtr spBroadcastPanel( new BroadcastPanel() );
    spBroadcastPanel->Init( spGUIPanel );
    spBroadcastPanel->SetContext( m_spContext );
    
    return spBroadcastPanel;
}

PlanningRackShelfWindow::BroadcastPanel::BroadcastPanel()
{
    m_spTooltipController = BroadcastTooltipControllerPtr( new BroadcastTooltipController() );
    m_spDragController = BroadcastDragControllerPtr( new BroadcastDragController() );
}

PlanningRackShelfWindow::BroadcastPanel::~BroadcastPanel()
{
}

void PlanningRackShelfWindow::BroadcastPanel::Init( CGui::PanelPtr _spGuiPanel )
{
    m_spPnlRoot = _spGuiPanel;
    m_spTbTitle = static_pointer_cast< CGui::TextBox >( m_spPnlRoot->FindChild( "TbTitle" ) );
    CF_WARN_IF( m_spTbTitle == NULL, L"PlanningRackShelfWindow::BroadcastPanel::Init: Found no TbBroadcast textbox in Broadcast panel" );

    // Set tooltip controller
    CGui::TooltipSystem::GetInstance().SetTooltipController( m_spPnlRoot, m_spTooltipController );

    // Add drag controller and set draggable
    m_spPnlRoot->SetDraggable( true );
    CGui::DragDropSystem::GetInstance().SetDragController( m_spPnlRoot, m_spDragController );
}

void PlanningRackShelfWindow::BroadcastPanel::SetContext( PlayerContextPtr _spContext )
{
    m_spContext = _spContext;
    m_spTooltipController->SetContext( m_spContext );
    m_spTooltipController->SetOwner( m_spContext != NULL ? m_spContext->GetPlayer() : PlayerPtr() );
}

void PlanningRackShelfWindow::BroadcastPanel::SetBroadcast( BroadcastPtr _spBroadcast )
{
    if ( m_spBroadcast != NULL )
    {
        // Remove tooltip controller
        CGui::TooltipSystem::GetInstance().RemoveTooltipController( m_spPnlRoot );

        // Remove drag controller
        CGui::DragDropSystem::GetInstance().RemoveDragController( m_spPnlRoot );
    }

    m_spBroadcast = _spBroadcast;

    if ( m_spBroadcast != NULL )
    {
        // Set name
        SAFE_CALL( m_spTbTitle )->SetText( m_spBroadcast->GetTitle() );
    }

    m_spTooltipController->SetBroadcast( m_spBroadcast );
    m_spDragController->SetBroadcast( m_spBroadcast );
}
