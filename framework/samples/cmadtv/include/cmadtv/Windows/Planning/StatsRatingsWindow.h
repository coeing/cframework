#ifndef CMADTV_STATS_RATINGS_WINDOW_H
#define CMADTV_STATS_RATINGS_WINDOW_H

#include "cctrl/Controls/ButtonGroup.h"

#include "cmadtv/Windows/Window.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( GuiObject );
    INCLUDE_SHARED_CLASS( TooltipController );
    TYPEDEF_DELEGATE3( TooltipControllerEvent, TooltipControllerPtr, GuiObjectPtr, const CFoundation::Vector2Di& );
    INCLUDE_SHARED_CLASS( CheckBox );
    TYPEDEF_DELEGATE3( CheckBoxEvent, CheckBoxPtr, bool, bool );
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Game );
    INCLUDE_SHARED_CLASS( Player );
    INCLUDE_SHARED_CLASS( StatsRatingTooltip );

    enum AccountIdentifier;

    class StatsRatingsWindow : public Window
    {
    public:
        /// Returns the xml file name
        static CFoundation::String  GetXMLFileName();

        /// Constructor
        StatsRatingsWindow();

        /// Destructor
        ~StatsRatingsWindow();

        /// Initializes the window
        virtual void            InitWindow();

        /// Deinitializes the window
        virtual void            DeinitWindow();

        /// Processes input (when window is active)
        virtual void            ProcessInput( const CSystem::InputState& _State );

        /// Callback when context changed
        virtual void            OnContextChanged();

    private:
        static const Unsigned32 NUM_AXIS_LABELS = 5;
        
        /// Period to show
        enum Period
        {
            PERIOD_NONE,

            PERIOD_TODAY,
            PERIOD_YESTERDAY,
            PERIOD_WEEK,

            PERIOD_END
        };

        typedef std::map< Unsigned32, CGui::PanelPtr > BarMap;
        typedef std::map< CGui::ButtonPtr, Unsigned32 > ButtonPlayerMap;

        /// Sets the game
        void                    SetGame( GamePtr _spGame );

        /// Sets the player
        void                    SetPlayer( Unsigned32 _u32Player );

        /// Updates the current shown player
        void                    UpdatePlayer();

        /// Sets the player
        void                    SetPlayer( PlayerPtr _spPlayer );

        /// Sets the period
        void                    SetPeriod( Period _ePeriod );

        /// Updates the stats
        void                    UpdateStats();

        /// Callbacks
        void                    OnPeriodPushed();
        void                    OnImagePushed();
        void                    OnChangedPlayerSelection( CCtrl::ButtonGroupPtr _spButtonGroup, CGui::ButtonPtr _spNewActiveButton, CGui::ButtonPtr _spOldActiveButton );

        /// Delegates
        CGui::ObjectInputEventDelegate0Ptr  m_spDelegatePeriodPushed;
        CGui::ObjectInputEventDelegate0Ptr  m_spDelegateImagePushed;
        CCtrl::ButtonGroupButtonEventDelegate3Ptr   m_spDelegateChangedPlayerSelection;

        /// Game
        GamePtr                 m_spGame;

        /// Player
        Unsigned32              m_u32Player;
        PlayerPtr               m_spPlayer;

        /// Period to show
        Period                  m_ePeriod;

        /// Controls
        std::vector< CGui::TextBoxPtr > m_aspAxisLabels;
        BarMap                  m_mapBars;
        CGui::ButtonPtr         m_spBtPeriod;
        CGui::ButtonPtr         m_spBtImage;

        /// Player button group
        CCtrl::ButtonGroupPtr   m_spPlayerButtons;
        ButtonPlayerMap         m_mapButtonPlayers;

        /// Initial (=maximum) bar height
        Unsigned32              m_u32MaxBarHeight;
    };

    INCLUDE_SHARED_CLASS( StatsRatingsWindow );
}

#endif
