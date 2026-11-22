/*
===============================================================================

	EffectController.cpp

===============================================================================
*/

#include <iostream>
#include "EffectController.h"
#include "NiAlphaProperty.h"
#include "NiMath.h"
#include "NiNode.h"
#include "UtopiaApp.h"
#include "NiMain.h"
#include "NiAnimation.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004cb350 _$E638
// FUNCTION: STBC 0x004cb370 _$E637

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004c9870 _$E32
// FUNCTION: STBC 0x004c9880 _$E29
// FUNCTION: STBC 0x004c9890 _$E31
// FUNCTION: STBC 0x004c98a0 _$E30
// FUNCTION: STBC 0x004c98b0 _$E38
// FUNCTION: STBC 0x004c98c0 _$E35
// FUNCTION: STBC 0x004c98d0 _$E37
// FUNCTION: STBC 0x004c98e0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004c9490 _$E80
// FUNCTION: STBC 0x004c94a0 _$E79
// FUNCTION: STBC 0x004c94b0 _$E435
// FUNCTION: STBC 0x004c94c0 _$E434
// FUNCTION: STBC 0x004c94d0 _$E439
// FUNCTION: STBC 0x004c94e0 _$E438
// FUNCTION: STBC 0x004c94f0 _$E443
// FUNCTION: STBC 0x004c9500 _$E442
// FUNCTION: STBC 0x004c9510 _$E466
// FUNCTION: STBC 0x004c9520 _$E465
// FUNCTION: STBC 0x004c9530 _$E457
// FUNCTION: STBC 0x004c9540 _$E456
// FUNCTION: STBC 0x004c9550 _$E470
// FUNCTION: STBC 0x004c9560 _$E469
// FUNCTION: STBC 0x004c9570 _$E474
// FUNCTION: STBC 0x004c9580 _$E473
// FUNCTION: STBC 0x004c9590 _$E535
// FUNCTION: STBC 0x004c95a0 _$E534
// FUNCTION: STBC 0x004c95b0 _$E538
// FUNCTION: STBC 0x004c95c0 _$E537
// FUNCTION: STBC 0x004c95d0 _$E495
// FUNCTION: STBC 0x004c95e0 _$E494
// FUNCTION: STBC 0x004c95f0 _$E511
// FUNCTION: STBC 0x004c9600 _$E510
// FUNCTION: STBC 0x004c9610 _$E478
// FUNCTION: STBC 0x004c9620 _$E477
// FUNCTION: STBC 0x004c9630 _$E518
// FUNCTION: STBC 0x004c9640 _$E517
// FUNCTION: STBC 0x004c9650 _$E521
// FUNCTION: STBC 0x004c9660 _$E520
// FUNCTION: STBC 0x004c9670 _$E557
// FUNCTION: STBC 0x004c9680 _$E556
// FUNCTION: STBC 0x004c9690 _$E481
// FUNCTION: STBC 0x004c96a0 _$E480
// FUNCTION: STBC 0x004c96b0 _$E484
// FUNCTION: STBC 0x004c96c0 _$E483
// FUNCTION: STBC 0x004c96d0 _$E487
// FUNCTION: STBC 0x004c96e0 _$E486
// FUNCTION: STBC 0x004c96f0 _$E68
// FUNCTION: STBC 0x004c9700 _$E67
// FUNCTION: STBC 0x004c9710 _$E491
// FUNCTION: STBC 0x004c9720 _$E490
// FUNCTION: STBC 0x004c9730 _$E524
// FUNCTION: STBC 0x004c9740 _$E523
// FUNCTION: STBC 0x004c9750 _$E499
// FUNCTION: STBC 0x004c9760 _$E498
// FUNCTION: STBC 0x004c9770 _$E503
// FUNCTION: STBC 0x004c9780 _$E502
// FUNCTION: STBC 0x004c9790 _$E507
// FUNCTION: STBC 0x004c97a0 _$E506
// FUNCTION: STBC 0x004c97b0 _$E565
// FUNCTION: STBC 0x004c97c0 _$E564
// FUNCTION: STBC 0x004c97d0 _$E590
// FUNCTION: STBC 0x004c97e0 _$E589
// FUNCTION: STBC 0x004c97f0 _$E578
// FUNCTION: STBC 0x004c9800 _$E577
// FUNCTION: STBC 0x004c9810 _$E515
// FUNCTION: STBC 0x004c9820 _$E514
// FUNCTION: STBC 0x004c9830 _$E528
// FUNCTION: STBC 0x004c9840 _$E527
// FUNCTION: STBC 0x004c9850 _$E532
// FUNCTION: STBC 0x004c9860 _$E531
// FUNCTION: STBC 0x004c98f0 _$E71
// FUNCTION: STBC 0x004c9900 _$E70
// FUNCTION: STBC 0x004c9910 _$E83
// FUNCTION: STBC 0x004c9920 _$E82
// FUNCTION: STBC 0x004c9930 _$E542
// FUNCTION: STBC 0x004c9940 _$E541
// FUNCTION: STBC 0x004c9950 _$E550
// FUNCTION: STBC 0x004c9960 _$E549
// FUNCTION: STBC 0x004c9970 _$E554
// FUNCTION: STBC 0x004c9980 _$E553
// FUNCTION: STBC 0x004c9990 _$E561
// FUNCTION: STBC 0x004c99a0 _$E560
// FUNCTION: STBC 0x004c99b0 _$E45
// FUNCTION: STBC 0x004c99c0 _$E44
// FUNCTION: STBC 0x004c99d0 _$E546
// FUNCTION: STBC 0x004c99e0 _$E545
// FUNCTION: STBC 0x004c99f0 _$E231
// FUNCTION: STBC 0x004c9a00 _$E230
// FUNCTION: STBC 0x004c9a10 _$E574
// FUNCTION: STBC 0x004c9a20 _$E573
// FUNCTION: STBC 0x004c9a30 _$E570
// FUNCTION: STBC 0x004c9a40 _$E569
// FUNCTION: STBC 0x004c9a50 _$E582
// FUNCTION: STBC 0x004c9a60 _$E581
// FUNCTION: STBC 0x004c9a70 _$E90
// FUNCTION: STBC 0x004c9a80 _$E89
// FUNCTION: STBC 0x004c9a90 _$E586
// FUNCTION: STBC 0x004c9aa0 _$E585
// FUNCTION: STBC 0x004c9ab0 _$E593
// FUNCTION: STBC 0x004c9ac0 _$E592
// FUNCTION: STBC 0x004c9ad0 _$E56
// FUNCTION: STBC 0x004c9ae0 _$E55
// FUNCTION: STBC 0x004c9af0 _$E596
// FUNCTION: STBC 0x004c9b00 _$E595
// FUNCTION: STBC 0x004c9b10 _$E600
// FUNCTION: STBC 0x004c9b20 _$E599
// FUNCTION: STBC 0x004c9b30 _$E94
// FUNCTION: STBC 0x004c9b40 _$E93
// FUNCTION: STBC 0x004c9b50 _$E604
// FUNCTION: STBC 0x004c9b60 _$E603
// FUNCTION: STBC 0x004c9b70 _$E98
// FUNCTION: STBC 0x004c9b80 _$E97
// FUNCTION: STBC 0x004c9ec0 _$E218
// FUNCTION: STBC 0x004c9ed0 _$E217
// FUNCTION: STBC 0x004ca8c0 _$E160
// FUNCTION: STBC 0x004ca8d0 _$E159
// FUNCTION: STBC 0x004ca8e0 _$E173
// FUNCTION: STBC 0x004ca8f0 _$E172
// FUNCTION: STBC 0x004ca960 _$E223
// FUNCTION: STBC 0x004ca970 _$E222
// FUNCTION: STBC 0x004caae0 _$E236
// FUNCTION: STBC 0x004caaf0 _$E235
// FUNCTION: STBC 0x004cab00 _$E227
// FUNCTION: STBC 0x004cab10 _$E226
// FUNCTION: STBC 0x004cab20 _$E240
// FUNCTION: STBC 0x004cab30 _$E239
// FUNCTION: STBC 0x004cab40 _$E247
// FUNCTION: STBC 0x004cab50 _$E246
// FUNCTION: STBC 0x004cab60 _$E277
// FUNCTION: STBC 0x004cab70 _$E276
// FUNCTION: STBC 0x004cab80 _$E281
// FUNCTION: STBC 0x004cab90 _$E280
// FUNCTION: STBC 0x004caba0 _$E103
// FUNCTION: STBC 0x004cabb0 _$E102
// FUNCTION: STBC 0x004cabc0 _$E263
// FUNCTION: STBC 0x004cabd0 _$E262
// FUNCTION: STBC 0x004cabe0 _$E270
// FUNCTION: STBC 0x004cabf0 _$E269
// FUNCTION: STBC 0x004cac00 _$E293
// FUNCTION: STBC 0x004cac10 _$E292
// FUNCTION: STBC 0x004cac20 _$E297
// FUNCTION: STBC 0x004cac30 _$E296
// FUNCTION: STBC 0x004cac40 _$E337
// FUNCTION: STBC 0x004cac50 _$E336
// FUNCTION: STBC 0x004cac60 _$E343
// FUNCTION: STBC 0x004cac70 _$E342
// FUNCTION: STBC 0x004cac80 _$E285
// FUNCTION: STBC 0x004cac90 _$E284
// FUNCTION: STBC 0x004caca0 _$E289
// FUNCTION: STBC 0x004cacb0 _$E288
// FUNCTION: STBC 0x004cacc0 _$E301
// FUNCTION: STBC 0x004cacd0 _$E300
// FUNCTION: STBC 0x004cace0 _$E305
// FUNCTION: STBC 0x004cacf0 _$E304
// FUNCTION: STBC 0x004cad00 _$E309
// FUNCTION: STBC 0x004cad10 _$E308
// FUNCTION: STBC 0x004cad20 _$E313
// FUNCTION: STBC 0x004cad30 _$E312
// FUNCTION: STBC 0x004cad40 _$E317
// FUNCTION: STBC 0x004cad50 _$E316
// FUNCTION: STBC 0x004cad60 _$E321
// FUNCTION: STBC 0x004cad70 _$E320
// FUNCTION: STBC 0x004cad80 _$E325
// FUNCTION: STBC 0x004cad90 _$E324
// FUNCTION: STBC 0x004cada0 _$E329
// FUNCTION: STBC 0x004cadb0 _$E328
// FUNCTION: STBC 0x004cadc0 _$E127
// FUNCTION: STBC 0x004cadd0 _$E126
// FUNCTION: STBC 0x004cade0 _$E333
// FUNCTION: STBC 0x004cadf0 _$E332
// FUNCTION: STBC 0x004cae00 _$E132
// FUNCTION: STBC 0x004cae10 _$E131
// FUNCTION: STBC 0x004cae20 _$E347
// FUNCTION: STBC 0x004cae30 _$E346
// FUNCTION: STBC 0x004cae40 _$E351
// FUNCTION: STBC 0x004cae50 _$E350
// FUNCTION: STBC 0x004cae60 _$E355
// FUNCTION: STBC 0x004cae70 _$E354
// FUNCTION: STBC 0x004cae80 _$E359
// FUNCTION: STBC 0x004cae90 _$E358
// FUNCTION: STBC 0x004caea0 _$E137
// FUNCTION: STBC 0x004caeb0 _$E136
// FUNCTION: STBC 0x004caec0 _$E363
// FUNCTION: STBC 0x004caed0 _$E362
// FUNCTION: STBC 0x004caee0 _$E367
// FUNCTION: STBC 0x004caef0 _$E366
// FUNCTION: STBC 0x004caf00 _$E371
// FUNCTION: STBC 0x004caf10 _$E370
// FUNCTION: STBC 0x004caf20 _$E143
// FUNCTION: STBC 0x004caf30 _$E142
// FUNCTION: STBC 0x004caf40 _$E148
// FUNCTION: STBC 0x004caf50 _$E147
// FUNCTION: STBC 0x004caf60 _$E178
// FUNCTION: STBC 0x004caf70 _$E177
// FUNCTION: STBC 0x004caf80 _$E375
// FUNCTION: STBC 0x004caf90 _$E374
// FUNCTION: STBC 0x004cafa0 _$E183
// FUNCTION: STBC 0x004cafb0 _$E182
// FUNCTION: STBC 0x004cafc0 _$E379
// FUNCTION: STBC 0x004cafd0 _$E378
// FUNCTION: STBC 0x004cafe0 _$E383
// FUNCTION: STBC 0x004caff0 _$E382
// FUNCTION: STBC 0x004cb000 _$E188
// FUNCTION: STBC 0x004cb010 _$E187
// FUNCTION: STBC 0x004cb020 _$E387
// FUNCTION: STBC 0x004cb030 _$E386
// FUNCTION: STBC 0x004cb040 _$E153
// FUNCTION: STBC 0x004cb050 _$E152
// FUNCTION: STBC 0x004cb060 _$E391
// FUNCTION: STBC 0x004cb070 _$E390
// FUNCTION: STBC 0x004cb080 _$E395
// FUNCTION: STBC 0x004cb090 _$E394
// FUNCTION: STBC 0x004cb0a0 _$E193
// FUNCTION: STBC 0x004cb0b0 _$E192
// FUNCTION: STBC 0x004cb0c0 _$E198
// FUNCTION: STBC 0x004cb0d0 _$E197
// FUNCTION: STBC 0x004cb0e0 _$E399
// FUNCTION: STBC 0x004cb0f0 _$E398
// FUNCTION: STBC 0x004cb100 _$E403
// FUNCTION: STBC 0x004cb110 _$E402
// FUNCTION: STBC 0x004cb120 _$E407
// FUNCTION: STBC 0x004cb130 _$E406
// FUNCTION: STBC 0x004cb140 _$E411
// FUNCTION: STBC 0x004cb150 _$E410
// FUNCTION: STBC 0x004cb160 _$E415
// FUNCTION: STBC 0x004cb170 _$E414
// FUNCTION: STBC 0x004cb180 _$E419
// FUNCTION: STBC 0x004cb190 _$E418
// FUNCTION: STBC 0x004cb1a0 _$E423
// FUNCTION: STBC 0x004cb1b0 _$E422
// FUNCTION: STBC 0x004cb1c0 _$E427
// FUNCTION: STBC 0x004cb1d0 _$E426
// FUNCTION: STBC 0x004cb1e0 _$E203
// FUNCTION: STBC 0x004cb1f0 _$E202
// FUNCTION: STBC 0x004cb200 _$E431
// FUNCTION: STBC 0x004cb210 _$E430
// FUNCTION: STBC 0x004cb220 _$E208
// FUNCTION: STBC 0x004cb230 _$E207
// FUNCTION: STBC 0x004cb240 _$E213
// FUNCTION: STBC 0x004cb250 _$E212
// FUNCTION: STBC 0x004cb380 _$E608
// FUNCTION: STBC 0x004cb390 _$E607
// FUNCTION: STBC 0x004cb3a0 _$E613
// FUNCTION: STBC 0x004cb3b0 _$E612
// FUNCTION: STBC 0x004cb3c0 _$E617
// FUNCTION: STBC 0x004cb3d0 _$E616

