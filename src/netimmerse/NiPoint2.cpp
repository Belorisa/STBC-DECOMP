// NiPoint2.cpp -- points and vectors in 2D space

#include "NiRTLib.h"
#include <stdio.h>
#include <string.h>

#include "NiPoint2.h"
#include "NiStream.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00820120 _$E49
// FUNCTION: STBC 0x00820140 _$E17
// FUNCTION: STBC 0x00820160 _$E15
// FUNCTION: STBC 0x00820170 _$E23
// FUNCTION: STBC 0x00820190 _$E21

// The three constants' dynamic initialisers.
// FUNCTION: STBC 0x00820010 _$E36
// FUNCTION: STBC 0x00820030 _$E39
// FUNCTION: STBC 0x00820050 _$E42

// DATA: STBC 0x9A2BB8 ?ZERO@NiPoint2@@2V1@B
const NiPoint2 NiPoint2::ZERO( 0.0f, 0.0f );
// DATA: STBC 0x9A2BC8 ?UNIT_X@NiPoint2@@2V1@B
const NiPoint2 NiPoint2::UNIT_X( 1.0f, 0.0f );
// DATA: STBC 0x9A2BD0 ?UNIT_Y@NiPoint2@@2V1@B
const NiPoint2 NiPoint2::UNIT_Y( 0.0f, 1.0f );

/*
================
NiPoint2::LoadBinary
================
*/
// FUNCTION: STBC 0x820070
void NiPoint2::LoadBinary( NiStream &stream ) {
	NiStreamLoadBinary( stream, x );
	NiStreamLoadBinary( stream, y );
}

/*
================
NiPoint2::SaveBinary
================
*/
// FUNCTION: STBC 0x8200A0
void NiPoint2::SaveBinary( NiStream &stream ) {
	NiStreamSaveBinary( stream, x );
	NiStreamSaveBinary( stream, y );
}

/*
================
NiPoint2::GetViewerString

Returns "<prefix> = (x,y)" in a new string that the caller deletes.
================
*/
// FUNCTION: STBC 0x8200D0
char *NiPoint2::GetViewerString( const char *prefix ) const {
	char *string = new char[strlen( prefix ) + 36];
	sprintf( string, "%s = (%f,%f)", prefix, x, y );
	return string;
}
