/*
===============================================================================

	DebrisParticleController.cpp

===============================================================================
*/

#include <iostream>
#include <stdlib.h>

#include "DebrisParticleController.h"
#include "NiMain.h"
#include "NiAnimation.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004c9400 _$E629
// FUNCTION: STBC 0x004c9420 _$E628

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004c8580 _$E32
// FUNCTION: STBC 0x004c8590 _$E29
// FUNCTION: STBC 0x004c85a0 _$E31
// FUNCTION: STBC 0x004c85b0 _$E30
// FUNCTION: STBC 0x004c85e0 _$E38
// FUNCTION: STBC 0x004c85f0 _$E35
// FUNCTION: STBC 0x004c8600 _$E37
// FUNCTION: STBC 0x004c8610 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004c8180 _$E80
// FUNCTION: STBC 0x004c8190 _$E79
// FUNCTION: STBC 0x004c81a0 _$E433
// FUNCTION: STBC 0x004c81b0 _$E432
// FUNCTION: STBC 0x004c81c0 _$E437
// FUNCTION: STBC 0x004c81d0 _$E436
// FUNCTION: STBC 0x004c81e0 _$E441
// FUNCTION: STBC 0x004c81f0 _$E440
// FUNCTION: STBC 0x004c8200 _$E464
// FUNCTION: STBC 0x004c8210 _$E463
// FUNCTION: STBC 0x004c8220 _$E455
// FUNCTION: STBC 0x004c8230 _$E454
// FUNCTION: STBC 0x004c8240 _$E468
// FUNCTION: STBC 0x004c8250 _$E467
// FUNCTION: STBC 0x004c8260 _$E472
// FUNCTION: STBC 0x004c8270 _$E471
// FUNCTION: STBC 0x004c8280 _$E533
// FUNCTION: STBC 0x004c8290 _$E532
// FUNCTION: STBC 0x004c82a0 _$E536
// FUNCTION: STBC 0x004c82b0 _$E535
// FUNCTION: STBC 0x004c82c0 _$E493
// FUNCTION: STBC 0x004c82d0 _$E492
// FUNCTION: STBC 0x004c82e0 _$E509
// FUNCTION: STBC 0x004c82f0 _$E508
// FUNCTION: STBC 0x004c8300 _$E476
// FUNCTION: STBC 0x004c8310 _$E475
// FUNCTION: STBC 0x004c8320 _$E516
// FUNCTION: STBC 0x004c8330 _$E515
// FUNCTION: STBC 0x004c8340 _$E519
// FUNCTION: STBC 0x004c8350 _$E518
// FUNCTION: STBC 0x004c8360 _$E555
// FUNCTION: STBC 0x004c8370 _$E554
// FUNCTION: STBC 0x004c8380 _$E479
// FUNCTION: STBC 0x004c8390 _$E478
// FUNCTION: STBC 0x004c83a0 _$E482
// FUNCTION: STBC 0x004c83b0 _$E481
// FUNCTION: STBC 0x004c83c0 _$E485
// FUNCTION: STBC 0x004c83d0 _$E484
// FUNCTION: STBC 0x004c83e0 _$E68
// FUNCTION: STBC 0x004c83f0 _$E67
// FUNCTION: STBC 0x004c8400 _$E489
// FUNCTION: STBC 0x004c8410 _$E488
// FUNCTION: STBC 0x004c8420 _$E522
// FUNCTION: STBC 0x004c8430 _$E521
// FUNCTION: STBC 0x004c8440 _$E497
// FUNCTION: STBC 0x004c8450 _$E496
// FUNCTION: STBC 0x004c8460 _$E501
// FUNCTION: STBC 0x004c8470 _$E500
// FUNCTION: STBC 0x004c8480 _$E505
// FUNCTION: STBC 0x004c8490 _$E504
// FUNCTION: STBC 0x004c84a0 _$E563
// FUNCTION: STBC 0x004c84b0 _$E562
// FUNCTION: STBC 0x004c84c0 _$E588
// FUNCTION: STBC 0x004c84d0 _$E587
// FUNCTION: STBC 0x004c84e0 _$E576
// FUNCTION: STBC 0x004c84f0 _$E575
// FUNCTION: STBC 0x004c8500 _$E513
// FUNCTION: STBC 0x004c8510 _$E512
// FUNCTION: STBC 0x004c8520 _$E526
// FUNCTION: STBC 0x004c8530 _$E525
// FUNCTION: STBC 0x004c8540 _$E530
// FUNCTION: STBC 0x004c8550 _$E529
// FUNCTION: STBC 0x004c8560 _$E71
// FUNCTION: STBC 0x004c8570 _$E70
// FUNCTION: STBC 0x004c85c0 _$E83
// FUNCTION: STBC 0x004c85d0 _$E82
// FUNCTION: STBC 0x004c8620 _$E540
// FUNCTION: STBC 0x004c8630 _$E539
// FUNCTION: STBC 0x004c8640 _$E548
// FUNCTION: STBC 0x004c8650 _$E547
// FUNCTION: STBC 0x004c8660 _$E552
// FUNCTION: STBC 0x004c8670 _$E551
// FUNCTION: STBC 0x004c8680 _$E559
// FUNCTION: STBC 0x004c8690 _$E558
// FUNCTION: STBC 0x004c86a0 _$E544
// FUNCTION: STBC 0x004c86b0 _$E543
// FUNCTION: STBC 0x004c86c0 _$E45
// FUNCTION: STBC 0x004c86d0 _$E44
// FUNCTION: STBC 0x004c86e0 _$E225
// FUNCTION: STBC 0x004c86f0 _$E224
// FUNCTION: STBC 0x004c8700 _$E572
// FUNCTION: STBC 0x004c8710 _$E571
// FUNCTION: STBC 0x004c8720 _$E568
// FUNCTION: STBC 0x004c8730 _$E567
// FUNCTION: STBC 0x004c8740 _$E580
// FUNCTION: STBC 0x004c8750 _$E579
// FUNCTION: STBC 0x004c8760 _$E584
// FUNCTION: STBC 0x004c8770 _$E583
// FUNCTION: STBC 0x004c8780 _$E97
// FUNCTION: STBC 0x004c8790 _$E96
// FUNCTION: STBC 0x004c87a0 _$E591
// FUNCTION: STBC 0x004c87b0 _$E590
// FUNCTION: STBC 0x004c87c0 _$E594
// FUNCTION: STBC 0x004c87d0 _$E593
// FUNCTION: STBC 0x004c87e0 _$E56
// FUNCTION: STBC 0x004c87f0 _$E55
// FUNCTION: STBC 0x004c8800 _$E598
// FUNCTION: STBC 0x004c8810 _$E597
// FUNCTION: STBC 0x004c8820 _$E101
// FUNCTION: STBC 0x004c8830 _$E100
// FUNCTION: STBC 0x004c8840 _$E602
// FUNCTION: STBC 0x004c8850 _$E601
// FUNCTION: STBC 0x004c8860 _$E105
// FUNCTION: STBC 0x004c8870 _$E104
// FUNCTION: STBC 0x004c8880 _$E218
// FUNCTION: STBC 0x004c8890 _$E217
// FUNCTION: STBC 0x004c8c20 _$E160
// FUNCTION: STBC 0x004c8c30 _$E159
// FUNCTION: STBC 0x004c8c40 _$E173
// FUNCTION: STBC 0x004c8c50 _$E172
// FUNCTION: STBC 0x004c8c60 _$E230
// FUNCTION: STBC 0x004c8c70 _$E229
// FUNCTION: STBC 0x004c8c80 _$E235
// FUNCTION: STBC 0x004c8c90 _$E234
// FUNCTION: STBC 0x004c8ca0 _$E239
// FUNCTION: STBC 0x004c8cb0 _$E238
// FUNCTION: STBC 0x004c8cc0 _$E243
// FUNCTION: STBC 0x004c8cd0 _$E242
// FUNCTION: STBC 0x004c8ce0 _$E250
// FUNCTION: STBC 0x004c8cf0 _$E249
// FUNCTION: STBC 0x004c8d00 _$E275
// FUNCTION: STBC 0x004c8d10 _$E274
// FUNCTION: STBC 0x004c8d20 _$E279
// FUNCTION: STBC 0x004c8d30 _$E278
// FUNCTION: STBC 0x004c8d40 _$E122
// FUNCTION: STBC 0x004c8d50 _$E121
// FUNCTION: STBC 0x004c8d60 _$E261
// FUNCTION: STBC 0x004c8d70 _$E260
// FUNCTION: STBC 0x004c8d80 _$E268
// FUNCTION: STBC 0x004c8d90 _$E267
// FUNCTION: STBC 0x004c8da0 _$E291
// FUNCTION: STBC 0x004c8db0 _$E290
// FUNCTION: STBC 0x004c8dc0 _$E295
// FUNCTION: STBC 0x004c8dd0 _$E294
// FUNCTION: STBC 0x004c8de0 _$E335
// FUNCTION: STBC 0x004c8df0 _$E334
// FUNCTION: STBC 0x004c8e00 _$E341
// FUNCTION: STBC 0x004c8e10 _$E340
// FUNCTION: STBC 0x004c8e20 _$E283
// FUNCTION: STBC 0x004c8e30 _$E282
// FUNCTION: STBC 0x004c8e40 _$E287
// FUNCTION: STBC 0x004c8e50 _$E286
// FUNCTION: STBC 0x004c8e60 _$E299
// FUNCTION: STBC 0x004c8e70 _$E298
// FUNCTION: STBC 0x004c8e80 _$E303
// FUNCTION: STBC 0x004c8e90 _$E302
// FUNCTION: STBC 0x004c8ea0 _$E307
// FUNCTION: STBC 0x004c8eb0 _$E306
// FUNCTION: STBC 0x004c8ec0 _$E311
// FUNCTION: STBC 0x004c8ed0 _$E310
// FUNCTION: STBC 0x004c8ee0 _$E315
// FUNCTION: STBC 0x004c8ef0 _$E314
// FUNCTION: STBC 0x004c8f00 _$E319
// FUNCTION: STBC 0x004c8f10 _$E318
// FUNCTION: STBC 0x004c8f20 _$E323
// FUNCTION: STBC 0x004c8f30 _$E322
// FUNCTION: STBC 0x004c8f40 _$E327
// FUNCTION: STBC 0x004c8f50 _$E326
// FUNCTION: STBC 0x004c8f60 _$E127
// FUNCTION: STBC 0x004c8f70 _$E126
// FUNCTION: STBC 0x004c8f80 _$E331
// FUNCTION: STBC 0x004c8f90 _$E330
// FUNCTION: STBC 0x004c8fa0 _$E132
// FUNCTION: STBC 0x004c8fb0 _$E131
// FUNCTION: STBC 0x004c8fc0 _$E345
// FUNCTION: STBC 0x004c8fd0 _$E344
// FUNCTION: STBC 0x004c8fe0 _$E349
// FUNCTION: STBC 0x004c8ff0 _$E348
// FUNCTION: STBC 0x004c9000 _$E353
// FUNCTION: STBC 0x004c9010 _$E352
// FUNCTION: STBC 0x004c9020 _$E357
// FUNCTION: STBC 0x004c9030 _$E356
// FUNCTION: STBC 0x004c9040 _$E137
// FUNCTION: STBC 0x004c9050 _$E136
// FUNCTION: STBC 0x004c9060 _$E361
// FUNCTION: STBC 0x004c9070 _$E360
// FUNCTION: STBC 0x004c9080 _$E365
// FUNCTION: STBC 0x004c9090 _$E364
// FUNCTION: STBC 0x004c90a0 _$E369
// FUNCTION: STBC 0x004c90b0 _$E368
// FUNCTION: STBC 0x004c90c0 _$E143
// FUNCTION: STBC 0x004c90d0 _$E142
// FUNCTION: STBC 0x004c90e0 _$E148
// FUNCTION: STBC 0x004c90f0 _$E147
// FUNCTION: STBC 0x004c9100 _$E178
// FUNCTION: STBC 0x004c9110 _$E177
// FUNCTION: STBC 0x004c9120 _$E373
// FUNCTION: STBC 0x004c9130 _$E372
// FUNCTION: STBC 0x004c9140 _$E183
// FUNCTION: STBC 0x004c9150 _$E182
// FUNCTION: STBC 0x004c9160 _$E377
// FUNCTION: STBC 0x004c9170 _$E376
// FUNCTION: STBC 0x004c9180 _$E381
// FUNCTION: STBC 0x004c9190 _$E380
// FUNCTION: STBC 0x004c91a0 _$E188
// FUNCTION: STBC 0x004c91b0 _$E187
// FUNCTION: STBC 0x004c91c0 _$E385
// FUNCTION: STBC 0x004c91d0 _$E384
// FUNCTION: STBC 0x004c91e0 _$E153
// FUNCTION: STBC 0x004c91f0 _$E152
// FUNCTION: STBC 0x004c9200 _$E389
// FUNCTION: STBC 0x004c9210 _$E388
// FUNCTION: STBC 0x004c9220 _$E393
// FUNCTION: STBC 0x004c9230 _$E392
// FUNCTION: STBC 0x004c9240 _$E193
// FUNCTION: STBC 0x004c9250 _$E192
// FUNCTION: STBC 0x004c9260 _$E198
// FUNCTION: STBC 0x004c9270 _$E197
// FUNCTION: STBC 0x004c9280 _$E397
// FUNCTION: STBC 0x004c9290 _$E396
// FUNCTION: STBC 0x004c92a0 _$E401
// FUNCTION: STBC 0x004c92b0 _$E400
// FUNCTION: STBC 0x004c92c0 _$E405
// FUNCTION: STBC 0x004c92d0 _$E404
// FUNCTION: STBC 0x004c92e0 _$E409
// FUNCTION: STBC 0x004c92f0 _$E408
// FUNCTION: STBC 0x004c9300 _$E413
// FUNCTION: STBC 0x004c9310 _$E412
// FUNCTION: STBC 0x004c9320 _$E417
// FUNCTION: STBC 0x004c9330 _$E416
// FUNCTION: STBC 0x004c9340 _$E421
// FUNCTION: STBC 0x004c9350 _$E420
// FUNCTION: STBC 0x004c9360 _$E425
// FUNCTION: STBC 0x004c9370 _$E424
// FUNCTION: STBC 0x004c9380 _$E203
// FUNCTION: STBC 0x004c9390 _$E202
// FUNCTION: STBC 0x004c93a0 _$E429
// FUNCTION: STBC 0x004c93b0 _$E428
// FUNCTION: STBC 0x004c93c0 _$E208
// FUNCTION: STBC 0x004c93d0 _$E207
// FUNCTION: STBC 0x004c93e0 _$E213
// FUNCTION: STBC 0x004c93f0 _$E212
// FUNCTION: STBC 0x004c9430 _$E606
// FUNCTION: STBC 0x004c9440 _$E605
// FUNCTION: STBC 0x004c9450 _$E611
// FUNCTION: STBC 0x004c9460 _$E610
// FUNCTION: STBC 0x004c9470 _$E615
// FUNCTION: STBC 0x004c9480 _$E614

