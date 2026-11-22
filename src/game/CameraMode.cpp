/*
===============================================================================

	CameraMode.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "CameraMode.h"
#include "TGEventHandlerTable.h"
#include "TGStream.h"
#include "TGEvent.h"
#include "TGEventManager.h"
#include "CameraObjectClass.h"
#include "ObjectClass.h"
#include "NiCamera.h"
#include "DamageableObject.h"
#include "PlacementObject.h"
#include "SetClass.h"
#include "UtopiaApp.h"
#include "BezierCurve.h"
#include "TGPoint3.h"
#include "NiMath.h"
#include "TorpedoTube.h"
#include "Torpedo.h"
#include "TGModelUtils.h"
#include "Waypoint.h"
#include "Nebula.h"
#include "SensorSubsystem.h"
#include "ShipSubsystem.h"
#include "ShipClass.h"
#include "NiLight.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// <iostream>'s statics: the ios_base::Init and _Winit quads.
// FUNCTION: STBC 0x0041d750 _$E32
// FUNCTION: STBC 0x0041d760 _$E29
// FUNCTION: STBC 0x0041d770 _$E31
// FUNCTION: STBC 0x0041d780 _$E30
// FUNCTION: STBC 0x0041d790 _$E38
// FUNCTION: STBC 0x0041d7a0 _$E35
// FUNCTION: STBC 0x0041d7b0 _$E37
// FUNCTION: STBC 0x0041d7c0 _$E36

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00429610 _$E590
// FUNCTION: STBC 0x00429630 _$E589

// The dynamic initialisers of the handler tables and their exit-time
// destructors, in the tables' order.
// FUNCTION: STBC 0x0041dc10 _$E497
// FUNCTION: STBC 0x0041dc20 _$E493
// FUNCTION: STBC 0x0041dc30 _$E496
// FUNCTION: STBC 0x0041dc40 _$E494
// FUNCTION: STBC 0x0041dc60 _$E502
// FUNCTION: STBC 0x0041dc70 _$E499
// FUNCTION: STBC 0x0041dc80 _$E501
// FUNCTION: STBC 0x0041dc90 _$E500
// FUNCTION: STBC 0x0041dcb0 _$E507
// FUNCTION: STBC 0x0041dcc0 _$E504
// FUNCTION: STBC 0x0041dcd0 _$E506
// FUNCTION: STBC 0x0041dce0 _$E505
// FUNCTION: STBC 0x0041dd00 _$E512
// FUNCTION: STBC 0x0041dd10 _$E509
// FUNCTION: STBC 0x0041dd20 _$E511
// FUNCTION: STBC 0x0041dd30 _$E510
// FUNCTION: STBC 0x0041dd50 _$E517
// FUNCTION: STBC 0x0041dd60 _$E514
// FUNCTION: STBC 0x0041dd70 _$E516
// FUNCTION: STBC 0x0041dd80 _$E515
// FUNCTION: STBC 0x0041dda0 _$E522
// FUNCTION: STBC 0x0041ddb0 _$E519
// FUNCTION: STBC 0x0041ddc0 _$E521
// FUNCTION: STBC 0x0041ddd0 _$E520
// FUNCTION: STBC 0x0041ddf0 _$E527
// FUNCTION: STBC 0x0041de00 _$E524
// FUNCTION: STBC 0x0041de10 _$E526
// FUNCTION: STBC 0x0041de20 _$E525
// FUNCTION: STBC 0x0041de40 _$E532
// FUNCTION: STBC 0x0041de50 _$E529
// FUNCTION: STBC 0x0041de60 _$E531
// FUNCTION: STBC 0x0041de70 _$E530
// FUNCTION: STBC 0x0041de90 _$E538
// FUNCTION: STBC 0x0041dea0 _$E534
// FUNCTION: STBC 0x0041deb0 _$E537
// FUNCTION: STBC 0x0041dec0 _$E535
// FUNCTION: STBC 0x0041dee0 _$E543
// FUNCTION: STBC 0x0041def0 _$E540
// FUNCTION: STBC 0x0041df00 _$E542
// FUNCTION: STBC 0x0041df10 _$E541
// FUNCTION: STBC 0x0041df30 _$E548
// FUNCTION: STBC 0x0041df40 _$E545
// FUNCTION: STBC 0x0041df50 _$E547
// FUNCTION: STBC 0x0041df60 _$E546
// DATA: STBC 0x0097ef40
TGEventHandlerTable CameraMode::classHandlers;

// DATA: STBC 0x0097eea0
TGEventHandlerTable IdealControlledCameraMode::classHandlers;

// DATA: STBC 0x0097ef90
TGEventHandlerTable ChaseCameraMode::classHandlers;

// DATA: STBC 0x0097ed90
TGEventHandlerTable TargetCameraMode::classHandlers;

// DATA: STBC 0x0097ee20
TGEventHandlerTable ZoomTargetMode::classHandlers;

// DATA: STBC 0x0097ee80
TGEventHandlerTable MapCameraMode::classHandlers;

// DATA: STBC 0x0097ef60
TGEventHandlerTable DropAndWatchMode::classHandlers;

// DATA: STBC 0x0097ee00
TGEventHandlerTable PlacementWatchMode::classHandlers;

// DATA: STBC 0x0097ee68
TGEventHandlerTable LockedPositionMode::classHandlers;

// DATA: STBC 0x0097eda8
TGEventHandlerTable TorpCameraMode::classHandlers;

// DATA: STBC 0x0097ede0
TGEventHandlerTable PlaceByDirectionMode::classHandlers;

// A static object of it files a mode's function in the registry when the
// game starts; each mode's class has one, under the name the scripts give
// the mode (invented name; retail's dynamic initialisers 0x41DFD0-0x41E720
// expand the registry's SetAt, one per mode, and store nothing).
struct CameraModeRegistration {
	CameraModeRegistration( const char *className, CameraMode::CreateFunction create ) {
		CameraMode::RegisterCreator( className, create );
	}
};

// The registry's dynamic initialiser, the registration of its destructor
// and the destructor.
// FUNCTION: STBC 0x0041df80 _$E554
// FUNCTION: STBC 0x0041df90 _$E550
// FUNCTION: STBC 0x0041dfa0 _$E553
// FUNCTION: STBC 0x0041dfb0 _$E551
// DATA: STBC 0x0097ed78
NiTStringMap<CameraMode::CreateFunction> CameraMode::creators;

// FUNCTION: STBC 0x0041dfd0 _$E557
// FUNCTION: STBC 0x0041dfe0 _$E556
static CameraModeRegistration chaseRegistration( "Chase", ChaseCameraMode::Create );
// FUNCTION: STBC 0x0041e0a0 _$E560
// FUNCTION: STBC 0x0041e0b0 _$E559
static CameraModeRegistration targetRegistration( "Target", TargetCameraMode::Create );
// FUNCTION: STBC 0x0041e170 _$E563
// FUNCTION: STBC 0x0041e180 _$E562
static CameraModeRegistration zoomTargetRegistration( "ZoomTarget", ZoomTargetMode::Create );
// FUNCTION: STBC 0x0041e240 _$E566
// FUNCTION: STBC 0x0041e250 _$E565
static CameraModeRegistration mapRegistration( "Map", MapCameraMode::Create );
// FUNCTION: STBC 0x0041e310 _$E569
// FUNCTION: STBC 0x0041e320 _$E568
static CameraModeRegistration dropAndWatchRegistration( "DropAndWatch", DropAndWatchMode::Create );
// FUNCTION: STBC 0x0041e3e0 _$E572
// FUNCTION: STBC 0x0041e3f0 _$E571
static CameraModeRegistration placementWatchRegistration( "PlacementWatch", PlacementWatchMode::Create );
// FUNCTION: STBC 0x0041e4b0 _$E575
// FUNCTION: STBC 0x0041e4c0 _$E574
static CameraModeRegistration lockedRegistration( "Locked", LockedPositionMode::Create );
// FUNCTION: STBC 0x0041e580 _$E578
// FUNCTION: STBC 0x0041e590 _$E577
static CameraModeRegistration torpCamRegistration( "TorpCam", TorpCameraMode::Create );
// FUNCTION: STBC 0x0041e650 _$E581
// FUNCTION: STBC 0x0041e660 _$E580
static CameraModeRegistration placeByDirectionRegistration( "PlaceByDirection", PlaceByDirectionMode::Create );

// DATA: STBC 0x00888dc4
static const float PI = 3.14159265f;


// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0041d7d0 _$E88
// FUNCTION: STBC 0x0041d7e0 _$E87
// FUNCTION: STBC 0x0041d7f0 _$E245
// FUNCTION: STBC 0x0041d800 _$E244
// FUNCTION: STBC 0x0041d810 _$E99
// FUNCTION: STBC 0x0041d820 _$E98
// FUNCTION: STBC 0x0041d830 _$E103
// FUNCTION: STBC 0x0041d840 _$E102
// FUNCTION: STBC 0x0041d850 _$E107
// FUNCTION: STBC 0x0041d860 _$E106
// FUNCTION: STBC 0x0041d870 _$E111
// FUNCTION: STBC 0x0041d880 _$E110
// FUNCTION: STBC 0x0041d890 _$E226
// FUNCTION: STBC 0x0041d8a0 _$E225
// FUNCTION: STBC 0x00420120 _$E168
// FUNCTION: STBC 0x00420130 _$E167
// FUNCTION: STBC 0x00420480 _$E181
// FUNCTION: STBC 0x00420490 _$E180
// FUNCTION: STBC 0x004207b0 _$E231
// FUNCTION: STBC 0x004207c0 _$E230
// FUNCTION: STBC 0x004218f0 _$E250
// FUNCTION: STBC 0x00421900 _$E249
// FUNCTION: STBC 0x00421d80 _$E235
// FUNCTION: STBC 0x00421d90 _$E234
// FUNCTION: STBC 0x00422bd0 _$E254
// FUNCTION: STBC 0x00422be0 _$E253
// FUNCTION: STBC 0x004235c0 _$E261
// FUNCTION: STBC 0x004235d0 _$E260
// FUNCTION: STBC 0x004243f0 _$E346
// FUNCTION: STBC 0x00424400 _$E345
// FUNCTION: STBC 0x00424860 _$E350
// FUNCTION: STBC 0x00424870 _$E349
// FUNCTION: STBC 0x004249c0 _$E126
// FUNCTION: STBC 0x004249d0 _$E125
// FUNCTION: STBC 0x00425f50 _$E276
// FUNCTION: STBC 0x00425f60 _$E275
// FUNCTION: STBC 0x00425f70 _$E283
// FUNCTION: STBC 0x00425f80 _$E282
// FUNCTION: STBC 0x00426750 _$E287
// FUNCTION: STBC 0x00426760 _$E286
// FUNCTION: STBC 0x00426a00 _$E291
// FUNCTION: STBC 0x00426a10 _$E290
// FUNCTION: STBC 0x00426c60 _$E295
// FUNCTION: STBC 0x00426c70 _$E294
// FUNCTION: STBC 0x00427040 _$E301
// FUNCTION: STBC 0x00427050 _$E300
// FUNCTION: STBC 0x00427880 _$E336
// FUNCTION: STBC 0x00427890 _$E335
// FUNCTION: STBC 0x00427bb0 _$E354
// FUNCTION: STBC 0x00427bc0 _$E353
// FUNCTION: STBC 0x00427e30 _$E358
// FUNCTION: STBC 0x00427e40 _$E357
// FUNCTION: STBC 0x00428180 _$E362
// FUNCTION: STBC 0x00428190 _$E361
// FUNCTION: STBC 0x004281a0 _$E366
// FUNCTION: STBC 0x004281b0 _$E365
// FUNCTION: STBC 0x004281c0 _$E370
// FUNCTION: STBC 0x004281d0 _$E369
// FUNCTION: STBC 0x004281e0 _$E374
// FUNCTION: STBC 0x004281f0 _$E373
// FUNCTION: STBC 0x00428200 _$E378
// FUNCTION: STBC 0x00428210 _$E377
// FUNCTION: STBC 0x00428220 _$E382
// FUNCTION: STBC 0x00428230 _$E381
// FUNCTION: STBC 0x00428240 _$E386
// FUNCTION: STBC 0x00428250 _$E385
// FUNCTION: STBC 0x00428260 _$E131
// FUNCTION: STBC 0x00428270 _$E130
// FUNCTION: STBC 0x00428280 _$E390
// FUNCTION: STBC 0x00428290 _$E389
// FUNCTION: STBC 0x004282a0 _$E136
// FUNCTION: STBC 0x004282b0 _$E135
// FUNCTION: STBC 0x004282c0 _$E394
// FUNCTION: STBC 0x004282d0 _$E393
// FUNCTION: STBC 0x004282e0 _$E398
// FUNCTION: STBC 0x004282f0 _$E397
// FUNCTION: STBC 0x00428300 _$E402
// FUNCTION: STBC 0x00428310 _$E401
// FUNCTION: STBC 0x00428320 _$E406
// FUNCTION: STBC 0x00428330 _$E405
// FUNCTION: STBC 0x00428340 _$E145
// FUNCTION: STBC 0x00428350 _$E144
// FUNCTION: STBC 0x00428360 _$E410
// FUNCTION: STBC 0x00428370 _$E409
// FUNCTION: STBC 0x00428380 _$E414
// FUNCTION: STBC 0x00428390 _$E413
// FUNCTION: STBC 0x004283a0 _$E418
// FUNCTION: STBC 0x004283b0 _$E417
// FUNCTION: STBC 0x004283c0 _$E151
// FUNCTION: STBC 0x004283d0 _$E150
// FUNCTION: STBC 0x004283e0 _$E156
// FUNCTION: STBC 0x004283f0 _$E155
// FUNCTION: STBC 0x00428400 _$E186
// FUNCTION: STBC 0x00428410 _$E185
// FUNCTION: STBC 0x00428420 _$E422
// FUNCTION: STBC 0x00428430 _$E421
// FUNCTION: STBC 0x00428440 _$E191
// FUNCTION: STBC 0x00428450 _$E190
// FUNCTION: STBC 0x00428460 _$E426
// FUNCTION: STBC 0x00428470 _$E425
// FUNCTION: STBC 0x00428480 _$E430
// FUNCTION: STBC 0x00428490 _$E429
// FUNCTION: STBC 0x004284a0 _$E196
// FUNCTION: STBC 0x004284b0 _$E195
// FUNCTION: STBC 0x004284c0 _$E434
// FUNCTION: STBC 0x004284d0 _$E433
// FUNCTION: STBC 0x004284e0 _$E161
// FUNCTION: STBC 0x004284f0 _$E160
// FUNCTION: STBC 0x00428500 _$E438
// FUNCTION: STBC 0x00428510 _$E437
// FUNCTION: STBC 0x00428520 _$E442
// FUNCTION: STBC 0x00428530 _$E441
// FUNCTION: STBC 0x00428540 _$E201
// FUNCTION: STBC 0x00428550 _$E200
// FUNCTION: STBC 0x00428560 _$E206
// FUNCTION: STBC 0x00428570 _$E205
// FUNCTION: STBC 0x00428580 _$E446
// FUNCTION: STBC 0x00428590 _$E445
// FUNCTION: STBC 0x004285a0 _$E450
// FUNCTION: STBC 0x004285b0 _$E449
// FUNCTION: STBC 0x004285c0 _$E454
// FUNCTION: STBC 0x004285d0 _$E453
// FUNCTION: STBC 0x004285e0 _$E458
// FUNCTION: STBC 0x004285f0 _$E457
// FUNCTION: STBC 0x00428600 _$E462
// FUNCTION: STBC 0x00428610 _$E461
// FUNCTION: STBC 0x00428620 _$E466
// FUNCTION: STBC 0x00428630 _$E465
// FUNCTION: STBC 0x00428680 _$E470
// FUNCTION: STBC 0x00428690 _$E469
// FUNCTION: STBC 0x004286a0 _$E474
// FUNCTION: STBC 0x004286b0 _$E473
// FUNCTION: STBC 0x004286c0 _$E211
// FUNCTION: STBC 0x004286d0 _$E210
// FUNCTION: STBC 0x004286e0 _$E478
// FUNCTION: STBC 0x004286f0 _$E477
// FUNCTION: STBC 0x00428700 _$E216
// FUNCTION: STBC 0x00428710 _$E215
// FUNCTION: STBC 0x00428720 _$E221
// FUNCTION: STBC 0x00428730 _$E220
// FUNCTION: STBC 0x004295b0 _$E482
// FUNCTION: STBC 0x004295c0 _$E481
// FUNCTION: STBC 0x004295d0 _$E487
// FUNCTION: STBC 0x004295e0 _$E486
// FUNCTION: STBC 0x004295f0 _$E491
// FUNCTION: STBC 0x00429600 _$E490

// DATA: STBC 0x00888dcc
const float CameraLagTracker::MAX_REDUNDANT_INTERVAL = 0.25f;
// DATA: STBC 0x00888dd0
const float CameraLagTracker::MAX_REDUNDANT_DISTANCE_SQUARED = 0.0f;
// DATA: STBC 0x00888dd4
const float CameraLagTracker::MIN_REDUNDANT_ROTATION_DOT = 1.01f;

/*
================
PlaceByDirectionMode::Cast

The mode as a place-by-direction mode; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0041da30 ?Cast@PlaceByDirectionMode@@SAPAV1@PAVCameraMode@@@Z
PlaceByDirectionMode *PlaceByDirectionMode::Cast( CameraMode *mode ) {
	if ( mode != NULL && mode->IsTypeOf( PLACEBYDIRECTIONMODE_TYPE ) ) {
		return (PlaceByDirectionMode *)mode;
	}
	return NULL;
}

/*
================
ChaseCameraMode::Cast

The mode as a chase mode; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0041d8b0 ?Cast@ChaseCameraMode@@SAPAV1@PAVCameraMode@@@Z
ChaseCameraMode *ChaseCameraMode::Cast( CameraMode *mode ) {
	if ( mode != NULL && mode->IsTypeOf( CHASECAMERAMODE_TYPE ) ) {
		return (ChaseCameraMode *)mode;
	}
	return NULL;
}

/*
================
TargetCameraMode::Cast

The mode as a target mode; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0041d8e0 ?Cast@TargetCameraMode@@SAPAV1@PAVCameraMode@@@Z
TargetCameraMode *TargetCameraMode::Cast( CameraMode *mode ) {
	if ( mode != NULL && mode->IsTypeOf( TARGETCAMERAMODE_TYPE ) ) {
		return (TargetCameraMode *)mode;
	}
	return NULL;
}

/*
================
ZoomTargetMode::Cast

The mode as a zoom target mode; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0041d910 ?Cast@ZoomTargetMode@@SAPAV1@PAVCameraMode@@@Z
ZoomTargetMode *ZoomTargetMode::Cast( CameraMode *mode ) {
	if ( mode != NULL && mode->IsTypeOf( ZOOMTARGETMODE_TYPE ) ) {
		return (ZoomTargetMode *)mode;
	}
	return NULL;
}

/*
================
MapCameraMode::Cast

The mode as a map mode; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0041d940 ?Cast@MapCameraMode@@SAPAV1@PAVCameraMode@@@Z
MapCameraMode *MapCameraMode::Cast( CameraMode *mode ) {
	if ( mode != NULL && mode->IsTypeOf( MAPCAMERAMODE_TYPE ) ) {
		return (MapCameraMode *)mode;
	}
	return NULL;
}

/*
================
DropAndWatchMode::Cast

The mode as a drop-and-watch mode; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0041d970 ?Cast@DropAndWatchMode@@SAPAV1@PAVCameraMode@@@Z
DropAndWatchMode *DropAndWatchMode::Cast( CameraMode *mode ) {
	if ( mode != NULL && mode->IsTypeOf( DROPANDWATCHMODE_TYPE ) ) {
		return (DropAndWatchMode *)mode;
	}
	return NULL;
}

/*
================
PlacementWatchMode::Cast

The mode as a placement watch mode; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0041d9a0 ?Cast@PlacementWatchMode@@SAPAV1@PAVCameraMode@@@Z
PlacementWatchMode *PlacementWatchMode::Cast( CameraMode *mode ) {
	if ( mode != NULL && mode->IsTypeOf( PLACEMENTWATCHMODE_TYPE ) ) {
		return (PlacementWatchMode *)mode;
	}
	return NULL;
}

/*
================
LockedPositionMode::Cast

The mode as a locked position mode; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0041d9d0 ?Cast@LockedPositionMode@@SAPAV1@PAVCameraMode@@@Z
LockedPositionMode *LockedPositionMode::Cast( CameraMode *mode ) {
	if ( mode != NULL && mode->IsTypeOf( LOCKEDPOSITIONMODE_TYPE ) ) {
		return (LockedPositionMode *)mode;
	}
	return NULL;
}

/*
================
TorpCameraMode::Cast

The mode as a torpedo mode; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0041da00 ?Cast@TorpCameraMode@@SAPAV1@PAVCameraMode@@@Z
TorpCameraMode *TorpCameraMode::Cast( CameraMode *mode ) {
	if ( mode != NULL && mode->IsTypeOf( TORPCAMERAMODE_TYPE ) ) {
		return (TorpCameraMode *)mode;
	}
	return NULL;
}

/*
================
ChaseCameraMode::Cast

The object as a chase mode; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0041da60 ?Cast@ChaseCameraMode@@SAPAV1@PAVTGObject@@@Z
ChaseCameraMode *ChaseCameraMode::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( CHASECAMERAMODE_TYPE ) ) {
		return (ChaseCameraMode *)object;
	}
	return NULL;
}

/*
================
TargetCameraMode::Cast

The object as a target mode; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0041da90 ?Cast@TargetCameraMode@@SAPAV1@PAVTGObject@@@Z
TargetCameraMode *TargetCameraMode::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TARGETCAMERAMODE_TYPE ) ) {
		return (TargetCameraMode *)object;
	}
	return NULL;
}

/*
================
ZoomTargetMode::Cast

The object as a zoom target mode; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0041dac0 ?Cast@ZoomTargetMode@@SAPAV1@PAVTGObject@@@Z
ZoomTargetMode *ZoomTargetMode::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( ZOOMTARGETMODE_TYPE ) ) {
		return (ZoomTargetMode *)object;
	}
	return NULL;
}

/*
================
MapCameraMode::Cast

The object as a map mode; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0041daf0 ?Cast@MapCameraMode@@SAPAV1@PAVTGObject@@@Z
MapCameraMode *MapCameraMode::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( MAPCAMERAMODE_TYPE ) ) {
		return (MapCameraMode *)object;
	}
	return NULL;
}

/*
================
DropAndWatchMode::Cast

The object as a drop-and-watch mode; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0041db20 ?Cast@DropAndWatchMode@@SAPAV1@PAVTGObject@@@Z
DropAndWatchMode *DropAndWatchMode::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( DROPANDWATCHMODE_TYPE ) ) {
		return (DropAndWatchMode *)object;
	}
	return NULL;
}

/*
================
PlacementWatchMode::Cast

The object as a placement watch mode; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0041db50 ?Cast@PlacementWatchMode@@SAPAV1@PAVTGObject@@@Z
PlacementWatchMode *PlacementWatchMode::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( PLACEMENTWATCHMODE_TYPE ) ) {
		return (PlacementWatchMode *)object;
	}
	return NULL;
}

/*
================
LockedPositionMode::Cast

The object as a locked position mode; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0041db80 ?Cast@LockedPositionMode@@SAPAV1@PAVTGObject@@@Z
LockedPositionMode *LockedPositionMode::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( LOCKEDPOSITIONMODE_TYPE ) ) {
		return (LockedPositionMode *)object;
	}
	return NULL;
}

/*
================
TorpCameraMode::Cast

The object as a torpedo mode; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0041dbb0 ?Cast@TorpCameraMode@@SAPAV1@PAVTGObject@@@Z
TorpCameraMode *TorpCameraMode::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TORPCAMERAMODE_TYPE ) ) {
		return (TorpCameraMode *)object;
	}
	return NULL;
}

/*
================
PlaceByDirectionMode::Cast

The object as a place-by-direction mode; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0041dbe0 ?Cast@PlaceByDirectionMode@@SAPAV1@PAVTGObject@@@Z
PlaceByDirectionMode *PlaceByDirectionMode::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( PLACEBYDIRECTIONMODE_TYPE ) ) {
		return (PlaceByDirectionMode *)object;
	}
	return NULL;
}

/*
================
NebulaClearDistance

The distance from the target, at least the one given, to the furthest
nebula entry on the line from the source to the target in the target's
set. Invented name.
================
*/
// FUNCTION: STBC 0x0041e720
float NebulaClearDistance( ObjectClass *source, ObjectClass *target, float distance ) {
	SetClass *set = target->GetContainingSet();
	if ( set != NULL ) {
		NiPoint3 start = source->GetWorldLocation();
		NiPoint3 end = target->GetWorldLocation();
		int count;
		Nebula **nebulas = Nebula::GetClassObjects( set, &count );
		for ( int i = 0; i < count; i++ ) {
			NiPoint3 entry;
			if ( nebulas[i]->VirtualSlot72( &start, &end, &entry ) ) {
				float entryDistance = ( entry - end ).Length();
				if ( entryDistance > distance ) {
					distance = entryDistance;
				}
			}
		}
	}
	return distance;
}

