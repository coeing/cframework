#ifndef CFOUNDATION_ASSERT_H
#define CFOUNDATION_ASSERT_H

#include "cfoundation/Base/Types.h"
#include "cfoundation/Text/String.h"

#if defined( _DEBUG ) || defined( _HYBRID )

	#define MAX_MSG_LENGTH	256

	#define UNREFERENCED_VAR(v)			v

	#define UNREFERENCED_PARAMETER(p)	(p)

	#ifdef WIN32

		// Windows

        namespace CFoundation
        {
            //! Shows an assertion message box
            /*! Returns if the execution should break into code.
             */
            bool HandleAssert( const String& _strTest, const String& _strMsg, const String& _strFile, const String& _strFunction, Integer32 _i32Line );

            //! Shows a warning message box
            /*! Returns if the execution should break or go on.
             *  _bCallAgain indicates if in this execution the warning should be called again.
             */
            bool HandleWarning( const String& _strTest, const String& _strMsg, const String& _strFile, const String& _strFunction, Integer32 _i32Line, bool& _bCallAgain );
        }

        #define CF_ERROR_IF( exp, msg, ... )            {   if ( ( exp ) ) \
                                                            { \
                                                                CFoundation::String sMsg; \
											                    sMsg.Format( msg, __VA_ARGS__ ); \
                                                                if ( HandleAssert( #exp, sMsg, __FILE__, __FUNCTION__, __LINE__ ) ) \
                                                                { \
                                                                    _asm { int 3 } \
                                                                } \
                                                                exit( 0 ); \
												            } \
                                                        }
        #define CF_ASSERT( exp, msg, ... )              {   CF_ERROR_IF( !( exp ), msg, __VA_ARGS__ ) }
        #define CF_ERROR( msg, ... )                    {   CF_ERROR_IF( true, msg, __VA_ARGS__ ) }

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

        #define CF_WARN_IF( exp, msg, ... ) {   if ( ( exp ) ) \
                                                { \
                                                    static bool bCallAgain = true; \
                                                    if ( bCallAgain ) \
                                                    { \
                                                        CFoundation::String sMsg; \
									                    sMsg.Format( msg, __VA_ARGS__ ); \
                                                        if ( HandleWarning( #exp, sMsg, __FILE__, __FUNCTION__, __LINE__, bCallAgain ) ) \
                                                        { \
                                                            _asm { int 3 } \
                                                        } \
                                                    } \
									            } \
									        }
        #define CF_WARN( msg, ... )         {   bool bFalse = false; \
                                                CF_WARN_IF( !bFalse, msg, __VA_ARGS__ ) \
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

    #undef UNREFERENCED_PARAMETER
	#define UNREFERENCED_PARAMETER( p )

    #define CF_ERROR_IF( exp, msg, ... )
    #define CF_ASSERT( exp, msg, ... )
    #define CF_ERROR( msg, ... )
    #define CF_WARN_IF( exp, msg, ... )
    #define CF_WARN( msg, ... )

#endif

#endif
