/*
===============================================================================

	TriShapeOrientedParticles.cpp

===============================================================================
*/

#include <iostream>
#include "TriShapeOrientedParticles.h"
#include "NiCamera.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004fb380 _$E289
// FUNCTION: STBC 0x004fb3a0 _$E288

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004fb2c0 _$E32
// FUNCTION: STBC 0x004fb2d0 _$E29
// FUNCTION: STBC 0x004fb2e0 _$E31
// FUNCTION: STBC 0x004fb2f0 _$E30
// FUNCTION: STBC 0x004fb300 _$E38
// FUNCTION: STBC 0x004fb310 _$E35
// FUNCTION: STBC 0x004fb320 _$E37
// FUNCTION: STBC 0x004fb330 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004fb340 _$E45
// FUNCTION: STBC 0x004fb350 _$E44
// FUNCTION: STBC 0x004fb360 _$E56
// FUNCTION: STBC 0x004fb370 _$E55
// FUNCTION: STBC 0x004fb3b0 _$E60
// FUNCTION: STBC 0x004fb3c0 _$E59
// FUNCTION: STBC 0x004fb3d0 _$E64
// FUNCTION: STBC 0x004fb3e0 _$E63
// FUNCTION: STBC 0x004fb3f0 _$E68
// FUNCTION: STBC 0x004fb400 _$E67
// FUNCTION: STBC 0x004fb410 _$E192
// FUNCTION: STBC 0x004fb420 _$E191
// FUNCTION: STBC 0x004fb430 _$E200
// FUNCTION: STBC 0x004fb440 _$E199
// FUNCTION: STBC 0x004fb460 _$E207
// FUNCTION: STBC 0x004fb470 _$E206
// FUNCTION: STBC 0x004fb480 _$E211
// FUNCTION: STBC 0x004fb490 _$E210
// FUNCTION: STBC 0x004fb4a0 _$E215
// FUNCTION: STBC 0x004fb4b0 _$E214
// FUNCTION: STBC 0x004fb4c0 _$E219
// FUNCTION: STBC 0x004fb4d0 _$E218
// FUNCTION: STBC 0x004fb4e0 _$E225
// FUNCTION: STBC 0x004fb4f0 _$E224
// FUNCTION: STBC 0x004fb590 _$E229
// FUNCTION: STBC 0x004fb5a0 _$E228
// FUNCTION: STBC 0x004fb5d0 _$E237
// FUNCTION: STBC 0x004fb5e0 _$E236
// FUNCTION: STBC 0x004fb5f0 _$E134
// FUNCTION: STBC 0x004fb600 _$E133
// FUNCTION: STBC 0x004fb610 _$E147
// FUNCTION: STBC 0x004fb620 _$E146
// FUNCTION: STBC 0x004fb650 _$E247
// FUNCTION: STBC 0x004fb660 _$E246
// FUNCTION: STBC 0x004fb690 _$E255
// FUNCTION: STBC 0x004fb6a0 _$E254
// FUNCTION: STBC 0x004fb6b0 _$E262
// FUNCTION: STBC 0x004fb6c0 _$E261

// The class's NiRTTI: its dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x004fa3a0 _$E268
// FUNCTION: STBC 0x004fa3b0 _$E264
// FUNCTION: STBC 0x004fa3d0 _$E267
// FUNCTION: STBC 0x004fa3e0 _$E265
// DATA: STBC 0x986B40 ?rtti@TriShapeOrientedParticlesData@@2VNiRTTI@@B
const NiRTTI TriShapeOrientedParticlesData::rtti( "TriShapeOrientedParticlesData", &TriShapeParticlesData::rtti );

// The class's NiRTTI: its dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x004fa400 _$E273
// FUNCTION: STBC 0x004fa410 _$E270
// FUNCTION: STBC 0x004fa430 _$E272
// FUNCTION: STBC 0x004fa440 _$E271
// DATA: STBC 0x986B08 ?rtti@TriShapeOrientedParticles@@2VNiRTTI@@B
const NiRTTI TriShapeOrientedParticles::rtti( "TriShapeOrientedParticles", &TriShapeParticles::rtti );