// The NiRTTI's dynamic initialiser, the registration of its exit-time destructor and the guarded destructor.
// FUNCTION: STBC 0x004c9b90 _$E623
// FUNCTION: STBC 0x004c9ba0 _$E619
// FUNCTION: STBC 0x004c9bc0 _$E622
// FUNCTION: STBC 0x004c9bd0 _$E620
// DATA: STBC 0x984A18 ?rtti@EffectControllerData@@2VNiRTTI@@B
const NiRTTI EffectControllerData::rtti( "EffectControllerData", &NiObject::rtti );

// Its initialisers, as EffectControllerData's (the second bit of the same guard byte).
// FUNCTION: STBC 0x004c9bf0 _$E628
// FUNCTION: STBC 0x004c9c00 _$E625
// FUNCTION: STBC 0x004c9c20 _$E627
// FUNCTION: STBC 0x004c9c30 _$E626
// DATA: STBC 0x9849D8 ?rtti@EffectController@@2VNiRTTI@@B
const NiRTTI EffectController::rtti( "EffectController", &NiTimeController::rtti );

// DATA: STBC 0x8E0D14 ?effectLevel@EffectController@@1HA
int EffectController::effectLevel = EffectController::HIGH;

/*
================
EffectController::FindController
================
*/
// FUNCTION: STBC 0x4C9C50
EffectController *EffectController::FindController( NiObjectNET *target ) {
	for ( NiTimeController *controller = target->GetControllers(); controller; controller = controller->GetNext() ) {
		EffectController *effectController = NiDynamicCast( EffectController, controller );
		if ( effectController ) {
			return effectController;
		}
	}
	return 0;
}

