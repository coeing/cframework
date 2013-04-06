#ifndef CMADTV_BROADCASTBLOCKDROPCONTROLLER_H
#define CMADTV_BROADCASTBLOCKDROPCONTROLLER_H

#include "cfoundation/DateTime/Date.h"

#include "cgui/DragDrop/DropController.h"

#include "cmadtv/Data/Player/Schedule.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Broadcast );
    INCLUDE_SHARED_CLASS( BroadcastBlock );
    INCLUDE_SHARED_CLASS( PlayerContext );
    
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// BroadcastBlockDropController
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    /// Controller to drop broadcasts to broadcast slots

    class BroadcastBlockDropController : public CGui::DropController
    {
    public:
        /// Constructor
        BroadcastBlockDropController( PlayerContextPtr _spContext, const CFoundation::Date& _Date, BroadcastBlockPtr _spBroadcastBlock, Hour _eStartSlot );

        /// Destructor
        virtual ~BroadcastBlockDropController();

        /// Executes a drop action
        /** The drag object is dropped on the drop object at the passed position.
         *  Position is relative to the drop object.
         */
        virtual bool        Execute( CGui::GuiObjectPtr _spDragObject, CGui::DragDataPtr _spDragData, const CFoundation::Vector2Di& _vDragPosition, const CFoundation::Vector2Di& _vDropPosition );

        /// Sets the context
        void                SetContext( PlayerContextPtr _spContext );

    private:
        /// Context
        PlayerContextPtr    m_spContext;

        /// Schedule date
        CFoundation::Date   m_Date;

        /// Broadcast block
        BroadcastBlockPtr   m_spBroadcastBlock;

        /// Start slot
        Hour                m_eStartSlot;
    };

    INCLUDE_SHARED_CLASS( BroadcastBlockDropController );
}

#endif
