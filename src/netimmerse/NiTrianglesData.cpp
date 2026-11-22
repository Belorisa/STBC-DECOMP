// NiTrianglesData.cpp -- triangle list data without an index list

#include "NiRTLib.h"
#include <assert.h>
#include "NiTrianglesData.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x0080b710 _$E77
// FUNCTION: STBC 0x0080b730 _$E17
// FUNCTION: STBC 0x0080b750 _$E15
// FUNCTION: STBC 0x0080b760 _$E23
// FUNCTION: STBC 0x0080b780 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0080b790 _$E41
// FUNCTION: STBC 0x0080b7a0 _$E49
// FUNCTION: STBC 0x0080b7b0 _$E53
// FUNCTION: STBC 0x0080b800 _$E64

// FUNCTION: STBC 0x0080b130 _$E70
// FUNCTION: STBC 0x0080b150 _$E67
// DATA: STBC 0x9A2540 ?rtti@NiTrianglesData@@2VNiRTTI@@B
const NiRTTI NiTrianglesData::rtti( "NiTrianglesData", &NiTriBasedGeomData::rtti );

/*
================
NiTrianglesData::Register
================
*/
// FUNCTION: STBC 0x80B170 ?Register@NiTrianglesData@@SAHXZ
NiImplementRegister( NiTrianglesData )

/*
================
NiTrianglesData::NiTrianglesData

Takes ownership of the arrays, three vertices to each triangle; every
triangle starts active.
================
*/
// FUNCTION: STBC 0x80B2C0 ??0NiTrianglesData@@QAE@PAVNiPoint3@@0PAVNiColorA@@PAVNiPoint2@@G@Z
NiTrianglesData::NiTrianglesData( NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, NiPoint2 *textures_, unsigned short triangleCount_ )
	: NiTriBasedGeomData( triangleCount_ * 3, vertices_, normals_, colors_, textures_, triangleCount_ ) {
	activeTriangleCount = triangleCount_;
}

/*
================
NiTrianglesData::NiTrianglesData
================
*/
// FUNCTION: STBC 0x80B300 ??0NiTrianglesData@@QAE@XZ
NiTrianglesData::NiTrianglesData() {
	activeTriangleCount = 0;
}

/*
================
NiTrianglesData::CalculateNormals

Each triangle's face normal, from the fast square root table, for all three
of its vertices. The square root works on the length's bits in place,
as NiPoint3::UnitizeVectors does.
================
*/
// FUNCTION: STBC 0x80B320
void NiTrianglesData::CalculateNormals() {
	CreateNormals( true );

	NiPoint3 *vert = vertices;	// (name invented; fits the /Od frame order)
	NiPoint3 *normal = normals;
	for ( unsigned short i = 0; i < triangleCount; i++ ) {
		NiPoint3 e0 = vert[1] - vert[0];	// (name invented; fits the /Od frame order)
		NiPoint3 e1 = vert[2] - vert[1];	// (name invented; fits the /Od frame order)
		NiPoint3 unitNormal = e0.Cross( e1 );	// (name invented; fits the /Od frame order)
		NiPoint3::UnitizeVector( unitNormal );
		*normal++ = unitNormal;
		*normal++ = unitNormal;
		*normal++ = unitNormal;
		vert += 3;
	}
}

/*
================
NiTrianglesData::CreateClone

The data is shared, not copied: the clone is the data itself.
================
*/
// FUNCTION: STBC 0x80B490
NiObject *NiTrianglesData::CreateClone() {
	return this;
}

/*
================
NiTrianglesData::CopyMembers
================
*/
// FUNCTION: STBC 0x80B4A0
void NiTrianglesData::CopyMembers( NiTrianglesData *dest ) {
	assert( false );
}

/*
================
NiTrianglesData::CreateObject

Creates the data and loads it.
================
*/
// FUNCTION: STBC 0x80B4B0
void NiTrianglesData::CreateObject( NiStream &stream, NiObject *&object ) {
	NiTrianglesData *data = new NiTrianglesData;
	stream.SetLoadRecord( new NiTrianglesDataLoadRecord );
	data->LoadBinary( stream );
	object = data;
}

/*
================
NiTrianglesData::LoadBinary
================
*/
// FUNCTION: STBC 0x80B580
void NiTrianglesData::LoadBinary( NiStream &stream ) {
	NiTriBasedGeomData::LoadBinary( stream );
	NiStreamLoadBinary( stream, activeTriangleCount );
}

/*
================
NiTrianglesData::LinkObject
================
*/
// FUNCTION: STBC 0x80B5B0
void NiTrianglesData::LinkObject( NiStream &stream ) {
	NiTriBasedGeomData::LinkObject( stream );
}

/*
================
NiTrianglesData::RegisterStreamables
================
*/
// FUNCTION: STBC 0x80B5C0
bool NiTrianglesData::RegisterStreamables( NiStream &stream ) {
	return NiTriBasedGeomData::RegisterStreamables( stream );
}

/*
================
NiTrianglesData::SaveBinary
================
*/
// FUNCTION: STBC 0x80B5D0
void NiTrianglesData::SaveBinary( NiStream &stream ) {
	NiTriBasedGeomData::SaveBinary( stream );
	NiStreamSaveBinary( stream, activeTriangleCount );
}

/*
================
NiTrianglesData::IsEqual
================
*/
// FUNCTION: STBC 0x80B600
bool NiTrianglesData::IsEqual( NiObject *object ) {
	return NiTriBasedGeomData::IsEqual( object );
}

/*
================
NiTrianglesData::GetViewerStrings
================
*/
// FUNCTION: STBC 0x80B610
void NiTrianglesData::GetViewerStrings( NiTArray<char *> *strings ) {
	NiTriBasedGeomData::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
}

/*
================
NiTrianglesData::SetActiveTriangleCount
================
*/
// FUNCTION: STBC 0x80B810
void NiTrianglesData::SetActiveTriangleCount( unsigned short count ) {
	if ( count <= triangleCount ) {
		activeTriangleCount = count;
	} else {
		activeTriangleCount = triangleCount;
	}
}

/*
================
NiTrianglesData::SetActiveVertexCount
================
*/
// FUNCTION: STBC 0x80B840
void NiTrianglesData::SetActiveVertexCount( unsigned short count ) {
	SetActiveTriangleCount( count / 3 );
}

/*
================
NiTrianglesData::GetActiveVertexCount
================
*/
// FUNCTION: STBC 0x80B870
unsigned short NiTrianglesData::GetActiveVertexCount() const {
	return activeTriangleCount * 3;
}
