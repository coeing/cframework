#ifndef CMADTV_NEWSSTUDIOWINDOW_H
#define CMADTV_NEWSSTUDIOWINDOW_H

#include "cctrl/WindowController.h"

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
    INCLUDE_SHARED_CLASS( NewsChannel );
    INCLUDE_SHARED_CLASS( Player );
    INCLUDE_SHARED_CLASS( World );

    class NewsStudioWindow : public Window
    {
    public:
        /// Returns the xml file name
        static CFoundation::String  GetXMLFileName();

        /// Constructor
        NewsStudioWindow();

        /// Destructor
        ~NewsStudioWindow();

        /// Initializes the window
        virtual void            InitWindow();

        /// Deinitializes the window
        virtual void            DeinitWindow();

        /// Callback when context changed
        virtual void            OnContextChanged();

        /// Closes the window
		virtual void            Close( Unsigned32 _u32ReturnValue = 0 );

    private:
        static const Unsigned32 MAX_CHANNELS = 3;

        typedef std::vector< NewsChannelPtr > ChannelVec;
        typedef std::map< CGui::CheckBoxPtr, Unsigned32 > ObjectChannelIDMap;
        typedef std::map< Unsigned32, CGui::CheckBoxPtr > ChannelIDObjectMap;

        /// Sets the player
        virtual void            SetPlayer( PlayerPtr _spPlayer );

        /// Searches for the channel that belongs to the passed object
        NewsChannelPtr          FindChannel( CGui::GuiObjectPtr _spObject ) const;

        /// Callbacks
        void                    OnTooltipChannelStarted( CGui::TooltipControllerPtr _spTooltipController, CGui::GuiObjectPtr _spObject );
        void                    OnChannelCheckChanged( CGui::CheckBoxPtr _spCheckBox, bool _bNewState, bool _bOldState );
        void                    OnBoardClicked();

        /// Delegates
        CGui::TooltipControllerEventDelegate2Ptr    m_spDelegateTooltipChannelStarted;
        CGui::CheckBoxEventDelegate3Ptr             m_spDelegateChannelCheckChanged;
        CGui::ObjectInputEventDelegate0Ptr          m_spDelegateBoardClicked;

        /// Player
        PlayerPtr               m_spPlayer;

        /// Controls
        CGui::GuiObjectPtr      m_spObjBoard;

        /// Tooltip for news channels
        CGui::PanelPtr          m_spPnlTooltipChannel;
        CGui::TextBoxPtr        m_spTbTooltipChannelName;
        CGui::TextBoxPtr        m_spTbTooltipChannelPrice;

        /// News channel buttons
        ObjectChannelIDMap      m_mapChannelIDs;
        ChannelIDObjectMap      m_mapChannelObjects;
    };
}

#endif
