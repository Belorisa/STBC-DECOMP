/*
===============================================================================

	ShieldsDisplay.cpp

===============================================================================
*/

#include "ShieldsDisplay.h"
#include "FloatRangeWatcher.h"
#include "Game.h"
#include "PyEmbed.h"
#include "SensorSubsystem.h"
#include "SetClass.h"
#include "STColors.h"
#include "TGIcon.h"
#include "ShipClass.h"
#include "ShipDisplay.h"
#include "TGEventManager.h"
#include "TGFloatEvent.h"
#include "TGStream.h"
#include <iostream>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00546800 _$E510
// FUNCTION: STBC 0x00546820 _$E509

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00545680 _$E33
// FUNCTION: STBC 0x00545690 _$E32
// FUNCTION: STBC 0x005456a0 _$E37
// FUNCTION: STBC 0x005456b0 _$E36
// FUNCTION: STBC 0x005456c0 _$E48
// FUNCTION: STBC 0x005456d0 _$E47
// FUNCTION: STBC 0x005456e0 _$E52
// FUNCTION: STBC 0x005456f0 _$E51
// FUNCTION: STBC 0x00545700 _$E56
// FUNCTION: STBC 0x00545710 _$E55
// FUNCTION: STBC 0x00545720 _$E60
// FUNCTION: STBC 0x00545730 _$E59
// FUNCTION: STBC 0x00545740 _$E184
// FUNCTION: STBC 0x00545750 _$E183
// FUNCTION: STBC 0x00546020 _$E126
// FUNCTION: STBC 0x00546030 _$E125
// FUNCTION: STBC 0x00546040 _$E139
// FUNCTION: STBC 0x00546050 _$E138
// FUNCTION: STBC 0x00546060 _$E189
// FUNCTION: STBC 0x00546070 _$E188
// FUNCTION: STBC 0x00546080 _$E194
// FUNCTION: STBC 0x00546090 _$E193
// FUNCTION: STBC 0x005460a0 _$E198
// FUNCTION: STBC 0x005460b0 _$E197
// FUNCTION: STBC 0x005460c0 _$E202
// FUNCTION: STBC 0x005460d0 _$E201
// FUNCTION: STBC 0x005460e0 _$E209
// FUNCTION: STBC 0x005460f0 _$E208
// FUNCTION: STBC 0x00546100 _$E346
// FUNCTION: STBC 0x00546110 _$E345
// FUNCTION: STBC 0x00546120 _$E350
// FUNCTION: STBC 0x00546130 _$E349
// FUNCTION: STBC 0x00546140 _$E84
// FUNCTION: STBC 0x00546150 _$E83
// FUNCTION: STBC 0x00546160 _$E289
// FUNCTION: STBC 0x00546170 _$E288
// FUNCTION: STBC 0x00546180 _$E296
// FUNCTION: STBC 0x00546190 _$E295
// FUNCTION: STBC 0x005461a0 _$E300
// FUNCTION: STBC 0x005461b0 _$E299
// FUNCTION: STBC 0x005461c0 _$E304
// FUNCTION: STBC 0x005461d0 _$E303
// FUNCTION: STBC 0x005461e0 _$E308
// FUNCTION: STBC 0x005461f0 _$E307
// FUNCTION: STBC 0x00546200 _$E314
// FUNCTION: STBC 0x00546210 _$E313
// FUNCTION: STBC 0x00546220 _$E354
// FUNCTION: STBC 0x00546230 _$E353
// FUNCTION: STBC 0x00546240 _$E358
// FUNCTION: STBC 0x00546250 _$E357
// FUNCTION: STBC 0x00546260 _$E362
// FUNCTION: STBC 0x00546270 _$E361
// FUNCTION: STBC 0x00546280 _$E366
// FUNCTION: STBC 0x00546290 _$E365
// FUNCTION: STBC 0x005462a0 _$E370
// FUNCTION: STBC 0x005462b0 _$E369
// FUNCTION: STBC 0x005462c0 _$E374
// FUNCTION: STBC 0x005462d0 _$E373
// FUNCTION: STBC 0x005462e0 _$E378
// FUNCTION: STBC 0x005462f0 _$E377
// FUNCTION: STBC 0x00546300 _$E382
// FUNCTION: STBC 0x00546310 _$E381
// FUNCTION: STBC 0x00546320 _$E386
// FUNCTION: STBC 0x00546330 _$E385
// FUNCTION: STBC 0x00546340 _$E390
// FUNCTION: STBC 0x00546350 _$E389
// FUNCTION: STBC 0x00546360 _$E89
// FUNCTION: STBC 0x00546370 _$E88
// FUNCTION: STBC 0x00546380 _$E394
// FUNCTION: STBC 0x00546390 _$E393
// FUNCTION: STBC 0x005463a0 _$E94
// FUNCTION: STBC 0x005463b0 _$E93
// FUNCTION: STBC 0x005463c0 _$E398
// FUNCTION: STBC 0x005463d0 _$E397
// FUNCTION: STBC 0x005463e0 _$E402
// FUNCTION: STBC 0x005463f0 _$E401
// FUNCTION: STBC 0x00546400 _$E406
// FUNCTION: STBC 0x00546410 _$E405
// FUNCTION: STBC 0x00546420 _$E410
// FUNCTION: STBC 0x00546430 _$E409
// FUNCTION: STBC 0x00546440 _$E103
// FUNCTION: STBC 0x00546450 _$E102
// FUNCTION: STBC 0x00546460 _$E414
// FUNCTION: STBC 0x00546470 _$E413
// FUNCTION: STBC 0x00546480 _$E418
// FUNCTION: STBC 0x00546490 _$E417
// FUNCTION: STBC 0x005464a0 _$E422
// FUNCTION: STBC 0x005464b0 _$E421
// FUNCTION: STBC 0x005464c0 _$E109
// FUNCTION: STBC 0x005464d0 _$E108
// FUNCTION: STBC 0x005464e0 _$E114
// FUNCTION: STBC 0x005464f0 _$E113
// FUNCTION: STBC 0x00546500 _$E144
// FUNCTION: STBC 0x00546510 _$E143
// FUNCTION: STBC 0x00546520 _$E426
// FUNCTION: STBC 0x00546530 _$E425
// FUNCTION: STBC 0x00546540 _$E149
// FUNCTION: STBC 0x00546550 _$E148
// FUNCTION: STBC 0x00546560 _$E430
// FUNCTION: STBC 0x00546570 _$E429
// FUNCTION: STBC 0x00546580 _$E434
// FUNCTION: STBC 0x00546590 _$E433
// FUNCTION: STBC 0x005465a0 _$E154
// FUNCTION: STBC 0x005465b0 _$E153
// FUNCTION: STBC 0x005465c0 _$E438
// FUNCTION: STBC 0x005465d0 _$E437
// FUNCTION: STBC 0x005465e0 _$E119
// FUNCTION: STBC 0x005465f0 _$E118
// FUNCTION: STBC 0x00546600 _$E442
// FUNCTION: STBC 0x00546610 _$E441
// FUNCTION: STBC 0x00546620 _$E446
// FUNCTION: STBC 0x00546630 _$E445
// FUNCTION: STBC 0x00546640 _$E159
// FUNCTION: STBC 0x00546650 _$E158
// FUNCTION: STBC 0x00546660 _$E164
// FUNCTION: STBC 0x00546670 _$E163
// FUNCTION: STBC 0x00546680 _$E450
// FUNCTION: STBC 0x00546690 _$E449
// FUNCTION: STBC 0x005466a0 _$E454
// FUNCTION: STBC 0x005466b0 _$E453
// FUNCTION: STBC 0x005466c0 _$E458
// FUNCTION: STBC 0x005466d0 _$E457
// FUNCTION: STBC 0x005466e0 _$E462
// FUNCTION: STBC 0x005466f0 _$E461
// FUNCTION: STBC 0x00546700 _$E466
// FUNCTION: STBC 0x00546710 _$E465
// FUNCTION: STBC 0x00546720 _$E470
// FUNCTION: STBC 0x00546730 _$E469
// FUNCTION: STBC 0x00546740 _$E474
// FUNCTION: STBC 0x00546750 _$E473
// FUNCTION: STBC 0x00546760 _$E478
// FUNCTION: STBC 0x00546770 _$E477
// FUNCTION: STBC 0x00546780 _$E169
// FUNCTION: STBC 0x00546790 _$E168
// FUNCTION: STBC 0x005467a0 _$E482
// FUNCTION: STBC 0x005467b0 _$E481
// FUNCTION: STBC 0x005467c0 _$E174
// FUNCTION: STBC 0x005467d0 _$E173
// FUNCTION: STBC 0x005467e0 _$E179
// FUNCTION: STBC 0x005467f0 _$E178
// FUNCTION: STBC 0x00546830 _$E486
// FUNCTION: STBC 0x00546840 _$E485
// FUNCTION: STBC 0x00546850 _$E491
// FUNCTION: STBC 0x00546860 _$E490
// FUNCTION: STBC 0x00546870 _$E495
// FUNCTION: STBC 0x00546880 _$E494

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00545600 _$E330
// FUNCTION: STBC 0x00545610 _$E327
// FUNCTION: STBC 0x00545620 _$E329
// FUNCTION: STBC 0x00545630 _$E328
// FUNCTION: STBC 0x00545640 _$E336
// FUNCTION: STBC 0x00545650 _$E333
// FUNCTION: STBC 0x00545660 _$E335
// FUNCTION: STBC 0x00545670 _$E334

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00545760 _$E501
// FUNCTION: STBC 0x00545770 _$E497
// FUNCTION: STBC 0x00545780 _$E500
// FUNCTION: STBC 0x00545790 _$E498
// DATA: STBC 0x0098a998
TGEventHandlerTable ShieldsDisplay::classHandlers;

