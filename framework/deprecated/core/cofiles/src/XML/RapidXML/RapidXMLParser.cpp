#include "cofiles/XML/RapidXML/XMLParser.h"

#include <fstream>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

#include "cofiles/Makros.h"

using namespace CoFiles;
using namespace rapidxml;

RapidXML::XMLParser::XMLParser()
{
}

RapidXML::XMLParser::~XMLParser()
{
}

CoFiles::XMLDocumentPtr RapidXML::XMLParser::ParseFile( const CoFiles::String& _strFileName )
{
    StartProfiling( "XMLParser::ParseFile" );

	// Test if file exists
    std::wifstream is( _strFileName.wc_str(), std::ios::binary );

	if( !is.is_open() )
	{
        CO_WARN( L"Could not open file %s for parsing.", _strFileName.wc_str() );
        EndProfiling( "ParseFile " + _strFileName );
		return CoFiles::XMLDocumentPtr();
	}

    // Create document
    xml_document< wchar_t >* pDOMDocument = new xml_document< wchar_t >();    // character type defaults to char

    // Get length of file:
    is.seekg( 0, std::ios::end );
    Integer32 i32Length = is.tellg();
    is.seekg( 0, std::ios::beg );

    // Allocate memory:
    wchar_t* szBuffer = pDOMDocument->allocate_string( NULL, ( i32Length + 1 ) * sizeof( wchar_t ) );
    wmemset( szBuffer, 0, i32Length + 1 );

    // Read data as a block:
    is.read( szBuffer, i32Length );
    is.close();
    szBuffer[ i32Length ] = '\0';

    // Parse text
    try
    {
        pDOMDocument->parse< 0 >( szBuffer );    // 0 means default parse flags
    }
    catch ( rapidxml::parse_error e )
    {
        CO_WARN( L"Error while parsing %s: %s (%s)", _strFileName.wc_str(), e.where< wchar_t >(), String( e.what() ).wc_str() );
        return XMLDocumentPtr();
    }

    XMLDocumentPtr spDocument;

    // Check if document already created
    /*for ( DocumentVector::const_iterator it = m_aspDocuments.begin(); it != m_aspDocuments.end(); ++it )
    {
        if ( (*it)->GetXMLNode() == pDOMDocument )
        {
            spDocument = (*it);
        }
    }*/

    if ( spDocument == NULL )
    {
	    // Create new document
        spDocument = XMLDocumentPtr( new XMLDocument() );
	    spDocument->SetXMLNode( pDOMDocument );

        // Save document
        //m_aspDocuments.push_back( spDocument );
    }

    EndProfiling( "XMLParser::ParseFile" );
 
	return spDocument;
}

CoFiles::XMLDocumentPtr RapidXML::XMLParser::CreateDocument()
{
    // Create document
    xml_document<>* pDOMDocument = new xml_document<>();

    XMLDocumentPtr spDocument( new XMLDocument() );    
    spDocument->SetXMLNode( pDOMDocument );
    return spDocument;
}
            
CoFiles::XMLDocumentPtr RapidXML::XMLParser::CreateDocument( const String& _strRootName )
{
    CoFiles::XMLDocumentPtr spDocument = CreateDocument();
    spDocument->InsertChildFirst( *spDocument->CreateElement( _strRootName ) );
    return spDocument;
}

bool RapidXML::XMLParser::WriteFile( CoFiles::XMLDocumentCPtr _spDocument, const CoFiles::String& _strFileName )
{
	if ( _strFileName == "" )
	{
		return false;
	}

	const xml_document< wchar_t >* pDocument = static_cast< const xml_document< wchar_t >* >( _spDocument->GetXMLNode() );

    // Write to file
    std::basic_ofstream< wchar_t > os( _strFileName.wc_str() );
    os.clear();
    std::ostream_iterator< wchar_t, wchar_t > it( os );
    os << *pDocument;

	return true;
}
