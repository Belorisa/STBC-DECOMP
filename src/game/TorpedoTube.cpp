/*
===============================================================================

	TorpedoTube.cpp

===============================================================================
*/

#include <math.h>

#include "TorpedoTube.h"
#include "NiMath.h"
#include "Game.h"
#include "SetClass.h"
#include "TGSound.h"
#include "TGSoundManager.h"
#include "TGSoundRegion.h"
#include "TGMatrix3.h"
#include "TorpedoAmmoType.h"
#include "MultiplayerGame.h"
#include "TGBufferStream.h"
#include "TGMessage.h"
#include "TGNetwork.h"
#include "TGStream.h"
#include <iostream>
#include "TorpedoTubeProperty.h"
#include "ShipClass.h"
#include "Torpedo.h"
#include "TGEventManager.h"
#include "TGIntEvent.h"
#include "TGObjPtrEvent.h"
#include "TGEventHandlerTable.h"
#include "TorpedoSystem.h"
#include "UtopiaModule.h"
#include "WeaponSystem.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0057e820 _$E507
// FUNCTION: STBC 0x0057e840 _$E506

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0057c250 _$E72
// FUNCTION: STBC 0x0057c260 _$E71
// FUNCTION: STBC 0x0057c270 _$E313
// FUNCTION: STBC 0x0057c280 _$E312
// FUNCTION: STBC 0x0057c290 _$E83
// FUNCTION: STBC 0x0057c2a0 _$E82
// FUNCTION: STBC 0x0057c2b0 _$E87
// FUNCTION: STBC 0x0057c2c0 _$E86
// FUNCTION: STBC 0x0057c2d0 _$E91
// FUNCTION: STBC 0x0057c2e0 _$E90
// FUNCTION: STBC 0x0057c2f0 _$E95
// FUNCTION: STBC 0x0057c300 _$E94
// FUNCTION: STBC 0x0057c310 _$E212
// FUNCTION: STBC 0x0057c320 _$E211
// FUNCTION: STBC 0x0057df00 _$E154
// FUNCTION: STBC 0x0057df10 _$E153
// FUNCTION: STBC 0x0057e060 _$E167
// FUNCTION: STBC 0x0057e070 _$E166
// FUNCTION: STBC 0x0057e080 _$E217
// FUNCTION: STBC 0x0057e090 _$E216
// FUNCTION: STBC 0x0057e0a0 _$E318
// FUNCTION: STBC 0x0057e0b0 _$E317
// FUNCTION: STBC 0x0057e0c0 _$E221
// FUNCTION: STBC 0x0057e0d0 _$E220
// FUNCTION: STBC 0x0057e0e0 _$E322
// FUNCTION: STBC 0x0057e0f0 _$E321
// FUNCTION: STBC 0x0057e100 _$E329
// FUNCTION: STBC 0x0057e110 _$E328
// FUNCTION: STBC 0x0057e120 _$E347
// FUNCTION: STBC 0x0057e130 _$E346
// FUNCTION: STBC 0x0057e140 _$E351
// FUNCTION: STBC 0x0057e150 _$E350
// FUNCTION: STBC 0x0057e160 _$E112
// FUNCTION: STBC 0x0057e170 _$E111
// FUNCTION: STBC 0x0057e180 _$E269
// FUNCTION: STBC 0x0057e190 _$E268
// FUNCTION: STBC 0x0057e1a0 _$E276
// FUNCTION: STBC 0x0057e1b0 _$E275
// FUNCTION: STBC 0x0057e1c0 _$E280
// FUNCTION: STBC 0x0057e1d0 _$E279
// FUNCTION: STBC 0x0057e1e0 _$E284
// FUNCTION: STBC 0x0057e1f0 _$E283
// FUNCTION: STBC 0x0057e200 _$E288
// FUNCTION: STBC 0x0057e210 _$E287
// FUNCTION: STBC 0x0057e220 _$E294
// FUNCTION: STBC 0x0057e230 _$E293
// FUNCTION: STBC 0x0057e240 _$E355
// FUNCTION: STBC 0x0057e250 _$E354
// FUNCTION: STBC 0x0057e260 _$E359
// FUNCTION: STBC 0x0057e270 _$E358
// FUNCTION: STBC 0x0057e280 _$E363
// FUNCTION: STBC 0x0057e290 _$E362
// FUNCTION: STBC 0x0057e2a0 _$E367
// FUNCTION: STBC 0x0057e2b0 _$E366
// FUNCTION: STBC 0x0057e2c0 _$E371
// FUNCTION: STBC 0x0057e2d0 _$E370
// FUNCTION: STBC 0x0057e2e0 _$E375
// FUNCTION: STBC 0x0057e2f0 _$E374
// FUNCTION: STBC 0x0057e300 _$E379
// FUNCTION: STBC 0x0057e310 _$E378
// FUNCTION: STBC 0x0057e320 _$E383
// FUNCTION: STBC 0x0057e330 _$E382
// FUNCTION: STBC 0x0057e340 _$E387
// FUNCTION: STBC 0x0057e350 _$E386
// FUNCTION: STBC 0x0057e360 _$E391
// FUNCTION: STBC 0x0057e370 _$E390
// FUNCTION: STBC 0x0057e380 _$E117
// FUNCTION: STBC 0x0057e390 _$E116
// FUNCTION: STBC 0x0057e3a0 _$E395
// FUNCTION: STBC 0x0057e3b0 _$E394
// FUNCTION: STBC 0x0057e3c0 _$E122
// FUNCTION: STBC 0x0057e3d0 _$E121
// FUNCTION: STBC 0x0057e3e0 _$E399
// FUNCTION: STBC 0x0057e3f0 _$E398
// FUNCTION: STBC 0x0057e400 _$E403
// FUNCTION: STBC 0x0057e410 _$E402
// FUNCTION: STBC 0x0057e420 _$E407
// FUNCTION: STBC 0x0057e430 _$E406
// FUNCTION: STBC 0x0057e440 _$E411
// FUNCTION: STBC 0x0057e450 _$E410
// FUNCTION: STBC 0x0057e460 _$E131
// FUNCTION: STBC 0x0057e470 _$E130
// FUNCTION: STBC 0x0057e480 _$E415
// FUNCTION: STBC 0x0057e490 _$E414
// FUNCTION: STBC 0x0057e4a0 _$E419
// FUNCTION: STBC 0x0057e4b0 _$E418
// FUNCTION: STBC 0x0057e4c0 _$E423
// FUNCTION: STBC 0x0057e4d0 _$E422
// FUNCTION: STBC 0x0057e4e0 _$E137
// FUNCTION: STBC 0x0057e4f0 _$E136
// FUNCTION: STBC 0x0057e500 _$E142
// FUNCTION: STBC 0x0057e510 _$E141
// FUNCTION: STBC 0x0057e520 _$E172
// FUNCTION: STBC 0x0057e530 _$E171
// FUNCTION: STBC 0x0057e540 _$E427
// FUNCTION: STBC 0x0057e550 _$E426
// FUNCTION: STBC 0x0057e560 _$E177
// FUNCTION: STBC 0x0057e570 _$E176
// FUNCTION: STBC 0x0057e580 _$E431
// FUNCTION: STBC 0x0057e590 _$E430
// FUNCTION: STBC 0x0057e5a0 _$E435
// FUNCTION: STBC 0x0057e5b0 _$E434
// FUNCTION: STBC 0x0057e5c0 _$E182
// FUNCTION: STBC 0x0057e5d0 _$E181
// FUNCTION: STBC 0x0057e5e0 _$E439
// FUNCTION: STBC 0x0057e5f0 _$E438
// FUNCTION: STBC 0x0057e600 _$E147
// FUNCTION: STBC 0x0057e610 _$E146
// FUNCTION: STBC 0x0057e620 _$E443
// FUNCTION: STBC 0x0057e630 _$E442
// FUNCTION: STBC 0x0057e640 _$E447
// FUNCTION: STBC 0x0057e650 _$E446
// FUNCTION: STBC 0x0057e660 _$E187
// FUNCTION: STBC 0x0057e670 _$E186
// FUNCTION: STBC 0x0057e680 _$E192
// FUNCTION: STBC 0x0057e690 _$E191
// FUNCTION: STBC 0x0057e6a0 _$E451
// FUNCTION: STBC 0x0057e6b0 _$E450
// FUNCTION: STBC 0x0057e6c0 _$E455
// FUNCTION: STBC 0x0057e6d0 _$E454
// FUNCTION: STBC 0x0057e6e0 _$E459
// FUNCTION: STBC 0x0057e6f0 _$E458
// FUNCTION: STBC 0x0057e700 _$E463
// FUNCTION: STBC 0x0057e710 _$E462
// FUNCTION: STBC 0x0057e720 _$E467
// FUNCTION: STBC 0x0057e730 _$E466
// FUNCTION: STBC 0x0057e740 _$E471
// FUNCTION: STBC 0x0057e750 _$E470
// FUNCTION: STBC 0x0057e760 _$E475
// FUNCTION: STBC 0x0057e770 _$E474
// FUNCTION: STBC 0x0057e780 _$E479
// FUNCTION: STBC 0x0057e790 _$E478
// FUNCTION: STBC 0x0057e7a0 _$E197
// FUNCTION: STBC 0x0057e7b0 _$E196
// FUNCTION: STBC 0x0057e7c0 _$E483
// FUNCTION: STBC 0x0057e7d0 _$E482
// FUNCTION: STBC 0x0057e7e0 _$E202
// FUNCTION: STBC 0x0057e7f0 _$E201
// FUNCTION: STBC 0x0057e800 _$E207
// FUNCTION: STBC 0x0057e810 _$E206
// FUNCTION: STBC 0x0057e850 _$E487
// FUNCTION: STBC 0x0057e860 _$E486
// FUNCTION: STBC 0x0057e870 _$E492
// FUNCTION: STBC 0x0057e880 _$E491
// FUNCTION: STBC 0x0057e890 _$E235
// FUNCTION: STBC 0x0057e8a0 _$E234

