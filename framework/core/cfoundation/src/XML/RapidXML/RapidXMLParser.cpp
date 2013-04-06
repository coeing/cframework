#include "cfoundation/XML/RapidXML/XMLParser.h"

#include <fstream>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

#include "cfoundation/Debug/Assert.h"
#include "cfoundation/Debug/Profiling.h"

using namespace CFoundation;
using namespace rapidxml;

RapidXML::XMLParser::XMLParser()
{
}

RapidXML::XMLParser::~XMLParser()
{
}

CFoundation::XMLDocumentPtr RapidXML::XMLParser::ParseFile( const CFoundation::String& _strFileName )
{
    PROFILING( "XMLParser::ParseFile" );

	// Test if file exists
    std::wifstream is( _strFileName.wc_str(), std::ios::binary );

	if( !is.is_open() )
	{
        CF_WARN( L"Could not open file %s for parsing.", _strFileName.wc_str() );
		return CFoundation::XMLDocumentPtr();
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
        CF_WARN( L"Error while parsing %s: %s (%s)", _strFileName.wc_str(), e.where< wchar_t >(), String( e.what() ).wc_str() );
        delete pDOMDocument;
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
 
	return spDocument;
}

CFoundation::XMLDocumentPtr RapidXML::XMLParser::CreateDocument()
{
    // Create document
    xml_document<>* pDOMDocument = new xml_document<>();

    XMLDocumentPtr spDocument( new XMLDocument() );    
    spDocument->SetXMLNode( pDOMDocument );
    return spDocument;
}
            
CFoundation::XMLDocumentPtr RapidXML::XMLParser::CreateDocument( const String& _strRootName )
{
    CFoundation::XMLDocumentPtr spDocument = CreateDocument();
    spDocument->InsertChildFirst( spDocument->CreateElement( _strRootName ) );
    return spDocument;
}

bool RapidXML::XMLParser::WriteFile( CFoundation::XMLDocumentCPtr _spDocument, const CFoundation::String& _strFileName )
{
	if ( _strFileName == "" )
	{
		return false;
	}

	const xml_document< wchar_t >* pDocument = static_cast< const xml_document< wchar_t >* >( _spDocument->GetXMLNode() );

    // Write to file
    std::basic_ofstream< wchar_t > os( _strFileName.wc_str() );
    if ( !os.is_open() )
    {
        CF_WARN( L"RapidXML::XMLParser::WriteFile: Couldn't open file %s.", _strFileName.wc_str() );
        return false;
    }

    os.clear();
    std::ostream_iterator< wchar_t, wchar_t > it( os );
    os << *pDocument;

    os.close();

	return true;
}
