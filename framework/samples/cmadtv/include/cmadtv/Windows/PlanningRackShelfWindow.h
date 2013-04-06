#ifndef CMADTV_PLANNINGRACKSHELFWINDOW_H
#define CMADTV_PLANNINGRACKSHELFWINDOW_H

#include "cmadtv/Windows/Window.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Broadcast );
    INCLUDE_SHARED_CLASS( Genre );
    INCLUDE_SHARED_CLASS( Player );
    INCLUDE_SHARED_CLASS( PlayerContext );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// PlanningRackShelfWindow
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class PlanningRackShelfWindow : public Window
    {
    public:
        //! Returns the xml file name
        static CFoundation::String  GetXMLFileName();

        //! Constructor
        PlanningRackShelfWindow();

        //! Destructor
        ~PlanningRackShelfWindow();

        //! Initializes the window
        virtual void            InitWindow();

        /// Initialization
        void                    Init( PlayerPtr _spPlayer, GenrePtr _spGenre );

        /// Deinitializes the window
		virtual void            DeinitWindow();

        /// Processes input (when window is active)
        virtual void            ProcessInput( const CSystem::InputState& _state );

        //! Closes the window
		virtual void            Close();

    private:
        /// Broadcast panel
        INCLUDE_SHARED_CLASS( BroadcastPanel );
        typedef std::vector< BroadcastPanelPtr > BroadcastPanelVec;

        /// Creates a new broadcast panel
        BroadcastPanelPtr   CreateBroadcastPanel();

        /// Player
        PlayerPtr           m_spPlayer;

        /// Genre
        GenrePtr            m_spGenre;

        /// Genre title textbox
        CGui::TextBoxPtr    m_spTbGenre;

        //! Broadcast panel template
        CGui::PanelPtr      m_spPnlTemplateBroadcast;

        //! Anchor for next broadcast panel
        CGui::GuiObjectPtr  m_spObjBroadcastsAnchor;

        /// Broadcast panels
        BroadcastPanelVec   m_aspBroadcastPanels;
    };

    INCLUDE_SHARED_CLASS( BroadcastTooltipController );
    INCLUDE_SHARED_CLASS( BroadcastDragController );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// PlanningRackShelfWindow::BroadcastPanel
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class PlanningRackShelfWindow::BroadcastPanel
    {
    public:
        /// Constructor
        BroadcastPanel();

        /// Destructor
        ~BroadcastPanel();

        /// Initialization
        void                Init( CGui::PanelPtr _spGuiPanel );

        /// Sets the context
        void                SetContext( PlayerContextPtr _spContext );

        /// Sets the broadcast
        void                SetBroadcast( BroadcastPtr _spBroadcast );

    private:
        /// Gui panel
        CGui::PanelPtr      m_spPnlRoot;

        /// Title
        CGui::TextBoxPtr    m_spTbTitle;

        /// Context
        PlayerContextPtr    m_spContext;

        /// Broadcast
        BroadcastPtr        m_spBroadcast;

        /// Tooltip controller
        BroadcastTooltipControllerPtr   m_spTooltipController;

        /// Drag controller
        BroadcastDragControllerPtr      m_spDragController;
    };

    typedef SharedPtr< PlanningRackShelfWindow > PlanningRackShelfWindowPtr;
}

#endif
