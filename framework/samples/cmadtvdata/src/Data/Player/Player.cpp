#include "cmadtv/Data/Player/Player.h"

#include <algorithm>

#include "cfoundation/Math/Utils.h"
#include "cfoundation/Serialization/Serializer.h"

#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Player/PlayerCompBetty.h"
#include "cmadtv/Data/Player/PlayerCompNews.h"
#include "cmadtv/Data/Player/PlayerCompSpectators.h"
#include "cmadtv/Data/Player/PlayerCompStats.h"
#include "cmadtv/Data/Player/Scheduling.h"

using namespace CMadTV;

Player::Player()
:   m_u8Image( 0 )
{
}

Player::~Player()
{
}

#define SHARED_PTR_COPY( spDest, spSrc )    if ( spDest == NULL || spSrc == NULL ) {    \
                                                spDest = spSrc; }                       \
                                            else {                                      \
                                                *spDest = *spSrc; }
        
void Player::operator=( const Player& _rhs )
{
    m_strName = _rhs.m_strName;
    m_colColor = _rhs.m_colColor;
    m_u8Image = _rhs.m_u8Image;
    m_clBroadcastRack = _rhs.m_clBroadcastRack;
    m_clBroadcastBag = _rhs.m_clBroadcastBag;
    m_clAdvertContracts = _rhs.m_clAdvertContracts;
    m_clAdvertBag = _rhs.m_clAdvertBag;
    m_clBudget = _rhs.m_clBudget;

    SHARED_PTR_COPY( m_spScheduling, _rhs.m_spScheduling );
    SHARED_PTR_COPY( m_spNews, _rhs.m_spNews );
    SHARED_PTR_COPY( m_spSpectators, _rhs.m_spSpectators );
    SHARED_PTR_COPY( m_spStats, _rhs.m_spStats );
    SHARED_PTR_COPY( m_spBetty, _rhs.m_spBetty );
}
        
void Player::Init( GamePtr _spGame )
{
    m_spGame = _spGame;
    m_spScheduling = SchedulingPtr( new Scheduling() );
    m_spNews = PlayerCompNewsPtr( new PlayerCompNews() );
    m_spNews->Init( GetSharedPtr() );
    m_spSpectators = PlayerCompSpectatorsPtr( new PlayerCompSpectators() );
    m_spStats = PlayerCompStatsPtr( new PlayerCompStats() );
    m_spBetty = PlayerCompBettyPtr( new PlayerCompBetty() );
}
        
void Player::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 5 );

    _Serializer.Serialize( m_strName );

    if ( u32Version >= 3 )
    {
        m_colColor.Serialize( _Serializer );
    }

    m_clBroadcastRack.Serialize( _Serializer );
    m_clBroadcastBag.Serialize( _Serializer );
    m_clAdvertContracts.Serialize( _Serializer );
    m_clAdvertBag.Serialize( _Serializer );
    _Serializer.SerializeProxy< CFoundation::SerializeWrapper_SharedPtrSimple >( m_spScheduling );
    m_clBudget.Serialize( _Serializer );

    _Serializer.SerializeProxy< CFoundation::SerializeWrapper_SharedPtrSimple >( m_spNews );
    if ( u32Version >= 2 )
    {
        _Serializer.SerializeProxy< CFoundation::SerializeWrapper_SharedPtrSimple >( m_spSpectators );
    }
    if ( u32Version >= 4 )
    {
        _Serializer.SerializeProxy< CFoundation::SerializeWrapper_SharedPtrSimple >( m_spStats );
    }
    if ( u32Version >= 5 )
    {
        _Serializer.SerializeProxy< CFoundation::SerializeWrapper_SharedPtrSimple >( m_spBetty );
    }

    _Serializer.Serialize( m_u8Image );
}
        
GamePtr Player::GetGame() const
{
    return m_spGame;
}

void Player::SetName( const CFoundation::String& _strName )
{
    m_strName = _strName;
}

const CFoundation::String& Player::GetName() const
{
    return m_strName;
}

void Player::SetColor( const CFoundation::Color& _colColor )
{
    m_colColor = _colColor;
}
      
const CFoundation::Color& Player::GetColor() const
{
    return m_colColor;
}
      
CFoundation::Color Player::GetColorLight() const
{
    Unsigned8 u8H, u8S, u8V;
    m_colColor.ToHSV( u8H, u8S, u8V );

    // Lower saturation
    u8S -= std::min( u8S, static_cast< Unsigned8 >( 20 ) );
    u8V += std::min( 100 - u8V, 20 );

    CFoundation::Color colLight = m_colColor;
    colLight.SetHSV( u8H, u8S, u8V );
    return colLight;
}
        
Unsigned8 Player::GetImage() const
{
    return m_u8Image;
}
       
void Player::SetImage( Unsigned8 _u8Image )
{
    CLAMP( _u8Image, 0, 100 );
    m_u8Image = _u8Image;
}

void Player::AddImage( Integer8 _i8Image )
{
    Integer8 i8NewImage = m_u8Image;
    i8NewImage += _i8Image;
    CLAMP( i8NewImage, 0, 100 );
    m_u8Image = i8NewImage;
}

BroadcastRack& Player::GetBroadcastRack()
{
    return m_clBroadcastRack;
}
        
BroadcastBag& Player::GetBroadcastBag()
{
    return m_clBroadcastBag;
}

AdvertContracts& Player::GetAdvertContracts()
{
    return m_clAdvertContracts;
}  

AdvertBag& Player::GetAdvertBag()
{
    return m_clAdvertBag;
}
        
SchedulingPtr Player::GetScheduling()
{
    return m_spScheduling;
}
        
Budget& Player::GetBudget()
{
    return m_clBudget;
}
        
PlayerCompNewsPtr Player::GetCompNews() const
{
    return m_spNews;
}
        
PlayerCompSpectatorsPtr Player::GetCompSpectators() const
{
    return m_spSpectators;
}
        
PlayerCompStatsPtr Player::GetCompStats() const
{
    return m_spStats;
}
        
PlayerCompBettyPtr Player::GetCompBetty() const
{
    return m_spBetty;
}
