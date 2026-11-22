/*
===============================================================================

	AnimTSParticleController.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "AnimTSParticleController.h"
#include "AnimatedTriShapeParticles.h"
#include "TextureAnimManager.h"
#include "TGSystemWrapperClass.h"
#include "NiAlphaProperty.h"
#include "NiTextureModeProperty.h"
#include "NiTextureProperty.h"
#include "NiVertexColorProperty.h"
#include "NiZBufferProperty.h"
#include "NiMain.h"
#include "NiAnimation.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004c80f0 _$E635
// FUNCTION: STBC 0x004c8110 _$E634

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004c6b40 _$E32
// FUNCTION: STBC 0x004c6b50 _$E29
// FUNCTION: STBC 0x004c6b60 _$E31
// FUNCTION: STBC 0x004c6b70 _$E30
// FUNCTION: STBC 0x004c6ba0 _$E38
// FUNCTION: STBC 0x004c6bb0 _$E35
// FUNCTION: STBC 0x004c6bc0 _$E37
// FUNCTION: STBC 0x004c6bd0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004c6740 _$E80
// FUNCTION: STBC 0x004c6750 _$E79
// FUNCTION: STBC 0x004c6760 _$E434
// FUNCTION: STBC 0x004c6770 _$E433
// FUNCTION: STBC 0x004c6780 _$E438
// FUNCTION: STBC 0x004c6790 _$E437
// FUNCTION: STBC 0x004c67a0 _$E442
// FUNCTION: STBC 0x004c67b0 _$E441
// FUNCTION: STBC 0x004c67c0 _$E465
// FUNCTION: STBC 0x004c67d0 _$E464
// FUNCTION: STBC 0x004c67e0 _$E456
// FUNCTION: STBC 0x004c67f0 _$E455
// FUNCTION: STBC 0x004c6800 _$E469
// FUNCTION: STBC 0x004c6810 _$E468
// FUNCTION: STBC 0x004c6820 _$E473
// FUNCTION: STBC 0x004c6830 _$E472
// FUNCTION: STBC 0x004c6840 _$E534
// FUNCTION: STBC 0x004c6850 _$E533
// FUNCTION: STBC 0x004c6860 _$E537
// FUNCTION: STBC 0x004c6870 _$E536
// FUNCTION: STBC 0x004c6880 _$E494
// FUNCTION: STBC 0x004c6890 _$E493
// FUNCTION: STBC 0x004c68a0 _$E510
// FUNCTION: STBC 0x004c68b0 _$E509
// FUNCTION: STBC 0x004c68c0 _$E477
// FUNCTION: STBC 0x004c68d0 _$E476
// FUNCTION: STBC 0x004c68e0 _$E517
// FUNCTION: STBC 0x004c68f0 _$E516
// FUNCTION: STBC 0x004c6900 _$E520
// FUNCTION: STBC 0x004c6910 _$E519
// FUNCTION: STBC 0x004c6920 _$E556
// FUNCTION: STBC 0x004c6930 _$E555
// FUNCTION: STBC 0x004c6940 _$E480
// FUNCTION: STBC 0x004c6950 _$E479
// FUNCTION: STBC 0x004c6960 _$E483
// FUNCTION: STBC 0x004c6970 _$E482
// FUNCTION: STBC 0x004c6980 _$E486
// FUNCTION: STBC 0x004c6990 _$E485
// FUNCTION: STBC 0x004c69a0 _$E68
// FUNCTION: STBC 0x004c69b0 _$E67
// FUNCTION: STBC 0x004c69c0 _$E490
// FUNCTION: STBC 0x004c69d0 _$E489
// FUNCTION: STBC 0x004c69e0 _$E523
// FUNCTION: STBC 0x004c69f0 _$E522
// FUNCTION: STBC 0x004c6a00 _$E498
// FUNCTION: STBC 0x004c6a10 _$E497
// FUNCTION: STBC 0x004c6a20 _$E502
// FUNCTION: STBC 0x004c6a30 _$E501
// FUNCTION: STBC 0x004c6a40 _$E506
// FUNCTION: STBC 0x004c6a50 _$E505
// FUNCTION: STBC 0x004c6a60 _$E564
// FUNCTION: STBC 0x004c6a70 _$E563
// FUNCTION: STBC 0x004c6a80 _$E589
// FUNCTION: STBC 0x004c6a90 _$E588
// FUNCTION: STBC 0x004c6aa0 _$E577
// FUNCTION: STBC 0x004c6ab0 _$E576
// FUNCTION: STBC 0x004c6ac0 _$E514
// FUNCTION: STBC 0x004c6ad0 _$E513
// FUNCTION: STBC 0x004c6ae0 _$E527
// FUNCTION: STBC 0x004c6af0 _$E526
// FUNCTION: STBC 0x004c6b00 _$E531
// FUNCTION: STBC 0x004c6b10 _$E530
// FUNCTION: STBC 0x004c6b20 _$E71
// FUNCTION: STBC 0x004c6b30 _$E70
// FUNCTION: STBC 0x004c6b80 _$E83
// FUNCTION: STBC 0x004c6b90 _$E82
// FUNCTION: STBC 0x004c6be0 _$E541
// FUNCTION: STBC 0x004c6bf0 _$E540
// FUNCTION: STBC 0x004c6c00 _$E549
// FUNCTION: STBC 0x004c6c10 _$E548
// FUNCTION: STBC 0x004c6c20 _$E553
// FUNCTION: STBC 0x004c6c30 _$E552
// FUNCTION: STBC 0x004c6c40 _$E560
// FUNCTION: STBC 0x004c6c50 _$E559
// FUNCTION: STBC 0x004c6c60 _$E545
// FUNCTION: STBC 0x004c6c70 _$E544
// FUNCTION: STBC 0x004c6c80 _$E45
// FUNCTION: STBC 0x004c6c90 _$E44
// FUNCTION: STBC 0x004c6ca0 _$E263
// FUNCTION: STBC 0x004c6cb0 _$E262
// FUNCTION: STBC 0x004c6cc0 _$E573
// FUNCTION: STBC 0x004c6cd0 _$E572
// FUNCTION: STBC 0x004c6ce0 _$E569
// FUNCTION: STBC 0x004c6cf0 _$E568
// FUNCTION: STBC 0x004c6d00 _$E581
// FUNCTION: STBC 0x004c6d10 _$E580
// FUNCTION: STBC 0x004c6d20 _$E585
// FUNCTION: STBC 0x004c6d30 _$E584
// FUNCTION: STBC 0x004c6d40 _$E97
// FUNCTION: STBC 0x004c6d50 _$E96
// FUNCTION: STBC 0x004c6d60 _$E592
// FUNCTION: STBC 0x004c6d70 _$E591
// FUNCTION: STBC 0x004c6d80 _$E595
// FUNCTION: STBC 0x004c6d90 _$E594
// FUNCTION: STBC 0x004c6da0 _$E56
// FUNCTION: STBC 0x004c6db0 _$E55
// FUNCTION: STBC 0x004c6dc0 _$E599
// FUNCTION: STBC 0x004c6dd0 _$E598
// FUNCTION: STBC 0x004c6de0 _$E101
// FUNCTION: STBC 0x004c6df0 _$E100
// FUNCTION: STBC 0x004c6e00 _$E603
// FUNCTION: STBC 0x004c6e10 _$E602
// FUNCTION: STBC 0x004c6e20 _$E105
// FUNCTION: STBC 0x004c6e30 _$E104
// FUNCTION: STBC 0x004c6e40 _$E218
// FUNCTION: STBC 0x004c6e50 _$E217
// FUNCTION: STBC 0x004c78c0 _$E160
// FUNCTION: STBC 0x004c78d0 _$E159
// FUNCTION: STBC 0x004c78e0 _$E173
// FUNCTION: STBC 0x004c78f0 _$E172
// FUNCTION: STBC 0x004c7900 _$E268
// FUNCTION: STBC 0x004c7910 _$E267
// FUNCTION: STBC 0x004c7920 _$E273
// FUNCTION: STBC 0x004c7930 _$E272
// FUNCTION: STBC 0x004c7940 _$E277
// FUNCTION: STBC 0x004c7950 _$E276
// FUNCTION: STBC 0x004c7960 _$E281
// FUNCTION: STBC 0x004c7970 _$E280
// FUNCTION: STBC 0x004c7980 _$E288
// FUNCTION: STBC 0x004c7990 _$E287
// FUNCTION: STBC 0x004c79a0 _$E298
// FUNCTION: STBC 0x004c79b0 _$E297
// FUNCTION: STBC 0x004c79c0 _$E302
// FUNCTION: STBC 0x004c79d0 _$E301
// FUNCTION: STBC 0x004c79e0 _$E122
// FUNCTION: STBC 0x004c79f0 _$E121
// FUNCTION: STBC 0x004c7a00 _$E228
// FUNCTION: STBC 0x004c7a10 _$E227
// FUNCTION: STBC 0x004c7a20 _$E235
// FUNCTION: STBC 0x004c7a30 _$E234
// FUNCTION: STBC 0x004c7a40 _$E239
// FUNCTION: STBC 0x004c7a50 _$E238
// FUNCTION: STBC 0x004c7a60 _$E243
// FUNCTION: STBC 0x004c7a70 _$E242
// FUNCTION: STBC 0x004c7a80 _$E247
// FUNCTION: STBC 0x004c7a90 _$E246
// FUNCTION: STBC 0x004c7aa0 _$E253
// FUNCTION: STBC 0x004c7ab0 _$E252
// FUNCTION: STBC 0x004c7ac0 _$E306
// FUNCTION: STBC 0x004c7ad0 _$E305
// FUNCTION: STBC 0x004c7ae0 _$E310
// FUNCTION: STBC 0x004c7af0 _$E309
// FUNCTION: STBC 0x004c7b00 _$E314
// FUNCTION: STBC 0x004c7b10 _$E313
// FUNCTION: STBC 0x004c7b20 _$E318
// FUNCTION: STBC 0x004c7b30 _$E317
// FUNCTION: STBC 0x004c7b40 _$E322
// FUNCTION: STBC 0x004c7b50 _$E321
// FUNCTION: STBC 0x004c7b60 _$E326
// FUNCTION: STBC 0x004c7b70 _$E325
// FUNCTION: STBC 0x004c7b80 _$E330
// FUNCTION: STBC 0x004c7b90 _$E329
// FUNCTION: STBC 0x004c7ba0 _$E334
// FUNCTION: STBC 0x004c7bb0 _$E333
// FUNCTION: STBC 0x004c7bc0 _$E338
// FUNCTION: STBC 0x004c7bd0 _$E337
// FUNCTION: STBC 0x004c7be0 _$E342
// FUNCTION: STBC 0x004c7bf0 _$E341
// FUNCTION: STBC 0x004c7c00 _$E127
// FUNCTION: STBC 0x004c7c10 _$E126
// FUNCTION: STBC 0x004c7c20 _$E346
// FUNCTION: STBC 0x004c7c30 _$E345
// FUNCTION: STBC 0x004c7c40 _$E132
// FUNCTION: STBC 0x004c7c50 _$E131
// FUNCTION: STBC 0x004c7c60 _$E350
// FUNCTION: STBC 0x004c7c70 _$E349
// FUNCTION: STBC 0x004c7c80 _$E354
// FUNCTION: STBC 0x004c7c90 _$E353
// FUNCTION: STBC 0x004c7ca0 _$E358
// FUNCTION: STBC 0x004c7cb0 _$E357
// FUNCTION: STBC 0x004c7cc0 _$E362
// FUNCTION: STBC 0x004c7cd0 _$E361
// FUNCTION: STBC 0x004c7ce0 _$E137
// FUNCTION: STBC 0x004c7cf0 _$E136
// FUNCTION: STBC 0x004c7d00 _$E366
// FUNCTION: STBC 0x004c7d10 _$E365
// FUNCTION: STBC 0x004c7d20 _$E370
// FUNCTION: STBC 0x004c7d30 _$E369
// FUNCTION: STBC 0x004c7d40 _$E374
// FUNCTION: STBC 0x004c7d50 _$E373
// FUNCTION: STBC 0x004c7d60 _$E143
// FUNCTION: STBC 0x004c7d70 _$E142
// FUNCTION: STBC 0x004c7d80 _$E148
// FUNCTION: STBC 0x004c7d90 _$E147
// FUNCTION: STBC 0x004c7da0 _$E178
// FUNCTION: STBC 0x004c7db0 _$E177
// FUNCTION: STBC 0x004c7dc0 _$E378
// FUNCTION: STBC 0x004c7dd0 _$E377
// FUNCTION: STBC 0x004c7de0 _$E183
// FUNCTION: STBC 0x004c7df0 _$E182
// FUNCTION: STBC 0x004c7e00 _$E382
// FUNCTION: STBC 0x004c7e10 _$E381
// FUNCTION: STBC 0x004c7e20 _$E386
// FUNCTION: STBC 0x004c7e30 _$E385
// FUNCTION: STBC 0x004c7e40 _$E188
// FUNCTION: STBC 0x004c7e50 _$E187
// FUNCTION: STBC 0x004c7e60 _$E390
// FUNCTION: STBC 0x004c7e70 _$E389
// FUNCTION: STBC 0x004c7e80 _$E153
// FUNCTION: STBC 0x004c7e90 _$E152
// FUNCTION: STBC 0x004c7ea0 _$E394
// FUNCTION: STBC 0x004c7eb0 _$E393
// FUNCTION: STBC 0x004c7ec0 _$E398
// FUNCTION: STBC 0x004c7ed0 _$E397
// FUNCTION: STBC 0x004c7ee0 _$E193
// FUNCTION: STBC 0x004c7ef0 _$E192
// FUNCTION: STBC 0x004c7f00 _$E198
// FUNCTION: STBC 0x004c7f10 _$E197
// FUNCTION: STBC 0x004c7f20 _$E402
// FUNCTION: STBC 0x004c7f30 _$E401
// FUNCTION: STBC 0x004c7f40 _$E406
// FUNCTION: STBC 0x004c7f50 _$E405
// FUNCTION: STBC 0x004c7fb0 _$E257
// FUNCTION: STBC 0x004c7fc0 _$E256
// FUNCTION: STBC 0x004c7fd0 _$E410
// FUNCTION: STBC 0x004c7fe0 _$E409
// FUNCTION: STBC 0x004c7ff0 _$E414
// FUNCTION: STBC 0x004c8000 _$E413
// FUNCTION: STBC 0x004c8010 _$E418
// FUNCTION: STBC 0x004c8020 _$E417
// FUNCTION: STBC 0x004c8030 _$E422
// FUNCTION: STBC 0x004c8040 _$E421
// FUNCTION: STBC 0x004c8050 _$E426
// FUNCTION: STBC 0x004c8060 _$E425
// FUNCTION: STBC 0x004c8070 _$E203
// FUNCTION: STBC 0x004c8080 _$E202
// FUNCTION: STBC 0x004c8090 _$E430
// FUNCTION: STBC 0x004c80a0 _$E429
// FUNCTION: STBC 0x004c80b0 _$E208
// FUNCTION: STBC 0x004c80c0 _$E207
// FUNCTION: STBC 0x004c80d0 _$E213
// FUNCTION: STBC 0x004c80e0 _$E212
// FUNCTION: STBC 0x004c8120 _$E607
// FUNCTION: STBC 0x004c8130 _$E606
// FUNCTION: STBC 0x004c8140 _$E612
// FUNCTION: STBC 0x004c8150 _$E611
// FUNCTION: STBC 0x004c8160 _$E616
// FUNCTION: STBC 0x004c8170 _$E615

// The NiRTTI's dynamic initialiser, the registration of its exit-time destructor and the guarded destructor.
// FUNCTION: STBC 0x004c6e60 _$E622
// FUNCTION: STBC 0x004c6e70 _$E618
// FUNCTION: STBC 0x004c6e90 _$E621
// FUNCTION: STBC 0x004c6ea0 _$E619
// DATA: STBC 0x984750 ?rtti@AnimTSParticleController@@2VNiRTTI@@B
const NiRTTI AnimTSParticleController::rtti( "AnimTSParticleController", &EmitterController::rtti );

/*
================
AnimTSParticleController::FindController
================
*/
// FUNCTION: STBC 0x004c6ec0
AnimTSParticleController *AnimTSParticleController::FindController( NiObjectNET *target ) {
	for ( NiTimeController *controller = target->GetControllers(); controller; controller = controller->GetNext() ) {
		AnimTSParticleController *animTSController = NiDynamicCast( AnimTSParticleController, controller );
		if ( animTSController ) {
			return animTSController;
		}
	}
	return 0;
}

