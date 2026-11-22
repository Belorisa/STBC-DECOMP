/*
===============================================================================

	PointParticleController.cpp

===============================================================================
*/

#include <iostream>
#include "PointParticleController.h"
#include "TriShapeParticles.h"
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
// FUNCTION: STBC 0x004d1bd0 _$E630
// FUNCTION: STBC 0x004d1bf0 _$E629

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004d0ab0 _$E32
// FUNCTION: STBC 0x004d0ac0 _$E29
// FUNCTION: STBC 0x004d0ad0 _$E31
// FUNCTION: STBC 0x004d0ae0 _$E30
// FUNCTION: STBC 0x004d0af0 _$E38
// FUNCTION: STBC 0x004d0b00 _$E35
// FUNCTION: STBC 0x004d0b10 _$E37
// FUNCTION: STBC 0x004d0b20 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004d06d0 _$E80
// FUNCTION: STBC 0x004d06e0 _$E79
// FUNCTION: STBC 0x004d06f0 _$E434
// FUNCTION: STBC 0x004d0700 _$E433
// FUNCTION: STBC 0x004d0710 _$E438
// FUNCTION: STBC 0x004d0720 _$E437
// FUNCTION: STBC 0x004d0730 _$E442
// FUNCTION: STBC 0x004d0740 _$E441
// FUNCTION: STBC 0x004d0750 _$E465
// FUNCTION: STBC 0x004d0760 _$E464
// FUNCTION: STBC 0x004d0770 _$E456
// FUNCTION: STBC 0x004d0780 _$E455
// FUNCTION: STBC 0x004d0790 _$E469
// FUNCTION: STBC 0x004d07a0 _$E468
// FUNCTION: STBC 0x004d07b0 _$E473
// FUNCTION: STBC 0x004d07c0 _$E472
// FUNCTION: STBC 0x004d07d0 _$E534
// FUNCTION: STBC 0x004d07e0 _$E533
// FUNCTION: STBC 0x004d07f0 _$E537
// FUNCTION: STBC 0x004d0800 _$E536
// FUNCTION: STBC 0x004d0810 _$E494
// FUNCTION: STBC 0x004d0820 _$E493
// FUNCTION: STBC 0x004d0830 _$E510
// FUNCTION: STBC 0x004d0840 _$E509
// FUNCTION: STBC 0x004d0850 _$E477
// FUNCTION: STBC 0x004d0860 _$E476
// FUNCTION: STBC 0x004d0870 _$E517
// FUNCTION: STBC 0x004d0880 _$E516
// FUNCTION: STBC 0x004d0890 _$E520
// FUNCTION: STBC 0x004d08a0 _$E519
// FUNCTION: STBC 0x004d08b0 _$E556
// FUNCTION: STBC 0x004d08c0 _$E555
// FUNCTION: STBC 0x004d08d0 _$E480
// FUNCTION: STBC 0x004d08e0 _$E479
// FUNCTION: STBC 0x004d08f0 _$E483
// FUNCTION: STBC 0x004d0900 _$E482
// FUNCTION: STBC 0x004d0910 _$E486
// FUNCTION: STBC 0x004d0920 _$E485
// FUNCTION: STBC 0x004d0930 _$E68
// FUNCTION: STBC 0x004d0940 _$E67
// FUNCTION: STBC 0x004d0950 _$E490
// FUNCTION: STBC 0x004d0960 _$E489
// FUNCTION: STBC 0x004d0970 _$E523
// FUNCTION: STBC 0x004d0980 _$E522
// FUNCTION: STBC 0x004d0990 _$E498
// FUNCTION: STBC 0x004d09a0 _$E497
// FUNCTION: STBC 0x004d09b0 _$E502
// FUNCTION: STBC 0x004d09c0 _$E501
// FUNCTION: STBC 0x004d09d0 _$E506
// FUNCTION: STBC 0x004d09e0 _$E505
// FUNCTION: STBC 0x004d09f0 _$E564
// FUNCTION: STBC 0x004d0a00 _$E563
// FUNCTION: STBC 0x004d0a10 _$E589
// FUNCTION: STBC 0x004d0a20 _$E588
// FUNCTION: STBC 0x004d0a30 _$E577
// FUNCTION: STBC 0x004d0a40 _$E576
// FUNCTION: STBC 0x004d0a50 _$E514
// FUNCTION: STBC 0x004d0a60 _$E513
// FUNCTION: STBC 0x004d0a70 _$E527
// FUNCTION: STBC 0x004d0a80 _$E526
// FUNCTION: STBC 0x004d0a90 _$E531
// FUNCTION: STBC 0x004d0aa0 _$E530
// FUNCTION: STBC 0x004d0b30 _$E71
// FUNCTION: STBC 0x004d0b40 _$E70
// FUNCTION: STBC 0x004d0b50 _$E83
// FUNCTION: STBC 0x004d0b60 _$E82
// FUNCTION: STBC 0x004d0b70 _$E541
// FUNCTION: STBC 0x004d0b80 _$E540
// FUNCTION: STBC 0x004d0b90 _$E549
// FUNCTION: STBC 0x004d0ba0 _$E548
// FUNCTION: STBC 0x004d0bb0 _$E553
// FUNCTION: STBC 0x004d0bc0 _$E552
// FUNCTION: STBC 0x004d0bd0 _$E560
// FUNCTION: STBC 0x004d0be0 _$E559
// FUNCTION: STBC 0x004d0bf0 _$E45
// FUNCTION: STBC 0x004d0c00 _$E44
// FUNCTION: STBC 0x004d0c10 _$E263
// FUNCTION: STBC 0x004d0c20 _$E262
// FUNCTION: STBC 0x004d0c30 _$E545
// FUNCTION: STBC 0x004d0c40 _$E544
// FUNCTION: STBC 0x004d0c50 _$E573
// FUNCTION: STBC 0x004d0c60 _$E572
// FUNCTION: STBC 0x004d0c70 _$E569
// FUNCTION: STBC 0x004d0c80 _$E568
// FUNCTION: STBC 0x004d0c90 _$E97
// FUNCTION: STBC 0x004d0ca0 _$E96
// FUNCTION: STBC 0x004d0cb0 _$E581
// FUNCTION: STBC 0x004d0cc0 _$E580
// FUNCTION: STBC 0x004d0cd0 _$E585
// FUNCTION: STBC 0x004d0ce0 _$E584
// FUNCTION: STBC 0x004d0cf0 _$E592
// FUNCTION: STBC 0x004d0d00 _$E591
// FUNCTION: STBC 0x004d0d10 _$E56
// FUNCTION: STBC 0x004d0d20 _$E55
// FUNCTION: STBC 0x004d0d30 _$E595
// FUNCTION: STBC 0x004d0d40 _$E594
// FUNCTION: STBC 0x004d0d50 _$E599
// FUNCTION: STBC 0x004d0d60 _$E598
// FUNCTION: STBC 0x004d0d70 _$E101
// FUNCTION: STBC 0x004d0d80 _$E100
// FUNCTION: STBC 0x004d0d90 _$E603
// FUNCTION: STBC 0x004d0da0 _$E602
// FUNCTION: STBC 0x004d0db0 _$E105
// FUNCTION: STBC 0x004d0dc0 _$E104
// FUNCTION: STBC 0x004d0dd0 _$E218
// FUNCTION: STBC 0x004d0de0 _$E217
// FUNCTION: STBC 0x004d13f0 _$E160
// FUNCTION: STBC 0x004d1400 _$E159
// FUNCTION: STBC 0x004d1410 _$E173
// FUNCTION: STBC 0x004d1420 _$E172
// FUNCTION: STBC 0x004d1430 _$E268
// FUNCTION: STBC 0x004d1440 _$E267
// FUNCTION: STBC 0x004d1450 _$E273
// FUNCTION: STBC 0x004d1460 _$E272
// FUNCTION: STBC 0x004d1470 _$E277
// FUNCTION: STBC 0x004d1480 _$E276
// FUNCTION: STBC 0x004d1490 _$E281
// FUNCTION: STBC 0x004d14a0 _$E280
// FUNCTION: STBC 0x004d14b0 _$E288
// FUNCTION: STBC 0x004d14c0 _$E287
// FUNCTION: STBC 0x004d14d0 _$E298
// FUNCTION: STBC 0x004d14e0 _$E297
// FUNCTION: STBC 0x004d14f0 _$E302
// FUNCTION: STBC 0x004d1500 _$E301
// FUNCTION: STBC 0x004d1510 _$E122
// FUNCTION: STBC 0x004d1520 _$E121
// FUNCTION: STBC 0x004d1530 _$E228
// FUNCTION: STBC 0x004d1540 _$E227
// FUNCTION: STBC 0x004d1550 _$E235
// FUNCTION: STBC 0x004d1560 _$E234
// FUNCTION: STBC 0x004d1570 _$E239
// FUNCTION: STBC 0x004d1580 _$E238
// FUNCTION: STBC 0x004d1590 _$E243
// FUNCTION: STBC 0x004d15a0 _$E242
// FUNCTION: STBC 0x004d15b0 _$E247
// FUNCTION: STBC 0x004d15c0 _$E246
// FUNCTION: STBC 0x004d15d0 _$E253
// FUNCTION: STBC 0x004d15e0 _$E252
// FUNCTION: STBC 0x004d15f0 _$E306
// FUNCTION: STBC 0x004d1600 _$E305
// FUNCTION: STBC 0x004d1610 _$E310
// FUNCTION: STBC 0x004d1620 _$E309
// FUNCTION: STBC 0x004d1630 _$E314
// FUNCTION: STBC 0x004d1640 _$E313
// FUNCTION: STBC 0x004d1650 _$E318
// FUNCTION: STBC 0x004d1660 _$E317
// FUNCTION: STBC 0x004d1670 _$E322
// FUNCTION: STBC 0x004d1680 _$E321
// FUNCTION: STBC 0x004d1690 _$E326
// FUNCTION: STBC 0x004d16a0 _$E325
// FUNCTION: STBC 0x004d16b0 _$E330
// FUNCTION: STBC 0x004d16c0 _$E329
// FUNCTION: STBC 0x004d16d0 _$E334
// FUNCTION: STBC 0x004d16e0 _$E333
// FUNCTION: STBC 0x004d16f0 _$E338
// FUNCTION: STBC 0x004d1700 _$E337
// FUNCTION: STBC 0x004d1710 _$E342
// FUNCTION: STBC 0x004d1720 _$E341
// FUNCTION: STBC 0x004d1730 _$E127
// FUNCTION: STBC 0x004d1740 _$E126
// FUNCTION: STBC 0x004d1750 _$E346
// FUNCTION: STBC 0x004d1760 _$E345
// FUNCTION: STBC 0x004d1770 _$E132
// FUNCTION: STBC 0x004d1780 _$E131
// FUNCTION: STBC 0x004d1790 _$E350
// FUNCTION: STBC 0x004d17a0 _$E349
// FUNCTION: STBC 0x004d17b0 _$E354
// FUNCTION: STBC 0x004d17c0 _$E353
// FUNCTION: STBC 0x004d17d0 _$E358
// FUNCTION: STBC 0x004d17e0 _$E357
// FUNCTION: STBC 0x004d17f0 _$E362
// FUNCTION: STBC 0x004d1800 _$E361
// FUNCTION: STBC 0x004d1810 _$E137
// FUNCTION: STBC 0x004d1820 _$E136
// FUNCTION: STBC 0x004d1830 _$E366
// FUNCTION: STBC 0x004d1840 _$E365
// FUNCTION: STBC 0x004d1850 _$E370
// FUNCTION: STBC 0x004d1860 _$E369
// FUNCTION: STBC 0x004d1870 _$E374
// FUNCTION: STBC 0x004d1880 _$E373
// FUNCTION: STBC 0x004d1890 _$E143
// FUNCTION: STBC 0x004d18a0 _$E142
// FUNCTION: STBC 0x004d18b0 _$E148
// FUNCTION: STBC 0x004d18c0 _$E147
// FUNCTION: STBC 0x004d18d0 _$E178
// FUNCTION: STBC 0x004d18e0 _$E177
// FUNCTION: STBC 0x004d18f0 _$E378
// FUNCTION: STBC 0x004d1900 _$E377
// FUNCTION: STBC 0x004d1910 _$E183
// FUNCTION: STBC 0x004d1920 _$E182
// FUNCTION: STBC 0x004d1930 _$E382
// FUNCTION: STBC 0x004d1940 _$E381
// FUNCTION: STBC 0x004d1950 _$E386
// FUNCTION: STBC 0x004d1960 _$E385
// FUNCTION: STBC 0x004d1970 _$E188
// FUNCTION: STBC 0x004d1980 _$E187
// FUNCTION: STBC 0x004d1990 _$E390
// FUNCTION: STBC 0x004d19a0 _$E389
// FUNCTION: STBC 0x004d19b0 _$E153
// FUNCTION: STBC 0x004d19c0 _$E152
// FUNCTION: STBC 0x004d19d0 _$E394
// FUNCTION: STBC 0x004d19e0 _$E393
// FUNCTION: STBC 0x004d19f0 _$E398
// FUNCTION: STBC 0x004d1a00 _$E397
// FUNCTION: STBC 0x004d1a10 _$E193
// FUNCTION: STBC 0x004d1a20 _$E192
// FUNCTION: STBC 0x004d1a30 _$E198
// FUNCTION: STBC 0x004d1a40 _$E197
// FUNCTION: STBC 0x004d1a50 _$E402
// FUNCTION: STBC 0x004d1a60 _$E401
// FUNCTION: STBC 0x004d1a70 _$E406
// FUNCTION: STBC 0x004d1a80 _$E405
// FUNCTION: STBC 0x004d1a90 _$E257
// FUNCTION: STBC 0x004d1aa0 _$E256
// FUNCTION: STBC 0x004d1ab0 _$E410
// FUNCTION: STBC 0x004d1ac0 _$E409
// FUNCTION: STBC 0x004d1ad0 _$E414
// FUNCTION: STBC 0x004d1ae0 _$E413
// FUNCTION: STBC 0x004d1af0 _$E418
// FUNCTION: STBC 0x004d1b00 _$E417
// FUNCTION: STBC 0x004d1b10 _$E422
// FUNCTION: STBC 0x004d1b20 _$E421
// FUNCTION: STBC 0x004d1b30 _$E426
// FUNCTION: STBC 0x004d1b40 _$E425
// FUNCTION: STBC 0x004d1b50 _$E203
// FUNCTION: STBC 0x004d1b60 _$E202
// FUNCTION: STBC 0x004d1b70 _$E430
// FUNCTION: STBC 0x004d1b80 _$E429
// FUNCTION: STBC 0x004d1b90 _$E208
// FUNCTION: STBC 0x004d1ba0 _$E207
// FUNCTION: STBC 0x004d1bb0 _$E213
// FUNCTION: STBC 0x004d1bc0 _$E212
// FUNCTION: STBC 0x004d1c00 _$E607
// FUNCTION: STBC 0x004d1c10 _$E606
// FUNCTION: STBC 0x004d1c20 _$E612
// FUNCTION: STBC 0x004d1c30 _$E611
// FUNCTION: STBC 0x004d1c40 _$E616
// FUNCTION: STBC 0x004d1c50 _$E615

