// NiParticles.cpp -- particle geometry objects

#include "NiRTLib.h"
#include <assert.h>
#include "NiProcessorSpecificCode.h"
#include "NiCamera.h"
#include "NiStream.h"
#include "NiParticles.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007f55c0 _$E35
// FUNCTION: STBC 0x007f55d0 _$E50
// FUNCTION: STBC 0x007f55e0 _$E54
// FUNCTION: STBC 0x007f55f0 _$E58
// FUNCTION: STBC 0x007f5600 _$E62
// FUNCTION: STBC 0x007f5610 _$E186
// FUNCTION: STBC 0x007f5620 _$E39
// FUNCTION: STBC 0x007f5630 _$E128
// FUNCTION: STBC 0x007f5640 _$E141
// FUNCTION: STBC 0x007f5650 _$E191
// FUNCTION: STBC 0x007f5660 _$E196
// FUNCTION: STBC 0x007f5670 _$E200
// FUNCTION: STBC 0x007f5680 _$E204
// FUNCTION: STBC 0x007f5690 _$E211
// FUNCTION: STBC 0x007f56a0 _$E219
// FUNCTION: STBC 0x007f56b0 _$E226
// FUNCTION: STBC 0x007f56c0 _$E230
// FUNCTION: STBC 0x007f56d0 _$E234
// FUNCTION: STBC 0x007f56e0 _$E238
// FUNCTION: STBC 0x007f5730 _$E242

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007f55a0 _$E255
// FUNCTION: STBC 0x007f5540 _$E17
// FUNCTION: STBC 0x007f5560 _$E15
// FUNCTION: STBC 0x007f5570 _$E23
// FUNCTION: STBC 0x007f5590 _$E21

// FUNCTION: STBC 0x007f4dc0 _$E248
// FUNCTION: STBC 0x007f4de0 _$E245
// DATA: STBC 0x9A1CB0 ?rtti@NiParticles@@2VNiRTTI@@B
const NiRTTI NiParticles::rtti( "NiParticles", &NiTriBasedGeom::rtti );

/*
================
NiParticles::Register
================
*/
// FUNCTION: STBC 0x7F4E00 ?Register@NiParticles@@SAHXZ
NiImplementRegister( NiParticles )

/*
================
NiParticles::NiParticles

Creates the data, which takes ownership of the arrays.
================
*/
// FUNCTION: STBC 0x7F4F50 ??0NiParticles@@QAE@GPAVNiPoint3@@0PAVNiColorA@@@Z
NiParticles::NiParticles( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_ )
	: NiTriBasedGeom( new NiParticlesData( vertexCount_, vertices_, normals_, colors_ ) ) {
}

/*
================
NiParticles::NiParticles
================
*/
// FUNCTION: STBC 0x7F4FD0 ??0NiParticles@@QAE@PAVNiParticlesData@@@Z
NiParticles::NiParticles( NiParticlesData *data ) : NiTriBasedGeom( data ) {
}

/*
================
NiParticles::NiParticles
================
*/
// FUNCTION: STBC 0x7F4FF0 ??0NiParticles@@QAE@XZ
NiParticles::NiParticles() {
}

/*
================
NiParticles::GetWorldTriangle

A particle is a single vertex: all three corners are the world copy of
vertex index, or NULL when there is no world copy.
================
*/
// FUNCTION: STBC 0x7F5010
void NiParticles::GetWorldTriangle( unsigned short index, NiPoint3 *&vertex0, NiPoint3 *&vertex1, NiPoint3 *&vertex2 ) {
	assert( index < GetTriangleCount() );
	if ( worldVertices ) {
		vertex0 = &worldVertices[index];
		vertex1 = vertex0;
		vertex2 = vertex0;
	} else {
		vertex0 = 0;
		vertex1 = 0;
		vertex2 = 0;
	}
}

/*
================
NiParticles::GetModelTriangle

Unlike GetWorldTriangle, the original takes the model vertices three to a
triangle, as NiTriangles does, rather than the one particle vertex; the
quirk is kept.
================
*/
// FUNCTION: STBC 0x7F5070
void NiParticles::GetModelTriangle( unsigned short index, NiPoint3 *&vertex0, NiPoint3 *&vertex1, NiPoint3 *&vertex2 ) {
	NiTriBasedGeomData *pModelData = (NiTriBasedGeomData *)(NiGeometryData *)modelData;
	assert( index < pModelData->GetTriangleCount() );
	NiPoint3 *vertices = pModelData->GetVertices();
	unsigned short i = 3 * index;
	vertex0 = &vertices[i++];
	vertex1 = &vertices[i++];
	vertex2 = &vertices[i];
}

