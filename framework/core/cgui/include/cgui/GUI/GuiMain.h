#ifndef CLIB_GUIMAIN_H
#define CLIB_GUIMAIN_H

#include <map>
#include <vector>

#include "cfoundation/Utils/SharedPtr.h"
#include "cfoundation/Utils/Singleton.h"

#include "cfoundation/Utils/Color.h"
#include "cfoundation/Math/Vector.h"

#include "cgui/GUI/LayerType.h"
#include "cgui/GUI/RenderParams.h"

namespace CFoundation
{
	INCLUDE_SHARED_CLASS( XMLDocument );
	INCLUDE_SHARED_CLASS( XMLElement );
}

namespace CGui
{
	INCLUDE_SHARED_CLASS( Cursor );
	INCLUDE_SHARED_CLASS( GuiObject );
	INCLUDE_SHARED_CLASS( GuiObjectCreator );
	INCLUDE_SHARED_CLASS( GuiStyle );
	INCLUDE_SHARED_CLASS( Layer );
	INCLUDE_SHARED_CLASS( Renderer );
	INCLUDE_SHARED_CLASS( RenderWindow );

	//! Main Gui class, holds and manages all \link CGui::Window Windows \endlink
	/*!
	 *
	 *	\author	Christian Oeing
	 *
	 *	\date	22.10.06
	 */
	class GuiMain : public CFoundation::Singleton< GuiMain >//, public CFoundation::MessageObject
	{
	public:
        /// Destructor
		~GuiMain();

        /// Initialization
		bool        Init( CFoundation::String sCaption = "Test Window", Unsigned32 u32Width = 800, Unsigned32 u32Height = 600, const CFoundation::Color& clBgColor = CFoundation::Color( 1.0f, 1.0f, 1.0f ) );

        /// Deinitialization
        void        Deinit();

		template < class T >
		SharedPtr< T > Create( const CFoundation::String& _sName ) const
		{
			return static_pointer_cast< T >( Create( T::GetTypeID(), _sName ) );
		}

		template < class T >
		SharedPtr< T > CreateDefault( const CFoundation::String& _sName ) const
		{
			return static_pointer_cast< T >( CreateDefault( T::GetTypeID(), _sName ) );
		}

		template < class T >
		SharedPtr< T > CreateFromXML( const CFoundation::String& _strXMLFile ) const
		{
			return static_pointer_cast< T >( CreateFromXML( _strXMLFile ) );
		}

		template < class T >
		SharedPtr< T > CreateFromXML( CFoundation::XMLElementCPtr _spXMLElement ) const
		{
			return static_pointer_cast< T >( CreateFromXML( _spXMLElement ) );
		}

		template < class T >
		SharedPtr< T > CreateFromTemplate( const CFoundation::String& _sName, SharedPtr< T > _spTemplate ) const
		{
            if ( _spTemplate == NULL )
            {
                return SharedPtr< T >();
            }
            else
            {
			    return static_pointer_cast< T >( CreateFromTemplate( _spTemplate->GetTypeId(), _sName, _spTemplate ) );
            }
		}

		template < class T >
		SharedPtr< T > CreateFromTemplate( const CFoundation::String& _sName, SharedPtr< const T > _spTemplate ) const
		{
            if ( _spTemplate == NULL )
            {
                return SharedPtr< T >();
            }
            else
            {
			    return static_pointer_cast< T >( CreateFromTemplate( _spTemplate->GetTypeId(), _sName, _spTemplate ) );
            }
		}

		GuiObjectPtr            Create( const CFoundation::String &sType, const CFoundation::String &sName ) const;
		GuiObjectPtr            CreateDefault( const CFoundation::String &sType, const CFoundation::String &sName ) const;
		GuiObjectPtr            CreateFromXML( const CFoundation::String& _strXMLFile ) const;
		GuiObjectPtr            CreateFromXML( CFoundation::XMLElementCPtr _spXMLElement ) const;        
		GuiObjectPtr            CreateFromTemplate( const CFoundation::String &sType, const CFoundation::String &sName, GuiObjectCPtr _spTemplate ) const;

		void                    InitDefault( GuiObjectPtr _spObject ) const;
		void                    InitFromXML( GuiObjectPtr _spObject, const CFoundation::String &sXMLFile ) const;
		void                    InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const;

        //! Initializes the passed object from the passed template
		void                    InitFromTemplate( GuiObjectPtr _spObject, GuiObjectCPtr _spTemplate ) const;

		bool                    SaveToXML( GuiObjectCPtr _spObject, const CFoundation::String &sXMLFile ) const;
		bool                    SaveToXML( GuiObjectCPtr _spObject, CFoundation::XMLDocumentPtr _spDoc, CFoundation::XMLElementPtr _spXMLElement ) const;

		void                    Delete( GuiObjectPtr _spObject ) const;

        //! Returns the passed layer
        Layer&                  GetLayer( LayerType _eType ) const;

        void                    SetSize( Unsigned32 u32Width, Unsigned32 u32Height );

		Unsigned32	            GetWidth() const;
		Unsigned32	            GetHeight() const;

        /// Per frame update
		void                    Update( Float32 _f32TimeDifference );

        /// Returns the render window
        RenderWindowPtr         GetRenderWindow() const;

        /// Sets the renderer
        void                    SetRenderer( RendererPtr _spRenderer );

        /// Renders the GUI
		void                    Render();

		//! Register a new object creator
		void                    RegisterObjectCreator( const CFoundation::String& _strObjectType, GuiObjectCreatorPtr _spObjectCreator );

        /// Returns the creator for the passed object type
		GuiObjectCreatorPtr     GetObjectCreator( const CFoundation::String& _strObjectType ) const;

		//! Remove an object creator
		void                    RemoveObjectCreator( const CFoundation::String& _strObjectType );

        /*! Returns the top object at the passed point
         *  Fills clRelativePoint with the relative point
         *  on the found object.
         */
		GuiObjectPtr            GetObjectAtPoint( const CFoundation::Vector2Di& _clPoint, CFoundation::Vector2Di& _clRelativePoint ) const;

        /// Returns the cursor
        CursorPtr               GetCursor() const;

        //! Sets the default gui style
		void                    SetDefaultStyle( GuiStylePtr _spStyle );

        //! Returns the default gui style
		GuiStyleCPtr            GetDefaultStyle() const;

	private:
		friend class CFoundation::Singleton< GuiMain >;

		//! Private constructor because it's a singleton
		GuiMain();

		typedef std::map< CFoundation::String, GuiObjectCreatorPtr > CreatorMap;
		CreatorMap			m_aspGuiObjectCreators;

        //! The render window
		RenderWindowPtr     m_spRenderWindow;

        /// Cursor
        CursorPtr           m_spCursor;

        //! The layers which contain the gui objects
        LayerPtr            m_aspLayers[ LAYER_END ];

        /// Renderer to render the interface elements
        RendererPtr         m_spRenderer;

		//! Render Params
		RenderParams		m_clRenderParams;

		//! The default style
		GuiStylePtr         m_spDefaultStyle;
	};
};

#endif

