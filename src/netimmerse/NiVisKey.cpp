// NiVisKey.cpp -- visibility keys

#include "NiRTLib.h"
#include "NiVisKey.h"
#include "NiStream.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x007b70d0 _$E37
// FUNCTION: STBC 0x007b70f0 _$E36
// FUNCTION: STBC 0x007b7100 _$E17
// FUNCTION: STBC 0x007b7110 _$E14
// FUNCTION: STBC 0x007b7120 _$E16
// FUNCTION: STBC 0x007b7130 _$E15
// FUNCTION: STBC 0x007b7140 _$E23
// FUNCTION: STBC 0x007b7150 _$E20
// FUNCTION: STBC 0x007b7160 _$E22
// FUNCTION: STBC 0x007b7170 _$E21

/*
================
NiVisKey::GenInterp

Returns the visibility of the last key at or before time. lastIndex keeps
the key found by the previous call, where the search starts again unless
time went back before it.
================
*/
// FUNCTION: STBC 0x7B6F70
bool NiVisKey::GenInterp( float time, NiVisKey **keys, unsigned int numKeys, unsigned int &lastIndex ) {
	if ( keys[0]->time >= time ) {
		lastIndex = 0;
		return keys[0]->visible;
	}

	unsigned int last = numKeys - 1;
	if ( keys[last]->time <= time ) {
		lastIndex = last;
		return keys[last]->visible;
	}

	if ( keys[lastIndex]->time > time ) {
		lastIndex = 0;
	}

	for ( unsigned int i = lastIndex + 1; i <= last; i++ ) {
		if ( keys[i]->time > time ) {
			return keys[lastIndex]->visible;
		}
		lastIndex++;
	}

	return true;
}

/*
================
NiVisKey::Copy
================
*/
// FUNCTION: STBC 0x7B7020
NiVisKey *NiVisKey::Copy() const {
	return new NiVisKey( time, visible );
}

/*
================
NiVisKey::LoadBinary

The time, then the visibility as a byte.
================
*/
// FUNCTION: STBC 0x7B7050
void NiVisKey::LoadBinary( NiStream &stream ) {
	NiStreamLoadBinary( stream, time );
	unsigned char value;
	NiStreamLoadBinary( stream, value );
	visible = ( value != 0 );
}

/*
================
NiVisKey::SaveBinary
================
*/
// FUNCTION: STBC 0x7B7090
void NiVisKey::SaveBinary( NiStream &stream ) {
	NiStreamSaveBinary( stream, time );
	unsigned char value = ( visible != 0 );
	NiStreamSaveBinary( stream, value );
}
