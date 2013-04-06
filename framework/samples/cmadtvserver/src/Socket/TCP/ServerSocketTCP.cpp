#include "cmadtvserver/Socket/TCP/ServerSocketTCP.h"

#include <boost/bind.hpp>

#include "cfoundation/Events/MessageFactory.h"
#include "cfoundation/Network/TCP/SocketConnectionTCP.h"

#include "cmadtv/Data/Network/NetworkMessages.h"

using namespace CMadTV;

using boost::asio::ip::tcp;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// ServerSocketTCP
///
////////////////////////////////////////////////////////////////////////////////////////////

ServerSocketTCP::ServerSocketTCP( boost::asio::io_service& _IOService )
:   m_Acceptor( _IOService, tcp::endpoint( tcp::v4(), 13 ) )
{
    StartAccept();
}

ServerSocketTCP::~ServerSocketTCP()
{
}

void ServerSocketTCP::StartAccept()
{
    CFoundation::SocketConnectionTCPPtr spNewConnection = CFoundation::SocketConnectionTCP::Create( m_Acceptor.io_service(), m_spMsgFactory );

    m_Acceptor.async_accept( spNewConnection->GetSocket(),
        boost::bind( &ServerSocketTCP::HandleAccept, this, spNewConnection,
          boost::asio::placeholders::error ) );
}

void ServerSocketTCP::HandleAccept( CFoundation::SocketConnectionTCPPtr _spNewConnection, const boost::system::error_code& _error )
{
    if ( !_error )
    {
        // Start receiving
        _spNewConnection->Start();

        // Add connection
        AddConnection( _spNewConnection );

        StartAccept();
    }
}

