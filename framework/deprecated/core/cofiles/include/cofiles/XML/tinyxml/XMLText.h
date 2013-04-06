#ifndef COFILES_TINYXML_XMLTEXT_H
#define COFILES_TINYXML_XMLTEXT_H

#include "cofiles/String.h"
#include "cofiles/XML/tinyxml/XMLNode.h"

class TiXmlText;

namespace CoFiles
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

