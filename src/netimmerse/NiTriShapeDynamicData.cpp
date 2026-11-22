// NiTriShapeDynamicData.cpp -- triangle list geometry data with active counts

#include "NiRTLib.h"
#include <assert.h>
#include "NiTriShapeDynamicData.h"
#include "NiTriBasedGeom.h"
#include "NiTriangles.h"
#include "NiTrianglesData.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x0080c820 _$E229
// FUNCTION: STBC 0x0080c840 _$E17
// FUNCTION: STBC 0x0080c860 _$E15
// FUNCTION: STBC 0x0080c870 _$E23
// FUNCTION: STBC 0x0080c890 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0080c8a0 _$E48
// FUNCTION: STBC 0x0080c8b0 _$E56
// FUNCTION: STBC 0x0080c8c0 _$E60
// FUNCTION: STBC 0x0080c8d0 _$E64
// FUNCTION: STBC 0x0080c910 _$E68

// FUNCTION: STBC 0x0080c250 _$E222
// FUNCTION: STBC 0x0080c270 _$E219
// DATA: STBC 0x9A25C0 ?rtti@NiTriShapeDynamicData@@2VNiRTTI@@B
const NiRTTI NiTriShapeDynamicData::rtti( "NiTriShapeDynamicData", &NiTriShapeData::rtti );

/*
================
NiTriShapeDynamicData::Register
================
*/
// FUNCTION: STBC 0x80C290 ?Register@NiTriShapeDynamicData@@SAHXZ
NiImplementRegister( NiTriShapeDynamicData )

/*
================
NiTriShapeDynamicData::NiTriShapeDynamicData

Takes ownership of the arrays. NiTriShapeData's members are built over
the active counts, then the full counts replace them.
================
*/
// FUNCTION: STBC 0x80C3E0 ??0NiTriShapeDynamicData@@QAE@GPAVNiPoint3@@0PAVNiColorA@@PAVNiPoint2@@GPAGGG@Z
NiTriShapeDynamicData::NiTriShapeDynamicData( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, NiPoint2 *textures_, unsigned short triangleCount_, unsigned short *triList_, unsigned short activeVertices_, unsigned short activeTriangles_ )
	: NiTriShapeData( activeVertices_, vertices_, normals_, colors_, textures_, activeTriangles_, triList_ ) {
	activeVertices = activeVertices_;
	activeTriangles = activeTriangles_;
	vertexCount = vertexCount_;
	triangleCount = triangleCount_;
	triListLength = 3 * triangleCount;
}

/*
================
NiTriShapeDynamicData::NiTriShapeDynamicData
================
*/
// FUNCTION: STBC 0x80C450 ??0NiTriShapeDynamicData@@QAE@XZ
NiTriShapeDynamicData::NiTriShapeDynamicData() {
	activeVertices = 0;
	activeTriangles = 0;
}

/*
================
NiTriShapeDynamicData::GetActiveVertexCount
================
*/
// FUNCTION: STBC 0x80C470
unsigned short NiTriShapeDynamicData::GetActiveVertexCount() const {
	return activeVertices;
}

/*
================
NiTriShapeDynamicData::GetActiveTriangleCount
================
*/
// FUNCTION: STBC 0x80C480
unsigned short NiTriShapeDynamicData::GetActiveTriangleCount() const {
	return activeTriangles;
}

/*
================
NiTriShapeDynamicData::CreateClone

The data is shared, not copied: the clone is the data itself.
================
*/
// FUNCTION: STBC 0x80C490
NiObject *NiTriShapeDynamicData::CreateClone() {
	return this;
}

/*
================
NiTriShapeDynamicData::CopyMembers
================
*/
// FUNCTION: STBC 0x80C4A0
void NiTriShapeDynamicData::CopyMembers( NiTriShapeDynamicData *dest ) {
	assert( false );
}

/*
================
NiTriShapeDynamicData::CreateObject

Creates the data and loads it.
================
*/
// FUNCTION: STBC 0x80C4B0
void NiTriShapeDynamicData::CreateObject( NiStream &stream, NiObject *&object ) {
	NiTriShapeDynamicData *data = new NiTriShapeDynamicData;
	stream.SetLoadRecord( new NiTriShapeDynamicDataLoadRecord );
	data->LoadBinary( stream );
	object = data;
}

/*
================
NiTriShapeDynamicData::LoadBinary
================
*/
// FUNCTION: STBC 0x80C580
void NiTriShapeDynamicData::LoadBinary( NiStream &stream ) {
	NiTriShapeData::LoadBinary( stream );
	NiStreamLoadBinary( stream, activeVertices );
	NiStreamLoadBinary( stream, activeTriangles );
}

/*
================
NiTriShapeDynamicData::LinkObject
================
*/
// FUNCTION: STBC 0x80C5B0
void NiTriShapeDynamicData::LinkObject( NiStream &stream ) {
	NiTriShapeData::LinkObject( stream );
}

/*
================
NiTriShapeDynamicData::RegisterStreamables
================
*/
// FUNCTION: STBC 0x80C5C0
bool NiTriShapeDynamicData::RegisterStreamables( NiStream &stream ) {
	return NiTriShapeData::RegisterStreamables( stream );
}

/*
================
NiTriShapeDynamicData::SaveBinary
================
*/
// FUNCTION: STBC 0x80C5D0
void NiTriShapeDynamicData::SaveBinary( NiStream &stream ) {
	NiTriShapeData::SaveBinary( stream );
	NiStreamSaveBinary( stream, activeVertices );
	NiStreamSaveBinary( stream, activeTriangles );
}

/*
================
NiTriShapeDynamicData::IsEqual
================
*/
// FUNCTION: STBC 0x80C600
bool NiTriShapeDynamicData::IsEqual( NiObject *object ) {
	if ( !NiTriShapeData::IsEqual( object ) ) {
		return false;
	}
	NiTriShapeDynamicData *other = (NiTriShapeDynamicData *)object;
	if ( activeVertices != other->activeVertices || activeTriangles != other->activeTriangles ) {
		return false;
	}
	return true;
}

/*
================
NiTriShapeDynamicData::GetViewerStrings
================
*/
// FUNCTION: STBC 0x80C640
void NiTriShapeDynamicData::GetViewerStrings( NiTArray<char *> *strings ) {
	NiTriShapeData::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_usActiveVertices", activeVertices ) );
	strings->Add( NiGetViewerString( "m_usActiveTriangles", activeTriangles ) );
}
