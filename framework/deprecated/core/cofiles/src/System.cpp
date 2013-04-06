#include "cofiles/System.h"
#include "cofiles/String.h"

using namespace CoFiles;

String System::ExtensionFromFilename( const String &sFileName )
{
	Unsigned32 u32Length = sFileName.GetLength();

	if( u32Length <= 1 )
	{
		return "";
	}

	Integer32 i32Char = 0;
	for( i32Char = u32Length - 1; i32Char >= 0; --i32Char )
	{
		if( sFileName[ i32Char ] == '.' )
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
		return sFileName.Tail( i32Char );
	}
}

String System::GetPath( const String& sFilename )
{
	Unsigned32 u32Length = sFilename.GetLength();

	if( u32Length <= 1 )
	{
		return "";
	}

	Integer32 i32Char = 0;
	for( i32Char = u32Length - 1; i32Char >= 0; --i32Char )
	{
		if( sFilename[ i32Char ] == '/' ||
			sFilename[ i32Char ] == '\\' )
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
		return sFilename.Head( i32Char + 1 );
	}
}
