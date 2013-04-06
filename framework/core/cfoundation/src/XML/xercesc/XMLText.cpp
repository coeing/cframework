#include "cfoundation/XML/Xerces/XMLText.h"

#ifdef XML_IMPL_XERCES

#include <xercesc/dom/DOMText.hpp>
#include <xercesc/util/XMLString.hpp>

using namespace CFoundation;
XERCES_CPP_NAMESPACE_USE

DOMText* ToDOMText( void* pNode )
{
    return static_cast<DOMText*>( pNode );
}

const DOMText* ToDOMText( const void* pNode )
{
    return static_cast<const DOMText*>( pNode );
}

Xerces::XMLText::XMLText()
{
    m_pNode = new Xerces::XMLNode();
    SetNodeType( TEXT_NODE );
}

Xerces::XMLText::XMLText( const String& sText )
{
    m_pNode = new Xerces::XMLNode();
    SetNodeType( TEXT_NODE );
    SetText( sText );
}
            
void Xerces::XMLText::SetNodeType( XMLNodeType _eNodeType )
{
    m_pNode->SetNodeType( _eNodeType );
}

XMLNodeType Xerces::XMLText::GetNodeType() const
{
    return m_pNode->GetNodeType();
}

bool Xerces::XMLText::IsValid() const
{
    return m_pNode->IsValid();
}

bool Xerces::XMLText::HasChildren() const
{
    return m_pNode->HasChildren();
}

XMLNodePtr Xerces::XMLText::GetFirstChild() const
{
    return m_pNode->GetFirstChild();
}

XMLNodePtr Xerces::XMLText::GetNextSibling() const
{
    return m_pNode->GetNextSibling();
}

void Xerces::XMLText::InsertChildFirst( CFoundation::XMLNode &clChildNode )
{
    m_pNode->InsertChildFirst( clChildNode );
}

void Xerces::XMLText::InsertChildBefore( CFoundation::XMLNode &clChildNode, CFoundation::XMLNode &clBefore )
{
    m_pNode->InsertChildBefore( clChildNode, clBefore );
}

void Xerces::XMLText::InsertChildAfter( CFoundation::XMLNode &clChildNode, CFoundation::XMLNode &clAfter )
{
    m_pNode->InsertChildAfter( clChildNode, clAfter );
}

void Xerces::XMLText::InsertChildLast( CFoundation::XMLNode &clChildNode )
{
    m_pNode->InsertChildLast( clChildNode );
}

bool Xerces::XMLText::IsDocument() const
{
    return m_pNode->IsDocument();
}

bool Xerces::XMLText::IsElement() const
{
    return m_pNode->IsElement();
}

bool Xerces::XMLText::IsText() const
{
    return m_pNode->IsText();
}
	        
void* Xerces::XMLText::GetXMLNode()
{
    return m_pNode->GetXMLNode();
}
	        
const void* Xerces::XMLText::GetXMLNode() const
{
    return m_pNode->GetXMLNode();
}
	        
void Xerces::XMLText::SetXMLNode( void* _pNode )
{
    m_pNode->SetXMLNode( _pNode );
}

const String Xerces::XMLText::GetText() const
{
    String sText = XMLString::transcode( ToDOMText( GetXMLNode() )->getData() );
    sText.Trim();
    return sText;
}

void Xerces::XMLText::SetText( const String &sText )
{
    ToDOMText( GetXMLNode() )->setData( XMLString::transcode( sText.c_str() ) );
}

#endif
