#ifndef CMADTV_SERVER_SOCKET_UDP_H
#define CMADTV_SERVER_SOCKET_UDP_H

#include <boost/asio.hpp>

#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/Utils/SharedPtr.h"

#include "cmadtv/Data/Game/GameSettings.h"

namespace CFoundation
{
    INCLUDE_SHARED_CLASS( BaseMessage )
}


namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Game );
    INCLUDE_SHARED_CLASS( LoginMsg );
    INCLUDE_SHARED_CLASS( LogoutMsg );
    INCLUDE_SHARED_CLASS( Player );
    INCLUDE_SHARED_CLASS( World );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// ServerSocketUDP
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class ServerSocketUDP
    {
    public:
        /// Constructor
        ServerSocketUDP( boost::asio::io_service& _IOService );

        /// Destructor
        ~ServerSocketUDP();

        /// Per frame update
        void            Update( Float32 _f32TimeDifference );

        /// Sets the world
        void            SetWorld( WorldPtr _spWorld );

    private:
        typedef boost::asio::ip::udp::endpoint Endpoint;
        typedef std::map< PlayerPtr, Endpoint > PlayerEndpointMap;

        /// Starts receiving
        void            StartReceive();

        /// Receive handler
        void            HandleReceive( const boost::system::error_code& error, std::size_t bytes_transferred );

        /// Send message to the passed endpoint
        void            SendMessage( CFoundation::BaseMessagePtr _spMsg, const Endpoint& _Endpoint );

        /// Send message to all players of the passed game
        void            SendMessage( CFoundation::BaseMessagePtr _spMsg, GamePtr _spGame );

        /// Send message handler
        void            HandleSendMessage( const boost::system::error_code& error );

        /// Message handlers
        void            HandleLogin( LoginMsgPtr _spMsg, const Endpoint& _Endpoint );
        void            HandleLogout( LogoutMsgPtr _spMsg, const Endpoint& _Endpoint );

        /// Starts the game
        void            StartGame();

        /// Sends the current state to all players
        void            BroadcastState();

        /// Sends the current state to all players of the passed game
        void            BroadcastState( GamePtr _spGame );

        boost::asio::ip::udp::socket    m_Socket;
        boost::asio::ip::udp::endpoint  m_RemoteEndpoint;

        char                            m_RecvBuffer[ 1024 ];
        
        /// Holds the last serialized message
        CFoundation::Serializer         m_Serializer;

        /// Player endpoint map
        PlayerEndpointMap               m_mapPlayerEndpoints;

        /// World
        WorldPtr                        m_spWorld;

        /// Game settings for not started games
        GameSettings                    m_GameSettings;

        /// Next state broadcast
        Float32                         m_f32NextUpdate;
    };
}

#endif
