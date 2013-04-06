#include <iostream>
#include <fstream>
#include <math.h>				// Header File For The Math Library

#ifdef WIN32
	#ifndef UNICODE
	#define UNICODE
	#endif
	#include <olectl.h>			// Header File For The OLE Controls Library
	#include <windows.h>
#endif

#include <gl/glu.h>
#include <gl/gl.h>

#include "tiffio.h"				/* Sam Leffler's libtiff library. */
#include "gl/glpng.h"      		/* Ben Wyatt, ben@wyatt100.freeserve.co.uk, http://www.wyatt100.freeserve.co.uk/ */

#include "cfoundation/Debug/Assert.h"

#include "csystem/Files/File.h"

#include "cgui/Texture/TextureLoader.h"
#include "cgui/Texture/Texture.h"

/****************************************************************************
*                                                                           *
* Texture Loader                                                            *
*                                                                           *
* Originally Based on Jeff Molofee's IPicture Basecode                      *
* Extensions By Chris Leathley (http://members.iinet.net.au/~cleathley/)    *
*                                                                           *
*****************************************************************************
*                                                                           *
*  Loads  : BMP, EMF, GIF, ICO, JPG, WMF and TGA                            *
*  Source : Reads From Disk, Ram, Project Resource or the Internet          *
*  Extras : Images Can Be Any Width Or Height                               *
*           Low Quality Textures can be created                             *
*           Different Filter Level Support (None, Bilinear and Trilinear    *
*           Mipmapping Support                                              *
*                                                                           *
*****************************************************************************
*                                                                           *
*  Free To Use In Projects Of Your Own.  All I Ask For Is A Simple Greet    *
*  Or Mention of my site in your readme or the project itself :)            *
*                                                                           *
*****************************************************************************
*                                                                           *
* Revision History                                                          *
*                                                                           *
* Version 1.0 Released                                                      *
* Version 1.1 Added FreeTexture and LoadTextureFromResource                 *
*			  Added TexType to the glTexture Struction                      *
*             Optimisations to the Alpha Conversion Loop			        *
* Version 1.2 Added Support PSP8 TGA files...                               *
*             Single TGA file loader                                        *
* Version 1.3 Added Support for low quality textures..                      *
*             Added ScaleTGA function                                       *
* Version 1.4 Added Support for gluScaleImage                               *
*             Removed ScaleTGA (replacew with gluScaleImage)                *
*             Added TextureFilter and MipMapping Support                    *
*                                                                           *
* Version 1.5 Fitted it to my own project CGui                              *
*                                                                           *
****************************************************************************/

// TGA Header
typedef struct {
	unsigned char ImgIdent;
	unsigned char ignored[ 1 ];
	unsigned char ImgType;
	unsigned char ignored2[ 9 ];
	unsigned char WidthLo;
	unsigned char WidthHi;
	unsigned char HeightLo;
	unsigned char HeightHi;
	unsigned char Bpp;
	unsigned char ignored3[ 1 ];
} _TGAHeader;

using namespace CGui;

TextureLoader::TextureLoader()
{
	// default to alpha matching BLACK
	SetAlphaMatch( true, CFoundation::Color( 0.0f, 0.0f, 0.0f ) );										// Set the Alpha Matching State

	// default to full sized textures
	SetHighQualityTextures( true );

	// no mipmap textures
	SetMipMapping( false );

	// texture filtering
	SetTextureFilter( FILTER_NONE );
}

// Load A Texture from Disk (based on the current location of the executable)
//
bool TextureLoader::GetTextureFromDisk( const CFoundation::String &sFileName, Texture &_clTexture )
{
	CFoundation::String sFullPath;													// Full Path To Picture
	CFoundation::String sExtension;													// Extenstion of Picture

	if( sFileName.Find( "http://" ) ||
		sFileName.Find( ":/" ) ||
		sFileName.Find( ":\\" ) )									// If PathName Contains http:// Then...
	{
		sFullPath = sFileName;											// Append The PathName To FullPath
	}
	else																// Otherwise... We Are Loading From A File
	{
		wchar_t szTmp[ MAX_PATH + 1 ];
		GetCurrentDirectory( MAX_PATH, szTmp );							// Get Our Working Directory
		sFullPath = szTmp;
		sFullPath += L"\\";												// Append "\" After The Working Directory
		sFullPath += sFileName;											// Append The PathName
	}

	// try to find in cache
	TextureParams clToCreate;
	clToCreate.m_sFileName			= sFullPath;
	clToCreate.m_bAlphaConversion	= m_bAlphaConversion;
	clToCreate.m_clAlphaMatch		= m_clAlphaMatch;
	clToCreate.m_bHighQuality		= m_bHighQualityTextures;
	clToCreate.m_bMipMapping		= m_bMipMapping;
	clToCreate.m_eFilterType		= m_eFilterType;

	TextureMap::const_iterator it = m_clCreatedTextures.begin();
	for( ; it != m_clCreatedTextures.end(); ++it )
	{
		if( *it->first == clToCreate )
		{
			_clTexture = it->second;
			return true;
		}
	}

	// check if file exists
	std::ifstream clFile( sFullPath.c_str() );
	if( !clFile.is_open() ) {
	    CF_WARN( L"Texture file %s doesn't exist", sFullPath.wc_str() );
	    return false;
	}

    sExtension = CSystem::File::GetExtension( sFileName );

	bool bSuccess = false;

	// if the file is a TGA then use the TGA file loader
	if( sExtension == "tga" )
	{
		_clTexture.m_eTexType = Texture::TYPE_TGA;
		bSuccess = LoadTGAFromDisk( sFullPath, _clTexture );				// Load TGA (Compressed/Uncompressed)
	}
	// if the file is a TIFF then use the TIFF file loader
	else if( sExtension == "tif" )
	{
		_clTexture.m_eTexType = Texture::TYPE_TIF;
		bSuccess = LoadTIFFromDisk( sFullPath, _clTexture );				// Load TIF (Compressed/Uncompressed)
	}
	else if( sExtension == "png" )
	{
		_clTexture.m_eTexType = Texture::TYPE_PNG;
		bSuccess = LoadPNGFromDisk( sFullPath, _clTexture );				// Load PNG (Compressed/Uncompressed)
	}
	else
	{
		// else load BMP, JPG, GIF

		_clTexture.m_eTexType = Texture::TYPE_UNKNOWN;
		if( sExtension == "bmp" )
		{
			_clTexture.m_eTexType = Texture::TYPE_BMP;
		}
		else if( sExtension == "jpg" ||
				 sExtension == "jpeg" )
		{
			_clTexture.m_eTexType = Texture::TYPE_JPG;
		}
		else if( sExtension == "gif" )
		{
			_clTexture.m_eTexType = Texture::TYPE_GIF;
		}
		else if( sExtension == "ico" )
		{
			_clTexture.m_eTexType = Texture::TYPE_ICO;
		}
		else if( sExtension == "emf" )
		{
			_clTexture.m_eTexType = Texture::TYPE_EMF;
		}
		else if( sExtension == "wmf" )
		{
			_clTexture.m_eTexType = Texture::TYPE_WMF;
		}

		bSuccess = BuildTexture( sFullPath, _clTexture );					// Load BMP, JPG, GIF etc..
	}

	if( bSuccess )
	{
		TextureParamsPtr spParams( new TextureParams( clToCreate ) );
		_clTexture.m_spParams = spParams;
		m_clCreatedTextures[ spParams ] = _clTexture;
	}

	return bSuccess;
}
       