/*
================
EffectController::EffectController

A controller with keys of its own.
================
*/
// FUNCTION: STBC 0x4C9CA0 ??0EffectController@@QAE@M@Z
EffectController::EffectController( float lifeTime_ ) : lifeTime( lifeTime_ ), data( new EffectControllerData ) {
	SetField( STATE_NEW, STATE_MASK, STATE_POS );
}

/*
================
EffectController::EffectController

A controller sharing another's keys, with no end.
================
*/
// FUNCTION: STBC 0x4C9EE0 ??0EffectController@@QAE@PAVEffectControllerData@@@Z
EffectController::EffectController( EffectControllerData *data_ ) : lifeTime( NI_INFINITY ), data( data_ ) {
	SetField( STATE_NEW, STATE_MASK, STATE_POS );
}

/*
================
EffectController::~EffectController
================
*/
// FUNCTION: STBC 0x4C9F10
EffectController::~EffectController() {
}

/*
================
EffectController::CreateClone

An effect controller is not cloned.
================
*/
// FUNCTION: STBC 0x4C9F80
NiObject *EffectController::CreateClone() {
	return NULL;
}

/*
================
EffectController::CopyMembers
================
*/
// FUNCTION: STBC 0x4C9F90
void EffectController::CopyMembers( EffectController *dest ) {
	NiTimeController::CopyMembers( dest );
	dest->lifeTime = lifeTime;
	dest->data = data;
}

