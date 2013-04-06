#include "csystem/Files/DirectoryManager.h"

#include <direct.h>
#include <windows.h>
#include "csystem/Files/Directory.h"

using namespace CSystem;

DirectoryManager::DirectoryManager()
{
    m_spCurrentWorkingDirectory = DirectoryPtr( new Directory( _wgetcwd( NULL, 0 ) ) );
}

DirectoryPtr DirectoryManager::GetCurrentWorkingDirectory() const
{
    return m_spCurrentWorkingDirectory;
}
