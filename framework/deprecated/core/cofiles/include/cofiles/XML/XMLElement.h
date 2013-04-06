#ifndef COFILES_XMLELEMENT_H
#define COFILES_XMLELEMENT_H

#include "cofiles/String.h"
#include "cofiles/XML/XMLNode.h"

namespace CoFiles
{
    //! XML element interface
    class XMLElement : public XMLNode
    {
    public:
        virtual bool            IsValid() const = 0;

        virtual const String    GetName() const = 0;
        virtual void            SetName( const String &sName ) = 0;

        //! Checks if passed attribute is set for this XML element
        virtual bool            HasAttribute( const String& _sAttribute ) const = 0;

        virtual const String    GetAttribute( const String& _sAttribute ) const = 0;
        virtual void            SetAttribute( const String& _sAttribute, const String& _sValue ) = 0;
        virtual void            SetAttribute( const String& _sAttribute, Unsigned32 _u32Value ) = 0;
        virtual void            SetAttribute( const String& _sAttribute, Unsigned64 _u64Value ) = 0;
        virtual void            SetAttribute( const String& _sAttribute, Integer32 _i32Value ) = 0;
        virtual void            SetAttribute( const String& _sAttribute, Float32 _f32Value ) = 0;
    };

    /// XMLElement pointer
    typedef SharedPtr< XMLElement > XMLElementPtr;

    typedef SharedPtr< const XMLElement > XMLElementConstPtr;

    /// XMLElement const pointer
    typedef SharedPtr< const XMLElement > XMLElementCPtr;
}

#endif
