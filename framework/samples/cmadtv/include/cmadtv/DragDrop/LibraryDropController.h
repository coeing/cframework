#ifndef CMADTV_LIBRARY_DROP_CONTROLLER_H
#define CMADTV_LIBRARY_DROP_CONTROLLER_H

#include "cfoundation/Events/DelegateInvoker.h"

#include "cgui/DragDrop/DropController.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( GuiObject );
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Broadcast );
    INCLUDE_SHARED_CLASS( PlayerContext );

    /// Dealer drop events
    enum LibraryDropEvent
    {
        LIBRARY_DROP_EVENT_NONE,
        LIBRARY_DROP_EVENT_BAG_FULL,
        LIBRARY_DROP_EVENT_END
    };

    /// Controller to drop broadcasts/adverts into the bag at the dealer
    class LibraryDropController : public CGui::DropController, public CFoundation::Delegate1Invoker< LibraryDropEvent, BroadcastPtr >
    {
    public:
        /// Constructor
        LibraryDropController( PlayerContextPtr _spContext, bool _bBag );

        /// Destructor
        virtual ~LibraryDropController();

        /// Executes a drop action
        /** The drag object is dropped on the drop object at the passed position.
         *  Position is relative to the drop object.
         */
        virtual bool        Execute( CGui::GuiObjectPtr _spDragObject, CGui::DragDataPtr _spDragData, const CFoundation::Vector2Di& _vDragPosition, const CFoundation::Vector2Di& _vDropPosition );

        /// Sets the context
        void                SetContext( PlayerContextPtr _spContext );

    private:
        /// Executes a broadcast drop action
        bool                ExecuteBroadcastDrop( BroadcastPtr _spBroadcast );

        /// Context
        PlayerContextPtr    m_spContext;

        /// Indicates if it's the bag the drop controller is for
        /// Otherwise it's the librarian
        bool                m_bBag;
    };

    INCLUDE_SHARED_CLASS( LibraryDropController );
}

#endif