/*
================
TriShapeOrientedParticles::TriShapeOrientedParticles

Builds the data for the particle count, with the per-particle arrays the
flags ask for.
================
*/
// FUNCTION: STBC 0x004fa460 ??0TriShapeOrientedParticles@@QAE@G_N000@Z
// PARKED: schedule: retail stores the vptr before the two size stores, this build after them (body, init list, setter and order tried)
TriShapeOrientedParticles::TriShapeOrientedParticles( unsigned short particleCount, bool hasNormals, bool hasColors, bool hasDimensions, bool hasTextures )
	: TriShapeParticles( new TriShapeOrientedParticlesData( particleCount,
		hasNormals ? new NiPoint3[particleCount] : NULL,
		hasColors ? new NiColorA[particleCount] : NULL,
		hasTextures ? new NiPoint2[particleCount * 4] : NULL,
		hasDimensions ? new NiPoint2[particleCount] : NULL ) ) {
	width = 1.0f;
	length = 1.0f;
}

/*
================
TriShapeOrientedParticlesData::TriShapeOrientedParticlesData

TriShapeParticlesData's without sizes, and a direction per particle.
Takes ownership of the arrays.
================
*/
// FUNCTION: STBC 0x004fa5f0
TriShapeOrientedParticlesData::TriShapeOrientedParticlesData( unsigned short particleCount, NiPoint3 *particleNormals_, NiColorA *particleColors_, NiPoint2 *textures_, NiPoint2 *dimensions_ )
	: TriShapeParticlesData( particleCount, particleNormals_, particleColors_, textures_, NULL ),
	dimensions( dimensions_ ), directions( new NiPoint3[particleCount] ) {
}

/*
================
TriShapeOrientedParticles::TriShapeOrientedParticles
================
*/
// FUNCTION: STBC 0x004fa710 ??0TriShapeOrientedParticles@@QAE@PAVTriShapeOrientedParticlesData@@@Z
TriShapeOrientedParticles::TriShapeOrientedParticles( TriShapeOrientedParticlesData *data ) : TriShapeParticles( data ) {
	width = 1.0f;
	length = 1.0f;
}

/*
================
TriShapeOrientedParticles::CreateClone
================
*/
// FUNCTION: STBC 0x004fa740
NiObject *TriShapeOrientedParticles::CreateClone() {
	TriShapeOrientedParticles *clone = new TriShapeOrientedParticles( NULL );
	CopyMembers( clone );
	return clone;
}

/*
================
TriShapeOrientedParticles::CopyMembers
================
*/
// FUNCTION: STBC 0x004fa7b0
void TriShapeOrientedParticles::CopyMembers( TriShapeOrientedParticles *dest ) {
	TriShapeParticles::CopyMembers( dest );
	dest->width = width;
	dest->length = length;
}

/*
================
TriShapeOrientedParticles::CreateData
================
*/
// FUNCTION: STBC 0x004fa7e0
TriShapeParticlesData *TriShapeOrientedParticles::CreateData() {
	unsigned int count = GetParticleCount();
	return new TriShapeOrientedParticlesData( count,
		GetParticleNormals() ? new NiPoint3[count] : NULL,
		GetParticleColors() ? new NiColorA[count] : NULL,
		modelData->GetTextures() ? new NiPoint2[count * 4] : NULL,
		GetDimensions() ? new NiPoint2[count] : NULL );
}

/*
================
TriShapeOrientedParticles::GetDirections
================
*/
// FUNCTION: STBC 0x004fa940
NiPoint3 *TriShapeOrientedParticles::GetDirections() {
	return GetOrientedData()->directions;
}

/*
================
TriShapeOrientedParticles::GetDimensions
================
*/
// FUNCTION: STBC 0x004fa950
NiPoint2 *TriShapeOrientedParticles::GetDimensions() {
	return GetOrientedData()->dimensions;
}

