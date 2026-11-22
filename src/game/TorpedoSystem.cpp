/*
===============================================================================

	TorpedoSystem.cpp

===============================================================================
*/

#include "TorpedoSystem.h"
#include "TorpedoSystemProperty.h"
#include "TorpedoTube.h"
#include "Game.h"
#include "ShipClass.h"
#include "TGCharEvent.h"
#include "TGEvent.h"
#include "TacticalControlWindow.h"
#include "WeaponsDisplay.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGStream.h"
#include <iostream>
#include "UtopiaModule.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0057c140 _$E499
// FUNCTION: STBC 0x0057c160 _$E498

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0057aec0 _$E79
// FUNCTION: STBC 0x0057aed0 _$E78
// FUNCTION: STBC 0x0057aee0 _$E278
// FUNCTION: STBC 0x0057aef0 _$E277
// FUNCTION: STBC 0x0057af00 _$E90
// FUNCTION: STBC 0x0057af10 _$E89
// FUNCTION: STBC 0x0057af20 _$E94
// FUNCTION: STBC 0x0057af30 _$E93
// FUNCTION: STBC 0x0057af40 _$E98
// FUNCTION: STBC 0x0057af50 _$E97
// FUNCTION: STBC 0x0057af60 _$E102
// FUNCTION: STBC 0x0057af70 _$E101
// FUNCTION: STBC 0x0057af80 _$E219
// FUNCTION: STBC 0x0057af90 _$E218
// FUNCTION: STBC 0x0057b960 _$E161
// FUNCTION: STBC 0x0057b970 _$E160
// FUNCTION: STBC 0x0057b980 _$E174
// FUNCTION: STBC 0x0057b990 _$E173
// FUNCTION: STBC 0x0057b9a0 _$E224
// FUNCTION: STBC 0x0057b9b0 _$E223
// FUNCTION: STBC 0x0057b9c0 _$E283
// FUNCTION: STBC 0x0057b9d0 _$E282
// FUNCTION: STBC 0x0057b9e0 _$E228
// FUNCTION: STBC 0x0057b9f0 _$E227
// FUNCTION: STBC 0x0057ba00 _$E287
// FUNCTION: STBC 0x0057ba10 _$E286
// FUNCTION: STBC 0x0057ba20 _$E294
// FUNCTION: STBC 0x0057ba30 _$E293
// FUNCTION: STBC 0x0057ba40 _$E335
// FUNCTION: STBC 0x0057ba50 _$E334
// FUNCTION: STBC 0x0057ba60 _$E339
// FUNCTION: STBC 0x0057ba70 _$E338
// FUNCTION: STBC 0x0057ba80 _$E119
// FUNCTION: STBC 0x0057ba90 _$E118
// FUNCTION: STBC 0x0057baa0 _$E244
// FUNCTION: STBC 0x0057bab0 _$E243
// FUNCTION: STBC 0x0057bac0 _$E251
// FUNCTION: STBC 0x0057bad0 _$E250
// FUNCTION: STBC 0x0057bae0 _$E255
// FUNCTION: STBC 0x0057baf0 _$E254
// FUNCTION: STBC 0x0057bb00 _$E259
// FUNCTION: STBC 0x0057bb10 _$E258
// FUNCTION: STBC 0x0057bb20 _$E263
// FUNCTION: STBC 0x0057bb30 _$E262
// FUNCTION: STBC 0x0057bb40 _$E269
// FUNCTION: STBC 0x0057bb50 _$E268
// FUNCTION: STBC 0x0057bb60 _$E343
// FUNCTION: STBC 0x0057bb70 _$E342
// FUNCTION: STBC 0x0057bb80 _$E347
// FUNCTION: STBC 0x0057bb90 _$E346
// FUNCTION: STBC 0x0057bba0 _$E351
// FUNCTION: STBC 0x0057bbb0 _$E350
// FUNCTION: STBC 0x0057bbc0 _$E355
// FUNCTION: STBC 0x0057bbd0 _$E354
// FUNCTION: STBC 0x0057bbe0 _$E359
// FUNCTION: STBC 0x0057bbf0 _$E358
// FUNCTION: STBC 0x0057bc00 _$E363
// FUNCTION: STBC 0x0057bc10 _$E362
// FUNCTION: STBC 0x0057bc20 _$E367
// FUNCTION: STBC 0x0057bc30 _$E366
// FUNCTION: STBC 0x0057bc40 _$E371
// FUNCTION: STBC 0x0057bc50 _$E370
// FUNCTION: STBC 0x0057bc60 _$E375
// FUNCTION: STBC 0x0057bc70 _$E374
// FUNCTION: STBC 0x0057bc80 _$E379
// FUNCTION: STBC 0x0057bc90 _$E378
// FUNCTION: STBC 0x0057bca0 _$E124
// FUNCTION: STBC 0x0057bcb0 _$E123
// FUNCTION: STBC 0x0057bcc0 _$E383
// FUNCTION: STBC 0x0057bcd0 _$E382
// FUNCTION: STBC 0x0057bce0 _$E129
// FUNCTION: STBC 0x0057bcf0 _$E128
// FUNCTION: STBC 0x0057bd00 _$E387
// FUNCTION: STBC 0x0057bd10 _$E386
// FUNCTION: STBC 0x0057bd20 _$E391
// FUNCTION: STBC 0x0057bd30 _$E390
// FUNCTION: STBC 0x0057bd40 _$E395
// FUNCTION: STBC 0x0057bd50 _$E394
// FUNCTION: STBC 0x0057bd60 _$E399
// FUNCTION: STBC 0x0057bd70 _$E398
// FUNCTION: STBC 0x0057bd80 _$E138
// FUNCTION: STBC 0x0057bd90 _$E137
// FUNCTION: STBC 0x0057bda0 _$E403
// FUNCTION: STBC 0x0057bdb0 _$E402
// FUNCTION: STBC 0x0057bdc0 _$E407
// FUNCTION: STBC 0x0057bdd0 _$E406
// FUNCTION: STBC 0x0057bde0 _$E411
// FUNCTION: STBC 0x0057bdf0 _$E410
// FUNCTION: STBC 0x0057be00 _$E144
// FUNCTION: STBC 0x0057be10 _$E143
// FUNCTION: STBC 0x0057be20 _$E149
// FUNCTION: STBC 0x0057be30 _$E148
// FUNCTION: STBC 0x0057be40 _$E179
// FUNCTION: STBC 0x0057be50 _$E178
// FUNCTION: STBC 0x0057be60 _$E415
// FUNCTION: STBC 0x0057be70 _$E414
// FUNCTION: STBC 0x0057be80 _$E184
// FUNCTION: STBC 0x0057be90 _$E183
// FUNCTION: STBC 0x0057bea0 _$E419
// FUNCTION: STBC 0x0057beb0 _$E418
// FUNCTION: STBC 0x0057bec0 _$E423
// FUNCTION: STBC 0x0057bed0 _$E422
// FUNCTION: STBC 0x0057bee0 _$E189
// FUNCTION: STBC 0x0057bef0 _$E188
// FUNCTION: STBC 0x0057bf00 _$E427
// FUNCTION: STBC 0x0057bf10 _$E426
// FUNCTION: STBC 0x0057bf20 _$E154
// FUNCTION: STBC 0x0057bf30 _$E153
// FUNCTION: STBC 0x0057bf40 _$E431
// FUNCTION: STBC 0x0057bf50 _$E430
// FUNCTION: STBC 0x0057bf60 _$E435
// FUNCTION: STBC 0x0057bf70 _$E434
// FUNCTION: STBC 0x0057bf80 _$E194
// FUNCTION: STBC 0x0057bf90 _$E193
// FUNCTION: STBC 0x0057bfa0 _$E199
// FUNCTION: STBC 0x0057bfb0 _$E198
// FUNCTION: STBC 0x0057bfc0 _$E439
// FUNCTION: STBC 0x0057bfd0 _$E438
// FUNCTION: STBC 0x0057bfe0 _$E443
// FUNCTION: STBC 0x0057bff0 _$E442
// FUNCTION: STBC 0x0057c000 _$E447
// FUNCTION: STBC 0x0057c010 _$E446
// FUNCTION: STBC 0x0057c020 _$E451
// FUNCTION: STBC 0x0057c030 _$E450
// FUNCTION: STBC 0x0057c040 _$E455
// FUNCTION: STBC 0x0057c050 _$E454
// FUNCTION: STBC 0x0057c060 _$E459
// FUNCTION: STBC 0x0057c070 _$E458
// FUNCTION: STBC 0x0057c080 _$E463
// FUNCTION: STBC 0x0057c090 _$E462
// FUNCTION: STBC 0x0057c0a0 _$E467
// FUNCTION: STBC 0x0057c0b0 _$E466
// FUNCTION: STBC 0x0057c0c0 _$E204
// FUNCTION: STBC 0x0057c0d0 _$E203
// FUNCTION: STBC 0x0057c0e0 _$E471
// FUNCTION: STBC 0x0057c0f0 _$E470
// FUNCTION: STBC 0x0057c100 _$E209
// FUNCTION: STBC 0x0057c110 _$E208
// FUNCTION: STBC 0x0057c120 _$E214
// FUNCTION: STBC 0x0057c130 _$E213
// FUNCTION: STBC 0x0057c170 _$E475
// FUNCTION: STBC 0x0057c180 _$E474
// FUNCTION: STBC 0x0057c190 _$E480
// FUNCTION: STBC 0x0057c1a0 _$E479
// FUNCTION: STBC 0x0057c1b0 _$E484
// FUNCTION: STBC 0x0057c1c0 _$E483

