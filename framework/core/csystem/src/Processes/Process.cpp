#include "csystem/Processes/Process.h"

#include "csystem/Processes/ProcessID.h"
#include "csystem/Processes/ProcessManager.h"

using namespace CSystem;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// Process
///
////////////////////////////////////////////////////////////////////////////////////////////

ProcessPtr Process::GetInstance()
{
    return ProcessManager::GetInstance()->GetProcess( Process::ID( boost::this_thread::get_id() ) );
}
        
void Process::EmitMsg( CFoundation::BaseMessagePtr _spMsg )
{
    ProcessManager::GetInstance()->BroadcastMsg( _spMsg );
}
