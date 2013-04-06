#ifndef CGUI_POINTER_TOOLTIP_CONTROLLER_H
#define CGUI_POINTER_TOOLTIP_CONTROLLER_H

#include "cgui/Tooltip/TooltipController.h"

namespace CGui
{
    /**
	 * \brief A tooltip controller for a tooltip that's glued to the pointer
	 * 
	 * 
	 * \remarks
	 * 
	 * \see TooltipController
	 */
    class PointerTooltipController : public TooltipController
    {
    public:        
        /// Constructor
        PointerTooltipController();

        /// Starts showing the tooltip for the passed object
        virtual void            Start( GuiObjectPtr _spObject, const CFoundation::Vector2Di& _vPointerPosition );

        /// Continues showing the tooltip
        virtual void            Continue( const CFoundation::Vector2Di& _vPointerMovement );

        /// Ends showing the tooltip
        virtual void            End();

        /// Sets the gui object that's glued to the pointer
        virtual void            SetTooltip( GuiObjectPtr _spTooltip );

        /// Returns the gui object that's glued to the pointer
        virtual GuiObjectPtr    GetTooltip() const;

    private:
        /// Updates the tooltip offset
        void                    UpdateOffset();

        /// Pointer to the object that's glued to the pointer
        GuiObjectPtr            m_spTooltip;

        /// Current offset
        CFoundation::Vector2Di  m_vOffset;
    };

    INCLUDE_SHARED_CLASS( PointerTooltipController );
}

#endif
