#include "cgui/GUI/Layer.h"

#include "cgui/GUI/GuiMain.h"
#include "cgui/GUI/Panel.h"

using namespace CGui;

Layer::Layer()
{
    m_spRoot = GuiMain::GetInstance().CreateDefault< Panel >( "LayerRoot" );
    m_spRoot->SetBgVisible( false );
}

Layer::~Layer()
{
}
        
void Layer::Deinit()
{
    // Delete layer
    GuiMain::GetInstance().Delete( m_spRoot );
}

void Layer::AddObjectFirst( GuiObjectPtr _spObject )
{
    m_spRoot->AddChildFirst( _spObject );
}

void Layer::AddObjectLast( GuiObjectPtr _spObject )
{
    m_spRoot->AddChildLast( _spObject );
}

void Layer::AddObjectAfter( GuiObjectPtr _spObject, GuiObjectCPtr _spObjectBefore )
{
    m_spRoot->AddChildAfter( _spObject, _spObjectBefore );
}

void Layer::RemoveObject( GuiObjectPtr _spObject )
{
    if ( m_spRoot->IsChild( _spObject ) )
    {
        m_spRoot->RemoveChild( _spObject );
    }
}
       
void Layer::SetSize( const CFoundation::Vector2Du& _vSize )
{
	m_spRoot->SetCustomSize( _vSize );
}
        
void Layer::SetBgColor( const CFoundation::Color& _clColor )
{
	m_spRoot->SetBgColor( STATE_NORMAL, _clColor );
}

void Layer::Update( Float32 _f32TimeDifference )
{
	m_spRoot->Update( _f32TimeDifference );
}

#ifdef MULTIPLE_RENDER_ELEMENTS
void Layer::Render( const RenderParams& _Params, std::vector< RenderElementPtr >& _aspRenderElements )
#else
void Layer::Render( const RenderParams& _Params, std::vector< RenderElement >& _aspRenderElements )
#endif
{
	m_spRoot->Render( _Params, _aspRenderElements );

    // Dispatch post render event
    // TODO CO m_spRoot->DispatchEvent( RENDER_EVENT_POST_RENDER, m_spRoot, _Params, _aspRenderElements );
}

GuiObjectPtr Layer::GetObjectAtPoint( const CFoundation::Vector2Di& _clPoint, CFoundation::Vector2Di& _clRelativePoint )
{
	_clRelativePoint = _clPoint;
	GuiObjectPtr pObject = m_spRoot->FindAt( _clPoint - m_spRoot->GetRelativePosition(), _clRelativePoint );
    // Don't return the layer itself
    if ( pObject == m_spRoot )
    {
        return GuiObjectPtr();
    }
    else
    {
        return pObject;
    }
}
