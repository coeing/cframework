#ifndef CGUI_RENDERPARAMS_H
#define CGUI_RENDERPARAMS_H

#include "cfoundation/Math/Rect.h"
#include "cfoundation/Math/Vector.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( RenderWindow );

	class RenderParams
	{
	public:
        /// Constructor
		RenderParams();

		void                            SetRenderWindow( RenderWindowPtr _spWindow );
		void                            SetRenderRect( const CFoundation::RectI32 &rRect );

		RenderWindowPtr                 GetRenderWindow();
		RenderWindowCPtr                GetRenderWindow() const;

		CFoundation::RectI32&           GetRenderRect();
		const CFoundation::RectI32&     GetRenderRect() const;

		void                            AddToPosition( Integer32 i32DeltaX, Integer32 i32DeltaY );
		void                            AddToPosition( const CFoundation::Vector2Di& _vDelta );

		const CFoundation::Vector2Di&   GetPosition() const;
		const CFoundation::Vector3Di    GetRotation() const;

        /// Sets the current angle
        void                            SetAngle( Float16 _f16Angle );

        /// Adds the passed angle to the current value
        void                            AddAngle( Float16 _f16Angle );

        /// Returns the current angle
        Float16                         GetAngle() const;

        /// Sets the current alpha value
        void                            SetAlpha( Float16 _f16Alpha );

        /// Multiplies the passed alpha to the current alpha value
        void                            MultiplyAlpha( Float16 _f16Alpha );

        /// Returns the current alpha value
        Float16                         GetAlpha() const;

	private:
		RenderWindowPtr		    m_spRenderWindow;

		//! Aarea where to render the object (position, width and height)
		CFoundation::RectI32	m_clRenderRect;

		/// Position where the object should be rendered
		CFoundation::Vector2Di	m_clPosition;
		//! Scale
		CFoundation::Vector3Df	m_clScale;
		//! Rotation
		CFoundation::Vector3Di	m_clRotation;

        /// Current angle
        Float16                 m_f16Angle;

        /// Current alpha value
        Float16                 m_f16Alpha;
	};
};

#endif
