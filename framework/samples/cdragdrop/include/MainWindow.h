#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "cctrl/WindowController.h"

#include "cgui/Gui/Image.h"
#include "cgui/Gui/TextBox.h"

class MainWindow : public CCtrl::WindowController
{
public:
    //! Constructor
	MainWindow();

    //! Returns the XML file name of this screen
    static CFoundation::String  GetXMLFileName();

    //! Initializes the window
	virtual void                InitWindow();

private:
};

#endif
