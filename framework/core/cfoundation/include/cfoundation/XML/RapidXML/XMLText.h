#ifndef CFoundation_RAPIDXML_XMLTEXT_H
#define CFoundation_RAPIDXML_XMLTEXT_H

#include "cfoundation/Text/String.h"
#include "cfoundation/XML/XMLText.h"

#include "cfoundation/XML/RapidXML/XMLNode.h"
#include "cfoundation/XML/RapidXML/XMLWrapper.h"

namespace CFoundation
{
	namespace RapidXML
    {
        class XMLText : public CFoundation::XMLText
        {
		public:
			XMLText();
        
            virtual void            SetNodeType( XMLNodeType _eNodeType );
            virtual XMLNodeType     GetNodeType() const;

			virtual bool            IsValid() const;

            virtual bool            HasChildren() const;

            /// Returns the first child
			virtual CFoundation::XMLNodePtr GetFirstChild() const;

            /// Returns the first child with the passed name
            virtual CFoundation::XMLNodePtr GetFirstChild( const String& _strName ) const;

            virtual CFoundation::XMLNodePtr GetNextSibling() const;

            virtual void            InsertChildFirst( CFoundation::XMLNodePtr _spChildNode );
            virtual void            InsertChildBefore( CFoundation::XMLNodePtr _spChildNode, CFoundation::XMLNodePtr _spBefore );
            virtual void            InsertChildAfter( CFoundation::XMLNodePtr _spChildNode, CFoundation::XMLNodePtr _spAfter );
            virtual void            InsertChildLast( CFoundation::XMLNodePtr _spChildNode );

            /// Removes the first child node
            virtual void            RemoveChildFirst();

            /// Removes the passed child node
            virtual void            RemoveChild( CFoundation::XMLNodePtr _spChildNode );

            /// Removes the last child node
            virtual void            RemoveChildLast();

            virtual bool            IsDocument() const;
            virtual bool            IsElement() const;
            virtual bool            IsText() const;

            /*
            /// Returns the document this node belongs to
            virtual XMLDocumentPtr  GetDocument();

            /// Returns the document this node belongs to
            virtual XMLDocumentCPtr GetDocument() const;*/

	        virtual void*           GetXMLNode();
	        virtual const void*     GetXMLNode() const;
	        virtual void            SetXMLNode( void* _pNode );

			virtual const String    GetText() const;
			virtual void            SetText( const String &sName );

		protected:
			friend class XMLDocument;

			XMLText( const String &sText );

        private:
            /// Node
            RapidXML::XMLNode*        m_pNode;
		};

        typedef SharedPtr< XMLText > XMLTextPtr;
	}
}

#endif

