/*
===============================================================================

	NiSystem.h

	NetImmerse's platform services. The names follow the Gamebryo lineage
	(invented for this binary: no strings name them).

===============================================================================
*/

#ifndef __NISYSTEM_H__
#define __NISYSTEM_H__

#include "NiRTLib.h"

// the performance counter in seconds (QueryPerformanceCounter over
// QueryPerformanceFrequency)
float						NiGetCurrentTimeInSec();

// writes a string to the standard error stream (no callers; name invented)
void						NiUnknown86E3D0( const char *text );

// writes a string to the debugger
void						NiOutputDebugString( const char *text );

// case-insensitive string comparisons, as _stricmp and _strnicmp
int							NiStricmp( const char *s1, const char *s2 );
int							NiStrnicmp( const char *s1, const char *s2, unsigned int count );

// a message box with an OK button and no owner
void						NiMessageBox( const char *text, const char *title );

// rotates a value right by a bit count (no callers; Gamebryo 1.2.2: NiRotr)
unsigned int				NiRotr( unsigned int value, int shift );

#endif /* !__NISYSTEM_H__ */
