#include "cfoundation/Network/TCP/SocketConnectionTCP.h"

#include <boost/bind.hpp>

#include "cfoundation/Events/Message.h"
#include "cfoundation/Serialization/SerializationContext.h"

using namespace CFoundation;

using boost::asio::ip::tcp;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// SocketConnectionTCP
///
////////////////////////////////////////////////////////////////////////////////////////////

SocketConnectionTCPPtr SocketConnectionTCP::Create( boost::asio::io_service& _IOService, MessageFactoryPtr _spMsgFactory, Unsigned32 _u32BufferSize )
{
    return SocketConnectionTCPPtr( new SocketConnectionTCP( _IOService, _spMsgFactory, _u32BufferSize ) );
}

SocketConnectionTCP::SocketConnectionTCP( boost::asio::io_service& _IOService, MessageFactoryPtr _spMsgFactory, Unsigned32 _u32BufferSize )
:   SocketConnection( _spMsgFactory ),
    m_IOService( _IOService ),
    m_Socket( _IOService ),
    m_bSending( false ),
    m_u32InitialBufferSize( _u32BufferSize ),
    m_u32BufferSize( _u32BufferSize )
{
    m_pRecvBuffer = new char[ m_u32BufferSize ];
}

tcp::socket& SocketConnectionTCP::GetSocket()
{
    return m_Socket;
}

void SocketConnectionTCP::Connect( const char* _szHost, const char* _szPort )
{
    SocketConnection::Connect( _szHost, _szPort );

    tcp::resolver resolver( m_IOService );
    tcp::resolver::query query( tcp::v4(), _szHost, _szPort );
    tcp::resolver::iterator iterator = resolver.resolve( query );
    m_RemoteEndpoint = *iterator;
    m_Socket.async_connect( m_RemoteEndpoint,
        boost::bind( &SocketConnectionTCP::HandleConnect, this,
          boost::asio::placeholders::error, ++iterator ) );
}
        
void SocketConnectionTCP::Disconnect()
{
    SocketConnection::Disconnect();
}

void SocketConnectionTCP::HandleConnect( const boost::system::error_code& error, tcp::resolver::iterator endpoint_iterator )
{
    if (!error)
    {
        Start();
    }
    else if ( endpoint_iterator != tcp::resolver::iterator() )
    {
      m_Socket.close();
      m_RemoteEndpoint = *endpoint_iterator;
      m_Socket.async_connect( m_RemoteEndpoint,
          boost::bind( &SocketConnectionTCP::HandleConnect, this,
            boost::asio::placeholders::error, ++endpoint_iterator ) );
    }
}

void SocketConnectionTCP::Start()
{
    StartReceiveHeader();
}

void SocketConnectionTCP::Send( CFoundation::BaseMessagePtr _spMsg )
{
    if ( IsConnected() )
    {
        m_IOService.post( boost::bind( &SocketConnectionTCP::QueueMsg, this, _spMsg ) );
    }
}

void SocketConnectionTCP::QueueMsg( CFoundation::BaseMessagePtr _spMsg )
{
    m_aspMessages.push_back( _spMsg );

    // Try to send message
    DoSend();
}

void SocketConnectionTCP::DoSend()
{
    if ( m_bSending ||
         m_aspMessages.empty() )
    {
        return;
    }

    m_bSending = true;
    
    // Get first message
    CFoundation::BaseMessagePtr spMsg = m_aspMessages.front();
    m_aspMessages.pop_front();

    // Send
    const void* pBuffer = NULL;
    SocketConnection::HandleSend( spMsg, pBuffer, m_SendMsgHeader.m_u32MsgSize );

    // Send header
    boost::asio::async_write( m_Socket,
        boost::asio::buffer( &m_SendMsgHeader, sizeof( m_SendMsgHeader ) ),
        boost::bind( &SocketConnectionTCP::HandleSendHeader, this, 
                        boost::asio::placeholders::error ) );        

    // Send message
    boost::asio::async_write( m_Socket,
        boost::asio::buffer( pBuffer, m_SendMsgHeader.m_u32MsgSize ),
        boost::bind( &SocketConnectionTCP::HandleSend, this, 
                        boost::asio::placeholders::error ) );
}
   
void SocketConnectionTCP::HandleSendHeader( const boost::system::error_code& /*error*/ )
{
}
   
void SocketConnectionTCP::HandleSend( const boost::system::error_code& /*error*/ )
{
    m_bSending = false;

    // Send next message
    DoSend();
}

void SocketConnectionTCP::StartReceiveHeader()
{
    boost::asio::async_read( m_Socket,
        boost::asio::buffer( &m_RecvMsgHeader, sizeof( m_RecvMsgHeader ) ),
        boost::bind( &SocketConnectionTCP::HandleReceiveHeader, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred ) );
}

void SocketConnectionTCP::HandleReceiveHeader( const boost::system::error_code& error, std::size_t /*bytes_transferred*/ )
{
    if ( !error || error == boost::asio::error::message_size )
    {
        StartReceiveMsg();
    }
    else
    {
        Disconnect();
    }
}

void SocketConnectionTCP::StartReceiveMsg()
{
    if ( m_RecvMsgHeader.m_u32MsgSize > m_u32BufferSize )
    {
        // Create buffer
        SAFE_DELETE_ARRAY( m_pRecvBuffer );
        m_pRecvBuffer = new char[ m_RecvMsgHeader.m_u32MsgSize ];
        m_u32BufferSize = m_RecvMsgHeader.m_u32MsgSize;
    }
    else if ( m_u32BufferSize > m_u32InitialBufferSize &&
              m_RecvMsgHeader.m_u32MsgSize <= m_u32InitialBufferSize )
    {
        // Create initial buffer
        SAFE_DELETE_ARRAY( m_pRecvBuffer );
        m_pRecvBuffer = new char[ m_u32InitialBufferSize ];
        m_u32BufferSize = m_u32InitialBufferSize;
    }

    boost::asio::async_read( m_Socket,
        boost::asio::buffer( m_pRecvBuffer, m_RecvMsgHeader.m_u32MsgSize ),
        boost::bind( &SocketConnectionTCP::HandleReceiveMsg, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred ) );
}

void SocketConnectionTCP::HandleReceiveMsg( const boost::system::error_code& error, std::size_t bytes_transferred )
{
    if ( !error || error == boost::asio::error::message_size )
    {
        SocketConnection::HandleReceive( m_pRecvBuffer, bytes_transferred );
        StartReceiveHeader();
    }
    else
    {
        Disconnect();
    }
}
