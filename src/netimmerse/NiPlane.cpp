// NiPlane.cpp -- planes by normal and constant

#include "NiRTLib.h"
#include <stdio.h>
#include <string.h>

#include "NiPlane.h"
#include "NiStream.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00816b30 _$E42
// FUNCTION: STBC 0x00816b50 _$E17
// FUNCTION: STBC 0x00816b70 _$E15
// FUNCTION: STBC 0x00816b80 _$E23
// FUNCTION: STBC 0x00816ba0 _$E21

/*
================
NiPlane::NiPlane

The zero plane.
================
*/
// FUNCTION: STBC 0x816860 ??0NiPlane@@QAE@XZ
NiPlane::NiPlane() {
	normal = NiPoint3::ZERO;
	constant = 0.0f;
}

/*
================
NiPlane::NiPlane
================
*/
// FUNCTION: STBC 0x816890 ??0NiPlane@@QAE@ABVNiPoint3@@M@Z
NiPlane::NiPlane( const NiPoint3 &normal_, float constant_ ) {
	normal = normal_;
	constant = constant_;
}

/*
================
NiPlane::NiPlane

The plane with the given normal through a point.
================
*/
// FUNCTION: STBC 0x8168B0 ??0NiPlane@@QAE@ABVNiPoint3@@0@Z
NiPlane::NiPlane( const NiPoint3 &normal_, const NiPoint3 &point ) {
	normal = normal_;
	constant = normal_.Dot( point );
}

/*
================
NiPlane::NiPlane

The plane through three points, facing the side from which they run
counter-clockwise.
================
*/
// FUNCTION: STBC 0x8168F0 ??0NiPlane@@QAE@ABVNiPoint3@@00@Z
NiPlane::NiPlane( const NiPoint3 &point0, const NiPoint3 &point1, const NiPoint3 &point2 ) {
	NiPoint3 edge0 = point1 - point0;
	NiPoint3 edge1 = point2 - point1;
	normal = edge0.UnitCross( edge1 );
	constant = normal.Dot( point0 );
}

/*
================
NiPlane::operator=
================
*/
// FUNCTION: STBC 0x816A00 ??4NiPlane@@QAEAAV0@ABV0@@Z
NiPlane &NiPlane::operator=( const NiPlane &plane ) {
	normal = plane.normal;
	constant = plane.constant;
	return *this;
}

/*
================
NiPlane::operator==
================
*/
// FUNCTION: STBC 0x816A20 ??8NiPlane@@QAE_NABV0@@Z
bool NiPlane::operator==( const NiPlane &plane ) {
	return normal == plane.normal && constant == plane.constant;
}

/*
================
NiPlane::LoadBinary
================
*/
// FUNCTION: STBC 0x816A70
void NiPlane::LoadBinary( NiStream &stream ) {
	normal.LoadBinary( stream );
	NiStreamLoadBinary( stream, constant );
}

/*
================
NiPlane::SaveBinary
================
*/
// FUNCTION: STBC 0x816AA0
void NiPlane::SaveBinary( NiStream &stream ) {
	normal.SaveBinary( stream );
	NiStreamSaveBinary( stream, constant );
}

/*
================
NiPlane::GetViewerString

Returns "(x,y,z), constant" in a new string that the caller deletes. The
prefix only sizes the string: unlike the other viewer strings, the format
leaves it out.
================
*/
// FUNCTION: STBC 0x816AD0
char *NiPlane::GetViewerString( const char *prefix ) const {
	char *string = new char[strlen( prefix ) + 56];
	sprintf( string, "(%f,%f,%f), %f", normal.x, normal.y, normal.z, constant );
	return string;
}
