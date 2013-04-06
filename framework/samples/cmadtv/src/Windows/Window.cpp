#include "cmadtv/Windows/Window.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// Window
///
////////////////////////////////////////////////////////////////////////////////////////////
        
Window::Window()
{
}

Window::~Window()
{
}
        
void Window::SetContext( PlayerContextPtr _spContext )
{
    if ( _spContext == m_spContext )
    {
        return;
    }

    m_spContext = _spContext;

    OnContextChanged();
}
        
void Window::OnContextChanged()
{
}
		
void Window::Close( Unsigned32 _u32ReturnValue )
{
    SetContext( PlayerContextPtr() );

    WindowController::Close( _u32ReturnValue );
}
        