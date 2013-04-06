#include "cfoundation/Network/SocketConnection.h"

#include "cfoundation/Events/MessageFactory.h"
#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/Serialization/SerializationContext.h"

using namespace CFoundation;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// SocketConnection
///
////////////////////////////////////////////////////////////////////////////////////////////

SocketConnection::SocketConnection( MessageFactoryPtr _spMsgFactory )
:   m_spMsgFactory( _spMsgFactory ),
    m_SerializerSend( Serializer::MODE_SAVE ),
    m_SerializerReceive( Serializer::MODE_LOAD )
{
}
        
void SocketConnection::Connect( const char* /*_szHost*/, const char* /*_szPort*/ )
{
    m_bConnected = true;
}
        
bool SocketConnection::IsConnected() const
{
    return m_bConnected;
}

void SocketConnection::Disconnect()
{
    m_bConnected = false;
}
        
void SocketConnection::SetSerializationContext( SerializationContextPtr _spContext )
{
    m_SerializerSend.SetContext( _spContext );
    m_SerializerReceive.SetContext( _spContext );
}
           
void SocketConnection::HandleSend( BaseMessagePtr _spMsg, const void*& _pBuffer, Unsigned32& _u32Size )
{    
    if ( !m_bConnected )
    {
        return;
    }

    // Clear serializer
    m_SerializerSend.Clear();

    // Serialize message
    m_SerializerSend.SerializeProxy< CFoundation::SerializeWrapper_Context >( _spMsg );

    _pBuffer = m_SerializerSend.GetBuffer();
    _u32Size = m_SerializerSend.GetOffset();
}

void SocketConnection::HandleReceive( char* _pData, Unsigned32 _u32Size )
{
    if ( !m_bConnected )
    {
        return;
    }

    // Unmarshal
    m_SerializerReceive.LoadData( _pData, _u32Size );

    // Load message
    BaseMessagePtr spMsg;
    m_SerializerReceive.SerializeProxy< SerializeWrapper_Context >( spMsg );

    // Dispatch message to listeners
    Dispatch( spMsg );
}
