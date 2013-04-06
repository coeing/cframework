#ifndef CADVENTCALENDAR_ADVENTCALENDARAPP_H
#define CADVENTCALENDAR_ADVENTCALENDARAPP_H

#include "cfoundation/Utils/Singleton.h"
#include "cctrl/App.h"

#include "Door.h"

namespace CAdventCalendar
{
    class AdventCalendarWindow;
    typedef SharedPtr< AdventCalendarWindow > AdventCalendarWindowPtr;

    class AdventCalendarApp : public CCtrl::App, public CFoundation::Singleton< AdventCalendarApp >
    {
    public:
        AdventCalendarApp();

	    virtual void                            CreateStartWindows();
		
		virtual void                            Save( CFoundation::Serializer& _clSerializer ) const;

		virtual void                            Load( const CFoundation::Serializer& _clSerializer );

        void                                    OnDoorOpened( Door& _clDoor );

        void                                    OnSurpriseClicked( const Surprise& _clSurprise );

		virtual void                            Exit();

    protected:
		virtual bool                            Init();

		virtual const CFoundation::String       GetWindowCaption() const;      

    private:
        typedef std::set< Unsigned8 > U8Set;
        Surprise                                m_clNoSurprise;

        // Opened doors
        U8Set                                   m_au8OpenedDoors;

        // Main window
        AdventCalendarWindowPtr                 m_spWindow;
    };
}

#endif