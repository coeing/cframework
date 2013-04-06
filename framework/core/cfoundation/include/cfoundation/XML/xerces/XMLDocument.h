#ifndef CFOUNDATION_XERCESC_XMLDOCUMENT_H
#define CFOUNDATION_XERCESC_XMLDOCUMENT_H

#ifdef XML_IMPL_XERCES

#include "cfoundation/XML/XMLDocument.h"

#include "cfoundation/XML/Xerces/XMLElement.h"
#include "cfoundation/XML/Xerces/XMLText.h"
#include "cfoundation/XML/Xerces/XMLWrapper.h"

namespace CFoundation
{
	namespace Xerces
	{
        class XMLNode;

        class XMLDocument : public XMLWrapper, public CFoundation::XMLDocument
		{
		public:
            //! Default constructor
			XMLDocument();

            //! Constructor
			explicit XMLDocument( const String& _sRootName );

            //! Destructor
			virtual ~XMLDocument();

            virtual void                    SetNodeType( XMLNodeType _eNodeType );
            virtual XMLNodeType             GetNodeType() const;

            virtual bool                    IsValid() const;

            virtual bool                    HasChildren() const;

            virtual CFoundation::XMLNodePtr     GetFirstChild() const;
            virtual CFoundation::XMLNodePtr     GetNextSibling() const;

            virtual void                    InsertChildFirst( CFoundation::XMLNode &clChildNode );
            virtual void                    InsertChildBefore( CFoundation::XMLNode &clChildNode, CFoundation::XMLNode &clBefore );
            virtual void                    InsertChildAfter( CFoundation::XMLNode &clChildNode, CFoundation::XMLNode &clAfter );
            virtual void                    InsertChildLast( CFoundation::XMLNode &clChildNode );

            virtual bool                    IsDocument() const;
            virtual bool                    IsElement() const;
            virtual bool                    IsText() const;

            /// Returns the document this node belongs to
            virtual XMLDocumentPtr          GetDocument();

            /// Returns the document this node belongs to
            virtual XMLDocumentCPtr         GetDocument() const;

	        virtual void*                   GetXMLNode();
	        virtual const void*             GetXMLNode() const;
	        virtual void                    SetXMLNode( void* _pNode );

			//! Returns the root element of the document
			virtual CFoundation::XMLElementPtr  GetRootElement() const;

			virtual CFoundation::XMLElementPtr  CreateElement( const CFoundation::String& _sName );
			virtual CFoundation::XMLTextPtr     CreateText( const CFoundation::String& _sText );

			void                            Refine();

		protected:
			friend class XMLParser;

			void                            Refine( CFoundation::XMLNodePtr _pNode, Unsigned32 u32Level );

        private:
            /// Node
            Xerces::XMLNode*                m_pNode;

            //! The root element
            Xerces::XMLElementPtr           m_spRootElement;
		};

        typedef SharedPtr< XMLDocument > XMLDocumentPtr;
	}
}

#endif

#endif
