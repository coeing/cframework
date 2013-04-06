#include "ced/Document.h"

#include "cfoundation/XML/XMLParser.h"

#include "cgui/Globals.h"
#include "cgui/DragDrop/DragDropSystem.h"
#include "cgui/DragDrop/FreeDragController.h"
#include "cgui/DragDrop/FreeDropController.h"

using namespace Ced;

Document::Document( CGui::WindowPtr _spWindow )
:	m_spWindow( _spWindow )
{
}
       
bool Document::Open( CSystem::FilePtr _spFile )
{
    if ( _spFile == NULL )
    {
        return false;
    }
    
    m_spFile = _spFile;

    // Check if xml exists
    CFoundation::XMLParser& clParser = CFoundation::XMLParser::GetInstance();
    CFoundation::XMLDocumentPtr spDocument = clParser.ParseFile( m_spFile->GetFullName() );
    if ( spDocument == NULL )
    {
        return false;
    }

	// Remove old stuff
	m_spWindow->Clear();

	// Init with new XML
    CGui::GuiMain::GetInstance().InitFromXML( m_spWindow, spDocument->GetRootElement() );

	return true;
}

bool Document::Open( const CFoundation::String& _strFileName )
{
    return Open( CSystem::FilePtr( new CSystem::File( _strFileName ) ) );
}

bool Document::Save()
{
    return SaveAs( m_spFile );
}

bool Document::SaveAs( CSystem::FilePtr _spFile )
{
    if ( _spFile == NULL )
    {
        return false;
    }
    CGui::GuiMain::GetInstance().SaveToXML( m_spWindow, _spFile->GetFullName() );
    m_spFile = _spFile;
    return true;
}
        
CSystem::FilePtr Document::GetFile() const
{
    return m_spFile;
}

