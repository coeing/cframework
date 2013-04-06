#include "cgui/GUI/TextBox.h"

#include "cfoundation/Enum/EnumConverter.h"
#include "cfoundation/XML/XMLElement.h"

#include "cgui/Font/FontLoader.h"

#include "cgui/Gui/TextBoxView.h"
#include "cgui/Gui/SizeMode.h"

using namespace CGui;

void CFoundation::EnumConverter< TextBox::CutType >::Initialize()
{
    Assign( TextBox::CUT_CONTINUE, "continue" );
    Assign( TextBox::CUT_ABBR, "abbr" );
    Assign( TextBox::CUT_WRAP, "wrap" );
    Assign( TextBox::CUT_WORD_WRAP, "word wrap" );
}

TextBox::TextBox( const CFoundation::String &rName )
:	GuiObject( rName ),
	m_eCutType( CUT_CONTINUE ),
    m_i32PaddingLeft( 0 ),
    m_i32PaddingTop( 0 ),
    m_i32PaddingRight( 0 ),
    m_i32PaddingBottom( 0 )
{
	m_eAlign[ ALIGN_HORIZONTAL ] = ALIGN_LEFT;
	m_eAlign[ ALIGN_VERTICAL ] = ALIGN_MIDDLE;

	m_pView = m_pTextBoxView  = new TextBoxView();

    // Load default font
	m_spFont = FontLoader::GetInstance().LoadFontFromFile( "common/fonts/arial.ttf", 12 );
    m_pTextBoxView->SetFont( m_spFont );
}

TextBox::~TextBox()
{
    SAFE_DELETE( m_pView );
    m_pTextBoxView = NULL;
}

const CFoundation::String TextBox::GetTypeID()
{
	return "textbox";
}

const CFoundation::String TextBox::GetTypeId() const
{
	return TextBox::GetTypeID();
}
        
void TextBox::EndInit()
{
	GuiObject::EndInit();

    UpdateOutputText();
    UpdateTextPosition();
}

void TextBox::SetCutType( CutType _eType )
{
    if ( _eType == m_eCutType )
    {
        return;
    }

	m_eCutType = _eType;

    switch ( m_eCutType )
    {
    case CUT_LEFT:
    case CUT_CONTINUE:
        {
            UpdateTextPosition();
        }
        break;
    }
}
        
void TextBox::SetCutOffset( const CFoundation::Vector2Df& _vCutOffset )
{
    if ( _vCutOffset == m_vCutOffset )
    {
        return;
    }

    m_vCutOffset = _vCutOffset;

    // Update text position
    UpdateTextPosition();
}
        
const CFoundation::Vector2Df& TextBox::GetCutOffset() const
{
    return m_vCutOffset;
}

Integer32 TextBox::GetInnerWidth() const
{
	return GetWidth() - GetPaddingLeft() - GetPaddingRight();
}

Integer32 TextBox::GetInnerHeight() const
{
	return GetWidth() - GetPaddingTop() - GetPaddingBottom();
}

void TextBox::SetText( const CFoundation::String& _strText )
{
    SetText( STATE_NORMAL, _strText );
}

const CFoundation::String& TextBox::GetText() const
{
	return m_strText;
}
		
void TextBox::SetText( State _eState, const CFoundation::String& _strText )
{
    m_mapTexts[ _eState ] = _strText;

    SetOutputText( GetText( GetState() ) );
}

const CFoundation::String& TextBox::GetText( State _eState ) const
{
    do
    {
        // Try to find text
        StateStringMap::const_iterator it = m_mapTexts.find( _eState );
        if ( it != m_mapTexts.end() )
        {
            return it->second;
        }

        // Get fallback state
        _eState = GetFallbackState( _eState );

    } while ( _eState != STATE_NONE );

    static CFoundation::String strEmpty;
    return strEmpty;
}

void TextBox::SetFont( const CFoundation::String& _strFontName, Unsigned8 _u8FontSize )
{
	FontCPtr spFont = FontLoader::GetInstance().LoadFontFromFile( _strFontName, _u8FontSize );
    SetFont( spFont );
}

void TextBox::SetFont( FontCPtr _spFont )
{
    if ( m_spFont == _spFont )
    {
        return;
    }

	m_spFont = _spFont;

    m_pTextBoxView->SetFont( m_spFont );

    if ( !IsInitializing() )
    {
        UpdateOutputText();
        UpdateTextPosition();
    }
}
        
