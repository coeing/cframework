#ifndef CFOUNDATION_ACCOUNT_H
#define CFOUNDATION_ACCOUNT_H

#include <vector>

#include "cfoundation/DateTime/DateTime.h"
#include "cfoundation/Events/DelegateInvoker.h"
#include "cfoundation/Finance/Money.h"
#include "cfoundation/Text/String.h"


namespace CFoundation
{
    class Serializer;

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// AccountEntry
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    struct AccountEntry
    {
        /// Constructor
        AccountEntry( const Money& _clAmount = Money(), const DateTime& _DateTime = DateTime(), const String& _strDescription = "" );

        /// Serialization
        virtual void    Serialize( CFoundation::Serializer& _Serializer );

        /// Amount
        Money           m_clAmount;

        /// Date
        DateTime        m_DateTime;

        /// Description
        String          m_strDescription;
    };

    /// Account events
    enum AccountEvent
    {
        ACCOUNT_EVENT_BALANCE_CHANGED
    };
    TYPEDEF_DELEGATE2( AccountEvent, const Money&, const Money& );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// Account
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class Account : public Delegate2Invoker< AccountEvent, const Money&, const Money& >
    {
    public:
        DELEGATE2_INVOKER( AccountEvent, const Money&, const Money& );

        /// Assignment operator
        void                        operator=( const Account& _rhs );

        /// Serialization
        void                        Serialize( CFoundation::Serializer& _Serializer );

        /// Returns the current balance
        const Money&    GetBalance() const;

        /// Debits the passed amount of money
        void            Debit( const Money& _clAmount, const DateTime& _DateTime, const String& _strDescription = "" );

        /// Credits the passed amount of money
        void            Credit( const Money& _clAmount, const DateTime& _DateTime, const String& _strDescription = "" );

        /// Returns the entries between the two dates
        std::vector< AccountEntry > GetEntries( const DateTime& _From, const DateTime& _To ) const;

        /// Returns the result of the transactions between the two dates
        Money           ComputeResult( const DateTime& _From, const DateTime& _To ) const;

        /// Returns the balance at the specified date
        Money           ComputeBalance( const DateTime& _DateTime ) const;

    private:
        typedef std::vector< AccountEntry > EntryVec;

        /// Sets the new balance
        void            SetBalance( const Money& _Balance );

        /// Current balance
        Money           m_clBalance;

        /// Entries
        EntryVec        m_aEntries;
    };
}

#endif
