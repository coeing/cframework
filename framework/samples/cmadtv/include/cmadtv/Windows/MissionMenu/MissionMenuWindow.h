#ifndef CMADTV_MISSION_MENU_WINDOW_H
#define CMADTV_MISSION_MENU_WINDOW_H

#include "cctrl/Controls/ButtonGroup.h"
#include "cctrl/WindowController.h"

namespace CGui
{
    TYPEDEF_DELEGATE2( ListBoxEvent, Unsigned32, Unsigned32 );
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Mission );
    INCLUDE_SHARED_CLASS( World );

    class MissionMenuWindow : public CCtrl::WindowController
    {
    public:
        //! Returns the xml file name
        static CFoundation::String  GetXMLFileName();

        /// Constructor
        MissionMenuWindow();

        /// Destructor
        ~MissionMenuWindow();

        /// Initializes the window
		virtual void            InitWindow();

        /// Deinitializes the window
        virtual void            DeinitWindow();

        /// Initialization
        void                    Init( WorldPtr _spWorld );

    private:
        /// Callback when game should be started
        void                    OnStart();

        /// Callback when game should be loaded
        void                    OnLoad();

        /// Callback when game should be exited
        void                    OnExit();

        /// Callback when mission was selected
        void                    OnSelectedMission( Unsigned32 _u32Row );

        /// Callback when player selection was changed
        void                    OnChangedPlayerSelection( CCtrl::ButtonGroupPtr _spButtonGroup, CGui::ButtonPtr _spNewActiveButton, CGui::ButtonPtr _spOldActiveButton );

        /// Delegates
        CGui::ObjectInputEventDelegate0Ptr          m_spDelegateClickedStart;
        CGui::ObjectInputEventDelegate0Ptr          m_spDelegateClickedLoad;
        CGui::ObjectInputEventDelegate0Ptr          m_spDelegateClickedExit;
        CGui::ListBoxEventDelegate1Ptr              m_spDelegateSelectedMission;

        /// World
        WorldPtr                m_spWorld;

        /// Difficulty button group
        CCtrl::ButtonGroupPtr   m_spDifficultyButtons;

        /// Mission Listbox
        CGui::ListBoxPtr        m_spLbMissions;

        /// Mission description
        CGui::TextBoxPtr        m_spTbMissionDesc;

        /// Start button
        CGui::ButtonPtr         m_spBtStart;

        /// Load button
        CGui::ButtonPtr         m_spBtLoad;

        /// Exit button
        CGui::ButtonPtr         m_spBtExit;

        /// Missions
        std::vector< MissionPtr >   m_aspMissions;
    };
}

#endif
