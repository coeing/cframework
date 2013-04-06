#ifndef CFOUNDATION_ENUMCONVERTER_H
#define CFOUNDATION_ENUMCONVERTER_H

#include <map>

#include "cfoundation/Text/String.h"
#include "cfoundation/Utils/Singleton.h"

namespace CFoundation
{
    //! Enum converter base
    class EnumConverterBase
    {
    public:
        //! Initializes the converter
        virtual void    Initialize() = 0;

        //! Checks if all values are assigned
        void            Validate( Unsigned32 _u32LastValue ) const;

        //! Assigns an enum value to a string value
        void            Assign( Unsigned32 _u32Value, const String& _strValue );

        //! Converts an enum value to a string
        const String&   Convert( Unsigned32 _u32Value ) const;

        //! Converts a string to an enum value
        Unsigned32      Convert( const String& _strValue ) const;

    private:
        typedef std::map< Unsigned32, String > EnumStringMap;
        typedef std::map< String, Unsigned32 > StringEnumMap;

        //! The mapping from an enum to the string
        EnumStringMap   m_mapEnumString;

        //! The mapping from a string to the enum
        StringEnumMap   m_mapStringEnum;
    };

    //! Enum converter template
    template < class Enum >
    class EnumConverter : public EnumConverterBase, public Singleton< EnumConverter< Enum > >
    {
    public:
        //! Initializes the converter
        /*! At the end of initialization all enum values must 
         *  be assigned to a string.
         */
        virtual void    Initialize();

        //! Assigns an enum value to a string value
        void            Assign( Enum _eValue, const String& _sValue );

        //! Converts an enum value to a string
        const String&   Convert( Enum _eValue ) const;

        //! Converts a string to an enum value
        Enum            Convert( const CFoundation::String& _sValue ) const;

    private:
        friend class Singleton< EnumConverter< Enum > >;

        //! Constructor
        EnumConverter();
    };
}

#include "cfoundation/Enum/EnumConverter.inl"

#endif
