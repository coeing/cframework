#include "cmadtvclient/Application/Application.h"

#include "cgui/Gui/GuiStyle.h"

#include "cmadtvclient/Processes/Logic/LogicProcess.h"
#include "cmadtvclient/Processes/Network/NetworkProcess.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// ClientApplication
///
////////////////////////////////////////////////////////////////////////////////////////////

ClientApplication::ClientApplication()
{
}

ClientApplication::~ClientApplication()
{
}
		
bool ClientApplication::Init()
{
    if ( !CCtrl::MultiThreadedApp::Init() )
    {
        return false;
    }

    // Add processes
    m_spProcessManager->AddProcess( CSystem::ProcessPtr( new NetworkProcess( m_spProcessManager ) ) );
    m_spProcessManager->AddProcess( CSystem::ProcessPtr( new LogicProcess( m_spProcessManager ) ) );

    return true;
}

void ClientApplication::Deinit()
{
    CCtrl::MultiThreadedApp::Deinit();
}
