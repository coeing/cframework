#include "cmadtv/Data/Player/PlayerCompNews.h"

#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/Serialization/SerializationContext.h"

#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/News/News.h"
#include "cmadtv/Data/News/NewsChannel.h"
#include "cmadtv/Data/News/NewsInstance.h"
#include "cmadtv/Data/Player/Player.h"

using namespace CMadTV;

PlayerCompNews::PlayerCompNews()
{
}

PlayerCompNews::~PlayerCompNews()
{
}
        
void PlayerCompNews::Init( PlayerPtr _spPlayer )
{
    m_spManager = _spPlayer;
}
       
void PlayerCompNews::operator=( const PlayerCompNews& _rhs )
{
    m_aspChannels = _rhs.m_aspChannels;

    // Remove non-existing news
    NewsInstanceVec aspNews = m_aspNews;
    Unsigned32 u32NumNews = aspNews.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumNews; ++u32Idx )
    {
        NewsInstancePtr spNews = aspNews[ u32Idx ];
        if ( std::find( _rhs.m_aspNews.begin(), _rhs.m_aspNews.end(), spNews ) == _rhs.m_aspNews.end() )
        {
            RemoveNews( spNews );
        }
    }

    // Add new news
    u32NumNews = _rhs.m_aspNews.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumNews; ++u32Idx )
    {
        NewsInstancePtr spNews = _rhs.m_aspNews[ u32Idx ];
        if ( std::find( aspNews.begin(), aspNews.end(), spNews ) == aspNews.end() )
        {
            AddNews( spNews );
        }
    }

    for ( Unsigned32 u32Idx = 0; u32Idx < NUM_PLANNED_NEWS; ++u32Idx )
    {
        SetPlannedNews( u32Idx, _rhs.m_aspPlannedNews[ u32Idx ] );
    }
    m_aspRecentlySubscribedChannels = _rhs.m_aspRecentlySubscribedChannels;
}
        
void PlayerCompNews::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    // Serialize subscribed channels
    _Serializer.SerializeVector< CFoundation::SerializeWrapper_Context >( m_aspChannels );

    // Serialize news
    _Serializer.SerializeVector< CFoundation::SerializeWrapper_Context >( m_aspNews );

    // Serialize planned news
    _Serializer.SerializeArray< CFoundation::SerializeWrapper_Context >( m_aspPlannedNews, NUM_PLANNED_NEWS );
}

void PlayerCompNews::AddChannel( NewsChannelPtr _spChannel )
{
    if ( IsSubscribed( _spChannel ) )
    {
        return;
    }

    m_aspChannels.push_back( _spChannel );
}

void PlayerCompNews::RemoveChannel( NewsChannelPtr _spChannel )
{
    NewsChannelVec::iterator it = std::find( m_aspChannels.begin(), m_aspChannels.end(), _spChannel );
    if ( it != m_aspChannels.end() )
    {
        m_aspChannels.erase( it );
    }
}
       
bool PlayerCompNews::IsSubscribed( NewsChannelPtr _spChannel ) const
{
    NewsChannelVec::const_iterator it = std::find( m_aspChannels.begin(), m_aspChannels.end(), _spChannel );
    return it != m_aspChannels.end();
}

const std::vector< NewsChannelPtr >& PlayerCompNews::GetChannels() const
{
    return m_aspChannels;
}
        
void PlayerCompNews::AddRecentlySubscribedChannel( NewsChannelPtr _spChannel )
{
    if ( WasRecentlySubscribed( _spChannel ) )
    {
        return;
    }

    m_aspRecentlySubscribedChannels.push_back( _spChannel );
}

void PlayerCompNews::RemoveRecentlySubscribedChannel( NewsChannelPtr _spChannel )
{
    NewsChannelVec::iterator it = std::find( m_aspRecentlySubscribedChannels.begin(), m_aspRecentlySubscribedChannels.end(), _spChannel );
    if ( it != m_aspRecentlySubscribedChannels.end() )
    {
        m_aspRecentlySubscribedChannels.erase( it );
    }
}
        
void PlayerCompNews::RemoveRecentlySubscribedChannels()
{
    m_aspRecentlySubscribedChannels.clear();
}

bool PlayerCompNews::WasRecentlySubscribed( NewsChannelPtr _spChannel ) const
{
    NewsChannelVec::const_iterator it = std::find( m_aspRecentlySubscribedChannels.begin(), m_aspRecentlySubscribedChannels.end(), _spChannel );
    return it != m_aspRecentlySubscribedChannels.end();
}

const std::vector< NewsChannelPtr >& PlayerCompNews::GetRecentlySubscribedChannels() const
{
    return m_aspRecentlySubscribedChannels;
}
        
void PlayerCompNews::AddNews( NewsInstancePtr _spNews )
{
    if ( _spNews == NULL )
    {
        return;
    }

    m_aspNews.push_back( _spNews );

    // Dispatch event
    DispatchEvent( EVENT_NEWS_ADDED, _spNews );
}

bool PlayerCompNews::RemoveNews( NewsInstancePtr _spNews )
{
    NewsInstanceVec::iterator it = std::find( m_aspNews.begin(), m_aspNews.end(), _spNews );
    if ( it == m_aspNews.end() )
    {
        return false;
    }

    m_aspNews.erase( it );

    // Dispatch event
    DispatchEvent( EVENT_NEWS_REMOVED, _spNews );

    return true;
}
        
bool PlayerCompNews::HasNews( NewsInstancePtr _spNews ) const
{
    NewsInstanceVec::const_iterator it = std::find( m_aspNews.begin(), m_aspNews.end(), _spNews );
    return it != m_aspNews.end();
}
       
const std::vector< NewsInstancePtr >& PlayerCompNews::GetNews() const
{
    return m_aspNews;
}

void PlayerCompNews::GetNews( NewsChannelPtr _spChannel, std::vector< NewsInstancePtr >& _aspNews ) const
{
    Unsigned32 u32NumNews = m_aspNews.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumNews; ++u32Idx )
    {
        NewsInstancePtr spNews = m_aspNews[ u32Idx ];
        if ( spNews != NULL &&
             spNews->GetNews()->GetChannel() == _spChannel )
        {
            _aspNews.push_back( spNews );
        }
    }
}
       
void PlayerCompNews::SetPlannedNews( Unsigned32 _u32Idx, NewsInstancePtr _spNews )
{
    if ( _u32Idx >= NUM_PLANNED_NEWS )
    {
        return;
    }

    // Check if already planned
    if ( IsPlannedNews( _spNews ) )
    {
        return;
    }

    // Remove it
    RemoveNews( _spNews );

    NewsInstancePtr spOldNews = m_aspPlannedNews[ _u32Idx ];
    m_aspPlannedNews[ _u32Idx ] = _spNews;

    // Dispatch event
    DispatchEvent( EVENT_NEWS_PLANNED_CHANGED, _u32Idx, _spNews, spOldNews );
}
     
bool PlayerCompNews::IsPlannedNews( NewsInstancePtr _spNews ) const
{
    if ( _spNews == NULL )
    {
        return false;
    }

    for ( Unsigned32 u32Idx = 0; u32Idx < NUM_PLANNED_NEWS; ++u32Idx )
    {
        if ( m_aspPlannedNews[ u32Idx ] == _spNews )
        {
            return true;
        }
    }
    return false;
}

NewsInstancePtr PlayerCompNews::GetPlannedNews( Unsigned32 _u32Idx ) const
{
    if ( _u32Idx < NUM_PLANNED_NEWS )
    {
        return m_aspPlannedNews[ _u32Idx ];
    }
    else
    {
        return NewsInstancePtr();
    }
}
