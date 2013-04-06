#ifndef CMADTV_PRESENT_TOOLTIP_H
#define CMADTV_PRESENT_TOOLTIP_H

#include "cfoundation/Text/String.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( GuiObject );
    INCLUDE_SHARED_CLASS( Panel );
    INCLUDE_SHARED_CLASS( TextBox );
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Game );
    INCLUDE_SHARED_CLASS( Present );
    INCLUDE_SHARED_CLASS( PresentTooltip );

    class PresentTooltip
    {
    public:
        /// Creates a tooltip
        static PresentTooltipPtr    Create();

        /// Returns the xml file name
        static CFoundation::String  GetXMLFileName();

        /// Constructor
        PresentTooltip( CGui::PanelPtr _spPnlRoot );

        /// Destructor
        ~PresentTooltip();

        /// Sets the Game
        void                    SetGame( GamePtr _spGame );

        /// Returns the Game
        GamePtr                GetGame() const;

        /// Sets the present
        void                    SetPresent( PresentPtr _spPresent );

        /// Returns the present
        PresentPtr              GetPresent() const;

        /// Returns the root panel
        CGui::PanelPtr          GetPanel() const;

    private:
        /// Updates the tooltip
        void                    UpdateTooltip();

        /// Game
        GamePtr                m_spGame;

        /// Present
        PresentPtr              m_spPresent;

        /// Controls
        CGui::PanelPtr          m_spPnlRoot;

        CGui::TextBoxPtr        m_spTbPresentName;
        CGui::TextBoxPtr        m_spTbPresentPrice;
        CGui::TextBoxPtr        m_spTbPresentGiven;
    };
}

#endif
