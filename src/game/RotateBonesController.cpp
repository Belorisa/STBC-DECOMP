/*
===============================================================================

	RotateBonesController.cpp

===============================================================================
*/

#include <stdlib.h>
#include <iostream>

#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "RotateBonesController.h"
#include "BoneState.h"
#include "NiMath.h"
#include "TGAnimNode.h"
#include "UtopiaApp.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00674ad0 _$E45
// FUNCTION: STBC 0x00674ae0 _$E44
// FUNCTION: STBC 0x00674af0 _$E64
// FUNCTION: STBC 0x00674b00 _$E63
// FUNCTION: STBC 0x00674b70 _$E422
// FUNCTION: STBC 0x00674b80 _$E421
// FUNCTION: STBC 0x00674b90 _$E49
// FUNCTION: STBC 0x00674ba0 _$E48
// FUNCTION: STBC 0x00674bb0 _$E60
// FUNCTION: STBC 0x00674bc0 _$E59
// FUNCTION: STBC 0x00674bd0 _$E68
// FUNCTION: STBC 0x00674be0 _$E67
// FUNCTION: STBC 0x00674bf0 _$E72
// FUNCTION: STBC 0x00674c00 _$E71
// FUNCTION: STBC 0x00674c10 _$E196
// FUNCTION: STBC 0x00674c20 _$E195
// FUNCTION: STBC 0x00674d50 _$E138
// FUNCTION: STBC 0x00674d60 _$E137
// FUNCTION: STBC 0x00674d70 _$E151
// FUNCTION: STBC 0x00674d80 _$E150
// FUNCTION: STBC 0x00674d90 _$E201
// FUNCTION: STBC 0x00674da0 _$E200
// FUNCTION: STBC 0x00674db0 _$E206
// FUNCTION: STBC 0x00674dc0 _$E205
// FUNCTION: STBC 0x00674dd0 _$E210
// FUNCTION: STBC 0x00674de0 _$E209
// FUNCTION: STBC 0x00674df0 _$E214
// FUNCTION: STBC 0x00674e00 _$E213
// FUNCTION: STBC 0x00674e10 _$E221
// FUNCTION: STBC 0x00674e20 _$E220
// FUNCTION: STBC 0x00674e30 _$E246
// FUNCTION: STBC 0x00674e40 _$E245
// FUNCTION: STBC 0x00674e50 _$E250
// FUNCTION: STBC 0x00674e60 _$E249
// FUNCTION: STBC 0x00674e70 _$E96
// FUNCTION: STBC 0x00674e80 _$E95
// FUNCTION: STBC 0x00674e90 _$E232
// FUNCTION: STBC 0x00674ea0 _$E231
// FUNCTION: STBC 0x00674eb0 _$E239
// FUNCTION: STBC 0x00674ec0 _$E238
// FUNCTION: STBC 0x00674ed0 _$E262
// FUNCTION: STBC 0x00674ee0 _$E261
// FUNCTION: STBC 0x00674ef0 _$E266
// FUNCTION: STBC 0x00674f00 _$E265
// FUNCTION: STBC 0x00674f10 _$E306
// FUNCTION: STBC 0x00674f20 _$E305
// FUNCTION: STBC 0x00674f30 _$E312
// FUNCTION: STBC 0x00674f40 _$E311
// FUNCTION: STBC 0x00674f50 _$E254
// FUNCTION: STBC 0x00674f60 _$E253
// FUNCTION: STBC 0x00674f70 _$E258
// FUNCTION: STBC 0x00674f80 _$E257
// FUNCTION: STBC 0x00674f90 _$E270
// FUNCTION: STBC 0x00674fa0 _$E269
// FUNCTION: STBC 0x00674fb0 _$E274
// FUNCTION: STBC 0x00674fc0 _$E273
// FUNCTION: STBC 0x00674fd0 _$E278
// FUNCTION: STBC 0x00674fe0 _$E277
// FUNCTION: STBC 0x00674ff0 _$E282
// FUNCTION: STBC 0x00675000 _$E281
// FUNCTION: STBC 0x00675010 _$E286
// FUNCTION: STBC 0x00675020 _$E285
// FUNCTION: STBC 0x00675030 _$E290
// FUNCTION: STBC 0x00675040 _$E289
// FUNCTION: STBC 0x00675050 _$E294
// FUNCTION: STBC 0x00675060 _$E293
// FUNCTION: STBC 0x00675070 _$E298
// FUNCTION: STBC 0x00675080 _$E297
// FUNCTION: STBC 0x00675090 _$E101
// FUNCTION: STBC 0x006750a0 _$E100
// FUNCTION: STBC 0x006750b0 _$E302
// FUNCTION: STBC 0x006750c0 _$E301
// FUNCTION: STBC 0x006750d0 _$E106
// FUNCTION: STBC 0x006750e0 _$E105
// FUNCTION: STBC 0x006750f0 _$E316
// FUNCTION: STBC 0x00675100 _$E315
// FUNCTION: STBC 0x00675110 _$E320
// FUNCTION: STBC 0x00675120 _$E319
// FUNCTION: STBC 0x00675130 _$E324
// FUNCTION: STBC 0x00675140 _$E323
// FUNCTION: STBC 0x00675150 _$E328
// FUNCTION: STBC 0x00675160 _$E327
// FUNCTION: STBC 0x00675170 _$E115
// FUNCTION: STBC 0x00675180 _$E114
// FUNCTION: STBC 0x00675190 _$E332
// FUNCTION: STBC 0x006751a0 _$E331
// FUNCTION: STBC 0x006751b0 _$E336
// FUNCTION: STBC 0x006751c0 _$E335
// FUNCTION: STBC 0x006751d0 _$E340
// FUNCTION: STBC 0x006751e0 _$E339
// FUNCTION: STBC 0x006751f0 _$E121
// FUNCTION: STBC 0x00675200 _$E120
// FUNCTION: STBC 0x00675210 _$E126
// FUNCTION: STBC 0x00675220 _$E125
// FUNCTION: STBC 0x00675230 _$E156
// FUNCTION: STBC 0x00675240 _$E155
// FUNCTION: STBC 0x00675250 _$E344
// FUNCTION: STBC 0x00675260 _$E343
// FUNCTION: STBC 0x00675270 _$E161
// FUNCTION: STBC 0x00675280 _$E160
// FUNCTION: STBC 0x00675290 _$E348
// FUNCTION: STBC 0x006752a0 _$E347
// FUNCTION: STBC 0x006752b0 _$E352
// FUNCTION: STBC 0x006752c0 _$E351
// FUNCTION: STBC 0x006752d0 _$E166
// FUNCTION: STBC 0x006752e0 _$E165
// FUNCTION: STBC 0x006752f0 _$E356
// FUNCTION: STBC 0x00675300 _$E355
// FUNCTION: STBC 0x00675310 _$E131
// FUNCTION: STBC 0x00675320 _$E130
// FUNCTION: STBC 0x00675330 _$E360
// FUNCTION: STBC 0x00675340 _$E359
// FUNCTION: STBC 0x00675350 _$E364
// FUNCTION: STBC 0x00675360 _$E363
// FUNCTION: STBC 0x00675370 _$E171
// FUNCTION: STBC 0x00675380 _$E170
// FUNCTION: STBC 0x00675390 _$E176
// FUNCTION: STBC 0x006753a0 _$E175
// FUNCTION: STBC 0x006753b0 _$E368
// FUNCTION: STBC 0x006753c0 _$E367
// FUNCTION: STBC 0x006753d0 _$E372
// FUNCTION: STBC 0x006753e0 _$E371
// FUNCTION: STBC 0x006753f0 _$E376
// FUNCTION: STBC 0x00675400 _$E375
// FUNCTION: STBC 0x00675410 _$E380
// FUNCTION: STBC 0x00675420 _$E379
// FUNCTION: STBC 0x00675430 _$E384
// FUNCTION: STBC 0x00675440 _$E383
// FUNCTION: STBC 0x00675450 _$E388
// FUNCTION: STBC 0x00675460 _$E387
// FUNCTION: STBC 0x00675470 _$E392
// FUNCTION: STBC 0x00675480 _$E391
// FUNCTION: STBC 0x00675490 _$E396
// FUNCTION: STBC 0x006754a0 _$E395
// FUNCTION: STBC 0x006754b0 _$E181
// FUNCTION: STBC 0x006754c0 _$E180
// FUNCTION: STBC 0x006754d0 _$E400
// FUNCTION: STBC 0x006754e0 _$E399
// FUNCTION: STBC 0x006754f0 _$E186
// FUNCTION: STBC 0x00675500 _$E185
// FUNCTION: STBC 0x00675510 _$E191
// FUNCTION: STBC 0x00675520 _$E190

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00674a50 _$E32
// FUNCTION: STBC 0x00674a60 _$E29
// FUNCTION: STBC 0x00674a70 _$E31
// FUNCTION: STBC 0x00674a80 _$E30
// FUNCTION: STBC 0x00674a90 _$E38
// FUNCTION: STBC 0x00674aa0 _$E35
// FUNCTION: STBC 0x00674ab0 _$E37
// FUNCTION: STBC 0x00674ac0 _$E36
// FUNCTION: STBC 0x00674c30 _$E444
// FUNCTION: STBC 0x00674c50 _$E443

