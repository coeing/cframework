#include "cmadtvclient/Application/Application.h"

#ifdef WIN32
#include <windows.h>
int WINAPI WinMain( HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/ )
#else
int main()
#endif
{    
    CMadTV::ClientApplication app;
    app.Start();
    return 0;
}
