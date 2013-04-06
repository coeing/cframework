#include "cctrl/WindowManager.h"

#include <algorithm>

#include "cgui/Input/Input.h"

using namespace CCtrl;

WindowManager::~WindowManager()
{
    m_lspWindows.clear();
    m_spModalWindow.reset();
}

void WindowManager::DestroyWindow( WindowControllerCPtr _spWindow )
{
	WindowList::iterator it = std::find( m_lspWindows.begin(), m_lspWindows.end(), _spWindow );
	if ( it != m_lspWindows.end() )
	{
		m_lspWindows.erase( it );
	}
}
		
void WindowManager::ProcessInput( const CSystem::InputState& _state )
{
    // If modal window, just let it process input
    if ( m_spModalWindow != NULL )
    {
        m_spModalWindow->ProcessInput( _state );
    }
    else
    {
        WindowList lspWindows = m_lspWindows;
        for ( WindowList::iterator it = lspWindows.begin(); it != lspWindows.end(); ++it )
        {
            (*it)->ProcessInput( _state );
        }
    }
}	

void WindowManager::Update( Float32 _f32TimeDifference )
{
    // Handle close requests
    for ( CloseRequestVec::iterator it = m_aCloseRequests.begin(); it != m_aCloseRequests.end(); ++it )
    {
        const CloseRequest& request = (*it);
        request.m_spWindow->Close( request.m_u32ReturnValue );
    }
    m_aCloseRequests.clear();

    // Update windows
    WindowList lspWindows = m_lspWindows;
    for ( WindowList::iterator it = lspWindows.begin(); it != lspWindows.end(); ++it )
    {
        (*it)->Update( _f32TimeDifference );
    }
}

void WindowManager::SetModalWindow( WindowControllerPtr _spWindow )
{
    m_spModalWindow = _spWindow;

    // Set as input root
    if ( m_spModalWindow != NULL )
    {
        CGui::Input::GetInstance().SetInputRoot( m_spModalWindow->GetWindow() );
    }
    else
    {
        CGui::Input::GetInstance().SetInputRoot( CGui::WindowPtr() );
    }
}

WindowControllerPtr WindowManager::GetModalWindow() const
{
    return m_spModalWindow;
}
       
void WindowManager::RequestCloseWindow( WindowControllerPtr _spWindow, Unsigned32 _u32ReturnValue )
{
    CloseRequest request;
    request.m_spWindow = _spWindow;
    request.m_u32ReturnValue = _u32ReturnValue;
    m_aCloseRequests.push_back( request );
}
