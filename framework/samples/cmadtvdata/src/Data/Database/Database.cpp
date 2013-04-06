#include "cmadtv/Data/Database/Database.h"

#include <algorithm>

#include "cfoundation/Base/Defines.h"
#include "cfoundation/Debug/Assert.h"
#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/Serialization/SerializationContext.h"
#include "cfoundation/XML/XMLParser.h"

#include "csystem/Files/File.h"

#include "cmadtv/Data/Advert.h"
#include "cmadtv/Data/Broadcast.h"
#include "cmadtv/Data/Genre.h"
#include "cmadtv/Data/Betty/Present.h"
#include "cmadtv/Data/News/News.h"
#include "cmadtv/Data/News/NewsChannel.h"
#include "cmadtv/Data/Spectators/Map.h"
#include "cmadtv/Data/Spectators/Transmitter.h"
#include "cmadtv/Data/World/Country.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// Database
///
////////////////////////////////////////////////////////////////////////////////////////////
Database::Database()
:   m_u32NextGenreID( 0 ),
    m_u32NextBroadcastID( 0 ),
    m_u32NextAdvertID( 0 ),
    m_u32NextNewsChannelID( 0 ),
    m_u32NextNewsID( 0 ),
    m_u32NextPresentID( 0 )
{
    // Load ratings
    m_au8Ratings[ HOUR_18 ] = 75;
    m_au8Ratings[ HOUR_19 ] = 75;
    m_au8Ratings[ HOUR_20 ] = 95;
    m_au8Ratings[ HOUR_21 ] = 90;
    m_au8Ratings[ HOUR_22 ] = 85;
    m_au8Ratings[ HOUR_23 ] = 75;
    m_au8Ratings[ HOUR_24 ] = 70;

    // Load genres
    GenrePtr spLoveGenre = CreateGenre();
    spLoveGenre->SetName( "Lovestory" );
    GenrePtr spActionGenre = CreateGenre();
    spActionGenre->SetName( "Actionfilm" );
    GenrePtr spMonumentalGenre = CreateGenre();
    spMonumentalGenre->SetName( "Monumentalfilm" );
    GenrePtr spComedyGenre = CreateGenre();
    spComedyGenre->SetName( "Komödie" );
    GenrePtr spCrimeGenre = CreateGenre();
    spCrimeGenre->SetName( "Krimi" );
    GenrePtr spCultureGenre = CreateGenre();
    spCultureGenre->SetName( "Kultur" );
    GenrePtr spLiveGenre = CreateGenre();
    spLiveGenre->SetName( "Live-Übertragung" );
    GenrePtr spShowGenre = CreateGenre();
    spShowGenre->SetName( "Show" );
    GenrePtr spScienceFictionGenre = CreateGenre();
    spScienceFictionGenre->SetName( "Science Fiction" );
    GenrePtr spOtherGenre = CreateGenre();
    spOtherGenre->SetName( "Sonstiges" );
    GenrePtr spSeriesGenre = CreateGenre();
    spSeriesGenre->SetName( "Serien" );

    // Load news channels
    NewsChannelPtr spNewsChannel = CreateNewsChannel();
    spNewsChannel->SetName( "PolitNews" );
    spNewsChannel->SetPrice( CFoundation::Money( 1000, CFoundation::CURRENCY_DOLLAR ) );
    spNewsChannel->SetColor( CFoundation::Color( (Unsigned8)84, 144, 188 ) );
    spNewsChannel = CreateNewsChannel();
    spNewsChannel->SetName( "Showbusiness" );
    spNewsChannel->SetPrice( CFoundation::Money( 10000, CFoundation::CURRENCY_DOLLAR ) );
    spNewsChannel->SetColor( CFoundation::Color( (Unsigned8)220, 36, 36 ) );
    spNewsChannel = CreateNewsChannel();
    spNewsChannel->SetName( "Vermischtes" );
    spNewsChannel->SetPrice( CFoundation::Money( 10000, CFoundation::CURRENCY_DOLLAR ) );
    spNewsChannel->SetColor( CFoundation::Color( (Unsigned8)64, 160, 64 ) );

    // Spectator map
    m_spMap = MapPtr( new Map() );
}
        
