/*
===============================================================================

	TGTrace.cpp

	Trace messages and their options (the names are invented; see
	TGTrace.h).

===============================================================================
*/

// <iostream>'s statics: the ios_base::Init and _Winit quads.
// FUNCTION: STBC 0x007213e0 _$E32
// FUNCTION: STBC 0x007213f0 _$E29
// FUNCTION: STBC 0x00721400 _$E31
// FUNCTION: STBC 0x00721410 _$E30
// FUNCTION: STBC 0x00721420 _$E38
// FUNCTION: STBC 0x00721430 _$E35
// FUNCTION: STBC 0x00721440 _$E37
// FUNCTION: STBC 0x00721450 _$E36
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "TGTrace.h"
#include "TGSystemWrapperClass.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00721540 _$E409
// FUNCTION: STBC 0x00721560 _$E408

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00721460 _$E46
// FUNCTION: STBC 0x00721470 _$E45
// FUNCTION: STBC 0x00721480 _$E50
// FUNCTION: STBC 0x00721490 _$E49
// FUNCTION: STBC 0x007214a0 _$E61
// FUNCTION: STBC 0x007214b0 _$E60
// FUNCTION: STBC 0x007214c0 _$E65
// FUNCTION: STBC 0x007214d0 _$E64
// FUNCTION: STBC 0x007214e0 _$E69
// FUNCTION: STBC 0x007214f0 _$E68
// FUNCTION: STBC 0x00721500 _$E73
// FUNCTION: STBC 0x00721510 _$E72
// FUNCTION: STBC 0x00721520 _$E197
// FUNCTION: STBC 0x00721530 _$E196
// FUNCTION: STBC 0x00721570 _$E139
// FUNCTION: STBC 0x00721580 _$E138
// FUNCTION: STBC 0x00721590 _$E152
// FUNCTION: STBC 0x007215a0 _$E151
// FUNCTION: STBC 0x007215b0 _$E202
// FUNCTION: STBC 0x007215c0 _$E201
// FUNCTION: STBC 0x007215d0 _$E207
// FUNCTION: STBC 0x007215e0 _$E206
// FUNCTION: STBC 0x007215f0 _$E211
// FUNCTION: STBC 0x00721600 _$E210
// FUNCTION: STBC 0x00721610 _$E215
// FUNCTION: STBC 0x00721620 _$E214
// FUNCTION: STBC 0x00721630 _$E222
// FUNCTION: STBC 0x00721640 _$E221
// FUNCTION: STBC 0x00721650 _$E247
// FUNCTION: STBC 0x00721660 _$E246
// FUNCTION: STBC 0x00721670 _$E251
// FUNCTION: STBC 0x00721680 _$E250
// FUNCTION: STBC 0x00721690 _$E97
// FUNCTION: STBC 0x007216a0 _$E96
// FUNCTION: STBC 0x007216b0 _$E233
// FUNCTION: STBC 0x007216c0 _$E232
// FUNCTION: STBC 0x007216d0 _$E240
// FUNCTION: STBC 0x007216e0 _$E239
// FUNCTION: STBC 0x007216f0 _$E263
// FUNCTION: STBC 0x00721700 _$E262
// FUNCTION: STBC 0x00721710 _$E267
// FUNCTION: STBC 0x00721720 _$E266
// FUNCTION: STBC 0x00721730 _$E307
// FUNCTION: STBC 0x00721740 _$E306
// FUNCTION: STBC 0x00721750 _$E313
// FUNCTION: STBC 0x00721760 _$E312
// FUNCTION: STBC 0x00721770 _$E255
// FUNCTION: STBC 0x00721780 _$E254
// FUNCTION: STBC 0x00721790 _$E259
// FUNCTION: STBC 0x007217a0 _$E258
// FUNCTION: STBC 0x007217b0 _$E271
// FUNCTION: STBC 0x007217c0 _$E270
// FUNCTION: STBC 0x007217d0 _$E275
// FUNCTION: STBC 0x007217e0 _$E274
// FUNCTION: STBC 0x007217f0 _$E279
// FUNCTION: STBC 0x00721800 _$E278
// FUNCTION: STBC 0x00721810 _$E283
// FUNCTION: STBC 0x00721820 _$E282
// FUNCTION: STBC 0x00721830 _$E287
// FUNCTION: STBC 0x00721840 _$E286
// FUNCTION: STBC 0x00721850 _$E291
// FUNCTION: STBC 0x00721860 _$E290
// FUNCTION: STBC 0x00721870 _$E295
// FUNCTION: STBC 0x00721880 _$E294
// FUNCTION: STBC 0x00721890 _$E299
// FUNCTION: STBC 0x007218a0 _$E298
// FUNCTION: STBC 0x007218b0 _$E102
// FUNCTION: STBC 0x007218c0 _$E101
// FUNCTION: STBC 0x007218d0 _$E303
// FUNCTION: STBC 0x007218e0 _$E302
// FUNCTION: STBC 0x007218f0 _$E107
// FUNCTION: STBC 0x00721900 _$E106
// FUNCTION: STBC 0x00721910 _$E317
// FUNCTION: STBC 0x00721920 _$E316
// FUNCTION: STBC 0x00721930 _$E321
// FUNCTION: STBC 0x00721940 _$E320
// FUNCTION: STBC 0x00721950 _$E325
// FUNCTION: STBC 0x00721960 _$E324
// FUNCTION: STBC 0x00721970 _$E329
// FUNCTION: STBC 0x00721980 _$E328
// FUNCTION: STBC 0x00721990 _$E116
// FUNCTION: STBC 0x007219a0 _$E115
// FUNCTION: STBC 0x007219b0 _$E333
// FUNCTION: STBC 0x007219c0 _$E332
// FUNCTION: STBC 0x007219d0 _$E337
// FUNCTION: STBC 0x007219e0 _$E336
// FUNCTION: STBC 0x007219f0 _$E341
// FUNCTION: STBC 0x00721a00 _$E340
// FUNCTION: STBC 0x00721a10 _$E122
// FUNCTION: STBC 0x00721a20 _$E121
// FUNCTION: STBC 0x00721a30 _$E127
// FUNCTION: STBC 0x00721a40 _$E126
// FUNCTION: STBC 0x00721a50 _$E157
// FUNCTION: STBC 0x00721a60 _$E156
// FUNCTION: STBC 0x00721a70 _$E345
// FUNCTION: STBC 0x00721a80 _$E344
// FUNCTION: STBC 0x00721a90 _$E162
// FUNCTION: STBC 0x00721aa0 _$E161
// FUNCTION: STBC 0x00721ab0 _$E349
// FUNCTION: STBC 0x00721ac0 _$E348
// FUNCTION: STBC 0x00721ad0 _$E353
// FUNCTION: STBC 0x00721ae0 _$E352
// FUNCTION: STBC 0x00721af0 _$E167
// FUNCTION: STBC 0x00721b00 _$E166
// FUNCTION: STBC 0x00721b10 _$E357
// FUNCTION: STBC 0x00721b20 _$E356
// FUNCTION: STBC 0x00721b30 _$E132
// FUNCTION: STBC 0x00721b40 _$E131
// FUNCTION: STBC 0x00721b50 _$E361
// FUNCTION: STBC 0x00721b60 _$E360
// FUNCTION: STBC 0x00721b70 _$E365
// FUNCTION: STBC 0x00721b80 _$E364
// FUNCTION: STBC 0x00721b90 _$E172
// FUNCTION: STBC 0x00721ba0 _$E171
// FUNCTION: STBC 0x00721bb0 _$E177
// FUNCTION: STBC 0x00721bc0 _$E176
// FUNCTION: STBC 0x00721bd0 _$E369
// FUNCTION: STBC 0x00721be0 _$E368
// FUNCTION: STBC 0x00721bf0 _$E373
// FUNCTION: STBC 0x00721c00 _$E372
// FUNCTION: STBC 0x00721c10 _$E377
// FUNCTION: STBC 0x00721c20 _$E376
// FUNCTION: STBC 0x00721c30 _$E381
// FUNCTION: STBC 0x00721c40 _$E380
// FUNCTION: STBC 0x00721c50 _$E385
// FUNCTION: STBC 0x00721c60 _$E384
// FUNCTION: STBC 0x00721c70 _$E389
// FUNCTION: STBC 0x00721c80 _$E388
// FUNCTION: STBC 0x00721c90 _$E393
// FUNCTION: STBC 0x00721ca0 _$E392
// FUNCTION: STBC 0x00721cb0 _$E397
// FUNCTION: STBC 0x00721cc0 _$E396
// FUNCTION: STBC 0x00721cd0 _$E182
// FUNCTION: STBC 0x00721ce0 _$E181
// FUNCTION: STBC 0x00721cf0 _$E401
// FUNCTION: STBC 0x00721d00 _$E400
// FUNCTION: STBC 0x00721d10 _$E187
// FUNCTION: STBC 0x00721d20 _$E186
// FUNCTION: STBC 0x00721d30 _$E192
// FUNCTION: STBC 0x00721d40 _$E191