/*
================
EffectController::GetTargetCast
================
*/
// FUNCTION: STBC 0x4C9FE0
NiAVObject *EffectController::GetTargetCast() {
	return NiDynamicCast( NiAVObject, target );
}

/*
================
EffectController::SetTargetAlphaBlendModes
================
*/
// FUNCTION: STBC 0x4CA020
void EffectController::SetTargetAlphaBlendModes( int srcMode, int destMode ) {
	NiAVObject *effect = GetTargetCast();
	if ( effect ) {
		NiAlphaProperty *alpha = NiDynamicCast( NiAlphaProperty, effect->GetProperty( NiAlphaProperty::ALPHA ) );
		if ( alpha ) {
			alpha->SetSrcBlendMode( (NiAlphaProperty::AlphaFunction)srcMode );
			alpha->SetDestBlendMode( (NiAlphaProperty::AlphaFunction)destMode );
		}
	}
}

/*
================
EffectController::AttachEffect
================
*/
// FUNCTION: STBC 0x4CA150
void EffectController::AttachEffect( NiNode *parent ) {
	NiAVObject *effect = GetTargetCast();
	if ( parent && effect ) {
		parent->AttachChild( effect, false );
	}
}

/*
================
EffectController::DetachEffect
================
*/
// FUNCTION: STBC 0x4CA170
void EffectController::DetachEffect() {
	NiAVObject *effect = GetTargetCast();
	if ( effect && effect->GetParent() ) {
		effect->GetParent()->DetachChild( effect );
	}
}

