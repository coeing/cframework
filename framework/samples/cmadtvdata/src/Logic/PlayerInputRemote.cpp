#include "cmadtv/Logic/PlayerInputRemote.h"

#include "csystem/Processes/Process.h"

#include "cmadtv/Data/Network/NetworkMessages.h"

#include "cmadtv/Logic/Commands/PlayerCommand.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// PlayerInputRemote
///
////////////////////////////////////////////////////////////////////////////////////////////

Unsigned32 PlayerInputRemote::Execute( PlayerCommandPtr _spCommand ) const
{
    if ( _spCommand == NULL )
    {
        return 0;
    }

    // Verify command
    Unsigned32 u32ReturnValue = _spCommand->Verify();
    if ( u32ReturnValue == 0 )
    {
        // Emit message if verification was ok
        PlayerCommandMsgPtr spMsg( new PlayerCommandMsg( _spCommand ) );
        CSystem::Process::EmitMsg( spMsg );
    }
    
    return u32ReturnValue;
}
