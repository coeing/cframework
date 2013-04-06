#ifndef CMADTV_GAME_OVER_WINDOW_H
#define CMADTV_GAME_OVER_WINDOW_H

#include "cctrl/Controls/ButtonGroup.h"

#include "cmadtv/Windows/Window.h"

namespace CGui
{
    TYPEDEF_DELEGATE2( ListBoxEvent, Unsigned32, Unsigned32 );
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Game );

    class GameOverWindow : public Window
    {
    public:
        //! Returns the xml file name
        static CFoundation::String  GetXMLFileName();

        /// Constructor
        GameOverWindow();

        /// Destructor
        ~GameOverWindow();

        /// Initializes the window
		virtual void            InitWindow();

        /// Deinitializes the window
        virtual void            DeinitWindow();

        /// Initialization
        void                    Init( GamePtr _spGame );

    private:
        /// Callback when game should be started again
        void                    OnRestart();

        /// Callback when to go to the main menu
        void                    OnMainMenu();

        /// Callback when game should be exited
        void                    OnExit();

        /// Delegates
        CGui::ObjectInputEventDelegate0Ptr          m_spDelegateClickedRestart;
        CGui::ObjectInputEventDelegate0Ptr          m_spDelegateClickedMainMenu;
        CGui::ObjectInputEventDelegate0Ptr          m_spDelegateClickedExit;

        /// Game
        GamePtr                 m_spGame;

        /// Points Listbox
        CGui::ListBoxPtr        m_spLbPoints;

        /// Highscore Listbox
        CGui::ListBoxPtr        m_spLbHighscores;

        /// Restart button
        CGui::ButtonPtr         m_spBtRestart;

        /// Main menu button
        CGui::ButtonPtr         m_spBtMainMenu;

        /// Exit button
        CGui::ButtonPtr         m_spBtExit;
    };
}

#endif
