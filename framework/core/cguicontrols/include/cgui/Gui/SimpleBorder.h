#ifndef CGUI_SIMPLEBORDER_H
#define CGUI_SIMPLEBORDER_H

#include "cgui/Gui/Border.h"

namespace CGui
{
	class SimpleBorder : public Border
	{
	public:
        /// Constructor
		SimpleBorder( const CFoundation::String& _strName );

        /// Returns the type of this class
        static const CFoundation::String    GetTypeID();

        /// Returns the type of this object
		virtual const CFoundation::String   GetTypeId() const;

    protected:
        /// Callback after reference object was rendered
        virtual void            OnPostRenderReference( GuiObjectPtr _spObject, const RenderParams& _Params, std::vector< RenderElementPtr >& _aspRenderElements );
	};

    INCLUDE_SHARED_CLASS( SimpleBorder );

	class SimpleBorderCreator : public GuiObjectCreator, public CFoundation::Singleton<SimpleBorderCreator>
	{
	public:
        /// Returns the type id of the objects that are created with this creator
		virtual const CFoundation::String GetTypeId() const;

        /// Creates an object with the passed name
		virtual GuiObjectPtr    CreateObject( const CFoundation::String& _strName ) const;
	};
}

#endif