/*
================
AnimTSParticleController::AnimTSParticleController
================
*/
// FUNCTION: STBC 0x004c6f10
AnimTSParticleController::AnimTSParticleController() : EmitterController( 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, NULL ) {
	textureAnim = NULL;
	positions = NULL;
	sizes = NULL;
	colors = NULL;
	frames = NULL;
	tracks = NULL;
	drawOldToNew = true;
}

/*
================
AnimTSParticleController::TargetIsRequiredType
================
*/
// FUNCTION: STBC 0x004c6f80
bool AnimTSParticleController::TargetIsRequiredType() const {
	if ( !target ) {
		return false;
	}
	return target->IsKindOf( &AnimatedTriShapeParticles::rtti );
}

/*
================
AnimTSParticleController::~AnimTSParticleController

The arrays the target was built over are the controller's.
================
*/
// FUNCTION: STBC 0x004c6fd0
AnimTSParticleController::~AnimTSParticleController() {
	delete [] positions;
	delete [] sizes;
	delete [] colors;
	delete [] frames;
	delete [] tracks;
}

/*
================
AnimTSParticleController::CreateClone
================
*/
// FUNCTION: STBC 0x004c7030
NiObject *AnimTSParticleController::CreateClone() {
	AnimTSParticleController *clone = new AnimTSParticleController;
	CopyMembers( clone );
	return clone;
}

