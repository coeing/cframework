#ifndef COFILES_XMLNODE_H
#define COFILES_XMLNODE_H

#include "cofiles/RCObject.h"
#include "cofiles/Utils/SharedPtr.h"

namespace CoFiles
{
    //! XML node types
    enum XMLNodeType 
    {
        XML_NODE_TYPE_NONE          = 0,

        ELEMENT_NODE                = 1,
        ATTRIBUTE_NODE              = 2,
        TEXT_NODE                   = 3,
        CDATA_SECTION_NODE          = 4,
        ENTITY_REFERENCE_NODE       = 5,
        ENTITY_NODE                 = 6,
        PROCESSING_INSTRUCTION_NODE = 7,
        COMMENT_NODE                = 8,
        DOCUMENT_NODE               = 9,
        DOCUMENT_TYPE_NODE          = 10,
        DOCUMENT_FRAGMENT_NODE      = 11,
        NOTATION_NODE               = 12
    };

    class XMLNode;
    typedef SharedPtr< XMLNode > XMLNodePtr;
    typedef SharedPtr< const XMLNode > XMLNodeCPtr;

    class XMLDocument;
    typedef SharedPtr< XMLDocument > XMLDocumentPtr;
    typedef SharedPtr< const XMLDocument > XMLDocumentCPtr;

    //! XML node interface
    class XMLNode : public RCObject
    {
    public:
        virtual void            SetNodeType( XMLNodeType _eNodeType ) = 0;
        virtual XMLNodeType     GetNodeType() const = 0;

        virtual bool            IsValid() const = 0;

        virtual bool            HasChildren() const = 0;

        virtual XMLNodePtr      GetFirstChild() const = 0;
        virtual XMLNodePtr      GetNextSibling() const = 0;

        virtual void            InsertChildFirst( XMLNode &clChildNode ) = 0;
        virtual void            InsertChildBefore( XMLNode &clChildNode, XMLNode &clBefore ) = 0;
        virtual void            InsertChildAfter( XMLNode &clChildNode, XMLNode &clAfter ) = 0;
        virtual void            InsertChildLast( XMLNode &clChildNode ) = 0;

        virtual bool            IsDocument() const = 0;
        virtual bool            IsElement() const = 0;
        virtual bool            IsText() const = 0;

        /// Returns the document this node belongs to
        virtual XMLDocumentPtr  GetDocument() = 0;

        /// Returns the document this node belongs to
        virtual XMLDocumentCPtr GetDocument() const = 0;

	    virtual void*           GetXMLNode() = 0;
	    virtual const void*     GetXMLNode() const = 0;
	    virtual void            SetXMLNode( void* _pNode ) = 0;
    };
}

#endif
