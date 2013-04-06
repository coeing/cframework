#ifndef CCTRL_WINDOWCONTROLLER_H
#define CCTRL_WINDOWCONTROLLER_H

#include "cfoundation/Base/Types.h"
#include "cfoundation/Events/DelegateInvoker.h"

#include "cctrl/ScreenClosedMsg.h"
#include "cctrl/Controls.h"
#include "cctrl/WindowManager.h"

namespace CSystem
{
    class InputState;
}

#define WINDOW_CONTROLLER_DECLARE_GET_GUI_OBJECT( Type )    CGui::##Type##Ptr Get##Type##( const CFoundation::String& _strName ); \
                                                            CGui::##Type##CPtr Get##Type##( const CFoundation::String& _strName ) const;
  
namespace CCtrl
{
    class WindowController;

    typedef SharedPtr< WindowController > WindowControllerPtr;

    enum WindowReturnValue
    {
        WINDOW_RETURN_VALUE_OK,
        WINDOW_RETURN_VALUE_CANCEL,
        WINDOW_RETURN_VALUE_YES,
        WINDOW_RETURN_VALUE_NO,
    };

    enum WindowEvent
    {
        EVENT_WINDOW_CLOSED
    };
    typedef Delegate2< WindowControllerPtr, Unsigned32 > WindowEventDelegate;
    typedef SharedPtr< WindowEventDelegate > WindowEventDelegatePtr;

    class WindowController : public CFoundation::MessageObject, public CFoundation::Delegate2Invoker< WindowEvent, WindowControllerPtr, Unsigned32 >
	{
	public:        
        DELEGATE2_INVOKER( WindowEvent, WindowControllerPtr, Unsigned32 );

		template < class T >
		static SharedPtr< T > Create( WindowControllerPtr _spParent = WindowControllerPtr() );

        //! Destructor
		~WindowController();

        /// Initializes the window
		virtual void            InitWindow();

        /// Deinitializes the window
        virtual void            DeinitWindow();

        /// Processes input (when window is active)
        virtual void            ProcessInput( const CSystem::InputState& _State );

		/// Per frame update
		virtual void			Update( Float32 _f32TimeDifference );

        /// Declarations for get methods

        WINDOW_CONTROLLER_DECLARE_GET_GUI_OBJECT( GuiObject )
        WINDOW_CONTROLLER_DECLARE_GET_GUI_OBJECT( Panel )
        WINDOW_CONTROLLER_DECLARE_GET_GUI_OBJECT( Image )
        WINDOW_CONTROLLER_DECLARE_GET_GUI_OBJECT( TextBox )
        
        WINDOW_CONTROLLER_DECLARE_GET_GUI_OBJECT( Button )
        WINDOW_CONTROLLER_DECLARE_GET_GUI_OBJECT( CheckBox )
        WINDOW_CONTROLLER_DECLARE_GET_GUI_OBJECT( ComboBox )
        WINDOW_CONTROLLER_DECLARE_GET_GUI_OBJECT( EditBox )
        WINDOW_CONTROLLER_DECLARE_GET_GUI_OBJECT( ListBox )
        WINDOW_CONTROLLER_DECLARE_GET_GUI_OBJECT( ProgressBar )
        WINDOW_CONTROLLER_DECLARE_GET_GUI_OBJECT( Tree )
        WINDOW_CONTROLLER_DECLARE_GET_GUI_OBJECT( Window )

        /// Processes messages send to this controller
		virtual void            ProcessMessage( const CFoundation::Message& _clMessage );        

        /// Shows/Hides the window
        void                    SetVisible( bool _bVisible );

        /// Called when a screen was closed
		virtual void            OnCloseScreen( const ScreenClosedMsg& _clMessage );

        /// Requests to close this window in the next update
        virtual void            RequestClose( Unsigned32 _u32ReturnValue = 0 );

        /// Closes the window
		virtual void            Close( Unsigned32 _u32ReturnValue = 0 );

	protected:
        /// Constructor
		WindowController();

        /// Returns the shared pointer of this
        WindowControllerPtr     GetSharedPtr();

        /// Returns the shared pointer of this
        WindowControllerCPtr    GetSharedPtr() const;

        /// Returns the parent
        WindowControllerPtr     GetParent();

		CGui::WindowCPtr        GetWindow() const;
		CGui::WindowPtr         GetWindow();

	private:
		friend class WindowManager;

        /// Creates a screen from the passed xml
        static CGui::WindowPtr  Create( const CFoundation::String& _strXMLFileName );

		/// Initializes the controller
		void                            InitController( CGui::WindowPtr _spWindow, WindowControllerPtr _spParent = WindowControllerPtr() );

        /// Window pointer
        CGui::WindowPtr     m_spWindow;

        /// Parent pointer
		WindowControllerPtr m_spParentScreen;
	};
}

#include "cctrl/WindowController.inl"

#endif
