/*
===============================================================================

	TGDirectory.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>
#include <windows.h>

#include "TGDirectory.h"
#include "TGSystemWrapperClass.h"
#include "NiAVObject.h"
#include "NiAccumulator.h"
#include "NiDynamicEffect.h"
#include "NiExtraData.h"
#include "renderer/NiImage.h"
#include "NiObject.h"
#include "NiObjectNET.h"
#include "NiProperty.h"
#include "NiRawImageData.h"
#include "renderer/NiRender.h"
#include "NiTimeController.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0071e590 _$E32
// FUNCTION: STBC 0x0071e5a0 _$E29
// FUNCTION: STBC 0x0071e5b0 _$E31
// FUNCTION: STBC 0x0071e5c0 _$E30
// FUNCTION: STBC 0x0071e5d0 _$E38
// FUNCTION: STBC 0x0071e5e0 _$E35
// FUNCTION: STBC 0x0071e5f0 _$E37
// FUNCTION: STBC 0x0071e600 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0071e610 _$E45
// FUNCTION: STBC 0x0071e620 _$E44
// FUNCTION: STBC 0x0071e630 _$E196
// FUNCTION: STBC 0x0071e640 _$E195
// FUNCTION: STBC 0x0071e650 _$E134
// FUNCTION: STBC 0x0071e660 _$E133
// FUNCTION: STBC 0x0071e670 _$E147
// FUNCTION: STBC 0x0071e680 _$E146
// FUNCTION: STBC 0x0071e690 _$E56
// FUNCTION: STBC 0x0071e6a0 _$E55
// FUNCTION: STBC 0x0071e6b0 _$E60
// FUNCTION: STBC 0x0071e6c0 _$E59
// FUNCTION: STBC 0x0071e6d0 _$E64
// FUNCTION: STBC 0x0071e6e0 _$E63
// FUNCTION: STBC 0x0071e6f0 _$E68
// FUNCTION: STBC 0x0071e700 _$E67
// FUNCTION: STBC 0x0071e710 _$E192
// FUNCTION: STBC 0x0071e720 _$E191
// FUNCTION: STBC 0x0071e730 _$E201
// FUNCTION: STBC 0x0071e740 _$E200
// FUNCTION: STBC 0x0071e750 _$E206
// FUNCTION: STBC 0x0071e760 _$E205

/*
================
TGDirectory::~TGDirectory

Frees the files, the following siblings and the subdirectories.
================
*/
// FUNCTION: STBC 0x0071eed0
TGDirectory::~TGDirectory( void ) {
	ClearFiles();
	if ( next ) {
		delete next;
	}
	if ( subdirectories ) {
		delete subdirectories;
	}
	if ( name ) {
		delete [] name;
	}
	if ( path ) {
		delete [] path;
	}
}

/*
================
TGDirectory::SetName
================
*/
// FUNCTION: STBC 0x0071ef30
void TGDirectory::SetName( char *name_ ) {
	if ( name ) {
		delete [] name;
		name = NULL;
	}
	int length = strlen( name_ );
	if ( length > 0 ) {
		name = new char[length + 1];
		strcpy( name, name_ );
	}
}

/*
================
TGDirectory::SetPath
================
*/
// FUNCTION: STBC 0x0071efa0
void TGDirectory::SetPath( char *path_ ) {
	if ( path ) {
		delete [] path;
		path = NULL;
	}
	int length = strlen( path_ );
	if ( length > 0 ) {
		path = new char[length + 1];
		strcpy( path, path_ );
	}
}

/*
================
TGDirectory::ClearFiles
================
*/
// FUNCTION: STBC 0x0071f010
void TGDirectory::ClearFiles( void ) {
	TGDirectoryFile *file = files;
	while ( file ) {
		TGDirectoryFile *following = file->next;
		delete file;
		file = following;
	}
	files = NULL;
	numFiles = 0;
}

/*
================
TGDirectory::AddFile

Inserts the file in name order.
================
*/
// FUNCTION: STBC 0x0071f050
void TGDirectory::AddFile( TGDirectoryFile *file ) {
	TGDirectoryFile *previous = files;
	if ( previous == NULL ) {
		files = file;
		numFiles = 1;
		return;
	}
	// BUG: compares the first file with itself, so a file is never put
	// first; the dead branch would also link the first file to itself
	if ( strcmp( previous->name, previous->name ) < 0 ) {
		previous->next = previous;
		files = file;
		numFiles++;
		return;
	}
	while ( previous->next ) {
		if ( strcmp( file->name, previous->next->name ) < 0 ) {
			file->next = previous->next;
			previous->next = file;
			numFiles++;
			return;
		}
		previous = previous->next;
	}
	previous->next = file;
	numFiles++;
}

/*
================
TGDirectory::AddSubdirectory

Inserts the subdirectory in name order.
================
*/
// FUNCTION: STBC 0x0071f140
void TGDirectory::AddSubdirectory( TGDirectory *directory ) {
	TGDirectory *previous = subdirectories;
	if ( previous == NULL ) {
		subdirectories = directory;
		numSubdirectories = 1;
		return;
	}
	// BUG: as AddFile, compares the first subdirectory with itself
	if ( strcmp( previous->name, previous->name ) < 0 ) {
		previous->next = previous;
		subdirectories = directory;
		numSubdirectories++;
		return;
	}
	while ( previous->next ) {
		if ( strcmp( directory->name, previous->next->name ) < 0 ) {
			directory->next = previous->next;
			previous->next = directory;
			numSubdirectories++;
			return;
		}
		previous = previous->next;
	}
	previous->next = directory;
	numSubdirectories++;
}

