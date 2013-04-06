#ifndef CMADTV_INFOWINDOW_H
#define CMADTV_INFOWINDOW_H

#include "CFoundation/Finance/Account.h"

#include "cmadtv/Windows/Window.h"

namespace CFoundation
{
    INCLUDE_SHARED_CLASS( DelegateConnection );
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Game );
    INCLUDE_SHARED_CLASS( Player );

    enum PlayerCompBettyEventU8U8;
    TYPEDEF_DELEGATE2( PlayerCompBettyEventU8U8, Unsigned8, Unsigned8 );

    class InfoWindow : public Window
    {
    public:
        //! Returns the xml file name
        static CFoundation::String  GetXMLFileName();        

        //! Constructor
        InfoWindow();

        /// Initializes the window
        virtual void            InitWindow();

        /// Deinitializes the window
        virtual void            DeinitWindow();

        /// Processes input (when window is active)
        virtual void            ProcessInput( const CSystem::InputState& _State );

        /// Per frame update
        virtual void            Update( Float32 _f32TimeDifference );

        /// Callback when context changed
        virtual void            OnContextChanged();

        /// Sets the fps to show
        void                    SetFPS( Float32 _f32FPS );

    private:
        /// Sets the game
        void                    SetGame( GamePtr _spGame );

        /// Sets the player for the info window
        void                    SetPlayer( PlayerPtr _spPlayer );

        /// Callbacks
        void                    OnTimeChanged();
        void                    OnDateChanged();
        void                    OnCashChanged( const CFoundation::Money& _clOldBalance, const CFoundation::Money& _clNewBalance );
        void                    OnCreditChanged( const CFoundation::Money& _clOldCredit, const CFoundation::Money& _clNewCredit );
        void                    OnLoveChanged( Unsigned8 _u8NewLove, Unsigned8 _u8OldLove );

        /// Delegates
        CFoundation::DelegateConnectionPtr      m_spDelegateTimeChanged;
        CFoundation::DelegateConnectionPtr      m_spDelegateDateChanged;
        CFoundation::DelegateConnectionPtr      m_spDelegateCashChanged;
        PlayerCompBettyEventU8U8Delegate2Ptr    m_spDelegateLoveChanged;

        /// Player this info window is for
        PlayerPtr               m_spPlayer;

        /// Game
        GamePtr                 m_spGame;

		/// Framerate
        CGui::TextBoxPtr        m_spTbFrameRate;

        /// Cash
        CGui::TextBoxPtr        m_spTbBudget;

        /// Spectators
        CGui::TextBoxPtr        m_spTbSpectators;

        /// Love progress bar
        CGui::ProgressBarPtr    m_spPbLove;

        /// Time
        CGui::TextBoxPtr        m_spTbTime;

        /// Tooltips
        CGui::PanelPtr          m_spPnlTooltipBudget;
        CGui::TextBoxPtr        m_spTbCash;
        CGui::TextBoxPtr        m_spTbCredit;

        CGui::PanelPtr          m_spPnlTooltipSpectators;
        CGui::TextBoxPtr        m_spTbMaxViewers;
        CGui::TextBoxPtr        m_spTbCurrentRating;
        CGui::TextBoxPtr        m_spTbNextAdvert;

        CGui::PanelPtr          m_spPnlTooltipTime;
        CGui::TextBoxPtr        m_spTbDay;
    };
}

#endif