Database::~Database()
{
}
        
void Database::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    _Serializer.Serialize( m_u32NextGenreID );
    _Serializer.SerializeVector< CFoundation::SerializeWrapper_SharedPtrSimple >( m_aspGenres );

    _Serializer.Serialize( m_u32NextBroadcastID );
    _Serializer.SerializeVector< CFoundation::SerializeWrapper_SharedPtrSimple >( m_aspBroadcasts );

    _Serializer.Serialize( m_u32NextAdvertID );
    _Serializer.SerializeVector< CFoundation::SerializeWrapper_SharedPtrSimple >( m_aspAdverts );

    _Serializer.Serialize( m_u32NextNewsChannelID );
    _Serializer.SerializeVector< CFoundation::SerializeWrapper_SharedPtrSimple >( m_aspNewsChannels );

    _Serializer.Serialize( m_u32NextNewsID );
    _Serializer.SerializeVector< CFoundation::SerializeWrapper_SharedPtrSimple >( m_aspNews );

    _Serializer.SerializeProxy< CFoundation::SerializeWrapper_SharedPtrSimple >( m_spMap );

    _Serializer.SerializeVector< CFoundation::SerializeWrapper_SharedPtrSimple >( m_aspSatellites );

    _Serializer.SerializeMap< CFoundation::SerializeWrapper_Context, CFoundation::SerializeWrapper_Simple >( m_mapSatellitePrices );

    _Serializer.SerializeVector< CFoundation::SerializeWrapper_SharedPtrSimple >( m_aspInitialAntennas );

    _Serializer.Serialize( m_u32NextPresentID );
    _Serializer.SerializeVector< CFoundation::SerializeWrapper_SharedPtrSimple >( m_aspPresents );

    _Serializer.SerializeArray< CFoundation::SerializeWrapper_Value >( m_au8Ratings, HOUR_END );

    /*
    // Serialize up2dateness of broadcasts
    Unsigned32 u32NumBroadcasts = m_aspBroadcasts.size();
    _Serializer.Serialize( u32NumBroadcasts );
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumBroadcasts; ++u32Idx )
    {
        // Get broadcast
        BroadcastPtr spBroadcast = GetBroadcast( u32Idx );
        
        // Serialize up2date
        Unsigned8 u8Up2date = spBroadcast != NULL ? spBroadcast->GetUp2date() : 100;
        _Serializer.Serialize( u8Up2date );

        // Set up2date
        if ( _Serializer.IsLoading() && 
             spBroadcast != NULL )
        {
            spBroadcast->SetUp2date( u8Up2date );
        }
    }*/
}
        
bool Database::LoadFromXML( const CSystem::FilePtr _spFile )
{
    // Load xml
    CFoundation::XMLDocumentPtr spDoc = CFoundation::XMLParser::GetInstance().ParseFile( _spFile->GetFullName() );
    if ( spDoc == NULL )
    {
        CF_WARN( "Database::LoadFromXML: Couldn't parse file %s.", _spFile->GetFullName().c_str() );
        return false;
    }

    // Get root
    CFoundation::XMLElementPtr spRoot = spDoc->GetRootElement();
    if ( spRoot == NULL )
    {
        CF_WARN( "Database::LoadFromXML: No root element." );
        return false;
    }

    // Load objects
    CFoundation::XMLNodePtr spNode = spRoot->GetFirstChild();
    while ( spNode != NULL )
    {
        if ( spNode->IsElement() )
        {
            CFoundation::XMLElementPtr spElement = static_pointer_cast< CFoundation::XMLElement >( spNode );

            // Get type
            const CFoundation::String& strType = spElement->GetName();

            if ( strType == "Broadcast" )
            {
                LoadBroadcast( spElement );
            }
            else if ( strType == "Advert" )
            {
                LoadAdvert( spElement );
            }
            else if ( strType == "News" )
            {
                LoadNews( spElement );
            }
            else if ( strType == "Map" )
            {
                // Initialize map
                CFoundation::Serializer serializer( CFoundation::Serializer::MODE_LOAD );
                if ( serializer.LoadFile( spElement->GetAttribute( "data" ) ) )
                {
                    m_spMap->Serialize( serializer );
                }
            }
            else if ( strType == "InitialAntenna" )
            {
                LoadInitialAntenna( spElement );
            }
            else if ( strType == "Satellite" )
            {
                LoadSatellite( spElement );
            }
            else if ( strType == "Present" )
            {
                LoadPresent( spElement );
            }
            else
            {
                CF_WARN( "Database::LoadFromXML: Unknown node %s.", strType.c_str() );
            }
        }

        spNode = spNode->GetNextSibling();
    }

    return true;
}
         
