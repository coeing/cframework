#ifndef CETRIS_CETRISAPP_H
#define CETRIS_CETRISAPP_H

#include "cfoundation/Utils/Singleton.h"

#include "cctrl/App.h"

namespace Cetris
{
	class CetrisApp : public CCtrl::App, public CFoundation::Singleton<CetrisApp>
	{
	public:		
		virtual void Save( CFoundation::Serializer &clSerializer ) const;
		virtual void Load( const CFoundation::Serializer &clSerializer );

		virtual void Exit();

	protected:
		virtual bool Init();

        /// Returns the caption of the render window
		virtual const CFoundation::String   GetWindowCaption() const;

        /// Returns the width of the render window
		virtual Unsigned32              GetWidth() const;
        
        /// Returns the height of the render window
		virtual Unsigned32              GetHeight() const;

        /// Creates the start windows
		virtual void                    CreateStartWindows();
	};
}

#endif
