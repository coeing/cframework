#ifndef COFILES_XERCESC_XMLNODE_H
#define COFILES_XERCESC_XMLNODE_H

#include "cofiles/String.h"
#include "cofiles/XML/XMLNode.h"
#include "cofiles/XML/Xerces/XMLWrapper.h"

namespace CoFiles
{
	namespace Xerces
	{
        class XMLNode : public CoFiles::XMLNode
		{
		public:
			XMLNode();

            virtual void                SetNodeType( XMLNodeType _eNodeType );
            virtual XMLNodeType         GetNodeType() const ;

			bool                        IsValid() const;

			bool                        HasChildren() const;

			virtual CoFiles::XMLNodePtr GetFirstChild() const;
			virtual CoFiles::XMLNodePtr GetNextSibling() const;

			virtual void                InsertChildFirst( CoFiles::XMLNode &clChildNode );
			virtual void                InsertChildBefore( CoFiles::XMLNode &clChildNode, CoFiles::XMLNode &clBefore );
			virtual void                InsertChildAfter( CoFiles::XMLNode &clChildNode, CoFiles::XMLNode &clAfter );
			virtual void                InsertChildLast( CoFiles::XMLNode &clChildNode );

			virtual bool                IsDocument() const;
			virtual bool                IsElement() const;
			virtual bool                IsText() const;

            /// Returns the document this node belongs to
            virtual XMLDocumentPtr      GetDocument();

            /// Returns the document this node belongs to
            virtual XMLDocumentCPtr     GetDocument() const;

		    void*                       GetXMLNode();
		    const void*                 GetXMLNode() const;
		    void                        SetXMLNode( void* _pNode );

		protected:
			friend class XMLDocument;
			friend class XMLParser;

			void                        Clear();

            //! Creates the right node out of the passed Xerces node
            static CoFiles::XMLNodePtr  CreateNode( XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* _pNode );

			static bool                 IsValidXMLNode( XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* _pNode );

		private:
            /// Xerces node
			XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* m_pNode;

            /// Node type
            XMLNodeType                             m_eNodeType;

            //! First child
            mutable SharedPtr< CoFiles::XMLNode >   m_spFirstChild;

            //! Next sibling
            mutable SharedPtr< CoFiles::XMLNode >   m_spNextSibling;
		};

        typedef SharedPtr< XMLNode > XMLNodePtr;
	}
}

#endif
