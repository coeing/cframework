#ifndef CMADTV_ADVERTDRAGCONTROLLER_H
#define CMADTV_ADVERTDRAGCONTROLLER_H

#include "cfoundation/DateTime/Date.h"

#include "cgui/DragDrop/ProxyDragController.h"
#include "cgui/Gui/Panel.h"

#include "cmadtv/Data/Player/Schedule.h"

namespace CMadTV
{
    class Advert;
    typedef SharedPtr< Advert > AdvertPtr;

    class AdvertBlock;
    typedef SharedPtr< AdvertBlock > AdvertBlockPtr;

    /// Drag data when dragging a Advert
    struct AdvertDragData : public CGui::DragData
    {
    public:
        /// Advert
        AdvertPtr           m_spAdvert;

        /// Date the advert was scheduled
        CFoundation::Date   m_Date;

        /// Slot the advert comes from
        Hour                m_eHour;
    };
    typedef SharedPtr< AdvertDragData > AdvertDragDataPtr;

    /// Advert drag type
    enum AdvertDragType
    {
        ADVERT_DRAG_TYPE_DEALER,
        ADVERT_DRAG_TYPE_BLOCK
    };

    /// Controller to drag Adverts to Advert slots
    class AdvertDragController : public CGui::ProxyDragController
    {
    public:
        /// Constructor
        explicit AdvertDragController( AdvertDragType _eDragType, AdvertPtr _spAdvert = AdvertPtr(), const CFoundation::Date& _Date = CFoundation::Date(), Hour _eHour = HOUR_NONE );

        /// Constructor
        explicit AdvertDragController( AdvertBlockPtr _spBlock, const CFoundation::Date& _Date = CFoundation::Date(), Hour _eHour = HOUR_NONE );

        /// Destructor
        virtual ~AdvertDragController();

        /// Starts dragging for the passed object
        virtual void                Start( CGui::GuiObjectPtr _spObject, const CFoundation::Vector2Di& _vRelativeMousePosition );

        /// Returns the drag data of the current drag operation
        virtual CGui::DragDataPtr   GetDragData() const;

        /// Sets the advert
        void                        SetAdvert( AdvertPtr _spAdvert );

    protected:
        /// Creates the proxy gui object that's shown while dragging
        virtual CGui::GuiObjectPtr  CreateProxyObject( CGui::GuiObjectPtr _spObject ) const;

    private:
        /// Drag type
        AdvertDragType              m_eDragType;

        /// Drag data
        AdvertDragDataPtr           m_spDragData;

        /// Advert block that is dragged
        AdvertBlockPtr              m_spAdvertBlock;
    };

    typedef SharedPtr< AdvertDragController > AdvertDragControllerPtr;
}

#endif
