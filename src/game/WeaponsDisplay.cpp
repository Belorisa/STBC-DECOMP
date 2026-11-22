/*
===============================================================================

	WeaponsDisplay.cpp

===============================================================================
*/

#include <iostream>
#include "WeaponsDisplay.h"
#include "TGString.h"
#include "TGParagraph.h"
#include "TGLocalizationManager.h"
#include "TGLocalizationDatabase.h"
#include "STStylizedWindow.h"
#include <stdio.h>
#include "ShipClass.h"
#include "TorpedoSystem.h"
#include "TorpedoTube.h"
#include "TGIcon.h"
#include "EnergyWeapon.h"
#include "EnergyWeaponProperty.h"
#include "PhaserBank.h"
#include "PulseWeapon.h"
#include "PhaserSystem.h"
#include "PulseWeaponSystem.h"
#include "TacticalControlWindow.h"
#include "STColors.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGTimer.h"
#include "TGTimerManager.h"
#include "TGFloatEvent.h"
#include "FloatRangeWatcher.h"
#include "UtopiaModule.h"
#include "PyEmbed.h"
#include "Game.h"
#include "WeaponSystem.h"
#include "SetClass.h"
#include "TGStream.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "TGModelUtils.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0054cda0 _$E555
// FUNCTION: STBC 0x0054cdc0 _$E554

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0054abe0 _$E32
// FUNCTION: STBC 0x0054abf0 _$E29
// FUNCTION: STBC 0x0054ac00 _$E31
// FUNCTION: STBC 0x0054ac10 _$E30
// FUNCTION: STBC 0x0054ac20 _$E38
// FUNCTION: STBC 0x0054ac30 _$E35
// FUNCTION: STBC 0x0054ac40 _$E37
// FUNCTION: STBC 0x0054ac50 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0054ac60 _$E57
// FUNCTION: STBC 0x0054ac70 _$E56
// FUNCTION: STBC 0x0054ac80 _$E61
// FUNCTION: STBC 0x0054ac90 _$E60
// FUNCTION: STBC 0x0054aca0 _$E72
// FUNCTION: STBC 0x0054acb0 _$E71
// FUNCTION: STBC 0x0054acc0 _$E76
// FUNCTION: STBC 0x0054acd0 _$E75
// FUNCTION: STBC 0x0054ace0 _$E80
// FUNCTION: STBC 0x0054acf0 _$E79
// FUNCTION: STBC 0x0054ad00 _$E84
// FUNCTION: STBC 0x0054ad10 _$E83
// FUNCTION: STBC 0x0054ad20 _$E208
// FUNCTION: STBC 0x0054ad30 _$E207
// FUNCTION: STBC 0x0054ad40 _$E150
// FUNCTION: STBC 0x0054ad50 _$E149
// FUNCTION: STBC 0x0054ad60 _$E163
// FUNCTION: STBC 0x0054ad70 _$E162
// FUNCTION: STBC 0x0054ad80 _$E213
// FUNCTION: STBC 0x0054ad90 _$E212
// FUNCTION: STBC 0x0054b6c0 _$E218
// FUNCTION: STBC 0x0054b6d0 _$E217
// FUNCTION: STBC 0x0054b9d0 _$E222
// FUNCTION: STBC 0x0054b9e0 _$E221
// FUNCTION: STBC 0x0054bfb0 _$E226
// FUNCTION: STBC 0x0054bfc0 _$E225
// FUNCTION: STBC 0x0054c500 _$E233
// FUNCTION: STBC 0x0054c510 _$E232
// FUNCTION: STBC 0x0054c5a0 _$E389
// FUNCTION: STBC 0x0054c5b0 _$E388
// FUNCTION: STBC 0x0054c5c0 _$E393
// FUNCTION: STBC 0x0054c5d0 _$E392
// FUNCTION: STBC 0x0054c5e0 _$E108
// FUNCTION: STBC 0x0054c5f0 _$E107
// FUNCTION: STBC 0x0054c600 _$E264
// FUNCTION: STBC 0x0054c610 _$E263
// FUNCTION: STBC 0x0054c620 _$E271
// FUNCTION: STBC 0x0054c630 _$E270
// FUNCTION: STBC 0x0054c640 _$E275
// FUNCTION: STBC 0x0054c650 _$E274
// FUNCTION: STBC 0x0054c660 _$E279
// FUNCTION: STBC 0x0054c670 _$E278
// FUNCTION: STBC 0x0054c680 _$E283
// FUNCTION: STBC 0x0054c690 _$E282
// FUNCTION: STBC 0x0054c6a0 _$E289
// FUNCTION: STBC 0x0054c6b0 _$E288
// FUNCTION: STBC 0x0054c6c0 _$E397
// FUNCTION: STBC 0x0054c6d0 _$E396
// FUNCTION: STBC 0x0054c6e0 _$E401
// FUNCTION: STBC 0x0054c6f0 _$E400
// FUNCTION: STBC 0x0054c700 _$E405
// FUNCTION: STBC 0x0054c710 _$E404
// FUNCTION: STBC 0x0054c720 _$E409
// FUNCTION: STBC 0x0054c730 _$E408
// FUNCTION: STBC 0x0054c740 _$E413
// FUNCTION: STBC 0x0054c750 _$E412
// FUNCTION: STBC 0x0054c760 _$E417
// FUNCTION: STBC 0x0054c770 _$E416
// FUNCTION: STBC 0x0054c780 _$E314
// FUNCTION: STBC 0x0054c790 _$E313
// FUNCTION: STBC 0x0054c7a0 _$E421
// FUNCTION: STBC 0x0054c7b0 _$E420
// FUNCTION: STBC 0x0054c7c0 _$E425
// FUNCTION: STBC 0x0054c7d0 _$E424
// FUNCTION: STBC 0x0054c7e0 _$E429
// FUNCTION: STBC 0x0054c7f0 _$E428
// FUNCTION: STBC 0x0054c800 _$E113
// FUNCTION: STBC 0x0054c810 _$E112
// FUNCTION: STBC 0x0054c820 _$E433
// FUNCTION: STBC 0x0054c830 _$E432
// FUNCTION: STBC 0x0054c840 _$E118
// FUNCTION: STBC 0x0054c850 _$E117
// FUNCTION: STBC 0x0054c860 _$E437
// FUNCTION: STBC 0x0054c870 _$E436
// FUNCTION: STBC 0x0054c880 _$E441
// FUNCTION: STBC 0x0054c890 _$E440
// FUNCTION: STBC 0x0054c8a0 _$E445
// FUNCTION: STBC 0x0054c8b0 _$E444
// FUNCTION: STBC 0x0054c8c0 _$E449
// FUNCTION: STBC 0x0054c8d0 _$E448
// FUNCTION: STBC 0x0054c8e0 _$E127
// FUNCTION: STBC 0x0054c8f0 _$E126
// FUNCTION: STBC 0x0054c900 _$E453
// FUNCTION: STBC 0x0054c910 _$E452
// FUNCTION: STBC 0x0054c920 _$E457
// FUNCTION: STBC 0x0054c930 _$E456
// FUNCTION: STBC 0x0054c940 _$E461
// FUNCTION: STBC 0x0054c950 _$E460
// FUNCTION: STBC 0x0054c960 _$E133
// FUNCTION: STBC 0x0054c970 _$E132
// FUNCTION: STBC 0x0054c980 _$E138
// FUNCTION: STBC 0x0054c990 _$E137
// FUNCTION: STBC 0x0054c9a0 _$E168
// FUNCTION: STBC 0x0054c9b0 _$E167
// FUNCTION: STBC 0x0054c9c0 _$E465
// FUNCTION: STBC 0x0054c9d0 _$E464
// FUNCTION: STBC 0x0054c9e0 _$E173
// FUNCTION: STBC 0x0054c9f0 _$E172
// FUNCTION: STBC 0x0054ca00 _$E469
// FUNCTION: STBC 0x0054ca10 _$E468
// FUNCTION: STBC 0x0054ca20 _$E473
// FUNCTION: STBC 0x0054ca30 _$E472
// FUNCTION: STBC 0x0054ca40 _$E178
// FUNCTION: STBC 0x0054ca50 _$E177
// FUNCTION: STBC 0x0054ca60 _$E477
// FUNCTION: STBC 0x0054ca70 _$E476
// FUNCTION: STBC 0x0054ca80 _$E143
// FUNCTION: STBC 0x0054ca90 _$E142
// FUNCTION: STBC 0x0054caa0 _$E481
// FUNCTION: STBC 0x0054cab0 _$E480
// FUNCTION: STBC 0x0054cac0 _$E485
// FUNCTION: STBC 0x0054cad0 _$E484
// FUNCTION: STBC 0x0054cae0 _$E183
// FUNCTION: STBC 0x0054caf0 _$E182
// FUNCTION: STBC 0x0054cb00 _$E188
// FUNCTION: STBC 0x0054cb10 _$E187
// FUNCTION: STBC 0x0054cb20 _$E489
// FUNCTION: STBC 0x0054cb30 _$E488
// FUNCTION: STBC 0x0054cb40 _$E493
// FUNCTION: STBC 0x0054cb50 _$E492
// FUNCTION: STBC 0x0054cb60 _$E497
// FUNCTION: STBC 0x0054cb70 _$E496
// FUNCTION: STBC 0x0054cb80 _$E501
// FUNCTION: STBC 0x0054cb90 _$E500
// FUNCTION: STBC 0x0054cba0 _$E505
// FUNCTION: STBC 0x0054cbb0 _$E504
// FUNCTION: STBC 0x0054cbc0 _$E509
// FUNCTION: STBC 0x0054cbd0 _$E508
// FUNCTION: STBC 0x0054cbe0 _$E513
// FUNCTION: STBC 0x0054cbf0 _$E512
// FUNCTION: STBC 0x0054cc00 _$E517
// FUNCTION: STBC 0x0054cc10 _$E516
// FUNCTION: STBC 0x0054cc20 _$E193
// FUNCTION: STBC 0x0054cc30 _$E192
// FUNCTION: STBC 0x0054cc40 _$E521
// FUNCTION: STBC 0x0054cc50 _$E520
// FUNCTION: STBC 0x0054cc60 _$E198
// FUNCTION: STBC 0x0054cc70 _$E197
// FUNCTION: STBC 0x0054cc80 _$E203
// FUNCTION: STBC 0x0054cc90 _$E202
// FUNCTION: STBC 0x0054cdd0 _$E525
// FUNCTION: STBC 0x0054cde0 _$E524
// FUNCTION: STBC 0x0054cdf0 _$E530
// FUNCTION: STBC 0x0054ce00 _$E529
// FUNCTION: STBC 0x0054ce10 _$E534
// FUNCTION: STBC 0x0054ce20 _$E533

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x0054ada0 _$E540
// FUNCTION: STBC 0x0054adb0 _$E536
// FUNCTION: STBC 0x0054adc0 _$E539
// FUNCTION: STBC 0x0054add0 _$E537

