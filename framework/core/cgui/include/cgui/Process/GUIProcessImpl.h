#ifndef CGUI_GUI_PROCESS_IMPL_H
#define CGUI_GUI_PROCESS_IMPL_H

#include "csystem/Processes/Impl/ProcessImpl.h"

namespace CGui
{    
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// GuiProcessImpl
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class GuiProcessImpl : public CSystem::ProcessImpl
	{
	public:
        /// Constructor
        GuiProcessImpl( CSystem::ProcessManagerPtr _spProcessManager );

        /// Per frame update
        virtual void            Update( Float32 _f32TimeDifference );

    private:
	};
}

#endif