/*
================
EffectController::AddDefaultKeys
================
*/
// FUNCTION: STBC 0x4CA1C0
void EffectController::AddDefaultKeys() {
	if ( data->colorKeys.count == 0 ) {
		AddColorKey( 0.0f, 1.0f, 1.0f, 1.0f );
	}
	if ( data->alphaKeys.count == 0 ) {
		AddAlphaKey( 0.0f, 1.0f );
	}
	if ( data->sizeKeys.count == 0 ) {
		AddSizeKey( 0.0f, 1.0f );
	}
}

/*
================
EffectController::Start

Starts the effect at the time, or at the game time for -NI_INFINITY. A
running effect is not restarted.
================
*/
// FUNCTION: STBC 0x4CA220
void EffectController::Start( float time ) {
	if ( GetField( STATE_MASK, STATE_POS ) == STATE_RUNNING ) {
		return;
	}
	if ( time == -NI_INFINITY ) {
		time = UtopiaApp::GetApp()->GetGameTime();
	}
	NiTimeController::Start( time );
	AddDefaultKeys();
	SetField( STATE_RUNNING, STATE_MASK, STATE_POS );
	startTime = time;
	lastTime = time;
}

/*
================
EffectController::Stop
================
*/
// FUNCTION: STBC 0x4CA290
void EffectController::Stop() {
	SetField( STATE_STOPPED, STATE_MASK, STATE_POS );
	NiTimeController::Stop();
}

