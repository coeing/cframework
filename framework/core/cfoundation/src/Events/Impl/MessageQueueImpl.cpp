#include "cfoundation/Events/Impl/MessageQueueImpl.h"

using namespace CFoundation;

void MessageQueueImpl::Send( BaseMessagePtr _spMsg )
{
    boost::mutex::scoped_lock lock( m_Mutex );

	// Add to open messages
	m_aspMessages.push_back( _spMsg );

    // Notify one thread that there is a new message
    m_CondReceive.notify_one();
}
        
void MessageQueueImpl::Clear()
{
    boost::mutex::scoped_lock lock( m_Mutex );

	// Clear messages
	m_aspMessages.clear();
}
       
BaseMessagePtr MessageQueueImpl::Receive()
{
    boost::mutex::scoped_lock lock( m_Mutex );

    // Wait till there is a message
    while ( m_aspMessages.empty() )
    {
        m_CondReceive.wait( lock );
    }

    // Get first message
    BaseMessagePtr spMsg = m_aspMessages.front();
    m_aspMessages.pop_front();
    return spMsg;
}
       
BaseMessagePtr MessageQueueImpl::TryReceive()
{
    boost::mutex::scoped_lock lock( m_Mutex );

    // Get first message if there is one
    BaseMessagePtr spMsg;
    if ( !m_aspMessages.empty() )
    {
        spMsg = m_aspMessages.front();
        m_aspMessages.pop_front();
    }
    return spMsg;
}
        
Unsigned32 MessageQueueImpl::GetNumMessages() const
{
    return m_aspMessages.size();
}
        