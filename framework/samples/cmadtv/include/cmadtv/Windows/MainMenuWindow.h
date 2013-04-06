#ifndef CMADTV_MAINMENUWINDOW_H
#define CMADTV_MAINMENUWINDOW_H

#include "cctrl/Controls/ButtonGroup.h"
#include "cctrl/WindowController.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( World );

    class MainMenuWindow : public CCtrl::WindowController
    {
    public:
        //! Returns the xml file name
        static CFoundation::String  GetXMLFileName();

        /// Constructor
        MainMenuWindow();

        /// Destructor
        ~MainMenuWindow();

        /// Initializes the window
		virtual void            InitWindow();

        /// Deinitializes the window
        virtual void            DeinitWindow();

        /// Sets the world
        void                    SetWorld( WorldPtr _spWorld );

    private:
        /// Callback when game should be started
        void                    OnStart();

        /// Callback when game should be loaded
        void                    OnLoad();

        /// Callback when game should be exited
        void                    OnExit();

        /// Callback when player selection was changed
        void                    OnChangedPlayerSelection( CCtrl::ButtonGroupPtr _spButtonGroup, CGui::ButtonPtr _spNewActiveButton, CGui::ButtonPtr _spOldActiveButton );

        /// Delegates
        CGui::ObjectInputEventDelegate0Ptr          m_spDelegateClickedStart;
        CGui::ObjectInputEventDelegate0Ptr          m_spDelegateClickedLoad;
        CGui::ObjectInputEventDelegate0Ptr          m_spDelegateClickedExit;
        CCtrl::ButtonGroupButtonEventDelegate3Ptr   m_spDelegateChangedPlayerSelection;

        /// World
        WorldPtr                m_spWorld;

        /// Player button group
        CCtrl::ButtonGroupPtr   m_spPlayerButtons;

        /// Difficulty button group
        CCtrl::ButtonGroupPtr   m_spDifficultyButtons;

        /// Start button
        CGui::ButtonPtr         m_spBtStart;

        /// Load button
        CGui::ButtonPtr         m_spBtLoad;

        /// Exit button
        CGui::ButtonPtr         m_spBtExit;
    };
}

#endif
