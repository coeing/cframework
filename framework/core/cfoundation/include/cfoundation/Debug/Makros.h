#ifndef CFOUNDATION_MAKROS_H
#define CFOUNDATION_MAKROS_H

#if defined( _DEBUG ) || defined( _HYBRID )

	#define UNREFERENCED_VAR( v )			v

	#define UNREFERENCED_PARAMETER( p )	    (p)

#else

	#define UNREFERENCED_VAR( v )

	#define UNREFERENCED_PARAMETER( p )

#endif

#endif
