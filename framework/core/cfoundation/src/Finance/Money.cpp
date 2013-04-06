#include "cfoundation/Finance/Money.h"

#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/Text/String.h"

using namespace CFoundation;

Money::Money()
:   m_f32Amount( 0.0f )
{
}

Money::Money( Float32 _f32Amount, Currency _eCurrency )
:   m_f32Amount( 0.0f )
{
    SetAmount( _f32Amount, _eCurrency );
}
        
void Money::Serialize( CFoundation::Serializer& _Serializer )
{
    _Serializer.Serialize( m_f32Amount );
}

void Money::SetAmount( Float32 _f32Amount, Currency _eCurrency )
{
    m_f32Amount = _f32Amount / GetFactor( _eCurrency );
}

Float32 Money::GetAmount( Currency _eCurrency ) const
{
    return m_f32Amount * GetFactor( _eCurrency );
}
        
const Money Money::operator-() const
{
    return Money( -m_f32Amount );
}

void Money::operator+=( const Money& _clAmount )
{
    m_f32Amount += _clAmount.m_f32Amount;
}

void Money::operator-=( const Money& _clAmount )
{
    m_f32Amount -= _clAmount.m_f32Amount;
}

void Money::operator*=( Float32 _f32Factor )
{
    m_f32Amount *= _f32Factor;
}

void Money::operator/=( Float32 _f32Factor )
{
    m_f32Amount /= _f32Factor;
}
        
bool Money::operator==( const Money& _rhs ) const
{
    return m_f32Amount == _rhs.m_f32Amount;
}
        
bool Money::operator<( const Money& _rhs ) const
{
    return m_f32Amount < _rhs.m_f32Amount;
}

bool Money::operator<=( const Money& _rhs ) const
{
    return m_f32Amount <= _rhs.m_f32Amount;
}
        
bool Money::operator>( const Money& _rhs ) const
{
    return m_f32Amount > _rhs.m_f32Amount;
}

bool Money::operator>=( const Money& _rhs ) const
{
    return m_f32Amount >= _rhs.m_f32Amount;
}
       
String Money::ToString( Currency _eCurrency ) const
{
    // Get amount
    Float32 f32Amount = GetAmount( _eCurrency );

    // Get num digits

    // Check how to round
    if ( f32Amount >= 100 )
    {
        //f32Amount
    }
    return String::Build( L"%c %s", GetCurrencySymbol( _eCurrency ), String::MakeMillionString( f32Amount ) );
}
        
Float32 Money::GetFactor( Currency _eCurrency ) const
{
    Float32 f32Factor = 1.0f;
    switch ( _eCurrency )
    {
    case CURRENCY_DOLLAR:
        {
            f32Factor = 1.3027;
        }
        break;
    }
    return f32Factor;
}

const Money CFoundation::operator+( const Money& _lhs, const Money& _rhs )
{
    Money clSum( _lhs );
    clSum += _rhs;
    return clSum;
}

const Money CFoundation::operator-( const Money& _lhs, const Money& _rhs )
{
    Money clSum( _lhs );
    clSum -= _rhs;
    return clSum;
}

const Money CFoundation::operator*( const Money& _lhs, Float32 _f32Factor )
{
    Money clProduct( _lhs );
    clProduct *= _f32Factor;
    return clProduct;
}

const Money CFoundation::operator*( Float32 _f32Factor, const Money& _rhs )
{
    return _rhs * _f32Factor;
}

const Money CFoundation::operator/( const Money& _lhs, Float32 _f32Factor )
{
    Money clProduct( _lhs );
    clProduct /= _f32Factor;
    return clProduct;
}

