#include "cmadtv/Windows/Library/LibraryWindow.h"

#include "cgui/DragDrop/DragDropSystem.h"
#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/Image.h"
#include "cgui/Gui/TextBox.h"
#include "cgui/Tooltip/TooltipSystem.h"

#include "cmadtv/Data/Broadcast.h"
#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Player/BroadcastBag.h"
#include "cmadtv/Data/Player/BroadcastRack.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerContext.h"

#include "cmadtv/DragDrop/LibraryDropController.h"
#include "cmadtv/DragDrop/DealerBroadcastDragController.h"
#include "cmadtv/Tooltip/BroadcastTooltipController.h"
#include "cmadtv/Windows/PlanningArchivWindow.h"
#include "cmadtv/Windows/BroadcastDealer/BroadcastBagPanel.h"

using namespace CMadTV;
        
CFoundation::String LibraryWindow::GetXMLFileName()
{
    return "data/windows/LibraryWindow.xml";
}

LibraryWindow::LibraryWindow()
{
    // Create delegates
    m_spDelegateLibrarianClicked = CGui::ObjectInputEventDelegate0Ptr( new CGui::ObjectInputEventDelegate0( this, &LibraryWindow::OnLibrarianClicked ) );
    m_spDelegateShelfClicked = GenrePanelEventDelegate1Ptr( new GenrePanelEventDelegate1( this, &LibraryWindow::OnShelfClicked ) );
    m_spDelegateBagDragStarted = BagPanelEventDelegate1Ptr( new BagPanelEventDelegate1( this, &LibraryWindow::OnBagDragStarted ) );

    // Create drag/drop controllers    
    m_spBagDropController = LibraryDropControllerPtr( new LibraryDropController( m_spContext, true ) );
    m_spRackDropController = LibraryDropControllerPtr( new LibraryDropController( m_spContext, false ) );
}

LibraryWindow::~LibraryWindow()
{
}

void LibraryWindow::InitWindow()
{
    // Get librarian
    m_spObjLibrarian = GetGuiObject( "ObjLibrarian" );
    m_spObjLibrarian->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateLibrarianClicked );
    CGui::DragDropSystem::GetInstance().SetDropController( m_spObjLibrarian, m_spRackDropController );

    CGui::PanelPtr spPnlRackShelf = GetPanel( "PnlRackShelf" );
    m_spRackShelf = RackShelfPtr( new RackShelf() );
    m_spRackShelf->Init( spPnlRackShelf );

    // Create bag
    CGui::PanelPtr spPnlBag = GetPanel( "PnlBag" );
    CGui::DragDropSystem::GetInstance().SetDropController( spPnlBag, m_spBagDropController );
    m_spBag = BroadcastBagPanelPtr( new BroadcastBagPanel( spPnlBag ) );  
    m_spBag->RegisterDelegate( EVENT_BAG_PANEL_DRAG_STARTED, m_spDelegateBagDragStarted );

    WindowController::InitWindow();
}
      		
void LibraryWindow::DeinitWindow()
{
    // Deinit bag
    m_spBag->RemoveDelegate( EVENT_BAG_PANEL_DRAG_STARTED, m_spDelegateBagDragStarted );
    CGui::DragDropSystem::GetInstance().RemoveDropController( m_spBag->GetObject() );

    /// Remove delegates
    m_spObjLibrarian->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateLibrarianClicked );
    CGui::DragDropSystem::GetInstance().RemoveDropController( m_spObjLibrarian );

    WindowController::DeinitWindow();
}
        
void LibraryWindow::OnContextChanged()
{
    SetPlayer( m_spContext != NULL ? m_spContext->GetPlayer() : PlayerPtr() );

    // Setup members
    m_spBag->SetContext( m_spContext );
    m_spRackShelf->SetContext( m_spContext );
    m_spBagDropController->SetContext( m_spContext );
    m_spRackDropController->SetContext( m_spContext );
    SAFE_CALL( m_spBroadcastRack )->SetContext( m_spContext );
}
        
void LibraryWindow::SetPlayer( PlayerPtr _spPlayer )
{
    m_spPlayer = _spPlayer;
}

