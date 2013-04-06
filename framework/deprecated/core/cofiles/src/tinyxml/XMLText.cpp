#include "cofiles/tinyxml/XMLText.h"

#include "tinyxml.h"

using namespace CoFiles;
using namespace TinyXml;

XMLText::XMLText()
{
}

XMLText::XMLText( const XMLNode &rhs )
: XMLNode( rhs )
{
}

XMLText::XMLText( const String& sText )
{
    SetText( sText );
}

XMLText& XMLText::operator=( const XMLNode& rhs )
{
    XMLNode::operator=( rhs );
    return *this;
}

const String XMLText::GetText() const
{
    return String( GetXMLText()->Value() );
}

void XMLText::SetText( const String &sText )
{
    GetXMLText()->SetValue( sText.c_str() );
}

const TiXmlText* XMLText::GetXMLText() const {
    return GetXMLNode()->ToText();
}

TiXmlText* XMLText::GetXMLText() {
    return GetXMLNode()->ToText();
}

