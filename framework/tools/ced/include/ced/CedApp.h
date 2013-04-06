#ifndef CED_CEDAPP_H
#define CED_CEDAPP_H

#include "cfoundation/Utils/Singleton.h"

#include "cctrl/App.h"

namespace Ced
{
	class CedApp : public CCtrl::App, public CFoundation::Singleton< CedApp >
	{
	public:		
		virtual void Save( CFoundation::Serializer &clSerializer ) const;
		virtual void Load( const CFoundation::Serializer &clSerializer );

		virtual void Exit();

	protected:
		virtual bool Init();

		virtual const CFoundation::String   GetWindowCaption() const;

        //! Creates the start windows
		virtual void                        CreateStartWindows();
	};
}

#endif
