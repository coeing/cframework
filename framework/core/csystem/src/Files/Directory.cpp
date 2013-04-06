#include "csystem/Files/Directory.h"

#include <windows.h>
#include "csystem/Files/File.h"

#include "cfoundation/Debug/Assert.h"
#include "cfoundation/Debug/Profiling.h"

using namespace CSystem;

Directory::Directory( const CFoundation::String& _strPath )
:   m_bInitialized( false ),
    m_strPath( _strPath )
{
}

Directory::Directory( DirectoryPtr _spParentDirectory, const CFoundation::String& _strName )
:   m_bInitialized( false )
{
    m_strPath = _spParentDirectory->GetPath() + "/" + _strName;
}
        
DirectoryPtr Directory::GetParentDirectory() const
{
    return DirectoryPtr( new Directory( File::GetDirectoryName( m_strPath ) ) );
}

const std::vector< FilePtr >& Directory::GetFiles() const
{
    if ( !m_bInitialized )
    {
        Init();
    }
    return m_aspFiles;
}

const std::vector< DirectoryPtr >& Directory::GetSubDirectories() const
{
    if ( !m_bInitialized )
    {
        Init();
    }
    return m_aspSubDirectories;
}

const CFoundation::String Directory::GetName() const
{
    return File::GetFileName( m_strPath );
}
        
const CFoundation::String& Directory::GetPath() const
{
    return m_strPath;
}
        
void Directory::Init() const
{
    PROFILE_FUNC();

    WIN32_FIND_DATA fileData;
    HANDLE handle = FindFirstFile( ( m_strPath + L"/*" ).wc_str(), &fileData );
    if ( handle == INVALID_HANDLE_VALUE )
    {
        CF_WARN( __FUNCTION__ ": Got no valid file handle for search %s" );
        return;
    }

    do
    {
        CFoundation::String strName( fileData.cFileName );
        if ( fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        {
            if ( strName != "." &&
                 strName != ".." )
            {
                DirectoryPtr spSubDirectory( new Directory( m_strPath + "/" + strName ) );
                m_aspSubDirectories.push_back( spSubDirectory );
            }
        }
        else
        {
            FilePtr spFile( new File( GetSharedPtr(), strName ) );
            m_aspFiles.push_back( spFile );
        }
    } while ( FindNextFile( handle, &fileData ) );

    FindClose( handle );

    m_bInitialized = true;
}

