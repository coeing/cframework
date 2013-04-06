#include "cmadtvserver/Socket/ServerSocket.h"

#include "cfoundation/Network/SocketConnection.h"

#include "cmadtv/Data/Network/NetworkMessages.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Player/PlayerContext.h"
#include "cmadtv/Data/Serialization/SerializationContext.h"
#include "cmadtv/Data/World/World.h"

#include "cmadtv/Logic/PlayerInputLocal.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// ServerSocket::ClientConnection
///
////////////////////////////////////////////////////////////////////////////////////////////

class ServerSocket::ClientConnection
{
public:
    /// Constructor
    ClientConnection();

    /// Sets the socket connection
    void                                SetConnection( CFoundation::SocketConnectionPtr _spConnection );

    /// Returns the socket connection
    CFoundation::SocketConnectionPtr    GetConnection() const;

    /// Sets the database to use
    void                                SetDatabase( DatabasePtr _spDatabase );

    /// Sets the game to use
    void                                SetGame( GamePtr _spGame );

    /// Sets the player index
    void                                SetPlayerIdx( Unsigned32 _u32Idx );

    /// Returns the player
    PlayerPtr                           GetPlayer() const;

    /// Sends the passed message to the client
    void                                Send( CFoundation::BaseMessagePtr _spMsg );

private:        
    /// Handles a player command
    void                                HandlePlayerCommand( PlayerCommandMsgPtr _spMsg );

    /// Connection
    CFoundation::SocketConnectionPtr    m_spConnection;

    /// Player context
    PlayerContextPtr                    m_spContext;

    /// Serialization context
    SerializationContextPtr             m_spSerializationContext;
};

////////////////////////////////////////////////////////////////////////////////////////////
///
/// ServerSocket
///
////////////////////////////////////////////////////////////////////////////////////////////

ServerSocket::ServerSocket()
:   m_f32NextUpdate( 0.0f ),    
    m_spMsgFactory( new NetworkMessageFactory() )
{
}

ServerSocket::~ServerSocket()
{
}
       
void ServerSocket::Update( Float32 _f32TimeDifference )
{
    m_f32NextUpdate -= _f32TimeDifference;
    if ( m_f32NextUpdate < 0.0f )
    {
        BroadcastState();
        m_f32NextUpdate += 0.2f;
    }
}
       
void ServerSocket::SetWorld( WorldPtr _spWorld )
{
    m_spWorld = _spWorld;
}
        
void ServerSocket::AddConnection( CFoundation::SocketConnectionPtr _spNewConnection )
{
    // Create client connection
    ClientConnectionPtr spConnection( new ClientConnection() );
    spConnection->SetConnection( _spNewConnection );

    // Register delegates
    m_aspDelegatesLogin.push_back( _spNewConnection->RegisterDelegate( CreateDelegate( this, &ServerSocket::HandleLogin ) ) );

    m_aspConnections.push_back( spConnection );

    printf( "New connection...\n" );
}

void ServerSocket::RemoveConnection( CFoundation::SocketConnectionPtr _spConnection )
{
    /*
    // Find connection
    ClientConnectionVec::iterator it = std::find( m_aspConnections.begin(), m_aspConnections.end(), _spConnection );
    if ( it == m_aspConnections.end() )
    {
        return;
    }

    // Get index
    Unsigned32 u32Idx = it - m_aspConnections.begin();

    // Remove from connections
    m_aspConnections.erase( it );

    // Remove delegates
    m_aspDelegatesLogin[ u32Idx ]->Disconnect();
    m_aspDelegatesLogin.erase( m_aspDelegatesLogin.begin() + u32Idx );*/
}
        
ServerSocket::ClientConnectionPtr ServerSocket::FindClientConnection( CFoundation::SocketConnectionPtr _spConnection ) const
{
    Unsigned32 u32NumConnections = m_aspConnections.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumConnections; ++u32Idx )
    {
        ClientConnectionPtr spConnection = m_aspConnections[ u32Idx ];
        if ( spConnection->GetConnection() == _spConnection )
        {
            return spConnection;
        }
    }
    return ClientConnectionPtr();
}
        
ServerSocket::ClientConnectionPtr ServerSocket::FindClientConnection( PlayerPtr _spPlayer ) const
{
    Unsigned32 u32NumConnections = m_aspConnections.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumConnections; ++u32Idx )
    {
        ClientConnectionPtr spConnection = m_aspConnections[ u32Idx ];
        if ( spConnection->GetPlayer() == _spPlayer )
        {
            return spConnection;
        }
    }
    return ClientConnectionPtr();
}
        
void ServerSocket::HandleLogin( CFoundation::MessageDispatcherPtr _spConnection, LoginMsgPtr _spMsg )
{
    GameSettings::PlayerInfo info;

    // Get name
    info.m_strName = _spMsg->m_Name;

    // Get color
    //info.m_colColor = spEbPlayerName->GetBgColor( CGui::STATE_NORMAL );

    // Set human
    info.m_bHuman = true;

    // Add player
    m_GameSettings.AddPlayer( info );

    // Get connection
    ClientConnectionPtr spConnection = FindClientConnection( static_pointer_cast< CFoundation::SocketConnection >( _spConnection ) );
    m_aspGameConnections.push_back( spConnection );

    printf( CFoundation::String::Build( "'%s' logged in...\n", info.m_strName.c_str() ).c_str() );

    // If three players are there start game
    //if ( m_GameSettings.GetNumPlayers() == 3 )
    {
        StartGame();
    }
}
       