// Standard streams, handler tables and other file-scope statics.
// FUNCTION: STBC 0x0057c1d0 _$E249
// FUNCTION: STBC 0x0057c1e0 _$E246
// FUNCTION: STBC 0x0057c1f0 _$E248
// FUNCTION: STBC 0x0057c200 _$E247
// FUNCTION: STBC 0x0057c210 _$E255
// FUNCTION: STBC 0x0057c220 _$E252
// FUNCTION: STBC 0x0057c230 _$E254
// FUNCTION: STBC 0x0057c240 _$E253
// FUNCTION: STBC 0x0057c430 _$E498
// FUNCTION: STBC 0x0057c440 _$E494
// FUNCTION: STBC 0x0057c450 _$E497
// FUNCTION: STBC 0x0057c460 _$E495

/*
================
TorpedoTube::GetProperty
================
*/
// FUNCTION: STBC 0x0057c330
TorpedoTubeProperty *TorpedoTube::GetProperty( void ) {
	return (TorpedoTubeProperty *)property;
}

/*
================
TorpedoTube::GetDirection

The launch direction, in the ship's frame.
================
*/
// FUNCTION: STBC 0x0057c340
TGPoint3 TorpedoTube::GetDirection( void ) {
	return GetProperty()->GetDirection();
}

