/*
===============================================================================

	ExplosionPlumeController.cpp

===============================================================================
*/

#include <iostream>
#include <stdlib.h>

#include "ExplosionPlumeController.h"
#include "NiNode.h"
#include "NiMath.h"
#include "NiPick.h"
#include "NiMain.h"
#include "NiAnimation.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004d0640 _$E622
// FUNCTION: STBC 0x004d0660 _$E621

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004ceef0 _$E32
// FUNCTION: STBC 0x004cef00 _$E29
// FUNCTION: STBC 0x004cef10 _$E31
// FUNCTION: STBC 0x004cef20 _$E30
// FUNCTION: STBC 0x004cef50 _$E38
// FUNCTION: STBC 0x004cef60 _$E35
// FUNCTION: STBC 0x004cef70 _$E37
// FUNCTION: STBC 0x004cef80 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004ceaf0 _$E80
// FUNCTION: STBC 0x004ceb00 _$E79
// FUNCTION: STBC 0x004ceb10 _$E432
// FUNCTION: STBC 0x004ceb20 _$E431
// FUNCTION: STBC 0x004ceb30 _$E436
// FUNCTION: STBC 0x004ceb40 _$E435
// FUNCTION: STBC 0x004ceb50 _$E440
// FUNCTION: STBC 0x004ceb60 _$E439
// FUNCTION: STBC 0x004ceb70 _$E463
// FUNCTION: STBC 0x004ceb80 _$E462
// FUNCTION: STBC 0x004ceb90 _$E454
// FUNCTION: STBC 0x004ceba0 _$E453
// FUNCTION: STBC 0x004cebb0 _$E467
// FUNCTION: STBC 0x004cebc0 _$E466
// FUNCTION: STBC 0x004cebd0 _$E471
// FUNCTION: STBC 0x004cebe0 _$E470
// FUNCTION: STBC 0x004cebf0 _$E532
// FUNCTION: STBC 0x004cec00 _$E531
// FUNCTION: STBC 0x004cec10 _$E535
// FUNCTION: STBC 0x004cec20 _$E534
// FUNCTION: STBC 0x004cec30 _$E492
// FUNCTION: STBC 0x004cec40 _$E491
// FUNCTION: STBC 0x004cec50 _$E508
// FUNCTION: STBC 0x004cec60 _$E507
// FUNCTION: STBC 0x004cec70 _$E475
// FUNCTION: STBC 0x004cec80 _$E474
// FUNCTION: STBC 0x004cec90 _$E515
// FUNCTION: STBC 0x004ceca0 _$E514
// FUNCTION: STBC 0x004cecb0 _$E518
// FUNCTION: STBC 0x004cecc0 _$E517
// FUNCTION: STBC 0x004cecd0 _$E554
// FUNCTION: STBC 0x004cece0 _$E553
// FUNCTION: STBC 0x004cecf0 _$E478
// FUNCTION: STBC 0x004ced00 _$E477
// FUNCTION: STBC 0x004ced10 _$E481
// FUNCTION: STBC 0x004ced20 _$E480
// FUNCTION: STBC 0x004ced30 _$E484
// FUNCTION: STBC 0x004ced40 _$E483
// FUNCTION: STBC 0x004ced50 _$E68
// FUNCTION: STBC 0x004ced60 _$E67
// FUNCTION: STBC 0x004ced70 _$E488
// FUNCTION: STBC 0x004ced80 _$E487
// FUNCTION: STBC 0x004ced90 _$E521
// FUNCTION: STBC 0x004ceda0 _$E520
// FUNCTION: STBC 0x004cedb0 _$E496
// FUNCTION: STBC 0x004cedc0 _$E495
// FUNCTION: STBC 0x004cedd0 _$E500
// FUNCTION: STBC 0x004cede0 _$E499
// FUNCTION: STBC 0x004cedf0 _$E504
// FUNCTION: STBC 0x004cee00 _$E503
// FUNCTION: STBC 0x004cee10 _$E562
// FUNCTION: STBC 0x004cee20 _$E561
// FUNCTION: STBC 0x004cee30 _$E587
// FUNCTION: STBC 0x004cee40 _$E586
// FUNCTION: STBC 0x004cee50 _$E575
// FUNCTION: STBC 0x004cee60 _$E574
// FUNCTION: STBC 0x004cee70 _$E512
// FUNCTION: STBC 0x004cee80 _$E511
// FUNCTION: STBC 0x004cee90 _$E525
// FUNCTION: STBC 0x004ceea0 _$E524
// FUNCTION: STBC 0x004ceeb0 _$E529
// FUNCTION: STBC 0x004ceec0 _$E528
// FUNCTION: STBC 0x004ceed0 _$E71
// FUNCTION: STBC 0x004ceee0 _$E70
// FUNCTION: STBC 0x004cef30 _$E83
// FUNCTION: STBC 0x004cef40 _$E82
// FUNCTION: STBC 0x004cef90 _$E539
// FUNCTION: STBC 0x004cefa0 _$E538
// FUNCTION: STBC 0x004cefb0 _$E547
// FUNCTION: STBC 0x004cefc0 _$E546
// FUNCTION: STBC 0x004cefd0 _$E551
// FUNCTION: STBC 0x004cefe0 _$E550
// FUNCTION: STBC 0x004ceff0 _$E558
// FUNCTION: STBC 0x004cf000 _$E557
// FUNCTION: STBC 0x004cf010 _$E543
// FUNCTION: STBC 0x004cf020 _$E542
// FUNCTION: STBC 0x004cf030 _$E45
// FUNCTION: STBC 0x004cf040 _$E44
// FUNCTION: STBC 0x004cf050 _$E236
// FUNCTION: STBC 0x004cf060 _$E235
// FUNCTION: STBC 0x004cf070 _$E571
// FUNCTION: STBC 0x004cf080 _$E570
// FUNCTION: STBC 0x004cf090 _$E567
// FUNCTION: STBC 0x004cf0a0 _$E566
// FUNCTION: STBC 0x004cf0b0 _$E579
// FUNCTION: STBC 0x004cf0c0 _$E578
// FUNCTION: STBC 0x004cf0d0 _$E583
// FUNCTION: STBC 0x004cf0e0 _$E582
// FUNCTION: STBC 0x004cf0f0 _$E97
// FUNCTION: STBC 0x004cf100 _$E96
// FUNCTION: STBC 0x004cf110 _$E590
// FUNCTION: STBC 0x004cf120 _$E589
// FUNCTION: STBC 0x004cf130 _$E593
// FUNCTION: STBC 0x004cf140 _$E592
// FUNCTION: STBC 0x004cf150 _$E56
// FUNCTION: STBC 0x004cf160 _$E55
// FUNCTION: STBC 0x004cf170 _$E597
// FUNCTION: STBC 0x004cf180 _$E596
// FUNCTION: STBC 0x004cf190 _$E101
// FUNCTION: STBC 0x004cf1a0 _$E100
// FUNCTION: STBC 0x004cf1b0 _$E601
// FUNCTION: STBC 0x004cf1c0 _$E600
// FUNCTION: STBC 0x004cf1d0 _$E105
// FUNCTION: STBC 0x004cf1e0 _$E104
// FUNCTION: STBC 0x004cf1f0 _$E218
// FUNCTION: STBC 0x004cf200 _$E217
// FUNCTION: STBC 0x004cfe60 _$E160
// FUNCTION: STBC 0x004cfe70 _$E159
// FUNCTION: STBC 0x004cfe80 _$E173
// FUNCTION: STBC 0x004cfe90 _$E172
// FUNCTION: STBC 0x004cfea0 _$E225
// FUNCTION: STBC 0x004cfeb0 _$E224
// FUNCTION: STBC 0x004cfec0 _$E241
// FUNCTION: STBC 0x004cfed0 _$E240
// FUNCTION: STBC 0x004cfee0 _$E229
// FUNCTION: STBC 0x004cfef0 _$E228
// FUNCTION: STBC 0x004cff00 _$E245
// FUNCTION: STBC 0x004cff10 _$E244
// FUNCTION: STBC 0x004cff20 _$E252
// FUNCTION: STBC 0x004cff30 _$E251
// FUNCTION: STBC 0x004cff40 _$E274
// FUNCTION: STBC 0x004cff50 _$E273
// FUNCTION: STBC 0x004cff60 _$E278
// FUNCTION: STBC 0x004cff70 _$E277
// FUNCTION: STBC 0x004cff80 _$E122
// FUNCTION: STBC 0x004cff90 _$E121
// FUNCTION: STBC 0x004cffa0 _$E260
// FUNCTION: STBC 0x004cffb0 _$E259
// FUNCTION: STBC 0x004cffc0 _$E267
// FUNCTION: STBC 0x004cffd0 _$E266
// FUNCTION: STBC 0x004cffe0 _$E290
// FUNCTION: STBC 0x004cfff0 _$E289
// FUNCTION: STBC 0x004d0000 _$E294
// FUNCTION: STBC 0x004d0010 _$E293
// FUNCTION: STBC 0x004d0020 _$E334
// FUNCTION: STBC 0x004d0030 _$E333
// FUNCTION: STBC 0x004d0040 _$E340
// FUNCTION: STBC 0x004d0050 _$E339
// FUNCTION: STBC 0x004d0060 _$E282
// FUNCTION: STBC 0x004d0070 _$E281
// FUNCTION: STBC 0x004d0080 _$E286
// FUNCTION: STBC 0x004d0090 _$E285
// FUNCTION: STBC 0x004d00a0 _$E298
// FUNCTION: STBC 0x004d00b0 _$E297
// FUNCTION: STBC 0x004d00c0 _$E302
// FUNCTION: STBC 0x004d00d0 _$E301
// FUNCTION: STBC 0x004d00e0 _$E306
// FUNCTION: STBC 0x004d00f0 _$E305
// FUNCTION: STBC 0x004d0100 _$E310
// FUNCTION: STBC 0x004d0110 _$E309
// FUNCTION: STBC 0x004d0120 _$E314
// FUNCTION: STBC 0x004d0130 _$E313
// FUNCTION: STBC 0x004d0140 _$E318
// FUNCTION: STBC 0x004d0150 _$E317
// FUNCTION: STBC 0x004d0160 _$E322
// FUNCTION: STBC 0x004d0170 _$E321
// FUNCTION: STBC 0x004d0180 _$E326
// FUNCTION: STBC 0x004d0190 _$E325
// FUNCTION: STBC 0x004d01a0 _$E127
// FUNCTION: STBC 0x004d01b0 _$E126
// FUNCTION: STBC 0x004d01c0 _$E330
// FUNCTION: STBC 0x004d01d0 _$E329
// FUNCTION: STBC 0x004d01e0 _$E132
// FUNCTION: STBC 0x004d01f0 _$E131
// FUNCTION: STBC 0x004d0200 _$E344
// FUNCTION: STBC 0x004d0210 _$E343
// FUNCTION: STBC 0x004d0220 _$E348
// FUNCTION: STBC 0x004d0230 _$E347
// FUNCTION: STBC 0x004d0240 _$E352
// FUNCTION: STBC 0x004d0250 _$E351
// FUNCTION: STBC 0x004d0260 _$E356
// FUNCTION: STBC 0x004d0270 _$E355
// FUNCTION: STBC 0x004d0280 _$E137
// FUNCTION: STBC 0x004d0290 _$E136
// FUNCTION: STBC 0x004d02a0 _$E360
// FUNCTION: STBC 0x004d02b0 _$E359
// FUNCTION: STBC 0x004d02c0 _$E364
// FUNCTION: STBC 0x004d02d0 _$E363
// FUNCTION: STBC 0x004d02e0 _$E368
// FUNCTION: STBC 0x004d02f0 _$E367
// FUNCTION: STBC 0x004d0300 _$E143
// FUNCTION: STBC 0x004d0310 _$E142
// FUNCTION: STBC 0x004d0320 _$E148
// FUNCTION: STBC 0x004d0330 _$E147
// FUNCTION: STBC 0x004d0340 _$E178
// FUNCTION: STBC 0x004d0350 _$E177
// FUNCTION: STBC 0x004d0360 _$E372
// FUNCTION: STBC 0x004d0370 _$E371
// FUNCTION: STBC 0x004d0380 _$E183
// FUNCTION: STBC 0x004d0390 _$E182
// FUNCTION: STBC 0x004d03a0 _$E376
// FUNCTION: STBC 0x004d03b0 _$E375
// FUNCTION: STBC 0x004d03c0 _$E380
// FUNCTION: STBC 0x004d03d0 _$E379
// FUNCTION: STBC 0x004d03e0 _$E188
// FUNCTION: STBC 0x004d03f0 _$E187
// FUNCTION: STBC 0x004d0400 _$E384
// FUNCTION: STBC 0x004d0410 _$E383
// FUNCTION: STBC 0x004d0420 _$E153
// FUNCTION: STBC 0x004d0430 _$E152
// FUNCTION: STBC 0x004d0440 _$E388
// FUNCTION: STBC 0x004d0450 _$E387
// FUNCTION: STBC 0x004d0460 _$E392
// FUNCTION: STBC 0x004d0470 _$E391
// FUNCTION: STBC 0x004d0480 _$E193
// FUNCTION: STBC 0x004d0490 _$E192
// FUNCTION: STBC 0x004d04a0 _$E198
// FUNCTION: STBC 0x004d04b0 _$E197
// FUNCTION: STBC 0x004d04c0 _$E396
// FUNCTION: STBC 0x004d04d0 _$E395
// FUNCTION: STBC 0x004d04e0 _$E400
// FUNCTION: STBC 0x004d04f0 _$E399
// FUNCTION: STBC 0x004d0500 _$E404
// FUNCTION: STBC 0x004d0510 _$E403
// FUNCTION: STBC 0x004d0520 _$E408
// FUNCTION: STBC 0x004d0530 _$E407
// FUNCTION: STBC 0x004d0540 _$E412
// FUNCTION: STBC 0x004d0550 _$E411
// FUNCTION: STBC 0x004d0560 _$E416
// FUNCTION: STBC 0x004d0570 _$E415
// FUNCTION: STBC 0x004d0580 _$E420
// FUNCTION: STBC 0x004d0590 _$E419
// FUNCTION: STBC 0x004d05a0 _$E424
// FUNCTION: STBC 0x004d05b0 _$E423
// FUNCTION: STBC 0x004d05c0 _$E203
// FUNCTION: STBC 0x004d05d0 _$E202
// FUNCTION: STBC 0x004d05e0 _$E428
// FUNCTION: STBC 0x004d05f0 _$E427
// FUNCTION: STBC 0x004d0600 _$E208
// FUNCTION: STBC 0x004d0610 _$E207
// FUNCTION: STBC 0x004d0620 _$E213
// FUNCTION: STBC 0x004d0630 _$E212
// FUNCTION: STBC 0x004d0670 _$E605
// FUNCTION: STBC 0x004d0680 _$E604
// FUNCTION: STBC 0x004d0690 _$E610
// FUNCTION: STBC 0x004d06a0 _$E609
// FUNCTION: STBC 0x004d06b0 _$E614
// FUNCTION: STBC 0x004d06c0 _$E613

