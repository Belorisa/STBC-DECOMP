/*
===============================================================================

	EmitterController.cpp

===============================================================================
*/

#include <iostream>
#include <math.h>
#include <stdlib.h>

#include "EmitterController.h"
#include "NiMath.h"
#include "NiNode.h"
#include "NiMain.h"
#include "NiAnimation.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004cd3e0 _$E628
// FUNCTION: STBC 0x004cd400 _$E627

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004cb7c0 _$E32
// FUNCTION: STBC 0x004cb7d0 _$E29
// FUNCTION: STBC 0x004cb7e0 _$E31
// FUNCTION: STBC 0x004cb7f0 _$E30
// FUNCTION: STBC 0x004cb800 _$E38
// FUNCTION: STBC 0x004cb810 _$E35
// FUNCTION: STBC 0x004cb820 _$E37
// FUNCTION: STBC 0x004cb830 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004cb3e0 _$E80
// FUNCTION: STBC 0x004cb3f0 _$E79
// FUNCTION: STBC 0x004cb400 _$E431
// FUNCTION: STBC 0x004cb410 _$E430
// FUNCTION: STBC 0x004cb420 _$E435
// FUNCTION: STBC 0x004cb430 _$E434
// FUNCTION: STBC 0x004cb440 _$E439
// FUNCTION: STBC 0x004cb450 _$E438
// FUNCTION: STBC 0x004cb460 _$E462
// FUNCTION: STBC 0x004cb470 _$E461
// FUNCTION: STBC 0x004cb480 _$E453
// FUNCTION: STBC 0x004cb490 _$E452
// FUNCTION: STBC 0x004cb4a0 _$E466
// FUNCTION: STBC 0x004cb4b0 _$E465
// FUNCTION: STBC 0x004cb4c0 _$E470
// FUNCTION: STBC 0x004cb4d0 _$E469
// FUNCTION: STBC 0x004cb4e0 _$E531
// FUNCTION: STBC 0x004cb4f0 _$E530
// FUNCTION: STBC 0x004cb500 _$E534
// FUNCTION: STBC 0x004cb510 _$E533
// FUNCTION: STBC 0x004cb520 _$E491
// FUNCTION: STBC 0x004cb530 _$E490
// FUNCTION: STBC 0x004cb540 _$E507
// FUNCTION: STBC 0x004cb550 _$E506
// FUNCTION: STBC 0x004cb560 _$E474
// FUNCTION: STBC 0x004cb570 _$E473
// FUNCTION: STBC 0x004cb580 _$E514
// FUNCTION: STBC 0x004cb590 _$E513
// FUNCTION: STBC 0x004cb5a0 _$E517
// FUNCTION: STBC 0x004cb5b0 _$E516
// FUNCTION: STBC 0x004cb5c0 _$E553
// FUNCTION: STBC 0x004cb5d0 _$E552
// FUNCTION: STBC 0x004cb5e0 _$E477
// FUNCTION: STBC 0x004cb5f0 _$E476
// FUNCTION: STBC 0x004cb600 _$E480
// FUNCTION: STBC 0x004cb610 _$E479
// FUNCTION: STBC 0x004cb620 _$E483
// FUNCTION: STBC 0x004cb630 _$E482
// FUNCTION: STBC 0x004cb640 _$E68
// FUNCTION: STBC 0x004cb650 _$E67
// FUNCTION: STBC 0x004cb660 _$E487
// FUNCTION: STBC 0x004cb670 _$E486
// FUNCTION: STBC 0x004cb680 _$E520
// FUNCTION: STBC 0x004cb690 _$E519
// FUNCTION: STBC 0x004cb6a0 _$E495
// FUNCTION: STBC 0x004cb6b0 _$E494
// FUNCTION: STBC 0x004cb6c0 _$E499
// FUNCTION: STBC 0x004cb6d0 _$E498
// FUNCTION: STBC 0x004cb6e0 _$E503
// FUNCTION: STBC 0x004cb6f0 _$E502
// FUNCTION: STBC 0x004cb700 _$E561
// FUNCTION: STBC 0x004cb710 _$E560
// FUNCTION: STBC 0x004cb720 _$E586
// FUNCTION: STBC 0x004cb730 _$E585
// FUNCTION: STBC 0x004cb740 _$E574
// FUNCTION: STBC 0x004cb750 _$E573
// FUNCTION: STBC 0x004cb760 _$E511
// FUNCTION: STBC 0x004cb770 _$E510
// FUNCTION: STBC 0x004cb780 _$E524
// FUNCTION: STBC 0x004cb790 _$E523
// FUNCTION: STBC 0x004cb7a0 _$E528
// FUNCTION: STBC 0x004cb7b0 _$E527
// FUNCTION: STBC 0x004cb840 _$E71
// FUNCTION: STBC 0x004cb850 _$E70
// FUNCTION: STBC 0x004cb860 _$E83
// FUNCTION: STBC 0x004cb870 _$E82
// FUNCTION: STBC 0x004cb880 _$E538
// FUNCTION: STBC 0x004cb890 _$E537
// FUNCTION: STBC 0x004cb8a0 _$E546
// FUNCTION: STBC 0x004cb8b0 _$E545
// FUNCTION: STBC 0x004cb8c0 _$E550
// FUNCTION: STBC 0x004cb8d0 _$E549
// FUNCTION: STBC 0x004cb8e0 _$E557
// FUNCTION: STBC 0x004cb8f0 _$E556
// FUNCTION: STBC 0x004cb900 _$E45
// FUNCTION: STBC 0x004cb910 _$E44
// FUNCTION: STBC 0x004cb920 _$E542
// FUNCTION: STBC 0x004cb930 _$E541
// FUNCTION: STBC 0x004cb940 _$E232
// FUNCTION: STBC 0x004cb950 _$E231
// FUNCTION: STBC 0x004cb960 _$E570
// FUNCTION: STBC 0x004cb970 _$E569
// FUNCTION: STBC 0x004cb980 _$E566
// FUNCTION: STBC 0x004cb990 _$E565
// FUNCTION: STBC 0x004cb9a0 _$E578
// FUNCTION: STBC 0x004cb9b0 _$E577
// FUNCTION: STBC 0x004cb9c0 _$E97
// FUNCTION: STBC 0x004cb9d0 _$E96
// FUNCTION: STBC 0x004cb9e0 _$E582
// FUNCTION: STBC 0x004cb9f0 _$E581
// FUNCTION: STBC 0x004cba00 _$E589
// FUNCTION: STBC 0x004cba10 _$E588
// FUNCTION: STBC 0x004cba20 _$E592
// FUNCTION: STBC 0x004cba30 _$E591
// FUNCTION: STBC 0x004cba40 _$E56
// FUNCTION: STBC 0x004cba50 _$E55
// FUNCTION: STBC 0x004cba60 _$E596
// FUNCTION: STBC 0x004cba70 _$E595
// FUNCTION: STBC 0x004cba80 _$E101
// FUNCTION: STBC 0x004cba90 _$E100
// FUNCTION: STBC 0x004cbaa0 _$E600
// FUNCTION: STBC 0x004cbab0 _$E599
// FUNCTION: STBC 0x004cbac0 _$E105
// FUNCTION: STBC 0x004cbad0 _$E104
// FUNCTION: STBC 0x004cbae0 _$E218
// FUNCTION: STBC 0x004cbaf0 _$E217
// FUNCTION: STBC 0x004ccc00 _$E160
// FUNCTION: STBC 0x004ccc10 _$E159
// FUNCTION: STBC 0x004ccc20 _$E173
// FUNCTION: STBC 0x004ccc30 _$E172
// FUNCTION: STBC 0x004ccc40 _$E224
// FUNCTION: STBC 0x004ccc50 _$E223
// FUNCTION: STBC 0x004ccc60 _$E237
// FUNCTION: STBC 0x004ccc70 _$E236
// FUNCTION: STBC 0x004ccc80 _$E228
// FUNCTION: STBC 0x004ccc90 _$E227
// FUNCTION: STBC 0x004ccca0 _$E241
// FUNCTION: STBC 0x004cccb0 _$E240
// FUNCTION: STBC 0x004cccc0 _$E248
// FUNCTION: STBC 0x004cccd0 _$E247
// FUNCTION: STBC 0x004ccce0 _$E273
// FUNCTION: STBC 0x004cccf0 _$E272
// FUNCTION: STBC 0x004ccd00 _$E277
// FUNCTION: STBC 0x004ccd10 _$E276
// FUNCTION: STBC 0x004ccd20 _$E122
// FUNCTION: STBC 0x004ccd30 _$E121
// FUNCTION: STBC 0x004ccd40 _$E259
// FUNCTION: STBC 0x004ccd50 _$E258
// FUNCTION: STBC 0x004ccd60 _$E266
// FUNCTION: STBC 0x004ccd70 _$E265
// FUNCTION: STBC 0x004ccd80 _$E289
// FUNCTION: STBC 0x004ccd90 _$E288
// FUNCTION: STBC 0x004ccda0 _$E293
// FUNCTION: STBC 0x004ccdb0 _$E292
// FUNCTION: STBC 0x004ccdc0 _$E333
// FUNCTION: STBC 0x004ccdd0 _$E332
// FUNCTION: STBC 0x004ccde0 _$E339
// FUNCTION: STBC 0x004ccdf0 _$E338
// FUNCTION: STBC 0x004cce00 _$E281
// FUNCTION: STBC 0x004cce10 _$E280
// FUNCTION: STBC 0x004cce20 _$E285
// FUNCTION: STBC 0x004cce30 _$E284
// FUNCTION: STBC 0x004cce40 _$E297
// FUNCTION: STBC 0x004cce50 _$E296
// FUNCTION: STBC 0x004cce60 _$E301
// FUNCTION: STBC 0x004cce70 _$E300
// FUNCTION: STBC 0x004cce80 _$E305
// FUNCTION: STBC 0x004cce90 _$E304
// FUNCTION: STBC 0x004ccea0 _$E309
// FUNCTION: STBC 0x004cceb0 _$E308
// FUNCTION: STBC 0x004ccec0 _$E313
// FUNCTION: STBC 0x004cced0 _$E312
// FUNCTION: STBC 0x004ccee0 _$E317
// FUNCTION: STBC 0x004ccef0 _$E316
// FUNCTION: STBC 0x004ccf00 _$E321
// FUNCTION: STBC 0x004ccf10 _$E320
// FUNCTION: STBC 0x004ccf20 _$E325
// FUNCTION: STBC 0x004ccf30 _$E324
// FUNCTION: STBC 0x004ccf40 _$E127
// FUNCTION: STBC 0x004ccf50 _$E126
// FUNCTION: STBC 0x004ccf60 _$E329
// FUNCTION: STBC 0x004ccf70 _$E328
// FUNCTION: STBC 0x004ccf80 _$E132
// FUNCTION: STBC 0x004ccf90 _$E131
// FUNCTION: STBC 0x004ccfa0 _$E343
// FUNCTION: STBC 0x004ccfb0 _$E342
// FUNCTION: STBC 0x004ccfc0 _$E347
// FUNCTION: STBC 0x004ccfd0 _$E346
// FUNCTION: STBC 0x004ccfe0 _$E351
// FUNCTION: STBC 0x004ccff0 _$E350
// FUNCTION: STBC 0x004cd000 _$E355
// FUNCTION: STBC 0x004cd010 _$E354
// FUNCTION: STBC 0x004cd020 _$E137
// FUNCTION: STBC 0x004cd030 _$E136
// FUNCTION: STBC 0x004cd040 _$E359
// FUNCTION: STBC 0x004cd050 _$E358
// FUNCTION: STBC 0x004cd060 _$E363
// FUNCTION: STBC 0x004cd070 _$E362
// FUNCTION: STBC 0x004cd080 _$E367
// FUNCTION: STBC 0x004cd090 _$E366
// FUNCTION: STBC 0x004cd0a0 _$E143
// FUNCTION: STBC 0x004cd0b0 _$E142
// FUNCTION: STBC 0x004cd0c0 _$E148
// FUNCTION: STBC 0x004cd0d0 _$E147
// FUNCTION: STBC 0x004cd0e0 _$E178
// FUNCTION: STBC 0x004cd0f0 _$E177
// FUNCTION: STBC 0x004cd100 _$E371
// FUNCTION: STBC 0x004cd110 _$E370
// FUNCTION: STBC 0x004cd120 _$E183
// FUNCTION: STBC 0x004cd130 _$E182
// FUNCTION: STBC 0x004cd140 _$E375
// FUNCTION: STBC 0x004cd150 _$E374
// FUNCTION: STBC 0x004cd160 _$E379
// FUNCTION: STBC 0x004cd170 _$E378
// FUNCTION: STBC 0x004cd180 _$E188
// FUNCTION: STBC 0x004cd190 _$E187
// FUNCTION: STBC 0x004cd1a0 _$E383
// FUNCTION: STBC 0x004cd1b0 _$E382
// FUNCTION: STBC 0x004cd1c0 _$E153
// FUNCTION: STBC 0x004cd1d0 _$E152
// FUNCTION: STBC 0x004cd1e0 _$E387
// FUNCTION: STBC 0x004cd1f0 _$E386
// FUNCTION: STBC 0x004cd200 _$E391
// FUNCTION: STBC 0x004cd210 _$E390
// FUNCTION: STBC 0x004cd220 _$E193
// FUNCTION: STBC 0x004cd230 _$E192
// FUNCTION: STBC 0x004cd240 _$E198
// FUNCTION: STBC 0x004cd250 _$E197
// FUNCTION: STBC 0x004cd260 _$E395
// FUNCTION: STBC 0x004cd270 _$E394
// FUNCTION: STBC 0x004cd280 _$E399
// FUNCTION: STBC 0x004cd290 _$E398
// FUNCTION: STBC 0x004cd2a0 _$E403
// FUNCTION: STBC 0x004cd2b0 _$E402
// FUNCTION: STBC 0x004cd2c0 _$E407
// FUNCTION: STBC 0x004cd2d0 _$E406
// FUNCTION: STBC 0x004cd2e0 _$E411
// FUNCTION: STBC 0x004cd2f0 _$E410
// FUNCTION: STBC 0x004cd300 _$E415
// FUNCTION: STBC 0x004cd310 _$E414
// FUNCTION: STBC 0x004cd320 _$E419
// FUNCTION: STBC 0x004cd330 _$E418
// FUNCTION: STBC 0x004cd340 _$E423
// FUNCTION: STBC 0x004cd350 _$E422
// FUNCTION: STBC 0x004cd360 _$E203
// FUNCTION: STBC 0x004cd370 _$E202
// FUNCTION: STBC 0x004cd380 _$E427
// FUNCTION: STBC 0x004cd390 _$E426
// FUNCTION: STBC 0x004cd3a0 _$E208
// FUNCTION: STBC 0x004cd3b0 _$E207
// FUNCTION: STBC 0x004cd3c0 _$E213
// FUNCTION: STBC 0x004cd3d0 _$E212
// FUNCTION: STBC 0x004cd410 _$E604
// FUNCTION: STBC 0x004cd420 _$E603
// FUNCTION: STBC 0x004cd430 _$E609
// FUNCTION: STBC 0x004cd440 _$E608
// FUNCTION: STBC 0x004cd450 _$E613
// FUNCTION: STBC 0x004cd460 _$E612