void LibraryWindow::OnLibrarianClicked()
{
    // Open broadcast rack if not open already
    if ( m_spBroadcastRack == NULL )
    {
        m_spBroadcastRack = Create< PlanningArchivWindow >( GetSharedPtr() );
        m_spBroadcastRack->SetContext( m_spContext );
        m_spBroadcastRack->SetDelegateShelfClicked( m_spDelegateShelfClicked );
    }
}
       
void LibraryWindow::OnShelfClicked( GenrePtr _spGenre )
{
    if ( m_spBroadcastRack != NULL )
    {
        m_spBroadcastRack->RequestClose();
        m_spBroadcastRack.reset();
    }

    m_spRackShelf->SetGenre( _spGenre );
}
       
void LibraryWindow::OnBagDragStarted( BroadcastPtr _spBroadcast )
{
    // Switch to genre of broadcast
    if ( _spBroadcast != NULL )
    {
        m_spRackShelf->SetGenre( _spBroadcast->GetGenre() );
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
///
/// LibraryWindow::BroadcastObject
///
////////////////////////////////////////////////////////////////////////////////////////////

LibraryWindow::BroadcastObject::BroadcastObject()
{
    // Create delegates
    m_spDelegateDragStarted = CGui::DragEventDelegatePtr( new CGui::DragEventDelegate( this, &LibraryWindow::BroadcastObject::OnDragStarted ) );
    m_spDelegateDragUndone = CGui::DragEventDelegatePtr( new CGui::DragEventDelegate( this, &LibraryWindow::BroadcastObject::OnDragUndone ) );

    m_spBroadcastDragController = DealerBroadcastDragControllerPtr( new DealerBroadcastDragController() );
    m_spBroadcastDragController->RegisterDelegate( CGui::EVENT_DRAG_STARTED, m_spDelegateDragStarted );
    m_spBroadcastDragController->RegisterDelegate( CGui::EVENT_DRAG_UNDONE, m_spDelegateDragUndone );
    m_spBroadcastTooltipController = BroadcastTooltipControllerPtr( new BroadcastTooltipController() );
}
        
LibraryWindow::BroadcastObject::~BroadcastObject()
{
    m_spBroadcastDragController->RemoveDelegate( CGui::EVENT_DRAG_UNDONE, m_spDelegateDragUndone );
    m_spBroadcastDragController->RemoveDelegate( CGui::EVENT_DRAG_STARTED, m_spDelegateDragStarted );
}

void LibraryWindow::BroadcastObject::Init( CGui::GuiObjectPtr _spObject )
{
    if ( m_spObject != NULL )
    {
        CGui::DragDropSystem::GetInstance().RemoveDragController( m_spObject );
        CGui::TooltipSystem::GetInstance().RemoveTooltipController( m_spObject );
    }

    m_spObject = _spObject;

    if ( m_spObject != NULL )
    {
        m_spObject->SetDraggable( true );
        CGui::DragDropSystem::GetInstance().SetDragController( m_spObject, m_spBroadcastDragController );
        CGui::TooltipSystem::GetInstance().SetTooltipController( m_spObject, m_spBroadcastTooltipController );
    }
}
        
void LibraryWindow::BroadcastObject::SetContext( PlayerContextPtr _spContext )
{
    m_spBroadcastTooltipController->SetContext( _spContext );
    m_spBroadcastTooltipController->SetOwner( _spContext != NULL ? _spContext->GetPlayer() : PlayerPtr() );
}

void LibraryWindow::BroadcastObject::SetBroadcast( BroadcastPtr _spBroadcast )
{
    if ( m_spObject != NULL )
    {
        m_spObject->SetVisible( _spBroadcast != NULL );
    }
    m_spBroadcastDragController->SetBroadcast( _spBroadcast );
    m_spBroadcastTooltipController->SetBroadcast( _spBroadcast );
}
        
BroadcastPtr LibraryWindow::BroadcastObject::GetBroadcast() const
{
    return m_spBroadcastTooltipController->GetBroadcast();
}
        
CGui::GuiObjectPtr LibraryWindow::BroadcastObject::GetObject() const
{
    return m_spObject;
}

void LibraryWindow::BroadcastObject::OnDragStarted( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spGuiObject, CGui::DragDataPtr _spDragData )
{
    m_spObject->SetVisible( false );

    // Dispatch event
    DispatchEvent( EVENT_LIBRARY_BROADCAST_OBJECT_DRAG_STARTED, GetBroadcast() );
}

void LibraryWindow::BroadcastObject::OnDragUndone( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spGuiObject, CGui::DragDataPtr _spDragData )
{
    m_spObject->SetVisible( true );
}

////////////////////////////////////////////////////////////////////////////////////////////
///
/// LibraryWindow::RackShelf
///
////////////////////////////////////////////////////////////////////////////////////////////

LibraryWindow::RackShelf::RackShelf()
{
    // Create delegates
    m_spDelegateBroadcastAdded = BroadcastRackEventDelegate1Ptr( new BroadcastRackEventDelegate1( this, &RackShelf::OnBroadcastAdded ) );
    m_spDelegateBroadcastRemoved = BroadcastRackEventDelegate1Ptr( new BroadcastRackEventDelegate1( this, &RackShelf::OnBroadcastRemoved ) );
    
    m_spDropController = LibraryDropControllerPtr( new LibraryDropController( m_spContext, false ) );
}
        
LibraryWindow::RackShelf::~RackShelf()
{
    CGui::DragDropSystem::GetInstance().RemoveDropController( m_spPnlRoot );

    SetPlayer( PlayerPtr() );
}

void LibraryWindow::RackShelf::Init( CGui::PanelPtr _spPnlRoot )
{
    if ( m_spPnlRoot != NULL )
    {
        CGui::DragDropSystem::GetInstance().RemoveDropController( m_spPnlRoot );
    }

    m_spPnlRoot = _spPnlRoot;

    if ( m_spPnlRoot != NULL )
    {
        m_spObjBroadcastAnchor = m_spObjCurrentBroadcastAnchor = m_spPnlRoot->FindChild( "ObjBroadcastAnchor" );
        m_spObjBroadcastTemplate = m_spPnlRoot->FindChild( "ObjBroadcastTemplate" );
        m_spObjBroadcastTemplate->RemoveFromParent();

        m_spPnlGenre = m_spPnlRoot->FindChild( "PnlGenre" );
        m_spPnlGenre->SetVisible( false );
        m_spTbGenre = static_pointer_cast< CGui::TextBox >( m_spPnlRoot->FindDescendant( "TbGenre" ) );

        CGui::DragDropSystem::GetInstance().SetDropController( m_spPnlRoot, m_spDropController );  
    }
}
        
void LibraryWindow::RackShelf::SetContext( PlayerContextPtr _spContext )
{
    m_spContext = _spContext;

    // Setup drop controller
    m_spDropController->SetContext( m_spContext );

    // Set player
    SetPlayer( m_spContext != NULL ? m_spContext->GetPlayer() : PlayerPtr() );
}
       
void LibraryWindow::RackShelf::SetPlayer( PlayerPtr _spPlayer )
{
    if ( _spPlayer == m_spPlayer )
    {
        return;
    }

    if ( m_spPlayer != NULL )
    {        
        // Remove delegates
        BroadcastRack& clRack = m_spPlayer->GetBroadcastRack();
        clRack.RemoveDelegate( BROADCAST_RACK_EVENT_BROADCAST_ADDED, m_spDelegateBroadcastAdded );
        clRack.RemoveDelegate( BROADCAST_RACK_EVENT_BROADCAST_REMOVED, m_spDelegateBroadcastRemoved );
    }

    m_spPlayer = _spPlayer;

    if ( m_spPlayer != NULL )
    {        
        // Register delegates
        BroadcastRack& clRack = m_spPlayer->GetBroadcastRack();
        clRack.RegisterDelegate( BROADCAST_RACK_EVENT_BROADCAST_ADDED, m_spDelegateBroadcastAdded );
        clRack.RegisterDelegate( BROADCAST_RACK_EVENT_BROADCAST_REMOVED, m_spDelegateBroadcastRemoved );
    }

    // Update broadcasts
    UpdateBroadcasts();
}
		        
void LibraryWindow::RackShelf::SetGenre( GenrePtr _spGenre )
{
    if ( _spGenre == m_spGenre )
    {
        return;
    }

    m_spGenre = _spGenre;

    // Update broadcasts
    UpdateBroadcasts();
}
       
void LibraryWindow::RackShelf::UpdateBroadcasts()
{
    // Remove old broadcasts
    Unsigned32 u32NumObjects = m_aspBroadcastObjects.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumObjects; ++u32Idx )
    {
        BroadcastObjectPtr spObject = m_aspBroadcastObjects[ u32Idx ];
        m_spPnlRoot->RemoveChild( spObject->GetObject() );
    }
    m_aspBroadcastObjects.clear();

    // Reset anchor
    m_spObjCurrentBroadcastAnchor = m_spObjBroadcastAnchor;

    if ( m_spPlayer == NULL ||
         m_spGenre == NULL )
    {
        return;
    }

    // Show genre name
    m_spPnlGenre->SetVisible( true );
    m_spTbGenre->SetText( m_spGenre->GetName() );

    // Get broadcasts of genre
    const BroadcastRack& clRack = m_spPlayer->GetBroadcastRack();
    BroadcastVec aspBroadcasts;
    clRack.GetBroadcasts( m_spGenre->GetID(), aspBroadcasts );

    // Show broadcasts of genre
    Unsigned32 u32NumBroadcasts = aspBroadcasts.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumBroadcasts; ++u32Idx )
    {
        BroadcastPtr spBroadcast = aspBroadcasts[ u32Idx ];

        // Create broadcast object
        AddBroadcastObject( spBroadcast );
    }
}
        
