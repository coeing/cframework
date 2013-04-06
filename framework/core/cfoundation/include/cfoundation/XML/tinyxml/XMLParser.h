#ifndef CFOUNDATION_TINYXML_XMLPARSER_H
#define CFOUNDATION_TINYXML_XMLPARSER_H

#ifdef XML_IMPL_TINYXML

#include "cfoundation/Text/String.h"
#include "cfoundation/Utils/Singleton.h"

#include "cfoundation/XML/tinyxml/XMLDocument.h"
#include "cfoundation/XML/tinyxml/XMLElement.h"
#include "cfoundation/XML/tinyxml/XMLNode.h"
#include "cfoundation/XML/tinyxml/XMLText.h"

namespace CFoundation
{
	namespace TinyXml
	{
		class XMLParser : public Singleton<XMLParser>
		{
		public:
			XMLParser();
			~XMLParser();

			bool ParseFile( const String &rFilename );
			bool ParseFile( const wchar_t *filename );

			XMLDocument GetDocument() const;
			XMLNode GetRoot() const;

			bool WriteFile( const CFoundation::TinyXml::XMLDocument &clDocument, const String &sFileName );

		private:
			mutable XMLDocument m_clDocument;
			String              m_sFilename;
		};
	}
}

#endif

#endif