// The NiRTTI's dynamic initialiser, the registration of its exit-time destructor and the guarded destructor.
// FUNCTION: STBC 0x004c88a0 _$E621
// FUNCTION: STBC 0x004c88b0 _$E617
// FUNCTION: STBC 0x004c88d0 _$E620
// FUNCTION: STBC 0x004c88e0 _$E618
// DATA: STBC 0x00984938 ?rtti@DebrisParticleController@@2VNiRTTI@@B
const NiRTTI DebrisParticleController::rtti( "DebrisParticleController", &PointParticleController::rtti );

/*
================
DebrisParticleController::Create
================
*/
// FUNCTION: STBC 0x004c8900
DebrisParticleController *DebrisParticleController::Create( float lifeTime, float emitLife, float emitFrequency, float emitVelocity, float angleVariance, NiAVObject *emitFrom ) {
	return new DebrisParticleController( lifeTime, emitLife, emitFrequency, emitVelocity, angleVariance, emitFrom );
}

/*
================
DebrisParticleController::DebrisParticleController

The emitter is built without an emit object.
================
*/
// FUNCTION: STBC 0x004c8980
// BUG: emitFrom_ is dropped, as PointParticleController's constructor drops its own
DebrisParticleController::DebrisParticleController( float lifeTime_, float emitLife_, float emitFrequency_, float emitVelocity_, float angleVariance_, NiAVObject *emitFrom_ ) : PointParticleController( lifeTime_, emitLife_, emitFrequency_, emitVelocity_, angleVariance_, NULL ) {
}