// The NiRTTI's dynamic initialiser, the registration of its exit-time destructor and the guarded destructor.
// FUNCTION: STBC 0x004cbb00 _$E619
// FUNCTION: STBC 0x004cbb10 _$E615
// FUNCTION: STBC 0x004cbb30 _$E618
// FUNCTION: STBC 0x004cbb40 _$E616
// DATA: STBC 0x00984ce8 ?rtti@EmitterController@@2VNiRTTI@@B
const NiRTTI EmitterController::rtti( "EmitterController", &EffectController::rtti );

/*
================
EmitterController::FindController
================
*/
// FUNCTION: STBC 0x004cbb60
EmitterController *EmitterController::FindController( NiObjectNET *target ) {
	for ( NiTimeController *controller = target->GetControllers(); controller; controller = controller->GetNext() ) {
		EmitterController *emitterController = NiDynamicCast( EmitterController, controller );
		if ( emitterController ) {
			return emitterController;
		}
	}
	return 0;
}

/*
================
EmitterController::EmitterController
================
*/
// FUNCTION: STBC 0x004cbbb0
EmitterController::EmitterController( float lifeTime_, float emitLife_, float emitFrequency_, float emitVelocity_, float angleVariance_, NiAVObject *emitFrom_ ) : EffectController( lifeTime_ ) {
	emitRadius = 0.0f;
	velocities = NULL;
	lifeSpans = NULL;
	birthTimes = NULL;
	sizes = NULL;
	positions = NULL;
	colors = NULL;
	emitTimer = 0;
	numActive = 0;
	numToEmit = 0;
	numParticles = 0;
	emitLife = emitLife_;
	emitLifeVariance = 0.0f;
	emitFrequency = emitFrequency_;
	emitFrequencyVariance = 0.0f;
	emitVelocity = emitVelocity_;
	emitVelocityVariance = 0.0f;
	angleVariance = angleVariance_;
	emitFrom = emitFrom_;
	gravity = NiPoint3::ZERO;
	damping = 0.0f;
	inheritedVelocity = NiPoint3::ZERO;
	emitPosition = NiPoint3::ZERO;
	emitDirection = NiMatrix3::IDENTITY;
	SetInheritsVelocity( false );
}

