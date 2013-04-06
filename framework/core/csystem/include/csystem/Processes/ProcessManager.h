#ifndef CSYSTEM_PROCESS_MANAGER_H
#define CSYSTEM_PROCESS_MANAGER_H

#include "cfoundation/Base/Types.h"
#include "cfoundation/Utils/SharedPtr.h"

#include "csystem/Processes/Process.h"

namespace CFoundation
{
    INCLUDE_SHARED_CLASS( BaseMessage );
}

namespace CSystem
{    
    INCLUDE_SHARED_CLASS( Process );
    INCLUDE_SHARED_CLASS( ProcessManager );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// ProcessManager
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class ProcessManager
	{
	public:
        /// Returns the process manager instance
        static ProcessManagerPtr    GetInstance();

        /// Runs the process manager
        virtual void                Run() = 0;

        /// Stops the process manager
        virtual void                Stop() = 0;

        /// Adds a process
        virtual void                AddProcess( ProcessPtr _spProcess ) = 0;

        /// Removes a process
        virtual void                RemoveProcess( ProcessPtr _spProcess ) = 0;

        /// Returns the process with the passed ID
        virtual ProcessPtr          GetProcess( const Process::ID& _ProcessID ) const = 0;

        /// Broadcast a message to all processes
        virtual void                BroadcastMsg( CFoundation::BaseMessagePtr _spMsg ) = 0;

    private:
        /// Instance
        static ProcessManagerPtr    s_spInstance;
	};
}

#endif
