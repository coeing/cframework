#ifndef CSYSTEM_INPUT_H
#define CSYSTEM_INPUT_H

#include "cfoundation/Utils/Singleton.h"
#include "cfoundation/Math/Rect.h"
#include "cfoundation/Math/Vector.h"
#include "cfoundation/Events/DelegateInvoker.h"

#include "csystem/Input/InputState.h"
#include "csystem/Input/KeyInputEvent.h"
#include "csystem/Input/MouseInputEvent.h"

struct HWND__;

namespace CSystem
{
    /// Delegate to exit the application
    typedef Delegate0<> ExitDelegate;
    typedef SharedPtr< ExitDelegate > ExitDelegatePtr;

	/// Catches Messages from the OS and updates the input state
    class Input : public CFoundation::RCSingleton< Input >, 
                  public CFoundation::Delegate0Invoker< MouseInputEvent >,
                  public CFoundation::Delegate1Invoker< KeyInputEvent, KeyCode >
	{
	public:
        DELEGATE0_INVOKER( MouseInputEvent );
        DELEGATE1_INVOKER( KeyInputEvent, KeyCode );

        /// Destructor
        virtual ~Input();

        /// Per frame update
        void                            Update( Float32 _fTimeDifference );

        /// Returns the current input state
        const InputState&               GetState() const;

        /// Sets the delegate to end the application
        void                            SetExitDelegate( ExitDelegatePtr _spDelegate );

#ifdef WIN32
		static long __stdcall           WndProc( HWND__*, Unsigned32, Unsigned32, long );
#endif

	private:
		friend class CFoundation::RCSingleton< Input >;

        /// Constructor
		Input();

#ifdef WIN32
		bool                            HandleWindowsMouseMessage( Unsigned32 uMsg, long );
		bool                            HandleWindowsKeyboardMessage( Unsigned32 uMsg, Unsigned32, long );
#endif

        /// Input state
        InputState                      m_state;

        /// Delegate to exit the application
        /// If not set the application is forced to exit
        ExitDelegatePtr                 m_spDelegateExit;
	};
};

#endif
