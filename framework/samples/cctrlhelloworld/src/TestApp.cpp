#include "cctrlhelloworld/TestApp.h"
#include "cctrlhelloworld/TestWindow.h"

using namespace CTest;

void TestApp::CreateStartWindows()
{
    m_spTestWindow = WindowController::CreateWindow< TestWindow >();
}
		
void TestApp::Deinit()
{
    if ( m_spTestWindow != NULL )
    {
        m_spTestWindow->Close();
        m_spTestWindow.reset();
    }

    CCtrl::App::Deinit();
}

