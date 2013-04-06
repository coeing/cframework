#ifndef CFOUNDATION_XERCESC_XMLPARSER_H
#define CFOUNDATION_XERCESC_XMLPARSER_H

#ifdef XML_IMPL_XERCES

#include <vector>

#include "cfoundation/Text/String.h"
#include "cfoundation/XML/XMLParser.h"

#include "cfoundation/XML/Xerces/XMLDocument.h"
#include "cfoundation/XML/Xerces/XMLElement.h"
#include "cfoundation/XML/Xerces/XMLNode.h"
#include "cfoundation/XML/Xerces/XMLText.h"

namespace CFoundation
{
	namespace Xerces
	{
        class XMLDocument;
        typedef SharedPtr< XMLDocument > XMLDocumentPtr;

        class XMLParser : public CFoundation::XMLParser
		{
		public:
            /// Constructor
			XMLParser();

            /// Destructor
			~XMLParser();

            /// Parses the passed file
            virtual CFoundation::XMLDocumentPtr ParseFile( const String& _strFileName );

            /// Creates an empty document
            virtual CFoundation::XMLDocumentPtr CreateDocument();

            /// Creates an empty document with the passed root name
            virtual CFoundation::XMLDocumentPtr CreateDocument( const String& _strRootName );

            /// Writes the passed document to the passed file
            virtual bool                    WriteFile( CFoundation::XMLDocumentCPtr _spDocument, const String& _strFileName );

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

#endif