FontCPtr TextBox::GetFont() const
{
    return m_spFont;
}

void TextBox::SetTextColor( State _eState, const CFoundation::Color& _clColor )
{
    m_clTextColors[ _eState ] = _clColor;

	if ( GetState() == _eState )
	{
		m_pTextBoxView->SetTextColor( _clColor );
	}
}

const CFoundation::Color& TextBox::GetTextColor( State _eState ) const
{
    do
    {
        // Try to find text color
        StateColorMap::const_iterator it = m_clTextColors.find( _eState );
        if ( it != m_clTextColors.end() )
        {
            return it->second;
        }

        // Get fallback state
        _eState = GetFallbackState( _eState );

    } while ( _eState != STATE_NONE );

    static CFoundation::Color clDefaultTextColor( 0.0f, 0.0f, 0.0f );
    return clDefaultTextColor;
}

void TextBox::SetTextAlignment( AlignmentType eAlignType, Alignment eAlign )
{
    m_eAlign[ eAlignType ] = eAlign;

    if ( !IsInitializing() )
    {
	    UpdateTextPosition();
    }
}

Alignment TextBox::GetTextAlignment( AlignmentType eAlignType ) const
{
	switch( eAlignType )
	{
	case ALIGN_HORIZONTAL:
	case ALIGN_VERTICAL:
		{
			return m_eAlign[ eAlignType ];
		}
		break;
    default:
        return ALIGN_UNDEFINED;
	}
}
        
void TextBox::SetUnderlined( State _eState, bool _bUnderlined )
{
    m_mapUnderlined[ _eState ] = _bUnderlined;

	if ( GetState() == _eState )
	{
		m_pTextBoxView->SetUnderlined( _bUnderlined );
	}
}

bool TextBox::IsUnderlined( State _eState ) const
{
    do
    {
        // Try to find flag
        StateBoolMap::const_iterator it = m_mapUnderlined.find( _eState );
        if ( it != m_mapUnderlined.end() )
        {
            return it->second;
        }

        // Get fallback state
        _eState = GetFallbackState( _eState );

    } while ( _eState != STATE_NONE );

    return false;
}

void TextBox::SetPadding( Integer32 _i32PaddingLeft, Integer32 _i32PaddingTop, Integer32 _i32PaddingRight, Integer32 _i32PaddingBottom )
{
    SetPaddingLeft( _i32PaddingLeft );
    SetPaddingTop( _i32PaddingTop );
    SetPaddingRight( _i32PaddingRight );
    SetPaddingBottom( _i32PaddingBottom );
}

void TextBox::SetPaddingLeft( Integer32 _i32Padding )
{
    m_i32PaddingLeft = _i32Padding;
    if ( m_eAlign[ ALIGN_HORIZONTAL ] == ALIGN_LEFT )
    {
        UpdateTextPosition();
    }
}

Integer32 TextBox::GetPaddingLeft() const
{
	return m_i32PaddingLeft;
}

void TextBox::SetPaddingTop( Integer32 _i32Padding )
{
    m_i32PaddingTop = _i32Padding;
    if ( m_eAlign[ ALIGN_VERTICAL ] == ALIGN_BOTTOM )
    {
        UpdateTextPosition();
    }
}

Integer32 TextBox::GetPaddingTop() const
{
	return m_i32PaddingTop;
}

void TextBox::SetPaddingRight( Integer32 _i32Padding )
{
    if ( _i32Padding == m_i32PaddingRight )
    {
        return;
    }

    m_i32PaddingRight = _i32Padding;
    if ( m_eAlign[ ALIGN_HORIZONTAL ] == ALIGN_RIGHT ||
         m_eCutType == CUT_LEFT )
    {
        UpdateTextPosition();
    }
}

Integer32 TextBox::GetPaddingRight() const
{
	return m_i32PaddingRight;
}

void TextBox::SetPaddingBottom( Integer32 _i32Padding )
{
    m_i32PaddingBottom = _i32Padding;
    if ( m_eAlign[ ALIGN_VERTICAL ] == ALIGN_BOTTOM )
    {
        UpdateTextPosition();
    }
}

