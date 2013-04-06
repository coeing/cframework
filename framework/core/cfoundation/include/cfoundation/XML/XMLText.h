#ifndef CFOUNDATION_XMLTEXT_H
#define CFOUNDATION_XMLTEXT_H

#include "cfoundation/Text/String.h"
#include "cfoundation/XML/XMLNode.h"

namespace CFoundation
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
