#ifndef CMADTV_BOSS_WINDOW_H
#define CMADTV_BOSS_WINDOW_H

#include "cmadtv/Windows/Window.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( BossWindowRequest );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// BossWindow
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class BossWindow : public Window
    {
    public:
        /// Returns the xml file name
        static CFoundation::String  GetXMLFileName();

        /// Constructor
        BossWindow();

        /// Destructor
        ~BossWindow();

        /// Initializes the window
        virtual void            InitWindow();

        /// Deinitializes the window
        virtual void            DeinitWindow();

        /// Sets the player
        virtual void            SetPlayer( PlayerPtr _spPlayer );

    private:
        /// Sets the current request
        void        SetRequest( BossWindowRequestPtr _spRequest );

        /// Sets the current choice
        void        SetChoice( Unsigned32 _u32Choice );

        /// Hide/Show player panel
        void        ShowPlayerPanel( bool _bShow );

        /// Callbacks
        void        OnPrevChoicePushed();
        void        OnNextChoicePushed();
        void        OnChoiceClicked();

        /// Delegates
        CGui::ObjectInputEventDelegate0Ptr  m_spDelegatePrevChoicePushed;
        CGui::ObjectInputEventDelegate0Ptr  m_spDelegateNextChoicePushed;
        CGui::ObjectInputEventDelegate0Ptr  m_spDelegateChoiceClicked;

        /// Player
        PlayerPtr           m_spPlayer;

        /// Current request
        BossWindowRequestPtr    m_spRequest;        

        /// Current choice  
        Unsigned32          m_u32Choice;

        /// Controls
        CGui::TextBoxPtr    m_spTbBoss;
        CGui::ButtonPtr     m_spBtPrevChoice;
        CGui::ButtonPtr     m_spBtNextChoice;
        CGui::GuiObjectPtr  m_spImgPlayerShadow;
        CGui::PanelPtr      m_spPnlPlayer;
        CGui::TextBoxPtr    m_spTbPlayer;
    };
}

#endif