/*
================
EffectController::GetColor
================
*/
// FUNCTION: STBC 0x4CA2A0
void EffectController::GetColor( float time, float &r, float &g, float &b ) {
	if ( data->colorKeys.count == 0 ) {
		return;
	}
	NiLinColorKey *key = data->colorKeys.keys;
	int i;
	for ( i = 0; i < data->colorKeys.count; i++ ) {
		if ( key[i].GetTime() > time ) {
			if ( i == 0 ) {
				r = key[0].color.r;
				g = key[0].color.g;
				b = key[0].color.b;
				return;
			}
			NiColorKey::InterpFunction interp = NiColorKey::interpFunctions[NiColorKey::LINKEY];
			NiLinColorKey *key0 = &key[i - 1];
			NiLinColorKey *key1 = &key[i];
			float time0 = key0->GetTime();
			float time1 = key1->GetTime();
			NiColorA color = interp( ( time - time0 ) / ( time1 - time0 ), key0, key1 );
			r = color.r;
			g = color.g;
			b = color.b;
			return;
		}
	}
	NiColorA &last = key[data->colorKeys.count - 1].color;
	r = last.r;
	g = last.g;
	b = last.b;
}

/*
================
EffectController::GetFloatValue
================
*/
// FUNCTION: STBC 0x4CA390
void EffectController::GetFloatValue( KeyArray<NiLinFloatKey> &keys, float time, float &value ) {
	if ( keys.count == 0 ) {
		return;
	}
	NiLinFloatKey *key = keys.keys;
	int i;
	for ( i = 0; i < keys.count; i++ ) {
		if ( key[i].GetTime() > time ) {
			if ( i == 0 ) {
				value = key[0].GetValue();
				return;
			}
			NiFloatKey::InterpFunction interp = NiFloatKey::interpFunctions[NiFloatKey::LINKEY];
			NiLinFloatKey *key0 = &key[i - 1];
			NiLinFloatKey *key1 = &key[i];
			float time0 = key0->GetTime();
			float time1 = key1->GetTime();
			value = interp( ( time - time0 ) / ( time1 - time0 ), key0, key1 );
			return;
		}
	}
	value = key[keys.count - 1].GetValue();
}

/*
================
EffectController::GetAlpha
================
*/
// FUNCTION: STBC 0x4CA420
void EffectController::GetAlpha( float time, float &alpha ) {
	GetFloatValue( data->alphaKeys, time, alpha );
}

