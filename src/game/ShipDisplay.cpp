/*
===============================================================================

	ShipDisplay.cpp

===============================================================================
*/

#include "ShipDisplay.h"
#include "Game.h"
#include "PyEmbed.h"
#include "SetClass.h"
#include "ShipClass.h"
#include "STColors.h"
#include "STFillGauge.h"
#include "ShieldsDisplay.h"
#include "DamageDisplay.h"
#include "ShipSubsystem.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGObject.h"
#include "TGStream.h"
#include <iostream>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x005478a0 _$E511
// FUNCTION: STBC 0x005478c0 _$E510

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00546910 _$E33
// FUNCTION: STBC 0x00546920 _$E32
// FUNCTION: STBC 0x00546930 _$E37
// FUNCTION: STBC 0x00546940 _$E36
// FUNCTION: STBC 0x00546950 _$E48
// FUNCTION: STBC 0x00546960 _$E47
// FUNCTION: STBC 0x00546970 _$E52
// FUNCTION: STBC 0x00546980 _$E51
// FUNCTION: STBC 0x00546990 _$E56
// FUNCTION: STBC 0x005469a0 _$E55
// FUNCTION: STBC 0x005469b0 _$E60
// FUNCTION: STBC 0x005469c0 _$E59
// FUNCTION: STBC 0x00546c30 _$E184
// FUNCTION: STBC 0x00546c40 _$E183
// FUNCTION: STBC 0x005470c0 _$E126
// FUNCTION: STBC 0x005470d0 _$E125
// FUNCTION: STBC 0x005470e0 _$E139
// FUNCTION: STBC 0x005470f0 _$E138
// FUNCTION: STBC 0x00547100 _$E189
// FUNCTION: STBC 0x00547110 _$E188
// FUNCTION: STBC 0x00547120 _$E194
// FUNCTION: STBC 0x00547130 _$E193
// FUNCTION: STBC 0x00547140 _$E198
// FUNCTION: STBC 0x00547150 _$E197
// FUNCTION: STBC 0x00547160 _$E202
// FUNCTION: STBC 0x00547170 _$E201
// FUNCTION: STBC 0x00547180 _$E209
// FUNCTION: STBC 0x00547190 _$E208
// FUNCTION: STBC 0x005471a0 _$E347
// FUNCTION: STBC 0x005471b0 _$E346
// FUNCTION: STBC 0x005471c0 _$E351
// FUNCTION: STBC 0x005471d0 _$E350
// FUNCTION: STBC 0x005471e0 _$E84
// FUNCTION: STBC 0x005471f0 _$E83
// FUNCTION: STBC 0x00547200 _$E281
// FUNCTION: STBC 0x00547210 _$E280
// FUNCTION: STBC 0x00547220 _$E288
// FUNCTION: STBC 0x00547230 _$E287
// FUNCTION: STBC 0x00547240 _$E292
// FUNCTION: STBC 0x00547250 _$E291
// FUNCTION: STBC 0x00547260 _$E296
// FUNCTION: STBC 0x00547270 _$E295
// FUNCTION: STBC 0x00547280 _$E300
// FUNCTION: STBC 0x00547290 _$E299
// FUNCTION: STBC 0x005472a0 _$E306
// FUNCTION: STBC 0x005472b0 _$E305
// FUNCTION: STBC 0x005472c0 _$E355
// FUNCTION: STBC 0x005472d0 _$E354
// FUNCTION: STBC 0x005472e0 _$E359
// FUNCTION: STBC 0x005472f0 _$E358
// FUNCTION: STBC 0x00547300 _$E363
// FUNCTION: STBC 0x00547310 _$E362
// FUNCTION: STBC 0x00547320 _$E367
// FUNCTION: STBC 0x00547330 _$E366
// FUNCTION: STBC 0x00547340 _$E371
// FUNCTION: STBC 0x00547350 _$E370
// FUNCTION: STBC 0x00547360 _$E375
// FUNCTION: STBC 0x00547370 _$E374
// FUNCTION: STBC 0x00547380 _$E379
// FUNCTION: STBC 0x00547390 _$E378
// FUNCTION: STBC 0x005473a0 _$E383
// FUNCTION: STBC 0x005473b0 _$E382
// FUNCTION: STBC 0x005473c0 _$E387
// FUNCTION: STBC 0x005473d0 _$E386
// FUNCTION: STBC 0x005473e0 _$E391
// FUNCTION: STBC 0x005473f0 _$E390
// FUNCTION: STBC 0x00547400 _$E89
// FUNCTION: STBC 0x00547410 _$E88
// FUNCTION: STBC 0x00547420 _$E395
// FUNCTION: STBC 0x00547430 _$E394
// FUNCTION: STBC 0x00547440 _$E94
// FUNCTION: STBC 0x00547450 _$E93
// FUNCTION: STBC 0x00547460 _$E399
// FUNCTION: STBC 0x00547470 _$E398
// FUNCTION: STBC 0x00547480 _$E403
// FUNCTION: STBC 0x00547490 _$E402
// FUNCTION: STBC 0x005474a0 _$E407
// FUNCTION: STBC 0x005474b0 _$E406
// FUNCTION: STBC 0x005474c0 _$E411
// FUNCTION: STBC 0x005474d0 _$E410
// FUNCTION: STBC 0x005474e0 _$E103
// FUNCTION: STBC 0x005474f0 _$E102
// FUNCTION: STBC 0x00547500 _$E415
// FUNCTION: STBC 0x00547510 _$E414
// FUNCTION: STBC 0x00547520 _$E419
// FUNCTION: STBC 0x00547530 _$E418
// FUNCTION: STBC 0x00547540 _$E423
// FUNCTION: STBC 0x00547550 _$E422
// FUNCTION: STBC 0x00547560 _$E109
// FUNCTION: STBC 0x00547570 _$E108
// FUNCTION: STBC 0x00547580 _$E114
// FUNCTION: STBC 0x00547590 _$E113
// FUNCTION: STBC 0x005475a0 _$E144
// FUNCTION: STBC 0x005475b0 _$E143
// FUNCTION: STBC 0x005475c0 _$E427
// FUNCTION: STBC 0x005475d0 _$E426
// FUNCTION: STBC 0x005475e0 _$E149
// FUNCTION: STBC 0x005475f0 _$E148
// FUNCTION: STBC 0x00547600 _$E431
// FUNCTION: STBC 0x00547610 _$E430
// FUNCTION: STBC 0x00547620 _$E435
// FUNCTION: STBC 0x00547630 _$E434
// FUNCTION: STBC 0x00547640 _$E154
// FUNCTION: STBC 0x00547650 _$E153
// FUNCTION: STBC 0x00547660 _$E439
// FUNCTION: STBC 0x00547670 _$E438
// FUNCTION: STBC 0x00547680 _$E119
// FUNCTION: STBC 0x00547690 _$E118
// FUNCTION: STBC 0x005476a0 _$E443
// FUNCTION: STBC 0x005476b0 _$E442
// FUNCTION: STBC 0x005476c0 _$E447
// FUNCTION: STBC 0x005476d0 _$E446
// FUNCTION: STBC 0x005476e0 _$E159
// FUNCTION: STBC 0x005476f0 _$E158
// FUNCTION: STBC 0x00547700 _$E164
// FUNCTION: STBC 0x00547710 _$E163
// FUNCTION: STBC 0x00547720 _$E451
// FUNCTION: STBC 0x00547730 _$E450
// FUNCTION: STBC 0x00547740 _$E455
// FUNCTION: STBC 0x00547750 _$E454
// FUNCTION: STBC 0x00547760 _$E459
// FUNCTION: STBC 0x00547770 _$E458
// FUNCTION: STBC 0x00547780 _$E463
// FUNCTION: STBC 0x00547790 _$E462
// FUNCTION: STBC 0x005477a0 _$E467
// FUNCTION: STBC 0x005477b0 _$E466
// FUNCTION: STBC 0x005477c0 _$E471
// FUNCTION: STBC 0x005477d0 _$E470
// FUNCTION: STBC 0x005477e0 _$E475
// FUNCTION: STBC 0x005477f0 _$E474
// FUNCTION: STBC 0x00547800 _$E479
// FUNCTION: STBC 0x00547810 _$E478
// FUNCTION: STBC 0x00547820 _$E169
// FUNCTION: STBC 0x00547830 _$E168
// FUNCTION: STBC 0x00547840 _$E483
// FUNCTION: STBC 0x00547850 _$E482
// FUNCTION: STBC 0x00547860 _$E174
// FUNCTION: STBC 0x00547870 _$E173
// FUNCTION: STBC 0x00547880 _$E179
// FUNCTION: STBC 0x00547890 _$E178
// FUNCTION: STBC 0x005478d0 _$E487
// FUNCTION: STBC 0x005478e0 _$E486
// FUNCTION: STBC 0x005478f0 _$E492
// FUNCTION: STBC 0x00547900 _$E491
// FUNCTION: STBC 0x00547910 _$E496
// FUNCTION: STBC 0x00547920 _$E495

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00546890 _$E331
// FUNCTION: STBC 0x005468a0 _$E328
// FUNCTION: STBC 0x005468b0 _$E330
// FUNCTION: STBC 0x005468c0 _$E329
// FUNCTION: STBC 0x005468d0 _$E337
// FUNCTION: STBC 0x005468e0 _$E334
// FUNCTION: STBC 0x005468f0 _$E336
// FUNCTION: STBC 0x00546900 _$E335

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x005469d0 _$E502
// FUNCTION: STBC 0x005469e0 _$E498
// FUNCTION: STBC 0x005469f0 _$E501
// FUNCTION: STBC 0x00546a00 _$E499
// DATA: STBC 0x0098aa00
TGEventHandlerTable ShipDisplay::classHandlers;

