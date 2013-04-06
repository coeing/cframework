#ifndef CMADTV_NEWS_DRAGCONTROLLER_H
#define CMADTV_NEWS_DRAGCONTROLLER_H

#include "cgui/DragDrop/ProxyDragController.h"
#include "cgui/Gui/Panel.h"

namespace CMadTV
{
    //INCLUDE_SHARED_CLASS( News );
    INCLUDE_SHARED_CLASS( NewsInstance );
    INCLUDE_SHARED_CLASS( NewsBlock );
    INCLUDE_SHARED_CLASS( Player );

    /// Drag data when dragging a News
    class NewsDragData : public CGui::DragData
    {
    public:
        /// News
        NewsInstancePtr m_spNews;

        /// Player the news comes from
        PlayerPtr       m_spPlayer;

        /// Slot the news comes from
        Unsigned32      m_u32Slot;
    };
    INCLUDE_SHARED_CLASS( NewsDragData );

    /// Controller to drag news to slots/trash
    class NewsDragController : public CGui::ProxyDragController
    {
    public:
        /// Constructor
        NewsDragController( NewsInstancePtr _spNews, PlayerPtr _spPlayer, Unsigned32 _u32Slot = MAX_U32 );

        /// Constructor
        //NewsDragController( NewsBlockPtr _spNewsBlock, SchedulePtr _spSchedule = SchedulePtr(), Hour _eHour = HOUR_NONE );

        /// Destructor
        virtual ~NewsDragController();

        /// Returns the drag data of the current drag operation
        virtual CGui::DragDataPtr   GetDragData() const;

        /// Sets the news
        void                        SetNews( NewsInstancePtr _spNews );

        /// Sets the player
        void                        SetPlayer( PlayerPtr _spPlayer );

    protected:
        /// Creates the proxy gui object that's shown while dragging
        virtual CGui::GuiObjectPtr  CreateProxyObject( CGui::GuiObjectPtr _spObject ) const;

    private:
        /// Drag data
        NewsDragDataPtr    m_spDragData;

        /// News block that is dragged
        NewsBlockPtr       m_spNewsBlock;
    };
    INCLUDE_SHARED_CLASS( NewsDragController );
}

#endif