Integer32 TextBox::GetPaddingBottom() const
{
	return m_i32PaddingBottom;
}

const CFoundation::RectF32& TextBox::GetTextBB() const
{
	return m_clOutputTextBB;
}

void TextBox::SetState( State _eState )
{
    if ( GetState() == _eState )
    {
        return;
    }

    GuiObject::SetState( _eState );

    // Set ouptut text
    SetOutputText( GetText( _eState ) );

    // Set text color
    m_pTextBoxView->SetTextColor( GetTextColor( _eState ) );

    // Set underlined flag
    m_pTextBoxView->SetUnderlined( IsUnderlined( _eState ) );
}

void TextBox::SetSize( const CFoundation::Vector2Du& clSize )
{
	bool bWidthChanged = clSize.GetX() != GetWidth();
	bool bHeightChanged = clSize.GetY() != GetHeight();
	if ( bWidthChanged ||
		 bHeightChanged )
	{
		GuiObject::SetSize( clSize );

		if( bWidthChanged )
		{
			UpdateOutputText();
		}

		UpdateTextPosition();
	}
}
    
CFoundation::Vector2Du TextBox::ComputeSize()
{
    CFoundation::Vector2Du32 vNewSize = GuiObject::ComputeSize();

    // Use text rect if necessary
    SizeMode eSizeModeX = GetSizeModeX();
    SizeMode eSizeModeY = GetSizeModeY();
    if ( eSizeModeX == SIZE_MODE_CONTENT ||
         eSizeModeY == SIZE_MODE_CONTENT )
    {
        // The rect must always start at position (0,0)
        // because it should not be moved
        if ( eSizeModeX == SIZE_MODE_CONTENT )
        {
            Integer32 i32Width = static_cast< Integer32 >( m_clOutputTextBB.GetWidth() + m_clOutputTextBB.GetX() );
            if ( i32Width > 0 )
            {
                vNewSize.SetX( static_cast< Unsigned32 >( i32Width ) );
            }
        }

        if ( eSizeModeY == SIZE_MODE_CONTENT )
        {
            Integer32 i32Height = static_cast< Integer32 >( m_clOutputTextBB.GetHeight() + m_clOutputTextBB.GetY() );
            if ( i32Height > 0 )
            {
                vNewSize.SetY( static_cast< Unsigned32 >( i32Height ) );
            }
        }
    }

    return vNewSize;
}
        
void TextBox::SetOutputText( const CFoundation::String& _strText )
{
	if ( m_strText == _strText )
	{
        return;
    }

	m_strText = _strText;

    if ( !IsLayoutSuspended() )
    {
	    UpdateOutputText();
	    UpdateTextPosition();
	}
}