void LibraryWindow::RackShelf::AddBroadcastObject( BroadcastPtr _spBroadcast )
{
    // Create gui object
    CGui::GuiObjectPtr spGuiObject = CGui::GuiMain::GetInstance().CreateFromTemplate( "ObjBroadcast", m_spObjBroadcastTemplate );
    
    // Connect to panel
    CGui::GuiObjectPtr spBroadcastAnchor = m_spObjCurrentBroadcastAnchor;
    m_spPnlRoot->AddChildAfter( spGuiObject, spBroadcastAnchor );
    m_spObjCurrentBroadcastAnchor = spGuiObject;

    // Create broadcast object
    BroadcastObjectPtr spBroadcastObject( new BroadcastObject() );
    spBroadcastObject->Init( spGuiObject );
    spBroadcastObject->SetContext( m_spContext );
    spBroadcastObject->SetBroadcast( _spBroadcast );
    m_aspBroadcastObjects.push_back( spBroadcastObject );
}
       
void LibraryWindow::RackShelf::RemoveBroadcastObject( BroadcastPtr _spBroadcast )
{
    if ( _spBroadcast == NULL )
    {
        return;
    }

    // Search for broadcast object
    Unsigned32 u32NumObjects = m_aspBroadcastObjects.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumObjects; ++u32Idx )
    {
        BroadcastObjectPtr spObject = m_aspBroadcastObjects[ u32Idx ];
        if ( spObject->GetBroadcast() == _spBroadcast )
        {
            // Check if current anchor
            if ( spObject->GetObject() == m_spObjCurrentBroadcastAnchor )
            {
                m_spObjCurrentBroadcastAnchor = spObject->GetObject()->GetPrevSibling();
            }

            // Remove from panel
            m_spPnlRoot->RemoveChild( spObject->GetObject() );
            
            // Remove from objects
            m_aspBroadcastObjects.erase( m_aspBroadcastObjects.begin() + u32Idx );

            break;
        }
    }
}
       
void LibraryWindow::RackShelf::OnBroadcastAdded( BroadcastPtr _spBroadcast )
{
    if ( _spBroadcast != NULL &&
         m_spGenre != NULL &&
         _spBroadcast->GetGenre() == m_spGenre )
    {
        AddBroadcastObject( _spBroadcast );
    }
}
        
void LibraryWindow::RackShelf::OnBroadcastRemoved( BroadcastPtr _spBroadcast )
{
    RemoveBroadcastObject( _spBroadcast );
}
