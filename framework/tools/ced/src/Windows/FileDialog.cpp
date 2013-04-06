#include "ced/Windows/FileDialog.h"

#include "csystem/Files/DirectoryManager.h"
#include "csystem/Files/File.h"

#include "cgui/Gui/Button.h"
#include "cgui/Gui/EditBox.h"
#include "cgui/Gui/ListBox.h"

using namespace Ced;

FileDialog::FileDialog()
:   m_bAllowNewFile( false )
{
    // Create delegates
    m_spDelegateClickedOk = CGui::ObjectInputEventDelegate0Ptr( new CGui::ObjectInputEventDelegate0( this, &FileDialog::OnOk ) );
    m_spDelegateClickedCancel = CGui::ObjectInputEventDelegate0Ptr( new CGui::ObjectInputEventDelegate0( this, &FileDialog::OnCancel ) );
    m_spDelegateSelectedRow = CGui::ListBoxEventI32Delegate2Ptr( new CGui::ListBoxEventI32Delegate2( this, &FileDialog::OnSelectedRow ) );
    m_spDelegateDoubleClickedCell = CGui::ListBoxEventDelegate0Ptr( new CGui::ListBoxEventDelegate0( this, &FileDialog::OnDoubleClickedCell ) );
}

const CFoundation::String FileDialog::GetXMLFileName()
{
	return "data/screens/FileDialog.xml";
}

void FileDialog::InitWindow()
{
    // Get file list box
    m_spLbFiles = GetListBox( "LbFiles" );
    SAFE_CALL( m_spLbFiles )->RegisterDelegate( CGui::EVENT_LISTBOX_SELECTED_ROW, m_spDelegateSelectedRow );
    SAFE_CALL( m_spLbFiles )->RegisterDelegate( CGui::EVENT_LISTBOX_DOUBLE_CLICKED_CELL, m_spDelegateDoubleClickedCell );

    // Get buttons
    m_spBtOk = GetButton( "BtOk" );
    SAFE_CALL( m_spBtOk )->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedOk );
    m_spBtCancel = GetButton( "BtCancel" );
    SAFE_CALL( m_spBtCancel )->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedCancel );
    //m_spBtUp = GetButton( "BtUp" );

    m_spEbFileName = GetEditBox( "EbFileName" );

    // Set current directory
    SetCurrentDirectory( CSystem::DirectoryManager::GetInstance().GetCurrentWorkingDirectory() );

    CCtrl::WindowController::InitWindow();
}
        
void FileDialog::SetCurrentDirectory( CSystem::DirectoryPtr _spDirectory )
{
    if ( m_spCurDirectory == _spDirectory )
    {
        return;
    }

    m_spCurDirectory = _spDirectory;

    // Update files
    UpdateFileListing();
}
        
CSystem::DirectoryPtr FileDialog::GetCurrentDirectory() const
{
    return m_spCurDirectory;
}

CSystem::DirectoryPtr FileDialog::GetSelectedDirectory() const
{
    // Get selected index of list box
    Integer32 i32SelectedIdx = m_spLbFiles->GetSelectedRow();
    if ( i32SelectedIdx < 0 ||
         m_spCurDirectory == NULL )
    {
        return CSystem::DirectoryPtr();
    }

    Unsigned32 u32SelectedIdx = i32SelectedIdx;

    // Check if parent directory selected
    if ( u32SelectedIdx == 0 )
    {
        return m_spCurDirectory->GetParentDirectory();
    }
    --u32SelectedIdx;

    // Check if sub directory selected
    const std::vector< CSystem::DirectoryPtr >& aspDirectories = m_spCurDirectory->GetSubDirectories();
    if ( u32SelectedIdx < aspDirectories.size() )
    {
        return aspDirectories[ u32SelectedIdx ];
    }

    return CSystem::DirectoryPtr();
}

CSystem::FilePtr FileDialog::GetSelectedFile() const
{
    // Get selected index of list box
    Integer32 i32SelectedIdx = m_spLbFiles->GetSelectedRow();
    if ( i32SelectedIdx < 0 ||
         m_spCurDirectory == NULL )
    {
        return CSystem::FilePtr();
    }

    Unsigned32 u32SelectedIdx = i32SelectedIdx;

    // Check if parent directory selected
    if ( u32SelectedIdx == 0 )
    {
        return CSystem::FilePtr();
    }
    --u32SelectedIdx;

    // Check if sub directory selected
    const std::vector< CSystem::DirectoryPtr >& aspDirectories = m_spCurDirectory->GetSubDirectories();
    if ( u32SelectedIdx < aspDirectories.size() )
    {
        return CSystem::FilePtr();
    }
    u32SelectedIdx -= aspDirectories.size();

    const std::vector< CSystem::FilePtr >& aspFiles = m_spCurDirectory->GetFiles();
    if ( u32SelectedIdx < aspFiles.size() )
    {
        return aspFiles[ u32SelectedIdx ];
    }

    return CSystem::FilePtr();
}

void FileDialog::UpdateFileListing()
{
    if ( m_spLbFiles == NULL )
    {
        return;
    }

    m_spLbFiles->ClearContent();

    // Add parent directory
    m_spLbFiles->AddColumnData( ".." );
    m_spLbFiles->AddRow();

    // Add sub directories
    const std::vector< CSystem::DirectoryPtr >& aspDirectories = m_spCurDirectory->GetSubDirectories();
    Unsigned32 u32NumDirectories = aspDirectories.size();
    for ( Unsigned32 u32Idx = 0; u32Idx != u32NumDirectories; ++u32Idx )
    {
        CSystem::DirectoryPtr spDirectory = aspDirectories[ u32Idx ];
        m_spLbFiles->AddColumnData( spDirectory->GetName() );
        m_spLbFiles->AddRow();
    }

    // Add files
    const std::vector< CSystem::FilePtr >& aspFiles = m_spCurDirectory->GetFiles();
    Unsigned32 u32NumFiles = aspFiles.size();
    for ( Unsigned32 u32Idx = 0; u32Idx != u32NumFiles; ++u32Idx )
    {
        CSystem::FilePtr spFile = aspFiles[ u32Idx ];
        m_spLbFiles->AddColumnData( spFile->GetName() );
        m_spLbFiles->AddRow();
    }
}
       
void FileDialog::OnSelectedRow( CGui::ListBoxPtr _spListBox, Integer32 _i32SelectedRow )
{
    if ( _i32SelectedRow < 0 )
    {
        return;
    }

    // Check if directory selected
    CSystem::DirectoryPtr spDirectory = GetSelectedDirectory();
    if ( spDirectory != NULL )
    {
        return;
    }

    // Get selected file
    const CFoundation::String& strFileName = m_spLbFiles->GetCellValue( _i32SelectedRow, 0 );

    // Set file name in edit box
    m_spEbFileName->SetText( strFileName );
}

void FileDialog::OnDoubleClickedCell()
{
    OnOk();
}

void FileDialog::OnOk()
{
    // Check if directory selected
    CSystem::DirectoryPtr spDirectory = GetSelectedDirectory();
    if ( spDirectory != NULL )
    {
        SetCurrentDirectory( spDirectory );
    }
    else
    {
        Close( WINDOW_RETURN_VALUE_OK );
    }
}

void FileDialog::OnCancel()
{
    Close( WINDOW_RETURN_VALUE_CANCEL );
}
