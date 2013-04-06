#ifndef CMADTV_PLAYER_COMP_NEWS_H
#define CMADTV_PLAYER_COMP_NEWS_H

#include "cfoundation/Base/Types.h"
#include "cfoundation/Events/DelegateInvoker.h"
#include "cfoundation/Utils/SharedPtr.h"

#include "cmadtv/Data/World/Hour.h"

namespace CFoundation
{
    class Serializer;
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( NewsChannel );
    INCLUDE_SHARED_CLASS( NewsInstance );
    INCLUDE_SHARED_CLASS( PlayerCompNews );
    INCLUDE_SHARED_CLASS( Player );

    /// Events
    enum PlayerCompNewsEventNews
    {
        EVENT_NEWS_ADDED,
        EVENT_NEWS_REMOVED
    };
    TYPEDEF_DELEGATE1( PlayerCompNewsEventNews, NewsInstancePtr );
    enum PlayerCompNewsEventU32NewsNews
    {
        EVENT_NEWS_PLANNED_CHANGED
    };
    TYPEDEF_DELEGATE3( PlayerCompNewsEventU32NewsNews, Unsigned32, NewsInstancePtr, NewsInstancePtr );

    /// PlayerCompNews of one broadcast station for one day
    class PlayerCompNews : public CFoundation::Delegate1Invoker< PlayerCompNewsEventNews, NewsInstancePtr >,
                           public CFoundation::Delegate3Invoker< PlayerCompNewsEventU32NewsNews, Unsigned32, NewsInstancePtr, NewsInstancePtr >
    {
    public:
        DELEGATE1_INVOKER( PlayerCompNewsEventNews, NewsInstancePtr );
        DELEGATE3_INVOKER( PlayerCompNewsEventU32NewsNews, Unsigned32, NewsInstancePtr, NewsInstancePtr );

        /// Number of planning slots for news
        static const Unsigned32 NUM_PLANNED_NEWS = 3;

        /// Constructor
        PlayerCompNews();

        /// Destructor
        ~PlayerCompNews();

        /// Initialization
        void                Init( PlayerPtr _spPlayer );

        /// Assignment operator
        void                operator=( const PlayerCompNews& _rhs );

        /// Serialization
        virtual void        Serialize( CFoundation::Serializer& _Serializer );

        //////////////////////////////////////////////////////////////////////
        /// Channels
        //////////////////////////////////////////////////////////////////////

        /// Adds a channel to the subscribed channels
        void                AddChannel( NewsChannelPtr _spChannel );

        /// Removes a channel from the subscribed channels
        void                RemoveChannel( NewsChannelPtr _spChannel );

        /// Indicates if the passed channel is subscribed
        bool                IsSubscribed( NewsChannelPtr _spChannel ) const;

        /// Returns all subscribed channels
        const std::vector< NewsChannelPtr >&    GetChannels() const;

        /// Adds a channel to the recently subscribed channels
        void                AddRecentlySubscribedChannel( NewsChannelPtr _spChannel );

        /// Removes a channel from the recently subscribed channels
        void                RemoveRecentlySubscribedChannel( NewsChannelPtr _spChannel );

        /// Removes all recently subscribed channels
        void                RemoveRecentlySubscribedChannels();

        /// Indicates if the passed channel was recently subscribed
        bool                WasRecentlySubscribed( NewsChannelPtr _spChannel ) const;

        /// Returns all recently subscribed channels
        const std::vector< NewsChannelPtr >&    GetRecentlySubscribedChannels() const;

        //////////////////////////////////////////////////////////////////////
        /// News
        //////////////////////////////////////////////////////////////////////

        /// Adds a news
        void                AddNews( NewsInstancePtr _spNews );

        /// Removes the passed news
        /// Returns if the news was removed
        bool                RemoveNews( NewsInstancePtr _spNews );

        /// Indicates if the passed news is available
        bool                HasNews( NewsInstancePtr _spNews ) const;

        /// Returns all enws
        const std::vector< NewsInstancePtr >&   GetNews() const;

        /// Returns all news of the passed channel
        void                GetNews( NewsChannelPtr _spChannel, std::vector< NewsInstancePtr >& _aspNews ) const;

        //////////////////////////////////////////////////////////////////////
        /// Planning
        //////////////////////////////////////////////////////////////////////

        /// Sets the passed news to the passed slot
        void                SetPlannedNews( Unsigned32 _u32Idx, NewsInstancePtr _spNews );

        /// Checks if the passed news is planned
        bool                IsPlannedNews( NewsInstancePtr _spNews ) const;

        /// Returns the planned news in the passed slot
        NewsInstancePtr     GetPlannedNews( Unsigned32 _u32Idx ) const;
        
    private:
        typedef std::vector< NewsChannelPtr > NewsChannelVec;
        typedef std::vector< NewsInstancePtr > NewsInstanceVec;

        /// Player
        PlayerPtr           m_spManager;

        /// Subscribed channels
        NewsChannelVec      m_aspChannels;

        /// News
        NewsInstanceVec     m_aspNews;

        /// Planned news
        NewsInstancePtr     m_aspPlannedNews[ NUM_PLANNED_NEWS ];

        /// Subscribed channels that can be reverted
        NewsChannelVec      m_aspRecentlySubscribedChannels;
    };
}

#endif
