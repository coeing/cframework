#ifndef CFOUNDATION_XMLELEMENT_H
#define CFOUNDATION_XMLELEMENT_H

#include "cfoundation/Text/String.h"
#include "cfoundation/XML/XMLNode.h"

namespace CFoundation
{
#define XML_ELEMENT_GETTER_FUNCTION( Type ) virtual Type    GetAttributeAs##Type( const String& _strAttribute ) const; \
                                            virtual void    GetAttributeAs##Type( const String& _strAttribute, Type& _Value ) const;

    //! XML element interface
    class XMLElement : public XMLNode
    {
    public:            
        /// Destructor
		virtual ~XMLElement() = 0 {}

        virtual bool            IsValid() const = 0;

        virtual const String    GetName() const = 0;
        virtual void            SetName( const String &sName ) = 0;

        //! Checks if passed attribute is set for this XML element
        virtual bool            HasAttribute( const String& _strAttribute ) const = 0;

        /// Returns the passed attribute as string
        virtual const String    GetAttribute( const String& _strAttribute ) const = 0;

        /// Only fills the passed parameter if the attribute exists
        virtual void            GetAttribute( const String& _strAttribute, String& _strValue ) const;

        virtual void            SetAttribute( const String& _sAttribute, const String& _sValue ) = 0;
        virtual void            SetAttribute( const String& _sAttribute, Unsigned32 _u32Value ) = 0;
        virtual void            SetAttribute( const String& _sAttribute, Unsigned64 _u64Value ) = 0;
        virtual void            SetAttribute( const String& _sAttribute, Integer32 _i32Value ) = 0;
        virtual void            SetAttribute( const String& _sAttribute, Float32 _f32Value ) = 0;
        virtual void            SetAttribute( const String& _strAttribute, bool _bValue ) = 0;

        /// Returns the passed attribute as bool
        virtual bool            GetAttributeAsBool( const String& _strAttribute ) const;

        /// Helper getter functions
        XML_ELEMENT_GETTER_FUNCTION( Unsigned8 );
        XML_ELEMENT_GETTER_FUNCTION( Unsigned16 );
        XML_ELEMENT_GETTER_FUNCTION( Unsigned32 );
        XML_ELEMENT_GETTER_FUNCTION( Float16 );
        XML_ELEMENT_GETTER_FUNCTION( Float32 );

        /// Returns the passed attribute as an enum of the passed type
        template < class Enum >
        Enum                    GetAttributeAsEnum( const String& _strAttribute ) const;
    };

#undef XML_ELEMENT_GETTER_FUNCTION

    INCLUDE_SHARED_CLASS( XMLElement );
}

#include "XMLElement.inl"

#endif
