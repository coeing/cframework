#ifndef CGUI_GUIWINDOW_H
#define CGUI_GUIWINDOW_H

#include "cgui/GUI/Panel.h"

namespace CGui
{
	//! Represents a Window in the app
	/*! This is the main class to realize a window in your app.
	 *
	 *	\author Christian Oeing
	 *
	 *	\date	22.10.06
	 *
	 */
	class Window : public Panel
	{
	public:
        /// Destructor
        virtual ~Window();

		static const CFoundation::String GetTypeID();
		virtual const CFoundation::String GetTypeId() const;

		virtual void Animate();

		//! Returns control with the specified Tabindex
		GuiObjectPtr GetTabObject( Unsigned16 u16TabIndex );
		GuiObjectPtr GetTabObjectBefore( Unsigned16 u16TabIndex );
		GuiObjectPtr GetTabObjectAfter( Unsigned16 u16TabIndex );

	private:
		friend class WindowCreator;

		//! Constructor
		Window( const CFoundation::String& _sName );
	};
    
    /// Window pointer
    typedef SharedPtr< Window > WindowPtr;
    
    /// Window const pointer
    typedef SharedPtr< const Window > WindowCPtr;

	class WindowCreator : public PanelCreator
	{
	public:
		virtual const CFoundation::String GetTypeId() const;

		virtual GuiObjectPtr    CreateObject( const CFoundation::String& _strName ) const;

        /// Initializes the passed object from a xml element
		virtual void            InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const;

        /// Initializes the passed object from another
		virtual void            InitFromTemplate( GuiObjectPtr _spObject, GuiObjectCPtr _spTemplate ) const;

        /// Saves the passed object to a xml element
		virtual bool            SaveToXML( GuiObjectCPtr _spObject, CFoundation::XMLDocumentPtr _spDoc, CFoundation::XMLElementPtr _spXMLElement ) const;

    private:
		friend class CFoundation::Singleton< WindowCreator >;
	};
};

#endif
