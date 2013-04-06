#include "cfoundation/XML/RapidXML/XMLNode.h"

#include "rapidxml/rapidxml.hpp"

#include "cfoundation/Base/Defines.h"
#include "cfoundation/XML/RapidXML/XMLDocument.h"
#include "cfoundation/XML/RapidXML/XMLElement.h"
#include "cfoundation/XML/RapidXML/XMLText.h"

using namespace CFoundation;
using namespace rapidxml;

xml_node< wchar_t >* ToDOMNode( void* _pNode )
{
    xml_node< wchar_t >* pNode = static_cast< xml_node< wchar_t >* >( _pNode );
    return pNode;
}

const xml_node< wchar_t >* ToDOMNode( const void* _pNode )
{
    const xml_node< wchar_t >* pNode = static_cast< const xml_node< wchar_t >* >( _pNode );
    return pNode;
}

RapidXML::XMLNode::XMLNode()
:	m_pNode( NULL ),
    m_eNodeType( XML_NODE_TYPE_NONE )
{
}

RapidXML::XMLNode::~XMLNode()
{
    if ( m_pNode != NULL &&
         m_eNodeType == DOCUMENT_NODE )
    {
        delete m_pNode;
        m_pNode = NULL;
    }
}
            
void RapidXML::XMLNode::SetNodeType( XMLNodeType _eNodeType )
{
    m_eNodeType = _eNodeType;
}
            
XMLNodeType RapidXML::XMLNode::GetNodeType() const
{
    return m_eNodeType;
}

bool RapidXML::XMLNode::IsValid() const
{
	return GetXMLNode() != NULL;
}

bool RapidXML::XMLNode::HasChildren() const
{
    if( IsValid() ) {
        return ToDOMNode( GetXMLNode() )->first_node() != NULL;
    }
    else {
        return false;
    }
}

XMLNodePtr RapidXML::XMLNode::GetFirstChild() const
{
    m_spFirstChild.reset();

	if ( ToDOMNode( GetXMLNode() ) != NULL )
	{
        xml_node< wchar_t >* pNode = ToDOMNode( GetXMLNode() )->first_node();

		while ( pNode != NULL &&
                !IsValidXMLNode( pNode ) )
		{
            pNode = pNode->next_sibling();
		}
        
        if ( pNode != NULL )
        {
            m_spFirstChild = CreateNode( pNode );
        }        
	}
    
    return m_spFirstChild;
}
            
CFoundation::XMLNodePtr RapidXML::XMLNode::GetFirstChild( const String& _strName ) const
{
    m_spFirstChild.reset();

    const xml_node< wchar_t >* pNode = ToDOMNode( GetXMLNode() );
	if ( pNode != NULL )
	{
        xml_node< wchar_t >* pChild = pNode->first_node();

		while ( pChild != NULL )
		{
            if ( IsValidXMLNode( pChild ) &&
                 _strName == pChild->name() )
            {
                break;
            }

            pChild = pChild->next_sibling();
		}
        
        if ( pChild != NULL )
        {
            m_spFirstChild = CreateNode( pChild );
        }        
	}
    
    return m_spFirstChild;
}

XMLNodePtr RapidXML::XMLNode::GetNextSibling() const
{
    m_spNextSibling.reset();

    if ( GetXMLNode() == NULL )
    {
        return XMLNodePtr();
    }

    xml_node< wchar_t >* pNode = ToDOMNode( GetXMLNode() )->next_sibling();
	while( pNode != NULL &&
           !IsValidXMLNode( pNode ) )
	{
        pNode = pNode->next_sibling();
	}

    if ( pNode != NULL )
    {
	    m_spNextSibling = CreateNode( pNode );
    }

    return m_spNextSibling;
}

void RapidXML::XMLNode::InsertChildFirst( CFoundation::XMLNodePtr _spChildNode )
{
	if ( IsValid() )
	{
        xml_node< wchar_t >* pNode = ToDOMNode( GetXMLNode() );
        pNode->insert_node( pNode->first_node(), ToDOMNode( _spChildNode->GetXMLNode() ) );
	}
}

