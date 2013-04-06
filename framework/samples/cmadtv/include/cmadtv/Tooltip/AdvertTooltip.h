#ifndef CMADTV_ADVERT_TOOLTIP_H
#define CMADTV_ADVERT_TOOLTIP_H

#include "CFoundation/Utils/Singleton.h"

#include "cgui/Gui/Panel.h"
#include "cgui/Gui/TextBox.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Advert );
    INCLUDE_SHARED_CLASS( AdvertTooltip );
    INCLUDE_SHARED_CLASS( PlayerContext );

    class AdvertTooltip
    {
    public:
        /// Creates a Advert tooltip
        static AdvertTooltipPtr  Create();

        /// Destructor
        ~AdvertTooltip();

        /// Updates the contents depending on the passed values
        void                Set( PlayerContextPtr _spContext, AdvertPtr _spAdvert );

        /// Clears the tooltip
        void                Clear();

        /// Returns the panel of the tooltip
        CGui::PanelPtr      GetPanel() const;

    private:
        /// Constructor
        AdvertTooltip( CGui::PanelPtr _spPnlRoot );

        /// Panel
        CGui::PanelPtr      m_spPnlRoot;

        /// Name
        CGui::TextBoxPtr    m_spTbName;

        /// Slogan
        CGui::TextBoxPtr    m_spTbSlogan;

        /// Bonus
        CGui::TextBoxPtr    m_spTbBonus;

        /// Penalty
        CGui::TextBoxPtr    m_spTbPenalty;

        /// Condition
        CGui::TextBoxPtr    m_spTbCondition;

        /// Spots & Days
        CGui::TextBoxPtr    m_spTbSpotsDays;
    };
}

#endif
