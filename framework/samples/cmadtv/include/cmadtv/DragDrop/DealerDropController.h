#ifndef CMADTV_DEALER_DROP_CONTROLLER_H
#define CMADTV_DEALER_DROP_CONTROLLER_H

#include "cfoundation/Events/DelegateInvoker.h"

#include "cgui/DragDrop/DropController.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( GuiObject );
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Advert );
    INCLUDE_SHARED_CLASS( PlayerContext );
    INCLUDE_SHARED_CLASS( Broadcast );

    /// Dealer drop events
    enum DealerDropEvent
    {
        DEALER_DROP_EVENT_NONE,
        DEALER_DROP_EVENT_BAG_FULL,
        DEALER_DROP_EVENT_INSUFFICIENT_MONEY,
        DEALER_DROP_EVENT_END
    };

    /// Controller to drop broadcasts/adverts into the bag at the dealer
    class DealerDropController : public CGui::DropController, public CFoundation::Delegate1Invoker< DealerDropEvent, BroadcastPtr >
    {
    public:
        /// Constructor
        DealerDropController( PlayerContextPtr _spContext, bool _bBag );

        /// Destructor
        virtual ~DealerDropController();

        //! Executes a drop action
        /*! The drag object is dropped on the drop object at the passed position.
         *  Position is relative to the drop object.
         */
        virtual bool        Execute( CGui::GuiObjectPtr _spDragObject, CGui::DragDataPtr _spDragData, const CFoundation::Vector2Di& _vDragPosition, const CFoundation::Vector2Di& _vDropPosition );

        /// Sets the context
        void                SetContext( PlayerContextPtr _spContext );

    private:
        /// Executes a broadcast drop action
        bool                ExecuteBroadcastDrop( BroadcastPtr _spBroadcast );

        /// Executes an advert drop action
        bool                ExecuteAdvertDrop( AdvertPtr _spAdvert );

        /// Context
        PlayerContextPtr    m_spContext;

        /// Indicates if it's the bag the drop controller is for
        bool                m_bBag;
    };

    /// DealerDropController pointer
    INCLUDE_SHARED_CLASS( DealerDropController );
}

#endif