// The NiRTTI's static initialiser, its exit-time thunk, the exit-time
// destructor and the guarded destructor.
// FUNCTION: STBC 0x00673530 _$E436
// FUNCTION: STBC 0x00673540 _$E432
// FUNCTION: STBC 0x00673560 _$E435
// FUNCTION: STBC 0x00673570 _$E433
// DATA: STBC 0x992040 ?rtti@RotateBonesController@@2VNiRTTI@@B
const NiRTTI RotateBonesController::rtti( "RotateBonesController", &NiTimeController::rtti );

/*
================
RotateBonesController::TargetIsRequiredType
================
*/
// FUNCTION: STBC 0x673590
bool RotateBonesController::TargetIsRequiredType() const {
	if ( !target ) {
		return false;
	}
	return target->IsKindOf( &NiAVObject::rtti );
}

/*
================
RotateBonesController::VirtualSlot22
================
*/
// FUNCTION: STBC 0x6735C0
const NiRTTI *RotateBonesController::VirtualSlot22() {
	return &NiAVObject::rtti;
}

/*
================
RotateBonesController::RotateBonesController
================
*/
// FUNCTION: STBC 0x6735D0
RotateBonesController::RotateBonesController() {
	VirtualSlot21();
}

/*
================
RotateBonesController::~RotateBonesController
================
*/
// FUNCTION: STBC 0x673650
RotateBonesController::~RotateBonesController() {
}

