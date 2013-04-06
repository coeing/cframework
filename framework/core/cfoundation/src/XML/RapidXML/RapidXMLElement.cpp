#include "cfoundation/XML/RapidXML/XMLElement.h"

#include "rapidxml/rapidxml.hpp"

#include "cfoundation/Base/Defines.h"
#include "cfoundation/XML/RapidXML/XMLDocument.h"
#include "cfoundation/XML/RapidXML/XMLNode.h"

#include "utf8.h"
#include <vector>

using namespace CFoundation;
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
    m_pNode = RapidXML::XMLNodePtr( new RapidXML::XMLNode() );
    SetNodeType( ELEMENT_NODE );
}

RapidXML::XMLElement::XMLElement( const String& sName )
{
    m_pNode = RapidXML::XMLNodePtr( new RapidXML::XMLNode() );
    SetNodeType( ELEMENT_NODE );
    SetName( sName );
}

RapidXML::XMLElement::~XMLElement()
{
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

XMLNodePtr RapidXML::XMLElement::GetFirstChild( const String& _strName ) const
{
    return m_pNode->GetFirstChild( _strName );
}

XMLNodePtr RapidXML::XMLElement::GetNextSibling() const
{
    return m_pNode->GetNextSibling();
}

void RapidXML::XMLElement::InsertChildFirst( CFoundation::XMLNodePtr _spChildNode )
{
    m_pNode->InsertChildFirst( _spChildNode );
}

void RapidXML::XMLElement::InsertChildBefore( CFoundation::XMLNodePtr _spChildNode, CFoundation::XMLNodePtr _spBefore )
{
    m_pNode->InsertChildBefore( _spChildNode, _spBefore );
}

void RapidXML::XMLElement::InsertChildAfter( CFoundation::XMLNodePtr _spChildNode, CFoundation::XMLNodePtr _spAfter )
{
    m_pNode->InsertChildAfter( _spChildNode, _spAfter );
}

void RapidXML::XMLElement::InsertChildLast( CFoundation::XMLNodePtr _spChildNode )
{
    m_pNode->InsertChildLast( _spChildNode );
}

void RapidXML::XMLElement::RemoveChildFirst()
{
    m_pNode->RemoveChildFirst();
}

void RapidXML::XMLElement::RemoveChild( CFoundation::XMLNodePtr _spChildNode )
{
    m_pNode->RemoveChild( _spChildNode );
}

void RapidXML::XMLElement::RemoveChildLast()
{
    m_pNode->RemoveChildLast();
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

/*
XMLDocumentPtr RapidXML::XMLElement::GetDocument()
{
    return m_pNode->GetDocument();
}

XMLDocumentCPtr RapidXML::XMLElement::GetDocument() const
{
    return m_pNode->GetDocument();
}*/
	        
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
        String strAttributeUTF8( pAttribute->value() );

        // Convert UTF-8 attribute to UTF-16
        std::vector <unsigned short> utf16result;
        utf8::utf8to16( strAttributeUTF8.c_str(), strAttributeUTF8.c_str() + strAttributeUTF8.GetLength(), std::back_inserter( utf16result ) );
        utf16result.push_back( 0 );

        // Return UTF-16 attribute
        String strAttributeUTF16( (wchar_t*)( &utf16result[ 0 ] ) );
        return strAttributeUTF16;
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
    xml_document< wchar_t >* pDocument = pElement->document();

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
	SetAttribute( sAttribute, CFoundation::String( u32Value ) );
}

void RapidXML::XMLElement::SetAttribute( const String &sAttribute, Unsigned64 _u64Value )
{
	SetAttribute( sAttribute, CFoundation::String( _u64Value ) );
}

void RapidXML::XMLElement::SetAttribute( const String &sAttribute, Integer32 i32Value )
{
	SetAttribute( sAttribute, CFoundation::String( i32Value ) );
}
			
void RapidXML::XMLElement::SetAttribute( const String& _sAttribute, Float32 _f32Value )
{
	SetAttribute( _sAttribute, CFoundation::String( _f32Value ) );
}
            
void RapidXML::XMLElement::SetAttribute( const String& _strAttribute, bool _bValue )
{
    SetAttribute( _strAttribute, CFoundation::String( _bValue ? "true" : "false" ) );
}