void TextureLoader::GetTexturePixels( const Texture& _Texture, Unsigned32* _pPixels )
{
    glBindTexture( GL_TEXTURE_2D, _Texture.GetID() ); 
    glGetTexImage( GL_TEXTURE_2D, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, _pPixels );
}

// Set Alpha Matching State and Match Colour
//
void TextureLoader::SetAlphaMatch( bool bEnable, const CFoundation::Color &clAlphaMatch )
{
	m_bAlphaConversion	= bEnable;

	// only set the colour match if the conversion is enabled
	if( bEnable )
	{
		m_clAlphaMatch = clAlphaMatch;
	}
}

// set the high quality texture flag
//
void TextureLoader::SetHighQualityTextures( bool bEnable )
{
	m_bHighQualityTextures = bEnable;
}


// set the mipmapping flag
//
void TextureLoader::SetMipMapping( bool bEnable )
{
	m_bMipMapping = bEnable;
}


// set the texture filtering flag
//
void TextureLoader::SetTextureFilter( TextureFilter eFilterType )
{
	m_eFilterType = eFilterType;
}

// Load A Texture from RAM
//
//int	TextureLoader::LoadTextureFromRam(unsigned char *pData, int Length, glTexture *pglTexture, eglTexType TexType)
//{
//	// it would be much easier to allocate some global memory and make a stream out of it but that
//	// would require changing the TGA loading code to use RAM and not Disk access commands
//
//	char szTempFileName[MAX_PATH+1];
//
//	GetCurrentDirectory(MAX_PATH, szTempFileName);						// Get Our Working Directory
//	lstrcat(szTempFileName, "glTEMP$$.tmp");
//
//	HANDLE hFile = CreateFile(szTempFileName,
//							  GENERIC_WRITE,
//							  0,
//							  NULL,
//							  CREATE_ALWAYS,
//							  FILE_ATTRIBUTE_TEMPORARY,
//							  NULL);
//
//	if (hFile != INVALID_HANDLE_VALUE)
//	{
//		int	  rc;
//		DWORD BytesWritten;
//
//		WriteFile(hFile, pData, Length, &BytesWritten, NULL);			// Write The Data To disk (To A Temp File)
//
//		CloseHandle(hFile);												// Close The File
//
//		// set the texture type
//		pglTexture->TexType = TexType;
//
//		// if the file is a TGA then use the TGA file loader
//		if (TexType == txTga)
//		{
//			rc = LoadTGAFromDisk(szTempFileName, pglTexture);
//		}
//		else
//		{
//			// else load BMP, JPG, GIF
//			rc = BuildTexture(szTempFileName, pglTexture);
//		}
//
//		DeleteFile(szTempFileName);										// Delete The Temp File
//
//		return (rc);													// Teturn State Of Texture Load
//	}
//	else
//	{
//		return FALSE;													// Return Failed
//	}
//}
//
//
//// Load A Texture from the project Resource
////
//int	TextureLoader::LoadTextureFromResource(unsigned int ResourceName, char *pResourceType, glTexture *pglTexture, eglTexType TexType)
//{
//	int	rc = FALSE;
//
//	if ((TexType == txJpg) ||
//		(TexType == txGif) )
//	{
//		// set the texture type
//		pglTexture->TexType = TexType;
//
//		rc = LoadJPG_GIFResource(MAKEINTRESOURCE(ResourceName), pResourceType, pglTexture);
//	}
//
//	if (TexType == txBmp)
//	{
//		// set the texture type
//		pglTexture->TexType = TexType;
//
//		// some bug with the bitmap resource loader..
//		//rc = LoadJPG_GIFResource(MAKEINTRESOURCE(ResourceName), pResourceType, pglTexture);
//		//rc = LoadBMPResource(pResourceName, pResourceType, pglTexture);
//	}
//
//	if (TexType == txTga)
//	{
//		// set the texture type
//		pglTexture->TexType = TexType;
//
//		rc = LoadTGAResource(MAKEINTRESOURCE(ResourceName), pResourceType, pglTexture);
//	}
//
//	return (rc);
//}
//
//
//// Free a Texture from openGL
////
//void TextureLoader::FreeTexture(glTexture *pglTexture)
//{
//	glDeleteTextures(1, &pglTexture->TextureID);
//}