/*
================
TorpedoTube::GetRight
================
*/
// FUNCTION: STBC 0x0057c3a0
TGPoint3 TorpedoTube::GetRight( void ) {
	return GetProperty()->GetRight();
}

/*
================
TorpedoTube::GetImmediateDelay
================
*/
// FUNCTION: STBC 0x0057c400
float TorpedoTube::GetImmediateDelay( void ) {
	return GetProperty()->GetImmediateDelay();
}

/*
================
TorpedoTube::GetReloadDelay
================
*/
// FUNCTION: STBC 0x0057c410
float TorpedoTube::GetReloadDelay( void ) {
	return GetProperty()->GetReloadDelay();
}

/*
================
TorpedoTube::GetMaxReady
================
*/
// FUNCTION: STBC 0x0057c420
int TorpedoTube::GetMaxReady( void ) {
	return GetProperty()->GetMaxReady();
}

/*
================
TorpedoTube::Cast

The object as a TorpedoTube; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0057c480
TorpedoTube *TorpedoTube::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TORPEDOTUBE_TYPE ) ) {
		return (TorpedoTube *)object;
	}
	return NULL;
}

/*
================
TorpedoTube::TorpedoTube

Empty, and last fired long ago.
================
*/
// FUNCTION: STBC 0x0057c4b0
TorpedoTube::TorpedoTube( long id ) : Weapon( id ) {
	numReady = 0;
	skewFire = false;
	reloadTimes = NULL;
	lastFireTime = -1000.0f;
}

/*
================
TorpedoTube::GetObjType
================
*/
// FUNCTION: STBC 0x0057c4f0
int TorpedoTube::GetObjType( void ) {
	return TORPEDOTUBE_TYPE;
}

// DATA: STBC 0x0098ce70
TGEventHandlerTable TorpedoTube::classHandlers;

/*
================
TorpedoTube::GetClassName
================
*/
// FUNCTION: STBC 0x0057c560
char *TorpedoTube::GetClassName( void ) {
	return "TorpedoTube";
}

/*
================
TorpedoTube::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x0057c570
char *TorpedoTube::GetSwigTypeName( void ) {
	return "_p_TorpedoTube";
}

/*
================
TorpedoTube::GetPythonClassName
================
*/
// FUNCTION: STBC 0x0057c580
char *TorpedoTube::GetPythonClassName( void ) {
	return "TorpedoTubePtr";
}

/*
================
TorpedoTube::GetClassHandlers
================
*/
// FUNCTION: STBC 0x0057c590
TGEventHandlerTable *TorpedoTube::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
TorpedoTube::CalculateWeaponAppeal

Full appeal while the tube can fire, none otherwise.
================
*/
// FUNCTION: STBC 0x0057c5a0
float TorpedoTube::CalculateWeaponAppeal( void ) {
	return CanFire();
}

/*
================
TorpedoTube::~TorpedoTube
================
*/
// FUNCTION: STBC 0x0057c5f0
// FUNCTION: STBC 0x0057c5c0 ??_GTorpedoTube@@UAEPAXI@Z
TorpedoTube::~TorpedoTube( void ) {
	if ( reloadTimes != NULL ) {
		delete[] reloadTimes;
	}
}

/*
================
TorpedoTube::SetProperty

A torpedo tube property gives the tube a reload time for each torpedo it
can hold ready, none of them reloading.
================
*/
// FUNCTION: STBC 0x0057c620
void TorpedoTube::SetProperty( SubsystemProperty *property_ ) {
	ShipSubsystem::SetProperty( property_ );

	if ( TorpedoTubeProperty::Cast( property_ ) != NULL ) {
		if ( reloadTimes != NULL ) {
			delete[] reloadTimes;
		}
		reloadTimes = new float[GetMaxReady()];
		for ( int i = 0; i < GetMaxReady(); i++ ) {
			reloadTimes[i] = -1.0f;
		}
	}
}

