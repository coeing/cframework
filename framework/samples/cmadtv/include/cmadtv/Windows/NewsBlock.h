#ifndef CMADTV_NEWS_BLOCK_H
#define CMADTV_NEWS_BLOCK_H

#include "cgui/Gui/Panel.h"
#include "cgui/Gui/Image.h"
#include "cgui/Gui/TextBox.h"

#include "cmadtv/data/World/WorldEvents.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( NewsBlock );
    INCLUDE_SHARED_CLASS( NewsInstance );

    class NewsBlock
    {
    public:
        /// Creates a news block for the passed news
        static NewsBlockPtr    Create( NewsInstancePtr _spNews );

        /// Constructor
        NewsBlock( CGui::PanelPtr _spPnlRoot );

        /// Destructor
        ~NewsBlock();

        /// Sets the news instance
        void                SetNewsInstance( NewsInstancePtr _spNewsInstance );

        /// Returns the news instance
        NewsInstancePtr     GetNewsInstance() const;

        /// Returns the panel of the block
        CGui::PanelPtr      GetPanel() const;

        /// Sets the news block visible/invisible
        void                SetVisible( bool _bVisible );

    private:
        /// Updates all contents
        void                        UpdateContents();

        /// Updates price
        void                        UpdatePrice();

        /// Updates Time/Day
        void                        UpdateTimeDay();

        /// Callbacks
        void                        OnHourChanged();
        void                        OnDayChanged();

        /// Delegates
        WorldEventDelegate0Ptr      m_spDelegateHourChanged;
        WorldEventDelegate0Ptr      m_spDelegateDayChanged;

        /// News instance
        NewsInstancePtr     m_spNewsInstance;

        /// Panel
        CGui::PanelPtr      m_spPnlRoot;

        /// Text
        CGui::TextBoxPtr    m_spTbText;

        /// Time
        CGui::TextBoxPtr    m_spTbTime;

        /// Price
        CGui::TextBoxPtr    m_spTbPrice;
    };
}

#endif