/*
================
EmitterController::~EmitterController
================
*/
// FUNCTION: STBC 0x004cbd60
EmitterController::~EmitterController() {
	delete [] velocities;
	delete [] lifeSpans;
	delete [] birthTimes;
}

/*
================
EmitterController::CreateClone

An emitter is not cloned.
================
*/
// FUNCTION: STBC 0x004cbdf0
NiObject *EmitterController::CreateClone() {
	return NULL;
}

/*
================
EmitterController::CopyMembers

The particles themselves are not copied.
================
*/
// FUNCTION: STBC 0x004cbe00
void EmitterController::CopyMembers( EmitterController *dest ) {
	EffectController::CopyMembers( dest );
	dest->sizes = NULL;
	dest->positions = NULL;
	dest->colors = NULL;
	dest->emitTimer = 0;
	dest->emitLife = emitLife;
	dest->emitLifeVariance = emitLifeVariance;
	dest->emitVelocity = emitVelocity;
	dest->emitVelocityVariance = emitVelocityVariance;
	dest->angleVariance = angleVariance;
	dest->numToEmit = 0;
	dest->numActive = 0;
	dest->numParticles = 0;
	dest->gravity = gravity;
	dest->damping = damping;
}

/*
================
EmitterController::VirtualSlot34

Sizes the particle arrays for as many particles as can live at once, and
counts the particles to emit over the effect's life, which is stretched to
hold at least one particle's longest life.
================
*/
// FUNCTION: STBC 0x004cbeb0
void EmitterController::VirtualSlot34() {
	VirtualSlot38();
	float maxLife = emitLife + emitLifeVariance * 0.5f;
	float rate = 1.0f / ( emitFrequency - emitFrequencyVariance * 0.5f );
	int oldCount = numParticles;
	emitTimer = 0.0f;
	if ( lifeTime < maxLife ) {
		lifeTime = maxLife;
	}
	numParticles = (int)( rate * maxLife ) + 1;
	numToEmit = (int)( ( lifeTime - maxLife ) * rate ) + 1;
	if ( numParticles > numToEmit ) {
		numParticles = numToEmit;
	}
	if ( numParticles != oldCount || !velocities || !lifeSpans || !birthTimes ) {
		delete [] velocities;
		delete [] lifeSpans;
		delete [] birthTimes;
		velocities = new NiPoint3[numParticles];
		lifeSpans = new float[numParticles];
		birthTimes = new float[numParticles];
	}
}

