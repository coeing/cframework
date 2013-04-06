#ifndef CMADTV_WINDOW_MANAGER_H
#define CMADTV_WINDOW_MANAGER_H

#include "cfoundation/Events/Delegate.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( InfoWindow );
    INCLUDE_SHARED_CLASS( NavigationWindow );
    INCLUDE_SHARED_CLASS( PlayerContext );
    INCLUDE_SHARED_CLASS( Window );

    enum Room;
    enum NavigationWindowEventRoom;

    TYPEDEF_DELEGATE1( NavigationWindowEventRoom, Room );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// WindowManager
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    /// Manages the main window

    class WindowManager
    {
    public:
        /// Constructor
        WindowManager();

        /// Destructor
        ~WindowManager();

        /// Initialization
        void                Init();

        /// Deinitialization
        void                Deinit();

        /// Sets the context
        void                SetContext( PlayerContextPtr _spContext );

        /// Callback when context changed
        void                OnContextChanged();

        /// Sets the active room
        void                SetActiveRoom( Room _eRoom );

        /// Returns the active room
        Room                GetActiveRoom() const;

        /// Shows/Hides the navigation window
        void                ShowNavigationWindow( bool _bShow );

    private:
        /// Callbacks
        void                                OnRoomClicked( Room _eRoom );

        /// Delegates
        CFoundation::DelegateConnectionPtr  m_spDelegateRoomClicked;

        /// Context
        PlayerContextPtr    m_spContext;

        /// Active room
        Room                m_eRoom;

        /// Active window
        WindowPtr           m_spWindow;

        /// Info window
        InfoWindowPtr       m_spInfoWindow;

        /// Navigation window
        NavigationWindowPtr m_spNavWindow;
    };

    INCLUDE_SHARED_CLASS( WindowManager );
}

#endif
