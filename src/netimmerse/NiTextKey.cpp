// NiTextKey.cpp -- a string at a time in an animation

#include "NiRTLib.h"
#include <string.h>

#include "NiTextKey.h"
#include "NiStream.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x007b6e00 _$E37
// FUNCTION: STBC 0x007b6e20 _$E36
// FUNCTION: STBC 0x007b6e30 _$E17
// FUNCTION: STBC 0x007b6e40 _$E14
// FUNCTION: STBC 0x007b6e50 _$E16
// FUNCTION: STBC 0x007b6e60 _$E15
// FUNCTION: STBC 0x007b6e70 _$E23
// FUNCTION: STBC 0x007b6e80 _$E20
// FUNCTION: STBC 0x007b6e90 _$E22
// FUNCTION: STBC 0x007b6ea0 _$E21

/*
================
NiTextKey::NiTextKey
================
*/
// FUNCTION: STBC 0x7B6CC0
NiTextKey::NiTextKey( float time_, const char *text_ ) {
	time = time_;
	text = 0;
	SetText( text_ );
}

/*
================
NiTextKey::~NiTextKey
================
*/
// FUNCTION: STBC 0x7B6CF0
NiTextKey::~NiTextKey() {
	if ( text ) {
		delete [] text;
	}
}

/*
================
NiTextKey::SetText
================
*/
// FUNCTION: STBC 0x7B6D00
void NiTextKey::SetText( const char *text_ ) {
	if ( text ) {
		delete [] text;
		text = 0;
	}
	if ( text_ ) {
		text = new char[strlen( text_ ) + 1];
		strcpy( text, text_ );
	}
}

/*
================
NiTextKey::Copy
================
*/
// FUNCTION: STBC 0x7B6D70
NiTextKey *NiTextKey::Copy() const {
	NiTextKey *copy = new NiTextKey;
	copy->SetTime( time );
	copy->SetText( text );
	return copy;
}

/*
================
NiTextKey::LoadBinary
================
*/
// FUNCTION: STBC 0x7B6DA0
void NiTextKey::LoadBinary( NiStream &stream ) {
	NiStreamLoadBinary( stream, time );
	stream.LoadCString( text );
}

/*
================
NiTextKey::SaveBinary
================
*/
// FUNCTION: STBC 0x7B6DD0
void NiTextKey::SaveBinary( NiStream &stream ) {
	NiStreamSaveBinary( stream, time );
	stream.SaveCString( text );
}
