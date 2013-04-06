#include "csystem/Processes/Impl/ProcessManagerImpl.h"

#include <algorithm>

#include <boost/thread.hpp> 

#include "cfoundation/Base/Defines.h"

#include "csystem/Processes/Process.h"
#include "csystem/Processes/ProcessID.h"
#include "csystem/Processes/Impl/MessageProcessImpl.h"

using namespace CSystem;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// ProcessManagerImpl::ProcessThread
///
////////////////////////////////////////////////////////////////////////////////////////////

class ProcessManagerImpl::ProcessThread
{
public:
    typedef boost::thread Thread;
    typedef SharedPtr< Thread > ThreadPtr;
    typedef boost::thread::id ThreadID;

    /// Constructor
    ProcessThread( ProcessPtr _spProcess )
    :   m_spProcess( _spProcess )
    {
    }

    /// Returns the thread ID
    Process::ID GetID() const
    {
        return m_spThread != NULL ? Process::ID( m_spThread->get_id() ) : Process::ID( ThreadID() );
    }

    /// Starts the thread
    void        Start()
    {
        if ( m_spThread != NULL )
        {
            return;
        }
        m_spThread = ThreadPtr( new Thread( boost::bind( &Process::Run, m_spProcess.get() ) ) );
    }

    /// Joins the thread
    void        Join()
    {
        SAFE_CALL( m_spThread )->join();
    }

    /// Stops the thread
    void        Stop()
    {
        if ( m_spThread == NULL )
        {
            return;
        }
        m_spProcess->Stop();
    }

    /// Process
    ProcessPtr  m_spProcess;

    /// Thread
    ThreadPtr   m_spThread;
};

////////////////////////////////////////////////////////////////////////////////////////////
///
/// ProcessManagerImpl
///
////////////////////////////////////////////////////////////////////////////////////////////

ProcessManagerImpl::ProcessManagerImpl()
:   m_bStarted( false ),
    m_spMessageProcess( new MessageProcessImpl )
{
    m_aspProcessThreads.push_back( ProcessThreadPtr( new ProcessThread( m_spMessageProcess ) ) );
}

void ProcessManagerImpl::Run()
{
    if ( m_bStarted )
    {
        return;
    }

    m_bStarted = true;

    // Start threads for added processes
    for ( ProcessVec::iterator it = m_aspProcessThreads.begin(); it != m_aspProcessThreads.end(); ++it )
    {
        ProcessThreadPtr spThread = *it;
        spThread->Start();
    }

    // Join threads
    for ( ProcessVec::iterator it = m_aspProcessThreads.begin(); it != m_aspProcessThreads.end(); ++it )
    {
        ProcessThreadPtr spThread = *it;
        spThread->Join();
    }
}
        
void ProcessManagerImpl::Stop()
{
    // Stop threads
    for ( ProcessVec::iterator it = m_aspProcessThreads.begin(); it != m_aspProcessThreads.end(); ++it )
    {
        ProcessThreadPtr spThread = *it;
        spThread->Stop();
    }
}
        
void ProcessManagerImpl::AddProcess( ProcessPtr _spProcess )
{
    if ( m_bStarted )
    {
        return;
    }

    boost::mutex::scoped_lock lock( m_ProcessesMutex );
    m_aspProcessThreads.push_back( ProcessThreadPtr( new ProcessThread( _spProcess ) ) );
    m_spMessageProcess->AddProcess( _spProcess );
}

void ProcessManagerImpl::RemoveProcess( ProcessPtr _spProcess )
{
    if ( m_bStarted )
    {
        return;
    }

    boost::mutex::scoped_lock lock( m_ProcessesMutex );
    for ( ProcessVec::iterator it = m_aspProcessThreads.begin(); it != m_aspProcessThreads.end(); ++it )
    {
        ProcessThreadPtr spThread = *it;
        if ( spThread->m_spProcess == _spProcess )
        {
            // Remove
            it = m_aspProcessThreads.erase( it );
            break;
        }
    }
    m_spMessageProcess->RemoveProcess( _spProcess );
}
        
ProcessPtr ProcessManagerImpl::GetProcess( const Process::ID& _ProcessID ) const
{
    boost::mutex::scoped_lock lock( m_ProcessesMutex );
    for ( ProcessVec::const_iterator it = m_aspProcessThreads.begin(); it != m_aspProcessThreads.end(); ++it )
    {
        ProcessThreadPtr spThread = *it;
        if ( spThread->GetID() == _ProcessID )
        {
            return spThread->m_spProcess;
        }
    }
    return ProcessPtr();
}
        
void ProcessManagerImpl::BroadcastMsg( CFoundation::BaseMessagePtr _spMsg )
{
    m_spMessageProcess->SendMsg( _spMsg );
}
