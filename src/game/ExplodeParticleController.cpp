/*
===============================================================================

	ExplodeParticleController.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "ExplodeParticleController.h"
#include "NiNode.h"
#include "SetClass.h"
#include "SetManager.h"
#include "NiMain.h"
#include "NiAnimation.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004cea60 _$E657
// FUNCTION: STBC 0x004cea80 _$E656

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004cd870 _$E32
// FUNCTION: STBC 0x004cd880 _$E29
// FUNCTION: STBC 0x004cd890 _$E31
// FUNCTION: STBC 0x004cd8a0 _$E30
// FUNCTION: STBC 0x004cd8d0 _$E38
// FUNCTION: STBC 0x004cd8e0 _$E35
// FUNCTION: STBC 0x004cd8f0 _$E37
// FUNCTION: STBC 0x004cd900 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004cd470 _$E80
// FUNCTION: STBC 0x004cd480 _$E79
// FUNCTION: STBC 0x004cd490 _$E455
// FUNCTION: STBC 0x004cd4a0 _$E454
// FUNCTION: STBC 0x004cd4b0 _$E459
// FUNCTION: STBC 0x004cd4c0 _$E458
// FUNCTION: STBC 0x004cd4d0 _$E463
// FUNCTION: STBC 0x004cd4e0 _$E462
// FUNCTION: STBC 0x004cd4f0 _$E486
// FUNCTION: STBC 0x004cd500 _$E485
// FUNCTION: STBC 0x004cd510 _$E477
// FUNCTION: STBC 0x004cd520 _$E476
// FUNCTION: STBC 0x004cd530 _$E490
// FUNCTION: STBC 0x004cd540 _$E489
// FUNCTION: STBC 0x004cd550 _$E494
// FUNCTION: STBC 0x004cd560 _$E493
// FUNCTION: STBC 0x004cd570 _$E555
// FUNCTION: STBC 0x004cd580 _$E554
// FUNCTION: STBC 0x004cd590 _$E558
// FUNCTION: STBC 0x004cd5a0 _$E557
// FUNCTION: STBC 0x004cd5b0 _$E515
// FUNCTION: STBC 0x004cd5c0 _$E514
// FUNCTION: STBC 0x004cd5d0 _$E531
// FUNCTION: STBC 0x004cd5e0 _$E530
// FUNCTION: STBC 0x004cd5f0 _$E498
// FUNCTION: STBC 0x004cd600 _$E497
// FUNCTION: STBC 0x004cd610 _$E538
// FUNCTION: STBC 0x004cd620 _$E537
// FUNCTION: STBC 0x004cd630 _$E541
// FUNCTION: STBC 0x004cd640 _$E540
// FUNCTION: STBC 0x004cd650 _$E577
// FUNCTION: STBC 0x004cd660 _$E576
// FUNCTION: STBC 0x004cd670 _$E501
// FUNCTION: STBC 0x004cd680 _$E500
// FUNCTION: STBC 0x004cd690 _$E504
// FUNCTION: STBC 0x004cd6a0 _$E503
// FUNCTION: STBC 0x004cd6b0 _$E507
// FUNCTION: STBC 0x004cd6c0 _$E506
// FUNCTION: STBC 0x004cd6d0 _$E68
// FUNCTION: STBC 0x004cd6e0 _$E67
// FUNCTION: STBC 0x004cd6f0 _$E511
// FUNCTION: STBC 0x004cd700 _$E510
// FUNCTION: STBC 0x004cd710 _$E544
// FUNCTION: STBC 0x004cd720 _$E543
// FUNCTION: STBC 0x004cd730 _$E519
// FUNCTION: STBC 0x004cd740 _$E518
// FUNCTION: STBC 0x004cd750 _$E523
// FUNCTION: STBC 0x004cd760 _$E522
// FUNCTION: STBC 0x004cd770 _$E527
// FUNCTION: STBC 0x004cd780 _$E526
// FUNCTION: STBC 0x004cd790 _$E585
// FUNCTION: STBC 0x004cd7a0 _$E584
// FUNCTION: STBC 0x004cd7b0 _$E610
// FUNCTION: STBC 0x004cd7c0 _$E609
// FUNCTION: STBC 0x004cd7d0 _$E598
// FUNCTION: STBC 0x004cd7e0 _$E597
// FUNCTION: STBC 0x004cd7f0 _$E535
// FUNCTION: STBC 0x004cd800 _$E534
// FUNCTION: STBC 0x004cd810 _$E548
// FUNCTION: STBC 0x004cd820 _$E547
// FUNCTION: STBC 0x004cd830 _$E552
// FUNCTION: STBC 0x004cd840 _$E551
// FUNCTION: STBC 0x004cd850 _$E71
// FUNCTION: STBC 0x004cd860 _$E70
// FUNCTION: STBC 0x004cd8b0 _$E83
// FUNCTION: STBC 0x004cd8c0 _$E82
// FUNCTION: STBC 0x004cd910 _$E562
// FUNCTION: STBC 0x004cd920 _$E561
// FUNCTION: STBC 0x004cd930 _$E570
// FUNCTION: STBC 0x004cd940 _$E569
// FUNCTION: STBC 0x004cd950 _$E574
// FUNCTION: STBC 0x004cd960 _$E573
// FUNCTION: STBC 0x004cd970 _$E581
// FUNCTION: STBC 0x004cd980 _$E580
// FUNCTION: STBC 0x004cd990 _$E566
// FUNCTION: STBC 0x004cd9a0 _$E565
// FUNCTION: STBC 0x004cd9b0 _$E45
// FUNCTION: STBC 0x004cd9c0 _$E44
// FUNCTION: STBC 0x004cd9d0 _$E263
// FUNCTION: STBC 0x004cd9e0 _$E262
// FUNCTION: STBC 0x004cd9f0 _$E594
// FUNCTION: STBC 0x004cda00 _$E593
// FUNCTION: STBC 0x004cda10 _$E590
// FUNCTION: STBC 0x004cda20 _$E589
// FUNCTION: STBC 0x004cda30 _$E602
// FUNCTION: STBC 0x004cda40 _$E601
// FUNCTION: STBC 0x004cda50 _$E606
// FUNCTION: STBC 0x004cda60 _$E605
// FUNCTION: STBC 0x004cda70 _$E97
// FUNCTION: STBC 0x004cda80 _$E96
// FUNCTION: STBC 0x004cda90 _$E613
// FUNCTION: STBC 0x004cdaa0 _$E612
// FUNCTION: STBC 0x004cdab0 _$E616
// FUNCTION: STBC 0x004cdac0 _$E615
// FUNCTION: STBC 0x004cdad0 _$E56
// FUNCTION: STBC 0x004cdae0 _$E55
// FUNCTION: STBC 0x004cdaf0 _$E620
// FUNCTION: STBC 0x004cdb00 _$E619
// FUNCTION: STBC 0x004cdb10 _$E101
// FUNCTION: STBC 0x004cdb20 _$E100
// FUNCTION: STBC 0x004cdb30 _$E624
// FUNCTION: STBC 0x004cdb40 _$E623
// FUNCTION: STBC 0x004cdb50 _$E105
// FUNCTION: STBC 0x004cdb60 _$E104
// FUNCTION: STBC 0x004cdb70 _$E218
// FUNCTION: STBC 0x004cdb80 _$E217
// FUNCTION: STBC 0x004ce280 _$E160
// FUNCTION: STBC 0x004ce290 _$E159
// FUNCTION: STBC 0x004ce2a0 _$E173
// FUNCTION: STBC 0x004ce2b0 _$E172
// FUNCTION: STBC 0x004ce2c0 _$E225
// FUNCTION: STBC 0x004ce2d0 _$E224
// FUNCTION: STBC 0x004ce2e0 _$E268
// FUNCTION: STBC 0x004ce2f0 _$E267
// FUNCTION: STBC 0x004ce300 _$E235
// FUNCTION: STBC 0x004ce310 _$E234
// FUNCTION: STBC 0x004ce320 _$E272
// FUNCTION: STBC 0x004ce330 _$E271
// FUNCTION: STBC 0x004ce340 _$E279
// FUNCTION: STBC 0x004ce350 _$E278
// FUNCTION: STBC 0x004ce360 _$E301
// FUNCTION: STBC 0x004ce370 _$E300
// FUNCTION: STBC 0x004ce380 _$E305
// FUNCTION: STBC 0x004ce390 _$E304
// FUNCTION: STBC 0x004ce3a0 _$E122
// FUNCTION: STBC 0x004ce3b0 _$E121
// FUNCTION: STBC 0x004ce3c0 _$E287
// FUNCTION: STBC 0x004ce3d0 _$E286
// FUNCTION: STBC 0x004ce3e0 _$E294
// FUNCTION: STBC 0x004ce3f0 _$E293
// FUNCTION: STBC 0x004ce400 _$E313
// FUNCTION: STBC 0x004ce410 _$E312
// FUNCTION: STBC 0x004ce420 _$E317
// FUNCTION: STBC 0x004ce430 _$E316
// FUNCTION: STBC 0x004ce440 _$E357
// FUNCTION: STBC 0x004ce450 _$E356
// FUNCTION: STBC 0x004ce460 _$E363
// FUNCTION: STBC 0x004ce470 _$E362
// FUNCTION: STBC 0x004ce480 _$E229
// FUNCTION: STBC 0x004ce490 _$E228
// FUNCTION: STBC 0x004ce4a0 _$E309
// FUNCTION: STBC 0x004ce4b0 _$E308
// FUNCTION: STBC 0x004ce4c0 _$E321
// FUNCTION: STBC 0x004ce4d0 _$E320
// FUNCTION: STBC 0x004ce4e0 _$E325
// FUNCTION: STBC 0x004ce4f0 _$E324
// FUNCTION: STBC 0x004ce500 _$E329
// FUNCTION: STBC 0x004ce510 _$E328
// FUNCTION: STBC 0x004ce520 _$E333
// FUNCTION: STBC 0x004ce530 _$E332
// FUNCTION: STBC 0x004ce540 _$E337
// FUNCTION: STBC 0x004ce550 _$E336
// FUNCTION: STBC 0x004ce560 _$E341
// FUNCTION: STBC 0x004ce570 _$E340
// FUNCTION: STBC 0x004ce580 _$E345
// FUNCTION: STBC 0x004ce590 _$E344
// FUNCTION: STBC 0x004ce5a0 _$E349
// FUNCTION: STBC 0x004ce5b0 _$E348
// FUNCTION: STBC 0x004ce5c0 _$E127
// FUNCTION: STBC 0x004ce5d0 _$E126
// FUNCTION: STBC 0x004ce5e0 _$E353
// FUNCTION: STBC 0x004ce5f0 _$E352
// FUNCTION: STBC 0x004ce600 _$E132
// FUNCTION: STBC 0x004ce610 _$E131
// FUNCTION: STBC 0x004ce620 _$E367
// FUNCTION: STBC 0x004ce630 _$E366
// FUNCTION: STBC 0x004ce640 _$E371
// FUNCTION: STBC 0x004ce650 _$E370
// FUNCTION: STBC 0x004ce660 _$E375
// FUNCTION: STBC 0x004ce670 _$E374
// FUNCTION: STBC 0x004ce680 _$E379
// FUNCTION: STBC 0x004ce690 _$E378
// FUNCTION: STBC 0x004ce6a0 _$E137
// FUNCTION: STBC 0x004ce6b0 _$E136
// FUNCTION: STBC 0x004ce6c0 _$E383
// FUNCTION: STBC 0x004ce6d0 _$E382
// FUNCTION: STBC 0x004ce6e0 _$E387
// FUNCTION: STBC 0x004ce6f0 _$E386
// FUNCTION: STBC 0x004ce700 _$E391
// FUNCTION: STBC 0x004ce710 _$E390
// FUNCTION: STBC 0x004ce720 _$E143
// FUNCTION: STBC 0x004ce730 _$E142
// FUNCTION: STBC 0x004ce740 _$E148
// FUNCTION: STBC 0x004ce750 _$E147
// FUNCTION: STBC 0x004ce760 _$E178
// FUNCTION: STBC 0x004ce770 _$E177
// FUNCTION: STBC 0x004ce780 _$E395
// FUNCTION: STBC 0x004ce790 _$E394
// FUNCTION: STBC 0x004ce7a0 _$E183
// FUNCTION: STBC 0x004ce7b0 _$E182
// FUNCTION: STBC 0x004ce7c0 _$E399
// FUNCTION: STBC 0x004ce7d0 _$E398
// FUNCTION: STBC 0x004ce7e0 _$E403
// FUNCTION: STBC 0x004ce7f0 _$E402
// FUNCTION: STBC 0x004ce800 _$E188
// FUNCTION: STBC 0x004ce810 _$E187
// FUNCTION: STBC 0x004ce820 _$E407
// FUNCTION: STBC 0x004ce830 _$E406
// FUNCTION: STBC 0x004ce840 _$E153
// FUNCTION: STBC 0x004ce850 _$E152
// FUNCTION: STBC 0x004ce860 _$E411
// FUNCTION: STBC 0x004ce870 _$E410
// FUNCTION: STBC 0x004ce880 _$E415
// FUNCTION: STBC 0x004ce890 _$E414
// FUNCTION: STBC 0x004ce8a0 _$E193
// FUNCTION: STBC 0x004ce8b0 _$E192
// FUNCTION: STBC 0x004ce8c0 _$E198
// FUNCTION: STBC 0x004ce8d0 _$E197
// FUNCTION: STBC 0x004ce8e0 _$E419
// FUNCTION: STBC 0x004ce8f0 _$E418
// FUNCTION: STBC 0x004ce900 _$E423
// FUNCTION: STBC 0x004ce910 _$E422
// FUNCTION: STBC 0x004ce920 _$E427
// FUNCTION: STBC 0x004ce930 _$E426
// FUNCTION: STBC 0x004ce940 _$E431
// FUNCTION: STBC 0x004ce950 _$E430
// FUNCTION: STBC 0x004ce960 _$E435
// FUNCTION: STBC 0x004ce970 _$E434
// FUNCTION: STBC 0x004ce980 _$E439
// FUNCTION: STBC 0x004ce990 _$E438
// FUNCTION: STBC 0x004ce9a0 _$E443
// FUNCTION: STBC 0x004ce9b0 _$E442
// FUNCTION: STBC 0x004ce9c0 _$E447
// FUNCTION: STBC 0x004ce9d0 _$E446
// FUNCTION: STBC 0x004ce9e0 _$E203
// FUNCTION: STBC 0x004ce9f0 _$E202
// FUNCTION: STBC 0x004cea00 _$E451
// FUNCTION: STBC 0x004cea10 _$E450
// FUNCTION: STBC 0x004cea20 _$E208
// FUNCTION: STBC 0x004cea30 _$E207
// FUNCTION: STBC 0x004cea40 _$E213
// FUNCTION: STBC 0x004cea50 _$E212
// FUNCTION: STBC 0x004cea90 _$E628
// FUNCTION: STBC 0x004ceaa0 _$E627
// FUNCTION: STBC 0x004ceab0 _$E633
// FUNCTION: STBC 0x004ceac0 _$E632
// FUNCTION: STBC 0x004cead0 _$E637
// FUNCTION: STBC 0x004ceae0 _$E636

// The NiRTTI's dynamic initialiser, the registration of its exit-time destructor and the guarded destructor.
// FUNCTION: STBC 0x004cdb90 _$E643
// FUNCTION: STBC 0x004cdba0 _$E639
// FUNCTION: STBC 0x004cdbc0 _$E642
// FUNCTION: STBC 0x004cdbd0 _$E640
// DATA: STBC 0x00984db0 ?rtti@ExplodeParticleController@@2VNiRTTI@@B
const NiRTTI ExplodeParticleController::rtti( "ExplodeParticleController", &PointParticleController::rtti );

/*
================
ExplodeParticleController::FindController
================
*/
// FUNCTION: STBC 0x004cdbf0
ExplodeParticleController *ExplodeParticleController::FindController( NiObjectNET *target ) {
	for ( NiTimeController *controller = target->GetControllers(); controller; controller = controller->GetNext() ) {
		ExplodeParticleController *explodeController = NiDynamicCast( ExplodeParticleController, controller );
		if ( explodeController ) {
			return explodeController;
		}
	}
	return 0;
}

