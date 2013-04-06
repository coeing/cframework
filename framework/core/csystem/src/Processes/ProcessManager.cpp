#include "csystem/Processes/ProcessManager.h"

#include "csystem/Processes/Impl/ProcessManagerImpl.h"

using namespace CSystem;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// ProcessManager
///
////////////////////////////////////////////////////////////////////////////////////////////

ProcessManagerPtr ProcessManager::s_spInstance;

ProcessManagerPtr ProcessManager::GetInstance()
{
    if ( s_spInstance == NULL )
    {
        s_spInstance = ProcessManagerPtr( new ProcessManagerImpl() );
    }
    return s_spInstance;
}