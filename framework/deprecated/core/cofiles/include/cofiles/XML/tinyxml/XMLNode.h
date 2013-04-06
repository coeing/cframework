#ifndef COFILES_TINYXML_XMLNODE_H
#define COFILES_TINYXML_XMLNODE_H

#include "cofiles/String.h"

class TiXmlNode;

namespace CoFiles
{
	namespace TinyXml
	{
		class XMLParser;

		class XMLNode
		{
		public:
			XMLNode();
			XMLNode( const XMLNode& rhs );

			XMLNode& operator=( const XMLNode& rhs );

			bool IsValid() const;

			bool HasChildren() const;

			XMLNode GetFirstChild();
			const XMLNode GetFirstChild() const;
			const XMLNode GetNextSibling() const;

			virtual void InsertChildFirst( XMLNode &clChildNode );
			virtual void InsertChildBefore( XMLNode &clChildNode, XMLNode &clBefore );
			virtual void InsertChildAfter( XMLNode &clChildNode, XMLNode &clAfter );
			virtual void InsertChildLast( XMLNode &clChildNode );

			virtual bool IsDocument() const;
			virtual bool IsElement() const;
			virtual bool IsText() const;

		protected:
			friend class XMLDocument;
			friend class XMLParser;

			void Clear();

			const TiXmlNode* GetXMLNode() const;
			TiXmlNode* GetXMLNode();
			void SetXMLNode( TiXmlNode *pNode );

		private:
			TiXmlNode               *m_pNode;
		};
	}
}

#endif
