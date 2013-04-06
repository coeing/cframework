#ifndef CSYSTEM_PROCESS_H
#define CSYSTEM_PROCESS_H

#include "cfoundation/Events/MessageDispatcher.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CFoundation
{
    INCLUDE_SHARED_CLASS( BaseMessage );
}

namespace CSystem
{    
    INCLUDE_SHARED_CLASS( Process );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// Process
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class Process : public CFoundation::MessageDispatcher
	{
	public:
        class ID;

        /// Returns the current process instance
        static ProcessPtr       GetInstance();

        /// Run method that's called when the thread of the process starts
        virtual void            Run() = 0;

        /// Stop method that's called before the thread of the process is stopped
        virtual void            Stop() = 0;

        /// Sends a message to this process
        virtual void            SendMsg( CFoundation::BaseMessagePtr _spMsg ) = 0;

        /// Emits a message
        /// I.e. sending it to all processes
        static void             EmitMsg( CFoundation::BaseMessagePtr _spMsg );
	};
}

#endif