// Load BMP, GIF and JPG and Convert To A Texture
//
bool TextureLoader::BuildTexture(const CFoundation::String &sPathName, Texture &_clTexture )
{
	HDC			hdcTemp;												// The DC To Hold Our Bitmap
	HBITMAP		hbmpTemp;												// Holds The Bitmap Temporarily
	IPicture	*pPicture;												// IPicture Interface
	wchar_t		wszPath[ MAX_PATH + 1 ];								// Full Path To Picture (WCHAR)
	long		lWidth;													// Width In Logical Units
	long		lHeight;												// Height In Logical Units
	long		lWidthPixels;											// Width In Pixels
	long		lHeightPixels;											// Height In Pixels
	GLint		glMaxTexDim;											// Holds Maximum Texture Size

	wcscpy_s( wszPath, MAX_PATH + 1, sPathName.wc_str() );
	HRESULT hr = OleLoadPicturePath(wszPath,							// Path
									NULL,								// punkCaller
									0,									// Reserved
									0,									// Reserved Transparent Color
									IID_IPicture,						// riid of Interface to return
									(void**)&pPicture);					// pointer to returned interface

	if(FAILED(hr))														// If Loading Failed
	{
		return false;													// Return False
	}

	hdcTemp = CreateCompatibleDC(GetDC(0));								// Create The Windows Compatible Device Context
	if(!hdcTemp)														// Did Creation Fail?
	{
		pPicture->Release();											// Decrements IPicture Reference Count
		return false;													// Return False (Failure)
	}

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);					// Get Maximum Texture Size Supported

	pPicture->get_Width(&lWidth);										// Get IPicture Width (Convert To Pixels)
	lWidthPixels	= MulDiv(lWidth, GetDeviceCaps(hdcTemp, LOGPIXELSX), 2540);
	pPicture->get_Height(&lHeight);										// Get IPicture Height (Convert To Pixels)
	lHeightPixels	= MulDiv(lHeight, GetDeviceCaps(hdcTemp, LOGPIXELSY), 2540);

	// Resize Image To Closest Power Of Two
	if (lWidthPixels <= glMaxTexDim) // Is Image Width Less Than Or Equal To Cards Limit
	{
		lWidthPixels = 1 << (int)floor((log((double)lWidthPixels)/log(2.0f)) + 0.5f);
	}
	else  // Otherwise  Set Width To "Max Power Of Two" That The Card Can Handle
	{
		lWidthPixels = glMaxTexDim;
	}

	if (lHeightPixels <= glMaxTexDim) // Is Image Height Greater Than Cards Limit
	{
		lHeightPixels = 1 << (int)floor((log((double)lHeightPixels)/log(2.0f)) + 0.5f);
	}
	else  // Otherwise  Set Height To "Max Power Of Two" That The Card Can Handle
	{
		lHeightPixels = glMaxTexDim;
	}

	// if low quality textures then make them halve the size which saved 4 times the texture space
	if( !m_bHighQualityTextures && lWidthPixels > 64 )
	{
		lWidthPixels /= 2;
		lHeightPixels /= 2;
	}

	//	Create A Temporary Bitmap
	BITMAPINFO	bi = {0};												// The Type Of Bitmap We Request
	DWORD		*pBits = 0;												// Pointer To The Bitmap Bits

	bi.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);				// Set Structure Size
	bi.bmiHeader.biBitCount		= 32;									// 32 Bit
	bi.bmiHeader.biWidth		= lWidthPixels;							// Power Of Two Width
	bi.bmiHeader.biHeight		= lHeightPixels;						// Make Image Top Up (Positive Y-Axis)
	bi.bmiHeader.biCompression	= BI_RGB;								// RGB Encoding
	bi.bmiHeader.biPlanes		= 1;									// 1 Bitplane

	//	Creating A Bitmap This Way Allows Us To Specify Color Depth And Gives Us Imediate Access To The Bits
	hbmpTemp = CreateDIBSection(hdcTemp,
								&bi,
								DIB_RGB_COLORS,
								(void**)&pBits,
								0,
								0);

	if(!hbmpTemp)														// Did Creation Fail?
	{
		DeleteDC(hdcTemp);												// Delete The Device Context
		pPicture->Release();											// Decrements IPicture Reference Count
		return FALSE;													// Return False (Failure)
	}

	SelectObject(hdcTemp, hbmpTemp);									// Select Handle To Our Temp DC And Our Temp Bitmap Object

	// Render The IPicture On To The Bitmap
	pPicture->Render(hdcTemp,
					 0,
					 0,
					 lWidthPixels,
					 lHeightPixels,
					 0,
					 lHeight,
					 lWidth,
					 -lHeight,
					 0);

	// Convert From BGR To RGB Format And Add An Alpha Value Of 255
	if( m_bAlphaConversion )
	{
		// it is important to remember that the colour order is different as we are reading in a DWORD
		// so the intel processor will read it in as little endian so the order is reversed (ABGR)
		DWORD AlphaMatch = ((( m_clAlphaMatch.GetU8R() ) | (m_clAlphaMatch.GetU8G() << 8)) | (m_clAlphaMatch.GetU8B() << 16));

		DWORD *pRGBA = (DWORD*)pBits;

		long NumPixels = lWidthPixels * lHeightPixels;
		while(NumPixels--)												// Loop Through All Of The Pixels
		{
			DWORD	tempRGB;

			tempRGB = *pRGBA;
			// swap red and blue over
			tempRGB = ((tempRGB & 0x00FF0000) >> 16) | (tempRGB & 0x0000FF00) | ((tempRGB & 0x000000FF) << 16);

			if (AlphaMatch != tempRGB)
			{
				tempRGB |= 0xFF000000;
			}

			*pRGBA++ = tempRGB;
		}
	}
	else
	{
		BYTE *pPixel = (BYTE*)pBits;									// Grab The Current Pixel

		long NumPixels = lWidthPixels * lHeightPixels;
		while(NumPixels--)
		{

			BYTE  temp	= pPixel[0];									// Store 1st Color In Temp Variable (Blue)
			pPixel[0]	= pPixel[2];									// Move Red Value To Correct Position (1st)
			pPixel[2]	= temp;											// Move Temp Value To Correct Blue Position (3rd)
			pPixel[3]	= 255;											// Set The Alpha Value To 255
			pPixel		+= 4;
		}
	}

	_clTexture.m_clSize.Set( bi.bmiHeader.biWidth, bi.bmiHeader.biHeight );
	_clTexture.m_u16Bpp = bi.bmiHeader.biBitCount;					// Image Color Depth In Bits Per Pixel
	//_clTexture.m_eType		= GL_RGBA;

	glGenTextures( 1, &_clTexture.m_u32ID );								// Create The Texture

	// generate the texture using the filtering model selected
	GenerateTexture( _clTexture, (BYTE *)pBits );

	DeleteObject( hbmpTemp );											// Delete The Object
	DeleteDC( hdcTemp );												// Delete The Device Context

	pPicture->Release();												// Decrements IPicture Reference Count

	return TRUE;														// Return True (All Good)
}


