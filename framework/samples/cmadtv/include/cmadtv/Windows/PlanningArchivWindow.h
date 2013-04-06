#ifndef CMADTV_PLANNINGARCHIVWINDOW_H
#define CMADTV_PLANNINGARCHIVWINDOW_H

#include "csystem/Input/Input.h"

#include "cgui/Gui/GuiObjectEvents.h"

#include "cmadtv/Data/Genre.h"
#include "cmadtv/Windows/Window.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( PlanningRackShelfWindow );
    INCLUDE_SHARED_CLASS( Player );
    INCLUDE_SHARED_CLASS( World );

    enum GenrePanelEvent
    {
        PLANNING_ARCHIV_EVENT_NONE,

        PLANNING_ARCHIV_EVENT_GENRE_CLICKED,

        PLANNING_ARCHIV_EVENT_END
    };
    TYPEDEF_DELEGATE1( GenrePanelEvent, GenrePtr );

    class PlanningArchivWindow : public Window
    {
    public:
        //! Returns the xml file name
        static CFoundation::String  GetXMLFileName();

        //! Constructor
        PlanningArchivWindow();

        //! Destructor
        ~PlanningArchivWindow();

        //! Initializes the window
        virtual void            InitWindow();

        /// Deinitializes the window
		virtual void            DeinitWindow();

        /// Sets the delegate that's called when a shelf was clicked
        void                    SetDelegateShelfClicked( GenrePanelEventDelegate1Ptr _spDelegateShelfClicked );

        /// Processes input (when window is active)
        virtual void            ProcessInput( const CSystem::InputState& _state );

        /// Called when a screen was closed
        virtual void            OnCloseScreen( const CCtrl::ScreenClosedMsg& _clMessage );

        /// Callback when context changed
        virtual void            OnContextChanged();

    private:
        /// Genre panel
        class GenrePanel;
        typedef SharedPtr< GenrePanel > GenrePanelPtr;
        typedef std::vector< GenrePanelPtr > GenrePanelVec;

        /// Sets the player
        void                    SetPlayer( PlayerPtr _spPlayer );

        /// Creates a new genre panel
        GenrePanelPtr           CreateGenrePanel();

        /// Callback when shelf was selected
        void                    OnShelfClicked( GenrePtr _pGenre );

        /// Delegate when genre was clicked
        GenrePanelEventDelegate1Ptr     m_spDelegateShelfClicked;

        /// World
        WorldPtr                        m_spWorld;

        /// Player
        PlayerPtr                       m_spPlayer;

        //! Genre panel template
        CGui::PanelPtr                  m_spPnlTemplateGenre;

        //! Genre anchor
        CGui::GuiObjectPtr              m_spObjGenresAnchor;

        /// Genre panels
        GenrePanelVec                   m_aspGenrePanels;

        /// Current open rack shelf window
        PlanningRackShelfWindowPtr      m_spRackShelfWindow;
    };

    INCLUDE_SHARED_CLASS( PlanningArchivWindow );

    class PlanningArchivWindow::GenrePanel : public CFoundation::Delegate1Invoker< GenrePanelEvent, GenrePtr >
    {
    public:
        DELEGATE1_INVOKER( GenrePanelEvent, GenrePtr );

        /// Constructor
        GenrePanel();

        /// Destructor
        ~GenrePanel();

        /// Initialization
        void                Init( CGui::PanelPtr _spGuiPanel );

        /// Deinitialization
        void                Deinit();

        /// Sets the genre
        void                SetGenre( GenrePtr _spGenre );

        /// Returns the genre
        GenrePtr            GetGenre() const;

        /// Sets the number of broadcast of this genre
        void                SetNumBroadcasts( Unsigned32 _u32NumBroadcasts );

    private:
        /// Callback when the shelf image was clicked
        void                    OnClicked( CGui::GuiObjectPtr _spObject );

        /// Gui panel
        CGui::PanelPtr          m_spPnlRoot;

        /// Shelf image
        CGui::ImagePtr          m_spImgShelf;

        /// Textbox
        CGui::TextBoxPtr        m_spTbGenre;

        /// Genre
        GenrePtr                m_spGenre;

        /// Input event
        CGui::ObjectInputEventDelegate1Ptr   m_spDelegateClicked;
    };
}

#endif
