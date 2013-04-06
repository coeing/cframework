#ifndef CMADTV_LIBRARY_WINDOW_H
#define CMADTV_LIBRARY_WINDOW_H

#include "cgui/DragDrop/DragController.h"

#include "cmadtv/Data/Player/BroadcastBag.h"

#include "cmadtv/Windows/Window.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( BroadcastBagPanel );
    INCLUDE_SHARED_CLASS( Genre );
    INCLUDE_SHARED_CLASS( LibraryDropController );
    INCLUDE_SHARED_CLASS( PlanningArchivWindow );
    INCLUDE_SHARED_CLASS( Player );
    
    enum GenrePanelEvent;
    TYPEDEF_DELEGATE1( GenrePanelEvent, GenrePtr );

    enum BagPanelEvent;
    TYPEDEF_DELEGATE1( BagPanelEvent, BroadcastPtr );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// LibraryWindow
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class LibraryWindow : public Window
    {
    public:
        /// Returns the xml file name
        static CFoundation::String  GetXMLFileName();

        /// Constructor
        LibraryWindow();

        /// Destructor
        ~LibraryWindow();

        /// Initializes the window
        virtual void            InitWindow();

        /// Deinitializes the window
		virtual void            DeinitWindow();

        /// Callback when context changed
        virtual void            OnContextChanged();

    private:
        INCLUDE_SHARED_CLASS( BroadcastObject );
        typedef std::vector< BroadcastObjectPtr > BroadcastObjectVec;
        INCLUDE_SHARED_CLASS( RackShelf );

        /// Sets the player
        void                    SetPlayer( PlayerPtr _spPlayer );

        /// Callbacks
        void                    OnLibrarianClicked();
        void                    OnShelfClicked( GenrePtr _spGenre );
        void                    OnBagDragStarted( BroadcastPtr _spBroadcast );

        /// Delegates
        CGui::ObjectInputEventDelegate0Ptr  m_spDelegateLibrarianClicked;
        GenrePanelEventDelegate1Ptr         m_spDelegateShelfClicked;
        BagPanelEventDelegate1Ptr           m_spDelegateBagDragStarted;
        
        /// Player
        PlayerPtr                   m_spPlayer;

        /// Bag
        BroadcastBagPanelPtr        m_spBag;

        /// Drop controller        
        LibraryDropControllerPtr    m_spBagDropController;
        LibraryDropControllerPtr    m_spRackDropController;

        /// Controls
        CGui::GuiObjectPtr      m_spObjLibrarian;

        /// Rack Shelf
        RackShelfPtr            m_spRackShelf;

        /// Rack window
        PlanningArchivWindowPtr m_spBroadcastRack;
    };

    INCLUDE_SHARED_CLASS( DealerBroadcastDragController );
    INCLUDE_SHARED_CLASS( BroadcastTooltipController );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// LibraryWindow::BroadcastObject
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    enum LibraryBroadcastObjectEvent
    {
        EVENT_LIBRARY_BROADCAST_OBJECT_NONE,
        EVENT_LIBRARY_BROADCAST_OBJECT_DRAG_STARTED,
        EVENT_LIBRARY_BROADCAST_OBJECT_END
    };
    TYPEDEF_DELEGATE1( LibraryBroadcastObjectEvent, BroadcastPtr );

    class LibraryWindow::BroadcastObject : public CFoundation::Delegate1Invoker< LibraryBroadcastObjectEvent, BroadcastPtr >
    {
    public:
        DELEGATE1_INVOKER( LibraryBroadcastObjectEvent, BroadcastPtr );

        /// Constructor
        BroadcastObject();

        /// Destructor
        ~BroadcastObject();

        /// Initialization
        void                    Init( CGui::GuiObjectPtr _spObject );

        /// Sets the context
        void                    SetContext( PlayerContextPtr _spContext );

        /// Sets the broadcast this object is for
        void                    SetBroadcast( BroadcastPtr _spBroadcast );

        /// Returns the broadcast
        BroadcastPtr            GetBroadcast() const;

        /// Returns the gui object
        CGui::GuiObjectPtr      GetObject() const;

    private:
        /// Called when dragging was started
        void                    OnDragStarted( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spGuiObject, CGui::DragDataPtr _spDragData );

        /// Called when dragging was undone
        void                    OnDragUndone( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spGuiObject, CGui::DragDataPtr _spDragData );

        /// Delegate when dragging was started
        CGui::DragEventDelegatePtr          m_spDelegateDragStarted;

        /// Delegate when dragging was undone
        CGui::DragEventDelegatePtr          m_spDelegateDragUndone;

        /// Object
        CGui::GuiObjectPtr                  m_spObject;

        /// Drag controller
        DealerBroadcastDragControllerPtr    m_spBroadcastDragController;

        /// Tooltip controller
        BroadcastTooltipControllerPtr       m_spBroadcastTooltipController;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// LibraryWindow::RackShelf
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    INCLUDE_SHARED_CLASS( LibraryDropController );
    enum BroadcastRackEvent;
    TYPEDEF_DELEGATE1( BroadcastRackEvent, BroadcastPtr );

    class LibraryWindow::RackShelf
    {
    public:
        /// Constructor
        RackShelf();

        /// Destructor
        ~RackShelf();

        /// Initialization
        void                    Init( CGui::PanelPtr _spPnlRoot );

        /// Sets the context
        void                    SetContext( PlayerContextPtr _spContext );

        /// Sets the genre
        void                    SetGenre( GenrePtr _spGenre );

    private:
        /// Sets the player
        void                    SetPlayer( PlayerPtr _spPlayer );

        /// Update the shown broadcasts
        void                    UpdateBroadcasts();

        /// Adds a broadcast for the passed broadcast
        void                    AddBroadcastObject( BroadcastPtr _spBroadcast );

        /// Removes a broadcast object
        void                    RemoveBroadcastObject( BroadcastPtr _spBroadcast );

        /// Callbacks
        void                    OnBroadcastAdded( BroadcastPtr _spBroadcast );
        void                    OnBroadcastRemoved( BroadcastPtr _spBroadcast );

        /// Delegates
        BroadcastRackEventDelegate1Ptr  m_spDelegateBroadcastAdded;
        BroadcastRackEventDelegate1Ptr  m_spDelegateBroadcastRemoved;

        /// Context
        PlayerContextPtr        m_spContext;

        /// Player
        PlayerPtr               m_spPlayer;

        /// Genre              
        GenrePtr                m_spGenre;

        /// Drop controller
        LibraryDropControllerPtr    m_spDropController;

        /// Controls
        CGui::PanelPtr          m_spPnlRoot;
        CGui::GuiObjectPtr      m_spObjBroadcastAnchor;
        CGui::GuiObjectPtr      m_spObjCurrentBroadcastAnchor;
        CGui::GuiObjectPtr      m_spObjBroadcastTemplate;

        CGui::GuiObjectPtr      m_spPnlGenre;
        CGui::TextBoxPtr        m_spTbGenre;

        /// Broadcast object
        BroadcastObjectVec      m_aspBroadcastObjects;
    };
}

#endif
