#ifndef CFoundation_RAPIDXML_XMLDOCUMENT_H
#define CFoundation_RAPIDXML_XMLDOCUMENT_H

#include "cfoundation/XML/XMLDocument.h"

#include "cfoundation/XML/RapidXML/XMLElement.h"
#include "cfoundation/XML/RapidXML/XMLText.h"

namespace CFoundation
{
	namespace RapidXML
	{
        class XMLNode;
        class XMLParser;

        class XMLDocument : public CFoundation::XMLDocument
		{
		public:
            //! Destructor
			virtual ~XMLDocument();

            virtual void                    SetNodeType( XMLNodeType _eNodeType );
            virtual XMLNodeType             GetNodeType() const;

            virtual bool                    IsValid() const;

            virtual bool                    HasChildren() const;

            /// Returns the first child
			virtual CFoundation::XMLNodePtr GetFirstChild() const;

            /// Returns the first child with the passed name
            virtual CFoundation::XMLNodePtr GetFirstChild( const String& _strName ) const;

            virtual CFoundation::XMLNodePtr     GetNextSibling() const;

            virtual void                    InsertChildFirst( CFoundation::XMLNodePtr _spChildNode );
            virtual void                    InsertChildBefore( CFoundation::XMLNodePtr _spChildNode, CFoundation::XMLNodePtr _spBefore );
            virtual void                    InsertChildAfter( CFoundation::XMLNodePtr _spChildNode, CFoundation::XMLNodePtr _spAfter );
            virtual void                    InsertChildLast( CFoundation::XMLNodePtr _spChildNode );

            /// Removes the first child node
            virtual void                    RemoveChildFirst();

            /// Removes the passed child node
            virtual void                    RemoveChild( CFoundation::XMLNodePtr _spChildNode );

            /// Removes the last child node
            virtual void                    RemoveChildLast();

            virtual bool                    IsDocument() const;
            virtual bool                    IsElement() const;
            virtual bool                    IsText() const;

            /*
            /// Returns the document this node belongs to
            virtual XMLDocumentPtr          GetDocument();

            /// Returns the document this node belongs to
            virtual XMLDocumentCPtr         GetDocument() const;*/

	        virtual void*                   GetXMLNode();
	        virtual const void*             GetXMLNode() const;
	        virtual void                    SetXMLNode( void* _pNode );

			//! Returns the root element of the document
			virtual CFoundation::XMLElementPtr  GetRootElement() const;

			virtual CFoundation::XMLElementPtr  CreateElement( const CFoundation::String& _strName = "" );
			virtual CFoundation::XMLTextPtr     CreateText( const CFoundation::String& _strText = "" );

			void                            Refine();

		protected:
            //! Default constructor
			XMLDocument();

            friend class XMLNode;
            friend class XMLParser;

			void                            Refine( CFoundation::XMLNodePtr _pNode, Unsigned32 u32Level );

        private:
            /// Node
            RapidXML::XMLNodePtr            m_pNode;
		};

        typedef SharedPtr< XMLDocument > XMLDocumentPtr;
	}
}

#endif
