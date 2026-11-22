/*
===============================================================================

	AnimatedTriShapeParticles.cpp

===============================================================================
*/

#include <iostream>
#include "AnimatedTriShapeParticles.h"
#include "NiCamera.h"
#include "TextureAnimManager.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004da5d0 _$E429
// FUNCTION: STBC 0x004da5f0 _$E428

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004da130 _$E32
// FUNCTION: STBC 0x004da140 _$E29
// FUNCTION: STBC 0x004da150 _$E31
// FUNCTION: STBC 0x004da160 _$E30
// FUNCTION: STBC 0x004da170 _$E38
// FUNCTION: STBC 0x004da180 _$E35
// FUNCTION: STBC 0x004da190 _$E37
// FUNCTION: STBC 0x004da1a0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004da1b0 _$E45
// FUNCTION: STBC 0x004da1c0 _$E44
// FUNCTION: STBC 0x004da1d0 _$E234
// FUNCTION: STBC 0x004da1e0 _$E233
// FUNCTION: STBC 0x004da1f0 _$E56
// FUNCTION: STBC 0x004da200 _$E55
// FUNCTION: STBC 0x004da210 _$E60
// FUNCTION: STBC 0x004da220 _$E59
// FUNCTION: STBC 0x004da230 _$E64
// FUNCTION: STBC 0x004da240 _$E63
// FUNCTION: STBC 0x004da250 _$E68
// FUNCTION: STBC 0x004da260 _$E67
// FUNCTION: STBC 0x004da270 _$E192
// FUNCTION: STBC 0x004da280 _$E191
// FUNCTION: STBC 0x004da290 _$E134
// FUNCTION: STBC 0x004da2a0 _$E133
// FUNCTION: STBC 0x004da2b0 _$E147
// FUNCTION: STBC 0x004da2c0 _$E146
// FUNCTION: STBC 0x004da2d0 _$E239
// FUNCTION: STBC 0x004da2e0 _$E238
// FUNCTION: STBC 0x004da2f0 _$E244
// FUNCTION: STBC 0x004da300 _$E243
// FUNCTION: STBC 0x004da310 _$E248
// FUNCTION: STBC 0x004da320 _$E247
// FUNCTION: STBC 0x004da330 _$E252
// FUNCTION: STBC 0x004da340 _$E251
// FUNCTION: STBC 0x004da350 _$E259
// FUNCTION: STBC 0x004da360 _$E258
// FUNCTION: STBC 0x004da370 _$E283
// FUNCTION: STBC 0x004da380 _$E282
// FUNCTION: STBC 0x004da390 _$E287
// FUNCTION: STBC 0x004da3a0 _$E286
// FUNCTION: STBC 0x004da3b0 _$E92
// FUNCTION: STBC 0x004da3c0 _$E91
// FUNCTION: STBC 0x004da3d0 _$E200
// FUNCTION: STBC 0x004da3e0 _$E199
// FUNCTION: STBC 0x004da3f0 _$E207
// FUNCTION: STBC 0x004da400 _$E206
// FUNCTION: STBC 0x004da410 _$E211
// FUNCTION: STBC 0x004da420 _$E210
// FUNCTION: STBC 0x004da430 _$E215
// FUNCTION: STBC 0x004da440 _$E214
// FUNCTION: STBC 0x004da450 _$E219
// FUNCTION: STBC 0x004da460 _$E218
// FUNCTION: STBC 0x004da470 _$E225
// FUNCTION: STBC 0x004da480 _$E224
// FUNCTION: STBC 0x004da490 _$E291
// FUNCTION: STBC 0x004da4a0 _$E290
// FUNCTION: STBC 0x004da4b0 _$E295
// FUNCTION: STBC 0x004da4c0 _$E294
// FUNCTION: STBC 0x004da4d0 _$E299
// FUNCTION: STBC 0x004da4e0 _$E298
// FUNCTION: STBC 0x004da4f0 _$E303
// FUNCTION: STBC 0x004da500 _$E302
// FUNCTION: STBC 0x004da510 _$E307
// FUNCTION: STBC 0x004da520 _$E306
// FUNCTION: STBC 0x004da530 _$E311
// FUNCTION: STBC 0x004da540 _$E310
// FUNCTION: STBC 0x004da550 _$E315
// FUNCTION: STBC 0x004da560 _$E314
// FUNCTION: STBC 0x004da570 _$E319
// FUNCTION: STBC 0x004da580 _$E318
// FUNCTION: STBC 0x004da590 _$E323
// FUNCTION: STBC 0x004da5a0 _$E322
// FUNCTION: STBC 0x004da5b0 _$E327
// FUNCTION: STBC 0x004da5c0 _$E326
// FUNCTION: STBC 0x004da600 _$E97
// FUNCTION: STBC 0x004da610 _$E96
// FUNCTION: STBC 0x004da620 _$E331
// FUNCTION: STBC 0x004da630 _$E330
// FUNCTION: STBC 0x004da640 _$E102
// FUNCTION: STBC 0x004da650 _$E101
// FUNCTION: STBC 0x004da660 _$E335
// FUNCTION: STBC 0x004da670 _$E334
// FUNCTION: STBC 0x004da680 _$E339
// FUNCTION: STBC 0x004da690 _$E338
// FUNCTION: STBC 0x004da6a0 _$E343
// FUNCTION: STBC 0x004da6b0 _$E342
// FUNCTION: STBC 0x004da6c0 _$E347
// FUNCTION: STBC 0x004da6d0 _$E346
// FUNCTION: STBC 0x004da6e0 _$E111
// FUNCTION: STBC 0x004da6f0 _$E110
// FUNCTION: STBC 0x004da700 _$E351
// FUNCTION: STBC 0x004da710 _$E350
// FUNCTION: STBC 0x004da720 _$E355
// FUNCTION: STBC 0x004da730 _$E354
// FUNCTION: STBC 0x004da740 _$E359
// FUNCTION: STBC 0x004da750 _$E358
// FUNCTION: STBC 0x004da760 _$E117
// FUNCTION: STBC 0x004da770 _$E116
// FUNCTION: STBC 0x004da780 _$E122
// FUNCTION: STBC 0x004da790 _$E121
// FUNCTION: STBC 0x004da7a0 _$E152
// FUNCTION: STBC 0x004da7b0 _$E151
// FUNCTION: STBC 0x004da7c0 _$E363
// FUNCTION: STBC 0x004da7d0 _$E362
// FUNCTION: STBC 0x004da7e0 _$E157
// FUNCTION: STBC 0x004da7f0 _$E156
// FUNCTION: STBC 0x004da800 _$E367
// FUNCTION: STBC 0x004da810 _$E366
// FUNCTION: STBC 0x004da820 _$E371
// FUNCTION: STBC 0x004da830 _$E370
// FUNCTION: STBC 0x004da840 _$E162
// FUNCTION: STBC 0x004da850 _$E161
// FUNCTION: STBC 0x004da860 _$E375
// FUNCTION: STBC 0x004da870 _$E374
// FUNCTION: STBC 0x004da880 _$E127
// FUNCTION: STBC 0x004da890 _$E126
// FUNCTION: STBC 0x004da8a0 _$E379
// FUNCTION: STBC 0x004da8b0 _$E378
// FUNCTION: STBC 0x004da8c0 _$E383
// FUNCTION: STBC 0x004da8d0 _$E382
// FUNCTION: STBC 0x004da8e0 _$E167
// FUNCTION: STBC 0x004da8f0 _$E166
// FUNCTION: STBC 0x004da900 _$E172
// FUNCTION: STBC 0x004da910 _$E171
// FUNCTION: STBC 0x004da920 _$E387
// FUNCTION: STBC 0x004da930 _$E386
// FUNCTION: STBC 0x004da940 _$E391
// FUNCTION: STBC 0x004da950 _$E390
// FUNCTION: STBC 0x004da960 _$E229
// FUNCTION: STBC 0x004da970 _$E228
// FUNCTION: STBC 0x004da980 _$E395
// FUNCTION: STBC 0x004da990 _$E394
// FUNCTION: STBC 0x004da9a0 _$E399
// FUNCTION: STBC 0x004da9b0 _$E398
// FUNCTION: STBC 0x004da9c0 _$E403
// FUNCTION: STBC 0x004da9d0 _$E402
// FUNCTION: STBC 0x004da9e0 _$E407
// FUNCTION: STBC 0x004da9f0 _$E406
// FUNCTION: STBC 0x004daa00 _$E411
// FUNCTION: STBC 0x004daa10 _$E410
// FUNCTION: STBC 0x004daa20 _$E177
// FUNCTION: STBC 0x004daa30 _$E176
// FUNCTION: STBC 0x004daa40 _$E415
// FUNCTION: STBC 0x004daa50 _$E414
// FUNCTION: STBC 0x004daa60 _$E182
// FUNCTION: STBC 0x004daa70 _$E181
// FUNCTION: STBC 0x004daa80 _$E187
// FUNCTION: STBC 0x004daa90 _$E186
// FUNCTION: STBC 0x004daaa0 _$E264
// FUNCTION: STBC 0x004daab0 _$E263
// FUNCTION: STBC 0x004daac0 _$E269
// FUNCTION: STBC 0x004daad0 _$E268
// FUNCTION: STBC 0x004daae0 _$E273
// FUNCTION: STBC 0x004daaf0 _$E272