// The size of a formatted message's buffer.
// DATA: STBC 0x0095cc88
static int traceBufferSize = 0x800;

// DATA: STBC 0x0099cb00
TGTraceOptions *TGTrace::options;

/*
================
TGTrace::TGTrace

Formats the text when the options let the type through.
================
*/
// FUNCTION: STBC 0x00720370
TGTrace::TGTrace( unsigned int type_, const char *format, ... ) {
	type = type_;
	if ( Init( type_, format ) ) {
		va_list args;
		va_start( args, format );
		Format( text, format, args );
		Start( text );
	}
}

/*
================
TGTrace::Init

Reads the options on first use; true when the type goes out.
================
*/
// FUNCTION: STBC 0x007203f0
bool TGTrace::Init( unsigned int type_, const char *format ) {
	enabled = false;
	if ( options == NULL ) {
		options = new TGTraceOptions;
	}

	if ( options->state != -1 && format != NULL ) {
		if ( options->state == 0 ) {
			options->Load();
		}
		if ( options->state == 1 && ( type_ == 0 || ( type_ & options->types ) != 0 ) ) {
			enabled = true;
		}
	}
	return enabled;
}

/*
================
TGTrace::Format
================
*/
// FUNCTION: STBC 0x007204a0
void TGTrace::Format( std::string &out, const char *format, va_list args ) {
	if ( options->outputs & TGTRACE_RAW ) {
		out += format;
		return;
	}

	char buffer[0x800];
	buffer[0] = '\0';
	_vsnprintf( buffer, traceBufferSize - 1, format, args );
	out = buffer;
}