// DATA: STBC 0x0098ac98
TGEventHandlerTable WeaponsDisplay::classHandlers;

// The colors of a torpedo tube's icon that STColors doesn't hold (invented names),
// and their dynamic initialisers.
// FUNCTION: STBC 0x0054ae20 _$E543
// FUNCTION: STBC 0x0054ae30 _$E542
// FUNCTION: STBC 0x0054ae60 _$E546
// FUNCTION: STBC 0x0054ae70 _$E545
static NiColorA	tubeDestroyedColor( 0.0f, 0.0f, 0.0f, 1.0f );
static NiColorA	tubeDisabledColor( 0.25f, 0.25f, 0.25f, 1.0f );

// The impulse setting shown, from the ship's impulse fraction (invented names).
#define IMPULSE_STEPS		9.0f
#define IMPULSE_ROUNDING	0.1f
// The display refreshes every half second (invented name).
static const float REFRESH_INTERVAL = 0.5f;
// The charge levels a weapon's watcher reports: nearly empty, then every
// twentieth (invented names).
#define CHARGE_EMPTY		0.001f
#define CHARGE_STEP			0.05f
#define NUM_CHARGE_STEPS	20

/*
================
WeaponsDisplay::Cast

The object as a WeaponsDisplay; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0054adf0
WeaponsDisplay *WeaponsDisplay::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( WEAPONSDISPLAY_TYPE ) ) {
		return (WeaponsDisplay *)object;
	}
	return NULL;
}

/*
================
WeaponsDisplay::WeaponsDisplay

A stylized window named "Speed"; Python builds the rest.
================
*/
// FUNCTION: STBC 0x0054aea0
WeaponsDisplay::WeaponsDisplay( float width, float height ) : STStylizedWindow( "StylizedWindow", "WeaponsDisplay", &TGString( "Speed" ), 0.0f, 0.0f, NULL, false, 0.0f, 0.0f, &g_kInterfaceBorderColor ), chargeChecks( 2 ) {
	shipID = 0;
	refreshTimerID = 0;
	TGPane::GetFirstChild()->SetNotBatchChildPolys( true );
	CreateInterface( width, height );
}