void TextBox::UpdateOutputText()
{
    // Remove current texts
    m_pTextBoxView->RemoveTexts();

	if ( m_spFont == NULL )
	{
        m_clOutputTextBB.Reset();
    }
    else
    {
        m_clOutputTextBB = m_spFont->GetBoundingBox( m_strText );
    }

    CFoundation::String strOutputText = m_strText;
    switch( m_eCutType )
    {
    case CUT_ABBR:
        {
            // get fitting text
            Float32 f32Width = static_cast< Float32 >( GetInnerWidth() );
            CFoundation::RectF32 clCurBB = m_clOutputTextBB;
            CFoundation::String sCurText = m_strText;
            bool bUseAbbr = false;

            while ( f32Width < clCurBB.GetWidth() )
            {
                CFoundation::String sTmp = "";
                if( sCurText.GetLength() > 0 )
                {
                    sTmp = sCurText.Head( sCurText.GetLength() - 1 );
                    bUseAbbr = true;
                }
                else
                {
                    bUseAbbr = false;
                    break;
                }

                sCurText = sTmp;
                sTmp += "[...]";
                clCurBB = m_spFont->GetBoundingBox( sTmp );
            }

            if ( bUseAbbr )
            {
                sCurText += "[...]";
            }

            strOutputText = sCurText;
            m_clOutputTextBB = clCurBB;
            m_pTextBoxView->SetText( strOutputText );
        }
        break;
    case CUT_WRAP:
        {
            Float32 f32Width = static_cast< Float32 >( GetInnerWidth() );
            CFoundation::RectF32 clCurBB = m_clOutputTextBB;
            if ( f32Width < clCurBB.GetWidth() )
            {
                // Reset text bounding box
                m_clOutputTextBB.Reset();

                // Build lines
                Unsigned32 u32Line = 0;
                CFoundation::String strTextLeft = m_strText;
                Unsigned32 u32NumCharsLeft = strTextLeft.GetLength();

                while ( u32NumCharsLeft > 0 )
                {
                    // Get characters till line is full
                    Unsigned32 u32From = 0;

                    // Skip whitespace
                    while ( u32From < u32NumCharsLeft )
                    {
                        if ( strTextLeft.IsWhitespace( u32From ) )
                        {
                            ++u32From;
                        }
                        else if ( u32From + 1 < u32NumCharsLeft &&
                             strTextLeft[ u32From ] == '\\' &&
                             strTextLeft[ u32From + 1 ] == 'n' )
                        {
                            u32From += 2;
                        }
                        else
                        {
                            break;
                        }
                    }

                    // Find end of line
                    Unsigned32 u32To = 0;
                    Unsigned32 u32Pos = u32From;
                    CFoundation::RectF32 clTmpBB;
                    while ( clTmpBB.GetWidth() < f32Width &&
                            u32Pos < u32NumCharsLeft )
                    {
                        // Check for manual line end
                        wchar_t ch = strTextLeft[ u32Pos ];
                        if ( ch == '\n' ||
                             ch == '\\' && u32Pos + 1 < u32NumCharsLeft && strTextLeft[ u32Pos + 1 ] == 'n' )
                        {
                            break;
                        }

                        CFoundation::String strTmp = strTextLeft.Middle( u32From, ++u32Pos );
                        u32To = u32Pos;
                        clTmpBB = m_spFont->GetBoundingBox( strTmp );
                    }

                    // Get at least one character
                    if ( u32To <= u32From )
                    {
                        u32To = u32From + 1;
                    }

                    // Get line
                    CFoundation::String strLine = strTextLeft.Middle( u32From, u32To );

                    // Trim line
                    strLine.Trim();

                    // Set line
                    m_pTextBoxView->SetText( strLine, u32Line );

                    // Expand bounding box
                    CFoundation::RectF32 clLineBB = m_spFont->GetBoundingBox( strLine );
                    clLineBB.SetPosition( 0, u32Line * m_spFont->GetHeight() );
                    m_clOutputTextBB.Expand( clLineBB );

                    ++u32Line;
                    strTextLeft = strTextLeft.Tail( u32To );
                    u32NumCharsLeft = strTextLeft.GetLength();
                }
            }
            else
            {
                // Text fits into one single line
                m_pTextBoxView->SetText( m_strText );
            }
        }
        break;
    case CUT_WORD_WRAP:
        {
            Float32 f32Width = static_cast< Float32 >( GetInnerWidth() );
            CFoundation::RectF32 clCurBB = m_clOutputTextBB;
            if ( f32Width > 0.0f &&
                 f32Width < clCurBB.GetWidth() )
            {
                // Reset text bounding box
                m_clOutputTextBB.Reset();

                // Build lines
                Unsigned32 u32Line = 0;
                CFoundation::String strTextLeft = m_strText;
                Unsigned32 u32NumCharsLeft = strTextLeft.GetLength();

                while ( u32NumCharsLeft > 0 )
                {
                    // Get words till line is full
                    Unsigned32 u32From = 0;

                    // Skip whitespace
                    while ( u32From < u32NumCharsLeft )
                    {
                        if ( strTextLeft.IsWhitespace( u32From ) )
                        {
                            ++u32From;
                        }
                        else if ( u32From + 1 < u32NumCharsLeft &&
                             strTextLeft[ u32From ] == '\\' &&
                             strTextLeft[ u32From + 1 ] == 'n' )
                        {
                            u32From += 2;
                        }
                        else
                        {
                            break;
                        }
                    }

                    CFoundation::RectF32 clTmpBB;
                    Unsigned32 u32To = 0;

                    // Check if complete text fits
                    clTmpBB = m_spFont->GetBoundingBox( strTextLeft );
                    if ( clTmpBB.GetWidth() <= f32Width )
                    {
                        u32To = u32NumCharsLeft;
                    }
                    else
                    {
                        // Find end of line
                        clTmpBB.Reset();
                        Unsigned32 u32Pos = u32From;
                        Unsigned32 u32ToFallback = u32From;
                        while ( clTmpBB.GetWidth() < f32Width &&
                                u32Pos < u32NumCharsLeft )
                        {
                            // Check for manual line end
                            wchar_t ch = strTextLeft[ u32Pos ];
                            if ( ch == '\n' ||
                                 ch == '\\' && u32Pos + 1 < u32NumCharsLeft && strTextLeft[ u32Pos + 1 ] == 'n' )
                            {
                                break;
                            }

                            CFoundation::String strTmp = strTextLeft.Head( ++u32Pos );
                            if ( strTextLeft.IsWhitespace( u32Pos ) )
                            {
                                u32To = u32Pos;
                            }
                            u32ToFallback = u32Pos;
                            clTmpBB = m_spFont->GetBoundingBox( strTmp );
                        }

                        // Use fallback if word is too long for one line
                        if ( u32To <= u32From )
                        {
                            u32To = u32ToFallback;
                        }
                    }

                    // Get line
                    CFoundation::String strLine = strTextLeft.Middle( u32From, u32To );

                    // Trim line
                    strLine.Trim();

                    // Set line
                    m_pTextBoxView->SetText( strLine, u32Line );

                    // Expand bounding box
                    CFoundation::RectF32 clLineBB = m_spFont->GetBoundingBox( strLine );
                    clLineBB.SetPosition( 0, u32Line * m_spFont->GetHeight() );
                    m_clOutputTextBB.Expand( clLineBB );

                    ++u32Line;
                    strTextLeft = strTextLeft.Tail( u32To );
                    u32NumCharsLeft = strTextLeft.GetLength();
                }
            }
            else
            {
                // Text fits into one single line
                m_pTextBoxView->SetText( m_strText );
            }
        }
        break;
    default:
        {
            m_pTextBoxView->SetText( m_strText );
        }
        break;
    }

    if ( GetSizeModeX() == SIZE_MODE_CONTENT ||
         GetSizeModeY() == SIZE_MODE_CONTENT )
    {
        // Update size
        UpdateSize();
    }
}

