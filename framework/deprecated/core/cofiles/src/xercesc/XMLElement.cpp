#include "cofiles/XML/Xerces/XMLElement.h"

#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/util/XMLString.hpp>

#include "cofiles/XML/Xerces/XMLNode.h"

using namespace CoFiles;
XERCES_CPP_NAMESPACE_USE

DOMElement* ToDOMElement( void* pNode )
{
    return static_cast<DOMElement*>( pNode );
}

const DOMElement* ToDOMElement( const void* pNode )
{
    return static_cast<const DOMElement*>( pNode );
}

Xerces::XMLElement::XMLElement()
{
    m_pNode = new Xerces::XMLNode();
    SetNodeType( ELEMENT_NODE );
}

Xerces::XMLElement::XMLElement( const String& sName )
{
    m_pNode = new Xerces::XMLNode();
    SetNodeType( ELEMENT_NODE );
    SetName( sName );
}
            
void Xerces::XMLElement::SetNodeType( XMLNodeType _eNodeType )
{
    m_pNode->SetNodeType( _eNodeType );
}

XMLNodeType Xerces::XMLElement::GetNodeType() const
{
    return m_pNode->GetNodeType();
}

bool Xerces::XMLElement::IsValid() const
{
	bool bValid = m_pNode->IsValid();
	if ( bValid )
	{
		return IsElement();
	}
	else
	{
		return bValid;
	}
}

bool Xerces::XMLElement::HasChildren() const
{
    return m_pNode->HasChildren();
}

XMLNodePtr Xerces::XMLElement::GetFirstChild() const
{
    return m_pNode->GetFirstChild();
}

XMLNodePtr Xerces::XMLElement::GetNextSibling() const
{
    return m_pNode->GetNextSibling();
}

void Xerces::XMLElement::InsertChildFirst( CoFiles::XMLNode &clChildNode )
{
    m_pNode->InsertChildFirst( clChildNode );
}

void Xerces::XMLElement::InsertChildBefore( CoFiles::XMLNode &clChildNode, CoFiles::XMLNode &clBefore )
{
    m_pNode->InsertChildBefore( clChildNode, clBefore );
}

void Xerces::XMLElement::InsertChildAfter( CoFiles::XMLNode &clChildNode, CoFiles::XMLNode &clAfter )
{
    m_pNode->InsertChildAfter( clChildNode, clAfter );
}

void Xerces::XMLElement::InsertChildLast( CoFiles::XMLNode &clChildNode )
{
    m_pNode->InsertChildLast( clChildNode );
}

bool Xerces::XMLElement::IsDocument() const
{
    return m_pNode->IsDocument();
}

bool Xerces::XMLElement::IsElement() const
{
    return m_pNode->IsElement();
}

bool Xerces::XMLElement::IsText() const
{
    return m_pNode->IsText();
}
	        
void* Xerces::XMLElement::GetXMLNode()
{
    return m_pNode->GetXMLNode();
}
	        
const void* Xerces::XMLElement::GetXMLNode() const
{
    return m_pNode->GetXMLNode();
}
	        
void Xerces::XMLElement::SetXMLNode( void* _pNode )
{
    m_pNode->SetXMLNode( _pNode );
}

const String Xerces::XMLElement::GetName() const
{
    if ( !IsValid() )
    {
        return "";
    }

    return String( XMLString::transcode( ToDOMElement( m_pNode->GetXMLNode() )->getTagName() ) );
}

void Xerces::XMLElement::SetName( const String &sName )
{
    if ( !IsValid() )
    {
        return;
    }

    ToDOMElement( m_pNode->GetXMLNode() )->setNodeValue( XMLString::transcode( sName.c_str() ) );
}

bool Xerces::XMLElement::HasAttribute( const String& sAttribute ) const
{
	if ( !IsValid() )
	{
		return false;
	}

	return ToDOMElement( m_pNode->GetXMLNode() )->hasAttribute( XMLString::transcode( sAttribute.c_str() ) );
}

const String Xerces::XMLElement::GetAttribute( const String& sAttribute ) const
{
	if ( !IsValid() )
	{
		return String( "" );
	}

	const char* szAttribute = XMLString::transcode( ToDOMElement( m_pNode->GetXMLNode() )->getAttribute( XMLString::transcode( sAttribute.c_str() ) ) );
	if( szAttribute == NULL )
	{
		return "";
	}
	else
	{
		return String( szAttribute );
	}
}

void Xerces::XMLElement::SetAttribute( const String &sAttribute, const String &sValue )
{
    if ( !IsValid() )
    {
        return;
    }

	ToDOMElement( m_pNode->GetXMLNode() )->setAttribute( XMLString::transcode( sAttribute.c_str() ), XMLString::transcode( sValue.c_str() ) );
}

void Xerces::XMLElement::SetAttribute( const String &sAttribute, Unsigned32 u32Value )
{
	SetAttribute( sAttribute, CoFiles::String( u32Value ) );
}

void Xerces::XMLElement::SetAttribute( const String &sAttribute, Unsigned64 _u64Value )
{
	SetAttribute( sAttribute, CoFiles::String( _u64Value ) );
}

void Xerces::XMLElement::SetAttribute( const String &sAttribute, Integer32 i32Value )
{
	SetAttribute( sAttribute, CoFiles::String( i32Value ) );
}
			
void Xerces::XMLElement::SetAttribute( const String& _sAttribute, Float32 _f32Value )
{
	SetAttribute( _sAttribute, CoFiles::String( _f32Value ) );
}
