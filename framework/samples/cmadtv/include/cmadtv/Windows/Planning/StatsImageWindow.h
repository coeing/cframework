#ifndef CMADTV_STATS_IMAGE_WINDOW_H
#define CMADTV_STATS_IMAGE_WINDOW_H

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
    INCLUDE_SHARED_CLASS( Transmitter );
    INCLUDE_SHARED_CLASS( World );

    enum AccountIdentifier;

    class StatsImageWindow : public Window
    {
    public:
        /// Returns the xml file name
        static CFoundation::String  GetXMLFileName();

        /// Constructor
        StatsImageWindow();

        /// Destructor
        ~StatsImageWindow();

        /// Initializes the window
        virtual void            InitWindow();

        /// Deinitializes the window
        virtual void            DeinitWindow();

        /// Processes input (when window is active)
        virtual void            ProcessInput( const CSystem::InputState& _State );

        /// Callback when context changed
        virtual void            OnContextChanged();

    private:
        typedef std::map< Unsigned32, CGui::PanelPtr > PlayerBarMap;

        /// Sets the game
        void                    SetGame( GamePtr _spGame );

        /// Updates the stats
        void                    UpdateStats();

        /// Callbacks
        void                    OnRatingsPushed();

        /// Delegates
        CGui::ObjectInputEventDelegate0Ptr  m_spDelegateRatingsPushed;

        /// Game
        GamePtr                 m_spGame;

        /// Controls
        PlayerBarMap            m_mapPlayerBars;
        CGui::ButtonPtr         m_spBtRatings;

        /// Initial (=maximum) bar height
        Unsigned32              m_u32MaxBarHeight;
    };

    INCLUDE_SHARED_CLASS( StatsImageWindow );
}

#endif
