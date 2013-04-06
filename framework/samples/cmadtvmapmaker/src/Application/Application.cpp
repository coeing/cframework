#include "cmadtvmapmaker/Application/Application.h"

#include "csystem/Core/Core.h"
#include "csystem/Files/File.h"

#include "cfoundation/DateTime/Timer.h"

#include "cgui/Globals.h"
#include "cgui/Texture/Texture.h"
#include "cgui/Texture/TextureLoader.h"

#include "cmadtv/Data/Spectators/Map.h"

using namespace CMadTV;

Application::Application()
{
}

Application::~Application()
{
}
		
void Application::Start( const CFoundation::String& _strFile )
{
    // Initialize
	Init();

    // Create density data
    CSystem::FilePtr spFile( new CSystem::File( _strFile ) );
    MapPtr spMap = CreateDensityData( _strFile );
    if ( spMap != NULL )
    {
        // Save to file
        CFoundation::Serializer serializer( CFoundation::Serializer::MODE_SAVE );
        spMap->Serialize( serializer );
        serializer.SaveFile( CSystem::File::GetDirectoryName( _strFile ) + "/" + CSystem::File::GetFileName( _strFile, false ) + ".map" );
    }

    // Deinitialize
    Deinit();
}

void Application::Exit()
{
    //m_bDone = true;
}
		
bool Application::Init()
{
    // Init gui
    if ( !CGui::Init( "CMadTV Map Maker", 0, 0 ) )
    {
        return false;
    }

    printf( "Server started...\n" );

    return true;
}

void Application::Deinit()
{
}

MapPtr Application::CreateDensityData( const CFoundation::String& _strFile )
{
    // Get texture from file
    CGui::Texture mapTexture;
    if ( !CGui::TextureLoader::GetInstance().GetTextureFromDisk( _strFile, mapTexture ) )
    {
        return MapPtr();
    }

    // Store size
    Unsigned32 u32Width = mapTexture.GetWidth();
    Unsigned32 u32Height = mapTexture.GetHeight();

    // Create density map
    Unsigned8* au8Density = new Unsigned8[ u32Width * u32Height ];
    memset( au8Density, 0, u32Width * u32Height * sizeof( Unsigned8 ) );

    // Get texture pixels
    Unsigned32* au32Pixels = new Unsigned32[ u32Width * u32Height ];
    CGui::TextureLoader::GetInstance().GetTexturePixels( mapTexture, au32Pixels );

    for ( Unsigned32 u32Y = 0; u32Y < u32Height; ++u32Y )
    {
        for ( Unsigned32 u32X = 0; u32X < u32Width; ++u32X )
        {
            // Get pixel (y coordinate is inverted)
            Unsigned32 u32Pixel = au32Pixels[ u32X + ( u32Height - u32Y ) * u32Width ];

            // Get density out of color code
            Unsigned8 u8Density = 0;
            switch ( u32Pixel )
            {
            case 0xffdc2424:
                {
                    u8Density = 3;
                }
                break;
            case 0xfff09080:
                {
                    u8Density = 2;
                }
                break;
            case 0xff808080:
                {
                    u8Density = 1;
                }
                break;
            }

            // Store density
            au8Density[ u32X + u32Y * u32Width ] = u8Density;
        }
    }

    SAFE_DELETE_ARRAY( au32Pixels );

    // Create map
    MapPtr spMap( new Map() );
    spMap->Init( u32Width, u32Height, au8Density );

    SAFE_DELETE_ARRAY( au8Density );

    return spMap;
}
         