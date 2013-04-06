#include "cmadtv/Data/Network/NetworkMessages.h"

using namespace CMadTV;

/// We need this definition here, even if the constructor is empty
/// The compiler won't parse this file and therefore not create the 
/// static members of the messages, that will call the constructor and
/// register the message at the factory, if there is no code that is
/// used by the application. By having the definition of the factory in
/// this file we can be sure that all stuff in this file is linked in the
/// application.

#define MESSAGE_FACTORY     NetworkMessageFactory
#include "cfoundation/Events/MessageFactoryDefinition.h"

#define MESSAGE_DEFINITION  CLASS( LoginMsg )
#include "cfoundation/Events/MessageDefinition.h"

#define MESSAGE_DEFINITION  CLASS( LoginResponseMsg )
#include "cfoundation/Events/MessageDefinition.h"

#define MESSAGE_DEFINITION  CLASS( LogoutMsg )
#include "cfoundation/Events/MessageDefinition.h"

#define MESSAGE_DEFINITION  CLASS( StateMsg )
#include "cfoundation/Events/MessageDefinition.h"

#define MESSAGE_DEFINITION  CLASS( DatabaseUpdateMsg )
#include "cfoundation/Events/MessageDefinition.h"

#define MESSAGE_DEFINITION  CLASS( PlayerCommandMsg )
#include "cfoundation/Events/MessageDefinition.h"

/*
////////////////////////////////////////////////////////////////////////////////////////////
///
/// NetworkMessageFactory
///
////////////////////////////////////////////////////////////////////////////////////////////

#define MESSAGE_FACTORY_CASE( Message ) if ( CFoundation::String( #Message ) == _strType ) \
                                            return CFoundation::BaseMessagePtr( new Message() );

CFoundation::BaseMessagePtr NetworkMessageFactory::Create( const CFoundation::String& _strType )
{
    MESSAGE_FACTORY_CASE( LoginMsg );
    MESSAGE_FACTORY_CASE( LoginResponseMsg );
    MESSAGE_FACTORY_CASE( LogoutMsg );
    MESSAGE_FACTORY_CASE( StateMsg );

    return CFoundation::BaseMessagePtr();
}*/