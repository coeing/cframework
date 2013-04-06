#ifndef CMADTV_NEWS_INSTANCE_H
#define CMADTV_NEWS_INSTANCE_H

#include "cfoundation/Base/Defines.h"
#include "cfoundation/DateTime/DateTime.h"
#include "cfoundation/RTTI/rtti.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CFoundation
{
    class Money;
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( News );
    INCLUDE_SHARED_CLASS( Game );

    /// Represents a NewsInstance
    /**
     *  \author Christian Oeing (oeing@gmx.at)
     *  \date 24.07.2010
     */
    class NewsInstance
    {
    public:
        /// Constructor
        NewsInstance( Unsigned32 _u32ID = MAX_U32 );

        /// Destructor
        ~NewsInstance();

        /// Initialization
        void                            Init( GamePtr _spGame, NewsPtr _spNews, const CFoundation::DateTime& _DateTime );

        /// Deinitialization
        void                            Deinit();

        /// Serialization
        virtual void                    Serialize( CFoundation::Serializer& _Serializer );

        /// Returns the game
        GamePtr                         GetGame() const;

        /// Returns the ID
        Unsigned32                      GetID() const;

        /// Returns the news
        NewsPtr                         GetNews() const;

        /// Returns the date
        const CFoundation::DateTime&    GetDateTime() const; 

        /// Returns the age (in days and hours)
        void                            GetAge( Integer64& _i64Days, Integer32& _i32Hours ) const;

        /// Returns the current price (depends on the age of the news)
        CFoundation::Money              GetCurrentPrice() const;

    private:
        /// Game
        GamePtr                     m_spGame;

        /// ID 
        Unsigned32                  m_u32ID;

        /// News
        NewsPtr                     m_spNews;

        /// Date/Time
        CFoundation::DateTime       m_DateTime;

        TYPE_DATA
    };

    INCLUDE_SHARED_CLASS( NewsInstance );
}

#endif
