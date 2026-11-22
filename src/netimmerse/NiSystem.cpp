// NiSystem.cpp -- platform services

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include "NiRTLib.h"
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "NiSystem.h"
#include "NiFile.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x0086e4d0 _$E33
// FUNCTION: STBC 0x0086e4f0 _$E17
// FUNCTION: STBC 0x0086e510 _$E15
// FUNCTION: STBC 0x0086e520 _$E23
// FUNCTION: STBC 0x0086e540 _$E21

/*
================
NiGetCurrentTimeInSec
================
*/
// FUNCTION: STBC 0x86E3A0
float NiGetCurrentTimeInSec() {
	LARGE_INTEGER frequency;
	LARGE_INTEGER counter;
	QueryPerformanceFrequency( &frequency );
	QueryPerformanceCounter( &counter );
	return (float)counter.QuadPart / (float)frequency.QuadPart;
}

/*
================
NiUnknown86E3D0
================
*/
// FUNCTION: STBC 0x86E3D0
void NiUnknown86E3D0( const char *text ) {
	std::cerr << text;
}

/*
================
NiOutputDebugString
================
*/
// FUNCTION: STBC 0x86E3F0
void NiOutputDebugString( const char *text ) {
	OutputDebugStringA( text );
}

/*
================
NiStricmp
================
*/
// FUNCTION: STBC 0x86E400
int NiStricmp( const char *s1, const char *s2 ) {
	return _stricmp( s1, s2 );
}

/*
================
NiStrnicmp
================
*/
// FUNCTION: STBC 0x86E420
int NiStrnicmp( const char *s1, const char *s2, unsigned int count ) {
	return _strnicmp( s1, s2, count );
}

/*
================
NiMessageBox
================
*/
// FUNCTION: STBC 0x86E440
void NiMessageBox( const char *text, const char *title ) {
	MessageBoxA( NULL, text, title, MB_OK );
}

/*
================
NiRotr

Gamebryo 1.2.2: NiRotr.
================
*/
// FUNCTION: STBC 0x86E460
unsigned int NiRotr( unsigned int value, int shift ) {
	return _rotr( value, shift );
}

/*
================
NiFile::Access
================
*/
// FUNCTION: STBC 0x86E470
int NiFile::Access( const char *name, int mode ) {
	return _access( name, mode );
}

/*
================
NiFile::StandardizeFilePath

Win32 takes the paths as they are.
================
*/
// FUNCTION: STBC 0x86E490
void NiFile::StandardizeFilePath( char **path ) {
}

/*
================
NiFile::GetFileSize
================
*/
// FUNCTION: STBC 0x86E4A0
unsigned int NiFile::GetFileSize( const char *name ) {
	struct _stat status;
	if ( _stat( name, &status ) != 0 ) {
		return 0;
	}
	return status.st_size;
}
