#include "cgui/Input/Cursor.h"

#ifdef WIN32
#include <windows.h>
#endif

#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/RenderWindow.h"

using namespace CGui;

#ifdef WIN32
extern HWND g_hWnd;
#endif

Cursor::Cursor()
:	m_eType( TYPE_NONE ),
    m_bVisible( true ),
    m_bClipped( false )
{
}

void Cursor::SetType( CursorType _eType )
{
    if( m_eType == _eType )
    {
        return;
    }

    m_eType = _eType;
            
    //Ogre::MaterialPtr pMaterial( NULL );
    HCURSOR hCursor = NULL;
    switch ( m_eType )
    {
    case TYPE_ARROW:
        SetVisible( true );
        hCursor = LoadCursor( NULL, IDC_ARROW );
        break;
    case TYPE_SIZE_HORIZONTAL:
        SetVisible( true );
        hCursor = LoadCursor( NULL, IDC_SIZEWE );
        break;
    case TYPE_SIZE_VERTICAL:
        SetVisible( true );
        hCursor = LoadCursor( NULL, IDC_SIZENS );
        break;
    case TYPE_SIZE_DIAGONAL_LR:
        SetVisible( true );
        hCursor = LoadCursor( NULL, IDC_SIZENWSE );
        break;
    case TYPE_SIZE_DIAGONAL_RL:
        SetVisible( true );
        hCursor = LoadCursor( NULL, IDC_SIZENESW );
        break;
    case TYPE_NONE:
    default:
        SetVisible( false );
        break;
    }

    if( hCursor != NULL )
    {
        SetCursor( hCursor );
        SetClassLong( g_hWnd, GCL_HCURSOR, (LONG)hCursor );
    }
}
       
void Cursor::SetVisible( bool _bVisible )
{
    if ( _bVisible == m_bVisible )
    {
        return;
    }

    m_bVisible = _bVisible;

#ifdef WIN32
    ::ShowCursor( m_bVisible );
#endif
}
       
bool Cursor::IsVisible() const
{
    return m_bVisible;
}
        
void Cursor::ClipCursor( const CFoundation::RectI32& _Rect, bool _bScreen )
{
    // Convert coordinates if necessary
    m_rectClipRect = _Rect;
    if ( !_bScreen )
    {
        RenderWindowPtr spRenderWindow = GuiMain::GetInstance().GetRenderWindow();
        m_rectClipRect.SetPosition( spRenderWindow->ClientToScreen( m_rectClipRect.GetPosition() ) );
    }

    m_bClipped = true;

#ifdef WIN32
    RECT rect;
    rect.top = m_rectClipRect.GetTop();
    rect.right = m_rectClipRect.GetRight();
    rect.bottom = m_rectClipRect.GetBottom();
    rect.left = m_rectClipRect.GetLeft();
    ::ClipCursor( &rect );
#endif
}
       
void Cursor::UnclipCursor()
{
#ifdef WIN32
    ::ClipCursor( NULL );
#endif
    
    m_bClipped = false;
}
       
bool Cursor::IsClipped( CFoundation::RectI32& _ClipRect ) const
{
    _ClipRect = m_rectClipRect;
    return m_bClipped;
}