// The NiRTTI's dynamic initialiser, the registration of its exit-time destructor and the guarded destructor.
// FUNCTION: STBC 0x004d0df0 _$E622
// FUNCTION: STBC 0x004d0e00 _$E618
// FUNCTION: STBC 0x004d0e20 _$E621
// FUNCTION: STBC 0x004d0e30 _$E619
// DATA: STBC 0x00985238 ?rtti@PointParticleController@@2VNiRTTI@@B
const NiRTTI PointParticleController::rtti( "PointParticleController", &EmitterController::rtti );

/*
================
PointParticleController::FindController
================
*/
// FUNCTION: STBC 0x004d0e50
PointParticleController *PointParticleController::FindController( NiObjectNET *target ) {
	for ( NiTimeController *controller = target->GetControllers(); controller; controller = controller->GetNext() ) {
		PointParticleController *pointController = NiDynamicCast( PointParticleController, controller );
		if ( pointController ) {
			return pointController;
		}
	}
	return 0;
}

/*
================
PointParticleController::Create
================
*/
// FUNCTION: STBC 0x004d0ea0
PointParticleController *PointParticleController::Create( float lifeTime, float emitLife, float emitFrequency ) {
	return new PointParticleController( lifeTime, emitLife, emitFrequency, 1.0f, 0.0f, NULL );
}