/*
================
ShipDisplay::Cast

The object as a ShipDisplay; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00546A20
ShipDisplay *ShipDisplay::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( SHIPDISPLAY_TYPE ) ) {
		return (ShipDisplay *)object;
	}
	return NULL;
}

/*
================
ShipDisplay::ShipDisplay

A window showing no ship yet; the scripts build its parts, and the health
gauge they make shows the ship's combined condition.
================
*/
// FUNCTION: STBC 0x00546a50 ??0ShipDisplay@@QAE@XZ
ShipDisplay::ShipDisplay( void ) : STStylizedWindow( "StylizedWindow", "NormalStyle", NULL, 0.0f, 0.0f, NULL, false, 0.0f, 0.0f, &g_kInterfaceBorderColor ) {
	shipID = 0;
	shieldsDisplay = NULL;
	damageDisplay = NULL;
	healthGauge = NULL;
	TGPane::GetFirstChild()->SetNotBatchChildPolys( true );
	PyObject *self = VirtualSlot08();
	PyEmbed::CallFunction( "Tactical.Interface.ShipDisplay", "Create", NULL, NULL, "(O)", self );
	Py_DECREF( self );
	STFillGauge *gauge = healthGauge;
	gauge->valueFunction = (GaugeValueFunction)&ShipSubsystem::GetCombinedConditionPercentage;
	gauge->maxFunction = NULL;
	flags |= TGUIO_NO_FOCUS | TGUIO_ALWAYS_HANDLE_EVENTS;
}

