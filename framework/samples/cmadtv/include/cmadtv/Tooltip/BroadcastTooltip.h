#ifndef CMADTV_BROADCAST_TOOLTIP_H
#define CMADTV_BROADCAST_TOOLTIP_H

#include "CFoundation/Utils/Singleton.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( Panel );
    INCLUDE_SHARED_CLASS( ProgressBar );
    INCLUDE_SHARED_CLASS( TextBox );
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Broadcast );
    INCLUDE_SHARED_CLASS( BroadcastTooltip );
    INCLUDE_SHARED_CLASS( Player );
    INCLUDE_SHARED_CLASS( PlayerContext );

    class BroadcastTooltip
    {
    public:
        /// Creates a broadcast tooltip
        static BroadcastTooltipPtr  Create();

        /// Destructor
        ~BroadcastTooltip();

        /// Sets the context
        void                SetContext( PlayerContextPtr _spContext );

        /// Sets the broadcast
        void                SetBroadcast( BroadcastPtr _spBroadcast );

        /// Returns the broadcast
        BroadcastPtr        GetBroadcast() const;

        /// Sets the owner
        void                SetOwner( PlayerPtr _spOwner );

        /// Clears the tooltip
        void                Clear();

        /// Returns the panel of the tooltip
        CGui::PanelPtr      GetPanel() const;

    private:
        /// Updates the contents
        void                UpdateTooltip();

        /// Constructor
        BroadcastTooltip( CGui::PanelPtr _spPnlRoot );

        /// Context
        PlayerContextPtr        m_spContext;

        /// Owner
        PlayerPtr               m_spOwner;

        /// Broadcast
        BroadcastPtr            m_spBroadcast;

        /// Panel
        CGui::PanelPtr          m_spPnlRoot;

        /// Title
        CGui::TextBoxPtr        m_spTbTitle;

        /// Description
        CGui::TextBoxPtr        m_spTbDescription;

        /// Genre
        CGui::TextBoxPtr        m_spTbGenre;

        /// Country/Year
        CGui::TextBoxPtr        m_spTbCountryYear;

        /// Blocks
        CGui::TextBoxPtr        m_spTbBlocks;

        /// Price
        CGui::TextBoxPtr        m_spTbPrice;

        /// Up2date
        CGui::ProgressBarPtr    m_spPbUp2date;

        /// Planned
        CGui::TextBoxPtr        m_spTbPlanned;
    };
}

#endif