void TextBox::UpdateTextPosition()
{
	if ( m_strText == "" )
	{
        m_clOutputTextBB.SetX( GetPaddingLeft() );
		return;
	}

	// fit position
	switch( m_eAlign[ ALIGN_HORIZONTAL ] )
	{
	case ALIGN_LEFT:
		{
            if ( GetInnerWidth() < m_clOutputTextBB.GetWidth() &&
				 m_eCutType == CUT_LEFT )
            {
				m_clOutputTextBB.SetX( GetWidth() - m_clOutputTextBB.GetWidth() - GetPaddingRight() );
            }
            else
            {
			    m_clOutputTextBB.SetX( GetPaddingLeft() );
            }
		}
		break;
	case ALIGN_CENTER:
		{
			if ( GetInnerWidth() < m_clOutputTextBB.GetWidth() &&
				 m_eCutType == CUT_CONTINUE )
			{
				m_clOutputTextBB.SetX( GetPaddingLeft() );
			}
			else
			{
				m_clOutputTextBB.SetX( ( GetWidth() - m_clOutputTextBB.GetWidth() ) / 2 );
			}
		}
		break;
	case ALIGN_RIGHT:
		{
			m_clOutputTextBB.SetX( GetWidth() - m_clOutputTextBB.GetWidth() - GetPaddingRight() );
		}
		break;
    default:
        break;
	}

    switch( m_eAlign[ ALIGN_VERTICAL ] )
    {
    case ALIGN_TOP:
        {
            m_clOutputTextBB.SetY( GetPaddingTop() );
        }
        break;
    case ALIGN_MIDDLE:
        {
            m_clOutputTextBB.SetY( static_cast< Integer32 >( floor( GetHeight() * 1.0f - m_clOutputTextBB.GetHeight() ) / 2 ) );
        }
        break;
    case ALIGN_BOTTOM:
        {
            m_clOutputTextBB.SetY( GetHeight() - m_clOutputTextBB.GetHeight() - GetPaddingBottom() );
        }
        break;
    default:
        break;
    }

    m_pTextBoxView->SetTextPosition( m_clOutputTextBB.GetPosition() - m_vCutOffset );

    if ( GetSizeModeX() == SIZE_MODE_CONTENT ||
         GetSizeModeY() == SIZE_MODE_CONTENT )
    {
        // Update size
        UpdateSize();
    }
}

