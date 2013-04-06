#ifndef CCTRL_FREEDRAGCONTROLLER_H
#define CCTRL_FREEDRAGCONTROLLER_H

#include "cgui/DragDrop/DragController.h"

namespace CGui
{
    class Panel;
    typedef SharedPtr< Panel > PanelPtr;

    class FreeDragController : public DragController
    {
    public:
        //! Constructor
        FreeDragController();

        //! Starts dragging for the passed object
        virtual void        Start( GuiObjectPtr _spObject, const CFoundation::Vector2Di& _vRelativeMousePosition );

        //! Continues dragging for the current drag object
        virtual void        Continue( const CFoundation::Vector2Di& _vMovement );

        //! Undoes the dragging for the current drag object
        virtual void        Undo();

        //! Ends dragging for the current drag object
        virtual void        End();

    private:
        //! Old parent of the drag object
        PanelPtr            m_spOldParent;

        //! Old position of the drag object
        CFoundation::Vector2Di  m_vOldPosition;
    };

    /// FreeDragController pointer
    typedef SharedPtr< FreeDragController > FreeDragControllerPtr;
}

#endif