/*
================
WeaponsDisplay::IsTypeOf
================
*/
// FUNCTION: STBC 0x0054af90
bool WeaponsDisplay::IsTypeOf( int type ) {
	if ( type == WEAPONSDISPLAY_TYPE ) {
		return true;
	}
	return STStylizedWindow::IsTypeOf( type );
}

/*
================
WeaponsDisplay::~WeaponsDisplay
================
*/
// FUNCTION: STBC 0x0054b070
WeaponsDisplay::~WeaponsDisplay( void ) {
	StopWatchingShip();
	g_kTimerManager.DeleteTimer( refreshTimerID );
}

/*
================
WeaponsDisplay::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x0054b120
void WeaponsDisplay::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&WeaponsDisplay::Update, "WeaponsDisplay::Update" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&WeaponsDisplay::HandleTargetChanged, "WeaponsDisplay::HandleTargetChanged" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&WeaponsDisplay::PeriodicUpdate, "WeaponsDisplay::PeriodicUpdate" );
}

/*
================
WeaponsDisplay::RegisterHandlers

Update and HandleTargetChanged are added for each instance.
================
*/
// FUNCTION: STBC 0x0054b160
void WeaponsDisplay::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_WEAPON_DISPLAY_REFRESH_TIMER, "WeaponsDisplay::PeriodicUpdate" );
}