GenrePtr Database::CreateGenre()
{
    GenrePtr spGenre( new Genre( m_u32NextGenreID ) );
    
    m_aspGenres.push_back( spGenre );
    ++m_u32NextGenreID;

    return spGenre;
}

const GenreVec& Database::GetGenres() const
{
    return m_aspGenres;
}
        
Unsigned32 Database::GetNumGenres() const
{
    return m_aspGenres.size();
}
        
GenrePtr Database::GetGenre( Unsigned32 _u32ID ) const
{
    if ( _u32ID < m_aspGenres.size() )
    {
        return m_aspGenres[ _u32ID ];
    }
    else
    {
        return GenrePtr();
    }
}
        
BroadcastPtr Database::CreateBroadcast()
{
    BroadcastPtr spBroadcast( new Broadcast( m_u32NextBroadcastID ) );
    
    m_aspBroadcasts.push_back( spBroadcast );
    ++m_u32NextBroadcastID;

    return spBroadcast;
}

const BroadcastVec& Database::GetBroadcasts() const
{
    return m_aspBroadcasts;
}
        
Unsigned32 Database::GetNumBroadcasts() const
{
    return m_aspBroadcasts.size();
}
        
BroadcastPtr Database::GetBroadcast( Unsigned32 _u32ID ) const
{
    if ( _u32ID < m_aspBroadcasts.size() )
    {
        return m_aspBroadcasts[ _u32ID ];
    }
    else
    {
        return BroadcastPtr();
    }
}
        
AdvertPtr Database::CreateAdvert()
{
    AdvertPtr spAdvert( new Advert( m_u32NextAdvertID ) );
    
    m_aspAdverts.push_back( spAdvert );
    ++m_u32NextAdvertID;

    return spAdvert;
}

const AdvertVec& Database::GetAdverts() const
{
    return m_aspAdverts;
}
        
Unsigned32 Database::GetNumAdverts() const
{
    return m_aspAdverts.size();
}
        
AdvertPtr Database::GetAdvert( Unsigned32 _u32ID ) const
{
    if ( _u32ID < m_aspAdverts.size() )
    {
        return m_aspAdverts[ _u32ID ];
    }
    else
    {
        return AdvertPtr();
    }
}
         
NewsChannelPtr Database::CreateNewsChannel()
{
    NewsChannelPtr spNewsChannel( new NewsChannel( m_u32NextNewsChannelID ) );
    
    m_aspNewsChannels.push_back( spNewsChannel );
    ++m_u32NextNewsChannelID;

    return spNewsChannel;
}

const NewsChannelVec& Database::GetNewsChannels() const
{
    return m_aspNewsChannels;
}
        
Unsigned32 Database::GetNumNewsChannels() const
{
    return m_aspNewsChannels.size();
}
        
NewsChannelPtr Database::GetNewsChannel( Unsigned32 _u32ID ) const
{
    if ( _u32ID < m_aspNewsChannels.size() )
    {
        return m_aspNewsChannels[ _u32ID ];
    }
    else
    {
        return NewsChannelPtr();
    }
}
        
