#ifndef CMADTV_DATABASE_H
#define CMADTV_DATABASE_H

#include <map>
#include <vector>

#include "cfoundation/Base/Types.h"
#include "cfoundation/Math/Vector.h"
#include "cfoundation/Utils/SharedPtr.h"

#include "cmadtv/Data/World/Hour.h"

namespace CFoundation
{
    class Money;
    class Serializer;
    INCLUDE_SHARED_CLASS( XMLElement );
}

namespace CSystem
{
    INCLUDE_SHARED_CLASS( File );
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Advert );
    INCLUDE_SHARED_CLASS( Broadcast );
    INCLUDE_SHARED_CLASS( Database );
    INCLUDE_SHARED_CLASS( Genre );
    INCLUDE_SHARED_CLASS( Map );
    INCLUDE_SHARED_CLASS( News );
    INCLUDE_SHARED_CLASS( NewsChannel );
    INCLUDE_SHARED_CLASS( Present );
    INCLUDE_SHARED_CLASS( Transmitter );

    typedef std::vector< AdvertPtr > AdvertVec;
    typedef std::vector< BroadcastPtr > BroadcastVec;
    typedef std::vector< GenrePtr > GenreVec;
    typedef std::vector< NewsPtr > NewsVec;
    typedef std::vector< NewsChannelPtr > NewsChannelVec;
    typedef std::vector< PresentPtr > PresentVec;
    typedef std::vector< TransmitterPtr > TransmitterVec;

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// Database
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class Database
    {
    public:
        /// Constructor
        Database();

        /// Destructor
        virtual ~Database();

        /// Serialization
        void                Serialize( CFoundation::Serializer& _Serializer );

        /// Loads data from XML
        bool                LoadFromXML( const CSystem::FilePtr _spFile );

        /////////////////////////////////////////////////////////////////
        // Genres
        /////////////////////////////////////////////////////////////////

        /// Creates a genre
        GenrePtr            CreateGenre();

        /// Returns the genres
        const GenreVec&     GetGenres() const;

        /// Returns the number of genres
        Unsigned32          GetNumGenres() const;

        /// Returns the genre with the passed index
        GenrePtr            GetGenre( Unsigned32 _u32ID ) const;

        /////////////////////////////////////////////////////////////////
        // Broadcasts
        /////////////////////////////////////////////////////////////////

        /// Creates a new broadcast
        BroadcastPtr        CreateBroadcast();

        /// Returns the broadcasts
        const BroadcastVec& GetBroadcasts() const;

        /// Returns the number of broadcasts
        Unsigned32          GetNumBroadcasts() const;

        /// Searches for a broadcast with the passed ID
        BroadcastPtr        GetBroadcast( Unsigned32 _u32ID ) const;

        /////////////////////////////////////////////////////////////////
        // Adverts
        /////////////////////////////////////////////////////////////////

        /// Creates a new advert
        AdvertPtr           CreateAdvert();

        /// Returns the adverts
        const AdvertVec&    GetAdverts() const;

        /// Returns the number of adverts
        Unsigned32          GetNumAdverts() const;

        /// Searches for an advert with the passed ID
        AdvertPtr           GetAdvert( Unsigned32 _u32ID ) const;

        /////////////////////////////////////////////////////////////////
        // News Channels
        /////////////////////////////////////////////////////////////////

        /// Creates a news channel
        NewsChannelPtr      CreateNewsChannel();

        /// Returns the news channels
        const NewsChannelVec&   GetNewsChannels() const;

        /// Returns the number of news channels
        Unsigned32          GetNumNewsChannels() const;

        /// Returns the news channel with the passed index
        NewsChannelPtr      GetNewsChannel( Unsigned32 _u32ID ) const;

        /////////////////////////////////////////////////////////////////
        // News
        /////////////////////////////////////////////////////////////////

        /// Creates a new news
        NewsPtr             CreateNews();

        /// Returns the news
        const NewsVec&      GetNews() const;

        /// Returns the number of news
        Unsigned32          GetNumNews() const;

        /// Searches for a news with the passed ID
        NewsPtr             GetNews( Unsigned32 _u32ID ) const;

        /////////////////////////////////////////////////////////////////
        // Spectators
        /////////////////////////////////////////////////////////////////

        /// Returns the spectator map
        MapPtr              GetMap() const;

        /// Returns the number of satellites
        Unsigned32          GetNumSatellites() const;

        /// Returns the satellite with the passed index
        TransmitterPtr      GetSatellite( Unsigned32 _u32Idx ) const;

        /// Returns the index of the passed satellite
        /// Return MAX_U32 if satellite not found
        Unsigned32          GetSatelliteIdx( TransmitterPtr _spSatellite ) const;

        /// Returns the possible initial antennas
        const TransmitterVec&   GetInitialAntennas() const;

        /// Indicates if the passed transmitter is valid
        bool                IsValidTransmitter( TransmitterPtr _spTransmitter ) const;

        /// Computes the price of the passed transmitter on the passed map
        CFoundation::Money  ComputeTransmitterPrice( TransmitterPtr _spTransmitter ) const;

        /// Computes the price for an antenna at the passed position
        CFoundation::Money  ComputeAntennaPrice( const CFoundation::Vector2Di& _vPosition ) const;

        /// Computes the price for an antenna that reaches the passed amount of spectators
        CFoundation::Money  ComputeAntennaPrice( Unsigned32 _u32NumSpectators ) const;

        /////////////////////////////////////////////////////////////////
        // Presents
        /////////////////////////////////////////////////////////////////

        /// Creates a new present
        PresentPtr          CreatePresent();

        /// Returns the presents
        const PresentVec&   GetPresents() const;

        /// Returns the number of presents
        Unsigned32          GetNumPresents() const;

        /// Searches for a present with the passed ID
        PresentPtr          GetPresent( Unsigned32 _u32ID ) const;
        
        /////////////////////////////////////////////////////////////////
        // Settings
        /////////////////////////////////////////////////////////////////

        /// Returns the base rating for the passed hour
        Unsigned8           GetRating( Hour _eHour ) const;

    private:
        /// Loads a broadcast from the passed xml element
        void                LoadBroadcast( CFoundation::XMLElementPtr _spElement );

        /// Loads an advert from the passed xml element
        void                LoadAdvert( CFoundation::XMLElementPtr _spElement );

        /// Loads a news from the passed xml element
        void                LoadNews( CFoundation::XMLElementPtr _spElement );

        /// Loads a satellite from the passed xml element
        void                LoadSatellite( CFoundation::XMLElementPtr _spElement );

        /// Loads an initial antenna from the passed xml element
        void                LoadInitialAntenna( CFoundation::XMLElementPtr _spElement );

        /// Loads a present from the passed xml element
        void                LoadPresent( CFoundation::XMLElementPtr _spElement );

        /// Next genre ID
        Unsigned32          m_u32NextGenreID;

        /// Genres
        GenreVec            m_aspGenres;

        /// Next broadcast ID
        Unsigned32          m_u32NextBroadcastID;

        /// Broadcasts
        BroadcastVec        m_aspBroadcasts;

        /// Next advert ID
        Unsigned32          m_u32NextAdvertID;

        /// Adverts
        AdvertVec           m_aspAdverts;

        /// Next genre ID
        Unsigned32          m_u32NextNewsChannelID;

        /// NewsChannels
        NewsChannelVec      m_aspNewsChannels;

        /// Next news ID
        Unsigned32          m_u32NextNewsID;

        /// News
        NewsVec             m_aspNews;

        /// Spectator map
        MapPtr              m_spMap;

        /// Satellites
        TransmitterVec      m_aspSatellites;

        /// Satellite prices
        std::map< TransmitterPtr, CFoundation::Money >  m_mapSatellitePrices;

        /// Initial antennas
        TransmitterVec      m_aspInitialAntennas;

        /// Next present ID
        Unsigned32          m_u32NextPresentID;

        /// Presents
        PresentVec          m_aspPresents;

        /// Base ratings
        Unsigned8           m_au8Ratings[ HOUR_END ];
    };
}

#endif
