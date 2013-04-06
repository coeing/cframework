#ifndef CMADTV_Sammy_TOOLTIP_H
#define CMADTV_Sammy_TOOLTIP_H

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
    INCLUDE_SHARED_CLASS( Sammy );
    INCLUDE_SHARED_CLASS( SammyTooltip );
    INCLUDE_SHARED_CLASS( World );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// SammyTooltip
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class SammyTooltip
    {
    public:
        /// Creates a tooltip
        static SammyTooltipPtr    Create();

        /// Returns the xml file name
        static CFoundation::String  GetXMLFileName();

        /// Constructor
        SammyTooltip( CGui::PanelPtr _spPnlRoot );

        /// Destructor
        ~SammyTooltip();

        /// Sets the Sammy
        void                    SetSammy( SammyPtr _spSammy );

        /// Returns the Sammy
        SammyPtr                GetSammy() const;

        /// Returns the root panel
        CGui::PanelPtr          GetPanel() const;

    private:
        /// Updates the tooltip
        void                    UpdateTooltip();

        /// Sammy
        SammyPtr                m_spSammy;

        /// Controls
        CGui::PanelPtr          m_spPnlRoot;

        CGui::TextBoxPtr        m_spTbName;
    };
}

#endif
