#ifndef COFILES_RAPIDXML_XMLPARSER_H
#define COFILES_RAPIDXML_XMLPARSER_H

#include <map>

#include "cofiles/String.h"
#include "cofiles/XML/XMLParser.h"

#include "cofiles/XML/RapidXML/XMLDocument.h"
#include "cofiles/XML/RapidXML/XMLElement.h"
#include "cofiles/XML/RapidXML/XMLNode.h"
#include "cofiles/XML/RapidXML/XMLText.h"

namespace CoFiles
{
	namespace RapidXML
	{
        class XMLParser : public CoFiles::XMLParser
		{
		public:
            /// Constructor
			XMLParser();

            /// Destructor
			~XMLParser();

            /// Parses the passed file
            virtual CoFiles::XMLDocumentPtr ParseFile( const String& _strFileName );

            /// Creates an empty document
            virtual CoFiles::XMLDocumentPtr CreateDocument();

            /// Creates an empty document with the passed root name
            virtual CoFiles::XMLDocumentPtr CreateDocument( const String& _strRootName );

            /// Writes the passed document to the passed file
            virtual bool                    WriteFile( CoFiles::XMLDocumentCPtr _spDocument, const String& _strFileName );

		private:
            typedef std::map< String, RapidXML::XMLDocumentPtr > FileDocumentMap;

            //! Parsed documents
            FileDocumentMap                 m_mapDocuments;
		};
	}
}

#endif
