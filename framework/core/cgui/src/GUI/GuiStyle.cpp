#include "cgui/GUI/GuiStyle.h"

#include "cfoundation/XML/XMLParser.h"

#include "csystem/Files/Directory.h"
#include "csystem/Files/File.h"

#include "cgui/GUI/CtrlStyle.h"

using namespace CGui;

GuiStyle::GuiStyle( const CFoundation::String& _strXMLFileName )
{
    m_spFile = CSystem::FilePtr( new CSystem::File( _strXMLFileName ) );
    m_spBasePath = m_spFile->GetDirectory();
	ReadXML();
}

GuiStyle::~GuiStyle()
{
    m_spFile.reset();
    m_spBasePath.reset();
    m_vStyles.clear();
    m_vDefaultStyles.clear();
}

CtrlStyleCPtr GuiStyle::GetDefaultStyle( const CFoundation::String& _strType ) const
{
	StyleMap::const_iterator it = m_vDefaultStyles.find( _strType );
	if ( it != m_vDefaultStyles.end() )
	{
		return it->second;
	}
	else
	{
		return CtrlStyleCPtr();
	}
}

CtrlStyleCPtr GuiStyle::GetStyle( const CFoundation::String& sName ) const
{
	StyleMap::const_iterator it = m_vStyles.find( sName );
	if( it != m_vStyles.end() )
	{
		return it->second;
	}
	else
	{
		return CtrlStyleCPtr();
	}
}

void GuiStyle::ReadXML()
{
	CFoundation::XMLParser &clParser = CFoundation::XMLParser::GetInstance();

    CFoundation::XMLDocumentPtr pDocument = clParser.ParseFile( m_spFile->GetFullName() );

    if ( pDocument == NULL )
	{
        return;
    }
	
    CFoundation::XMLNodePtr pRoot = pDocument->GetRootElement();
    if ( pRoot == NULL )
    {
        return;
    }

    CFoundation::XMLNodePtr spChild = pRoot->GetFirstChild();
	while ( spChild != NULL )
	{
		if ( spChild->IsElement() )
		{
            CFoundation::XMLElementPtr spElement = static_pointer_cast< CFoundation::XMLElement >( spChild );

			CFoundation::String strType = spElement->GetAttribute( "type" );
			CFoundation::String sName = spElement->GetAttribute( "name" );
            CFoundation::String strXMLFile = m_spBasePath->GetPath() + L"/" + spElement->GetAttribute( "xml_file" );
			CtrlStylePtr spStyle( new CtrlStyle( strXMLFile ) );
			m_vStyles[ sName ] = spStyle;

			CFoundation::String sDefault = spElement->GetAttribute( "default" );
			if ( sDefault == "true" )
			{
				m_vDefaultStyles[ strType ] = spStyle;		
			}
		}
		
        spChild = spChild->GetNextSibling();
	}
}