// Standard streams, handler tables and other file-scope statics.
// FUNCTION: STBC 0x0057ae40 _$E309
// FUNCTION: STBC 0x0057ae50 _$E306
// FUNCTION: STBC 0x0057ae60 _$E308
// FUNCTION: STBC 0x0057ae70 _$E307
// FUNCTION: STBC 0x0057ae80 _$E315
// FUNCTION: STBC 0x0057ae90 _$E312
// FUNCTION: STBC 0x0057aea0 _$E314
// FUNCTION: STBC 0x0057aeb0 _$E313
// FUNCTION: STBC 0x0057afa0 _$E490
// FUNCTION: STBC 0x0057afb0 _$E486
// FUNCTION: STBC 0x0057afc0 _$E489
// FUNCTION: STBC 0x0057afd0 _$E487

/*
================
TorpedoSystem::Cast

The object as a TorpedoSystem; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0057aff0
TorpedoSystem *TorpedoSystem::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TORPEDOSYSTEM_TYPE ) ) {
		return (TorpedoSystem *)object;
	}
	return NULL;
}

/*
================
TorpedoSystem::TorpedoSystem

No torpedoes and no ammo types loaded.
================
*/
// FUNCTION: STBC 0x0057b020
TorpedoSystem::TorpedoSystem( long id ) : WeaponSystem( id ) {
	lastFireTime = -1000.0f;
	int i;
	for ( i = 0; i < MAX_AMMO_TYPES; i++ ) {
		numAvailableTorps[i] = 0;
	}
	for ( i = 0; i < MAX_AMMO_TYPES; i++ ) {
		ammoTypes[i] = NULL;
	}
	currentAmmoType = 0;
	numReady = 0;
}

