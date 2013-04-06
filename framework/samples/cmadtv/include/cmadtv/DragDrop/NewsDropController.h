#ifndef CMADTV_NewsDROPCONTROLLER_H
#define CMADTV_NewsDROPCONTROLLER_H

#include "cgui/DragDrop/DropController.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( PlayerContext );

    /// Controller to drop news to slots
    class NewsDropController : public CGui::DropController
    {
    public:
        /// Constructor
        NewsDropController( PlayerContextPtr _spContext, Unsigned32 _u32Slot );

        /// Destructor
        virtual ~NewsDropController();

        //! Executes a drop action
        /*! The drag object is dropped on the drop object at the passed position.
         *  Position is relative to the drop object.
         */
        virtual bool        Execute( CGui::GuiObjectPtr _spDragObject, CGui::DragDataPtr _spDragData, const CFoundation::Vector2Di& _vDragPosition, const CFoundation::Vector2Di& _vDropPosition );
        
        /// Sets the context
        void                SetContext( PlayerContextPtr _spContext );

    private:
        /// Context
        PlayerContextPtr    m_spContext;

        /// Slot
        Unsigned32          m_u32Slot;
    };

    INCLUDE_SHARED_CLASS( NewsDropController );
}

#endif
