#ifndef CMADTV_PRESENT_DROP_CONTROLLER_H
#define CMADTV_PRESENT_DROP_CONTROLLER_H

#include "cgui/DragDrop/DropController.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Player );
    INCLUDE_SHARED_CLASS( Present );
    INCLUDE_SHARED_CLASS( PresentDragData );

    /// Controller to drag present to slots/trash
    class PresentDropController : public CGui::DropController
    {
    public:
        /// Type
        enum Type
        {
            TYPE_NONE,
            TYPE_BAG,
            TYPE_PRESENTS,
            TYPE_BETTY,
            TYPE_END
        };

        /// Constructor
        PresentDropController( Type _eType );

        /// Destructor
        virtual ~PresentDropController();

        /// Executes a drop action
        /** The drag object is dropped on the drop object at the passed position.
         *  Position is relative to the drop object.
         */
        virtual bool        Execute( CGui::GuiObjectPtr _spDragObject, CGui::DragDataPtr _spDragData, const CFoundation::Vector2Di& _vDragPosition, const CFoundation::Vector2Di& _vDropPosition );
        
        /// Sets the player
        void                SetPlayer( PlayerPtr _spPlayer );

    private:
        /// Executes a drop to the bag
        bool                ExecuteBagDrop( PresentDragDataPtr _spDragData );

        /// Executes a drop to the presents
        bool                ExecutePresentsDrop( PresentDragDataPtr _spDragData );

        /// Executes a drop to betty
        bool                ExecuteBettyDrop( PresentDragDataPtr _spDragData );

        /// Type
        Type                m_eType;

        /// Player
        PlayerPtr           m_spPlayer;
    };

    INCLUDE_SHARED_CLASS( PresentDropController );
}

#endif
