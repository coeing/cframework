#ifndef CSYSTEM_PROCESS_ID_H
#define CSYSTEM_PROCESS_ID_H

#include <boost/thread/thread.hpp>

#include "csystem/Processes/Process.h"

namespace CSystem
{    
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// Process::ID
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class Process::ID
    {
    public:
        /// Constructor
        ID( const boost::thread::id& _threadID );

        /// Copy operator
        const ID&           operator=( const ID& _rhs );

        /// Compare operator
        bool                operator==( const ID& _rhs );

        /// Thread ID
        boost::thread::id   m_threadID;
    };
}

#endif
