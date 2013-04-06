#ifndef CMADTV_LOGIN_MENU_WINDOW_H
#define CMADTV_LOGIN_MENU_WINDOW_H

#include "cgui/Gui/GuiObjectEvents.h"

#include "cctrl/Controls/ButtonGroup.h"

#include "cmadtv/Windows/Window.h"

namespace CMadTV
{
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// LoginMenuWindow
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class LoginMenuWindow : public Window
    {
    public:
        /// Returns the xml file name
        static CFoundation::String  GetXMLFileName();

        /// Constructor
        LoginMenuWindow();

        /// Destructor
        ~LoginMenuWindow();

        /// Initializes the window
		virtual void            InitWindow();

        /// Deinitializes the window
        virtual void            DeinitWindow();

    private:
        /// Server info
        struct ServerInfo;

        /// Reads the server infos from the passed xml
        void                    ReadServerInfos( const CFoundation::String& _strXMLFile );

        /// Adds a server info
        void                    AddServerInfo( const ServerInfo& _Info );

        /// Removes all server infos
        void                    RemoveServerInfos();

        /// Callbacks
        void                    OnConnect();
        void                    OnBack();

        /// Delegates
        CFoundation::DelegateConnectionPtr  m_spDelegateClickedConnect;
        CFoundation::DelegateConnectionPtr  m_spDelegateClickedBack;

        /// Server infos
        std::vector< ServerInfo >   m_aServerInfos;

        /// Controls
        CGui::ComboBoxPtr       m_spCbServer;
        CGui::EditBoxPtr        m_spEbName;
        CGui::EditBoxPtr        m_spEbColor;

        CGui::ButtonPtr         m_spBtConnect;
        CGui::ButtonPtr         m_spBtBack;
    };
}

#endif
