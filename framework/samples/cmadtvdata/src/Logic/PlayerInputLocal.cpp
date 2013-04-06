#include "cmadtv/Logic/PlayerInputLocal.h"

#include "cmadtv/Logic/Commands/PlayerCommand.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// PlayerInputLocal
///
////////////////////////////////////////////////////////////////////////////////////////////

Unsigned32 PlayerInputLocal::Execute( PlayerCommandPtr _spCommand ) const
{
    if ( _spCommand == NULL )
    {
        return 0;
    }

    // Verify command
    Unsigned32 u32ReturnValue = _spCommand->Verify();
    if ( u32ReturnValue == 0 )
    {
        // Execute command
        _spCommand->Execute();
    }
    return u32ReturnValue;
}
