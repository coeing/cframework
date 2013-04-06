#include "cofiles/MessageObject.h"
#include "cofiles/MessageSystem.h"
#include "cofiles/Message.h"

using namespace CoFiles;

MessageObject::MessageObject()
{
    //m_spThis = WeakPtr< MessageObject >( this );
}

MessageObject::~MessageObject()
{
	m_aListenersAll.clear();

    for ( MessageListenerMap::iterator it = m_aListeners.begin(); it != m_aListeners.end(); ++it )
    {
        ListenerSet* pListeners = it->second;
        if ( pListeners != NULL )
        {
            delete pListeners;
            pListeners = NULL;
        }
    }
    m_aListeners.clear();
}

void MessageObject::Clear()
{
	m_aListenersAll.clear();

    for ( MessageListenerMap::iterator it = m_aListeners.begin(); it != m_aListeners.end(); ++it )
    {
        ListenerSet* pListeners = it->second;
        if ( pListeners != NULL )
        {
            delete pListeners;
            pListeners = NULL;
        }
    }
    m_aListeners.clear();
}

void MessageObject::ProcessMessage( const Message& _clMessage )
{
    // Check if message should be forwarded
    if ( IsMessageEnabled( _clMessage.GetType() ) )
    {
    	// send message to listeners
	    SendToListeners( _clMessage );
    }
}

void MessageObject::AddListener( MessageReceiverPtr _spListener )
{
	m_aListenersAll.insert( _spListener );
}

void MessageObject::AddListener( MessageReceiverPtr _spListener, Unsigned32 _u32MessageID )
{
    if ( _spListener == NULL )
    {
        return;
    }

    ListenerSet* pListeners = GetListeners( _u32MessageID );
    if ( pListeners == NULL )
    {
        pListeners = new ListenerSet();
        m_aListeners[ _u32MessageID ] = pListeners;
    }
	pListeners->insert( _spListener );
}

void MessageObject::RemoveListener( MessageReceiverPtr _spListener )
{
	m_aListenersAll.erase( _spListener );
}
		
void MessageObject::RemoveListener( MessageReceiverPtr _spListener, Unsigned32 _u32MessageID )
{
    if ( _spListener == NULL )
    {
        return;
    }

    ListenerSet* pListeners = GetListeners( _u32MessageID );
    if ( pListeners == NULL )
    {
        return;
    }
    pListeners->erase( _spListener );

    // Check if empty
    if ( pListeners->empty() )
    {
        m_aListeners.erase( _u32MessageID );
    }
}
        
void MessageObject::Send( const Message& _clMessage ) const
{
    MessageSystem::GetInstance().Send( _clMessage );
}

void MessageObject::SendToListeners( const Message &clMessage ) const
{
    const ListenerSet* pListeners = GetListeners( clMessage.GetType() );

    // Check if there are any listeners
    if ( m_aListenersAll.empty() &&
         pListeners == NULL )
    {
        return;
    }

	Message* pNewMsg = clMessage.Clone();
    // TODO pNewMsg->SetSender( SharedPtr< MessageObject >( m_spThis ) );

	// send message to listeners
	for( ListenerSet::const_iterator it = m_aListenersAll.begin(); it != m_aListenersAll.end(); ++it )
	{
		pNewMsg->SetReceiver( *it );
        MessageSystem::GetInstance().Send( *pNewMsg );
	}

    // Send to listeners who listen to the specific message
    if ( pListeners != NULL )
    {
	    for( ListenerSet::const_iterator it = pListeners->begin(); it != pListeners->end(); ++it )
	    {
		    pNewMsg->SetReceiver( *it );
		    MessageSystem::GetInstance().Send( *pNewMsg );
	    }
    }

	delete pNewMsg;
}

MessageObject::ListenerSet* MessageObject::GetListeners( Unsigned32 _u32MessageID )
{
    MessageListenerMap::iterator it = m_aListeners.find( _u32MessageID );
    if ( it != m_aListeners.end() )
    {
        return it->second;
    }
    else
    {
        return NULL;
    }
}

const MessageObject::ListenerSet* MessageObject::GetListeners( Unsigned32 _u32MessageID ) const
{
    return const_cast<MessageObject*>( this )->GetListeners( _u32MessageID );
}

bool MessageObject::IsMessageEnabled( Unsigned32 u32MessageID ) const
{
    if ( !m_aListenersAll.empty() )
    {
        return true;
    }

    const ListenerSet* pListeners = GetListeners( u32MessageID );
    return pListeners != NULL;
}
