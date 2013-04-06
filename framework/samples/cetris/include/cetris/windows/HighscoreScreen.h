#ifndef CETRIS_HIGHSCORESCREEN_H
#define CETRIS_HIGHSCORESCREEN_H

#include "cctrl/WindowController.h"

namespace Cetris
{
	class HighscoreScreen : public CCtrl::WindowController
	{
	public:
		static const CFoundation::String GetXMLFileName();

        /// Constructor
        HighscoreScreen();

        /// Destructor
        ~HighscoreScreen();

        /// Initialization
		virtual void InitWindow();

        /// Deinitialization
		virtual void DeinitWindow();

		void NewHighscore( bool bNewHighscore );

		void Update();

	private:

		void EnableInput( bool bEnable );
		void AddHighscore();

        /// Called when the ok button was clicked
        void                                OnClickedBtOk();

        /// Called when the menu button was clicked
        void                                OnClickedBtMenu();

        /// Delegate when the ok button was clicked
        CGui::ObjectInputEventDelegate0Ptr  m_spDelegateClickedBtOk;

        /// Delegate when the menu button was clicked
        CGui::ObjectInputEventDelegate0Ptr  m_spDelegateClickedBtMenu;
        
		// Controls
		CGui::TextBoxPtr    m_spTbCongratulations;
		CGui::TextBoxPtr    m_spTbName;
		CGui::TextBoxPtr    m_spTbTop10;

		CGui::EditBoxPtr    m_spEbName;
        CGui::ButtonPtr     m_spBtOk;

		CGui::ListBoxPtr    m_spLbHighscores;
		
		CGui::ButtonPtr     m_spBtMenu;
	};
}

#endif
