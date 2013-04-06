namespace CoFiles
{    
    template < class Enum >
    EnumConverter< Enum >::EnumConverter()
    {
    }

    template < class Enum >
    void EnumConverter< Enum >::Initialize()
    {
    }

    template < class Enum >
    void EnumConverter< Enum >::Assign( Enum _eValue, const String& _sValue )
    {
        EnumConverterBase::Assign( _eValue, _sValue );
    }

    template < class Enum >
    const String& EnumConverter< Enum >::Convert( Enum _eValue ) const
    {
        return EnumConverterBase::Convert( _eValue );
    }

    template < class Enum >
    Enum EnumConverter< Enum >::Convert( const CoFiles::String& _sValue ) const
    {
        return static_cast< Enum >( EnumConverterBase::Convert( _sValue ) );
    }
}
