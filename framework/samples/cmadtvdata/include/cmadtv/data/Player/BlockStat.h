#ifndef CMADTV_BLOCK_STAT_H
#define CMADTV_BLOCK_STAT_H

#include "cfoundation/Base/Types.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CFoundation
{
    class Serializer;
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Advert );
    INCLUDE_SHARED_CLASS( Broadcast );

    /// Represents a statistic for one hour of broadcasting
    /**
     *  \author Christian Oeing (oeing@gmx.at)
     *  \date 27.09.2009
     */
    class BlockStat
    {
    public:
        /// Effect the block had on the image of the player
        enum ImageEffect
        {
            IMAGE_EFFECT_NONE,
            IMAGE_EFFECT_DECREASE,
            IMAGE_EFFECT_INCREASE,
            IMAGE_EFFECT_END
        };

        /// Constructor
        BlockStat();

        /// Destructor
        ~BlockStat();

        /// Serialization
        virtual void            Serialize( CFoundation::Serializer& _Serializer );

        /// Sets the news rating (0.0 - 1.0)
        void                    SetNewsRating( Float32 _f32Rating );

        /// Returns the news rating (0.0 - 1.0)
        Float32                 GetNewsRating() const;

        /// Sets the number of spectators of the news
        void                    SetNewsSpectators( Unsigned32 _u32Spectators );

        /// Returns the number of spectators of the news
        Unsigned32              GetNewsSpectators() const;

        /// Sets the broadcast of this hour
        void                    SetBroadcast( BroadcastPtr _spBroadcast );

        /// Returns the broadcast of this hour
        BroadcastPtr            GetBroadcast() const;

        /// Sets the broadcast rating (0.0 - 1.0)
        void                    SetBroadcastRating( Float32 _f32Rating );

        /// Returns the broadcast rating (0.0 - 1.0)
        Float32                 GetBroadcastRating() const;

        /// Sets the number of spectators of the broadcast
        void                    SetBroadcastSpectators( Unsigned32 _u32Spectators );

        /// Returns the number of spectators of the broadcast
        Unsigned32              GetBroadcastSpectators() const;

        /// Sets the broadcast order number (i.e. the block of the broadcast which is sent)
        void                    SetBroadcastOrder( Unsigned32 _u32Order );

        /// Returns the broadcast order number (i.e. the block of the broadcast which is sent)
        Unsigned32              GetBroadcastOrder() const;

        /// Sets the advert of this hour
        void                    SetAdvert( AdvertPtr _spAdvert );

        /// Returns the advert of this hour
        AdvertPtr               GetAdvert() const;

        /// Sets if the advert was successful
        void                    SetAdvertSuccessful( bool _bSuccess );

        /// Indicates if the advert was successful
        bool                    WasAdvertSuccessful() const;

        /// Sets the advert order number
        void                    SetAdvertOrder( Unsigned32 _u32Order );

        /// Returns the advert order number
        Unsigned32              GetAdvertOrder() const;

        /// Sets the image effect
        void                    SetImageEffect( ImageEffect _eEffect );

        /// Returns the image effect
        ImageEffect             GetImageEffect() const;

    private:
        /// News rating
        Float32                 m_f32NewsRating;

        /// Number of spectators of the news
        Unsigned32              m_u32NewsSpectators;

        /// Broadcast
        BroadcastPtr            m_spBroadcast;

        /// Broadcast rating
        Float32                 m_f32BroadcastRating;

        /// Number of spectators of the broadcast
        Unsigned32              m_u32BroadcastSpectators;

        /// Broadcast order number
        Unsigned32              m_u32BroadcastOrder;

        /// Advert
        AdvertPtr               m_spAdvert;

        /// Indicates if the advert was successful
        bool                    m_bAdvertSuccessful;

        /// Advert order number
        Unsigned32              m_u32AdvertOrder;

        /// Effect the block had on the player's image
        ImageEffect             m_eImageEffect;
    };

    INCLUDE_SHARED_CLASS( BlockStat );
}

#endif
