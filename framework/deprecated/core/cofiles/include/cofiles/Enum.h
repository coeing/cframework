#ifndef COFILES_ENUM_H
#define COFILES_ENUM_H

template< class Enum >
Enum& enum_increment( Enum& _eValue, Enum _eBegin, Enum _eEnd )
{
    return _eValue = ( _eValue == _eEnd ) ? _eBegin : static_cast< Enum >( _eValue + 1 );
}

template< class Enum >
Enum& enum_decrement( Enum& _eValue, Enum _eBegin, Enum _eEnd )
{
    return _eValue = ( _eValue == _eBegin ) ? _eEnd : static_cast< Enum >( _eValue - 1 );
}

#endif
