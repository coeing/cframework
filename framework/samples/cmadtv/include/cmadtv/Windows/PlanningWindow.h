#ifndef CMADTV_PLANNINGWINDOW_H
#define CMADTV_PLANNINGWINDOW_H

#include "cgui/Gui/GuiObjectEvents.h"

#include "cmadtv/Data/World/WorldEvents.h"
#include "cmadtv/Windows/Window.h"

namespace CFoundation
{
    INCLUDE_SHARED_CLASS( DelegateConnection );
}

namespace CCtrl
{
    INCLUDE_SHARED_CLASS( ButtonGroup );
    TYPEDEF_DELEGATE3( ButtonGroupButtonEvent, ButtonGroupPtr, CGui::ButtonPtr, CGui::ButtonPtr );
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( BlockTrashDropController );
    INCLUDE_SHARED_CLASS( PlanningSchedulePanel );

    enum ScheduleDay;

    class PlanningWindow : public Window
    {
    public:
        /// Returns the xml file name
        static CFoundation::String  GetXMLFileName();

        /// Constructor
        PlanningWindow();

        /// Destructor
        ~PlanningWindow();

        /// Initializes the window
        virtual void            InitWindow();

        /// Deinitializes the window
        virtual void            DeinitWindow();

        /// Called when a screen was closed
        virtual void            OnCloseScreen( const CCtrl::ScreenClosedMsg& _clMessage );

        /// Callback when context changed
        virtual void            OnContextChanged();

    private:
        /// Sets the game
        void                        SetGame( GamePtr _spGame );

        /// Sets the player
        virtual void                SetPlayer( PlayerPtr _spPlayer );

        /// Sets the schedule that is shwon
        void                        SetSchedule( ScheduleDay _eScheduleDay );

        /// Callbacks
        void                        OnChangedSchedule( CCtrl::ButtonGroupPtr _spButtonGroup );
        void                        OnClickedFunction( CGui::GuiObjectPtr _spObject );

        /// Delegates
        CCtrl::ButtonGroupButtonEventDelegate1Ptr   m_spDelegateChangedSchedule;
        CGui::ObjectInputEventDelegate1Ptr          m_spDelegateClickedFunction;

        /// Game
        GamePtr                     m_spGame;

        /// Player
        PlayerPtr                   m_spPlayer;

        /// Current schedule
        ScheduleDay                 m_eScheduleDay;

        /// Schedule button group
        CCtrl::ButtonGroupPtr       m_spScheduleButtons;

        /// Controls
        CGui::ImagePtr              m_spImgControlCenter;
        CGui::ImagePtr              m_spImgAdvertArchiv;
        CGui::ImagePtr              m_spImgBroadcastArchiv;
        CGui::ImagePtr              m_spImgStatsBudget;
        CGui::ImagePtr              m_spImgStatsImage;
        CGui::ImagePtr              m_spImgTrash;

        /// Schedule panel controller
        PlanningSchedulePanelPtr    m_spSchedulePanel;

        /// Trash drop controller
        BlockTrashDropControllerPtr m_spTrashDropController;

        /// Current open sub window
        WindowPtr                   m_spSubWindow;
    };
}

#endif