/*
================
TGTrace::Start

With timing on the clock starts, otherwise the text goes out now.
================
*/
// FUNCTION: STBC 0x00720680
void TGTrace::Start( const std::string &line ) {
	if ( options->outputs & TGTRACE_TIMING ) {
		_ftime( &startTime );
		return;
	}
	Output( text );
}

/*
================
TGTrace::~TGTrace

With timing on the text goes out now, with the milliseconds since it was
made.
================
*/
// FUNCTION: STBC 0x007206b0
TGTrace::~TGTrace( void ) {
	if ( enabled && ( options->outputs & TGTRACE_TIMING ) ) {
		struct _timeb now;
		_ftime( &now );

		char elapsed[0x400];
		sprintf( elapsed, "...%d", Elapsed( startTime, now ) );

		std::string line( text );
		line += elapsed;
		line += "\n";
		Output( line );
	}
}

/*
================
TGTrace::Output
================
*/
// FUNCTION: STBC 0x007208e0
void TGTrace::Output( const std::string &line ) {
	if ( ( options->outputs & TGTRACE_DEBUG_WINDOW ) || ( options->outputs & TGTRACE_ALL_OUTPUTS ) ) {
		OutputDebugString( line.c_str() );
	}
	if ( ( ( options->outputs & TGTRACE_LOG_FILE ) || ( options->outputs & TGTRACE_ALL_OUTPUTS ) ) && options->logFile != NULL ) {
		fwrite( line.c_str(), line.length(), 1, options->logFile );
		fflush( options->logFile );
	}
}

