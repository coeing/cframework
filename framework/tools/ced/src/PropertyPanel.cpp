#include "ced/PropertyPanel.h"

#include "cfoundation/Debug/Assert.h"

#include "cgui/GUI/Button.h"
#include "cgui/GUI/Image.h"

using namespace Ced;

PropertyPanel::PropertyPanel()
{
    // Create delegates
    m_spDelegateSizeChanged = CGui::SizeEventDelegate0Ptr( new CGui::SizeEventDelegate0( this, &PropertyPanel::OnSizeChanged ) );
    m_spDelegatePositionChanged = CGui::PositionEventDelegate0Ptr( new CGui::PositionEventDelegate0( this, &PropertyPanel::OnPositionChanged ) );
    m_spDelegateListBoxValueChanged = CGui::ListBoxEventU32U32StrDelegate4Ptr( new CGui::ListBoxEventU32U32StrDelegate4( this, &PropertyPanel::OnListBoxValueChanged ) );
}

void PropertyPanel::Init( CGui::PanelPtr _spPnlProperties )
{
    m_spListBox = static_pointer_cast< CGui::ListBox >( _spPnlProperties->FindChild( "LbProperties" ) );
	CF_ASSERT( m_spListBox != NULL, L"No ListBox LbProperties in XML file for MainWindow" );
	//m_spListBoxChild = static_pointer_cast< CGui::ListBox >( _spPnlProperties->FindChild( "LbPropertiesChild" ) );
	//CF_ASSERT( m_spListBoxChild != NULL, L"No ListBox LbPropertiesChild in XML file for MainWindow" );

    m_spListBox->RegisterDelegate( CGui::EVENT_LISTBOX_VALUE_CHANGED, m_spDelegateListBoxValueChanged );
}

void PropertyPanel::ProcessMessage( const CFoundation::Message& clMessage )
{
}

void PropertyPanel::SetSelected( CGui::GuiObjectPtr pObject )
{
    if ( m_spSelObject == pObject )
    {
        return;
    }

    // Remove delegates
    if ( m_spSelObject != NULL )
    {
        m_spSelObject->RemoveDelegate( CGui::SIZE_EVENT_SIZE_CHANGED, m_spDelegateSizeChanged );
        m_spSelObject->RemoveDelegate( CGui::POSITION_EVENT_POSITION_CHANGED, m_spDelegatePositionChanged );
        m_spSelObject->RemoveListener( GetSharedPtr() );
    }

    m_spSelObject = pObject;

    // Register delegates
    if ( m_spSelObject != NULL )
    {
        m_spSelObject->RegisterDelegate( CGui::SIZE_EVENT_SIZE_CHANGED, m_spDelegateSizeChanged );
        m_spSelObject->RegisterDelegate( CGui::POSITION_EVENT_POSITION_CHANGED, m_spDelegatePositionChanged );
        m_spSelObject->AddListener( GetSharedPtr() );
    }

    Update();
}

void PropertyPanel::SetProp( Property eProp, const CFoundation::String &sValue )
{
    if ( m_spSelObject == NULL )
    {
        return;
    }

    CGui::GuiObjectPtr pGuiObject = m_spSelObject;
    if ( pGuiObject != NULL )
    {
        SetGeneralProp( pGuiObject, eProp, sValue );
    }
    CGui::ButtonPtr pButton = dynamic_pointer_cast<CGui::Button>( m_spSelObject );
    if ( pButton != NULL )
    {
        SetButtonProp( pButton, eProp, sValue );
    }
    CGui::ImagePtr pImage = dynamic_pointer_cast<CGui::Image>( m_spSelObject );
    if ( pImage != NULL )
    {
        SetImageProp( pImage, eProp, sValue );
    }
    CGui::ListBoxPtr pListBox = dynamic_pointer_cast<CGui::ListBox>( m_spSelObject );
    if ( pListBox != NULL )
    {
        SetListBoxProp( pListBox, eProp, sValue );
    }
}