/*
================
CameraMode::CameraMode

A mode of the camera with no attributes but an empty "Target".
================
*/
// FUNCTION: STBC 0x0041e810
CameraMode::CameraMode( CameraObjectClass *camera_ ) : lagTrackers( 2 ) {
	camera = camera_;
	referenceCount = 0;
	unknown34 = false;
	name = NULL;
	SetAttrID( "Target", 0 );
}

/*
================
CameraMode::GetObjType
================
*/
// FUNCTION: STBC 0x0041e960
int CameraMode::GetObjType( void ) {
	return CAMERAMODE_TYPE;
}

/*
================
CameraMode::GetClassName
================
*/
// FUNCTION: STBC 0x0041e9b0
char *CameraMode::GetClassName( void ) {
	return "CameraMode";
}

/*
================
CameraMode::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x0041e9c0
char *CameraMode::GetSwigTypeName( void ) {
	return "_p_CameraMode";
}

/*
================
CameraMode::GetPythonClassName
================
*/
// FUNCTION: STBC 0x0041e9d0
char *CameraMode::GetPythonClassName( void ) {
	return "CameraModePtr";
}

/*
================
CameraMode::GetClassHandlers
================
*/
// FUNCTION: STBC 0x0041e9e0
TGEventHandlerTable *CameraMode::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
CameraMode::SnapToIdealPosition

A plain mode has no ideal position.
================
*/
// FUNCTION: STBC 0x0041eab0
void CameraMode::SnapToIdealPosition( void ) {
}

/*
================
CameraMode::Reset
================
*/
// FUNCTION: STBC 0x0041eac0
void CameraMode::Reset( void ) {
}

/*
================
CameraMode::IsValid
================
*/
// FUNCTION: STBC 0x0041ead0
bool CameraMode::IsValid( void ) {
	return true;
}

/*
================
CameraMode::Update
================
*/
// FUNCTION: STBC 0x0041eae0
void CameraMode::Update( void ) {
}

/*
================
CameraMode::VirtualSlot29

The camera left the mode.
================
*/
// FUNCTION: STBC 0x0041eb00
void CameraMode::VirtualSlot29( void ) {
	unknown34 = false;
}

/*
================
CameraMode::VirtualSlot30
================
*/
// FUNCTION: STBC 0x0041eb10
void CameraMode::VirtualSlot30( BezierCurve *sweep ) {
}

/*
================
CameraMode::VirtualSlot31
================
*/
// FUNCTION: STBC 0x0041eb20
void CameraMode::VirtualSlot31( NiPoint3 *forward, NiPoint3 *up ) {
}

/*
================
CameraMode::Zoom
================
*/
// FUNCTION: STBC 0x0041eb30
void CameraMode::Zoom( float zoom ) {
}

/*
================
CameraMode::~CameraMode

Deletes the lag records the mode still holds.
================
*/
// FUNCTION: STBC 0x0041eb70
CameraMode::~CameraMode( void ) {
	NiTListIterator pos = lagTrackers.GetHeadPos();
	while ( pos != NULL ) {
		CameraLagTracker *tracker = lagTrackers.GetNext( pos );
		if ( tracker != NULL ) {
			lagTrackers.Remove( tracker );
			delete tracker;
		}
	}
	if ( name != NULL ) {
		delete [] name;
	}
}

/*
================
CameraMode::Create

Makes a mode of the named class for the camera; NULL when no function is
registered for the name.
================
*/
// FUNCTION: STBC 0x0041ed80 ?Create@CameraMode@@SAPAV1@PADPAVCameraObjectClass@@@Z
CameraMode *CameraMode::Create( char *className, CameraObjectClass *camera ) {
	CreateFunction create;
	if ( creators.GetAt( className, create ) && create != NULL ) {
		return create( camera );
	}
	return NULL;
}

/*
================
CameraMode::SetName

Keeps a copy of the name the camera files the mode under.
================
*/
// FUNCTION: STBC 0x0041ede0 ?SetName@CameraMode@@QAEXPAD@Z
void CameraMode::SetName( char *name_ ) {
	if ( name_ != name ) {
		delete [] name;
		name = NULL;
		if ( name_ != NULL ) {
			name = new char[strlen( name_ ) + 1];
			strcpy( name, name_ );
		}
	}
}

/*
================
CameraMode::VirtualSlot32

Turns the camera by the rotation, given in world space, and keeps its
local rotation orthogonal.
================
*/
// FUNCTION: STBC 0x0041ee50
void CameraMode::VirtualSlot32( const NiMatrix3 &rotation ) {
	NiCamera *niCamera = camera->GetNiCamera();
	NiMatrix3 localRotation = niCamera->GetWorldRotate().Transpose() * rotation * niCamera->GetWorldRotate();
	niCamera->SetRotate( niCamera->GetRotate() * localRotation );
	NiMatrix3 rotate = niCamera->GetRotate();
	rotate.Reorthogonalize();
	niCamera->SetRotate( rotate );
}

/*
================
CameraMode::SetAttrID

Sets an object attribute, and follows the object: the mode keeps its lag
record and hears when it becomes a hulk.
================
*/
// FUNCTION: STBC 0x0041eee0
void CameraMode::SetAttrID( char *attr, long id ) {
	long oldID;
	if ( idAttrs.GetAt( attr, oldID ) ) {
		if ( oldID == id ) {
			return;
		}
		StopLagTracking( oldID );
		ObjectClass *oldObject = ObjectClass::GetObjectByID( NULL, oldID );
		if ( oldObject != NULL ) {
			g_kEventManager.RemoveBroadcastHandler( ET_OBJECT_CONVERTED_TO_HULK, this, "CameraMode::ObjectChangedToHulk", oldObject );
		}
	}
	idAttrs.SetAt( attr, id );
	ObjectClass *object = ObjectClass::GetObjectByID( NULL, id );
	if ( object != NULL ) {
		StartLagTracking( object );
		g_kEventManager.AddBroadcastMethodHandler( ET_OBJECT_CONVERTED_TO_HULK, this, "CameraMode::ObjectChangedToHulk", object );
	}
}

/*
================
CameraMode::GetAttrIDObject

The object the attribute's ID names in the camera's set: an ordinary object,
else a placement.
================
*/
// FUNCTION: STBC 0x0041f040 ?GetAttrIDObject@CameraMode@@QAEPAVObjectClass@@PAD@Z
ObjectClass *CameraMode::GetAttrIDObject( char *attr ) {
	long id;
	if ( idAttrs.GetAt( attr, id ) && id != 0 ) {
		SetClass *set = camera->GetContainingSet();
		if ( set != NULL ) {
			ObjectClass *object = ObjectClass::GetObjectByID( set, id );
			if ( object == NULL ) {
				object = PlacementObject::GetObjectByID( set, id );
			}
			return object;
		}
	}
	return NULL;
}

/*
================
CameraMode::SetAttrIDObject

Sets the attribute to the object's ID, or to none.
================
*/
// FUNCTION: STBC 0x0041f020 ?SetAttrIDObject@CameraMode@@QAEXPADPAVObjectClass@@@Z
void CameraMode::SetAttrIDObject( char *attr, ObjectClass *object ) {
	long id = 0;
	if ( object != NULL ) {
		id = object->GetObjID();
	}
	SetAttrID( attr, id );
}

