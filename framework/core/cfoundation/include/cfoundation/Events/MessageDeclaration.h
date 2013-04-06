////////////////////////////////////////////////////////////////////////////////////////////
///
/// X-Macro to simplify the definition of messages
///
////////////////////////////////////////////////////////////////////////////////////////////

// Member shortcuts
#define CLASS_MEMBER_DEFAULT( Type, Name )          CLASS_MEMBER( Type, Name, Type##(), CFoundation::SerializeWrapper_Simple )
#define CLASS_MEMBER_BOOL( Name, Default )          CLASS_MEMBER( bool, Name, Default, CFoundation::SerializeWrapper_Value )
#define CLASS_MEMBER_STRING( Name, Default )        CLASS_MEMBER( CFoundation::String, Name, Default, CFoundation::SerializeWrapper_Value )
#define CLASS_MEMBER_U32( Name, Default )           CLASS_MEMBER( Unsigned32, Name, Default, CFoundation::SerializeWrapper_Value )

// Class definition
#define CLASS( Class )  \
class Class : public CFoundation::BaseMessage
#define CLASS_MEMBER( Type, Name, Default, SerializeWrapper ) 

MESSAGE_DEFINITION
{
public:

#undef CLASS
#undef CLASS_MEMBER

    /// Constructor
#define CLASS( Class ) \
    Class(
#define CLASS_MEMBER( Type, Name, Default, SerializeWrapper ) \
    const Type& _##Name = Default, 
    
    MESSAGE_DEFINITION char _cUndefined = 0 )
        :

#undef CLASS
#undef CLASS_MEMBER

    /// Constructor #2
#define CLASS( Class )
#define CLASS_MEMBER( Type, Name, Default, SerializeWrapper ) \
    m_##Name( _##Name ),

    MESSAGE_DEFINITION m_cUndefined( _cUndefined )
    {
    }

#undef CLASS
#undef CLASS_MEMBER

    /// Destructor
#define CLASS( Class ) \
    ~##Class()
#define CLASS_MEMBER( Type, Name, Default, SerializeWrapper ) 

    MESSAGE_DEFINITION
    {
    }

#undef CLASS
#undef CLASS_MEMBER

    /// Returns the type
#define CLASS( Class ) \
    return #Class;
#define CLASS_MEMBER( Type, Name, Default, SerializeWrapper ) 

    virtual CFoundation::String GetType() const
    {
        MESSAGE_DEFINITION
    }

#undef CLASS
#undef CLASS_MEMBER

    /// Serialization
#define CLASS( Class )
#define CLASS_MEMBER( Type, Name, Default, SerializeWrapper ) \
    _Serializer.SerializeProxy< SerializeWrapper >( m_##Name );

    virtual void                Serialize( CFoundation::Serializer& _Serializer )
    {
        MESSAGE_DEFINITION
    }

#undef CLASS
#undef CLASS_MEMBER

    /// Members
#define CLASS( Class )
#define CLASS_MEMBER( Type, Name, Default, SerializeWrapper ) \
    Type        m_##Name;

    MESSAGE_DEFINITION
    char        m_cUndefined;

#undef CLASS
#undef CLASS_MEMBER

    TYPE_DATA
};

#define CLASS( Class )  \
INCLUDE_SHARED_CLASS( Class )
#define CLASS_MEMBER( Type, Name, Default, SerializeWrapper ) 

MESSAGE_DEFINITION

#undef CLASS
#undef CLASS_MEMBER

#ifdef MESSAGE_FACTORY

/*
// Create message factory for this message
#define CLASS( Class )  \
extern char const Class##MakerTypeName[] = #Class; /* Workaround to use string as a template parameter *//*  \
class Class##Maker : public CFoundation::PluggableMessageFactoryTemplate< MESSAGE_FACTORY, Class, Class##MakerTypeName >
#define CLASS_MEMBER( Type, Name, Default, SerializeWrapper ) 

MESSAGE_DEFINITION
{
};

#undef CLASS
#undef CLASS_MEMBER*/

#define CLASS_MEMBER( Type, Name, Default, SerializeWrapper ) 

#define CLASS( Class )  \
class Class##Maker : public MESSAGE_FACTORY::Maker

MESSAGE_DEFINITION
{
public:

#undef CLASS

    /// Make message instance
#define CLASS( Class )  \
    return SharedPtr< Class >( new Class() );

    virtual CFoundation::BaseMessagePtr  OnCreate() const 
    { 
        MESSAGE_DEFINITION
    } 

#undef CLASS
  

    /// Constructor
#define CLASS( Class )  \
    Class##Maker() : MESSAGE_FACTORY::Maker( #Class ) { } 

    MESSAGE_DEFINITION

#undef CLASS

    /// Static instance to make sure the constructor is called
#define CLASS( Class )  \
    static const Class##Maker     m_RegisterThis;

    MESSAGE_DEFINITION

#undef CLASS
};

/*
#define CLASS( Class )  \
    register##Class##Maker

} MESSAGE_DEFINITION;

#undef CLASS*/

#undef CLASS_MEMBER

/*
// Create message factory for this message
#define CLASS( Class )  \
class Class##Maker : public MESSAGE_FACTORY
#define CLASS_MEMBER( Type, Name, Default, SerializeWrapper ) 

MESSAGE_DEFINITION
{
private:

#undef CLASS
#undef CLASS_MEMBER

    /// Constructor
#define CLASS( Class )  \
    Class##Maker()      \
    :   MESSAGE_FACTORY( #Class )
#define CLASS_MEMBER( Type, Name, Default, SerializeWrapper ) 

    MESSAGE_DEFINITION
    {
    }

#undef CLASS
#undef CLASS_MEMBER

    /// Static 
#define CLASS( Class )  \
    Class##Maker()      \
    :   MESSAGE_FACTORY( #Class )
#define CLASS_MEMBER( Type, Name, Default, SerializeWrapper ) 

    MESSAGE_DEFINITION
    {
    }

#undef CLASS
#undef CLASS_MEMBER

};*/

#endif

#undef MESSAGE_DEFINITION
         