/*
================
RotateBonesController::Update

Gives each bone that has finished its track a new random delay; the first
two bones also pick their next track, the others share one delay.
================
*/
// FUNCTION: STBC 0x673680
void RotateBonesController::Update( float time ) {
	if ( !started ) {
		return;
	}

	int count = boneControllers.GetSize();
	bool haveDelay = false;
	float delay;
	for ( int i = 0; i < count; i++ ) {
		BoneStateController *controller = boneControllers.GetAt( i );
		if ( controller == NULL ) {
			continue;
		}
		BoneState *state = controller->state;
		if ( state == NULL || !state->finished ) {
			continue;
		}

		if ( i == 0 ) {
			delay = (float)( rand() % 300 ) / 100.0f;
			state->finished = false;
			state->delay = delay;
			state->currentTrack = rand() % 3;
		} else if ( i == 1 ) {
			delay = (float)( rand() % 300 ) / 100.0f;
			state->finished = false;
			state->delay = delay;
			state->currentTrack = rand() % 2;
		} else {
			if ( !haveDelay ) {
				delay = (float)( rand() % 300 ) / 100.0f;
				haveDelay = true;
			}
			state->finished = false;
			state->delay = delay;
		}
	}
}

/*
================
RotateBonesController::CreateClone
================
*/
// FUNCTION: STBC 0x6737A0
NiObject *RotateBonesController::CreateClone() {
	RotateBonesController *clone = new RotateBonesController;
	CopyMembers( clone );
	return clone;
}

/*
================
RotateBonesController::CopyMembers
================
*/
// FUNCTION: STBC 0x673800
void RotateBonesController::CopyMembers( RotateBonesController *dest ) {
	NiTimeController::CopyMembers( dest );
	dest->CopyFrom( this );
}

