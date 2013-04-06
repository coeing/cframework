#include "cgui/GUI/TextBoxView.h"

#include "cfoundation/Debug/Assert.h"

#include "cgui/Renderer/Elements/RenderElementPrimitives.h"
#include "cgui/Renderer/Elements/RenderElementText.h"

using namespace CGui;

Unsigned32 TextBoxView::sID = 0;

TextBoxView::TextBoxView()
:	m_clTextColor( 0.0f, 0.0f, 0.0f ),
    m_bUnderlined( false )
{
}

#ifdef MULTIPLE_RENDER_ELEMENTS
void TextBoxView::Render( const RenderParams& _Params, std::vector< RenderElementPtr >& _aspRenderElements )
#else
void TextBoxView::Render( const RenderParams& _Params, std::vector< RenderElement >& _aRenderElements )
#endif
{
#ifdef MULTIPLE_RENDER_ELEMENTS
	GuiObjectView::Render( _Params, _aspRenderElements );
#else
	GuiObjectView::Render( _Params, _aRenderElements );
#endif

	if ( m_spFont == NULL )
	{
#ifndef FINAL
        for ( LineTextMap::const_iterator it = m_strTexts.begin(); it != m_strTexts.end(); ++it )
        {
            const CFoundation::String& strText = it->second;
			CF_WARN( L"TextBoxView::Render: No Font available to print out '%s'", strText.wc_str() );
		}
#endif
		return;
	}

    // Compute start position
    CFoundation::Vector2Di clBasePosition = _Params.GetPosition() + m_clTextPosition;

    // Create render elements
    for ( LineTextMap::const_iterator it = m_strTexts.begin(); it != m_strTexts.end(); ++it )
    {
        // Compute position
        Unsigned32 u32Line = it->first;
        const CFoundation::String& strText = it->second;
        if ( strText.GetLength() == 0 )
        {
            continue;
        }

        CFoundation::Vector2Di vPosition = clBasePosition;
        vPosition.AddY( ( u32Line + 1 ) * m_spFont->GetHeight() );

        // Create render element        
#ifdef MULTIPLE_RENDER_ELEMENTS
        RenderElementTextPtr spElement = RenderElementTextPtr( new RenderElementText() );
        spElement->SetClippingPlane( _Params.GetRenderRect() );
        spElement->SetFont( m_spFont );
        CFoundation::Color clColor = m_clTextColor;
        clColor.SetF16A( _Params.GetAlpha() * GetAlpha() );
        spElement->SetColor( clColor );
        spElement->SetText( strText );
        spElement->SetPosition( vPosition );

        // Add render element
        _aspRenderElements.push_back( spElement );
#else
        RenderElement newElement( RenderElement::TYPE_TEXT );
        newElement.SetClippingPlane( _Params.GetRenderRect() );
        newElement.SetFont( m_spFont );
        CFoundation::Color clColor = m_clTextColor;
        clColor.SetF16A( _Params.GetAlpha() * GetAlpha() );
        newElement.SetColor( clColor );
        newElement.SetText( strText );
        newElement.SetPosition( vPosition );

        // Add render element
        _aRenderElements.push_back( newElement );
#endif

        // Render underline
        if ( m_bUnderlined )
        {  
#ifdef MULTIPLE_RENDER_ELEMENTS
            // Create render element
            RenderElementPrimitivesPtr spElement = RenderElementPrimitivesPtr( new RenderElementPrimitives( RenderElementPrimitives::TYPE_LINES ) );
            spElement->SetClippingPlane( _Params.GetRenderRect() );
            spElement->SetColor( m_clTextColor );

            static const Unsigned32 GAP_TEXT_LINE = 2;
            spElement->AddPoint( CFoundation::Vector2Di( vPosition.GetX(), vPosition.GetY() + GAP_TEXT_LINE ) );
            spElement->AddPoint( CFoundation::Vector2Di( vPosition.GetX() + m_spFont->GetSize( strText ).GetX(), vPosition.GetY() + GAP_TEXT_LINE ) );

            // Add render element
            _aspRenderElements.push_back( spElement );
#else
            // Create render element
            RenderElement newElement( RenderElement::TYPE_LINES );
            newElement.SetClippingPlane( _Params.GetRenderRect() );
            newElement.SetColor( m_clTextColor );

            static const Unsigned32 GAP_TEXT_LINE = 2;
            newElement.AddPoint( CFoundation::Vector2Di( vPosition.GetX(), vPosition.GetY() + GAP_TEXT_LINE ) );
            newElement.AddPoint( CFoundation::Vector2Di( vPosition.GetX() + m_spFont->GetSize( strText ).GetX(), vPosition.GetY() + GAP_TEXT_LINE ) );

            // Add render element
            _aRenderElements.push_back( newElement );
#endif
        }
    }
}

void TextBoxView::SetFont( FontCPtr _spFont )
{
	m_spFont = _spFont;
}

void TextBoxView::SetText( const CFoundation::String& _strText, Unsigned32 _u32Line )
{
	m_strTexts[ _u32Line ] = _strText;
}

void TextBoxView::RemoveText( Unsigned32 _u32Line )
{
    LineTextMap::iterator it = m_strTexts.find( _u32Line );
    if ( it != m_strTexts.end() )
    {
        m_strTexts.erase( it );
    }
}

void TextBoxView::RemoveTexts()
{
    m_strTexts.clear();
}

void TextBoxView::SetTextColor( const CFoundation::Color &rColor )
{
	m_clTextColor = rColor;
}

void TextBoxView::SetTextPosition( const CFoundation::Vector2Di &clTextPosition )
{
	m_clTextPosition = clTextPosition;
}
       
void TextBoxView::SetUnderlined( bool _bUnderlined )
{
    m_bUnderlined = _bUnderlined;
}

bool TextBoxView::IsUnderlined() const
{
    return m_bUnderlined;
}