// The light pointer's initialiser, the registration of its exit-time destructor and the guarded destructor.
// FUNCTION: STBC 0x004cdc40 _$E649
// FUNCTION: STBC 0x004cdc50 _$E645
// FUNCTION: STBC 0x004cdc60 _$E648
// FUNCTION: STBC 0x004cdc70 _$E646
// DATA: STBC 0x984D88 ?light@ExplodeParticleController@@1V?$NiPointer@VNiLight@@@@A
NiPointer<NiLight> ExplodeParticleController::light;

/*
================
ExplodeParticleController::Create
================
*/
// FUNCTION: STBC 0x004cdcb0
ExplodeParticleController *ExplodeParticleController::Create( float lifeTime, float emitLife, float emitFrequency, float emitVelocity, float angleVariance, NiAVObject *emitFrom ) {
	return new ExplodeParticleController( lifeTime, emitLife, emitFrequency, emitVelocity, angleVariance, emitFrom );
}

/*
================
ExplodeParticleController::ExplodeParticleController

The emitter is built without an emit object.
================
*/
// FUNCTION: STBC 0x004cdd30
// BUG: emitFrom_ is dropped, as PointParticleController's constructor drops its own
ExplodeParticleController::ExplodeParticleController( float lifeTime_, float emitLife_, float emitFrequency_, float emitVelocity_, float angleVariance_, NiAVObject *emitFrom_ ) : PointParticleController( lifeTime_, emitLife_, emitFrequency_, emitVelocity_, angleVariance_, NULL ) {
	sceneRootName = NULL;
}

