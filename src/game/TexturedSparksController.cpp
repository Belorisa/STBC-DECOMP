/*
===============================================================================

	TexturedSparksController.cpp

===============================================================================
*/

#include <iostream>
#include "TexturedSparksController.h"
#include "TriShapeOrientedParticles.h"
#include "ImageManagerClass.h"
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
// FUNCTION: STBC 0x004d40a0 _$E633
// FUNCTION: STBC 0x004d40c0 _$E632

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004d2040 _$E32
// FUNCTION: STBC 0x004d2050 _$E29
// FUNCTION: STBC 0x004d2060 _$E31
// FUNCTION: STBC 0x004d2070 _$E30
// FUNCTION: STBC 0x004d2080 _$E38
// FUNCTION: STBC 0x004d2090 _$E35
// FUNCTION: STBC 0x004d20a0 _$E37
// FUNCTION: STBC 0x004d20b0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004d1c60 _$E80
// FUNCTION: STBC 0x004d1c70 _$E79
// FUNCTION: STBC 0x004d1c80 _$E453
// FUNCTION: STBC 0x004d1c90 _$E452
// FUNCTION: STBC 0x004d1ca0 _$E457
// FUNCTION: STBC 0x004d1cb0 _$E456
// FUNCTION: STBC 0x004d1cc0 _$E461
// FUNCTION: STBC 0x004d1cd0 _$E460
// FUNCTION: STBC 0x004d1ce0 _$E470
// FUNCTION: STBC 0x004d1cf0 _$E469
// FUNCTION: STBC 0x004d1d00 _$E233
// FUNCTION: STBC 0x004d1d10 _$E232
// FUNCTION: STBC 0x004d1d20 _$E474
// FUNCTION: STBC 0x004d1d30 _$E473
// FUNCTION: STBC 0x004d1d40 _$E478
// FUNCTION: STBC 0x004d1d50 _$E477
// FUNCTION: STBC 0x004d1d60 _$E236
// FUNCTION: STBC 0x004d1d70 _$E235
// FUNCTION: STBC 0x004d1d80 _$E539
// FUNCTION: STBC 0x004d1d90 _$E538
// FUNCTION: STBC 0x004d1da0 _$E499
// FUNCTION: STBC 0x004d1db0 _$E498
// FUNCTION: STBC 0x004d1dc0 _$E515
// FUNCTION: STBC 0x004d1dd0 _$E514
// FUNCTION: STBC 0x004d1de0 _$E482
// FUNCTION: STBC 0x004d1df0 _$E481
// FUNCTION: STBC 0x004d1e00 _$E522
// FUNCTION: STBC 0x004d1e10 _$E521
// FUNCTION: STBC 0x004d1e20 _$E525
// FUNCTION: STBC 0x004d1e30 _$E524
// FUNCTION: STBC 0x004d1e40 _$E558
// FUNCTION: STBC 0x004d1e50 _$E557
// FUNCTION: STBC 0x004d1e60 _$E485
// FUNCTION: STBC 0x004d1e70 _$E484
// FUNCTION: STBC 0x004d1e80 _$E488
// FUNCTION: STBC 0x004d1e90 _$E487
// FUNCTION: STBC 0x004d1ea0 _$E491
// FUNCTION: STBC 0x004d1eb0 _$E490
// FUNCTION: STBC 0x004d1ec0 _$E68
// FUNCTION: STBC 0x004d1ed0 _$E67
// FUNCTION: STBC 0x004d1ee0 _$E495
// FUNCTION: STBC 0x004d1ef0 _$E494
// FUNCTION: STBC 0x004d1f00 _$E528
// FUNCTION: STBC 0x004d1f10 _$E527
// FUNCTION: STBC 0x004d1f20 _$E503
// FUNCTION: STBC 0x004d1f30 _$E502
// FUNCTION: STBC 0x004d1f40 _$E507
// FUNCTION: STBC 0x004d1f50 _$E506
// FUNCTION: STBC 0x004d1f60 _$E511
// FUNCTION: STBC 0x004d1f70 _$E510
// FUNCTION: STBC 0x004d1f80 _$E566
// FUNCTION: STBC 0x004d1f90 _$E565
// FUNCTION: STBC 0x004d1fa0 _$E591
// FUNCTION: STBC 0x004d1fb0 _$E590
// FUNCTION: STBC 0x004d1fc0 _$E579
// FUNCTION: STBC 0x004d1fd0 _$E578
// FUNCTION: STBC 0x004d1fe0 _$E519
// FUNCTION: STBC 0x004d1ff0 _$E518
// FUNCTION: STBC 0x004d2000 _$E532
// FUNCTION: STBC 0x004d2010 _$E531
// FUNCTION: STBC 0x004d2020 _$E536
// FUNCTION: STBC 0x004d2030 _$E535
// FUNCTION: STBC 0x004d20c0 _$E71
// FUNCTION: STBC 0x004d20d0 _$E70
// FUNCTION: STBC 0x004d20e0 _$E83
// FUNCTION: STBC 0x004d20f0 _$E82
// FUNCTION: STBC 0x004d2100 _$E543
// FUNCTION: STBC 0x004d2110 _$E542
// FUNCTION: STBC 0x004d2120 _$E551
// FUNCTION: STBC 0x004d2130 _$E550
// FUNCTION: STBC 0x004d2140 _$E555
// FUNCTION: STBC 0x004d2150 _$E554
// FUNCTION: STBC 0x004d2160 _$E562
// FUNCTION: STBC 0x004d2170 _$E561
// FUNCTION: STBC 0x004d2180 _$E45
// FUNCTION: STBC 0x004d2190 _$E44
// FUNCTION: STBC 0x004d21a0 _$E282
// FUNCTION: STBC 0x004d21b0 _$E281
// FUNCTION: STBC 0x004d21c0 _$E547
// FUNCTION: STBC 0x004d21d0 _$E546
// FUNCTION: STBC 0x004d21e0 _$E575
// FUNCTION: STBC 0x004d21f0 _$E574
// FUNCTION: STBC 0x004d2200 _$E571
// FUNCTION: STBC 0x004d2210 _$E570
// FUNCTION: STBC 0x004d2220 _$E97
// FUNCTION: STBC 0x004d2230 _$E96
// FUNCTION: STBC 0x004d2240 _$E583
// FUNCTION: STBC 0x004d2250 _$E582
// FUNCTION: STBC 0x004d2260 _$E587
// FUNCTION: STBC 0x004d2270 _$E586
// FUNCTION: STBC 0x004d2280 _$E594
// FUNCTION: STBC 0x004d2290 _$E593
// FUNCTION: STBC 0x004d22a0 _$E56
// FUNCTION: STBC 0x004d22b0 _$E55
// FUNCTION: STBC 0x004d22c0 _$E597
// FUNCTION: STBC 0x004d22d0 _$E596
// FUNCTION: STBC 0x004d22e0 _$E101
// FUNCTION: STBC 0x004d22f0 _$E100
// FUNCTION: STBC 0x004d2300 _$E601
// FUNCTION: STBC 0x004d2310 _$E600
// FUNCTION: STBC 0x004d2320 _$E605
// FUNCTION: STBC 0x004d2330 _$E604
// FUNCTION: STBC 0x004d2340 _$E105
// FUNCTION: STBC 0x004d2350 _$E104
// FUNCTION: STBC 0x004d2360 _$E218
// FUNCTION: STBC 0x004d2370 _$E217
// FUNCTION: STBC 0x004d37f0 _$E160
// FUNCTION: STBC 0x004d3800 _$E159
// FUNCTION: STBC 0x004d3810 _$E173
// FUNCTION: STBC 0x004d3820 _$E172
// FUNCTION: STBC 0x004d3830 _$E287
// FUNCTION: STBC 0x004d3840 _$E286
// FUNCTION: STBC 0x004d3850 _$E292
// FUNCTION: STBC 0x004d3860 _$E291
// FUNCTION: STBC 0x004d3920 _$E296
// FUNCTION: STBC 0x004d3930 _$E295
// FUNCTION: STBC 0x004d3940 _$E300
// FUNCTION: STBC 0x004d3950 _$E299
// FUNCTION: STBC 0x004d3960 _$E307
// FUNCTION: STBC 0x004d3970 _$E306
// FUNCTION: STBC 0x004d3980 _$E317
// FUNCTION: STBC 0x004d3990 _$E316
// FUNCTION: STBC 0x004d39a0 _$E321
// FUNCTION: STBC 0x004d39b0 _$E320
// FUNCTION: STBC 0x004d39c0 _$E122
// FUNCTION: STBC 0x004d39d0 _$E121
// FUNCTION: STBC 0x004d39e0 _$E245
// FUNCTION: STBC 0x004d39f0 _$E244
// FUNCTION: STBC 0x004d3a00 _$E252
// FUNCTION: STBC 0x004d3a10 _$E251
// FUNCTION: STBC 0x004d3a20 _$E256
// FUNCTION: STBC 0x004d3a30 _$E255
// FUNCTION: STBC 0x004d3a40 _$E260
// FUNCTION: STBC 0x004d3a50 _$E259
// FUNCTION: STBC 0x004d3a60 _$E264
// FUNCTION: STBC 0x004d3a70 _$E263
// FUNCTION: STBC 0x004d3a80 _$E270
// FUNCTION: STBC 0x004d3a90 _$E269
// FUNCTION: STBC 0x004d3aa0 _$E325
// FUNCTION: STBC 0x004d3ab0 _$E324
// FUNCTION: STBC 0x004d3ac0 _$E329
// FUNCTION: STBC 0x004d3ad0 _$E328
// FUNCTION: STBC 0x004d3ae0 _$E333
// FUNCTION: STBC 0x004d3af0 _$E332
// FUNCTION: STBC 0x004d3b00 _$E337
// FUNCTION: STBC 0x004d3b10 _$E336
// FUNCTION: STBC 0x004d3b20 _$E341
// FUNCTION: STBC 0x004d3b30 _$E340
// FUNCTION: STBC 0x004d3b40 _$E345
// FUNCTION: STBC 0x004d3b50 _$E344
// FUNCTION: STBC 0x004d3b60 _$E349
// FUNCTION: STBC 0x004d3b70 _$E348
// FUNCTION: STBC 0x004d3b80 _$E353
// FUNCTION: STBC 0x004d3b90 _$E352
// FUNCTION: STBC 0x004d3ba0 _$E357
// FUNCTION: STBC 0x004d3bb0 _$E356
// FUNCTION: STBC 0x004d3bc0 _$E361
// FUNCTION: STBC 0x004d3bd0 _$E360
// FUNCTION: STBC 0x004d3be0 _$E127
// FUNCTION: STBC 0x004d3bf0 _$E126
// FUNCTION: STBC 0x004d3c00 _$E365
// FUNCTION: STBC 0x004d3c10 _$E364
// FUNCTION: STBC 0x004d3c20 _$E132
// FUNCTION: STBC 0x004d3c30 _$E131
// FUNCTION: STBC 0x004d3c40 _$E369
// FUNCTION: STBC 0x004d3c50 _$E368
// FUNCTION: STBC 0x004d3c60 _$E373
// FUNCTION: STBC 0x004d3c70 _$E372
// FUNCTION: STBC 0x004d3c80 _$E377
// FUNCTION: STBC 0x004d3c90 _$E376
// FUNCTION: STBC 0x004d3ca0 _$E381
// FUNCTION: STBC 0x004d3cb0 _$E380
// FUNCTION: STBC 0x004d3cc0 _$E137
// FUNCTION: STBC 0x004d3cd0 _$E136
// FUNCTION: STBC 0x004d3ce0 _$E385
// FUNCTION: STBC 0x004d3cf0 _$E384
// FUNCTION: STBC 0x004d3d00 _$E389
// FUNCTION: STBC 0x004d3d10 _$E388
// FUNCTION: STBC 0x004d3d20 _$E393
// FUNCTION: STBC 0x004d3d30 _$E392
// FUNCTION: STBC 0x004d3d40 _$E143
// FUNCTION: STBC 0x004d3d50 _$E142
// FUNCTION: STBC 0x004d3d60 _$E148
// FUNCTION: STBC 0x004d3d70 _$E147
// FUNCTION: STBC 0x004d3d80 _$E178
// FUNCTION: STBC 0x004d3d90 _$E177
// FUNCTION: STBC 0x004d3da0 _$E397
// FUNCTION: STBC 0x004d3db0 _$E396
// FUNCTION: STBC 0x004d3dc0 _$E183
// FUNCTION: STBC 0x004d3dd0 _$E182
// FUNCTION: STBC 0x004d3de0 _$E401
// FUNCTION: STBC 0x004d3df0 _$E400
// FUNCTION: STBC 0x004d3e00 _$E405
// FUNCTION: STBC 0x004d3e10 _$E404
// FUNCTION: STBC 0x004d3e20 _$E188
// FUNCTION: STBC 0x004d3e30 _$E187
// FUNCTION: STBC 0x004d3e40 _$E409
// FUNCTION: STBC 0x004d3e50 _$E408
// FUNCTION: STBC 0x004d3e60 _$E153
// FUNCTION: STBC 0x004d3e70 _$E152
// FUNCTION: STBC 0x004d3e80 _$E413
// FUNCTION: STBC 0x004d3e90 _$E412
// FUNCTION: STBC 0x004d3ea0 _$E417
// FUNCTION: STBC 0x004d3eb0 _$E416
// FUNCTION: STBC 0x004d3ec0 _$E193
// FUNCTION: STBC 0x004d3ed0 _$E192
// FUNCTION: STBC 0x004d3ee0 _$E198
// FUNCTION: STBC 0x004d3ef0 _$E197
// FUNCTION: STBC 0x004d3f00 _$E421
// FUNCTION: STBC 0x004d3f10 _$E420
// FUNCTION: STBC 0x004d3f20 _$E425
// FUNCTION: STBC 0x004d3f30 _$E424
// FUNCTION: STBC 0x004d3f40 _$E274
// FUNCTION: STBC 0x004d3f50 _$E273
// FUNCTION: STBC 0x004d3f60 _$E429
// FUNCTION: STBC 0x004d3f70 _$E428
// FUNCTION: STBC 0x004d3f80 _$E433
// FUNCTION: STBC 0x004d3f90 _$E432
// FUNCTION: STBC 0x004d3fa0 _$E437
// FUNCTION: STBC 0x004d3fb0 _$E436
// FUNCTION: STBC 0x004d3fc0 _$E441
// FUNCTION: STBC 0x004d3fd0 _$E440
// FUNCTION: STBC 0x004d3fe0 _$E445
// FUNCTION: STBC 0x004d3ff0 _$E444
// FUNCTION: STBC 0x004d4000 _$E203
// FUNCTION: STBC 0x004d4010 _$E202
// FUNCTION: STBC 0x004d4020 _$E449
// FUNCTION: STBC 0x004d4030 _$E448
// FUNCTION: STBC 0x004d4040 _$E208
// FUNCTION: STBC 0x004d4050 _$E207
// FUNCTION: STBC 0x004d4060 _$E213
// FUNCTION: STBC 0x004d4070 _$E212
// FUNCTION: STBC 0x004d40d0 _$E609
// FUNCTION: STBC 0x004d40e0 _$E608
// FUNCTION: STBC 0x004d40f0 _$E614
// FUNCTION: STBC 0x004d4100 _$E613
// FUNCTION: STBC 0x004d4110 _$E618
// FUNCTION: STBC 0x004d4120 _$E617