const CFoundation::String TextBoxCreator::GetTypeId() const
{
	return CFoundation::String( "textbox" );
}

GuiObjectPtr TextBoxCreator::CreateObject( const CFoundation::String &sName ) const
{
	TextBoxPtr spTextBox( new TextBox( sName ) );
	return spTextBox;
}

void TextBoxCreator::InitDefault( GuiObjectPtr _spObject ) const
{
	GuiObjectCreator::InitDefault( _spObject );
}

void TextBoxCreator::InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const
{
	GuiObjectCreator::InitFromXML( _spObject, _spXMLElement );

	TextBoxPtr spTextBox = static_pointer_cast< TextBox >( _spObject );

	CFoundation::String sTmp;

	// get text
    if ( _spXMLElement->HasAttribute( "text" ) )
    {
	    sTmp = _spXMLElement->GetAttribute( "text" );
	    spTextBox->SetText( STATE_NORMAL, sTmp );
    }

    // Get cut type
    if ( _spXMLElement->HasAttribute( "cut" ) )
    {
	    sTmp = _spXMLElement->GetAttribute( "cut" );
        const CFoundation::EnumConverter< TextBox::CutType >& clConverterCutType = CFoundation::EnumConverter< TextBox::CutType >::GetInstance();
        spTextBox->SetCutType( clConverterCutType.Convert( sTmp ) );
    }

	// get font and font size
    CFoundation::String sFontName = "common/fonts/arial.ttf";
	if ( _spXMLElement->HasAttribute( "font_name" ) )
    {
        sFontName = _spXMLElement->GetAttribute( "font_name" ); 
    }
    Unsigned8 u8FontSize = 12;
    if ( _spXMLElement->HasAttribute( "font_size" ) )
    {
        u8FontSize = _spXMLElement->GetAttribute( "font_size" ).ToUnsigned8();
    }
    spTextBox->SetFont( sFontName, u8FontSize );

	// Set default text color
    if ( _spXMLElement->HasAttribute( "text_color" ) )
    {
        sTmp = _spXMLElement->GetAttribute( "text_color" );
        spTextBox->SetTextColor( STATE_NORMAL, CFoundation::Color( sTmp ) );
    }

	// Set default underline flag
    if ( _spXMLElement->HasAttribute( "underline" ) )
    {
        spTextBox->SetUnderlined( STATE_NORMAL, _spXMLElement->GetAttributeAsBool( "underline" ) );
    }
	
	// set alignment
    const CFoundation::EnumConverter< Alignment >& clAlignmentConverter = CFoundation::EnumConverter< Alignment >::GetInstance();
	if ( _spXMLElement->HasAttribute( "text_align_horizontal" ) )
	{
        sTmp = _spXMLElement->GetAttribute( "text_align_horizontal" );
        spTextBox->SetTextAlignment( ALIGN_HORIZONTAL, clAlignmentConverter.Convert( sTmp ) );
	}
	if ( _spXMLElement->HasAttribute( "text_align_vertical" ) )
	{
        sTmp = _spXMLElement->GetAttribute( "text_align_vertical" );
        spTextBox->SetTextAlignment( ALIGN_VERTICAL, clAlignmentConverter.Convert( sTmp ) );
	}

    // Get padding
    if ( _spXMLElement->HasAttribute( "padding_left" ) )
    {
        spTextBox->SetPaddingLeft( _spXMLElement->GetAttribute( "padding_left" ).ToInteger32() );
    }
    if ( _spXMLElement->HasAttribute( "padding_top" ) )
    {
        spTextBox->SetPaddingTop( _spXMLElement->GetAttribute( "padding_top" ).ToInteger32() );
    }
    if ( _spXMLElement->HasAttribute( "padding_right" ) )
    {
        spTextBox->SetPaddingRight( _spXMLElement->GetAttribute( "padding_right" ).ToInteger32() );
    }
    if ( _spXMLElement->HasAttribute( "padding_bottom" ) )
    {
        spTextBox->SetPaddingBottom( _spXMLElement->GetAttribute( "padding_bottom" ).ToInteger32() );
    }

    // Read state infos    
    const CFoundation::EnumConverter< State >& clStateConverter = CFoundation::EnumConverter< State >::GetInstance();
    CFoundation::XMLNodePtr pChildNode = _spXMLElement->GetFirstChild();
    while ( pChildNode != NULL )
    {
        if ( pChildNode->IsElement() )
        {
            CFoundation::XMLElementPtr pElement = static_pointer_cast< CFoundation::XMLElement >( pChildNode );

            // Get state
            State eState = clStateConverter.Convert( pElement->GetName() );

            if ( eState != STATE_END )
            {
	            // Get text color
	            if ( pElement->HasAttribute( "text_color" ) )
	            {
		            const CFoundation::String& sTextColor = pElement->GetAttribute( "text_color" );
                    spTextBox->SetTextColor( eState, CFoundation::Color( sTextColor ) );
	            }
	            // Get underline flag
                if ( pElement->HasAttribute( "underline" ) )
                {
                    spTextBox->SetUnderlined( eState, pElement->GetAttributeAsBool( "underline" ) );
                }
	            // Get text
	            if ( pElement->HasAttribute( "text" ) )
	            {
		            const CFoundation::String& strText = pElement->GetAttribute( "text" );
                    spTextBox->SetText( eState, strText );
	            }
            }
        }

        pChildNode = pChildNode->GetNextSibling();
    }
}

