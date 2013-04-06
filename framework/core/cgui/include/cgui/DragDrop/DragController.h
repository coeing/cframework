#ifndef CGUI_DRAGCONTROLLER_H
#define CGUI_DRAGCONTROLLER_H

#include "cfoundation/Math/Vector.h"
#include "cfoundation/Events/DelegateInvoker.h"

#include "cgui/DragDrop/DragData.h"
#include "cgui/DragDrop/DragEvent.h"

namespace CGui
{
    class GuiObject;
    typedef SharedPtr< GuiObject > GuiObjectPtr;

    /// Interface of a drag controller for a gui object
    class DragController : public CFoundation::Delegate3Invoker< DragEvent, DragControllerPtr, GuiObjectPtr, DragDataPtr >, public CFoundation::SharedPtrBase< DragController >
    {
    public:
        DELEGATE3_INVOKER( DragEvent, DragControllerPtr, GuiObjectPtr, DragDataPtr );

        //! Constructor
        DragController();

        /// Starts dragging for the passed object
        virtual void        Start( GuiObjectPtr _spObject, const CFoundation::Vector2Di& _vRelativeMousePosition );

        /// Returns the current drag object
        GuiObjectPtr        GetObject() const;

        /// Continues dragging for the current drag object
        virtual void        Continue( const CFoundation::Vector2Di& _vMovement ) = 0;

        /// Indicates if dropping is valid on the passed object
        virtual bool        IsDropValid( GuiObjectPtr _spObject );

        /// Returns the drag data of the current drag operation
        virtual DragDataPtr GetDragData() const;

        /// Undoes the dragging for the current drag object
        virtual void        Undo();

        /// Ends dragging for the current drag object
        virtual void        End();

    private:
        //! Pointer to the current drag object
        GuiObjectPtr        m_spObject;
    };
}

#endif