/*
================
DebrisParticleController::CreateClone
================
*/
// FUNCTION: STBC 0x004c8a00
NiObject *DebrisParticleController::CreateClone() {
	DebrisParticleController *clone = new DebrisParticleController( 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, NULL );
	CopyMembers( clone );
	return clone;
}

/*
================
DebrisParticleController::CopyMembers
================
*/
// FUNCTION: STBC 0x004c8a80
void DebrisParticleController::CopyMembers( DebrisParticleController *dest ) {
	EmitterController::CopyMembers( dest );
}

/*
================
DebrisParticleController::VirtualSlot35

Debris keeps the size it was given; it is coloured over its life, and its
velocity is updated before it moves.
================
*/
// FUNCTION: STBC 0x004c8a90
void DebrisParticleController::VirtualSlot35( int index, float time, float deltaTime, float lifeFraction ) {
	GetColor( lifeFraction, colors[index].r, colors[index].g, colors[index].b );
	GetAlpha( lifeFraction, colors[index].a );
	velocities[index] += gravity * deltaTime;
	velocities[index] -= deltaTime * damping * velocities[index];
	positions[index] += velocities[index] * deltaTime;
}

/*
================
DebrisParticleController::VirtualSlot36

Each piece of debris gets a size from a random point of the size track.
================
*/
// FUNCTION: STBC 0x004c8bc0
bool DebrisParticleController::VirtualSlot36( float time ) {
	if ( PointParticleController::VirtualSlot36( time ) ) {
		float fraction = rand() * ( 1.0f / RAND_MAX );
		GetSize( fraction, sizes[numActive - 1] );
		return true;
	}
	return false;
}
