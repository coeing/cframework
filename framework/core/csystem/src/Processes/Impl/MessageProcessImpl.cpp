#include "csystem/Processes/Impl/MessageProcessImpl.h"

#include "cfoundation/Events/Impl/MessageQueueImpl.h"

using namespace CSystem;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// ProcessImpl
///
////////////////////////////////////////////////////////////////////////////////////////////

MessageProcessImpl::MessageProcessImpl()
:   m_bStop( false ),
    m_spMessageQueue( new CFoundation::MessageQueueImpl() )
{
}
        
void MessageProcessImpl::Run()
{
    while ( !m_bStop )
    {
        boost::mutex::scoped_lock lock( m_Mutex );

        // Wait for update
        while ( !m_bStop &&
                 m_spMessageQueue->GetNumMessages() == NULL )
        {
            m_CondUpdate.wait( lock );
        }

        // Get message from queue and broadcast
        CFoundation::BaseMessagePtr spMsg = m_spMessageQueue->TryReceive();
        BroadcastMsg( spMsg );
    }
}
        
void MessageProcessImpl::Stop()
{
    boost::mutex::scoped_lock lock( m_Mutex );

    m_bStop = true;

    // Clear remaining messages
    m_spMessageQueue->Clear();

    // Notify about update
    m_CondUpdate.notify_one();
}
        
void MessageProcessImpl::SendMsg( CFoundation::BaseMessagePtr _spMsg )
{
    boost::mutex::scoped_lock lock( m_Mutex );

    if ( m_bStop )
    {
        return;
    }

    // Add to queue
    m_spMessageQueue->Send( _spMsg );

    // Notify about update
    m_CondUpdate.notify_one();
}
        
void MessageProcessImpl::EmitMsg( CFoundation::BaseMessagePtr _spMsg )
{
    // Not possible from message process
}
        
void MessageProcessImpl::AddProcess( ProcessPtr _spProcess )
{
    boost::mutex::scoped_lock lock( m_ProcessesMutex );
    m_aspProcesses.push_back( _spProcess );
}

void MessageProcessImpl::RemoveProcess( ProcessPtr _spProcess )
{
    boost::mutex::scoped_lock lock( m_ProcessesMutex );
    ProcessVec::iterator it = std::find( m_aspProcesses.begin(), m_aspProcesses.end(), _spProcess );
    if ( it != m_aspProcesses.end() )
    {
        m_aspProcesses.erase( it );
    }
}
        
void MessageProcessImpl::BroadcastMsg( CFoundation::BaseMessagePtr _spMsg )
{
    if ( _spMsg == NULL )
    {
        return;
    }

    boost::mutex::scoped_lock lock( m_ProcessesMutex );
    for ( ProcessVec::iterator it = m_aspProcesses.begin(); it != m_aspProcesses.end(); ++it )
    {
        ProcessPtr spProcess = *it;
        spProcess->SendMsg( _spMsg );
    }
}

