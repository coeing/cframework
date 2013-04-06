#ifdef WIN32
#include <windows.h>
#endif

#include "csystem/Input/Input.h"

#include <vector>

#include "cfoundation/Debug/Logger.h"
#include "cfoundation/Debug/Makros.h"
#include "cfoundation/DateTime/Timer.h"

using namespace CSystem;

Input::Input()
{
}

Input::~Input()
{
}
       
void Input::Update( Float32 /*_f32TimeDifference*/ )
{
    // Store last mouse position    
    m_state.m_vLastMousePosition = m_state.m_vMousePosition;

    // Update mouse button states
    for ( Unsigned32 u32MouseButton = 0; u32MouseButton != MOUSE_END; ++u32MouseButton )
    {
        if ( m_state.m_eMouseButtonStates[ u32MouseButton ] == MBS_FIRST_DOWN )
        {
            m_state.m_eMouseButtonStates[ u32MouseButton ] = MBS_DOWN;
        }
        else if ( m_state.m_eMouseButtonStates[ u32MouseButton ] == MBS_FIRST_UP )
        {
            m_state.m_eMouseButtonStates[ u32MouseButton ] = MBS_UP;
        }
    }

    // Update key states
    for ( Unsigned32 u32Key = 0; u32Key < NUM_KEYS; ++u32Key )
    {
        if ( m_state.m_eKeyStates[ u32Key ] == KEY_STATE_FIRST_DOWN )
        {
            m_state.m_eKeyStates[ u32Key ] = KEY_STATE_DOWN;
        }
        else if ( m_state.m_eKeyStates[ u32Key ] == KEY_STATE_FIRST_UP )
        {
            m_state.m_eKeyStates[ u32Key ] = KEY_STATE_UP;
        }
    }

    // Clear keys
    m_state.m_awcTypedCharacters.clear();

    // Clear events
    m_state.m_u32Events = 0;
    
    // Maximum time to process messages (in one frame)
    Unsigned64 u64Time = 30;

	// Timer => don't process all messages
    CFoundation::Timer clTimer;
	clTimer.Start();

	while ( clTimer.GetElapsedMs() < u64Time )
	{
#ifdef WIN32
	    MSG msg;
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			if ( msg.message == WM_QUIT )
			{
                if ( m_spDelegateExit != NULL )
                {
                    (*m_spDelegateExit)();
                }
                else
                {
                    exit( 0 );
                }
			}
			else
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
        else
        {
            break;
        }
#endif
	}
}
        
const InputState& Input::GetState() const
{
    return m_state;
}
       
void Input::SetExitDelegate( ExitDelegatePtr _spDelegate )
{
    m_spDelegateExit = _spDelegate;
}

#ifdef WIN32
LRESULT CALLBACK Input::WndProc( HWND _hWnd, UINT _uMsg, WPARAM _wParam, LPARAM _lParam )
{
    switch ( _uMsg )
    {
    case WM_ACTIVATE:
        {
            if ( _wParam == WA_INACTIVE )
            {
                GetInstance().m_state.m_u32Events |= InputState::EVENT_WINDOW_DEACTIVATED;
            }
            else
            {
                GetInstance().m_state.m_u32Events |= InputState::EVENT_WINDOW_ACTIVATED;
            }
        }
        break;
    case WM_SYSCOMMAND: 
        {
            switch ( _wParam )
            {
            case SC_SCREENSAVE:
            case SC_MONITORPOWER:
                return 0;
            }
        }
        break;
    case WM_SIZE:
        {
            //clGuiMain.SetSize( LOWORD( lParam ), HIWORD( lParam ) ) ;
		    return 0;
        }
        break;
    case WM_CLOSE:
		{
            PostQuitMessage( 0 );
            return 0;
		}
        break;
	case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDBLCLK:
	    {
		    // If we don't set the capture we won't get mouse move
		    // messages when the mouse moves outside the window.
		    SetCapture( _hWnd );
		    GetInstance().HandleWindowsMouseMessage( _uMsg, _lParam );
		    return 0;
	    }
        break;
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
	    {
		    // Remember to release the capture when we are finished.
		    ReleaseCapture();
		    GetInstance().HandleWindowsMouseMessage( _uMsg, _lParam );
		    return 0;
	    }
        break;
    case WM_MOUSEMOVE:
	    {
		    GetInstance().HandleWindowsMouseMessage( _uMsg, _lParam );
		    return 0;
	    }
        break;
    case WM_KEYDOWN:
    case WM_KEYUP:
	case WM_CHAR:
        {
            GetInstance().HandleWindowsKeyboardMessage( _uMsg, _wParam, _lParam );
            return 0;
        }
        break;
    }
    
    return DefWindowProc( _hWnd, _uMsg, _wParam, _lParam );
}

