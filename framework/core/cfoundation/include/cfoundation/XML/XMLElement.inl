namespace CFoundation
{    
    template < class Enum >
    Enum CFoundation::XMLElement::GetAttributeAsEnum( const String& _strAttribute ) const
    {
        return CFoundation::EnumConverter< Enum >::GetInstance().Convert( GetAttribute( _strAttribute ) );
    }
}