/*
================
AnimTSParticleController::CopyMembers
================
*/
// FUNCTION: STBC 0x004c70a0
void AnimTSParticleController::CopyMembers( AnimTSParticleController *dest ) {
	EmitterController::CopyMembers( dest );
	dest->textureAnim = textureAnim;
}

/*
================
AnimTSParticleController::Update

Drawn newest first, the live quads' triangles are listed from the last
particle back.
================
*/
// FUNCTION: STBC 0x004c70c0
void AnimTSParticleController::Update( float time ) {
	EmitterController::Update( time );
	if ( !drawOldToNew ) {
		unsigned short *triList = ( (AnimatedTriShapeParticles *)target )->GetDynamicData()->GetTriList();
		int vertex = numActive * 4 - 4;
		for ( int i = 0; i < numActive; i++ ) {
			*triList++ = vertex;
			*triList++ = vertex + 1;
			*triList++ = vertex + 2;
			*triList++ = vertex;
			*triList++ = vertex + 2;
			*triList++ = vertex + 3;
			vertex -= 4;
		}
	}
}

/*
================
AnimTSParticleController::VirtualSlot37
================
*/
// FUNCTION: STBC 0x004c7140
void AnimTSParticleController::VirtualSlot37( int index ) {
	if ( index != numActive - 1 ) {
		int following = numActive - index - 1;
		memmove( &tracks[index], &tracks[index + 1], following * sizeof( TextureAnimTrack * ) );
		memmove( &frames[index], &frames[index + 1], following * sizeof( unsigned short ) );
		memmove( &positions[index], &positions[index + 1], following * sizeof( NiPoint3 ) );
		if ( colors ) {
			memmove( &colors[index], &colors[index + 1], following * sizeof( NiColorA ) );
		}
		if ( sizes ) {
			memmove( &sizes[index], &sizes[index + 1], following * sizeof( float ) );
		}
		memmove( &lifeSpans[index], &lifeSpans[index + 1], following * sizeof( float ) );
		// BUG: moves a float per particle, a third of the velocities
		memmove( &velocities[index], &velocities[index + 1], following * sizeof( float ) );
		memmove( &birthTimes[index], &birthTimes[index + 1], following * sizeof( float ) );
	}
	numActive--;
	( (AnimatedTriShapeParticles *)target )->SetActiveParticleCount( numActive );
}