/*
================
TorpedoTube::Update

Each reloading slot advances by the system's power; a slot past the
reload delay reloads a torpedo.
================
*/
// FUNCTION: STBC 0x0057c6a0
void TorpedoTube::Update( float deltaTime ) {
	Weapon::Update( deltaTime );

	if ( GetNumReady() < GetMaxReady() ) {
		TorpedoSystem *system = TorpedoSystem::Cast( GetParentSubsystem() );
		for ( int i = 0; i < GetMaxReady(); i++ ) {
			if ( reloadTimes[i] >= 0.0f ) {
				reloadTimes[i] += system->GetNormalPowerPercentage() * deltaTime;
				if ( reloadTimes[i] > GetReloadDelay() ) {
					ReloadTorpedo();
				}
			}
		}
	}
}

/*
================
TorpedoTube::ClearReloadTimes

Sets every reload time to 0 (invented name).
================
*/
// FUNCTION: STBC 0x0057c740
void TorpedoTube::ClearReloadTimes( void ) {
	for ( int i = 0; i < GetMaxReady(); i++ ) {
		reloadTimes[i] = 0;
	}
}

/*
================
TorpedoTube::Fire

Launches a torpedo at the tube's target: the weapon system finds where the
target is, and the tube must be able to hit that point; otherwise the
system hears that the tube can't fire. Then the tube and its system take
one torpedo, the first free reload slot starts reloading, and the tube's
and the ship's handlers hear of the shot. In multiplayer the shot is sent
to the other players.
================
*/
// FUNCTION: STBC 0x0057c770
// PARKED: the two can't-fire sends tail-merge and 0 stays an immediate; retail keeps 0 in EDI and both blocks apart
bool TorpedoTube::Fire( float, bool playSound ) {
	if ( !CanFire() ) {
		return false;
	}

	NiPoint3 targetPosition;
	ObjectClass *targetObject = NULL;
	if ( !( (WeaponSystem *)parentSubsystem )->GetTargetPosition( &target, &targetPosition, &targetObject ) ) {
		if ( target.targetID != 0 ) {
			TGEvent *event = new TGEvent;
			event->SetEventType( ET_CANT_FIRE );
			event->SetSource( parentSubsystem );
			event->SetDestination( parentShip );
			g_kEventManager.AddEvent( event );
		}
		return false;
	}
	if ( !CanHit( targetPosition ) ) {
		TGEvent *event = new TGEvent;
		event->SetEventType( ET_CANT_FIRE );
		event->SetSource( parentSubsystem );
		event->SetDestination( parentShip );
		g_kEventManager.AddEvent( event );
		return false;
	}

	Torpedo *torpedo = CreateTorpedo( playSound );
	torpedo->SetTarget( target );
	lastFireTime = g_kUtopiaModule.GetGameTime();
	numReady--;

	TorpedoSystem *system = (TorpedoSystem *)parentSubsystem;
	system->UseTorpedo();
	system->DecNumReady();

	for ( int i = 0; i < GetMaxReady(); i++ ) {
		if ( reloadTimes[i] <= 0.0f ) {
			reloadTimes[i] = 0.0f;
			break;
		}
	}

	SendTorpedoFiredEvent( torpedo );

	TGObjPtrEvent *event = new TGObjPtrEvent;
	event->SetSource( this );
	event->SetDestination( parentShip );
	event->SetEventType( ET_WEAPON_FIRED );
	event->SetObjPtr( targetObject );
	g_kEventManager.AddEvent( event );

	system->SetLastFireTime( g_kUtopiaModule.GetGameTime() );
	if ( g_kUtopiaModule.IsMultiplayer() ) {
		SendCreateTorpMessage( torpedo, playSound, false );
	}
	return true;
}

/*
================
TorpedoTube::FireDumb

Launches a torpedo without a target: the tube and its system take one
torpedo, the first free reload slot starts reloading, and the tube's and
the ship's handlers hear of the shot. In multiplayer the shot is sent to
the other players.
================
*/
// FUNCTION: STBC 0x0057c9e0
bool TorpedoTube::FireDumb( float, bool playSound ) {
	if ( !CanFire() ) {
		return false;
	}

	Torpedo *torpedo = CreateTorpedo( playSound );
	torpedo->SetTarget( 0 );
	lastFireTime = g_kUtopiaModule.GetGameTime();
	numReady--;

	TorpedoSystem *system = (TorpedoSystem *)parentSubsystem;
	system->UseTorpedo();
	system->DecNumReady();

	for ( int i = 0; i < GetMaxReady(); i++ ) {
		if ( reloadTimes[i] <= 0.0f ) {
			reloadTimes[i] = 0.0f;
			break;
		}
	}

	SendTorpedoFiredEvent( torpedo );

	TGObjPtrEvent *event = new TGObjPtrEvent;
	event->SetSource( this );
	event->SetDestination( parentShip );
	event->SetEventType( ET_WEAPON_FIRED );
	event->SetObjPtr( NULL );
	g_kEventManager.AddEvent( event );

	system->SetLastFireTime( g_kUtopiaModule.GetGameTime() );
	if ( g_kUtopiaModule.IsMultiplayer() ) {
		SendCreateTorpMessage( torpedo, playSound, true );
	}
	return true;
}

