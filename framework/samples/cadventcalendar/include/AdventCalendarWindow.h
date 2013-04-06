#ifndef CADVENTCALENDAR_ADVENTCALENDARWINDOW_H
#define CADVENTCALENDAR_ADVENTCALENDARWINDOW_H

#include "cctrl/WindowController.h"

#include "cgui/Gui/Image.h"
#include "cgui/Gui/TextBox.h"

namespace CAdventCalendar
{
    class Door;

    class AdventCalendarWindow : public CCtrl::WindowController
    {
    public:
		AdventCalendarWindow();

        static const CFoundation::String    GetXMLFileName();

		virtual void                        InitWindow();

        Door*                               GetDoor( Unsigned8 _u8Day );

    private:
        typedef std::map< Unsigned8, Door* >    DayDoorMap;

        DayDoorMap          m_apDoors;

        CGui::PanelPtr      m_spPnlCalendar;

        CGui::TextBoxPtr    m_pTbDoorTemplate;

        CGui::ImagePtr      m_pImgDoorTemplate;
    };

    typedef SharedPtr< AdventCalendarWindow > AdventCalendarWindowPtr;
}

#endif
