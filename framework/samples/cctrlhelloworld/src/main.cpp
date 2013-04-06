//#include <vld.h>

#include "cctrlhelloworld/TestApp.h"

int main()
{
    CTest::TestApp::GetInstance().Start();
    CTest::TestApp::DestroyInstance();

    return 0;
}
