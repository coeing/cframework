#ifndef CGUI_SCROLLBAR_H
#define CGUI_SCROLLBAR_H

#include "cfoundation/Math/Rect.h"

#include "cgui/Gui/CombinedCtrl.h"

namespace CGui
{
	class Button;
    typedef SharedPtr< Button > ButtonPtr;
	class Image;
    typedef SharedPtr< Image > ImagePtr;

	class ScrollBar : public CombinedCtrl
	{
	public:
		enum Orientation
		{
			SCROLLBAR_VERTICAL,
			SCROLLBAR_HORIZONTAL
		};

        /// Destructor
		virtual ~ScrollBar();

		static const CFoundation::String    GetTypeID();
		virtual const CFoundation::String   GetTypeId() const;

        /// Clears the object
		virtual void                        Clear();

		// Modify Functions

        /// Sets the orientation
		void            SetOrientation( Orientation eOrientation );

        /// Returns the orientation
        Orientation     GetOrientation() const;

        /// Sets the scroll step (in pixels)
        void            SetScrollStep( Unsigned32 _u32Step );

        /// Returns the scroll step (in pixels)
        Unsigned32      GetScrollStep() const;

		void            SetTotalSize( Unsigned32 u32Size );
		void            SetVisibleSize( Unsigned32 u32Size );

        /// Sets the current bar offset
		void            SetBarOffset( Unsigned32 u32Offset );

        /// Returns the current bar offset
		Unsigned32      GetBarOffset() const;

		// Get Functions
		Unsigned32      GetScrollPosition() const;

    protected:
        /// Changes the size of the object
        virtual void    SetSize( const CFoundation::Vector2Du& _clSize );

	private:
		friend class ScrollBarCreator;

        /// Constructor
		ScrollBar( const CFoundation::String& _strName );

		virtual void    UpdateAllSizes();

		void            UpdateBarPosition();
		void            UpdateBar();

		Unsigned32	    GetMaxBarOffset() const;

		void		    SetBarSize( Unsigned32 u32Size );
		void		    SetBarPosition( Unsigned32 u32Position );

		Unsigned32	    GetBarRectPosition() const;
		Unsigned32      GetBarRectSize() const;

		void            FireScrollMsg();

        /// Called when the up button was pressed
        void                OnPressedUp( GuiObjectPtr _spObject );

        /// Called when the down button was pressed
        void                OnPressedDown( GuiObjectPtr _spObject );

		/// Up button
		ButtonPtr           m_spBtUp;

		/// Down button
		ButtonPtr           m_spBtDown;

		/// Background
		ImagePtr            m_spImgBg;

        /// Bar image
		ImagePtr            m_spImgBar;

		/// Orientation
		Orientation			m_eOrientation;

        /// Scroll step
        Unsigned32          m_u32ScrollStep;

		CFoundation::RectI32	m_clBarRect;

		Unsigned32			m_u32Size;
		Unsigned32			m_u32VisibleSize;

        /// Current bar offset
		Unsigned32			m_u32BarOffset;
	};

    INCLUDE_SHARED_CLASS( ScrollBar );

	class ScrollBarCreator : public CombinedCtrlCreator
	{
	public:
        /// Returns the type id of the objects that are created with this creator
		virtual const CFoundation::String GetTypeId() const;

        /// Creates an object with the passed name
		virtual GuiObjectPtr    CreateObject( const CFoundation::String& _strName ) const;

        /// Initializes the passed object after creation
		virtual void            InitDefault( GuiObjectPtr _spObject ) const;

        /// Initializes the passed object from a xml element
		virtual void            InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const;
	};
}

#endif
