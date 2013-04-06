#ifndef COFILES_ACCOUNT_H
#define COFILES_ACCOUNT_H

#include <vector>

#include "cofiles/Events/DelegateInvoker.h"
#include "cofiles/Finance/Money.h"
#include "cofiles/String.h"

namespace CoFiles
{
    struct AccountEntry
    {
        /// Constructor
        AccountEntry( const Money& _clAmount = Money(), const String& _strDescription = "" );

        /// Amount
        Money       m_clAmount;

        /// Description
        String      m_strDescription;
    };

    /// Account events
    enum AccountEvent
    {
        ACCOUNT_EVENT_BALANCE_CHANGED
    };
    typedef Delegate2< const Money&, const Money& > AccountEventDelegate;
    typedef SharedPtr< AccountEventDelegate > AccountEventDelegatePtr;

    class Account : public Delegate2Invoker< AccountEvent, const Money&, const Money& >
    {
    public:
        /// Returns the current balance
        const Money&    GetBalance() const;

        /// Debits the passed amount of money
        void            Debit( const Money& _clAmount, const String& _strDescription = "" );

        /// Credits the passed amount of money
        void            Credit( const Money& _clAmount, const String& _strDescription = "" );

    private:
        typedef std::vector< AccountEntry > EntryVec;

        /// Current balance
        Money           m_clBalance;

        /// Entries
        EntryVec        m_aEntries;
    };
}

#endif
