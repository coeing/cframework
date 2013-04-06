#include "cmadtv/Windows/NavigationWindow.h"

#include "cgui/Gui/Button.h"
#include "cgui/Gui/GuiObject.h"

using namespace CMadTV;
        
CFoundation::String NavigationWindow::GetXMLFileName()
{
    return "data/windows/NavigationWindow.xml";
}

NavigationWindow::NavigationWindow()
{
    m_spDelegateClickedRoom = CGui::ObjectInputEventDelegate1Ptr( new CGui::ObjectInputEventDelegate1( this, &NavigationWindow::OnClickedRoom ) );
}
        
NavigationWindow::~NavigationWindow()
{
}

void NavigationWindow::InitWindow()
{
    // Create button group
    m_spRoomButtons = CCtrl::ButtonGroupPtr( new CCtrl::ButtonGroup() );

    // Get room buttons
    CGui::ButtonPtr spBtBoss = GetButton( "BtBoss" );
    m_mapButtonsRooms[ spBtBoss ] = ROOM_BOSS;

    CGui::ButtonPtr spBtPlanning = GetButton( "BtPlanning" );
    m_mapButtonsRooms[ spBtPlanning ] = ROOM_PLANNING;

    CGui::ButtonPtr spBtSpectators = GetButton( "BtSpectators" );
    m_mapButtonsRooms[ spBtSpectators ] = ROOM_SPECTATORS;

    CGui::ButtonPtr spBtBroadcastDealer = GetButton( "BtBroadcastDealer" );
    m_mapButtonsRooms[ spBtBroadcastDealer ] = ROOM_BROADCAST_DEALER;

    CGui::ButtonPtr spBtAdvertDealer = GetButton( "BtAdvertDealer" );
    m_mapButtonsRooms[ spBtAdvertDealer ] = ROOM_ADVERT_DEALER;

    CGui::ButtonPtr spBtBtNewsStudio = GetButton( "BtNewsStudio" );
    m_mapButtonsRooms[ spBtBtNewsStudio ] = ROOM_NEWS_STUDIO;

    CGui::ButtonPtr spBtLibrary = GetButton( "BtLibrary" );
    m_mapButtonsRooms[ spBtLibrary ] = ROOM_LIBRARY;

    CGui::ButtonPtr spBtSupermarket = GetButton( "BtSupermarket" );
    m_mapButtonsRooms[ spBtSupermarket ] = ROOM_SUPERMARKET;

    CGui::ButtonPtr spBtBetty = GetButton( "BtBetty" );
    m_mapButtonsRooms[ spBtBetty ] = ROOM_BETTY;

    // Register delegates   
    for ( ButtonRoomMap::iterator it = m_mapButtonsRooms.begin(); it != m_mapButtonsRooms.end(); ++it )
    { 
        CGui::ButtonPtr spButton = static_pointer_cast< CGui::Button >( it->first );
        if ( spButton == NULL )
        {
            continue;
        }

        spButton->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedRoom );        
        m_spRoomButtons->AddButton( spButton );
    }

    WindowController::InitWindow();
}
        
void NavigationWindow::DeinitWindow()
{
    // Remove delegates   
    for ( ButtonRoomMap::iterator it = m_mapButtonsRooms.begin(); it != m_mapButtonsRooms.end(); ++it )
    { 
        CGui::ButtonPtr spButton = static_pointer_cast< CGui::Button >( it->first );
        if ( spButton == NULL )
        {
            continue;
        }

        spButton->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedRoom );
        m_spRoomButtons->RemoveButton( spButton );
    }

    m_mapButtonsRooms.clear();

    WindowController::DeinitWindow();
}
		
void NavigationWindow::OnClickedRoom( CGui::GuiObjectPtr _spObject )
{
    ButtonRoomMap::const_iterator it = m_mapButtonsRooms.find( _spObject );
    if ( it != m_mapButtonsRooms.end() )
    {
        Room eRoom = it->second;

        // Dispatch event
        DispatchEvent( EVENT_NAVIGATION_WINDOW_ROOM_CLICKED, eRoom );
    }
}
