// NiTriangles.cpp -- triangle list geometry objects without an index list

#include "NiRTLib.h"
#include "NiOBBRoot.h"
#include "NiCamera.h"
#include "NiStream.h"
#include "NiTriangles.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0080c0d0 _$E37
// FUNCTION: STBC 0x0080c0e0 _$E52
// FUNCTION: STBC 0x0080c0f0 _$E56
// FUNCTION: STBC 0x0080c100 _$E60
// FUNCTION: STBC 0x0080c110 _$E64
// FUNCTION: STBC 0x0080c120 _$E181
// FUNCTION: STBC 0x0080c130 _$E41
// FUNCTION: STBC 0x0080c140 _$E123
// FUNCTION: STBC 0x0080c150 _$E136
// FUNCTION: STBC 0x0080c160 _$E186
// FUNCTION: STBC 0x0080c170 _$E191
// FUNCTION: STBC 0x0080c180 _$E195
// FUNCTION: STBC 0x0080c190 _$E199
// FUNCTION: STBC 0x0080c1a0 _$E206
// FUNCTION: STBC 0x0080c1b0 _$E214
// FUNCTION: STBC 0x0080c1c0 _$E221
// FUNCTION: STBC 0x0080c1d0 _$E225
// FUNCTION: STBC 0x0080c1e0 _$E229
// FUNCTION: STBC 0x0080c1f0 _$E233
// FUNCTION: STBC 0x0080c240 _$E237

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x0080c0b0 _$E250
// FUNCTION: STBC 0x0080c050 _$E17
// FUNCTION: STBC 0x0080c070 _$E15
// FUNCTION: STBC 0x0080c080 _$E23
// FUNCTION: STBC 0x0080c0a0 _$E21

// FUNCTION: STBC 0x0080b880 _$E243
// FUNCTION: STBC 0x0080b8a0 _$E240
// DATA: STBC 0x9A2598 ?rtti@NiTriangles@@2VNiRTTI@@B
const NiRTTI NiTriangles::rtti( "NiTriangles", &NiTriBasedGeom::rtti );

/*
================
NiTriangles::Register
================
*/
// FUNCTION: STBC 0x80B8C0 ?Register@NiTriangles@@SAHXZ
NiImplementRegister( NiTriangles )

/*
================
NiTriangles::NiTriangles

Creates the data, which takes ownership of the arrays.
================
*/
// FUNCTION: STBC 0x80BA10 ??0NiTriangles@@QAE@PAVNiPoint3@@0PAVNiColorA@@PAVNiPoint2@@G@Z
NiTriangles::NiTriangles( NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, NiPoint2 *textures_, unsigned short triangleCount_ )
	: NiTriBasedGeom( new NiTrianglesData( vertices_, normals_, colors_, textures_, triangleCount_ ) ) {
}

/*
================
NiTriangles::NiTriangles
================
*/
// FUNCTION: STBC 0x80BAA0 ??0NiTriangles@@QAE@PAVNiTrianglesData@@@Z
NiTriangles::NiTriangles( NiTrianglesData *data ) : NiTriBasedGeom( data ) {
}

/*
================
NiTriangles::NiTriangles
================
*/
// FUNCTION: STBC 0x80BAC0 ??0NiTriangles@@QAE@XZ
NiTriangles::NiTriangles() {
}

/*
================
NiTriangles::GetTriangleIndices

Triangle i is made of the vertices 3i, 3i+1 and 3i+2.
================
*/
// FUNCTION: STBC 0x80BAE0
void NiTriangles::GetTriangleIndices( unsigned short index, unsigned short &index0, unsigned short &index1, unsigned short &index2 ) const {
	index0 = 3 * index;
	index1 = index0 + 1;
	index2 = index1 + 1;
}

/*
================
NiTriangles::GetWorldTriangle

NULL vertices when there is no world copy.
================
*/
// FUNCTION: STBC 0x80BB10
void NiTriangles::GetWorldTriangle( unsigned short index, NiPoint3 *&vertex0, NiPoint3 *&vertex1, NiPoint3 *&vertex2 ) {
	assert( index < GetTriangleCount() );
	if ( worldVertices ) {
		unsigned short i = 3 * index;
		vertex0 = &worldVertices[i++];
		vertex1 = &worldVertices[i++];
		vertex2 = &worldVertices[i];
	} else {
		vertex0 = 0;
		vertex1 = 0;
		vertex2 = 0;
	}
}