/*
================
TGDirectory::Exists
================
*/
// FUNCTION: STBC 0x0071f230
// PARKED: complete; retail's find data takes 0x280 bytes of frame, twice a WIN32_FIND_DATA (a doubled buffer matches exactly; the real type is unknown)
bool TGDirectory::Exists( char *path ) {
	// TODO identity: retail's find data takes 0x280 bytes of frame, which matches no WIN32_FIND_DATA variant (A is 0x140, W 0x250); the real type is unknown (user's ruling, 2026-11-18: no type change)
	WIN32_FIND_DATA data;
	HANDLE handle = FindFirstFile( path, &data );
	if ( handle == INVALID_HANDLE_VALUE ) {
		return false;
	}
	FindClose( handle );
	return true;
}

/*
================
TGDirectory::Read

Replaces the listing with the directory's.
================
*/
// FUNCTION: STBC 0x0071f270
bool TGDirectory::Read( char *path_, char *filter, bool recurse ) {
	char directory[MAX_PATH];
	strcpy( directory, path_ );
	FixPath( directory );
	ClearFiles();
	if ( next ) {
		delete next;
		next = NULL;
	}
	if ( subdirectories ) {
		delete subdirectories;
		subdirectories = NULL;
	}
	bool result = Scan( directory, filter, recurse );
	SetName( directory );
	SetPath( directory );
	return result;
}

/*
================
TGDirectory::Scan

Lists the files of path matching filter and, with recurse, its
subdirectories, each scanned in turn. When no subdirectory matches the
filter, the subdirectories are listed again without it.
================
*/
// FUNCTION: STBC 0x0071f340
// PARKED: complete; as Exists, retail's find data takes 0x280 bytes (with a doubled WIN32_FIND_DATA the function matches exactly)
bool TGDirectory::Scan( char *path_, char *filter, bool recurse ) {
	bool first = true;
	bool result = true;
	char pattern[MAX_PATH];
	// TODO identity: retail's find data takes 0x280 bytes of frame, which matches no WIN32_FIND_DATA variant (A is 0x140, W 0x250); the real type is unknown (user's ruling, 2026-11-18: no type change)
	WIN32_FIND_DATA data;
	HANDLE handle;
	char subPath[MAX_PATH];
	char workingDirectory[MAX_PATH];

	if ( path_ == NULL ) {
		return false;
	}
	FixPath( path_ );
	MakePattern( pattern, path_, filter );
	g_kSystemWrapper.GetWorkingDirectory( workingDirectory, MAX_PATH );

	for ( ;; ) {
		if ( first ) {
			first = false;
			handle = FindFirstFile( pattern, &data );
			if ( handle == INVALID_HANDLE_VALUE ) {
				result = false;
				goto subdirectories;
			}
			SetPath( path_ );
		} else if ( !FindNextFile( handle, &data ) ) {
			break;
		}
		if ( data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
			if ( recurse == true && strcmp( data.cFileName, "." ) != 0 && strcmp( data.cFileName, ".." ) != 0 ) {
				TGDirectory *subdirectory = new TGDirectory;
				subdirectory->SetName( data.cFileName );
				strcpy( subPath, path_ );
				strcat( subPath, "/" );
				strcat( subPath, data.cFileName );
				// BUG: sets this directory's path, not the subdirectory's;
				// Read sets it back at the end
				SetPath( subPath );
				AddSubdirectory( subdirectory );
			}
		} else {
			TGDirectoryFile *file = new TGDirectoryFile;
			if ( file == NULL ) {
				break;
			}
			file->name = new char[strlen( data.cFileName ) + 2];
			if ( file->name == NULL ) {
				delete file;
				break;
			}
			strcpy( file->name, data.cFileName );
			AddFile( file );
		}
	}
	FindClose( handle );

subdirectories:
	if ( recurse == true ) {
		if ( filter != NULL && subdirectories == NULL ) {
			MakePattern( pattern, path_, NULL );
			first = true;
			for ( ;; ) {
				if ( first ) {
					first = false;
					handle = FindFirstFile( pattern, &data );
					if ( handle == INVALID_HANDLE_VALUE ) {
						break;
					}
				} else if ( !FindNextFile( handle, &data ) ) {
					break;
				}
				if ( ( data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) && strcmp( data.cFileName, "." ) != 0 &&
						strcmp( data.cFileName, ".." ) != 0 ) {
					TGDirectory *subdirectory = new TGDirectory;
					subdirectory->SetName( data.cFileName );
					AddSubdirectory( subdirectory );
				}
			}
			// BUG: also closes the invalid handle of a failed FindFirstFile
			FindClose( handle );
		}
		for ( TGDirectory *subdirectory = subdirectories; subdirectory; subdirectory = subdirectory->next ) {
			strcpy( pattern, path_ );
			strcat( pattern, "/" );
			strcat( pattern, subdirectory->name );
			result &= subdirectory->Scan( pattern, filter, recurse );
		}
	}
	g_kSystemWrapper.SetWorkingDirectory( workingDirectory );
	return result;
}

/*
================
TGDirectory::FixPath
================
*/
// FUNCTION: STBC 0x0071f890
void TGDirectory::FixPath( char *path_ ) {
	int length = strlen( path_ );
	for ( int i = 0; i < length; i++ ) {
		if ( path_[i] == '\\' ) {
			path_[i] = '/';
		}
	}
	if ( path_[length - 1] == '\n' || path_[length - 1] == '\r' ) {
		path_[length - 1] = '\0';
		length--;
	}
	if ( path_[length - 1] == '/' ) {
		path_[length - 1] = '\0';
	}
}

/*
================
TGDirectory::MakePattern
================
*/
// FUNCTION: STBC 0x0071f8e0
void TGDirectory::MakePattern( char *pattern, char *directory, char *filter ) {
	strcpy( pattern, directory );
	strcat( pattern, "/" );
	strcat( pattern, filter ? filter : "*" );
}
