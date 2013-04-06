#include "cmadtv/Windows/PlanningAdvertContractsWindow.h"

#include "cfoundation/Debug/Makros.h"

#include "cgui/DragDrop/DragDropSystem.h"
#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/TextBox.h"
#include "cgui/Gui/Window.h"
#include "cgui/Input/Input.h"
#include "cgui/Tooltip/TooltipSystem.h"

#include "cmadtv/Data/Advert.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/AdvertContract.h"
#include "cmadtv/Data/Player/AdvertContracts.h"
#include "cmadtv/Data/Player/PlayerContext.h"

#include "cmadtv/DragDrop/AdvertDragController.h"
#include "cmadtv/Input/InputTranslator.h"
#include "cmadtv/Tooltip/AdvertTooltipController.h"
#include "cmadtv/Windows/PlanningRackShelfWindow.h"

using namespace CMadTV;
        
CFoundation::String PlanningAdvertContractsWindow::GetXMLFileName()
{
    return "data/windows/PlanningAdvertContractsWindow.xml";
}

PlanningAdvertContractsWindow::PlanningAdvertContractsWindow()
{
    m_spDelegateAdvertContractDragStarted = AdvertContractEventDelegatePtr( new AdvertContractEventDelegate( this, &PlanningAdvertContractsWindow::OnAdvertContractDragStarted ) );
    m_spDelegateAdvertContractDragEnded = AdvertContractEventDelegatePtr( new AdvertContractEventDelegate( this, &PlanningAdvertContractsWindow::OnAdvertContractDragEnded ) );
}

PlanningAdvertContractsWindow::~PlanningAdvertContractsWindow()
{
}

void PlanningAdvertContractsWindow::InitWindow()
{
    // Get controls
    m_spPnlTemplateAdvert = GetPanel( "PnlTemplateAdvert" );
    m_spPnlTemplateAdvert->RemoveFromParent();
    m_spObjAdvertsAnchor = GetGuiObject( "ObjAdvertsAnchor" );

    WindowController::InitWindow();
}
		
void PlanningAdvertContractsWindow::DeinitWindow()
{
    // Delete template
    CGui::GuiMain::GetInstance().Delete( m_spPnlTemplateAdvert );

    WindowController::DeinitWindow();
}
        
void PlanningAdvertContractsWindow::ProcessInput( const CSystem::InputState& _state )
{
    // Check if the window should be closed
    if ( InputTranslator::GetInstance().WasExecuted( INPUT_COMMAND_PLANNING_ADVERT_ARCHIV_WINDOW_CLOSE, _state ) )
    {
        Close();
    }
}

void PlanningAdvertContractsWindow::OnContextChanged()
{
    SetPlayer( m_spContext != NULL ? m_spContext->GetPlayer() : PlayerPtr() );
}
       
void PlanningAdvertContractsWindow::SetPlayer( PlayerPtr _spPlayer )
{
    if ( _spPlayer == m_spPlayer )
    {
        return;
    }

    if ( m_spPlayer != NULL )
    {
        // Remove all contract panels
        RemoveAllAdvertContractPanels();
    }

    m_spPlayer = _spPlayer;

    if ( m_spPlayer != NULL )
    {
        // Create advert contrats
        AdvertContracts& clContracts = m_spPlayer->GetAdvertContracts();
        Unsigned32 u32NumContracts = clContracts.GetNumContracts();
        for ( Unsigned32 u32Idx = 0; u32Idx < u32NumContracts; ++u32Idx )
        {
            AdvertContractPtr spContract = clContracts.GetContract( u32Idx );

            // Create advert panel
            AdvertContractPanelPtr spAdvertContractPanel = CreateAdvertContractPanel();
            spAdvertContractPanel->SetContract( spContract );
            m_aspAdvertContractPanels.push_back( spAdvertContractPanel );
        }
    }
}
       
PlanningAdvertContractsWindow::AdvertContractPanelPtr PlanningAdvertContractsWindow::CreateAdvertContractPanel()
{
    // Create gui elements
    CGui::PanelPtr spGUIPanel = CGui::GuiMain::GetInstance().CreateFromTemplate( "PnlAdvertX", m_spPnlTemplateAdvert );
    m_spObjAdvertsAnchor->GetParent()->AddChildAfter( spGUIPanel, m_spObjAdvertsAnchor );
    m_spObjAdvertsAnchor = spGUIPanel;

    AdvertContractPanelPtr spAdvertContractPanel( new AdvertContractPanel() );
    spAdvertContractPanel->Init( spGUIPanel );
    spAdvertContractPanel->SetContext( m_spContext );

    // Register delegates
    spAdvertContractPanel->RegisterDelegate( ADVERT_CONTRACT_EVENT_DRAG_STARTED, m_spDelegateAdvertContractDragStarted );
    spAdvertContractPanel->RegisterDelegate( ADVERT_CONTRACT_EVENT_DRAG_ENDED, m_spDelegateAdvertContractDragEnded );
    
    return spAdvertContractPanel;
}

