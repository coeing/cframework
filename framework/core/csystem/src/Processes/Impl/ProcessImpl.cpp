#include "csystem/Processes/Impl/ProcessImpl.h"

#include "cfoundation/DateTime/Timer.h"
#include "cfoundation/Events/Impl/MessageQueueImpl.h"

#include "csystem/Core/Core.h"
#include "csystem/Processes/ProcessManager.h"

using namespace CSystem;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// ProcessImpl
///
////////////////////////////////////////////////////////////////////////////////////////////

ProcessImpl::ProcessImpl( ProcessManagerPtr _spProcessManager )
:   m_bStop( false ),
    m_spProcessManager( _spProcessManager ),
    m_spMessageQueue( new CFoundation::MessageQueueImpl ),
    m_u64MaxFrameTimeMs( 0 )
{
}
        
void ProcessImpl::Run()
{
    // Initialize
    Init();

	// Start loop
    Unsigned64 u64ElapsedMs = 0;
	CFoundation::Timer clFrameTimer;
	m_bStop = false;
	while( !m_bStop )
	{
		clFrameTimer.Start();

        Float32 f32TimeDifference = u64ElapsedMs / 1000.0f;

        // Dispatch messages
        CFoundation::BaseMessagePtr spMsg = m_spMessageQueue->TryReceive();
        while ( spMsg != NULL )
        {
            Dispatch( spMsg );
            spMsg = m_spMessageQueue->TryReceive();
        }

        // Update application
        Update( f32TimeDifference );

        // Get elapsed ms
        u64ElapsedMs = clFrameTimer.GetElapsedMs();

        if ( u64ElapsedMs < m_u64MaxFrameTimeMs )
        {
		    // Frame Brake
		    Integer64 i64SleepTimeMs = m_u64MaxFrameTimeMs - u64ElapsedMs;
		    if ( i64SleepTimeMs > 0 )
		    {
                Core::GetInstance().Sleep( i64SleepTimeMs );
                u64ElapsedMs += i64SleepTimeMs;
		    }
        }
	}

    // Deinitialize
    Deinit();
}

void ProcessImpl::Stop()
{
    m_bStop = true;
}
        
void ProcessImpl::Init()
{
}

void ProcessImpl::Deinit()
{
}
        
void ProcessImpl::Update( Float32 _f32TimeDifference )
{
}
        
void ProcessImpl::SendMsg( CFoundation::BaseMessagePtr _spMsg )
{
    m_spMessageQueue->Send( _spMsg );
}
        
void ProcessImpl::SetMaxFrameTime( Unsigned64 _u64MaxFrameTimeMs )
{
    m_u64MaxFrameTimeMs = _u64MaxFrameTimeMs;
}
       