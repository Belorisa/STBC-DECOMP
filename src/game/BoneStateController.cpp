/*
===============================================================================

	BoneStateController.cpp

===============================================================================
*/

#include <iostream>
#include "BoneStateController.h"
#include "NiAVObject.h"
#include "NiKeyframeController.h"
#include "UtopiaApp.h"
#include "NiBone.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0065fd40 _$E473
// FUNCTION: STBC 0x0065fd60 _$E472

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0065fcb0 _$E45
// FUNCTION: STBC 0x0065fcc0 _$E44
// FUNCTION: STBC 0x0065fcd0 _$E56
// FUNCTION: STBC 0x0065fce0 _$E55
// FUNCTION: STBC 0x0065fd20 _$E74
// FUNCTION: STBC 0x0065fd30 _$E73
// FUNCTION: STBC 0x0065fd70 _$E246
// FUNCTION: STBC 0x0065fd80 _$E245
// FUNCTION: STBC 0x0065fd90 _$E79
// FUNCTION: STBC 0x0065fda0 _$E78
// FUNCTION: STBC 0x0065fdb0 _$E83
// FUNCTION: STBC 0x0065fdc0 _$E82
// FUNCTION: STBC 0x0065fdd0 _$E87
// FUNCTION: STBC 0x0065fde0 _$E86
// FUNCTION: STBC 0x0065fdf0 _$E202
// FUNCTION: STBC 0x0065fe00 _$E201
// FUNCTION: STBC 0x0065fe10 _$E144
// FUNCTION: STBC 0x0065fe20 _$E143
// FUNCTION: STBC 0x0065fe30 _$E157
// FUNCTION: STBC 0x0065fe40 _$E156
// FUNCTION: STBC 0x0065fe50 _$E238
// FUNCTION: STBC 0x0065fe60 _$E237
// FUNCTION: STBC 0x0065fe70 _$E251
// FUNCTION: STBC 0x0065fe80 _$E250
// FUNCTION: STBC 0x0065fe90 _$E242
// FUNCTION: STBC 0x0065fea0 _$E241
// FUNCTION: STBC 0x0065feb0 _$E255
// FUNCTION: STBC 0x0065fec0 _$E254
// FUNCTION: STBC 0x0065fed0 _$E262
// FUNCTION: STBC 0x0065fee0 _$E261
// FUNCTION: STBC 0x0065fef0 _$E296
// FUNCTION: STBC 0x0065ff00 _$E295
// FUNCTION: STBC 0x0065ff10 _$E300
// FUNCTION: STBC 0x0065ff20 _$E299
// FUNCTION: STBC 0x0065ff30 _$E102
// FUNCTION: STBC 0x0065ff40 _$E101
// FUNCTION: STBC 0x0065ff50 _$E282
// FUNCTION: STBC 0x0065ff60 _$E281
// FUNCTION: STBC 0x0065ff70 _$E289
// FUNCTION: STBC 0x0065ff80 _$E288
// FUNCTION: STBC 0x0065ff90 _$E312
// FUNCTION: STBC 0x0065ffa0 _$E311
// FUNCTION: STBC 0x0065ffb0 _$E316
// FUNCTION: STBC 0x0065ffc0 _$E315
// FUNCTION: STBC 0x0065ffd0 _$E352
// FUNCTION: STBC 0x0065ffe0 _$E351
// FUNCTION: STBC 0x0065fff0 _$E358
// FUNCTION: STBC 0x00660000 _$E357
// FUNCTION: STBC 0x00660010 _$E304
// FUNCTION: STBC 0x00660020 _$E303
// FUNCTION: STBC 0x00660030 _$E308
// FUNCTION: STBC 0x00660040 _$E307
// FUNCTION: STBC 0x00660050 _$E320
// FUNCTION: STBC 0x00660060 _$E319
// FUNCTION: STBC 0x00660070 _$E324
// FUNCTION: STBC 0x00660080 _$E323
// FUNCTION: STBC 0x00660090 _$E328
// FUNCTION: STBC 0x006600a0 _$E327
// FUNCTION: STBC 0x006600b0 _$E332
// FUNCTION: STBC 0x006600c0 _$E331
// FUNCTION: STBC 0x006600d0 _$E336
// FUNCTION: STBC 0x006600e0 _$E335
// FUNCTION: STBC 0x006600f0 _$E271
// FUNCTION: STBC 0x00660100 _$E270
// FUNCTION: STBC 0x00660110 _$E340
// FUNCTION: STBC 0x00660120 _$E339
// FUNCTION: STBC 0x00660130 _$E344
// FUNCTION: STBC 0x00660140 _$E343
// FUNCTION: STBC 0x00660150 _$E107
// FUNCTION: STBC 0x00660160 _$E106
// FUNCTION: STBC 0x00660170 _$E348
// FUNCTION: STBC 0x00660180 _$E347
// FUNCTION: STBC 0x00660190 _$E112
// FUNCTION: STBC 0x006601a0 _$E111
// FUNCTION: STBC 0x006601b0 _$E362
// FUNCTION: STBC 0x006601c0 _$E361
// FUNCTION: STBC 0x006601d0 _$E366
// FUNCTION: STBC 0x006601e0 _$E365
// FUNCTION: STBC 0x006601f0 _$E370
// FUNCTION: STBC 0x00660200 _$E369
// FUNCTION: STBC 0x00660210 _$E374
// FUNCTION: STBC 0x00660220 _$E373
// FUNCTION: STBC 0x00660230 _$E121
// FUNCTION: STBC 0x00660240 _$E120
// FUNCTION: STBC 0x00660250 _$E378
// FUNCTION: STBC 0x00660260 _$E377
// FUNCTION: STBC 0x00660270 _$E382
// FUNCTION: STBC 0x00660280 _$E381
// FUNCTION: STBC 0x00660290 _$E386
// FUNCTION: STBC 0x006602a0 _$E385
// FUNCTION: STBC 0x006602b0 _$E127
// FUNCTION: STBC 0x006602c0 _$E126
// FUNCTION: STBC 0x006602d0 _$E132
// FUNCTION: STBC 0x006602e0 _$E131
// FUNCTION: STBC 0x006602f0 _$E162
// FUNCTION: STBC 0x00660300 _$E161
// FUNCTION: STBC 0x00660310 _$E390
// FUNCTION: STBC 0x00660320 _$E389
// FUNCTION: STBC 0x00660330 _$E167
// FUNCTION: STBC 0x00660340 _$E166
// FUNCTION: STBC 0x00660350 _$E394
// FUNCTION: STBC 0x00660360 _$E393
// FUNCTION: STBC 0x00660370 _$E398
// FUNCTION: STBC 0x00660380 _$E397
// FUNCTION: STBC 0x00660390 _$E172
// FUNCTION: STBC 0x006603a0 _$E171
// FUNCTION: STBC 0x006603b0 _$E402
// FUNCTION: STBC 0x006603c0 _$E401
// FUNCTION: STBC 0x006603d0 _$E137
// FUNCTION: STBC 0x006603e0 _$E136
// FUNCTION: STBC 0x006603f0 _$E406
// FUNCTION: STBC 0x00660400 _$E405
// FUNCTION: STBC 0x00660410 _$E410
// FUNCTION: STBC 0x00660420 _$E409
// FUNCTION: STBC 0x00660430 _$E177
// FUNCTION: STBC 0x00660440 _$E176
// FUNCTION: STBC 0x00660450 _$E182
// FUNCTION: STBC 0x00660460 _$E181
// FUNCTION: STBC 0x00660470 _$E414
// FUNCTION: STBC 0x00660480 _$E413
// FUNCTION: STBC 0x00660490 _$E418
// FUNCTION: STBC 0x006604a0 _$E417
// FUNCTION: STBC 0x006604b0 _$E422
// FUNCTION: STBC 0x006604c0 _$E421
// FUNCTION: STBC 0x006604d0 _$E426
// FUNCTION: STBC 0x006604e0 _$E425
// FUNCTION: STBC 0x006604f0 _$E430
// FUNCTION: STBC 0x00660500 _$E429
// FUNCTION: STBC 0x00660510 _$E434
// FUNCTION: STBC 0x00660520 _$E433
// FUNCTION: STBC 0x00660530 _$E438
// FUNCTION: STBC 0x00660540 _$E437
// FUNCTION: STBC 0x00660550 _$E442
// FUNCTION: STBC 0x00660560 _$E441
// FUNCTION: STBC 0x00660570 _$E187
// FUNCTION: STBC 0x00660580 _$E186
// FUNCTION: STBC 0x00660590 _$E446
// FUNCTION: STBC 0x006605a0 _$E445
// FUNCTION: STBC 0x006605b0 _$E192
// FUNCTION: STBC 0x006605c0 _$E191
// FUNCTION: STBC 0x006605d0 _$E197
// FUNCTION: STBC 0x006605e0 _$E196
// FUNCTION: STBC 0x00660610 _$E216
// FUNCTION: STBC 0x00660620 _$E215
// FUNCTION: STBC 0x00660630 _$E225
// FUNCTION: STBC 0x00660640 _$E224
// FUNCTION: STBC 0x00660650 _$E229
// FUNCTION: STBC 0x00660660 _$E228
// FUNCTION: STBC 0x00660670 _$E233
// FUNCTION: STBC 0x00660680 _$E232

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0065fc30 _$E32
// FUNCTION: STBC 0x0065fc40 _$E29
// FUNCTION: STBC 0x0065fc50 _$E31
// FUNCTION: STBC 0x0065fc60 _$E30
// FUNCTION: STBC 0x0065fc70 _$E38
// FUNCTION: STBC 0x0065fc80 _$E35
// FUNCTION: STBC 0x0065fc90 _$E37
// FUNCTION: STBC 0x0065fca0 _$E36

