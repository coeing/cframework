#ifndef CMADTV_ADVERT_TOOLTIP_CONTROLLER_H
#define CMADTV_ADVERT_TOOLTIP_CONTROLLER_H

#include "cgui/Tooltip/PointerTooltipController.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Advert );
    INCLUDE_SHARED_CLASS( AdvertTooltip );
    INCLUDE_SHARED_CLASS( PlayerContext );

    /// Controller to show a Advert tooltip
    class AdvertTooltipController : public CGui::PointerTooltipController
    {
    public:
        /// Destroys the advert tooltip
        static void         DestroyTooltip();

        /// Constructor
        explicit AdvertTooltipController( PlayerContextPtr _spContext = PlayerContextPtr(), AdvertPtr _spAdvert = AdvertPtr() );

        /// Destructor
        virtual ~AdvertTooltipController();

        /// Starts showing the tooltip for the passed object
        virtual void        Start( CGui::GuiObjectPtr _spObject, const CFoundation::Vector2Di& _vPointerPosition );

        /// Sets the player and advert
        void                Set( PlayerContextPtr _spContext, AdvertPtr _spAdvert );

    private:
        /// Advert tooltip (only one for all, because there could be only one at one time)
        static AdvertTooltipPtr  s_spTooltip;

        /// Context
        PlayerContextPtr    m_spContext;

        /// Advert
        AdvertPtr           m_spAdvert;
    };

    /// AdvertTooltipController pointer
    typedef SharedPtr< AdvertTooltipController > AdvertTooltipControllerPtr;
}

#endif
