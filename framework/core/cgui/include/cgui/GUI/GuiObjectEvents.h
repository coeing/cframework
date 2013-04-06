#ifndef CGUI_GUI_OBJECT_EVENTS_H
#define CGUI_GUI_OBJECT_EVENTS_H

#include <vector>

#include "cfoundation/Events/Delegate.h"
#include "cfoundation/Math/Vector.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CGui
{
    class RenderParams;
    enum State;

    INCLUDE_SHARED_CLASS( GuiObject );
    INCLUDE_SHARED_CLASS( RenderElement );

    /// Render events
    enum RenderEvent
    {
        RENDER_EVENT_NONE,

        RENDER_EVENT_POST_RENDER,

        RENDER_EVENT_END
    };
    TYPEDEF_DELEGATE3( RenderEvent, GuiObjectPtr, const RenderParams&, std::vector< RenderElementPtr >& );

    /// State events
    enum StateEvent
    {
        STATE_EVENT_ACTIVATED,
        STATE_EVENT_DEACTIVATED,
        STATE_EVENT_FOCUSED,
        STATE_EVENT_UNFOCUSED,
    };
    TYPEDEF_DELEGATE1( StateEvent, GuiObjectPtr );

    enum StateEventStateState
    {
        STATE_EVENT_CHANGED,
    };
    TYPEDEF_DELEGATE3( StateEventStateState, GuiObjectPtr, State, State );

    /// Input events
    enum ObjectInputEvent
    {
        INPUT_EVENT_PRESSED,
        INPUT_EVENT_RELEASED,
        INPUT_EVENT_CLICKED,
        INPUT_EVENT_DOUBLE_CLICKED,
        INPUT_EVENT_POINTER_ENTERED,
        INPUT_EVENT_POINTER_LEFT,
        INPUT_EVENT_GAINED_FOCUS,
        INPUT_EVENT_LOST_FOCUS,
        INPUT_EVENT_END,
    };
    TYPEDEF_DELEGATE1( ObjectInputEvent, GuiObjectPtr );

    /// Size events
    enum SizeEvent
    {
        SIZE_EVENT_SIZE_CHANGED,
    };
    TYPEDEF_DELEGATE1( SizeEvent, GuiObjectPtr );

    /// Position events
    enum PositionEvent
    {
        POSITION_EVENT_POSITION_CHANGED,
    };
    TYPEDEF_DELEGATE3( PositionEvent, GuiObjectPtr, const CFoundation::Vector2Di&, const CFoundation::Vector2Di& );
}

#endif
