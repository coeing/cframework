#ifndef CMADTV_NAVIGATIONWINDOW_H
#define CMADTV_NAVIGATIONWINDOW_H

#include "cgui/Gui/GuiObjectEvents.h"

#include "cctrl/Controls/ButtonGroup.h"
#include "cctrl/WindowController.h"

#include "cmadtv/Data/Room.h"

namespace CMadTV
{
    enum NavigationWindowEventRoom
    {
        EVENT_NAVIGATION_WINDOW_ROOM_CLICKED,
    };
    TYPEDEF_DELEGATE1( NavigationWindowEventRoom, Room );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// NavigationWindow
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class NavigationWindow : public CCtrl::WindowController,
                             public CFoundation::Delegate1Invoker< NavigationWindowEventRoom, Room >
    {
    public:
        DELEGATE1_INVOKER( NavigationWindowEventRoom, Room );

        //! Returns the xml file name
        static CFoundation::String  GetXMLFileName();        

        /// Constructor
        NavigationWindow();

        /// Destructor
        virtual ~NavigationWindow();

        /// Initializes the window
		virtual void            InitWindow();

        /// Deinitializes the window
        virtual void            DeinitWindow();

    private:
        typedef std::map< CGui::GuiObjectPtr, Room > ButtonRoomMap;

        /// Callback when a room button was clicked
        void                    OnClickedRoom( CGui::GuiObjectPtr _spObject );

        //! Room button group
        CCtrl::ButtonGroupPtr   m_spRoomButtons;

        //! Button -> room mapping
        ButtonRoomMap           m_mapButtonsRooms;

        /// Delegate when a room button was clicked
        CGui::ObjectInputEventDelegate1Ptr   m_spDelegateClickedRoom;
    };
}

#endif
