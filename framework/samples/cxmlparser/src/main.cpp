#include <iostream>

#include "cfoundation/XML/XMLParser.h"

using namespace std;

void printNodes( CFoundation::XMLNodePtr pNode, Unsigned32 u32Level = 0 )
{
	while ( pNode != NULL )
	{
		for ( Unsigned32 u32Idx = 0; u32Idx < u32Level; ++u32Idx ) {
			std::cout << "  ";
		}
		if ( pNode->IsElement() ) {
            CFoundation::XMLElementPtr pElement = static_pointer_cast< CFoundation::XMLElement >( pNode );
			std::cout << "ElementNode: Name = " + pElement->GetName() << std::endl;
		}
		else if ( pNode->IsText() ) {
            CFoundation::XMLTextPtr pText = static_pointer_cast< CFoundation::XMLText >( pNode );
			std::cout << "TextNode: Text = " + pText->GetText() << std::endl;
		}

		CFoundation::XMLNodePtr spChildNode = pNode->GetFirstChild();
		printNodes( spChildNode, u32Level + 1 );

		pNode = pNode->GetNextSibling();
	}
}

int main()
{
    CFoundation::XMLParser &clParser = CFoundation::XMLParser::GetInstance();

	// Parsing very simple XML
	CFoundation::String sFilename = "data/test.xml";
	std::cout << "Parsing simple XML " << sFilename << std::endl;
    CFoundation::XMLDocumentPtr spDoc = clParser.ParseFile( sFilename );
    if ( spDoc != NULL ) {
        printNodes( spDoc->GetRootElement() );
    }
    else {
        std::cout << "Parsing of file " << sFilename << " failed" << std::endl;
    }

    // Writing very simple XML
    CFoundation::String sOutFile = "data/out.xml";
    CFoundation::XMLDocumentPtr spOutDoc = clParser.CreateDocument( "Output" );
    CFoundation::XMLElementPtr spHello = spOutDoc->CreateElement( "Hello" );
    spOutDoc->InsertChildLast( spHello );
    CFoundation::XMLElementPtr spWorld = spOutDoc->CreateElement( "World" );
    spHello->InsertChildLast( spWorld );
    clParser.WriteFile( spOutDoc, sOutFile );

	// Parsing complex XML
	/*sFilename = "data/common/ctrls/styles/default/default_button_style.xml";
    if( clParser.ParseFile( sFilename ) ) {
        CFoundation::XMLDocument clDoc = clParser.GetDocument();
        if( clDoc.IsValid() ) {
            CFoundation::XMLNode clNode = clDoc.GetFirstChild();
            printNodes( clNode );
        }
        else {
            std::cout << "XMLDocument " << sFilename << " not valid." << std::endl;
        }
    }
    else {
        std::cout << "Parsing of file " << sFilename << " failed" << std::endl;
    }*/

    getchar();

    return 0;
}