NewsPtr Database::CreateNews()
{
    NewsPtr spNews( new News( m_u32NextNewsID ) );
    
    m_aspNews.push_back( spNews );
    ++m_u32NextNewsID;

    return spNews;
}

const NewsVec& Database::GetNews() const
{
    return m_aspNews;
}
        
Unsigned32 Database::GetNumNews() const
{
    return m_aspNews.size();
}
        
NewsPtr Database::GetNews( Unsigned32 _u32ID ) const
{
    if ( _u32ID < m_aspNews.size() )
    {
        return m_aspNews[ _u32ID ];
    }
    else
    {
        return NewsPtr();
    }
}
     
MapPtr Database::GetMap() const
{
    return m_spMap;
}

Unsigned32 Database::GetNumSatellites() const
{
    return m_aspSatellites.size();
}

TransmitterPtr Database::GetSatellite( Unsigned32 _u32Idx ) const
{
    if ( _u32Idx < m_aspSatellites.size() )
    {
        return m_aspSatellites[ _u32Idx ];
    }
    else
    {
        return TransmitterPtr();
    }
}

Unsigned32 Database::GetSatelliteIdx( TransmitterPtr _spTransmitter ) const
{
    TransmitterVec::const_iterator it = std::find( m_aspSatellites.begin(), m_aspSatellites.end(), _spTransmitter );
    if ( it != m_aspSatellites.end() )
    {
        return it - m_aspSatellites.begin();
    }
    else
    {
        return MAX_U32;
    }
}
       
const TransmitterVec& Database::GetInitialAntennas() const
{
    return m_aspInitialAntennas;
}
        
bool Database::IsValidTransmitter( TransmitterPtr _spTransmitter ) const
{
    if ( m_spMap == NULL ||
         _spTransmitter == NULL )
    {
        return false;
    }

    switch ( _spTransmitter->GetType() )
    {
    case Transmitter::TYPE_ANTENNA:
        return m_spMap->IsValidPosition( _spTransmitter->GetX(), _spTransmitter->GetY() );
    case Transmitter::TYPE_SATELLITE:
        {
            TransmitterVec::const_iterator it = std::find( m_aspSatellites.begin(), m_aspSatellites.end(), _spTransmitter );
            return it != m_aspSatellites.end();
        }
        break;
    }

    return false;
}
        
CFoundation::Money Database::ComputeTransmitterPrice( TransmitterPtr _spTransmitter ) const
{
    CFoundation::Money price;

    if ( m_spMap == NULL ||
         _spTransmitter == NULL )
    {
        return price;
    }

    switch ( _spTransmitter->GetType() )
    {
    case Transmitter::TYPE_ANTENNA:
        {
            // Compute number of spectators
            Unsigned32 u32NumSpectators = m_spMap->ComputeNumSpectators( _spTransmitter->GetX(), _spTransmitter->GetY() );

            // Compute price
            price = ComputeAntennaPrice( u32NumSpectators );
        }
        break;
    case Transmitter::TYPE_SATELLITE:
        {
            // Satellites have a fixed price
            std::map< TransmitterPtr, CFoundation::Money >::const_iterator it = m_mapSatellitePrices.find( _spTransmitter );
            if ( it != m_mapSatellitePrices.end() )
            {
                price = it->second;
            }
        }
        break;
    }

    return price;
}
       
CFoundation::Money Database::ComputeAntennaPrice( const CFoundation::Vector2Di& _vPosition ) const
{
    CFoundation::Money price;
    if ( m_spMap == NULL )
    {
        return price;
    }

    // Compute number of spectators
    Unsigned32 u32NumSpectators = m_spMap->ComputeNumSpectators( _vPosition.GetX(), _vPosition.GetY() );

    // Compute price
    price = ComputeAntennaPrice( u32NumSpectators );

    return price;
}
       
