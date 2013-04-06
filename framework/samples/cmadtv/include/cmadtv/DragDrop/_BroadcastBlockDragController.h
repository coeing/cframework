#ifndef CMADTV_BROADCASTBLOCKDRAGCONTROLLER_H
#define CMADTV_BROADCASTBLOCKDRAGCONTROLLER_H

#include "cofiles/Utils/SharedPtr.h"
#include "cgui/Input/FreeDragController.h"

namespace CMadTV
{
    class Broadcast;
    typedef SharedPtr< Broadcast > BroadcastPtr;

    enum ScheduleSlot;

    class BroadcastBlock;
    typedef SharedPtr< BroadcastBlock > BroadcastBlockPtr;

    /// Drag data when dragging a broadcast
    struct BroadcastSlotDragData : public CGui::DragData
    {
    public:
        /// Broadcast
        BroadcastPtr        m_spBroadcast;

        /// Slot
        ScheduleSlot        m_eScheduleSlot;
    };
    typedef SharedPtr< BroadcastSlotDragData > BroadcastSlotDragDataPtr;

    class BroadcastBlockDragController : public CGui::FreeDragController
    {
    public:
        /// Constructor
        BroadcastBlockDragController( BroadcastBlockPtr _spBroadcastBlock, ScheduleSlot _eScheduleSlot );

        /// Destructor
        ~BroadcastBlockDragController();

        /// Returns the drag data of the current drag operation
        virtual CGui::DragDataPtr   GetDragData() const;

    private:
        /// Broadcast block
        BroadcastBlockPtr           m_spBroadcastBlock;

        /// Drag data
        BroadcastSlotDragDataPtr    m_spDragData;
    };

    /// BroadcastBlockDragController pointer
    typedef SharedPtr< BroadcastBlockDragController > BroadcastBlockDragControllerPtr;
}

#endif