// Load a TGA file
//
bool TextureLoader::LoadTGAFromDisk( const CFoundation::String &sFileName, Texture &_clTexture )
{
	_TGAHeader	header;
	GLubyte		*pImgData = NULL;
	GLint		glMaxTexDim;											// Holds Maximum Texture Size

    FILE		*fTGA = NULL;											// File pointer to texture file
	_wfopen_s( &fTGA, sFileName.wc_str(), L"rb" );						// Open file for reading

	if( fTGA == NULL )													// If it didn't open....
	{
		return false;													// Exit function
	}

	if( fread( &header, sizeof(_TGAHeader), 1, fTGA ) == 0 )			// Attempt to read 12 byte header from file
	{
		if( fTGA != NULL )												// Check to seeiffile is still open
		{
			fclose( fTGA );												// If it is, close it
		}
		return false;													// Exit function
	}

	// Precalc some values from the header
	const unsigned int	imageType			= header.ImgType;
	const unsigned int	imageWidth			= header.WidthLo  + header.WidthHi  * 256;
	const unsigned int	imageHeight			= header.HeightLo + header.HeightHi * 256;
	const Unsigned8		imageBytesPerPel	= header.Bpp / 8;
	const unsigned int	imageSize			= imageWidth * imageHeight * imageBytesPerPel;

	// load up our texture information
	_clTexture.m_clSize.Set( imageWidth, imageHeight );
	_clTexture.m_u16Bpp = header.Bpp;

	if ( _clTexture.m_u16Bpp == 24 )											// If the BPP of the image is 24...
	{
		_clTexture.m_eColorType = GL_RGB;									// Set Image type to GL_RGB
	}
	else																// Else if its 32 BPP
	{
		_clTexture.m_eColorType = GL_RGBA;								// Set image type to GL_RGBA
	}

	// Validate header info
	if( ( imageType != 2 && imageType != 10 ) ||
	    ( imageWidth == 0 ) || ( imageHeight == 0 ) ||
		( imageBytesPerPel != 3 && imageBytesPerPel != 4 ) )
	{
		// invalid header, bomb out
		fclose( fTGA );
		return false;
	}

	// Allocate the memory for the image size
	pImgData = (GLubyte *)malloc(imageSize);

	if(pImgData == NULL)												// If no space was allocated
	{
		fclose(fTGA);													// Close the file
		return FALSE;													// Return failed
	}

	// Skip image ident field
	if( header.ImgIdent > 0 )
	{
		fseek(fTGA, header.ImgIdent, SEEK_CUR);
	}

	// un-compresses image ?
	if (imageType == 2)
	{
		if(fread(pImgData, 1, imageSize, fTGA) != imageSize)			// Attempt to read image data
		{
			if(pImgData != NULL)										// If imagedata has data in it
			{
				free(pImgData);											// Delete data from memory
			}

			fclose(fTGA);												// Close file
			return FALSE;												// Return failed
		}

		// Byte Swapping Optimized By Steve Thomas
		for(GLuint cswap = 0; cswap < imageSize; cswap += imageBytesPerPel)
		{
			pImgData[cswap] ^= pImgData[cswap+2] ^=
			pImgData[cswap] ^= pImgData[cswap+2];
		}
	}
	else
	{
		// compressed image
		GLuint pixelcount	= imageHeight * imageWidth;					// Nuber of pixels in the image
		GLuint currentpixel	= 0;										// Current pixel being read
		GLuint currentbyte	= 0;										// Current byte
		GLubyte * colorbuffer = (GLubyte *)malloc(imageBytesPerPel);	// Storage for 1 pixel

		do
		{
			GLubyte chunkheader = 0;										// Storage for "chunk" header

			if(fread(&chunkheader, sizeof(GLubyte), 1, fTGA) == 0)			// Read in the 1 byte header
			{
				if(fTGA != NULL)											// If file is open
				{
					fclose(fTGA);											// Close file
				}
				if(pImgData != NULL)										// If there is stored image data
				{
					free(pImgData);											// Delete image data
				}
				return FALSE;												// Return failed
			}

			if(chunkheader < 128)											// If the ehader is < 128, it means the that is the number of RAW color packets minus 1
			{																// that follow the header
				chunkheader++;												// add 1 to get number of following color values
				for(short counter = 0; counter < chunkheader; counter++)	// Read RAW color values
				{
					if(fread(colorbuffer, 1, imageBytesPerPel, fTGA) != imageBytesPerPel) // Try to read 1 pixel
					{
						if(fTGA != NULL)									// See if file is open
						{
							fclose(fTGA);									// If so, close file
						}

						if(colorbuffer != NULL)							// See if colorbuffer has data in it
						{
							free(colorbuffer);							// If so, delete it
						}

						if(pImgData != NULL)							// See if there is stored Image data
						{
							free(pImgData);								// If so, delete it too
						}

						return FALSE;									// Return failed
					}
																		// write to memory
					pImgData[currentbyte		] = colorbuffer[2];		// Flip R and B vcolor values around in the process
					pImgData[currentbyte + 1	] = colorbuffer[1];
					pImgData[currentbyte + 2	] = colorbuffer[0];

					if (imageBytesPerPel == 4)							// if its a 32 bpp image
					{
						pImgData[currentbyte + 3] = colorbuffer[3];		// copy the 4th byte
					}

					currentbyte += imageBytesPerPel;					// Increase thecurrent byte by the number of bytes per pixel
					currentpixel++;										// Increase current pixel by 1

					if(currentpixel > pixelcount)						// Make sure we havent read too many pixels
					{
						if(fTGA != NULL)								// If there is a file open
						{
							fclose(fTGA);								// Close file
						}

						if(colorbuffer != NULL)							// If there is data in colorbuffer
						{
							free(colorbuffer);							// Delete it
						}

						if(pImgData != NULL)							// If there is Image data
						{
							free(pImgData);								// delete it
						}

						return FALSE;									// Return failed
					}
				}
			}
			else														// chunkheader > 128 RLE data, next color reapeated chunkheader - 127 times
			{
				chunkheader -= 127;										// Subtract 127 to get rid of the ID bit
				if(fread(colorbuffer, 1, imageBytesPerPel, fTGA) != imageBytesPerPel)	// Attempt to read following color values
				{
					if(fTGA != NULL)									// If thereis a file open
					{
						fclose(fTGA);									// Close it
					}

					if(colorbuffer != NULL)								// If there is data in the colorbuffer
					{
						free(colorbuffer);								// delete it
					}

					if(pImgData != NULL)								// If there is image data
					{
						free(pImgData);									// delete it
					}

					return FALSE;										// return failed
				}

				for(short counter = 0; counter < chunkheader; counter++)// copy the color into the image data as many times as dictated
				{														// by the header
					pImgData[currentbyte		] = colorbuffer[2];		// switch R and B bytes areound while copying
					pImgData[currentbyte + 1	] = colorbuffer[1];
					pImgData[currentbyte   + 2	] = colorbuffer[0];

					if(imageBytesPerPel == 4)							// If TGA images is 32 bpp
					{
						pImgData[currentbyte + 3] = colorbuffer[3];		// Copy 4th byte
					}

					currentbyte += imageBytesPerPel;					// Increase current byte by the number of bytes per pixel
					currentpixel++;										// Increase pixel count by 1

					if(currentpixel > pixelcount)						// Make sure we havent written too many pixels
					{
						if(fTGA != NULL)								// If there is a file open
						{
							fclose(fTGA);								// Close file
						}

						if(colorbuffer != NULL)							// If there is data in colorbuffer
						{
							free(colorbuffer);							// Delete it
						}

						if(pImgData != NULL)							// If there is Image data
						{
							free(pImgData);								// delete it
						}

						return FALSE;									// Return failed
					}
				} // for(counter)
			} // if(chunkheader)
		}
		while(currentpixel < pixelcount);								// Loop while there are still pixels left
	} // if (imageType == 2)

	fclose (fTGA);														// Close the TGA file

	/*
	** Scale Image to be a power of 2
	*/

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);					// Get Maximum Texture Size Supported

	int lWidthPixels  = imageWidth;
	int lHeightPixels = imageHeight;

	// Resize Image To Closest Power Of Two
	if (lWidthPixels <= glMaxTexDim) // Is Image Width Less Than Or Equal To Cards Limit
		lWidthPixels = 1 << (int)floor((log((double)lWidthPixels)/log(2.0f)) + 0.5f);
	else  // Otherwise  Set Width To "Max Power Of Two" That The Card Can Handle
		lWidthPixels = glMaxTexDim;

	if (lHeightPixels <= glMaxTexDim) // Is Image Height Greater Than Cards Limit
		lHeightPixels = 1 << (int)floor((log((double)lHeightPixels)/log(2.0f)) + 0.5f);
	else  // Otherwise  Set Height To "Max Power Of Two" That The Card Can Handle
		lHeightPixels = glMaxTexDim;

	// if low quality textures then make them halve the size which saved 4 times the texture space
	if( !m_bHighQualityTextures && lWidthPixels > 64 )
	{
		lWidthPixels /= 2;
		lHeightPixels /= 2;
	}

	// if the size needs to change, the rescale the raw image data
	if ( (lWidthPixels  != (int)imageWidth)	&&
		 (lHeightPixels != (int)imageHeight) )
	{
		// allocated the some memory for the new texture
		GLubyte	*pNewImgData = (GLubyte *)malloc(lWidthPixels * lHeightPixels * imageBytesPerPel);

		GLenum format;
		if (imageBytesPerPel == 4)
		{
			format = GL_RGBA;
		}
		else
		{
			format = GL_RGB;
		}

		gluScaleImage(format, imageWidth, imageHeight, GL_UNSIGNED_BYTE, pImgData,
							  lWidthPixels, lHeightPixels, GL_UNSIGNED_BYTE, pNewImgData);

		// free the original image data
		free(pImgData);

		// old becomes new..
		pImgData = pNewImgData;

		// update our texture structure
        _clTexture.m_clSize.Set( lWidthPixels, lHeightPixels );
	}

	// Typical Texture Generation Using Data From The TGA loader
	glGenTextures(1, &_clTexture.m_u32ID);						// Create The Texture

	// generate the texture using the filtering model selected
	GenerateTexture( _clTexture, (BYTE *)pImgData);

	// free the memory allocated
	free(pImgData);

	return true;														// All went well, continue on
}

