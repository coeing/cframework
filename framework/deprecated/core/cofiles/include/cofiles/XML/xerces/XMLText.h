#ifndef COFILES_XERCESC_XMLTEXT_H
#define COFILES_XERCESC_XMLTEXT_H

#include "cofiles/String.h"
#include "cofiles/XML/XMLText.h"

#include "cofiles/XML/Xerces/XMLNode.h"
#include "cofiles/XML/Xerces/XMLWrapper.h"

namespace CoFiles
{
	namespace Xerces
    {
        class XMLText : public XMLWrapper, public CoFiles::XMLText
        {
		public:
			XMLText();
        
            virtual void            SetNodeType( XMLNodeType _eNodeType );
            virtual XMLNodeType     GetNodeType() const;

			virtual bool            IsValid() const;

            virtual bool            HasChildren() const;

            virtual CoFiles::XMLNodePtr GetFirstChild() const;
            virtual CoFiles::XMLNodePtr GetNextSibling() const;

            virtual void            InsertChildFirst( CoFiles::XMLNode &clChildNode );
            virtual void            InsertChildBefore( CoFiles::XMLNode &clChildNode, CoFiles::XMLNode &clBefore );
            virtual void            InsertChildAfter( CoFiles::XMLNode &clChildNode, CoFiles::XMLNode &clAfter );
            virtual void            InsertChildLast( CoFiles::XMLNode &clChildNode );

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

