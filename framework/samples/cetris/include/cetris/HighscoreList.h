#ifndef CETRIS_HIGHSCORELIST_H
#define CETRIS_HIGHSCORELIST_H

#include <vector>

#include "cfoundation/Text/String.h"
#include "cfoundation/Serialization/Serializable.h"

namespace Cetris
{
	class HighscoreList : public CFoundation::Serializable
	{
	public:
		HighscoreList();

		void AddHighscore( const CFoundation::String &sName, Unsigned32 u32Points );

		Unsigned8 GetNumHighscores() const;
		const CFoundation::String& GetName( Unsigned8 u8Idx ) const;
		Unsigned32 GetPoints( Unsigned8 u8Idx ) const;

		virtual void Save( CFoundation::Serializer &clSerializer ) const;
		virtual void Load( const CFoundation::Serializer &clSerializer );

	private:
		struct Highscore
		{
			CFoundation::String		m_sName;
			Unsigned32			m_u32Points;

			bool operator>( const Highscore &rhs ) const
			{
				return m_u32Points > rhs.m_u32Points;
			}
		};

		typedef std::vector<Highscore>	HighscoreVec;
		HighscoreVec		m_clHighscores;
	};
}

#endif
