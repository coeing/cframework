#include "cmadtvserver/Application/Application.h"

#include "csystem/Core/Core.h"

#include "cfoundation/DateTime/Timer.h"

#include "cmadtv/Logic/Commands/Commands.h"

#include "cmadtvserver/Socket/TCP/ServerSocketTCP.h"

using namespace CMadTV;

ServerApplication::ServerApplication()
:   m_IOServiceWork( m_IOService ),
    m_ThreadIOService( ServerApplication::RunIOService )
{
}

ServerApplication::~ServerApplication()
{
}
		
void ServerApplication::Start()
{
    // Initialize
	Init();

	// Start loop
    Unsigned64 u64ElapsedMs = 0;
	bool bDone = false;
    bool bUseFrameBrake = true;

	CFoundation::Timer clFrameTimer;
	while( !bDone )
	{
		clFrameTimer.Start();

        Float32 f32TimeDifference = u64ElapsedMs / 1000.0f;

        // Update application
        Update( f32TimeDifference );

        // Get elapsed ms
        u64ElapsedMs = clFrameTimer.GetElapsedMs();

        if ( bUseFrameBrake )
        {
		    // Frame Brake
		    Integer64 i64SleepTimeMs = 30 - u64ElapsedMs;
		    if ( i64SleepTimeMs > 0 )
		    {
                CSystem::Core::GetInstance().Sleep( i64SleepTimeMs );
                u64ElapsedMs += i64SleepTimeMs;
		    }
        }
	}

    // Deinitialize
    Deinit();
}

void ServerApplication::Exit()
{
    //m_bDone = true;
}
		
bool ServerApplication::Init()
{
    // Include commands
    IncludeCommands();

    printf( "Load database...\n" );

    // Create world
    m_spWorld = WorldPtr( new World() );

    printf( "Database loaded...\n" );

    // Init socket
    m_spSocket = ServerSocketPtr( new ServerSocketTCP( m_IOService ) );
    m_spSocket->SetWorld( m_spWorld );

    printf( "Server started...\n" );

    return true;
}

void ServerApplication::Deinit()
{
    // Remove world
    m_spWorld.reset();

    // Stop io service
    m_IOService.stop();
}
  
void ServerApplication::Update( Float32 _f32TimeDifference )
{
    // Update world
    m_spWorld->Update( _f32TimeDifference );

    // Update socket
    m_spSocket->Update( _f32TimeDifference );
}
       
void ServerApplication::RunIOService()
{
    GetInstance().m_IOService.run();
}
  