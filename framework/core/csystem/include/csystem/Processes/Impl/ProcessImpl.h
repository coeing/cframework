#ifndef CSYSTEM_PROCESS_IMPL_H
#define CSYSTEM_PROCESS_IMPL_H

#include "cfoundation/Base/Types.h"
#include "cfoundation/Utils/SharedPtr.h"

#include "csystem/Processes/Process.h"

namespace CFoundation
{
    INCLUDE_SHARED_CLASS( MessageQueue );
}

namespace CSystem
{
    INCLUDE_SHARED_CLASS( ProcessManager );
    
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// ProcessImpl
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class ProcessImpl : public Process
	{
	public:
        /// Constructor
        ProcessImpl( ProcessManagerPtr _spProcessManager );

        /// Run method that's called when the thread of the process starts
        virtual void            Run();

        /// Stop method that's called before the thread of the process is stopped
        virtual void            Stop();

        /// Initialization
        virtual void            Init();

        /// Deinitialization
        virtual void            Deinit();

        /// Per frame update
        virtual void            Update( Float32 _f32TimeDifference );

        /// Sends a message to this process
        virtual void            SendMsg( CFoundation::BaseMessagePtr _spMsg );

    protected:
        /// Sets the maximum frame time
        void                    SetMaxFrameTime( Unsigned64 _u64MaxFrameTimeMs );

    private:
        /// Indicates if the process should stop
        bool                            m_bStop;

        /// Process manager
        ProcessManagerPtr               m_spProcessManager;

        /// Message queue
        CFoundation::MessageQueuePtr    m_spMessageQueue;

        /// Maximum frame time
        Unsigned64                      m_u64MaxFrameTimeMs;
	};
}

#endif
