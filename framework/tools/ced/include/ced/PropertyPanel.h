#ifndef CED_PROPERTY_PANEL_H
#define CED_PROPERTY_PANEL_H

#include "cfoundation/Events/MessageReceiver.h"

#include "cgui/GUI/Button.h"
#include "cgui/GUI/Image.h"
#include "cgui/GUI/ListBox.h"

namespace Ced
{
	class PropertyPanel : public CFoundation::MessageReceiver
	{
	public:
		PropertyPanel();
        
        /// Initialization
        void            Init( CGui::PanelPtr _spPnlProperties );

		virtual void    ProcessMessage( const CFoundation::Message& clMessage );

		void            SetSelected( CGui::GuiObjectPtr _spObject );

	private:

		enum Property
		{
			PROP_NAME,
			PROP_TEXT,
			PROP_WIDTH,
			PROP_HEIGHT,
            PROP_ATTACH_OBJECT,
            PROP_ATTACH_LOCAL,
            PROP_ATTACH_REFERENCE,
			PROP_POSX,
			PROP_POSY,
			PROP_BGTRANSPARENT,
			PROP_BGCOLOR,
			PROP_FONT,
			PROP_FONT_SIZE,
			PROP_NUM_COLS,
			PROP_NUM_MAX_ROWS,
			PROP_INNER_GRID_SIZE_HORIZ,
			PROP_INNER_GRID_SIZE_VERT,
			PROP_OUTER_GRID_SIZE,
			PROP_HEADER_HEIGHT,
			PROP_ROW_HEIGHT,
			PROP_TEXTURE_FILE,
			PROP_REPEAT_X,
			PROP_REPEAT_Y,
			PROP_SCALE_X,
			PROP_SCALE_Y,
			NUM_PROPS
		};

		void operator=( const PropertyPanel& );

		void SetProp( Property eProp, const CFoundation::String &sValue );

		void Update();

		void AddGeneralProps( CGui::GuiObjectCPtr _spGuiObject );
		void AddButtonProps( CGui::ButtonCPtr _spButton );
		void AddImageProps( CGui::ImageCPtr _spImage );
		void AddListBoxProps( CGui::ListBoxCPtr _spListBox );

		void SetGeneralProp( CGui::GuiObjectPtr _spGuiObject, Property eProp, const CFoundation::String &sValue  );
		void SetButtonProp( CGui::ButtonPtr _spButton, Property eProp, const CFoundation::String &sValue );
		void SetImageProp( CGui::ImagePtr _spImage, Property eProp, const CFoundation::String &sValue );
		void SetListBoxProp( CGui::ListBoxPtr _spListBox, Property eProp, const CFoundation::String &sValue );

        /// Callback when object size changed
        void                            OnSizeChanged();

        /// Callback when object position changed
        void                            OnPositionChanged();

        /// Callback when value of a listbox changed
        void                            OnListBoxValueChanged( CGui::ListBoxPtr _spListBox, Unsigned32 _u32Row, Unsigned32 _u32Col, const CFoundation::String& _strNewValue );

        /// Delegates
        CGui::SizeEventDelegate0Ptr     m_spDelegateSizeChanged;
        CGui::PositionEventDelegate0Ptr m_spDelegatePositionChanged;
        CGui::ListBoxEventU32U32StrDelegate4Ptr m_spDelegateListBoxValueChanged;

		CGui::ListBoxPtr    m_spListBox;
		CGui::ListBoxPtr    m_spListBoxChild;

		CGui::GuiObjectPtr  m_spSelObject;

		// Data
		Unsigned32	        m_u32Rows[ NUM_PROPS ];
	};
}

#endif
