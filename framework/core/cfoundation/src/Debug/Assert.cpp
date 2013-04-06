#include "cfoundation/Debug/Assert.h"

#include "cfoundation/Text/String.h"

using namespace CFoundation;

#if defined( _DEBUG ) || defined( _HYBRID )

#ifdef WIN32

#include <windows.h>

bool CFoundation::HandleAssert( const String& _sTest, const String& _sMsg, const String& _sFile, const String& _strFunction, Integer32 _i32Line )
{
    CFoundation::String sText;
    sText.Format( L"FAILED: %s\r\n"
                  L"Error: %s\r\n"
                  L"File '%s', Line %d, Function '%s'\r\n\r\n"
                  L"Break into code with Retry, or just exit application with Cancel?",
                  _sTest.wc_str(), _sMsg.wc_str(), _sFile.wc_str(), _i32Line, _strFunction.wc_str() );
    Integer32 i32Result = MessageBox( NULL, sText.wc_str(), L"Assertion failed", MB_RETRYCANCEL );
    switch ( i32Result )
    {
    case IDRETRY:
        return true;
    default:
        return false;
    }
}

bool CFoundation::HandleWarning( const String& _sTest, const String& _sMsg, const String& _sFile, const String& _strFunction, Integer32 _i32Line, bool& _bCallAgain )
{
    CFoundation::String sText;
    sText.Format( L"FAILED: %s\r\n"
                  L"Warning: %s\r\n"
                  L"File '%s', Line %d, Function '%s'\r\n\r\n"
                  L"Go on with Abort, Break into code with Retry, or Ignore in future?",
                  _sTest.wc_str(), _sMsg.wc_str(), _sFile.wc_str(), _i32Line, _strFunction.wc_str() );
    Integer32 i32Result = MessageBox( NULL, sText.wc_str(), L"Warning", MB_ABORTRETRYIGNORE );
    switch ( i32Result )
    {
    case IDIGNORE:
        _bCallAgain = false;
        return false;
    case IDRETRY:
        return true;
    default:
        return false;
    }
}
#endif
#endif
