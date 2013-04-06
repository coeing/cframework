#include "cfoundation/XML/tinyxml/XMLNode.h"

#ifdef XML_IMPL_TINYXML

#include "tinyxml.h"

using namespace CFoundation;
using namespace TinyXml;

XMLNode::XMLNode()
:	m_pNode( NULL )
{
}

XMLNode::XMLNode( const XMLNode& rhs )
{
    m_pNode = rhs.m_pNode;
}

XMLNode& XMLNode::operator=( const XMLNode& rhs )
{
    m_pNode = rhs.m_pNode;
    return *this;
}

void XMLNode::Clear()
{
    if( m_pNode != NULL )
    {
        delete m_pNode;
        m_pNode = NULL;
    }
}

bool XMLNode::IsValid() const
{
	if( m_pNode != NULL ) {
		m_pNode->Value();
	}
	return ( m_pNode != NULL );
}

bool XMLNode::HasChildren() const
{
    if( IsValid() ) {
        return !m_pNode->NoChildren();
    }
    else {
        return false;
    }
}

XMLNode XMLNode::GetFirstChild()
{
	if( m_pNode != NULL )
	{
		TiXmlNode *pNode = m_pNode->FirstChild();
		while(	pNode != NULL && (
				pNode->ToComment() != NULL ||
				pNode->ToDeclaration() != NULL ||
				pNode->ToUnknown() != NULL ) )
		{
			pNode = pNode->NextSibling();
		}

        XMLNode clFirstChildNode;
        clFirstChildNode.m_pNode = pNode;
        return clFirstChildNode;
	}
	else
	{
		return XMLNode();
	}
}

const XMLNode XMLNode::GetFirstChild() const
{
	XMLNode *pThis = const_cast<XMLNode*>( this );
	return pThis->GetFirstChild();
}

XMLNode* XMLNode::GetNextSibling() const
{
	TiXmlNode *pNode = m_pNode->NextSibling();
    while(	pNode != NULL && (
			pNode->ToComment() != NULL ||
			pNode->ToDeclaration() != NULL ||
			pNode->ToUnknown() != NULL ) )
    {
        pNode = pNode->NextSibling();
    }

	XMLNode clFirstChildNode;
	clFirstChildNode.m_pNode = pNode;
	return clFirstChildNode;
}

void XMLNode::InsertChildFirst( XMLNode &clChildNode )
{
	if( IsValid() )
	{
		m_pNode->InsertBeforeChild( NULL, *clChildNode.m_pNode );
	}
}

void XMLNode::InsertChildBefore( XMLNode &clChildNode, XMLNode &clBefore )
{
	if( IsValid() )
	{
		m_pNode->InsertBeforeChild( clBefore.m_pNode, *clChildNode.m_pNode );
	}
}

void XMLNode::InsertChildAfter( XMLNode &clChildNode, XMLNode &clAfter )
{
	if( IsValid() )
	{
		m_pNode->InsertAfterChild( clAfter.m_pNode, *clChildNode.m_pNode );
	}
}

void XMLNode::InsertChildLast( XMLNode &clChildNode )
{
	if( IsValid() )
	{
		m_pNode->InsertAfterChild( NULL, *clChildNode.m_pNode );
	}
}

bool XMLNode::IsDocument() const
{
    if( IsValid() ) {
        return m_pNode->ToDocument() != NULL;
    }
    else {
        return false;
    }
}

bool XMLNode::IsElement() const
{
    if( IsValid() ) {
        return m_pNode->ToElement() != NULL;
    }
    else {
        return false;
    }
}

bool XMLNode::IsText() const
{
    if( IsValid() ) {
        return m_pNode->ToText() != NULL;
    }
    else {
        return false;
    }
}

const TiXmlNode* XMLNode::GetXMLNode() const
{
	return m_pNode;
}

TiXmlNode* XMLNode::GetXMLNode()
{
	return m_pNode;
}

void XMLNode::SetXMLNode( TiXmlNode *pNode )
{
    m_pNode = pNode;
}

#endif
