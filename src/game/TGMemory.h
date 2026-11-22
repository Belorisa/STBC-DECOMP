/*
===============================================================================

	TGMemory.h

	The macros that route the C allocation calls of game code through the
	pool manager. The SWIG App unit sees them before its first line (it is
	compiled with /FITGMemory.h): the SWIG runtime's own malloc and free
	calls already go to g_kPoolManager.

	Evidence: game code calls malloc and free as thiscalls on g_kPoolManager
	(0x99C478), passing a file name and a line number ("UNKNOWN", 0 in this
	release build). The C runtime's malloc/realloc/free overrides at
	0x718D60.. call the same methods with "UNKNOWN C malloc()" and so on. The
	string "Define TGMEMORY_DEBUG if you want to see mem-info." names the
	module; the file name is invented.

===============================================================================
*/

#ifndef __TGMEMORY_H__
#define __TGMEMORY_H__

// The CRT declarations must be seen before the macros below rename the calls.
#include <stdlib.h>
#include <malloc.h>

#include "TGPoolManager.h"

// The file name and line number of an allocation; a TGMEMORY_DEBUG build
// presumably passed __FILE__ and __LINE__.
#define TGMEMORY_FILE		"UNKNOWN"
#define TGMEMORY_LINE		0

// The C allocation entry points of the pool manager (TGMemory.cpp); C code
// built with the game's prelude calls these.
#ifdef __cplusplus
extern "C" {
#endif
// The same with the caller's file name and line number; nothing in retail
// calls them (presumably the TGMEMORY_DEBUG build's).
void *	Unknown718D00( size_t size, const char *file, int line );
void *	Unknown718D20( size_t count, size_t size, const char *file, int line );
void *	Unknown718D40( void *block, size_t size, const char *file, int line );
void *	Unknown718D60( size_t size );
void *	Unknown718D80( size_t count, size_t size );
void *	Unknown718DA0( void *block, size_t size );
void	Unknown718DC0( void *block );
#ifdef __cplusplus
}
#endif

#define malloc( size )		g_kPoolManager.Unknown717840( ( size ), TGMEMORY_FILE, TGMEMORY_LINE )
#define calloc( count, size )	g_kPoolManager.Unknown717920( ( count ), ( size ), TGMEMORY_FILE, TGMEMORY_LINE )
#define realloc( block, size )	g_kPoolManager.Unknown7179C0( ( block ), ( size ), TGMEMORY_FILE, TGMEMORY_LINE )
#define free( block )		g_kPoolManager.Unknown717960( block )

#endif /* !__TGMEMORY_H__ */
