#include "csystem/Input/Keyboard.h"

using namespace CSystem;

#ifdef WIN32
#include <windows.h>

wchar_t CSystem::KeyCodeToChar( const KeyCode &eKeyCode )
{
	static HKL layout = GetKeyboardLayout( 0 );
	static BYTE State[ 256 ];

	if ( GetKeyboardState( State ) == FALSE )
	{
		return 0;
	}

	wchar_t wChar;
	if ( ToUnicodeEx( eKeyCode, 0, State, &wChar, 1, 0, layout ) > 0 )
	{
		return wChar;
	}
	else
	{
		return 0;
	}
}

#endif

template<>
void CFoundation::EnumConverter< KeyCode >::Initialize()
{
#ifdef WIN32
#include <windows.h>
	static HKL layout = GetKeyboardLayout( 0 );
	static BYTE State[ 256 ];

	if ( GetKeyboardState( State ) == FALSE )
	{
		return;
	}

    wchar_t wChar = 0;
    for ( Unsigned32 u32Key = KEY_0; u32Key <= KEY_Z; ++u32Key )
    {
	    if ( ToUnicodeEx( u32Key, 0, State, &wChar, 1, 0, layout ) > 0 )
	    {
            wChar = towupper( wChar );
            Assign( static_cast< KeyCode >( u32Key ), wChar );
	    }
    }
#endif

    for ( Unsigned32 u32Idx = 0; u32Idx < 10; ++u32Idx )
    {
        Assign( static_cast< KeyCode >( KEY_NUMPAD_0 + u32Idx ), CFoundation::String::Build( "Numpad%d", u32Idx ) );
    }
    
    
    Assign( KEY_BACKSPACE,  "Backspace" );
    Assign( KEY_TAB,        "Tab" );
    Assign( KEY_RETURN,     "Return" );
    Assign( KEY_SHIFT,      "Shift" );
    Assign( KEY_ESC,        "Esc" );
    Assign( KEY_SPACE,      "Space" );
    Assign( KEY_LEFT,       "Left" );
    Assign( KEY_UP,         "Up" );
    Assign( KEY_RIGHT,      "Right" );
    Assign( KEY_DOWN,       "Down" );
    Assign( KEY_DEL,        "Delete" );
}

template<>
void CFoundation::EnumConverter< KeyState >::Initialize()
{
    Assign( KEY_STATE_FIRST_DOWN,   "first down" );
    Assign( KEY_STATE_DOWN,         "down" );
    Assign( KEY_STATE_FIRST_UP,     "first up" );
    Assign( KEY_STATE_UP,           "up" );
}

