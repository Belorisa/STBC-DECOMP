/*
===============================================================================

	BridgeEffectAction.cpp

===============================================================================
*/

#include <iostream>
#include <windows.h>

// <windows.h>'s ANSI/wide macro would rename TGObject's method.
#undef GetClassName

#include "BridgeEffectAction.h"
#include "BridgeObjectClass.h"
#include "DebrisParticleController.h"
#include "EffectEmitterProperty.h"
#include "ExplodeEmitterProperty.h"
#include "ExplodeParticleController.h"
#include "SetClass.h"
#include "SmokeEmitterProperty.h"
#include "SparkEmitterProperty.h"
#include "SparkParticleController.h"
#include "NiTriBasedGeomData.h"
#include "NiMain.h"
#include "NiAnimation.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004dd0f0 _$E650
// FUNCTION: STBC 0x004dd110 _$E649

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004dbfd0 _$E32
// FUNCTION: STBC 0x004dbfe0 _$E29
// FUNCTION: STBC 0x004dbff0 _$E31
// FUNCTION: STBC 0x004dc000 _$E30
// FUNCTION: STBC 0x004dc010 _$E38
// FUNCTION: STBC 0x004dc020 _$E35
// FUNCTION: STBC 0x004dc030 _$E37
// FUNCTION: STBC 0x004dc040 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004dbc30 _$E240
// FUNCTION: STBC 0x004dbc40 _$E239
// FUNCTION: STBC 0x004dbc50 _$E460
// FUNCTION: STBC 0x004dbc60 _$E459
// FUNCTION: STBC 0x004dbc70 _$E464
// FUNCTION: STBC 0x004dbc80 _$E463
// FUNCTION: STBC 0x004dbc90 _$E468
// FUNCTION: STBC 0x004dbca0 _$E467
// FUNCTION: STBC 0x004dbcb0 _$E491
// FUNCTION: STBC 0x004dbcc0 _$E490
// FUNCTION: STBC 0x004dbcd0 _$E482
// FUNCTION: STBC 0x004dbce0 _$E481
// FUNCTION: STBC 0x004dbcf0 _$E495
// FUNCTION: STBC 0x004dbd00 _$E494
// FUNCTION: STBC 0x004dbd10 _$E499
// FUNCTION: STBC 0x004dbd20 _$E498
// FUNCTION: STBC 0x004dbd30 _$E560
// FUNCTION: STBC 0x004dbd40 _$E559
// FUNCTION: STBC 0x004dbd50 _$E563
// FUNCTION: STBC 0x004dbd60 _$E562
// FUNCTION: STBC 0x004dbd70 _$E520
// FUNCTION: STBC 0x004dbd80 _$E519
// FUNCTION: STBC 0x004dbd90 _$E536
// FUNCTION: STBC 0x004dbda0 _$E535
// FUNCTION: STBC 0x004dbdb0 _$E503
// FUNCTION: STBC 0x004dbdc0 _$E502
// FUNCTION: STBC 0x004dbdd0 _$E543
// FUNCTION: STBC 0x004dbde0 _$E542
// FUNCTION: STBC 0x004dbdf0 _$E546
// FUNCTION: STBC 0x004dbe00 _$E545
// FUNCTION: STBC 0x004dbe10 _$E582
// FUNCTION: STBC 0x004dbe20 _$E581
// FUNCTION: STBC 0x004dbe30 _$E506
// FUNCTION: STBC 0x004dbe40 _$E505
// FUNCTION: STBC 0x004dbe50 _$E509
// FUNCTION: STBC 0x004dbe60 _$E508
// FUNCTION: STBC 0x004dbe70 _$E512
// FUNCTION: STBC 0x004dbe80 _$E511
// FUNCTION: STBC 0x004dbe90 _$E228
// FUNCTION: STBC 0x004dbea0 _$E227
// FUNCTION: STBC 0x004dbeb0 _$E516
// FUNCTION: STBC 0x004dbec0 _$E515
// FUNCTION: STBC 0x004dbed0 _$E549
// FUNCTION: STBC 0x004dbee0 _$E548
// FUNCTION: STBC 0x004dbef0 _$E524
// FUNCTION: STBC 0x004dbf00 _$E523
// FUNCTION: STBC 0x004dbf10 _$E528
// FUNCTION: STBC 0x004dbf20 _$E527
// FUNCTION: STBC 0x004dbf30 _$E532
// FUNCTION: STBC 0x004dbf40 _$E531
// FUNCTION: STBC 0x004dbf50 _$E590
// FUNCTION: STBC 0x004dbf60 _$E589
// FUNCTION: STBC 0x004dbf70 _$E615
// FUNCTION: STBC 0x004dbf80 _$E614
// FUNCTION: STBC 0x004dbf90 _$E603
// FUNCTION: STBC 0x004dbfa0 _$E602
// FUNCTION: STBC 0x004dbfb0 _$E540
// FUNCTION: STBC 0x004dbfc0 _$E539
// FUNCTION: STBC 0x004dc050 _$E553
// FUNCTION: STBC 0x004dc060 _$E552
// FUNCTION: STBC 0x004dc070 _$E557
// FUNCTION: STBC 0x004dc080 _$E556
// FUNCTION: STBC 0x004dc090 _$E231
// FUNCTION: STBC 0x004dc0a0 _$E230
// FUNCTION: STBC 0x004dc0b0 _$E243
// FUNCTION: STBC 0x004dc0c0 _$E242
// FUNCTION: STBC 0x004dc0d0 _$E567
// FUNCTION: STBC 0x004dc0e0 _$E566
// FUNCTION: STBC 0x004dc0f0 _$E575
// FUNCTION: STBC 0x004dc100 _$E574
// FUNCTION: STBC 0x004dc110 _$E52
// FUNCTION: STBC 0x004dc120 _$E51
// FUNCTION: STBC 0x004dc130 _$E292
// FUNCTION: STBC 0x004dc140 _$E291
// FUNCTION: STBC 0x004dc150 _$E579
// FUNCTION: STBC 0x004dc160 _$E578
// FUNCTION: STBC 0x004dc170 _$E586
// FUNCTION: STBC 0x004dc180 _$E585
// FUNCTION: STBC 0x004dc190 _$E571
// FUNCTION: STBC 0x004dc1a0 _$E570
// FUNCTION: STBC 0x004dc1b0 _$E599
// FUNCTION: STBC 0x004dc1c0 _$E598
// FUNCTION: STBC 0x004dc1d0 _$E595
// FUNCTION: STBC 0x004dc1e0 _$E594
// FUNCTION: STBC 0x004dc1f0 _$E63
// FUNCTION: STBC 0x004dc200 _$E62
// FUNCTION: STBC 0x004dc210 _$E67
// FUNCTION: STBC 0x004dc220 _$E66
// FUNCTION: STBC 0x004dc230 _$E71
// FUNCTION: STBC 0x004dc240 _$E70
// FUNCTION: STBC 0x004dc250 _$E607
// FUNCTION: STBC 0x004dc260 _$E606
// FUNCTION: STBC 0x004dc270 _$E611
// FUNCTION: STBC 0x004dc280 _$E610
// FUNCTION: STBC 0x004dc290 _$E89
// FUNCTION: STBC 0x004dc2a0 _$E88
// FUNCTION: STBC 0x004dc2b0 _$E618
// FUNCTION: STBC 0x004dc2c0 _$E617
// FUNCTION: STBC 0x004dc2d0 _$E621
// FUNCTION: STBC 0x004dc2e0 _$E620
// FUNCTION: STBC 0x004dc2f0 _$E206
// FUNCTION: STBC 0x004dc300 _$E205
// FUNCTION: STBC 0x004dc310 _$E625
// FUNCTION: STBC 0x004dc320 _$E624
// FUNCTION: STBC 0x004dc330 _$E629
// FUNCTION: STBC 0x004dc340 _$E628
// FUNCTION: STBC 0x004dc350 _$E148
// FUNCTION: STBC 0x004dc360 _$E147
// FUNCTION: STBC 0x004dc370 _$E161
// FUNCTION: STBC 0x004dc380 _$E160
// FUNCTION: STBC 0x004dc390 _$E211
// FUNCTION: STBC 0x004dc3a0 _$E210
// FUNCTION: STBC 0x004dc3b0 _$E297
// FUNCTION: STBC 0x004dc3c0 _$E296
// FUNCTION: STBC 0x004dc3d0 _$E215
// FUNCTION: STBC 0x004dc3e0 _$E214
// FUNCTION: STBC 0x004dc3f0 _$E301
// FUNCTION: STBC 0x004dc400 _$E300
// FUNCTION: STBC 0x004dc410 _$E308
// FUNCTION: STBC 0x004dc420 _$E307
// FUNCTION: STBC 0x004dc430 _$E318
// FUNCTION: STBC 0x004dc440 _$E317
// FUNCTION: STBC 0x004dc450 _$E322
// FUNCTION: STBC 0x004dc460 _$E321
// FUNCTION: STBC 0x004dc470 _$E106
// FUNCTION: STBC 0x004dc480 _$E105
// FUNCTION: STBC 0x004dc490 _$E268
// FUNCTION: STBC 0x004dc4a0 _$E267
// FUNCTION: STBC 0x004dc4b0 _$E275
// FUNCTION: STBC 0x004dc4c0 _$E274
// FUNCTION: STBC 0x004dc9d0 _$E288
// FUNCTION: STBC 0x004dc9e0 _$E287
// FUNCTION: STBC 0x004dcab0 _$E330
// FUNCTION: STBC 0x004dcac0 _$E329
// FUNCTION: STBC 0x004dcad0 _$E370
// FUNCTION: STBC 0x004dcae0 _$E369
// FUNCTION: STBC 0x004dcaf0 _$E376
// FUNCTION: STBC 0x004dcb00 _$E375
// FUNCTION: STBC 0x004dcb10 _$E253
// FUNCTION: STBC 0x004dcb20 _$E252
// FUNCTION: STBC 0x004dcb30 _$E326
// FUNCTION: STBC 0x004dcb40 _$E325
// FUNCTION: STBC 0x004dcb50 _$E334
// FUNCTION: STBC 0x004dcb60 _$E333
// FUNCTION: STBC 0x004dcb70 _$E338
// FUNCTION: STBC 0x004dcb80 _$E337
// FUNCTION: STBC 0x004dcb90 _$E342
// FUNCTION: STBC 0x004dcba0 _$E341
// FUNCTION: STBC 0x004dcbb0 _$E346
// FUNCTION: STBC 0x004dcbc0 _$E345
// FUNCTION: STBC 0x004dcbd0 _$E350
// FUNCTION: STBC 0x004dcbe0 _$E349
// FUNCTION: STBC 0x004dcbf0 _$E354
// FUNCTION: STBC 0x004dcc00 _$E353
// FUNCTION: STBC 0x004dcc10 _$E358
// FUNCTION: STBC 0x004dcc20 _$E357
// FUNCTION: STBC 0x004dcc30 _$E362
// FUNCTION: STBC 0x004dcc40 _$E361
// FUNCTION: STBC 0x004dcc50 _$E111
// FUNCTION: STBC 0x004dcc60 _$E110
// FUNCTION: STBC 0x004dcc70 _$E366
// FUNCTION: STBC 0x004dcc80 _$E365
// FUNCTION: STBC 0x004dcc90 _$E116
// FUNCTION: STBC 0x004dcca0 _$E115
// FUNCTION: STBC 0x004dccb0 _$E380
// FUNCTION: STBC 0x004dccc0 _$E379
// FUNCTION: STBC 0x004dccd0 _$E384
// FUNCTION: STBC 0x004dcce0 _$E383
// FUNCTION: STBC 0x004dccf0 _$E388
// FUNCTION: STBC 0x004dcd00 _$E387
// FUNCTION: STBC 0x004dcd10 _$E392
// FUNCTION: STBC 0x004dcd20 _$E391
// FUNCTION: STBC 0x004dcd30 _$E125
// FUNCTION: STBC 0x004dcd40 _$E124
// FUNCTION: STBC 0x004dcd50 _$E279
// FUNCTION: STBC 0x004dcd60 _$E278
// FUNCTION: STBC 0x004dcd70 _$E283
// FUNCTION: STBC 0x004dcd80 _$E282
// FUNCTION: STBC 0x004dcd90 _$E396
// FUNCTION: STBC 0x004dcda0 _$E395
// FUNCTION: STBC 0x004dcdb0 _$E131
// FUNCTION: STBC 0x004dcdc0 _$E130
// FUNCTION: STBC 0x004dcdd0 _$E136
// FUNCTION: STBC 0x004dcde0 _$E135
// FUNCTION: STBC 0x004dcdf0 _$E166
// FUNCTION: STBC 0x004dce00 _$E165
// FUNCTION: STBC 0x004dce10 _$E400
// FUNCTION: STBC 0x004dce20 _$E399
// FUNCTION: STBC 0x004dce30 _$E171
// FUNCTION: STBC 0x004dce40 _$E170
// FUNCTION: STBC 0x004dce50 _$E404
// FUNCTION: STBC 0x004dce60 _$E403
// FUNCTION: STBC 0x004dce70 _$E408
// FUNCTION: STBC 0x004dce80 _$E407
// FUNCTION: STBC 0x004dce90 _$E176
// FUNCTION: STBC 0x004dcea0 _$E175
// FUNCTION: STBC 0x004dceb0 _$E412
// FUNCTION: STBC 0x004dcec0 _$E411
// FUNCTION: STBC 0x004dced0 _$E141
// FUNCTION: STBC 0x004dcee0 _$E140
// FUNCTION: STBC 0x004dcef0 _$E416
// FUNCTION: STBC 0x004dcf00 _$E415
// FUNCTION: STBC 0x004dcf10 _$E420
// FUNCTION: STBC 0x004dcf20 _$E419
// FUNCTION: STBC 0x004dcf30 _$E181
// FUNCTION: STBC 0x004dcf40 _$E180
// FUNCTION: STBC 0x004dcf50 _$E186
// FUNCTION: STBC 0x004dcf60 _$E185
// FUNCTION: STBC 0x004dcf70 _$E424
// FUNCTION: STBC 0x004dcf80 _$E423
// FUNCTION: STBC 0x004dcf90 _$E428
// FUNCTION: STBC 0x004dcfa0 _$E427
// FUNCTION: STBC 0x004dcfb0 _$E432
// FUNCTION: STBC 0x004dcfc0 _$E431
// FUNCTION: STBC 0x004dcfd0 _$E436
// FUNCTION: STBC 0x004dcfe0 _$E435
// FUNCTION: STBC 0x004dcff0 _$E440
// FUNCTION: STBC 0x004dd000 _$E439
// FUNCTION: STBC 0x004dd010 _$E444
// FUNCTION: STBC 0x004dd020 _$E443
// FUNCTION: STBC 0x004dd030 _$E448
// FUNCTION: STBC 0x004dd040 _$E447
// FUNCTION: STBC 0x004dd050 _$E452
// FUNCTION: STBC 0x004dd060 _$E451
// FUNCTION: STBC 0x004dd070 _$E191
// FUNCTION: STBC 0x004dd080 _$E190
// FUNCTION: STBC 0x004dd090 _$E456
// FUNCTION: STBC 0x004dd0a0 _$E455
// FUNCTION: STBC 0x004dd0b0 _$E196
// FUNCTION: STBC 0x004dd0c0 _$E195
// FUNCTION: STBC 0x004dd0d0 _$E201
// FUNCTION: STBC 0x004dd0e0 _$E200
// FUNCTION: STBC 0x004dd120 _$E633
// FUNCTION: STBC 0x004dd130 _$E632
// FUNCTION: STBC 0x004dd140 _$E638
// FUNCTION: STBC 0x004dd150 _$E637
// FUNCTION: STBC 0x004dd160 _$E642
// FUNCTION: STBC 0x004dd170 _$E641

