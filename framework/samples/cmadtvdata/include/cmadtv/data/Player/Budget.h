#ifndef CMADTV_BUDGET_H
#define CMADTV_BUDGET_H

#include "CFoundation/Finance/Account.h"
#include "CFoundation/Finance/Money.h"

namespace CFoundation
{
    class Serializer;
}

namespace CMadTV
{
    // Accounts
    enum AccountIdentifier
    {
        ACCOUNT_NONE,

        ACCOUNT_BROADCAST_PURCHASES,
        ACCOUNT_TRANSMITTERS_BOUGHT,
        ACCOUNT_SCREENPLAYS_BOUGHT,
        ACCOUNT_ACTOR_SCENE,
        ACCOUNT_LIQUIDATION_DAMAGES,
        ACCOUNT_STUDIO_RENTAL,
        ACCOUNT_NEWS_COSTS,
        ACCOUNT_TRANSMITTER_FEES,
        ACCOUNT_SUNDRY_COSTS,
        ACCOUNT_EXPENSES_END = ACCOUNT_SUNDRY_COSTS,

        ACCOUNT_BROADCAST_SALES,
        ACCOUNT_ADVERT_INCOME,
        ACCOUNT_SUNDRY_INCOME,
        ACCOUNT_INCOME_END = ACCOUNT_SUNDRY_INCOME,

        ACCOUNT_CASH,
        ACCOUNT_INTEREST,

        ACCOUNT_END
    };

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// Budget
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class Budget
    {
    public:
        /// Constructor
        Budget();

        /// Assignment operator
        void                        operator=( const Budget& _rhs );

        /// Serialization
        void                        Serialize( CFoundation::Serializer& _Serializer );

        /// Makes a book
        void                        Book( AccountIdentifier _eDebit, AccountIdentifier _eCredit, const CFoundation::Money& _clAmount, const CFoundation::DateTime& _DateTime, const CFoundation::String& _strDescription = "" );

        /// Returns the cash
        const CFoundation::Money&   GetCash() const;

        /// Returns the account with the passed identifier
        CFoundation::Account&       GetAccount( AccountIdentifier _eAccount );

        /// Adds the passed amount of credit
        void                        AddCredit( const CFoundation::Money& _Credit );

        /// Returns the credit amount
        const CFoundation::Money&   GetCredit() const;

    private:
        /// Accounts
        CFoundation::Account        m_aAccounts[ ACCOUNT_END ];

        /// Credit
        CFoundation::Money          m_Credit;
    };
}

#endif
