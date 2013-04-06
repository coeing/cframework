#ifndef COFILES_INTRUSIVEPTR_H
#define COFILES_INTRUSIVEPTR_H

#include <memory>

namespace CoFiles
{
	/// Define for the used shared pointer implementation
#define SharedPtr std::tr1::
    std::tr1::enable_shared_from_this
}

#endif