// The NiRTTI's dynamic initialiser, the registration of its exit-time destructor and the guarded destructor.
// FUNCTION: STBC 0x004d2d40 _$E624
// FUNCTION: STBC 0x004d2d50 _$E620
// FUNCTION: STBC 0x004d2d70 _$E623
// FUNCTION: STBC 0x004d2d80 _$E621
// DATA: STBC 0x985478 ?rtti@TexturedSparksController@@2VNiRTTI@@B
const NiRTTI TexturedSparksController::rtti( "TexturedSparksController", &EmitterController::rtti );

/*
================
TexturedSparksController::FindController
================
*/
// FUNCTION: STBC 0x004d2da0
TexturedSparksController *TexturedSparksController::FindController( NiObjectNET *target ) {
	for ( NiTimeController *controller = target->GetControllers(); controller; controller = controller->GetNext() ) {
		TexturedSparksController *found = NiDynamicCast( TexturedSparksController, controller );
		if ( found ) {
			return found;
		}
	}
	return 0;
}

/*
================
TexturedSparksController::Create
================
*/
// FUNCTION: STBC 0x004d2df0
TexturedSparksController *TexturedSparksController::Create( float lifeTime, float emitLife, float emitFrequency, float emitVelocity, float angleVariance, NiAVObject *emitFrom ) {
	return new TexturedSparksController( lifeTime, emitLife, emitFrequency, emitVelocity, angleVariance, emitFrom );
}

