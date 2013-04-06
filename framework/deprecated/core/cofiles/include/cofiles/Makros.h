#ifndef COMAKROS_H
#define COMAKROS_H

#include "cofiles/String.h"
#include <wchar.h>

#ifdef UNREFERENCED_PARAMETER
#undef UNREFERENCED_PARAMETER
#endif

#ifdef DEBUG

	#define MAX_MSG_LENGTH	256

	#define UNREFERENCED_VAR(v)			v

	#define UNREFERENCED_PARAMETER(p)	(p)

	#ifdef WIN32

		// Windows

        namespace CoFiles
        {
            //! Shows an assertion message box
            /*! Returns if the execution should break into code.
             */
            bool HandleAssert( const String& _sTest, const String& _sMsg, const String& _sFile, Integer32 _i32Line );

            //! Shows a warning message box
            /*! Returns if the execution should break or go on.
             *  _bCallAgain indicates if in this execution the warning should be called again.
             */
            bool HandleWarning( const String& _sTest, const String& _sMsg, const String& _sFile, Integer32 _i32Line, bool& _bCallAgain );
        }

        #define CO_ERROR_IF( exp, msg, ... )           {   if ( ( exp ) ) \
                                                            { \
                                                                CoFiles::String sMsg; \
											                    sMsg.Format( msg, __VA_ARGS__ ); \
                                                                if ( HandleAssert( #exp, sMsg, __FILE__, __LINE__ ) ) \
                                                                { \
                                                                    _asm { int 3 } \
                                                                } \
                                                                exit( 0 ); \
												            } \
                                                        }
        #define CO_ASSERT( exp, msg, ... )              {   CO_ERROR_IF( !( exp ), msg, __VA_ARGS__ ) }
        #define CO_ERROR( msg, ... )                    {   CO_ERROR_IF( true, msg, __VA_ARGS__ ) }

		#define CoAssert(exp)			                {	CoAssert0( exp, L"" ); }
		#define CoAssert0(exp, msg)			            {	if ( !( exp ) ) \
												            { \
                                                                MessageBox( NULL, msg, L"Assert failed", MB_OK ); \
													            exit( 0 ); \
												            } \
                                                        }
		#define CoAssert1(exp, msg, arg1)	            {	wchar_t szTmp[ MAX_MSG_LENGTH ]; \
												            swprintf( szTmp, MAX_MSG_LENGTH, msg, arg1 ); \
												            CoAssert0( exp, szTmp ); }
		#define CoAssert2(exp, msg, arg1, arg2)         {	wchar_t szTmp[ MAX_MSG_LENGTH ]; \
												            swprintf( szTmp, MAX_MSG_LENGTH, msg, arg1, arg2 ); \
												            CoAssert0( exp, szTmp ); }
		#define CoAssert3(exp, msg, arg1, arg2, arg3)   {	wchar_t szTmp[ MAX_MSG_LENGTH ]; \
												            swprintf( szTmp, MAX_MSG_LENGTH, msg, arg1, arg2, arg3 ); \
												            CoAssert0( exp, szTmp ); }
		#define CoAssert4(exp, msg, arg1, arg2, arg3, arg4)   {	wchar_t szTmp[ MAX_MSG_LENGTH ]; \
												                swprintf( szTmp, MAX_MSG_LENGTH, msg, arg1, arg2, arg3, arg4 ); \
												                CoAssert0( exp, szTmp ); }

        #define CO_WARN_IF( exp, msg, ... ) {   if ( ( exp ) ) \
                                                { \
                                                    static bool bCallAgain = true; \
                                                    if ( bCallAgain ) \
                                                    { \
                                                        CoFiles::String sMsg; \
									                    sMsg.Format( msg, __VA_ARGS__ ); \
                                                        if ( HandleWarning( #exp, sMsg, __FILE__, __LINE__, bCallAgain ) ) \
                                                        { \
                                                            _asm { int 3 } \
                                                        } \
                                                    } \
									            } \
									        }
        #define CO_WARN( msg, ... )         {   bool bFalse = false; \
                                                CO_WARN_IF( !bFalse, msg, __VA_ARGS__ ) \
                                            }
		#define CoWarning0(msg)				{	MessageBox( NULL, msg, L"Warning", MB_OK ); }
		#define CoWarning1(msg, arg1)		{	wchar_t szTmp[ MAX_MSG_LENGTH ]; \
												swprintf( szTmp, MAX_MSG_LENGTH, msg, arg1 ); \
												CoWarning0( szTmp ); }
		#define CoWarning2(msg, arg1, arg2)	{	wchar_t szTmp[ MAX_MSG_LENGTH ]; \
												swprintf( szTmp, MAX_MSG_LENGTH, msg, arg1, arg2 ); \
												CoWarning0( szTmp ); }
	#else

		#include <wchar.h>
        #include <stdio.h>

		// Other OS
		#define CoAssert(exp)			    {	if( !(exp) ) \
												{	wprintf( L"Assert failed" ); \
													getchar(); \
													exit( 0 ); \
												} }
		#define CoAssert0(exp, msg)			{	if( !(exp) ) \
												{	wprintf( msg ); \
													getchar(); \
													exit( 0 ); \
												} }
		#define CoWarning0(msg)				                {	wprintf( msg ); }
		#define CoAssert1(exp, msg, arg1)	                {	wchar_t szTmp[ MAX_MSG_LENGTH ]; \
												                swprintf_s( szTmp, MAX_MSG_LENGTH, msg, arg1 ); \
												                CoAssert0(exp, szTmp); }
		#define CoAssert2(exp, msg, arg1, arg2)	            {	wchar_t szTmp[ MAX_MSG_LENGTH ]; \
												                swprintf_s( szTmp, MAX_MSG_LENGTH, msg, arg1, arg2 ); \
												                CoAssert0(exp, szTmp); }
		#define CoAssert3(exp, msg, arg1, arg2, arg3)       {	wchar_t szTmp[ MAX_MSG_LENGTH ]; \
												                swprintf_s( szTmp, MAX_MSG_LENGTH, msg, arg1, arg2, arg3 ); \
												                CoAssert0(exp, szTmp); }
		#define CoAssert4(exp, msg, arg1, arg2, arg3, arg4)	{	wchar_t szTmp[ MAX_MSG_LENGTH ]; \
												                swprintf_s( szTmp, MAX_MSG_LENGTH, msg, arg1, arg2, arg3, arg4 ); \
												                CoAssert0(exp, szTmp); }
		#define CoWarning1(msg, arg1)		{	wchar_t szTmp[ MAX_MSG_LENGTH ]; \
												swprintf_s( szTmp, MAX_MSG_LENGTH, msg, arg1 ); \
												CoWarning(szTmp); }
	#endif

#else

	#define UNREFERENCED_VAR( v )

	#define UNREFERENCED_PARAMETER( p )

    #define CO_ERROR_IF( exp, msg, ... )
    #define CO_ASSERT( exp, msg, ... )
    #define CO_ERROR( msg, ... )
    #define CO_WARN_IF( exp, msg, ... )
    #define CO_WARN( msg, ... )

#endif

#ifdef PROFILING

	#include "CoFiles/Profiler.h"

	// Profiling
	#define	InitProfiling
	#define StartProfiling( sName )		{ CoFiles::Profiler::GetInstance().StartProfiling( sName ); }
	#define	EndProfiling( sName )		{ CoFiles::Profiler::GetInstance().EndProfiling(); }
	#define ProfilingEndFrame()			{ CoFiles::Profiler::GetInstance().FrameEnd(); }
	#define OutputProfiling()			{ CoFiles::Profiler::GetInstance().Output(); }

#else

	// No Profiling
	#define	InitProfiling
	#define StartProfiling( sName )
	#define	EndProfiling( sName )
	#define ProfilingEndFrame()
	#define	OutputProfiling()

#endif

#endif