/*
================
ShieldsDisplay::Cast

The object as a ShieldsDisplay; NULL when it is none.
================
*/
// FUNCTION: STBC 0x005457B0
ShieldsDisplay *ShieldsDisplay::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( SHIELDSDISPLAY_TYPE ) ) {
		return (ShieldsDisplay *)object;
	}
	return NULL;
}

/*
================
ShieldsDisplay::ShieldsDisplay

A pane of the given size with no range checks yet; the scripts build the
display's icons.
================
*/
// FUNCTION: STBC 0x005457e0 ??0ShieldsDisplay@@QAE@MM@Z
ShieldsDisplay::ShieldsDisplay( float width, float height ) : TGPane( 0 ) {
	for ( int shield = 0; shield < ShieldClass::NUM_SHIELDS + 1; shield++ ) {
		for ( int check = 0; check < SHIELD_LEVEL_CHECKS; check++ ) {
			shieldChecks[shield][check] = FloatRangeWatcher::RID_INVALID;
		}
	}
	PyObject *self = VirtualSlot08();
	PyEmbed::CallFunction( "Tactical.Interface.ShieldsDisplay", "Create", NULL, NULL, "(O)", self );
	Py_DECREF( self );
}

/*
================
ShieldsDisplay::AddEvents

Listens to the ship the ship display above shows, and puts range checks on
each of its shields that tell the ship when the shield's level passes
SHIELD_LEVEL_LOWEST or a multiple of SHIELD_LEVEL_STEP.
================
*/
// FUNCTION: STBC 0x00545970
void ShieldsDisplay::AddEvents( void ) {
	SetClass *set = Game::GetCurrentGame()->GetPlayerSet();
	ShipDisplay *shipDisplay = (ShipDisplay *)GetConceptualParent();
	ShipClass *ship = ShipClass::GetObjectByID( set, shipDisplay->GetShipID() );
	g_kEventManager.AddBroadcastMethodHandler( ET_SHIELD_LEVEL_CHANGE, this, "ShieldsDisplay::Update", ship );
	g_kEventManager.AddBroadcastMethodHandler( ET_SENSORS_SHIP_IDENTIFIED, this, "ShieldsDisplay::ShipIdentified", ship );

	// BUG: the ship is not tested before its shields are read
	ShieldClass *shields = ship->GetShields();
	if ( shields == NULL ) {
		return;
	}
	for ( int i = 0; i < ShieldClass::NUM_SHIELDS + 1; i++ ) {
		FloatRangeWatcher *watcher = shields->GetShieldWatcher( i );
		TGFloatEvent *event = new TGFloatEvent;
		event->SetDestination( ship );
		event->SetEventType( ET_SHIELD_LEVEL_CHANGE );
		shieldChecks[i][0] = watcher->AddRangeCheck( SHIELD_LEVEL_LOWEST, FloatRangeWatcher::FRW_BOTH, event );
		for ( int j = 1; j <= SHIELD_LEVEL_STEPS; j++ ) {
			float level = j * SHIELD_LEVEL_STEP;
			event = new TGFloatEvent;
			event->SetDestination( ship );
			event->SetEventType( ET_SHIELD_LEVEL_CHANGE );
			shieldChecks[i][j] = watcher->AddRangeCheck( level, FloatRangeWatcher::FRW_BOTH, event );
		}
	}
}

