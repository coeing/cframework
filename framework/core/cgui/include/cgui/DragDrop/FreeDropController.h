#ifndef CGUI_FREEDROPCONTROLLER_H
#define CGUI_FREEDROPCONTROLLER_H

#include "cfoundation/Utils/Singleton.h"

#include "cgui/DragDrop/DropController.h"
#include "cgui/Gui/GuiObject.h"

namespace CGui
{
    class FreeDropController : public DropController, public CFoundation::Singleton< FreeDropController >
    {
    public:
        //! Constructor
        FreeDropController();

        //! Changes the current drop object
        virtual void        SetObject( GuiObjectPtr _spObject );

        //! Removes the current drop object
        virtual void        RemoveObject();

        //! Executes a drop action
        /*! The drag object is dropped on the drop object at the passed position.
         *  Position is relative to the drop object.
         */
        virtual bool        Execute( GuiObjectPtr _spDragObject, DragDataPtr _spDragData, const CFoundation::Vector2Di& _vDragPosition, const CFoundation::Vector2Di& _vDropPosition );

        //! Callback after drop object was rendered
        void                OnPostRenderDropObject( GuiObjectPtr _spObject );

    private:
        /// Post render callback
        RenderEventDelegate1Ptr m_spPostRenderDelegate;
    };

    /// Shared pointer
    typedef SharedPtr< FreeDropController > FreeDropControllerPtr;
}

#endif
