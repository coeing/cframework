#ifndef CADVENTCALENDAR_SURPRISEWINDOW_H
#define CADVENTCALENDAR_SURPRISEWINDOW_H

#include "csystem/Input/Input.h"
#include "cctrl/WindowController.h"

#include "Surprise.h"

namespace CAdventCalendar
{
    class SurpriseWindow : public CCtrl::WindowController
    {
    public:
		SurpriseWindow();

        static const CFoundation::String    GetXMLFileName();

        /// Initialization
		virtual void                        InitWindow();   

        /// Deinitialization
		virtual void                        DeinitWindow();   

        virtual void                        Init( const Surprise& _clSuprise );

    private:
        const Surprise* m_pSurprise;

        /// Callback when left mouse button was clicked
        void                            OnClickedLeft();

        /// Delegate when left mouse button was clicked
        CSystem::InputEventDelegatePtr  m_spDelegateClickedLeft;

        CGui::ImagePtr      m_pImgImage;
        CGui::TextBoxPtr    m_pTbCaption;
        CGui::TextBoxPtr    m_pTbText;
    };

    typedef SharedPtr< SurpriseWindow > SurpriseWindowPtr;
}

#endif
