#ifndef CMADTV_BROADCAST_DEALER_WINDOW_H
#define CMADTV_BROADCAST_DEALER_WINDOW_H

#include "cgui/DragDrop/DragController.h"

#include "cctrl/WindowController.h"

#include "cmadtv/Data/Player/BroadcastBag.h"
#include "cmadtv/Data/World/BroadcastDealer.h"

#include "cmadtv/Windows/Window.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( BroadcastBagPanel );
    INCLUDE_SHARED_CLASS( BroadcastDealer );
    INCLUDE_SHARED_CLASS( DealerDropController );
    INCLUDE_SHARED_CLASS( Game );

    /// Window of the broadcast dealer room
    class BroadcastDealerWindow : public Window
    {
    public:
        /// Returns the xml file name
        static CFoundation::String  GetXMLFileName();

        /// Constructor
        BroadcastDealerWindow();

        /// Destructor
        ~BroadcastDealerWindow();

        /// Initializes the window
        virtual void            InitWindow();

        /// Deinitializes the window
		virtual void            DeinitWindow();

        /// Deinitialization
        void                    Deinit();

        /// Closes the window
		virtual void            Close( Unsigned32 _u32ReturnValue = 0 );

        /// Callback when context changed
        virtual void            OnContextChanged();

        /// Sets the game
        virtual void            SetGame( GamePtr _spGame );

        /// Sets the player
        virtual void            SetPlayer( PlayerPtr _spPlayer );

    private:
        INCLUDE_SHARED_CLASS( BroadcastObject );

        /// Sets the dealer
        void                    SetDealer( BroadcastDealerPtr _spDealer );

        /// Callbacks
        void                    OnBroadcastAdded( BroadcastPtr _spBroadcast, Unsigned32 _u32Slot );
        void                    OnBroadcastRemoved( BroadcastPtr _spBroadcast, Unsigned32 _u32Slot );

        /// Delegates
        BroadcastDealerEventDelegate2Ptr    m_spDelegateBroadcastAdded;
        BroadcastDealerEventDelegate2Ptr    m_spDelegateBroadcastRemoved;
        
        /// Game
        GamePtr                 m_spGame;

        /// Dealer
        BroadcastDealerPtr      m_spDealer;

        /// Player
        PlayerPtr               m_spPlayer;

        /// Broadcast objects
        BroadcastObjectPtr      m_aspBroadcastObjects[ BroadcastDealer::NUM_SLOTS ];

        /// Bag
        BroadcastBagPanelPtr    m_spBag;

        /// Drop controller for bag
        DealerDropControllerPtr m_spBagDropController;

        /// Dealer
        CGui::ImagePtr          m_spImgDealer;
    };

    class DealerBroadcastDragController;
    typedef SharedPtr< DealerBroadcastDragController > DealerBroadcastDragControllerPtr;

    class BroadcastTooltipController;
    typedef SharedPtr< BroadcastTooltipController > BroadcastTooltipControllerPtr;

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// BroadcastDealerWindow::BroadcastObject
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class BroadcastDealerWindow::BroadcastObject
    {
    public:
        /// Constructor
        BroadcastObject();

        /// Destructor
        ~BroadcastObject();

        /// Initialization
        void                    Init( CGui::GuiObjectPtr _spObject );

        /// Sets the broadcast this object is for
        void                    SetBroadcast( BroadcastPtr _spBroadcast );

    private:
        /// Callbacks
        void                    OnDragStarted( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spGuiObject, CGui::DragDataPtr _spDragData );
        void                    OnDragUndone();
        void                    OnDragEnded();

        /// Delegates
        CGui::DragEventDelegatePtr          m_spDelegateDragStarted;
        CFoundation::DelegateConnectionPtr  m_spDelegateDragUndone;
        CFoundation::DelegateConnectionPtr  m_spDelegateDragEnded;

        /// Object
        CGui::GuiObjectPtr                  m_spObject;

        /// Drag controller
        DealerBroadcastDragControllerPtr    m_spBroadcastDragController;

        /// Tooltip controller
        BroadcastTooltipControllerPtr       m_spBroadcastTooltipController;
    };
}

#endif
