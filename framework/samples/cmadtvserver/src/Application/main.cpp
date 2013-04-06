#include "cmadtvserver/Application/Application.h"

int main()
{    
    CMadTV::ServerApplication::GetInstance().Start();
    CMadTV::ServerApplication::DestroyInstance();

    return 0;
}
