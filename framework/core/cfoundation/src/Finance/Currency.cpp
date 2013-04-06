#include "cfoundation/Finance/Currency.h"

using namespace CFoundation;

wchar_t CFoundation::GetCurrencySymbol( Currency _eCurrency )
{
    switch ( _eCurrency )
    {
    case CURRENCY_EURO:
        {
            return L'€';
        }
        break;
    case CURRENCY_DOLLAR:
        {
            return L'$';
        }
        break;
    default:
        {
            return L'?';
        }
        break;
    }
}
