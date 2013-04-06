#ifndef CGUI_MESSAGETYPE_H
#define CGUI_MESSAGETYPE_H

namespace CGui
{
    enum MessageType
    {
        MSG_UNDEFINED,

        // Keyboard messages
        MSG_KEYBOARD,
        MSG_KEYBOARD_EVENT,

        // Mouse messages
        MSG_MOUSE_MOVE,
        MSG_MOUSE_EVENT,

        // Object messages
        MSG_PARENT_CHANGED,
        MSG_CHILDREN_CHANGED,
        MSG_SIZE_CHANGED,
        MSG_POSITION_CHANGED,
        MSG_RESIZABILITY_CHANGED,
        MSG_DRAGGABILITY_CHANGED,
        MSG_FOCUS_CHANGED,
        MSG_TEXT_CHANGED,
        MSG_STATE_FLAG_CHANGED,
        MSG_STATE_CHANGED,

        // Checkbox messages
        MSG_CHECK_CHANGED,

        // Listbox messages
        MSG_CELL_SELECTED,
        MSG_COLUMN_CONTENT_CHANGED,
        MSG_CONTENT_CHANGED,
        MSG_ROW_SELECTED,

        // Scrollbar messages
        MSG_SCROLL_POSITION_CHANGED,

        MSG_OUTLINE_CHANGED,

        MSG_TEXTURE_REPEAT_CHANGED,
        MSG_TEXTURE_SCALE_CHANGED,

        MSG_POST_RENDER,

        MSG_DESTROY,

        MSG_END_GUI_MESSAGES,
    };
}

#endif
