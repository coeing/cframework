#ifndef CGUI_MOUSE_H
#define CGUI_MOUSE_H

namespace CGui
{
	enum MouseEvent
	{
		ME_UNDEFINED,
		ME_PRESSED,
		ME_RELEASED,
        ME_CLICKED,
		ME_ENTERED,
		ME_EXITED,
		ME_DRAGGING,
		ME_RESIZING,
		NUM_MOUSE_EVENTS
	};
}

#endif
