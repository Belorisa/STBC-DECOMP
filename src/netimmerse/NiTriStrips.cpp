// NiTriStrips.cpp -- triangle strip geometry objects

#include "NiRTLib.h"
#include <assert.h>
#include "NiCamera.h"
#include "NiStream.h"
#include "NiTriStrips.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0080f510 _$E30
// FUNCTION: STBC 0x0080f520 _$E45
// FUNCTION: STBC 0x0080f530 _$E49
// FUNCTION: STBC 0x0080f540 _$E53
// FUNCTION: STBC 0x0080f550 _$E57
// FUNCTION: STBC 0x0080f560 _$E181
// FUNCTION: STBC 0x0080f570 _$E214
// FUNCTION: STBC 0x0080f580 _$E221
// FUNCTION: STBC 0x0080f590 _$E225
// FUNCTION: STBC 0x0080f5a0 _$E229
// FUNCTION: STBC 0x0080f5b0 _$E233
// FUNCTION: STBC 0x0080f600 _$E237
// FUNCTION: STBC 0x0080f610 _$E34
// FUNCTION: STBC 0x0080f620 _$E123
// FUNCTION: STBC 0x0080f630 _$E136
// FUNCTION: STBC 0x0080f640 _$E186
// FUNCTION: STBC 0x0080f650 _$E191
// FUNCTION: STBC 0x0080f660 _$E195
// FUNCTION: STBC 0x0080f670 _$E199
// FUNCTION: STBC 0x0080f680 _$E206

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x0080f4f0 _$E250
// FUNCTION: STBC 0x0080f490 _$E17
// FUNCTION: STBC 0x0080f4b0 _$E15
// FUNCTION: STBC 0x0080f4c0 _$E23
// FUNCTION: STBC 0x0080f4e0 _$E21

// FUNCTION: STBC 0x0080ebf0 _$E243
// FUNCTION: STBC 0x0080ec10 _$E240
// DATA: STBC 0x9A2708 ?rtti@NiTriStrips@@2VNiRTTI@@B
const NiRTTI NiTriStrips::rtti( "NiTriStrips", &NiTriBasedGeom::rtti );

/*
================
NiTriStrips::Register
================
*/
// FUNCTION: STBC 0x80EC30 ?Register@NiTriStrips@@SAHXZ
NiImplementRegister( NiTriStrips )

/*
================
NiTriStrips::NiTriStrips

Creates the data, which takes ownership of the arrays.
================
*/
// FUNCTION: STBC 0x80ED80 ??0NiTriStrips@@QAE@GPAVNiPoint3@@0PAVNiColorA@@PAVNiPoint2@@GGPAG3@Z
NiTriStrips::NiTriStrips( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, NiPoint2 *textures_, unsigned short triangleCount_, unsigned short stripCount_, unsigned short *stripLengths_, unsigned short *stripLists_ )
	: NiTriBasedGeom( new NiTriStripsData( vertexCount_, vertices_, normals_, colors_, textures_, triangleCount_, stripCount_, stripLengths_, stripLists_ ) ) {
}

/*
================
NiTriStrips::NiTriStrips
================
*/
// FUNCTION: STBC 0x80EE20 ??0NiTriStrips@@QAE@PAVNiTriStripsData@@@Z
NiTriStrips::NiTriStrips( NiTriStripsData *data ) : NiTriBasedGeom( data ) {
}

/*
================
NiTriStrips::NiTriStrips
================
*/
// FUNCTION: STBC 0x80EE40 ??0NiTriStrips@@QAE@XZ
NiTriStrips::NiTriStrips() {
}

/*
================
NiTriStrips::GetWorldTriangle

NULL vertices when there is no world copy.
================
*/
// FUNCTION: STBC 0x80EE60
void NiTriStrips::GetWorldTriangle( unsigned short index, NiPoint3 *&vertex0, NiPoint3 *&vertex1, NiPoint3 *&vertex2 ) {
	assert( index < GetTriangleCount() );
	if ( worldVertices ) {
		unsigned short i0, i1, i2;	// (names invented; they fit the /Od frame order)
		GetTriangleIndices( index, i0, i1, i2 );
		vertex0 = &worldVertices[i0];
		vertex1 = &worldVertices[i1];
		vertex2 = &worldVertices[i2];
	} else {
		vertex0 = 0;
		vertex1 = 0;
		vertex2 = 0;
	}
}

/*
================
NiTriStrips::GetModelTriangle
================
*/
// FUNCTION: STBC 0x80EF10
void NiTriStrips::GetModelTriangle( unsigned short index, NiPoint3 *&vertex0, NiPoint3 *&vertex1, NiPoint3 *&vertex2 ) {
	// AnimOnly.exe 0x4544C4 (NiTriStrips.cpp)
	NiTriStripsData *data = NiSmartPointerCast( NiTriStripsData, modelData );
	assert( index < data->GetTriangleCount() );
	NiPoint3 *vertices = data->GetVertices();
	unsigned short i0, i1, i2;
	GetTriangleIndices( index, i0, i1, i2 );
	vertex0 = &vertices[i0];
	vertex1 = &vertices[i1];
	vertex2 = &vertices[i2];
}

