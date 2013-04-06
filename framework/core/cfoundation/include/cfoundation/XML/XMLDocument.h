#ifndef CFOUNDATION_XMLDOCUMENT_H
#define CFOUNDATION_XMLDOCUMENT_H

#include "cfoundation/Utils/SharedPtr.h"
#include "cfoundation/XML/XMLElement.h"
#include "cfoundation/XML/XMLNode.h"
#include "cfoundation/XML/XMLText.h"

namespace CFoundation
{
	class XMLDocument : public XMLNode
	{
	public:
		//! Returns the root element of the document
		virtual XMLElementPtr   GetRootElement() const = 0;

		virtual XMLElementPtr   CreateElement( const CFoundation::String& _strName = "" ) = 0;
		virtual XMLTextPtr      CreateText( const CFoundation::String& _strText = "" ) = 0;

		virtual void        Refine() = 0;
	};

    /// Document pointer
    typedef SharedPtr< XMLDocument > XMLDocumentPtr;

    /// Document const pointer
    typedef SharedPtr< const XMLDocument > XMLDocumentCPtr;
}

#endif