/*
================
ShipDisplay::ResizeUI

Has the scripts redo the display's sizes.
================
*/
// FUNCTION: STBC 0x00546C50
void ShipDisplay::ResizeUI( void ) {
	PyObject *self = VirtualSlot08();
	PyEmbed::CallFunction( "Tactical.Interface.ShipDisplay", "ResizeUI", NULL, NULL, "(O)", self );
	Py_DECREF( self );
}

/*
================
ShipDisplay::RepositionUI

Has the scripts redo the display's layout.
================
*/
// FUNCTION: STBC 0x00546C90
void ShipDisplay::RepositionUI( void ) {
	PyObject *self = VirtualSlot08();
	PyEmbed::CallFunction( "Tactical.Interface.ShipDisplay", "RepositionUI", NULL, NULL, "(O)", self );
	Py_DECREF( self );
}

/*
================
ShipDisplay::SetShipID

Stops listening to the ship shown, has the scripts show the ship with the
ID, and listens to that ship's destruction and to ships leaving a set.
================
*/
// FUNCTION: STBC 0x00546CD0
void ShipDisplay::SetShipID( long id ) {
	if ( shipID != 0 ) {
		ShipClass *oldShip = ShipClass::GetObjectByID( NULL, shipID );
		g_kEventManager.RemoveBroadcastHandler( ET_OBJECT_DESTROYED, this, "ShipDisplay::HandleShipDestroyed", oldShip );
		g_kEventManager.RemoveBroadcastHandler( ET_EXITED_SET, this, "ShipDisplay::HandleShipExitedSet" );
	}

	PyObject *self = VirtualSlot08();
	PyEmbed::CallFunction( "Tactical.Interface.ShipDisplay", "SetShipID", NULL, NULL, "Oi", self, id );
	Py_DECREF( self );

	if ( shipID != 0 ) {
		ShipClass *ship = ShipClass::GetObjectByID( NULL, shipID );
		g_kEventManager.AddBroadcastMethodHandler( ET_OBJECT_DESTROYED, this, "ShipDisplay::HandleShipDestroyed", ship );
		g_kEventManager.AddBroadcastMethodHandler( ET_EXITED_SET, this, "ShipDisplay::HandleShipExitedSet" );
	}
}

/*
================
ShipDisplay::AddTargetChangeEvent

Listens to the player changing target.
================
*/
// FUNCTION: STBC 0x00546DC0
void ShipDisplay::AddTargetChangeEvent( void ) {
	ShipClass *player = ShipClass::Cast( Game::GetCurrentGame()->GetPlayer() );
	g_kEventManager.AddBroadcastMethodHandler( ET_TARGET_WAS_CHANGED, this, "ShipDisplay::HandleChangeTarget", player );
}

