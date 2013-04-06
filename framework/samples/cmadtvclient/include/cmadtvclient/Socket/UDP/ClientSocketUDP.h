#ifndef CMADTV_CLIENT_SOCKET_UDP_H
#define CMADTV_CLIENT_SOCKET_UDP_H

#include <boost/asio.hpp>

#include "cfoundation/Base/Types.h"
#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CFoundation
{
    INCLUDE_SHARED_CLASS( BaseMessage )
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( StateMsg );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// ClientSocketUDP
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class ClientSocketUDP
    {
    public:
        /// Constructor
        ClientSocketUDP( boost::asio::io_service& _IOService );

        /// Destructor
        ~ClientSocketUDP();

        /// Connects to server
        void            Connect();

    private:
        /// Connect handler
        void            HandleConnect( const boost::system::error_code& error, boost::asio::ip::udp::resolver::iterator endpoint_iterator );

        /// Starts receiving
        void            StartReceive();

        /// Send message to server
        void            SendMessage( CFoundation::BaseMessagePtr _spMsg );

        /// Send message handler
        void            HandleSendMessage( const boost::system::error_code& error );

        /// Receive handler
        void            HandleReceive( const boost::system::error_code& error, std::size_t bytes_transferred );

        /// Handles state updates
        void            HandleStateUpdate( StateMsgPtr _spMsg );
    
        boost::asio::io_service&        m_IOService;
        boost::asio::ip::udp::socket    m_Socket;
        boost::asio::ip::udp::endpoint  m_ServerEndpoint;
        boost::asio::ip::udp::endpoint  m_RemoteEndpoint;
        std::string                     m_strSendMessage;
        boost::array< char, 1024 >      m_RecvBuffer;
        
        /// Holds the last serialized message
        CFoundation::Serializer         m_Serializer;
    };
}

#endif
