#include "cetris/CetrisApp.h"

#include "cetris/windows/MainMenu.h"

using namespace Cetris;

const CFoundation::String CetrisApp::GetWindowCaption() const
{
	return "CeTris";
}
		
Unsigned32 CetrisApp::GetWidth() const
{
    return 800;
}
        
Unsigned32 CetrisApp::GetHeight() const
{
    return 600;
}

void CetrisApp::CreateStartWindows()
{
    CCtrl::WindowController::CreateWindow< MainMenu >();
}

bool CetrisApp::Init()
{
	bool bSuccess = App::Init();

	// load highscores
	LoadFile( "highscores.txt" );

	return bSuccess;
}

void CetrisApp::Save( CFoundation::Serializer &clSerializer ) const
{
	Game::GetInstance().Save( clSerializer );
}

void CetrisApp::Load( const CFoundation::Serializer &clSerializer ) 
{
	Game::GetInstance().Load( clSerializer );
}

void CetrisApp::Exit()
{
	// save highscores
	SaveFile( "highscores.txt" );

	App::Exit();
}