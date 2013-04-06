#include "AdventCalendarApp.h"

#include "cfoundation/DateTime/Date.h"

#include "AdventCalendarWindow.h"
#include "SurpriseWindow.h"

using namespace CAdventCalendar;

AdventCalendarApp::AdventCalendarApp()
{
    m_clNoSurprise.m_sImage = "data/images/Curious.png";
    m_clNoSurprise.m_sCaption = "Na, na, na...";
    m_clNoSurprise.m_sText = "...Nicht so neugierig meine Kleine :)";
}

void AdventCalendarApp::CreateStartWindows()
{
	m_spWindow = WindowController::CreateWindow< AdventCalendarWindow >();

    LoadFile( "save.dat" );
}
		
const CFoundation::String AdventCalendarApp::GetWindowCaption() const
{
    return "Adventskalendar 2008 für meine Liebste";
}

void AdventCalendarApp::OnDoorOpened( Door& _clDoor )
{
    CFoundation::Date clToday;
    Unsigned8 u8Day, u8Tmp;
    Integer64 i64Year;
    clToday.GetDate( u8Day, u8Tmp, i64Year );
    if ( _clDoor.GetDay() <= u8Day )
    {
        m_au8OpenedDoors.insert( _clDoor.GetDay() );
        _clDoor.Open();
        SaveFile( "save.dat" );
    }
    else
    {
        SurpriseWindowPtr spSurpriseWindow = WindowController::CreateWindow< SurpriseWindow >();
        spSurpriseWindow->Init( m_clNoSurprise );
    }
}

void AdventCalendarApp::OnSurpriseClicked( const Surprise& _clSurprise )
{
    SurpriseWindowPtr spSurpriseWindow = WindowController::CreateWindow< SurpriseWindow >();
    spSurpriseWindow->Init( _clSurprise );
}

void AdventCalendarApp::Save( CFoundation::Serializer& _clSerializer ) const
{
    Unsigned8 u8NumOpenedDoors = static_cast< Unsigned8 >( m_au8OpenedDoors.size() );
    _clSerializer.Add( u8NumOpenedDoors );
    for ( U8Set::const_iterator it = m_au8OpenedDoors.begin(); it != m_au8OpenedDoors.end(); ++it )
    {
        _clSerializer.Add( *it );
    }
}

void AdventCalendarApp::Load( const CFoundation::Serializer& _clSerializer )
{
    Unsigned8 u8NumOpenedDoors = _clSerializer.GetU8();
    for ( Unsigned32 u32Idx = 0; u32Idx != u8NumOpenedDoors; ++u32Idx )
    {
        Unsigned8 u8Day = _clSerializer.GetU8();
        Door* pDoor = m_spWindow->GetDoor( u8Day );
        if ( pDoor != NULL )
        {
            pDoor->Open();
        }
        m_au8OpenedDoors.insert( u8Day );
    }
}
		
void AdventCalendarApp::Exit()
{
    CCtrl::App::Exit();
}
		
bool AdventCalendarApp::Init()
{
    CCtrl::App::Init();

    return true;
}