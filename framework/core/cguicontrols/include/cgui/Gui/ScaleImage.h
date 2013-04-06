#ifndef CGUI_SCALEIMAGE_H
#define CGUI_SCALEIMAGE_H

#include "cgui/Gui/CombinedCtrl.h"
#include "cgui/Gui/Image.h"

namespace CGui
{
	class ScaleImage : public CombinedCtrl
	{
	public:
		enum Position
		{
			INVALID_POS,

			TOP_LEFT,
			TOP_CENTER,
			TOP_RIGHT,
			MIDDLE_LEFT,
			MIDDLE_CENTER,
			MIDDLE_RIGHT,
			BOTTOM_LEFT,
			BOTTOM_CENTER,
			BOTTOM_RIGHT,
			
            POSITION_END
		};

        /// Destructor
		virtual ~ScaleImage();

		static const CFoundation::String    GetTypeID();
		virtual const CFoundation::String   GetTypeId() const;

        /// Clears the object
		virtual void                    Clear();

        /// Finishes initializing the object
        virtual void                    EndInit();

		virtual void                    SetSize( Unsigned32 u32Width, Unsigned32 u32Height );
		virtual void                    SetSize( const CFoundation::Vector2Du& clSize );

		void                            SetTexture( Position ePosition, State _eState, const CFoundation::String& sTexture );

        //! Returns the image on the passed position
        ImagePtr                        GetImage( Position ePosition );

        //! Sets the size of the scale image sides
        void                            SetSideSizes( Unsigned32 _u32Left, Unsigned32 _u32Right, Unsigned32 _u32Top, Unsigned32 _u32Bottom );

        //! Returns the left side size
        virtual Unsigned32              GetSideSizeLeft() const;

        //! Returns the right side size
        virtual Unsigned32              GetSideSizeRight() const;

        //! Returns the top side size
        virtual Unsigned32              GetSideSizeTop() const;

        //! Returns the bottom side size
        virtual Unsigned32              GetSideSizeBottom() const;

	private:
        friend class ScaleImageCreator;
        
        /// Constructor
		ScaleImage( const CFoundation::String& _strName );

		void                            UpdatePieceSizes();
		void                            UpdatePositions();

		// Sub Controls
        ImagePtr    m_aspImages[ POSITION_END ];

        // Side sizes
        Unsigned32  m_u32Left;
        Unsigned32  m_u32Right;
        Unsigned32  m_u32Top;
        Unsigned32  m_u32Bottom;
	};
    
    /// ScaleImage pointer
    typedef SharedPtr< ScaleImage > ScaleImagePtr;
    
    /// ScaleImage const pointer
    typedef SharedPtr< const ScaleImage > ScaleImageCPtr;

	class ScaleImageCreator : public CombinedCtrlCreator, public CFoundation::Singleton< ScaleImageCreator >
	{
	public:
		virtual const CFoundation::String GetTypeId() const;

		virtual GuiObjectPtr    CreateObject( const CFoundation::String &sName ) const;
		virtual void            InitDefault( GuiObjectPtr _spObject ) const;

        /// Initializes the passed object from a xml element
		virtual void            InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const;

		//virtual ScaleImage& AssignStyle( GuiObject& clObject, const CtrlStyle& clStyle ) const;
	};
}

#endif
