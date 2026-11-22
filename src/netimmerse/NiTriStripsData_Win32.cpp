// NiTriStripsData_Win32.cpp -- the platform part of NiTriStripsData: the normals' computation
//
// Gamebryo 1.2.2: Win32/NiTriStripsData_Win32.cpp holds CalculateNormals apart
// from NiTriStripsData.cpp; retail keeps it in a unit of its own (0x823C50),
// with its locale pair, <iostream> quads and registrations.

#include "NiRTLib.h"
#include "NiTriStripsData.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00823f20 _$E48
// FUNCTION: STBC 0x00823f30 _$E56
// FUNCTION: STBC 0x00823f40 _$E60
// FUNCTION: STBC 0x00823f50 _$E64

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00823ea0 _$E71
// FUNCTION: STBC 0x00823ec0 _$E17
// FUNCTION: STBC 0x00823ee0 _$E15
// FUNCTION: STBC 0x00823ef0 _$E23
// FUNCTION: STBC 0x00823f10 _$E21

/*
================
NiTriStripsData::CalculateNormals

Each triangle of each strip adds its face normal to its three vertices; the
sums are then made unit length. The face normal's square root is the table's,
as in NiPoint3::UnitizeVectors. Gamebryo 1.2.2: NiTriStripsData::CalculateNormals.
================
*/
// FUNCTION: STBC 0x823C50 ?CalculateNormals@NiTriStripsData@@UAEXXZ
void NiTriStripsData::CalculateNormals() {
	// stripIndex, base, side0, side1, surfaceNormal: (names invented; fit the /Od frame order)
	CreateNormals( true );

	unsigned short i0, i1, i2;
	NiPoint3 side0;
	NiPoint3 side1;
	NiPoint3 surfaceNormal;
	const unsigned short *base = stripLists;

	for ( unsigned short stripIndex = 0; stripIndex < stripCount; stripIndex++ ) {
		for ( unsigned short j = 2; j < stripLengths[stripIndex]; j++ ) {
			if ( ( j % 2 ) == 0 ) {
				i0 = base[j - 2];
				i1 = base[j - 1];
				i2 = base[j - 0];
			} else {
				i0 = base[j - 2];
				i1 = base[j - 0];
				i2 = base[j - 1];
			}
			side0 = vertices[i1] - vertices[i0];
			side1 = vertices[i2] - vertices[i1];
			surfaceNormal = side0.Cross( side1 );

			NiPoint3::UnitizeVector( surfaceNormal );

			normals[i0] += surfaceNormal;
			normals[i1] += surfaceNormal;
			normals[i2] += surfaceNormal;
		}

		base += stripLengths[stripIndex];
	}

	NiPoint3::UnitizeVectors( normals, vertexCount );
}
