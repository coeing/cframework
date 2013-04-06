#include "cofiles/Finance/Account.h"

using namespace CoFiles;

AccountEntry::AccountEntry( const Money& _clAmount, const String& _strDescription )
:   m_clAmount( _clAmount ),
    m_strDescription( _strDescription )
{
}

const Money& Account::GetBalance() const
{
    return m_clBalance;
}

void Account::Debit( const Money& _clAmount, const String& _strDescription )
{
    if ( _clAmount == Money( 0 ) )
    {
        return;
    }

    m_clBalance -= _clAmount;
    m_aEntries.push_back( AccountEntry( -_clAmount, _strDescription ) );

    // Dispatch event
    DispatchEvent( ACCOUNT_EVENT_BALANCE_CHANGED, m_clBalance + _clAmount, m_clBalance );
}

void Account::Credit( const Money& _clAmount, const String& _strDescription )
{
    if ( _clAmount == Money( 0 ) )
    {
        return;
    }

    m_clBalance += _clAmount;
    m_aEntries.push_back( AccountEntry( _clAmount, _strDescription ) );

    // Dispatch event
    DispatchEvent( ACCOUNT_EVENT_BALANCE_CHANGED, m_clBalance - _clAmount, m_clBalance );
}
