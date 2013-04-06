#ifndef CMADTV_SPECTATORS_WINDOW_H
#define CMADTV_SPECTATORS_WINDOW_H

#include "cgui/Gui/GuiObjectEvents.h"

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
    INCLUDE_SHARED_CLASS( Transmitter );

    class SpectatorsWindow : public Window
    {
    public:
        /// Returns the xml file name
        static CFoundation::String  GetXMLFileName();

        /// Constructor
        SpectatorsWindow();

        /// Destructor
        ~SpectatorsWindow();

        /// Initializes the window
        virtual void            InitWindow();

        /// Deinitializes the window
        virtual void            DeinitWindow();

        /// Processes input (when window is active)
        virtual void            ProcessInput( const CSystem::InputState& _State );

        /// Callback when context changed
        virtual void            OnContextChanged();

    private:
        enum Mode
        {
            MODE_NONE,
            MODE_BUY,
            MODE_SELL,
            MODE_END
        };

        typedef std::map< CGui::ButtonPtr, Unsigned32 > ButtonPlayerMap;
        typedef std::map< Unsigned32, CGui::ImagePtr > PlayerTransmitterIconMap;

        static const Unsigned32 MAX_NUM_SATELLITES = 2;
        static const Unsigned32 MAX_NUM_PLAYERS = 3;

        /// Sets the game
        void                    SetGame( GamePtr _spGame );

        /// Sets the player
        void                    SetPlayer( Unsigned32 _u32Player );

        /// Updates the player
        void                    UpdatePlayer();

        /// Sets the player
        void                    SetPlayer( PlayerPtr _spPlayer );

        /// Sets the mode
        void                    SetMode( Mode _eMode );

        /// Start the "buy transmitter" mode
        void                    StartBuyMode();

        /// Tries to buy an antenna at the passed position
        void                    BuyAntenna( const CFoundation::Vector2Di& _vPosition );

        /// Tries to buy the passed transmitter
        void                    BuyTransmitter( TransmitterPtr _spTransmitter );

        /// Ends the "buy transmitter" mode and tries to buy the transmitter
        /// currently pointed at
        void                    EndBuyMode( bool _bBuyTransmitter );

        /// Start the "sell" mode
        void                    StartSellMode();

        /// Selects the previous transmitter in sell mode
        void                    SelectPrevTransmitter();

        /// Selects the next transmitter in sell mode
        void                    SelectNextTransmitter();

        /// Updates the sell mode view
        void                    UpdateSelectedTransmitter();

        /// Sells the current selected transmitter
        void                    SellTransmitter();

        /// Ends the "sell" mode and sells the current selected transmitter
        void                    EndSellMode( bool _bSell );

        /// Computes the current spectators under the mouse cursor
        Unsigned32              ComputeNumNewSpectators( Unsigned32& _u32NumSpectators ) const;

        /// Shows all transmitters of the current player
        void                    ShowAllTransmitters();

        /// Shows the passed transmitter on the map
        void                    ShowTransmitter( TransmitterPtr _spTransmitter );

        /// Removes all shown transmitters
        void                    RemoveTransmitters();

        /// Sets the transmitter tooltip
        void                    SetTransmitterTooltip( bool _bVisible, TransmitterPtr _spTransmitter = TransmitterPtr() );

        /// Callbacks
        void                    OnTransmitterAdded( TransmitterPtr _spTransmitter );
        void                    OnTransmitterRemoved( TransmitterPtr _spTransmitter );

        void                    OnSatelliteEntered( CGui::GuiObjectPtr _spObject );
        void                    OnSatelliteLeft( CGui::GuiObjectPtr _spObject );
        void                    OnSatelliteClicked( CGui::GuiObjectPtr _spObject );
        void                    OnBuyActivated();
        void                    OnBuyDeactivated();
        void                    OnSellActivated();
        void                    OnSellDeactivated();
        void                    OnPrevTransmitterClicked();
        void                    OnNextTransmitterClicked();
        void                    OnSellOkClicked();
        void                    OnChangedPlayerSelection( CCtrl::ButtonGroupPtr _spButtonGroup, CGui::ButtonPtr _spNewActiveButton, CGui::ButtonPtr _spOldActiveButton );

        /// Delegates
        CFoundation::DelegateConnectionPtr  m_spDelegateTransmitterAdded;
        CFoundation::DelegateConnectionPtr  m_spDelegateTransmitterRemoved;

        CGui::ObjectInputEventDelegate1Ptr  m_spDelegateSatelliteEntered;
        CGui::ObjectInputEventDelegate1Ptr  m_spDelegateSatelliteLeft;
        CGui::ObjectInputEventDelegate1Ptr  m_spDelegateSatelliteClicked;
        CGui::StateEventDelegate0Ptr        m_spDelegateBuyActivated;
        CGui::StateEventDelegate0Ptr        m_spDelegateBuyDeactivated;
        CGui::StateEventDelegate0Ptr        m_spDelegateSellActivated;
        CGui::StateEventDelegate0Ptr        m_spDelegateSellDeactivated;
        CGui::ObjectInputEventDelegate0Ptr  m_spDelegatePrevTransmitterClicked;
        CGui::ObjectInputEventDelegate0Ptr  m_spDelegateNextTransmitterClicked;
        CGui::ObjectInputEventDelegate0Ptr  m_spDelegateSellOkClicked;        
        CCtrl::ButtonGroupButtonEventDelegate3Ptr   m_spDelegateChangedPlayerSelection;

        /// Game
        GamePtr                 m_spGame;

        /// Current shown player
        Unsigned32              m_u32Player;
        PlayerPtr               m_spPlayer;

        /// Current mode
        Mode                    m_eMode;

        /// Selected transmitter in sell mode
        Unsigned32              m_u32SelectedTransmitter;

        /// Map
        CGui::PanelPtr                  m_spPnlMap;
        PlayerTransmitterIconMap        m_mapPlayerImgTplTransmitter;
        std::vector< CGui::ImagePtr >   m_aspImgAntennaIcons;

        /// Satellites
        std::vector< CGui::ImagePtr >   m_aspImgSatellites;

        /// Controls
        CGui::ButtonPtr         m_spBtBuy;
        CGui::ButtonPtr         m_spBtSell;
        CGui::ButtonPtr         m_spBtPrevTransmitter;
        CGui::ButtonPtr         m_spBtNextTransmitter;
        CGui::ButtonPtr         m_spBtSellOk;

        /// Player button group
        CCtrl::ButtonGroupPtr   m_spPlayerButtons;
        ButtonPlayerMap         m_mapButtonPlayers;

        /// Spectators tooltip buy
        CGui::PanelPtr          m_spPnlTooltipBuy;
        CGui::TextBoxPtr        m_spTbTooltipBuySpectators;
        CGui::TextBoxPtr        m_spTbTooltipBuyPrice;

        /// Spectators tooltip sell
        CGui::PanelPtr          m_spPnlTooltipSell;
        CGui::TextBoxPtr        m_spTbTooltipSellSpectators;
        CGui::TextBoxPtr        m_spTbTooltipSellPrice;

        /// Satellite tooltip
        CGui::PanelPtr          m_spPnlTooltipSatellite;
        CGui::PanelPtr          m_spPnlTooltipSatelliteOwn;
    };
}

#endif
