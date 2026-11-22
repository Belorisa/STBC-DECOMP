/*
===============================================================================

	TGTraceStream.h

	The trace stream's configuration: which trace types are shown, which are
	left out, and where the shown text goes. The configuration file is
	"TGTraceStream.cfg"; its lines are "Outputs:", "ShowType:" and
	"ExcludeType:" (a '#' starts a comment). An Outputs line lists the
	outputs, separated by commas: "DebugWindow", "Console", "LogFile(name)"
	and "PythonFunc(module:function)". The class name follows the file's;
	the member names and the output classes' names are invented. Nothing in
	retail writes to the outputs: the trace calls were compiled out of the
	release build, and the type test (0x722E70) has no caller.

	Evidence: 0x722CC0 opens the file through g_kSystemWrapper and parses
	it; the startup at 0x716007 calls it and the shutdown at 0x716095 jumps
	to 0x722E30, which deletes the outputs. The two NiTStringMap<bool>
	statics (0x99CC58 shown, 0x99CCD8 left out) share the guard byte
	0x99CC70, so they are class statics declared together.

	An output (TGTraceOutput, vtable 0x896BB0) has a scalar deleting
	destructor and a pure virtual that takes the text. The debug window's
	(0x896BA8) passes it to OutputDebugString, the console's (0x896BA0)
	drops it, the log file's (0x896BE0, 0xC bytes) writes it with fputs,
	and the Python function's (0x896BE8, 0x10 bytes) calls the function
	with it through PyEmbed::CallFunction.

===============================================================================
*/

#ifndef __TGTRACESTREAM_H__
#define __TGTRACESTREAM_H__

#include <stdio.h>

#include "NiTStringMap.h"

// Where shown trace text goes (invented name).
// FUNCTION: STBC 0x007236f0 ??_GTGTraceOutput@@UAEPAXI@Z @game/TGTraceStream
class TGTraceOutput {
public:
	// FUNCTION: STBC 0x00724310 ??1TGTraceOutput@@UAE@XZ @game/TGTraceOutput
	virtual				~TGTraceOutput( void ) { unknown4 = 0; }

	virtual void		Output( const char *text ) = 0;

protected:
	int					unknown4;		// 0x04 identity-TODO: only the destructor touches it (it clears it)
};

// OutputDebugString (invented name).
// FUNCTION: STBC 0x007231d0 ??_GTGDebugWindowOutput@@UAEPAXI@Z @game/TGTraceStream
// FUNCTION: STBC 0x00723210 ??1TGDebugWindowOutput@@UAE@XZ @game/TGTraceStream
class TGDebugWindowOutput : public TGTraceOutput {
public:
	virtual void		Output( const char *text );
};

// The text is dropped (invented name).
// FUNCTION: STBC 0x007231f0 ??_GTGConsoleOutput@@UAEPAXI@Z @game/TGTraceStream
// FUNCTION: STBC 0x00723220 ??1TGConsoleOutput@@UAE@XZ @game/TGTraceStream
class TGConsoleOutput : public TGTraceOutput {
public:
	virtual void		Output( const char *text );
};

// A file opened for writing (invented name).
// FUNCTION: STBC 0x00724120 ??_GTGLogFileOutput@@UAEPAXI@Z @game/TGTraceOutput
class TGLogFileOutput : public TGTraceOutput {
public:
						TGLogFileOutput( char *name );
	virtual				~TGLogFileOutput( void );

	virtual void		Output( const char *text );

protected:
	FILE *				file;			// 0x08 NULL when it couldn't be opened
};

// A Python function called with the text (invented name).
// FUNCTION: STBC 0x00724290 ??_GTGPythonFuncOutput@@UAEPAXI@Z @game/TGTraceOutput
class TGPythonFuncOutput : public TGTraceOutput {
public:
						TGPythonFuncOutput( char *module, char *function );
	virtual				~TGPythonFuncOutput( void );

	virtual void		Output( const char *text );

protected:
	char *				module;			// 0x08 owned copy
	char *				function;		// 0x0C owned copy
};

class TGTraceStream {
public:
	// Reads TGTraceStream.cfg (invented names).
	static void			LoadConfiguration( void );
	static void			DeleteOutputs( void );

	// False for a type outside the shown ones (when any are listed) or among
	// the left-out ones (invented name).
	static bool			IsTypeShown( const char *type );

protected:
	static void			SetOutputs( char *list );
	static void			AddShownType( char *type );
	static void			AddExcludedType( char *type );

	static NiTStringMap<bool>	shownTypes;
	static NiTStringMap<bool>	excludedTypes;

	static int			numOutputs;
	static TGTraceOutput **	outputs;
};

#endif /* !__TGTRACESTREAM_H__ */