void RapidXML::XMLNode::InsertChildBefore( CFoundation::XMLNodePtr _spChildNode, CFoundation::XMLNodePtr _spBefore )
{
	if( IsValid() )
	{
        xml_node< wchar_t >* pNode = ToDOMNode( GetXMLNode() );
        pNode->insert_node( ToDOMNode( _spBefore->GetXMLNode() ), ToDOMNode( _spChildNode->GetXMLNode() ) );
	}
}

void RapidXML::XMLNode::InsertChildAfter( CFoundation::XMLNodePtr _spChildNode, CFoundation::XMLNodePtr _spAfter )
{
	if( IsValid() )
	{
        xml_node< wchar_t >* pNode = ToDOMNode( GetXMLNode() );
        pNode->insert_node( ToDOMNode( _spAfter->GetXMLNode() )->next_sibling(), ToDOMNode( _spChildNode->GetXMLNode() ) );
	}
}

void RapidXML::XMLNode::InsertChildLast( CFoundation::XMLNodePtr _spChildNode )
{
	if( IsValid() )
	{
        xml_node< wchar_t >* pNode = ToDOMNode( GetXMLNode() );
        pNode->insert_node( NULL, ToDOMNode( _spChildNode->GetXMLNode() ) );
	}
}

void RapidXML::XMLNode::RemoveChildFirst()
{
    xml_node< wchar_t >* pNode = ToDOMNode( GetXMLNode() );
    if ( pNode == NULL )
    {
        return;
    }

    pNode->remove_first_node();
}

void RapidXML::XMLNode::RemoveChild( CFoundation::XMLNodePtr _spChildNode )
{
    xml_node< wchar_t >* pNode = ToDOMNode( GetXMLNode() );
    if ( pNode == NULL )
    {
        return;
    }

    xml_node< wchar_t >* pChild = ToDOMNode( _spChildNode->GetXMLNode() );
    if ( pChild == NULL )
    {
        return;
    }

    pNode->remove_node( pChild );
}

void RapidXML::XMLNode::RemoveChildLast()
{
    xml_node< wchar_t >* pNode = ToDOMNode( GetXMLNode() );
    if ( pNode == NULL )
    {
        return;
    }

    pNode->remove_last_node();
}

bool RapidXML::XMLNode::IsDocument() const
{
    return m_eNodeType == DOCUMENT_NODE;
}

bool RapidXML::XMLNode::IsElement() const
{
    return m_eNodeType == ELEMENT_NODE;
}

bool RapidXML::XMLNode::IsText() const
{
    return m_eNodeType == TEXT_NODE;
}

/*
XMLDocumentPtr RapidXML::XMLNode::GetDocument()
{
	XMLDocumentPtr spDocument( new XMLDocument() );
    spDocument->SetXMLNode( m_pNode->document() );
    return spDocument;
}

XMLDocumentCPtr RapidXML::XMLNode::GetDocument() const
{
    return const_cast< XMLNode* >( this )->GetDocument();
}*/

const void* RapidXML::XMLNode::GetXMLNode() const
{
	return m_pNode;
}

void* RapidXML::XMLNode::GetXMLNode()
{
	return m_pNode;
}

void RapidXML::XMLNode::SetXMLNode( void* pNode )
{
	m_pNode = ToDOMNode( pNode );
}

XMLNodePtr RapidXML::XMLNode::CreateNode( xml_node< wchar_t >* _pNode )
{
	if ( _pNode == NULL )
	{
        return XMLNodePtr();
    }

    CFoundation::XMLNodePtr spNewNode;
    if ( _pNode->type() == node_element )
    {
        spNewNode = XMLElementPtr( new RapidXML::XMLElement() );
    }
    else if ( _pNode->type() == node_data )
    {
        spNewNode = XMLTextPtr( new RapidXML::XMLText() );
    }
	    
    if ( spNewNode!= NULL )
    {
        spNewNode->SetXMLNode( _pNode );
    }

    return spNewNode;
}

bool RapidXML::XMLNode::IsValidXMLNode( xml_node< wchar_t >* pNode )
{
	if ( pNode == NULL )
	{
        return false;
    }

    if ( pNode->type() == node_comment )
	{
		return false;
	}
	else if ( pNode->type() == node_data )
	{
		XMLText clTextNode;
		clTextNode.SetXMLNode( pNode );
		String sText = clTextNode.GetText();
		if ( sText == "" )
		{
			return false;
		}
	}

	return true;
}