#include "cofiles/Profiler.h"
#include "cofiles/Time.h"
#include "cofiles/XML/XMLParser.h"

using namespace CoFiles;

Profiler::Profiler()
:	m_pCurElement( NULL ),
	m_u64CountFrames( 0 )
{
    m_spDocument = XMLParser::GetInstance().CreateDocument( "Profiling" );
}

void Profiler::StartProfiling( const CoFiles::String &sName )
{
    Unsigned64 u64Start = Time::GetTimeMs();

	ProfilingElement *pElement = NULL;

	NameProfilingMap *pElements = NULL;
	if( m_pCurElement != NULL )
	{
		pElements = &m_pCurElement->m_clElements;
	}
	else
	{
		pElements = &m_clElements;
	}

	NameProfilingMap::iterator it = pElements->find( sName );
	if( it != pElements->end() )
	{
		pElement = it->second;
	}
    else
	{
		// add new Element
		pElement = new ProfilingElement( sName );
		pElement->m_pParent = m_pCurElement;
		(*pElements)[ sName ] = pElement;
	}

	pElement->m_u64LastStart = pElement->m_u64LastEnd = u64Start;

	m_pCurElement = pElement;
}

void Profiler::EndProfiling()
{
    if ( m_pCurElement == NULL )
    {
        return;
    }

    /*
	ProfilingElement *pElement = NULL;
	NameProfilingMap *pElements = NULL;

	if( m_pCurElement->m_pParent != NULL )
	{
		pElements = &m_pCurElement->m_pParent->m_clElements;
	}
	else
	{
		pElements = &m_clElements;
	}

	NameProfilingMap::iterator it = pElements->find( sName );
	if( it != pElements->end() )
	{
		pElement = it->second;
	}*/

	m_pCurElement->m_u64LastEnd = Time::GetTimeMs();

    // Calculate durations
    Unsigned64 u64Duration = m_pCurElement->m_u64LastEnd - m_pCurElement->m_u64LastStart;
    m_pCurElement->m_u64MinDuration = std::min( m_pCurElement->m_u64MinDuration, u64Duration );
    m_pCurElement->m_u64MaxDuration = std::max( m_pCurElement->m_u64MaxDuration, u64Duration );

    m_pCurElement->m_u64DurationWhole += u64Duration;
	m_pCurElement->m_u64DurationFrame += u64Duration;

	++m_pCurElement->m_u64CountFrame;
	++m_pCurElement->m_u64CountWhole;

	m_pCurElement = m_pCurElement->m_pParent;
}

void Profiler::FrameEnd( NameProfilingMap &clElements )
{
	for( NameProfilingMap::iterator it = clElements.begin(); it != clElements.end(); ++it )
	{
		ProfilingElement *pElement = it->second;

		if( pElement == NULL )
		{
			continue;
		}

		pElement->m_u64DurationFrame = 0;
		pElement->m_u64CountFrame = 0;

		FrameEnd( pElement->m_clElements );
	}
}

void Profiler::FrameEnd()
{
	FrameEnd( m_clElements );

	++m_u64CountFrames;
}

void Profiler::Output( const NameProfilingMap &clElements, XMLElement& _clXMLElement )
{
	for ( NameProfilingMap::const_iterator it = clElements.begin(); it != clElements.end(); ++it )
	{
		ProfilingElement *pElement = it->second;

		if ( pElement == NULL )
		{
			continue;
		}

        // Create new XML element
        XMLElementPtr spXMLElement = m_spDocument->CreateElement( "Section" );
        _clXMLElement.InsertChildLast( *spXMLElement );

        Unsigned64 u64DurationParent = pElement->m_u64DurationWhole;
		if ( pElement->m_pParent != NULL )
		{
			u64DurationParent = pElement->m_pParent->m_u64DurationWhole;
		}

        spXMLElement->SetAttribute( "name", pElement->m_sName );
        spXMLElement->SetAttribute( "min", pElement->m_u64MinDuration );
        spXMLElement->SetAttribute( "max", pElement->m_u64MaxDuration );
        spXMLElement->SetAttribute( "average", pElement->m_u64DurationWhole * 1.0f / pElement->m_u64CountWhole );
        spXMLElement->SetAttribute( "whole", pElement->m_u64DurationWhole );
        spXMLElement->SetAttribute( "count", pElement->m_u64CountWhole );
        spXMLElement->SetAttribute( "percentage", pElement->m_u64DurationWhole * 100.0f / u64DurationParent );

		Output( pElement->m_clElements, *spXMLElement );
	}
}

void Profiler::Output()
{
    // Create new report node
    XMLElementPtr spElement = m_spDocument->CreateElement( "Report" );
    m_spDocument->GetRootElement()->InsertChildLast( *spElement );

	Output( m_clElements, *spElement );

    // Write to file
    XMLParser::GetInstance().WriteFile( m_spDocument, "Profiler.xml" );
}
