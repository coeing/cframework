#ifndef CFOUNDATION_TIMER_H
#define CFOUNDATION_TIMER_H

#include "cfoundation/Base/Types.h"

namespace CFoundation
{
	class Time;
	class Timer
	{
	public:
		Timer();

		void Start();
		void Pause();
		void Continue();
		void Stop();

		Unsigned64 GetElapsedMs() const;
		const Time GetElapsedTime() const;

	private:
		Unsigned64		m_u64ElapsedMsBeforeLastStart;

		Unsigned64		m_u64LastStartTime;
		Unsigned64		m_u64LastEndTime;

		enum State
		{
			STATE_NOT_STARTED,
			STATE_TIMING,
			STATE_PAUSED,
			STATE_ENDED
		};

		State			m_eState;
	};
}

#endif