void PropertyPanel::Update()
{
    // Clear
    m_spListBox->ClearContent();

    for ( Unsigned32 u32Idx = 0; u32Idx < NUM_PROPS; ++u32Idx )
    {
        m_u32Rows[ u32Idx ] = MAX_U32;
    }

    if ( m_spSelObject != NULL )
    {
        CGui::GuiObjectCPtr pGuiObject = m_spSelObject;
        if ( pGuiObject != NULL )
        {
            AddGeneralProps( pGuiObject );

            CGui::ButtonCPtr pButton = dynamic_pointer_cast<const CGui::Button>( m_spSelObject );
            if ( pButton != NULL )
            {
                AddButtonProps( pButton );
            }
            CGui::ImageCPtr pImage = dynamic_pointer_cast<const CGui::Image>( m_spSelObject );
            if ( pImage != NULL )
            {
                AddImageProps( pImage );
            }
            CGui::ListBoxCPtr pListBox = dynamic_pointer_cast<const CGui::ListBox>( m_spSelObject );
            if ( pListBox != NULL )
            {
                AddListBoxProps( pListBox );
            }
        }
    }
}

void PropertyPanel::AddGeneralProps( CGui::GuiObjectCPtr _spGuiObject )
{
    Unsigned32 u32Idx = m_spListBox->GetNumRows();

    m_spListBox->AddColumnData( "Name:" );
    m_spListBox->AddColumnData( _spGuiObject->GetName(), 0, CGui::TYPE_EDITBOX );
    m_spListBox->AddRow();
    m_u32Rows[ PROP_NAME ] = u32Idx++;

    m_spListBox->AddColumnData( "Attach Object:" );
    std::vector< CFoundation::String > astrValues;
    std::vector< Integer64 > ai64Data;
    for ( Unsigned32 u32Idx = 0; u32Idx < CGui::ATTACH_OBJECT_END; ++u32Idx )
    {
        astrValues.push_back( CGui::g_ConverterAttachObject.Convert( static_cast< CGui::AttachObject >( u32Idx ) ) );
        ai64Data.push_back( u32Idx );
    }
    m_spListBox->AddColumnData( astrValues, ai64Data );
    m_spListBox->AddRow();
    m_u32Rows[ PROP_ATTACH_OBJECT ] = u32Idx++;

    m_spListBox->AddColumnData( "Attach Point Local:" );
    //m_spListBox->AddColumnData();
    m_spListBox->AddRow();
    m_u32Rows[ PROP_ATTACH_LOCAL ] = u32Idx++;

    m_spListBox->AddColumnData( "Attach Point Reference:" );
    //m_spListBox->AddColumnData();
    m_spListBox->AddRow();
    m_u32Rows[ PROP_ATTACH_REFERENCE ] = u32Idx++;

    m_spListBox->AddColumnData( "Offset X:" );
    m_spListBox->AddColumnData( _spGuiObject->GetOffset().GetX(), 0, CGui::TYPE_EDITBOX );
    m_spListBox->AddRow();
    m_u32Rows[ PROP_POSX ] = u32Idx++;

    m_spListBox->AddColumnData( "Offset Y:" );
    m_spListBox->AddColumnData( _spGuiObject->GetOffset().GetY(), 0, CGui::TYPE_EDITBOX );
    m_spListBox->AddRow();
    m_u32Rows[ PROP_POSY ] = u32Idx++;

    m_spListBox->AddColumnData( "Width:" );
    m_spListBox->AddColumnData( _spGuiObject->GetWidth(), 0, CGui::TYPE_EDITBOX );
    m_spListBox->AddRow();
    m_u32Rows[ PROP_WIDTH ] = u32Idx++;

    m_spListBox->AddColumnData( "Height:" );
    m_spListBox->AddColumnData( _spGuiObject->GetHeight(), 0, CGui::TYPE_EDITBOX );
    m_spListBox->AddRow();
    m_u32Rows[ PROP_HEIGHT ] = u32Idx++;

    m_spListBox->AddColumnData( "Bg visible:" );
    m_spListBox->AddColumnData( _spGuiObject->IsBgVisible( CGui::STATE_NORMAL ), 0, CGui::TYPE_CHECKBOX );
    m_spListBox->AddRow();
    m_u32Rows[ PROP_BGTRANSPARENT ] = u32Idx++;

    m_spListBox->AddColumnData( "Bg color:" );
    m_spListBox->AddColumnData( _spGuiObject->GetBgColor( CGui::STATE_NORMAL ).GetHexString(), 0, CGui::TYPE_EDITBOX );
    m_spListBox->AddRow();
    m_u32Rows[ PROP_BGCOLOR ] = u32Idx++;
}

