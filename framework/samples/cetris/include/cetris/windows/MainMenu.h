#ifndef CETRIS_MAINMENU_H
#define CETRIS_MAINMENU_H

#include "cfoundation/DateTime/Timer.h"

#include "cctrl/WindowController.h"

#include "cetris/Game.h"

namespace Cetris
{
	class GameMain;

	class MainMenu : public CCtrl::WindowController
	{
	public:
		MainMenu();

		static const CFoundation::String GetXMLFileName();

		virtual void InitWindow();

        /// Called when a screen was closed
		virtual void            OnCloseScreen( const ScreenClosedMsg& _clMessage );

	private:
        /// Called when a button was clicked
        void                    OnClickedButton( CGui::GuiObjectPtr _spObject );

        /// Delegate when a button was clicked
        CGui::ObjectInputEventDelegate1Ptr  m_spDelegateClickedButton;

		// Data
		SharedPtr< GameMain >           m_pGameMainScreen;

		// Controls
		CGui::ButtonPtr m_pBtStart;
		CGui::ButtonPtr m_pBtHighscores;
		CGui::ButtonPtr m_pBtExit;
	};
}

#endif
