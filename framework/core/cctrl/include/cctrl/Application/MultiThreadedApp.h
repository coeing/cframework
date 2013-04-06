#ifndef CCTRL_MULTI_THREADED_APP_H
#define CCTRL_MULTI_THREADED_APP_H

#include "csystem/Processes/ProcessManager.h"

namespace CCtrl
{
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// MultiThreadedApp
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    /// Base class for a multi threaded application that uses processes

	class MultiThreadedApp
	{
	public:
        /// Initializes and starts the app
		void                                Start();

        /// Quits the app
		void                                Exit();

	protected:
        /// Constructor
		MultiThreadedApp();

        /// Destructor
		virtual ~MultiThreadedApp() = 0;

        /// Initialization
		virtual bool                        Init();

        /// Deinitialization
		virtual void                        Deinit();

        /// Process manager
        CSystem::ProcessManagerPtr          m_spProcessManager;

	private:
	};
}

#endif
