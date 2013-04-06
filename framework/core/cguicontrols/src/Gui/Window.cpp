#include "cgui/GUI/Window.h"

#include "cfoundation/Debug/Makros.h"

using namespace CGui;

Window::Window( const CFoundation::String& _sName )
:	Panel( _sName )
{
}
        
Window::~Window()
{
}

const CFoundation::String Window::GetTypeID()
{
	return CFoundation::String( "window" );
}

const CFoundation::String Window::GetTypeId() const
{
	return Window::GetTypeID();
}

void Window::Animate()
{
}

GuiObjectPtr Window::GetTabObject( Unsigned16 u16TabIndex )
{
	for( Unsigned32 u16Child = 0; u16Child < GetNumChildren(); ++u16Child )
	{
		GuiObjectPtr pObject = GetChild( u16Child );
		if( pObject->GetTabIndex() == u16TabIndex )
		{
			return pObject;
		}
	}
	return GuiObjectPtr();
}

GuiObjectPtr Window::GetTabObjectBefore( Unsigned16 u16TabIndex )
{
	Unsigned16 u16MinDiff = MAX_U16;
	GuiObjectPtr pMinObject;
	for( Unsigned16 u16Child = 0; u16Child < GetNumChildren(); ++u16Child )
	{
		GuiObjectPtr pObject = GetChild( u16Child );
		Unsigned16 u16Diff = MAX_U16;
		if( pObject->GetTabIndex() < u16TabIndex &&
			pObject->GetTabIndex() != 0 )
		{
			u16Diff = u16TabIndex - pObject->GetTabIndex();
			if( u16Diff < u16MinDiff )
			{
				u16MinDiff = u16Diff;
				pMinObject = pObject;
			}
		}
	}
	return pMinObject;
}

GuiObjectPtr Window::GetTabObjectAfter( Unsigned16 u16TabIndex )
{
	Unsigned16 u16MinDiff = MAX_U16;
	GuiObjectPtr pMinObject;
	for( Unsigned16 u16Child = 0; u16Child < GetNumChildren(); ++u16Child )
	{
		GuiObjectPtr pObject = GetChild( u16Child );
		Unsigned16 u16Diff = MAX_U16;
		if( pObject->GetTabIndex() > u16TabIndex &&
			pObject->GetTabIndex() != 0 )
		{
			u16Diff = pObject->GetTabIndex() - u16TabIndex;
			if( u16Diff < u16MinDiff )
			{
				u16MinDiff = u16Diff;
				pMinObject = pObject;
			}
		}
	}
	return pMinObject;
}

const CFoundation::String WindowCreator::GetTypeId() const
{
	return Window::GetTypeID();
}

GuiObjectPtr WindowCreator::CreateObject( const CFoundation::String &sName ) const
{
	WindowPtr spObject( new Window( sName ) );
	return spObject;
}

void WindowCreator::InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const
{
    PanelCreator::InitFromXML( _spObject, _spXMLElement );
}
		
void WindowCreator::InitFromTemplate( GuiObjectPtr _spObject, GuiObjectCPtr _spTemplate ) const
{
    PanelCreator::InitFromTemplate( _spObject, _spTemplate );
}

bool WindowCreator::SaveToXML( GuiObjectCPtr _spObject, CFoundation::XMLDocumentPtr _spDoc, CFoundation::XMLElementPtr _spXMLElement ) const
{
    return PanelCreator::SaveToXML( _spObject, _spDoc, _spXMLElement );
}
