#include "cofiles/XML/RapidXML/XMLDocument.h"

#include "rapidxml/rapidxml.hpp"

#include "cofiles/Makros.h"
#include "cofiles/XML/RapidXML/XMLNode.h"

using namespace CoFiles;
using namespace rapidxml;

xml_document< wchar_t >* ToDOMDocument( void* pNode )
{
    return static_cast< xml_document< wchar_t >* >( pNode );
}

const xml_document< wchar_t >* ToDOMDocument( const void* pNode )
{
    return static_cast< const xml_document< wchar_t >* >( pNode );
}
			
RapidXML::XMLDocument::XMLDocument()
{
    m_pNode = new RapidXML::XMLNode();
    SetNodeType( DOCUMENT_NODE );
}

RapidXML::XMLDocument::~XMLDocument()
{
}
            
void RapidXML::XMLDocument::SetNodeType( XMLNodeType _eNodeType )
{
    m_pNode->SetNodeType( _eNodeType );
}

XMLNodeType RapidXML::XMLDocument::GetNodeType() const
{
    return m_pNode->GetNodeType();
}

bool RapidXML::XMLDocument::IsValid() const
{
	bool bValid = m_pNode->IsValid();
	if ( bValid )
	{
		return IsDocument();
	}
	else
	{
		return bValid;
	}
}

bool RapidXML::XMLDocument::HasChildren() const
{
    return m_pNode->HasChildren();
}

XMLNodePtr RapidXML::XMLDocument::GetFirstChild() const
{
    return m_pNode->GetFirstChild();
}

XMLNodePtr RapidXML::XMLDocument::GetNextSibling() const
{
    return m_pNode->GetNextSibling();
}

void RapidXML::XMLDocument::InsertChildFirst( CoFiles::XMLNode &clChildNode )
{
    m_pNode->InsertChildFirst( clChildNode );
}

void RapidXML::XMLDocument::InsertChildBefore( CoFiles::XMLNode &clChildNode, CoFiles::XMLNode &clBefore )
{
    m_pNode->InsertChildBefore( clChildNode, clBefore );
}

void RapidXML::XMLDocument::InsertChildAfter( CoFiles::XMLNode &clChildNode, CoFiles::XMLNode &clAfter )
{
    m_pNode->InsertChildAfter( clChildNode, clAfter );
}

void RapidXML::XMLDocument::InsertChildLast( CoFiles::XMLNode& _clChildNode )
{
    m_pNode->InsertChildLast( _clChildNode );
}

bool RapidXML::XMLDocument::IsDocument() const
{
    return m_pNode->IsDocument();
}

bool RapidXML::XMLDocument::IsElement() const
{
    return m_pNode->IsElement();
}

bool RapidXML::XMLDocument::IsText() const
{
    return m_pNode->IsText();
}

XMLDocumentPtr RapidXML::XMLDocument::GetDocument()
{
    return m_pNode->GetDocument();
}

XMLDocumentCPtr RapidXML::XMLDocument::GetDocument() const
{
    return m_pNode->GetDocument();
}
	        
void* RapidXML::XMLDocument::GetXMLNode()
{
    return m_pNode->GetXMLNode();
}
	        
const void* RapidXML::XMLDocument::GetXMLNode() const
{
    return m_pNode->GetXMLNode();
}
	        
void RapidXML::XMLDocument::SetXMLNode( void* _pNode )
{
    m_pNode->SetXMLNode( _pNode );
}

CoFiles::XMLElementPtr RapidXML::XMLDocument::GetRootElement() const
{
    return static_pointer_cast< CoFiles::XMLElement >( GetFirstChild() );
}

CoFiles::XMLElementPtr RapidXML::XMLDocument::CreateElement( const CoFiles::String& _strName )
{
    // Get document
    xml_document< wchar_t >* pDocument = ToDOMDocument( m_pNode->GetXMLNode() );

    // Allocate memory for element name
    wchar_t* szName = pDocument->allocate_string( _strName.wc_str() );

    // Create element
    xml_node< wchar_t >* pElement = pDocument->allocate_node( node_element, szName );

	XMLElementPtr spNewElement( new XMLElement() );
	spNewElement->SetXMLNode( pElement );

	return spNewElement;
}

CoFiles::XMLTextPtr RapidXML::XMLDocument::CreateText( const CoFiles::String& _strText )
{
    // Get document
    xml_document< wchar_t >* pDocument = ToDOMDocument( m_pNode->GetXMLNode() );

    // Allocate memory for text
    wchar_t* szText = pDocument->allocate_string( _strText.wc_str() );

    // Create text
    xml_node< wchar_t >* pText = pDocument->allocate_node( node_data, L"", szText );

    XMLTextPtr spNewText( new XMLText() );
	spNewText->SetXMLNode( pText );

	return spNewText;
}

void RapidXML::XMLDocument::Refine()
{
	Refine( GetFirstChild(), 0 );
}

void RapidXML::XMLDocument::Refine( CoFiles::XMLNodePtr _pNode, Unsigned32 u32Level )
{
	if( _pNode != NULL &&
        _pNode->IsValid() )
	{
		// refine children
		String sCR = "\r\n";
		for( Unsigned32 u32Idx = 0; u32Idx < u32Level; ++u32Idx )
		{
			sCR += "\t";
		}

		CoFiles::XMLNodePtr spChildNode = _pNode->GetFirstChild();
		while ( spChildNode != NULL &&
                spChildNode->IsValid() )
		{
			Refine( spChildNode, u32Level + 1 );
			if ( spChildNode->HasChildren() )
			{
			    CoFiles::XMLTextPtr spCarriageReturn = CreateText( sCR );
			    spChildNode->InsertChildFirst( *spCarriageReturn );
			}

            CoFiles::XMLTextPtr spCarriageReturn = CreateText( sCR );
			_pNode->InsertChildBefore( *spCarriageReturn, *spChildNode );
			spChildNode = spChildNode->GetNextSibling();
		}
	}
}
