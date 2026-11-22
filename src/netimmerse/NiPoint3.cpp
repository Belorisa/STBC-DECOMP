// NiPoint3.cpp -- points and vectors in 3D space

#include "NiRTLib.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "NiPoint3.h"
#include "NiStream.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00811c60 _$E62
// FUNCTION: STBC 0x00811c80 _$E17
// FUNCTION: STBC 0x00811ca0 _$E15
// FUNCTION: STBC 0x00811cb0 _$E23
// FUNCTION: STBC 0x00811cd0 _$E21

// FUNCTION: STBC 0x008119e0 _$E38
// FUNCTION: STBC 0x00811a00 _$E41
// FUNCTION: STBC 0x00811a20 _$E44
// FUNCTION: STBC 0x00811a40 _$E47
// DATA: STBC 0x9A2878 ?ZERO@NiPoint3@@2V1@B
const NiPoint3 NiPoint3::ZERO( 0.0f, 0.0f, 0.0f );
// DATA: STBC 0x9A2840 ?UNIT_X@NiPoint3@@2V1@B
const NiPoint3 NiPoint3::UNIT_X( 1.0f, 0.0f, 0.0f );
// DATA: STBC 0x9A2858 ?UNIT_Y@NiPoint3@@2V1@B
const NiPoint3 NiPoint3::UNIT_Y( 0.0f, 1.0f, 0.0f );
// DATA: STBC 0x9A2868 ?UNIT_Z@NiPoint3@@2V1@B
const NiPoint3 NiPoint3::UNIT_Z( 0.0f, 0.0f, 1.0f );

// FUNCTION: STBC 0x00811a60 _$E50
// DATA: STBC 0x9A284C
unsigned int *NiPoint3::sqrtTable = NiPoint3::InitSqrtTable();

// Frees the square root table at exit (the class name is invented).
class NiSqrtTableRelease {
public:
	~NiSqrtTableRelease() {
		delete [] NiPoint3::sqrtTable;
		NiPoint3::sqrtTable = 0;
	}
};

// FUNCTION: STBC 0x00811ae0 _$E55
// FUNCTION: STBC 0x00811af0 _$E53
static NiSqrtTableRelease sqrtTableRelease;

/*
================
NiPoint3::LoadBinary
================
*/
// FUNCTION: STBC 0x811B10
void NiPoint3::LoadBinary( NiStream &stream ) {
	NiStreamLoadBinary( stream, x );
	NiStreamLoadBinary( stream, y );
	NiStreamLoadBinary( stream, z );
}

/*
================
NiPoint3::SaveBinary
================
*/
// FUNCTION: STBC 0x811B50
void NiPoint3::SaveBinary( NiStream &stream ) {
	NiStreamSaveBinary( stream, x );
	NiStreamSaveBinary( stream, y );
	NiStreamSaveBinary( stream, z );
}

/*
================
NiPoint3::GetViewerString

Returns "<prefix> = (x,y,z)" in a new string that the caller deletes.
================
*/
// FUNCTION: STBC 0x811B90
char *NiPoint3::GetViewerString( const char *prefix ) const {
	char *string = new char[strlen( prefix ) + 48];
	sprintf( string, "%s = (%f,%f,%f)", prefix, x, y, z );
	return string;
}

/*
================
NiPoint3::InitSqrtTable

Builds the 256-entry table: the square root of 1.m (exponent 0) and of
2 * 1.m (exponent 1) for the top seven mantissa bits m, keeping only the
mantissa bits of each root.
================
*/
// FUNCTION: STBC 0x811BE0
unsigned int *NiPoint3::InitSqrtTable() {
	unsigned int *table = new unsigned int[256];

	for ( unsigned int i = 0; i < 128; i++ ) {
		union {
			float			f;
			unsigned int	i;
		} root;

		root.i = ( i << 16 ) | ( 127 << 23 );
		root.f = (float)sqrt( root.f );
		table[i] = root.i & 0x7FFFFF;

		root.i = ( i << 16 ) | ( 128 << 23 );
		root.f = (float)sqrt( root.f );
		table[i + 128] = root.i & 0x7FFFFF;
	}

	return table;
}
