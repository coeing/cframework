#ifndef COFILES_TINYXML_XMLELEMENT_H
#define COFILES_TINYXML_XMLELEMENT_H

#include "cofiles/String.h"
#include "cofiles/XML/tinyxml/XMLNode.h"

class TiXmlElement;

namespace CoFiles
{
	namespace TinyXml
	{
		class XMLElement : public XMLNode {

		public:
			XMLElement();
			XMLElement( const XMLNode &rhs );
			XMLElement& operator=( const XMLNode& rhs );

			const String GetName() const;
			void SetName( const String &sName );

			const String GetAttribute( const String& sAttribute ) const;
			void		 SetAttribute( const String& sAttribute, const String& sValue );
			void		 SetAttribute( const String& sAttribute, Unsigned32 u32Value );
			void		 SetAttribute( const String& sAttribute, Integer32 i32Value );

		protected:
			friend class XMLDocument;

			XMLElement( const String &sName );

			const TiXmlElement* GetXMLElement() const;
			TiXmlElement* GetXMLElement();
		};
	}
}

#endif
