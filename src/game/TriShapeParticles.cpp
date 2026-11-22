/*
===============================================================================

	TriShapeParticles.cpp

===============================================================================
*/

#include <string.h>

#include "TriShapeParticles.h"
#include "NiCamera.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The class's NiRTTI: its dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x004f93f0 _$E255
// FUNCTION: STBC 0x004f9400 _$E251
// FUNCTION: STBC 0x004f9420 _$E254
// FUNCTION: STBC 0x004f9430 _$E252
// DATA: STBC 0x986B50 ?rtti@TriShapeParticlesData@@2VNiRTTI@@B
const NiRTTI TriShapeParticlesData::rtti( "TriShapeParticlesData", &NiTriShapeDynamicData::rtti );

// The class's NiRTTI: its dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x004f9450 _$E260
// FUNCTION: STBC 0x004f9460 _$E257
// FUNCTION: STBC 0x004f9480 _$E259
// FUNCTION: STBC 0x004f9490 _$E258
// DATA: STBC 0x986B30 ?rtti@TriShapeParticles@@2VNiRTTI@@B
const NiRTTI TriShapeParticles::rtti( "TriShapeParticles", &NiTriShape::rtti );

/*
================
TriShapeParticles::TriShapeParticles

Builds the data for the particle count, with the per-particle arrays the
flags ask for.
================
*/
// FUNCTION: STBC 0x004f94b0 ??0TriShapeParticles@@QAE@G_N000@Z
TriShapeParticles::TriShapeParticles( unsigned short particleCount, bool hasNormals, bool hasColors, bool hasSizes, bool hasTextures )
	: NiTriShape( new TriShapeParticlesData( particleCount,
		hasNormals ? new NiPoint3[particleCount] : NULL,
		hasColors ? new NiColorA[particleCount] : NULL,
		hasTextures ? new NiPoint2[particleCount * 4] : NULL,
		hasSizes ? new float[particleCount] : NULL ) ) {
	size = 1.0f;
	BuildTriangleList();
}

/*
================
TriShapeParticlesData::TriShapeParticlesData

Four vertices and two triangles per particle, none of them active yet.
Takes ownership of the arrays.
================
*/
// FUNCTION: STBC 0x004f9630
TriShapeParticlesData::TriShapeParticlesData( unsigned short particleCount, NiPoint3 *particleNormals_, NiColorA *particleColors_, NiPoint2 *textures_, float *sizes_ )
	: NiTriShapeDynamicData( particleCount * 4, new NiPoint3[particleCount * 4],
		particleNormals_ ? new NiPoint3[particleCount * 4] : NULL,
		particleColors_ ? new NiColorA[particleCount * 4] : NULL,
		textures_, particleCount * 2, new unsigned short[particleCount * 6], 0, 0 ),
	sizes( sizes_ ), positions( new NiPoint3[particleCount] ), particleColors( particleColors_ ), particleNormals( particleNormals_ ) {
}

/*
================
TriShapeParticles::GetParticleCount
================
*/
// FUNCTION: STBC 0x004f9810
unsigned int TriShapeParticles::GetParticleCount() {
	return modelData->GetVertexCount() / 4;
}

/*
================
TriShapeParticles::TriShapeParticles
================
*/
// FUNCTION: STBC 0x004f9830 ??0TriShapeParticles@@QAE@PAVTriShapeParticlesData@@@Z
TriShapeParticles::TriShapeParticles( TriShapeParticlesData *data ) : NiTriShape( data ) {
	if ( data ) {
		BuildTriangleList();
	}
	size = 1.0f;
}

/*
================
TriShapeParticles::CreateClone
================
*/
// FUNCTION: STBC 0x004f9890
NiObject *TriShapeParticles::CreateClone() {
	TriShapeParticles *clone = new TriShapeParticles( NULL );
	CopyMembers( clone );
	return clone;
}

/*
================
TriShapeParticles::CopyMembers

The copy gets new data of the same shape; only the texture coordinates are
copied over.
================
*/
// FUNCTION: STBC 0x004f9900
void TriShapeParticles::CopyMembers( TriShapeParticles *dest ) {
	NiTriShape::CopyMembers( dest );
	unsigned int count = GetParticleCount();
	dest->SetModelData( CreateData() );
	dest->size = size;
	// BUG: builds this shape's triangle list, not the copy's, which keeps the new data's uninitialised list
	BuildTriangleList();
	if ( modelData->GetTextures() && dest->modelData->GetTextures() ) {
		// BUG: copies one coordinate per particle where each has four
		memcpy( dest->modelData->GetTextures(), modelData->GetTextures(), count * sizeof( NiPoint2 ) );
	}
	BuildTriangleList();
}

