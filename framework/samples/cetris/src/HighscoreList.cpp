#include "cetris/HighscoreList.h"

#include <algorithm>
#include <functional>

#include "cfoundation/Serialization/Serializer.h"

using namespace Cetris;

HighscoreList::HighscoreList()
{
}

void HighscoreList::AddHighscore( const CFoundation::String &sName, Unsigned32 u32Points )
{
	Highscore clNewHighscore;
	clNewHighscore.m_sName = sName;
	clNewHighscore.m_u32Points = u32Points;

	m_clHighscores.push_back( clNewHighscore );

	// sort highscores
	std::sort( m_clHighscores.begin(), m_clHighscores.end(), std::greater<Highscore>() );
}

Unsigned8 HighscoreList::GetNumHighscores() const
{
	return static_cast< Unsigned8 >( m_clHighscores.size() );
}

const CFoundation::String& HighscoreList::GetName( Unsigned8 u8Idx ) const
{
	return m_clHighscores[ u8Idx ].m_sName;
}

Unsigned32 HighscoreList::GetPoints( Unsigned8 u8Idx ) const
{
	return m_clHighscores[ u8Idx ].m_u32Points;
}
		
void HighscoreList::Save( CFoundation::Serializer &clSerializer ) const
{
	Unsigned8 u8Size = GetNumHighscores();
    clSerializer.Add( u8Size );
	for( Unsigned8 u8Idx = 0; u8Idx < u8Size; ++u8Idx )
	{
		clSerializer.Add( m_clHighscores[ u8Idx ].m_sName );
		clSerializer.Add( m_clHighscores[ u8Idx ].m_u32Points );
	}
}
		
void HighscoreList::Load( const CFoundation::Serializer &clSerializer )
{
	Unsigned8 u8Size = clSerializer.GetU8();
	for( Unsigned8 u8Idx = 0; u8Idx < u8Size; ++u8Idx )
	{
		const CFoundation::String sName = clSerializer.GetString();
		Unsigned32 u32Points = clSerializer.GetU32();
		AddHighscore( sName, u32Points );
	}
}