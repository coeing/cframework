#ifndef CMADTV_PLAYER_CONTEXT_H
#define CMADTV_PLAYER_CONTEXT_H

#include "cfoundation/Base/Types.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Database );
    INCLUDE_SHARED_CLASS( Game );
    INCLUDE_SHARED_CLASS( Player );
    INCLUDE_SHARED_CLASS( PlayerInput );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// PlayerContext
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class PlayerContext
    {
    public:
        /// Constructor
        PlayerContext();

        /// Sets the database
        void                    SetDatabase( DatabasePtr _spDatabase );

        /// Returns the database
        DatabasePtr             GetDatabase() const;

        /// Sets the game
        void                    SetGame( GamePtr _spGame );

        /// Returns the game
        GamePtr                 GetGame() const;

        /// Sets the active player index
        void                    SetPlayerIdx( Unsigned32 _u32PlayerIdx );

        /// Returns the active player index
        Unsigned32              GetPlayerIdx() const;

        /// Returns the player
        PlayerPtr               GetPlayer() const;

        /// Sets the player input interface
        void                    SetInput( PlayerInputPtr _spPlayerInput );

        /// Returns the player input interface
        PlayerInputPtr          GetInput() const;

    private:
        /// Database
        DatabasePtr             m_spDatabase;

        /// Game
        GamePtr                 m_spGame;

        /// Active player index
        Unsigned32              m_u32PlayerIdx;

        /// Player input interface
        PlayerInputPtr          m_spPlayerInput;
    };
}

#endif
