#ifndef COFILES_COGLOBALS_H
#define COFILES_COGLOBALS_H

#ifdef WIN32
	#ifndef UNICODE
	#define UNICODE
	#endif
    #define NOMINMAX
	#undef XMLDocument
	#include <windows.h>
	#undef GetObject
	#undef XMLDocument
#endif

#include "cofiles/Makros.h"

#endif
