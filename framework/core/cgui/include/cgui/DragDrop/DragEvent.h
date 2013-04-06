#ifndef CGUI_DRAGEVENT_H
#define CGUI_DRAGEVENT_H

#include "cfoundation/Events/Delegate.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( DragController );
    INCLUDE_SHARED_CLASS( DragData );
    INCLUDE_SHARED_CLASS( GuiObject );

    /// Drag events
    enum DragEvent
    {
        EVENT_DRAG_STARTED,     ///< Dragging was started
        EVENT_DRAG_ENDED,       ///< Dragging was ended
        EVENT_DRAG_UNDONE,      ///< Dragging was undone
    };

    /// Drag event delegate
    TYPEDEF_DELEGATE3( DragEvent, DragControllerPtr, GuiObjectPtr, DragDataPtr );
    typedef Delegate3< DragControllerPtr, GuiObjectPtr, DragDataPtr > DragEventDelegate;
    typedef SharedPtr< DragEventDelegate > DragEventDelegatePtr;
}

#endif
