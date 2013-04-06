#ifndef CGUI_DROPCONTROLLER_H
#define CGUI_DROPCONTROLLER_H

#include "cfoundation/Math/Vector.h"

#include "cgui/DragDrop/DragData.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( GuiObject );

    //! Interface of a drop controller for a gui object
    class DropController
    {
    public:
        //! Constructor
        DropController();

        //! Changes the current drop object
        virtual void            SetObject( GuiObjectPtr _spObject );

        //! Removes the current drop object
        virtual void            RemoveObject();

        //! Returns the current drop object
        virtual GuiObjectPtr    GetObject() const;

        /// Enables/Disables the drop controller
        virtual void            SetEnabled( bool _bEnabled );

        /// Returns if the drop controller is enabled
        virtual bool            IsEnabled() const;

        //! Executes a drop action
        /*! The drag object is dropped on the drop object at the passed position.
         *  Position is relative to the drop object.
         */
        virtual bool            Execute( GuiObjectPtr _spDragObject, DragDataPtr _spDragData, const CFoundation::Vector2Di& _vDragPosition, const CFoundation::Vector2Di& _vDropPosition ) = 0;

    private:
        //! Current object
        GuiObjectPtr            m_spObject;

        /// Indicates if the drop controller is enabled
        bool                    m_bEnabled;
    };

    typedef SharedPtr< DropController > DropControllerPtr;
}

#endif
