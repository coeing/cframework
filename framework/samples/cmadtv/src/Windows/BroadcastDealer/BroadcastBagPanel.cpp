#include "cmadtv/Windows/BroadcastDealer/BroadcastBagPanel.h"

#include "cgui/DragDrop/DragDropSystem.h"
#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/Panel.h"
#include "cgui/Tooltip/TooltipSystem.h"

#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerContext.h"
#include "cmadtv/DragDrop/DealerBroadcastDragController.h"
#include "cmadtv/Tooltip/BroadcastTooltipController.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// BroadcastBagPanel
///
////////////////////////////////////////////////////////////////////////////////////////////

BroadcastBagPanel::BroadcastBagPanel( CGui::PanelPtr _spPanel )
:   m_spPanel( _spPanel )
{
    // Create delegates
    m_spDelegateBroadcastAdded = BroadcastBagEventDelegate1Ptr( new BroadcastBagEventDelegate1( this, &BroadcastBagPanel::OnBroadcastAdded ) );
    m_spDelegateBroadcastRemoved = BroadcastBagEventDelegate1Ptr( new BroadcastBagEventDelegate1( this, &BroadcastBagPanel::OnBroadcastRemoved ) );
    m_spDelegateBroadcastDragStarted = BagBroadcastObjectEventDelegate1Ptr( new BagBroadcastObjectEventDelegate1( this, &BroadcastBagPanel::OnBroadcastDragStarted ) );

    // Get broadcast object template
    m_spObjBroadcastTemplate = m_spPanel->FindChild( "ObjBroadcastTemplate" );
    m_spObjBroadcastTemplate->RemoveFromParent();
    m_spObjBroadcastAnchor1 = m_spPanel->FindChild( "ObjBroadcastAnchor1" );
    m_spObjBroadcastAnchor2 = m_spPanel->FindChild( "ObjBroadcastAnchor2" );
}

BroadcastBagPanel::~BroadcastBagPanel()
{
    SetContext( PlayerContextPtr() );
}
       
void BroadcastBagPanel::SetContext( PlayerContextPtr _spContext )
{
    m_spContext = _spContext;
    SetPlayer( m_spContext != NULL ? m_spContext->GetPlayer() : PlayerPtr() );

    // Update context of broadcast objects
    Unsigned32 u32NumBroadcastObjects = m_aspBroadcastObjects.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumBroadcastObjects; ++u32Idx )
    {
        m_aspBroadcastObjects[ u32Idx ]->SetContext( m_spContext );
    }
}

void BroadcastBagPanel::SetPlayer( PlayerPtr _spPlayer )
{
    if ( _spPlayer == m_spPlayer )
    {
        return;
    }

    if ( m_spPlayer != NULL )
    {
        BroadcastBag& clBag = m_spPlayer->GetBroadcastBag();

        // Remove delegates
        clBag.RemoveDelegate( BROADCAST_BAG_EVENT_BROADCAST_ADDED, m_spDelegateBroadcastAdded );
        clBag.RemoveDelegate( BROADCAST_BAG_EVENT_BROADCAST_REMOVED, m_spDelegateBroadcastRemoved );
    }

    m_spPlayer = _spPlayer;

    if ( m_spPlayer != NULL )
    {
        // Setup start broadcasts
        BroadcastBag& clBag = m_spPlayer->GetBroadcastBag();
        Unsigned32 u32NumBroadcasts = clBag.GetNumBroadcasts();
        for ( Unsigned32 u32Idx = 0; u32Idx != u32NumBroadcasts; ++u32Idx )
        {
            BroadcastPtr spBroadcast = clBag.GetBroadcast( u32Idx );
            AddBroadcastObject( spBroadcast );
        }

        // Register delegates
        clBag.RegisterDelegate( BROADCAST_BAG_EVENT_BROADCAST_ADDED, m_spDelegateBroadcastAdded );
        clBag.RegisterDelegate( BROADCAST_BAG_EVENT_BROADCAST_REMOVED, m_spDelegateBroadcastRemoved );
    }
}
        