/*
================
WeaponsDisplay::CreateInterface
================
*/
// FUNCTION: STBC 0x0054b180
void WeaponsDisplay::CreateInterface( float width, float height ) {
	PyObject *self = VirtualSlot08();
	PyEmbed::CallFunction( "Tactical.Interface.WeaponsDisplay", "Create", NULL, NULL, "(Off)", self, (double)width, (double)height );
	Py_DECREF( self );
}

/*
================
WeaponsDisplay::StartRefreshTimer
================
*/
inline void WeaponsDisplay::StartRefreshTimer( void ) {
	TGEvent *event = new TGEvent;
	event->SetSource( this );
	event->SetDestination( this );
	event->SetEventType( ET_WEAPON_DISPLAY_REFRESH_TIMER );
	TGTimer *timer = new TGTimer;
	timer->SetTimerStart( g_kUtopiaModule.GetGameTime() + REFRESH_INTERVAL );
	timer->SetEvent( event );
	refreshTimerID = timer->GetObjID();
	g_kTimerManager.AddTimer( timer );
}

/*
================
WeaponsDisplay::HandleTargetChanged

The targeting colours follow the ship's new target.
================
*/
// FUNCTION: STBC 0x0054b1d0
void WeaponsDisplay::HandleTargetChanged( TGEvent *event ) {
	ShipClass *ship = ShipClass::Cast( event->GetDestination() );
	if ( ship != NULL ) {
		UpdateTargeting( ship );
		Layout();
	}
	CallNextHandler( event );
}

/*
================
WeaponsDisplay::PeriodicUpdate

Refreshes the targeting colours and starts the next timer.
================
*/
// FUNCTION: STBC 0x0054b210
void WeaponsDisplay::PeriodicUpdate( TGEvent *event ) {
	ShipClass *ship = ShipClass::GetObjectByID( NULL, GetShipID() );
	if ( ship != NULL ) {
		UpdateTargeting( ship );
		Layout();
	}
	StartRefreshTimer();
}

/*
================
WeaponsDisplay::WatchShip

Listens for the events of the player's ship and its torpedo tubes, and
watches the charge of its phasers and pulse weapons.
================
*/
// FUNCTION: STBC 0x0054b2f0
void WeaponsDisplay::WatchShip( void ) {
	Game *game = Game::GetCurrentGame();
	if ( game == NULL ) {
		return;
	}
	ShipClass *ship = ShipClass::Cast( game->GetPlayer() );
	if ( ship == NULL ) {
		return;
	}
	TorpedoSystem *torpedoes = ship->GetTorpedoSystem();
	if ( torpedoes != NULL ) {
		int numTubes = torpedoes->GetNumChildSubsystems();
		for ( int i = 0; i < numTubes; i++ ) {
			ShipSubsystem *tube = torpedoes->GetChildSubsystem( i );
			g_kEventManager.AddBroadcastMethodHandler( ET_TORPEDO_RELOAD, this, "WeaponsDisplay::Update", tube );
			g_kEventManager.AddBroadcastMethodHandler( ET_TORPEDO_FIRED, this, "WeaponsDisplay::Update", tube );
		}
	}
	g_kEventManager.AddBroadcastMethodHandler( ET_ENTERED_SET, this, "WeaponsDisplay::Update", ship );
	g_kEventManager.AddBroadcastMethodHandler( ET_WEAPON_CHARGE_UPDATED, this, "WeaponsDisplay::Update", ship );
	g_kEventManager.AddBroadcastMethodHandler( ET_TARGET_WAS_CHANGED, this, "WeaponsDisplay::HandleTargetChanged", ship );
	g_kEventManager.AddBroadcastMethodHandler( ET_SUBSYSTEM_STATE_CHANGED, this, "WeaponsDisplay::Update", ship );
	g_kEventManager.AddBroadcastMethodHandler( ET_SUBSYSTEM_DISABLED, this, "WeaponsDisplay::Update", ship );
	g_kEventManager.AddBroadcastMethodHandler( ET_SUBSYSTEM_OPERATIONAL, this, "WeaponsDisplay::Update", ship );
	g_kEventManager.AddBroadcastMethodHandler( ET_SUBSYSTEM_DESTROYED, this, "WeaponsDisplay::Update", ship );

	PhaserSystem *phasers = ship->GetPhaserSystem();
	PulseWeaponSystem *pulseWeapons = ship->GetPulseWeaponSystem();
	if ( phasers != NULL ) {
		WatchWeaponSystem( phasers );
	}
	if ( pulseWeapons != NULL ) {
		WatchWeaponSystem( pulseWeapons );
	}
	StartRefreshTimer();
}

/*
================
WeaponsDisplay::WatchWeaponSystem
================
*/
// FUNCTION: STBC 0x0054b510
void WeaponsDisplay::WatchWeaponSystem( ShipSubsystem *system ) {
	for ( int i = 0; i < system->GetNumChildSubsystems(); i++ ) {
		WatchWeaponCharge( EnergyWeapon::Cast( system->GetChildSubsystem( i ) ) );
	}
}