/*
================
BridgeEffectAction::Create
================
*/
// FUNCTION: STBC 0x004dc4d0
BridgeEffectAction *BridgeEffectAction::Create( float lifeTime, float emitLife, float emitFrequency, BridgeObjectClass *bridgeObject, char *hardpointName, char *textureName ) {
	return new BridgeEffectAction( EFFECT_POINT, lifeTime, emitLife, emitFrequency, bridgeObject, hardpointName, textureName );
}

/*
================
BridgeEffectAction::CreateSmoke
================
*/
// FUNCTION: STBC 0x004dc520
BridgeEffectAction *BridgeEffectAction::CreateSmoke( float lifeTime, float emitLife, float emitFrequency, BridgeObjectClass *bridgeObject, char *hardpointName, char *textureName ) {
	return new BridgeEffectAction( EFFECT_SMOKE, lifeTime, emitLife, emitFrequency, bridgeObject, hardpointName, textureName );
}

/*
================
BridgeEffectAction::CreateSparks
================
*/
// FUNCTION: STBC 0x004dc570
BridgeEffectAction *BridgeEffectAction::CreateSparks( float lifeTime, float emitLife, float emitFrequency, BridgeObjectClass *bridgeObject, char *hardpointName, char *textureName ) {
	return new BridgeEffectAction( EFFECT_SPARKS, lifeTime, emitLife, emitFrequency, bridgeObject, hardpointName, textureName );
}

