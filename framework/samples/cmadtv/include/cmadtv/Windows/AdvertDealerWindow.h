#ifndef CMADTV_ADVERT_DEALER_WINDOW_H
#define CMADTV_ADVERT_DEALER_WINDOW_H

#include "cfoundation/DateTime/Date.h"

#include "cgui/DragDrop/DragController.h"

#include "cmadtv/Data/Player/AdvertBag.h"
#include "cmadtv/Data/World/AdvertDealer.h"

#include "cmadtv/Windows/Window.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( AdvertBagPanel );
    INCLUDE_SHARED_CLASS( AdvertDealer );
    INCLUDE_SHARED_CLASS( DealerDropController );
    INCLUDE_SHARED_CLASS( Game );
    INCLUDE_SHARED_CLASS( Player );

    /// Window of the Advert dealer room
    class AdvertDealerWindow : public Window
    {
    public:
        /// Returns the xml file name
        static CFoundation::String      GetXMLFileName();

        /// Constructor
        AdvertDealerWindow();

        /// Destructor
        ~AdvertDealerWindow();

        /// Initializes the window
        virtual void                InitWindow();

        /// Deinitializes the window
		virtual void                DeinitWindow();

        /// Closes the window
		virtual void                Close( Unsigned32 _u32ReturnValue = 0 );

        /// Callback when context changed
        virtual void                OnContextChanged();

    private:
        INCLUDE_SHARED_CLASS( AdvertObject );

        /// Sets the game
        void                        SetGame( GamePtr _spGame );

        /// Sets the dealer
        void                        SetDealer( AdvertDealerPtr _spDealer );

        /// Sets the player
        void                        SetPlayer( PlayerPtr _spPlayer );

        /// Callbacks
        void                        OnAdvertAdded( AdvertPtr _spAdvert, Unsigned32 _u32Slot );
        void                        OnAdvertRemoved( AdvertPtr _spAdvert, Unsigned32 _u32Slot );

        /// Delegates
        CFoundation::DelegateConnectionPtr  m_spDelegateAdvertAdded;
        CFoundation::DelegateConnectionPtr  m_spDelegateAdvertRemoved;

        /// Game
        GamePtr                     m_spGame;

        /// Dealer
        AdvertDealerPtr             m_spDealer;
        
        /// Player
        PlayerPtr                   m_spPlayer;

        /// Advert objects
        AdvertObjectPtr             m_aspAdvertObjects[ AdvertDealer::NUM_SLOTS ];

        /// Bag
        AdvertBagPanelPtr           m_spBag;

        /// Drop controllers
        DealerDropControllerPtr     m_spBagDropController;
        DealerDropControllerPtr     m_spDealerDropController;

        /// Dealer
        CGui::ImagePtr              m_spImgDealer;
    };

    class AdvertDragController;
    typedef SharedPtr< AdvertDragController > AdvertDragControllerPtr;

    class AdvertTooltipController;
    typedef SharedPtr< AdvertTooltipController > AdvertTooltipControllerPtr;

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// AdvertDealerWindow::AdvertObject
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class AdvertDealerWindow::AdvertObject
    {
    public:
        /// Constructor
        AdvertObject( CGui::GuiObjectPtr _spObject );

        /// Destructor
        ~AdvertObject();

        /// Sets the player and advert this object is for
        void                    Set( PlayerContextPtr _spContext, AdvertPtr _spAdvert );

    private:
        /// Called when dragging was started
        void                    OnDragStarted( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spGuiObject, CGui::DragDataPtr _spDragData );

        /// Called when dragging was undone
        void                    OnDragUndone( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spGuiObject, CGui::DragDataPtr _spDragData );

        /// Delegates
        CGui::DragEventDelegatePtr          m_spDelegateDragStarted;
        CGui::DragEventDelegatePtr          m_spDelegateDragUndone;

        /// Object
        CGui::GuiObjectPtr                  m_spObject;

        /// Drag controller
        AdvertDragControllerPtr             m_spAdvertDragController;

        /// Tooltip controller
        AdvertTooltipControllerPtr          m_spAdvertTooltipController;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// AdvertBag
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class AdvertBagPanel
    {
    public:
        /// Constructor
        AdvertBagPanel( CGui::PanelPtr _spPanel );

        /// Destructor
        ~AdvertBagPanel();

        /// Returns the gui object
        CGui::GuiObjectPtr          GetObject() const;

        /// Sets the context
        void                        SetContext( PlayerContextPtr _spContext );

    private:
        class AdvertObject;
        typedef SharedPtr< AdvertObject > AdvertObjectPtr;
        typedef std::vector< AdvertObjectPtr > AdvertObjectVec;

        /// Sets the player
        void                        SetPlayer( PlayerPtr _spPlayer );

        /// Adds a Advert object for the passed Advert
        void                        AddAdvertObject( AdvertPtr _spAdvert );

        /// Removes all objects        
        void                        RemoveAllAdvertObjects();

        /// Removes the Advert object of the passed Advert
        void                        RemoveAdvertObject( AdvertPtr _spAdvert );

        /// Returns the anchor for a new Advert object
        CGui::GuiObjectPtr          GetAdvertObjectAnchor() const;

        /// Callbacks
        void                        OnAdvertAdded( AdvertPtr _spAdvert );
        void                        OnAdvertRemoved( AdvertPtr _spAdvert );

        /// Delegates
        CFoundation::DelegateConnectionPtr  m_spDelegateAdvertAdded;
        CFoundation::DelegateConnectionPtr  m_spDelegateAdvertRemoved; 

        /// Context
        PlayerContextPtr            m_spContext;
        
        /// Player
        PlayerPtr                   m_spPlayer;

        /// Panel
        CGui::PanelPtr              m_spPanel;

        /// Advert object template
        CGui::GuiObjectPtr          m_spObjAdvertTemplate;

        /// First row anchor
        CGui::GuiObjectPtr          m_spObjAdvertAnchor1;

        /// Second row anchor
        CGui::GuiObjectPtr          m_spObjAdvertAnchor2;

        /// Advert objects
        AdvertObjectVec             m_aspAdvertObjects;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// AdvertBagPanel::AdvertObject
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class AdvertBagPanel::AdvertObject
    {
    public:
        /// Constructor
        AdvertObject( CGui::GuiObjectPtr _spObject );

        /// Destructor
        ~AdvertObject();

        /// Returns the gui object
        CGui::GuiObjectPtr          GetObject() const;

        /// Sets the player and advert this object is for
        void                        Set( PlayerContextPtr _spContext, AdvertPtr _spAdvert );

        /// Returns the Advert this object is for
        AdvertPtr                   GetAdvert() const;

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

        /// Advert
        AdvertPtr                           m_spAdvert;

        /// Drag controller
        AdvertDragControllerPtr             m_spDragController;

        /// Tooltip controller
        AdvertTooltipControllerPtr          m_spTooltipController;
    };
}

#endif
