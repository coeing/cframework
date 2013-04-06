#ifndef CFOUNDATION_XERCESC_XMLELEMENT_H
#define CFOUNDATION_XERCESC_XMLELEMENT_H

#ifdef XML_IMPL_XERCES

#include "cfoundation/Text/String.h"
#include "cfoundation/XML/XMLElement.h"

#include "cfoundation/XML/Xerces/XMLNode.h"

namespace CFoundation
{
	namespace Xerces
	{
        class XMLElement : public XMLWrapper, public CFoundation::XMLElement
		{
		public:
			XMLElement();

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

		protected:
			friend class XMLDocument;

			XMLElement( const String &sName );

        private:
            /// Node
            Xerces::XMLNode*        m_pNode;
		};

        typedef SharedPtr< XMLElement > XMLElementPtr;
	}
}

#endif

#endif
