#include "csystem/Core/Core.h"

#ifdef WIN32
#include <windows.h>
#endif

using namespace CSystem;

/////////////////////////////////////////////////
/// Core
/////////////////////////////////////////////////

Core::Core()
{
}

Core::~Core()
{
}

void Core::Sleep( Unsigned64 _u64Milliseconds )
{
#ifdef WIN32
    ::Sleep( static_cast< DWORD >( _u64Milliseconds ) );
#endif
}
        
Unsigned64 Core::GetTickCount()
{
#ifdef WIN32
    return ::GetTickCount();
#endif
}