/*
================
TriShapeParticles::CreateData
================
*/
// FUNCTION: STBC 0x004f9980
TriShapeParticlesData *TriShapeParticles::CreateData() {
	unsigned int count = GetParticleCount();
	return new TriShapeParticlesData( count,
		GetParticleNormals() ? new NiPoint3[count] : NULL,
		GetParticleColors() ? new NiColorA[count] : NULL,
		modelData->GetTextures() ? new NiPoint2[count * 4] : NULL,
		GetSizes() ? new float[count] : NULL );
}

/*
================
TriShapeParticles::BuildTriangleList
================
*/
// FUNCTION: STBC 0x004f9ad0
void TriShapeParticles::BuildTriangleList() {
	TriShapeParticlesData *data = GetParticleData();
	unsigned short *triList = data->GetTriList();
	int length = data->GetTriListLength();
	unsigned short vertex = 0;
	for ( int i = 0; i < length; i += 6 ) {
		triList[i] = vertex;
		triList[i + 1] = vertex + 1;
		triList[i + 2] = vertex + 2;
		triList[i + 3] = vertex;
		triList[i + 4] = vertex + 2;
		triList[i + 5] = vertex + 3;
		vertex += 4;
	}
}

/*
================
TriShapeParticles::SetTextureCoordinates

Gives each particle's quad the whole texture: its four corners take (0, 1),
(1, 1), (1, 0) and (0, 0) in turn.
================
*/
// FUNCTION: STBC 0x004f9b30
void TriShapeParticles::SetTextureCoordinates() {
	NiPoint2 *textures = modelData->GetTextures();
	if ( textures ) {
		int i = 0;
		int count = modelData->GetVertexCount();
		while ( i < count ) {
			textures[i++] = NiPoint2::UNIT_Y;
			textures[i++] = NiPoint2::UNIT_Y + NiPoint2::UNIT_X;
			textures[i++] = NiPoint2::UNIT_X;
			textures[i++] = NiPoint2::ZERO;
		}
	}
}

/*
================
TriShapeParticles::SetActiveParticleCount
================
*/
// FUNCTION: STBC 0x004f9be0
void TriShapeParticles::SetActiveParticleCount( unsigned short count ) {
	GetParticleData()->SetActiveTriangleCount( count * 2 );
	GetParticleData()->SetActiveVertexCount( count * 4 );
	modelData->GetBound().unknown10 = true;
}

/*
================
TriShapeParticles::GetActiveParticleCount
================
*/
// FUNCTION: STBC 0x004f9c20
unsigned short TriShapeParticles::GetActiveParticleCount() {
	return ( (TriShapeParticlesData *)GetModelData() )->GetActiveTriangleCount() / 2;
}

/*
================
TriShapeParticles::GetPositions
================
*/
// FUNCTION: STBC 0x004f9c40
NiPoint3 *TriShapeParticles::GetPositions() {
	return GetParticleData()->positions;
}

/*
================
TriShapeParticles::GetSizes
================
*/
// FUNCTION: STBC 0x004f9c50
float *TriShapeParticles::GetSizes() {
	return GetParticleData()->sizes;
}

/*
================
TriShapeParticles::GetParticleColors
================
*/
// FUNCTION: STBC 0x004f9c60
NiColorA *TriShapeParticles::GetParticleColors() {
	return GetParticleData()->particleColors;
}

/*
================
TriShapeParticles::GetParticleNormals
================
*/
// FUNCTION: STBC 0x004f9c70
NiPoint3 *TriShapeParticles::GetParticleNormals() {
	return GetParticleData()->particleNormals;
}

