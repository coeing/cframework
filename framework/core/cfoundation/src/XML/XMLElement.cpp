#include "cfoundation/XML/XMLElement.h"

using namespace CFoundation;

#define XML_ELEMENT_GETTER_FUNCTION( Type ) Type XMLElement::GetAttributeAs##Type( const String& _strAttribute ) const   \
                                            {                                                                   \
                                                return GetAttribute( _strAttribute ).To##Type();                \
                                            }                                                                   \
                                            void XMLElement::GetAttributeAs##Type( const String& _strAttribute, Type& _Value ) const    \
                                            {                                                                                           \
                                                if ( HasAttribute( _strAttribute ) )                                                    \
                                                {                                                                                       \
                                                    _Value = GetAttribute( _strAttribute ).To##Type();                                  \
                                                }                                                                                       \
                                            }                                                                                           

        
void XMLElement::GetAttribute( const String& _strAttribute, String& _strValue ) const
{
    if ( HasAttribute( _strAttribute ) )
    {
        _strValue = GetAttribute( _strAttribute );
    }
}

bool XMLElement::GetAttributeAsBool( const String& _strAttribute ) const
{
    return GetAttribute( _strAttribute ) == "true";
}

XML_ELEMENT_GETTER_FUNCTION( Unsigned8 );
XML_ELEMENT_GETTER_FUNCTION( Unsigned16 );
XML_ELEMENT_GETTER_FUNCTION( Unsigned32 );
XML_ELEMENT_GETTER_FUNCTION( Float16 );
XML_ELEMENT_GETTER_FUNCTION( Float32 );