bool TextureLoader::LoadTIFFromDisk( const CFoundation::String &sFileName, CGui::Texture &_clTexture )
{
	TIFFRGBAImage img;
	Unsigned32 *raster;
	Unsigned32 npixels;
	int tw, th;

	int hasABGR = 0;
	char emsg[1024];

	//glutInit(&argc, argv);

	TIFF *tif = TIFFOpenW( sFileName.wc_str(), "r" );
	if( tif == NULL )
	{
		CF_WARN( L"Problem open %s\n", sFileName.wc_str() );
		return false;
	}

	if( !TIFFRGBAImageBegin( &img, tif, 0, emsg ) )
	{
		CF_WARN( L"Problem: %s\n", emsg );
		return false;
	}

	npixels = img.width * img.height;
	raster = (uint32 *) _TIFFmalloc(npixels * sizeof(uint32));
	if( raster != NULL )
	{
		if( TIFFRGBAImageGet( &img, raster, img.width, img.height ) == 0 )
		{
			CF_WARN( L"Problem: %s\n", emsg );
			return false;
		}
	}
	TIFFRGBAImageEnd(&img);

#ifdef GL_EXT_abgr
	if (glutExtensionSupported("GL_EXT_abgr"))
		hasABGR = 1;
#else
	hasABGR = 0;
#endif

	/* If cannot directly display ABGR format, we need to reverse the component
		ordering in each pixel. :-( */
	if( !hasABGR )
	{
		//for( Unsigned32 u32Idx = 0; u32Idx < npixels; ++u32Idx )
		{
			//register unsigned char *cp = (unsigned char *) &raster[ u32Idx ];
			//int t;

			//t = cp[3];
			//cp[3] = cp[0];
			//cp[0] = t;
			//t = cp[2];
			//cp[2] = cp[1];
			//cp[1] = t;
		}
	}
	/* OpenGL's default unpack (and pack) alignment is 4.  In the case of the
	 data returned by libtiff which is already aligned on 32-bit boundaries,
	 setting the pack to 1 isn't strictly necessary. */
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	gluOrtho2D(-1, 1, -1, 1);

	/* A TIFF file could be any size; OpenGL textures are allowed to have a
	 width and height that is a power of two (32, 64, 128, etc.). To maximize
	 the use of available texture memory, we scale the image to gluScaleImage
	 to the next larger power of 2 width or height dimension (not exceeding
	 512, don't want to use too much texture memory!).  This rescaling can
	 result in a bit of image bluring because of the resampling done by
	 gluScaleImage.  An alternative would be to change the texture coordinates
	 to only use a portion texture area. */

    tw = 1 << (int) ceil( log( static_cast<Float32>( img.width ) ) / log( 2.0f ));
	th = 1 << (int) ceil( log( static_cast<Float32>( img.height ) ) / log( 2.0f ));
	if (tw > 512)
	tw = 512;
	if (th > 512)
	th = 512;
	GLubyte* pImgData = new GLubyte[ 4 * tw * th ];

	gluScaleImage(	GL_RGBA,
					img.width, img.height, GL_UNSIGNED_BYTE, raster,
					tw, th, GL_UNSIGNED_BYTE, pImgData );
	_TIFFfree(raster);

	_clTexture.m_clSize.Set( tw, th );
	_clTexture.m_u16Bpp = img.bitspersample * img.samplesperpixel;
	_clTexture.m_eColorType = GL_RGBA;

	glGenTextures( 1, &_clTexture.m_u32ID );						// Create The Texture

	// generate the texture using the filtering model selected
	GenerateTexture( _clTexture, (BYTE *)pImgData );

    delete [] pImgData;

	return true;
}

