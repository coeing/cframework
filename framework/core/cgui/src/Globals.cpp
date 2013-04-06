#include "cgui/Globals.h"

#include "cfoundation/Debug/Logger.h"
#include "cfoundation/Debug/Profiling.h"
#include "cfoundation/DateTime/Timer.h"
#include "cfoundation/XML/XMLParser.h"

#include "csystem/Core/Core.h"
#include "csystem/Input/Input.h"

#include "cgui/DragDrop/DragDropSystem.h"
#include "cgui/Font/FontLoader.h"
#include "cgui/Gui/Drawer.h"
#include "cgui/Input/Input.h"
#include "cgui/Texture/TextureLoader.h"
#include "cgui/Tooltip/TooltipSystem.h"

#include "cgui/GUI/Panel.h"
#include "cgui/GUI/Image.h"
#include "cgui/GUI/TextBox.h"

#include "cgui/Input/Input.h"

#include "cgui/Renderer/OpenGL/RendererOpenGL.h"

#include "cgui/Tooltip/TooltipSystem.h"

using namespace CGui;

#define REGISTER_OBJECT_CREATOR( Creator )  { GuiObjectCreatorPtr spCreator( new Creator() ); \
                                              CGui::GuiMain::GetInstance().RegisterObjectCreator( spCreator->GetTypeId(), spCreator ); }

bool CGui::Init( const CFoundation::String &sCaption, Unsigned32 u32Width, Unsigned32 u32Height, CFoundation::Color clBgColor )
{
    CGui::GuiMain& clGuiMain = CGui::GuiMain::GetInstance();

    REGISTER_OBJECT_CREATOR( GuiObjectCreator );
    REGISTER_OBJECT_CREATOR( PanelCreator );
    REGISTER_OBJECT_CREATOR( ImageCreator );
    REGISTER_OBJECT_CREATOR( TextBoxCreator );

	if( !clGuiMain.Init( sCaption, u32Width, u32Height, clBgColor ) )
	{
		return false;
	}

    // Set renderer
    clGuiMain.SetRenderer( RendererPtr( new RendererOpenGL() ) );

    InitProfiling();

	return true;
}
    
void CGui::Deinit()
{
    DeinitProfiling();

    CGui::GuiMain& clGuiMain = CGui::GuiMain::GetInstance();
    clGuiMain.Deinit();

    CFoundation::EnumConverter< CSystem::KeyCode >::DestroyInstance();
    CFoundation::EnumConverter< CSystem::KeyState >::DestroyInstance();
    CFoundation::EnumConverter< CSystem::MouseButton >::DestroyInstance();
    CFoundation::EnumConverter< CSystem::MouseButtonState >::DestroyInstance();

    CFoundation::EnumConverter< Alignment >::DestroyInstance();
    CFoundation::EnumConverter< AttachPoint >::DestroyInstance();
    CFoundation::EnumConverter< AttachObject >::DestroyInstance();
    CFoundation::EnumConverter< SizeMode >::DestroyInstance();
    CFoundation::EnumConverter< State >::DestroyInstance();
    CFoundation::EnumConverter< TextBox::CutType >::DestroyInstance();

    CGui::Drawer::DestroyInstance();

    CGui::DragDropSystem::DestroyInstance();
    CGui::FontLoader::DestroyInstance();
    CGui::TextureLoader::DestroyInstance();
    CGui::TooltipSystem::DestroyInstance();

    CGui::Input::DestroyInstance();
    CGui::GuiMain::DestroyInstance();

    CFoundation::XMLParser::DestroyInstance();
}

void CGui::Run()
{
    CGui::GuiMain& clGuiMain = CGui::GuiMain::GetInstance();

    static const Unsigned32 MAX_NUM_FRAMES = 30;

    // Calculate time for one frame
    Unsigned64 u64FrameMs = 1000 / MAX_NUM_FRAMES;
    Unsigned64 u64ElapsedMs = 0;

    // start render loop
	bool m_bDone = false;
    CFoundation::Timer clTimer;
	while( !m_bDone )
	{
	    clTimer.Start();

        Float32 f32TimeDifference = u64ElapsedMs / 1000.0f;

        // Update the system input
        CSystem::Input::GetInstance().Update( f32TimeDifference );

        // Check for input
        const CSystem::InputState& inputState = CSystem::Input::GetInstance().GetState();
        if ( inputState.WasPressed( CSystem::KEY_O ) &&
             inputState.IsDown( CSystem::KEY_P ) )
        {
            OutputProfiling();
        }

        // Update tooltip system
        TooltipSystem::GetInstance().Update( f32TimeDifference );

        // Update gui input
		CGui::Input::GetInstance().Update( f32TimeDifference );

		clGuiMain.Update( f32TimeDifference );
		clGuiMain.Render();

		ProfilingEndFrame();

		u64ElapsedMs = clTimer.GetElapsedMs();
		if( u64ElapsedMs < u64FrameMs )
		{
		    Unsigned64 u64SleepMs = u64FrameMs - u64ElapsedMs;
            CSystem::Core::GetInstance().Sleep( u64SleepMs );
            u64ElapsedMs = u64FrameMs;
		}
	}
}
