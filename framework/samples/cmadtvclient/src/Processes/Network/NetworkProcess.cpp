#include "cmadtvclient/Processes/Network/NetworkProcess.h"

#include "cfoundation/DateTime/Timer.h"
#include "cfoundation/Events/Impl/MessageQueueImpl.h"

#include "cmadtv/Data/Network/NetworkMessages.h"

#include "cmadtvclient/Socket/TCP/ClientSocketTCP.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// NetworkProcess::IOServiceRunner
///
////////////////////////////////////////////////////////////////////////////////////////////

class NetworkProcess::IOServiceRunner
{
public:
    /// Constructor
    IOServiceRunner( boost::asio::io_service& _IOService )
    :   m_IOService( _IOService ),
        m_IOServiceWork( m_IOService )
    {
    }

    /// Run
    void                operator()()
    {
        m_IOService.run();
    }

    /// Stop
    void                Stop()
    {
        m_IOService.stop();
    }

private:
    /// IO Service
    boost::asio::io_service&        m_IOService;
    boost::asio::io_service::work   m_IOServiceWork;
};

////////////////////////////////////////////////////////////////////////////////////////////
///
/// NetworkProcess
///
////////////////////////////////////////////////////////////////////////////////////////////

NetworkProcess::NetworkProcess( CSystem::ProcessManagerPtr _spProcessManager )
:   ProcessImpl( _spProcessManager )
{
    // Create service runner
    m_spIOServiceRunner = IOServiceRunnerPtr( new IOServiceRunner( m_IOService ) );

    // Create socket
    m_spSocket = ClientSocketPtr( new ClientSocketTCP( m_IOService ) );
}
        
void NetworkProcess::Init()
{
    // Set frame brake
    SetMaxFrameTime( 30 );

    // Run IO service
    boost::thread threadIOService( *m_spIOServiceRunner );

    // Register message delegates
    RegisterDelegate( CreateDelegate( this, &NetworkProcess::OnLogin ) );
    RegisterDelegate( CreateDelegate( this, &NetworkProcess::OnLogout ) );
    RegisterDelegate( CreateDelegate( this, &NetworkProcess::OnPlayerCommand ) );
}

void NetworkProcess::Deinit()
{
    // Logout
    //m_spSocket->Logout();

    // Stop io service
    m_spIOServiceRunner->Stop();
}
        
void NetworkProcess::Update( Float32 _f32TimeDifference )
{
}
        
void NetworkProcess::OnLogin( LoginMsgPtr _spMsg )
{
    // Connect to server
    m_spSocket->Connect( _spMsg->m_Host, _spMsg->m_Port );

    // Send message over connection
    m_spSocket->Send( _spMsg );
}
        
void NetworkProcess::OnLogout( LogoutMsgPtr _spMsg )
{
    // Send message over connection
    m_spSocket->Send( _spMsg );
}
        
void NetworkProcess::OnPlayerCommand( PlayerCommandMsgPtr _spMsg )
{
    // Send message over connection
    m_spSocket->Send( _spMsg );
}
  