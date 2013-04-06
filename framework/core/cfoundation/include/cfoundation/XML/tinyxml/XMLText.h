#ifndef CFOUNDATION_TINYXML_XMLTEXT_H
#define CFOUNDATION_TINYXML_XMLTEXT_H

#ifdef XML_IMPL_TINYXML

#include "cfoundation/Text/String.h"
#include "cfoundation/XML/tinyxml/XMLNode.h"

class TiXmlText;

namespace CFoundation
{
	namespace TinyXml
	{
		class XMLText : public XMLNode {

		public:
			XMLText();
			XMLText( const XMLNode &rhs );
			XMLText& operator=( const XMLNode& rhs );

			const String GetText() const;
			void SetText( const String &sName );

		protected:
			friend class XMLDocument;

			XMLText( const String &sText );

			const TiXmlText* GetXMLText() const;
			TiXmlText* GetXMLText();
		};
	}
}

#endif

#endif