/*
================
TexturedSparksController::TexturedSparksController
================
*/
// FUNCTION: STBC 0x004d2e70
TexturedSparksController::TexturedSparksController( float lifeTime_, float emitLife_, float emitFrequency_, float emitVelocity_, float angleVariance_, NiAVObject *emitFrom_ )
	: EmitterController( lifeTime_, emitLife_, emitFrequency_, emitVelocity_, angleVariance_, emitFrom_ ) {
	directions = NULL;
	dimensions = NULL;
}

/*
================
TexturedSparksController::CreateClone
================
*/
// FUNCTION: STBC 0x004d2f90
NiObject *TexturedSparksController::CreateClone() {
	TexturedSparksController *clone = new TexturedSparksController( 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, NULL );
	CopyMembers( clone );
	return clone;
}

/*
================
TexturedSparksController::CopyMembers
================
*/
// FUNCTION: STBC 0x004d3010
void TexturedSparksController::CopyMembers( TexturedSparksController *dest ) {
	EmitterController::CopyMembers( dest );
}

/*
================
TexturedSparksController::CreateTarget

Builds a quad per particle over the target's own arrays, textured with the
named image when there is one and added to what is behind it. Without
dimensions of their own the quads take the first dimension key's size.
================
*/
// FUNCTION: STBC 0x004d3020
// PARKED: inline budget and EH states, as FlareController::CreateTarget: retail calls NiPointer's constructor (0x42FD00) out of line for the last three properties, opens no unwind state for the property bases and keeps this in EBX; calls and control flow match
void TexturedSparksController::CreateTarget( char *textureName ) {
	if ( numParticles <= 0 ) {
		VirtualSlot34();
		if ( numParticles <= 0 ) {
			return;
		}
	}

	TriShapeOrientedParticles *particles = new TriShapeOrientedParticles( numParticles, false, true, dimensionKeys.GetCount() > 1, true );
	particles->SetTextureCoordinates();

	if ( textureName ) {
		NiTextureProperty *texture = new NiTextureProperty;
		texture->SetImage( g_kImageManager.GetImage( textureName, true, true ) );
		particles->AttachProperty( texture );

		NiTextureModeProperty *textureMode = new NiTextureModeProperty;
		textureMode->SetApplyMode( NiTextureModeProperty::APPLY_MODULATE );
		textureMode->SetFilterMode( g_kImageManager.GetTextureFilterMode() );
		particles->AttachProperty( textureMode );
	}

	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	particles->AttachProperty( vertexColor );

	NiAlphaProperty *alpha = new NiAlphaProperty;
	alpha->SetSrcBlendMode( NiAlphaProperty::ALPHA_SRCALPHA );
	alpha->SetDestBlendMode( NiAlphaProperty::ALPHA_ONE );
	alpha->SetAlphaBlending( true );
	particles->AttachProperty( alpha );

	NiZBufferProperty *zBuffer = new NiZBufferProperty;
	zBuffer->SetZBufferTest( true );
	zBuffer->SetZBufferWrite( false );
	particles->AttachProperty( zBuffer );

	particles->UpdateProperties();
	particles->SetActiveParticleCount( 0 );
	dimensions = particles->GetDimensions();
	positions = particles->GetPositions();
	colors = particles->GetParticleColors();
	directions = particles->GetDirections();
	if ( dimensions == NULL && dimensionKeys.GetCount() != 0 ) {
		particles->SetDefaultDimensions( dimensionKeys.keys[0].GetPos().x, dimensionKeys.keys[0].GetPos().y );
	}
	SetTarget( particles );
}

