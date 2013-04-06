#ifndef CGUI_FONT_H
#define CGUI_FONT_H

#include "cfoundation/Text/String.h"
#include "cfoundation/Utils/SharedPtr.h"

#include "cgui/Font/CharMetrics.h"

namespace CGui
{
	class Font
	{
	public:
        /// Destructor
		~Font();

		const CFoundation::String& GetName() const;

		Unsigned8	GetSize() const;

		Unsigned8	GetHeight() const;
		Unsigned8	GetDepth() const;

		void		Activate() const;
		void		Deactivate() const;

		CFoundation::RectF32    GetBoundingBox( const CFoundation::String& _strText ) const;

        /// Returns the size of the passed text
        CFoundation::Vector2Df  GetSize( const CFoundation::String& _strText ) const;

        /// Returns the position of the passed offset in the passed text
        Float32                 GetPosition( const CFoundation::String& _strText, Float32 _f32Offset ) const;

	private:
		friend class FontLoader;

		//! Fonts can only be created by FontLoader
		Font();
		explicit Font( const CFoundation::String &sFontName, Unsigned8 u8FontSize );

		static const Unsigned16 NUM_CHARS = 512;

		CFoundation::String		m_sName;

		//! The size is the height of the whole font
		Unsigned8			m_u8Size;
		//! The height goes from the baseline to the rooftop
		mutable Integer8	m_i8Height;
		//! The depth goes from the baseline to the ground
		mutable Integer8	m_i8Depth;

		CharMetrics			m_clCharMetrics[ NUM_CHARS ];

		Unsigned32			m_u32FontBase;
		Unsigned32			m_u32Textures[ NUM_CHARS ];

		//void		BuildFont();
		//void		DestroyFont();

		void		CalculateHeightDepth() const;
	};

    /// Font pointer
    typedef SharedPtr< Font > FontPtr;

    /// Font const pointer
    typedef SharedPtr< const Font > FontCPtr;
}

#endif
