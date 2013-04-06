#ifndef CMADTV_DEALER_BROADCAST_DRAG_CONTROLLER_H
#define CMADTV_DEALER_BROADCAST_DRAG_CONTROLLER_H

#include "cgui/DragDrop/ProxyDragController.h"

namespace CMadTV
{
    class Broadcast;
    typedef SharedPtr< Broadcast > BroadcastPtr;

    /// Drag data when dragging a broadcast
    struct DealerBroadcastDragData : public CGui::DragData
    {
    public:
        /// Broadcast
        BroadcastPtr        m_spBroadcast;
    };
    typedef SharedPtr< DealerBroadcastDragData > DealerBroadcastDragDataPtr;

    /// Controller to drag broadcasts at the dealer
    class DealerBroadcastDragController : public CGui::ProxyDragController
    {
    public:
        /// Constructor
        DealerBroadcastDragController( BroadcastPtr _spBroadcast = BroadcastPtr() );

        /// Destructor
        virtual ~DealerBroadcastDragController();

        /// Returns the drag data of the current drag operation
        virtual CGui::DragDataPtr   GetDragData() const;

        /// Sets the broadcast
        void                        SetBroadcast( BroadcastPtr _spBroadcast );

    protected:
        /// Creates the proxy gui object that's shown while dragging
        virtual CGui::GuiObjectPtr  CreateProxyObject( CGui::GuiObjectPtr _spObject ) const;

    private:
        /// Drag data
        DealerBroadcastDragDataPtr  m_spDragData;
    };

    typedef SharedPtr< DealerBroadcastDragController > DealerBroadcastDragControllerPtr;
}

#endif