CGui::GuiObjectPtr BroadcastBagPanel::GetObject() const
{
    return m_spPanel;
}
        
void BroadcastBagPanel::AddBroadcastObject( BroadcastPtr _spBroadcast )
{
    // Create gui object
    CGui::GuiObjectPtr spGuiObject = CGui::GuiMain::GetInstance().CreateFromTemplate( "ObjBroadcast", m_spObjBroadcastTemplate );
    
    // Connect to panel
    CGui::GuiObjectPtr spBroadcastAnchor = GetBroadcastObjectAnchor();
    m_spPanel->AddChildAfter( spGuiObject, spBroadcastAnchor );

    // Create broadcast object
    BroadcastObjectPtr spBroadcastObject( new BroadcastObject( spGuiObject ) );
    spBroadcastObject->SetContext( m_spContext );
    spBroadcastObject->SetBroadcast( _spBroadcast );
    spBroadcastObject->RegisterDelegate( EVENT_BAG_BROADCAST_OBJECT_DRAG_STARTED, m_spDelegateBroadcastDragStarted );
    m_aspBroadcastObjects.push_back( spBroadcastObject );
}

void BroadcastBagPanel::RemoveBroadcastObject( BroadcastPtr _spBroadcast )
{
    Unsigned32 u32NumBroadcastObjects = m_aspBroadcastObjects.size();
    if ( u32NumBroadcastObjects == 0 )
    {
        return;
    }

    // Move broadcast after removed broadcast one forward
    bool bAfterRemoved = false;
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumBroadcastObjects; ++u32Idx )
    {
        BroadcastObjectPtr spBroadcastObject = m_aspBroadcastObjects[ u32Idx ];

        // Get next broadcast
        BroadcastPtr spNextBroadcast;
        if ( u32Idx < u32NumBroadcastObjects - 1 )
        {
            spNextBroadcast = m_aspBroadcastObjects[ u32Idx + 1 ]->GetBroadcast();
        }

        if ( bAfterRemoved )
        {
            spBroadcastObject->SetBroadcast( spNextBroadcast );
        }
        else if ( spBroadcastObject->GetBroadcast() == _spBroadcast )
        {
            bAfterRemoved = true;
            spBroadcastObject->SetBroadcast( spNextBroadcast );
        }
    }

    // Remove last broadcast
    if ( bAfterRemoved )
    {
        BroadcastObjectVec::iterator it = m_aspBroadcastObjects.end() - 1;
        BroadcastObjectPtr spBroadcastObject = *it;
        
        // Remove delegate
        spBroadcastObject->RemoveDelegate( EVENT_BAG_BROADCAST_OBJECT_DRAG_STARTED, m_spDelegateBroadcastDragStarted );

        CGui::GuiObjectPtr spGuiObject = spBroadcastObject->GetObject();
        m_spPanel->RemoveChild( spGuiObject );
        m_aspBroadcastObjects.erase( it );
    }
}
        
CGui::GuiObjectPtr BroadcastBagPanel::GetBroadcastObjectAnchor() const
{
    Unsigned32 u32NumBroadcastObjects = m_aspBroadcastObjects.size();
    if ( u32NumBroadcastObjects == 0 )
    {
        return m_spObjBroadcastAnchor1;
    }
    else if ( u32NumBroadcastObjects == BroadcastBag::MAX_NUM_BROADCASTS / 2 )
    {
        return m_spObjBroadcastAnchor2;
    }
    else
    {
        BroadcastObjectPtr spLastBroadcastObject = *( m_aspBroadcastObjects.end() - 1 );
        return spLastBroadcastObject->GetObject();
    }
}

void BroadcastBagPanel::OnBroadcastAdded( BroadcastPtr _spBroadcast )
{
    AddBroadcastObject( _spBroadcast );
}

void BroadcastBagPanel::OnBroadcastRemoved( BroadcastPtr _spBroadcast )
{
    RemoveBroadcastObject( _spBroadcast );
}
        
