#ifndef COFILES_XERCESC_XMLDOCUMENT_H
#define COFILES_XERCESC_XMLDOCUMENT_H

#include "cofiles/XML/XMLDocument.h"

#include "cofiles/XML/Xerces/XMLElement.h"
#include "cofiles/XML/Xerces/XMLText.h"
#include "cofiles/XML/Xerces/XMLWrapper.h"

namespace CoFiles
{
	namespace Xerces
	{
        class XMLNode;

        class XMLDocument : public XMLWrapper, public CoFiles::XMLDocument
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

            virtual CoFiles::XMLNodePtr     GetFirstChild() const;
            virtual CoFiles::XMLNodePtr     GetNextSibling() const;

            virtual void                    InsertChildFirst( CoFiles::XMLNode &clChildNode );
            virtual void                    InsertChildBefore( CoFiles::XMLNode &clChildNode, CoFiles::XMLNode &clBefore );
            virtual void                    InsertChildAfter( CoFiles::XMLNode &clChildNode, CoFiles::XMLNode &clAfter );
            virtual void                    InsertChildLast( CoFiles::XMLNode &clChildNode );

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
			virtual CoFiles::XMLElementPtr  GetRootElement() const;

			virtual CoFiles::XMLElementPtr  CreateElement( const CoFiles::String& _sName );
			virtual CoFiles::XMLTextPtr     CreateText( const CoFiles::String& _sText );

			void                            Refine();

		protected:
			friend class XMLParser;

			void                            Refine( CoFiles::XMLNodePtr _pNode, Unsigned32 u32Level );

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
