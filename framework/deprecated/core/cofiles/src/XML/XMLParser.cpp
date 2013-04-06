#include "cofiles/XML/XMLParser.h"

//#include "cofiles/XML/Xerces/XMLParser.h"
#include "cofiles/XML/RapidXML/XMLParser.h"

using namespace CoFiles;

XMLParser& XMLParser::GetInstance()
{
    static XMLParser* pInstance = NULL;
    if ( pInstance == NULL )
    {
        pInstance = new RapidXML::XMLParser();
    }
    return *pInstance;
}
