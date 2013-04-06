#ifndef CFOUNDATION_XERCESC_XMLNODE_H
#define CFOUNDATION_XERCESC_XMLNODE_H

#ifdef XML_IMPL_XERCES

#include "cfoundation/Text/String.h"
#include "cfoundation/XML/XMLNode.h"
#include "cfoundation/XML/Xerces/XMLWrapper.h"

namespace CFoundation
{
	namespace Xerces
	{
        class XMLNode : public CFoundation::XMLNode
		{
		public:
			XMLNode();

            virtual void                SetNodeType( XMLNodeType _eNodeType );
            virtual XMLNodeType         GetNodeType() const ;

			bool                        IsValid() const;

			bool                        HasChildren() const;

			virtual CFoundation::XMLNodePtr GetFirstChild() const;
			virtual CFoundation::XMLNodePtr GetNextSibling() const;

			virtual void                InsertChildFirst( CFoundation::XMLNode &clChildNode );
			virtual void                InsertChildBefore( CFoundation::XMLNode &clChildNode, CFoundation::XMLNode &clBefore );
			virtual void                InsertChildAfter( CFoundation::XMLNode &clChildNode, CFoundation::XMLNode &clAfter );
			virtual void                InsertChildLast( CFoundation::XMLNode &clChildNode );

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
            static CFoundation::XMLNodePtr  CreateNode( XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* _pNode );

			static bool                 IsValidXMLNode( XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* _pNode );

		private:
            /// Xerces node
			XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* m_pNode;

            /// Node type
            XMLNodeType                             m_eNodeType;

            //! First child
            mutable SharedPtr< CFoundation::XMLNode >   m_spFirstChild;

            //! Next sibling
            mutable SharedPtr< CFoundation::XMLNode >   m_spNextSibling;
		};

        typedef SharedPtr< XMLNode > XMLNodePtr;
	}
}

#endif

#endif