/*
================
ExplodeParticleController::~ExplodeParticleController

Takes the shared light out of the scene and releases it.
================
*/
// FUNCTION: STBC 0x004cddc0
ExplodeParticleController::~ExplodeParticleController() {
	RemoveAllIntensityKeys();
	if ( light ) {
		light->SetSwitch( false );
		if ( light->GetParent() ) {
			light->GetParent()->DetachChild( light );
		}
		light->DetachAllAffectedNodes();
		light = NULL;
	}
	if ( sceneRootName ) {
		delete [] sceneRootName;
	}
}

/*
================
ExplodeParticleController::CreateClone
================
*/
// FUNCTION: STBC 0x004cdef0
NiObject *ExplodeParticleController::CreateClone() {
	ExplodeParticleController *clone = new ExplodeParticleController( 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, NULL );
	CopyMembers( clone );
	return clone;
}

/*
================
ExplodeParticleController::CopyMembers
================
*/
// FUNCTION: STBC 0x004cdf70
void ExplodeParticleController::CopyMembers( ExplodeParticleController *dest ) {
	EmitterController::CopyMembers( dest );
	// BUG: frees this controller's set name instead of the copy's, so the name is never copied
	if ( sceneRootName ) {
		delete [] sceneRootName;
		sceneRootName = NULL;
	}
	if ( sceneRootName ) {
		dest->sceneRootName = new char[strlen( sceneRootName ) + 1];
		strcpy( dest->sceneRootName, sceneRootName );
	}
}

