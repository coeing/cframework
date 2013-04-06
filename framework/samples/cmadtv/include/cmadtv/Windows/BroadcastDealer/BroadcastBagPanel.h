#ifndef CMADTV_BROADCAST_BAG_PANEL_H
#define CMADTV_BROADCAST_BAG_PANEL_H

#include <vector>

#include "cfoundation/Utils/SharedPtr.h"

#include "cgui/DragDrop/DragEvent.h"

#include "cmadtv/Data/Player/BroadcastBag.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( DragController );
    INCLUDE_SHARED_CLASS( DragData );
    INCLUDE_SHARED_CLASS( GuiObject );
    INCLUDE_SHARED_CLASS( Panel );
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Broadcast );
    INCLUDE_SHARED_CLASS( BroadcastTooltipController );
    INCLUDE_SHARED_CLASS( DealerBroadcastDragController );
    INCLUDE_SHARED_CLASS( Player );
    INCLUDE_SHARED_CLASS( PlayerContext );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// BroadcastBagPanel
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    enum BagPanelEvent
    {
        EVENT_BAG_PANEL_NONE,
        EVENT_BAG_PANEL_DRAG_STARTED,
        EVENT_BAG_PANEL_END
    };
    TYPEDEF_DELEGATE1( BagPanelEvent, BroadcastPtr );

    enum BagBroadcastObjectEvent
    {
        EVENT_BAG_BROADCAST_OBJECT_NONE,
        EVENT_BAG_BROADCAST_OBJECT_DRAG_STARTED,
        EVENT_BAG_BROADCAST_OBJECT_END
    };
    TYPEDEF_DELEGATE1( BagBroadcastObjectEvent, BroadcastPtr );

    class BroadcastBagPanel : public CFoundation::Delegate1Invoker< BagPanelEvent, BroadcastPtr >
    {
    public:
        DELEGATE1_INVOKER( BagPanelEvent, BroadcastPtr );

        /// Constructor
        BroadcastBagPanel( CGui::PanelPtr _spPanel );

        /// Destructor
        ~BroadcastBagPanel();

        /// Sets the context
        void                        SetContext( PlayerContextPtr _spContext );

        /// Returns the gui object
        CGui::GuiObjectPtr          GetObject() const;

    private:
        INCLUDE_SHARED_CLASS( BroadcastObject );
        typedef std::vector< BroadcastObjectPtr > BroadcastObjectVec;

        /// Sets the player
        void                        SetPlayer( PlayerPtr _spPlayer );

        /// Adds a broadcast object for the passed broadcast
        void                        AddBroadcastObject( BroadcastPtr _spBroadcast );

        /// Removes the broadcast object of the passed broadcast
        void                        RemoveBroadcastObject( BroadcastPtr _spBroadcast );

        /// Returns the anchor for a new broadcast object
        CGui::GuiObjectPtr          GetBroadcastObjectAnchor() const;

        /// Callbacks
        void                        OnBroadcastAdded( BroadcastPtr _spBroadcast );
        void                        OnBroadcastRemoved( BroadcastPtr _spBroadcast );
        void                        OnBroadcastDragStarted( BroadcastPtr _spBroadcast );

        /// Delegates
        BroadcastBagEventDelegate1Ptr   m_spDelegateBroadcastAdded;
        BroadcastBagEventDelegate1Ptr   m_spDelegateBroadcastRemoved;
        BagBroadcastObjectEventDelegate1Ptr m_spDelegateBroadcastDragStarted;

        /// Context
        PlayerContextPtr            m_spContext;

        /// Player
        PlayerPtr                   m_spPlayer;

        /// Panel
        CGui::PanelPtr              m_spPanel;

        /// Broadcast object template
        CGui::GuiObjectPtr          m_spObjBroadcastTemplate;

        /// First row anchor
        CGui::GuiObjectPtr          m_spObjBroadcastAnchor1;

        /// Second row anchor
        CGui::GuiObjectPtr          m_spObjBroadcastAnchor2;

        /// Broadcast objects
        BroadcastObjectVec          m_aspBroadcastObjects;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// BroadcastBagPanel::BroadcastObject
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class BroadcastBagPanel::BroadcastObject : public CFoundation::Delegate1Invoker< BagBroadcastObjectEvent, BroadcastPtr >
    {
    public:
        DELEGATE1_INVOKER( BagBroadcastObjectEvent, BroadcastPtr );

        /// Constructor
        BroadcastObject( CGui::GuiObjectPtr _spObject );

        /// Destructor
        ~BroadcastObject();

        /// Returns the gui object
        CGui::GuiObjectPtr          GetObject() const;

        /// Sets the context this object is for
        void                        SetContext( PlayerContextPtr _spContext );

        /// Sets the broadcast this object is for
        void                        SetBroadcast( BroadcastPtr _spBroadcast );

        /// Returns the broadcast this object is for
        BroadcastPtr                GetBroadcast() const;

    private:
        /// Called when dragging was started
        void                        OnDragStarted( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spGuiObject, CGui::DragDataPtr _spDragData );

        /// Called when dragging was undone
        void                        OnDragUndone( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spGuiObject, CGui::DragDataPtr _spDragData );

        /// Called when dragging ended
        void                        OnDragEnded( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spGuiObject, CGui::DragDataPtr _spDragData );

        /// Delegates
        CGui::DragEventDelegatePtr          m_spDelegateDragStarted;
        CGui::DragEventDelegatePtr          m_spDelegateDragUndone;
        CGui::DragEventDelegatePtr          m_spDelegateDragEnded;

        /// Object
        CGui::GuiObjectPtr                  m_spObject;

        /// Broadcast
        BroadcastPtr                        m_spBroadcast;

        /// Drag controller
        DealerBroadcastDragControllerPtr    m_spDragController;

        /// Tooltip controller
        BroadcastTooltipControllerPtr       m_spTooltipController;
    };
}

#endif