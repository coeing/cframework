#ifndef CCTRL_WINDOWMANAGER_H
#define CCTRL_WINDOWMANAGER_H

#include <list>

#include "cfoundation/Utils/Singleton.h"

#include "cgui/Gui/Window.h"

#include "cctrl/WindowController.h"

namespace CCtrl
{
	class WindowManager : public CFoundation::Singleton< WindowManager >
	{
	public:
        /// Destructor
        virtual ~WindowManager();

        //! Creates a screen of the passed type
		template < class T >
		SharedPtr< T >      Create( CGui::WindowPtr _spWindow, WindowControllerPtr _spParent );

        //! Destroy the passed screen
		void                DestroyWindow( WindowControllerCPtr _spWindow );

        /// Processes input
        virtual void        ProcessInput( const CSystem::InputState& _state );

		/// Per frame update
		virtual void        Update( Float32 _f32TimeDifference );

        //! Sets the modal window
        void                SetModalWindow( WindowControllerPtr _spWindow );

        //! Returns the modal window
        WindowControllerPtr GetModalWindow() const;

        /// Requests to close the passed window in the next frame
        void                RequestCloseWindow( WindowControllerPtr _spWindow, Unsigned32 _u32ReturnValue = 0 );

	private:
        struct CloseRequest
        {
            /// Window
            WindowControllerPtr     m_spWindow;

            /// Return value
            Unsigned32              m_u32ReturnValue;
        };

        typedef std::list< WindowControllerPtr > WindowList;
        typedef std::vector< CloseRequest > CloseRequestVec;

        //! The active windows
		WindowList		        m_lspWindows;

        //! Modal screen
        WindowControllerPtr     m_spModalWindow;

        /// Close requests
        CloseRequestVec         m_aCloseRequests;
	};
}

#include "cctrl/WindowManager.inl"

#endif
