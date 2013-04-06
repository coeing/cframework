#include "cmadtv/Windows/LoginMenu/LoginMenuWindow.h"

#include "cfoundation/XML/XMLParser.h"

#include "csystem/Processes/Process.h"

#include "cgui/Gui/ComboBox.h"
#include "cgui/Gui/EditBox.h"

#include "cmadtv/Data/Network/NetworkMessages.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// LoginMenuWindow
///
////////////////////////////////////////////////////////////////////////////////////////////

struct LoginMenuWindow::ServerInfo
{
    CFoundation::String m_strName;
    CFoundation::String m_strHost;
    Unsigned32          m_u32Port;
};

CFoundation::String LoginMenuWindow::GetXMLFileName()
{
    return "data/windows/LoginMenuWindow.xml";
}

LoginMenuWindow::LoginMenuWindow()
{
}

LoginMenuWindow::~LoginMenuWindow()
{
}

void LoginMenuWindow::InitWindow()
{    
    // Get edit boxes
    m_spCbServer = GetComboBox( "CbServer" );
    m_spEbName = GetEditBox( "EbName" );
    m_spEbColor = GetEditBox( "EbColor" );

    // Load buttons
    m_spBtConnect = GetButton( "BtConnect" );
    m_spDelegateClickedConnect = m_spBtConnect->RegisterDelegate( CGui::INPUT_EVENT_PUSHED, CFoundation::CreateDelegate( this, &LoginMenuWindow::OnConnect ) );

    m_spBtBack = GetButton( "BtBack" );
    m_spDelegateClickedBack = m_spBtBack->RegisterDelegate( CGui::INPUT_EVENT_PUSHED, CFoundation::CreateDelegate( this, &LoginMenuWindow::OnBack ) );

    // Read server infos
    ReadServerInfos( "data/configuration/Servers.xml" );
}
        
void LoginMenuWindow::DeinitWindow()
{
    // Remove delegates
    DISCONNECT_DELEGATE_CONNECTION( m_spDelegateClickedConnect );
    DISCONNECT_DELEGATE_CONNECTION( m_spDelegateClickedBack );
}
        
void LoginMenuWindow::ReadServerInfos( const CFoundation::String& _strXMLFile )
{
    // Open xml file
    CFoundation::XMLDocumentPtr spDoc = CFoundation::XMLParser::GetInstance().ParseFile( _strXMLFile );
    if ( spDoc == NULL )
    {
        return;
    }

    // Parse file
    CFoundation::XMLElementPtr spRoot = spDoc->GetRootElement();
    CFoundation::XMLNodePtr spNode = spRoot != NULL ? spRoot->GetFirstChild() : CFoundation::XMLNodePtr();
    while ( spNode != NULL )
    {
        if ( spNode->IsElement() )
        {
            CFoundation::XMLElementPtr spElement = static_pointer_cast< CFoundation::XMLElement >( spNode );
            if ( spElement->GetName() == "server" )
            {
                // Create server info
                ServerInfo info;
                info.m_strName = spElement->GetAttribute( "name" );
                info.m_strHost = spElement->GetAttribute( "host" );
                info.m_u32Port = spElement->GetAttributeAsUnsigned32( "port" );

                // Add server info
                AddServerInfo( info );
            }
        }
        spNode = spNode->GetNextSibling();
    }
}
        
void LoginMenuWindow::AddServerInfo( const ServerInfo& _Info )
{
    // Add to infos
    Unsigned32 u32Idx = m_aServerInfos.size();
    m_aServerInfos.push_back( _Info );

    // Add to combobox
    if ( m_spCbServer != NULL )
    {
        m_spCbServer->AddItem( _Info.m_strName, u32Idx );
    }
}

void LoginMenuWindow::RemoveServerInfos()
{
}
               
void LoginMenuWindow::OnConnect()
{
    // Get server
    Unsigned32 u32ServerIdx = m_spCbServer->GetSelectedItemData();
    if ( u32ServerIdx >= m_aServerInfos.size() )
    {
        return;
    }

    const ServerInfo& serverInfo = m_aServerInfos[ u32ServerIdx ];

    // Get name
    const CFoundation::String& strName = m_spEbName->GetText();

    // Get color
    const CFoundation::String& strColor = m_spEbColor->GetText();

    // Login to server
    LoginMsgPtr spMsg( new LoginMsg( serverInfo.m_strHost, serverInfo.m_u32Port, strName, CFoundation::Color( strColor ) ) );

    // Send message
    CSystem::Process::GetInstance()->EmitMsg( spMsg );
}

void LoginMenuWindow::OnBack()
{
//    ClientApplication::GetInstance().Exit();
}
