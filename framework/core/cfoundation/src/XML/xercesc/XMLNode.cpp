#include "cfoundation/XML/Xerces/XMLNode.h"

#ifdef XML_IMPL_XERCES

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/dom/DOMImplementation.hpp>

#include "cfoundation/Makros.h"
#include "cfoundation/XML/Xerces/XMLElement.h"
#include "cfoundation/XML/Xerces/XMLText.h"

using namespace CFoundation;
XERCES_CPP_NAMESPACE_USE

DOMNode* ToDOMNode( void* _pNode )
{
    DOMNode* pNode = static_cast< DOMNode* >( _pNode );
    return pNode;
}

const DOMNode* ToDOMNode( const void* _pNode )
{
    const DOMNode* pNode = static_cast< const DOMNode* >( _pNode );
    return pNode;
}

Xerces::XMLNode::XMLNode()
:	m_pNode( NULL ),
    m_eNodeType( XML_NODE_TYPE_NONE )
{
}
            
void Xerces::XMLNode::SetNodeType( XMLNodeType _eNodeType )
{
    m_eNodeType = _eNodeType;
}
            
XMLNodeType Xerces::XMLNode::GetNodeType() const
{
    return m_eNodeType;
}

bool Xerces::XMLNode::IsValid() const
{
	return GetXMLNode() != NULL;
}

bool Xerces::XMLNode::HasChildren() const
{
    if( IsValid() ) {
        return !ToDOMNode( GetXMLNode() )->getChildNodes()->getLength();
    }
    else {
        return false;
    }
}

XMLNodePtr Xerces::XMLNode::GetFirstChild() const
{
    m_spFirstChild.reset();

	if ( ToDOMNode( GetXMLNode() ) != NULL )
	{
		DOMNode *pNode = ToDOMNode( GetXMLNode() )->getFirstChild();

		while ( pNode != NULL &&
                !IsValidXMLNode( pNode ) )
		{
			pNode = pNode->getNextSibling();
		}
        
        if ( pNode != NULL )
        {
            m_spFirstChild = CreateNode( pNode );
        }        
	}
    
    return m_spFirstChild;
}

XMLNodePtr Xerces::XMLNode::GetNextSibling() const
{
    m_spNextSibling.reset();

    if ( GetXMLNode() == NULL )
    {
        return XMLNodePtr();
    }

	DOMNode *pNode = ToDOMNode( GetXMLNode() )->getNextSibling();
	while( pNode != NULL &&
           !IsValidXMLNode( pNode ) )
	{
		pNode = pNode->getNextSibling();
	}

    if ( pNode != NULL )
    {
	    m_spNextSibling = CreateNode( pNode );
    }

    return m_spNextSibling;
}

void Xerces::XMLNode::InsertChildFirst( CFoundation::XMLNode& _clChildNode )
{
	if ( IsValid() )
	{
        try
        {
            ToDOMNode( GetXMLNode() )->insertBefore( ToDOMNode( _clChildNode.GetXMLNode() ), NULL );
        }
        catch ( const DOMException& e )
        {
            CO_WARN( "XMLNode::InsertChildFirst: %s", XMLString::transcode( e.msg ) );
        }
	}
}

void Xerces::XMLNode::InsertChildBefore( CFoundation::XMLNode& _clChildNode, CFoundation::XMLNode& _clBefore )
{
	if( IsValid() )
	{
        ToDOMNode( GetXMLNode() )->insertBefore( ToDOMNode( _clChildNode.GetXMLNode() ), ToDOMNode( _clBefore.GetXMLNode() ) );
	}
}

void Xerces::XMLNode::InsertChildAfter( CFoundation::XMLNode& _clChildNode, CFoundation::XMLNode& _clAfter )
{
	if( IsValid() )
	{
		if( _clAfter.GetXMLNode() != NULL )
		{
            ToDOMNode( GetXMLNode() )->insertBefore( ToDOMNode( _clChildNode.GetXMLNode() ), ToDOMNode( _clAfter.GetXMLNode() )->getNextSibling() );
		}
		else
		{
            ToDOMNode( GetXMLNode() )->appendChild( ToDOMNode( _clChildNode.GetXMLNode() ) );
		}
	}
}

void Xerces::XMLNode::InsertChildLast( CFoundation::XMLNode& _clChildNode )
{
	if( IsValid() )
	{
	    DOMNode* pDOMNode = ToDOMNode( GetXMLNode() );
	    DOMNode* pChildDOMNode = ToDOMNode( _clChildNode.GetXMLNode() );
        if ( pDOMNode != NULL &&
             pChildDOMNode != NULL )
        {
            try
            {
                pDOMNode->appendChild( pChildDOMNode );
            }
            catch ( const DOMException& e )
            {
                CO_WARN( "XMLNode::InsertChildLast: %s", XMLString::transcode( e.msg ) );
            }
        }
	}
}

bool Xerces::XMLNode::IsDocument() const
{
    return m_eNodeType == DOCUMENT_NODE;
}

bool Xerces::XMLNode::IsElement() const
{
    return m_eNodeType == ELEMENT_NODE;
}

bool Xerces::XMLNode::IsText() const
{
    return m_eNodeType == TEXT_NODE;
}

const void* Xerces::XMLNode::GetXMLNode() const
{
	return m_pNode;
}

void* Xerces::XMLNode::GetXMLNode()
{
	return m_pNode;
}

void Xerces::XMLNode::SetXMLNode( void* pNode )
{
	m_pNode = ToDOMNode( pNode );
}

XMLNodePtr Xerces::XMLNode::CreateNode( DOMNode* _pNode )
{
	if ( _pNode == NULL )
	{
        return XMLNodePtr();
    }

    CFoundation::XMLNodePtr spNewNode;
    if ( _pNode->getNodeType() == DOMNode::ELEMENT_NODE )
    {
        XMLElement* pElement = new Xerces::XMLElement();
        spNewNode = CFoundation::XMLNodePtr( pElement );
    }
    else if ( _pNode->getNodeType() == DOMNode::TEXT_NODE )
    {
        XMLText* pText = new Xerces::XMLText();
        spNewNode = CFoundation::XMLNodePtr( pText );
    }
	    
    if ( spNewNode!= NULL )
    {
        spNewNode->SetXMLNode( _pNode );
    }

    return spNewNode;
}

bool Xerces::XMLNode::IsValidXMLNode( DOMNode* pNode )
{
	if ( pNode == NULL )
	{
        return false;
    }

	if ( pNode->getNodeType() == DOMNode::COMMENT_NODE )
	{
		return false;
	}
	else if ( pNode->getNodeType() == DOMNode::NOTATION_NODE )
	{
		return false;
	}
	else if ( pNode->getNodeType() == DOMNode::TEXT_NODE )
	{
		XMLText clTextNode;
		clTextNode.SetXMLNode( pNode );
		String sText = clTextNode.GetText();
		if( sText == "" )
		{
			return false;
		}
	}

	return true;
}

#endif
