#ifndef CSYSTEM_MESSAGE_PROCESS_IMPL_H
#define CSYSTEM_MESSAGE_PROCESS_IMPL_H

#include <vector>

#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>

#include "cfoundation/Utils/SharedPtr.h"

#include "csystem/Processes/Process.h"

namespace CFoundation
{
    INCLUDE_SHARED_CLASS( MessageQueue );
}

namespace CSystem
{
    INCLUDE_SHARED_CLASS( Process );
    
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// MessageProcessImpl
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    /// The message process is a special process that's contained by each process manager. 
    /// All messages that are send to this process are broadcasted to all other processes that
    /// were added to this process.

    class MessageProcessImpl : public Process
	{
	public:
        /// Constructor
        MessageProcessImpl();

        /// Run method that's called when the thread of the process starts
        virtual void            Run();

        /// Stop method that's called before the thread of the process is stopped
        virtual void            Stop();

        /// Sends a message to this process
        virtual void            SendMsg( CFoundation::BaseMessagePtr _spMsg );

        /// Emits a message
        /// I.e. sending it to all processes
        virtual void            EmitMsg( CFoundation::BaseMessagePtr _spMsg );

        /// Adds a process
        virtual void            AddProcess( ProcessPtr _spProcess );

        /// Removes a process
        virtual void            RemoveProcess( ProcessPtr _spProcess );

    private:
        typedef std::vector< ProcessPtr > ProcessVec;

        /// Broadcast a message to all processes
        void                    BroadcastMsg( CFoundation::BaseMessagePtr _spMsg );

        /// Indicates if the process should be stopped
        bool                            m_bStop;

        /// Mutex to protect members
        boost::mutex                    m_Mutex;

        /// Condition that indicates that an update is needed
        boost::condition                m_CondUpdate;

        /// Message queue
        CFoundation::MessageQueuePtr    m_spMessageQueue;

        /// Processes
        ProcessVec                      m_aspProcesses;

        /// Mutex to protect the process vector
        boost::mutex                    m_ProcessesMutex;
	};
}

#endif