/*
================
ShieldsDisplay::RemoveEvents

Stops listening to the ship the ship display above shows, and removes the
range checks from its shields.
================
*/
// FUNCTION: STBC 0x00545af0
void ShieldsDisplay::RemoveEvents( void ) {
	if ( Game::GetCurrentGame() == NULL ) {
		return;
	}
	ShipDisplay *shipDisplay = (ShipDisplay *)GetConceptualParent();
	if ( shipDisplay == NULL ) {
		return;
	}
	ShipClass *ship = ShipClass::GetObjectByID( NULL, shipDisplay->GetShipID() );
	g_kEventManager.RemoveBroadcastHandler( ET_SHIELD_LEVEL_CHANGE, this, "ShieldsDisplay::Update", ship );
	g_kEventManager.RemoveBroadcastHandler( ET_SENSORS_SHIP_IDENTIFIED, this, "ShieldsDisplay::ShipIdentified", ship );

	int i;
	int j;
	if ( ship != NULL ) {
		ShieldClass *shields = ship->GetShields();
		if ( shields != NULL ) {
			for ( i = 0; i < ShieldClass::NUM_SHIELDS + 1; i++ ) {
				FloatRangeWatcher *watcher = shields->GetShieldWatcher( i );
				if ( watcher != NULL ) {
					for ( j = 0; j < SHIELD_LEVEL_CHECKS; j++ ) {
						watcher->RemoveRangeCheck( shieldChecks[i][j] );
						shieldChecks[i][j] = FloatRangeWatcher::RID_INVALID;
					}
				}
			}
		}
	} else {
		for ( i = 0; i < ShieldClass::NUM_SHIELDS + 1; i++ ) {
			for ( j = 0; j < SHIELD_LEVEL_CHECKS; j++ ) {
				shieldChecks[i][j] = FloatRangeWatcher::RID_INVALID;
			}
		}
	}
}

