#include "cmadtvclient/Socket/TCP/ClientSocketTCP.h"

#include <boost/bind.hpp>

#include "cfoundation/Network/TCP/SocketConnectionTCP.h"

#include "cmadtvclient/Application/Application.h"

using namespace CMadTV;
using boost::asio::ip::tcp;

ClientSocketTCP::ClientSocketTCP( boost::asio::io_service& _IOService )
{
    m_spConnection = CFoundation::SocketConnectionTCP::Create( _IOService, m_spMsgFactory );
}

ClientSocketTCP::~ClientSocketTCP()
{
}
        