CFoundation::Money Database::ComputeAntennaPrice( Unsigned32 _u32NumSpectators ) const
{
    if ( _u32NumSpectators < 2000000 )
    {
        return CFoundation::Money( 50000, CFoundation::CURRENCY_DOLLAR );
    }
    else if ( _u32NumSpectators < 4000000 )
    {
        return CFoundation::Money( 100000, CFoundation::CURRENCY_DOLLAR );
    }
    else if ( _u32NumSpectators < 10000000 )
    {
        return CFoundation::Money( 250000, CFoundation::CURRENCY_DOLLAR );
    }
    else
    {
        return CFoundation::Money( 500000, CFoundation::CURRENCY_DOLLAR );
    }
}

PresentPtr Database::CreatePresent()
{
    PresentPtr spPresent( new Present( m_u32NextPresentID ) );
    
    m_aspPresents.push_back( spPresent );
    ++m_u32NextPresentID;

    return spPresent;
}

const PresentVec& Database::GetPresents() const
{
    return m_aspPresents;
}

Unsigned32 Database::GetNumPresents() const
{
    return m_aspPresents.size();
}

PresentPtr Database::GetPresent( Unsigned32 _u32ID ) const
{
    if ( _u32ID < m_aspPresents.size() )
    {
        return m_aspPresents[ _u32ID ];
    }
    else
    {
        return PresentPtr();
    }
}

Unsigned8 Database::GetRating( Hour _eHour ) const
{
    return m_au8Ratings[ _eHour ];
}
        
void Database::LoadBroadcast( CFoundation::XMLElementPtr _spElement )
{
    // Create broadcast
    BroadcastPtr spBroadcast = CreateBroadcast();

    // Get title
    if ( _spElement->HasAttribute( "title" ) )
    {
        spBroadcast->SetTitle( _spElement->GetAttribute( "title" ) );
    }

    // Get description
    if ( _spElement->HasAttribute( "description" ) )
    {
        spBroadcast->SetDescription( _spElement->GetAttribute( "description" ) );
    }

    // Get genre
    if ( _spElement->HasAttribute( "genre" ) )
    {
        spBroadcast->SetGenre( GetGenre( _spElement->GetAttributeAsUnsigned32( "genre" ) ) );
    }

    // Get country
    if ( _spElement->HasAttribute( "country" ) )
    {
        spBroadcast->SetCountry( _spElement->GetAttributeAsEnum< Country >( "country" ) );
    }

    // Get year
    if ( _spElement->HasAttribute( "year" ) )
    {
        spBroadcast->SetYear( _spElement->GetAttributeAsUnsigned16( "year" ) );
    }

    // Get number of blocks
    if ( _spElement->HasAttribute( "num_blocks" ) )
    {
        spBroadcast->SetNumBlocks( _spElement->GetAttributeAsUnsigned32( "num_blocks" ) );
    }

    // Get price
    if ( _spElement->HasAttribute( "price" ) )
    {
        spBroadcast->SetPrice( CFoundation::Money( _spElement->GetAttributeAsUnsigned32( "price" ), CFoundation::CURRENCY_DOLLAR ) );
    }
}
        
void Database::LoadAdvert( CFoundation::XMLElementPtr _spElement )
{
    // Create advert
    AdvertPtr spAdvert = CreateAdvert();

    // Get name
    if ( _spElement->HasAttribute( "name" ) )
    {
        spAdvert->SetName( _spElement->GetAttribute( "name" ) );
    }

    // Get slogan
    if ( _spElement->HasAttribute( "slogan" ) )
    {
        spAdvert->SetSlogan( _spElement->GetAttribute( "slogan" ) );
    }

    // Get bonus
    if ( _spElement->HasAttribute( "bonus" ) )
    {
        spAdvert->SetBonus( CFoundation::Money( _spElement->GetAttributeAsUnsigned32( "bonus" ), CFoundation::CURRENCY_DOLLAR ) );
    }

    // Get penalty
    if ( _spElement->HasAttribute( "penalty" ) )
    {
        spAdvert->SetPenalty( CFoundation::Money( _spElement->GetAttributeAsUnsigned32( "penalty" ), CFoundation::CURRENCY_DOLLAR ) );
    }

    // Get min spectators
    if ( _spElement->HasAttribute( "min_spectators" ) )
    {
        spAdvert->SetMinSpectators( _spElement->GetAttributeAsUnsigned32( "min_spectators" ) );
    }

    // Get num spots
    if ( _spElement->HasAttribute( "num_spots" ) )
    {
        spAdvert->SetNumSpots( _spElement->GetAttributeAsUnsigned32( "num_spots" ) );
    }

    // Get num days
    if ( _spElement->HasAttribute( "num_days" ) )
    {
        spAdvert->SetNumDays( _spElement->GetAttributeAsUnsigned32( "num_days" ) );
    }
}
        
