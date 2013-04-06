#ifndef CFoundation_RAPIDXML_XMLPARSER_H
#define CFoundation_RAPIDXML_XMLPARSER_H

#include <map>

#include "cfoundation/Text/String.h"
#include "cfoundation/XML/XMLParser.h"

#include "cfoundation/XML/RapidXML/XMLDocument.h"
#include "cfoundation/XML/RapidXML/XMLElement.h"
#include "cfoundation/XML/RapidXML/XMLNode.h"
#include "cfoundation/XML/RapidXML/XMLText.h"

namespace CFoundation
{
	namespace RapidXML
	{
        class XMLParser : public CFoundation::XMLParser
		{
		public:
            /// Constructor
			XMLParser();

            /// Destructor
			virtual ~XMLParser();

            /// Parses the passed file
            virtual CFoundation::XMLDocumentPtr ParseFile( const String& _strFileName );

            /// Creates an empty document
            virtual CFoundation::XMLDocumentPtr CreateDocument();

            /// Creates an empty document with the passed root name
            virtual CFoundation::XMLDocumentPtr CreateDocument( const String& _strRootName );

            /// Writes the passed document to the passed file
            virtual bool                    WriteFile( CFoundation::XMLDocumentCPtr _spDocument, const String& _strFileName );

		private:
            typedef std::map< String, RapidXML::XMLDocumentPtr > FileDocumentMap;

            //! Parsed documents
            FileDocumentMap                 m_mapDocuments;
		};
	}
}

#endif
