#ifndef CMADTV_NEWSPLANNINGWINDOW_H
#define CMADTV_NEWSPLANNINGWINDOW_H

#include "cctrl/Controls/ButtonGroup.h"

#include "cmadtv/data/Player/PlayerCompNews.h"

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
    INCLUDE_SHARED_CLASS( NewsBlock );
    INCLUDE_SHARED_CLASS( NewsChannel );
    INCLUDE_SHARED_CLASS( NewsDragController );
    INCLUDE_SHARED_CLASS( NewsDropController );
    INCLUDE_SHARED_CLASS( Player );
    INCLUDE_SHARED_CLASS( World );

    class NewsPlanningWindow : public Window
    {
    public:
        /// Returns the xml file name
        static CFoundation::String  GetXMLFileName();

        /// Constructor
        NewsPlanningWindow();

        /// Destructor
        ~NewsPlanningWindow();

        /// Initializes the window
        virtual void            InitWindow();

        /// Deinitializes the window
        virtual void            DeinitWindow();

        /// Processes input (when window is active)
        virtual void            ProcessInput( const CSystem::InputState& _state );

        /// Callback when context changed
        virtual void            OnContextChanged();

    private:
        static const Unsigned32 NUM_NEWS_BLOCKS = 2;
        static const Unsigned32 MAX_CHANNELS = 3;

        typedef std::map< CGui::ButtonPtr, Unsigned32 > ObjectChannelIDMap;
        typedef std::map< Unsigned32, CGui::ButtonPtr > ChannelIDObjectMap;

        /// Sets the player
        void                    SetPlayer( PlayerPtr _spPlayer );

        /// Searches for the channel that belongs to the passed object
        NewsChannelPtr          FindChannel( CGui::ButtonPtr _spObject ) const;

        /// Sets the active channel
        void                    SetActiveChannel( NewsChannelPtr _spChannel );

        /// Sets the first news index that's shown
        void                    SetFirstNewsIdx( Unsigned32 _u32Idx );

        /// Sets the number of news
        void                    SetNumNews( Unsigned32 _u32NumNews );

        /// Updates the channel visiblity
        void                    UpdateChannelButtons() const;

        /// Updates the button visiblity
        void                    UpdateScrollButtons() const;

        /// Updates the news blocks
        void                    UpdateNewsBlocks() const;

        /// Updates the number of news
        void                    UpdateNumNews();

        /// Sets the passed news to the passed slot
        void                    SetPlannedNews( Unsigned32 _u32Slot, NewsInstancePtr _spNews );

        /// Callbacks
        void                    OnChannelSelectionChanged( CCtrl::ButtonGroupPtr _spButtonGroup, CGui::ButtonPtr _spNewActiveButton, CGui::ButtonPtr _spOldActiveButton );
        void                    OnButtonClicked( CGui::GuiObjectPtr _spButton );
        void                    OnNewsAdded( NewsInstancePtr _spNews );
        void                    OnNewsRemoved( NewsInstancePtr _spNews );
        void                    OnPlannedNewsChanged( Unsigned32 _u32Slot, NewsInstancePtr _spNews );

        /// Delegates
        CCtrl::ButtonGroupButtonEventDelegate3Ptr   m_spDelegateChannelSelectionChanged;
        CGui::ObjectInputEventDelegate1Ptr          m_spDelegateButtonClicked;
        PlayerCompNewsEventU32NewsNewsDelegate2Ptr  m_spDelegatePlannedNewsChanged;
        PlayerCompNewsEventNewsDelegate1Ptr         m_spDelegateNewsAdded;
        PlayerCompNewsEventNewsDelegate1Ptr         m_spDelegateNewsRemoved;

        /// World
        WorldPtr                m_spWorld;

        /// Player
        PlayerPtr               m_spPlayer;

        /// Active channel
        NewsChannelPtr          m_spActiveChannel;

        /// Controls
        CGui::ButtonPtr         m_spBtUp;
        CGui::ButtonPtr         m_spBtDown;

        /// Tooltip for news channels
        CGui::PanelPtr          m_spPnlTooltipChannel;
        CGui::TextBoxPtr        m_spTbTooltipChannelName;
        CGui::TextBoxPtr        m_spTbTooltipChannelPrice;

        /// Channel button group
        CCtrl::ButtonGroupPtr   m_spChannelButtons;

        /// News channel buttons
        ObjectChannelIDMap      m_mapChannelIDs;
        ChannelIDObjectMap      m_mapChannelObjects;

        /// News blocks
        NewsBlockPtr            m_aspNewsBlocks[ NUM_NEWS_BLOCKS ];

        /// News block drag controller
        NewsDragControllerPtr   m_aspNewsBlockDragControllers[ NUM_NEWS_BLOCKS ];

        /// News slots
        CGui::PanelPtr          m_aspNewsSlots[ PlayerCompNews::NUM_PLANNED_NEWS ];

        /// News block drag controller
        NewsDropControllerPtr   m_aspNewsSlotDropControllers[ PlayerCompNews::NUM_PLANNED_NEWS ];        

        /// Planned news blocks
        NewsBlockPtr            m_aspPlannedNewsBlocks[ PlayerCompNews::NUM_PLANNED_NEWS ];

        /// Planned news block drag controller
        NewsDragControllerPtr   m_aspPlannedNewsBlockDragControllers[ PlayerCompNews::NUM_PLANNED_NEWS ];

        /// Current news offset
        Unsigned32              m_u32FirstNewsIdx;

        /// Current number of news
        Unsigned32              m_u32NumNews;
    };

    INCLUDE_SHARED_CLASS( NewsPlanningWindow );
}

#endif
