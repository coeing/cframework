#ifndef CMADTV_NETWORK_PROCESS_IMPL_H
#define CMADTV_NETWORK_PROCESS_IMPL_H

#include <boost/asio/io_service.hpp>

#include "csystem/Processes/Impl/ProcessImpl.h"

namespace CMadTV
{    
    INCLUDE_SHARED_CLASS( ClientSocket );
    INCLUDE_SHARED_CLASS( LoginMsg );
    INCLUDE_SHARED_CLASS( LogoutMsg );
    INCLUDE_SHARED_CLASS( PlayerCommandMsg );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// NetworkProcess
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class NetworkProcess : public CSystem::ProcessImpl
	{
	public:
        /// Constructor
        NetworkProcess( CSystem::ProcessManagerPtr _spProcessManager );

        /// Initialization
        virtual void            Init();

        /// Deinitialization
        virtual void            Deinit();

        /// Per frame update
        virtual void            Update( Float32 _f32TimeDifference );

    private:
        INCLUDE_SHARED_CLASS( IOServiceRunner );

        /// Callbacks
        void                    OnLogin( LoginMsgPtr _spMsg );
        void                    OnLogout( LogoutMsgPtr _spMsg );
        void                    OnPlayerCommand( PlayerCommandMsgPtr _spMsg );

        /// IO Service
        IOServiceRunnerPtr              m_spIOServiceRunner;
        boost::asio::io_service         m_IOService;

        /// Socket
        ClientSocketPtr                 m_spSocket;
	};
}

#endif
