#ifndef CMADTV_AdvertDROPCONTROLLER_H
#define CMADTV_AdvertDROPCONTROLLER_H

#include "cfoundation/DateTime/Date.h"

#include "cgui/DragDrop/DropController.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( GuiObject );
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( PlayerContext );

    enum Hour;
    enum ScheduleDay;
    
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// AdvertDropController
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    /// Controller to drop adverts to schedule slots

    class AdvertDropController : public CGui::DropController
    {
    public:
        /// Constructor
        AdvertDropController( PlayerContextPtr _spContext, const CFoundation::Date& _Date, Hour _eSlot );

        /// Destructor
        virtual ~AdvertDropController();

        /// Executes a drop action
        /*! The drag object is dropped on the drop object at the passed position.
         *  Position is relative to the drop object.
         */
        virtual bool        Execute( CGui::GuiObjectPtr _spDragObject, CGui::DragDataPtr _spDragData, const CFoundation::Vector2Di& _vDragPosition, const CFoundation::Vector2Di& _vDropPosition );

        /// Sets the context
        void                SetContext( PlayerContextPtr _spContext );

    private:
        /// Context
        PlayerContextPtr    m_spContext;

        /// Schedule day
        CFoundation::Date   m_Date;

        /// Schedule slot
        Hour                m_eSlot;
    };

    INCLUDE_SHARED_CLASS( AdvertDropController );
}

#endif
