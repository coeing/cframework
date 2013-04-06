#ifndef CGUI_TOOLTIPCONTROLLER_H
#define CGUI_TOOLTIPCONTROLLER_H

#include "cfoundation/Events/DelegateInvoker.h"
#include "cfoundation/Math/Vector.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( GuiObject );
    INCLUDE_SHARED_CLASS( TooltipController );

    enum TooltipControllerEvent
    {
        EVENT_TOOLTIP_CONTROLLER_STARTED,
        EVENT_TOOLTIP_CONTROLLER_ENDED,
    };
    TYPEDEF_DELEGATE3( TooltipControllerEvent, TooltipControllerPtr, GuiObjectPtr, const CFoundation::Vector2Di& );

    /// Interface of a tooltip controller for a gui object
    class TooltipController : public CFoundation::SharedPtrBase< TooltipController >, public CFoundation::Delegate3Invoker< TooltipControllerEvent, TooltipControllerPtr, GuiObjectPtr, const CFoundation::Vector2Di& >
    {
    public:
        DELEGATE3_INVOKER( TooltipControllerEvent, TooltipControllerPtr, GuiObjectPtr, const CFoundation::Vector2Di& );

        /// Constructor
        TooltipController();

        /// Starts showing the tooltip for the passed object
        virtual void        Start( GuiObjectPtr _spObject, const CFoundation::Vector2Di& _vPointerPosition );

        /// Continues showing the tooltip
        virtual void        Continue( const CFoundation::Vector2Di& _vPointerMovement );

        /// Ends showing the tooltip
        virtual void        End();

        /// Returns the object a tooltip is shown for
        GuiObjectPtr        GetObject() const;

    private:
        /// Pointer to the object a tooltip is shown for
        GuiObjectPtr        m_spObject;
    };
}

#endif
