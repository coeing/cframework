#include "cofiles/XML/RapidXML/XMLText.h"

#include "rapidxml/rapidxml.hpp"

using namespace CoFiles;
using namespace rapidxml;

xml_node< wchar_t >* ToDOMText( void* pNode )
{
    return static_cast< xml_node< wchar_t >* >( pNode );
}

const xml_node< wchar_t >* ToDOMText( const void* pNode )
{
    return static_cast< const xml_node< wchar_t >* >( pNode );
}

RapidXML::XMLText::XMLText()
{
    m_pNode = new RapidXML::XMLNode();
    SetNodeType( TEXT_NODE );
}

RapidXML::XMLText::XMLText( const String& sText )
{
    m_pNode = new RapidXML::XMLNode();
    SetNodeType( TEXT_NODE );
    SetText( sText );
}
            
void RapidXML::XMLText::SetNodeType( XMLNodeType _eNodeType )
{
    m_pNode->SetNodeType( _eNodeType );
}

XMLNodeType RapidXML::XMLText::GetNodeType() const
{
    return m_pNode->GetNodeType();
}

bool RapidXML::XMLText::IsValid() const
{
    return m_pNode->IsValid();
}

bool RapidXML::XMLText::HasChildren() const
{
    return m_pNode->HasChildren();
}

XMLNodePtr RapidXML::XMLText::GetFirstChild() const
{
    return m_pNode->GetFirstChild();
}

XMLNodePtr RapidXML::XMLText::GetNextSibling() const
{
    return m_pNode->GetNextSibling();
}

void RapidXML::XMLText::InsertChildFirst( CoFiles::XMLNode &clChildNode )
{
    m_pNode->InsertChildFirst( clChildNode );
}

void RapidXML::XMLText::InsertChildBefore( CoFiles::XMLNode &clChildNode, CoFiles::XMLNode &clBefore )
{
    m_pNode->InsertChildBefore( clChildNode, clBefore );
}

void RapidXML::XMLText::InsertChildAfter( CoFiles::XMLNode &clChildNode, CoFiles::XMLNode &clAfter )
{
    m_pNode->InsertChildAfter( clChildNode, clAfter );
}

void RapidXML::XMLText::InsertChildLast( CoFiles::XMLNode &clChildNode )
{
    m_pNode->InsertChildLast( clChildNode );
}

bool RapidXML::XMLText::IsDocument() const
{
    return m_pNode->IsDocument();
}

bool RapidXML::XMLText::IsElement() const
{
    return m_pNode->IsElement();
}

bool RapidXML::XMLText::IsText() const
{
    return m_pNode->IsText();
}

XMLDocumentPtr RapidXML::XMLText::GetDocument()
{
    return m_pNode->GetDocument();
}

XMLDocumentCPtr RapidXML::XMLText::GetDocument() const
{
    return m_pNode->GetDocument();
}
	        
void* RapidXML::XMLText::GetXMLNode()
{
    return m_pNode->GetXMLNode();
}
	        
const void* RapidXML::XMLText::GetXMLNode() const
{
    return m_pNode->GetXMLNode();
}
	        
void RapidXML::XMLText::SetXMLNode( void* _pNode )
{
    m_pNode->SetXMLNode( _pNode );
}

const String RapidXML::XMLText::GetText() const
{
    String sText = ToDOMText( GetXMLNode() )->value();
    sText.Trim();
    return sText;
}

void RapidXML::XMLText::SetText( const String& _strText )
{
    ToDOMText( GetXMLNode() )->value( _strText.wc_str() );
}
