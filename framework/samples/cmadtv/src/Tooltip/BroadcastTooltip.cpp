#include "cmadtv/Tooltip/BroadcastTooltip.h"

#include "cfoundation/Debug/Makros.h"

#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/Panel.h"
#include "cgui/Gui/ProgressBar.h"
#include "cgui/Gui/TextBox.h"

#include "cmadtv/Data/Broadcast.h"
#include "cmadtv/Data/Genre.h"
#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerContext.h"
#include "cmadtv/Data/Player/Schedule.h"
#include "cmadtv/Data/Player/Scheduling.h"
#include "cmadtv/Data/World/Country.h"

using namespace CMadTV;

BroadcastTooltipPtr BroadcastTooltip::Create()
{
    // Create panel
    CGui::PanelPtr spPnlBlock = CGui::GuiMain::GetInstance().CreateFromXML< CGui::Panel >( "data/windows/BroadcastTooltip.xml" );
    if ( spPnlBlock == NULL )
    {
        return BroadcastTooltipPtr();
    }

    // Create broadcast tooltip
    BroadcastTooltipPtr spBroadcastTooltip( new BroadcastTooltip( spPnlBlock ) );
    return spBroadcastTooltip;
}

BroadcastTooltip::BroadcastTooltip( CGui::PanelPtr _spPnlRoot )
{
    // Store pointer to root panel
    m_spPnlRoot = _spPnlRoot;

    // Get controls
    m_spTbTitle = static_pointer_cast< CGui::TextBox >( m_spPnlRoot->FindChild( "TbTitle" ) );
    m_spTbDescription = static_pointer_cast< CGui::TextBox >( m_spPnlRoot->FindChild( "TbDescription" ) );
    m_spTbGenre = static_pointer_cast< CGui::TextBox >( m_spPnlRoot->FindChild( "TbGenre" ) );
    m_spTbCountryYear = static_pointer_cast< CGui::TextBox >( m_spPnlRoot->FindChild( "TbCountryYear" ) );
    m_spTbBlocks = static_pointer_cast< CGui::TextBox >( m_spPnlRoot->FindChild( "TbBlocks" ) );
    m_spTbPrice = static_pointer_cast< CGui::TextBox >( m_spPnlRoot->FindChild( "TbPrice" ) );
    m_spPbUp2date = static_pointer_cast< CGui::ProgressBar >( m_spPnlRoot->FindChild( "PbUp2date" ) );
    m_spTbPlanned = static_pointer_cast< CGui::TextBox >( m_spPnlRoot->FindChild( "TbPlanned" ) );
}

BroadcastTooltip::~BroadcastTooltip()
{
    CGui::GuiMain::GetInstance().Delete( m_spPnlRoot );
}
       
void BroadcastTooltip::SetContext( PlayerContextPtr _spContext )
{
    if ( _spContext == m_spContext )
    {
        return;
    }

    m_spContext = _spContext;

    // Update tooltip
    UpdateTooltip();
}
       
void BroadcastTooltip::SetBroadcast( BroadcastPtr _spBroadcast )
{
    if ( _spBroadcast == m_spBroadcast )
    {
        return;
    }

    m_spBroadcast = _spBroadcast;

    // Update tooltip
    UpdateTooltip();
}

BroadcastPtr BroadcastTooltip::GetBroadcast() const
{
    return m_spBroadcast;
}
       
void BroadcastTooltip::SetOwner( PlayerPtr _spOwner )
{
    if ( _spOwner == m_spOwner )
    {
        return;
    }

    m_spOwner = _spOwner;

    // Update tooltip
    UpdateTooltip();
}
       
void BroadcastTooltip::Clear()
{
    m_spBroadcast.reset();

    if ( m_spTbTitle != NULL )
    {
        m_spTbTitle->SetText( "" );
    }

    if ( m_spTbCountryYear != NULL )
    {
        m_spTbCountryYear->SetText( "" );
    }
}

CGui::PanelPtr BroadcastTooltip::GetPanel() const
{
    return m_spPnlRoot;
}
        
void BroadcastTooltip::UpdateTooltip()
{
    if ( m_spBroadcast == NULL )
    {
        Clear();
        return;
    }

    // Get game
    GamePtr spGame = m_spContext != NULL ? m_spContext->GetGame() : GamePtr();

    // Set title
    if ( m_spTbTitle != NULL )
    {
        m_spTbTitle->SetText( m_spBroadcast->GetTitle() );
    }

    // Set description
    if ( m_spTbDescription != NULL )
    {
        m_spTbDescription->SetText( m_spBroadcast->GetDescription() );
    }

    // Set genre
    if ( m_spTbGenre != NULL )
    {
        GenrePtr spGenre = m_spBroadcast->GetGenre();
        if ( spGenre != NULL )
        {
            m_spTbGenre->SetText( spGenre->GetName() );
        }
        else
        {
            m_spTbGenre->SetText( "-" );
        }
    }

    // Set country and year
    if ( m_spTbCountryYear != NULL )
    {
        CFoundation::String strCountryYear;
        strCountryYear.Format( "%s %d", CFoundation::EnumConverter< Country >::GetInstance().Convert( m_spBroadcast->GetCountry() ).c_str(), m_spBroadcast->GetYear() );
        m_spTbCountryYear->SetText( strCountryYear );
    }

    // Set blocks
    if ( m_spTbBlocks != NULL )
    {
        CFoundation::String strBlockText;
        Unsigned32 u32NumBlocks = m_spBroadcast->GetNumBlocks();
        if ( u32NumBlocks == 1 )
        {
            strBlockText = "1 Block";
        }
        else
        {
            strBlockText.Format( "%d Blocks", u32NumBlocks );
        }            
        m_spTbBlocks->SetText( strBlockText );
    }

    // Set price
    if ( m_spTbPrice != NULL )
    {
        CFoundation::String strPriceText;
        strPriceText.Format( L"$ %.0f", m_spBroadcast->GetCurrentPrice().GetAmount( CFoundation::CURRENCY_DOLLAR ) );
        m_spTbPrice->SetText( strPriceText );
    }

    // Set up2date
    if ( m_spPbUp2date != NULL )
    {
        m_spPbUp2date->SetRatio( m_spBroadcast->GetUp2date() / 100.0f );
    }

    // Set if planned
    if ( m_spTbPlanned != NULL )
    {
        bool bPlanned = false;
        if ( m_spOwner != NULL &&
             spGame != NULL )
        {
            // Compute start hour
            Hour eStartHour = spGame->GetHour();
            if ( spGame->GetHourStage() > HOUR_STAGE_BROADCAST )
            {
                ++eStartHour;
            }

            SchedulingPtr spScheduling = m_spOwner->GetScheduling();
            bPlanned = spScheduling->IsBroadcastPlanned( m_spBroadcast, SCHEDULE_DAY_TODAY, eStartHour );
        }

        m_spTbPlanned->SetVisible( bPlanned );
    }
}
