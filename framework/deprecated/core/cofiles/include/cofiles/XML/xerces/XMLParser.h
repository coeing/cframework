#ifndef COFILES_XERCESC_XMLPARSER_H
#define COFILES_XERCESC_XMLPARSER_H

#include <vector>

#include "cofiles/String.h"
#include "cofiles/XML/XMLParser.h"

#include "cofiles/XML/Xerces/XMLDocument.h"
#include "cofiles/XML/Xerces/XMLElement.h"
#include "cofiles/XML/Xerces/XMLNode.h"
#include "cofiles/XML/Xerces/XMLText.h"

namespace CoFiles
{
	namespace Xerces
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

		protected:

		private:
            typedef std::vector< Xerces::XMLDocumentPtr > DocumentVector;

            //! Xerces Parser
			void*                               m_pParser;

            //! Parsed documents
            DocumentVector                      m_aspDocuments;
		};
	}
}

#endif