// DATA: STBC 0x0098cd70
TGEventHandlerTable TorpedoSystem::classHandlers;

/*
================
TorpedoSystem::GetObjType
================
*/
// FUNCTION: STBC 0x0057b090
int TorpedoSystem::GetObjType( void ) {
	return TORPEDOSYSTEM_TYPE;
}

/*
================
TorpedoSystem::GetClassName
================
*/
// FUNCTION: STBC 0x0057b100
char *TorpedoSystem::GetClassName( void ) {
	return "TorpedoSystem";
}

/*
================
TorpedoSystem::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x0057b110
char *TorpedoSystem::GetSwigTypeName( void ) {
	return "_p_TorpedoSystem";
}

/*
================
TorpedoSystem::GetPythonClassName
================
*/
// FUNCTION: STBC 0x0057b120
char *TorpedoSystem::GetPythonClassName( void ) {
	return "TorpedoSystemPtr";
}

/*
================
TorpedoSystem::GetClassHandlers
================
*/
// FUNCTION: STBC 0x0057b130
TGEventHandlerTable *TorpedoSystem::GetClassHandlers( void ) {
	return &classHandlers;
}


/*
================
TorpedoSystem::~TorpedoSystem
================
*/
// FUNCTION: STBC 0x0057b170
// FUNCTION: STBC 0x0057b140 ??_GTorpedoSystem@@UAEPAXI@Z
TorpedoSystem::~TorpedoSystem( void ) {
}

