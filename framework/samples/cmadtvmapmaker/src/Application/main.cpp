#include "cmadtvmapmaker/Application/Application.h"

int main()
{    
    CMadTV::Application::GetInstance().Start( "data/database/map.png" );
    CMadTV::Application::DestroyInstance();

    return 0;
}