/*
================
EmitterController::AddColorKey

The keys of an emitter are over a particle's life, from 0 to 1.
================
*/
// FUNCTION: STBC 0x004cbfe0
void EmitterController::AddColorKey( float time, float r, float g, float b ) {
	if ( time >= 0.0f && time <= 1.0f ) {
		EffectController::AddColorKey( time, r, g, b );
	}
}

/*
================
EmitterController::AddAlphaKey
================
*/
// FUNCTION: STBC 0x004cc020
void EmitterController::AddAlphaKey( float time, float alpha ) {
	if ( time >= 0.0f && time <= 1.0f ) {
		EffectController::AddAlphaKey( time, alpha );
	}
}

/*
================
EmitterController::AddSizeKey
================
*/
// FUNCTION: STBC 0x004cc060
void EmitterController::AddSizeKey( float time, float size ) {
	if ( time >= 0.0f && time <= 1.0f ) {
		EffectController::AddSizeKey( time, size );
	}
}

/*
================
EmitterController::Stop
================
*/
// FUNCTION: STBC 0x004cc0a0
void EmitterController::Stop() {
	VirtualSlot38();
	EffectController::Stop();
}

/*
================
EmitterController::VirtualSlot35
================
*/
// FUNCTION: STBC 0x004cc0c0
void EmitterController::VirtualSlot35( int index, float time, float deltaTime, float lifeFraction ) {
	if ( colors ) {
		GetColor( lifeFraction, colors[index].r, colors[index].g, colors[index].b );
		GetAlpha( lifeFraction, colors[index].a );
	}
	if ( sizes ) {
		GetSize( lifeFraction, sizes[index] );
	}
	positions[index] += velocities[index] * deltaTime;
	velocities[index] += gravity * deltaTime;
	velocities[index] -= velocities[index] * ( damping * deltaTime );
}

