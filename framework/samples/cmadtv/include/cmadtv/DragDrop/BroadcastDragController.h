#ifndef CMADTV_BROADCASTDRAGCONTROLLER_H
#define CMADTV_BROADCASTDRAGCONTROLLER_H

#include "cfoundation/DateTime/Date.h"

#include "cgui/DragDrop/ProxyDragController.h"
#include "cgui/Gui/Panel.h"

#include "cmadtv/Data/Player/Schedule.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Broadcast );
    INCLUDE_SHARED_CLASS( BroadcastBlock );
    
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// BroadcastDragData
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    /// Drag data when dragging a broadcast

    class BroadcastDragData : public CGui::DragData
    {
    public:
        /// Broadcast
        BroadcastPtr        m_spBroadcast;

        /// Date the advert was scheduled
        CFoundation::Date   m_Date;

        /// Slot the broadcast comes from
        Hour                m_eHour;
    };

    INCLUDE_SHARED_CLASS( BroadcastDragData );
    
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// BroadcastDragController
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    /// Controller to drag broadcasts to broadcast slots

    class BroadcastDragController : public CGui::ProxyDragController
    {
    public:
        /// Constructor
        explicit BroadcastDragController( BroadcastPtr _spBroadcast = BroadcastPtr(), const CFoundation::Date& _Date = CFoundation::Date(), Hour _eHour = HOUR_NONE );

        /// Constructor
        explicit BroadcastDragController( BroadcastBlockPtr _spBroadcastBlock, const CFoundation::Date& _Date = CFoundation::Date(), Hour _eHour = HOUR_NONE );

        /// Destructor
        virtual ~BroadcastDragController();

        /// Returns the drag data of the current drag operation
        virtual CGui::DragDataPtr   GetDragData() const;

        /// Sets the broadcast
        void                        SetBroadcast( BroadcastPtr _spBroadcast );

    protected:
        /// Creates the proxy gui object that's shown while dragging
        virtual CGui::GuiObjectPtr  CreateProxyObject( CGui::GuiObjectPtr _spObject ) const;

    private:
        /// Drag data
        BroadcastDragDataPtr    m_spDragData;

        /// Broadcast block that is dragged
        BroadcastBlockPtr       m_spBroadcastBlock;
    };

    INCLUDE_SHARED_CLASS( BroadcastDragController );
}

#endif