/*
================
ExplodeParticleController::Update

Dims the light by the intensity track while the effect runs.
================
*/
// FUNCTION: STBC 0x004cdff0
void ExplodeParticleController::Update( float time ) {
	// BUG: redundant: the stopped state passes the first test only to fail the second
	if ( ( GetField( STATE_MASK, STATE_POS ) == STATE_RUNNING || GetField( STATE_MASK, STATE_POS ) == STATE_STOPPED )
		 && GetField( STATE_MASK, STATE_POS ) != STATE_STOPPED ) {
		PointParticleController::Update( time );
		if ( light ) {
			GetIntensity( ( time - startTime ) / lifeTime, time );
			light->SetDimmer( time );
		}
	}
}

/*
================
ExplodeParticleController::AddDefaultKeys

An empty intensity track rises from 0 to 1 over the effect's life.
================
*/
// FUNCTION: STBC 0x004ce060
void ExplodeParticleController::AddDefaultKeys() {
	if ( intensityKeys.count == 0 ) {
		AddIntensityKey( 0.0f, 0.0f );
		AddIntensityKey( 1.0f, 1.0f );
	}
	EffectController::AddDefaultKeys();
}

/*
================
ExplodeParticleController::GetIntensity
================
*/
// FUNCTION: STBC 0x004ce0a0
void ExplodeParticleController::GetIntensity( float time, float &intensity ) {
	GetFloatValue( intensityKeys, time, intensity );
}