/*
================
NiTriStrips::GetTriangleIndices

The vertex indices of a triangle of the strips, in the winding of the
first: every other triangle of a strip swaps its first two. The search
for the triangle's strip resumes from the last call's when the triangles
are asked for in order on the same data.
================
*/
// FUNCTION: STBC 0x80EF80
void NiTriStrips::GetTriangleIndices( unsigned short index, unsigned short &index0, unsigned short &index1, unsigned short &index2 ) const {
	// lengths, triangles: (names invented; fit the /Od frame order)
	static unsigned short lastID = 0xffff;
	static unsigned short lastIndex = 0xffff;
	static unsigned short *strip;			// the vertex indices of the strip searched
	static unsigned short triangle;		// the triangle within that strip
	static unsigned short stripIndex;

	// AnimOnly.exe 0x454580 (NiTriStrips.cpp)
	assert( index < GetTriangleCount() );
	unsigned short id = modelData->GetID();
	NiTriStripsData *data = NiSmartPointerCast( NiTriStripsData, modelData );
	if ( lastID == id && lastIndex + 1 == index ) {
		triangle++;
	} else {
		lastID = id;
		stripIndex = 0;
		strip = data->GetStripLists();
		triangle = index;
	}

	unsigned short *lengths = data->GetStripLengths();
	unsigned short triangles;
	while ( triangle > ( triangles = lengths[stripIndex] - 2 ) ) {
		triangle -= triangles;
		strip += lengths[stripIndex];
		stripIndex++;
	}

	if ( triangle & 1 ) {
		index0 = strip[triangle + 1];
		index1 = strip[triangle];
	} else {
		index0 = strip[triangle];
		index1 = strip[triangle + 1];
	}
	index2 = strip[triangle + 2];
	lastIndex = index;
}

/*
================
NiTriStrips::Display

Draws the strips unless the camera's sorter takes them, and counts them.
================
*/
// FUNCTION: STBC 0x80F0E0
void NiTriStrips::Display( NiCamera *camera ) {
	NiRender *renderer = camera->GetRenderer();
	assert( renderer );
	if ( !renderer->RegisterObject( this ) ) {
		camera->UpdateActivePlanes();
		renderer->VirtualSlot42( worldTranslate, worldRotate, worldScale );
		renderer->VirtualSlot48( NiSmartPointerCast( NiTriStripsData, modelData ), GetWorldBound() );
		shapesDrawn++;
		trianglesDrawn += GetTriangleCount();
		verticesDrawn += GetVertexCount();
	}
}

/*
================
NiTriStrips::VirtualSlot23

Strips build no bounding box tree.
================
*/
// FUNCTION: STBC 0x80F180
void NiTriStrips::VirtualSlot23( int unknown ) {
}

/*
================
NiTriStrips::CalculateNormals

The model data's normals from its strips (AnimOnly.exe 0x454867, defined
here after VirtualSlot23).
================
*/
// FUNCTION: STBC 0x80F190
void NiTriStrips::CalculateNormals() {
	NiTriStripsData *data = NiSmartPointerCast( NiTriStripsData, modelData );
	data->CalculateNormals();
}

/*
================
NiTriStrips::CreateClone
================
*/
// FUNCTION: STBC 0x80F1A0
NiObject *NiTriStrips::CreateClone() {
	NiTriStrips *clone = new NiTriStrips;
	assert( clone );
	CopyMembers( clone );
	return clone;
}

/*
================
NiTriStrips::CopyMembers
================
*/
// FUNCTION: STBC 0x80F210
void NiTriStrips::CopyMembers( NiTriStrips *dest ) {
	NiTriBasedGeom::CopyMembers( dest );
}

/*
================
NiTriStrips::CreateObject

Creates the strips and loads them.
================
*/
// FUNCTION: STBC 0x80F220
void NiTriStrips::CreateObject( NiStream &stream, NiObject *&object ) {
	NiTriStrips *strips = new NiTriStrips;
	stream.SetLoadRecord( new NiTriStripsLoadRecord );
	strips->LoadBinary( stream );
	object = strips;
}

/*
================
NiTriStrips::LoadBinary
================
*/
// FUNCTION: STBC 0x80F340
void NiTriStrips::LoadBinary( NiStream &stream ) {
	NiTriBasedGeom::LoadBinary( stream );
}

/*
================
NiTriStrips::LinkObject
================
*/
// FUNCTION: STBC 0x80F350
void NiTriStrips::LinkObject( NiStream &stream ) {
	NiTriBasedGeom::LinkObject( stream );
}

/*
================
NiTriStrips::RegisterStreamables
================
*/
// FUNCTION: STBC 0x80F360
bool NiTriStrips::RegisterStreamables( NiStream &stream ) {
	return NiTriBasedGeom::RegisterStreamables( stream );
}

/*
================
NiTriStrips::SaveBinary
================
*/
// FUNCTION: STBC 0x80F370
void NiTriStrips::SaveBinary( NiStream &stream ) {
	NiTriBasedGeom::SaveBinary( stream );
}

/*
================
NiTriStrips::IsEqual
================
*/
// FUNCTION: STBC 0x80F380
bool NiTriStrips::IsEqual( NiObject *object ) {
	return NiTriBasedGeom::IsEqual( object );
}

/*
================
NiTriStrips::GetViewerStrings
================
*/
// FUNCTION: STBC 0x80F390
void NiTriStrips::GetViewerStrings( NiTArray<char *> *strings ) {
	NiTriBasedGeom::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
}
