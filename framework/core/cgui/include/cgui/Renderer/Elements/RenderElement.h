#ifndef CGUI_RENDER_ELEMENT_H
#define CGUI_RENDER_ELEMENT_H

#include "cfoundation/Math/Rect.h"
#include "cfoundation/Utils/SharedPtr.h"

#ifndef MULTIPLE_RENDER_ELEMENTS
#include "cfoundation/Utils/Color.h"
#include "cgui/Texture/Texture.h"
#endif

namespace CGui
{
#ifndef MULTIPLE_RENDER_ELEMENTS
    INCLUDE_SHARED_CLASS( Font );

    /** RenderElement
     *
     *  Base class that holds the information of an element that should be rendered
	 * 
	 */
	class RenderElement
    {
    public:
        typedef std::vector< CFoundation::Vector2Df > PointVec;

        /// Type
        enum Type
        {
            TYPE_RECT,
            TYPE_TEXTURED_RECT,
            TYPE_TEXT,
            TYPE_POINTS,
            TYPE_LINES,
            TYPE_TRIANGLES,
            TYPE_QUADS,

            TYPE_END
        };

        /// Constructor
        RenderElement( Type _eType = TYPE_END );

        /// Destructor
        virtual ~RenderElement();

        /// Sets the clipping plane
        void                            SetClippingPlane( const CFoundation::RectI32& _rectClipping );

        /// Returns the clipping plane
        const CFoundation::RectI32&     GetClippingPlane() const;

        /// Sets the position where the element should be rendered
        void                            SetPosition( const CFoundation::Vector2Di& _vPosition );

        /// Returns the position where the element should be rendered
        const CFoundation::Vector2Di&   GetPosition() const;

        /// Sets the size
        void                            SetSize( const CFoundation::Vector2Du& _vSize );

        /// Returns the size
        const CFoundation::Vector2Du&   GetSize() const;

        /// Sets the angle
        void                            SetAngle( Float16 _f16Angle );

        /// Returns the angle
        Float16                         GetAngle() const;

        /// Sets the mask to use
        void                            SetMask( const Texture& _Mask );

        /// Returns the mask to use
        const Texture&                  GetMask() const;

        /// Sets the text that should be rendered
        void                            SetText( const CFoundation::String& _strText );

        /// Returns the text that should be rendered
        const CFoundation::String&      GetText() const;

        /// Sets the font that should be us
        void                            SetFont( FontCPtr _spFont );

        /// Returns the font that should be used
        FontCPtr                        GetFont() const;

        /// Sets if the background should be visible
        void                            SetBgVisible( bool _bBgVisible );

        /// Indicates if the background should be visible
        bool                            IsBgVisible() const;

        /// Sets the color that should be us
        void                            SetBgColor( CFoundation::Color _Color );

        /// Returns the color that should be used
        CFoundation::Color              GetBgColor() const;

        /// Sets the texture to use
        void                            SetTexture( const Texture& _Texture );

        /// Returns the texture to use
        const Texture&                  GetTexture() const;

        /// Sets the texture rectangle
        void                            SetTextureRect( const CFoundation::RectF16& _rectTexture );

        /// Returns the texture rectangle
        const CFoundation::RectF16&     GetTextureRect() const;

        /// Returns the texture rectangle in relative coordinates
        const CFoundation::RectF16      GetRelativeTextureRect() const;

        /// Sets the type
        void                    SetType( Type _eType );

        /// Returns the type
        Type                    GetType() const;

        /// Clears all points
        void                    ClearPoints();

        /// Adds a point
        void                    AddPoint( const CFoundation::Vector2Df& _vPoint );

        /// Returns all points
        const std::vector< CFoundation::Vector2Df > GetPoints() const;

        /// Sets the thickness of the lines
        void                    SetThickness( Float16 _16Thickness );

        /// Returns the thickness of the lines
        Float16                 GetThickness() const;

        /// Sets the color that should be us
        void                    SetColor( const CFoundation::Color& _Color );

        /// Returns the color that should be used
        const CFoundation::Color&   GetColor() const;

    private:
        /// Clipping plane
        CFoundation::RectI32    m_rectClipping;

        /// Position
        CFoundation::Vector2Di  m_vPosition;

        /// Size
        CFoundation::Vector2Du  m_vSize;

        /// Angle
        Float16                 m_f16Angle;

        /// Text to render
        CFoundation::String     m_strText;

        /// Font to use
        FontCPtr                m_spFont;

        /// Texture to use
        Texture                 m_Texture;

        /// Mask to use
        Texture                 m_Mask;

        /// Indicates if the background should be visible
        bool                    m_bBgVisible;

        /// Color to use
        CFoundation::Color      m_BgColor;

        /// Color to use
        CFoundation::Color      m_FgColor;

        /// Texture rect
        CFoundation::RectF16    m_rectTexture;

        /// Type of primitive
        Type                    m_eType;

        /// Points
        PointVec                m_avPoints;

        /// Thickness of the lines
        Float16                 m_f16Thickness;
    };

    INCLUDE_SHARED_CLASS( RenderElement );
#else
    /** RenderElement
     *
     *  Base class that holds the information of an element that should be rendered
	 * 
	 */
	class RenderElement
    {
    public:
        /// Constructor
        RenderElement();

        /// Destructor
        virtual ~RenderElement();

        /// Sets the clipping plane
        void                            SetClippingPlane( const CFoundation::RectI32& _rectClipping );

        /// Returns the clipping plane
        const CFoundation::RectI32&     GetClippingPlane() const;

        /// Sets the position where the element should be rendered
        void                            SetPosition( const CFoundation::Vector2Di& _vPosition );

        /// Returns the position where the element should be rendered
        const CFoundation::Vector2Di&   GetPosition() const;

    private:
        /// Clipping plane
        CFoundation::RectI32    m_rectClipping;

        /// Position
        CFoundation::Vector2Di  m_vPosition;
    };
#endif
}

#endif
