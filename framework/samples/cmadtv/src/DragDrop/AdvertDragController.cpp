#include "cmadtv/DragDrop/AdvertDragController.h"

#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/Layer.h"
#include "cgui/Gui/Panel.h"
#include "cgui/Gui/TextBox.h"
#include "cgui/Input/Input.h"

#include "cmadtv/Data/Advert.h"
#include "cmadtv/Windows/AdvertBlock.h"

using namespace CMadTV;

AdvertDragController::AdvertDragController( AdvertDragType _eDragType, AdvertPtr _spAdvert, const CFoundation::Date& _Date, Hour _eHour )
:   m_eDragType( _eDragType ),
    m_spDragData( new AdvertDragData() )
{
    if ( m_eDragType == ADVERT_DRAG_TYPE_BLOCK )
    {
        // Create Advert block
        m_spAdvertBlock = AdvertBlock::Create( _spAdvert );
    }

    m_spDragData->m_spAdvert = _spAdvert;
    m_spDragData->m_Date = _Date;
    m_spDragData->m_eHour = _eHour;
}
        
AdvertDragController::AdvertDragController( AdvertBlockPtr _spBlock, const CFoundation::Date& _Date, Hour _eHour )
:   m_eDragType( ADVERT_DRAG_TYPE_BLOCK ),
    m_spDragData( new AdvertDragData() ),
    m_spAdvertBlock( _spBlock )
{
    m_spDragData->m_spAdvert = m_spAdvertBlock->GetAdvert();
    m_spDragData->m_Date = _Date;
    m_spDragData->m_eHour = _eHour;
}

AdvertDragController::~AdvertDragController()
{
}
        
void AdvertDragController::Start( CGui::GuiObjectPtr _spObject, const CFoundation::Vector2Di& _vRelativeMousePosition )
{
    // Clear advert block sub text
    if ( m_spAdvertBlock != NULL )
    {
        m_spAdvertBlock->SetSubText( "" );
    }

    ProxyDragController::Start( _spObject, _vRelativeMousePosition );
}
        
CGui::DragDataPtr AdvertDragController::GetDragData() const
{
    return m_spDragData;
}
        
void AdvertDragController::SetAdvert( AdvertPtr _spAdvert )
{
    m_spDragData->m_spAdvert = _spAdvert;

    if ( m_spAdvertBlock != NULL )
    {
        m_spAdvertBlock->SetAdvert( _spAdvert );
    }
}
        
CGui::GuiObjectPtr AdvertDragController::CreateProxyObject( CGui::GuiObjectPtr _spObject ) const
{
    if ( m_eDragType == ADVERT_DRAG_TYPE_BLOCK )
    {
        return m_spAdvertBlock->GetPanel();
    }
    else
    {
        return CGui::GuiMain::GetInstance().CreateFromXML( "data/Windows/AdvertDragObject.xml" );
    }
}