/*
================
EmitterController::VirtualSlot37

Removes the particle at the index, moving the last live one into its place.
================
*/
// FUNCTION: STBC 0x004cc200
void EmitterController::VirtualSlot37( int index ) {
	if ( index != numActive - 1 ) {
		positions[index] = positions[numActive - 1];
		if ( colors ) {
			colors[index] = colors[numActive - 1];
		}
		if ( sizes ) {
			sizes[index] = sizes[numActive - 1];
		}
		lifeSpans[index] = lifeSpans[numActive - 1];
		velocities[index] = velocities[numActive - 1];
		birthTimes[index] = birthTimes[numActive - 1];
	}
	numActive--;
}

/*
================
EmitterController::VirtualSlot38

Drops the live particles.
================
*/
// FUNCTION: STBC 0x004cc300
void EmitterController::VirtualSlot38() {
	numActive = 0;
}

/*
================
EmitterController::VirtualSlot36

Emits one particle, unless its random life would end within the time
already owed: at the emit position, spread over the emit radius, heading
off the emit direction by up to half the angle variance, at the emit
velocity plus the emit object's when it is inherited. Returns whether it
was emitted.
================
*/
// FUNCTION: STBC 0x004cc310
// PARKED: inline budget: retail calls NiPoint3::Length (0x4145B0) out of line inside Unitize; the remaining residue is x87 operand order (the angle product, the inherited-velocity add)
bool EmitterController::VirtualSlot36( float time ) {
	float life = ( rand() * ( 1.0f / RAND_MAX ) - 0.5f ) * emitLifeVariance + emitLife;
	if ( life > emitTimer ) {
		positions[numActive] = modelEmitPosition;
		lifeSpans[numActive] = life;

		NiPoint3 direction = NiPoint3::UNIT_X;
		NiMatrix3 rotation;
		float angle = rand() * ( angleVariance * ( 1.0f / 360.0f ) ) * NI_PI * ( 1.0f / RAND_MAX );
		rotation.MakeZRotation( angle );
		direction = rotation * direction;
		angle = rand() * NI_TWO_PI * ( 1.0f / RAND_MAX );
		rotation.MakeXRotation( angle );
		direction = rotation * direction;
		direction = modelEmitDirection * direction;
		direction.Unitize();

		if ( emitRadius > 0.0f ) {
			positions[numActive] += direction * ( rand() * ( 1.0f / RAND_MAX ) ) * emitRadius;
		}
		velocities[numActive] = direction * ( ( rand() * ( 1.0f / RAND_MAX ) - 0.5f ) * emitVelocityVariance + emitVelocity );
		if ( GetBit( INHERITS_VELOCITY_POS ) ) {
			velocities[numActive] += inheritedVelocity;
		}
		birthTimes[numActive] = time - emitTimer;
		numActive++;
		return true;
	}
	return false;
}