/*
================
TorpedoTube::SendCreateTorpMessage

Sends the other players the torpedo just fired: the tube, the firing
player, the flags and the direction, then the target and the point aimed
at when the torpedo homes. The host sends it to every other player, a
client to the host.
================
*/
// FUNCTION: STBC 0x0057cb10
void TorpedoTube::SendCreateTorpMessage( Torpedo *torpedo, bool playSound, bool dumbFire ) {
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network == NULL ) {
		return;
	}

	TGBufferStream stream;
	char buffer[1024];
	stream.VirtualSlot60( buffer, sizeof( buffer ) );
	stream.WriteChar( CREATE_TORP_MESSAGE );
	stream.WriteID( GetObjID() );
	stream.WriteChar( torpedo->GetPlayerID() );

	char flags = 0;
	if ( playSound ) {
		flags |= CREATE_TORP_PLAY_SOUND;
	}
	if ( skewFire ) {
		flags |= CREATE_TORP_SKEW_FIRE;
	}
	ShipClass *targetShip = ShipClass::GetObjectByID( NULL, GetTargetID() );
	if ( targetShip == NULL || dumbFire ) {
		flags |= CREATE_TORP_NO_TARGET;
	}
	stream.WriteChar( flags );

	NiPoint3 direction = torpedo->GetVelocity();
	direction.Unitize();
	stream.VirtualSlot34( direction.x, direction.y, direction.z );

	if ( targetShip != NULL && !dumbFire ) {
		float radius = targetShip->GetRadius();
		NiPoint3 offset = target.offset;
		stream.WriteID( GetTargetID() );
		stream.VirtualSlot38( offset.x, offset.y, offset.z, radius );
	}

	TGMessage *message = new TGMessage;
	message->SetData( buffer, stream.GetPos() );
	message->SetGuaranteed( true );
	if ( g_kUtopiaModule.IsHost() ) {
		network->SendTGMessageToGroup( "NoMe", message );
	} else {
		network->SendTGMessage( network->GetHostID(), message, 0 );
	}
}

/*
================
TorpedoTube::CreateTorpedo

Creates a torpedo of the system's current ammunition at the tube, in the
ship's set, moving with the ship plus the launch speed along the tube's
direction. A skewed tube aims a little to its right, and its torpedo
starts homing later. The player's torpedoes do the difficulty's damage
outside multiplayer. The launch sound plays from the ship when asked.
================
*/
// FUNCTION: STBC 0x0057cd90
// PARKED: operand sort of the launch position's y and z fadds: retail loads the location first, which needs its result temp (#31, hash 03E0) to outweigh the product's (#287, 23E0), i.e. numbered above it; the location as the left operand flips all three components and calls it after the rotation (84.3)
Torpedo *TorpedoTube::CreateTorpedo( bool playSound ) {
	TorpedoSystem *system = (TorpedoSystem *)parentSubsystem;
	ShipClass *ship = system->GetParentShip();
	SetClass *set = ship->GetContainingSet();

	NiPoint3 position = ship->GetWorldRotation() * TGPoint3( GetDamagePoint() ) + ship->GetWorldLocation();
	unsigned char homingDelay = 0;
	if ( skewFire == true ) {
		homingDelay = SKEW_HOMING_DELAY;
	}
	Torpedo *torpedo = Torpedo::Create( system->GetCurrentAmmoType()->GetTorpedoScript(), position, homingDelay );

	if ( ship == Game::GetCurrentPlayer() && !g_kUtopiaModule.IsMultiplayer() ) {
		torpedo->SetDamage( torpedo->GetDamage() * Game::GetOffensiveDifficultyMultiplier() );
	}

	NiPoint3 velocity = ship->GetVelocity();
	NiPoint3 direction;
	if ( skewFire == true ) {
		direction = ship->GetWorldRotation() * ( GetDirection() + GetRight() * SKEW_OFFSET );
	} else {
		direction = ship->GetWorldRotation() * GetDirection();
	}
	velocity += direction * torpedo->GetLaunchSpeed();
	torpedo->SetVelocity( &velocity );

	torpedo->SetParent( ship );
	torpedo->UpdateNodeOnly();
	set->AddObjectToSet( torpedo, NULL );

	if ( g_kUtopiaModule.IsMultiplayer() ) {
		if ( ship == Game::GetCurrentGame()->GetPlayer() && g_kUtopiaModule.GetNetwork() != NULL ) {
			torpedo->SetPlayerID( g_kUtopiaModule.GetNetwork()->GetLocalID() );
		}
	} else {
		torpedo->SetPlayerID( ship->GetObjID() );
	}

	char *launchSound = torpedo->GetLaunchSound();
	if ( launchSound != NULL && playSound == true ) {
		TGSound *sound = g_kSoundManager.GetSound( launchSound );
		if ( sound != NULL ) {
			if ( sound->GetParentNode() != ship->GetNode() ) {
				sound->AttachToNode( ship->GetNode() );
			}
			TGSoundRegion *region = TGSoundRegion::GetRegion( set->GetName() );
			if ( region != NULL ) {
				region->AddSound( sound );
			}
			float priority = LAUNCH_PRIORITY;
			if ( ship == Game::GetCurrentPlayer() ) {
				priority = PLAYER_LAUNCH_PRIORITY;
			}
			sound->SetPriority( priority );
			sound->Play( true );
		}
	}
	return torpedo;
}

