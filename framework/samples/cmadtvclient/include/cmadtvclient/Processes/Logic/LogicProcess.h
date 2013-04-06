#ifndef CMADTV_LOGIC_PROCESS_IMPL_H
#define CMADTV_LOGIC_PROCESS_IMPL_H

#include "cfoundation/DateTime/Timer.h"
#include "cfoundation/Events/Delegate.h"
#include "cfoundation/Utils/SharedPtr.h"

#include "csystem/Processes/Impl/ProcessImpl.h"

namespace CMadTV
{    
    INCLUDE_SHARED_CLASS( Database );
    INCLUDE_SHARED_CLASS( DatabaseUpdateMsg );
    INCLUDE_SHARED_CLASS( Game );
    INCLUDE_SHARED_CLASS( Player );
    INCLUDE_SHARED_CLASS( PlayerContext );
    INCLUDE_SHARED_CLASS( StateMsg );
    INCLUDE_SHARED_CLASS( WindowManager );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// LogicProcess
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class LogicProcess : public CSystem::ProcessImpl
	{
	public:
        /// Constructor
        LogicProcess( CSystem::ProcessManagerPtr _spProcessManager );

        /// Initialization
        virtual void            Init();

        /// Deinitialization
        virtual void            Deinit();

        /// Per frame update
        virtual void            Update( Float32 _f32TimeDifference );

        /// Sets the game
        void                    SetGame( GamePtr _spGame );

    private:
        /// Callbacks
        void                                    OnDatabaseUpdate( DatabaseUpdateMsgPtr _spMsg );
        void                                    OnStateUpdate( StateMsgPtr _spMsg );
        void                                    OnGameStarted( GamePtr _spGame );
        void                                    OnGameLost( GamePtr _spGame );
        void                                    OnGameWon( GamePtr _spGame );
        void                                    OnGameEnded( GamePtr _spGame, bool _bWon );

        /// FPS
        Unsigned32                      m_u32Frames;
        CFoundation::Timer              m_TimerFPS;
        Float32                         m_f32FPS;

        /// Context
        PlayerContextPtr                m_spContext;

        /// Window manager
        WindowManagerPtr                m_spWindowManager;
	};
}

#endif