bool Input::HandleWindowsMouseMessage( UINT _uMsg, LPARAM _lParam )
{
	Integer32 i32PosX = LOWORD( _lParam );
	Integer32 i32PosY = HIWORD( _lParam );

	if ( i32PosX > 10000 )
	{
		i32PosX = 0;
	}
	if ( i32PosY > 10000 )
	{
		i32PosY = 0;
	}

	// Update mouse position
    m_state.m_vMousePosition.Set( i32PosX, i32PosY );

	// Update mouse button states
	switch ( _uMsg )
	{
	case WM_LBUTTONDOWN:
        {
            m_state.m_eMouseButtonStates[ MOUSE_LEFT ] = MBS_FIRST_DOWN;

            // Dispatch event
            DispatchEvent( INPUT_EVENT_LMB_PRESSED );
        }
		break;
	case WM_RBUTTONDOWN:
        {
            m_state.m_eMouseButtonStates[ MOUSE_RIGHT ] = MBS_FIRST_DOWN;

            // Dispatch event
            DispatchEvent( INPUT_EVENT_RMB_PRESSED );
        }
		break;
	case WM_LBUTTONUP:
        {
            m_state.m_eMouseButtonStates[ MOUSE_LEFT ] = MBS_FIRST_UP;

            // Dispatch event
            DispatchEvent( INPUT_EVENT_LMB_RELEASED );
        }
		break;
	case WM_RBUTTONUP:
        {
            m_state.m_eMouseButtonStates[ MOUSE_RIGHT ] = MBS_FIRST_UP;

            // Dispatch event
            DispatchEvent( INPUT_EVENT_RMB_RELEASED );
        }
		break;
	case WM_LBUTTONDBLCLK:
        {
            m_state.m_eMouseButtonStates[ MOUSE_LEFT ] = MBS_FIRST_DOWN;

            // Dispatch event
            DispatchEvent( INPUT_EVENT_LMB_DOUBLE_CLICKED );
        }
		break;
	case WM_RBUTTONDBLCLK:
        {
            m_state.m_eMouseButtonStates[ MOUSE_RIGHT ] = MBS_FIRST_DOWN;

            // Dispatch event
            DispatchEvent( INPUT_EVENT_RMB_DOUBLE_CLICKED );
        }
		break;
	}

    return true;
}

bool Input::HandleWindowsKeyboardMessage( UINT _uMsg, WPARAM _wParam, LPARAM /*lParam*/ )
{
    KeyCode eKey = static_cast< KeyCode >( _wParam );

	switch ( _uMsg )
	{
	case WM_CHAR:
		{
            m_state.m_awcTypedCharacters.push_back( static_cast< wchar_t >( _wParam ) );
		}
		break;
	case WM_KEYDOWN:
        {
            //if ( m_state.m_eKeyStates[ eKey ] != KEY_STATE_DOWN )
            if ( eKey < NUM_KEYS )
            {
                m_state.m_eKeyStates[ eKey ] = KEY_STATE_FIRST_DOWN;

                // Dispatch event
                DispatchEvent( INPUT_EVENT_KEY_FIRST_DOWN, eKey );
            }

            // Dispatch event
            DispatchEvent( INPUT_EVENT_KEY_DOWN, eKey );
        }
        break;
	case WM_KEYUP:
		{
            //if ( m_state.m_eKeyStates[ eKey ] != KEY_STATE_UP )
            if ( eKey < NUM_KEYS )
            {
                m_state.m_eKeyStates[ eKey ] = KEY_STATE_FIRST_UP;

                // Dispatch event
                DispatchEvent( INPUT_EVENT_KEY_FIRST_UP, eKey );
            }

            // Dispatch event
            DispatchEvent( INPUT_EVENT_KEY_UP, eKey );
        }
        break;
	}

	return true;
}
#endif