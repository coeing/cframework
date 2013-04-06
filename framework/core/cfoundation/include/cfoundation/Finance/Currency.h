#ifndef CFOUNDATION_CURRENCY_H
#define CFOUNDATION_CURRENCY_H

namespace CFoundation
{
    enum Currency
    {
        CURRENCY_NONE,
        CURRENCY_EURO,
        CURRENCY_DOLLAR,
        CURRENCY_END
    };

    // Returns the symbol for the passed currency
    wchar_t GetCurrencySymbol( Currency _eCurrency );
}

#endif