/*
================
TexturedSparksController::AddDimensionKey
================
*/
// FUNCTION: STBC 0x004d33d0
void TexturedSparksController::AddDimensionKey( float time, float width, float length ) {
	dimensionKeys.Insert( NiLinPosKey( time, NiPoint3( width, length, 0.0f ) ) );
}

/*
================
TexturedSparksController::VirtualSlot35

The quad lies along the particle's velocity, sized by the dimension keys.
================
*/
// FUNCTION: STBC 0x004d3570
void TexturedSparksController::VirtualSlot35( int index, float time, float deltaTime, float lifeFraction ) {
	directions[index] = velocities[index];
	directions[index].Unitize();
	EmitterController::VirtualSlot35( index, time, deltaTime, lifeFraction );
	if ( dimensions ) {
		int count = dimensionKeys.GetCount();
		NiLinPosKey *keys = dimensionKeys.keys;
		for ( int i = 0; i < count; i++ ) {
			if ( keys[i].GetTime() > lifeFraction ) {
				if ( i == 0 ) {
					dimensions[index].x = keys[0].GetPos().x;
					dimensions[index].y = keys[0].GetPos().y;
				} else {
					NiPosKey::InterpFunction interpolate = NiPosKey::interpFunctions[NiPosKey::LINKEY];
					float time0 = keys[i - 1].GetTime();
					NiPoint3 dimension = interpolate( ( lifeFraction - time0 ) / ( keys[i].GetTime() - time0 ), &keys[i - 1], &keys[i] );
					dimensions[index].x = dimension.x;
					dimensions[index].y = dimension.y;
				}
				break;
			}
		}
		// BUG: the last key's dimensions are always stored, over the ones
		// just found
		dimensions[index].x = keys[count - 1].GetPos().x;
		dimensions[index].y = keys[count - 1].GetPos().y;
	}
}

