#include "cofiles/XML/Xerces/XMLParser.h"

#include <fstream>

#include <xercesc/parsers/XercesDOMParser.hpp>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMLSOutput.hpp>
#include <xercesc/dom/DOMLSSerializer.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>

#include "cofiles/Makros.h"

using namespace CoFiles;
XERCES_CPP_NAMESPACE_USE

XercesDOMParser* ToDOMParser( void* pParser )
{
    return static_cast<XercesDOMParser*>( pParser );
}

const XercesDOMParser* ToDOMParser( const void* pParser )
{
    return static_cast<const XercesDOMParser*>( pParser );
}

Xerces::XMLParser::XMLParser()
:	m_pParser( NULL )
{
    XMLPlatformUtils::Initialize();

	XercesDOMParser* pParser = new XercesDOMParser();
	pParser->setValidationScheme( XercesDOMParser::Val_Always );
	pParser->setDoNamespaces( true );
    pParser->setDoSchema( true );
    pParser->setValidationSchemaFullChecking( true );
    pParser->setCreateEntityReferenceNodes( true );
	pParser->setIncludeIgnorableWhitespace( false );

	m_pParser = pParser;
}

Xerces::XMLParser::~XMLParser()
{
    if ( m_pParser != NULL )
    {
        XercesDOMParser* pParser = ToDOMParser( m_pParser );
        delete pParser;
        m_pParser = NULL;
    }

    XMLPlatformUtils::Terminate();
}

CoFiles::XMLDocumentPtr Xerces::XMLParser::ParseFile( const CoFiles::String& _sFileName )
{
    StartProfiling( "XMLParser::ParseFile" );

    // Get Xerces parser
    XercesDOMParser* pParser = ToDOMParser( m_pParser );

	// Test if file exists
	std::ifstream fp( _sFileName.c_str() );

	if( !fp.is_open() )
	{
        CO_WARN( L"Could not open file %s for parsing.", _sFileName.wc_str() );
        EndProfiling( "ParseFile " + _sFileName );
		return CoFiles::XMLDocumentPtr();
	}


    bool errorsOccured = false;
    try
    {
        pParser->parse( XMLString::transcode( _sFileName.wc_str() ) );
    }
    catch ( const RuntimeException& e )
    {
        CO_WARN( L"An error occurred during parsing\n   Message: %s", CoFiles::String( XMLString::transcode(e.getMessage()) ).wc_str() );
        errorsOccured = true;
    }
    catch ( const OutOfMemoryException& )
    {
        XERCES_STD_QUALIFIER cerr << "OutOfMemoryException" << XERCES_STD_QUALIFIER endl;
        errorsOccured = true;
    }
    catch ( const XMLException& e )
    {
        CO_WARN( L"An error occurred during parsing\n   Message: %s", CoFiles::String( XMLString::transcode(e.getMessage()) ).wc_str() );
        errorsOccured = true;
    }
    catch ( const DOMException& e )
    {
        const unsigned int maxChars = 2047;
        XMLCh errText[maxChars + 1];

        XERCES_STD_QUALIFIER cerr << "\nDOM Error during parsing: '" << _sFileName.c_str() << "'\n"
             << "DOMException code is:  " << e.code << XERCES_STD_QUALIFIER endl;

        if (DOMImplementation::loadDOMExceptionMsg(e.code, errText, maxChars))
             XERCES_STD_QUALIFIER cerr << "Message is: " << XMLString::transcode(errText) << XERCES_STD_QUALIFIER endl;

        errorsOccured = true;
    }
    catch (...)
    {
        CO_WARN( L"An error occurred during parsing\n " );
        errorsOccured = true;
    }

	fp.close();

    // Get document node
    DOMDocument* pDOMDocument = pParser->getDocument();

    // Check if document already created
    XMLDocumentPtr spDocument;
    for ( DocumentVector::const_iterator it = m_aspDocuments.begin(); it != m_aspDocuments.end(); ++it )
    {
        if ( (*it)->GetXMLNode() == pDOMDocument )
        {
            spDocument = (*it);
        }
    }

    if ( spDocument == NULL )
    {
	    // Create new document
        spDocument = XMLDocumentPtr( new XMLDocument() );
	    spDocument->SetXMLNode( pDOMDocument );

        // Save document
        m_aspDocuments.push_back( spDocument );
    }

    EndProfiling( "XMLParser::ParseFile" );
 
	return spDocument;
}

CoFiles::XMLDocumentPtr Xerces::XMLParser::CreateDocument()
{
    return XMLDocumentPtr( new XMLDocument() );
}
            
CoFiles::XMLDocumentPtr Xerces::XMLParser::CreateDocument( const String& _sRootName )
{
    return XMLDocumentPtr( new XMLDocument( _sRootName ) );
}

bool Xerces::XMLParser::WriteFile( CoFiles::XMLDocumentCPtr _spDocument, const CoFiles::String &sFileName )
{
	if( sFileName == "" )
	{
		return false;
	}

    DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation( XMLString::transcode( "LS" ) );

    // Create output target
    XMLFormatTarget* myFormTarget = new LocalFileFormatTarget( sFileName.c_str() );

    // Create output
    DOMLSOutput* pOutput = ((DOMImplementationLS*)impl)->createLSOutput();
    pOutput->setByteStream( myFormTarget );

    // Create serializer
    DOMLSSerializer* theSerializer = ((DOMImplementationLS*)impl)->createLSSerializer();
    theSerializer->getDomConfig()->setParameter( XMLString::transcode( "format-pretty-print" ), true);

    // get the DOM representation    
	const DOMDocument* doc = static_cast< const DOMDocument* >( _spDocument->GetXMLNode() );

    try
    {
        //
        // do the serialization through DOMWriter::writeNode();
        //
        theSerializer->write( doc, pOutput );
    }
    catch ( const DOMException& e )
    {
        CO_WARN( "XMLNode::WriteFile: %s", XMLString::transcode( e.msg ) );
    }

	delete myFormTarget;
	theSerializer->release();
    pOutput->release();

	return true;
}
