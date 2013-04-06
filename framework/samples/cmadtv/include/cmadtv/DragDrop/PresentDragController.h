#ifndef CMADTV_PRESENT_DRAGCONTROLLER_H
#define CMADTV_PRESENT_DRAGCONTROLLER_H

#include "cgui/DragDrop/ProxyDragController.h"
#include "cgui/Gui/Panel.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Present );
    INCLUDE_SHARED_CLASS( PresentObject );
    INCLUDE_SHARED_CLASS( Player );

    /// Drag data when dragging a Present
    class PresentDragData : public CGui::DragData
    {
    public:
        /// Present
        PresentPtr      m_spPresent;

        /// Player the present comes from
        PlayerPtr       m_spPlayer;
    };
    INCLUDE_SHARED_CLASS( PresentDragData );

    /// Controller to drag present to slots/trash
    class PresentDragController : public CGui::ProxyDragController
    {
    public:
        /// Constructor
        PresentDragController( PresentPtr _spPresent = PresentPtr(), PlayerPtr _spPlayer = PlayerPtr() );

        /// Destructor
        virtual ~PresentDragController();

        /// Returns the drag data of the current drag operation
        virtual CGui::DragDataPtr   GetDragData() const;

        /// Sets the present
        void                        SetPresent( PresentPtr _spPresent );

        /// Sets the player
        void                        SetPlayer( PlayerPtr _spPlayer );

    protected:
        /// Creates the proxy gui object that's shown while dragging
        virtual CGui::GuiObjectPtr  CreateProxyObject( CGui::GuiObjectPtr _spObject ) const;

    private:
        /// Drag data
        PresentDragDataPtr      m_spDragData;

        /// Present object that is dragged
        PresentObjectPtr        m_spPresentObject;
    };
    INCLUDE_SHARED_CLASS( PresentDragController );
}

#endif
