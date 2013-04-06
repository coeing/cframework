#ifndef COFILES_XMLTEXT_H
#define COFILES_XMLTEXT_H

#include "cofiles/String.h"
#include "cofiles/XML/XMLNode.h"

namespace CoFiles
{
	class XMLText : public XMLNode 
    {
	public:
		virtual const String    GetText() const = 0;
		virtual void            SetText( const String &sName ) = 0;
	};

    typedef SharedPtr< XMLText > XMLTextPtr;
}

#endif
