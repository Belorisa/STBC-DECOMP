// NiImageFile.cpp -- reads image files by their extension

#include "NiRTLib.h"
#include <string.h>
#include "NiFile.h"
#include "NiStream.h"
#include "NiSystem.h"
#include "NiRawImageData.h"
#include "NiImageFile.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00819270 _$E54
// FUNCTION: STBC 0x00819290 _$E17
// FUNCTION: STBC 0x008192b0 _$E15
// FUNCTION: STBC 0x008192c0 _$E23
// FUNCTION: STBC 0x008192e0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x008192f0 _$E40
// FUNCTION: STBC 0x00819300 _$E47

/*
================
NiReadImageFile

A NIF file must hold exactly one top-level object, a NiRawImageData, whose
pixels the image takes over. The other formats go to their readers.
The NIF case returns early at each failed test and tests the object's
kind with NiIsKindOf, as the SDK's debug NetImmerse 3.1 compiles it
(AnimOnly.exe 0x457A10).
================
*/
// FUNCTION: STBC 0x819000
bool NiReadImageFile( const char *file, NiRawImageData *image ) {
	if ( !file ) {
		return false;
	}

	char diskPath[256];	// (name invented; fits the /Od frame order)
	if ( NiStrnicmp( file, "file://", 7 ) == 0 ) {
		unsigned int i;
		for ( i = 7; file[i] != '\0'; i++ ) {
			diskPath[i - 7] = file[i];
		}
		diskPath[i - 7] = '\0';
	} else {
		strcpy( diskPath, file );
	}

	char *extension = strrchr( diskPath, '.' );
	if ( !extension ) {
		return false;
	}

	if ( NiStricmp( extension, ".nif" ) == 0 ) {
		NiStream loadStream;	// (name invented; fits the /Od frame order)
		loadStream.Load( diskPath );
		if ( loadStream.GetObjectCount() != 1 ) {
			return false;
		}
		NiObject *object = loadStream.GetObjectAt( 0 );
		if ( !NiIsKindOf( NiRawImageData, object ) ) {
			return false;
		}
		image->Unknown7DEEA0( (NiRawImageData *)object );
		return true;
	}

	NiFile imageFile( diskPath, NiFile::READ_ONLY, 0x2800 );
	if ( !imageFile ) {
		return false;
	}

	bool bResult;	// the name NiImage.cpp's assert gives a read's result
	if ( NiStricmp( extension, ".tga" ) == 0 ) {
		bResult = NiReadTGA( imageFile, image );
	} else if ( NiStricmp( extension, ".bmp" ) == 0 ) {
		bResult = NiReadBMPFile( imageFile, image );
	} else if ( NiStricmp( extension, ".rgb" ) == 0 || NiStricmp( extension, ".rgba" ) == 0
			|| NiStricmp( extension, ".int" ) == 0 || NiStricmp( extension, ".inta" ) == 0 ) {
		bResult = NiReadSGIFile( imageFile, image );
	} else {
		bResult = false;
	}
	return bResult;
}
