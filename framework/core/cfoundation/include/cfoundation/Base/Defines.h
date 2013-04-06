#ifndef CFOUNDATION_DEFINES_H
#define CFOUNDATION_DEFINES_H

#include <limits.h>

#ifndef NULL
#define NULL		0
#endif

#define MAX_U8		UCHAR_MAX
#define MAX_U16		USHRT_MAX
#define MAX_U32		UINT_MAX
#define MAX_U64		ULONG_MAX

#define MIN_I8      SCHAR_MIN
#define MAX_I8      SCHAR_MAX
#define MIN_I16     INT_MIN
#define MAX_I16     INT_MAX
#define MIN_I32     LONG_MIN
#define MAX_I32     LONG_MAX
#define MIN_I64     LLONG_MIN
#define MAX_I64     LLONG_MAX

#define MAX_F16     3.40282e+038
#define MAX_F32     1.79769e+308

#define PI			3.14159265

#define SAFE_DELETE( _pObject )         { if ( _pObject != NULL ) { delete _pObject; _pObject = NULL; } }
#define SAFE_DELETE_ARRAY( _pArray )    { if ( _pArray != NULL ) { delete [] _pArray; _pArray = NULL; } }
#define SAFE_CALL( _pObject )           if ( _pObject != NULL ) _pObject

#endif