void Database::LoadNews( CFoundation::XMLElementPtr _spElement )
{
    // Create News
    NewsPtr spNews = CreateNews();

    // Get channel
    if ( _spElement->HasAttribute( "channel" ) )
    {
        // Find channel
        NewsChannelPtr spChannel = GetNewsChannel( _spElement->GetAttributeAsUnsigned32( "channel" ) );
        spNews->SetChannel( spChannel );
    }

    // Get text
    if ( _spElement->HasAttribute( "text" ) )
    {
        spNews->SetText( _spElement->GetAttribute( "text" ) );
    }

    // Get price
    if ( _spElement->HasAttribute( "price" ) )
    {
        spNews->SetPrice( CFoundation::Money( _spElement->GetAttributeAsUnsigned32( "price" ), CFoundation::CURRENCY_DOLLAR ) );
    }
}
        
void Database::LoadSatellite( CFoundation::XMLElementPtr _spElement )
{
    // Create satellite
    TransmitterPtr spSatellite( new Transmitter() );

    // Load coordinates
    Unsigned32 u32X = _spElement->GetAttributeAsUnsigned32( "x" );
    Unsigned32 u32Y = _spElement->GetAttributeAsUnsigned32( "y" );

    // Initialize satellite
    spSatellite->Init( Transmitter::TYPE_SATELLITE, u32X, u32Y );

    // Get price
    CFoundation::Money price( _spElement->GetAttributeAsUnsigned32( "price" ), CFoundation::CURRENCY_DOLLAR );
    m_mapSatellitePrices[ spSatellite ] = price;

    // Add satellite
    m_aspSatellites.push_back( spSatellite );
}
        
void Database::LoadInitialAntenna( CFoundation::XMLElementPtr _spElement )
{
    // Create antenna
    TransmitterPtr spAntenna( new Transmitter() );

    // Load coordinates
    Unsigned32 u32X = _spElement->GetAttributeAsUnsigned32( "x" );
    Unsigned32 u32Y = _spElement->GetAttributeAsUnsigned32( "y" );

    // Initialize antenna
    spAntenna->Init( Transmitter::TYPE_ANTENNA, u32X, u32Y );

    // Add antenna
    m_aspInitialAntennas.push_back( spAntenna );
}

void Database::LoadPresent( CFoundation::XMLElementPtr _spElement )
{
    // Create present
    PresentPtr spPresent = CreatePresent();

    // Get name
    if ( _spElement->HasAttribute( "name" ) )
    {
        spPresent->SetName( _spElement->GetAttribute( "name" ) );
    }

    // Get image
    if ( _spElement->HasAttribute( "image" ) )
    {
        // Load texture
        //CGui::Texture texture;
        //if ( CGui::TextureLoader::GetInstance().GetTextureFromDisk( _spElement->GetAttribute( "image" ), texture ) )
        {
            spPresent->SetImage( _spElement->GetAttribute( "image" ) );
        }
    }

    // Get price
    if ( _spElement->HasAttribute( "price" ) )
    {
        spPresent->SetPrice( CFoundation::Money( _spElement->GetAttributeAsUnsigned32( "price" ), CFoundation::CURRENCY_DOLLAR ) );
    }
}