/*
================
PointParticleController::PointParticleController

The emitter is built without an emit object.
================
*/
// FUNCTION: STBC 0x004d0f20
// BUG: emitFrom_ is dropped, so the object the Create functions of the particle controllers take is never used
PointParticleController::PointParticleController( float lifeTime_, float emitLife_, float emitFrequency_, float emitVelocity_, float angleVariance_, NiAVObject *emitFrom_ ) : EmitterController( lifeTime_, emitLife_, emitFrequency_, emitVelocity_, angleVariance_, NULL ) {
}

/*
================
PointParticleController::VirtualSlot37

Also shrinks the drawn quads to the live particles.
================
*/
// FUNCTION: STBC 0x004d0f90
void PointParticleController::VirtualSlot37( int index ) {
	EmitterController::VirtualSlot37( index );
	( (TriShapeParticles *)target )->SetActiveParticleCount( numActive );
}

/*
================
PointParticleController::VirtualSlot36
================
*/
// FUNCTION: STBC 0x004d0fc0
bool PointParticleController::VirtualSlot36( float time ) {
	if ( EmitterController::VirtualSlot36( time ) ) {
		( (TriShapeParticles *)target )->SetActiveParticleCount( numActive );
		return true;
	}
	return false;
}

/*
================
PointParticleController::Update
================
*/
// FUNCTION: STBC 0x004d0ff0
void PointParticleController::Update( float time ) {
	EmitterController::Update( time );
	if ( target ) {
		( (TriShapeParticles *)target )->InvalidateBound();
	}
}

