#ifndef CGUI_TEXTURE_H
#define CGUI_TEXTURE_H

#include "cfoundation/Math/Vector.h"
#include "cfoundation/Text/String.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CGui
{
	class TextureLoader;
    INCLUDE_SHARED_CLASS( TextureParams );

	class Texture
	{
	public:
		enum TexType
		{
			TYPE_UNKNOWN,
			TYPE_BMP,
			TYPE_JPG,
			TYPE_GIF,
			TYPE_TGA,
			TYPE_ICO,
			TYPE_WMF,
			TYPE_EMF,
			TYPE_TIF,
			TYPE_PNG,
			NUM_TYPES
		};

		Texture();

        //! Compares two textures
        bool                        operator==( const Texture& _rhs ) const;

        //! Compares two textures
        bool                        operator!=( const Texture& _rhs ) const;

		void                        Clear();

		bool                        IsValid() const;

		Unsigned32                  GetID() const;

		const CFoundation::String       GetFileName() const;

		Unsigned32                      GetWidth() const;
		Unsigned32                      GetHeight() const;
		const CFoundation::Vector2Du&   GetSize() const;

	private:
		friend class TextureLoader;

		Unsigned32		        m_u32ID;
		TextureParamsCPtr       m_spParams;

		CFoundation::Vector2Du  m_clSize;
		Unsigned16		        m_u16Bpp;

		TexType			        m_eTexType;
		Unsigned16		        m_eColorType;
	};
}

#endif
