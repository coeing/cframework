#ifndef CTEST_TESTAPP_H
#define CTEST_TESTAPP_H

#include "cfoundation/Utils/Singleton.h"
#include "cctrl/App.h"

namespace CTest
{
    class TestApp : public CCtrl::App, public CFoundation::Singleton<TestApp>
    {
    public:
        /// Creates the start windows
	    virtual void                CreateStartWindows();

    protected:
        /// Deinitialization
		virtual void                Deinit();

    private:
        /// Test window
        WindowControllerPtr         m_spTestWindow;
    };
}

#endif