/*
================
WeaponsDisplay::WatchWeaponCharge

Asks the weapon's charge watcher to tell the ship when the charge crosses
nearly empty or a twentieth step, and keeps the checks' IDs.
================
*/
// FUNCTION: STBC 0x0054b550
void WeaponsDisplay::WatchWeaponCharge( EnergyWeapon *weapon ) {
	WeaponChargeChecks *checks = new WeaponChargeChecks;
	ShipClass *ship = weapon->GetParentShip();
	checks->weapon = weapon;
	checks->numChecks = NUM_CHARGE_STEPS + 1;
	checks->checkIDs = new int[NUM_CHARGE_STEPS + 1];
	FloatRangeWatcher *watcher = weapon->GetChargeWatcher();

	TGFloatEvent *event = new TGFloatEvent;
	event->SetDestination( ship );
	event->SetEventType( ET_WEAPON_CHARGE_UPDATED );
	checks->checkIDs[0] = watcher->AddRangeCheck( CHARGE_EMPTY, 3, event );
	for ( int i = 1; i <= NUM_CHARGE_STEPS; i++ ) {
		float level = i * CHARGE_STEP;
		event = new TGFloatEvent;
		event->SetDestination( ship );
		event->SetEventType( ET_WEAPON_CHARGE_UPDATED );
		checks->checkIDs[i] = watcher->AddRangeCheck( level, 3, event );
	}
	chargeChecks.AddTail( checks );
}

/*
================
WeaponsDisplay::StopWatchingShip

Removes the handlers WatchShip added for the ship of shipID and the
charge checks, and deletes the refresh timers.
================
*/
// FUNCTION: STBC 0x0054b6e0
void WeaponsDisplay::StopWatchingShip( void ) {
	ShipClass *ship = ShipClass::GetObjectByID( NULL, GetShipID() );
	if ( ship != NULL ) {
		TorpedoSystem *torpedoes = ship->GetTorpedoSystem();
		if ( torpedoes != NULL ) {
			int numTubes = torpedoes->GetNumChildSubsystems();
			for ( int i = 0; i < numTubes; i++ ) {
				ShipSubsystem *tube = torpedoes->GetChildSubsystem( i );
				g_kEventManager.RemoveBroadcastHandler( ET_TORPEDO_RELOAD, this, "WeaponsDisplay::Update", tube );
				g_kEventManager.RemoveBroadcastHandler( ET_TORPEDO_FIRED, this, "WeaponsDisplay::Update", tube );
			}
		}
		g_kEventManager.RemoveBroadcastHandler( ET_ENTERED_SET, this, "WeaponsDisplay::Update", ship );
		g_kEventManager.RemoveBroadcastHandler( ET_WEAPON_CHARGE_UPDATED, this, "WeaponsDisplay::Update", ship );
		g_kEventManager.RemoveBroadcastHandler( ET_SUBSYSTEM_STATE_CHANGED, this, "WeaponsDisplay::Update", ship );
		g_kEventManager.RemoveBroadcastHandler( ET_TARGET_WAS_CHANGED, this, "WeaponsDisplay::HandleTargetChanged", ship );
		g_kEventManager.RemoveBroadcastHandler( ET_SUBSYSTEM_DISABLED, this, "WeaponsDisplay::Update", ship );
		g_kEventManager.RemoveBroadcastHandler( ET_SUBSYSTEM_OPERATIONAL, this, "WeaponsDisplay::Update", ship );
		g_kEventManager.RemoveBroadcastHandler( ET_SUBSYSTEM_DESTROYED, this, "WeaponsDisplay::Update", ship );

		NiTListIterator pos = chargeChecks.GetHeadPos();
		while ( pos ) {
			WeaponChargeChecks *checks = chargeChecks.GetNext( pos );
			FloatRangeWatcher *watcher = checks->weapon->GetChargeWatcher();
			for ( int i = 0; i < checks->numChecks; i++ ) {
				watcher->RemoveRangeCheck( checks->checkIDs[i] );
			}
		}
	}

	NiTListIterator pos = chargeChecks.GetHeadPos();
	while ( pos ) {
		WeaponChargeChecks *checks = chargeChecks.GetNext( pos );
		delete checks->checkIDs;
		delete checks;
	}
	chargeChecks.RemoveAll();
	g_kTimerManager.DeleteTimersForDestination( this, ET_WEAPON_DISPLAY_REFRESH_TIMER );
}

/*
================
WeaponsDisplay::SetShipID

Shows the ship of the ID; 0 shows none.
================
*/
// FUNCTION: STBC 0x0054b950
void WeaponsDisplay::SetShipID( long id ) {
	if ( shipID != 0 ) {
		StopWatchingShip();
	}
	shipID = id;
	CreateInterface( GetWidth(), GetHeight() );
	if ( shipID == 0 ) {
		StopWatchingShip();
		Update( NULL );
		return;
	}
	SetShipIcon();
	WatchShip();
	GetNthChild( 0 )->SetVisible( true );
	Update( NULL );
}

