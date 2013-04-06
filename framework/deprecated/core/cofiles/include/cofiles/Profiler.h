#ifndef COFILES_PROFILER_H
#define COFILES_PROFILER_H

#include <map>

#include "cofiles/Defines.h"
#include "cofiles/Singleton.h"
#include "cofiles/String.h"
#include "cofiles/Utils/SharedPtr.h"

namespace CoFiles
{
    class XMLDocument;
    typedef SharedPtr< XMLDocument > XMLDocumentPtr;
    class XMLElement;

	class Profiler : public Singleton< Profiler >
	{
	public:
		void StartProfiling( const CoFiles::String &sName );
		void EndProfiling();
		void FrameEnd();
		void Output();

	private:
		friend class Singleton<Profiler>;
		Profiler();

		struct ProfilingElement;
			
		typedef std::map<CoFiles::String, ProfilingElement*> NameProfilingMap;

		void FrameEnd( NameProfilingMap& _clElements );
		void Output( const NameProfilingMap& _clElements, XMLElement& _clXMLElement );

		struct ProfilingElement
		{
			ProfilingElement( const CoFiles::String &sName )
			:	m_sName( sName ),
				m_u64LastStart( 0 ),
				m_u64LastEnd( 0 ),
                m_u64DurationWhole( 0 ),
                m_u64DurationFrame( 0 ),
                m_u64MinDuration( MAX_U64 ),
                m_u64MaxDuration( 0 ),
				m_u64CountWhole( 0 ),
				m_u64CountFrame( 0 )
			{
			}

            /// Parent element
			ProfilingElement	*m_pParent;
			
            /// Child elements
			NameProfilingMap	m_clElements;

            /// Name
			CoFiles::String		m_sName;

            /// Time last start
			Unsigned64			m_u64LastStart;

            /// Time last end
			Unsigned64			m_u64LastEnd;

            /// Whole duration (in ms)
			Unsigned64		    m_u64DurationWhole;

            /// Duration in current frame (in ms)
			Unsigned64			m_u64DurationFrame;

            /// Minimum duration (in ms)
            Unsigned64          m_u64MinDuration;

            /// Maximum duration (in ms)
            Unsigned64          m_u64MaxDuration;

            /// Whole calls
			Unsigned64			m_u64CountWhole;

            /// Calls in current frame
			Unsigned64			m_u64CountFrame;
		};
		
		NameProfilingMap		m_clElements;

		ProfilingElement		*m_pCurElement;

		Unsigned64				m_u64CountFrames;

        //! XML document
        XMLDocumentPtr          m_spDocument;
	};
};

#endif
