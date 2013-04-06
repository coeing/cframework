#include "cctrl/App.h"

#include "cfoundation/Debug/Assert.h"
#include "cfoundation/Debug/Profiling.h"
#include "cfoundation/Serialization/Serializer.h"

#include "csystem/Core/Core.h"
#include "csystem/Input/Input.h"

#include "cgui/Globals.h"
#include "cgui/GuiControls.h"
#include "cgui/Gui/GuiStyle.h"
#include "cgui/Input/Input.h"
#include "cgui/Tooltip/TooltipSystem.h"

#include "cctrl/WindowController.h"
#include "cctrl/WindowManager.h"

using namespace CCtrl;

App::App()
:	m_bDone( false ),
	m_u32NumFPS( 0 ),
	m_f32FPS( 0.0f )
{
}

App::~App()
{
}

bool App::Init()
{
	// Init Gui
	if( !CGui::Init( GetWindowCaption(), GetWidth(), GetHeight() ) )
	{
		CF_WARN( L"Gui couldn't be initialized..." );
		return false;
	}

    // Init Gui controls
    if ( !CGui::InitControls() )
    {
		CF_WARN( L"Gui controls couldn't be initialized..." );
		return false;
    }

    // Set exit delegate
    CSystem::ExitDelegatePtr spExitDelegate( new CSystem::ExitDelegate( this, &App::OnExit ) );
    CSystem::Input::GetInstance().SetExitDelegate( spExitDelegate );

	return true;
}
		
void App::Deinit()
{
	// Deinit gui
	CGui::Deinit();

    // Destroy window manager
    WindowManager::DestroyInstance();
}

void App::ProcessMessage( const CFoundation::Message& clMessage )
{
	UNREFERENCED_PARAMETER( clMessage );
}

Float32 App::GetFPS() const
{
	return m_f32FPS;
}

void App::Start()
{
	Init();

    // Set default gui style
    CGui::GuiMain::GetInstance().SetDefaultStyle( GetDefaultStyle() );

    // Create the start windows
	CreateStartWindows();

	// start render loop
    Unsigned64 u64ElapsedMs = 0;
	m_bDone = false;
    bool bUseFrameBrake = false;
	m_clFPSTimer.Start();
	CFoundation::Timer clFrameTimer;
	while( !m_bDone )
	{
		clFrameTimer.Start();
		StartProfiling( "MainLoop" );

        Float32 f32TimeDifference = u64ElapsedMs / 1000.0f;

        // Update the system input
        CSystem::Input::GetInstance().Update( f32TimeDifference );

        // Update application
        Update( f32TimeDifference );

        // Update tooltip system
        CGui::TooltipSystem::GetInstance().Update( f32TimeDifference );

        // Update gui input
		CGui::Input::GetInstance().Update( f32TimeDifference );

        // Update GUI
		StartProfiling( "Update" );
		CGui::GuiMain::GetInstance().Update( f32TimeDifference );
		EndProfiling();

        // Let windows process input
        WindowManager::GetInstance().ProcessInput( CSystem::Input::GetInstance().GetState() );

        // Update Window Manager
        WindowManager::GetInstance().Update( f32TimeDifference );

		StartProfiling( "Render" );
		CGui::GuiMain::GetInstance().Render();
		EndProfiling();

		EndProfiling();

		ProfilingEndFrame();

		++m_u32NumFPS;

		if( m_clFPSTimer.GetElapsedMs() >= 1000 )
		{
			m_f32FPS = m_u32NumFPS / ( m_clFPSTimer.GetElapsedMs() / 1000.0f );

			m_clFPSTimer.Start();
			m_u32NumFPS = 0;
		}

		//OutputProfiling();

        // Get elapsed ms
        u64ElapsedMs = clFrameTimer.GetElapsedMs();

        if ( bUseFrameBrake )
        {
		    // Frame Brake
		    Integer64 i64SleepTimeMs = 30 - u64ElapsedMs;
		    if ( i64SleepTimeMs > 0 )
		    {
                CSystem::Core::GetInstance().Sleep( i64SleepTimeMs );
		    }
        }
	}

    // Deinitialize
    Deinit();
}

void App::Save( CFoundation::Serializer& ) const
{
}

void App::Load( const CFoundation::Serializer& )
{
}

void App::SaveFile( const CFoundation::String &sFileName ) const
{
	CFoundation::Serializer clSerializer;

	// add data
	Save( clSerializer );

	// save
	clSerializer.SaveFile( sFileName );
}

void App::LoadFile( const CFoundation::String &sFileName )
{
	CFoundation::Serializer clSerializer;

	// load
	if( clSerializer.LoadFile( sFileName ) )
	{
		// get data
		Load( clSerializer );
	}
}

void App::Exit()
{
    OnExit();
}

const CFoundation::String   App::GetWindowCaption() const
{
	return "Set a Caption by overloading App::GetWindowCaption.";
}

Unsigned32 App::GetWidth() const
{
	return GetDimensions().GetX();
}

Unsigned32 App::GetHeight() const
{
	return GetDimensions().GetY();
}
        
CFoundation::Vector2Du App::GetDimensions() const
{
    return CFoundation::Vector2Du( 1024, 768 );
}

CGui::GuiStylePtr App::GetDefaultStyle() const
{
    return CGui::GuiStylePtr( new CGui::GuiStyle( "common/ctrls/styles/default/default_gui_style.xml" ) );
}
     
void App::Update( Float32 /*_f32TimeDifference*/ )
{
}
        
void App::OnExit()
{
    m_bDone = true;
}
