#include "cfoundation/XML/XMLParser.h"

#include "cfoundation/Base/Defines.h"

//#include "cfoundation/XML/Xerces/XMLParser.h"
#include "cfoundation/XML/RapidXML/XMLParser.h"

using namespace CFoundation;

XMLParser* XMLParser::s_pParser = NULL;

XMLParser::~XMLParser()
{
}

XMLParser& XMLParser::GetInstance()
{
    if ( s_pParser == NULL )
    {
        s_pParser = new RapidXML::XMLParser();
    }
    return *s_pParser;
}
        
void XMLParser::DestroyInstance()
{
    SAFE_DELETE( s_pParser );
}

