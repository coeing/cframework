#include "AdventCalendarWindow.h"

#include "Door.h"

#include "cfoundation/XML/XMLParser.h"

using namespace CAdventCalendar;

AdventCalendarWindow::AdventCalendarWindow()
{
}

const CFoundation::String AdventCalendarWindow::GetXMLFileName()
{
	return "./data/AdventCalendarWindow.xml";
};

void AdventCalendarWindow::InitWindow()
{
    m_spPnlCalendar = GetPanel( "PnlCalendar" );

    m_pTbDoorTemplate = GetTextBox( "TbDoorTemplate" );
    m_pImgDoorTemplate = GetImage( "ImgDoorTemplate" );

    // Add doors
    Unsigned32 u32Width = m_spPnlCalendar->GetWidth();
    Unsigned32 u32Height = m_spPnlCalendar->GetHeight();

    Unsigned32 u32DoorWidth = u32Width / 13;
    Unsigned32 u32DoorHeight = u32Height / 9;

    // Create day offsets
    std::vector< CFoundation::Vector2Di > aDayOffsets;

    for ( Unsigned32 u32X = 0; u32X < 6; ++u32X )
    {
        Integer32 i32OffsetX = ( u32X * 2 + 1 ) * u32DoorWidth;
        for ( Unsigned32 u32Y = 0; u32Y < 4; ++u32Y )
        {
            Integer32 i32OffsetY = ( u32Y * 2 + 1 ) * u32DoorHeight;
            aDayOffsets.push_back( CFoundation::Vector2Di( i32OffsetX, i32OffsetY ) );
        }
    }
    std::random_shuffle( aDayOffsets.begin(), aDayOffsets.end() );

    CFoundation::XMLParser& clParser = CFoundation::XMLParser::GetInstance();
    CFoundation::XMLDocumentPtr spDocument = clParser.ParseFile( "data/Config.xml" );
    if ( spDocument == NULL )
    {
        return;
    }
    
    CFoundation::XMLElementPtr spRoot = spDocument->GetRootElement();

    CFoundation::XMLNodePtr spDoorNode = spRoot->GetFirstChild();
    while ( spDoorNode != NULL )
    {
        if ( spDoorNode->IsElement() )
        {
            CFoundation::XMLElementPtr spDoorElement = static_pointer_cast< CFoundation::XMLElement >( spDoorNode );

            Unsigned8 u8Day = spDoorElement->GetAttribute( "day" ).ToUnsigned8();

            const CFoundation::Vector2Di vDayOffset = aDayOffsets[ u8Day - 1 ];
            
            CGui::ImagePtr spImgDoor = CGui::GuiMain::GetInstance().CreateFromTemplate( "ImgDoor", m_pImgDoorTemplate );
            spImgDoor->SetCustomSize( u32DoorWidth, u32DoorHeight );
            spImgDoor->SetOffset( vDayOffset );

            m_spPnlCalendar->AddChildLast( spImgDoor );

            CGui::TextBoxPtr spTbDoor = CGui::GuiMain::GetInstance().CreateFromTemplate( "TbDoor", m_pTbDoorTemplate );
            spTbDoor->SetCustomSize( u32DoorWidth, u32DoorHeight );
            spTbDoor->SetOffset( vDayOffset );
            spTbDoor->SetText( CFoundation::String( u8Day ) );
            
            m_spPnlCalendar->AddChildLast( spTbDoor );

            Door* pDoor = new Door( spTbDoor, spImgDoor );
            pDoor->InitFromXML( spDoorElement );
            m_apDoors[ u8Day ] = pDoor;
        }

        spDoorNode = spDoorNode->GetNextSibling();
    }

    m_pTbDoorTemplate->SetVisible( false );
    m_pImgDoorTemplate->SetVisible( false );
}
        
Door* AdventCalendarWindow::GetDoor( Unsigned8 _u8Day )
{
    DayDoorMap::iterator it = m_apDoors.find( _u8Day );
    if ( it != m_apDoors.end() )
    {
        return it->second;
    }
    else
    {
        return NULL;
    }
}
