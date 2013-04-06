#ifndef CMADTV_PLAYER_INPUT_LOCAL_H
#define CMADTV_PLAYER_INPUT_LOCAL_H

#include "cmadtv/Logic/PlayerInput.h"

namespace CMadTV
{
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// PlayerInputLocal
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class PlayerInputLocal : public PlayerInput
    {
    public:
        /// Executes the passed command
        virtual Unsigned32      Execute( PlayerCommandPtr _spCommand ) const;
    };
}

#endif
