// NiTriShapeData_Win32.cpp -- the platform part of NiTriShapeData: the normals' computation
//
// Gamebryo 1.2.2: Win32/NiTriShapeData_Win32.cpp holds CalculateNormals apart
// from NiTriShapeData.cpp; retail keeps it in a unit of its own (0x821440),
// with its locale pair, <iostream> quads and registrations.

#include "NiRTLib.h"
#include "NiTriShapeData.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x008217f0 _$E48
// FUNCTION: STBC 0x00821800 _$E56
// FUNCTION: STBC 0x00821810 _$E60
// FUNCTION: STBC 0x00821820 _$E64

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00821770 _$E71
// FUNCTION: STBC 0x00821790 _$E17
// FUNCTION: STBC 0x008217b0 _$E15
// FUNCTION: STBC 0x008217c0 _$E23
// FUNCTION: STBC 0x008217e0 _$E21

/*
================
NiTriShapeData::CalculateNormals

Each triangle's face normal is added to its three vertices, and to the
vertices that share their normals; the sums are then made unit length. The
face normal's square root is the table's, as in NiPoint3::UnitizeVectors.
Gamebryo 1.2.2: NiTriShapeData::CalculateNormals.
================
*/
// FUNCTION: STBC 0x821440 ?CalculateNormals@NiTriShapeData@@UAEXXZ
void NiTriShapeData::CalculateNormals() {
	CreateNormals( true );

	unsigned short *list = triList;
	unsigned short triangles = GetActiveTriangleCount();
	for ( unsigned short i = 0; i < triangles; i++ ) {
		unsigned short u0 = *list++;	// (name invented; fits the /Od frame order)
		unsigned short u1 = *list++;	// (name invented; fits the /Od frame order)
		unsigned short u2 = *list++;	// (name invented; fits the /Od frame order)

		NiPoint3 dir0 = vertices[u1] - vertices[u0];	// (name invented; fits the /Od frame order)
		NiPoint3 dir1 = vertices[u2] - vertices[u1];	// (name invented; fits the /Od frame order)
		NiPoint3 normal = dir0.Cross( dir1 );
		NiPoint3::UnitizeVector( normal );

		normals[u0] += normal;
		normals[u1] += normal;
		normals[u2] += normal;

		if ( sharedNormals && sharedNormalsArraySize == vertexCount ) {
			unsigned short *same = sharedNormals[u0].GetArray();	// (name invented; fits the /Od frame order)
			unsigned short count = sharedNormals[u0].GetCount();
			while ( count-- ) {
				normals[same[count]] += normal;
			}

			same = sharedNormals[u1].GetArray();
			count = sharedNormals[u1].GetCount();
			while ( count-- ) {
				normals[same[count]] += normal;
			}

			same = sharedNormals[u2].GetArray();
			count = sharedNormals[u2].GetCount();
			while ( count-- ) {
				normals[same[count]] += normal;
			}
		}
	}

	NiPoint3::UnitizeVectors( normals, GetActiveVertexCount() );
}
