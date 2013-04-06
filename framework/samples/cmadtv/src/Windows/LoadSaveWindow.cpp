#include "cmadtv/Windows/LoadSaveWindow.h"

#include "csystem/Input/Input.h"

#include "cgui/Gui/EditBox.h"
#include "cgui/Gui/GuiMain.h"

#include "cmadtv/Application/Application.h"

using namespace CMadTV;
        
CFoundation::String LoadSaveWindow::GetXMLFileName()
{
    return "data/windows/LoadSaveWindow.xml";
}

LoadSaveWindow::LoadSaveWindow()
:   m_eMode( MODE_NONE ),
    m_ePrevRoom( ROOM_NONE )
{
    // Create delegates
    m_spDelegatePressedMouseRight = CSystem::InputEventDelegate0Ptr( new CSystem::InputEventDelegate0( this, &LoadSaveWindow::OnClose ) );
}

LoadSaveWindow::~LoadSaveWindow()
{
}

void LoadSaveWindow::InitWindow()
{
    Unsigned32 u32NumColumns = 2;
    Unsigned32 u32NumRows = 5;
    for ( Unsigned32 u32Column = 0; u32Column < u32NumColumns; ++u32Column )
    {
        // Get slot template and anchor
        CGui::PanelPtr spPnlSlotTemplate = GetPanel( CFoundation::String::Build( "PnlSlotTemplate%d", u32Column + 1 ) );
        GetWindow()->RemoveChild( spPnlSlotTemplate );
        CGui::GuiObjectPtr spObjSlotAnchor = GetGuiObject( CFoundation::String::Build( "ObjSlotAnchor%d", u32Column + 1 ) );

        // Create slots
        for ( Unsigned32 u32Row = 0; u32Row < u32NumRows; ++u32Row )
        {
            // Create slot
            Unsigned32 u32Idx = u32Column * u32NumRows + u32Row;
            CGui::PanelPtr spPnlSlot = CGui::GuiMain::GetInstance().CreateFromTemplate( CFoundation::String::Build( "PnlSlot%d", u32Idx ), spPnlSlotTemplate );
            CGui::EditBoxPtr spEbName = static_pointer_cast< CGui::EditBox >( spPnlSlot->FindChild( "EbName" ) );
            spEbName->SetText( CFoundation::String::Build( "Slot%d", u32Idx ) );
            m_aspEbSlots.push_back( spEbName );

            // Add slot
            spObjSlotAnchor->AddAfter( spPnlSlot );
            spObjSlotAnchor = spPnlSlot;
        }

        CGui::GuiMain::GetInstance().Delete( spPnlSlotTemplate );
    }

    // Add delegates
    CSystem::Input::GetInstance().RegisterDelegate( CSystem::INPUT_EVENT_RMB_PRESSED, m_spDelegatePressedMouseRight );

    WindowController::InitWindow();
}
        
void LoadSaveWindow::DeinitWindow()
{
    // Remove delegates
    CSystem::Input::GetInstance().RemoveDelegate( CSystem::INPUT_EVENT_RMB_PRESSED, m_spDelegatePressedMouseRight );
}
      
void LoadSaveWindow::Init( Mode _eMode, Room _ePrevRoom )
{
    m_ePrevRoom = _ePrevRoom;
    SetMode( _eMode );
}
        
void LoadSaveWindow::ProcessInput( const CSystem::InputState& _State )
{
}
       
void LoadSaveWindow::SetMode( Mode _eMode )
{
    switch ( _eMode )
    {
    case MODE_NONE:
        {
            // TODO CO Show mode select window
        }
        break;
    case MODE_SAVE:
        {
            EnableSlots( true );
        }
        break;
    case MODE_LOAD:
        {
            EnableSlots( false );
        }
        break;
    }
}
        
void LoadSaveWindow::EnableSlots( bool _bEnable )
{
    Unsigned32 u32NumSlots = m_aspEbSlots.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumSlots; ++u32Idx )
    {
        CGui::EditBoxPtr spEbSlot = m_aspEbSlots[ u32Idx ];
        spEbSlot->SetDisabled( !_bEnable );
    }
}
        
void LoadSaveWindow::OnClose()
{
    Application::GetInstance().SetActiveRoom( m_ePrevRoom );
}
