// NiColor.cpp -- RGB and RGBA colours

#include "NiRTLib.h"
#include <stdio.h>
#include <string.h>

#include "NiColor.h"
#include "NiStream.h"
#include "NiExtraData.h"
#include "NiObject.h"
#include "NiObjectNET.h"
#include "NiProperty.h"
#include "NiTimeController.h"
#include "NiZBufferProperty.h"

// The four colour constants' dynamic initialisers.
// FUNCTION: STBC 0x008115a0 _$E65
// FUNCTION: STBC 0x008115c0 _$E68
// FUNCTION: STBC 0x008115e0 _$E71
// FUNCTION: STBC 0x00811610 _$E74

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00811960 _$E81
// FUNCTION: STBC 0x00811980 _$E17
// FUNCTION: STBC 0x008119a0 _$E15
// FUNCTION: STBC 0x008119b0 _$E23
// FUNCTION: STBC 0x008119d0 _$E21

// DATA: STBC 0x9A2800 ?BLACK@NiColor@@2V1@B
const NiColor NiColor::BLACK( 0.0f, 0.0f, 0.0f );
// DATA: STBC 0x9A2820 ?WHITE@NiColor@@2V1@B
const NiColor NiColor::WHITE( 1.0f, 1.0f, 1.0f );
// DATA: STBC 0x9A2830 ?BLACK@NiColorA@@2V1@B
const NiColorA NiColorA::BLACK( 0.0f, 0.0f, 0.0f, 1.0f );
// DATA: STBC 0x9A2810 ?WHITE@NiColorA@@2V1@B
const NiColorA NiColorA::WHITE( 1.0f, 1.0f, 1.0f, 1.0f );

/*
================
NiColor::Clamp

Limits each channel to at most 1. Negative channels are left alone.
================
*/
// FUNCTION: STBC 0x811640
void NiColor::Clamp() {
	if ( r > 1.0f ) {
		r = 1.0f;
	}
	if ( g > 1.0f ) {
		g = 1.0f;
	}
	if ( b > 1.0f ) {
		b = 1.0f;
	}
}

/*
================
NiColor::Scale

Scales the colour down so that its largest channel is at most 1.
================
*/
// FUNCTION: STBC 0x811680
void NiColor::Scale() {
	float max = r;
	if ( g > max ) {
		max = g;
	}
	if ( b > max ) {
		max = b;
	}
	if ( max > 1.0f ) {
		float invMax = 1.0f / max;
		r *= invMax;
		g *= invMax;
		b *= invMax;
	}
}

/*
================
NiColorA::Clamp

Limits each channel to at most 1. Negative channels are left alone.
================
*/
// FUNCTION: STBC 0x8116D0
void NiColorA::Clamp() {
	if ( r > 1.0f ) {
		r = 1.0f;
	}
	if ( g > 1.0f ) {
		g = 1.0f;
	}
	if ( b > 1.0f ) {
		b = 1.0f;
	}
	if ( a > 1.0f ) {
		a = 1.0f;
	}
}

/*
================
NiColorA::Scale

Scales the colour channels down so that the largest is at most 1, and
limits alpha to at most 1.
================
*/
// FUNCTION: STBC 0x811720
void NiColorA::Scale() {
	float max = r;
	if ( g > max ) {
		max = g;
	}
	if ( b > max ) {
		max = b;
	}
	if ( max > 1.0f ) {
		float invMax = 1.0f / max;
		r *= invMax;
		g *= invMax;
		b *= invMax;
	}
	if ( a > 1.0f ) {
		a = 1.0f;
	}
}

/*
================
NiColor::LoadBinary
================
*/
// FUNCTION: STBC 0x811790
void NiColor::LoadBinary( NiStream &stream ) {
	NiStreamLoadBinary( stream, r );
	NiStreamLoadBinary( stream, g );
	NiStreamLoadBinary( stream, b );
}

/*
================
NiColor::SaveBinary
================
*/
// FUNCTION: STBC 0x8117D0
void NiColor::SaveBinary( NiStream &stream ) {
	NiStreamSaveBinary( stream, r );
	NiStreamSaveBinary( stream, g );
	NiStreamSaveBinary( stream, b );
}

/*
================
NiColor::GetViewerString

Returns "<prefix> = (r,g,b)" in a new string that the caller deletes.
================
*/
// FUNCTION: STBC 0x811810
char *NiColor::GetViewerString( const char *prefix ) const {
	char *string = new char[strlen( prefix ) + 28];
	sprintf( string, "%s = (%5.3f,%5.3f,%5.3f)", prefix, r, g, b );
	return string;
}

/*
================
NiColorA::LoadBinary
================
*/
// FUNCTION: STBC 0x811860
void NiColorA::LoadBinary( NiStream &stream ) {
	NiStreamLoadBinary( stream, r );
	NiStreamLoadBinary( stream, g );
	NiStreamLoadBinary( stream, b );
	NiStreamLoadBinary( stream, a );
}

/*
================
NiColorA::SaveBinary
================
*/
// FUNCTION: STBC 0x8118B0
void NiColorA::SaveBinary( NiStream &stream ) {
	NiStreamSaveBinary( stream, r );
	NiStreamSaveBinary( stream, g );
	NiStreamSaveBinary( stream, b );
	NiStreamSaveBinary( stream, a );
}

/*
================
NiColorA::GetViewerString

Returns "<prefix> = (r,g,b,a)" in a new string that the caller deletes.
================
*/
// FUNCTION: STBC 0x811900
char *NiColorA::GetViewerString( const char *prefix ) const {
	char *string = new char[strlen( prefix ) + 36];
	sprintf( string, "%s = (%5.3f,%5.3f,%5.3f,%5.3f)", prefix, r, g, b, a );
	return string;
}
