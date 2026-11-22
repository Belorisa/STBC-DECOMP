// NiFilename.cpp -- paths split into their parts

#include <stdlib.h>
#include "NiRTLib.h"
#include "NiFilename.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x0086eb90 _$E30
// FUNCTION: STBC 0x0086ebb0 _$E17
// FUNCTION: STBC 0x0086ebd0 _$E15
// FUNCTION: STBC 0x0086ebe0 _$E23
// FUNCTION: STBC 0x0086ec00 _$E21

/*
================
NiFilename::NiFilename
================
*/
// FUNCTION: STBC 0x86EA70
NiFilename::NiFilename( const char *fullPath ) {
	dir = new char[_MAX_DIR];
	drive = new char[_MAX_DRIVE];
	ext = new char[_MAX_EXT];
	fname = new char[_MAX_FNAME];
	_splitpath( fullPath, drive, dir, fname, ext );
}

/*
================
NiFilename::~NiFilename
================
*/
// FUNCTION: STBC 0x86EAD0
NiFilename::~NiFilename() {
	delete [] dir;
	delete [] drive;
	delete [] ext;
	delete [] fname;
}

/*
================
NiFilename::GetFullPath
================
*/
// FUNCTION: STBC 0x86EB00
void NiFilename::GetFullPath( char *fullPath ) const {
	_makepath( fullPath, drive, dir, fname, ext );
}

/*
================
NiFilename::GetMaxPathLength
================
*/
// FUNCTION: STBC 0x86EB20
unsigned short NiFilename::GetMaxPathLength() {
	return _MAX_PATH;
}

/*
================
NiFilename::Splitpath
================
*/
// FUNCTION: STBC 0x86EB30
void NiFilename::Splitpath( const char *path, char *drive_, char *dir_, char *fname_, char *ext_ ) {
	_splitpath( path, drive_, dir_, fname_, ext_ );
}

/*
================
NiFilename::Makepath
================
*/
// FUNCTION: STBC 0x86EB60
void NiFilename::Makepath( char *path, const char *drive_, const char *dir_, const char *fname_, const char *ext_ ) const {
	_makepath( path, drive_, dir_, fname_, ext_ );
}
