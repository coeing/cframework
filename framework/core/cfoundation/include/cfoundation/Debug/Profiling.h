#ifndef CFOUNDATION_PROFILING_H
#define CFOUNDATION_PROFILING_H

//#include "Shiny.h"

#ifdef _PROFILING

#include "cfoundation/Debug/Profiler.h"

// Profiling
#define	InitProfiling()             { CFoundation::Profiler::GetInstance(); }
#define	DeinitProfiling()           { CFoundation::Profiler::DestroyInstance(); }
#define StartProfiling( sName )		{ CFoundation::Profiler::GetInstance().StartProfiling( sName ); }
#define	EndProfiling()		        { CFoundation::Profiler::GetInstance().EndProfiling(); }
#define ProfilingEndFrame()			{ CFoundation::Profiler::GetInstance().FrameEnd(); }
#define OutputProfiling()			{ CFoundation::Profiler::GetInstance().Output(); }

#define PROFILING2( _strName, _varName )    CFoundation::ProfilingHelper __Profiling##_varName ( _strName );
#define PROFILING1( _strName, _varName )    PROFILING2( _strName, _varName )
#define PROFILING( _strName )               PROFILING1( _strName, __LINE__ )

#else

// No Profiling
#define	InitProfiling()
#define	DeinitProfiling()
#define StartProfiling( sName )
#define	EndProfiling()
#define ProfilingEndFrame()
#define	OutputProfiling()

#define PROFILING2( _strName, _varName )
#define PROFILING1( _strName, _varName )
#define PROFILING( _strName )

#endif

#define PROFILE_FUNC()                      PROFILING( __FUNCTION__ )

#endif
