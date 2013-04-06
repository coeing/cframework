#ifndef CFoundation_RAPIDXML_XMLNODE_H
#define CFoundation_RAPIDXML_XMLNODE_H

#include "cfoundation/Text/String.h"
#include "cfoundation/XML/XMLNode.h"
#include "cfoundation/XML/RapidXML/XMLWrapper.h"

namespace CFoundation
{
	namespace RapidXML
	{
        class XMLNode : public CFoundation::XMLNode
		{
		public:
            /// Constructor
			XMLNode();

            /// Destructor
            virtual ~XMLNode();

            virtual void                SetNodeType( XMLNodeType _eNodeType );
            virtual XMLNodeType         GetNodeType() const ;

			bool                        IsValid() const;

			bool                        HasChildren() const;

            /// Returns the first child
			virtual CFoundation::XMLNodePtr GetFirstChild() const;

            /// Returns the first child with the passed name
            virtual CFoundation::XMLNodePtr GetFirstChild( const String& _strName ) const;

			virtual CFoundation::XMLNodePtr GetNextSibling() const;

			virtual void                InsertChildFirst( CFoundation::XMLNodePtr _spChildNode );
			virtual void                InsertChildBefore( CFoundation::XMLNodePtr _spChildNode, CFoundation::XMLNodePtr _spBefore );
			virtual void                InsertChildAfter( CFoundation::XMLNodePtr _spChildNode, CFoundation::XMLNodePtr _spAfter );
			virtual void                InsertChildLast( CFoundation::XMLNodePtr _spChildNode );

            /// Removes the first child node
            virtual void                RemoveChildFirst();

            /// Removes the passed child node
            virtual void                RemoveChild( CFoundation::XMLNodePtr _spChildNode );

            /// Removes the last child node
            virtual void                RemoveChildLast();

			virtual bool                IsDocument() const;
			virtual bool                IsElement() const;
			virtual bool                IsText() const;

            /*
            /// Returns the document this node belongs to
            virtual XMLDocumentPtr      GetDocument();

            /// Returns the document this node belongs to
            virtual XMLDocumentCPtr     GetDocument() const;*/

		    void*                       GetXMLNode();
		    const void*                 GetXMLNode() const;
		    void                        SetXMLNode( void* _pNode );

		protected:
			friend class XMLDocument;
			friend class XMLParser;

			void                        Clear();

            //! Creates the right node out of the passed RapidXML node
            static CFoundation::XMLNodePtr  CreateNode( rapidxml::xml_node< wchar_t >* _pNode );

			static bool                 IsValidXMLNode( rapidxml::xml_node< wchar_t >* _pNode );

		private:
            /// RapidXML node
			rapidxml::xml_node< wchar_t >*  m_pNode;

            /// Node type
            XMLNodeType                     m_eNodeType;

            //! First child
            mutable CFoundation::XMLNodePtr     m_spFirstChild;

            //! Next sibling
            mutable CFoundation::XMLNodePtr     m_spNextSibling;
		};

        typedef SharedPtr< XMLNode > XMLNodePtr;
	}
}

#endif