/*
================
EffectController::GetSize
================
*/
// FUNCTION: STBC 0x4CA440
void EffectController::GetSize( float time, float &size ) {
	GetFloatValue( data->sizeKeys, time, size );
}

/*
================
EffectController::AddColorKey

Replaces the colour of a key at the same time.
================
*/
// FUNCTION: STBC 0x4CA460
// PARKED: inline budget: retail calls KeyArray::SetSize (0x4CB260) out of line and expands KeyArray::Compare; this build the reverse
void EffectController::AddColorKey( float time, float r, float g, float b ) {
	int count = data->colorKeys.count;
	NiLinColorKey *key = data->colorKeys.keys;
	for ( int i = 0; i < count; i++ ) {
		if ( key[i].time == time ) {
			key[i].color = NiColorA( r, g, b );
			return;
		}
	}
	data->colorKeys.Insert( NiLinColorKey( time, NiColorA( r, g, b ) ) );
}

/*
================
EffectController::AddFloatKey

Replaces the value of a key at the same time.
================
*/
// FUNCTION: STBC 0x4CA5B0
void EffectController::AddFloatKey( KeyArray<NiLinFloatKey> &keys, float time, float value ) {
	NiLinFloatKey *key = keys.keys;
	for ( int i = 0; i < keys.count; i++ ) {
		if ( key[i].time == time ) {
			key[i].value = value;
			return;
		}
	}
	keys.Insert( NiLinFloatKey( time, value ) );
}

/*
================
EffectController::AddAlphaKey
================
*/
// FUNCTION: STBC 0x4CA740
void EffectController::AddAlphaKey( float time, float alpha ) {
	AddFloatKey( data->alphaKeys, time, alpha );
}

/*
================
EffectController::AddSizeKey
================
*/
// FUNCTION: STBC 0x4CA760
void EffectController::AddSizeKey( float time, float size ) {
	AddFloatKey( data->sizeKeys, time, size );
}

/*
================
EffectController::RemoveColorKey
================
*/
// FUNCTION: STBC 0x4CA780
void EffectController::RemoveColorKey( float time ) {
	NiLinColorKey *key = data->colorKeys.keys;
	for ( int i = 0; i < data->colorKeys.count; i++ ) {
		if ( key[i].time == time ) {
			data->colorKeys.RemoveAt( i );
			return;
		}
	}
}

/*
================
EffectController::RemoveFloatKey
================
*/
// FUNCTION: STBC 0x4CA7F0
void EffectController::RemoveFloatKey( KeyArray<NiLinFloatKey> &keys, float time ) {
	NiLinFloatKey *key = keys.keys;
	for ( int i = 0; i < keys.count; i++ ) {
		if ( key[i].time == time ) {
			keys.RemoveAt( i );
			return;
		}
	}
}

/*
================
EffectController::RemoveAlphaKey
================
*/
// FUNCTION: STBC 0x4CA850
void EffectController::RemoveAlphaKey( float time ) {
	RemoveFloatKey( data->alphaKeys, time );
}

/*
================
EffectController::RemoveSizeKey
================
*/
// FUNCTION: STBC 0x4CA870
void EffectController::RemoveSizeKey( float time ) {
	RemoveFloatKey( data->sizeKeys, time );
}

/*
================
EffectController::RemoveAllColorKeys
================
*/
// FUNCTION: STBC 0x4CA890
void EffectController::RemoveAllColorKeys() {
	data->colorKeys.RemoveAll();
}

/*
================
EffectController::RemoveAllAlphaKeys
================
*/
// FUNCTION: STBC 0x4CA8A0
void EffectController::RemoveAllAlphaKeys() {
	data->alphaKeys.RemoveAll();
}

/*
================
EffectController::RemoveAllSizeKeys
================
*/
// FUNCTION: STBC 0x4CA8B0
void EffectController::RemoveAllSizeKeys() {
	data->sizeKeys.RemoveAll();
}
