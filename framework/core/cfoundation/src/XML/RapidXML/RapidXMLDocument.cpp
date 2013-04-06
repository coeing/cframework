#include "cfoundation/XML/RapidXML/XMLDocument.h"

#include "rapidxml/rapidxml.hpp"

#include "cfoundation/Base/Defines.h"
#include "cfoundation/XML/RapidXML/XMLNode.h"

using namespace CFoundation;
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
    m_pNode = RapidXML::XMLNodePtr( new RapidXML::XMLNode() );
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

XMLNodePtr RapidXML::XMLDocument::GetFirstChild( const String& _strName ) const
{
    return m_pNode->GetFirstChild( _strName );
}

XMLNodePtr RapidXML::XMLDocument::GetNextSibling() const
{
    return m_pNode->GetNextSibling();
}

void RapidXML::XMLDocument::InsertChildFirst( CFoundation::XMLNodePtr _spChildNode )
{
    m_pNode->InsertChildFirst( _spChildNode );
}

void RapidXML::XMLDocument::InsertChildBefore( CFoundation::XMLNodePtr _spChildNode, CFoundation::XMLNodePtr _spBefore )
{
    m_pNode->InsertChildBefore( _spChildNode, _spBefore );
}

void RapidXML::XMLDocument::InsertChildAfter( CFoundation::XMLNodePtr _spChildNode, CFoundation::XMLNodePtr _spAfter )
{
    m_pNode->InsertChildAfter( _spChildNode, _spAfter );
}

void RapidXML::XMLDocument::InsertChildLast( CFoundation::XMLNodePtr _spChildNode )
{
    m_pNode->InsertChildLast( _spChildNode );
}

void RapidXML::XMLDocument::RemoveChildFirst()
{
    m_pNode->RemoveChildFirst();
}

void RapidXML::XMLDocument::RemoveChild( CFoundation::XMLNodePtr _spChildNode )
{
    m_pNode->RemoveChild( _spChildNode );
}

void RapidXML::XMLDocument::RemoveChildLast()
{
    m_pNode->RemoveChildLast();
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

/*
XMLDocumentPtr RapidXML::XMLDocument::GetDocument()
{
    return m_pNode->GetDocument();
}

XMLDocumentCPtr RapidXML::XMLDocument::GetDocument() const
{
    return m_pNode->GetDocument();
}*/
	        
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

CFoundation::XMLElementPtr RapidXML::XMLDocument::GetRootElement() const
{
    return static_pointer_cast< CFoundation::XMLElement >( GetFirstChild() );
}

CFoundation::XMLElementPtr RapidXML::XMLDocument::CreateElement( const CFoundation::String& _strName )
{
    // Get document
    xml_document< wchar_t >* pDocument = ToDOMDocument( m_pNode->GetXMLNode() );

    // Allocate memory for element name
    wchar_t* szName = pDocument->allocate_string( _strName.wc_str() );

    // Create element
    xml_node< wchar_t >* pElement = pDocument->allocate_node( node_element, szName );
    pElement->document();

	XMLElementPtr spNewElement( new XMLElement() );
	spNewElement->SetXMLNode( pElement );

	return spNewElement;
}

CFoundation::XMLTextPtr RapidXML::XMLDocument::CreateText( const CFoundation::String& _strText )
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

void RapidXML::XMLDocument::Refine( CFoundation::XMLNodePtr _pNode, Unsigned32 u32Level )
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

		CFoundation::XMLNodePtr spChildNode = _pNode->GetFirstChild();
		while ( spChildNode != NULL &&
                spChildNode->IsValid() )
		{
			Refine( spChildNode, u32Level + 1 );
			if ( spChildNode->HasChildren() )
			{
			    CFoundation::XMLTextPtr spCarriageReturn = CreateText( sCR );
			    spChildNode->InsertChildFirst( spCarriageReturn );
			}

            CFoundation::XMLTextPtr spCarriageReturn = CreateText( sCR );
			_pNode->InsertChildBefore( spCarriageReturn, spChildNode );
			spChildNode = spChildNode->GetNextSibling();
		}
	}
}
