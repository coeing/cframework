#ifndef CMADTV_BLOCK_TRASH_DROP_CONTROLLER_H
#define CMADTV_BLOCK_TRASH_DROP_CONTROLLER_H

#include "cgui/DragDrop/DropController.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( PlayerContext );

    class BlockTrashDropController : public CGui::DropController
    {
    public:
        /// Constructor
        BlockTrashDropController( PlayerContextPtr _spContext = PlayerContextPtr() );

        /// Destructor
        ~BlockTrashDropController();

        /// Executes a drop action
        /*! The drag object is dropped on the drop object at the passed position.
         *  Position is relative to the drop object.
         */
        virtual bool                Execute( CGui::GuiObjectPtr _spDragObject, CGui::DragDataPtr _spDragData, const CFoundation::Vector2Di& _vDragPosition, const CFoundation::Vector2Di& _vDropPosition );

        /// Sets the context
        void                        SetContext( PlayerContextPtr _spContext );

    private:
        /// Context
        PlayerContextPtr            m_spContext;
    };

    INCLUDE_SHARED_CLASS( BlockTrashDropController );
}

#endif
