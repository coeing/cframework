#ifndef CMADTV_BROADCASTTOOLTIPCONTROLLER_H
#define CMADTV_BROADCASTTOOLTIPCONTROLLER_H

#include "cgui/Tooltip/PointerTooltipController.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Broadcast );
    INCLUDE_SHARED_CLASS( BroadcastTooltip );
    INCLUDE_SHARED_CLASS( Player );
    INCLUDE_SHARED_CLASS( PlayerContext );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// BroadcastTooltipController
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    /// Controller to show a broadcast tooltip

    class BroadcastTooltipController : public CGui::PointerTooltipController
    {
    public:
        /// Destroys the broadcast tooltip
        static void         DestroyTooltip();

        /// Constructor
        explicit BroadcastTooltipController( PlayerContextPtr _spContext = PlayerContextPtr(), BroadcastPtr _spBroadcast = BroadcastPtr() );

        /// Destructor
        virtual ~BroadcastTooltipController();

        /// Starts showing the tooltip for the passed object
        virtual void        Start( CGui::GuiObjectPtr _spObject, const CFoundation::Vector2Di& _vPointerPosition );

        /// Sets the context
        void                SetContext( PlayerContextPtr _spContext );

        /// Sets the broadcast
        void                SetBroadcast( BroadcastPtr _spBroadcast );

        /// Sets the owner
        void                SetOwner( PlayerPtr _spOwner );

        /// Returns the broadcast
        BroadcastPtr        GetBroadcast() const;

    private:
        /// Broadcast tooltip (only one for all, because there could be only one at one time)
        static BroadcastTooltipPtr  s_spTooltip;

        /// Context
        PlayerContextPtr    m_spContext;

        /// Broadcast
        BroadcastPtr        m_spBroadcast;

        /// Owner
        PlayerPtr           m_spOwner;
    };

    /// BroadcastTooltipController pointer
    typedef SharedPtr< BroadcastTooltipController > BroadcastTooltipControllerPtr;
}

#endif
