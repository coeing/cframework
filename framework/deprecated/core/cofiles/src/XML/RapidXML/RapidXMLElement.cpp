#include "cofiles/XML/RapidXML/XMLElement.h"

#include "rapidxml/rapidxml.hpp"

#include "cofiles/XML/RapidXML/XMLDocument.h"
#include "cofiles/XML/RapidXML/XMLNode.h"

using namespace CoFiles;
using namespace rapidxml;

xml_node< wchar_t >* ToDOMElement( void* pNode )
{
    return static_cast< xml_node< wchar_t >* >( pNode );
}

const xml_node< wchar_t >* ToDOMElement( const void* pNode )
{
    return static_cast< const xml_node< wchar_t >* >( pNode );
}

RapidXML::XMLElement::XMLElement()
{
    m_pNode = new RapidXML::XMLNode();
    SetNodeType( ELEMENT_NODE );
}

RapidXML::XMLElement::XMLElement( const String& sName )
{
    m_pNode = new RapidXML::XMLNode();
    SetNodeType( ELEMENT_NODE );
    SetName( sName );
}
            
void RapidXML::XMLElement::SetNodeType( XMLNodeType _eNodeType )
{
    m_pNode->SetNodeType( _eNodeType );
}

XMLNodeType RapidXML::XMLElement::GetNodeType() const
{
    return m_pNode->GetNodeType();
}

bool RapidXML::XMLElement::IsValid() const
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

bool RapidXML::XMLElement::HasChildren() const
{
    return m_pNode->HasChildren();
}

XMLNodePtr RapidXML::XMLElement::GetFirstChild() const
{
    return m_pNode->GetFirstChild();
}

XMLNodePtr RapidXML::XMLElement::GetNextSibling() const
{
    return m_pNode->GetNextSibling();
}

void RapidXML::XMLElement::InsertChildFirst( CoFiles::XMLNode &clChildNode )
{
    m_pNode->InsertChildFirst( clChildNode );
}

void RapidXML::XMLElement::InsertChildBefore( CoFiles::XMLNode &clChildNode, CoFiles::XMLNode &clBefore )
{
    m_pNode->InsertChildBefore( clChildNode, clBefore );
}

void RapidXML::XMLElement::InsertChildAfter( CoFiles::XMLNode &clChildNode, CoFiles::XMLNode &clAfter )
{
    m_pNode->InsertChildAfter( clChildNode, clAfter );
}

void RapidXML::XMLElement::InsertChildLast( CoFiles::XMLNode &clChildNode )
{
    m_pNode->InsertChildLast( clChildNode );
}

bool RapidXML::XMLElement::IsDocument() const
{
    return m_pNode->IsDocument();
}

bool RapidXML::XMLElement::IsElement() const
{
    return m_pNode->IsElement();
}

bool RapidXML::XMLElement::IsText() const
{
    return m_pNode->IsText();
}
	        
void* RapidXML::XMLElement::GetXMLNode()
{
    return m_pNode->GetXMLNode();
}

XMLDocumentPtr RapidXML::XMLElement::GetDocument()
{
    return m_pNode->GetDocument();
}

XMLDocumentCPtr RapidXML::XMLElement::GetDocument() const
{
    return m_pNode->GetDocument();
}
	        
const void* RapidXML::XMLElement::GetXMLNode() const
{
    return m_pNode->GetXMLNode();
}
	        
void RapidXML::XMLElement::SetXMLNode( void* _pNode )
{
    m_pNode->SetXMLNode( _pNode );
}

const String RapidXML::XMLElement::GetName() const
{
    if ( !IsValid() )
    {
        return "";
    }

    return String( ToDOMElement( m_pNode->GetXMLNode() )->name() );
}

void RapidXML::XMLElement::SetName( const String &sName )
{
    if ( !IsValid() )
    {
        return;
    }

    ToDOMElement( m_pNode->GetXMLNode() )->name( sName.wc_str() );
}

bool RapidXML::XMLElement::HasAttribute( const String& _strAttribute ) const
{
	if ( !IsValid() )
	{
		return false;
	}

    return ToDOMElement( m_pNode->GetXMLNode() )->first_attribute( _strAttribute.wc_str() ) != NULL;
}

const String RapidXML::XMLElement::GetAttribute( const String& _strAttribute ) const
{
	if ( !IsValid() )
	{
		return "";
	}

    xml_attribute< wchar_t >* pAttribute = ToDOMElement( m_pNode->GetXMLNode() )->first_attribute( _strAttribute.wc_str() );
	if ( pAttribute == NULL )
	{
		return "";
	}
	else
	{
        return pAttribute->value();
	}
}

void RapidXML::XMLElement::SetAttribute( const String& _strAttribute, const String& _strValue )
{
    if ( !IsValid() )
    {
        return;
    }

    xml_node< wchar_t >* pElement = ToDOMElement( m_pNode->GetXMLNode() );

    // Get document
    CoFiles::XMLDocumentPtr spDocument = GetDocument();
    xml_document< wchar_t >* pDocument = static_cast< xml_document< wchar_t >* >( spDocument->GetXMLNode() );

    xml_attribute< wchar_t >* pAttribute = pElement->first_attribute( _strAttribute.wc_str() );
    if ( pAttribute == NULL )
    {
        // Allocate memory for attribute name
        wchar_t* szName = pDocument->allocate_string( _strAttribute.wc_str() );

        // Create attribute
        pAttribute = pDocument->allocate_attribute( szName );
        pElement->append_attribute( pAttribute );
    }

    // Allocate memory for attribute string
    wchar_t* szValue = pDocument->allocate_string( _strValue.wc_str() );
    pAttribute->value( szValue );
}

void RapidXML::XMLElement::SetAttribute( const String &sAttribute, Unsigned32 u32Value )
{
	SetAttribute( sAttribute, CoFiles::String( u32Value ) );
}

void RapidXML::XMLElement::SetAttribute( const String &sAttribute, Unsigned64 _u64Value )
{
	SetAttribute( sAttribute, CoFiles::String( _u64Value ) );
}

void RapidXML::XMLElement::SetAttribute( const String &sAttribute, Integer32 i32Value )
{
	SetAttribute( sAttribute, CoFiles::String( i32Value ) );
}
			
void RapidXML::XMLElement::SetAttribute( const String& _sAttribute, Float32 _f32Value )
{
	SetAttribute( _sAttribute, CoFiles::String( _f32Value ) );
}