/*
================
TriShapeOrientedParticles::UpdateQuads

The camera's axes are brought into model space. Each quad starts at its
particle and runs its length along the particle's direction; its width
lies across the direction, turned to face the camera (the cross product of
the direction with the way to the camera). A direction pointing at the
camera leaves the quad collapsed on its particle.
================
*/
// FUNCTION: STBC 0x004fa960
// DRAFT: call set as retail's (the direction column set by components: budget 319 at the colour loop, in retail's [317,333)); the direction column's copy and the side's x87 products are scheduled elsewhere
void TriShapeOrientedParticles::UpdateQuads( NiCamera *camera ) {
	unsigned short count = GetActiveParticleCount();
	if ( count == 0 ) {
		return;
	}
	TriShapeOrientedParticlesData *data = GetOrientedData();
	NiPoint3 *directions = data->directions;
	NiPoint2 *dimensions = data->dimensions;
	NiPoint3 *positions = data->positions;
	NiPoint3 *vertices = data->GetVertices();
	NiPoint3 direction = camera->GetWorldDirection() * worldRotate;
	// BUG: redundant: the camera's up and right axes are computed as in TriShapeParticles::UpdateQuads and never used
	NiPoint3 up = camera->GetWorldUpVector() * worldRotate;
	NiPoint3 right = camera->GetWorldRightVector() * worldRotate;
	unsigned short i;
	unsigned short vertex;
	if ( dimensions ) {
		vertex = 0;
		for ( i = 0; i < count; i++ ) {
			NiPoint3 side = directions[i].Cross( -direction );
			float sideLength = side.Length();
			if ( sideLength > 1e-06f ) {
				side *= 1.0f / sideLength;
				NiPoint3 normal = side.Cross( directions[i] );
				NiMatrix3 rotation;
				rotation.SetCol( 0, directions[i].x, directions[i].y, directions[i].z );
				rotation.SetCol( 1, side );
				rotation.SetCol( 2, normal );
				NiPoint3 lengthOffset = rotation * NiPoint3( dimensions[i].y, 0.0f, 0.0f );
				NiPoint3 widthOffset = rotation * NiPoint3( 0.0f, dimensions[i].x * 0.5f, 0.0f );
				vertices[vertex] = positions[i];
				vertices[vertex] -= widthOffset;
				vertex++;
				vertices[vertex] = positions[i];
				vertices[vertex] += widthOffset;
				vertex++;
				vertices[vertex] = positions[i];
				vertices[vertex] += lengthOffset;
				vertices[vertex] += widthOffset;
				vertex++;
				vertices[vertex] = positions[i];
				vertices[vertex] += lengthOffset;
				vertices[vertex] -= widthOffset;
				vertex++;
			} else {
				vertices[vertex++] = positions[i];
				vertices[vertex++] = positions[i];
				vertices[vertex++] = positions[i];
				vertices[vertex++] = positions[i];
			}
		}
	} else {
		NiPoint3 lengthAxis( length, 0.0f, 0.0f );
		NiPoint3 widthAxis( 0.0f, width * 0.5f, 0.0f );
		vertex = 0;
		for ( i = 0; i < count; i++ ) {
			NiPoint3 side = directions[i].Cross( -direction );
			float sideLength = side.Length();
			if ( sideLength > 1e-06f ) {
				side *= 1.0f / sideLength;
				NiPoint3 normal = side.Cross( directions[i] );
				NiMatrix3 rotation;
				rotation.SetCol( 0, directions[i].x, directions[i].y, directions[i].z );
				rotation.SetCol( 1, side );
				rotation.SetCol( 2, normal );
				NiPoint3 lengthOffset = rotation * lengthAxis;
				NiPoint3 widthOffset = rotation * widthAxis;
				vertices[vertex] = positions[i];
				vertices[vertex] -= widthOffset;
				vertex++;
				vertices[vertex] = positions[i];
				vertices[vertex] += widthOffset;
				vertex++;
				vertices[vertex] = positions[i];
				vertices[vertex] += lengthOffset;
				vertices[vertex] += widthOffset;
				vertex++;
				vertices[vertex] = positions[i];
				vertices[vertex] += lengthOffset;
				vertices[vertex] -= widthOffset;
				vertex++;
			} else {
				vertices[vertex++] = positions[i];
				vertices[vertex++] = positions[i];
				vertices[vertex++] = positions[i];
				vertices[vertex++] = positions[i];
			}
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
