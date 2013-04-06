#ifndef CGUI_FONTLOADER_H
#define CGUI_FONTLOADER_H

#include <vector>

#include "cfoundation/Text/String.h"
#include "cfoundation/Utils/Singleton.h"

#include "cgui/Font/Font.h"

struct FT_FaceRec_;

namespace CGui
{
	class FontLoader : public CFoundation::Singleton<FontLoader>
	{
	public:
		FontPtr     LoadFontFromFile( const CFoundation::String& _strFontName, Unsigned8 _u8FontSize );

	private:
		friend class CFoundation::Singleton< FontLoader >;
		FontLoader();
		~FontLoader();

		void CreateCharacter( FT_FaceRec_ *pFace, wchar_t ch, Font &clFont );
		
		typedef std::vector< FontPtr > FontVec;
		FontVec		m_pCreatedFonts;
	};
}

#endif
