/*
===============================================================================

	TGProfilingInfo.h

	The game's profiler: named timings that are started and stopped, with
	the time of each recorded. Only what the SWIG wrappers TGProfilingInfo_*
	call is declared, and the members Enable and DisableProfiling use. An
	instance is 4 bytes: the ID of the timing it runs.

	Evidence: EnableProfiling (0x718F10) reads the counter's frequency and
	starts a timing named "Total profiling time" (the string at 0x95C614),
	which DisableProfiling (0x718F60) stops. StartTiming (0x718F80) is
	invented: it returns the ID that StopTiming takes. The three statics
	are invented names.

===============================================================================
*/

#ifndef __TGPROFILINGINFO_H__
#define __TGPROFILINGINFO_H__

#include "NiTList.h"
#include "NiTStringMap.h"

// The samples of one name: the milliseconds each took and the frame it ended
// in, in two lists of 512-item blocks (8 bytes; the name is invented).
struct TGTimingSamples {
	NiTList<float> *	times;			// 0x00 owned
	// The element type is inferred: StopTiming's two NewItem copies (0x719E80
	// for the times, 0x719F40 for the frames) are the unit's own, so neither
	// is NiTList<long>'s (0x434810) or NiTList<int>'s (0x486880), which units
	// linked earlier define; SaveRawData prints a frame with %ld.
	NiTList<unsigned long> *	frames;	// 0x04 owned
};

// A timing that has started: the frame, the counter at the start and the
// samples of its name (0x18 bytes; the name is invented).
struct TGTiming {
	int					frame;			// 0x00 the frame the timing started in
	int					unknown04;		// 0x04
	__int64				start;			// 0x08 QueryPerformanceCounter at the start (a LARGE_INTEGER; the header keeps windows.h out)
	TGTimingSamples *	samples;		// 0x10
	int					unknown14;		// 0x14
};

class TGProfilingInfo {
public:
	// An instance times its own scope: it starts a timing of the name and
	// stops it when it goes (the SWIG wrappers new_ and delete_TGProfilingInfo
	// expand both).
					TGProfilingInfo( char *name ) { timingID = StartTiming( name ); }
					~TGProfilingInfo( void ) { StopTiming( timingID ); }

	static void	EnableProfiling( void );
	static void	DisableProfiling( void );
	static void	StopTiming( long );
	static void	SetTimingData( char *, float );
	static bool	SaveRawData( char * );
	static void	Terminate( void );

	static long	StartTiming( char *name );		// invented name

protected:
	long			timingID;			// 0x00 the instance's timing (invented name)

	static bool		isEnabled;			// 0x99C5F4 set once the counter's frequency is known
	static double	ticksPerSecond;		// 0x95C608
	static long		totalTiming;		// 0x99C5F8 the timing of the whole run
	static long		nextTimingID;		// 0x95C610 StartTiming's next ID, from 1
	static NiTStringMap<TGTimingSamples *>	timingsByName;	// 0x99C4E8 307 buckets; owns the samples
	static NiTMap<long, TGTiming *>	timingsByID;	// 0x99C530 47 buckets; the timings in progress
};

#endif /* !__TGPROFILINGINFO_H__ */
