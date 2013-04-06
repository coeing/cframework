#include "cgui/GUI/RenderWindow.h"

#include <windows.h>

#include "csystem/Input/Input.h"

using namespace CGui;

HDC g_hDC = NULL;
HWND g_hWnd = NULL;         // The window handle

bool RegisterCGuiClass( WNDPROC WndProc, HINSTANCE hInstance, const CFoundation::String &sClassName )
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	wc.lpfnWndProc = WndProc;					// WndProc behandelt die auftretenden Nachrichten
	wc.cbClsExtra = 0;							// Wird nicht ben�tigt
	wc.cbWndExtra = 0;							// und das auch nicht
	wc.hInstance = hInstance;					// Die Instanz wird festgelegt
	wc.hIcon = LoadIcon( NULL, IDI_WINLOGO ); 	// L�dt das Standardsymbol
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );	// L�dt einen Cursor
	wc.hbrBackground = NULL;					// Es soll kein bestimmter Hintergrund angezeigt werden
	wc.lpszMenuName = NULL;						// Auch ein Men� wird nicht ben�tigt.
	wc.lpszClassName = sClassName.wc_str();		// Name der Klasse

	if( !RegisterClass( &wc ) ) // Versuch die Klasse zu registrieren
	{
		MessageBox( NULL, L"Konnte die Fensterklasse nicht registrieren.", L"ERROR", MB_OK|MB_ICONEXCLAMATION );
		return false;
	}
	else
	{
		return true;
	}
}