/*
================
BridgeEffectAction::CreateExplosion
================
*/
// FUNCTION: STBC 0x004dc5c0
BridgeEffectAction *BridgeEffectAction::CreateExplosion( float lifeTime, float emitLife, float emitFrequency, BridgeObjectClass *bridgeObject, char *hardpointName, char *textureName ) {
	return new BridgeEffectAction( EFFECT_EXPLOSION, lifeTime, emitLife, emitFrequency, bridgeObject, hardpointName, textureName );
}

/*
================
BridgeEffectAction::CreateDebris
================
*/
// FUNCTION: STBC 0x004dc610
BridgeEffectAction *BridgeEffectAction::CreateDebris( float lifeTime, float emitLife, float emitFrequency, BridgeObjectClass *bridgeObject, char *hardpointName, char *textureName ) {
	return new BridgeEffectAction( EFFECT_DEBRIS, lifeTime, emitLife, emitFrequency, bridgeObject, hardpointName, textureName );
}

/*
================
BridgeEffectAction::BridgeEffectAction

Builds the controller of the effect type and attaches it at the bridge
object's emitter hardpoint. Without a bridge object or a texture the
action has no controller.
================
*/
// FUNCTION: STBC 0x004dc660
BridgeEffectAction::BridgeEffectAction( int type, float lifeTime, float emitLife, float emitFrequency, BridgeObjectClass *bridgeObject, char *hardpointName_, char *textureName ) : EffectAction( NULL ), hardpointName( NULL ) {
	if ( bridgeObject && textureName ) {
		switch ( type ) {
		case EFFECT_POINT:
			controller = PointParticleController::Create( lifeTime, emitLife, emitFrequency );
			hardpointName = bridgeObject->CreateEffect( NiDynamicCast( PointParticleController, controller ), EFFECTEMITTERPROPERTY_TYPE, textureName, hardpointName_ );
			break;
		case EFFECT_SMOKE:
			controller = PointParticleController::Create( lifeTime, emitLife, emitFrequency );
			hardpointName = bridgeObject->CreateEffect( NiDynamicCast( PointParticleController, controller ), SMOKEEMITTERPROPERTY_TYPE, textureName, hardpointName_ );
			break;
		case EFFECT_SPARKS:
			controller = SparkParticleController::Create( lifeTime, emitLife, emitFrequency );
			hardpointName = bridgeObject->CreateEffect( NiDynamicCast( PointParticleController, controller ), SPARKEMITTERPROPERTY_TYPE, textureName, hardpointName_ );
			break;
		case EFFECT_EXPLOSION: {
			controller = ExplodeParticleController::Create( lifeTime, emitLife, emitFrequency );
			SetClass *set = bridgeObject->GetContainingSet();
			NiDynamicCast( ExplodeParticleController, controller )->SetSceneRoot( set->GetName() );
			hardpointName = bridgeObject->CreateEffect( NiDynamicCast( PointParticleController, controller ), EXPLODEEMITTERPROPERTY_TYPE, textureName, hardpointName_ );
			break;
		}
		case EFFECT_DEBRIS:
			controller = DebrisParticleController::Create( lifeTime, emitLife, emitFrequency );
			hardpointName = bridgeObject->CreateEffect( NiDynamicCast( PointParticleController, controller ), EXPLODEEMITTERPROPERTY_TYPE, textureName, hardpointName_ );
			break;
		}
	}
}

/*
================
BridgeEffectAction::~BridgeEffectAction
================
*/
// FUNCTION: STBC 0x004dc9b0
// FUNCTION: STBC 0x004dc980 ??_GBridgeEffectAction@@UAEPAXI@Z
BridgeEffectAction::~BridgeEffectAction( void ) {
	delete [] hardpointName;
}

/*
================
BridgeEffectAction::GetController
================
*/
// FUNCTION: STBC 0x004dc9f0
EmitterController *BridgeEffectAction::GetController( void ) {
	return NiDynamicCast( EmitterController, controller );
}

/*
================
BridgeEffectAction::GetSparkController
================
*/
// FUNCTION: STBC 0x004dca30
SparkParticleController *BridgeEffectAction::GetSparkController( void ) {
	return NiDynamicCast( SparkParticleController, controller );
}

/*
================
BridgeEffectAction::GetExplosionController
================
*/
// FUNCTION: STBC 0x004dca70
ExplodeParticleController *BridgeEffectAction::GetExplosionController( void ) {
	return NiDynamicCast( ExplodeParticleController, controller );
}