void PropertyPanel::SetGeneralProp( CGui::GuiObjectPtr _spGuiObject, Property eProp, const CFoundation::String& sValue )
{
    if ( eProp == PROP_NAME )
    {
        _spGuiObject->SetName( sValue );
    }
    else if ( eProp == PROP_ATTACH_OBJECT )
    {
        //CFoundation::Vector2Di vOffset = _spGuiObject->GetOffset();
        //vOffset.SetX( sValue.ToInteger32() );
        //_spGuiObject->SetOffset( vOffset );
    }
    else if ( eProp == PROP_POSX )
    {
        CFoundation::Vector2Di vOffset = _spGuiObject->GetOffset();
        vOffset.SetX( sValue.ToInteger32() );
        _spGuiObject->SetOffset( vOffset );
    }
    else if ( eProp == PROP_POSY )
    {
        CFoundation::Vector2Di vOffset = _spGuiObject->GetOffset();
        vOffset.SetY( sValue.ToInteger32() );
        _spGuiObject->SetOffset( vOffset );
    }
    else if ( eProp == PROP_WIDTH )
    {
        CFoundation::Vector2Du vSize = _spGuiObject->GetCustomSize();
        vSize.SetX( sValue.ToUnsigned32() );
        _spGuiObject->SetCustomSize( vSize );
    }
    else if ( eProp == PROP_HEIGHT )
    {
        CFoundation::Vector2Du vSize = _spGuiObject->GetCustomSize();
        vSize.SetY( sValue.ToUnsigned32() );
        _spGuiObject->SetCustomSize( vSize );
    }
    else if ( eProp == PROP_BGTRANSPARENT )
    {
        _spGuiObject->SetBgVisible( CGui::STATE_NORMAL, sValue != "0" ? true : false );
    }
    else if ( eProp == PROP_BGCOLOR )
    {
        _spGuiObject->SetBgColor( CFoundation::Color( sValue ) );
    }
}

void PropertyPanel::AddButtonProps( CGui::ButtonCPtr _spButton )
{
    Unsigned32 u32Idx = m_spListBox->GetNumRows();

    m_spListBox->AddColumnData( "Text:" );
    m_spListBox->AddColumnData( _spButton->GetText(), 0, CGui::TYPE_EDITBOX );
    m_spListBox->AddRow();
    m_u32Rows[ PROP_TEXT ] = u32Idx++;

    //m_spListBox->AddColumnData( "Font:" );
    //m_spListBox->AddColumnData( _spButton->GetFontName(), 0, CGui::TYPE_EDITBOX );
    //m_spListBox->AddRow();
    //m_u32Rows[ PROP_FONT ] = u32Idx++;

    //m_spListBox->AddColumnData( "Font Size:" );
    //m_spListBox->AddColumnData( _spButton->GetFontSize(), 0, CGui::TYPE_EDITBOX );
    //m_spListBox->AddRow();
    //m_u32Rows[ PROP_FONT_SIZE ] = u32Idx++;
}
void PropertyPanel::SetButtonProp( CGui::ButtonPtr _spButton, Property eProp, const CFoundation::String &sValue )
{
    if ( eProp == PROP_TEXT )
    {
        _spButton->SetText( sValue );
    }
    else if ( eProp == PROP_FONT )
    {
//        _spButton->SetFont( sValue, _spButton->GetFontSize() );
    }
    else if ( eProp == PROP_FONT_SIZE )
    {
//        _spButton->SetFont( _spButton->GetFontName(), sValue.ToUnsigned8() );
    }
}

