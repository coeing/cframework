#include "cfoundation/XML/tinyxml/XMLParser.h"

#ifdef XML_IMPL_TINYXML

#include <stdio.h>
#include <wchar.h>

#include "tinyxml.h"

#include "cfoundation/Makros.h"
#include "cfoundation/XML/tinyxml/XMLDocument.h"

using namespace CFoundation;

TinyXml::XMLParser::XMLParser()
{
}

TinyXml::XMLParser::~XMLParser()
{
}

bool TinyXml::XMLParser::ParseFile( const CFoundation::String &sFilename )
{
	m_sFilename = sFilename;

    if( m_sFilename != "" ) {
        m_clDocument.Clear();
    }
    else {
        return false;
    }

	TiXmlDocument *pDoc = new TiXmlDocument( m_sFilename.c_str() );
	bool loadOkay = pDoc->LoadFile();

	m_clDocument.SetXMLNode( pDoc );

    return loadOkay;
}

bool TinyXml::XMLParser::ParseFile( const wchar_t *filename )
{
    return ParseFile( String( filename ) );
}

TinyXml::XMLDocument TinyXml::XMLParser::GetDocument() const
{
	return m_clDocument;
}

TinyXml::XMLNode TinyXml::XMLParser::GetRoot() const
{
	XMLNode clNode;

	if( m_clDocument.IsValid() )
	{
		clNode.m_pNode = m_clDocument.GetFirstChild().GetXMLNode();
	}
	else
	{
		clNode.m_pNode = NULL;
	}

	return clNode;
}

bool TinyXml::XMLParser::WriteFile( const XMLDocument &clDocument, const CFoundation::String &sFileName )
{
	if( sFileName == "" )
	{
		return false;
	}

    clDocument.GetXMLDocument()->SaveFile( sFileName.c_str() );

	return true;
}

#endif

