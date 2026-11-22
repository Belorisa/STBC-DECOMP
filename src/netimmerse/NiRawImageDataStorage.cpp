// NiRawImageDataStorage.cpp -- the pixel buffer of a raw image
//
// Retail keeps these two members apart from the rest of the class (0x8221F0,
// 0x822210, beside NiBoundingVolume and NiSortedObject), out of reach of the
// inliner. The unit name is invented.

#include "NiRTLib.h"
#include "NiRawImageData.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00822220 _$E51
// FUNCTION: STBC 0x00822240 _$E17
// FUNCTION: STBC 0x00822260 _$E15
// FUNCTION: STBC 0x00822270 _$E23
// FUNCTION: STBC 0x00822290 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x008222a0 _$E30
// FUNCTION: STBC 0x008222b0 _$E44

/*
================
NiRawImageData::AllocateData

The pixel buffer, of a size in bytes.
================
*/
// FUNCTION: STBC 0x8221F0
void NiRawImageData::AllocateData( unsigned int size ) {
	data = new unsigned char[size];
}

/*
================
NiRawImageData::FreeData
================
*/
// FUNCTION: STBC 0x822210
void NiRawImageData::FreeData() {
	delete [] data;
}
