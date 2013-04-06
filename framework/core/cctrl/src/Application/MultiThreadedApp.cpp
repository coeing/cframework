#include "cctrl/Application/MultiThreadedApp.h"

#include "csystem/Input/Input.h"
#include "csystem/Processes/ProcessManager.h"

using namespace CCtrl;

MultiThreadedApp::MultiThreadedApp()
:   m_spProcessManager( CSystem::ProcessManager::GetInstance() )
{
}

MultiThreadedApp::~MultiThreadedApp()
{
}

bool MultiThreadedApp::Init()
{
    // Set exit delegate
    CSystem::ExitDelegatePtr spExitDelegate( new CSystem::ExitDelegate( this, &MultiThreadedApp::Exit ) );
    CSystem::Input::GetInstance().SetExitDelegate( spExitDelegate );

	return true;
}
		
void MultiThreadedApp::Deinit()
{
}

void MultiThreadedApp::Start()
{
	Init();

    // Run process manager
    m_spProcessManager->Run();

    // Deinitialize
    Deinit();
}

void MultiThreadedApp::Exit()
{
    // Stop process manager
    m_spProcessManager->Stop();
}
