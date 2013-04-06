#ifndef COFILES_XMLDOCUMENT_H
#define COFILES_XMLDOCUMENT_H

#include "cofiles/Utils/SharedPtr.h"
#include "cofiles/XML/XMLElement.h"
#include "cofiles/XML/XMLNode.h"
#include "cofiles/XML/XMLText.h"

namespace CoFiles
{
	class XMLDocument : public XMLNode
	{
	public:
		//! Returns the root element of the document
		virtual XMLElementPtr   GetRootElement() const = 0;

		virtual XMLElementPtr   CreateElement( const CoFiles::String& _strName = "" ) = 0;
		virtual XMLTextPtr      CreateText( const CoFiles::String& _strText = "" ) = 0;

		virtual void        Refine() = 0;
	};

    /// Document pointer
    typedef SharedPtr< XMLDocument > XMLDocumentPtr;

    /// Document const pointer
    typedef SharedPtr< const XMLDocument > XMLDocumentCPtr;
}

#endif