/*
================
ExplodeParticleController::AddIntensityKey
================
*/
// FUNCTION: STBC 0x004ce0c0
void ExplodeParticleController::AddIntensityKey( float time, float intensity ) {
	AddFloatKey( intensityKeys, time, intensity );
}

/*
================
ExplodeParticleController::RemoveIntensityKeyAt
================
*/
// FUNCTION: STBC 0x004ce0e0
void ExplodeParticleController::RemoveIntensityKeyAt( float time ) {
	RemoveFloatKey( intensityKeys, time );
}

/*
================
ExplodeParticleController::RemoveAllIntensityKeys
================
*/
// FUNCTION: STBC 0x004ce100
void ExplodeParticleController::RemoveAllIntensityKeys() {
	intensityKeys.RemoveAll();
}

/*
================
ExplodeParticleController::DetachEffect
================
*/
// FUNCTION: STBC 0x004ce110
void ExplodeParticleController::DetachEffect() {
	if ( light ) {
		light->DetachAllAffectedNodes();
		light->SetSwitch( false );
		light = NULL;
	}
	EmitterController::DetachEffect();
}

/*
================
ExplodeParticleController::AttachEffect

The light, if there is one, shines on the scene of the named set from the
emit position.
================
*/
// FUNCTION: STBC 0x004ce160
void ExplodeParticleController::AttachEffect( NiNode *parent ) {
	EmitterController::AttachEffect( parent );
	if ( sceneRootName && light ) {
		light->DetachAllAffectedNodes();
		SetClass *set = g_kSetManager.GetSet( sceneRootName );
		if ( set ) {
			light->AttachAffectedNode( set->GetSceneRoot() );
			set->SetSceneChanged( true );
		}
		light->SetTranslate( modelEmitPosition );
		light->Update( 0.0f );
	}
}

/*
================
ExplodeParticleController::CreateTarget
================
*/
// FUNCTION: STBC 0x004ce200
void ExplodeParticleController::CreateTarget( char *textureName ) {
	PointParticleController::CreateTarget( textureName );
}

/*
================
ExplodeParticleController::SetSceneRoot

Keeps a copy of the name of the set whose scene the light shines on.
================
*/
// FUNCTION: STBC 0x004ce210
// PARKED: register allocation: retail keeps this in EBX and the name in ESI, swapped here (comparison order and an early return tried)
void ExplodeParticleController::SetSceneRoot( char *setName ) {
	if ( setName != sceneRootName ) {
		if ( sceneRootName ) {
			delete [] sceneRootName;
			sceneRootName = NULL;
		}
		if ( setName ) {
			sceneRootName = new char[strlen( setName ) + 1];
			strcpy( sceneRootName, setName );
		}
	}
}
