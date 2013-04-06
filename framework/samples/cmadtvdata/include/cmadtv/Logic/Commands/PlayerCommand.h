#ifndef CMADTV_PLAYER_COMMAND_H
#define CMADTV_PLAYER_COMMAND_H

#include "cfoundation/Base/Types.h"
#include "cfoundation/RTTI/rtti.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CFoundation
{
    class Serializer;
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( PlayerContext );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// PlayerCommand
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class PlayerCommand
    {
    public:
        /// Constructor
        PlayerCommand( PlayerContextPtr _spContext = PlayerContextPtr() );

        /// Initialization
        void                    Init( PlayerContextPtr _spContext );

        /// Serialization
        /// Should serialize the non-context dependent data
        virtual void            Serialize( CFoundation::Serializer& _Serializer );

        /// Verification
        /// Verifies if the command can be executed without changing any data
        /// Returns 0 if everything is ok, otherwise a error code is returned
        virtual Unsigned32      Verify() const = 0;

        /// Executes the command
        virtual void            Execute() const = 0;

    protected:
        /// Called by base class when context changed
        virtual void            OnContextChanged();

        /// Context
        PlayerContextPtr        m_spContext;

        TYPE_DATA
    };

    INCLUDE_SHARED_CLASS( PlayerCommand );
}

#endif
