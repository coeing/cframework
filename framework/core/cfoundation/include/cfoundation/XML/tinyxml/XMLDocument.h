#ifndef CFOUNDATION_TINYXML_XMLDOCUMENT_H
#define CFOUNDATION_TINYXML_XMLDOCUMENT_H

#ifdef XML_IMPL_TINYXML

#include "cfoundation/XML/tinyxml/XMLElement.h"
#include "cfoundation/XML/tinyxml/XMLText.h"

class TiXmlDocument;

namespace CFoundation
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

			XMLElement*	CreateElement( const CFoundation::String &sName );
			XMLText* 	CreateText( const CFoundation::String &sText );

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

#endif
