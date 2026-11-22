/*
===============================================================================

	TGTrace.h

	Trace messages: a TGTrace is built with a type mask and a printf
	format, and sends the text to the debugger's log or a log file when the
	options let the type through. With timing on, the text goes out when the
	message is destroyed, with the milliseconds it lived.

	The options come from "trace.opt": 32 lines whose first character is
	'1' or not, the bits of the outputs (TGTRACE_*), 32 more for the types
	let through, then the log file's name. Without the file every type goes
	to the debugger's log ("No TGTRACE options file (%s) found, I'd suggest
	you get one.").

	The names are invented (the class's after the message's TGTRACE). Only
	TGUIObject::Print (0x730F90) and TGModelContainer::ExtractModel
	(0x6C9520) build one. Evidence: the constructor (0x720370) stores the
	vtable 0x896B90 (one slot, Output 0x7208E0) and the type at 0x24, builds
	the text at 0x14 and asks Init (0x7203F0), which makes the options
	(0x24 bytes, vtable 0x896B9C, one slot: the scalar deleting destructor
	0x7209C0) on first use and reads them (0x7209E0, 0x720B90).

===============================================================================
*/

#ifndef __TGTRACE_H__
#define __TGTRACE_H__

#include <stdarg.h>
#include <stdio.h>
#include <sys/timeb.h>
#include <string>

// The outputs' bits (invented names).
#define TGTRACE_ENABLED			0x01
#define TGTRACE_TIMING			0x02		// the text goes out when the message is destroyed
#define TGTRACE_DEBUG_WINDOW	0x04
#define TGTRACE_LOG_FILE		0x08
#define TGTRACE_ALL_OUTPUTS		0x10		// both the debugger's log and the log file
#define TGTRACE_RAW				0x20		// the format is taken as it is

// The message types the callers pass (invented names).
#define TGTRACE_TYPE_MODEL		0x08		// TGModelContainer
#define TGTRACE_TYPE_UI			0x20		// TGUIObject::Print
#define TGTRACE_TYPE_DEBUG		0x40		// the debug dumps TGTRACE sends (invented)

// A trace message in a debug build; nothing at all in the release build
// (invented: retail keeps loops whose bodies compiled to nothing, which a
// macro empty in release explains). VC6 has no variadic macros, so the
// arguments, TGTrace's own, take a second pair of parentheses:
// TGTRACE( ( TGTRACE_TYPE_DEBUG, "%d\n", i ) ).
#ifdef _DEBUG
#define TGTRACE( args )			TGTrace args
#else
#define TGTRACE( args )
#endif

// The trace options, read once.
// FUNCTION: STBC 0x007209c0 ??_GTGTraceOptions@@UAEPAXI@Z
class TGTraceOptions {
public:
					TGTraceOptions( void );
	virtual			~TGTraceOptions( void );

	// Reads trace.opt and opens the log file (state 1), or gives up
	// (state -1).
	void			Load( void );
	bool			ReadOptions( std::string fileName );

	int				state;				// 0x04 0 not read yet, 1 on, -1 off
	FILE *			logFile;			// 0x08
	std::string		logFileName;		// 0x0C
	unsigned int	outputs;			// 0x1C TGTRACE_*
	unsigned int	types;				// 0x20 the types let through
};

class TGTrace {
public:
					TGTrace( unsigned int type, const char *format, ... );
					~TGTrace( void );

	virtual void	Output( const std::string &line );

protected:
	friend class TGTraceOptions;		// its destructor closes the shared options' file

	bool			Init( unsigned int type, const char *format );
	void			Format( std::string &out, const char *format, va_list args );
	void			Start( const std::string &line );
	int				Elapsed( struct _timeb from, struct _timeb to );

	struct _timeb	startTime;			// 0x04 with timing on
	bool			enabled;			// 0x10
	std::string		text;				// 0x14
	unsigned int	type;				// 0x24

	static TGTraceOptions *	options;
};

#endif /* !__TGTRACE_H__ */