/*
================
WeaponsDisplay::SetShipIcon
================
*/
// FUNCTION: STBC 0x0054b9f0
void WeaponsDisplay::SetShipIcon( void ) {
	PyObject *self = VirtualSlot08();
	PyEmbed::CallFunction( "Tactical.Interface.WeaponsDisplay", "SetShipIcon", NULL, NULL, "(O)", self );
	Py_DECREF( self );
}

/*
================
WeaponsDisplay::Update

Recolours what the event concerns: a set entry or a weapon's state
change redraws everything, a destroyed ship hides the icons. Without an
event (SetShipID) everything is redrawn.
================
*/
// FUNCTION: STBC 0x0054ba30
void WeaponsDisplay::Update( TGEvent *event ) {
	TGPane *icons = TGPane::Cast( GetNthChild( 0 ) );
	ShipClass *ship = ShipClass::GetObjectByID( NULL, shipID );
	bool weaponChanged = false;
	if ( event != NULL ) {
		int type = event->GetEventType();
		if ( ( type == ET_SUBSYSTEM_DISABLED || type == ET_SUBSYSTEM_OPERATIONAL || type == ET_SUBSYSTEM_DESTROYED ) && Weapon::Cast( event->GetSource() ) != NULL ) {
			weaponChanged = true;
		}
	}
	if ( event == NULL || event->GetEventType() == ET_ENTERED_SET || weaponChanged == true ) {
		Unknown54BEE0( ship );
		Unknown54C110( ship );
		UpdateTargeting( ship );
		Layout();
	} else {
		if ( ship == NULL ) {
			CallNextHandler( event );
			return;
		}
		int type = event->GetEventType();
		if ( type == ET_OBJECT_DESTROYED ) {
			shipID = 0;
			icons->SetNotVisible( true );
		} else if ( type == ET_SUBSYSTEM_STATE_CHANGED ) {
			if ( event->GetSource()->IsTypeOf( WEAPONSYSTEM_TYPE ) ) {
				TGObject *system = event->GetSource();
				if ( system == ship->GetPhaserSystem() || system == ship->GetPulseWeaponSystem() ) {
					Unknown54BEE0( ship );
				} else if ( system == ship->GetTorpedoSystem() ) {
					Unknown54C110( ship );
				}
			}
		} else if ( type == ET_WEAPON_CHARGE_UPDATED ) {
			Unknown54BEE0( ship );
		} else {
			Unknown54C110( ship );
		}
	}
	if ( event != NULL ) {
		CallNextHandler( event );
	}
}

/*
================
WeaponsDisplay::UpdateTargeting
================
*/
// FUNCTION: STBC 0x0054bb70
void WeaponsDisplay::UpdateTargeting( ShipClass *ship ) {
	TGPane *icons = TGPane::Cast( GetNthChild( 0 ) );

	PhaserSystem *phasers = NULL;
	if ( ship != NULL ) {
		phasers = ship->GetPhaserSystem();
	}
	TGPane *above = TGPane::Cast( icons->GetNthChild( 2 ) );
	TGPane *below = TGPane::Cast( icons->GetNthChild( 6 ) );
	if ( phasers != NULL ) {
		UpdateWeaponTargeting( phasers, above, below );
	}

	PulseWeaponSystem *pulseWeapons = NULL;
	if ( ship != NULL ) {
		pulseWeapons = ship->GetPulseWeaponSystem();
	}
	above = TGPane::Cast( icons->GetNthChild( 4 ) );
	below = TGPane::Cast( icons->GetNthChild( 8 ) );
	if ( pulseWeapons != NULL ) {
		UpdateWeaponTargeting( pulseWeapons, above, below );
	}
}

