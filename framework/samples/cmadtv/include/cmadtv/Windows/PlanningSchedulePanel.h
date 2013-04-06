#ifndef CMADTV_PLANNINGSCHEDULEPANEL_H
#define CMADTV_PLANNINGSCHEDULEPANEL_H

#include "cfoundation/DateTime/Date.h"

#include "cgui/DragDrop/DragController.h"
#include "cgui/Gui/Image.h"
#include "cgui/Gui/Panel.h"

#include "cmadtv/Data/Player/Schedule.h"
#include "cmadtv/Data/World/Hour.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( DropController );
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Advert );
    INCLUDE_SHARED_CLASS( AdvertBlock );
    INCLUDE_SHARED_CLASS( Broadcast );
    INCLUDE_SHARED_CLASS( BroadcastBlock );
    INCLUDE_SHARED_CLASS( BroadcastDropController );
    INCLUDE_SHARED_CLASS( Game );
    INCLUDE_SHARED_CLASS( Player );
    INCLUDE_SHARED_CLASS( PlayerContext );
    INCLUDE_SHARED_CLASS( Schedule );

    enum ScheduleDay;

    class PlanningSchedulePanel
    {
    public:
        /// Constructor
        PlanningSchedulePanel();

        /// Destructor
        ~PlanningSchedulePanel();

        /// Initialization
        void                    Init( CGui::PanelPtr _spPanel );

        /// Deinitialization
        void                    Deinit();

        /// Sets the context
        void                    SetContext( PlayerContextPtr _spContext );

        /// Sets the schedule this panel shows
        void                    SetSchedule( ScheduleDay _eScheduleDay );

    private:
        /// Sets the game
        void                    SetGame( GamePtr _spGame );

        /// Sets the player
        void                    SetPlayer( PlayerPtr _spPlayer );

        /// Updates the schedule
        void                    UpdateSchedule();

        /// Returns the shown date
        CFoundation::Date       GetShownDate() const;

        /// Sets the schedule this panel shows
        void                    SetSchedule( SchedulePtr _spSchedule );

        /// Updates the active state of the slots and blocks
        void                    UpdateActiveStates();

        /// Updates the sub texts of the adverts
        void                    UpdateAdvertSubTexts();

        /// Updates the sub texts of the passed advert block
        void                    UpdateAdvertSubText( AdvertBlockPtr _spBlock, Hour _eHour );

        /// Callbacks
        void                    OnDateChanged();
        void                    OnTimeChanged();

        /// Delegate when broadcast was added to schedule
        void                    OnBroadcastAdded( BroadcastPtr _spBroadcast, Hour _eStartSlot );

        /// Delegate when broadcast was removed from schedule
        void                    OnBroadcastRemoved( BroadcastPtr _spBroadcast, Hour _eStartSlot );

        /// Delegate when a broadcast block was started to drag
        void                    OnBroadcastDragStarted( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spObject, CGui::DragDataPtr _spDragData );

        /// Delegate when a broadcast block was undone to drag
        void                    OnBroadcastDragUndone( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spObject, CGui::DragDataPtr _spDragData );

        /// Delegate when a broadcast block was ended to drag
        void                    OnBroadcastDragEnded( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spObject, CGui::DragDataPtr _spDragData );
        
        /// Delegate when an advert was added to schedule
        void                    OnAdvertAdded( AdvertPtr _spAdvert, Hour _eSlot );

        /// Delegate when an advert was removed from schedule
        void                    OnAdvertRemoved( AdvertPtr _spAdvert, Hour _eSlot );

        /// Delegate when a advert block was started to drag
        void                    OnAdvertDragStarted( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spObject, CGui::DragDataPtr _spDragData );

        /// Delegate when a advert block was undone to drag
        void                    OnAdvertDragUndone( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spObject, CGui::DragDataPtr _spDragData );

        /// Delegate when a advert block was ended to drag
        void                    OnAdvertDragEnded( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spObject, CGui::DragDataPtr _spDragData );

        /// Add a broadcast block for the passed broadcast at the passed time
        BroadcastBlockPtr       AddBroadcastBlock( BroadcastPtr _spBroadcast, Hour _eStartSlot );

        /// Removes a broadcast block from the passed time
        void                    RemoveBroadcastBlock( Hour _eStartSlot );

        /// Returns the broadcast block at the passed time
        BroadcastBlockPtr       GetBroadcastBlock( Hour _eSlot ) const;

        /// Add an advert block for the passed broadcast at the passed time
        void                    AddAdvertBlock( AdvertPtr _spAdvert, Hour _eSlot );

        /// Removes an advert block from the passed time
        void                    RemoveAdvertBlock( Hour _eSlot );

        /// Returns the broadcast block at the passed time
        AdvertBlockPtr          GetAdvertBlock( Hour _eSlot ) const;

        /// Delegates
        CFoundation::DelegateConnectionPtr  m_spDelegateHourStageChanged;
        CFoundation::DelegateConnectionPtr  m_spDelegateDateChanged;

        ScheduleBroadcastEventDelegatePtr   m_spDelegateBroadcastAdded;
        ScheduleBroadcastEventDelegatePtr   m_spDelegateBroadcastRemoved;
        CGui::DragEventDelegatePtr          m_spDelegateBroadcastDragStarted;
        CGui::DragEventDelegatePtr          m_spDelegateBroadcastDragUndone;
        CGui::DragEventDelegatePtr          m_spDelegateBroadcastDragEnded;

        ScheduleAdvertEventDelegatePtr      m_spDelegateAdvertAdded;
        ScheduleAdvertEventDelegatePtr      m_spDelegateAdvertRemoved;
        CGui::DragEventDelegatePtr          m_spDelegateAdvertDragStarted;
        CGui::DragEventDelegatePtr          m_spDelegateAdvertDragUndone;
        CGui::DragEventDelegatePtr          m_spDelegateAdvertDragEnded;

        /// Gui panel
        CGui::PanelPtr          m_spPnlRoot;

        /// Empty slots
        CGui::ImagePtr          m_aspTimeSlot[ HOUR_END ];
        CGui::ImagePtr          m_aspBroadcastSlot[ HOUR_END ];
        CGui::ImagePtr          m_aspAdvertSlot[ HOUR_END ];

        /// Context
        PlayerContextPtr        m_spContext;

        /// Game
        GamePtr                 m_spGame;

        /// Player
        PlayerPtr               m_spPlayer;

        /// Current day that's shown
        ScheduleDay             m_eScheduleDay;
        SchedulePtr             m_spSchedule;

        /// Blocks
        BroadcastBlockPtr       m_aspBroadcastBlocks[ HOUR_END ];
        AdvertBlockPtr          m_aspAdvertBlocks[ HOUR_END ];

        /// Drop controller
        BroadcastDropControllerPtr  m_aspBroadcastSlotDropController[ HOUR_END ];
        CGui::DropControllerPtr m_aspBroadcastDropController[ HOUR_END ];
        CGui::DropControllerPtr m_aspAdvertSlotDropController[ HOUR_END ];
        CGui::DropControllerPtr m_aspAdvertDropController[ HOUR_END ];
    };

    typedef SharedPtr< PlanningSchedulePanel > PlanningSchedulePanelPtr;
}

#endif
