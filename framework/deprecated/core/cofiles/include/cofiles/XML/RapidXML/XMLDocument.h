#ifndef COFILES_RAPIDXML_XMLDOCUMENT_H
#define COFILES_RAPIDXML_XMLDOCUMENT_H

#include "cofiles/XML/XMLDocument.h"

#include "cofiles/XML/RapidXML/XMLElement.h"
#include "cofiles/XML/RapidXML/XMLText.h"

namespace CoFiles
{
	namespace RapidXML
	{
        class XMLNode;
        class XMLParser;

        class XMLDocument : public CoFiles::XMLDocument
		{
		public:
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

			virtual CoFiles::XMLElementPtr  CreateElement( const CoFiles::String& _strName = "" );
			virtual CoFiles::XMLTextPtr     CreateText( const CoFiles::String& _strText = "" );

			void                            Refine();

		protected:
            //! Default constructor
			XMLDocument();

            friend class XMLNode;
            friend class XMLParser;

			void                            Refine( CoFiles::XMLNodePtr _pNode, Unsigned32 u32Level );

        private:
            /// Node
            RapidXML::XMLNode*              m_pNode;
		};

        typedef SharedPtr< XMLDocument > XMLDocumentPtr;
	}
}

#endif