bool TextureLoader::LoadPNGFromDisk( const CFoundation::String& sFileName, Texture& clTexture )
{
    Unsigned32 u32Texture = 0;
	pngInfo info;
   	#if 1 // Using pngLoad and setting texture parameters manually.
		glGenTextures( 1, &u32Texture );
		glBindTexture( GL_TEXTURE_2D, u32Texture );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

		bool	bMipping;
		switch( m_eFilterType )
		{
			default:
			case FILTER_NONE:
				{
					//glPixelStorei(GL_UNPACK_ALIGNMENT, 1 );
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
					bMipping = false;
				}
				break;

			case FILTER_BILINEAR:
				{
					if ( m_bMipMapping )
					{
						glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
						glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
						bMipping = false;
					}
					else
					{
						glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
						glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
						bMipping = true;
					}
				}
				break;

			case FILTER_TRILINEAR:
				{
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
					// always mip mapping for trilinear
					bMipping = true;
				}
				break;
		}

		Integer8 i8MipMap = PNG_NOMIPMAP;
		if( bMipping ) {
			i8MipMap = PNG_BUILDMIPMAP;
		}

		if( !pngLoad( sFileName.c_str(), i8MipMap, PNG_ALPHA, &info ) ) {
		    CF_WARN( L"Can't load PNG file %s.", sFileName.wc_str() );
			return false;
		}
	#else // Using pngLoadAndBind to set texture parameters automatically.
		u32Texture = pngBind( sFileName.c_str(), PNG_NOMIPMAP, PNG_ALPHA, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST );

		if( u32Texture == 0 ) {
		    CF_WARN( L"Can't load PNG file %s.", sFileName.wc_str() );
			return false;
		}
	#endif

	clTexture.m_u32ID = u32Texture;
	clTexture.m_clSize.Set( info.Width, info.Height );

	return true;
}

