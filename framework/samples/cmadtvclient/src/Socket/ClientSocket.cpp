#include "cmadtvclient/Socket/ClientSocket.h"

#include "cfoundation/Network/SocketConnection.h"
#include "cfoundation/Serialization/Serializer.h"

#include "cmadtv/Data/Network/NetworkMessages.h"
#include "cmadtv/Data/Serialization/SerializationContext.h"

#include "cmadtvclient/Application/Application.h"

using namespace CMadTV;

ClientSocket::ClientSocket()  
:   m_spMsgFactory( new NetworkMessageFactory() ),
    m_spSerializationContext( new SerializationContext() )
{
}

ClientSocket::~ClientSocket()
{
}
       
void ClientSocket::Connect( const CFoundation::String& _strHost, Unsigned32 _u32Port )
{
    m_spConnection->Connect( _strHost.c_str(), CFoundation::String::Build( "%d", _u32Port ).c_str() );
    m_spConnection->SetSerializationContext( m_spSerializationContext );

    // Register delegates
    m_spConnection->RegisterDelegate( CFoundation::CreateDelegate( this, &ClientSocket::HandleDatabaseUpdate ) );
    m_spConnection->RegisterDelegate( CFoundation::CreateDelegate( this, &ClientSocket::HandleStateUpdate ) );
    m_spDelegateStateMsg = m_spConnection->RegisterDelegate( CFoundation::CreateDelegate( this, &ClientSocket::HandleMessage ) );
}
        
void ClientSocket::Disconnect()
{
    // Remove delegates
    m_spDelegateStateMsg->Disconnect();

    //m_spConnection->Disconnect();
}
        
void ClientSocket::Send( CFoundation::BaseMessagePtr _spMsg )
{
    m_spConnection->Send( _spMsg );
}
        
void ClientSocket::HandleDatabaseUpdate( DatabaseUpdateMsgPtr _spMsg )
{
    m_spSerializationContext->SetDatabase( _spMsg->m_Database );
}
        
void ClientSocket::HandleStateUpdate( StateMsgPtr _spMsg )
{
    int a = 0;
}
        
void ClientSocket::HandleMessage( CFoundation::BaseMessagePtr _spMsg )
{
    // Emit message
    CSystem::Process::EmitMsg( _spMsg );
}
