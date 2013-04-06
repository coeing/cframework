#include "cfoundation/DateTime/Timer.h"

#ifdef WIN32
#include <windows.h>
#endif

#include "cfoundation/DateTime/Time.h"

using namespace CFoundation;

Timer::Timer()
:	m_u64ElapsedMsBeforeLastStart( 0 ),
	m_u64LastStartTime( 0 ),
	m_u64LastEndTime( 0 ),
	m_eState( STATE_NOT_STARTED )
{
}

void Timer::Start()
{
	if( m_eState == STATE_NOT_STARTED ||
		m_eState == STATE_TIMING ||
        m_eState == STATE_ENDED )
	{
		// reset values
		m_u64ElapsedMsBeforeLastStart = 0;
		m_u64LastEndTime = 0;

#ifdef WIN32
		// get start time
		m_u64LastStartTime = GetTickCount();
#else
#endif
		
		m_eState = STATE_TIMING;
	}
}

void Timer::Pause() 
{
	if( m_eState == STATE_TIMING )
	{
		// get paused time
		m_u64LastEndTime = GetTickCount();

		m_eState = STATE_PAUSED;
	}
}

void Timer::Continue()
{
	if( m_eState == STATE_PAUSED )
	{
		// add elapsed time
		m_u64ElapsedMsBeforeLastStart += m_u64LastEndTime - m_u64LastStartTime;

		// set new start tim
		m_u64LastStartTime = GetTickCount();

		m_eState = STATE_TIMING;
	}
}

void Timer::Stop()
{
	if( m_eState == STATE_TIMING )
	{
		// get end time
		m_u64LastEndTime = GetTickCount();
	
		m_eState = STATE_ENDED;
	}
	else if( m_eState == STATE_PAUSED )
	{
		m_eState = STATE_ENDED;
	}
}

Unsigned64	Timer::GetElapsedMs() const
{
	// if timing not started
	if( m_eState == STATE_NOT_STARTED )
	{
		return 0;
	}
	// if timer is running
	else if( m_eState == STATE_TIMING )
	{
		return m_u64ElapsedMsBeforeLastStart + ( GetTickCount() - m_u64LastStartTime );
	}
	// timing is paused
	else if( m_eState == STATE_PAUSED )
	{
		return m_u64ElapsedMsBeforeLastStart + ( m_u64LastEndTime - m_u64LastStartTime );
	}
	// timing ended
	else if( m_eState == STATE_ENDED )
	{
		return m_u64ElapsedMsBeforeLastStart + ( m_u64LastEndTime - m_u64LastStartTime );
	}

	return 0;
}

const Time	Timer::GetElapsedTime() const
{	
	return Time( GetElapsedMs() / 1000 );
}

