#ifndef CMADTV_SERVER_SOCKET_H
#define CMADTV_SERVER_SOCKET_H

#include <boost/asio.hpp>

#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/Utils/SharedPtr.h"

#include "cmadtv/Data/Game/GameSettings.h"

namespace CFoundation
{    
    INCLUDE_SHARED_CLASS( BaseMessage );    
    INCLUDE_SHARED_CLASS( DelegateConnection );
    INCLUDE_SHARED_CLASS( MessageDispatcher );
    INCLUDE_SHARED_CLASS( MessageFactory );
    INCLUDE_SHARED_CLASS( SocketConnection );
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Game );
    INCLUDE_SHARED_CLASS( LoginMsg );
    INCLUDE_SHARED_CLASS( LogoutMsg );
    INCLUDE_SHARED_CLASS( Player );
    INCLUDE_SHARED_CLASS( PlayerCommandMsg );
    INCLUDE_SHARED_CLASS( World );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// ServerSocket
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class ServerSocket : public CFoundation::SharedPtrBase< ServerSocket >
    {
    public:
        /// Constructor
        ServerSocket();

        /// Destructor
        ~ServerSocket();

        /// Per frame update
        virtual void    Update( Float32 _f32TimeDifference );

        /// Sets the world
        virtual void    SetWorld( WorldPtr _spWorld );

    protected:
        /// Message factory
        CFoundation::MessageFactoryPtr  m_spMsgFactory;

        /// Adds a new connection
        void            AddConnection( CFoundation::SocketConnectionPtr _spNewConnection );

        /// Removes a connection
        void            RemoveConnection( CFoundation::SocketConnectionPtr _spConnection );

    private:
        /// Client connections
        INCLUDE_SHARED_CLASS( ClientConnection );

        typedef std::vector< ClientConnectionPtr > ClientConnectionVec;
        typedef std::vector< CFoundation::DelegateConnectionPtr > ConnectionDelegateVec;
        typedef std::map< PlayerPtr, CFoundation::SocketConnectionPtr > PlayerConnectionMap;

        /// Returns the client connection for the passed connection
        ClientConnectionPtr FindClientConnection( CFoundation::SocketConnectionPtr _spConnection ) const;

        /// Returns the client connection for the passed player
        ClientConnectionPtr FindClientConnection( PlayerPtr _spPlayer ) const;

        /// Send message to all players of the passed game
        void            SendMessage( CFoundation::BaseMessagePtr _spMsg, GamePtr _spGame );

        /// Message handlers
        void            HandleLogin( CFoundation::MessageDispatcherPtr _spConnection, LoginMsgPtr _spMsg );
        void            HandleLogout( CFoundation::MessageDispatcherPtr _spConnection, LogoutMsgPtr _spMsg );

        /// Starts the game
        void            StartGame();

        /// Sends the current state to all players
        void            BroadcastState();

        /// Sends the current state to all players of the passed game
        void            BroadcastState( GamePtr _spGame );

        /// Game settings for not started games
        GameSettings                                    m_GameSettings;
        ClientConnectionVec                             m_aspGameConnections;

        /// Connections for players in game settings
        ClientConnectionVec                             m_aspConnections;
        ConnectionDelegateVec                           m_aspDelegatesLogin;
        
        /// World
        WorldPtr                                        m_spWorld;

        /// Next state broadcast
        Float32                                         m_f32NextUpdate;
    };
}

#endif
