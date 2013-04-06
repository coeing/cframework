#include "csystem/Files/File.h"

#include "csystem/Files/Directory.h"

using namespace CSystem;
      
File::File( const CFoundation::String& _strPath )
{
    m_spDirectory = DirectoryPtr( new Directory( GetDirectoryName( _strPath ) ) );
    m_strName = GetFileName( _strPath );
}

File::File( DirectoryCPtr _spDirectory, const CFoundation::String& _strName )
:   m_spDirectory( _spDirectory ),
    m_strName( _strName )
{
}
        
DirectoryCPtr File::GetDirectory() const
{
    return m_spDirectory;
}

const CFoundation::String& File::GetName() const
{
    return m_strName;
}
        
const CFoundation::String File::GetFullName() const
{
    if ( m_spDirectory != NULL )
    {
        const CFoundation::String& strPath = m_spDirectory->GetPath();
        if ( strPath.GetLength() > 0 )
        {
            return strPath + "/" + m_strName;
        }
        else
        {
            return m_strName;
        }
    }
    else
    {
        return m_strName;
    }
}
        
const CFoundation::String File::GetExtension() const
{
    return GetExtension( m_strName );
}
        
const CFoundation::String File::GetDirectoryName( const CFoundation::String& _strFullName )
{
	Unsigned32 u32Length = _strFullName.GetLength();

	if( u32Length <= 1 )
	{
		return "";
	}

	Integer32 i32Char = 0;
	for( i32Char = u32Length - 1; i32Char >= 0; --i32Char )
	{
		if ( _strFullName[ i32Char ] == '/' ||
			 _strFullName[ i32Char ] == '\\' )
		{
			break;
		}
	}

	if( i32Char <= 0 )
	{
		return "";
	}
	else
	{
		return _strFullName.Head( i32Char );
	}
}
        
const CFoundation::String File::GetFileName( const CFoundation::String& _strFullName, bool _bExtension )
{
	Unsigned32 u32Length = _strFullName.GetLength();

	if( u32Length <= 1 )
	{
		return "";
	}

	Integer32 i32Char = 0;
	for( i32Char = u32Length - 1; i32Char >= 0; --i32Char )
	{
		if ( _strFullName[ i32Char ] == '/' ||
			 _strFullName[ i32Char ] == '\\' )
		{
			break;
		}
	}
    i32Char++;

    // Search extension
    Integer32 i32End = u32Length;
    if ( !_bExtension )
    {
	    for( i32End = u32Length - 1; i32End > i32Char; --i32End )
	    {
		    if ( _strFullName[ i32End ] == '.' )
		    {
			    break;
		    }
	    }
        if ( i32End <= i32Char )
        {
            i32End = u32Length;
        }
    }

    return _strFullName.Middle( i32Char, i32End );
}

const CFoundation::String File::GetExtension( const CFoundation::String& _strFileName )
{
	Unsigned32 u32Length = _strFileName.GetLength();

	if( u32Length <= 1 )
	{
		return "";
	}

	Integer32 i32Char = 0;
	for( i32Char = u32Length - 1; i32Char >= 0; --i32Char )
	{
		if ( _strFileName[ i32Char ] == '.' )
		{
			i32Char++;
			break;
		}
	}

	if( i32Char <= 0 )
	{
		return "";
	}
	else
	{
		return _strFileName.Tail( i32Char );
	}
}
