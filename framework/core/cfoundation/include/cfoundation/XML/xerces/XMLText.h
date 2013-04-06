#ifndef CFOUNDATION_XERCESC_XMLTEXT_H
#define CFOUNDATION_XERCESC_XMLTEXT_H

#ifdef XML_IMPL_XERCES

#include "cfoundation/Text/String.h"
#include "cfoundation/XML/XMLText.h"

#include "cfoundation/XML/Xerces/XMLNode.h"
#include "cfoundation/XML/Xerces/XMLWrapper.h"

namespace CFoundation
{
	namespace Xerces
    {
        class XMLText : public XMLWrapper, public CFoundation::XMLText
        {
		public:
			XMLText();
        
            virtual void            SetNodeType( XMLNodeType _eNodeType );
            virtual XMLNodeType     GetNodeType() const;

			virtual bool            IsValid() const;

            virtual bool            HasChildren() const;

            virtual CFoundation::XMLNodePtr GetFirstChild() const;
            virtual CFoundation::XMLNodePtr GetNextSibling() const;

            virtual void            InsertChildFirst( CFoundation::XMLNode &clChildNode );
            virtual void            InsertChildBefore( CFoundation::XMLNode &clChildNode, CFoundation::XMLNode &clBefore );
            virtual void            InsertChildAfter( CFoundation::XMLNode &clChildNode, CFoundation::XMLNode &clAfter );
            virtual void            InsertChildLast( CFoundation::XMLNode &clChildNode );

            virtual bool            IsDocument() const;
            virtual bool            IsElement() const;
            virtual bool            IsText() const;

            /// Returns the document this node belongs to
            virtual XMLDocumentPtr  GetDocument();

            /// Returns the document this node belongs to
            virtual XMLDocumentCPtr GetDocument() const;

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
            Xerces::XMLNode*        m_pNode;
		};

        typedef SharedPtr< XMLText > XMLTextPtr;
	}
}

#endif

#endif

