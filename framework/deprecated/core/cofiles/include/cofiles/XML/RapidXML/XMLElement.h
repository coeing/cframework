#ifndef COFILES_RAPIDXML_XMLELEMENT_H
#define COFILES_RAPIDXML_XMLELEMENT_H

#include "cofiles/String.h"
#include "cofiles/XML/XMLElement.h"

#include "cofiles/XML/RapidXML/XMLNode.h"

namespace CoFiles
{
	namespace RapidXML
	{
        class XMLElement : public CoFiles::XMLElement
		{
		public:
			XMLElement();

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
            RapidXML::XMLNode*        m_pNode;
		};

        typedef SharedPtr< XMLElement > XMLElementPtr;
	}
}

#endif