/*
================
ShieldsDisplay::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00545be0
void ShieldsDisplay::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ShieldsDisplay::Update, "ShieldsDisplay::Update" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ShieldsDisplay::ShipIdentified, "ShieldsDisplay::ShipIdentified" );
}

/*
================
ShieldsDisplay::RegisterHandlers

The display's handlers are broadcast ones only.
================
*/
// FUNCTION: STBC 0x00545c10
void ShieldsDisplay::RegisterHandlers( void ) {
}

/*
================
ShieldsDisplay::ResizeUI

Has the scripts redo the display's sizes.
================
*/
// FUNCTION: STBC 0x00545C20
void ShieldsDisplay::ResizeUI( void ) {
	PyObject *self = VirtualSlot08();
	PyEmbed::CallFunction( "Tactical.Interface.ShieldsDisplay", "ResizeUI", NULL, NULL, "(O)", self );
	Py_DECREF( self );
}

/*
================
ShieldsDisplay::RepositionUI

Has the scripts redo the display's layout.
================
*/
// FUNCTION: STBC 0x00545C60
void ShieldsDisplay::RepositionUI( void ) {
	PyObject *self = VirtualSlot08();
	PyEmbed::CallFunction( "Tactical.Interface.ShieldsDisplay", "RepositionUI", NULL, NULL, "(O)", self );
	Py_DECREF( self );
}