void PlanningAdvertContractsWindow::RemoveAllAdvertContractPanels()
{    
    // Remove Advert panels
    for ( AdvertContractPanelVec::iterator it = m_aspAdvertContractPanels.begin(); it != m_aspAdvertContractPanels.end(); ++it )
    {
        AdvertContractPanelPtr spAdvertContractPanel = *it;

        // Remove delegates
        spAdvertContractPanel->RemoveDelegate( ADVERT_CONTRACT_EVENT_DRAG_STARTED, m_spDelegateAdvertContractDragStarted );
        spAdvertContractPanel->RemoveDelegate( ADVERT_CONTRACT_EVENT_DRAG_ENDED, m_spDelegateAdvertContractDragEnded );

        spAdvertContractPanel->Deinit();
    }
    m_aspAdvertContractPanels.clear();
}

void PlanningAdvertContractsWindow::OnAdvertContractDragStarted()
{
    // Hide
    GetWindow()->SetVisible( false );
}

void PlanningAdvertContractsWindow::OnAdvertContractDragEnded()
{
    // Show
    GetWindow()->SetVisible( true );
}

PlanningAdvertContractsWindow::AdvertContractPanel::AdvertContractPanel()
{
    m_spDragController = AdvertDragControllerPtr( new AdvertDragController( ADVERT_DRAG_TYPE_BLOCK ) );

    m_spDelegateDragStarted = CGui::DragEventDelegatePtr( new CGui::DragEventDelegate( this, &AdvertContractPanel::OnDragStarted ) );
    m_spDragController->RegisterDelegate( CGui::EVENT_DRAG_STARTED, m_spDelegateDragStarted );
    m_spDelegateDragEnded = CGui::DragEventDelegatePtr( new CGui::DragEventDelegate( this, &AdvertContractPanel::OnDragEnded ) );
    m_spDragController->RegisterDelegate( CGui::EVENT_DRAG_ENDED, m_spDelegateDragEnded );
}

PlanningAdvertContractsWindow::AdvertContractPanel::~AdvertContractPanel()
{
}

void PlanningAdvertContractsWindow::AdvertContractPanel::Init( CGui::PanelPtr _spGuiPanel )
{
    m_spPnlRoot = _spGuiPanel;
    m_spTbName = static_pointer_cast< CGui::TextBox >( m_spPnlRoot->FindChild( "TbName" ) );

    // Set drag controller
    m_spPnlRoot->SetDraggable( true );
    CGui::DragDropSystem::GetInstance().SetDragController( m_spPnlRoot, m_spDragController );
}
        
void PlanningAdvertContractsWindow::AdvertContractPanel::Deinit()
{
    // Remove drag controller
    CGui::DragDropSystem::GetInstance().RemoveDragController( m_spPnlRoot );
}
        
void PlanningAdvertContractsWindow::AdvertContractPanel::SetContext( PlayerContextPtr _spContext )
{
    m_spContext = _spContext;
}

void PlanningAdvertContractsWindow::AdvertContractPanel::SetContract( AdvertContractPtr _spContract )
{
    m_spContract = _spContract;

    // Get advert
    AdvertPtr spAdvert = m_spContract != NULL ? m_spContract->GetAdvert() : AdvertPtr();

    // Set tooltip controller
    AdvertTooltipControllerPtr spTooltipController( new AdvertTooltipController( m_spContext, spAdvert ) );
    CGui::TooltipSystem::GetInstance().SetTooltipController( m_spPnlRoot, spTooltipController );

    // Set advert of drag controller
    m_spDragController->SetAdvert( spAdvert );

    // Set name
    if ( m_spTbName != NULL )
    {
        m_spTbName->SetText( m_spContract->GetAdvert()->GetName() );
    }
}

void PlanningAdvertContractsWindow::AdvertContractPanel::OnDragStarted( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spGuiObject, CGui::DragDataPtr _spDragData )
{
    DispatchEvent( ADVERT_CONTRACT_EVENT_DRAG_STARTED );
}

void PlanningAdvertContractsWindow::AdvertContractPanel::OnDragEnded( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spGuiObject, CGui::DragDataPtr _spDragData )
{
    DispatchEvent( ADVERT_CONTRACT_EVENT_DRAG_ENDED );
}
