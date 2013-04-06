#ifndef CGUI_GLOBALS_H
#define CGUI_GLOBALS_H

#include "cfoundation/Text/String.h"
#include "cgui/GUI/GuiMain.h"

namespace CGui
{
    /// Initialization of the gui system
	bool    Init( const CFoundation::String &sCaption, Unsigned32 u32Width, Unsigned32 u32Height, CFoundation::Color clBgColor = CFoundation::Color( 1.0f, 1.0f, 1.0f ) );

    /// Deinitialization of the gui system
    void    Deinit();

    /// Let's the gui system run
	void    Run();
};

#endif
