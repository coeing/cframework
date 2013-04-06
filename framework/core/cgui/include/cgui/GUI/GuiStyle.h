#ifndef CGUI_GUISTYLE_H
#define CGUI_GUISTYLE_H

#include <map>

#include "cfoundation/Text/String.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CSystem
{
    INCLUDE_SHARED_CLASS( File );
    INCLUDE_SHARED_CLASS( Directory );
}

namespace CGui
{
    INCLUDE_SHARED_CLASS( CtrlStyle );

	class GuiStyle
	{
	public:
		GuiStyle( const CFoundation::String& sXMLFilename );
		~GuiStyle();

		CtrlStyleCPtr   GetDefaultStyle( const CFoundation::String& sType ) const;

		CtrlStyleCPtr   GetStyle( const CFoundation::String& sName ) const;

	private:
		void ReadXML();

        /// File
        CSystem::FilePtr		m_spFile;

        CSystem::DirectoryCPtr  m_spBasePath;

		//! Styles
		typedef std::map< const CFoundation::String, CtrlStylePtr > StyleMap;
		StyleMap			m_vStyles;

		StyleMap			m_vDefaultStyles;
	};

    INCLUDE_SHARED_CLASS( GuiStyle );
}

#endif