/*
================
ExplosionPlumeController::ExplosionPlumeController
================
*/
// FUNCTION: STBC 0x004cf210
ExplosionPlumeController::ExplosionPlumeController() : emitterVelocity( NiPoint3::ZERO ) {
	pick = NULL;
}

/*
================
ExplosionPlumeController::~ExplosionPlumeController
================
*/
// FUNCTION: STBC 0x004cf270
ExplosionPlumeController::~ExplosionPlumeController() {
	delete pick;
}

/*
================
ExplosionPlumeController::SetPickRoot

Sets up a pick of the subtree's triangles, nearest first, with their
normals and texture coordinates in world space.
================
*/
// FUNCTION: STBC 0x004cf2e0
void ExplosionPlumeController::SetPickRoot( NiNode *root ) {
	if ( root ) {
		if ( pick == NULL ) {
			pick = new NiPick;
			pick->SetPickType( NiPick::FIND_ALL );
			pick->SetSortType( NiPick::SORT );
			pick->SetIntersectType( NiPick::TRIANGLE_INTERSECT );
			pick->SetFrontOnly( true );
			pick->SetReturnTexture( false );
			pick->SetReturnNormal( true );
			pick->SetReturnColor( false );
			pick->SetCoordinateType( NiPick::WORLD_COORDINATES );
		}
		pick->SetTarget( root );
	} else {
		delete pick;
	}
	// BUG: forgets the pick in both cases: the one just set up leaks, and
	// the next call builds another
	pick = NULL;
}