// The NiRTTI's static initialiser, its exit-time thunk, the exit-time
// destructor and the guarded destructor.
// FUNCTION: STBC 0x0065f8f0 _$E465
// FUNCTION: STBC 0x0065f900 _$E461
// FUNCTION: STBC 0x0065f920 _$E464
// FUNCTION: STBC 0x0065f930 _$E462
// DATA: STBC 0x9916E0 ?rtti@BoneStateController@@2VNiRTTI@@B
const NiRTTI BoneStateController::rtti( "BoneStateController", &NiTimeController::rtti );

/*
================
BoneStateController::VirtualSlot22
================
*/
// FUNCTION: STBC 0x65F950
const NiRTTI *BoneStateController::VirtualSlot22() {
	return &NiAVObject::rtti;
}

/*
================
BoneStateController::BoneStateController
================
*/
// FUNCTION: STBC 0x65F960
BoneStateController::BoneStateController() {
	VirtualSlot21();
}

/*
================
BoneStateController::~BoneStateController
================
*/
// FUNCTION: STBC 0x65F9B0
BoneStateController::~BoneStateController() {
	if ( state != NULL ) {
		delete state;
	}
}

/*
================
BoneStateController::Update
================
*/
// FUNCTION: STBC 0x65FA20
void BoneStateController::Update( float time ) {
	if ( started && state != NULL ) {
		float delta = time - lastUpdate;
		lastUpdate = time;
		state->Update( delta );
	}
}

