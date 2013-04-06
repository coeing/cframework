#ifndef COFILES_TINYXML_XMLPARSER_H
#define COFILES_TINYXML_XMLPARSER_H

#include "cofiles/Singleton.h"
#include "cofiles/String.h"

#include "cofiles/XML/tinyxml/XMLDocument.h"
#include "cofiles/XML/tinyxml/XMLElement.h"
#include "cofiles/XML/tinyxml/XMLNode.h"
#include "cofiles/XML/tinyxml/XMLText.h"

namespace CoFiles
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

			bool WriteFile( const CoFiles::TinyXml::XMLDocument &clDocument, const String &sFileName );

		private:
			mutable XMLDocument m_clDocument;
			String              m_sFilename;
		};
	}
}

#endif