/*
================
CameraMode::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x0041f0b0
void CameraMode::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&CameraMode::ObjectChangedToHulk, "CameraMode::ObjectChangedToHulk" );
	TorpCameraMode::RegisterHandlerNames();
}

/*
================
CameraMode::RegisterHandlers

The modes have no class handlers of their own.
================
*/
// FUNCTION: STBC 0x0041f0d0
void CameraMode::RegisterHandlers( void ) {
	TorpCameraMode::RegisterHandlers();
}

/*
================
CameraMode::WriteToStream
================
*/
// FUNCTION: STBC 0x0041f0e0
bool CameraMode::WriteToStream( TGStream *stream ) {
	// skips TGEventHandlerObject's: the mode's own handlers are not saved
	if ( !TGObject::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteInt( referenceCount );
	stream->WriteBool( unknown34 );
	stream->WriteID( camera->GetObjID() );
	stream->VirtualSlot11( name );

	const char *attr;
	NiTMapIterator pos;

	stream->WriteInt( floatAttrs.GetCount() );
	pos = floatAttrs.GetFirstPos();
	while ( pos != NULL ) {
		float value;
		floatAttrs.GetNext( pos, attr, value );
		stream->VirtualSlot11( (char *)attr );
		stream->WriteFloat( value );
	}

	stream->WriteInt( idAttrs.GetCount() );
	pos = idAttrs.GetFirstPos();
	while ( pos != NULL ) {
		long id;
		idAttrs.GetNext( pos, attr, id );
		stream->VirtualSlot11( (char *)attr );
		stream->WriteID( id );
	}

	stream->WriteInt( pointAttrs.GetCount() );
	pos = pointAttrs.GetFirstPos();
	while ( pos != NULL ) {
		NiPoint3 point;
		pointAttrs.GetNext( pos, attr, point );
		stream->VirtualSlot11( (char *)attr );
		stream->WriteFloat( point.x );
		stream->WriteFloat( point.y );
		stream->WriteFloat( point.z );
	}

	stream->WriteInt( lagTrackers.GetSize() );
	NiTListIterator trackerPos = lagTrackers.GetHeadPos();
	while ( trackerPos != NULL ) {
		if ( !lagTrackers.GetNext( trackerPos )->WriteToStream( stream ) ) {
			return false;
		}
	}
	return true;
}

/*
================
CameraMode::ReadFromStream
================
*/
// FUNCTION: STBC 0x0041f350
// PARKED: register: the only difference is the base/index order of items[size - 1].next = 0 in NiTList::AddTail's inlined NiTListBlock constructor (shared NiTList.h)
bool CameraMode::ReadFromStream( TGStream *stream ) {
	int i;
	char *attr;
	float value;
	long id;
	NiPoint3 point;

	referenceCount = stream->ReadInt();
	unknown34 = stream->ReadBool();
	camera = (CameraObjectClass *)stream->ReadID();
	name = stream->VirtualSlot10( -1 );

	for ( i = stream->ReadInt() - 1; i >= 0; i-- ) {
		attr = stream->VirtualSlot10( -1 );
		value = stream->ReadFloat();
		floatAttrs.SetAt( attr, value );
		delete [] attr;
	}

	for ( i = stream->ReadInt() - 1; i >= 0; i-- ) {
		attr = stream->VirtualSlot10( -1 );
		id = stream->ReadID();
		idAttrs.SetAt( attr, id );
		delete [] attr;
	}

	for ( i = stream->ReadInt() - 1; i >= 0; i-- ) {
		attr = stream->VirtualSlot10( -1 );
		point.x = stream->ReadFloat();
		point.y = stream->ReadFloat();
		point.z = stream->ReadFloat();
		pointAttrs.SetAt( attr, point );
		delete [] attr;
	}

	for ( i = stream->ReadInt() - 1; i >= 0; i-- ) {
		CameraLagTracker *tracker = new CameraLagTracker;
		tracker->ReadFromStream( stream );
		lagTrackers.AddTail( tracker );
	}
	return true;
}

/*
================
CameraMode::FixUpPointers
================
*/
// FUNCTION: STBC 0x0041f6d0
bool CameraMode::FixUpPointers( void ) {
	return TGObject::GetTGObjectPtr( (long)camera ) ? true : false;
}

/*
================
CameraMode::PostFixUp
================
*/
// FUNCTION: STBC 0x0041f6f0
void CameraMode::PostFixUp( void ) {
	camera = (CameraObjectClass *)TGObject::GetTGObjectPtr( (long)camera );
}

/*
================
CameraMode::SaveModes

Writes every mode there is: the count, then each mode.
================
*/
// FUNCTION: STBC 0x0041f710
bool CameraMode::SaveModes( TGStream *stream ) {
	bool result = true;
	NiTList<TGObject *> modes( 2 );

	NiTMap<long, TGObject *> *objects = TGObject::objectTable;
	NiTMapIterator pos = objects->GetFirstPos();
	while ( pos != NULL ) {
		long id;
		TGObject *object;
		objects->GetNext( pos, id, object );
		if ( object->IsTypeOf( CAMERAMODE_TYPE ) ) {
			modes.AddTail( object );
		}
	}

	stream->WriteInt( modes.GetSize() );
	while ( !modes.IsEmpty() && result ) {
		result &= modes.RemoveHead()->WriteToStream( stream );
	}
	return result;
}

/*
================
CameraMode::LoadModes

Reads the modes SaveModes wrote; false when one is missing or no mode.
================
*/
// FUNCTION: STBC 0x0041f8d0
bool CameraMode::LoadModes( TGStream *stream ) {
	bool result = true;
	for ( int i = stream->ReadInt() - 1; i >= 0 && result; i-- ) {
		TGObject *object = TGObject::LoadObject( stream, 0 );
		if ( object == NULL || !object->IsTypeOf( CAMERAMODE_TYPE ) ) {
			result = false;
		}
	}
	return result;
}

/*
================
CameraMode::GetZoomedDistance

The distance scaled by the zoom (a zoom of 1 halves it), kept between the
limits.
================
*/
// FUNCTION: STBC 0x0041f920
float CameraMode::GetZoomedDistance( float distance, float minimum, float maximum, float zoom ) {
	float zoomed = ( 1.0f - zoom * 0.5f ) * distance;
	if ( zoomed < minimum ) {
		return minimum;
	}
	if ( zoomed > maximum ) {
		zoomed = maximum;
	}
	return zoomed;
}

/*
================
CameraMode::StartLagTracking

Counts one more watch of the object, starting its lag record with the
first.
================
*/
// FUNCTION: STBC 0x0041f960
// PARKED: 74.0, complete; register allocation: retail keeps this in ebp and the NULL constant in ebx, this build the reverse (declaration order, loop form, NULL test and a block-local tracker change nothing)
void CameraMode::StartLagTracking( ObjectClass *object ) {
	if ( object == NULL ) {
		return;
	}

	CameraLagTracker *tracker;
	NiTListIterator pos = lagTrackers.GetHeadPos();
	while ( pos != NULL ) {
		tracker = lagTrackers.GetNext( pos );
		if ( tracker->objectID == object->GetObjID() ) {
			tracker->referenceCount++;
			return;
		}
	}

	tracker = new CameraLagTracker( object, 1.0f );
	tracker->referenceCount++;
	tracker->AddSample();
	lagTrackers.AddTail( tracker );
}

/*
================
CameraMode::StopLagTracking

Counts one watch of the object less, dropping its lag record with the
last.
================
*/
// FUNCTION: STBC 0x0041fab0
void CameraMode::StopLagTracking( long id ) {
	NiTListIterator pos = lagTrackers.GetHeadPos();
	while ( pos != NULL ) {
		NiTListIterator trackerPos = pos;
		CameraLagTracker *tracker = lagTrackers.GetNext( pos );
		if ( tracker->objectID == id ) {
			if ( --tracker->referenceCount < 1 ) {
				lagTrackers.RemovePos( trackerPos );
				delete tracker;
			}
			return;
		}
	}
}

/*
================
CameraMode::UpdateLagTrackers

Records where each watched object is now.
================
*/
// FUNCTION: STBC 0x0041ff30
void CameraMode::UpdateLagTrackers( void ) {
	NiTListIterator pos = lagTrackers.GetHeadPos();
	while ( pos != NULL ) {
		lagTrackers.GetNext( pos )->AddSample();
	}
}

/*
================
CameraLagTracker::CameraLagTracker
================
*/
// FUNCTION: STBC 0x00420140
CameraLagTracker::CameraLagTracker( ObjectClass *object, float lagTime_ ) : samples( 2 ) {
	referenceCount = 0;
	objectID = object->GetObjID();
	lagTime = lagTime_;
}

/*
================
CameraLagTracker::~CameraLagTracker
================
*/
// FUNCTION: STBC 0x00420170
CameraLagTracker::~CameraLagTracker( void ) {
	while ( !samples.IsEmpty() ) {
		delete samples.RemoveHead();
	}
}

/*
================
CameraLagTracker::AddSample

Records where the object is now. The samples older than the lag time go,
but the last of them, which the oldest lagged placement still needs; so
do the samples the ones around them already give.
================
*/
// FUNCTION: STBC 0x004201f0
// PARKED: complete body; retail's removal loops fetch the dropped sample through a NULL-checked getter and delete it in each RemovePos branch, with RemoveHead/RemoveTail/DeleteItem out of line: the list-call spelling is not found yet
void CameraLagTracker::AddSample( void ) {
	ObjectClass *object = ObjectClass::GetObjectByID( NULL, objectID );
	if ( object == NULL ) {
		return;
	}

	CameraLagSample *sample = new CameraLagSample;
	sample->time = UtopiaApp::GetApp()->GetGameTime();
	sample->position = object->GetWorldLocation();
	sample->rotation.FromRotation( object->GetWorldRotation() );
	samples.AddTail( sample );

	float expireTime = sample->time - lagTime;
	NiTListIterator pos = samples.GetHeadPos();
	while ( pos != NULL && samples.Get( pos )->time < expireTime ) {
		NiTListIterator nextPos = samples.GetNextPos( pos );
		if ( nextPos == NULL || !( samples.Get( nextPos )->time < expireTime ) ) {
			break;
		}
		NiTListIterator oldPos = pos;
		CameraLagSample *old = samples.GetNext( pos );
		samples.RemovePos( oldPos );
		delete old;
	}

	CameraLagSample *previous = NULL;
	pos = samples.GetHeadPos();
	while ( pos != NULL ) {
		NiTListIterator currentPos = pos;
		CameraLagSample *current = samples.GetNext( pos );
		if ( previous != NULL && ( previous->time == current->time || ( pos != NULL && IsRedundant( previous, current, samples.Get( pos ) ) ) ) ) {
			NiTListIterator removePos = currentPos;
			CameraLagSample *removed = samples.GetNext( currentPos );
			samples.RemovePos( removePos );
			delete removed;
		} else {
			previous = current;
		}
	}
}

/*
================
CameraLagTracker::GetLaggedPosition

Where the object was lag seconds ago: between the two samples around that
time, else extrapolated back from the first two.
================
*/
// FUNCTION: STBC 0x004204a0
// PARKED: x87: the branches are retail's; retail keeps the second sample's position in memory (ours holds its x on the FPU), so its two return-first arms merge into one and the interpolation scales by the time difference on the stack
NiPoint3 CameraLagTracker::GetLaggedPosition( float lag ) {
	float time = UtopiaApp::GetApp()->GetGameTime() - lag;
	CameraLagSample *previous = NULL;
	NiTListIterator pos = samples.GetHeadPos();
	while ( pos != NULL ) {
		CameraLagSample *sample = samples.GetNext( pos );
		if ( previous != NULL && previous->time <= time && sample->time >= time ) {
			return InterpolatePosition( previous, sample, time );
		}
		previous = sample;
	}

	pos = samples.GetHeadPos();
	CameraLagSample *first = samples.GetNext( pos );
	if ( first != NULL ) {
		float firstTime = first->time;
		NiPoint3 firstPosition = first->position;
		CameraLagSample *second = samples.GetNext( pos );
		if ( second != NULL ) {
			float secondTime = second->time;
			NiPoint3 secondPosition = second->position;
			if ( secondTime <= firstTime ) {
				return firstPosition;
			}
			return ( firstPosition - secondPosition ) * ( firstTime - time ) / ( secondTime - firstTime ) + firstPosition;
		}
		return firstPosition;
	}
	return NiPoint3::ZERO;
}

/*
================
CameraLagTracker::GetLaggedRotation

How the object was turned lag seconds ago; the first sample's rotation
before the samples start.
================
*/
// FUNCTION: STBC 0x00420640
NiMatrix3 CameraLagTracker::GetLaggedRotation( float lag ) {
	float time = UtopiaApp::GetApp()->GetGameTime() - lag;
	NiMatrix3 rotation;
	CameraLagSample *previous = NULL;
	NiTListIterator pos = samples.GetHeadPos();
	while ( pos != NULL ) {
		CameraLagSample *sample = samples.GetNext( pos );
		if ( previous != NULL && previous->time <= time && sample->time >= time ) {
			NiQuaternion quaternion = InterpolateRotation( previous, sample, time );
			quaternion.ToRotation( rotation );
			return rotation;
		}
		previous = sample;
	}

	CameraLagSample *first = samples.GetHead();
	if ( first != NULL ) {
		first->rotation.ToRotation( rotation );
		return rotation;
	}
	return NiMatrix3::ZERO;
}

/*
================
CameraLagTracker::WriteToStream
================
*/
// FUNCTION: STBC 0x004206f0
bool CameraLagTracker::WriteToStream( TGStream *stream ) {
	stream->WriteInt( referenceCount );
	stream->WriteID( objectID );
	stream->WriteFloat( lagTime );
	stream->WriteInt( samples.GetSize() );
	NiTListIterator pos = samples.GetHeadPos();
	while ( pos != NULL ) {
		CameraLagSample *sample = samples.GetNext( pos );
		stream->WriteFloat( sample->time );
		stream->WriteFloat( sample->position.x );
		stream->WriteFloat( sample->position.y );
		stream->WriteFloat( sample->position.z );
		stream->WriteFloat( sample->rotation.GetW() );
		stream->WriteFloat( sample->rotation.GetX() );
		stream->WriteFloat( sample->rotation.GetY() );
		stream->WriteFloat( sample->rotation.GetZ() );
	}
	return true;
}

/*
================
CameraLagTracker::ReadFromStream
================
*/
// FUNCTION: STBC 0x004207d0
bool CameraLagTracker::ReadFromStream( TGStream *stream ) {
	referenceCount = stream->ReadInt();
	objectID = stream->ReadID();
	lagTime = stream->ReadFloat();
	for ( int i = stream->ReadInt() - 1; i >= 0; i-- ) {
		CameraLagSample *sample = new CameraLagSample;
		sample->time = stream->ReadFloat();
		sample->position.x = stream->ReadFloat();
		sample->position.y = stream->ReadFloat();
		sample->position.z = stream->ReadFloat();
		sample->rotation.w = stream->ReadFloat();
		sample->rotation.x = stream->ReadFloat();
		sample->rotation.y = stream->ReadFloat();
		sample->rotation.z = stream->ReadFloat();
		samples.AddTail( sample );
	}
	return true;
}

/*
================
CameraLagTracker::IsRedundant

Whether the sample between the other two adds nothing they give already.
The limits keep nearly every sample: the position must come out exact, and
no two rotations have a dot product of 1.01.
================
*/
// FUNCTION: STBC 0x00420990
bool CameraLagTracker::IsRedundant( CameraLagSample *previous, CameraLagSample *sample, CameraLagSample *next ) {
	float t = ( sample->time - previous->time ) / ( next->time - previous->time );
	if ( t >= 1.0f || t <= 0.0f ) {
		return true;
	}
	if ( sample->time - previous->time > MAX_REDUNDANT_INTERVAL ) {
		return false;
	}
	NiPoint3 position = InterpolatePosition( previous, next, sample->time );
	if ( ( position - sample->position ).SqrLength() > MAX_REDUNDANT_DISTANCE_SQUARED ) {
		return false;
	}
	NiQuaternion rotation = InterpolateRotation( previous, next, sample->time );
	if ( NiQuaternion::Dot( rotation, sample->rotation ) < MIN_REDUNDANT_ROTATION_DOT ) {
		return false;
	}
	return true;
}

/*
================
CameraLagTracker::InterpolatePosition

The position at the time, along the line between the two samples.
================
*/
// FUNCTION: STBC 0x00420a90
NiPoint3 CameraLagTracker::InterpolatePosition( CameraLagSample *from, CameraLagSample *to, float time ) {
	if ( from->time == to->time ) {
		return from->position;
	}
	float t = ( time - from->time ) / ( to->time - from->time );
	NiPoint3 position = ( 1.0f - t ) * from->position + t * to->position;
	return position;
}

/*
================
CameraLagTracker::InterpolateRotation

The rotation at the time, the shorter way between the two samples.
================
*/
// FUNCTION: STBC 0x00420b40
NiQuaternion CameraLagTracker::InterpolateRotation( CameraLagSample *from, CameraLagSample *to, float time ) {
	if ( !( from->time < to->time ) ) {
		return from->rotation;
	}
	float t = ( time - from->time ) / ( to->time - from->time );
	NiQuaternion difference = from->rotation - to->rotation;
	NiQuaternion sum = from->rotation + to->rotation;
	if ( NiQuaternion::Dot( difference, difference ) < NiQuaternion::Dot( sum, sum ) ) {
		return NiQuaternion::Slerp( t, from->rotation, to->rotation );
	} else {
		return NiQuaternion::Slerp( t, from->rotation, -to->rotation );
	}
}

/*
================
CameraMode::ObjectChangedToHulk

A watched object became a hulk: the attributes that named it name the hulk.
================
*/
// FUNCTION: STBC 0x00420c00
void CameraMode::ObjectChangedToHulk( TGEvent *event ) {
	ObjectClass *hulk = ObjectClass::Cast( event->GetSource() );
	long oldID = ObjectClass::Cast( event->GetDestination() )->GetObjID();

	NiTMapIterator pos = idAttrs.GetFirstPos();
	while ( pos != NULL ) {
		const char *attr;
		long id;
		idAttrs.GetNext( pos, attr, id );
		if ( id == oldID ) {
			// SetAttrID replaces the entry: it gets a copy of the name
			char *attrCopy = new char[strlen( attr ) + 1];
			strcpy( attrCopy, attr );
			SetAttrID( attrCopy, hulk->GetObjID() );
			delete [] attrCopy;
		}
	}
}

/*
================
IdealControlledCameraMode::IdealControlledCameraMode
================
*/
// FUNCTION: STBC 0x00420d20
IdealControlledCameraMode::IdealControlledCameraMode( CameraObjectClass *camera_ ) : CameraMode( camera_ ) {
	SetAttrFloat( "SweepTime", 1.0f );
	SetAttrFloat( "PositionThreshold", 0.01f );
	SetAttrFloat( "DotThreshold", 0.98f );
	SetAttrFloat( "MaxUpAngleChange", -1.0f );
	unknown6C = false;
}

/*
================
IdealControlledCameraMode::GetObjType
================
*/
// FUNCTION: STBC 0x00420f90
int IdealControlledCameraMode::GetObjType( void ) {
	return IDEALCONTROLLEDCAMERAMODE_TYPE;
}

/*
================
IdealControlledCameraMode::GetClassName
================
*/
// FUNCTION: STBC 0x00420ff0
char *IdealControlledCameraMode::GetClassName( void ) {
	return "IdealControlledCameraMode";
}

/*
================
IdealControlledCameraMode::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00421000
char *IdealControlledCameraMode::GetSwigTypeName( void ) {
	return "_p_IdealControlledCameraMode";
}

/*
================
IdealControlledCameraMode::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00421010
char *IdealControlledCameraMode::GetPythonClassName( void ) {
	return "IdealControlledCameraModePtr";
}

/*
================
IdealControlledCameraMode::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00421020
TGEventHandlerTable *IdealControlledCameraMode::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
IdealControlledCameraMode::GetIdealPosition
================
*/
// FUNCTION: STBC 0x00421030
NiPoint3 IdealControlledCameraMode::GetIdealPosition( void ) {
	return NiPoint3::ZERO;
}

/*
================
IdealControlledCameraMode::GetIdealDirection
================
*/
// FUNCTION: STBC 0x00421060
void IdealControlledCameraMode::GetIdealDirection( NiPoint3 *forward, NiPoint3 *up ) {
}

/*
================
IdealControlledCameraMode::VirtualSlot28

The camera made the mode current: it moves toward the ideal position.
================
*/
// FUNCTION: STBC 0x004210b0
void IdealControlledCameraMode::VirtualSlot28( void ) {
	CameraMode::VirtualSlot28();
	unknown6C = false;
	Unknown421410();
}

/*
================
IdealControlledCameraMode::Update

While the camera sweeps between modes it only clears "PosBeforeSweep";
otherwise it snaps to the ideal position unless the mode moved it itself.
================
*/
// FUNCTION: STBC 0x004210c0
void IdealControlledCameraMode::Update( void ) {
	if ( camera->sweep == NULL ) {
		if ( !unknown6C || !Unknown421910() ) {
			SnapToIdealPosition();
		}
		unknown6C = true;
	} else {
		TGPoint3 position = NiPoint3::ZERO;
		SetAttrPoint( "PosBeforeSweep", &position );
	}
}

/*
================
IdealControlledCameraMode::VirtualSlot30

A sweep of four control points ends at this mode's ideal position and
starts where the mode swept from had the camera ("PosBeforeSweep"), else
at that mode's ideal position.
================
*/
// FUNCTION: STBC 0x00421160
void IdealControlledCameraMode::VirtualSlot30( BezierCurve *sweep ) {
	NiPoint3 position = GetIdealPosition();
	NiPoint3 *points = sweep->GetPoints();
	if ( sweep->GetCount() != 4 ) {
		return;
	}
	points[2] = position;
	points[3] = position;

	TGObject *object = camera->Unknown42BDF0();
	if ( object == NULL || !object->IsTypeOf( IDEALCONTROLLEDCAMERAMODE_TYPE ) ) {
		return;
	}
	IdealControlledCameraMode *from = (IdealControlledCameraMode *)object;
	position = from->GetAttrPoint( "PosBeforeSweep" );
	if ( position == NiPoint3::ZERO ) {
		position = from->GetIdealPosition();
	}
	points[0] = position;
	points[1] = position;
}

/*
================
IdealControlledCameraMode::VirtualSlot31

The ideal direction; the camera's own where the mode gives none.
================
*/
// FUNCTION: STBC 0x00421350
void IdealControlledCameraMode::VirtualSlot31( NiPoint3 *forward, NiPoint3 *up ) {
	GetIdealDirection( forward, up );
	if ( forward->SqrLength() < 0.5f ) {
		*forward = camera->GetWorldForward();
	}
	if ( up->SqrLength() < 0.5f ) {
		*up = camera->GetWorldUp();
	}
}

/*
================
IdealControlledCameraMode::Unknown421410

When the camera is further than "PositionThreshold" from the ideal
position, or turned past "DotThreshold" from the ideal direction, sweeps
it there over "SweepTime" (from the last mode's ideal position when that
was an ideal-controlled one), or snaps it there without a sweep time.
================
*/
// FUNCTION: STBC 0x00421410
// DRAFT: register picks in the sweep points' copies (the start point's x stays in the FPU in retail)
void IdealControlledCameraMode::Unknown421410( void ) {
	if ( !unknown34 ) {
		return;
	}

	NiPoint3 idealPosition = GetIdealPosition();
	NiPoint3 forward = camera->GetWorldForward();
	NiPoint3 up = camera->GetWorldUp();
	GetIdealDirection( &forward, &up );
	if ( forward * forward < 0.5f ) {
		forward = camera->GetWorldForward();
	}
	if ( up * up < 0.5f ) {
		up = camera->GetWorldUp();
	}

	NiPoint3 offset = idealPosition - camera->GetWorldLocation();
	float forwardDot = forward * camera->GetWorldForward();
	float upDot = up * camera->GetWorldUp();
	float positionThreshold = GetAttrFloat( "PositionThreshold" );
	float dotThreshold = GetAttrFloat( "DotThreshold" );
	if ( offset * offset > positionThreshold || forwardDot < dotThreshold || upDot < dotThreshold ) {
		float sweepTime = GetAttrFloat( "SweepTime" );
		if ( sweepTime <= 0.0f ) {
			SnapToIdealPosition();
			return;
		}

		NiPoint3 start = camera->GetWorldLocation();
		TGObject *lastMode = TGObject::GetTGObjectPtr( camera->lastModeID );
		if ( lastMode != NULL && lastMode->IsTypeOf( IDEALCONTROLLEDCAMERAMODE_TYPE ) ) {
			start = ( (IdealControlledCameraMode *)lastMode )->GetIdealPosition();
		}
		NiPoint3 points[4];
		points[0] = start;
		points[1] = start;
		points[2] = idealPosition;
		points[3] = idealPosition;
		camera->StartSweep( 4, points, sweepTime, false, camera->lastModeID );
		camera->StartTurn( &camera->GetWorldForward(), &camera->GetWorldUp(), &forward, &up, &camera->GetNiCamera()->GetWorldRotate(), 0.0f, sweepTime );
	}
}

/*
================
IdealControlledCameraMode::Unknown421910

When the ideal up turns from the camera's faster than "MaxUpAngleChange"
per second, places the camera at the ideal position facing the ideal
forward, with its up turned only that far toward the ideal one, and
answers true; false when the camera can face the ideal direction.
================
*/
// FUNCTION: STBC 0x00421910
// DRAFT: call set and frame size as retail's; the quaternion temporaries' slots are ordered differently and several products have their x87 operands swapped
bool IdealControlledCameraMode::Unknown421910( void ) {
	NiPoint3 forward = camera->GetWorldForward();
	NiPoint3 up = camera->GetWorldUp();
	GetIdealDirection( &forward, &up );
	if ( forward * forward < 0.5f || up * up < 0.5f ) {
		return false;
	}

	float upAngle = NiACos( up * camera->GetWorldUp() );
	float maxUpAngleChange = GetAttrFloat( "MaxUpAngleChange" );
	if ( maxUpAngleChange <= 0.0f ) {
		return false;
	}
	float frameStep = UtopiaApp::GetApp()->GetGameFrameStep();
	if ( frameStep < 0.001f ) {
		frameStep = 0.001f;
	}
	float upAngleChange = upAngle / frameStep;
	if ( upAngleChange <= maxUpAngleChange ) {
		return false;
	}

	NiCamera *niCamera = camera->GetNiCamera();
	NiQuaternion from;
	NiQuaternion to;
	NiPoint3 right = forward.UnitCross( camera->GetWorldUp() );
	NiPoint3 keptUp = right.UnitCross( forward );
	from.FromRotation( NiMatrix3( forward, keptUp, right ) );
	NiPoint3 side = forward.Cross( up );
	to.FromRotation( NiMatrix3( forward, up, side ) );
	float t = maxUpAngleChange / upAngleChange;

	NiQuaternion difference = from - to;
	NiQuaternion sum = from + to;
	NiMatrix3 rotation;
	if ( NiQuaternion::Dot( difference, difference ) < NiQuaternion::Dot( sum, sum ) ) {
		NiQuaternion::Slerp( t, from, to ).ToRotation( rotation );
	} else {
		NiQuaternion::Slerp( t, from, -to ).ToRotation( rotation );
	}
	niCamera->SetRotate( rotation );

	NiPoint3 position = GetIdealPosition();
	camera->SetTranslate( &position );
	camera->UpdateNodeOnly();
	return true;
}

/*
================
ChaseCameraMode::GetObjType
================
*/
// FUNCTION: STBC 0x00421ee0
int ChaseCameraMode::GetObjType( void ) {
	return CHASECAMERAMODE_TYPE;
}

/*
================
ChaseCameraMode::IsTypeOf
================
*/
// FUNCTION: STBC 0x00421ef0
bool ChaseCameraMode::IsTypeOf( int type ) {
	if ( type == CHASECAMERAMODE_TYPE ) {
		return true;
	}
	return IdealControlledCameraMode::IsTypeOf( type );
}

/*
================
ChaseCameraMode::GetClassName
================
*/
// FUNCTION: STBC 0x00421f50
char *ChaseCameraMode::GetClassName( void ) {
	return "ChaseCameraMode";
}

/*
================
ChaseCameraMode::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00421f60
char *ChaseCameraMode::GetSwigTypeName( void ) {
	return "_p_ChaseCameraMode";
}

/*
================
ChaseCameraMode::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00421f70
char *ChaseCameraMode::GetPythonClassName( void ) {
	return "ChaseCameraModePtr";
}

/*
================
ChaseCameraMode::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00421f80
TGEventHandlerTable *ChaseCameraMode::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
ChaseCameraMode::IsValid

The mode needs its "Target" object.
================
*/
// FUNCTION: STBC 0x00421f90
bool ChaseCameraMode::IsValid( void ) {
	return GetAttrIDObject( "Target" ) != NULL;
}

/*
================
ChaseCameraMode::Create
================
*/
// FUNCTION: STBC 0x00421ff0
CameraMode *ChaseCameraMode::Create( CameraObjectClass *camera_ ) {
	return new ChaseCameraMode( camera_ );
}

/*
================
TargetCameraMode::GetObjType
================
*/
// FUNCTION: STBC 0x00422aa0
int TargetCameraMode::GetObjType( void ) {
	return TARGETCAMERAMODE_TYPE;
}

/*
================
TargetCameraMode::IsTypeOf
================
*/
// FUNCTION: STBC 0x00422ab0
bool TargetCameraMode::IsTypeOf( int type ) {
	if ( type == TARGETCAMERAMODE_TYPE ) {
		return true;
	}
	return IdealControlledCameraMode::IsTypeOf( type );
}

/*
================
TargetCameraMode::GetClassName
================
*/
// FUNCTION: STBC 0x00422b10
char *TargetCameraMode::GetClassName( void ) {
	return "TargetCameraMode";
}

/*
================
TargetCameraMode::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00422b20
char *TargetCameraMode::GetSwigTypeName( void ) {
	return "_p_TargetCameraMode";
}

/*
================
TargetCameraMode::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00422b30
char *TargetCameraMode::GetPythonClassName( void ) {
	return "TargetCameraModePtr";
}

/*
================
TargetCameraMode::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00422b40
TGEventHandlerTable *TargetCameraMode::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
TargetCameraMode::IsValid

The mode needs both objects.
================
*/
// FUNCTION: STBC 0x00422b50
bool TargetCameraMode::IsValid( void ) {
	return GetAttrIDObject( "Source" ) != NULL && GetAttrIDObject( "Target" ) != NULL;
}

/*
================
TargetCameraMode::VirtualSlot32

The camera does not turn in this mode.
================
*/
// FUNCTION: STBC 0x00422b80
void TargetCameraMode::VirtualSlot32( const NiMatrix3 &rotation ) {
}

/*
================
TargetCameraMode::Create
================
*/
// FUNCTION: STBC 0x00422bf0
CameraMode *TargetCameraMode::Create( CameraObjectClass *camera_ ) {
	return new TargetCameraMode( camera_ );
}

/*
================
ZoomTargetMode::GetObjType
================
*/
// FUNCTION: STBC 0x00423bb0
int ZoomTargetMode::GetObjType( void ) {
	return ZOOMTARGETMODE_TYPE;
}

/*
================
ZoomTargetMode::IsTypeOf
================
*/
// FUNCTION: STBC 0x00423bc0
bool ZoomTargetMode::IsTypeOf( int type ) {
	if ( type == ZOOMTARGETMODE_TYPE ) {
		return true;
	}
	return IdealControlledCameraMode::IsTypeOf( type );
}

/*
================
ZoomTargetMode::GetClassName
================
*/
// FUNCTION: STBC 0x00423c20
char *ZoomTargetMode::GetClassName( void ) {
	return "ZoomTargetMode";
}

/*
================
ZoomTargetMode::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00423c30
char *ZoomTargetMode::GetSwigTypeName( void ) {
	return "_p_ZoomTargetMode";
}

/*
================
ZoomTargetMode::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00423c40
char *ZoomTargetMode::GetPythonClassName( void ) {
	return "ZoomTargetModePtr";
}

/*
================
ZoomTargetMode::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00423c50
TGEventHandlerTable *ZoomTargetMode::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
ZoomTargetMode::Create
================
*/
// FUNCTION: STBC 0x00423ca0
CameraMode *ZoomTargetMode::Create( CameraObjectClass *camera_ ) {
	return new ZoomTargetMode( camera_ );
}

/*
================
MapCameraMode::GetObjType
================
*/
// FUNCTION: STBC 0x004245f0
int MapCameraMode::GetObjType( void ) {
	return MAPCAMERAMODE_TYPE;
}

/*
================
MapCameraMode::IsTypeOf
================
*/
// FUNCTION: STBC 0x00424600
bool MapCameraMode::IsTypeOf( int type ) {
	if ( type == MAPCAMERAMODE_TYPE ) {
		return true;
	}
	return IdealControlledCameraMode::IsTypeOf( type );
}

/*
================
MapCameraMode::GetClassName
================
*/
// FUNCTION: STBC 0x00424660
char *MapCameraMode::GetClassName( void ) {
	return "MapCameraMode";
}

/*
================
MapCameraMode::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00424670
char *MapCameraMode::GetSwigTypeName( void ) {
	return "_p_MapCameraMode";
}

/*
================
MapCameraMode::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00424680
char *MapCameraMode::GetPythonClassName( void ) {
	return "MapCameraModePtr";
}

/*
================
MapCameraMode::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00424690
TGEventHandlerTable *MapCameraMode::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
MapCameraMode::IsValid

The mode needs its "Target" object.
================
*/
// FUNCTION: STBC 0x004246a0
bool MapCameraMode::IsValid( void ) {
	return GetAttrIDObject( "Target" ) != NULL;
}

/*
================
MapCameraMode::Create
================
*/
// FUNCTION: STBC 0x00424700
CameraMode *MapCameraMode::Create( CameraObjectClass *camera_ ) {
	return new MapCameraMode( camera_ );
}

/*
================
MapCameraMode::VirtualSlot28

The camera became the current one: the offset from the target to the camera
is kept as a direction, or the opposite of the target's forward when the
camera sits on it.
================
*/
// FUNCTION: STBC 0x00424730
void MapCameraMode::VirtualSlot28( void ) {
	ObjectClass *target = GetAttrIDObject( "Target" );
	if ( target != NULL ) {
		unknown70 = camera->GetWorldLocation() - target->GetWorldLocation();
		unknown70.Unitize();
		if ( unknown70.SqrLength() < 0.5f ) {
			unknown70 = -target->GetWorldForward();
		}
	}
	IdealControlledCameraMode::VirtualSlot28();
}

/*
================
MapCameraMode::VirtualSlot32

The position saved with the mode is turned by the rotation.
================
*/
// FUNCTION: STBC 0x00424880
void MapCameraMode::VirtualSlot32( const NiMatrix3 &rotation ) {
	unknown70 = rotation * unknown70;
}

/*
================
MapCameraMode::Zoom

The distance saved with the mode is scaled by the zoom and kept between the
mode's minimum and maximum distances, then stored back as its distance.
================
*/
// FUNCTION: STBC 0x004248B0
void MapCameraMode::Zoom( float zoom ) {
	float distance = -1.0f;
	floatAttrs.GetAt( "Distance", distance );
	float maximum = -1.0f;
	floatAttrs.GetAt( "MaximumDistance", maximum );
	float minimum = -1.0f;
	floatAttrs.GetAt( "MinimumDistance", minimum );
	SetAttrFloat( "Distance", GetZoomedDistance( distance, minimum, maximum, zoom ) );
}

/*
================
MapCameraMode::GetIdealPosition

The camera sits at the mode's saved offset from the target it watches, scaled
by the mode's distance; without a target the ideal position is where the camera
is now.
================
*/
// FUNCTION: STBC 0x004249E0
NiPoint3 MapCameraMode::GetIdealPosition( void ) {
	NiPoint3 position = camera->GetWorldLocation();
	ObjectClass *target = GetAttrIDObject( "Target" );
	if ( target != NULL ) {
		float distance = -1.0f;
		// The map holds the distance the offset is scaled by; a mode without one
		// scales it by the default of -1, turning the offset around.
		floatAttrs.GetAt( "Distance", distance );
		position = distance * unknown70 + target->GetWorldLocation();
	}
	return position;
}

/*
================
MapCameraMode::GetIdealDirection

The camera looks at the target it watches: forward is the unit vector from the
ideal position to the target, and up is the camera's own up made
perpendicular to that direction.  Without a target the two vectors are left
as they are.
================
*/
// FUNCTION: STBC 0x00424AE0
void MapCameraMode::GetIdealDirection( NiPoint3 *forward, NiPoint3 *up ) {
	ObjectClass *target = GetAttrIDObject( "Target" );
	if ( target != NULL ) {
		NiPoint3 position = GetIdealPosition();
		*forward = target->GetWorldLocation() - position;
		// A direction that collapses to a point leaves forward as zero.
		forward->Unitize();
		// The direction and the camera's up give the side of the view; a second
		// cross product brings the camera's up into the plane at right angles to
		// the direction it now looks along.
		NiPoint3 right = forward->Cross( camera->GetWorldUp() );
		*up = right.UnitCross( *forward );
	}
}

/*
================
DropAndWatchMode::DropAndWatchMode

The drop-and-watch attributes; the axis-avoidance angle is a quarter turn
of PI.
================
*/
// FUNCTION: STBC 0x00424d00
DropAndWatchMode::DropAndWatchMode( CameraObjectClass *camera_ ) : CameraMode( camera_ ) {
	SetAttrFloat( "AwayDistance", 0.0f );
	SetAttrFloat( "RotateSpeed", 0.0f );
	SetAttrFloat( "AnticipationTime", 2.5f );
	SetAttrFloat( "ForwardOffset", 0.5f );
	SetAttrFloat( "SideOffset", 3.0f );
	SetAttrFloat( "AwayDistanceFactor", 1.3f );
	SetAttrFloat( "AxisAvoidAngles", PI * 0.25f );
	SetAttrFloat( "SlowSpeedThreshold", 0.5f );
	SetAttrFloat( "SlowRotationThreshold", 0.1f );
	SetAttrFloat( "RotateSpeedAccel", 0.025f );
	SetAttrFloat( "MaxRotateSpeed", 0.2f );
}

/*
================
DropAndWatchMode::GetObjType
================
*/
// FUNCTION: STBC 0x00425060
int DropAndWatchMode::GetObjType( void ) {
	return DROPANDWATCHMODE_TYPE;
}

/*
================
DropAndWatchMode::IsTypeOf
================
*/
// FUNCTION: STBC 0x00425070
bool DropAndWatchMode::IsTypeOf( int type ) {
	if ( type == DROPANDWATCHMODE_TYPE ) {
		return true;
	}
	return CameraMode::IsTypeOf( type );
}

/*
================
DropAndWatchMode::GetClassName
================
*/
// FUNCTION: STBC 0x004250c0
char *DropAndWatchMode::GetClassName( void ) {
	return "DropAndWatchMode";
}

/*
================
DropAndWatchMode::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x004250d0
char *DropAndWatchMode::GetSwigTypeName( void ) {
	return "_p_DropAndWatchMode";
}

/*
================
DropAndWatchMode::GetPythonClassName
================
*/
// FUNCTION: STBC 0x004250e0
char *DropAndWatchMode::GetPythonClassName( void ) {
	return "DropAndWatchModePtr";
}

/*
================
DropAndWatchMode::GetClassHandlers
================
*/
// FUNCTION: STBC 0x004250f0
TGEventHandlerTable *DropAndWatchMode::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
DropAndWatchMode::IsValid

The mode needs its "Target" object.
================
*/
// FUNCTION: STBC 0x00425100
bool DropAndWatchMode::IsValid( void ) {
	return GetAttrIDObject( "Target" ) != NULL;
}

/*
================
DropAndWatchMode::VirtualSlot32

The camera does not turn in this mode.
================
*/
// FUNCTION: STBC 0x00425120
void DropAndWatchMode::VirtualSlot32( const NiMatrix3 &rotation ) {
}

/*
================
DropAndWatchMode::Create
================
*/
// FUNCTION: STBC 0x00425170
CameraMode *DropAndWatchMode::Create( CameraObjectClass *camera_ ) {
	return new DropAndWatchMode( camera_ );
}

// the most places Update tries for the camera before it takes the last
#define DROP_AND_WATCH_MAX_TRIES	8

/*
================
DropAndWatchMode::Update

Drops the camera near the "Target" object and watches it go by. While the
target is farther than its radius and nearer than the away distance, the
camera keeps its place, and turns about the target once the target is
slow, faster up to MaxRotateSpeed, slower again when it moves. Otherwise
the camera is dropped anew ahead of where the target will be after
AnticipationTime, at a random angle about its forward axis (inside the
range angles when all four are set, else away from the up and down axes),
with a clear line to it; the away distance becomes AwayDistanceFactor times
the drop's distance. The camera then looks at the target.
================
*/
// FUNCTION: STBC 0x004251c0
// DRAFT: inline budget: retail calls NiPoint3's constructor for the first and last target offsets and UnitCross at the end out of line, and expands the rotation offset this build calls; the non-ranged angle loops are placed after the ranged ones in retail
void DropAndWatchMode::Update( void ) {
	ObjectClass *target = GetAttrIDObject( "Target" );
	if ( target == NULL ) {
		return;
	}

	NiPoint3 toTarget = target->GetWorldLocation() - camera->GetWorldLocation();
	float distance = toTarget.Length();
	if ( distance >= target->GetRadius() && distance <= GetAttrFloat( "AwayDistance" ) ) {
		// the camera stays, and turns about a slow target
		float speed = 0.0f;
		float angularSpeed = 0.0f;
		PhysicsObjectClass *physics = PhysicsObjectClass::Cast( target );
		if ( physics != NULL ) {
			speed = physics->GetVelocity().Length();
			angularSpeed = physics->GetAngularVelocity().Length();
		}
		float frameStep = UtopiaApp::GetApp()->GetGameFrameStep();
		float rotateSpeed = GetAttrFloat( "RotateSpeed" );
		if ( speed < GetAttrFloat( "SlowSpeedThreshold" ) && angularSpeed < GetAttrFloat( "SlowRotationThreshold" ) ) {
			rotateSpeed = GetAttrFloat( "RotateSpeedAccel" ) * frameStep + rotateSpeed;
			float maxRotateSpeed = GetAttrFloat( "MaxRotateSpeed" );
			if ( rotateSpeed > maxRotateSpeed ) {
				rotateSpeed = maxRotateSpeed;
			}
		} else {
			rotateSpeed = rotateSpeed - GetAttrFloat( "RotateSpeedAccel" ) * frameStep;
			if ( rotateSpeed <= 0.0f ) {
				rotateSpeed = 0.0f;
			}
		}
		SetAttrFloat( "RotateSpeed", rotateSpeed );
		if ( rotateSpeed > 0.0f ) {
			toTarget = target->GetWorldLocation() - camera->GetWorldLocation();
			NiPoint3 axis = toTarget.UnitCross( target->GetWorldForward() );
			if ( axis == NiPoint3::ZERO ) {
				axis = target->GetWorldUp();
			}
			if ( axis.Dot( target->GetWorldUp() ) < 0.0f ) {
				axis *= -1.0f;
			}
			NiMatrix3 rotation;
			rotation.MakeRotation( rotateSpeed * frameStep, axis );
			NiPoint3 position = rotation * ( camera->GetWorldLocation() - target->GetWorldLocation() ) + target->GetWorldLocation();
			camera->SetTranslate( &position );
		}
	} else {
		// drop the camera anew, ahead of the target
		NiPoint3 position = target->GetWorldLocation();
		NiPoint3 forward = target->GetWorldForward();
		NiPoint3 right = target->GetWorldRight();
		NiPoint3 up = target->GetWorldUp();
		PhysicsObjectClass *physics = PhysicsObjectClass::Cast( target );
		if ( physics != NULL ) {
			NiPoint3 velocity;
			NiPoint3 acceleration;
			NiPoint3 angularAxis;
			float angularSpeed;
			NiPoint3 angularAcceleration;
			NiPoint3 unknownC0;
			float unknownE4;
			NiMatrix3 rotation;
			physics->Unknown5A08F0( &position, &velocity, &acceleration, &angularAxis, &angularSpeed, &angularAcceleration, &unknownC0, &unknownE4, &rotation );
			physics->Unknown5A09D0( GetAttrFloat( "AnticipationTime" ), &position, &velocity, &acceleration, &angularAxis, &angularSpeed, &angularAcceleration, &unknownC0, &unknownE4, &rotation );
			forward = rotation * MODEL_FORWARD;
			right = rotation * MODEL_RIGHT;
			up = rotation * MODEL_UP;
		}

		int tries = 0;
		NiPoint3 cameraPosition = NiPoint3::ZERO;
		SetClass *set;
		do {
			if ( tries >= DROP_AND_WATCH_MAX_TRIES ) {
				break;
			}
			float rightAngle = NiSymmetricRandom() * ( 2.0f * PI );
			float upAngle = NiSymmetricRandom() * ( 2.0f * PI );
			// BUG: read as degrees, but the constructor stores it in radians (PI * 0.25f)
			float avoidAngle = GetAttrFloat( "AxisAvoidAngles" ) * PI * ( 1.0f / 180.0f );
			float range1 = GetAttrFloat( "RangeAngle1" );
			float range2 = GetAttrFloat( "RangeAngle2" );
			float range3 = GetAttrFloat( "RangeAngle3" );
			float range4 = GetAttrFloat( "RangeAngle4" );
			bool ranged;
			if ( range1 == -1.0f || range2 == -1.0f || range3 == -1.0f || range4 == -1.0f ) {
				ranged = false;
			} else {
				ranged = true;
			}
			float minRightAngle = range1 * PI * ( 1.0f / 180.0f );
			float maxRightAngle = range2 * PI * ( 1.0f / 180.0f );
			float minUpAngle = range3 * PI * ( 1.0f / 180.0f );
			float maxUpAngle = range4 * PI * ( 1.0f / 180.0f );
			if ( ranged ) {
				do {
					rightAngle = NiSymmetricRandom() * ( 2.0f * PI );
				} while ( rightAngle < minRightAngle || rightAngle > maxRightAngle );
				do {
					upAngle = NiSymmetricRandom() * ( 2.0f * PI );
				} while ( upAngle < minUpAngle || upAngle > maxUpAngle );
			} else {
				while ( fabs( rightAngle - PI * 0.5f ) < avoidAngle || fabs( rightAngle - PI * 0.5f * 3.0f ) < avoidAngle ) {
					rightAngle = NiSymmetricRandom() * ( 2.0f * PI );
				}
				while ( fabs( upAngle - PI * 0.5f ) < avoidAngle || fabs( upAngle - PI * 0.5f * 3.0f ) < avoidAngle ) {
					upAngle = NiSymmetricRandom() * ( 2.0f * PI );
				}
			}

			NiCamera *niCamera = camera->GetNiCamera();
			float reach = target->GetRadius() * 4.0f;
			float sideOffset = ( reach + niCamera->GetViewFrustum().nearDistance ) / reach * GetAttrFloat( "SideOffset" );
			float radius = target->GetRadius();
			NiPoint3 side = ( right * (float)cos( rightAngle ) + up * (float)sin( upAngle ) ) * sideOffset;
			cameraPosition = position + ( forward * GetAttrFloat( "ForwardOffset" ) + side ) * radius;
			tries++;

			set = target->GetContainingSet();
			if ( set == NULL ) {
				break;
			}
			NiPoint3 toTargetFromCamera = target->GetWorldLocation() - cameraPosition;
			set->StartPick( &cameraPosition, &toTargetFromCamera, false );
		} while ( set->GetPickedObject() != target );
		camera->SetTranslate( &cameraPosition );
		SetAttrFloat( "AwayDistance", ( target->GetWorldLocation() - cameraPosition ).Length() * GetAttrFloat( "AwayDistanceFactor" ) );
		SetAttrFloat( "RotateSpeed", 0.0f );
	}

	// the camera looks at the target
	camera->UpdateNodeOnly();
	NiPoint3 direction = target->GetWorldLocation() - camera->GetWorldLocation();
	direction.Unitize();
	NiPoint3 right = direction.UnitCross( target->GetWorldUp() );
	NiPoint3 up = right.Cross( direction );
	camera->GetNiCamera()->SetRotate( NiMatrix3( direction, up, right ) );
}

/*
================
PlacementWatchMode::PlacementWatchMode

Watches the "Source" placement along its path.
================
*/
// FUNCTION: STBC 0x00425f90
PlacementWatchMode::PlacementWatchMode( CameraObjectClass *camera_ ) : CameraMode( camera_ ) {
	SetAttrID( "Source", 0 );
	SetAttrFloat( "PathSpeedScale", 0.2f );
	SetAttrFloat( "TimeAlongPath", 0.0f );
}

/*
================
PlacementWatchMode::GetObjType
================
*/
// FUNCTION: STBC 0x00426100
int PlacementWatchMode::GetObjType( void ) {
	return PLACEMENTWATCHMODE_TYPE;
}

/*
================
PlacementWatchMode::IsTypeOf
================
*/
// FUNCTION: STBC 0x00426110
bool PlacementWatchMode::IsTypeOf( int type ) {
	if ( type == PLACEMENTWATCHMODE_TYPE ) {
		return true;
	}
	return CameraMode::IsTypeOf( type );
}

/*
================
PlacementWatchMode::GetClassName
================
*/
// FUNCTION: STBC 0x00426160
char *PlacementWatchMode::GetClassName( void ) {
	return "PlacementWatchMode";
}

/*
================
PlacementWatchMode::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00426170
char *PlacementWatchMode::GetSwigTypeName( void ) {
	return "_p_PlacementWatchMode";
}

/*
================
PlacementWatchMode::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00426180
char *PlacementWatchMode::GetPythonClassName( void ) {
	return "PlacementWatchModePtr";
}

/*
================
PlacementWatchMode::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00426190
TGEventHandlerTable *PlacementWatchMode::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
PlacementWatchMode::IsValid

The mode needs its "Source" object.
================
*/
// FUNCTION: STBC 0x004261a0
bool PlacementWatchMode::IsValid( void ) {
	return GetAttrIDObject( "Source" ) != NULL;
}

/*
================
PlacementWatchMode::VirtualSlot32

The camera does not turn in this mode.
================
*/
// FUNCTION: STBC 0x004261c0
void PlacementWatchMode::VirtualSlot32( const NiMatrix3 &rotation ) {
}

/*
================
PlacementWatchMode::SetAttrID

A new source placement starts its path from the beginning.
================
*/
// FUNCTION: STBC 0x00426770
void PlacementWatchMode::SetAttrID( char *attr, long id ) {
	CameraMode::SetAttrID( attr, id );
	if ( strcmp( attr, "Source" ) == 0 ) {
		SetAttrFloat( "TimeAlongPath", 0.0f );
	}
}

/*
================
PlacementWatchMode::Create
================
*/
// FUNCTION: STBC 0x00426210
CameraMode *PlacementWatchMode::Create( CameraObjectClass *camera_ ) {
	return new PlacementWatchMode( camera_ );
}

/*
================
PlacementWatchMode::Update

The camera sits at the "Source" object (moving along its waypoint path
when it has one) and looks at "Target" plus "TargetOffsetWorld"; with no
target it takes the source's orientation.
================
*/
// FUNCTION: STBC 0x00426240
// PARKED: x87 operand order: retail adds the look-at's x as offset + location (y and z the other way), this build as y and z; the one-expression and swapped-operand spellings cost 30 points
void PlacementWatchMode::Update( void ) {
	ObjectClass *source = GetAttrIDObject( "Source" );
	if ( source == NULL ) {
		return;
	}

	NiPoint3 position = source->GetWorldLocation();
	NiPoint3 up = source->GetWorldUp();
	Waypoint *waypoint = Waypoint::Cast( source );
	if ( waypoint != NULL && waypoint->GetNext() != NULL ) {
		MoveAlongPath( waypoint, waypoint->GetNext(), &position, &up );
	}
	camera->SetTranslate( &position );

	ObjectClass *target = GetAttrIDObject( "Target" );
	if ( target != NULL ) {
		NiPoint3 lookAt = target->GetWorldLocation() + GetAttrPoint( "TargetOffsetWorld" );
		NiPoint3 forward = lookAt - position;
		forward.Unitize();
		NiPoint3 right = forward.UnitCross( up );
		NiPoint3 newUp = right.Cross( forward );
		NiMatrix3 rotation( forward, newUp, right );
		camera->GetNiCamera()->SetRotate( rotation );
	} else if ( GetAttrID( "Target" ) == 0 ) {
		NiMatrix3 rotation( source->GetWorldForward(), source->GetWorldUp(), source->GetWorldRight() );
		camera->GetNiCamera()->SetRotate( rotation );
	}
}

/*
================
PlacementWatchMode::MoveAlongPath

Moves "TimeAlongPath" on by the frame step times "PathSpeedScale", passing
on to the next waypoints (the new "Source") as it reaches them, and gives
the position on the path and the up turned between the two waypoints'.
At the path's end the position and up are the last waypoint's. Invented
name.
================
*/
// FUNCTION: STBC 0x00426540
void PlacementWatchMode::MoveAlongPath( Waypoint *waypoint, Waypoint *next, NiPoint3 *position, NiPoint3 *up ) {
	float time = GetAttrFloat( "TimeAlongPath" );
	while ( time >= 1.0f ) {
		SetAttrIDObject( "Source", next );
		time -= 1.0f;
		waypoint = next;
		next = next->GetNext();
		SetAttrFloat( "TimeAlongPath", time );
		if ( next == NULL ) {
			*position = waypoint->GetWorldLocation();
			*up = waypoint->GetWorldUp();
			return;
		}
	}

	*position = waypoint->GetPathPoint( time );
	float angle;
	NiPoint3 axis;
	GetTurnBetween( angle, axis, waypoint->GetWorldUp(), next->GetWorldUp() );
	NiMatrix3 turn;
	turn.MakeRotation( angle * time, axis );
	*up = turn * waypoint->GetWorldUp();
	float speedScale = GetAttrFloat( "PathSpeedScale" );
	SetAttrFloat( "TimeAlongPath", time + UtopiaApp::GetApp()->GetGameFrameStep() * speedScale );
}

/*
================
LockedPositionMode::GetObjType
================
*/
// FUNCTION: STBC 0x004268f0
int LockedPositionMode::GetObjType( void ) {
	return LOCKEDPOSITIONMODE_TYPE;
}

/*
================
LockedPositionMode::IsTypeOf
================
*/
// FUNCTION: STBC 0x00426900
bool LockedPositionMode::IsTypeOf( int type ) {
	if ( type == LOCKEDPOSITIONMODE_TYPE ) {
		return true;
	}
	return CameraMode::IsTypeOf( type );
}

/*
================
LockedPositionMode::GetClassName
================
*/
// FUNCTION: STBC 0x00426950
char *LockedPositionMode::GetClassName( void ) {
	return "LockedPositionMode";
}

/*
================
LockedPositionMode::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00426960
char *LockedPositionMode::GetSwigTypeName( void ) {
	return "_p_LockedPositionMode";
}

/*
================
LockedPositionMode::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00426970
char *LockedPositionMode::GetPythonClassName( void ) {
	return "LockedPositionModePtr";
}

/*
================
LockedPositionMode::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00426980
TGEventHandlerTable *LockedPositionMode::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
LockedPositionMode::IsValid

The mode needs its "Target" object.
================
*/
// FUNCTION: STBC 0x00426990
bool LockedPositionMode::IsValid( void ) {
	return GetAttrIDObject( "Target" ) != NULL;
}

/*
================
LockedPositionMode::VirtualSlot32

The camera does not turn in this mode.
================
*/
// FUNCTION: STBC 0x004269b0
void LockedPositionMode::VirtualSlot32( const NiMatrix3 &rotation ) {
}

/*
================
LockedPositionMode::Create
================
*/
// FUNCTION: STBC 0x00426a20
CameraMode *LockedPositionMode::Create( CameraObjectClass *camera_ ) {
	return new LockedPositionMode( camera_ );
}

/*
================
LockedPositionMode::Update

Holds the camera at "Position" in the target's frame, facing "Forward"
with "Up" in that frame.
================
*/
// FUNCTION: STBC 0x00426a50
void LockedPositionMode::Update( void ) {
	ObjectClass *target = GetAttrIDObject( "Target" );
	if ( target != NULL ) {
		NiPoint3 position = TGModelUtils::LocalToWorldPoint( target->GetNode(), GetAttrPoint( "Position" ) );
		NiPoint3 forward = GetAttrPoint( "Forward" );
		NiPoint3 up = GetAttrPoint( "Up" );
		NiPoint3 right = forward.Cross( up );
		NiMatrix3 rotation( forward, up, right );
		NiMatrix3 worldRotation = target->GetWorldRotation() * rotation;
		NiCamera *niCamera = camera->GetNiCamera();
		niCamera->SetTranslate( position );
		niCamera->SetRotate( worldRotation );
	}
}

/*
================
TorpCameraMode::TorpCameraMode

Follows no torpedo yet; listens for the torpedoes fired.
================
*/
// FUNCTION: STBC 0x00426c80
TorpCameraMode::TorpCameraMode( CameraObjectClass *camera_ ) : IdealControlledCameraMode( camera_ ) {
	CameraMode::SetAttrID( "MainTorp", 0 );
	SetAttrFloat( "DelayAfterTorpGone", 2.0f );
	SetAttrFloat( "TorpGoneTimeLeft", 0.0f );
	SetAttrPoint( "LastIdealPos", &TGPoint3( NiPoint3::ZERO ) );
	SetAttrPoint( "LastIdealFwd", &TGPoint3( NiPoint3::UNIT_Y ) );
	SetAttrPoint( "LastIdealUp", &TGPoint3( NiPoint3::UNIT_Z ) );
	SetAttrFloat( "CurrentDistance", 4.0f );
	SetAttrFloat( "StartDistance", 4.0f );
	SetAttrFloat( "LaterDistance", 8.0f );
	SetAttrFloat( "MoveDistanceTime", 6.0f );
	g_kEventManager.AddBroadcastMethodHandler( ET_TORPEDO_FIRED, this, "TorpCameraMode::TorpFired" );
}

/*
================
TorpCameraMode::GetObjType
================
*/
// FUNCTION: STBC 0x00426ea0
int TorpCameraMode::GetObjType( void ) {
	return TORPCAMERAMODE_TYPE;
}

/*
================
TorpCameraMode::IsTypeOf
================
*/
// FUNCTION: STBC 0x00426eb0
bool TorpCameraMode::IsTypeOf( int type ) {
	if ( type == TORPCAMERAMODE_TYPE ) {
		return true;
	}
	return IdealControlledCameraMode::IsTypeOf( type );
}

/*
================
TorpCameraMode::GetClassName
================
*/
// FUNCTION: STBC 0x00426f10
char *TorpCameraMode::GetClassName( void ) {
	return "TorpCameraMode";
}

/*
================
TorpCameraMode::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00426f20
char *TorpCameraMode::GetSwigTypeName( void ) {
	return "_p_TorpCameraMode";
}

/*
================
TorpCameraMode::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00426f30
char *TorpCameraMode::GetPythonClassName( void ) {
	return "TorpCameraModePtr";
}

/*
================
TorpCameraMode::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00426f40
TGEventHandlerTable *TorpCameraMode::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
TorpCameraMode::VirtualSlot32

The camera does not turn in this mode.
================
*/
// FUNCTION: STBC 0x00426f50
void TorpCameraMode::VirtualSlot32( const NiMatrix3 &rotation ) {
}

/*
================
TorpCameraMode::Create
================
*/
// FUNCTION: STBC 0x00426fa0
CameraMode *TorpCameraMode::Create( CameraObjectClass *camera_ ) {
	return new TorpCameraMode( camera_ );
}

/*
================
TorpCameraMode::IsValid

The mode needs its "Target" object, and its torpedo or the time it still
stays after the torpedo is gone.
================
*/
// FUNCTION: STBC 0x00426fd0
bool TorpCameraMode::IsValid( void ) {
	if ( GetAttrIDObject( "Target" ) != NULL ) {
		if ( GetAttrIDObject( "MainTorp" ) != NULL || GetAttrFloat( "TorpGoneTimeLeft" ) > 0.0f ) {
			return true;
		}
	}
	return false;
}

/*
================
TorpCameraMode::Update

While the torpedo exists the time to stay after it is gone is the full
delay; afterwards it counts down by the game's frame step.
================
*/
// FUNCTION: STBC 0x00427060
void TorpCameraMode::Update( void ) {
	IdealControlledCameraMode::Update();
	if ( GetAttrIDObject( "MainTorp" ) != NULL ) {
		SetAttrFloat( "TorpGoneTimeLeft", GetAttrFloat( "DelayAfterTorpGone" ) );
	} else {
		SetAttrFloat( "TorpGoneTimeLeft", GetAttrFloat( "TorpGoneTimeLeft" ) - UtopiaApp::GetApp()->GetGameFrameStep() );
	}
}

/*
================
TorpCameraMode::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00427180
void TorpCameraMode::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TorpCameraMode::TorpFired, "TorpCameraMode::TorpFired" );
}

/*
================
TorpCameraMode::RegisterHandlers

The torpedo mode has no class handlers.
================
*/
// FUNCTION: STBC 0x004271a0
void TorpCameraMode::RegisterHandlers( void ) {
}

/*
================
TorpCameraMode::TorpFired

The first torpedo fired at the mode's target, while no torpedo is followed
yet, becomes the one the camera follows.
================
*/
// FUNCTION: STBC 0x004271b0
void TorpCameraMode::TorpFired( TGEvent *event ) {
	if ( !unknown34 || camera->sweep != NULL || GetAttrIDObject( "MainTorp" ) == NULL ) {
		ObjectClass *torpedo = (ObjectClass *)event->GetSource();
		if ( torpedo != NULL ) {
			ObjectClass *target = GetAttrIDObject( "Target" );
			if ( target != NULL && ( (ShipSubsystem *)event->GetDestination() )->GetParentShip() == target ) {
				SetAttrIDObject( "MainTorp", torpedo );
				float delay = -1.0f;
				floatAttrs.GetAt( "DelayAfterTorpGone", delay );
				SetAttrFloat( "TorpGoneTimeLeft", delay );
				float distance = -1.0f;
				floatAttrs.GetAt( "StartDistance", distance );
				SetAttrFloat( "CurrentDistance", distance );
			}
		}
	}
}

/*
================
TorpCameraMode::GetIdealPosition

"CurrentDistance" behind the torpedo, on the line from its target (or
against its flight without one); the last position while there is no
torpedo.
================
*/
// FUNCTION: STBC 0x004272d0
NiPoint3 TorpCameraMode::GetIdealPosition( void ) {
	NiPoint3 offset = camera->GetWorldLocation();
	Torpedo *torpedo = Torpedo::Cast( GetAttrIDObject( "MainTorp" ) );
	if ( torpedo == NULL ) {
		return GetAttrPoint( "LastIdealPos" );
	}

	ObjectClass *target = ObjectClass::GetObjectByID( torpedo->GetContainingSet(), torpedo->GetTargetID() );
	if ( target != NULL ) {
		offset = torpedo->GetWorldLocation() - target->GetWorldLocation();
	} else {
		offset = -torpedo->GetVelocity();
	}
	offset.Unitize();
	if ( offset.SqrLength() < 0.5f ) {
		offset = -camera->GetWorldForward();
	}
	offset *= GetAttrFloat( "CurrentDistance" );
	offset += torpedo->GetWorldLocation();

	TGPoint3 lastIdealPos( offset );
	SetAttrPoint( "LastIdealPos", &lastIdealPos );
	return offset;
}

/*
================
TorpCameraMode::GetIdealDirection

Faces the torpedo from the ideal position, keeping the camera's up as
close as it can; the last direction while there is no torpedo.
================
*/
// FUNCTION: STBC 0x00427550
// PARKED: EH state: retail stores state -1 before the second TGPoint3 is built (its constructor 0x429650 treated as may-throw); this build treats the out-of-line copy as nothrow here, even with TorpCameraMode's constructor not calling it; retail's copy sits at 0x429650 after the unit's _$E stubs, ours right after its first caller
void TorpCameraMode::GetIdealDirection( NiPoint3 *forward, NiPoint3 *up ) {
	ObjectClass *torpedo = GetAttrIDObject( "MainTorp" );
	if ( torpedo == NULL || !torpedo->IsTypeOf( TORPEDO_TYPE ) ) {
		*forward = GetAttrPoint( "LastIdealFwd" );
		*up = GetAttrPoint( "LastIdealUp" );
		return;
	}

	*forward = torpedo->GetWorldLocation() - GetIdealPosition();
	forward->Unitize();
	if ( forward->SqrLength() < 0.5f ) {
		*forward = camera->GetWorldForward();
	}
	*up = camera->GetWorldUp();
	NiPoint3 right = forward->Cross( *up );
	*up = right.UnitCross( *forward );
	if ( up->SqrLength() < 0.5f ) {
		*up = camera->GetWorldRight();
	}

	{
		TGPoint3 lastIdealFwd( *forward );
		SetAttrPoint( "LastIdealFwd", &lastIdealFwd );
	}
	{
		TGPoint3 lastIdealUp( *up );
		SetAttrPoint( "LastIdealUp", &lastIdealUp );
	}
}

/*
================
IdealControlledCameraMode::SnapToIdealPosition

Puts a current mode's camera at once at the ideal position, facing the ideal
direction (the camera's own where the mode gives none), and ends any sweep
or turn.
================
*/
// FUNCTION: STBC 0x00421760
void IdealControlledCameraMode::SnapToIdealPosition( void ) {
	if ( unknown34 ) {
		camera->StopSweep();
		camera->StopTurn();

		NiPoint3 position = GetIdealPosition();
		camera->SetTranslate( &position );
		camera->UpdateNodeOnly();

		NiPoint3 forward = camera->GetWorldForward();
		NiPoint3 up = camera->GetWorldUp();
		GetIdealDirection( &forward, &up );
		if ( forward.SqrLength() < 0.5f ) {
			forward = camera->GetWorldForward();
		}
		if ( up.SqrLength() < 0.5f ) {
			up = camera->GetWorldUp();
		}
		NiPoint3 right = forward.Cross( up );
		NiMatrix3 rotation( forward, up, right );
		camera->GetNiCamera()->SetRotate( rotation );
	}
}

/*
================
IdealControlledCameraMode::WriteToStream
================
*/
// FUNCTION: STBC 0x00421da0
bool IdealControlledCameraMode::WriteToStream( TGStream *stream ) {
	if ( !CameraMode::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteBool( unknown6C );
	return true;
}

/*
================
IdealControlledCameraMode::ReadFromStream
================
*/
// FUNCTION: STBC 0x00421dd0
bool IdealControlledCameraMode::ReadFromStream( TGStream *stream ) {
	if ( !CameraMode::ReadFromStream( stream ) ) {
		return false;
	}
	unknown6C = stream->ReadBool();
	return true;
}

/*
================
ChaseCameraMode::WriteToStream
================
*/
// FUNCTION: STBC 0x00422810
bool ChaseCameraMode::WriteToStream( TGStream *stream ) {
	if ( !IdealControlledCameraMode::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( unknown70.x );
	stream->WriteFloat( unknown70.y );
	stream->WriteFloat( unknown70.z );
	return true;
}

/*
================
ChaseCameraMode::ReadFromStream
================
*/
// FUNCTION: STBC 0x00422850
bool ChaseCameraMode::ReadFromStream( TGStream *stream ) {
	if ( !IdealControlledCameraMode::ReadFromStream( stream ) ) {
		return false;
	}
	unknown70.x = stream->ReadFloat();
	unknown70.y = stream->ReadFloat();
	unknown70.z = stream->ReadFloat();
	return true;
}

/*
================
TargetCameraMode::VirtualSlot28

The camera made the mode current: the sweep between the objects starts
over.
================
*/
// FUNCTION: STBC 0x00422df0
void TargetCameraMode::VirtualSlot28( void ) {
	IdealControlledCameraMode::VirtualSlot28();
	Reset();
}

/*
================
TargetCameraMode::Reset
================
*/
// FUNCTION: STBC 0x00422e10
void TargetCameraMode::Reset( void ) {
	SetAttrFloat( "BetweenTimeStart", -1.0f );
}

/*
================
TargetCameraMode::GetIdealPosition

Behind the source on the line from the target ("BackWatchPos") and above
it ("UpWatchPos"), pushed right ("RightPush") as that line nears the
source's up, at "Distance" source radii; the camera's own position while
either object is missing.
================
*/
// FUNCTION: STBC 0x00422e20
// DRAFT: inline budget: this build calls GetAttrFloat out of line once where retail expands it (42 calls each side)
NiPoint3 TargetCameraMode::GetIdealPosition( void ) {
	NiPoint3 position = camera->GetWorldLocation();
	ObjectClass *source = GetAttrIDObject( "Source" );
	ObjectClass *target = GetAttrIDObject( "Target" );
	if ( source != NULL && target != NULL ) {
		NiPoint3 lookAt = GetLaggedPosition( target, -1.0f ) + target->GetWorldRotation() * TGPoint3( GetTargetOffset() );
		NiPoint3 sourcePosition = GetLaggedPosition( source, -1.0f );
		NiPoint3 toTarget = lookAt - sourcePosition;
		toTarget.Unitize();
		if ( toTarget.SqrLength() < 0.5f ) {
			toTarget = source->GetWorldForward();
		}
		NiPoint3 side = toTarget.Cross( source->GetWorldUp() );
		side.Unitize();
		NiPoint3 up = side.Cross( toTarget );

		position = -( toTarget * GetAttrFloat( "BackWatchPos" ) );
		position += up * GetAttrFloat( "UpWatchPos" );

		float upDot = (float)fabs( toTarget * source->GetWorldUp() );
		float maxUpDot = GetAttrFloat( "MaxSourceUpDot" );
		if ( upDot > maxUpDot ) {
			float upAngle = NiACos( upDot );
			float maxUpAngle = NiACos( maxUpDot );
			float push = 0.5f - (float)cos( ( maxUpAngle - upAngle ) / maxUpAngle * PI ) * 0.5f;
			float rightPush = GetAttrFloat( "RightPush" );
			position += source->GetWorldRight() * rightPush * push;
			position -= GetAttrFloat( "UpWatchPos" ) * up * push;
		}
		position.Unitize();
		float radius = source->GetWorldBound().GetRadius();
		position *= GetAttrFloat( "Distance" ) * radius;
		position += GetLaggedPosition( source, -1.0f );
		KeepClearOfObjects( &position, &lookAt, source, NULL );
	}
	return position;
}

/*
================
TargetCameraMode::GetIdealDirection

Faces from the ideal position toward a point "LookBetween" of the way
from the source to the target (with its offset), with the source's up
kept square to that.
================
*/
// FUNCTION: STBC 0x004235e0
// DRAFT: inline budget: Unitize's Length gets a share of 57 (cost 57) and is expanded where retail calls it (needs 4 more units spent before Unitize or one more site after it); x87 operand order of the target point's x and of the cross product
void TargetCameraMode::GetIdealDirection( NiPoint3 *forward, NiPoint3 *up ) {
	ObjectClass *source = GetAttrIDObject( "Source" );
	ObjectClass *target = GetAttrIDObject( "Target" );
	if ( source != NULL && target != NULL ) {
		NiPoint3 position = GetIdealPosition();
		NiPoint3 offset = GetTargetOffset();
		float lookBetween = GetAttrFloat( "LookBetween" );
		NiPoint3 targetPoint = target->GetWorldLocation() + target->GetWorldRotation() * TGPoint3( offset );
		targetPoint *= lookBetween;
		NiPoint3 lookAt = source->GetWorldLocation() * ( 1.0f - lookBetween ) + targetPoint;
		*forward = lookAt - position;
		forward->Unitize();
		NiPoint3 right = forward->Cross( source->GetWorldUp() );
		*up = right.UnitCross( *forward );
	}
}

/*
================
TargetCameraMode::GetTargetOffset

"TargetOffset", eased in from "BetweenPositionStart" over
"BetweenSweepTime" from the game time "BetweenTimeStart" (-1 when no
sweep runs, which the sweep's end sets again). Invented name.
================
*/
// FUNCTION: STBC 0x00423820
// PARKED: frame slots: retail keeps the sweep fraction in the start time's slot, this build in the sweep time's; reusing startTime, splitting the division and the blend spellings leave it
NiPoint3 TargetCameraMode::GetTargetOffset( void ) {
	NiPoint3 offset = GetAttrPoint( "TargetOffset" );
	float startTime = GetAttrFloat( "BetweenTimeStart" );
	if ( startTime != -1.0f ) {
		float sweepTime = GetAttrFloat( "BetweenSweepTime" );
		if ( sweepTime > 0.0f ) {
			float t = ( UtopiaApp::GetApp()->GetGameTime() - startTime ) / sweepTime;
			if ( t >= 0.0f && t < 1.0f ) {
				float weight = 0.5f - (float)cos( PI * t ) * 0.5f;
				float startWeight = 1.0f - weight;
				offset = offset * weight + GetAttrPoint( "BetweenPositionStart" ) * startWeight;
			} else {
				SetAttrFloat( "BetweenTimeStart", -1.0f );
			}
		}
	}
	return offset;
}

/*
================
MapCameraMode::WriteToStream
================
*/
// FUNCTION: STBC 0x00424c80
bool MapCameraMode::WriteToStream( TGStream *stream ) {
	if ( !IdealControlledCameraMode::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( unknown70.x );
	stream->WriteFloat( unknown70.y );
	stream->WriteFloat( unknown70.z );
	return true;
}

/*
================
MapCameraMode::ReadFromStream
================
*/
// FUNCTION: STBC 0x00424cc0
bool MapCameraMode::ReadFromStream( TGStream *stream ) {
	if ( !IdealControlledCameraMode::ReadFromStream( stream ) ) {
		return false;
	}
	unknown70.x = stream->ReadFloat();
	unknown70.y = stream->ReadFloat();
	unknown70.z = stream->ReadFloat();
	return true;
}

/*
================
ChaseCameraMode::ChaseCameraMode
================
*/
// FUNCTION: STBC 0x00421e00
ChaseCameraMode::ChaseCameraMode( CameraObjectClass *camera_ ) : IdealControlledCameraMode( camera_ ) {
	SetAttrFloat( "MinimumDistance", 2.0f );
	SetAttrFloat( "Distance", 4.0f );
	SetAttrFloat( "MaximumDistance", 40.0f );
	// The points are NiPoint3 values made TGPoint3s, as the other modes'
	// NiPoint3 constants are: with TGPoint3's own float constructor the
	// last SetAttrFloat's SetAt gets a share of 200 and is expanded, where
	// retail calls it (the share needs a sixth site after it).
	// MATCH-PENDING: R17, the NiPoint3-in-TGPoint3 spelling of these two points is chosen by the inline budget alone; no evidence yet that the original wrote NiPoint3 values here (user's ruling, 2026-11-21)
	SetAttrPoint( "ViewTargetOffset", &TGPoint3( NiPoint3( 0.0f, 0.0f, 0.1f ) ) );
	SetAttrPoint( "DefaultPosition", &TGPoint3( NiPoint3( 0.0f, -1.0f, 0.1f ) ) );
}

/*
================
TargetCameraMode::TargetCameraMode
================
*/
// FUNCTION: STBC 0x00422890
TargetCameraMode::TargetCameraMode( CameraObjectClass *camera_ ) : IdealControlledCameraMode( camera_ ) {
	SetAttrFloat( "MinimumDistance", 2.0f );
	SetAttrFloat( "Distance", 4.0f );
	SetAttrFloat( "MaximumDistance", 40.0f );
	SetAttrFloat( "BackWatchPos", 7.95f );
	SetAttrFloat( "UpWatchPos", 0.95f );
	SetAttrFloat( "LookBetween", 0.1f );
	SetAttrPoint( "TargetOffset", &TGPoint3( NiPoint3::ZERO ) );
	SetAttrFloat( "MaxSourceUpDot", 0.6f );
	SetAttrFloat( "RightPush", 1.0f );
	SetAttrFloat( "BetweenTimeStart", -1.0f );
	SetAttrFloat( "BetweenSweepTime", 1.0f );
	SetAttrPoint( "BetweenPositionStart", &TGPoint3( NiPoint3::ZERO ) );
	SetAttrID( "Source", 0 );
}

/*
================
ZoomTargetMode::ZoomTargetMode
================
*/
// FUNCTION: STBC 0x004239c0
ZoomTargetMode::ZoomTargetMode( CameraObjectClass *camera_ ) : IdealControlledCameraMode( camera_ ) {
	CameraMode::SetAttrID( "Source", 0 );
	SetAttrFloat( "MinimumDistance", 1.25f );
	SetAttrFloat( "Distance", 2.25f );
	SetAttrFloat( "MaximumDistance", 8.0f );
}

/*
================
ZoomTargetMode::IsValid

The mode needs both objects, and the target can't be a sun.
================
*/
// FUNCTION: STBC 0x00423cd0
bool ZoomTargetMode::IsValid( void ) {
	ObjectClass *source = GetAttrIDObject( "Source" );
	ObjectClass *target = GetAttrIDObject( "Target" );
	if ( source != NULL && target != NULL ) {
		return target->IsTypeOf( 0x8005 ) ? false : true;
	}
	return false;
}

/*
================
ZoomTargetMode::Zoom
================
*/
// FUNCTION: STBC 0x00423d10
void ZoomTargetMode::Zoom( float zoom ) {
	float distance = -1.0f;
	floatAttrs.GetAt( "Distance", distance );
	float maximum = -1.0f;
	floatAttrs.GetAt( "MaximumDistance", maximum );
	float minimum = -1.0f;
	floatAttrs.GetAt( "MinimumDistance", minimum );
	SetAttrFloat( "Distance", GetZoomedDistance( distance, minimum, maximum, zoom ) );
}

/*
================
MapCameraMode::MapCameraMode
================
*/
// FUNCTION: STBC 0x00424410
MapCameraMode::MapCameraMode( CameraObjectClass *camera_ ) : IdealControlledCameraMode( camera_ ) {
	SetAttrFloat( "MinimumDistance", 2.0f );
	SetAttrFloat( "Distance", 4.0f );
	SetAttrFloat( "MaximumDistance", 40.0f );
}

/*
================
LockedPositionMode::LockedPositionMode
================
*/
// FUNCTION: STBC 0x004267e0
LockedPositionMode::LockedPositionMode( CameraObjectClass *camera_ ) : CameraMode( camera_ ) {
	SetAttrPoint( "Position", &TGPoint3( NiPoint3::ZERO ) );
	SetAttrPoint( "Forward", &TGPoint3( NiPoint3::UNIT_Y ) );
	SetAttrPoint( "Up", &TGPoint3( NiPoint3::UNIT_Z ) );
}

/*
================
PlaceByDirectionMode::PlaceByDirectionMode
================
*/
// FUNCTION: STBC 0x004278a0
PlaceByDirectionMode::PlaceByDirectionMode( CameraObjectClass *camera_ ) : CameraMode( camera_ ) {
	SetAttrPoint( "BasePosition", &TGPoint3( NiPoint3::ZERO ) );
	SetAttrPoint( "Movement", &TGPoint3( NiPoint3::ZERO ) );
	SetAttrPoint( "Forward", &TGPoint3( -NiPoint3::UNIT_Y ) );
	SetAttrPoint( "Right", &TGPoint3( -NiPoint3::UNIT_X ) );
	SetAttrFloat( "StartMoveAngle", 1.25f );
	SetAttrFloat( "EndMoveAngle", 2.5f );
}

/*
================
PlaceByDirectionMode::GetObjType
================
*/
// FUNCTION: STBC 0x00427aa0
int PlaceByDirectionMode::GetObjType( void ) {
	return PLACEBYDIRECTIONMODE_TYPE;
}

/*
================
PlaceByDirectionMode::IsTypeOf
================
*/
// FUNCTION: STBC 0x00427ab0
bool PlaceByDirectionMode::IsTypeOf( int type ) {
	if ( type == PLACEBYDIRECTIONMODE_TYPE ) {
		return true;
	}
	return CameraMode::IsTypeOf( type );
}

/*
================
PlaceByDirectionMode::GetClassName
================
*/
// FUNCTION: STBC 0x00427b00
char *PlaceByDirectionMode::GetClassName( void ) {
	return "PlaceByDirectionMode";
}

/*
================
PlaceByDirectionMode::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00427b10
char *PlaceByDirectionMode::GetSwigTypeName( void ) {
	return "_p_PlaceByDirectionMode";
}

/*
================
PlaceByDirectionMode::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00427b20
char *PlaceByDirectionMode::GetPythonClassName( void ) {
	return "PlaceByDirectionModePtr";
}

/*
================
PlaceByDirectionMode::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00427b30
TGEventHandlerTable *PlaceByDirectionMode::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
PlaceByDirectionMode::Create
================
*/
// FUNCTION: STBC 0x00427b80
CameraMode *PlaceByDirectionMode::Create( CameraObjectClass *camera_ ) {
	return new PlaceByDirectionMode( camera_ );
}

/*
================
PlaceByDirectionMode::Update

Moves the camera from "BasePosition" by "Movement" as it turns away from
"Forward": none up to "StartMoveAngle", all of it past "EndMoveAngle", and
along a cosine ease in between. Nothing moves while the camera animates.
================
*/
// FUNCTION: STBC 0x00427bd0
void PlaceByDirectionMode::Update( void ) {
	if ( camera->IsAnimating() ) {
		return;
	}

	NiPoint3 forward = GetAttrPoint( "Forward" );
	NiPoint3 right = GetAttrPoint( "Right" );
	float forwardDot = forward * camera->GetWorldForward();
	float rightDot = right * camera->GetWorldForward();
	float length = sqrtf( rightDot * rightDot + forwardDot * forwardDot );
	if ( length == 0.0f ) {
		return;
	}
	forwardDot /= length;
	float angle = NiACos( forwardDot );

	NiPoint3 position = GetAttrPoint( "BasePosition" );
	float startAngle = GetAttrFloat( "StartMoveAngle" );
	float endAngle = GetAttrFloat( "EndMoveAngle" );
	if ( angle > endAngle ) {
		position += GetAttrPoint( "Movement" );
	} else if ( angle > startAngle ) {
		float fraction = 0.5f - (float)cos( ( angle - startAngle ) * PI / ( endAngle - startAngle ) ) * 0.5f;
		position += fraction * GetAttrPoint( "Movement" );
	}
	camera->SetTranslate( &position );
}

/*
================
PlaceByDirectionMode::Unknown427E50

The camera position Update gives when the camera looks at the point:
bisects the turn from "Forward" between none and a half turn until the
position it gives looks at the point at that angle.
================
*/
// FUNCTION: STBC 0x00427e50
// PARKED: schedule: retail loads PI for the first loop test before copying the movement point, this build after the first two words of the copy
NiPoint3 PlaceByDirectionMode::Unknown427E50( const NiPoint3 &target ) {
	NiPoint3 forward = GetAttrPoint( "Forward" );
	NiPoint3 right = GetAttrPoint( "Right" );
	float startAngle = GetAttrFloat( "StartMoveAngle" );
	float endAngle = GetAttrFloat( "EndMoveAngle" );
	NiPoint3 basePosition = GetAttrPoint( "BasePosition" );
	NiPoint3 movement = GetAttrPoint( "Movement" );

	NiPoint3 position;
	float low = 0.0f;
	float high = PI;
	while ( high - low > 0.00125f ) {
		float angle = ( high + low ) * 0.5f;
		position = basePosition;
		if ( angle > endAngle ) {
			position += movement;
		} else if ( angle > startAngle ) {
			float fraction = 0.5f - (float)cos( ( angle - startAngle ) * PI / ( endAngle - startAngle ) ) * 0.5f;
			position += movement * fraction;
		}

		NiPoint3 direction = target - position;
		direction.Unitize();
		float forwardDot = forward.Dot( direction );
		float rightDot = right.Dot( direction );
		float length = sqrtf( rightDot * rightDot + forwardDot * forwardDot );
		if ( length == 0.0f ) {
			break;
		}
		float lookAngle = NiACos( forwardDot / length );
		if ( lookAngle > angle ) {
			low = angle;
		} else if ( lookAngle < angle ) {
			high = angle;
		} else {
			break;
		}
	}
	return position;
}

/*
================
ChaseCameraMode::SetAttrPoint

A new default position puts the camera back on it.
================
*/
// FUNCTION: STBC 0x00422020
void ChaseCameraMode::SetAttrPoint( char *attr, TGPoint3 *point ) {
	CameraMode::SetAttrPoint( attr, point );
	if ( strcmp( attr, "DefaultPosition" ) == 0 ) {
		Reset();
	}
}

/*
================
ChaseCameraMode::Reset

The offset from the target becomes the direction of the default position,
and the camera snaps to its ideal position.
================
*/
// FUNCTION: STBC 0x004220a0
void ChaseCameraMode::Reset( void ) {
	unknown70 = GetAttrPoint( "DefaultPosition" );
	unknown70.Unitize();
	SnapToIdealPosition();
}

/*
================
ChaseCameraMode::VirtualSlot32

Turns the offset from the target by the rotation, taken in the target's
frame, unless the turn would bring it within about 18 degrees of the
target's up or down.
================
*/
// FUNCTION: STBC 0x00422140
void ChaseCameraMode::VirtualSlot32( const NiMatrix3 &rotation ) {
	ObjectClass *target = GetAttrIDObject( "Target" );
	if ( target != NULL ) {
		TGPoint3 offset = unknown70;
		TGMatrix3 turn( rotation );
		NiPoint3 turned = target->GetWorldRotation().Inverse() * turn * target->GetWorldRotation() * offset;
		turned.Unitize();
		if ( turned.z < 0.95f && turned.z > -0.95f ) {
			unknown70 = turned;
		}
	}
}

/*
================
ChaseCameraMode::Zoom
================
*/
// FUNCTION: STBC 0x004222f0
void ChaseCameraMode::Zoom( float zoom ) {
	float distance = -1.0f;
	floatAttrs.GetAt( "Distance", distance );
	float maximum = -1.0f;
	floatAttrs.GetAt( "MaximumDistance", maximum );
	float minimum = -1.0f;
	floatAttrs.GetAt( "MinimumDistance", minimum );
	SetAttrFloat( "Distance", GetZoomedDistance( distance, minimum, maximum, zoom ) );
}

/*
================
ChaseCameraMode::GetIdealDirection

The camera looks from its ideal position at the target's view point (the
view target offset in the target's frame, scaled by its radius), or along
the target's forward when that point is too close; its up is the target's,
brought into the plane at right angles to the view.
================
*/
// FUNCTION: STBC 0x00422550
void ChaseCameraMode::GetIdealDirection( NiPoint3 *forward, NiPoint3 *up ) {
	ObjectClass *target = GetAttrIDObject( "Target" );
	if ( target != NULL ) {
		NiPoint3 position = GetIdealPosition();
		float radius = target->GetWorldBound().GetRadius();
		NiPoint3 viewOffset = GetAttrPoint( "ViewTargetOffset" ) * radius;
		NiPoint3 viewPoint = target->GetWorldLocation() + target->GetWorldRotation() * viewOffset;
		*forward = viewPoint - position;
		forward->Unitize();
		if ( forward->SqrLength() < 0.5f ) {
			*forward = target->GetWorldForward();
		}
		*up = target->GetWorldUp();
		NiPoint3 right = forward->Cross( *up );
		*up = right.UnitCross( *forward );
	}
}

/*
================
TargetCameraMode::Zoom
================
*/
// FUNCTION: STBC 0x00422c20
void TargetCameraMode::Zoom( float zoom ) {
	float distance = -1.0f;
	floatAttrs.GetAt( "Distance", distance );
	float maximum = -1.0f;
	floatAttrs.GetAt( "MaximumDistance", maximum );
	float minimum = -1.0f;
	floatAttrs.GetAt( "MinimumDistance", minimum );
	SetAttrFloat( "Distance", GetZoomedDistance( distance, minimum, maximum, zoom ) );
}

/*
================
TargetCameraMode::SetAttrID

A new target or source while the mode is current sweeps from where the
camera is now.
================
*/
// FUNCTION: STBC 0x00422d30
void TargetCameraMode::SetAttrID( char *attr, long id ) {
	CameraMode::SetAttrID( attr, id );
	if ( strcmp( attr, "Target" ) == 0 || strcmp( attr, "Source" ) == 0 ) {
		if ( unknown34 ) {
			SetAttrPoint( "PosBeforeSweep", (TGPoint3 *)&camera->GetWorldLocation() );
			camera->lastModeID = 0;
		}
		Unknown421410();
	}
}

/*
================
DropAndWatchMode::VirtualSlot28

The mode starts still, next to the target.
================
*/
// FUNCTION: STBC 0x004251a0
void DropAndWatchMode::VirtualSlot28( void ) {
	SetAttrFloat( "AwayDistance", 0.0f );
	SetAttrFloat( "RotateSpeed", 0.0f );
}

/*
================
CameraMode::GetLaggedPosition

Where the object was lag seconds ago; with "MaxLagDist" set, the lagged
place is pulled toward the object, never farther than that distance.
================
*/
// FUNCTION: STBC 0x0041fba0
NiPoint3 CameraMode::GetLaggedPosition( ObjectClass *object, float lag ) {
	if ( object != NULL ) {
		if ( lag == -1.0f ) {
			lag = object->GetRadius() * 0.22935779f;
			if ( lag < 0.125f ) {
				lag = 0.125f;
			}
		}
		float lagPosition = GetAttrFloat( "LagPos" );
		if ( lagPosition != -1.0f ) {
			if ( lagPosition <= 0.0f ) {
				return object->GetWorldLocation();
			}
			lag = lagPosition;
		}
		NiTListIterator pos = lagTrackers.GetHeadPos();
		long id = object->GetObjID();
		while ( pos != NULL ) {
			CameraLagTracker *tracker = lagTrackers.GetNext( pos );
			if ( tracker->objectID == id ) {
				NiPoint3 position = tracker->GetLaggedPosition( lag );
				float maxDistance = GetAttrFloat( "MaxLagDist" );
				if ( maxDistance > 0.0f ) {
					NiPoint3 offset = position - object->GetWorldLocation();
					float distance = offset.Unitize();
					float scale = (float)atan( distance ) * maxDistance / NI_HALF_PI;
					position = object->GetWorldLocation() + offset * scale;
				}
				return position;
			}
		}
	}
	return NiPoint3::ZERO;
}

/*
================
CameraMode::GetLaggedRotation
================
*/
// FUNCTION: STBC 0x0041fe10
NiMatrix3 CameraMode::GetLaggedRotation( ObjectClass *object, float lag ) {
	if ( object != NULL ) {
		if ( lag == -1.0f ) {
			lag = object->GetRadius() * 0.22935779f;
			if ( lag < 0.125f ) {
				lag = 0.125f;
			}
		}
		float lagRotation = -1.0f;
		floatAttrs.GetAt( "LagRot", lagRotation );
		if ( lagRotation != -1.0f ) {
			if ( lagRotation <= 0.0f ) {
				return object->GetWorldRotation();
			}
			lag = lagRotation;
		}
		NiTListIterator pos = lagTrackers.GetHeadPos();
		long id = object->GetObjID();
		while ( pos != NULL ) {
			CameraLagTracker *tracker = lagTrackers.GetNext( pos );
			if ( tracker->objectID == id ) {
				return tracker->GetLaggedRotation( lag );
			}
		}
	}
	return NiMatrix3::ZERO;
}

/*
================
CameraMode::KeepClearOfObjects

Moves the position back along its view of the point while the target is
nearer along it than its radius plus the near plane, and forward while the
source reaches back over it.
================
*/
// FUNCTION: STBC 0x0041ff50
// PARKED: register: retail keeps the frustum's address (camera + 0x144) across GetRadius's call and reads the near distance at 0x10; this build keeps the camera and reads 0x154; a reference and a pointer to the frustum compile alike
void CameraMode::KeepClearOfObjects( NiPoint3 *position, const NiPoint3 *lookAt, ObjectClass *target, ObjectClass *source ) {
	if ( target != NULL ) {
		NiPoint3 direction = *lookAt - *position;
		direction.Unitize();
		float along = ( target->GetWorldLocation() - *position ) * direction;
		const NiFrustum &frustum = camera->GetNiCamera()->GetViewFrustum();
		float clearance = target->GetRadius() + frustum.nearDistance;
		if ( clearance > along ) {
			*position -= direction * ( clearance - along );
		}
		if ( source != NULL ) {
			float behind = -( ( source->GetWorldLocation() - *position ) * direction );
			if ( source->GetRadius() > behind ) {
				*position += direction * ( source->GetRadius() - behind );
			}
		}
	}
}

/*
================
ChaseCameraMode::GetIdealPosition

Behind the target where it was a moment ago: the mode's offset turned as
the target was then, scaled by the distance and the target's radius, from
its lagged place, kept clear of the target.
================
*/
// FUNCTION: STBC 0x00422400
NiPoint3 ChaseCameraMode::GetIdealPosition( void ) {
	NiPoint3 position = camera->GetWorldLocation();
	ObjectClass *target = GetAttrIDObject( "Target" );
	if ( target != NULL ) {
		NiPoint3 lagged = GetLaggedPosition( target, -1.0f );
		float distance = -1.0f;
		floatAttrs.GetAt( "Distance", distance );
		position = GetLaggedRotation( target, -1.0f ) * unknown70;
		distance *= target->GetRadius();
		position *= distance;
		position += lagged;
		KeepClearOfObjects( &position, &lagged, target, NULL );
	}
	return position;
}

/*
================
ZoomTargetMode::GetIdealPosition

At the source's surface toward a placement target, or one the player's
sensors haven't identified; otherwise behind the target on the line from
the source, "Distance" target radii out (or out to the furthest nebula
entry on that line).
The camera's own position while either object is missing.
================
*/
// FUNCTION: STBC 0x00423e20
// DRAFT: inline budget: retail calls NiPoint3::operator*= (0x423530) out of line for the distance scale, this build expands it (budget 55 >= 45 at site 25: needs 6 fewer own tuples or 11 more units spent before it)
NiPoint3 ZoomTargetMode::GetIdealPosition( void ) {
	NiPoint3 position = camera->GetWorldLocation();
	ObjectClass *source = GetAttrIDObject( "Source" );
	ObjectClass *target = GetAttrIDObject( "Target" );
	if ( source != NULL && target != NULL ) {
		NiPoint3 targetPosition = GetLaggedPosition( target, -1.0f );
		NiPoint3 sourcePosition = GetLaggedPosition( source, -1.0f );
		if ( PlacementObject::Cast( target ) != NULL ) {
			position = targetPosition - source->GetWorldLocation();
			position.Unitize();
			position *= source->GetWorldBound().GetRadius();
			position += source->GetWorldLocation();
			return position;
		}
		ShipClass *ship = ShipClass::Cast( source );
		if ( ship != NULL && ship->IsPlayerShip() && ship->GetSensorSubsystem() != NULL && !ship->GetSensorSubsystem()->IsObjectKnown( target ) ) {
			position = targetPosition - source->GetWorldLocation();
			position.Unitize();
			position *= source->GetWorldBound().GetRadius();
			position += source->GetWorldLocation();
			return position;
		}

		position = sourcePosition - targetPosition;
		if ( position * position < 0.5f ) {
			position = source->GetWorldForward();
		}
		float radius = target->GetWorldBound().GetRadius();
		float distance = NebulaClearDistance( source, target, GetAttrFloat( "Distance" ) * radius );
		position.Unitize();
		position *= distance;
		position += targetPosition;
		KeepClearOfObjects( &position, &targetPosition, target, source );
	}
	return position;
}

/*
================
ZoomTargetMode::GetIdealDirection

The camera looks from its ideal position at where the target was a moment
ago, turned around when the source lies ahead; its up is the source's,
brought into the plane at right angles to the view.
================
*/
// FUNCTION: STBC 0x00424210
void ZoomTargetMode::GetIdealDirection( NiPoint3 *forward, NiPoint3 *up ) {
	ObjectClass *source = GetAttrIDObject( "Source" );
	ObjectClass *target = GetAttrIDObject( "Target" );
	if ( source != NULL && target != NULL ) {
		NiPoint3 position = GetIdealPosition();
		*forward = GetLaggedPosition( target, -1.0f ) - position;
		forward->Unitize();
		if ( ( source->GetWorldLocation() - position ) * *forward > 0.0f ) {
			*forward = -*forward;
		}
		NiPoint3 side = forward->Cross( source->GetWorldUp() );
		*up = side.Cross( *forward );
		up->Unitize();
	}
}