/*
================
TorpedoSystem::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x0057b180
void TorpedoSystem::RegisterHandlerNames( void ) {
	TorpedoTube::RegisterHandlerNames();
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TorpedoSystem::TorpedoTypeChangedHandler, "TorpedoSystem::TorpedoTypeChangedHandler" );
}

/*
================
TorpedoSystem::RegisterHandlers
================
*/
// FUNCTION: STBC 0x0057b1a0
void TorpedoSystem::RegisterHandlers( void ) {
	TorpedoTube::RegisterHandlers();
	AddMethodHandlerForClass( &classHandlers, ET_NET_TORPEDO_TYPE_CHANGED, "TorpedoSystem::TorpedoTypeChangedHandler" );
}

/*
================
TorpedoSystem::SetSkewFire

Every tube fires skewed, or none.
================
*/
// FUNCTION: STBC 0x0057b1c0
void TorpedoSystem::SetSkewFire( bool skewFire ) {
	for ( int i = 0; i < numChildSubsystems; i++ ) {
		( (TorpedoTube *)GetChildSubsystem( i ) )->SetSkewFire( skewFire );
	}
}

/*
================
TorpedoSystem::SetGroupFireMode

Group fire skews the tubes' shots.
================
*/
// FUNCTION: STBC 0x0057b1f0
void TorpedoSystem::SetGroupFireMode( int mode ) {
	if ( mode == 0 ) {
		SetSkewFire( false );
	} else {
		SetSkewFire( true );
	}
	WeaponSystem::SetGroupFireMode( mode );
}

/*
================
TorpedoSystem::SetAmmoType

Unloads every tube, and reloads them unless the flag is set, then makes
the type the loaded one. The player's ship tells its handlers of the new
count and type; in multiplayer the change is announced to the other
players.
================
*/
// FUNCTION: STBC 0x0057b230
void TorpedoSystem::SetAmmoType( int type, bool flag ) {
	for ( int i = 0; i < numChildSubsystems; i++ ) {
		TorpedoTube *tube = (TorpedoTube *)GetChildSubsystem( i );
		while ( tube->GetNumReady() > 0 ) {
			tube->UnloadTorpedo();
		}
		tube->ClearReloadTimes();
		if ( !flag ) {
			for ( int j = 0; j < tube->GetMaxReady(); j++ ) {
				tube->ReloadTorpedo();
			}
		}
	}

	ShipClass *player = Game::GetCurrentPlayer();
	if ( player == parentShip ) {
		TGEvent *event = new TGEvent;
		event->SetSource( this );
		event->SetDestination( this );
		event->SetEventType( ET_PLAYER_TORPEDO_COUNT_CHANGED );
		g_kEventManager.AddEvent( event );
		if ( flag == true ) {
			event = new TGEvent;
			event->SetSource( this );
			event->SetDestination( player );
			event->SetEventType( ET_PLAYER_TORPEDO_TYPE_CHANGED );
			g_kEventManager.AddEvent( event );
		} else if ( !flag ) {
			if ( TacticalControlWindow::GetTacticalControlWindow() != NULL && TacticalControlWindow::GetTacticalControlWindow()->GetWeaponsDisplay() != NULL ) {
				TacticalControlWindow::GetTacticalControlWindow()->GetWeaponsDisplay()->Unknown54C110( player );
			}
		}
	}

	if ( currentAmmoType != type && g_kUtopiaModule.IsMultiplayer() && parentShip != NULL && player != NULL ) {
		if ( g_kUtopiaModule.IsHost() ? ( player == parentShip || !parentShip->IsPlayerShip() ) : player == parentShip ) {
			TGCharEvent *event = new TGCharEvent;
			event->SetChar( type );
			event->SetSource( this );
			event->SetDestination( this );
			event->SetEventType( ET_NET_TORPEDO_TYPE_CHANGED_NOTIFY );
			g_kEventManager.AddEvent( event );
		}
	}
	currentAmmoType = type;
}

/*
================
TorpedoSystem::RemoveAmmoType

None of the type is left.
================
*/
// FUNCTION: STBC 0x0057b420
void TorpedoSystem::RemoveAmmoType( int type ) {
	numAvailableTorps[type] = 0;
}

