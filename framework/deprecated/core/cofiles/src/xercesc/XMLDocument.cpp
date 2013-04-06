#include "cofiles/XML/Xerces/XMLDocument.h"

#include "cofiles/Makros.h"
#include "cofiles/XML/Xerces/XMLNode.h"

#include "xercesc/parsers/XercesDOMParser.hpp"

#include "xercesc/dom/DOM.hpp"
#include "xercesc/dom/DOMImplementation.hpp"
#include "xercesc/util/PlatformUtils.hpp"
#include "xercesc/util/XMLString.hpp"

using namespace CoFiles;

XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument* ToDOMDocument( void* pNode )
{
    return static_cast<XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument*>( pNode );
}

const XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument* ToDOMDocument( const void* pNode )
{
    return static_cast<const XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument*>( pNode );
}
			
Xerces::XMLDocument::XMLDocument()
{
    m_pNode = new Xerces::XMLNode();
    SetNodeType( DOCUMENT_NODE );
    m_spRootElement = XMLElementPtr( new XMLElement() );
}

Xerces::XMLDocument::XMLDocument( const String& _sRootName )
{
    m_pNode = new Xerces::XMLNode();
    SetNodeType( DOCUMENT_NODE );
    m_spRootElement = XMLElementPtr( new XMLElement() );

    XERCES_CPP_NAMESPACE_QUALIFIER XMLPlatformUtils::Initialize();

    XMLCh tempStr[100];
    XERCES_CPP_NAMESPACE_QUALIFIER XMLString::transcode("Core", tempStr, 99);
    XERCES_CPP_NAMESPACE_QUALIFIER DOMImplementation* impl = XERCES_CPP_NAMESPACE_QUALIFIER DOMImplementationRegistry::getDOMImplementation( tempStr );

    XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument* pDocNode = impl->createDocument( 0, XERCES_CPP_NAMESPACE_QUALIFIER XMLString::transcode( _sRootName.c_str() ), 0 );
    CO_ASSERT( pDocNode->getNodeType() == XERCES_CPP_NAMESPACE_QUALIFIER DOMNode::DOCUMENT_NODE, "Document with no document node?!" );
	
    SetXMLNode( pDocNode );
    m_spRootElement->SetXMLNode( pDocNode->getDocumentElement() );
}

Xerces::XMLDocument::~XMLDocument()
{
}
            
void Xerces::XMLDocument::SetNodeType( XMLNodeType _eNodeType )
{
    m_pNode->SetNodeType( _eNodeType );
}

XMLNodeType Xerces::XMLDocument::GetNodeType() const
{
    return m_pNode->GetNodeType();
}

bool Xerces::XMLDocument::IsValid() const
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

bool Xerces::XMLDocument::HasChildren() const
{
    return m_pNode->HasChildren();
}

XMLNodePtr Xerces::XMLDocument::GetFirstChild() const
{
    return m_pNode->GetFirstChild();
}

XMLNodePtr Xerces::XMLDocument::GetNextSibling() const
{
    return m_pNode->GetNextSibling();
}

void Xerces::XMLDocument::InsertChildFirst( CoFiles::XMLNode &clChildNode )
{
    m_pNode->InsertChildFirst( clChildNode );
}

void Xerces::XMLDocument::InsertChildBefore( CoFiles::XMLNode &clChildNode, CoFiles::XMLNode &clBefore )
{
    m_pNode->InsertChildBefore( clChildNode, clBefore );
}

void Xerces::XMLDocument::InsertChildAfter( CoFiles::XMLNode &clChildNode, CoFiles::XMLNode &clAfter )
{
    m_pNode->InsertChildAfter( clChildNode, clAfter );
}

void Xerces::XMLDocument::InsertChildLast( CoFiles::XMLNode& _clChildNode )
{
    Xerces::XMLNode& clChildNode = static_cast< Xerces::XMLNode& >( _clChildNode );
    XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument* pDOMNode = ToDOMDocument( GetXMLNode());
    XERCES_CPP_NAMESPACE_QUALIFIER DOMElement* pChildDOMNode = static_cast< XERCES_CPP_NAMESPACE_QUALIFIER DOMElement* >( clChildNode.GetXMLNode() );
    if ( pDOMNode != NULL &&
         pChildDOMNode != NULL )
    {
        try
        {
            pDOMNode->getDocumentElement()->appendChild( pChildDOMNode );
        }
        catch ( const XERCES_CPP_NAMESPACE_QUALIFIER DOMException& e )
        {
            CO_WARN( "XMLNode::InsertChildLast: %s", XERCES_CPP_NAMESPACE_QUALIFIER XMLString::transcode( e.msg ) );
        }
    }
}

bool Xerces::XMLDocument::IsDocument() const
{
    return m_pNode->IsDocument();
}

bool Xerces::XMLDocument::IsElement() const
{
    return m_pNode->IsElement();
}

bool Xerces::XMLDocument::IsText() const
{
    return m_pNode->IsText();
}
	        
void* Xerces::XMLDocument::GetXMLNode()
{
    return m_pNode->GetXMLNode();
}
	        
const void* Xerces::XMLDocument::GetXMLNode() const
{
    return m_pNode->GetXMLNode();
}
	        
void Xerces::XMLDocument::SetXMLNode( void* _pNode )
{
    XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument* pDoc = static_cast< XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument* >( _pNode );
    m_spRootElement->SetXMLNode( pDoc->getDocumentElement() );
    m_pNode->SetXMLNode( _pNode );
}

CoFiles::XMLElementPtr Xerces::XMLDocument::GetRootElement() const
{
    return m_spRootElement;
}

CoFiles::XMLElementPtr Xerces::XMLDocument::CreateElement( const CoFiles::String& _sName )
{
    // Try to create a new DOM element
    XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument* pDocument = ToDOMDocument( m_pNode->GetXMLNode() );
    XERCES_CPP_NAMESPACE_QUALIFIER DOMElement* pElement = NULL;
    try
    {
        pElement = pDocument->createElement( XERCES_CPP_NAMESPACE_QUALIFIER XMLString::transcode( _sName.c_str() ) );
    }
    catch ( const XERCES_CPP_NAMESPACE_QUALIFIER DOMException& e )
    {
        CO_WARN( "XMLDocument::CreateElement: %s", XERCES_CPP_NAMESPACE_QUALIFIER XMLString::transcode( e.msg ) );
    }

	XMLElementPtr spNewElement( new XMLElement() );
	spNewElement->SetXMLNode( pElement );

	return spNewElement;
}

CoFiles::XMLTextPtr Xerces::XMLDocument::CreateText( const CoFiles::String &sText )
{
    XMLTextPtr spNewText( new XMLText() );
	spNewText->SetXMLNode( ToDOMDocument( m_pNode->GetXMLNode() )->createTextNode( XERCES_CPP_NAMESPACE_QUALIFIER XMLString::transcode( sText.c_str() ) ) );

	return spNewText;
}

void Xerces::XMLDocument::Refine()
{
	Refine( GetFirstChild(), 0 );
}

void Xerces::XMLDocument::Refine( CoFiles::XMLNodePtr _pNode, Unsigned32 u32Level )
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