void PropertyPanel::AddImageProps( CGui::ImageCPtr _spImage )
{
    Unsigned32 u32Idx = m_spListBox->GetNumRows();

    m_spListBox->AddColumnData( "Texture:" );
    m_spListBox->AddColumnData( _spImage->GetTexture( CGui::STATE_NORMAL ).GetFileName(), 0, CGui::TYPE_EDITBOX );
    m_spListBox->AddRow();
    m_u32Rows[ PROP_TEXTURE_FILE ] = u32Idx++;

    m_spListBox->AddColumnData( "Repeat X:" );
    m_spListBox->AddColumnData( true, 0, CGui::TYPE_CHECKBOX );
    m_spListBox->AddRow();
    m_u32Rows[ PROP_REPEAT_X ] = u32Idx++;

    m_spListBox->AddColumnData( "Repeat Y:" );
    m_spListBox->AddColumnData( true, 0, CGui::TYPE_CHECKBOX );
    m_spListBox->AddRow();
    m_u32Rows[ PROP_REPEAT_Y ] = u32Idx++;

    m_spListBox->AddColumnData( "Scale X:" );
    m_spListBox->AddColumnData( true, 0, CGui::TYPE_CHECKBOX );
    m_spListBox->AddRow();
    m_u32Rows[ PROP_SCALE_X ] = u32Idx++;

    m_spListBox->AddColumnData( "Scale Y:" );
    m_spListBox->AddColumnData( true, 0, CGui::TYPE_CHECKBOX );
    m_spListBox->AddRow();
    m_u32Rows[ PROP_SCALE_Y ] = u32Idx++;
}

void PropertyPanel::SetImageProp( CGui::ImagePtr _spImage, Property eProp, const CFoundation::String &sValue )
{
    if ( eProp == PROP_TEXTURE_FILE )
    {
        _spImage->SetTexture( sValue );
    }
    else if ( eProp == PROP_REPEAT_X )
    {
        bool bValue = sValue == "0" ? false : true;
        _spImage->SetRepeatX( bValue );
    }
    else if ( eProp == PROP_REPEAT_Y )
    {
        bool bValue = sValue == "0" ? false : true;
        _spImage->SetRepeatY( bValue );
    }
    else if ( eProp == PROP_SCALE_X )
    {
        bool bValue = sValue == "0" ? false : true;
        _spImage->SetScaleX( bValue );
    }
    else if ( eProp == PROP_SCALE_Y )
    {
        bool bValue = sValue == "0" ? false : true;
        _spImage->SetScaleY( bValue );
    }
}

void PropertyPanel::AddListBoxProps( CGui::ListBoxCPtr _spListBox )
{
    Unsigned32 u32Idx = m_spListBox->GetNumRows();

    m_spListBox->AddColumnData( "Num Cols:" );
    m_spListBox->AddColumnData( _spListBox->GetNumColumns(), 0, CGui::TYPE_EDITBOX );
    m_spListBox->AddRow();
    m_u32Rows[ PROP_NUM_COLS ] = u32Idx++;

    m_spListBox->AddColumnData( "Num Max Rows:" );
    m_spListBox->AddColumnData( _spListBox->GetNumMaxRows(), 0, CGui::TYPE_EDITBOX );
    m_spListBox->AddRow();
    m_u32Rows[ PROP_NUM_MAX_ROWS ] = u32Idx++;

    m_spListBox->AddColumnData( "Inner grid horizontal:" );
    m_spListBox->AddColumnData( _spListBox->GetGridSize( CGui::ListBox::GRID_INNER_HORIZ ), 0, CGui::TYPE_EDITBOX );
    m_spListBox->AddRow();
    m_u32Rows[ PROP_INNER_GRID_SIZE_HORIZ ] = u32Idx++;

    m_spListBox->AddColumnData( "Inner grid vertical:" );
    m_spListBox->AddColumnData( _spListBox->GetGridSize( CGui::ListBox::GRID_INNER_HORIZ ), 0, CGui::TYPE_EDITBOX );
    m_spListBox->AddRow();
    m_u32Rows[ PROP_INNER_GRID_SIZE_VERT ] = u32Idx++;

    m_spListBox->AddColumnData( "Outer grid:" );
    m_spListBox->AddColumnData( _spListBox->GetGridSize( CGui::ListBox::GRID_OUTER ), 0, CGui::TYPE_EDITBOX );
    m_spListBox->AddRow();
    m_u32Rows[ PROP_OUTER_GRID_SIZE ] = u32Idx++;

    m_spListBox->AddColumnData( "Height Header:" );
    m_spListBox->AddColumnData( _spListBox->GetHeaderHeight(), 0, CGui::TYPE_EDITBOX );
    m_spListBox->AddRow();
    m_u32Rows[ PROP_HEADER_HEIGHT ] = u32Idx++;

    m_spListBox->AddColumnData( "Height Rows:" );
    m_spListBox->AddColumnData( _spListBox->GetRowHeight(), 0, CGui::TYPE_EDITBOX );
    m_spListBox->AddRow();
    m_u32Rows[ PROP_ROW_HEIGHT ] = u32Idx++;
}
void PropertyPanel::SetListBoxProp( CGui::ListBoxPtr _spListBox, Property eProp, const CFoundation::String &sValue )
{
    if ( eProp == PROP_NUM_COLS )
    {
        //_spListBox->SetNumCols( sValue.ToInteger() );
    }
    else if ( eProp == PROP_NUM_MAX_ROWS )
    {
        _spListBox->SetNumMaxRows( sValue.ToUnsigned16() );
    }
    else if ( eProp == PROP_INNER_GRID_SIZE_HORIZ )
    {
        _spListBox->SetGridSize( CGui::ListBox::GRID_INNER_HORIZ, sValue.ToUnsigned8() );
    }
    else if ( eProp == PROP_INNER_GRID_SIZE_VERT )
    {
        _spListBox->SetGridSize( CGui::ListBox::GRID_INNER_VERT, sValue.ToUnsigned8() );
    }
    else if ( eProp == PROP_OUTER_GRID_SIZE )
    {
        _spListBox->SetGridSize( CGui::ListBox::GRID_OUTER, sValue.ToUnsigned8() );
    }
    else if ( eProp == PROP_HEADER_HEIGHT )
    {
        _spListBox->SetHeaderHeight( sValue.ToUnsigned32() );
    }
    else if ( eProp == PROP_ROW_HEIGHT )
    {
        _spListBox->SetRowHeight( sValue.ToUnsigned32() );
    }
}