/*
================
PointParticleController::CreateTarget

Builds a quad per particle over the target's own arrays, textured with the
named image when there is one and blended over what is behind it.
================
*/
// FUNCTION: STBC 0x004d1010
void PointParticleController::CreateTarget( char *textureName ) {
	if ( numParticles <= 0 ) {
		VirtualSlot34();
		if ( numParticles <= 0 ) {
			return;
		}
	}

	TriShapeParticles *particles = new TriShapeParticles( numParticles, false, true, true, true );
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
	alpha->SetDestBlendMode( NiAlphaProperty::ALPHA_INVSRCALPHA );
	alpha->SetAlphaBlending( true );
	particles->AttachProperty( alpha );

	NiZBufferProperty *zBuffer = new NiZBufferProperty;
	zBuffer->SetZBufferTest( true );
	zBuffer->SetZBufferWrite( false );
	particles->AttachProperty( zBuffer );

	particles->UpdateProperties();
	particles->SetActiveParticleCount( 0 );
	sizes = particles->GetSizes();
	positions = particles->GetPositions();
	colors = particles->GetParticleColors();
	SetTarget( particles );
}

/*
================
PointParticleController::VirtualSlot38
================
*/
// FUNCTION: STBC 0x004d1390
void PointParticleController::VirtualSlot38() {
	if ( target ) {
		( (TriShapeParticles *)target )->SetActiveParticleCount( 0 );
	}
	EmitterController::VirtualSlot38();
}

/*
================
PointParticleController::TargetIsRequiredType
================
*/
// FUNCTION: STBC 0x004d13b0
bool PointParticleController::TargetIsRequiredType() const {
	if ( !target ) {
		return false;
	}
	return target->IsKindOf( &TriShapeParticles::rtti );
}
