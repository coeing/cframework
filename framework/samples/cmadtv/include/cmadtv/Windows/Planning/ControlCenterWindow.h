#ifndef CMADTV_CONTROL_CENTER_WINDOW_H
#define CMADTV_CONTROL_CENTER_WINDOW_H

#include "cfoundation/Math/Vector.h"

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

    class ControlCenterWindow : public Window
    {
    public:
        /// Returns the xml file name
        static CFoundation::String  GetXMLFileName();

        /// Constructor
        ControlCenterWindow();

        /// Destructor
        ~ControlCenterWindow();

        /// Initializes the window
        virtual void            InitWindow();

        /// Deinitializes the window
        virtual void            DeinitWindow();

        /// Processes input (when window is active)
        virtual void            ProcessInput( const CSystem::InputState& _State );

        /// Callback when context changed
        virtual void            OnContextChanged();

    private:
        typedef std::map< CGui::ButtonPtr, Float32 > ButtonSpeedMap;

        /// Sets the game
        void                    SetGame( GamePtr _spGame );

        /// Callbacks
        void                    OnFamilyActivated();
        void                    OnFamilyDeactivated();
        void                    OnPictureActivated();
        void                    OnPictureDeactivated();
        void                    OnMusicActivated();
        void                    OnMusicDeactivated();
        void                    OnChangedSpeedSelection( CCtrl::ButtonGroupPtr _spButtonGroup, CGui::ButtonPtr _spNewActiveButton, CGui::ButtonPtr _spOldActiveButton );

        /// Delegates
        CGui::StateEventDelegate0Ptr  m_spDelegateFamilyActivated;
        CGui::StateEventDelegate0Ptr  m_spDelegateFamilyDeactivated;
        CGui::StateEventDelegate0Ptr  m_spDelegatePictureActivated;
        CGui::StateEventDelegate0Ptr  m_spDelegatePictureDeactivated;
        CGui::StateEventDelegate0Ptr  m_spDelegateMusicActivated;
        CGui::StateEventDelegate0Ptr  m_spDelegateMusicDeactivated;
        CCtrl::ButtonGroupButtonEventDelegate3Ptr   m_spDelegateChangedPlayerSelection;

        /// Game
        GamePtr                 m_spGame;

        /// Controls
        CGui::ButtonPtr         m_spBtFamily;
        CGui::ButtonPtr         m_spBtPicture;
        CGui::ButtonPtr         m_spBtMusic;

        /// Speed button group
        CCtrl::ButtonGroupPtr   m_spSpeedButtons;
        ButtonSpeedMap          m_mapButtonSpeeds;
    };

    INCLUDE_SHARED_CLASS( ControlCenterWindow );
}

#endif