/*
================
TriShapeParticles::UpdateQuads

The camera's axes are brought into model space; each quad is centred half
its size behind its particle, towards the camera, and spans the size
along the camera's up and right axes.
================
*/
// FUNCTION: STBC 0x004f9c80
// PARKED: register allocation: retail keeps the camera in EBX and the particle count in a frame slot, this build keeps the count in EBP; the body otherwise matches (declaration order and statement order tried)
void TriShapeParticles::UpdateQuads( NiCamera *camera ) {
	unsigned short count = GetActiveParticleCount();
	if ( count == 0 ) {
		return;
	}
	TriShapeParticlesData *data = GetParticleData();
	NiPoint3 *positions = data->positions;
	float *sizes = data->sizes;
	NiPoint3 *vertices = data->GetVertices();
	NiPoint3 up = camera->GetWorldUpVector() * worldRotate;
	NiPoint3 right = camera->GetWorldRightVector() * worldRotate;
	NiPoint3 direction = camera->GetWorldDirection() * worldRotate;
	unsigned short i;
	unsigned short vertex;
	if ( sizes ) {
		vertex = 0;
		for ( i = 0; i < count; i++ ) {
			float halfSize = sizes[i] * 0.5f;
			NiPoint3 upOffset = up * halfSize;
			NiPoint3 rightOffset = right * halfSize;
			NiPoint3 center = positions[i] - direction * halfSize;
			vertices[vertex] = center;
			vertices[vertex] -= upOffset;
			vertices[vertex] -= rightOffset;
			vertex++;
			vertices[vertex] = center;
			vertices[vertex] -= upOffset;
			vertices[vertex] += rightOffset;
			vertex++;
			vertices[vertex] = center;
			vertices[vertex] += upOffset;
			vertices[vertex] += rightOffset;
			vertex++;
			vertices[vertex] = center;
			vertices[vertex] += upOffset;
			vertices[vertex] -= rightOffset;
			vertex++;
		}
	} else {
		float halfSize = size * 0.5f;
		NiPoint3 upOffset = up * halfSize;
		NiPoint3 rightOffset = right * halfSize;
		NiPoint3 directionOffset = direction * halfSize;
		vertex = 0;
		for ( i = 0; i < count; i++ ) {
			NiPoint3 center = positions[i] - directionOffset;
			vertices[vertex] = center;
			vertices[vertex] -= upOffset;
			vertices[vertex] -= rightOffset;
			vertex++;
			vertices[vertex] = center;
			vertices[vertex] -= upOffset;
			vertices[vertex] += rightOffset;
			vertex++;
			vertices[vertex] = center;
			vertices[vertex] += upOffset;
			vertices[vertex] += rightOffset;
			vertex++;
			vertices[vertex] = center;
			vertices[vertex] += upOffset;
			vertices[vertex] -= rightOffset;
			vertex++;
		}
	}
	NiColorA *particleColors = data->particleColors;
	NiColorA *colors = data->GetColors();
	if ( particleColors && colors ) {
		vertex = 0;
		for ( i = 0; i < count; i++ ) {
			colors[vertex++] = particleColors[i];
			colors[vertex++] = particleColors[i];
			colors[vertex++] = particleColors[i];
			colors[vertex++] = particleColors[i];
		}
	}
	NiPoint3 *particleNormals = data->particleNormals;
	NiPoint3 *normals = data->GetNormals();
	if ( particleNormals && normals ) {
		vertex = 0;
		for ( i = 0; i < count; i++ ) {
			normals[vertex++] = particleNormals[i];
			normals[vertex++] = particleNormals[i];
			normals[vertex++] = particleNormals[i];
			normals[vertex++] = particleNormals[i];
		}
	}
	modelData->IncRevisionID();
}

/*
================
TriShapeParticles::InvalidateBound
================
*/
// FUNCTION: STBC 0x004fa320
void TriShapeParticles::InvalidateBound() {
	modelData->GetBound().unknown10 = true;
}

/*
================
TriShapeParticles::VirtualSlot35
================
*/
// FUNCTION: STBC 0x004fa330
void TriShapeParticles::VirtualSlot35( NiCamera *camera ) {
	UpdateQuads( camera );
	NiGeometry::VirtualSlot35( camera );
}

/*
================
TriShapeParticles::UpdateWorldBound
================
*/
// FUNCTION: STBC 0x004fa350
void TriShapeParticles::UpdateWorldBound() {
	NiBound &modelBound = modelData->GetBound();
	if ( modelBound.unknown10 ) {
		modelBound.ComputeFromData( GetActiveParticleCount(), GetPositions() );
	}
	worldBound.Update( modelBound, worldRotate, worldTranslate, worldScale );
}
