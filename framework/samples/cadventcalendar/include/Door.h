#ifndef CADVENTCALENDAR_DOOR_H
#define CADVENTCALENDAR_DOOR_H

#include "cfoundation/Events/MessageObject.h"

#include "cgui/Gui/Image.h"
#include "cgui/Gui/TextBox.h"

#include "Surprise.h"

namespace CAdventCalendar
{
    class Door : public CFoundation::MessageObject
    {
    public:
        Door( CGui::TextBoxPtr _spTbDoor, CGui::ImagePtr _spImgSuprise );

        void                InitFromXML( const CFoundation::XMLElementPtr _spXMLElement );

        Unsigned8           GetDay() const;

        void                Open();

    private:
        /// Callback when door was clicked
        void                OnClickedDoor();

        /// Callback when surprise was clicked
        void                OnClickedSurprise();

        /// Delegate when door was clicked
        CGui::ObjectInputEventDelegate0Ptr  m_spDelegateClickedDoor;

        /// Delegate when surprise was clicked
        CGui::ObjectInputEventDelegate0Ptr  m_spDelegateClickedSurprise;

        Unsigned8           m_u8Day;

        CGui::TextBoxPtr    m_spTbDoor;

        CGui::ImagePtr      m_spImgSurprise;

        Surprise            m_clSurprise;
    };
}

#endif
