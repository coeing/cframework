#ifndef CGUI_PROXYDRAGCONTROLLER_H
#define CGUI_PROXYDRAGCONTROLLER_H

#include "cgui/DragDrop/DragController.h"

namespace CGui
{
    class Panel;
    typedef SharedPtr< Panel > PanelPtr;

    /**
	 * \brief
	 * Drag controller which uses a proxy gui object when dragging
	 * 
	 * The proxy drag controller provides the interface for a drag
     * controller that uses a special gui object while dragging an
     * object.
	 * 
	 * \remarks
	 * 
	 * \see
	 * DragController
	 */
	class ProxyDragController : public DragController
    {
    public:
        /// Constructor
        ProxyDragController();

        /// Destructor
        virtual ~ProxyDragController();

        /// Starts dragging for the passed object
        virtual void            Start( GuiObjectPtr _spObject, const CFoundation::Vector2Di& _vRelativeMousePosition );

        /// Continues dragging for the current drag object
        virtual void            Continue( const CFoundation::Vector2Di& _vMovement );

        /// Undoes the dragging for the current drag object
        virtual void            Undo();

        /// Ends dragging for the current drag object
        virtual void            End();

    protected:
        /// Creates the proxy gui object that's shown while dragging
        virtual GuiObjectPtr    CreateProxyObject( GuiObjectPtr _spObject ) const = 0;

    private:
        /// Proxy drag object
        GuiObjectPtr            m_spProxyObject;
    };

    /// ProxyDragController pointer
    typedef SharedPtr< ProxyDragController > ProxyDragControllerPtr;
}

#endif