/*
================
BoneStateController::SetTarget

Moves each keyframe controller of the target to the head of its list.
================
*/
// FUNCTION: STBC 0x65FA60
// PARKED: retail releases the controller with no null test before its deleting destructor; DecRefCount's delete this keeps one (a NiPointer adds an EH frame), and no spelling of the release without it is known
void BoneStateController::SetTarget( NiObjectNET *target_ ) {
	NiTimeController::SetTarget( target_ );
	for ( NiTimeController *controller = target_->GetControllers(); controller != NULL; controller = controller->GetNext() ) {
		if ( controller->IsKindOf( &NiKeyframeController::rtti ) ) {
			// keep the controller alive while it is out of the list
			controller->IncRefCount();
			target_->RemoveController( controller );
			target_->PrependController( controller );
			controller->DecRefCount();
		}
	}
}

/*
================
BoneStateController::CreateClone
================
*/
// FUNCTION: STBC 0x65FAD0
NiObject *BoneStateController::CreateClone() {
	BoneStateController *clone = new BoneStateController;
	CopyMembers( clone );
	return clone;
}

/*
================
BoneStateController::CopyMembers
================
*/
// FUNCTION: STBC 0x65FB30
void BoneStateController::CopyMembers( BoneStateController *dest ) {
	NiTimeController::CopyMembers( dest );
	dest->CopyFrom( this );
}

/*
================
BoneStateController::CopyFrom
================
*/
// FUNCTION: STBC 0x65FB50
void BoneStateController::CopyFrom( BoneStateController *source ) {
	if ( source->state != NULL ) {
		state = source->state->Clone();
	} else {
		state = NULL;
	}
	started = source->started;
	lastUpdate = source->lastUpdate;
}

/*
================
BoneStateController::Start
================
*/
// FUNCTION: STBC 0x65FB90
void BoneStateController::Start( float time ) {
	NiTimeController::Start( time );
	if ( state != NULL ) {
		state->Reset();
	}
	started = true;
	lastUpdate = time;
}

/*
================
BoneStateController::VirtualSlot20
================
*/
// FUNCTION: STBC 0x65FBC0
void BoneStateController::VirtualSlot20() {
	Start( UtopiaApp::GetApp()->GetGameTime() );
}

/*
================
BoneStateController::Stop
================
*/
// FUNCTION: STBC 0x65FBE0
void BoneStateController::Stop() {
	NiTimeController::Stop();
	started = false;
}

/*
================
BoneStateController::VirtualSlot21
================
*/
// FUNCTION: STBC 0x65FBF0
void BoneStateController::VirtualSlot21() {
	started = false;
}

/*
================
BoneStateController::TargetIsRequiredType
================
*/
// FUNCTION: STBC 0x65FC00
bool BoneStateController::TargetIsRequiredType() const {
	if ( !target ) {
		return false;
	}
	return target->IsKindOf( &NiAVObject::rtti );
}
