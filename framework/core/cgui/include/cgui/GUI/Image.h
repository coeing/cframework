#ifndef CGUI_IMAGE_H
#define CGUI_IMAGE_H

#include "cgui/GUI/GuiObject.h"
#include "cgui/GUI/GuiObjectCreator.h"

#include "cgui/Texture/Texture.h"

namespace CGui
{
	class Image : public GuiObject
	{
	public:
        /// Destructor
		virtual ~Image();

		static const CFoundation::String    GetTypeID();
		virtual const CFoundation::String   GetTypeId() const;

        //! Changes the current state
        virtual void                    SetState( State _eState );

        //! Sets the texture for all states
		void                            SetTexture( const CFoundation::String& _sTextureFile );

        //! Sets the texture for all states
        void                            SetTexture( const Texture& _clTexture );

        /// Sets the texture rectangle for all states
        void                            SetTextureRect( const CFoundation::RectF16& _rectTexture );

        //! Sets the texture for the passed state
		void                            SetTexture( State _eState, const CFoundation::String& _sTextureFile );

        /// Sets the texture for the passed state
        void                            SetTexture( State _eState, const Texture& _clTexture );

        /// Returns the texture of the passed state
		const Texture&                  GetTexture( State _eState ) const;

        /// Sets the texture rectangle for the passed state
        void                            SetTextureRect( State _eState, const CFoundation::RectF16& _rectTexture );

        /// Returns the texture rectangle of the passed state
		const CFoundation::RectF16&     GetTextureRect( State _eState ) const;

		void                            SetMask( const CFoundation::String &sMask );
		const CFoundation::String           GetMask() const;

        //! Repeats the texture over the image width
		virtual void                    SetRepeatX( bool bRepeat );

        //! Returns if the texture is repeated over the image width
		virtual bool                    GetRepeatX() const;

        //! Repeats the texture over the image height
		virtual void                    SetRepeatY( bool bRepeat );

        //! Returns if the texture is repeated over the image height
		virtual bool                    GetRepeatY() const;

        //! Scales the texture to the image width
		virtual void                    SetScaleX( bool bScale );

        //! Returns if the texture is scaled to the image width
		virtual bool                    GetScaleX() const;

        //! Scales the texture to the image height
		virtual void                    SetScaleY( bool bScale );

        //! Returns if the texture is scaled to the image height
		virtual bool                    GetScaleY() const;

        /// Sets the rotation of the image (in degrees)
        virtual void                    SetRotation( Float16 _fAngle );

        /// Returns the rotation of the image (in degrees)
        virtual Float16                 GetRotation() const;

    protected:
        /// Computes the size of the object
        /*! Depending on the size mode the real size
         *  of the object is computed.
         */
        virtual CFoundation::Vector2Du  ComputeSize();

    private:
        friend class ImageCreator;

        typedef std::map< State, Texture > StateTextureMap;
        typedef std::map< State, CFoundation::RectF16 > StateRectMap;

        /// Constructor
		Image( const CFoundation::String& _strName );

        /// Texture of each state
        StateTextureMap                 m_clTextures;

        /// Texture rectangle of each state
        StateRectMap                    m_mapTextureRects;
	};

    /// Image pointer
    typedef SharedPtr< Image > ImagePtr;

    /// Image const pointer
    typedef SharedPtr< const Image > ImageCPtr;

	class ImageCreator : public GuiObjectCreator, public CFoundation::Singleton<ImageCreator>
	{
	public:
		virtual const CFoundation::String GetTypeId() const;

		virtual GuiObjectPtr    CreateObject( const CFoundation::String &sName ) const;
		virtual void            InitDefault( GuiObjectPtr _spObject ) const;
		virtual void            InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const;

        //! Initializes an object from another
		virtual void            InitFromTemplate( GuiObjectPtr _spObject, GuiObjectCPtr _spTemplate ) const;

        /// Saves the passed object to a xml element
		virtual bool            SaveToXML( GuiObjectCPtr _spObject, CFoundation::XMLDocumentPtr _spDoc, CFoundation::XMLElementPtr _spXMLElement ) const;
	};
};

#endif
