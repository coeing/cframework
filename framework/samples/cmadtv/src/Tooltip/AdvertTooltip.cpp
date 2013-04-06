#include "cmadtv/Tooltip/AdvertTooltip.h"

#include "cfoundation/Debug/Makros.h"

#include "cgui/Gui/GuiMain.h"

#include "cmadtv/Data/Advert.h"
#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Genre.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Player/AdvertContract.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerCompSpectators.h"
#include "cmadtv/Data/Player/PlayerContext.h"

using namespace CMadTV;

AdvertTooltipPtr AdvertTooltip::Create()
{
    // Create panel
    CGui::PanelPtr spPnlBlock = CGui::GuiMain::GetInstance().CreateFromXML< CGui::Panel >( "data/windows/AdvertTooltip.xml" );
    if ( spPnlBlock == NULL )
    {
        return AdvertTooltipPtr();
    }

    // Create Advert tooltip
    AdvertTooltipPtr spAdvertTooltip( new AdvertTooltip( spPnlBlock ) );
    return spAdvertTooltip;
}

AdvertTooltip::AdvertTooltip( CGui::PanelPtr _spPnlRoot )
{
    // Store pointer to root panel
    m_spPnlRoot = _spPnlRoot;

    // Get controls
    m_spTbName = static_pointer_cast< CGui::TextBox >( m_spPnlRoot->FindChild( "TbName" ) );
    m_spTbSlogan = static_pointer_cast< CGui::TextBox >( m_spPnlRoot->FindChild( "TbSlogan" ) );
    m_spTbBonus = static_pointer_cast< CGui::TextBox >( m_spPnlRoot->FindChild( "TbBonus" ) );
    m_spTbPenalty = static_pointer_cast< CGui::TextBox >( m_spPnlRoot->FindChild( "TbPenalty" ) );
    m_spTbCondition = static_pointer_cast< CGui::TextBox >( m_spPnlRoot->FindChild( "TbCondition" ) );
    m_spTbSpotsDays = static_pointer_cast< CGui::TextBox >( m_spPnlRoot->FindChild( "TbSpotsDays" ) );
}

AdvertTooltip::~AdvertTooltip()
{
    CGui::GuiMain::GetInstance().Delete( m_spPnlRoot );
}
        
void AdvertTooltip::Set( PlayerContextPtr _spContext, AdvertPtr _spAdvert )
{
    if ( _spContext == NULL ||
         _spAdvert == NULL )
    {
        Clear();
        return;
    }

    // Get player
    PlayerPtr spPlayer = _spContext->GetPlayer();

    // Get contract
    AdvertContractPtr spContract = spPlayer->GetAdvertContracts().GetContract( _spAdvert );

    // Set name
    if ( m_spTbName != NULL )
    {
        m_spTbName->SetText( _spAdvert->GetName() );
    }

    // Set slogan
    if ( m_spTbSlogan != NULL )
    {
        m_spTbSlogan->SetText( _spAdvert->GetSlogan() );
    }

    // Set bonus
    if ( m_spTbBonus != NULL )
    {
        CFoundation::String strBonusText;
        strBonusText.Format( L"Bonus: $ %.0f", _spAdvert->GetBonus().GetAmount( CFoundation::CURRENCY_DOLLAR ) );
        m_spTbBonus->SetText( strBonusText );
    }

    // Set penalty
    if ( m_spTbPenalty != NULL )
    {
        CFoundation::String strText;
        strText.Format( L"Strafe: $ %.0f", _spAdvert->GetPenalty().GetAmount( CFoundation::CURRENCY_DOLLAR ) );
        m_spTbPenalty->SetText( strText );
    }

    // Set condition
    if ( m_spTbCondition != NULL )
    {
        CFoundation::String strSpectators = CFoundation::String::Build( "%.2f", _spAdvert->GetMinSpectators() / 1000000.0f );
        Float32 f32Percentage = 100.0f;
        Unsigned32 u32MaxNumSpectators = spPlayer->GetCompSpectators()->GetMaxNumSpectators( _spContext->GetDatabase()->GetMap() );
        if ( u32MaxNumSpectators > 0 )
        {
            f32Percentage = _spAdvert->GetMinSpectators() * 100.0f / u32MaxNumSpectators;
        }
        CFoundation::String strText;
        strText.Format( L"Mind. %s Mio. Z. (= %.0f%%)", strSpectators.wc_str(), f32Percentage );
        m_spTbCondition->SetText( strText );
    }

    // Set Spots & Days
    if ( m_spTbSpotsDays != NULL )
    {
        // Get current date
        CFoundation::Date currentDate;
        GamePtr spGame = spPlayer->GetGame();
        if ( spGame != NULL )
        {
            currentDate = spGame->GetDateTime().GetDate();
        }

        Unsigned32 u32NumSpots = _spAdvert->GetNumSpots();
        Unsigned32 u32NumSpotsLeft = spContract != NULL ? spContract->GetNumSpotsLeft() : u32NumSpots;
        Unsigned32 u32NumDays = _spAdvert->GetNumDays();
        Unsigned32 u32NumDaysLeft = spContract != NULL ? spContract->GetNumDaysLeft( currentDate ) : u32NumDays;
        
        CFoundation::String strBlockText;
        if ( spContract != NULL )
        {
            strBlockText.Format( "%d von ", u32NumSpotsLeft );
        }

        if ( u32NumSpots == 1 )
        {
            strBlockText += CFoundation::String::Build( "%d Spot ", u32NumSpots );
        }
        else
        {
            strBlockText += CFoundation::String::Build( "%d Spots ", u32NumSpots );
        }   
        
        if ( u32NumDaysLeft == 0 )
        {
            strBlockText += CFoundation::String::Build( "bis heute abend" );    
        }
        else if ( u32NumDaysLeft == 1 )
        {
            strBlockText += CFoundation::String::Build( "bis morgen" );    
        }
        else
        {
            strBlockText += CFoundation::String::Build( "in %d Tagen", u32NumDaysLeft );    
        }

        m_spTbSpotsDays->SetText( strBlockText );
    }
}
       
void AdvertTooltip::Clear()
{
    if ( m_spTbName != NULL )
    {
        m_spTbName->SetText( "" );
    }
}

CGui::PanelPtr AdvertTooltip::GetPanel() const
{
    return m_spPnlRoot;
}
