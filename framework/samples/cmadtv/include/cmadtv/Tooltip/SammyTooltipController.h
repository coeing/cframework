#ifndef CMADTV_Sammy_TOOLTIP_CONTROLLER_H
#define CMADTV_Sammy_TOOLTIP_CONTROLLER_H

#include "cgui/Tooltip/PointerTooltipController.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Sammy );
    INCLUDE_SHARED_CLASS( SammyTooltip );
    INCLUDE_SHARED_CLASS( World );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// SammyTooltipController
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    /// Controller to show a sammy tooltip

    class SammyTooltipController : public CGui::PointerTooltipController
    {
    public:
        /// Destroys the tooltip
        static void         DestroyTooltip();

        /// Constructor
        explicit SammyTooltipController( SammyPtr _spSammy = SammyPtr() );

        /// Destructor
        virtual ~SammyTooltipController();

        /// Starts showing the tooltip for the passed object
        virtual void        Start( CGui::GuiObjectPtr _spObject, const CFoundation::Vector2Di& _vPointerPosition );

        /// Sets the Sammy
        void                SetSammy( SammyPtr _spSammy );

        /// Returns the Sammy
        SammyPtr            GetSammy() const;

    private:
        /// Tooltip (only one for all, because there could be only one at one time)
        static SammyTooltipPtr  s_spTooltip;


        /// Sammy
        SammyPtr            m_spSammy;
    };

    /// SammyTooltipController pointer
    INCLUDE_SHARED_CLASS( SammyTooltipController );
}

#endif
