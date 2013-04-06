#include "cgui/GUI/Border.h"

#include "cgui/GUI/GuiObjectView.h"

using namespace CGui;

Border::Border( const CFoundation::String &sName )
:	Decorator( sName ),
	m_u8Thickness( 0 )
{
}

void Border::operator=( const Border &rhs )
{
    Decorator::operator=( rhs );
    m_u8Thickness = rhs.m_u8Thickness;
}

const CFoundation::String Border::GetTypeID()
{
	return "border";
}

const CFoundation::String Border::GetTypeId() const
{
	return Border::GetTypeID();
}

void Border::CreatePieces()
{
}

/*
void Border::SetCustomSize( Unsigned32 u32Width, Unsigned32 u32Height )
{
	if( u32Width != GetWidth() ||
		u32Height != GetHeight() )
	{
		Decorator::SetCustomSize( u32Width, u32Height );
		FitReferenceSize();
	}
}

void Border::SetCustomSize( const CFoundation::Vector2Du& clSize )
{
	SetCustomSize( clSize.GetX(), clSize.GetY() );
}*/

/*
void Border::SetWidth( Unsigned32 u32Width )
{
	if( u32Width != GetWidth() )
	{
		Decorator::SetWidth( u32Width );
		FitReferenceWidth();
	}
}

void Border::SetHeight( Unsigned32 u32Height )
{
	if( u32Height != GetHeight() )
	{
		Decorator::SetHeight( u32Height );
		FitReferenceHeight();
	}
}
*/

void Border::SetReference( GuiObjectPtr _spReference )
{
	Decorator::SetReference( _spReference );

	if ( _spReference != NULL )
	{
		// fit size
		CFoundation::Vector2Du clSize( 2 * GetThickness(), 2 * GetThickness() );
		SetSize( clSize + _spReference->GetSize() );
	}
}

/*Unsigned32 Border::GetWidth() const
{
	Unsigned32 u32Width = 2*GetSize();
	if( HasReference() )
	{
		u32Width += GetReference()->GetWidth();
	}
	return u32Width;
}

Unsigned32 Border::GetHeight() const
{
	Unsigned32 u32Height = 2 * GetSize();
	if( HasReference() )
	{
		u32Height += GetReference()->GetHeight();
	}
	return u32Height;
}

const CFoundation::Vector2Du Border::GetSize() const
{
	CFoundation::Vector2Du clSize( 2 * GetSize(), 2 * GetSize() );
	if( HasReference() )
	{
		clSize += GetReference()->GetSize();
	}
	return clSize;
}*/

//GuiObject* Border::GetObject()
//{
//	ret;
//}
//
//const GuiObject* Border::GetObject() const
//{
//	return m_pObject;
//}

void Border::SetThickness( Unsigned8 u8Thickness )
{
	m_u8Thickness = u8Thickness;

	FitReference();
}

Unsigned8 Border::GetThickness() const
{
	return m_u8Thickness;
}

void Border::InitPieces()
{
}

void Border::UpdatePieces()
{
}

/*void Border::NewReferenceSize( Unsigned32 u32Width, Unsigned32 u32Height )
{
	SetSize( u32Width + GetLeftMargin() + GetRightMargin(),
				   u32Height + GetTopMargin() + GetBottomMargin() );
}*/

Unsigned32 Border::GetBorderWidth() const
{
	return m_u8Thickness;
}

Unsigned32 Border::GetBorderHeight() const
{
	return m_u8Thickness;
}

Unsigned32 Border::GetLeftMargin() const
{
	return GetBorderWidth();
}

Unsigned32 Border::GetRightMargin() const
{
	return GetBorderWidth();
}

Unsigned32 Border::GetTopMargin() const
{
	return GetBorderHeight();
}

Unsigned32 Border::GetBottomMargin() const
{
	return GetBorderHeight();
}

/*
void Border::FitReferenceSize()
{
	if( HasReference() )
	{
		Integer32 i32Width = GetWidth() - 2 * GetThickness();
		Integer32 i32Height = GetHeight() - 2 * GetThickness();
		GetReference()->SetCustomSize( std::max( i32Width, 0 ), std::max( i32Height, 0 ) );
	}
}

void Border::FitReferenceWidth()
{
	if( HasReference() )
	{
		Integer32 i32Width = GetWidth() - 2 * GetThickness();
		GetReference()->SetWidth( std::max( i32Width, 0 ) );
	}
}

void Border::FitReferenceHeight()
{
	if( HasReference() )
	{
		Integer32 i32Height = GetHeight() - 2 * GetThickness();
		GetReference()->SetHeight( std::max( i32Height, 0 ) );
	}
}
*/

const CFoundation::String BorderCreator::GetTypeId() const
{
	return Border::GetTypeID();
}

GuiObjectPtr BorderCreator::CreateObject( const CFoundation::String &sName ) const
{
	GuiObjectPtr spBorder( new Border( sName ) );
	return spBorder;
}

//CFoundation::XMLElement& BorderCreator::SaveToXML( CFoundation::XMLElement &clXMLElement, const GuiNode &clNode ) const
//{
//	CombinedCtrlCreator::SaveToXML( clXMLElement, clNode );
//	return clXMLElement;
//}