void TextBoxCreator::InitFromTemplate( GuiObjectPtr _spObject, GuiObjectCPtr _spTemplate ) const
{
	GuiObjectCreator::InitFromTemplate( _spObject, _spTemplate );

	TextBoxPtr spTextBox = static_pointer_cast< TextBox >( _spObject );
	TextBoxCPtr spTextBoxTemplate = static_pointer_cast< const TextBox >( _spTemplate );

	// set text
	spTextBox->SetText( spTextBoxTemplate->GetText() );

	// set font and font size
    spTextBox->SetFont( spTextBoxTemplate->GetFont() );

	// set colors
    //spTextBox->SetTextColor( TEXT_COL_NORMAL, spTextBoxTemplate->GetTextColor( TEXT_COL_NORMAL ) );
	//spTextBox->SetTextColor( TEXT_COL_NOT_ACTIVE, spTextBoxTemplate->GetTextColor( TEXT_COL_NOT_ACTIVE ) );
	//spTextBox->SetTextColor( TEXT_COL_DISABLED, spTextBoxTemplate->GetTextColor( TEXT_COL_DISABLED ) );

	// set alignment
    spTextBox->SetTextAlignment( ALIGN_HORIZONTAL, spTextBoxTemplate->GetTextAlignment( ALIGN_HORIZONTAL ) );
    spTextBox->SetTextAlignment( ALIGN_VERTICAL, spTextBoxTemplate->GetTextAlignment( ALIGN_VERTICAL ) );

    // Set padding
    spTextBox->SetPaddingLeft( spTextBoxTemplate->GetPaddingLeft() );
    spTextBox->SetPaddingTop( spTextBoxTemplate->GetPaddingTop() );
    spTextBox->SetPaddingRight( spTextBoxTemplate->GetPaddingRight() );
    spTextBox->SetPaddingBottom( spTextBoxTemplate->GetPaddingBottom() );

    // Read state infos    
    for ( TextBox::StateColorMap::const_iterator it = spTextBoxTemplate->m_clTextColors.begin(); it != spTextBoxTemplate->m_clTextColors.end(); ++it )
    {
        spTextBox->SetTextColor( it->first, it->second );
    }
    for ( TextBox::StateBoolMap::const_iterator it = spTextBoxTemplate->m_mapUnderlined.begin(); it != spTextBoxTemplate->m_mapUnderlined.end(); ++it )
    {
        spTextBox->SetUnderlined( it->first, it->second );
    }
    for ( TextBox::StateStringMap::const_iterator it = spTextBoxTemplate->m_mapTexts.begin(); it != spTextBoxTemplate->m_mapTexts.end(); ++it )
    {
        spTextBox->SetText( it->first, it->second );
    }
}

    /*
void TextBoxCreator::AssignStyle( GuiObjectPtr _clObject, const CGui::CtrlStyle& _clStyle ) const
{
	GuiObjectCreator::AssignStyle( _clObject, _clStyle );

	TextBox& clTextBox = static_cast< TextBox& >( _clObject );

	CFoundation::XMLElementConstPtr pRoot = _clStyle.GetRoot();
    if ( pRoot == NULL )
    {
        return clTextBox;
    }

    // Init from xml element
    return InitFromXML( _clObject, *pRoot );

	CFoundation::String sTmp;

	// get font and font size
	if ( pRoot->HasAttribute( "font_name" ) &&
	     pRoot->HasAttribute( "font_size" ) )
    {
	    sTmp = pRoot->GetAttribute( "font_name" );
	    CFoundation::String sFontName = sTmp;
	    sTmp = pRoot->GetAttribute( "font_size" );
	    Unsigned8 u8FontSize = sTmp.ToUnsigned8();
	    clTextBox.SetFont( sFontName, u8FontSize );
    }

	// set colors
	if ( pRoot->HasAttribute( "color_normal" ) )
    {
	    sTmp = pRoot->GetAttribute( "color_normal" );
	    clTextBox.SetTextColor( TEXT_COL_NORMAL, CFoundation::Color( sTmp.HexToInteger() ) );
    }
	if ( pRoot->HasAttribute( "color_not_active" ) )
    {
	    sTmp = pRoot->GetAttribute( "color_not_active" );
	    clTextBox.SetTextColor( TEXT_COL_NOT_ACTIVE, CFoundation::Color( sTmp.HexToInteger() ) );
    }
	if ( pRoot->HasAttribute( "color_disabled" ) )
    {
	    sTmp = pRoot->GetAttribute( "color_disabled" );
	    clTextBox.SetTextColor( TEXT_COL_DISABLED, CFoundation::Color( sTmp.HexToInteger() ) );
    }

	// set alignment
    const CFoundation::EnumConverter< Alignment, ALIGNMENT_END >& clAlignmentConverter = CFoundation::EnumConverter< Alignment, ALIGNMENT_END >::GetInstance();
	if ( pRoot->HasAttribute( "text_align_horizontal" ) )
    {
	    sTmp = pRoot->GetAttribute( "text_align_horizontal" );
        clTextBox.SetTextAlignment( ALIGN_HORIZONTAL, clAlignmentConverter.ConvertFromString( sTmp ) );
    }
	if ( pRoot->HasAttribute( "color_disabled" ) )
    {
	    sTmp = pRoot->GetAttribute( "text_align_vertical" );
        clTextBox.SetTextAlignment( ALIGN_VERTICAL, clAlignmentConverter.ConvertFromString( sTmp ) );
    }

	return clTextBox;
}*/

