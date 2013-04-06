#ifndef CFoundation_INTRUSIVEPTR_H
#define CFoundation_INTRUSIVEPTR_H

#include <memory>

namespace CFoundation
{
	/// Define for the used shared pointer implementation
#define SharedPtr std::tr1::
    std::tr1::enable_shared_from_this
}

#endif
