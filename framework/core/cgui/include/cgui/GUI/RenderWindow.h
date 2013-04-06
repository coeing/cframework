#ifndef CGUI_RENDERWINDOW_H
#define CGUI_RENDERWINDOW_H

#include "cfoundation/Base/Types.h"
#include "cfoundation/Math/Vector.h"
#include "cfoundation/Text/String.h"
#include "cfoundation/Utils/Singleton.h"

namespace CGui
{
	class RenderWindow
	{
	public:
		Unsigned32 GetWidth() const;
		Unsigned32 GetHeight() const;

		void SetTitle( const CFoundation::String &sTitle );
		const CFoundation::String GetTitle() const;

        /// Converts the passed render window point to screen point
        CFoundation::Vector2Di32    ClientToScreen( const CFoundation::Vector2Di32& _vPoint ) const;

        /// Converts the passed screen point to render window point
        CFoundation::Vector2Di32    ScreenToClient( const CFoundation::Vector2Di32& _vPoint ) const;

	private:
		friend class CFoundation::Singleton< RenderWindow >;
		friend class GuiMain;

		RenderWindow();

		bool Init( const CFoundation::String &sWindowName, Unsigned32 u32Width, Unsigned32 u32Height );

		void Update();

		Unsigned32	m_u32Width;
		Unsigned32	m_u32Height;
	};
};

#endif

