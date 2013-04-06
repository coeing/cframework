#ifndef CMADTV_PLAYER_INPUT_H
#define CMADTV_PLAYER_INPUT_H

#include "cfoundation/Base/Types.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( PlayerCommand );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// PlayerInput
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class PlayerInput
    {
    public:
        /// Verifies and executes the passed command
        virtual Unsigned32      Execute( PlayerCommandPtr _spCommand ) const = 0;
    };
}

#endif