/*
================
NiTriangles::GetModelTriangle
================
*/
// FUNCTION: STBC 0x80BB90
void NiTriangles::GetModelTriangle( unsigned short index, NiPoint3 *&vertex0, NiPoint3 *&vertex1, NiPoint3 *&vertex2 ) {
	// AnimOnly.exe 0x45097B (NiTriangles.cpp)
	assert( index < NiSmartPointerCast( NiTrianglesData, modelData )->GetTriangleCount() );
	NiPoint3 *vertices = modelData->GetVertices();
	unsigned short i = 3 * index;
	vertex0 = &vertices[i++];
	vertex1 = &vertices[i++];
	vertex2 = &vertices[i];
}

/*
================
NiTriangles::Display

Draws the triangles unless the camera's sorter takes them, and counts them.
================
*/
// FUNCTION: STBC 0x80BBE0
void NiTriangles::Display( NiCamera *camera ) {
	NiRender *renderer = camera->GetRenderer();
	assert( renderer );
	if ( !renderer->RegisterObject( this ) ) {
		camera->UpdateActivePlanes();
		renderer->VirtualSlot42( worldTranslate, worldRotate, worldScale );
		renderer->VirtualSlot46( NiSmartPointerCast( NiTrianglesData, modelData ), GetWorldBound() );
		shapesDrawn++;
		trianglesDrawn += GetActiveTriangleCount();
		verticesDrawn += GetActiveVertexCount();
	}
}

/*
================
NiTriangles::VirtualSlot23

Builds the bounding box tree once, over the model and world vertices,
from a temporary index list of the vertices in order.
The counts and the world vertices come through NiGeometry's and
NiTriBasedGeom's accessors, as the SDK's debug NetImmerse 3.1 reads them
(AnimOnly.exe 0x450B32).
================
*/
// FUNCTION: STBC 0x80BC90
void NiTriangles::VirtualSlot23( int unknown ) {
	unsigned short indexCount = 3 * GetTriangleCount();
	unsigned short *triList = new unsigned short[indexCount];
	assert( triList );
	for ( unsigned short i = 0; i < indexCount; i++ ) {
		triList[i] = i;
	}

	if ( !obbRoot ) {
		obbRoot = new NiOBBRoot( GetTriangleCount(), triList, GetVertices(), GetWorldVertices(), unknown );
		assert( obbRoot );
	}

	delete [] triList;
}

/*
================
NiTriangles::CreateClone
================
*/
// FUNCTION: STBC 0x80BD60
NiObject *NiTriangles::CreateClone() {
	NiTriangles *clone = new NiTriangles;
	CopyMembers( clone );
	return clone;
}

/*
================
NiTriangles::CopyMembers
================
*/
// FUNCTION: STBC 0x80BDD0
void NiTriangles::CopyMembers( NiTriangles *dest ) {
	NiTriBasedGeom::CopyMembers( dest );
}

/*
================
NiTriangles::CreateObject

Creates the triangles and loads them.
================
*/
// FUNCTION: STBC 0x80BDE0
void NiTriangles::CreateObject( NiStream &stream, NiObject *&object ) {
	NiTriangles *triangles = new NiTriangles;
	stream.SetLoadRecord( new NiTrianglesLoadRecord );
	triangles->LoadBinary( stream );
	object = triangles;
}

/*
================
NiTriangles::LoadBinary
================
*/
// FUNCTION: STBC 0x80BF00
void NiTriangles::LoadBinary( NiStream &stream ) {
	NiTriBasedGeom::LoadBinary( stream );
}

/*
================
NiTriangles::LinkObject
================
*/
// FUNCTION: STBC 0x80BF10
void NiTriangles::LinkObject( NiStream &stream ) {
	NiTriBasedGeom::LinkObject( stream );
}

/*
================
NiTriangles::RegisterStreamables
================
*/
// FUNCTION: STBC 0x80BF20
bool NiTriangles::RegisterStreamables( NiStream &stream ) {
	return NiTriBasedGeom::RegisterStreamables( stream );
}

/*
================
NiTriangles::SaveBinary
================
*/
// FUNCTION: STBC 0x80BF30
void NiTriangles::SaveBinary( NiStream &stream ) {
	NiTriBasedGeom::SaveBinary( stream );
}

/*
================
NiTriangles::IsEqual
================
*/
// FUNCTION: STBC 0x80BF40
bool NiTriangles::IsEqual( NiObject *object ) {
	return NiTriBasedGeom::IsEqual( object );
}

/*
================
NiTriangles::GetViewerStrings
================
*/
// FUNCTION: STBC 0x80BF50
void NiTriangles::GetViewerStrings( NiTArray<char *> *strings ) {
	NiTriBasedGeom::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
}
