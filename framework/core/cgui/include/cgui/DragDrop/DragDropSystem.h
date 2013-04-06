#ifndef CGUI_DRAGDROPSYSTEM_H
#define CGUI_DRAGDROPSYSTEM_H

#include "cfoundation/Math/BorderType.h"
#include "cfoundation/Utils/Singleton.h"

#include "csystem/Input/InputState.h"

#include "cgui/DragDrop/DragController.h"
#include "cgui/DragDrop/DropController.h"

namespace CGui
{
    class GuiObject;
    typedef SharedPtr< GuiObject > GuiObjectPtr;
    typedef SharedPtr< const GuiObject > GuiObjectCPtr;

    //! This system handles all drag&drop stuff
	class DragDropSystem : public CFoundation::Singleton< DragDropSystem >
	{
	public:
        //! Constructor
		DragDropSystem();

        //! Destructor
		~DragDropSystem();

        /// Processes input
        bool                    ProcessInput( const CSystem::InputState& _state, GuiObjectPtr _spPointedObject, const CFoundation::Vector2Di& _vRelativePoint );

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// Drag & Drop
        ////////////////////////////////////////////////////////////////////////////////////////////

        //! Sets the drag controller for the passed object
        void                    SetDragController( CGui::GuiObjectCPtr _spObject, DragControllerPtr _spDragController );

        //! Removes the drag controller for the passed object
        void                    RemoveDragController( CGui::GuiObjectCPtr _spObject );

        //! Returns the drag controller for the passed object
        DragControllerPtr       GetDragController( CGui::GuiObjectCPtr _spObject ) const;

        //! Sets the drop controller for the passed object
        void                    SetDropController( CGui::GuiObjectCPtr _spObject, DropControllerPtr _spDropController );

        //! Removes the drop controller for the passed object
        void                    RemoveDropController( CGui::GuiObjectCPtr _spObject );

        //! Returns the drop controller for the passed object
        DropControllerPtr       GetDropController( CGui::GuiObjectCPtr _spObject ) const;

        /// Ends dragging the current object
        /// _bUndo indicates if the changes should be undone
		void                    EndDragging( bool _bUndo );

		const CGui::GuiObjectPtr    GetCurrentDragObject() const;

		void                    StartResizing( CGui::GuiObjectPtr pResizeObject, CFoundation::BorderType eBorder );
		void                    ContinueResizing( const CFoundation::Vector2Di &clMovement );

        /// Ends resizing the current object
        /// _bUndo indicates if the changes should be undone
		void                    EndResizing( bool _bUndo );

		CGui::GuiObjectPtr        GetCurrentResizeObject() const;

	private:
		CFoundation::Vector2Di	    m_clMousePosition;

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// Drag & Drop
        ////////////////////////////////////////////////////////////////////////////////////////////

        typedef std::map< CGui::GuiObjectCPtr, DragControllerPtr > ObjectDragControllerMap;
        typedef std::map< CGui::GuiObjectCPtr, DropControllerPtr > ObjectDropControllerMap;

        //! Updates the drop controller
        void                    UpdateDropController( const CFoundation::Vector2Di& _clMousePosition, GuiObjectPtr _spPointedObject );

        /// Drag object
        CGui::GuiObjectPtr      m_spDragObject;

        //! The current active drag controller
		DragControllerPtr       m_spDragController;

        //! The current drop controller
        DropControllerPtr       m_spDropController;

        //! Drag controllers for the gui objects
        ObjectDragControllerMap m_mapDragControllers;

        //! Drop controllers for the gui objects
        ObjectDropControllerMap m_mapDropControllers;

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// Resizing
        ////////////////////////////////////////////////////////////////////////////////////////////

		void                    UpdateResizeCursor( CFoundation::BorderType eBorder ) const;

        /// Object which is resized at the moment
		CGui::GuiObjectPtr      m_spResizeObject;

        /// Initial offset
        CFoundation::Vector2Di  m_vResizeInitialOffset;

        /// Initial size
        CFoundation::Vector2Du  m_vResizeInitialSize;

        /// Anchor for resizing
		CFoundation::BorderType m_eResizeAnchor;
	};
}

#endif
