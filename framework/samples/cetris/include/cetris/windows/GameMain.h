#ifndef CETRIS_GAMEMAIN_H
#define CETRIS_GAMEMAIN_H

#include "cfoundation/DateTime/Timer.h"

#include "cctrl/WindowController.h"

#include "cetris/Game.h"

namespace CSystem
{
    enum KeyCode;
    typedef Delegate1< KeyCode > KeyInputEventDelegate;
    typedef SharedPtr< KeyInputEventDelegate > KeyInputEventDelegatePtr;
}

namespace Cetris
{
	class GameMain : public CCtrl::WindowController
	{
	public:
		GameMain();

		static const CFoundation::String GetXMLFileName();

		virtual void            InitWindow();

        /// Per frame update
		virtual void            Update( Float32 _f32TimeDifference );

        /// Processes input (when window is active)
        virtual void            ProcessInput( const CSystem::InputState& _state );

	private:
        /// Called when the user wants to pause the game
        void                    OnPause();

        /// Called when the user wants to exit the game
        void                    OnExit();

        /// Delegate when the user wants to pause the game
        Delegate0Ptr            m_spDelegatePause;

        /// Delegate when the user wants to exit the game
        Delegate0Ptr            m_spDelegateExit;

		// Data
		OneGame			*m_pOneGame;

		// Controls
		CGui::PanelPtr      m_pMainPanel;
		CGui::TextBoxPtr    m_pTbGameOver;

		CGui::TextBoxPtr    m_pTbTimer;
		CGui::TextBoxPtr    m_pTbPoints;
		CGui::TextBoxPtr    m_pTbRows;
		CGui::TextBoxPtr    m_pTbLevel;

		CGui::ButtonPtr     m_pBtPause;
		CGui::ButtonPtr     m_pBtExit;
	};
}

#endif
