#ifndef CMADTV_NETWORK_MESSAGES
#define CMADTV_NETWORK_MESSAGES

#include "cfoundation/Events/MessageFactory.h"
#include "cfoundation/RTTI/SerializeWrapper_SharedPtrDyn.h"
#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/Utils/Color.h"

#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Logic/Commands/PlayerCommand.h"

namespace CMadTV
{
    #define MESSAGE_FACTORY     NetworkMessageFactory
    #include "cfoundation/Events/MessageFactoryDeclaration.h"

    #define MESSAGE_DEFINITION  CLASS( LoginMsg ) CLASS_MEMBER_STRING( Host, "localhost" ) CLASS_MEMBER_U32( Port, 13 ) CLASS_MEMBER_STRING( Name, "Empty" ) CLASS_MEMBER_DEFAULT( CFoundation::Color, Color )
    #include "cfoundation/Events/MessageDeclaration.h"

    #define MESSAGE_DEFINITION  CLASS( LoginResponseMsg ) CLASS_MEMBER_BOOL( Success, false ) CLASS_MEMBER_U32( Player, MAX_U32 )
    #include "cfoundation/Events/MessageDeclaration.h"

    #define MESSAGE_DEFINITION  CLASS( LogoutMsg ) CLASS_MEMBER_U32( Player, MAX_U32 )
    #include "cfoundation/Events/MessageDeclaration.h"

    #define MESSAGE_DEFINITION  CLASS( StateMsg ) CLASS_MEMBER( GamePtr, Game, GamePtr(), CFoundation::SerializeWrapper_SharedPtrSimple )
    #include "cfoundation/Events/MessageDeclaration.h"

    #define MESSAGE_DEFINITION  CLASS( DatabaseUpdateMsg ) CLASS_MEMBER( DatabasePtr, Database, DatabasePtr(), CFoundation::SerializeWrapper_SharedPtrSimple )
    #include "cfoundation/Events/MessageDeclaration.h"

    #define MESSAGE_DEFINITION  CLASS( PlayerCommandMsg ) CLASS_MEMBER( PlayerCommandPtr, Command, PlayerCommandPtr(), CFoundation::SerializeWrapper_SharedPtrDyn )
    #include "cfoundation/Events/MessageDeclaration.h"

    #undef MESSAGE_FACTORY
    
    /*
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// NetworkMessageFactory
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class NetworkMessageFactory : public CFoundation::MessageFactory
    {
    public:
        /// Creates a message of the passed type
        virtual CFoundation::BaseMessagePtr Create( const CFoundation::String& _strType );
    };*/
}

#endif
