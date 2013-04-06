#ifndef CMADTV_STATS_RATING_TOOLTIP_H
#define CMADTV_STATS_RATING_TOOLTIP_H

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
    INCLUDE_SHARED_CLASS( BlockStat );
    INCLUDE_SHARED_CLASS( PlayerContext );
    INCLUDE_SHARED_CLASS( StatsRatingTooltip );

    class StatsRatingTooltip
    {
    public:
        /// Creates a tooltip
        static StatsRatingTooltipPtr    Create();

        /// Returns the xml file name
        static CFoundation::String  GetXMLFileName();

        /// Constructor
        StatsRatingTooltip( CGui::PanelPtr _spPnlRootBroadcast, CGui::PanelPtr _spPnlRootSeries );

        /// Destructor
        ~StatsRatingTooltip();

        /// Sets the context
        void                    SetContext( PlayerContextPtr _spContext );

        /// Sets the block stat
        void                    SetBlockStat( BlockStatPtr _spStat );

        /// Returns the root panel
        CGui::PanelPtr          GetPanel() const;

    private:
        /// Updates the tooltip
        void                    UpdateTooltip();

        /// Context
        PlayerContextPtr        m_spContext;

        /// Block stat
        BlockStatPtr            m_spStat;

        /// Controls
        CGui::PanelPtr          m_spPnlRootBroadcast;
        CGui::PanelPtr          m_spPnlRootSeries;

        CGui::TextBoxPtr        m_spTbBroadcastTitle;
        CGui::TextBoxPtr        m_spTbBroadcastGenre;
        CGui::TextBoxPtr        m_spTbBroadcastBlock;
        CGui::TextBoxPtr        m_spTbBroadcastSpectators;

        CGui::TextBoxPtr        m_spTbSeriesTitle;
        CGui::TextBoxPtr        m_spTbSeriesSubTitle;
        CGui::TextBoxPtr        m_spTbSeriesGenre;
        CGui::TextBoxPtr        m_spTbSeriesBlock;
        CGui::TextBoxPtr        m_spTbSeriesSpectators;
    };
}

#endif
