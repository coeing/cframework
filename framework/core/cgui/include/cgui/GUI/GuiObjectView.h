#ifndef CGUI_GUIOBJECT_VIEW
#define CGUI_GUIOBJECT_VIEW

#include <map>

#include "cfoundation/Utils/Color.h"

#include "cgui/GUI/Outline.h"
#include "cgui/GUI/RenderParams.h"

#include "cgui/Texture/Texture.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( RenderElement );

	class GuiObjectView
	{
	public:
		GuiObjectView();
		virtual ~GuiObjectView();

        virtual void                        SetSize( const CFoundation::Vector2Du32& _clSize );
        virtual void                        SetSize( Unsigned32 u32Width, Unsigned32 u32Height );

        virtual const CFoundation::Vector2Du&   GetSize() const;

		virtual void                        SetWidth( Unsigned32 u32Width );
		virtual Unsigned32                  GetWidth() const;

		virtual void                        SetHeight( Unsigned32 u32Height );
		virtual Unsigned32                  GetHeight() const;

		/// Sets the alpha value of this object
		virtual void                        SetAlpha( Float16 _f16Alpha );

		/// Returns the alpha value of this object
		virtual Float16                     GetAlpha() const;

		virtual void                        SetBgVisible( bool _bVisible );
		virtual bool                        IsBgVisible() const;

		virtual void                        SetBgColor( const CFoundation::Color &rColor );
		virtual const CFoundation::Color&       GetBgColor() const;

		virtual void                        SetTexture( const Texture& _clTexture );
		virtual const Texture&              GetTexture() const;

		virtual void                        SetMask( const Texture& _clMask );
		virtual const Texture&              GetMask() const;

		virtual void                        SetRepeatX( bool bRepeat );
		virtual bool                        GetRepeatX() const;
		virtual void                        SetRepeatY( bool bRepeat );
		virtual bool                        GetRepeatY() const;

		virtual void                        SetScaleX( bool bScale );
		virtual bool                        GetScaleX() const;
		virtual void                        SetScaleY( bool bScale );
		virtual bool                        GetScaleY() const;

        /// Sets the texture rectangle
        void                                SetTextureRect( const CFoundation::RectF16& _rectTexture );

        /// Returns the texture rectangle
        const CFoundation::RectF16&         GetTextureRect() const;

		virtual void                        SetOutline( Outline eOutline );
		virtual Outline                     GetOutline() const;

        /// Sets the rotation of the image (in degrees)
        virtual void                        SetRotation( Float16 _fAngle );

        /// Returns the rotation of the image (in degrees)
        virtual Float16                     GetRotation() const;

        /// Collects the render elements
#ifdef MULTIPLE_RENDER_ELEMENTS
		virtual void Render( const RenderParams& _Params, std::vector< RenderElementPtr >& _aspRenderElements );
#else
		virtual void Render( const RenderParams& _Params, std::vector< RenderElement >& _aRenderElements );
#endif

	private:
		void                                SetTexturePosition( const CFoundation::Vector2Df &clTexturePosition );
		void                                SetTextureSize( const CFoundation::Vector2Df &clTextureSize );

		void                                RecalcTextureRect();

        //! Renders the background
#ifdef MULTIPLE_RENDER_ELEMENTS
		void                                RenderBg( const RenderParams& _Params, std::vector< RenderElementPtr >& _aspRenderElements );
#else
		void                                RenderBg( const RenderParams& _Params, std::vector< RenderElement >& _aRenderElements );
#endif
#ifdef MULTIPLE_RENDER_ELEMENTS
		void                                RenderTexture( const RenderParams& _Params, std::vector< RenderElementPtr >& _aspRenderElements );
#else
		void                                RenderTexture( const RenderParams& _Params, std::vector< RenderElement >& _aRenderElements );
#endif

        //! Size of the render area
        CFoundation::Vector2Du32            m_vSize;

        //! Outline of the render area
		Outline				                m_eOutline;

        /// Alpha value
		Float16				                m_f16Alpha;

        //! Indicates if the background should be visible
        bool                                m_bBgVisible;

		//! The background color of the render area
		CFoundation::Color		            m_clBgColor;

        /// Texture
		Texture				                m_clTexture;

        /// Texture rect (uv and size)
		CFoundation::RectF16	            m_rectTexture;

        /// Mask
		Texture				                m_clMask;

		bool				                m_bRepeatX;
		bool				                m_bRepeatY;

		bool				                m_bScaleX;
		bool				                m_bScaleY;

        /// Rotation angle (in degrees)
        Float16                             m_f16Angle;
	};
}

#endif