/*
================
TorpedoTube::CreateNetTorpedo

The torpedo another player fired: it is made as CreateTorpedo makes it, but
leaves along the direction the message gave, at the speed of the launch
velocity. It aims at the target's point, unless there is none. A torpedo
that cannot steer is pointed at where its target will be.
================
*/
// FUNCTION: STBC 0x0057d110
// DRAFT: structure and call order follow retail; the frame is 0xC bytes larger (retail keeps fewer NiPoint3 temporaries around the launch position and the aim) and the speed rescale and the intercept scaling are spelt differently
Torpedo *TorpedoTube::CreateNetTorpedo( bool skewFire_, bool playSound, bool noTarget, int playerID, long targetID, const NiPoint3 &offset, const NiPoint3 &direction ) {
	TorpedoSystem *system = (TorpedoSystem *)parentSubsystem;
	if ( system == NULL ) {
		return NULL;
	}
	ShipClass *ship = system->GetParentShip();
	SetClass *set = ship->GetContainingSet();
	if ( set == NULL ) {
		return NULL;
	}

	NiPoint3 position = ship->GetWorldRotation() * TGPoint3( GetDamagePoint() ) + ship->GetWorldLocation();
	unsigned char homingDelay = 0;
	if ( skewFire == true ) {
		homingDelay = SKEW_HOMING_DELAY;
	}
	Torpedo *torpedo = Torpedo::Create( system->GetCurrentAmmoType()->GetTorpedoScript(), position, homingDelay );

	NiPoint3 velocity = ship->GetVelocity();
	NiPoint3 launchDirection;
	if ( skewFire_ == true ) {
		launchDirection = ship->GetWorldRotation() * ( GetDirection() + GetRight() * SKEW_OFFSET );
	} else {
		launchDirection = ship->GetWorldRotation() * GetDirection();
	}
	velocity += launchDirection * torpedo->GetLaunchSpeed();
	velocity = direction * velocity.Length();
	torpedo->SetVelocity( &velocity );

	torpedo->SetParent( ship );
	torpedo->UpdateNodeOnly();
	set->AddObjectToSet( torpedo, NULL );
	torpedo->SetPlayerID( playerID );

	char *launchSound = torpedo->GetLaunchSound();
	if ( launchSound != NULL && playSound == true ) {
		TGSound *sound = g_kSoundManager.GetSound( launchSound );
		if ( sound != NULL ) {
			if ( sound->GetParentNode() != ship->GetNode() ) {
				sound->AttachToNode( ship->GetNode() );
			}
			TGSoundRegion *region = TGSoundRegion::GetRegion( set->GetName() );
			if ( region != NULL ) {
				region->AddSound( sound );
			}
			sound->Play( true );
		}
	}

	if ( noTarget == true ) {
		torpedo->SetTarget( 0 );
	} else {
		torpedo->SetTarget( targetID );
		torpedo->SetTargetOffset( offset );
	}

	if ( torpedo->GetMaxAngularAccel() == 0.0f || torpedo->GetGuidanceLifeTime() == 0.0f ) {
		ShipClass *target = ShipClass::GetObjectByID( NULL, targetID );
		if ( target != NULL ) {
			NiPoint3 torpedoPosition = torpedo->GetTranslate();
			NiPoint3 targetPosition = target->GetWorldLocation();
			NiPoint3 torpedoVelocity = torpedo->GetVelocity();
			float speed = torpedoVelocity.Length();
			NiPoint3 targetVelocity = target->GetVelocity();
			NiPoint3 targetAcceleration = target->GetAcceleration();
			if ( speed > 0.0f ) {
				float time = ( targetPosition - torpedoPosition ).Length() / speed;
				if ( time > 0.0f ) {
					NiPoint3 predicted = target->PredictPosition( targetPosition, targetVelocity, targetAcceleration, time );
					NiPoint3 aim = predicted - torpedoPosition;
					aim.Unitize();
					torpedoVelocity = aim * speed;
					torpedo->SetVelocity( &torpedoVelocity );
				}
			}
		}
	} else {
		float guidanceLifetime = torpedo->GetGuidanceLifeTime() * 1.25f;
		torpedo->SetMaxAngularAccel( torpedo->GetMaxAngularAccel() * 1.5f );
		torpedo->SetGuidanceLifetime( guidanceLifetime );
	}
	return torpedo;
}