/*
================
ShieldsDisplay::UpdateForNewShip

For the ship the ship display above shows: the ship's icon, the events and
the display shown; then the shields drawn.
================
*/
// FUNCTION: STBC 0x00545ca0
void ShieldsDisplay::UpdateForNewShip( void ) {
	ShipDisplay *shipDisplay = (ShipDisplay *)GetConceptualParent();
	if ( shipDisplay->GetShipID() != 0 ) {
		SetShipIcon();
		AddEvents();
		GetNthChild( 0 )->SetVisible( true );
	}
	Update( NULL );
}

/*
================
ShieldsDisplay::SetShipIcon

Has the scripts show the ship's icon.
================
*/
// FUNCTION: STBC 0x00545cf0
void ShieldsDisplay::SetShipIcon( void ) {
	PyObject *self = VirtualSlot08();
	PyEmbed::CallFunction( "Tactical.Interface.ShieldsDisplay", "SetShipIcon", NULL, NULL, "(O)", self );
	Py_DECREF( self );
}

/*
================
ShieldsDisplay::ShipIdentified

The sensors identified the ship: its icon may change.
================
*/
// FUNCTION: STBC 0x00545d30
void ShieldsDisplay::ShipIdentified( TGEvent *event ) {
	SetShipIcon();
	if ( event != NULL ) {
		CallNextHandler( event );
	}
}

/*
================
ShieldsDisplay::Update

The shields' levels changed: each facing's icon takes the condition colour
of its level. Without a ship the script lays the display out again; a ship
that is gone hides the display.
================
*/
// FUNCTION: STBC 0x00545d50
void ShieldsDisplay::Update( TGEvent *event ) {
	TGUIObject *pane = GetNthChild( 0 );
	ShipDisplay *shipDisplay = (ShipDisplay *)GetConceptualParent();
	if ( shipDisplay->GetShipID() == 0 ) {
		PyObject *self = VirtualSlot08();
		PyEmbed::CallFunction( "Tactical.Interface.ShieldsDisplay", "RepositionUI", NULL, NULL, "(O)", self );
		Py_DECREF( self );
		if ( event != NULL ) {
			CallNextHandler( event );
		}
		return;
	}

	SetClass *playerSet = Game::GetCurrentGame()->GetPlayerSet();
	if ( playerSet == NULL ) {
		pane->SetNotVisible( true );
		if ( event != NULL ) {
			CallNextHandler( event );
		}
		return;
	}

	ShipClass *ship = ShipClass::GetObjectByID( playerSet, shipDisplay->GetShipID() );
	if ( ship == NULL ) {
		pane->SetNotVisible( true );
		if ( event != NULL ) {
			CallNextHandler( event );
		}
		return;
	}

	ShieldClass *shields = ship->GetShields();
	for ( int i = 0; i < ShieldClass::NUM_SHIELDS; i++ ) {
		UpdateShieldIcon( shields, i );
	}
	if ( event != NULL ) {
		CallNextHandler( event );
	}
}