/*
================
RotateBonesController::SetTarget

Puts a BoneStateController on each of the character's head, spine,
clavicle and upper arm bones it finds, with the bone's rotation tracks: the
head nods, shakes and tilts by a degree, the spine sways by half a degree
and the shoulders and arms roll by three. One bone, axis and angle local
serve every bone and key: retail keeps one frame slot for the six bones and
computes each angle into its slot before the key is allocated.
================
*/
// FUNCTION: STBC 0x673820
void RotateBonesController::SetTarget( NiObjectNET *target_ ) {
	NiTimeController::SetTarget( target_ );

	TGAnimNode *animNode = NiDynamicCast( TGAnimNode, target_ );
	if ( animNode == NULL ) {
		return;
	}

	NiPoint3 axis;
	float angle;
	NiRotKey **keys;
	BoneState *state;
	BoneStateController *controller;
	NiNode *bone;

	bone = NiDynamicCast( NiNode, animNode->GetObjectByName( "Bip01 Head" ) );
	if ( bone != NULL ) {
		state = new BoneState;
		state->SetBone( bone );

		axis = NiPoint3( 0.0f, 0.0f, 1.0f );
		keys = new NiRotKey *[4];
		keys[0] = new NiRotKey( 0.0f, 0.0f, axis );
		angle = NI_TWO_PI * ( 1.0f / 360.0f );
		keys[1] = new NiRotKey( 0.5f, angle, axis );
		angle = -( NI_TWO_PI * ( 1.0f / 360.0f ) );
		keys[2] = new NiRotKey( 1.0f, angle, axis );
		keys[3] = new NiRotKey( 1.5f, 0.0f, axis );
		state->SetTrack( 0, keys, 4 );

		axis = NiPoint3( 0.0f, 1.0f, 0.0f );
		keys = new NiRotKey *[4];
		keys[0] = new NiRotKey( 0.0f, 0.0f, axis );
		angle = NI_TWO_PI * ( 1.0f / 360.0f );
		keys[1] = new NiRotKey( 0.5f, angle, axis );
		angle = -( NI_TWO_PI * ( 1.0f / 360.0f ) );
		keys[2] = new NiRotKey( 1.0f, angle, axis );
		keys[3] = new NiRotKey( 1.5f, 0.0f, axis );
		state->SetTrack( 1, keys, 4 );

		axis = NiPoint3( 1.0f, 0.0f, 0.0f );
		keys = new NiRotKey *[4];
		keys[0] = new NiRotKey( 0.0f, 0.0f, axis );
		angle = NI_TWO_PI * ( 1.0f / 360.0f );
		keys[1] = new NiRotKey( 0.5f, angle, axis );
		angle = -( NI_TWO_PI * ( 1.0f / 360.0f ) );
		keys[2] = new NiRotKey( 1.0f, angle, axis );
		keys[3] = new NiRotKey( 1.5f, 0.0f, axis );
		state->SetTrack( 2, keys, 4 );

		controller = new BoneStateController;
		controller->state = state;
		controller->SetTarget( bone );
		boneControllers.Add( controller );
	}

	bone = NiDynamicCast( NiNode, animNode->GetObjectByName( "Bip01 Spine1" ) );
	if ( bone != NULL ) {
		state = new BoneState;
		state->SetBone( bone );

		axis = NiPoint3( 0.0f, 0.0f, 1.0f );
		// room for four keys; the track uses three
		keys = new NiRotKey *[4];
		keys[0] = new NiRotKey( 0.0f, 0.0f, axis );
		angle = NI_TWO_PI * ( 0.5f / 360.0f );
		keys[1] = new NiRotKey( 1.5f, angle, axis );
		keys[2] = new NiRotKey( 3.0f, 0.0f, axis );
		state->SetTrack( 0, keys, 3 );

		axis = NiPoint3( 1.0f, 0.0f, 0.0f );
		keys = new NiRotKey *[4];
		keys[0] = new NiRotKey( 0.0f, 0.0f, axis );
		angle = NI_TWO_PI * ( 0.5f / 360.0f );
		keys[1] = new NiRotKey( 1.0f, angle, axis );
		angle = -( NI_TWO_PI * ( 0.5f / 360.0f ) );
		keys[2] = new NiRotKey( 2.0f, angle, axis );
		keys[3] = new NiRotKey( 3.0f, 0.0f, axis );
		state->SetTrack( 1, keys, 4 );

		controller = new BoneStateController;
		controller->state = state;
		controller->SetTarget( bone );
		boneControllers.Add( controller );
	}

	bone = NiDynamicCast( NiNode, animNode->GetObjectByName( "Bip01 L Clavicle" ) );
	if ( bone != NULL ) {
		state = new BoneState;
		state->SetBone( bone );

		axis = NiPoint3( 0.0f, 1.0f, 0.0f );
		keys = new NiRotKey *[3];
		keys[0] = new NiRotKey( 0.0f, 0.0f, axis );
		angle = NI_TWO_PI * ( 3.0f / 360.0f );
		keys[1] = new NiRotKey( 1.0f, angle, axis );
		keys[2] = new NiRotKey( 3.0f, 0.0f, axis );
		state->SetTrack( 0, keys, 3 );

		controller = new BoneStateController;
		controller->state = state;
		controller->SetTarget( bone );
		boneControllers.Add( controller );
	}

	bone = NiDynamicCast( NiNode, animNode->GetObjectByName( "Bip01 R Clavicle" ) );
	if ( bone != NULL ) {
		state = new BoneState;
		state->SetBone( bone );

		axis = NiPoint3( 0.0f, -1.0f, 0.0f );
		keys = new NiRotKey *[3];
		keys[0] = new NiRotKey( 0.0f, 0.0f, axis );
		angle = NI_TWO_PI * ( 3.0f / 360.0f );
		keys[1] = new NiRotKey( 1.0f, angle, axis );
		keys[2] = new NiRotKey( 3.0f, 0.0f, axis );
		state->SetTrack( 0, keys, 3 );

		controller = new BoneStateController;
		controller->state = state;
		controller->SetTarget( bone );
		boneControllers.Add( controller );
	}

	bone = NiDynamicCast( NiNode, animNode->GetObjectByName( "Bip01 L UpperArm" ) );
	if ( bone != NULL ) {
		state = new BoneState;
		state->SetBone( bone );

		axis = NiPoint3( 0.0f, 1.0f, 0.0f );
		keys = new NiRotKey *[3];
		keys[0] = new NiRotKey( 0.0f, 0.0f, axis );
		angle = NI_TWO_PI * ( 3.0f / 360.0f );
		keys[1] = new NiRotKey( 1.0f, angle, axis );
		keys[2] = new NiRotKey( 3.0f, 0.0f, axis );
		state->SetTrack( 0, keys, 3 );

		controller = new BoneStateController;
		controller->state = state;
		controller->SetTarget( bone );
		boneControllers.Add( controller );
	}

	bone = NiDynamicCast( NiNode, animNode->GetObjectByName( "Bip01 R UpperArm" ) );
	if ( bone != NULL ) {
		state = new BoneState;
		state->SetBone( bone );

		axis = NiPoint3( 0.0f, -1.0f, 0.0f );
		keys = new NiRotKey *[3];
		keys[0] = new NiRotKey( 0.0f, 0.0f, axis );
		angle = NI_TWO_PI * ( 3.0f / 360.0f );
		keys[1] = new NiRotKey( 1.0f, angle, axis );
		keys[2] = new NiRotKey( 3.0f, 0.0f, axis );
		state->SetTrack( 0, keys, 3 );

		controller = new BoneStateController;
		controller->state = state;
		controller->SetTarget( bone );
		boneControllers.Add( controller );
	}
}

