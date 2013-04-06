#include "cmadtv/Windows/PlanningArchivWindow.h"

#include "cfoundation/Debug/Makros.h"

#include "cgui/Gui/Image.h"
#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/Panel.h"
#include "cgui/Gui/TextBox.h"
#include "cgui/Input/Input.h"

#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Player/BroadcastRack.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerContext.h"

#include "cmadtv/Input/InputTranslator.h"
#include "cmadtv/Windows/PlanningRackShelfWindow.h"

using namespace CMadTV;
        
CFoundation::String PlanningArchivWindow::GetXMLFileName()
{
    return "data/windows/PlanningArchivWindow.xml";
}

PlanningArchivWindow::PlanningArchivWindow()
{
    m_spDelegateShelfClicked = GenrePanelEventDelegate1Ptr( new GenrePanelEventDelegate1( this, &PlanningArchivWindow::OnShelfClicked ) );
}

PlanningArchivWindow::~PlanningArchivWindow()
{
}

void PlanningArchivWindow::InitWindow()
{
    // Get controls
    m_spPnlTemplateGenre = GetPanel( "PnlTemplateGenre" );
    m_spPnlTemplateGenre->RemoveFromParent();
    m_spObjGenresAnchor = GetGuiObject( "ObjGenresAnchor" );

    WindowController::InitWindow();
}
		
void PlanningArchivWindow::DeinitWindow()
{
    // Close current open rack shelf window
    if ( m_spRackShelfWindow != NULL )
    {
        m_spRackShelfWindow->Close();
        m_spRackShelfWindow.reset();
    }

    // Deinit and remove genre panels
    for ( GenrePanelVec::iterator it = m_aspGenrePanels.begin(); it != m_aspGenrePanels.end(); ++it )
    {
        GenrePanelPtr spGenrePanel = (*it);

        // Remove delegate
        spGenrePanel->RemoveDelegate( PLANNING_ARCHIV_EVENT_GENRE_CLICKED, m_spDelegateShelfClicked );

        spGenrePanel->Deinit();
    }
    m_aspGenrePanels.clear();

    // Delete template
    CGui::GuiMain::GetInstance().Delete( m_spPnlTemplateGenre );
    m_spPnlTemplateGenre.reset();

    WindowController::DeinitWindow();
}

void PlanningArchivWindow::SetDelegateShelfClicked( GenrePanelEventDelegate1Ptr _spDelegateShelfClicked )
{
    if ( _spDelegateShelfClicked == m_spDelegateShelfClicked )
    {
        return;
    }

    if ( m_spDelegateShelfClicked != NULL )
    {
        for ( GenrePanelVec::iterator it = m_aspGenrePanels.begin(); it != m_aspGenrePanels.end(); ++it )
        {
            GenrePanelPtr spPnlGenre = (*it);
            spPnlGenre->RemoveDelegate( PLANNING_ARCHIV_EVENT_GENRE_CLICKED, m_spDelegateShelfClicked );
        }
    }

    m_spDelegateShelfClicked = _spDelegateShelfClicked;

    if ( m_spDelegateShelfClicked != NULL )
    {
        for ( GenrePanelVec::iterator it = m_aspGenrePanels.begin(); it != m_aspGenrePanels.end(); ++it )
        {
            GenrePanelPtr spPnlGenre = (*it);
            spPnlGenre->RegisterDelegate( PLANNING_ARCHIV_EVENT_GENRE_CLICKED, m_spDelegateShelfClicked );
        }
    }
}
        
void PlanningArchivWindow::ProcessInput( const CSystem::InputState& _state )
{
    // Check if the window should be closed
    if ( InputTranslator::GetInstance().WasExecuted( INPUT_COMMAND_PLANNING_BROADCAST_ARCHIV_WINDOW_CLOSE, _state ) )
    {
        // Only close if no rack shelf window is open
        if ( m_spRackShelfWindow == NULL )
        {
            Close();
        }
    }
}
		
void PlanningArchivWindow::OnCloseScreen( const CCtrl::ScreenClosedMsg& _clMessage )
{
    if ( _clMessage.GetScreen() == m_spRackShelfWindow )
    {
        m_spRackShelfWindow.reset();
    }
}
        
void PlanningArchivWindow::OnContextChanged()
{
    // Create genres
    DatabasePtr spDatabase = m_spContext != NULL ? m_spContext->GetDatabase() : DatabasePtr();
    Unsigned32 u32NumGenres = spDatabase != NULL ? spDatabase->GetNumGenres() : 0;
    for ( Unsigned32 u32ID = 0; u32ID < u32NumGenres; ++u32ID )
    {
        GenrePtr spGenre = spDatabase->GetGenre( u32ID );

        GenrePanelPtr spGenrePanel = CreateGenrePanel();
        spGenrePanel->SetGenre( spGenre );

        // Set number of broadcasts per genre
        if ( m_spPlayer != NULL )
        {
            spGenrePanel->SetNumBroadcasts( m_spPlayer->GetBroadcastRack().GetNumBroadcasts( spGenre ) );
        }

        // Register delegate
        spGenrePanel->RegisterDelegate( PLANNING_ARCHIV_EVENT_GENRE_CLICKED, m_spDelegateShelfClicked );

        m_aspGenrePanels.push_back( spGenrePanel );
    }

    SetPlayer( m_spContext != NULL ? m_spContext->GetPlayer() : PlayerPtr() );
}

