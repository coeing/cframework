#include "cmadtvserver/Socket/UDP/ServerSocketUDP.h"

#include <boost/bind.hpp>

#include "cfoundation/Serialization/Serializer.h"

#include "cmadtv/Data/Network/NetworkMessages.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/World/World.h"

using namespace CMadTV;

using boost::asio::ip::udp;

ServerSocketUDP::ServerSocketUDP( boost::asio::io_service& _IOService )
:   m_Socket( _IOService, udp::endpoint( udp::v4(), 13 ) ),
    m_Serializer( CFoundation::Serializer::MODE_SAVE ),
    m_f32NextUpdate( 0.0f )
{
    StartReceive();
}

ServerSocketUDP::~ServerSocketUDP()
{
}
       
void ServerSocketUDP::Update( Float32 _f32TimeDifference )
{
    m_f32NextUpdate -= _f32TimeDifference;
    if ( m_f32NextUpdate < 0.0f )
    {
        BroadcastState();
        m_f32NextUpdate += 0.2f;
    }
}
       
void ServerSocketUDP::SetWorld( WorldPtr _spWorld )
{
    m_spWorld = _spWorld;
}

void ServerSocketUDP::StartReceive()
{
    m_Socket.async_receive_from(
        boost::asio::buffer( m_RecvBuffer, 1024 ), m_RemoteEndpoint,
        boost::bind( &ServerSocketUDP::HandleReceive, this,
                     boost::asio::placeholders::error,
                     boost::asio::placeholders::bytes_transferred ) );
}
      
void ServerSocketUDP::SendMessage( CFoundation::BaseMessagePtr _spMsg, const Endpoint& _Endpoint )
{
    // Clear serializer
    m_Serializer.Clear();

    // Serialize message
    _spMsg->Serialize( m_Serializer );

    // Send to all connected players
    m_Socket.async_send_to( boost::asio::buffer( m_Serializer.GetBuffer(), m_Serializer.GetOffset() ), _Endpoint,
          boost::bind( &ServerSocketUDP::HandleSendMessage, this, boost::asio::placeholders::error ) );
}
        
void ServerSocketUDP::SendMessage( CFoundation::BaseMessagePtr _spMsg, GamePtr _spGame )
{
    // Clear serializer
    m_Serializer.Clear();

    // Serialize message
    _spMsg->Serialize( m_Serializer );


    // To all players
    Unsigned32 u32NumPlayers = _spGame->GetNumPlayers();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPlayers; ++u32Idx )
    {
        PlayerPtr spPlayer = _spGame->GetPlayer( u32Idx );

        // Get endpoint
        const Endpoint& endpoint = m_mapPlayerEndpoints[ spPlayer ];

        // Send 
        m_Socket.async_send_to( boost::asio::buffer( m_Serializer.GetBuffer(), m_Serializer.GetOffset() ), endpoint,
              boost::bind( &ServerSocketUDP::HandleSendMessage, this, boost::asio::placeholders::error ) );
    }
}
        
void ServerSocketUDP::HandleSendMessage( const boost::system::error_code& error )
{
    int a = 0;
}

void ServerSocketUDP::HandleReceive( const boost::system::error_code& error, std::size_t bytes_transferred )
{
    if ( !error || error == boost::asio::error::message_size )
    {
        // Unmarshal
        CFoundation::Serializer serializer( CFoundation::Serializer::MODE_LOAD );
        serializer.LoadData( m_RecvBuffer, bytes_transferred );

        // Get message type
        CFoundation::String strType;
        serializer.Serialize( strType );

        if ( strType == "LoginMsg" )
        {
            // Create message
            LoginMsgPtr spMsg( new LoginMsg() );

            // Load message
            spMsg->Serialize( serializer );

            // Handle message
            HandleLogin( spMsg, m_RemoteEndpoint );
        }
        else if ( strType == "LogoutMsg" )
        {
            // Create message
            LogoutMsgPtr spMsg( new LogoutMsg() );

            // Load message
            spMsg->Serialize( serializer );

            // Handle message
            HandleLogout( spMsg, m_RemoteEndpoint );
        }
    }

    StartReceive();
}
       
void ServerSocketUDP::HandleLogin( LoginMsgPtr _spMsg, const Endpoint& _Endpoint )
{
    GameSettings::PlayerInfo info;

    // Get name
    info.m_strName = _spMsg->m_Name;

    // Get color
    //info.m_colColor = spEbPlayerName->GetBgColor( CGui::STATE_NORMAL );

    // Set human
    info.m_bHuman = true;

//    info.m_RemoteEndpoint = _Endpoint;

    // Add player
    m_GameSettings.AddPlayer( info );

    // If three players are there start game
    //if ( m_GameSettings.GetNumPlayers() == 3 )
    {
        StartGame();
    }
}
        
void ServerSocketUDP::HandleLogout( LogoutMsgPtr _spMsg, const Endpoint& _Endpoint )
{
    // Remove from game settings
    Unsigned32 u32NumPlayers = m_GameSettings.GetNumPlayers();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPlayers; ++u32Idx )
    {
//        if ( m_GameSettings.GetPlayerInfo( u32Idx ).m_RemoteEndpoint == _Endpoint )
        {
            //m_GameSettings.RemovePlayer( u32Idx );
            //break;
        }
    }
}
       
void ServerSocketUDP::StartGame()
{
    // Start game
    m_spWorld->StartGame( m_GameSettings );

    // Map players and endpoints
    GamePtr spGame = m_spWorld->GetGame();
    Unsigned32 u32NumPlayers = spGame->GetNumPlayers();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPlayers; ++u32Idx )
    {
        PlayerPtr spPlayer = spGame->GetPlayer( u32Idx );
//        m_mapPlayerEndpoints[ spPlayer ] = m_GameSettings.GetPlayerInfo( u32Idx ).m_RemoteEndpoint;
    }

    // Clear game settings
    while ( m_GameSettings.GetNumPlayers() > 0 )
    {
        m_GameSettings.RemovePlayer( 0 );
    }

    // Send first state update
    BroadcastState( spGame );
}
       
void ServerSocketUDP::BroadcastState()
{
    GamePtr spGame = m_spWorld->GetGame();
    BroadcastState( spGame );
}
        
void ServerSocketUDP::BroadcastState( GamePtr _spGame )
{
    if ( _spGame == NULL )
    {
        return;
    }

    // Create message
    StateMsgPtr spMsg( new StateMsg( _spGame ) );

    SendMessage( spMsg, _spGame );
}
