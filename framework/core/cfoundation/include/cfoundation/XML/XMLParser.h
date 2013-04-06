#ifndef CFOUNDATION_XMLPARSER_H
#define CFOUNDATION_XMLPARSER_H

#include "cfoundation/XML/XMLDocument.h"

namespace CFoundation
{
    //! XML parser interface
    class XMLParser
    {
    public:
        /// Destructor
        virtual ~XMLParser() = 0;

        //! Parses the passed file
		virtual XMLDocumentPtr      ParseFile( const String& _sFileName ) = 0;

        virtual XMLDocumentPtr      CreateDocument() = 0;
        virtual XMLDocumentPtr      CreateDocument( const String& _sRootName ) = 0;

		virtual bool                WriteFile( XMLDocumentCPtr _spDocument, const String& _sFileName ) = 0;

        //! Returns the parser instance
        static XMLParser&           GetInstance();

        /// Destroys the parser instance
        static void                 DestroyInstance();

    private:
        /// Parser instance
        static XMLParser*           s_pParser;
    };
}

#endif