/*
================
NiParticles::Display

Draws the particles unless the camera's sorter takes them, and counts them.
================
*/
// FUNCTION: STBC 0x7F50C0
void NiParticles::Display( NiCamera *camera ) {
	NiRender *renderer = camera->GetRenderer();
	assert( renderer );
	if ( !renderer->RegisterObject( this ) ) {
		camera->UpdateActivePlanes();
		renderer->VirtualSlot42( worldTranslate, worldRotate, worldScale );
		renderer->VirtualSlot49( NiSmartPointerCast( NiParticlesData, modelData ), GetWorldBound() );
		shapesDrawn++;
		trianglesDrawn += GetActiveParticleCount();
		verticesDrawn += GetActiveParticleCount();
	}
}

/*
================
NiParticles::UpdateWorldVertices

Transforms the active particles into the world copy when the world
transform moved.
================
*/
// FUNCTION: STBC 0x7F5160
void NiParticles::UpdateWorldVertices() {
	if ( worldVertices && worldVerticesChanged ) {
		NiProcessorSpecificCode::TransformPoints( GetActiveParticleCount(), (const float *)GetVertices(), (float *)worldVertices, &worldRotate, worldScale, (const float *)&worldTranslate );
		worldVerticesChanged = false;
	}
}

/*
================
NiParticles::UpdateWorldNormals
================
*/
// FUNCTION: STBC 0x7F51B0
void NiParticles::UpdateWorldNormals() {
	if ( worldNormals && worldNormalsChanged ) {
		NiProcessorSpecificCode::TransformVectors( GetActiveParticleCount(), (const float *)GetNormals(), (float *)worldNormals, &worldRotate );
		worldNormalsChanged = false;
	}
}

/*
================
NiParticles::UpdateWorldBound

The model bound of the active particles, recomputed first when it is out
of date, carried into world space.
================
*/
// FUNCTION: STBC 0x7F51F0
void NiParticles::UpdateWorldBound() {
	NiBound &modelBound = modelData->GetBound();
	if ( modelBound.GetNeedUpdate() ) {
		modelBound.ComputeFromData( GetActiveParticleCount(), GetVertices() );
	}
	worldBound.Update( modelBound, worldRotate, worldTranslate, worldScale );
}

/*
================
NiParticles::VirtualSlot34
================
*/
// FUNCTION: STBC 0x7F5240
bool NiParticles::FindIntersections( const NiPoint3 &origin, const NiPoint3 &dir, NiPick &pick ) {
	return false;
}

/*
================
NiParticles::CreateClone
================
*/
// FUNCTION: STBC 0x7F5250
NiObject *NiParticles::CreateClone() {
	NiParticles *clone = new NiParticles;
	CopyMembers( clone );
	return clone;
}

/*
================
NiParticles::CopyMembers
================
*/
// FUNCTION: STBC 0x7F52C0
void NiParticles::CopyMembers( NiParticles *dest ) {
	NiTriBasedGeom::CopyMembers( dest );
}

/*
================
NiParticles::CreateObject

Creates the particles and loads them.
================
*/
// FUNCTION: STBC 0x7F52D0
void NiParticles::CreateObject( NiStream &stream, NiObject *&object ) {
	NiParticles *particles = new NiParticles;
	stream.SetLoadRecord( new NiParticlesLoadRecord );
	particles->LoadBinary( stream );
	object = particles;
}

/*
================
NiParticles::LoadBinary
================
*/
// FUNCTION: STBC 0x7F53F0
void NiParticles::LoadBinary( NiStream &stream ) {
	NiTriBasedGeom::LoadBinary( stream );
}

/*
================
NiParticles::LinkObject
================
*/
// FUNCTION: STBC 0x7F5400
void NiParticles::LinkObject( NiStream &stream ) {
	NiTriBasedGeom::LinkObject( stream );
}

/*
================
NiParticles::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7F5410
bool NiParticles::RegisterStreamables( NiStream &stream ) {
	return NiTriBasedGeom::RegisterStreamables( stream );
}

/*
================
NiParticles::SaveBinary
================
*/
// FUNCTION: STBC 0x7F5420
void NiParticles::SaveBinary( NiStream &stream ) {
	NiTriBasedGeom::SaveBinary( stream );
}

/*
================
NiParticles::IsEqual
================
*/
// FUNCTION: STBC 0x7F5430
bool NiParticles::IsEqual( NiObject *object ) {
	return NiTriBasedGeom::IsEqual( object );
}

/*
================
NiParticles::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7F5440
void NiParticles::GetViewerStrings( NiTArray<char *> *strings ) {
	NiTriBasedGeom::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
}
