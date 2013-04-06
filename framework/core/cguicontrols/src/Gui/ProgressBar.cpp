#include "cgui/Gui/ProgressBar.h"

#include "cfoundation/Debug/Assert.h"
#include "cfoundation/XML/XMLElement.h"

#include "cgui/Gui/CtrlStyle.h"
#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/Image.h"

using namespace CGui;

ProgressBar::ProgressBar( const CFoundation::String& _strName )
:	CombinedCtrl( _strName ),
    m_f16Ratio( 1.0f )
{
}

ProgressBar::~ProgressBar()
{
}

const CFoundation::String ProgressBar::GetTypeID()
{
	return "progressbar";
}

const CFoundation::String ProgressBar::GetTypeId() const
{
	return ProgressBar::GetTypeID();
}

void ProgressBar::SetRatio( Float16 _f16Ratio )
{
    if ( _f16Ratio == m_f16Ratio )
    {
        return;
    }

    CF_WARN_IF( _f16Ratio < 0.0f || _f16Ratio > 1.0f, "Invalid progress bar ratio." );

    m_f16Ratio = _f16Ratio;
    UpdateBar();
}

Float16 ProgressBar::GetRatio() const
{
    return m_f16Ratio;
}
        
void ProgressBar::SetBar( GuiObjectPtr _spObjBar )
{
    if ( _spObjBar == m_spObjBar )
    {
        return;
    }

    m_spObjBar = _spObjBar;

    // Update initial size
    UpdateInitialSize();
}

void ProgressBar::UpdateInitialSize()
{
    // Get initial size
    if ( m_spObjBar != NULL )
    {
        m_vInitialSize = m_spObjBar->GetSize();
    }
    else
    {
        m_vInitialSize.Reset();
    }

    // Update bar
    UpdateBar();
}
        
void ProgressBar::UpdateBar()
{
    if ( m_spObjBar == NULL )
    {
        return;
    }

    // Update bar size
    CFoundation::Vector2Du vBarSize( static_cast< Unsigned32 >( m_vInitialSize.GetX() * m_f16Ratio ), m_vInitialSize.GetY() );
    m_spObjBar->SetCustomSize( vBarSize );
}

const CFoundation::String ProgressBarCreator::GetTypeId() const
{
	return ProgressBar::GetTypeID();
}

GuiObjectPtr ProgressBarCreator::CreateObject( const CFoundation::String& _strName ) const
{
	ProgressBarPtr spProgressBar( new ProgressBar( _strName ) );
    InitDefault( spProgressBar );
	return spProgressBar;
}
		
void ProgressBarCreator::InitDefault( GuiObjectPtr _spObject ) const
{
    CombinedCtrlCreator::InitDefault( _spObject );

	ProgressBarPtr spProgressBar = static_pointer_cast< ProgressBar >( _spObject );

    /*
    // Create background image
    ImagePtr spImgBg = GuiMain::GetInstance().CreateDefault< Image >( "theBg" );
    spProgressBar->AddPieceLast( spImgBg );
    spProgressBar->m_spImgBg = spImgBg;

    // Create bar image
    ImagePtr spImgBar = GuiMain::GetInstance().CreateDefault< Image >( "theBar" );
    spProgressBar->AddPieceLast( spImgBar );
    spProgressBar->SetBar( spImgBar );

    // Create foreground image
    ImagePtr spImgFg = GuiMain::GetInstance().CreateDefault< Image >( "theFg" );
    spProgressBar->AddPieceLast( spImgFg );
    spProgressBar->m_spImgFg = spImgFg;*/
}

void ProgressBarCreator::InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const
{
	CombinedCtrlCreator::InitFromXML( _spObject, _spXMLElement );

	ProgressBarPtr spProgressBar = static_pointer_cast< ProgressBar >( _spObject );

    // Get controls
    spProgressBar->SetBar( spProgressBar->FindChild( "theBar" ) );

    // Get initial ratio
    if ( _spXMLElement->HasAttribute( "initial_ratio" ) )
    {
        spProgressBar->SetRatio( _spXMLElement->GetAttributeAsFloat16( "initial_ratio" ) );
    }

    // Read child infos    
	CFoundation::XMLNodePtr spChild = _spXMLElement->GetFirstChild();
	while ( spChild != NULL )
	{
		if ( spChild->IsElement() )
		{
            CFoundation::XMLElementPtr spElement = static_pointer_cast< CFoundation::XMLElement >( spChild );

			const CFoundation::String& strName = spElement->GetName();
                        
            /*
		    // Setup background
		    if ( strName == "background" )
		    {
                spProgressBar->m_spImgBg->SetStyle( CtrlStylePtr( new CtrlStyle( spElement ) ) );
		    }

		    // Setup bar
		    if ( strName == "bar" )
		    {
                spProgressBar->m_spImgBar->SetStyle( CtrlStylePtr( new CtrlStyle( spElement ) ) );
                spProgressBar->UpdateInitialSize();
		    }

		    // Setup foreground
		    if ( strName == "foreground" )
		    {
                spProgressBar->m_spImgFg->SetStyle( CtrlStylePtr( new CtrlStyle( spElement ) ) );
		    }*/
		}
			
        spChild = spChild->GetNextSibling();
	}
}
		
void ProgressBarCreator::InitFromTemplate( GuiObjectPtr _spObject, GuiObjectCPtr _spTemplate ) const
{
	CombinedCtrlCreator::InitFromTemplate( _spObject, _spTemplate );

	ProgressBarPtr spProgressBar = static_pointer_cast< ProgressBar >( _spObject );
	ProgressBarCPtr spTemplate = static_pointer_cast< const ProgressBar >( _spTemplate );
}
