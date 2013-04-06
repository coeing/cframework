#include "cofiles/MessageSystem.h"

#include "cofiles/Timer.h"

using namespace CoFiles;

bool MessageSystem::ProcessMessage()
{
    if ( m_pMessages.size() > 0 )
    {
	    CoFiles::Message *pMessage = m_pMessages.front();
	    m_pMessages.pop_front();

	    pMessage->SendToReceiver();

        #ifndef FINAL
        //CoFiles::String sLog;
        //sLog.FormatString( "MessageSystem::ProcessMessages: Sended message with ID %d", pMessage->GetId() );
        //CoFiles::Logger::GetInstance().Log( sLog );
        #endif

	    delete pMessage;

        return true;
    }
	else
	{
		return false;
	}
}

void MessageSystem::Send( const Message& _clMessage )
{
	// Add to open messages
	m_pMessages.push_back( _clMessage.Clone() );
}
        
Unsigned32 MessageSystem::GetNumMessages() const
{
    return m_pMessages.size();
}
        
void MessageSystem::DeletedReceiver( MessageReceiverConstPtr _spMessageReceiver )
{
	// check for unhandled messages
	for ( std::list< Message* >::iterator it = m_pMessages.begin(); it != m_pMessages.end(); )
	{
		Message* pMessage = *it;

		if( pMessage->IsReceiver( _spMessageReceiver ) )
		{
			it = m_pMessages.erase( it );
			delete pMessage;
		}
		else
		{
		    ++it;
		}
	}
}
