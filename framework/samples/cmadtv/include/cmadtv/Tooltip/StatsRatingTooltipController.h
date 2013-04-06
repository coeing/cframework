#ifndef CMADTV_STATS_RATING_TOOLTIP_CONTROLLER_H
#define CMADTV_STATS_RATING_TOOLTIP_CONTROLLER_H

#include "cgui/Tooltip/PointerTooltipController.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( BlockStat );
    INCLUDE_SHARED_CLASS( StatsRatingTooltip );

    /// Controller to show a broadcast tooltip
    class StatsRatingTooltipController : public CGui::PointerTooltipController
    {
    public:
        /// Destroys the tooltip
        static void         DestroyTooltip();

        /// Constructor
        explicit StatsRatingTooltipController( BlockStatPtr _spBlockStat = BlockStatPtr() );

        /// Destructor
        virtual ~StatsRatingTooltipController();

        /// Starts showing the tooltip for the passed object
        virtual void        Start( CGui::GuiObjectPtr _spObject, const CFoundation::Vector2Di& _vPointerPosition );

        /// Sets the broadcast
        void                SetBlockStat( BlockStatPtr _spBlockStat );

    private:
        /// Tooltip (only one for all, because there could be only one at one time)
        static StatsRatingTooltipPtr  s_spTooltip;

        /// Block stat
        BlockStatPtr        m_spBlockStat;
    };

    /// StatsRatingTooltipController pointer
    INCLUDE_SHARED_CLASS( StatsRatingTooltipController );
}

#endif