bool CreateCGuiWindow(	HWND &hWnd, HINSTANCE hInstance, const CFoundation::String &sWindowName,
						Unsigned32 u32Width, Unsigned32 u32Height )
{
	hWnd = CreateWindowEx(
			WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,		// Die erweiterten Eigenschaften des Fensters
			L"CGui Window",							// Der Name der Klasse
			sWindowName.wc_str(),					// Der Titel des Fensters
			WS_CLIPSIBLINGS |						// Wird von OpenGL ben�tigt
			WS_CLIPCHILDREN |						// Wird auch von OpenGL ben�tigt
			WS_OVERLAPPEDWINDOW,					// auch Eigenschaften des Fensters
			0, 0,									// Die Position des zu erstellenden Fensters
			u32Width, 								// Hier werden die ermittelten Werte f�r die Breite eingesetzt
			u32Height,								// und hier f�r die H�he
			NULL,									// Es soll kein �bergordnetes Fendster erstellt werden
			NULL,									// kein Men�
			hInstance,								// Die Instanz wird �bergeben
			NULL );									// Wird nicht ben�tigt

	if( hWnd == NULL )	// test if creating the window succeded
	{
		MessageBox( NULL, L"Fenster konnte nicht erstellt werden.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
		return false;
	}

	return true;
}

RenderWindow::RenderWindow()
:	m_u32Width( 800 ),
	m_u32Height( 600 )
{
}

bool RenderWindow::Init( const CFoundation::String &sWindowName, Unsigned32 u32Width, Unsigned32 u32Height )
{
	m_u32Width = u32Width;
	m_u32Height = u32Height;

	HINSTANCE hInstance = GetModuleHandle( NULL );

    if( !RegisterCGuiClass( &(CSystem::Input::WndProc), hInstance, "CGui Window" ) )
	{
		return false;
	}

	if( !CreateCGuiWindow( g_hWnd, hInstance, sWindowName, m_u32Width + 8, m_u32Height + 27 ) )
	{
		return false;
	}

	g_hDC = GetDC( g_hWnd );

	if( g_hDC == NULL )
	{
		MessageBox( NULL, L"Konnte keinen DC erstellen.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
		return false;
	}

	static PIXELFORMATDESCRIPTOR pfd =
	// pdf ist jetzt ein PIXELFORMATDESCRIPTOR
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		// Die gr��e muss nat�rlich stimmen
		1, // Versionsnummer
		PFD_DRAW_TO_WINDOW |
		// Das Format muss in Fenster sichtbar sein k�nnen
		PFD_SUPPORT_OPENGL |
		// OpenGL muss unterst�tzt werden
		PFD_DOUBLEBUFFER,
		// Double Buffering muss unterst�tzt werden
		PFD_TYPE_RGBA,
		// Das RGBA (Rot,Gr�n,Blau,Alpha(Transparenz))
		// muss unterst�tzt werden
		32,
		// Die Farbtiefe, die schon
		// �bergeben wurde, wird hier ben�tigt
		0, 0, 0, 0, 0, 0, // wird nicht ben�tigt
		0, // kein Alpha Buffer
		0, // Shift Bit ignoriert
		0, // kein Accumulation Buffer
		0, 0, 0, 0, // nicht ben�tigt
		16, // 16Bit Z-Buffer (Depth Buffer)
		0, // kein Stencil Buffer
		0, // kein Auxiliary Buffer
		PFD_MAIN_PLANE,
		// Die Hauptebene auf die sp�ter gezeichnet wird
		0, // unwichtig
		0, 0, 0 // keine Ebenenmasken ben�tigt
	};

	Unsigned32 u32PixelFormat = ChoosePixelFormat( g_hDC, &pfd );
	if ( u32PixelFormat == 0 )
	// Kann Windows ein passendes finden?
	{
		// Falls keins gefunden werden kann:
		MessageBox( NULL, L"Konnte kein passendes Pixelformat finden.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
		return false; // FALSE zur�ck und Ende.
	}


	if ( !SetPixelFormat( g_hDC, u32PixelFormat, &pfd ) )
	// Kann das Pixelformat gesetzt werden?
    {
		MessageBox( NULL, L"Could not set pixel format", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
		return false; // FALSE zur�ck und raus
	}

	HGLRC hRC = wglCreateContext( g_hDC ); // Versuch den RC zu bekommen
	if( hRC == NULL )
    {
		MessageBox( NULL, L"Could not create rendering context", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
		return false;
    }



    if( !wglMakeCurrent( g_hDC, hRC ) ) // Versuch den RC zu aktivieren
    {
        MessageBox( NULL, L"Could not activate rendering context", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
        return false;
    }


	ShowWindow( g_hWnd, SW_SHOW );	// Fenster anzeigen
	SetForegroundWindow( g_hWnd );	// Priorit�t des Programms wird erh�ht
	SetFocus( g_hWnd );				// Tastatureingaben werden
									// jetzt an das Programm geleitet


	return true;
}

Unsigned32 RenderWindow::GetWidth() const
{
	return m_u32Width;
}

Unsigned32 RenderWindow::GetHeight() const
{
	return m_u32Height;
}

void RenderWindow::SetTitle( const CFoundation::String &sTitle )
{
	SetWindowText( g_hWnd, sTitle.wc_str() );
}

const CFoundation::String RenderWindow::GetTitle() const
{
	wchar_t szTmp[ 256 ];
	GetWindowText( g_hWnd, szTmp, 256 );
	return CFoundation::String( szTmp );
}
        
CFoundation::Vector2Di32 RenderWindow::ClientToScreen( const CFoundation::Vector2Di32& _vPoint ) const
{
    CFoundation::Vector2Di32 vPoint = _vPoint;
#ifdef WIN32
    POINT point;
    point.x = vPoint.GetX();
    point.y = vPoint.GetY();
    ::ClientToScreen( g_hWnd, &point );
    vPoint.Set( point.x, point.y );
#endif
    return vPoint;
}
 
CFoundation::Vector2Di32 RenderWindow::ScreenToClient( const CFoundation::Vector2Di32& _vPoint ) const
{
    CFoundation::Vector2Di32 vPoint = _vPoint;
#ifdef WIN32
    POINT point;
    point.x = vPoint.GetX();
    point.y = vPoint.GetY();
    ::ScreenToClient( g_hWnd, &point );
    vPoint.Set( point.x, point.y );
#endif
    return vPoint;
}

void RenderWindow::Update()
{
	SwapBuffers( g_hDC ); // Die Puffer werden getauscht
}