/*
================
ShieldsDisplay::UpdateShieldIcon

The facing's icon takes the condition colour of the shield's level; a
facing without shields, or shields that are off, shows the first colour.
The bottom icon is in the display's third pane, the others in its first.
================
*/
// FUNCTION: STBC 0x00545e60
void ShieldsDisplay::UpdateShieldIcon( ShieldClass *shields, int shield ) {
	TGIcon *icon = NULL;
	TGPane *pane = (TGPane *)GetNthChild( 0 );
	TGPane *icons = (TGPane *)pane->GetNthChild( 0 );
	TGPane *bottomIcons = (TGPane *)pane->GetNthChild( 2 );
	switch ( shield ) {
	case ShieldClass::TOP_SHIELDS:
		icon = (TGIcon *)icons->GetNthChild( 0 );
		break;
	case ShieldClass::BOTTOM_SHIELDS:
		icon = (TGIcon *)bottomIcons->GetNthChild( 0 );
		break;
	case ShieldClass::FRONT_SHIELDS:
		icon = (TGIcon *)icons->GetNthChild( 1 );
		break;
	case ShieldClass::REAR_SHIELDS:
		icon = (TGIcon *)icons->GetNthChild( 2 );
		break;
	case ShieldClass::LEFT_SHIELDS:
		icon = (TGIcon *)icons->GetNthChild( 3 );
		break;
	case ShieldClass::RIGHT_SHIELDS:
		icon = (TGIcon *)icons->GetNthChild( 4 );
		break;
	}
	if ( shields->GetMaxShields( shield ) > 0.0f && shields->IsOn() ) {
		icon->SetColor( &g_pConditionColors[(int)( shields->GetSingleShieldPercentage( shield ) * 19.0f )] );
	} else {
		icon->SetColor( &g_pConditionColors[0] );
	}
}

/*
================
ShieldsDisplay::WriteToStream

The pane, then the range check ID of every shield watcher.
================
*/
// FUNCTION: STBC 0x00545f50 ?WriteToStream@ShieldsDisplay@@UAE_NPAVTGStream@@@Z
bool ShieldsDisplay::WriteToStream( TGStream *stream ) {
	if ( !TGPane::WriteToStream( stream ) ) {
		return false;
	}
	for ( int shield = 0; shield < ShieldClass::NUM_SHIELDS + 1; shield++ ) {
		for ( int check = 0; check < SHIELD_LEVEL_CHECKS; check++ ) {
			stream->WriteID( shieldChecks[shield][check] );
		}
	}
	return true;
}

/*
================
ShieldsDisplay::ReadFromStream

The pane, then the range check ID of every shield watcher.
================
*/
// FUNCTION: STBC 0x00545fa0 ?ReadFromStream@ShieldsDisplay@@UAE_NPAVTGStream@@@Z
bool ShieldsDisplay::ReadFromStream( TGStream *stream ) {
	if ( !TGPane::ReadFromStream( stream ) ) {
		return false;
	}
	for ( int shield = 0; shield < ShieldClass::NUM_SHIELDS + 1; shield++ ) {
		for ( int check = 0; check < SHIELD_LEVEL_CHECKS; check++ ) {
			shieldChecks[shield][check] = stream->ReadID();
		}
	}
	return true;
}

/*
================
ShieldsDisplay::FixUpPointers

The pane's fix-up; the range check IDs stay as they were saved.
================
*/
// FUNCTION: STBC 0x00545ff0 ?FixUpPointers@ShieldsDisplay@@UAE_NXZ
bool ShieldsDisplay::FixUpPointers( void ) {
	return TGUIObject::FixUpPointers() ? true : false;
}

/*
================
ShieldsDisplay::PostFixUp

After the pane's own fix-up it asks for the first child.
================
*/
// FUNCTION: STBC 0x00546000 ?PostFixUp@ShieldsDisplay@@UAEXXZ
void ShieldsDisplay::PostFixUp( void ) {
	TGUIObject::PostFixUp();
	GetNthChild( 0 );
}
