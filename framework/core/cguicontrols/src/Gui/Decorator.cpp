#include "cgui/GUI/Decorator.h"

#include <math.h>

#include "cfoundation/Debug/Makros.h"

#include "cgui/GUI/GuiMain.h"

#include "cgui/Messages/DraggabilityChangedMsg.h"
#include "cgui/Messages/ParentChangedMsg.h"
#include "cgui/Messages/ResizabilityChangedMsg.h"

using namespace CGui;

Decorator::Decorator( const CFoundation::String &sName, GuiObject *pReference )
:	CombinedCtrl( sName ),
	m_spReference( pReference ),
	m_u32ParentChange( 0 ),
	m_u32PositionChange( 0 ),
	m_u32SizeChange( 0 ),
	m_u32ResizabilityChange( 0 ),
	m_u32DraggabilityChange( 0 )
{
    // Create delegates
    m_spDelegatePostRenderReference = RenderEventDelegate3Ptr( new RenderEventDelegate3( this, &Decorator::OnPostRenderReference ) );
}

void Decorator::Clear()
{
    m_spReference.reset();

    CombinedCtrl::Clear();
}

void Decorator::SetResizable( bool bResizable )
{
	CombinedCtrl::SetResizable( bResizable );

	if( m_spReference != NULL )
	{
		++m_u32ResizabilityChange;
		m_spReference->SetResizable( false );
	}
}

void Decorator::SetDraggable( bool bDraggable )
{
	CombinedCtrl::SetDraggable( bDraggable );

	if( m_spReference != NULL )
	{
		++m_u32DraggabilityChange;
		m_spReference->SetDraggable( false );
	}
}

void Decorator::SetReference( GuiObjectPtr pReference )
{
	if( m_spReference == pReference )
	{
		return;
	}

    if ( m_spReference != NULL )
    {
        // Remove delegates
        m_spReference->RemoveDelegate( RENDER_EVENT_POST_RENDER, m_spDelegatePostRenderReference );
    }

//	ReleaseReference();

	m_spReference = pReference;

	if( m_spReference != NULL )
	{
        // Register delegates
        m_spReference->RegisterDelegate( RENDER_EVENT_POST_RENDER, m_spDelegatePostRenderReference );

        /*
		PanelPtr pParent = m_spReference->GetParent();

		if( pParent != NULL )
		{
			pParent->AddChildAfter( GetSharedPtr< GuiObject >(), m_spReference );
			pParent->RemoveChild( m_spReference );
		}*/
		//NewReferenceParent( pParent );

		//AddPieceFirst( m_spReference );

		//FitReference();

		// take settings from reference object
		//NewReferencePosition( CFoundation::Vector2Di( m_spReference->GetPosition().GetX(), m_spReference->GetPosition().GetY() ) );
		//NewReferenceSize( m_spReference->GetWidth(),
		//						m_spReference->GetHeight() );

	}
}

GuiObjectPtr Decorator::GetReference() const
{
	return m_spReference;
}

bool Decorator::HasReference() const
{
	return m_spReference != NULL;
}

//void Decorator::NewReferenceParent( GuiObject *pParent )
//{
//	GuiObject *pThisParent = GetParent();
//	if( pThisParent != NULL )
//	{
//		pThisParent->RemoveChild( *this );
//	}
//
//	if( pParent != NULL )
//	{
//		pParent->AddChildAfter( *this, *m_spReference );
//
//		++m_u32ParentChange;
//		pParent->RemoveChild( *m_spReference );
//	}
//}

void Decorator::NewReferencePosition( const CFoundation::Vector2Di &clPosition )
{
	CombinedCtrl::SetOffset( clPosition );
	FitReference();
}

void Decorator::SetReferencePosition( const CFoundation::Vector2Di &clPosition )
{
	if( m_spReference != NULL &&
		m_spReference->GetRelativePosition() != clPosition )
	{
		++m_u32PositionChange;
		m_spReference->SetOffset( clPosition );
	}
}

void Decorator::NewReferenceSize( Unsigned32 u32Width, Unsigned32 u32Height )
{
	UNREFERENCED_PARAMETER( u32Width );
	UNREFERENCED_PARAMETER( u32Height );
}

void Decorator::SetReferenceSize( Unsigned32 u32Width, Unsigned32 u32Height )
{
	++m_u32SizeChange;
    m_spReference->SetCustomSize( CFoundation::Vector2Du( u32Width, u32Height ) );
}

void Decorator::FitReference()
{
	if( m_spReference != NULL )
	{
		SetReferencePosition( CFoundation::Vector2Di( GetLeftMargin(), GetTopMargin() ) );

		Integer32 i32Width = GetWidth() - GetLeftMargin() - GetRightMargin();
		Integer32 i32Height = GetHeight() - GetTopMargin() - GetBottomMargin();
		SetReferenceSize( std::max( i32Width, 0 ), std::max( i32Height, 0 ) );
	}
}

void Decorator::ReleaseReference()
{
	if( m_spReference != NULL )
	{
		PanelPtr pParent = GetParent();

		if( pParent != NULL )
		{
			pParent->AddChildAfter( m_spReference, GetSharedPtr< GuiObject >() );
			pParent->RemoveChild( GetSharedPtr< GuiObject >() );
		}

		// take settings from reference object
		m_spReference->SetOffset( GetRelativePosition() );

		m_spReference->SetDraggable( IsDraggable() );

		m_spReference->SetResizableAnchor( GetResizableAnchor() );

		m_spReference.reset();
	}
}

Unsigned32 Decorator::GetLeftMargin() const
{
	return 0;
}

Unsigned32 Decorator::GetRightMargin() const
{
	return 0;
}

Unsigned32 Decorator::GetTopMargin() const
{
	return 0;
}

Unsigned32 Decorator::GetBottomMargin() const
{
	return 0;
}
        
void Decorator::OnPostRenderReference( GuiObjectPtr _spObject, const RenderParams& _Params, std::vector< RenderElementPtr >& _aspRenderElements )
{
}

/*
void DecoratorCreator::InitFromXML( GuiObjectPtr _spObject, const CFoundation::XMLElement &clXMLElement ) const
{
	CombinedCtrlCreator::InitFromXML( _spObject, clXMLElement );
	Decorator &clDecorator = static_cast<Decorator&>( clObject );

	CFoundation::XMLNodePtr pReferenceNode = clXMLElement.GetFirstChild();
	if ( pReferenceNode != NULL &&
         pReferenceNode->IsElement() )
	{
        GuiObject &clReference = clGuiMain.CreateFromXML( *static_pointer_cast< CFoundation::XMLElement >( pReferenceNode ) );

		clDecorator.SetReference( clReference.GetSharedPtr() );
	}

	return clDecorator;
}*/

//CFoundation::XMLElement& DecoratorCreator::SaveToXML( const GuiObject &clObject, CFoundation::XMLDocument &clDoc, CFoundation::XMLElement &clXMLElement ) const
//{
//	CombinedCtrlCreator::SaveToXML( clObject, clDoc, clXMLElement );
//
//	const Decorator &clDecorator = dynamic_cast<const Decorator&>( clObject );
//
//	GuiObjectCPtr pReference = clDecorator.GetReference();
//	if( pReference != NULL )
//	{
//		CFoundation::XMLElementPtr spReferenceNode = clDoc.CreateElement( pReference->GetTypeId() );
//		clGuiMain.SaveToXML( pReference, clDoc, *spReferenceNode );
//
//		clXMLElement.InsertChildFirst( *spReferenceNode );
//	}
//
//	return clXMLElement;
//}