/*
================
TorpedoTube::CanFire

A tube needs a torpedo ready, and not while the ship is cloaked. Tubes
fire at least half a second apart, unless the tube fires skewed with the
group the system fired last. The tube's own immediate delay must have
passed since it fired, and it must not be disabled.
================
*/
// FUNCTION: STBC 0x0057d780
// PARKED: register: the inline GetOverallConditionPercentage takes the parent subsystem in eax in retail (this in ecx first, for GetDisabledPercentage), ecx here; PulseWeapon::CanFire (0x575180) has the same residue
bool TorpedoTube::CanFire( void ) {
	if ( numReady <= 0 ) {
		return false;
	}
	if ( parentShip != NULL && parentShip->IsCloaked() == true ) {
		return false;
	}

	TorpedoSystem *system = (TorpedoSystem *)parentSubsystem;
	float now = g_kUtopiaModule.GetGameTime();
	if ( !skewFire ) {
		if ( now - system->GetLastFireTime() < TUBE_FIRE_INTERVAL ) {
			return false;
		}
	}
	if ( skewFire == true ) {
		int group = system->GetLastGroupFired();
		if ( group != -1 || system->GetGroupFireMode() != 0 ) {
			if ( !IsMemberOfGroup( group ) && now - system->GetLastFireTime() < TUBE_FIRE_INTERVAL ) {
				return false;
			}
		}
	}

	float sinceFired = now - lastFireTime;
	if ( GetProperty()->GetImmediateDelay() > sinceFired ) {
		return false;
	}
	if ( GetOverallConditionPercentage() < GetDisabledPercentage() ) {
		return false;
	}
	return true;
}

/*
================
TorpedoTube::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x0057d880
void TorpedoTube::RegisterHandlerNames( void ) {
}

/*
================
TorpedoTube::RegisterHandlers
================
*/
// FUNCTION: STBC 0x0057d890
void TorpedoTube::RegisterHandlers( void ) {
}

/*
================
TorpedoTube::ReloadTorpedo

Loads a torpedo, when the tube has room and the system has one of the
loaded type that is not in a tube yet. The slot that has reloaded the
longest stops reloading, and the tube's handlers hear of it.
================
*/
// FUNCTION: STBC 0x0057d8a0
void TorpedoTube::ReloadTorpedo( void ) {
	TorpedoSystem *system = (TorpedoSystem *)parentSubsystem;
	if ( numReady >= GetProperty()->GetMaxReady() ) {
		return;
	}
	if ( system->GetNumAvailableTorps( system->GetCurrentAmmoTypeNumber() ) - system->GetNumReady() <= 0 ) {
		return;
	}

	numReady++;
	system->IncNumReady();

	float longest = -1.0f;
	int longestIndex = -1;
	for ( int i = 0; i < GetMaxReady(); i++ ) {
		if ( reloadTimes[i] > longest ) {
			longest = reloadTimes[i];
			longestIndex = i;
		}
	}
	if ( longestIndex != -1 ) {
		reloadTimes[longestIndex] = -1.0f;
	}

	TGIntEvent *event = new TGIntEvent;
	event->SetEventType( ET_TORPEDO_RELOAD );
	event->SetInt( 0 );
	event->SetDestination( this );
	g_kEventManager.AddEvent( event );
}

/*
================
TorpedoTube::UnloadTorpedo

Takes a ready torpedo out of the tube and its system; its slot starts
reloading. The handlers hear of it as a firing without a torpedo.
================
*/
// FUNCTION: STBC 0x0057d9a0
void TorpedoTube::UnloadTorpedo( void ) {
	if ( numReady == 0 ) {
		return;
	}

	numReady--;
	for ( int i = 0; i < GetMaxReady(); i++ ) {
		if ( reloadTimes[i] <= 0.0f ) {
			reloadTimes[i] = 0.0f;
			break;
		}
	}

	TorpedoSystem *system = TorpedoSystem::Cast( GetParentSubsystem() );
	if ( system != NULL ) {
		system->DecNumReady();
	}
	SendTorpedoFiredEvent( NULL );
}

/*
================
TorpedoTube::SendTorpedoFiredEvent

Tells the tube's handlers that it launched the torpedo.
================
*/
// FUNCTION: STBC 0x0057da20
void TorpedoTube::SendTorpedoFiredEvent( Torpedo *torpedo ) {
	TGIntEvent *event = new TGIntEvent;
	event->SetEventType( ET_TORPEDO_FIRED );
	event->SetInt( 0 );
	event->SetSource( torpedo );
	event->SetDestination( this );
	g_kEventManager.AddEvent( event );
}

// DATA: STBC 0x00893448
const unsigned char TorpedoTube::SKEW_HOMING_DELAY = 2;

