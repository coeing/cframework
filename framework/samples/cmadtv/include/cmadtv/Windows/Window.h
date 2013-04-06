#ifndef CMADTV_WINDOW_H
#define CMADTV_WINDOW_H

#include "cctrl/WindowController.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Player );
    INCLUDE_SHARED_CLASS( PlayerContext );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// Window
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    /// Base class for a window

    class Window : public CCtrl::WindowController
    {
    public:
        /// Constructor
        Window();

        /// Destructor
        ~Window();

        /// Sets the context
        void                SetContext( PlayerContextPtr _spContext );

        /// Callback when context changed
        virtual void        OnContextChanged();

        /// Closes the window
		virtual void        Close( Unsigned32 _u32ReturnValue = 0 );

    protected:
        /// Context
        PlayerContextPtr    m_spContext;
    };

    INCLUDE_SHARED_CLASS( Window );
}

#endif