/*
================
WeaponsDisplay::UpdateWeaponTargeting

Shows the targeting icon of each energy weapon of the system that can hit
the ship's target (the targeted subsystem's point), while the system is
on and the weapon not disabled, and tells the tactical window which can
fire (row 0 phasers, 1 pulse weapons).
================
*/
// FUNCTION: STBC 0x0054bc30
void WeaponsDisplay::UpdateWeaponTargeting( WeaponSystem *system, TGPane *above, TGPane *below ) {
	ShipClass *ship = system->GetParentShip();
	ObjectClass *target = ship->GetTarget();
	ship->GetTargetSubsystem();
	NiPoint3 targetPoint = NiPoint3::ZERO;
	bool hasTarget = target != NULL;
	if ( target != NULL ) {
		targetPoint = TGModelUtils::LocalToWorldPoint( target->GetNode(), ship->GetTargetOffset() );
	}

	int numWeapons = system->GetNumChildSubsystems();
	int aboveIndex = 0;
	int belowIndex = 0;
	TacticalControlWindow::GetTacticalControlWindow()->SetUnknown4C( -1, 0, false );
	for ( int i = 0; i < numWeapons; i++ ) {
		EnergyWeapon *weapon = EnergyWeapon::Cast( system->GetChildSubsystem( i ) );
		if ( weapon == NULL ) {
			continue;
		}
		EnergyWeaponProperty *property = weapon->GetProperty();
		if ( property == NULL ) {
			continue;
		}
		TGPane *icons;
		int slot;
		if ( property->IsIconAboveShip() == true ) {
			slot = aboveIndex++;
			icons = above;
		} else {
			slot = belowIndex++;
			icons = below;
		}
		TGIcon *icon = TGIcon::Cast( icons->GetNthChild( slot ) );

		bool canHit = false;
		if ( hasTarget == true ) {
			NiPoint3 localTarget = TGModelUtils::WorldToLocalPoint( ship->GetNode(), targetPoint );
			NiPoint3 position;
			if ( weapon->IsTypeOf( PHASERBANK_TYPE ) ) {
				float widthAngle;
				if ( ( (PhaserBank *)weapon )->FindFiringPosition( &localTarget, &position, &widthAngle ) ) {
					canHit = true;
				}
			} else if ( weapon->IsTypeOf( PULSEWEAPON_TYPE ) ) {
				if ( ( (PulseWeapon *)weapon )->CanHitTarget( &targetPoint, &position ) ) {
					canHit = true;
				}
			}
			if ( canHit ) {
				if ( weapon->IsTypeOf( PHASERBANK_TYPE ) ) {
					TacticalControlWindow::GetTacticalControlWindow()->SetUnknown4C( 0, i, true );
				} else if ( weapon->IsTypeOf( PULSEWEAPON_TYPE ) ) {
					TacticalControlWindow::GetTacticalControlWindow()->SetUnknown4C( 1, i, true );
				}
			}
		}

		if ( icon != NULL ) {
			if ( canHit && system->IsOn() == true && !weapon->IsDisabled() ) {
				icon->SetVisible( false );
			} else {
				icon->SetNotVisible( false );
			}
		}
	}
}

/*
================
WeaponsDisplay::Unknown54BEE0

Colors the phaser and pulse weapon icons of the ship (invented name).
================
*/
// FUNCTION: STBC 0x0054bee0
void WeaponsDisplay::Unknown54BEE0( ShipClass *ship ) {
	TGPane *icons = TGPane::Cast( GetNthChild( 0 ) );
	TacticalControlWindow::GetTacticalControlWindow()->SetUnknown8C( -1, 0, false );

	PhaserSystem *phasers = NULL;
	if ( ship != NULL ) {
		phasers = ship->GetPhaserSystem();
	}
	TGPane *above = TGPane::Cast( icons->GetNthChild( 1 ) );
	TGPane *below = TGPane::Cast( icons->GetNthChild( 7 ) );
	if ( phasers != NULL ) {
		Unknown54BFD0( phasers, above, below );
	}

	PulseWeaponSystem *pulseWeapons = NULL;
	if ( ship != NULL ) {
		pulseWeapons = ship->GetPulseWeaponSystem();
	}
	above = TGPane::Cast( icons->GetNthChild( 3 ) );
	below = TGPane::Cast( icons->GetNthChild( 9 ) );
	if ( pulseWeapons != NULL ) {
		Unknown54BFD0( pulseWeapons, above, below );
	}
}

/*
================
WeaponsDisplay::Unknown54BFD0

Colors the icon of each energy weapon of the system by its charge: the icons
above the ship are taken from the first pane, the others from the second.
A weapon that can fire tells the tactical window which row of its table to
set (phasers 0, pulse weapons 1; invented description).
================
*/
// FUNCTION: STBC 0x0054bfd0
void WeaponsDisplay::Unknown54BFD0( ShipSubsystem *system, TGPane *above, TGPane *below ) {
	int numWeapons = system->GetNumChildSubsystems();
	int belowIndex = 0;
	int aboveIndex = 0;
	for ( int i = 0; i < numWeapons; i++ ) {
		EnergyWeapon *weapon = EnergyWeapon::Cast( system->GetChildSubsystem( i ) );
		if ( weapon == NULL ) {
			continue;
		}
		EnergyWeaponProperty *property = weapon->GetProperty();
		if ( property == NULL ) {
			continue;
		}
		TGPane *icons;
		int slot;
		if ( property->IsIconAboveShip() == true ) {
			slot = aboveIndex++;
			icons = above;
		} else {
			slot = belowIndex++;
			icons = below;
		}
		TGIcon *icon = TGIcon::Cast( icons->GetNthChild( slot ) );
		if ( icon == NULL ) {
			continue;
		}

		float charge = weapon->GetChargePercentage();
		if ( charge <= 0.0f ) {
			icon->SetColor( &g_pConditionColors[0] );
		} else if ( weapon->IsDisabled() == true ) {
			icon->SetColor( &tubeDisabledColor );
		} else {
			if ( weapon->CanFire() ) {
				if ( weapon->IsTypeOf( PHASERBANK_TYPE ) ) {
					TacticalControlWindow::GetTacticalControlWindow()->SetUnknown8C( 0, i, true );
				} else if ( weapon->IsTypeOf( PULSEWEAPON_TYPE ) ) {
					TacticalControlWindow::GetTacticalControlWindow()->SetUnknown8C( 1, i, true );
				}
			}
			icon->SetColor( g_pConditionColors - (int)( charge * -19.0f ) );
		}
	}
}