/*
================
TorpedoTube::CanHit

Whether a torpedo launched now could reach the point: the time it takes to
cover the distance at the launch speed plus the closing speed has to be
positive, and the point has to lie in the tube's firing arc.
================
*/
// FUNCTION: STBC 0x0057da90 ?CanHit@TorpedoTube@@QAE_NVNiPoint3@@@Z
// PARKED: x87 order and temporaries: retail writes the velocity difference straight into its local (ours through a temporary copied after the call), stores the launch speed through the FPU and keeps only the z of the distance in memory
bool TorpedoTube::CanHit( NiPoint3 position ) {
	ShipSubsystem *system = parentSubsystem;
	ShipClass *ship = system->GetParentShip();
	PhysicsObjectClass *targetObject = PhysicsObjectClass::GetObjectByID( ship->GetContainingSet(), GetTargetID() );

	NiPoint3 targetVelocity = targetObject != NULL ? targetObject->GetVelocity() : NiPoint3::ZERO;
	NiPoint3 relativeVelocity = ship->GetVelocity() - targetVelocity;
	float launchSpeed = ( (TorpedoSystem *)system )->GetCurrentAmmoType()->GetLaunchSpeed();
	float distance = ( position - ship->GetWorldLocation() ).Length();
	float time = distance / ( relativeVelocity.Length() + launchSpeed );
	if ( time < 0.0f ) {
		return false;
	}

	if ( targetObject != NULL ) {
		NiPoint3 targetPosition = targetObject->GetWorldLocation();
		// BUG: the predicted position is thrown away, so the arc is tested against the point as given.
		targetObject->PredictPosition( targetPosition, targetObject->GetVelocity(), targetObject->GetAcceleration(), time );
	}
	return IsInArc( position );
}

// The half-angle of a tube's firing arc, 30 degrees (invented name).
#define ARC_LIMIT 0.5235988f

/*
================
TorpedoTube::IsInArc

Whether the point is in front of the tube and within the arc of ARC_LIMIT
either side of its direction, horizontally and vertically.
================
*/
// FUNCTION: STBC 0x0057dc10 ?IsInArc@TorpedoTube@@QAE_NVNiPoint3@@@Z
// DRAFT: call set agrees; retail builds offset in place (its out-of-line constructor writes the local, then EH state 0) where this build copies a temporary into the TGPoint3, numbers its EH states 0-4 against ours 0-2, and keeps the rotated directions' copies partly on the FPU
bool TorpedoTube::IsInArc( NiPoint3 position ) {
	ShipClass *ship = parentShip;
	NiMatrix3 rotation( ship->GetWorldRotation() );

	NiPoint3 origin = ship->GetWorldLocation() + rotation * GetDamagePoint();
	TGPoint3 offset = position - origin;

	TGPoint3 forward = rotation * GetProperty()->GetDirection();
	TGPoint3 right = rotation * GetProperty()->GetRight();
	TGPoint3 up = right.Cross( forward );

	float ahead = forward * offset;
	if ( ahead > 0.0f ) {
		float side = right * offset;
		float above = up * offset;
		float horizontal = NiFastATan2( side, ahead );
		if ( horizontal >= -ARC_LIMIT && horizontal <= ARC_LIMIT ) {
			float vertical = NiFastATan2( above, ahead );
			if ( vertical >= -ARC_LIMIT && vertical <= ARC_LIMIT ) {
				return true;
			}
		}
	}
	return false;
}
/*
================
TorpedoTube::CalculateRoughDirection

The launch direction in the world.
================
*/
// FUNCTION: STBC 0x0057de90
TGPoint3 TorpedoTube::CalculateRoughDirection( void ) {
	return parentShip->GetWorldRotation() * GetDirection();
}

/*
================
TorpedoTube::SetParentSubsystem

A tube's parent is its torpedo system, which reloads its tubes with the
first ammo type.
================
*/
// FUNCTION: STBC 0x0057df20
void TorpedoTube::SetParentSubsystem( ShipSubsystem *parent ) {
	parentSubsystem = parent;
	( (TorpedoSystem *)parent )->SetAmmoType( 0, false );
}

/*
================
TorpedoTube::WriteToStream

The torpedoes ready, the time of the last firing, the skew flag, then
each reload time.
================
*/
// FUNCTION: STBC 0x0057df40
bool TorpedoTube::WriteToStream( TGStream *stream ) {
	if ( !Weapon::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteLong( numReady );
	stream->WriteFloat( lastFireTime );
	stream->WriteBool( skewFire );
	stream->WriteInt( GetMaxReady() );
	for ( int i = 0; i < GetMaxReady(); i++ ) {
		stream->WriteFloat( reloadTimes[i] );
	}
	return true;
}

/*
================
TorpedoTube::ReadFromStream

Reads what WriteToStream writes, allocating the reload times.
================
*/
// FUNCTION: STBC 0x0057dfd0
bool TorpedoTube::ReadFromStream( TGStream *stream ) {
	if ( !Weapon::ReadFromStream( stream ) ) {
		return false;
	}

	numReady = stream->ReadLong();
	lastFireTime = stream->ReadFloat();
	skewFire = stream->ReadBool();
	int maxReady = stream->ReadInt();
	reloadTimes = new float[maxReady];
	for ( int i = 0; i < maxReady; i++ ) {
		reloadTimes[i] = stream->ReadFloat();
	}
	return true;
}