void PropertyPanel::OnSizeChanged()
{
    if ( m_spSelObject == NULL )
    {
        return;
    }

    if ( m_spListBox->GetCellValue( m_u32Rows[ PROP_WIDTH ], 1 ).ToUnsigned32() != m_spSelObject->GetWidth() )
    {
        m_spListBox->SetCellValue( m_u32Rows[ PROP_WIDTH ], 1, CFoundation::String( m_spSelObject->GetWidth() ) );
    }
    if ( m_spListBox->GetCellValue( m_u32Rows[ PROP_HEIGHT ], 1 ).ToUnsigned32() != m_spSelObject->GetHeight() )
    {
        m_spListBox->SetCellValue( m_u32Rows[ PROP_HEIGHT ], 1, CFoundation::String( m_spSelObject->GetHeight() ) );
    }
}
        
void PropertyPanel::OnPositionChanged()
{
    if ( m_spSelObject == NULL )
    {
        return;
    }

    if ( m_spListBox->GetCellValue( m_u32Rows[ PROP_POSX ], 1 ).ToInteger32() != m_spSelObject->GetOffset().GetX() )
    {
        m_spListBox->SetCellValue( m_u32Rows[ PROP_POSX ], 1, CFoundation::String( m_spSelObject->GetOffset().GetX() ) );
    }
    if ( m_spListBox->GetCellValue( m_u32Rows[ PROP_POSY ], 1 ).ToInteger32() != m_spSelObject->GetOffset().GetY() )
    {
        m_spListBox->SetCellValue( m_u32Rows[ PROP_POSY ], 1, CFoundation::String( m_spSelObject->GetOffset().GetY() ) );
    }
}
        
void PropertyPanel::OnListBoxValueChanged( CGui::ListBoxPtr _spListBox, Unsigned32 _u32Row, Unsigned32 _u32Col, const CFoundation::String& _strNewValue )
{
    // Search the property which was edited
    for ( Unsigned32 u32Idx = 0; u32Idx < NUM_PROPS; ++u32Idx )
    {
        if ( _u32Row == m_u32Rows[ u32Idx ] )
        {
            SetProp( Property( u32Idx ), _strNewValue );
        }
    }
}