/*
================
WeaponsDisplay::Unknown54C110

Colors the torpedo tube icons by the state of the ship's tubes: destroyed, disabled,
loaded with the torpedo system on, or not ready.
================
*/
// FUNCTION: STBC 0x0054c110
void WeaponsDisplay::Unknown54C110( ShipClass *ship ) {
	TGPane *tubeIcons = (TGPane *)( (TGPane *)GetNthChild( 0 ) )->GetNthChild( 0 );
	if ( ship == NULL ) {
		return;
	}
	TorpedoSystem *torpedoSystem = ship->GetTorpedoSystem();
	if ( torpedoSystem == NULL ) {
		return;
	}

	int numTubes = torpedoSystem->GetNumChildSubsystems();
	bool systemOn = true;
	if ( !torpedoSystem->IsOn() ) {
		systemOn = false;
	}
	for ( int i = 0; i < numTubes; i++ ) {
		TorpedoTube *tube = (TorpedoTube *)torpedoSystem->GetChildSubsystem( i );
		if ( tube == NULL ) {
			return;
		}
		TGIcon *icon = (TGIcon *)tubeIcons->GetNthChild( i );
		if ( icon != NULL ) {
			if ( tube->GetCondition() <= 0.0f ) {
				icon->SetColor( &tubeDestroyedColor );
			} else if ( tube->IsDisabled() == true ) {
				icon->SetColor( &tubeDisabledColor );
			} else if ( tube->GetNumReady() != 0 && systemOn == true ) {
				icon->SetColor( &g_kTIPhotonReadyColor );
			} else {
				icon->SetColor( &g_kTIPhotonNotReadyColor );
			}
		}
	}
}

/*
================
WeaponsDisplay::Unknown54C200

Writes the ship's speed into the window's name paragraph: "Speed" and the
impulse setting, a velocity in kph, and an "R" when the ship is reversing.
Called by TacticalControlWindow::VirtualSlot83 with the player's ship.
================
*/
// FUNCTION: STBC 0x0054c200
void WeaponsDisplay::Unknown54C200( ShipClass *ship ) {
	if ( ship == NULL ) {
		return;
	}
	TGParagraph *paragraph = GetNameParagraph();
	if ( paragraph == NULL ) {
		return;
	}

	// kilometres per second to kph, as PlayerReticleWindow::Unknown512310 shows it
	int speed = (int)( UtopiaModule::ConvertGameUnitsToKilometers( ship->GetVelocity().Length() ) * 3600.0f );
	char text[32];
	bool reverse = false;
	if ( ship->IsDoingInSystemWarp() == true ) {
		sprintf( text, "9+ : %d ", speed );
	} else if ( ship->IsReverse() ) {
		NiPoint3 forward = ship->GetWorldForward();
		NiPoint3 velocity = ship->GetVelocity();
		reverse = true;
		if ( forward.Dot( velocity ) < 0.0f ) {
			sprintf( text, " : -%d ", speed );
		} else {
			sprintf( text, " : %d ", speed );
		}
	} else {
		sprintf( text, "%d : %d ", (int)( ship->GetImpulse() * IMPULSE_STEPS + IMPULSE_ROUNDING ), speed );
	}

	TGString caption( "" );
	TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Bridge Menus.tgl", true );
	caption = *database->GetString( "Speed" ) + " ";
	if ( reverse ) {
		caption.Unknown6F4970( database->GetString( "ReverseLetter" ) );
	}
	caption.Unknown6F4970( &( TGString( text ) + TGString( " " ) + *database->GetString( "kph" ) ) );
	paragraph->SetString( &caption, true );
	g_kLocalizationManager.Unload( database );
}

/*
================
WeaponsDisplay::WriteToStream
================
*/
// FUNCTION: STBC 0x0054c520
bool WeaponsDisplay::WriteToStream( TGStream *stream ) {
	if ( !STStylizedWindow::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( shipID );
	stream->WriteID( refreshTimerID );
	return true;
}

/*
================
WeaponsDisplay::ReadFromStream
================
*/
// FUNCTION: STBC 0x0054c560
bool WeaponsDisplay::ReadFromStream( TGStream *stream ) {
	if ( !STStylizedWindow::ReadFromStream( stream ) ) {
		return false;
	}
	shipID = stream->ReadID();
	refreshTimerID = stream->ReadID();
	return true;
}