/*
================
TexturedSparksController::VirtualSlot37
================
*/
// FUNCTION: STBC 0x004d3700
void TexturedSparksController::VirtualSlot37( int index ) {
	EmitterController::VirtualSlot37( index );
	if ( target ) {
		( (TriShapeParticles *)target )->SetActiveParticleCount( numActive );
	}
}

/*
================
TexturedSparksController::VirtualSlot36
================
*/
// FUNCTION: STBC 0x004d3730
bool TexturedSparksController::VirtualSlot36( float time ) {
	if ( EmitterController::VirtualSlot36( time ) ) {
		if ( target ) {
			( (TriShapeParticles *)target )->SetActiveParticleCount( numActive );
		}
		return true;
	}
	return false;
}

/*
================
TexturedSparksController::VirtualSlot38
================
*/
// FUNCTION: STBC 0x004d3770
void TexturedSparksController::VirtualSlot38() {
	EmitterController::VirtualSlot38();
	if ( target ) {
		( (TriShapeParticles *)target )->SetActiveParticleCount( 0 );
	}
}

/*
================
TexturedSparksController::Update
================
*/
// FUNCTION: STBC 0x004d3790
void TexturedSparksController::Update( float time ) {
	EmitterController::Update( time );
	if ( target ) {
		( (TriShapeParticles *)target )->InvalidateBound();
	}
}

/*
================
TexturedSparksController::TargetIsRequiredType
================
*/
// FUNCTION: STBC 0x004d37b0
bool TexturedSparksController::TargetIsRequiredType() const {
	if ( !target ) {
		return false;
	}
	return target->IsKindOf( &TriShapeOrientedParticles::rtti );
}