/*
================
TGTrace::Elapsed

The milliseconds from one time to the other.
================
*/
// FUNCTION: STBC 0x00720950
int TGTrace::Elapsed( struct _timeb from, struct _timeb to ) {
	return ( to.time - from.time ) * 1000 - from.millitm + to.millitm;
}

/*
================
TGTraceOptions::TGTraceOptions
================
*/
// FUNCTION: STBC 0x00720990
TGTraceOptions::TGTraceOptions( void ) {
	state = 0;
	outputs = 0;
	types = 0;
	logFile = NULL;
}

/*
================
TGTraceOptions::Load
================
*/
// FUNCTION: STBC 0x007209e0
void TGTraceOptions::Load( void ) {
	state = -1;
	if ( !ReadOptions( std::string( "trace.opt" ) ) ) {
		return;
	}
	if ( !( outputs & TGTRACE_ENABLED ) ) {
		return;
	}

	if ( !( outputs & TGTRACE_DEBUG_WINDOW ) ) {
		if ( !( outputs & ( TGTRACE_LOG_FILE | TGTRACE_ALL_OUTPUTS ) ) ) {
			return;
		}
		if ( logFileName.length() == 0 ) {
			return;
		}
		logFile = g_kSystemWrapper.FileOpen( (char *)logFileName.c_str(), "w" );
		if ( logFile == NULL ) {
			return;
		}
	}
	state = 1;
}

/*
================
TGTraceOptions::~TGTraceOptions

Closes the log file.
================
*/
// FUNCTION: STBC 0x00720ab0
TGTraceOptions::~TGTraceOptions( void ) {
	if ( TGTrace::options->state == 1 ) {
		if ( TGTrace::options->logFile != NULL ) {
			g_kSystemWrapper.FileClose( TGTrace::options->logFile );
			TGTrace::options->logFile = NULL;
		}
		TGTrace::options->state = -1;
	}
}

/*
================
TGTraceOptions::ReadOptions

Without the file everything goes to the debugger's log, and app.log is
the log file's name.
================
*/
// FUNCTION: STBC 0x00720b90
bool TGTraceOptions::ReadOptions( std::string fileName ) {
	char line[0x400];
	char typeLine[0x400];
	bool bits[32];
	int i;

	FILE *file = g_kSystemWrapper.FileOpen( (char *)fileName.c_str(), "r" );
	if ( file == NULL ) {
		outputs = TGTRACE_ENABLED | TGTRACE_DEBUG_WINDOW;
		types = 0xFFFFFFFF;
		logFileName = "app.log";

		sprintf( line, "No TGTRACE options file (%s) found, I'd suggest you get one.\n", fileName.c_str() );
		OutputDebugString( line );
		return true;
	}

	unsigned int mask = 0;
	// the bit tests are written without braces: the braces' scopes would cost the
	// inline budget that leaves basic_string::_Grow out of line at the last assign
	for ( i = 0; i < 32; i++ ) {
		fgets( line, sizeof( line ), file );
		if ( line[0] == '1' )
			bits[i] = true;
		else
			bits[i] = false;
	}
	for ( i = 0; i < 32; i++ ) {
		if ( bits[i] ) {
			mask |= 1 << i;
		}
	}
	outputs = mask;

	mask = 0;
	for ( i = 0; i < 32; i++ ) {
		fgets( typeLine, sizeof( typeLine ), file );
		if ( typeLine[0] == '1' )
			bits[i] = true;
		else
			bits[i] = false;
	}
	for ( i = 0; i < 32; i++ ) {
		if ( bits[i] ) {
			mask |= 1 << i;
		}
	}
	types = mask;

	fgets( line, sizeof( line ), file );
	while ( line[strlen( line ) - 1] == '\r' || line[strlen( line ) - 1] == '\n' ) {
		line[strlen( line ) - 1] = '\0';
	}
	logFileName = line;

	g_kSystemWrapper.FileClose( file );
	return true;
}
