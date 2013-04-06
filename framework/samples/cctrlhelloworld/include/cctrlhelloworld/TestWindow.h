#ifndef CTEST_TESTWINDOW_H
#define CTEST_TESTWINDOW_H

#include "cctrl/WindowController.h"

namespace CGui
{
    TYPEDEF_DELEGATE0( CheckBoxEvent );
}

namespace CTest
{
	class TestWindow : public CCtrl::WindowController
	{
	public:
        /// Returns the xml file name
        static const CFoundation::String GetXMLFileName();

        /// Constructor
		TestWindow();

        /// Initializes the window
		virtual void        InitWindow();

        /// Deinitializes the window
        virtual void        DeinitWindow();

        /// Processes input (when window is active)
        virtual void        ProcessInput( const CSystem::InputState& _State );

	private:
        /// Callback when a direction button was clicked
        void                OnClickedDirection( CGui::GuiObjectPtr _spObject );

        /// Callback when exit button was clicked
        void                OnClickedExit();

        /// Callback when highlight color should be changed
        void                OnChangeHighlightColor();

        /// Delegate when a direction button was clicked
        CGui::ObjectInputEventDelegate1Ptr  m_spDelegateClickedDirection;

        /// Delegate when exit button was clicked
        CGui::ObjectInputEventDelegate0Ptr  m_spDelegateClickedExit;

        /// Delegate when highlight color should be changed
        CGui::CheckBoxEventDelegate0Ptr     m_spDelegateChangeHighlightColor;

        CGui::TextBoxPtr    m_spTbFirst;
		CGui::ButtonPtr		m_spBtUp;
		CGui::ButtonPtr		m_spBtDown;
		CGui::ButtonPtr		m_spBtLeft;
		CGui::ButtonPtr		m_spBtRight;
		CGui::ButtonPtr		m_spBtExit;
		CGui::ListBoxPtr    m_spLbFirst;
		CGui::CheckBoxPtr   m_spChbChangeHighlightColor;

		CFoundation::Color	m_clHighlightColors[ 2 ];
	};
}

#endif
