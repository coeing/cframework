#include "cfoundation/XML/tinyxml/XMLDocument.h"

#ifdef XML_IMPL_TINYXML

#include "cfoundation/XML/tinyxml/XMLElement.h"

#include "tinyxml.h"

using namespace CFoundation;
using namespace TinyXml;

XMLDocument::XMLDocument( const String& _sRootName )
{
    m_pNode = new TiXmlDocument( _sRootName.c_str() );
}

XMLDocument::~XMLDocument()
{
}

XMLElement	XMLDocument::CreateElement( const CFoundation::String &sName )
{
    XMLElement clNewElement;
    clNewElement.SetXMLNode( new TiXmlElement( sName.c_str() ) );

	return clNewElement;
}

XMLText	XMLDocument::CreateText( const CFoundation::String &sText )
{
    XMLText clNewText;
    clNewText.SetXMLNode( new TiXmlText( sText.c_str() ) );

	return clNewText;
}

void XMLDocument::Refine()
{
	//DOMNode *pCarriageReturn = m_pDoc->createTextNode( XMLString::transcode( "\r\n" ) );
	//m_pDoc->insertBefore( pCarriageReturn, m_pDoc->getFirstChild() );

	Refine( GetFirstChild(), 0 );
}

void XMLDocument::Refine( XMLNode clXMLNode, Unsigned32 u32Level )
{
    TiXmlNode *pNode = clXMLNode.GetXMLNode();
	if( pNode != NULL )
	{
		// refine children
		String sCR = "\r\n";
		for( Unsigned32 u32Idx = 0; u32Idx < u32Level; ++u32Idx )
		{
			sCR += "\t";
		}

		XMLNode clChildNode = clXMLNode.GetFirstChild();
		while( clChildNode.IsValid() )
		{
			Refine( clChildNode, u32Level + 1 );
			if( clChildNode.HasChildren() )
			{
			    XMLText clCarriageReturn = CreateText( sCR );
			    clChildNode.InsertChildFirst( clCarriageReturn );
			}

            XMLText clCarriageReturn = CreateText( sCR );
			clXMLNode.InsertChildBefore( clCarriageReturn, clChildNode );
			clChildNode = clChildNode.GetNextSibling();
		}
	}
}

const TiXmlDocument* XMLDocument::GetXMLDocument() const {
    return GetXMLNode()->ToDocument();
}

TiXmlDocument* XMLDocument::GetXMLDocument() {
    return GetXMLNode()->ToDocument();
}

#endif