void ServerSocket::HandleLogout( CFoundation::MessageDispatcherPtr _spConnection, LogoutMsgPtr _spMsg )
{
    /*
    // Get index of connection
    ClientConnectionVec::iterator it = std::find( m_aspConnections.begin(), m_aspConnections.end(), _spConnection );
    if ( it != m_aspConnections.end() )
    {
        // Remove from game settings
        m_GameSettings.RemovePlayer( it - m_aspConnections.begin() );

        // Remove connection
        m_aspConnections.erase( it );
    }*/
}
        
void ServerSocket::SendMessage( CFoundation::BaseMessagePtr _spMsg, GamePtr _spGame )
{
    // To all players
    Unsigned32 u32NumPlayers = _spGame->GetNumPlayers();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPlayers; ++u32Idx )
    {
        PlayerPtr spPlayer = _spGame->GetPlayer( u32Idx );

        // Get connection
        ClientConnectionPtr spConnection = FindClientConnection( spPlayer );

        // Send 
        SAFE_CALL( spConnection )->Send( _spMsg );
    }
}
                   
void ServerSocket::StartGame()
{
    // Start game
    m_spWorld->StartGame( m_GameSettings );

    // Update client connections
    DatabasePtr spDatabase = m_spWorld->GetDatabase();
    GamePtr spGame = m_spWorld->GetGame();

    Unsigned32 u32NumPlayers = spGame->GetNumPlayers();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPlayers; ++u32Idx )
    {
        PlayerPtr spPlayer = spGame->GetPlayer( u32Idx );

        // Get connection
        ClientConnectionPtr spConnection = m_aspGameConnections[ u32Idx ];

        // Setup connection
        spConnection->SetDatabase( spDatabase );
        spConnection->SetGame( spGame );
        spConnection->SetPlayerIdx( u32Idx );

        // Send database
        spConnection->Send( DatabaseUpdateMsgPtr( new DatabaseUpdateMsg( spDatabase ) ) );
    }

    // Clear game settings
    while ( m_GameSettings.GetNumPlayers() > 0 )
    {
        m_GameSettings.RemovePlayer( 0 );
    }
    m_aspGameConnections.clear();

    // Send first state update in next update
    m_f32NextUpdate = 0.0f;
}
       
void ServerSocket::BroadcastState()
{
    GamePtr spGame = m_spWorld->GetGame();
    BroadcastState( spGame );
}
        
void ServerSocket::BroadcastState( GamePtr _spGame )
{
    if ( _spGame == NULL )
    {
        return;
    }

    // Create message
    StateMsgPtr spMsg( new StateMsg( _spGame ) );

    SendMessage( spMsg, _spGame );
}


////////////////////////////////////////////////////////////////////////////////////////////
///
/// ServerSocket::ClientConnection
///
////////////////////////////////////////////////////////////////////////////////////////////

ServerSocket::ClientConnection::ClientConnection()
:   m_spContext( new PlayerContext() ),
    m_spSerializationContext( new SerializationContext() )
{
    m_spContext->SetInput( PlayerInputPtr( new PlayerInputLocal() ) );
}

void ServerSocket::ClientConnection::SetConnection( CFoundation::SocketConnectionPtr _spConnection )
{        
    if ( _spConnection == m_spConnection )
    {
        return;
    }

    m_spConnection = _spConnection;

    if ( m_spConnection != NULL )
    {
        m_spConnection->RegisterDelegate( CreateDelegate( this, &ClientConnection::HandlePlayerCommand ) );
        m_spConnection->SetSerializationContext( m_spSerializationContext );
    }
}
   
CFoundation::SocketConnectionPtr ServerSocket::ClientConnection::GetConnection() const
{
    return m_spConnection;
}
    
void ServerSocket::ClientConnection::SetDatabase( DatabasePtr _spDatabase )
{
    if ( _spDatabase == m_spContext->GetDatabase() )
    {
        return;
    }

    m_spContext->SetDatabase( _spDatabase );
    m_spSerializationContext->SetDatabase( _spDatabase );
}

void ServerSocket::ClientConnection::SetGame( GamePtr _spGame )
{
    if ( _spGame == m_spContext->GetGame() )
    {
        return;
    }

    m_spContext->SetGame( _spGame );
    m_spSerializationContext->SetGame( _spGame );
}
    
void ServerSocket::ClientConnection::SetPlayerIdx( Unsigned32 _u32Idx )
{
    m_spContext->SetPlayerIdx( _u32Idx );
}

PlayerPtr ServerSocket::ClientConnection::GetPlayer() const
{
    return m_spContext->GetPlayer();
}

void ServerSocket::ClientConnection::Send( CFoundation::BaseMessagePtr _spMsg )
{
    SAFE_CALL( m_spConnection )->Send( _spMsg );
}
    
void ServerSocket::ClientConnection::HandlePlayerCommand( PlayerCommandMsgPtr _spMsg )
{
    if ( _spMsg == NULL )
    {
        return;
    }
    
    PlayerCommandPtr spCommand = _spMsg->m_Command;
    if ( spCommand == NULL )
    {
        return;
    }

    // Execute player command
    spCommand->Init( m_spContext );
    m_spContext->GetInput()->Execute( spCommand );
}