// extract the extension from the specified filename
//
//int TextureLoader::LoadJPG_GIFResource( const CFoundation::String &sResourceName, const CFoundation::String &sResourceType, Texture &_clTexture )
//{
//	HDC			hdcTemp;												// The DC To Hold Our Bitmap
//	HBITMAP		hbmpTemp;												// Holds The Bitmap Temporarily
//	IPicture	*pPicture;												// IPicture Interface
//	long		lWidth;													// Width In Logical Units
//	long		lHeight;												// Height In Logical Units
//	long		lWidthPixels;											// Width In Pixels
//	long		lHeightPixels;											// Height In Pixels
//	GLint		glMaxTexDim ;											// Holds Maximum Texture Size
//
//	// from resources
//	HRSRC	hRes;
//	hRes = FindResource(NULL, pResourceName, pResourceType);
//
//	DWORD	dwDataSize	= SizeofResource(NULL,hRes);
//	HGLOBAL	hGlob		= LoadResource(NULL,hRes);
//	LPVOID	pData		= LockResource(hGlob);
//
//	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwDataSize);
//	void* pData2 = GlobalLock(hGlobal);
//	memcpy(pData2, pData, dwDataSize);
//	GlobalUnlock(hGlobal);
//	UnlockResource(hGlob);
//	FreeResource(hGlob);
//
//	IStream* pStream = NULL;
//	pPicture = NULL;
//	HRESULT hr;
//
//	if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK)
//	{
//		hr = OleLoadPicture(pStream, 0, 0, IID_IPicture, (void**)&pPicture);
//		pStream->Release();
//	}
//
//	if(FAILED(hr))														// If Loading Failed
//		return FALSE;													// Return False
//
//	hdcTemp = CreateCompatibleDC(GetDC(0));								// Create The Windows Compatible Device Context
//	if(!hdcTemp)														// Did Creation Fail?
//	{
//		pPicture->Release();											// Decrements IPicture Reference Count
//		return FALSE;													// Return False (Failure)
//	}
//
//	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);					// Get Maximum Texture Size Supported
//
//	pPicture->get_Width(&lWidth);										// Get IPicture Width (Convert To Pixels)
//	lWidthPixels	= MulDiv(lWidth, GetDeviceCaps(hdcTemp, LOGPIXELSX), 2540);
//	pPicture->get_Height(&lHeight);										// Get IPicture Height (Convert To Pixels)
//	lHeightPixels	= MulDiv(lHeight, GetDeviceCaps(hdcTemp, LOGPIXELSY), 2540);
//
//	// Resize Image To Closest Power Of Two
//	if (lWidthPixels <= glMaxTexDim) // Is Image Width Less Than Or Equal To Cards Limit
//		lWidthPixels = 1 << (int)floor((log((double)lWidthPixels)/log(2.0f)) + 0.5f);
//	else  // Otherwise  Set Width To "Max Power Of Two" That The Card Can Handle
//		lWidthPixels = glMaxTexDim;
//
//	if (lHeightPixels <= glMaxTexDim) // Is Image Height Greater Than Cards Limit
//		lHeightPixels = 1 << (int)floor((log((double)lHeightPixels)/log(2.0f)) + 0.5f);
//	else  // Otherwise  Set Height To "Max Power Of Two" That The Card Can Handle
//		lHeightPixels = glMaxTexDim;
//
//	// if low quality textures then make them halve the size which saved 4 times the texture space
//	if ((m_fHighQualityTextures == FALSE) && (lWidthPixels > 64))
//	{
//		lWidthPixels /= 2;
//		lHeightPixels /= 2;
//	}
//
//	//	Create A Temporary Bitmap
//	BITMAPINFO	bi = {0};												// The Type Of Bitmap We Request
//	DWORD		*pBits = 0;												// Pointer To The Bitmap Bits
//
//	bi.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);				// Set Structure Size
//	bi.bmiHeader.biBitCount		= 32;									// 32 Bit
//	bi.bmiHeader.biWidth		= lWidthPixels;							// Power Of Two Width
//	bi.bmiHeader.biHeight		= lHeightPixels;						// Make Image Top Up (Positive Y-Axis)
//	bi.bmiHeader.biCompression	= BI_RGB;								// RGB Encoding
//	bi.bmiHeader.biPlanes		= 1;									// 1 Bitplane
//
//	//	Creating A Bitmap This Way Allows Us To Specify Color Depth And Gives Us Imediate Access To The Bits
//	hbmpTemp = CreateDIBSection(hdcTemp, &bi, DIB_RGB_COLORS, (void**)&pBits, 0, 0);
//
//	if(!hbmpTemp)														// Did Creation Fail?
//	{
//		DeleteDC(hdcTemp);												// Delete The Device Context
//		pPicture->Release();											// Decrements IPicture Reference Count
//		return FALSE;													// Return False (Failure)
//	}
//
//	SelectObject(hdcTemp, hbmpTemp);									// Select Handle To Our Temp DC And Our Temp Bitmap Object
//
//	// Render The IPicture On To The Bitmap
//	pPicture->Render(hdcTemp, 0, 0, lWidthPixels, lHeightPixels, 0, lHeight, lWidth, -lHeight, 0);
//
//	// Convert From BGR To RGB Format And Add An Alpha Value Of 255
//	if( m_bAlphaConversion && _clTexture.m_eTexType != Texture::TYPE_JPG )
//	{
//		// it is important to remember that the colour order is different as we are reading in a DWORD
//		// so the intel processor will read it in as little endian so the order is reversed (ABGR)
//		DWORD AlphaMatch = (((m_RedAlphaMatch) | (m_GreenAlphaMatch << 8)) | (m_BlueAlphaMatch << 16));
//
//		DWORD *pRGBA = (DWORD*)pBits;
//
//		long NumPixels = lWidthPixels * lHeightPixels;
//		while(NumPixels--)												// Loop Through All Of The Pixels
//		{
//			DWORD	tempRGB;
//
//			tempRGB = *pRGBA;
//			// swap red and blue over
//			tempRGB = ((tempRGB & 0x00FF0000) >> 16) | (tempRGB & 0x0000FF00) | ((tempRGB & 0x000000FF) << 16);
//
//			if (AlphaMatch != tempRGB)
//			{
//				tempRGB |= 0xFF000000;
//			}
//
//			*pRGBA++ = tempRGB;
//		}
//	}
//	else
//	{
//		BYTE *pPixel = (BYTE*)pBits;									// Grab The Current Pixel
//
//		long NumPixels = lWidthPixels * lHeightPixels;
//		while(NumPixels--)
//		{
//
//			BYTE  temp	= pPixel[0];									// Store 1st Color In Temp Variable (Blue)
//			pPixel[0]	= pPixel[2];									// Move Red Value To Correct Position (1st)
//			pPixel[2]	= temp;											// Move Temp Value To Correct Blue Position (3rd)
//			pPixel[3]	= 255;											// Set The Alpha Value To 255
//			pPixel		+= 4;
//		}
//	}
//
//	_clTexture.m_u32Width		= bi.bmiHeader.biWidth;
//	_clTexture.m_u32Height	= bi.bmiHeader.biHeight;
//	_clTexture.m_u8Bpp		= bi.bmiHeader.biBitCount;					// Image Color Depth In Bits Per Pixel
//	_clTexture.m_eColorType	= GL_RGBA;
//
//	glGenTextures( 1, &_clTexture.m_u32ID );								// Create The Texture
//
//	// generate the texture using the filtering model selected
//	(void)GenerateTexture(pglTexture, (BYTE *)pBits);
//
//	DeleteObject(hbmpTemp);												// Delete The Object
//	DeleteDC(hdcTemp);													// Delete The Device Context
//
//	pPicture->Release();												// Decrements IPicture Reference Count
//
//	return TRUE;														// Return True (All Good)
//}
//
//
//int	TextureLoader::LoadTGAResource(char *pResourceName, char *pResourceType, glTexture *pglTexture)
//{
//	// from resources
//	HRSRC	hRes;
//	hRes = FindResource(NULL, pResourceName, pResourceType);
//
//	DWORD	dwDataSize	= SizeofResource(NULL,hRes);
//	HGLOBAL	hGlob		= LoadResource(NULL,hRes);
//	LPVOID	pData		= LockResource(hGlob);
//
//	// allocate some global memory and copy the resource data into it..
//	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwDataSize);
//	void* pData2 = GlobalLock(hGlobal);
//	memcpy(pData2, pData, dwDataSize);
//
//	// unlock and free the resource
//	GlobalUnlock(hGlobal);
//	UnlockResource(hGlob);
//	FreeResource(hGlob);
//
//	// load it
//	int rc = LoadTextureFromRam((unsigned char *)pData2, dwDataSize, pglTexture, txTga);
//
//	// free our global memory (16 bit os's only)
//	GlobalFree(hGlobal);
//
//	return rc;
//}