/*
================
TorpedoSystem::AddTorpedo

Puts a torpedo of the loaded type back, if there is room for it. The player's
ship tells its handlers that the count changed.
================
*/
// FUNCTION: STBC 0x0057b440
void TorpedoSystem::AddTorpedo( void ) {
	if ( numAvailableTorps[currentAmmoType] < ammoTypes[currentAmmoType]->GetMaxTorpedoes() ) {
		if ( Game::GetCurrentPlayer() == parentShip ) {
			TGEvent *event = new TGEvent;
			event->SetSource( this );
			event->SetDestination( this );
			event->SetEventType( ET_PLAYER_TORPEDO_COUNT_CHANGED );
			g_kEventManager.AddEvent( event );
		}
		numAvailableTorps[currentAmmoType]++;
	}
}

/*
================
TorpedoSystem::UseTorpedo

Takes a torpedo of the loaded type, if any are left. The player's ship
tells its handlers that the count changed.
================
*/
// FUNCTION: STBC 0x0057b4d0
void TorpedoSystem::UseTorpedo( void ) {
	if ( numAvailableTorps[currentAmmoType] > 0 ) {
		if ( Game::GetCurrentPlayer() == parentShip ) {
			TGEvent *event = new TGEvent;
			event->SetSource( this );
			event->SetDestination( this );
			event->SetEventType( ET_PLAYER_TORPEDO_COUNT_CHANGED );
			g_kEventManager.AddEvent( event );
		}
		numAvailableTorps[currentAmmoType]--;
	}
}

/*
================
TorpedoSystem::IncNumReady
================
*/
// FUNCTION: STBC 0x0057b560
void TorpedoSystem::IncNumReady( void ) {
	numReady++;
}

/*
================
TorpedoSystem::DecNumReady

One torpedo fewer ready, never below none.
================
*/
// FUNCTION: STBC 0x0057b570
void TorpedoSystem::DecNumReady( void ) {
	if ( --numReady < 0 ) {
		numReady = 0;
	}
}

/*
================
TorpedoSystem::SetProperty

The ammo types and their counts come from the property.
================
*/
// FUNCTION: STBC 0x0057b590
void TorpedoSystem::SetProperty( SubsystemProperty *property_ ) {
	ShipSubsystem::SetProperty( property_ );
	for ( int i = 0; i < GetProperty()->GetNumAmmoTypes(); i++ ) {
		ammoTypes[i] = GetProperty()->GetAmmoType( i );
		numAvailableTorps[i] = GetProperty()->GetMaxTorpedoes( i );
	}
}

/*
================
TorpedoSystem::FillAmmoType

Loads the type to the most the system carries. The player's ship tells
its handlers that the count changed.
================
*/
// FUNCTION: STBC 0x0057b5e0
void TorpedoSystem::FillAmmoType( int type ) {
	numAvailableTorps[type] = ammoTypes[type]->GetMaxTorpedoes();
	if ( Game::GetCurrentGame() != NULL && Game::GetCurrentGame()->GetPlayer() == parentShip ) {
		TGEvent *event = new TGEvent;
		event->SetSource( this );
		event->SetDestination( this );
		event->SetEventType( ET_PLAYER_TORPEDO_COUNT_CHANGED );
		g_kEventManager.AddEvent( event );
	}
}

/*
================
TorpedoSystem::LoadAmmoType

Adds torpedoes of the type, at most up to the most the system carries and
at least down to none; returns how many were added.
================
*/
// FUNCTION: STBC 0x0057b660
int TorpedoSystem::LoadAmmoType( int type, int count ) {
	int loaded = numAvailableTorps[type];
	int maxTorpedoes = ammoTypes[type]->GetMaxTorpedoes();
	if ( loaded + count > maxTorpedoes ) {
		count = maxTorpedoes - loaded;
	}
	if ( loaded + count < 0 ) {
		count = -loaded;
	}
	numAvailableTorps[type] = loaded + count;
	if ( Game::GetCurrentGame() != NULL && Game::GetCurrentGame()->GetPlayer() == parentShip ) {
		TGEvent *event = new TGEvent;
		event->SetSource( this );
		event->SetDestination( this );
		event->SetEventType( ET_PLAYER_TORPEDO_COUNT_CHANGED );
		g_kEventManager.AddEvent( event );
	}
	return count;
}

/*
================
TorpedoSystem::IsTubeReady

Whether the tube that is child subsystem number index has a torpedo ready.
================
*/
// FUNCTION: STBC 0x0057b710
bool TorpedoSystem::IsTubeReady( int index ) {
	TorpedoTube *tube = (TorpedoTube *)GetChildSubsystem( index );
	if ( tube == NULL ) {
		return false;
	}
	if ( tube->GetNumReady() > 0 ) {
		return true;
	}
	return false;
}