/*
================
ShipDisplay::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00546df0
void ShipDisplay::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ShipDisplay::HandleShipDestroyed, "ShipDisplay::HandleShipDestroyed" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ShipDisplay::HandleShipExitedSet, "ShipDisplay::HandleShipExitedSet" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ShipDisplay::HandleChangeTarget, "ShipDisplay::HandleChangeTarget" );
}

/*
================
ShipDisplay::RegisterHandlers

The display's handlers are added for each instance, not to the class's table.
================
*/
// FUNCTION: STBC 0x00546e30
void ShipDisplay::RegisterHandlers( void ) {
}

/*
================
ShipDisplay::HandleShipDestroyed

The ship shown is gone: shows none.
================
*/
// FUNCTION: STBC 0x00546e40
void ShipDisplay::HandleShipDestroyed( TGEvent *event ) {
	SetShipID( 0 );
	CallNextHandler( event );
}

/*
================
ShipDisplay::HandleShipExitedSet

Shows no ship when it no longer lies in the set of the player's ship.
================
*/
// FUNCTION: STBC 0x00546e60
void ShipDisplay::HandleShipExitedSet( TGEvent *event ) {
	if ( Game::GetCurrentGame() == NULL ) {
		CallNextHandler( event );
		return;
	}
	ShipClass *player = ShipClass::Cast( Game::GetCurrentGame()->GetPlayer() );
	ObjectClass *ship = ObjectClass::GetObjectByID( NULL, GetShipID() );
	if ( player == NULL || player->GetContainingSet() != ship->GetContainingSet() ) {
		SetShipID( 0 );
	}
	CallNextHandler( event );
}

/*
================
ShipDisplay::HandleChangeTarget

Shows the player's new target, or no ship when the target is none.
================
*/
// FUNCTION: STBC 0x00546ec0
void ShipDisplay::HandleChangeTarget( TGEvent *event ) {
	ShipClass *player = Game::GetCurrentGame()->GetPlayer();
	if ( player != NULL ) {
		ShipClass *target = ShipClass::Cast( player->GetTarget() );
		if ( target != NULL ) {
			SetShipID( target->GetObjID() );
		} else {
			SetShipID( 0 );
		}
	}
	CallNextHandler( event );
}

/*
================
ShipDisplay::WriteToStream

The window, then the ship's ID and the IDs of the parts.
================
*/
// FUNCTION: STBC 0x00546f00
bool ShipDisplay::WriteToStream( TGStream *stream ) {
	if ( !STStylizedWindow::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( shipID );
	stream->WriteID( GetSavedID( shieldsDisplay ) );
	stream->WriteID( GetSavedID( damageDisplay ) );
	stream->WriteID( GetSavedID( healthGauge ) );
	return true;
}

/*
================
ShipDisplay::ReadFromStream

The window, then the ship's ID and the IDs of the parts.
================
*/
// FUNCTION: STBC 0x00546f90
bool ShipDisplay::ReadFromStream( TGStream *stream ) {
	if ( !STStylizedWindow::ReadFromStream( stream ) ) {
		return false;
	}
	shipID = stream->ReadID();
	shieldsDisplay = (ShieldsDisplay *)stream->ReadID();
	damageDisplay = (DamageDisplay *)stream->ReadID();
	healthGauge = (STFillGauge *)stream->ReadID();
	return true;
}

/*
================
ShipDisplay::FixUpPointers

BUG: looks for the object of ID 0 once for each part that is set, not for
the part's saved ID, so a part that was saved fails the fix-up whenever there
is an object of ID 0.
================
*/
// FUNCTION: STBC 0x00546ff0 ?FixUpPointers@ShipDisplay@@UAE_NXZ
bool ShipDisplay::FixUpPointers( void ) {
	if ( !TGUIObject::FixUpPointers() ) {
		return false;
	}
	if ( shieldsDisplay != NULL && TGObject::GetTGObjectPtr( 0 ) != NULL ) {
		return false;
	}
	if ( damageDisplay != NULL && TGObject::GetTGObjectPtr( 0 ) != NULL ) {
		return false;
	}
	if ( healthGauge != NULL && TGObject::GetTGObjectPtr( 0 ) != NULL ) {
		return false;
	}
	return true;
}

/*
================
ShipDisplay::PostFixUp

The parts' pointers are the objects of the saved IDs.
================
*/
// FUNCTION: STBC 0x00547060 ?PostFixUp@ShipDisplay@@UAEXXZ
void ShipDisplay::PostFixUp( void ) {
	TGUIObject::PostFixUp();
	shieldsDisplay = (ShieldsDisplay *)GetSavedObject( (long)shieldsDisplay );
	damageDisplay = (DamageDisplay *)GetSavedObject( (long)damageDisplay );
	healthGauge = (STFillGauge *)GetSavedObject( (long)healthGauge );
}
