#ifndef CMADTV_PRESENT_TOOLTIP_CONTROLLER_H
#define CMADTV_PRESENT_TOOLTIP_CONTROLLER_H

#include "cgui/Tooltip/PointerTooltipController.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Game );
    INCLUDE_SHARED_CLASS( Present );
    INCLUDE_SHARED_CLASS( PresentTooltip );

    /// Controller to show a broadcast tooltip
    class PresentTooltipController : public CGui::PointerTooltipController
    {
    public:
        /// Destroys the tooltip
        static void         DestroyTooltip();

        /// Constructor
        explicit PresentTooltipController( PresentPtr _spPresent = PresentPtr() );

        /// Destructor
        virtual ~PresentTooltipController();

        /// Starts showing the tooltip for the passed object
        virtual void        Start( CGui::GuiObjectPtr _spObject, const CFoundation::Vector2Di& _vPointerPosition );

        /// Sets the Game
        void                SetGame( GamePtr _spGame );

        /// Returns the Game
        GamePtr             GetGame() const;

        /// Sets the present
        void                SetPresent( PresentPtr _spPresent );

        /// Returns the present
        PresentPtr          GetPresent() const;

    private:
        /// Tooltip (only one for all, because there could be only one at one time)
        static PresentTooltipPtr  s_spTooltip;

        /// Game
        GamePtr                m_spGame;

        /// Present
        PresentPtr          m_spPresent;
    };

    /// PresentTooltipController pointer
    INCLUDE_SHARED_CLASS( PresentTooltipController );
}

#endif
