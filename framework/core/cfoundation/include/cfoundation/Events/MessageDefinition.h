////////////////////////////////////////////////////////////////////////////////////////////
///
/// X-Macro to simplify the definition of messages
///
////////////////////////////////////////////////////////////////////////////////////////////

#define CLASS( Class )  \
    const Class##Maker    Class##Maker::m_RegisterThis;

MESSAGE_DEFINITION

#undef CLASS

#define CLASS( Class )  \
    RTTI_DEF1_INST_EASY( Class, CFoundation::BaseMessage )

MESSAGE_DEFINITION

#undef CLASS

#undef MESSAGE_DEFINITION
         