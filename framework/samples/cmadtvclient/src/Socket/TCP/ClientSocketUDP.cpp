#include "cmadtvclient/Socket/UDP/ClientSocketUDP.h"

#include <boost/bind.hpp>

#include "cfoundation/Serialization/Serializer.h"

#include "cmadtvclient/Application/Application.h"

using namespace CMadTV;
using boost::asio::ip::udp;

ClientSocketUDP::ClientSocketUDP( boost::asio::io_service& _IOService )
:   m_IOService( _IOService ),
    m_Socket( _IOService ),
    m_Serializer( CFoundation::Serializer::MODE_SAVE )
{
}

ClientSocketUDP::~ClientSocketUDP()
{
}
       
void ClientSocketUDP::Connect()
{
    udp::resolver resolver( m_IOService );
    udp::resolver::query query( udp::v4(), "localhost", "13" );
    udp::resolver::iterator iterator = resolver.resolve( query );
    m_ServerEndpoint = *iterator;
    m_Socket.async_connect( m_ServerEndpoint,
        boost::bind( &ClientSocketUDP::HandleConnect, this,
          boost::asio::placeholders::error, ++iterator ) );
}

void ClientSocketUDP::HandleConnect( const boost::system::error_code& error, udp::resolver::iterator endpoint_iterator )
{
    if (!error)
    {
        StartReceive();
    }
    else if ( endpoint_iterator != udp::resolver::iterator() )
    {
      m_Socket.close();
      m_ServerEndpoint = *endpoint_iterator;
      m_Socket.async_connect( m_ServerEndpoint,
          boost::bind( &ClientSocketUDP::HandleConnect, this,
            boost::asio::placeholders::error, ++endpoint_iterator ) );
    }
}

void ClientSocketUDP::StartReceive()
{
    m_Socket.async_receive_from(
        boost::asio::buffer( m_RecvBuffer, m_RecvBuffer.size() ), m_RemoteEndpoint,
        boost::bind( &ClientSocketUDP::HandleReceive, this,
                     boost::asio::placeholders::error,
                     boost::asio::placeholders::bytes_transferred ) );
}
      
void ClientSocketUDP::SendMessage( CFoundation::BaseMessagePtr _spMsg )
{
    // Clear serializer
    m_Serializer.Clear();

    // Serialize message
    _spMsg->Serialize( m_Serializer );

    // Send to server
    m_Socket.async_send_to( boost::asio::buffer( m_Serializer.GetBuffer(), m_Serializer.GetOffset() ), m_ServerEndpoint,
          boost::bind( &ClientSocketUDP::HandleSendMessage, this, boost::asio::placeholders::error ) );
}
        
void ClientSocketUDP::HandleSendMessage( const boost::system::error_code& error )
{
    int a = 0;
}
        
void ClientSocketUDP::HandleReceive( const boost::system::error_code& error, std::size_t bytes_transferred )
{
    if ( !error || error == boost::asio::error::message_size )
    {
        if ( bytes_transferred > 0 )
        {
        }
    }

    StartReceive();
}
       
void ClientSocketUDP::HandleStateUpdate( StateMsgPtr _spMsg )
{
//    ClientApplication::GetInstance().SetGame( _spMsg->m_Game );
}
