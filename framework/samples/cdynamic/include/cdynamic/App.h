#ifndef CDYNAMIC_APP_H
#define CDYNAMIC_APP_H

#include "cofiles/Singleton.h"

#include "cctrl/App.h"

namespace CDynamic
{
	class App : public CCtrl::App, public CoFiles::Singleton<App>
	{
	public:

	protected:
		virtual const CCtrl::WindowController* CreateStartScreen() const;
	};
}

#endif