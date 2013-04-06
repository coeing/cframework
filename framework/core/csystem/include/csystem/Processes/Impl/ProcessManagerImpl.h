#ifndef CSYSTEM_PROCESS_MANAGER_IMPL_H
#define CSYSTEM_PROCESS_MANAGER_IMPL_H

#include <vector>

#include <boost/thread/mutex.hpp>

#include "csystem/Processes/ProcessManager.h"

namespace CSystem
{    
    INCLUDE_SHARED_CLASS( MessageProcessImpl );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// ProcessManagerImpl
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class ProcessManagerImpl : public ProcessManager
	{
	public:
        /// Constructor
        ProcessManagerImpl();

        /// Runs the process manager
        virtual void                Run();

        /// Stops the process manager
        virtual void                Stop();

        /// Adds a process
        virtual void                AddProcess( ProcessPtr _spProcess );

        /// Removes a process
        virtual void                RemoveProcess( ProcessPtr _spProcess );

        /// Returns the process with the passed ID
        virtual ProcessPtr          GetProcess( const Process::ID& _ProcessID ) const;

        /// Broadcast a message to all processes
        virtual void                BroadcastMsg( CFoundation::BaseMessagePtr _spMsg );

    private:
        /// Process thread
        INCLUDE_SHARED_CLASS( ProcessThread );

        typedef std::vector< ProcessThreadPtr > ProcessVec;

        /// Indicates if the process manager was already started
        bool                        m_bStarted;

        /// Message process
        MessageProcessImplPtr       m_spMessageProcess;

        /// Mutex to protect the process vector
        mutable boost::mutex        m_ProcessesMutex;

        /// Process threads
        ProcessVec                  m_aspProcessThreads;
	};
}

#endif
