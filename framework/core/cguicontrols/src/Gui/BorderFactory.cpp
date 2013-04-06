#include "cgui/Gui/BorderFactory.h"

#include "cfoundation/Debug/Makros.h"

#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/SimpleBorder.h"

using namespace CGui;

BorderFactory& gBorderFactory = CGui::BorderFactory::GetInstance();

/*
Border& BorderFactory::CreateBorder( const BorderType& eType ) const
{
	switch( eType )
	{
		case BORDER_SIMPLE:
		{
			CGui::SimpleBorder &clBorder = clGuiMain.CreateDefault<CGui::SimpleBorder>( "BorderSimple" );
			return clBorder;
		}
		break;
		case BORDER_WINDOWS:
		default:
		{
			CF_WARN( L"No valid border type. Returning simple border." );
			CGui::SimpleBorder& clBorder = clGuiMain.CreateDefault<CGui::SimpleBorder>( "BorderSimple" );
			return clBorder;
		}
		break;
	}
}*/
