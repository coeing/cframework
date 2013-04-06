#ifndef CFOUNDATION_PROFILER_H
#define CFOUNDATION_PROFILER_H

#include <map>

#include "cfoundation/Base/Defines.h"
#include "cfoundation/Text/String.h"
#include "cfoundation/Utils/SharedPtr.h"
#include "cfoundation/Utils/Singleton.h"

namespace CFoundation
{
    INCLUDE_SHARED_CLASS( XMLDocument );
    INCLUDE_SHARED_CLASS( XMLElement );

	class Profiler : public Singleton< Profiler >
	{
	public:
		void StartProfiling( const CFoundation::String &sName );

        /// Ends 
		void EndProfiling();
		void FrameEnd();
		void Output();

	private:
		friend class Singleton<Profiler>;

        /// Constructor
		Profiler();

        /// Destructor
        ~Profiler();

		struct ProfilingElement;
			
		typedef std::map<CFoundation::String, ProfilingElement*> NameProfilingMap;

        /// Starts profiling for the passed element
		void    StartProfiling( ProfilingElement* _pElement );

        /// Adds the passed time to the current element
        void    AddPassedTime();

		void FrameEnd( NameProfilingMap& _clElements );
		void Output( const NameProfilingMap& _clElements, XMLElementPtr _spXMLElement );

		struct ProfilingElement
		{
			ProfilingElement( const CFoundation::String &sName )
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
			CFoundation::String		m_sName;

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
		
        /// Root element
        ProfilingElement        m_RootElement;

		ProfilingElement		*m_pCurElement;

		Unsigned64				m_u64CountFrames;

        //! XML document
        XMLDocumentPtr          m_spDocument;
	};

    class ProfilingHelper
    {
    public:
        /// Constructor
        ProfilingHelper( const String& _strName );

        /// Destructor
        ~ProfilingHelper();
    };
};

#endif
