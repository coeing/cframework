#ifndef DRAGDROPAPP_H
#define DRAGDROPAPP_H

#include "cfoundation/Utils/Singleton.h"

#include "cctrl/App.h"

class MainWindow;
typedef SharedPtr< MainWindow > MainWindowPtr;

class DragDropApp : public CCtrl::App, public CFoundation::Singleton< DragDropApp >
{
public:
    DragDropApp();

    virtual void                            CreateStartWindows();

	virtual void                            Exit();

protected:
	virtual bool                            Init();

	virtual const CFoundation::String       GetWindowCaption() const;      

private:
    // Main window
    MainWindowPtr                           m_spWindow;
};

#endif