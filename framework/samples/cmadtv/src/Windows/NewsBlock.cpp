#include "cmadtv/Windows/NewsBlock.h"

#include "cfoundation/Debug/Makros.h"

#include "cgui/Gui/GuiMain.h"

#include "cmadtv/Data/Game/Game.h"

#include "cmadtv/Data/News/News.h"
#include "cmadtv/Data/News/NewsChannel.h"
#include "cmadtv/Data/News/NewsInstance.h"

using namespace CMadTV;

NewsBlockPtr NewsBlock::Create( NewsInstancePtr _spNews )
{
    // Check which xml to use
    CFoundation::String strXML = "data/windows/NewsBlock.xml";

    // Create panel
    CGui::PanelPtr spPnlBlock = CGui::GuiMain::GetInstance().CreateFromXML< CGui::Panel >( strXML );

    // Create News block
    NewsBlockPtr spNewsBlock( new NewsBlock( spPnlBlock ) );
    spNewsBlock->SetNewsInstance( _spNews );

    return spNewsBlock;
}

NewsBlock::NewsBlock( CGui::PanelPtr _spPnlRoot )
{
    // Create delegates
    m_spDelegateHourChanged = WorldEventDelegate0Ptr( new WorldEventDelegate0( this, &NewsBlock::OnHourChanged ) );
    m_spDelegateDayChanged = WorldEventDelegate0Ptr( new WorldEventDelegate0( this, &NewsBlock::OnDayChanged ) );

    // Store pointer to root panel
    m_spPnlRoot = _spPnlRoot;

    // Get controls
    m_spTbText = static_pointer_cast< CGui::TextBox >( m_spPnlRoot->FindChild( "TbText" ) );
    m_spTbTime = static_pointer_cast< CGui::TextBox >( m_spPnlRoot->FindChild( "TbTime" ) );
    m_spTbPrice = static_pointer_cast< CGui::TextBox >( m_spPnlRoot->FindChild( "TbPrice" ) );
}

NewsBlock::~NewsBlock()
{
    SetNewsInstance( NewsInstancePtr() );
    CGui::GuiMain::GetInstance().Delete( m_spPnlRoot );
}
                
void NewsBlock::SetNewsInstance( NewsInstancePtr _spNewsInstance )
{
    if ( _spNewsInstance == m_spNewsInstance )
    {
        return;
    }

    // Remove delegates
    if ( m_spNewsInstance != NULL )
    {
        GamePtr spGame = m_spNewsInstance->GetGame();
        if ( spGame != NULL )
        {
            spGame->RemoveDelegate( WORLD_EVENT_HOUR_CHANGED, m_spDelegateHourChanged );
            spGame->RemoveDelegate( WORLD_EVENT_DAY_STARTED, m_spDelegateDayChanged );
        }
    }

    m_spNewsInstance = _spNewsInstance;

    // Register delegates
    if ( m_spNewsInstance != NULL )
    {
        GamePtr spGame = m_spNewsInstance->GetGame();
        if ( spGame != NULL )
        {
            spGame->RegisterDelegate( WORLD_EVENT_HOUR_CHANGED, m_spDelegateHourChanged );
            spGame->RegisterDelegate( WORLD_EVENT_DAY_STARTED, m_spDelegateDayChanged );
        }
    }

    // Update contents
    UpdateContents();
}

NewsInstancePtr NewsBlock::GetNewsInstance() const
{
    return m_spNewsInstance;
}

CGui::PanelPtr NewsBlock::GetPanel() const
{
    return m_spPnlRoot;
}
        
void NewsBlock::SetVisible( bool _bVisible )
{
    if ( m_spPnlRoot != NULL )
    {
        m_spPnlRoot->SetVisible( _bVisible );
    }
}
        
void NewsBlock::UpdateContents()
{
    // Update price
    UpdatePrice();

    // Update time/day
    UpdateTimeDay();

    // Set news
    NewsPtr spNews = m_spNewsInstance != NULL ? m_spNewsInstance->GetNews() : NewsPtr();
    
    // Update content
    if ( spNews != NULL )
    {
        m_spTbText->SetText( spNews->GetText() );

        // Set background color
        m_spPnlRoot->SetBgColor( spNews->GetChannel()->GetColor() );
    }
    else
    {
        m_spTbText->SetText( "" );
    }
}

void NewsBlock::UpdatePrice()
{
    if ( m_spNewsInstance != NULL )
    {
        m_spTbPrice->SetText( CFoundation::String::Build( "$ %.0f", m_spNewsInstance->GetCurrentPrice().GetAmount( CFoundation::CURRENCY_DOLLAR ) ) );
    }
    else
    {
        m_spTbPrice->SetText( "$ 0" );
    }
}

void NewsBlock::UpdateTimeDay()
{
    if ( m_spNewsInstance != NULL )
    {
        // Get age of news
        Integer64 i64Days = 0;
        Integer32 i32Hours = 0;
        m_spNewsInstance->GetAge( i64Days, i32Hours );

        // Create day token
        CFoundation::String strDay = "Heute";
        if ( i64Days > 0 )
        {
            strDay = "(Alt)";
        }

        const CFoundation::Time& time = m_spNewsInstance->GetDateTime().GetTime();
        m_spTbTime->SetText( CFoundation::String::Build( "%02d:%02d %s", time.GetHour(), time.GetMinute(), strDay.c_str() ) );
    }
    else
    {
        m_spTbTime->SetText( "xx:xx" );
    }
}
        
void NewsBlock::OnHourChanged()
{
    UpdatePrice();
    UpdateTimeDay();
}

void NewsBlock::OnDayChanged()
{
    UpdateTimeDay();
}

