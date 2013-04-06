#ifndef CGUI_CURSOR_H
#define CGUI_CURSOR_H

#include "cfoundation/Math/Rect.h"
#include "cfoundation/Math/Vector.h"

namespace CGui
{
    /// Wrapper for the system mouse cursor
	class Cursor
	{
	public:
		enum CursorType
		{
			TYPE_NONE,
			TYPE_ARROW,
			TYPE_SIZE_HORIZONTAL,
			TYPE_SIZE_VERTICAL,
			TYPE_SIZE_DIAGONAL_LR,
			TYPE_SIZE_DIAGONAL_RL,
			NUM_TYPES
		};

        /// Constructor
		Cursor();

        /// Sets the type
		void        SetType( CursorType _eType );

        /// Shows/Hides the cursor
        void        SetVisible( bool _bVisible );

        /// Indicates if cursor is visible
        bool        IsVisible() const;

        /// Confines the cursor to a rectangular area on the window/screen
        void        ClipCursor( const CFoundation::RectI32& _Rect, bool _bScreen = false );

        /// Unclips the cursor
        void        UnclipCursor();

        /// Indicates if cursor is clipped
        /// _ClipRect returns the clip rectangle
        bool        IsClipped( CFoundation::RectI32& _ClipRect ) const;

	private:
        //! Current cursor type
		CursorType				m_eType;

        /// Indicates if the cursor is visible
        bool                    m_bVisible;

        /// Indicates if the cursor is clipped
        bool                    m_bClipped;

        /// Clip rect
        CFoundation::RectI32    m_rectClipRect;
	};
};

#endif