/*
================
EmitterController::VirtualSlot39

Brings the emit position and direction, and the emit object's velocity,
into the target's model space, where the particles live.
================
*/
// FUNCTION: STBC 0x004cc640
// PARKED: register allocation: retail reloads the emit object from the member at each use and saves EBX only around the velocity block; this build keeps it in EBX throughout (accessor reads and a split expression tried)
void EmitterController::VirtualSlot39() {
	NiAVObject *targetObject = (NiAVObject *)target;
	if ( !targetObject ) {
		return;
	}
	if ( emitFrom ) {
		modelEmitDirection = emitFrom->GetWorldRotate() * emitDirection;
		float scale = emitFrom->GetWorldScale();
		modelEmitPosition = scale * ( emitFrom->GetWorldRotate() * emitPosition ) + emitFrom->GetWorldTranslate();
		if ( GetBit( INHERITS_VELOCITY_POS ) ) {
			inheritedVelocity = emitFrom->GetWorldVelocity();
			inheritedVelocity = ( inheritedVelocity * targetObject->GetWorldRotate() ) / targetObject->GetWorldScale();
		}
		modelEmitPosition = ( ( modelEmitPosition - targetObject->GetWorldTranslate() ) * targetObject->GetWorldRotate() ) / targetObject->GetWorldScale();
		modelEmitDirection = targetObject->GetWorldRotate().TransposeTimes( modelEmitDirection );
	} else {
		modelEmitPosition = ( ( emitPosition - targetObject->GetWorldTranslate() ) * targetObject->GetWorldRotate() ) / targetObject->GetWorldScale();
		modelEmitDirection = targetObject->GetWorldRotate().TransposeTimes( emitDirection );
	}
}

