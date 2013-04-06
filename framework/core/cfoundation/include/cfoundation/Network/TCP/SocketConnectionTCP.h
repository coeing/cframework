#ifndef CFOUNDATION_SOCKET_CONNECTION_TCP_H
#define CFOUNDATION_SOCKET_CONNECTION_TCP_H

#pragma warning( disable : 4800 )
#include <boost/asio.hpp>

#include "cfoundation/Events/Message.h"
#include "cfoundation/Network/SocketConnection.h"
#include "cfoundation/Serialization/Serializer.h"

namespace CFoundation
{
    INCLUDE_SHARED_CLASS( SocketConnectionTCP );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// SocketConnectionTCP
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class SocketConnectionTCP : public SocketConnection
    {
    public:
        /// Factory method
        static SocketConnectionTCPPtr   Create( boost::asio::io_service& _IOService, MessageFactoryPtr _spMsgFactory, Unsigned32 _u32BufferSize = 2048 );

        /// Returns the socket
        boost::asio::ip::tcp::socket&   GetSocket();

        /// Connects to the passed socket
        virtual void                    Connect( const char* _szHost, const char* _szPort );

        /// Disconnects the socket
        virtual void                    Disconnect();

        /// Starts the connection
        void                            Start();

        /// Sends a message
        virtual void                    Send( CFoundation::BaseMessagePtr _spMsg );

    private:
        typedef std::list< CFoundation::BaseMessagePtr > MessageList;

        /// Constructor
        SocketConnectionTCP( boost::asio::io_service& _IOService, MessageFactoryPtr _spMsgFactory, Unsigned32 _u32BufferSize );

        /// Connect handler
        void            HandleConnect( const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator endpoint_iterator );

        /// Add message to queue
        void            QueueMsg( CFoundation::BaseMessagePtr _spMsg );

        /// Sends the first queued message
        void            DoSend();

        /// Send handler
        void            HandleSendHeader( const boost::system::error_code& error );
        void            HandleSend( const boost::system::error_code& error );

        /// Starts receiving
        void            StartReceiveHeader();

        /// Receive handler
        void            HandleReceiveHeader( const boost::system::error_code& error, std::size_t bytes_transferred );

        /// Starts receiving
        void            StartReceiveMsg();

        /// Receive handler
        void            HandleReceiveMsg( const boost::system::error_code& error, std::size_t bytes_transferred );

        /// IO Service
        boost::asio::io_service&        m_IOService;

        /// Socket
        boost::asio::ip::tcp::socket    m_Socket;

        /// Remote endpoint
        boost::asio::ip::tcp::endpoint  m_RemoteEndpoint;

        /// Messages to send
        MessageList                     m_aspMessages;

        /// Indicates if the connection is busy sending a message
        bool                            m_bSending;

        /// Holds the last serialized message
        CFoundation::MessageHeader      m_SendMsgHeader;

        /// Buffer to receive data
        CFoundation::MessageHeader      m_RecvMsgHeader;
        Unsigned32                      m_u32InitialBufferSize;
        Unsigned32                      m_u32BufferSize;
        char*                           m_pRecvBuffer;
    };
}

#endif
