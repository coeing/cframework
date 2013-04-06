#include "cfoundation/tinyxml/XMLElement.h"

#ifdef XML_IMPL_TINYXML

#include "tinyxml.h"

using namespace CFoundation;
using namespace TinyXml;

XMLElement::XMLElement()
{
}

XMLElement::XMLElement( const XMLNode& rhs )
: XMLNode( rhs )
{
}

XMLElement::XMLElement( const String& sName )
{
    SetName( sName );
}

XMLElement& XMLElement::operator=( const XMLNode& rhs )
{
    XMLNode::operator=( rhs );
    return *this;
}

const String XMLElement::GetName() const
{
    return String( GetXMLElement()->Value() );
}

void XMLElement::SetName( const String &sName )
{
    GetXMLElement()->SetValue( sName.c_str() );
}

const String XMLElement::GetAttribute( const String& sAttribute ) const
{
	if( !IsValid() )
	{
		return String( "" );
	}

	const char* szAttribute = GetXMLElement()->Attribute( sAttribute.c_str() );
	if( szAttribute == NULL ) {
		return "";
	}
	else {
		return String( szAttribute );
	}
}

void		 XMLElement::SetAttribute( const String &sAttribute, const String &sValue )
{
	GetXMLElement()->SetAttribute( sAttribute.c_str(), sValue.c_str() );
}

void		 XMLElement::SetAttribute( const String &sAttribute, Unsigned32 u32Value )
{
	SetAttribute( sAttribute, CFoundation::String( u32Value ) );
}

void		 XMLElement::SetAttribute( const String &sAttribute, Integer32 i32Value )
{
	SetAttribute( sAttribute, CFoundation::String( i32Value ) );
}

const TiXmlElement* XMLElement::GetXMLElement() const {
    return GetXMLNode()->ToElement();
}

TiXmlElement* XMLElement::GetXMLElement() {
    return GetXMLNode()->ToElement();
}

#endif