void BroadcastBagPanel::OnBroadcastDragStarted( BroadcastPtr _spBroadcast )
{
    // Dispatch event
    DispatchEvent( EVENT_BAG_PANEL_DRAG_STARTED, _spBroadcast );
}

////////////////////////////////////////////////////////////////////////////////////////////
///
/// BroadcastBagPanel::BroadcastObject
///
////////////////////////////////////////////////////////////////////////////////////////////

BroadcastBagPanel::BroadcastObject::BroadcastObject( CGui::GuiObjectPtr _spObject )
:   m_spObject( _spObject )
{
    // Create delegates
    m_spDelegateDragStarted = CGui::DragEventDelegatePtr( new CGui::DragEventDelegate( this, &BroadcastBagPanel::BroadcastObject::OnDragStarted ) );
    m_spDelegateDragUndone = CGui::DragEventDelegatePtr( new CGui::DragEventDelegate( this, &BroadcastBagPanel::BroadcastObject::OnDragUndone ) );
    m_spDelegateDragEnded = CGui::DragEventDelegatePtr( new CGui::DragEventDelegate( this, &BroadcastBagPanel::BroadcastObject::OnDragEnded ) );

    m_spDragController = DealerBroadcastDragControllerPtr( new DealerBroadcastDragController() );
    m_spDragController->RegisterDelegate( CGui::EVENT_DRAG_STARTED, m_spDelegateDragStarted );
    m_spDragController->RegisterDelegate( CGui::EVENT_DRAG_UNDONE, m_spDelegateDragUndone );
    m_spDragController->RegisterDelegate( CGui::EVENT_DRAG_ENDED, m_spDelegateDragEnded );

    m_spTooltipController = BroadcastTooltipControllerPtr( new BroadcastTooltipController() );

    if ( m_spObject != NULL )
    {
        m_spObject->SetDraggable( true );
        CGui::DragDropSystem::GetInstance().SetDragController( m_spObject, m_spDragController );
        CGui::TooltipSystem::GetInstance().SetTooltipController( m_spObject, m_spTooltipController );
    }
}

BroadcastBagPanel::BroadcastObject::~BroadcastObject()
{
    // Remove delegates
    m_spDragController->RemoveDelegate( CGui::EVENT_DRAG_ENDED, m_spDelegateDragEnded );
    m_spDragController->RemoveDelegate( CGui::EVENT_DRAG_UNDONE, m_spDelegateDragUndone );
    m_spDragController->RemoveDelegate( CGui::EVENT_DRAG_STARTED, m_spDelegateDragStarted );
}
        
CGui::GuiObjectPtr BroadcastBagPanel::BroadcastObject::GetObject() const
{
    return m_spObject;
}

void BroadcastBagPanel::BroadcastObject::SetContext( PlayerContextPtr _spContext )
{
    m_spTooltipController->SetContext( _spContext );
    m_spTooltipController->SetOwner( _spContext != NULL ? _spContext->GetPlayer() : PlayerPtr() );
}

void BroadcastBagPanel::BroadcastObject::SetBroadcast( BroadcastPtr _spBroadcast )
{
    m_spBroadcast = _spBroadcast;
    m_spDragController->SetBroadcast( m_spBroadcast );
    m_spTooltipController->SetBroadcast( m_spBroadcast );
}
        
BroadcastPtr BroadcastBagPanel::BroadcastObject::GetBroadcast() const
{
    return m_spBroadcast;
}

void BroadcastBagPanel::BroadcastObject::OnDragStarted( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spGuiObject, CGui::DragDataPtr _spDragData )
{
    m_spObject->SetVisible( false );

    // Dispatch event
    DispatchEvent( EVENT_BAG_BROADCAST_OBJECT_DRAG_STARTED, m_spBroadcast );
}

void BroadcastBagPanel::BroadcastObject::OnDragUndone( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spGuiObject, CGui::DragDataPtr _spDragData )
{
    m_spObject->SetVisible( true );
}

void BroadcastBagPanel::BroadcastObject::OnDragEnded( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spGuiObject, CGui::DragDataPtr _spDragData )
{
    //m_spObject->SetVisible( true );
}
