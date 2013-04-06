#ifndef CGUI_TEXTURELOADER_H
#define CGUI_TEXTURELOADER_H

#include <map>

#include "cfoundation/Utils/Color.h"
#include "cfoundation/Utils/Singleton.h"

namespace CGui
{
	class Texture;

	enum TextureFilter
	{
		FILTER_NONE,
		FILTER_BILINEAR,
		FILTER_TRILINEAR,
		NUM_FILTER
	};

	class TextureParams
	{
    public:
		bool operator==( const TextureParams &rhs ) const
		{
			return	m_sFileName == rhs.m_sFileName &&
					m_bAlphaConversion == rhs.m_bAlphaConversion &&
					m_clAlphaMatch == rhs.m_clAlphaMatch &&
					m_bHighQuality == rhs.m_bHighQuality &&
					m_bMipMapping == rhs.m_bMipMapping &&
					m_eFilterType == rhs.m_eFilterType;
		}

		bool operator<( const TextureParams &rhs ) const
		{
			return m_sFileName < rhs.m_sFileName;
		}

		CFoundation::String		m_sFileName;

		bool				m_bAlphaConversion;
		CFoundation::Color		m_clAlphaMatch;

		bool				m_bHighQuality;
		bool				m_bMipMapping;
		TextureFilter		m_eFilterType;
	};
    typedef SharedPtr< TextureParams > TextureParamsPtr;

	class TextureLoader : public CFoundation::Singleton<TextureLoader>
	{
	public:
		void	SetAlphaMatch( bool bEnable, const CFoundation::Color &clAlphaMatch );
		void	SetHighQualityTextures( bool bEnable );
		void	SetMipMapping( bool bEnable );
		void	SetTextureFilter( TextureFilter eFilterType );

        /// Creates raw pixel data from the passed file
        /// Note: The pixel buffer has to be deleted manually
        //bool    GetPixelDataFromDisk( const CFoundation::String& _strFileName, Unsigned32** _pPixels, Unsigned32& _u32Width, Unsigned32& _u32Height );

        /// Creates a texture from the passed file
		bool	GetTextureFromDisk( const CFoundation::String& _strFileName, Texture &rTexture );
		//int		LoadTextureFromRam( unsigned char *pData, int Length, glTexture *pglTexture, eglTexType TexType );
		//int		LoadTextureFromResource( unsigned int ResourceName, char *pResourceType, glTexture *pglTexture, eglTexType TexType );
		//void	FreeTexture( glTexture *pglTexture );

        /// Reads the pixels of the passed texture into the passed array
        /// Make sure that there's enough space in the array (width*height)
        void    GetTexturePixels( const Texture& _Texture, Unsigned32* _pPixels );

	private:
		friend class CFoundation::Singleton<TextureLoader>;

		TextureLoader();

		// methods
		bool		BuildTexture( const CFoundation::String& sPathName, Texture& clTexture );
		bool		LoadTGAFromDisk( const CFoundation::String& sFileName, Texture& clTexture );
		bool		LoadTIFFromDisk( const CFoundation::String& sFileName, Texture& clTexture );
        bool        LoadPNGFromDisk( const CFoundation::String& sFileName, Texture& clTexture );

		//bool		LoadJPG_GIFResource(char *pResourceName, char *pResourceType, Texture &rTexture );
		//bool		LoadTGAResource(char *pResourceName, char *pResourceType, Texture &rTexture );

		bool		GenerateTexture( Texture &rTexture, unsigned char *pImgData );

		void		ExtensionFromFilename( char *szFileName, char *szExtension );

		typedef std::map< TextureParamsPtr, Texture >   TextureMap;
		TextureMap		m_clCreatedTextures;

		bool			m_bAlphaConversion;
		CFoundation::Color	m_clAlphaMatch;

		bool			m_bHighQualityTextures;
		bool			m_bMipMapping;
		TextureFilter	m_eFilterType;
	};
}

#endif
