#ifndef CGUI_CHARMETRICS_H
#define CGUI_CHARMETRICS_H

#include "cfoundation/Math/Rect.h"

namespace CGui
{
	class CharMetrics
	{
	public:
		CharMetrics();
		explicit CharMetrics( const CFoundation::RectF32 &clBoundingBox );

		void                    SetBoundingBox( const CFoundation::RectF32 &clBoundingBox );
		const CFoundation::RectF32& GetBoundingBox() const;

        /// Sets the horizontal bearing x
        void                    SetHoriBearingX( Float32 _f32HoriBearingX );

        /// Returns the horizontal bearing x
        Float32                 GetHoriBearingX() const;

        /// Sets the horizontal bearing y
        void                    SetHoriBearingY( Float32 _f32HoriBearingY );

        /// Returns the horizontal bearing y
        Float32                 GetHoriBearingY() const;

        /// Sets the horizontal advance
        void                    SetHoriAdvance( Float32 _f32HoriAdvance );

        /// Returns the horizontal advance
        Float32                 GetHoriAdvance() const;

        /// Sets the vertical bearing x
        void                    SetVertBearingX( Float32 _f32VertBearingX );

        /// Returns the vertical bearing x
        Float32                 GetVertBearingX() const;

        /// Sets the vertical bearing y
        void                    SetVertBearingY( Float32 _f32VertBearingY );

        /// Returns the vertical bearing y
        Float32                 GetVertBearingY() const;

        /// Sets the vertical advance
        void                    SetVertAdvance( Float32 _f32VertAdvance );

        /// Returns the vertical advance
        Float32                 GetVertAdvance() const;

	private:
		CFoundation::RectF32	    m_clBoundingBox;

        /// Horizontal bearing x
        Float32                 m_f32HoriBearingX;

        /// Horizontal bearing y
        Float32                 m_f32HoriBearingY;

        /// Horizontal advance
        Float32                 m_f32HoriAdvance;

        /// Vertical bearing x
        Float32                 m_f32VertBearingX;

        /// Vertical bearing y
        Float32                 m_f32VertBearingY;

        /// Vertical advance
        Float32                 m_f32VertAdvance;
	};
}

#endif