/*
================
AnimTSParticleController::VirtualSlot36

A new particle takes a random track of the texture animation, from its
first frame.
================
*/
// FUNCTION: STBC 0x004c7280
bool AnimTSParticleController::VirtualSlot36( float time ) {
	if ( EmitterController::VirtualSlot36( time ) ) {
		AnimatedTriShapeParticles *particles = (AnimatedTriShapeParticles *)target;
		int track = 0;
		if ( textureAnim->GetNumTracks() > 1 ) {
			track = g_kSystemWrapper.GetRandomNumber( textureAnim->GetNumTracks() - 1 );
		}
		tracks[numActive - 1] = textureAnim->GetTrack( track );
		frames[numActive - 1] = 0;
		particles->SetActiveParticleCount( numActive );
		return true;
	}
	return false;
}

/*
================
AnimTSParticleController::VirtualSlot35

A particle plays its track once over its life.
================
*/
// FUNCTION: STBC 0x004c7310
void AnimTSParticleController::VirtualSlot35( int index, float time, float deltaTime, float lifeFraction ) {
	EmitterController::VirtualSlot35( index, time, deltaTime, lifeFraction );
	frames[index] = (unsigned short)( ( tracks[index]->GetNumFrames() - 1 ) * lifeFraction );
}

/*
================
AnimTSParticleController::CreateTarget

Builds the particles over arrays of the controller's: one quad per
particle, textured with the texture animation's image and blended over
what is behind it.
================
*/
// FUNCTION: STBC 0x004c7360
void AnimTSParticleController::CreateTarget( char *textureAnimName ) {
	if ( numParticles <= 0 ) {
		VirtualSlot34();
		if ( numParticles <= 0 ) {
			return;
		}
	}

	TextureAnimContainer *container = NULL;
	g_kTextureAnimManager.GetContainer( textureAnimName, container );
	textureAnim = container;
	if ( textureAnim == NULL || textureAnim->GetImage() == NULL ) {
		return;
	}

	NiPoint3 *vertices = new NiPoint3[numParticles * 4];
	NiColorA *vertexColors = new NiColorA[numParticles * 4];
	NiPoint2 *textures = new NiPoint2[numParticles * 4];
	positions = new NiPoint3[numParticles];
	sizes = new float[numParticles];
	colors = new NiColorA[numParticles];
	frames = new unsigned short[numParticles];
	tracks = new TextureAnimTrack *[numParticles];

	// two triangles per quad
	unsigned short *triList = new unsigned short[numParticles * 6];
	unsigned short vertex = 0;
	for ( int i = 0; i < numParticles * 6; i += 6 ) {
		triList[i] = vertex;
		triList[i + 1] = vertex + 1;
		triList[i + 2] = vertex + 2;
		triList[i + 3] = vertex;
		triList[i + 4] = vertex + 2;
		triList[i + 5] = vertex + 3;
		vertex += 4;
	}

	AnimatedTriShapeParticles *particles = new AnimatedTriShapeParticles( numParticles, vertices, NULL, vertexColors, textures, triList, positions, sizes, colors, frames, tracks );

	NiTextureProperty *texture = new NiTextureProperty;
	texture->SetImage( textureAnim->GetImage() );
	particles->AttachProperty( texture );

	NiTextureModeProperty *textureMode = new NiTextureModeProperty;
	textureMode->SetApplyMode( NiTextureModeProperty::APPLY_MODULATE );
	textureMode->SetFilterMode( NiTextureModeProperty::FILTER_TRILERP );
	particles->AttachProperty( textureMode );

	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	particles->AttachProperty( vertexColor );

	NiAlphaProperty *alpha = new NiAlphaProperty;
	alpha->SetAlphaBlending( true );
	alpha->SetSrcBlendMode( NiAlphaProperty::ALPHA_SRCALPHA );
	alpha->SetDestBlendMode( NiAlphaProperty::ALPHA_INVSRCALPHA );
	particles->AttachProperty( alpha );

	NiZBufferProperty *zBuffer = new NiZBufferProperty;
	zBuffer->SetZBufferTest( true );
	zBuffer->SetZBufferWrite( false );
	particles->AttachProperty( zBuffer );

	particles->UpdateProperties();
	particles->SetActiveParticleCount( 0 );
	SetTarget( particles );
}

/*
================
AnimTSParticleController::VirtualSlot38
================
*/
// FUNCTION: STBC 0x004c78a0
void AnimTSParticleController::VirtualSlot38() {
	EmitterController::VirtualSlot38();
	if ( target ) {
		( (AnimatedTriShapeParticles *)target )->SetActiveParticleCount( 0 );
	}
}
