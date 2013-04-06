#ifndef CMADTV_CLIENT_APPLICATION_H
#define CMADTV_CLIENT_APPLICATION_H

#include "cctrl/Application/MultiThreadedApp.h"

namespace CMadTV
{
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// ClientApplication
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class ClientApplication : public CCtrl::MultiThreadedApp
    {
    public:
        /// Constructor
        ClientApplication();

        /// Destructor
        ~ClientApplication();

    protected:
        /// Initialization
		virtual bool                            Init();

        /// Deinitialization
		virtual void                            Deinit();
    };
}

#endif
