#include "NiMain.h"

/*
===============================================================================

	TGMemory.cpp

	The global operator new and delete of the game: every C++ allocation goes
	to the pool manager, like the C allocations TGMemory.h routes there.

	Evidence: new-expressions in game code call 0x718CB0, which passes
	"UNKNOWN", 0 to the pool manager's allocator, and the standard library's
	allocator<T>::deallocate calls 0x718CF0, which passes its pointer to the
	pool manager's free.

===============================================================================
*/

/*
================
operator new
================
*/
// FUNCTION: STBC 0x00718cb0 ??2@YAPAXI@Z
void *operator new( size_t size ) {
	return g_kPoolManager.Unknown717840( size, TGMEMORY_FILE, TGMEMORY_LINE );
}

/*
================
operator delete
================
*/
// FUNCTION: STBC 0x00718cf0 ??3@YAXPAX@Z
void operator delete( void *block ) {
	g_kPoolManager.Unknown717960( block );
}

/*
===============================================================================

	The C allocation entry points: the Python units (and other C code built
	with the game's prelude) reach the pool manager through these, each
	passing its own file name for the TGMEMORY_DEBUG reports. Their names
	have no evidence; they have C linkage because C units call them.

===============================================================================
*/

/*
================
Unknown718D00

The C malloc of the pool manager, with the caller's file and line.
================
*/
// FUNCTION: STBC 0x00718d00
void *Unknown718D00( size_t size, const char *file, int line ) {
	return g_kPoolManager.Unknown717840( size, file, line );
}

/*
================
Unknown718D20

The C calloc of the pool manager, with the caller's file and line.
================
*/
// FUNCTION: STBC 0x00718d20
void *Unknown718D20( size_t count, size_t size, const char *file, int line ) {
	return g_kPoolManager.Unknown717920( count, size, file, line );
}

/*
================
Unknown718D40

The C realloc of the pool manager, with the caller's file and line.
================
*/
// FUNCTION: STBC 0x00718d40
void *Unknown718D40( void *block, size_t size, const char *file, int line ) {
	return g_kPoolManager.Unknown7179C0( block, size, file, line );
}

/*
================
Unknown718D60

The C malloc of the pool manager.
================
*/
// FUNCTION: STBC 0x00718d60
void *Unknown718D60( size_t size ) {
	return g_kPoolManager.Unknown717840( size, "UNKNOWN C malloc()", 0 );
}

/*
================
Unknown718D80

The C calloc of the pool manager.
================
*/
// FUNCTION: STBC 0x00718d80
void *Unknown718D80( size_t count, size_t size ) {
	return g_kPoolManager.Unknown717920( count, size, "UNKNOWN C calloc()", 0 );
}

/*
================
Unknown718DA0

The C realloc of the pool manager.
================
*/
// FUNCTION: STBC 0x00718da0
void *Unknown718DA0( void *block, size_t size ) {
	return g_kPoolManager.Unknown7179C0( block, size, "UNKNOWN C realloc()", 0 );
}

/*
================
Unknown718DC0

The C free of the pool manager.
================
*/
// FUNCTION: STBC 0x00718dc0
void Unknown718DC0( void *block ) {
	g_kPoolManager.Unknown717960( block );
}
