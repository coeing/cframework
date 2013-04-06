#ifndef CMADTV_Advert_BLOCK_H
#define CMADTV_Advert_BLOCK_H

#include "cgui/Gui/Panel.h"
#include "cgui/Gui/Image.h"
#include "cgui/Gui/TextBox.h"

namespace CMadTV
{
    class Advert;
    typedef SharedPtr< Advert > AdvertPtr;

    class AdvertBlock;
    typedef SharedPtr< AdvertBlock > AdvertBlockPtr;

    class AdvertBlock
    {
    public:
        /// Creates a Advert block for the passed Advert
        static AdvertBlockPtr    Create( AdvertPtr _spAdvert );

        /// Constructor
        AdvertBlock( CGui::PanelPtr _spPnlRoot, AdvertPtr _spAdvert );

        /// Destructor
        ~AdvertBlock();

        /// Sets the advert
        void                SetAdvert( AdvertPtr _spAdvert );

        /// Returns the advert
        AdvertPtr           GetAdvert() const;

        /// Returns the panel of the block
        CGui::PanelPtr      GetPanel() const;

        /// Sets the block active
        void                SetActive( bool _bActive );

        /// Sets the sub text
        void                SetSubText( const CFoundation::String& _strText );

    private:
        /// Advert
        AdvertPtr           m_spAdvert;

        /// Panel
        CGui::PanelPtr      m_spPnlRoot;

        /// Background
        CGui::ImagePtr      m_spImgBackground;

        /// Name
        CGui::TextBoxPtr    m_spTbName;

        /// Sub text
        CGui::TextBoxPtr    m_spTbSubText;
    };
}

#endif
