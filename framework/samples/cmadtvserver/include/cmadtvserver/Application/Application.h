#ifndef CMADTV_SERVER_APPLICATION_H
#define CMADTV_SERVER_APPLICATION_H

#include <boost/thread.hpp> 
#include <boost/asio/io_service.hpp>

#include "cfoundation/Utils/Singleton.h"

#include "cmadtv/Data/World/World.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( ServerSocket );
    INCLUDE_SHARED_CLASS( World );

    class ServerApplication : public CFoundation::Singleton< ServerApplication >
    {
    public:
        /// Constructor
        ServerApplication();

        /// Destructor
        ~ServerApplication();

        /// Initializes and starts the application
		void                                    Start();

        /// Exits the game
        virtual void                            Exit();

    protected:
        /// Initialization
		virtual bool                            Init();

        /// Deinitialization
		virtual void                            Deinit();

        /// Per frame update
        virtual void                            Update( Float32 _f32TimeDifference );

    private:
        /// Runs the io service
        static void                             RunIOService();

        /// World
        WorldPtr                        m_spWorld;

        /// IO Service
        boost::asio::io_service         m_IOService;
        boost::asio::io_service::work   m_IOServiceWork;
        boost::thread                   m_ThreadIOService;

        /// Socket
        ServerSocketPtr                 m_spSocket;
    };
}

#endif
