#ifndef CGUI_SCROLLPANEL_H
#define CGUI_SCROLLPANEL_H

#include "cgui/Gui/Decorator.h"
#include "cgui/Gui/ScrollingType.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( ScrollBar );
    INCLUDE_SHARED_CLASS( ScrollPanel );

	//! A Scroll-Panel is a panel with the functionality to show two scrollbars
	/*! With this two scrollbars (horizontal and vertical) the content in this panel
	 *  can be scrolled.
	 */
	class ScrollPanel : public Decorator
	{
	public:
		static const CFoundation::String    GetTypeID();
		virtual const CFoundation::String   GetTypeId() const;

        /// Clears the object
		virtual void                        Clear();

		void ShowScrollBarHoriz( bool bEnable );
		void ShowScrollBarVert( bool bEnable );

		bool IsScrollBarHorizVisible() const;
		bool IsScrollBarVertVisible() const;

        /// Sets the reference object
		virtual void                        SetReference( GuiObjectPtr _spReference );

		virtual void SetScrollingHoriz( ScrollingType eType );
		virtual void SetScrollingVert( ScrollingType eType );

	protected:
        /// Changes the size of the object
        virtual void    SetSize( const CFoundation::Vector2Du& _clSize );

		virtual void NewReferenceSize( Unsigned32 u32Width, Unsigned32 u32Height );

		virtual void FitReference();

		virtual Unsigned32 GetLeftMargin() const;
		virtual Unsigned32 GetRightMargin() const;
		virtual Unsigned32 GetTopMargin() const;
		virtual Unsigned32 GetBottomMargin() const;

		Unsigned32  GetVisibleWidth() const;
		Unsigned32  GetVisibleHeight() const;

		Unsigned32  GetScrollPositionX() const;
		Unsigned32  GetScrollPositionY() const;

        /// Updates the scroll bar sizes
		void        UpdateScrollBarSizes();

        /// Updates the scroll bar settings
		void        UpdateScrollBarSettings();

	private:
        friend class ScrollPanelCreator;

        /// Constructor
		ScrollPanel( const CFoundation::String& _strName );

        /// Called when the view panel's size changed
        void                    OnViewPanelSizeChanged();

        /// Delegates
        SizeEventDelegate0Ptr   m_spDelegateViewPanelSizeChanged;

		// Sub-Controls
		ScrollBarPtr        m_spScrollBarHoriz;
		ScrollBarPtr        m_spScrollBarVert;

        /// Clipping panel
		PanelPtr            m_spPnlClipping;

        /// View panel
		PanelPtr            m_spViewPanel;

		// Data
		CFoundation::Vector2Di	m_clRefPosition;
		ScrollingType		m_eScrollingHoriz;
		ScrollingType		m_eScrollingVert;
	};

	class ScrollPanelCreator : public DecoratorCreator
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
		//virtual void            InitFromTemplate( GuiObjectPtr _spObject, GuiObjectCPtr _spTemplate ) const;

        /// Saves the passed object to a xml element
		//virtual bool            SaveToXML( GuiObjectCPtr _spObject, CFoundation::XMLDocumentPtr _spDoc, CFoundation::XMLElementPtr _spXMLElement ) const;
	};
}

#endif
