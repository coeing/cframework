#ifndef CED_SETTINGSWINDOW_H
#define CED_SETTINGSWINDOW_H

#include "cctrl/WindowController.h"

namespace Ced
{
	class SettingsWindow : public CCtrl::WindowController
	{
	public:
        /// Returns the xml file name for this window
		static const CFoundation::String    GetXMLFileName();

        /// Constructor
        SettingsWindow();

        /// Initializes the window
		virtual void                        InitWindow();

        /// Deinitializes the window
        virtual void                        DeinitWindow();

    private:
        /// Callbacks
        void                                OnOk();
        void                                OnCancel();

        /// Delegates
        CGui::ObjectInputEventDelegate0Ptr  m_spDelegateClickedOk;
        CGui::ObjectInputEventDelegate0Ptr  m_spDelegateClickedCancel;

        /// Controls
		CGui::ButtonPtr                     m_spBtOk;
		CGui::ButtonPtr                     m_spBtCancel;
    };
}

#endif