/*
================
ExplosionPlumeController::Update

Keeps the time since the plume started.
================
*/
// FUNCTION: STBC 0x004cf3f0
void ExplosionPlumeController::Update( float time ) {
	if ( lastTime >= 0.0f ) {
		elapsed = time - lastTime;
	} else {
		elapsed = 0.0f;
	}
	AnimTSParticleController::Update( time );
}

/*
================
ExplosionPlumeController::SetUpRandomVelocity
================
*/
// FUNCTION: STBC 0x004cf440
void ExplosionPlumeController::SetUpRandomVelocity( float angleVariance, float speed, float speedVariance ) {
	NiPoint3 direction = NiPoint3::UNIT_X;
	NiMatrix3 rotation;
	float angle = rand() * ( angleVariance * ( 1.0f / 360.0f ) ) * NI_PI * ( 1.0f / RAND_MAX );
	rotation.MakeZRotation( angle );
	direction = rotation * direction;
	angle = rand() * NI_TWO_PI * ( 1.0f / RAND_MAX );
	rotation.MakeXRotation( angle );
	direction = rotation * direction;
	if ( emitFrom ) {
		direction = emitFrom->GetWorldRotate() * direction;
	}
	direction.Unitize();
	float randomSpeed = rand() * speedVariance * ( 1.0f / RAND_MAX ) + speed;
	emitterVelocity = direction * randomSpeed;
	EmitterController::VirtualSlot39();
}

