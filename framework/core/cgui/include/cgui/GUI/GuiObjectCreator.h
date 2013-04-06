#ifndef CGUI_GUIOBJECTCREATOR_H
#define CGUI_GUIOBJECTCREATOR_H

#include "cfoundation/Utils/SharedPtr.h"

#include "cgui/GUI/GuiObject.h"

namespace CFoundation
{
    INCLUDE_SHARED_CLASS( XMLDocument );
    INCLUDE_SHARED_CLASS( XMLElement );
}

namespace CGui
{
	class GuiNode;

	class GuiObjectCreator
	{
	public:
        /// Returns the type id of the objects that are created with this creator
		virtual const CFoundation::String GetTypeId() const;

        /// Creates an object with the passed name
		virtual GuiObjectPtr    CreateObject( const CFoundation::String& _strName ) const;

        /// Does the default initialization
		virtual void            InitDefault( GuiObjectPtr _spObject ) const;

        /// Initializes the passed object from the passed xml element
		virtual void            InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const;

        /// Initializes an object from another
		virtual void            InitFromTemplate( GuiObjectPtr _spObject, GuiObjectCPtr _spTemplate ) const;

        /// Assigns the passed style to the passed object
		virtual void            AssignStyle( GuiObjectPtr _spObject, const CtrlStyle& _clStyle ) const;

        /// Saves the passed object to a xml element
		virtual bool            SaveToXML( GuiObjectCPtr _spObject, CFoundation::XMLDocumentPtr _spDoc, CFoundation::XMLElementPtr _spXMLElement ) const;
	};

    typedef SharedPtr< GuiObjectCreator > GuiObjectCreatorPtr;
};

#endif