// The standard streams' initialisers and exit-time destructors (<iostream>).

// The NiRTTI's dynamic initialiser, the registration of its exit-time destructor and the guarded destructor.
// FUNCTION: STBC 0x004d9780 _$E421
// FUNCTION: STBC 0x004d9790 _$E417
// FUNCTION: STBC 0x004d97b0 _$E420
// FUNCTION: STBC 0x004d97c0 _$E418
// DATA: STBC 0x985AA8 ?rtti@AnimatedTriShapeParticles@@2VNiRTTI@@B
const NiRTTI AnimatedTriShapeParticles::rtti( "AnimatedTriShapeParticles", &NiTriShape::rtti );

/*
================
AnimatedTriShapeParticles::AnimatedTriShapeParticles

Four vertices and two triangles per particle, none of them active yet,
over the caller's arrays.
================
*/
// FUNCTION: STBC 0x004d97e0
AnimatedTriShapeParticles::AnimatedTriShapeParticles( unsigned short particleCount, NiPoint3 *vertices, NiPoint3 *normals, NiColorA *colors, NiPoint2 *textures, unsigned short *triList, NiPoint3 *positions_, float *sizes_, NiColorA *particleColors_, unsigned short *frames_, TextureAnimTrack **tracks_ )
	: NiTriShape( new NiTriShapeDynamicData( particleCount * 4, vertices, normals, colors, textures, particleCount * 2, triList, 0, 0 ) ) {
	positions = positions_;
	sizes = sizes_;
	particleColors = particleColors_;
	frames = frames_;
	tracks = tracks_;
}

