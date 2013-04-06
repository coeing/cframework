#ifndef CDYNAMIC_DYNAMICWINDOW_H
#define CDYNAMIC_DYNAMICWINDOW_H

#include "cctrl/WindowController.h"

namespace CDynamic
{
	class DynamicWindow : public CCtrl::WindowController
	{
	public:
		static const CoFiles::String GetXMLFileName() { return "data/screens/DynamicWindow.xml"; };

		virtual void InitWindow();

		virtual void OnKeyDown( const CGui::KeyboardEventMsg &clMessage );

	private:
		// Controls
		Panel		*m_pPanel;

		Image		*m_pImage;
		Border		*m_pBorder;

		ListBox		*m_pListBox;
	};
}

#endif