/*
================
RotateBonesController::CopyFrom

Nothing is copied: the clone gets no child controllers.
================
*/
// FUNCTION: STBC 0x674980
void RotateBonesController::CopyFrom( RotateBonesController *source ) {
}

/*
================
RotateBonesController::Start
================
*/
// FUNCTION: STBC 0x674990
void RotateBonesController::Start( float time ) {
	NiTimeController::Start( time );
	int count = boneControllers.GetSize();
	for ( int i = 0; i < count; i++ ) {
		BoneStateController *controller = boneControllers.GetAt( i );
		if ( controller != NULL ) {
			controller->VirtualSlot20();
		}
	}
	started = true;
}

/*
================
RotateBonesController::VirtualSlot20
================
*/
// FUNCTION: STBC 0x6749D0
void RotateBonesController::VirtualSlot20() {
	Start( UtopiaApp::GetApp()->GetGameTime() );
}

/*
================
RotateBonesController::Stop
================
*/
// FUNCTION: STBC 0x6749F0
void RotateBonesController::Stop() {
	NiTimeController::Stop();
	started = false;
	int count = boneControllers.GetSize();
	for ( int i = 0; i < count; i++ ) {
		BoneStateController *controller = boneControllers.GetAt( i );
		if ( controller != NULL ) {
			controller->Stop();
		}
	}
}

/*
================
RotateBonesController::VirtualSlot21
================
*/
// FUNCTION: STBC 0x674A20
void RotateBonesController::VirtualSlot21() {
	started = false;
}