/*
================
AnimatedTriShapeParticles::SetActiveParticleCount
================
*/
// FUNCTION: STBC 0x004d98f0
void AnimatedTriShapeParticles::SetActiveParticleCount( unsigned short count ) {
	GetDynamicData()->SetActiveTriangleCount( count * 2 );
	GetDynamicData()->SetActiveVertexCount( count * 4 );
	modelData->GetBound().unknown10 = true;
}

/*
================
AnimatedTriShapeParticles::GetActiveParticleCount
================
*/
// FUNCTION: STBC 0x004d9930
unsigned short AnimatedTriShapeParticles::GetActiveParticleCount() {
	return ( (NiTriShapeDynamicData *)GetModelData() )->GetActiveTriangleCount() / 2;
}

/*
================
AnimatedTriShapeParticles::UpdateQuads

The camera's axes are brought into model space; each quad is centred half
its size behind its particle, towards the camera, and spans the size along
the camera's up and right axes. Each quad shows its particle's frame of its
track.
================
*/
// FUNCTION: STBC 0x004d9950
// PARKED: inline budget and register allocation: in the colourless loop retail expands the last quad's operator+= where this build calls it out of line, and keeps the particle count in a frame slot as TriShapeParticles::UpdateQuads does
void AnimatedTriShapeParticles::UpdateQuads( NiCamera *camera ) {
	NiTriShapeDynamicData *data = GetDynamicData();
	unsigned short count = GetActiveParticleCount();
	NiPoint3 *vertices = data->GetVertices();
	NiColorA *colors = data->GetColors();
	NiPoint3 up = camera->GetWorldUpVector() * worldRotate;
	NiPoint3 right = camera->GetWorldRightVector() * worldRotate;
	NiPoint3 direction = camera->GetWorldDirection() * worldRotate;
	unsigned short i;
	unsigned short vertex;
	if ( colors && particleColors ) {
		vertex = 0;
		for ( i = 0; i < count; i++ ) {
			float halfSize = sizes[i] * 0.5f;
			NiPoint3 upOffset = up * halfSize;
			NiPoint3 rightOffset = right * halfSize;
			NiPoint3 directionOffset = direction * halfSize;
			vertices[vertex] = positions[i];
			vertices[vertex] -= upOffset;
			vertices[vertex] -= rightOffset;
			vertices[vertex] -= directionOffset;
			colors[vertex] = particleColors[i];
			vertex++;
			vertices[vertex] = positions[i];
			vertices[vertex] -= upOffset;
			vertices[vertex] += rightOffset;
			vertices[vertex] -= directionOffset;
			colors[vertex] = particleColors[i];
			vertex++;
			vertices[vertex] = positions[i];
			vertices[vertex] += upOffset;
			vertices[vertex] += rightOffset;
			vertices[vertex] -= directionOffset;
			colors[vertex] = particleColors[i];
			vertex++;
			vertices[vertex] = positions[i];
			vertices[vertex] += upOffset;
			vertices[vertex] -= rightOffset;
			vertices[vertex] -= directionOffset;
			colors[vertex] = particleColors[i];
			vertex++;
		}
	} else {
		vertex = 0;
		for ( i = 0; i < count; i++ ) {
			float halfSize = sizes[i] * 0.5f;
			NiPoint3 upOffset = up * halfSize;
			NiPoint3 rightOffset = right * halfSize;
			NiPoint3 directionOffset = direction * halfSize;
			vertices[vertex] = positions[i];
			vertices[vertex] -= upOffset;
			vertices[vertex] -= rightOffset;
			vertices[vertex] -= directionOffset;
			vertex++;
			vertices[vertex] = positions[i];
			vertices[vertex] -= upOffset;
			vertices[vertex] += rightOffset;
			vertices[vertex] -= directionOffset;
			vertex++;
			vertices[vertex] = positions[i];
			vertices[vertex] += upOffset;
			vertices[vertex] += rightOffset;
			vertices[vertex] -= directionOffset;
			vertex++;
			vertices[vertex] = positions[i];
			vertices[vertex] += upOffset;
			vertices[vertex] -= rightOffset;
			vertices[vertex] -= directionOffset;
			vertex++;
		}
	}
	NiPoint2 *textures = data->GetTextures();
	if ( textures ) {
		for ( i = 0; i < count; i++ ) {
			tracks[i]->GetFrameCorners( frames[i], &textures[i * 4] );
		}
	}
	modelData->IncRevisionID();
}

/*
================
AnimatedTriShapeParticles::VirtualSlot35
================
*/
// FUNCTION: STBC 0x004da0c0
void AnimatedTriShapeParticles::VirtualSlot35( NiCamera *camera ) {
	NiGeometry::VirtualSlot35( camera );
	UpdateQuads( camera );
}

/*
================
AnimatedTriShapeParticles::UpdateWorldBound
================
*/
// FUNCTION: STBC 0x004da0e0
void AnimatedTriShapeParticles::UpdateWorldBound() {
	NiBound &modelBound = modelData->GetBound();
	if ( modelBound.unknown10 ) {
		modelBound.ComputeFromData( GetActiveParticleCount(), GetPositions() );
	}
	worldBound.Update( modelBound, worldRotate, worldTranslate, worldScale );
}