/*
================
TorpedoSystem::GetNumAvailableTorpsToType

The number of torpedoes of the type left, or -1 for a type out of range.
================
*/
// FUNCTION: STBC 0x0057b740
int TorpedoSystem::GetNumAvailableTorpsToType( int type ) {
	if ( type >= 0 && type < MAX_AMMO_TYPES ) {
		return numAvailableTorps[type];
	}
	return -1;
}

/*
================
TorpedoSystem::TorpedoTypeChangedHandler

The loaded type is the one the event carries.
================
*/
// FUNCTION: STBC 0x0057b760
void TorpedoSystem::TorpedoTypeChangedHandler( TGEvent *event ) {
	currentAmmoType = ( (TGCharEvent *)event )->GetChar();
	CallNextHandler( event );
}

/*
================
TorpedoSystem::VirtualSlot26

After WeaponSystem's state, the loaded ammo type.
================
*/
// FUNCTION: STBC 0x0057b780
void TorpedoSystem::VirtualSlot26( TGStream *stream ) {
	WeaponSystem::VirtualSlot26( stream );

	stream->WriteChar( (char)currentAmmoType );
	stream->GetPos();
}

/*
================
TorpedoSystem::VirtualSlot27

Reads what VirtualSlot26 writes. The system stops trying to fire.
================
*/
// FUNCTION: STBC 0x0057b7b0
void TorpedoSystem::VirtualSlot27( TGStream *stream ) {
	WeaponSystem::VirtualSlot27( stream );

	currentAmmoType = stream->ReadChar();
	tryingToFire = false;
	stream->GetPos();
}

/*
================
TorpedoSystem::WriteToStream

The time of the last firing, the parent subsystem's ID again, the
torpedoes left of each type, the current type and the number ready.
================
*/
// FUNCTION: STBC 0x0057b7f0
bool TorpedoSystem::WriteToStream( TGStream *stream ) {
	if ( !WeaponSystem::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteFloat( lastFireTime );
	stream->WriteID( GetSavedID( parentSubsystem ) );
	for ( int i = 0; i < MAX_AMMO_TYPES; i++ ) {
		stream->WriteInt( numAvailableTorps[i] );
	}
	stream->WriteLong( currentAmmoType );
	stream->WriteInt( numReady );
	return true;
}

/*
================
TorpedoSystem::ReadFromStream

Reads what WriteToStream writes.
================
*/
// FUNCTION: STBC 0x0057b870
bool TorpedoSystem::ReadFromStream( TGStream *stream ) {
	if ( !WeaponSystem::ReadFromStream( stream ) ) {
		return false;
	}

	lastFireTime = stream->ReadFloat();
	parentSubsystem = (ShipSubsystem *)stream->ReadID();
	for ( int i = 0; i < MAX_AMMO_TYPES; i++ ) {
		numAvailableTorps[i] = stream->ReadInt();
	}
	currentAmmoType = stream->ReadLong();
	numReady = stream->ReadInt();
	return true;
}

/*
================
TorpedoSystem::FixUpPointers

Fails when the parent subsystem it was saved with has not been loaded.
================
*/
// FUNCTION: STBC 0x0057b8e0
bool TorpedoSystem::FixUpPointers( void ) {
	if ( !WeaponSystem::FixUpPointers() ) {
		return false;
	}
	if ( parentSubsystem != NULL && GetTGObjectPtr( (long)parentSubsystem ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
TorpedoSystem::PostFixUp

Besides ShipSubsystem's fix-ups, the parent is looked up again (a second
time) and the ammo types are the property's.
================
*/
// FUNCTION: STBC 0x0057b910
void TorpedoSystem::PostFixUp( void ) {
	ShipSubsystem::PostFixUp();
	parentSubsystem = (ShipSubsystem *)GetSavedObject( (long)parentSubsystem );
	TorpedoSystemProperty *torpedoProperty = GetProperty();
	if ( torpedoProperty != NULL ) {
		for ( int i = 0; i < MAX_AMMO_TYPES; i++ ) {
			ammoTypes[i] = torpedoProperty->GetAmmoType( i );
		}
	}
}
