#ifndef COFILES_TINYXML_XMLDOCUMENT_H
#define COFILES_TINYXML_XMLDOCUMENT_H

#include "cofiles/XML/tinyxml/XMLElement.h"
#include "cofiles/XML/tinyxml/XMLText.h"

class TiXmlDocument;

namespace CoFiles
{
	namespace TinyXml
	{
		class XMLDocument : public XMLNode
		{
		public:
            //! Constructor
			XMLDocument( const String& _sRootName = "" );

            //! Destructor
			virtual ~XMLDocument();

			XMLElement*	CreateElement( const CoFiles::String &sName );
			XMLText* 	CreateText( const CoFiles::String &sText );

			void Refine();

		protected:
			friend class XMLParser;

			void Refine( XMLNode clNode, Unsigned32 u32Level );

			const TiXmlDocument* GetXMLDocument() const;
			TiXmlDocument* GetXMLDocument();
		};
	}
}

#endif