// Set the Texture parameters to match the type of filtering we want.
//
bool TextureLoader::GenerateTexture( Texture& _clTexture, unsigned char *pImgData)
{
	int result = 0;

	int	components;
	// set the bytes per pixel
	if( _clTexture.m_eColorType == GL_RGBA )
	{
		components = 4;
	}
	else
	{
		components = 3;
	}

	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture( GL_TEXTURE_2D, _clTexture.m_u32ID );				// Bind To The Texture ID

	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP );

	BOOL	Mipping;

	switch( m_eFilterType )
	{
		default:
		case FILTER_NONE:
			{
				//glPixelStorei(GL_UNPACK_ALIGNMENT, 1 );
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				Mipping = FALSE;
			}
			break;

		case FILTER_BILINEAR:
			{
				if( m_bMipMapping )
				{
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
					// set the build type flag
					Mipping = FALSE;
				}
				else
				{
					glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
					Mipping = TRUE;
				}
			}
			break;

		case FILTER_TRILINEAR:
			{
				glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				// always mip mapping for trilinear
				Mipping = TRUE;
			}
			break;
	}


	// crank out the texture
    Unsigned32 u32Width = _clTexture.m_clSize.GetX();
    Unsigned32 u32Height = _clTexture.m_clSize.GetY();
	if (Mipping == FALSE)
	{
		glTexImage2D(GL_TEXTURE_2D,
					 0,
					 components,
					 u32Width,
					 u32Height,
					 0,
					 _clTexture.m_eColorType,
					 GL_UNSIGNED_BYTE,
					 pImgData);
	}
	else
	{
		// Build Mipmaps (builds different versions of the picture for distances - looks better)
		result = gluBuild2DMipmaps(GL_TEXTURE_2D,
								   components,
								   u32Width,
								   u32Height,
								   _clTexture.m_eColorType,
								   GL_UNSIGNED_BYTE,
								   pImgData );
	}

	return 0;
}
