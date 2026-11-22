/*
===============================================================================

	AILog.h

	The AI tree log: while it is open ("AITree.txt"), each AI's update
	and state change opens a timed scope named by a formatted text, and
	the scopes nest into a tree that is written to the file. A local
	AILog is one scope: the constructor opens it, the destructor closes
	it. The class and member names are invented; the Python module the
	log also reports to is "AILogging".

	Evidence: 0x4747E0 closes the open log, then opens the file it is
	given for writing into 0x981ADC (it refuses an existing file other
	than "AITree.txt", tested with 0x71AA80), imports "AILogging" and
	keeps its second argument;ArtificialIntelligence::LogAITree calls it. The constructor
	(0x474540, a varargs cdecl taking the object first) does nothing while
	0x981ADC is NULL, else makes a 0x40-byte scope record under the
	current one (0x981AE4) with the formatted text and the time; the
	destructor (0x474700) closes the current record. The scope object
	itself holds nothing.

	A record (AILogScope, 0x40 bytes) holds its text (a heap block
	strlen( format ) + 0x80 long, filled by _vsnprintf), its parent, the
	records opened under it, the times it was closed (0x10-byte entries:
	the update number and the seconds since it opened, kept while the
	flag at 0x981AE8 is set) and the time it opened
	(TGSystemWrapperClass::GetTimeSinceFrameStart). AIManager::RemoveAI opens "Ship(%s)::UpdateAI";
	the AIs open "PlainAI(%s)::Update" and the like.

===============================================================================
*/

#ifndef __AILOG_H__
#define __AILOG_H__

#include <stdio.h>

#include "NiTList.h"
#include "NiTStringMap.h"
#include "Python.h"
#include "TGSystemWrapperClass.h"

// One closing of a record (invented names).
struct AILogTiming {
	int				updateNumber;			// 0x00 g_kSystemWrapper's update number
	double			elapsed;				// 0x08 seconds since the record opened
};

// One scope of the tree (invented names).
struct AILogScope {
					AILogScope( void ) : text( NULL ), parent( NULL ), children( 2 ), timings( 2 ) { start = g_kSystemWrapper.GetTimeSinceFrameStart(); }

	char *			text;					// 0x00 owned
	AILogScope *	parent;					// 0x04 NULL for a root
	NiTList<AILogScope *>	children;		// 0x08 owned, in opening order
	NiTList<AILogTiming *>	timings;		// 0x20 owned
	double			start;					// 0x38 GetTimeSinceFrameStart when it opened

					~AILogScope( void );

	bool			SameTree( AILogScope *other );
	void			Merge( AILogScope *newer );
	void			Write( int depth );
	void			Report( int depth, PyObject *list );
};

class AILog {
public:
				AILog( char *format, ... );
				~AILog( void );

	// Closes the open log and opens the named file; NULL closes it only.
	// false when no log is open after.
	static bool	Open( char *fileName, bool unknown );

	static bool	IsOpen( void ) { return file != NULL; }

	// Hands a finished root scope over to the log (invented name).
	void		CloseRoot( AILogScope *root );

protected:
	friend struct AILogScope;

	static FILE *	file;			// 0x981ADC the open log, NULL when off
	static bool		reportToPython;	// 0x981AE0 AILogging was importable when the log opened (invented name)
	static bool		keepTimings;	// 0x981AE8 the second argument of Open (invented name)
	static NiTStringMap<AILogScope *>	rootsByText;	// 0x9819B8 the finished trees kept, by their text (invented name)
	static AILogScope *	current;	// 0x981AE4 the innermost open record (invented name)
};

#endif /* !__AILOG_H__ */