bool TextBoxCreator::SaveToXML( GuiObjectCPtr _spObject, CFoundation::XMLDocumentPtr _spDoc, CFoundation::XMLElementPtr _spXMLElement ) const
{
	return GuiObjectCreator::SaveToXML( _spObject, _spDoc, _spXMLElement );

	//const TextBox &clTextBox = static_cast< const TextBox& >( clObject );

	//CFoundation::String sTmp;

	//// set text
	//_spXMLElement->SetAttribute( "text", clTextBox.GetText() );

	//// set font and font size
 //   FontCPtr spFont = clTextBox.GetFont();
 //   if ( spFont != NULL )
 //   {
 //       _spXMLElement->SetAttribute( "font_name", spFont->GetName() );
 //       _spXMLElement->SetAttribute( "font_size", spFont->GetSize() );
 //   }

	//// set colors
	//// _spXMLElement->SetAttribute( "color_normal", clTextBox.GetTextColor( TEXT_COL_NORMAL ).ToHex() );

	//// set alignment
 //   const CFoundation::EnumConverter< Alignment, ALIGNMENT_END >& clAlignmentConverter = CFoundation::EnumConverter< Alignment, ALIGNMENT_END >::GetInstance();
 //   _spXMLElement->SetAttribute( "text_align_horizontal", clAlignmentConverter.Convert( clTextBox.GetTextAlignment( ALIGN_HORIZONTAL ) ) );
 //   _spXMLElement->SetAttribute( "text_align_vertical", clAlignmentConverter.Convert( clTextBox.GetTextAlignment( ALIGN_VERTICAL ) ) );
}