/*
================
EmitterController::Update

Ages the live particles, dropping the expired ones, then emits the
particles owed since the last update. The effect stops once nothing is
left alive or to emit.
================
*/
// FUNCTION: STBC 0x004cc8c0
void EmitterController::Update( float time ) {
	if ( GetField( STATE_MASK, STATE_POS ) != STATE_RUNNING ) {
		return;
	}
	if ( lastTime < 0.0f ) {
		lastTime = time - 0.01f;
	}
	float deltaTime = time - lastTime;
	if ( deltaTime < 0.0f ) {
		return;
	}
	VirtualSlot39();
	for ( int i = 0; i < numActive; i++ ) {
		float age = time - birthTimes[i];
		if ( age > lifeSpans[i] ) {
			VirtualSlot37( i );
			i--;
		} else {
			VirtualSlot35( i, time, deltaTime, age / lifeSpans[i] );
		}
	}
	emitTimer += deltaTime;
	while ( emitTimer >= emitFrequency && numToEmit > 0 && numActive < numParticles ) {
		emitTimer -= emitFrequency + ( rand() * ( 1.0f / RAND_MAX ) - 0.5f ) * emitFrequencyVariance;
		if ( VirtualSlot36( time ) ) {
			VirtualSlot35( numActive - 1, time, emitTimer, ( time - birthTimes[numActive - 1] ) / lifeSpans[numActive - 1] );
		}
		numToEmit--;
	}
	lastTime = time;
	if ( numActive == 0 && numToEmit <= 0 ) {
		SetField( STATE_STOPPED, STATE_MASK, STATE_POS );
	}
}

/*
================
EmitterController::DetachEffect

Also takes the emit object out of the scene graph, when the emitter owns it.
================
*/
// FUNCTION: STBC 0x004cca80
void EmitterController::DetachEffect() {
	if ( GetBit( DETACH_EMIT_OBJECT_POS ) && emitFrom && emitFrom->GetParent() ) {
		emitFrom->GetParent()->DetachChild( emitFrom );
		emitFrom = NULL;
	}
	EffectController::DetachEffect();
}

/*
================
EmitterController::SetEmitPositionAndDirection

The direction's matrix gets two axes across the direction, the first
the direction's perpendicular (built from the world axis it leans on
least), as rows.
================
*/
// FUNCTION: STBC 0x004ccb00
void EmitterController::SetEmitPositionAndDirection( const NiPoint3 &position, const NiPoint3 &direction ) {
	emitDirection.SetRow( 0, direction );
	NiPoint3 up = direction.Perpendicular();
	emitDirection.SetRow( 1, up );
	NiPoint3 right = up.Cross( direction );
	emitDirection.SetRow( 2, right );
	emitPosition = position;
}
