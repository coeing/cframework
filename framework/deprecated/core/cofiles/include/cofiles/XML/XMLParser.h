#ifndef COFILES_XMLPARSER_H
#define COFILES_XMLPARSER_H

#include "cofiles/XML/XMLDocument.h"

namespace CoFiles
{
    //! XML parser interface
    class XMLParser
    {
    public:
        //! Parses the passed file
		virtual XMLDocumentPtr      ParseFile( const String& _sFileName ) = 0;

        virtual XMLDocumentPtr      CreateDocument() = 0;
        virtual XMLDocumentPtr      CreateDocument( const String& _sRootName ) = 0;

		virtual bool                WriteFile( XMLDocumentCPtr _spDocument, const String& _sFileName ) = 0;

        //! Returns the parser instance
        static XMLParser&           GetInstance();
    };
}

#endif
