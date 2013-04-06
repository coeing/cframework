#ifndef CFoundation_RAPIDXML_XMLELEMENT_H
#define CFoundation_RAPIDXML_XMLELEMENT_H

#include "cfoundation/Text/String.h"
#include "cfoundation/XML/XMLElement.h"

#include "cfoundation/XML/RapidXML/XMLNode.h"

namespace CFoundation
{
	namespace RapidXML
	{
        class XMLElement : public CFoundation::XMLElement
		{
		public:
            /// Constructor
			XMLElement();
            
            /// Destructor
			virtual ~XMLElement();

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

			const String            GetName() const;
			void                    SetName( const String &sName );

			//! Checks if passed attribute is set for this XML element
			bool                    HasAttribute( const String& sAttribute ) const;

			const String            GetAttribute( const String& _sAttribute ) const;
			void		            SetAttribute( const String& _sAttribute, const String& _sValue );
			void		            SetAttribute( const String& _sAttribute, Unsigned32 _u32Value );
			void		            SetAttribute( const String& _sAttribute, Unsigned64 _u64Value );
			void		            SetAttribute( const String& _sAttribute, Integer32 _i32Value );
			void		            SetAttribute( const String& _sAttribute, Float32 _f32Value );
            virtual void            SetAttribute( const String& _strAttribute, bool _bValue );

		protected:
			friend class XMLDocument;

			XMLElement( const String &sName );

        private:
            /// Node
            RapidXML::XMLNodePtr    m_pNode;
		};

        typedef SharedPtr< XMLElement > XMLElementPtr;
	}
}

#endif
