#include "cgui/GUI/CtrlStyle.h"

#include "cfoundation/Debug/Assert.h"
#include "cfoundation/XML/XMLParser.h"

using namespace CGui;

CtrlStyle::CtrlStyle()
{
}

CtrlStyle::CtrlStyle( const CFoundation::String& _sXMLFilename )
{    
    CFoundation::XMLParser& clParser = CFoundation::XMLParser::GetInstance();
	m_spDocument = clParser.ParseFile( _sXMLFilename );
    if ( m_spDocument != NULL )
    {
        m_spElement = m_spDocument->GetRootElement();
    }
    else
    {
        CF_WARN( "CtrlStyle::CtrlStyle: File %s not found.", _sXMLFilename.c_str() );
    }
}

CtrlStyle::CtrlStyle( CFoundation::XMLElementCPtr _spElement )
:   m_spElement( _spElement )
{
}

const CtrlStyle& CtrlStyle::operator=( const CtrlStyle& _rhs )
{
    m_spElement = _rhs.m_spElement;
    return *this;
}

CtrlStyle::~CtrlStyle()
{
}

CFoundation::XMLElementCPtr CtrlStyle::GetRoot() const
{
    return m_spElement;
}
