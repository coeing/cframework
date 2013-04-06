#ifndef CMADTV_PLAYER_INPUT_REMOTE_H
#define CMADTV_PLAYER_INPUT_REMOTE_H

#include "cmadtv/Logic/PlayerInput.h"

namespace CMadTV
{
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// PlayerInputRemote
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class PlayerInputRemote : public PlayerInput
    {
    public:
        /// Executes the passed command
        virtual Unsigned32      Execute( PlayerCommandPtr _spCommand ) const;
    };
}

#endif