/*
================
ExplosionPlumeController::VirtualSlot39

Moves the plume's emit position by its velocity over the time since the
last update, in world space. A hit of the pick against the scene turns the
velocity round off the surface's normal at a fraction of its speed, and
leaves the rest of the time to the next step; with nothing to hit, the
velocity is now and then nudged in a random direction. Gravity and damping
act on the velocity, then the emit position and direction are brought into
the target's model space as EmitterController::VirtualSlot39 does.
================
*/
// FUNCTION: STBC 0x004cf610
// DRAFT: structure follows retail's operations in order; retail expands the reflection's subtraction and calls NiPoint3's constructor in the * 0.6f (ours the reverse), keeps the address of modelEmitPosition in EBX and has a frame 0x18 smaller
void ExplosionPlumeController::VirtualSlot39() {
	if ( elapsed <= 0.0f ) {
		return;
	}
	NiAVObject *targetObject = (NiAVObject *)target;
	if ( !targetObject ) {
		return;
	}

	NiPoint3 worldPosition = targetObject->GetWorldRotate() * ( modelEmitPosition * targetObject->GetWorldScale() ) + targetObject->GetWorldTranslate();
	NiPoint3 worldMove = ( targetObject->GetWorldRotate() * emitterVelocity ) * targetObject->GetWorldScale() * elapsed;

	if ( pick && pick->PickObjects( worldPosition, worldMove, false ) ) {
		NiPick::Record *record = pick->GetRecord( 0 );
		if ( record->distance <= 2.0f ) {
			NiPoint3 normal = record->normal * targetObject->GetWorldRotate();
			NiPoint3 incoming = -emitterVelocity;
			float along = incoming * normal;
			emitterVelocity = ( normal * ( along + along ) - incoming ) * 0.6f;
			elapsed = elapsed - record->distance * elapsed;
			emitPosition = ( ( record->intersection - targetObject->GetWorldTranslate() ) * targetObject->GetWorldRotate() ) / targetObject->GetWorldScale() - modelEmitPosition;
		}
	} else if ( rand() % 200 <= 1 ) {
		NiPoint3 kick( rand() * ( 2.0f / RAND_MAX ) - 1.0f, rand() * ( 2.0f / RAND_MAX ) - 1.0f, rand() * ( 2.0f / RAND_MAX ) - 1.0f );
		kick.Unitize();
		float speed = emitterVelocity.Length();
		NiPoint3 keep = emitterVelocity * 0.7f;
		NiPoint3 add = kick * 0.3f;
		emitterVelocity = keep + add * speed;
	}

	if ( elapsed > 0.0f ) {
		emitPosition += elapsed * emitterVelocity;
		emitterVelocity += elapsed * gravity;
		if ( damping > 0.0f ) {
			emitterVelocity -= ( damping * elapsed ) * emitterVelocity;
		}
	}

	// the same transform as EmitterController::VirtualSlot39
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
