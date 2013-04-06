#include "cfoundation/Events/Message.h"

using namespace CFoundation;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// BaseMessage
///
////////////////////////////////////////////////////////////////////////////////////////////
RTTI_DEF_EASY( BaseMessage )

////////////////////////////////////////////////////////////////////////////////////////////
///
/// Message
///
////////////////////////////////////////////////////////////////////////////////////////////

Message::Message()
:	m_bHandled( false )
{
}

Message::~Message()
{
}

void Message::SetSender( MessageReceiverConstPtr _spSender )
{
	m_spSender = _spSender;
}

void Message::SetReceiver( MessageReceiverConstPtr _spReceiver )
{
	m_spReceiver = _spReceiver;
}

void Message::SendToReceiver()
{
	if ( m_spReceiver != NULL )
	{
		const_cast<MessageReceiver&>( *m_spReceiver ).ProcessMessage( *this );
		SetHandled( true );
	}
}

bool Message::IsBroadcast() const
{
	return m_spReceiver == NULL;
}

bool Message::IsSender( MessageReceiverConstPtr _spObject ) const
{
    return m_spSender == _spObject;
}

bool Message::IsReceiver( MessageReceiverConstPtr _spObject ) const
{
	return m_spReceiver == _spObject;
}

void Message::SetHandled( bool bHandled ) const
{
	m_bHandled = bHandled;
}

bool Message::IsHandled() const
{
	return m_bHandled;
}

bool Message::IsMessage( Unsigned32 _u32Type ) const
{
	return GetType() == _u32Type;
}
