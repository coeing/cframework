#ifndef CMADTV_BROADCAST_BLOCK_H
#define CMADTV_BROADCAST_BLOCK_H

#include "cgui/Gui/Panel.h"
#include "cgui/Gui/Image.h"
#include "cgui/Gui/TextBox.h"

namespace CMadTV
{
    class Broadcast;
    typedef SharedPtr< Broadcast > BroadcastPtr;

    class BroadcastBlock;
    typedef SharedPtr< BroadcastBlock > BroadcastBlockPtr;

    class BroadcastBlock
    {
    public:
        /// Creates a broadcast block for the passed broadcast
        static BroadcastBlockPtr    Create( BroadcastPtr _spBroadcast );

        /// Constructor
        BroadcastBlock( CGui::PanelPtr _spPnlRoot, BroadcastPtr _spBroadcast );

        /// Destructor
        ~BroadcastBlock();

        /// Returns the broadcast
        BroadcastPtr        GetBroadcast() const;

        /// Returns the panel of the block
        CGui::PanelPtr      GetPanel() const;

        /// Sets the block active
        void                SetActive( bool _bActive );

        /// Computes the block number at the passed position of the panel
        Unsigned32          ComputeBlock( const CFoundation::Vector2Di& _vPosition ) const;

    private:
        /// Broadcast
        BroadcastPtr        m_spBroadcast;

        /// Panel
        CGui::PanelPtr      m_spPnlRoot;

        /// Background
        CGui::ImagePtr      m_spImgBackground;

        /// Title
        CGui::TextBoxPtr    m_spTbTitle;
    };
}

#endif