void PlanningArchivWindow::SetPlayer( PlayerPtr _spPlayer )
{
    m_spPlayer = _spPlayer;

    if ( m_spPlayer != NULL )
    {
        const BroadcastRack& clRack = m_spPlayer->GetBroadcastRack();
        // Set number of broadcasts per genre
        for ( GenrePanelVec::iterator it = m_aspGenrePanels.begin(); it != m_aspGenrePanels.end(); ++it )
        {
            GenrePanelPtr spGenrePanel = (*it);
        
            spGenrePanel->SetNumBroadcasts( clRack.GetNumBroadcasts( spGenrePanel->GetGenre() ) );
        }
    }
}
        
void PlanningArchivWindow::OnShelfClicked( GenrePtr _spGenre )
{
    // Open rack shelf window
    m_spRackShelfWindow = Create< PlanningRackShelfWindow >( GetSharedPtr() );
    m_spRackShelfWindow->Init( m_spPlayer, _spGenre );
}
       
PlanningArchivWindow::GenrePanelPtr PlanningArchivWindow::CreateGenrePanel()
{
    // Create gui elements
    CGui::PanelPtr spGUIPanel = CGui::GuiMain::GetInstance().CreateFromTemplate( "PnlGenreX", m_spPnlTemplateGenre );
    m_spObjGenresAnchor->GetParent()->AddChildAfter( spGUIPanel, m_spObjGenresAnchor );
    m_spObjGenresAnchor = spGUIPanel;

    GenrePanelPtr spGenrePanel( new GenrePanel() );
    spGenrePanel->Init( spGUIPanel );
    
    return spGenrePanel;
}

PlanningArchivWindow::GenrePanel::GenrePanel()
{
    m_spDelegateClicked = CGui::ObjectInputEventDelegate1Ptr( new CGui::ObjectInputEventDelegate1( this, &PlanningArchivWindow::GenrePanel::OnClicked ) );
}

PlanningArchivWindow::GenrePanel::~GenrePanel()
{
}

void PlanningArchivWindow::GenrePanel::Init( CGui::PanelPtr _spGuiPanel )
{
    m_spPnlRoot = _spGuiPanel;
    m_spTbGenre = static_pointer_cast< CGui::TextBox >( m_spPnlRoot->FindChild( "TbGenre" ) );
    CF_WARN_IF( m_spTbGenre == NULL, L"PlanningArchivWindow::GenrePanel::Init: Found no TbGenre textbox in genre panel" );
    if ( m_spTbGenre != NULL )
    {
        m_spTbGenre->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClicked );
    }

    m_spImgShelf = static_pointer_cast< CGui::Image >( m_spPnlRoot->FindChild( "ImgShelf" ) );
    CF_WARN_IF( m_spImgShelf == NULL, L"PlanningArchivWindow::GenrePanel::Init: Found no ImgShelf image in genre panel" );
    if ( m_spImgShelf != NULL )
    {
        m_spImgShelf->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClicked );
    }
}
        
void PlanningArchivWindow::GenrePanel::Deinit()
{
    // Remove listener from genre textbox
    if ( m_spTbGenre != NULL )
    {
        m_spTbGenre->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClicked );
    }

    // Remove listener from shelf image
    if ( m_spImgShelf != NULL )
    {
        m_spImgShelf->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClicked );
    }

    m_spPnlRoot.reset();
}

void PlanningArchivWindow::GenrePanel::SetGenre( GenrePtr _spGenre )
{
    m_spGenre = _spGenre;

    // Set name
    if ( m_spTbGenre != NULL )
    {
        m_spTbGenre->SetText( m_spGenre->GetName() );
    }
}

GenrePtr PlanningArchivWindow::GenrePanel::GetGenre() const
{
    return m_spGenre;
}
        
void PlanningArchivWindow::GenrePanel::SetNumBroadcasts( Unsigned32 _u32NumBroadcasts )
{
    if ( m_spImgShelf == NULL )
    {
        return;
    }

    // Choose right texture
    CFoundation::String strTexture;
    strTexture.Format( "data/art/windows/PlanningArchivWindow/button_%d.tif", std::min< Unsigned32 >( _u32NumBroadcasts, 4 ) );

    m_spImgShelf->SetTexture( strTexture );
}
        
void PlanningArchivWindow::GenrePanel::OnClicked( CGui::GuiObjectPtr _spObject )
{
    // Dispatch event
    DispatchEvent( PLANNING_ARCHIV_EVENT_GENRE_CLICKED, m_spGenre );
}
