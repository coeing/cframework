#include <vector>
#include "csystem/Files/Directory.h"
#include "csystem/Files/DirectoryManager.h"
#include "csystem/Files/File.h"

void printDirectory( CSystem::DirectoryPtr _spDirectory, Unsigned32 u32Indent = 0 )
{
    // Print directory name
    std::cout << _spDirectory->GetPath() << std::endl;

    // Print files
    const std::vector< CSystem::FilePtr >& aspFiles = _spDirectory->GetFiles();
    Unsigned32 u32NumFiles = aspFiles.size();
    for ( Unsigned32 u32Idx = 0; u32Idx != u32NumFiles; ++u32Idx )
    {
        CSystem::FilePtr spFile = aspFiles[ u32Idx ];
        std::cout << spFile->GetName() << std::endl;
    }

    // Print sub directories
    const std::vector< CSystem::DirectoryPtr >& aspSubDirectories = _spDirectory->GetSubDirectories();
    Unsigned32 u32NumSubDirectories = aspSubDirectories.size();
    for ( Unsigned32 u32Idx = 0; u32Idx != u32NumSubDirectories; ++u32Idx )
    {
        CSystem::DirectoryPtr spSubDirectory = aspSubDirectories[ u32Idx ];
        printDirectory( spSubDirectory, u32Indent + 2 );
    }
}

int main()
{
    CSystem::DirectoryManager& clDirectoryManager = CSystem::DirectoryManager::GetInstance();

    // Print out files of current directory
    CSystem::DirectoryPtr spDirectory = clDirectoryManager.GetCurrentWorkingDirectory();
    printDirectory( spDirectory );

    getchar();

    return 0;
}
