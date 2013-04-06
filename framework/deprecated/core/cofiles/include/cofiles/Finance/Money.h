#ifndef COFILES_MONEY_H
#define COFILES_MONEY_H

#include "cofiles/Types.h"
#include "cofiles/Finance/Currency.h"

namespace CoFiles
{
    class Money
    {
    public:
        /// Default constructor
        Money();

        /// Constructor
        explicit Money( Float32 _f32Amount, Currency _eCurrency = CURRENCY_EURO );

        /// Sets the amount in the passed currency
        void            SetAmount( Float32 _f32Amount, Currency _eCurrency = CURRENCY_EURO );

        /// Returns the amount in the passed currency
        Float32         GetAmount( Currency _eCurrency = CURRENCY_EURO ) const;

        /// Negates the amount
        const Money     operator-() const;

        /// Adds the passed amount
        void            operator+=( const Money& _clAmount );

        /// Subtracts the passed amount
        void            operator-=( const Money& _clAmount );

        /// Multiplies the amount by the passed factor
        void            operator*=( Float32 _f32Factor );

        /// Divides the amount by the passed factor
        void            operator/=( Float32 _f32Factor );

        /// Checks if the passed amount is equal
        bool            operator==( const Money& _rhs ) const;

        /// Checks if the passed amount is bigger
        bool            operator<( const Money& _rhs ) const;

        /// Checks if the passed amount is less
        bool            operator>( const Money& _rhs ) const;

    private:
        /// Returns the factor to convert from euro to the passed currency
        Float32         GetFactor( Currency _eCurrency ) const;

        /// Amount (in Euro)
        Float32         m_f32Amount;
    };

    /// Adds the two amounts of money together
    const Money operator+( const Money& _lhs, const Money& _rhs );

    /// Subtracts the two amounts of money
    const Money operator-( const Money& _lhs, const Money& _rhs );

    /// Multiplies the money by the passed amount
    const Money operator*( const Money& _lhs, Float32 _f32Factor );

    /// Divides the money by the passed amount
    const Money operator/( const Money& _lhs, Float32 _f32Factor );
}

#endif
