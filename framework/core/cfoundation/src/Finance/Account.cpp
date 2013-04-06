#include "cfoundation/Finance/Account.h"

#include "cfoundation/Serialization/Serializer.h"

using namespace CFoundation;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// AccountEntry
///
////////////////////////////////////////////////////////////////////////////////////////////

AccountEntry::AccountEntry( const Money& _clAmount, const DateTime& _DateTime, const String& _strDescription )
:   m_clAmount( _clAmount ),
    m_DateTime( _DateTime ),    
    m_strDescription( _strDescription )
{
}
        
void AccountEntry::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    m_clAmount.Serialize( _Serializer );
    m_DateTime.Serialize( _Serializer );
    _Serializer.Serialize( m_strDescription );
}

////////////////////////////////////////////////////////////////////////////////////////////
///
/// Account
///
////////////////////////////////////////////////////////////////////////////////////////////
        
void Account::operator=( const Account& _rhs )
{
    SetBalance( _rhs.m_clBalance );
    m_aEntries.resize( _rhs.m_aEntries.size() );
    for ( Unsigned32 u32Idx = 0; u32Idx < _rhs.m_aEntries.size(); ++u32Idx )
    {
        m_aEntries[ u32Idx ] = _rhs.m_aEntries[ u32Idx ];
    }
}
        
void Account::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    m_clBalance.Serialize( _Serializer );
    _Serializer.SerializeVector< SerializeWrapper_Simple >( m_aEntries );
}

const Money& Account::GetBalance() const
{
    return m_clBalance;
}

void Account::Debit( const Money& _clAmount, const DateTime& _DateTime, const String& _strDescription )
{
    if ( _clAmount == Money( 0 ) )
    {
        return;
    }

    m_aEntries.push_back( AccountEntry( -_clAmount, _DateTime, _strDescription ) );

    // Set new balance
    SetBalance( m_clBalance - _clAmount );
}

void Account::Credit( const Money& _clAmount, const DateTime& _DateTime, const String& _strDescription )
{
    if ( _clAmount == Money( 0 ) )
    {
        return;
    }

    m_aEntries.push_back( AccountEntry( _clAmount, _DateTime, _strDescription ) );

    // Set new balance
    SetBalance( m_clBalance + _clAmount );
}
       
std::vector< AccountEntry > Account::GetEntries( const DateTime& _From, const DateTime& _To ) const
{
    EntryVec aEntries;
    Unsigned32 u32NumEntries = m_aEntries.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumEntries; ++u32Idx )
    {
        const AccountEntry& entry = m_aEntries[ u32Idx ];
        if ( entry.m_DateTime >= _From &&
             entry.m_DateTime < _To )
        {
            aEntries.push_back( entry );
        }
    }
    return aEntries;
}
        
Money Account::ComputeResult( const DateTime& _From, const DateTime& _To ) const
{
    // Get entries in the passed period
    EntryVec aEntries = GetEntries( _From, _To );

    // Compute result
    Money result;
    Unsigned32 u32NumEntries = aEntries.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumEntries; ++u32Idx )
    {
        const AccountEntry& entry = aEntries[ u32Idx ];
        result += entry.m_clAmount;
    }
    return result;
}
        
Money Account::ComputeBalance( const DateTime& _DateTime ) const
{
    // Subtract all entries after the passed date
    Money balance = m_clBalance;
    Unsigned32 u32NumEntries = m_aEntries.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumEntries; ++u32Idx )
    {
        const AccountEntry& entry = m_aEntries[ u32Idx ];
        if ( entry.m_DateTime > _DateTime )
        {
            balance -= entry.m_clAmount;
        }
    }
    return balance;
}
       
void Account::SetBalance( const Money& _Balance )
{
    if ( _Balance == m_clBalance )
    {
        return;
    }

    Money oldBalance = m_clBalance;
    m_clBalance = _Balance;

    // Dispatch event
    DispatchEvent( ACCOUNT_EVENT_BALANCE_CHANGED, oldBalance, m_clBalance );
}
