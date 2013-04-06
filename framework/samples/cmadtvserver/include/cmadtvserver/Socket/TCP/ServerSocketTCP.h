#ifndef CMADTV_SERVER_SOCKET_TCP_H
#define CMADTV_SERVER_SOCKET_TCP_H

#include "cmadtvserver/Socket/ServerSocket.h"

namespace CFoundation
{
    INCLUDE_SHARED_CLASS( SocketConnectionTCP );
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( ServerSocket );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// ServerSocketTCP
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class ServerSocketTCP : public ServerSocket
    {
    public:
        /// Constructor
        ServerSocketTCP( boost::asio::io_service& _IOService );

        /// Destructor
        ~ServerSocketTCP();

    private:
        /// Starts accepting incoming connections
        void            StartAccept();

        /// Accept handler
        void            HandleAccept( CFoundation::SocketConnectionTCPPtr _spNewConnection, const boost::system::error_code& error );

        /// Acceptor
        boost::asio::ip::tcp::acceptor  m_Acceptor;
    };
}

#endif
