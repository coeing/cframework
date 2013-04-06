#include "cmadtv/Data/Player/Budget.h"

#include "cfoundation/Serialization/Serializer.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// Budget
///
////////////////////////////////////////////////////////////////////////////////////////////

Budget::Budget()
{
}
       
void Budget::operator=( const Budget& _rhs )
{
    for ( Unsigned32 u32Account = 0; u32Account < ACCOUNT_END; ++u32Account )
    {
        m_aAccounts[ u32Account ] = _rhs.m_aAccounts[ u32Account ];
    }

    m_Credit = _rhs.m_Credit;
}
        
void Budget::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    _Serializer.SerializeArray< CFoundation::SerializeWrapper_Simple >( m_aAccounts, ACCOUNT_END );
    m_Credit.Serialize( _Serializer );
}
       
void Budget::Book( AccountIdentifier _eDebit, AccountIdentifier _eCredit, const CFoundation::Money& _clAmount, const CFoundation::DateTime& _DateTime, const CFoundation::String& _strDescription )
{
    if ( _eDebit >= ACCOUNT_END ||
         _eCredit >= ACCOUNT_END ||
         _clAmount == CFoundation::Money( 0 ) )
    {
        return;
    }

    m_aAccounts[ _eDebit ].Debit( _clAmount, _DateTime, _strDescription );
    m_aAccounts[ _eCredit ].Credit( _clAmount, _DateTime, _strDescription );
}

const CFoundation::Money& Budget::GetCash() const
{
    return m_aAccounts[ ACCOUNT_CASH ].GetBalance();
}
        
CFoundation::Account& Budget::GetAccount( AccountIdentifier _eAccount )
{
    return m_aAccounts[ _eAccount ];
}
        
void Budget::AddCredit( const CFoundation::Money& _Credit )
{
    m_Credit += _Credit;
}

const CFoundation::Money& Budget::GetCredit() const
{
    return m_Credit;
}
