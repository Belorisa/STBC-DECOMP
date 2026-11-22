/*
===============================================================================

	WeaponSystem.cpp

===============================================================================
*/

#include <iostream>
#include "WeaponSystem.h"
#include "PhaserSystem.h"
#include "SensorSubsystem.h"
#include "PulseWeaponSystem.h"
#include "TorpedoSystem.h"
#include "TractorBeamSystem.h"
#include "FiringChain.h"
#include "WeaponSystemProperty.h"
#include "Weapon.h"
#include "ObjectClass.h"
#include "SetClass.h"
#include "ShipClass.h"
#include "DamageableObject.h"
#include <math.h>
#include <string.h>
#include "HullClass.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGStream.h"
#include "PyEmbed.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00586b80 _$E509
// FUNCTION: STBC 0x00586ba0 _$E508

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00583e50 _$E32
// FUNCTION: STBC 0x00583e60 _$E29
// FUNCTION: STBC 0x00583e70 _$E31
// FUNCTION: STBC 0x00583e80 _$E30
// FUNCTION: STBC 0x00583e90 _$E38
// FUNCTION: STBC 0x00583ea0 _$E35
// FUNCTION: STBC 0x00583eb0 _$E37
// FUNCTION: STBC 0x00583ec0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00583ed0 _$E86
// FUNCTION: STBC 0x00583ee0 _$E85
// FUNCTION: STBC 0x00583ef0 _$E313
// FUNCTION: STBC 0x00583f00 _$E312
// FUNCTION: STBC 0x00584ca0 _$E97
// FUNCTION: STBC 0x00584cb0 _$E96
// FUNCTION: STBC 0x00584e20 _$E101
// FUNCTION: STBC 0x00584e30 _$E100
// FUNCTION: STBC 0x00585280 _$E105
// FUNCTION: STBC 0x00585290 _$E104
// FUNCTION: STBC 0x00585660 _$E109
// FUNCTION: STBC 0x00585670 _$E108
// FUNCTION: STBC 0x00585b60 _$E226
// FUNCTION: STBC 0x00585b70 _$E225
// FUNCTION: STBC 0x005862e0 _$E168
// FUNCTION: STBC 0x005862f0 _$E167
// FUNCTION: STBC 0x00586300 _$E181
// FUNCTION: STBC 0x00586310 _$E180
// FUNCTION: STBC 0x00586320 _$E231
// FUNCTION: STBC 0x00586330 _$E230
// FUNCTION: STBC 0x00586340 _$E318
// FUNCTION: STBC 0x00586350 _$E317
// FUNCTION: STBC 0x00586360 _$E235
// FUNCTION: STBC 0x00586370 _$E234
// FUNCTION: STBC 0x00586380 _$E322
// FUNCTION: STBC 0x00586390 _$E321
// FUNCTION: STBC 0x005863a0 _$E329
// FUNCTION: STBC 0x005863b0 _$E328
// FUNCTION: STBC 0x005863c0 _$E339
// FUNCTION: STBC 0x005863d0 _$E338
// FUNCTION: STBC 0x005863e0 _$E343
// FUNCTION: STBC 0x005863f0 _$E342
// FUNCTION: STBC 0x00586400 _$E126
// FUNCTION: STBC 0x00586410 _$E125
// FUNCTION: STBC 0x00586420 _$E256
// FUNCTION: STBC 0x00586430 _$E255
// FUNCTION: STBC 0x00586440 _$E263
// FUNCTION: STBC 0x00586450 _$E262
// FUNCTION: STBC 0x00586460 _$E267
// FUNCTION: STBC 0x00586470 _$E266
// FUNCTION: STBC 0x00586480 _$E271
// FUNCTION: STBC 0x00586490 _$E270
// FUNCTION: STBC 0x005864a0 _$E275
// FUNCTION: STBC 0x005864b0 _$E274
// FUNCTION: STBC 0x005864c0 _$E281
// FUNCTION: STBC 0x005864d0 _$E280
// FUNCTION: STBC 0x005864e0 _$E347
// FUNCTION: STBC 0x005864f0 _$E346
// FUNCTION: STBC 0x00586500 _$E351
// FUNCTION: STBC 0x00586510 _$E350
// FUNCTION: STBC 0x00586520 _$E355
// FUNCTION: STBC 0x00586530 _$E354
// FUNCTION: STBC 0x00586540 _$E359
// FUNCTION: STBC 0x00586550 _$E358
// FUNCTION: STBC 0x00586560 _$E363
// FUNCTION: STBC 0x00586570 _$E362
// FUNCTION: STBC 0x00586580 _$E367
// FUNCTION: STBC 0x00586590 _$E366
// FUNCTION: STBC 0x005865a0 _$E371
// FUNCTION: STBC 0x005865b0 _$E370
// FUNCTION: STBC 0x005865c0 _$E375
// FUNCTION: STBC 0x005865d0 _$E374
// FUNCTION: STBC 0x005865e0 _$E379
// FUNCTION: STBC 0x005865f0 _$E378
// FUNCTION: STBC 0x00586600 _$E383
// FUNCTION: STBC 0x00586610 _$E382
// FUNCTION: STBC 0x00586620 _$E131
// FUNCTION: STBC 0x00586630 _$E130
// FUNCTION: STBC 0x00586640 _$E387
// FUNCTION: STBC 0x00586650 _$E386
// FUNCTION: STBC 0x00586660 _$E136
// FUNCTION: STBC 0x00586670 _$E135
// FUNCTION: STBC 0x00586680 _$E391
// FUNCTION: STBC 0x00586690 _$E390
// FUNCTION: STBC 0x005866a0 _$E395
// FUNCTION: STBC 0x005866b0 _$E394
// FUNCTION: STBC 0x005866c0 _$E399
// FUNCTION: STBC 0x005866d0 _$E398
// FUNCTION: STBC 0x005866e0 _$E403
// FUNCTION: STBC 0x005866f0 _$E402
// FUNCTION: STBC 0x00586700 _$E145
// FUNCTION: STBC 0x00586710 _$E144
// FUNCTION: STBC 0x00586720 _$E407
// FUNCTION: STBC 0x00586730 _$E406
// FUNCTION: STBC 0x00586740 _$E411
// FUNCTION: STBC 0x00586750 _$E410
// FUNCTION: STBC 0x00586760 _$E415
// FUNCTION: STBC 0x00586770 _$E414
// FUNCTION: STBC 0x00586780 _$E151
// FUNCTION: STBC 0x00586790 _$E150
// FUNCTION: STBC 0x005867a0 _$E156
// FUNCTION: STBC 0x005867b0 _$E155
// FUNCTION: STBC 0x005867c0 _$E186
// FUNCTION: STBC 0x005867d0 _$E185
// FUNCTION: STBC 0x005867e0 _$E419
// FUNCTION: STBC 0x005867f0 _$E418
// FUNCTION: STBC 0x00586800 _$E191
// FUNCTION: STBC 0x00586810 _$E190
// FUNCTION: STBC 0x00586820 _$E423
// FUNCTION: STBC 0x00586830 _$E422
// FUNCTION: STBC 0x00586880 _$E427
// FUNCTION: STBC 0x00586890 _$E426
// FUNCTION: STBC 0x005868a0 _$E196
// FUNCTION: STBC 0x005868b0 _$E195
// FUNCTION: STBC 0x00586900 _$E431
// FUNCTION: STBC 0x00586910 _$E430
// FUNCTION: STBC 0x00586920 _$E161
// FUNCTION: STBC 0x00586930 _$E160
// FUNCTION: STBC 0x00586940 _$E435
// FUNCTION: STBC 0x00586950 _$E434
// FUNCTION: STBC 0x00586960 _$E439
// FUNCTION: STBC 0x00586970 _$E438
// FUNCTION: STBC 0x005869c0 _$E201
// FUNCTION: STBC 0x005869d0 _$E200
// FUNCTION: STBC 0x005869e0 _$E206
// FUNCTION: STBC 0x005869f0 _$E205
// FUNCTION: STBC 0x00586a00 _$E443
// FUNCTION: STBC 0x00586a10 _$E442
// FUNCTION: STBC 0x00586a20 _$E447
// FUNCTION: STBC 0x00586a30 _$E446
// FUNCTION: STBC 0x00586a40 _$E451
// FUNCTION: STBC 0x00586a50 _$E450
// FUNCTION: STBC 0x00586a60 _$E455
// FUNCTION: STBC 0x00586a70 _$E454
// FUNCTION: STBC 0x00586a80 _$E459
// FUNCTION: STBC 0x00586a90 _$E458
// FUNCTION: STBC 0x00586aa0 _$E463
// FUNCTION: STBC 0x00586ab0 _$E462
// FUNCTION: STBC 0x00586ac0 _$E467
// FUNCTION: STBC 0x00586ad0 _$E466
// FUNCTION: STBC 0x00586ae0 _$E471
// FUNCTION: STBC 0x00586af0 _$E470
// FUNCTION: STBC 0x00586b00 _$E211
// FUNCTION: STBC 0x00586b10 _$E210
// FUNCTION: STBC 0x00586b20 _$E475
// FUNCTION: STBC 0x00586b30 _$E474
// FUNCTION: STBC 0x00586b40 _$E216
// FUNCTION: STBC 0x00586b50 _$E215
// FUNCTION: STBC 0x00586b60 _$E221
// FUNCTION: STBC 0x00586b70 _$E220
// FUNCTION: STBC 0x00586ca0 _$E479
// FUNCTION: STBC 0x00586cb0 _$E478
// FUNCTION: STBC 0x00586cc0 _$E484
// FUNCTION: STBC 0x00586cd0 _$E483
// FUNCTION: STBC 0x00586ce0 _$E488
// FUNCTION: STBC 0x00586cf0 _$E487

// FUNCTION: STBC 0x00583f10 _$E494
// FUNCTION: STBC 0x00583f20 _$E490
// FUNCTION: STBC 0x00583f30 _$E493
// FUNCTION: STBC 0x00583f40 _$E491
// DATA: STBC 0x0098d300
TGEventHandlerTable WeaponSystem::classHandlers;

/*
================
WeaponSystem::Cast

The object as a WeaponSystem; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00583f60
WeaponSystem *WeaponSystem::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( WEAPONSYSTEM_TYPE ) ) {
		return (WeaponSystem *)object;
	}
	return NULL;
}

// FUNCTION: STBC 0x00583f90 _$E500
// FUNCTION: STBC 0x00583fa0 _$E496
// FUNCTION: STBC 0x00583fd0 _$E499
// FUNCTION: STBC 0x00583fe0 _$E497
// DATA: STBC 0x0098d328
NiTList<ObjectClass *> WeaponSystem::visibleTargets( 2 );

/*
================
WeaponSystem::GetProperty
================
*/
// FUNCTION: STBC 0x00584050
WeaponSystemProperty *WeaponSystem::GetProperty( void ) {
	return (WeaponSystemProperty *)property;
}

/*
================
WeaponSystem::IsSingleFire
================
*/
// FUNCTION: STBC 0x00584060
bool WeaponSystem::IsSingleFire( void ) {
	return GetProperty()->IsSingleFire();
}

/*
================
WeaponSystem::ShouldBeAimed
================
*/
// FUNCTION: STBC 0x00584070
bool WeaponSystem::ShouldBeAimed( void ) {
	return GetProperty()->IsAimedWeapon();
}

/*
================
WeaponSystem::Update

Updates the weapons: while it is trying to fire, each weapon gets its turn to
fire; a system that is off or disabled stops. Every weapon is then updated for
the time it has accumulated.
================
*/
// FUNCTION: STBC 0x005847d0
// PARKED: x87 operand order of the inlined time-slice accumulation: retail loads the weapon's time first, this build the argument (accessors, a shared counter or weapon local, unknown48 = unknown48 + deltaTime tried)
void WeaponSystem::Update( float deltaTime ) {
	PoweredSubsystem::Update( deltaTime );

	bool didFire = false;
	Weapon *firedWeapon = NULL;
	if ( !IsDisabled() ) {
		if ( IsOn() == true ) {
			if ( tryingToFire ) {
				firedWeapon = UpdateWeapons( deltaTime, &didFire );
			} else {
				if ( firing == true ) {
					for ( int i = 0; i < numChildSubsystems; i++ ) {
						Weapon *weapon = GetWeapon( i );
						if ( weapon->IsFiring() == true ) {
							weapon->StopFiring();
						}
					}
				}
			}
			firing = didFire;
		} else {
			StopFiring();
		}
	} else {
		StopFiring();
	}

	canFire = false;
	for ( int i = 0; i < numChildSubsystems; i++ ) {
		Weapon *weapon = GetWeapon( i );
		if ( IsSingleFire() == true && weapon != firedWeapon ) {
			weapon->StopFiring();
		}
		if ( weapon->CanFire() ) {
			canFire = true;
		}
		weapon->TimeSlicedUpdate( deltaTime );
	}
	forceUpdate = false;
}

/*
================
WeaponSystem::UpdateTargetList

Drops the targets that are gone from the set or have cloaked.
================
*/
// FUNCTION: STBC 0x00584cc0
void WeaponSystem::UpdateTargetList( void ) {
	NiTListIterator pos = targets.GetHeadPos();
	while ( pos != NULL ) {
		WeaponSystemTarget *target = targets.GetNext( pos );
		ObjectClass *object = ObjectClass::GetObjectByID( parentShip->GetContainingSet(), target->targetID );
		ShipClass *ship = ShipClass::Cast( object );
		bool cloaked = false;
		if ( ship != NULL ) {
			cloaked = ship->IsCloaked();
		}
		if ( object == NULL || cloaked == true ) {
			if ( pos == NULL ) {
				delete targets.GetTail();
				targets.RemoveTail();
			} else {
				NiTListIterator current = targets.GetPrevPos( pos );
				if ( current != NULL ) {
					delete targets.RemovePos( current );
				}
			}
		}
	}
}

/*
================
WeaponSystem::IsInTargetList

The target's record in the target list; NULL when it is not there.
================
*/
// FUNCTION: STBC 0x00584080
WeaponSystemTarget *WeaponSystem::IsInTargetList( ObjectClass *target ) {
	return FindTarget( target->GetObjID() );
}

/*
================
WeaponSystem::RemoveFromTargetList
================
*/
// FUNCTION: STBC 0x00584090
void WeaponSystem::RemoveFromTargetList( ObjectClass *target ) {
	RemoveTarget( target->GetObjID() );
}

/*
================
WeaponSystem::WeaponSystem

Not firing, without targets or firing chains. A weapon system created
locally listens for objects leaving the set, to drop them as targets.
================
*/
// FUNCTION: STBC 0x005840a0
WeaponSystem::WeaponSystem( long id ) : PoweredSubsystem( id ), targets( 2 ), firingChains( 2 ) {
	tryingToFire = false;
	firing = false;
	forceUpdate = false;
	groupFireMode = 0;
	lastWeaponFired = -1;
	firingChainMode = 0;
	lastGroupFired = -1;
	if ( id == 0 ) {
		g_kEventManager.AddBroadcastMethodHandler( ET_EXITED_SET, this, "WeaponSystem::ObjectExitedSet" );
	}
}

/*
================
WeaponSystem::GetObjType
================
*/
// FUNCTION: STBC 0x00584190
int WeaponSystem::GetObjType( void ) {
	return WEAPONSYSTEM_TYPE;
}

/*
================
WeaponSystem::GetClassName
================
*/
// FUNCTION: STBC 0x00584200
char *WeaponSystem::GetClassName( void ) {
	return "WeaponSystem";
}

/*
================
WeaponSystem::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00584210
char *WeaponSystem::GetSwigTypeName( void ) {
	return "_p_WeaponSystem";
}

/*
================
WeaponSystem::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00584220
char *WeaponSystem::GetPythonClassName( void ) {
	return "WeaponSystemPtr";
}

/*
================
WeaponSystem::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00584230
TGEventHandlerTable *WeaponSystem::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
WeaponSystem::~WeaponSystem
================
*/
// FUNCTION: STBC 0x00584270
// FUNCTION: STBC 0x00584240 ??_GWeaponSystem@@UAEPAXI@Z
WeaponSystem::~WeaponSystem( void ) {
	ClearTargetList();
	DeleteFiringChains();
}

/*
================
WeaponSystem::GetFiringChain

The firing chain at the index; NULL when the index is past the end.
================
*/
// FUNCTION: STBC 0x00584fc0
FiringChain *WeaponSystem::GetFiringChain( int index ) {
	NiTListIterator pos = firingChains.GetHeadPos();
	FiringChain *chain = firingChains.Get( pos );
	for ( int i = 0; pos != NULL && i < index; i++ ) {
		pos = firingChains.GetNextPos( pos );
		chain = firingChains.Get( pos );
	}
	return chain;
}

/*
================
WeaponSystem::GetFiringChainName

The name of the firing chain at the index; NULL when there is none.
================
*/
// FUNCTION: STBC 0x00585000
char *WeaponSystem::GetFiringChainName( int index ) {
	FiringChain *chain = GetFiringChain( index );
	if ( chain != NULL ) {
		return chain->GetName();
	}
	return NULL;
}

/*
================
WeaponSystem::SetFiringChains

Replaces the firing chains with the ones the property's string lists: ';'
separated pairs of the groups (the digits 1 to 9 in it) and the chain's name
(invented name; ShipClass hands it the weapon system property's
GetFiringChainString).
================
*/
// FUNCTION: STBC 0x00585020
void WeaponSystem::SetFiringChains( TGString *chains ) {
	DeleteFiringChains();
	char *buffer = chains->Unknown6F56F0();
	bool haveGroups = false;
	FiringChain *chain = NULL;
	char *token = strtok( buffer, ";" );
	while ( token != NULL ) {
		if ( !haveGroups ) {
			chain = new FiringChain;
			int groups = 0;
			for ( ; *token != '\0'; token++ ) {
				// RULE-EXCEPTION: R11, retail's bound is ':', one past '9' (a <= '9' test compiles to cmp 0x39)
				if ( *token > '0' && *token < ':' ) {
					groups |= 1 << ( *token - '1' );
				}
			}
			chain->SetWeaponGroups( groups );
			haveGroups = true;
		} else {
			chain->SetName( token );
			firingChains.AddTail( chain );
			chain = NULL;
			haveGroups = false;
		}
		token = strtok( NULL, ";" );
	}
	delete buffer;
}

/*
================
WeaponSystem::GetTargetPosition

The world position of the target record's point (its offset carried
through the target node's world transform), and the target object; false
when the ship is in no set or the target no longer exists (invented name).
================
*/
// FUNCTION: STBC 0x005852a0
bool WeaponSystem::GetTargetPosition( WeaponSystemTarget *target, NiPoint3 *position, ObjectClass **targetObject ) {
	SetClass *set = parentShip->GetContainingSet();
	if ( set == NULL ) {
		return false;
	}
	ObjectClass *object = ObjectClass::GetObjectByID( set, target->targetID );
	if ( object == NULL ) {
		return false;
	}
	NiNode *node = object->GetNode();
	*position = node->GetWorldRotate() * ( node->GetWorldScale() * target->offset ) + node->GetWorldTranslate();
	if ( targetObject != NULL ) {
		*targetObject = object;
	}
	return true;
}

/*
================
WeaponSystem::RemoveTarget

Removes the record of the target with the ID from the target list, and
deletes it.
================
*/
// FUNCTION: STBC 0x00585390 ?RemoveTarget@WeaponSystem@@IAEXJ@Z
void WeaponSystem::RemoveTarget( long targetID ) {
	WeaponSystemTarget *target = FindTarget( targetID );
	if ( target != NULL ) {
		targets.Remove( target );
		delete target;
	}
}

/*
================
WeaponSystem::RemoveTarget

Takes the target record off the list and deletes it (invented name;
nothing in retail calls it).
================
*/
// FUNCTION: STBC 0x00585490 ?RemoveTarget@WeaponSystem@@QAEXPAUWeaponSystemTarget@@@Z
void WeaponSystem::RemoveTarget( WeaponSystemTarget *target ) {
	targets.Remove( target );
	delete target;
}

/*
================
WeaponSystem::TurnOff

A system turned off forgets its targets.
================
*/
// FUNCTION: STBC 0x005856b0
void WeaponSystem::TurnOff( void ) {
	if ( on ) {
		PoweredSubsystem::TurnOff();
		ClearTargetList();
	}
}

/*
================
WeaponSystem::IsVisibleTarget

Whether the object is in the list of targets the ship's sensors see.
================
*/
// FUNCTION: STBC 0x005859d0
bool WeaponSystem::IsVisibleTarget( ObjectClass *object ) {
	return visibleTargets.FindPos( object ) ? true : false;
}

/*
================
WeaponSystem::GatherTargets

Adds the objects the system has as targets to the list, each once (invented
name).
================
*/
// FUNCTION: STBC 0x00585910
void WeaponSystem::GatherTargets( WeaponSystem *system, NiTList<ObjectClass *> *list ) {
	NiTListIterator pos = system->targets.GetHeadPos();
	while ( pos != NULL ) {
		WeaponSystemTarget *record = system->targets.GetNext( pos );
		ObjectClass *object = ObjectClass::GetObjectByID( NULL, record->targetID );
		if ( object != NULL && list->FindPos( object ) == NULL ) {
			list->AddTail( object );
		}
	}
}

/*
================
WeaponSystem::Unknown5856D0

Gathers the objects the ship's weapon systems have as targets and keeps
those its sensors see in the static list of visible targets.
================
*/
// FUNCTION: STBC 0x005856d0
void WeaponSystem::Unknown5856D0( ShipClass *ship ) {
	visibleTargets.RemoveAll();
	PhaserSystem *phasers = ship->GetPhaserSystem();
	TorpedoSystem *torpedoes = ship->GetTorpedoSystem();
	PulseWeaponSystem *pulseWeapons = ship->GetPulseWeaponSystem();
	TractorBeamSystem *tractorBeams = ship->GetTractorBeamSystem();
	SensorSubsystem *sensors = ship->GetSensorSubsystem();
	if ( sensors != NULL ) {
		NiTList<ObjectClass *> targetObjects( 2 );
		if ( phasers != NULL ) {
			GatherTargets( phasers, &targetObjects );
		}
		if ( torpedoes != NULL ) {
			GatherTargets( torpedoes, &targetObjects );
		}
		if ( pulseWeapons != NULL ) {
			GatherTargets( pulseWeapons, &targetObjects );
		}
		if ( tractorBeams != NULL ) {
			GatherTargets( tractorBeams, &targetObjects );
		}
		NiTListIterator pos = targetObjects.GetHeadPos();
		while ( pos != NULL ) {
			ObjectClass *object = targetObjects.GetNext( pos );
			if ( sensors->IsObjectVisible( object ) == true ) {
				visibleTargets.AddTail( object );
			}
		}
	}
}

/*
================
WeaponSystem::VirtualSlot26

After PoweredSubsystem's state, whether the system is trying to fire.
================
*/
// FUNCTION: STBC 0x00585a10
void WeaponSystem::VirtualSlot26( TGStream *stream ) {
	PoweredSubsystem::VirtualSlot26( stream );

	stream->WriteBool( tryingToFire );
	stream->GetPos();
}

/*
================
WeaponSystem::VirtualSlot27

Reads what VirtualSlot26 writes.
================
*/
// FUNCTION: STBC 0x00585a40
void WeaponSystem::VirtualSlot27( TGStream *stream ) {
	PoweredSubsystem::VirtualSlot27( stream );

	tryingToFire = stream->ReadBool();
	stream->GetPos();
}

/*
================
WeaponSystem::WriteToStream

The firing flags and modes, then the targets and the firing chains,
each list as its length followed by the entries writing themselves.
================
*/
// FUNCTION: STBC 0x00585a70
bool WeaponSystem::WriteToStream( TGStream *stream ) {
	if ( !PoweredSubsystem::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteBool( tryingToFire );
	stream->WriteBool( firing );
	stream->WriteBool( unknownAA );
	stream->WriteBool( forceUpdate );
	stream->WriteInt( groupFireMode );
	stream->WriteInt( lastWeaponFired );
	stream->WriteInt( firingChainMode );
	stream->WriteInt( lastGroupFired );

	stream->WriteLong( targets.GetSize() );
	NiTListIterator pos = targets.GetHeadPos();
	while ( pos ) {
		targets.GetNext( pos )->WriteToStream( stream );
	}

	stream->WriteLong( firingChains.GetSize() );
	pos = firingChains.GetHeadPos();
	while ( pos ) {
		firingChains.GetNext( pos )->WriteToStream( stream );
	}
	return true;
}

/*
================
WeaponSystem::ReadFromStream

Reads what WriteToStream wrote, after deleting the firing chains: the
targets and the chains are created anew and appended to their lists.
================
*/
// FUNCTION: STBC 0x00585b80
bool WeaponSystem::ReadFromStream( TGStream *stream ) {
	if ( !PoweredSubsystem::ReadFromStream( stream ) ) {
		return false;
	}

	DeleteFiringChains();

	tryingToFire = stream->ReadBool();
	firing = stream->ReadBool();
	unknownAA = stream->ReadBool();
	forceUpdate = stream->ReadBool();
	groupFireMode = stream->ReadInt();
	lastWeaponFired = stream->ReadInt();
	firingChainMode = stream->ReadInt();
	lastGroupFired = stream->ReadInt();

	int count = stream->ReadLong();
	int i;
	for ( i = 0; i < count; i++ ) {
		WeaponSystemTarget *target = new WeaponSystemTarget;
		target->ReadFromStream( stream );
		targets.AddTail( target );
	}

	count = stream->ReadLong();
	for ( i = 0; i < count; i++ ) {
		FiringChain *chain = new FiringChain;
		chain->ReadFromStream( stream );
		firingChains.AddTail( chain );
	}
	return true;
}

/*
================
WeaponSystemTarget::WriteToStream
================
*/
// FUNCTION: STBC 0x00585ec0
bool WeaponSystemTarget::WriteToStream( TGStream *stream ) {
	stream->WriteID( targetID );
	stream->WriteFloat( offset.x );
	stream->WriteFloat( offset.y );
	stream->WriteFloat( offset.z );
	return true;
}

/*
================
WeaponSystemTarget::ReadFromStream
================
*/
// FUNCTION: STBC 0x00585f00
bool WeaponSystemTarget::ReadFromStream( TGStream *stream ) {
	targetID = stream->ReadID();
	offset.x = stream->ReadFloat();
	offset.y = stream->ReadFloat();
	offset.z = stream->ReadFloat();
	return true;
}

/*
================
WeaponSystem_GetTargetList

The Python method WeaponSystem.GetTargetList: a tuple of the target
objects still in the ship's set; empty when the ship is in no set.
================
*/
// FUNCTION: STBC 0x00585f40
// PARKED: declaration-sensitive schedule: exact or one load apart (the set reloaded before the list position at the loop head) depending on the declarations PyEmbed.h carries; source unchanged
PyObject *WeaponSystem_GetTargetList( PyObject *self, PyObject *args ) {
	PyObject *instance = NULL;
	if ( PyArg_ParseTuple( args, "O", &instance ) == -1 ) {
		return NULL;
	}
	WeaponSystem *system = NULL;
	PyEmbed::Unknown6F98E0( instance, (void **)&system, "WeaponSystem" );
	if ( system == NULL ) {
		return NULL;
	}
	if ( system->GetParentShip() == NULL ) {
		PyErr_SetString( PyExc_AttributeError, "Weapon system has no parent ship." );
		return NULL;
	}
	SetClass *set = system->GetParentShip()->GetContainingSet();
	if ( set == NULL ) {
		return PyTuple_New( 0 );
	}

	NiTList<ObjectClass *> objects( 2 );
	NiTList<WeaponSystemTarget *> *targets = system->GetTargets();
	NiTListIterator pos = targets->GetHeadPos();
	while ( pos ) {
		WeaponSystemTarget *target = targets->GetNext( pos );
		ObjectClass *object = ObjectClass::GetObjectByID( set, target->targetID );
		if ( object != NULL ) {
			objects.AddTail( object );
		}
	}
	PyObject *tuple = PyTuple_New( objects.GetSize() );
	int index = 0;
	while ( objects.GetSize() != 0 ) {
		PyTuple_SetItem( tuple, index++, objects.RemoveHead()->VirtualSlot08() );
	}
	return tuple;
}

/*
================
WeaponSystem::Fire

Starting after the weapon fired last, fires the first weapon of the group
that can. A weapon with no target of its own takes the ship's target; with
none it fires dumb. A single-fire system stops at the first shot.
================
*/
// FUNCTION: STBC 0x00584600
void WeaponSystem::Fire( void ) {
	if ( IsDisabled() || !IsOn() || GetParentShip()->IsDoingInSystemWarp() == true ) {
		return;
	}
	UpdateTargetList();
	int i;
	int end = lastWeaponFired + numChildSubsystems + 1;
	for ( i = lastWeaponFired + 1; i < end; i++ ) {
		int index = i % numChildSubsystems;
		Weapon *weapon = GetWeapon( index );
		if ( groupFireMode == 0 || weapon->IsMemberOfGroup( groupFireMode ) ) {
			if ( !weapon->CanFire() ) {
				weapon->StopFiring();
			} else {
				if ( weapon->GetTargetID() == 0 ) {
					ObjectClass *target = GetParentShip()->GetTarget();
					if ( target != NULL ) {
						weapon->SetTarget( target->GetObjID(), GetParentShip()->GetTargetOffset() );
					} else {
						weapon->FireDumb( 0.0f, true );
						firing = true;
						lastWeaponFired = index;
						if ( IsSingleFire() == true ) {
							return;
						}
						continue;
					}
				}
				if ( weapon->Fire( 0.0f, true ) == true ) {
					firing = true;
					lastWeaponFired = index;
					if ( IsSingleFire() == true ) {
						return;
					}
				}
			}
		}
	}
}

/*
================
WeaponSystem::VirtualSlot34

Adds the target to the target list, if it is not there yet, and aims at the
offset; with no offset at the target's hull's damage point, else at its
center. A null target only sets the flag that the weapons are to fire.
================
*/
// FUNCTION: STBC 0x00584390
void WeaponSystem::VirtualSlot34( ObjectClass *target, NiPoint3 *offset ) {
	if ( IsDisabled() || !on ) {
		return;
	}
	if ( target == NULL ) {
		tryingToFire = true;
		return;
	}
	WeaponSystemTarget *record = IsInTargetList( target );
	if ( record == NULL ) {
		record = new WeaponSystemTarget;
		record->targetID = target->GetObjID();
		targets.AddTail( record );
	}
	if ( offset != NULL ) {
		record->offset = *offset;
	} else {
		record->offset = NiPoint3::ZERO;
		ShipClass *ship = ShipClass::Cast( target );
		if ( ship != NULL ) {
			HullClass *hull = ship->GetHull();
			if ( hull != NULL ) {
				record->offset = hull->GetDamagePoint();
			}
		}
	}
	tryingToFire = true;
}

/*
================
WeaponSystem::UpdateWeapon

Fires the weapon once its firing interval has run, at its own target or at
the first of the system's targets it can hit; true when it fired (invented
name).
================
*/
// FUNCTION: STBC 0x00584e40
// DRAFT: x87: retail computes the time as one fld/fadd (this->time + dt) joined to a single fstp; here the stores sit per branch and the fadd operands are swapped; the not-yet-due return is a separate epilogue in retail
bool WeaponSystem::UpdateWeapon( Weapon *weapon, float deltaTime ) {
	if ( forceUpdate ) {
		weapon->unknown9C = Weapon::FIRING_INTERVAL;
	} else {
		weapon->unknown9C += deltaTime;
	}
	if ( !weapon->firing && weapon->unknown9C < Weapon::FIRING_INTERVAL ) {
		return false;
	}
	float remainder = fmod( weapon->unknown9C, Weapon::FIRING_INTERVAL );
	if ( weapon->unknown9C < Weapon::FIRING_INTERVAL ) {
		remainder = 0.0f;
	}
	weapon->unknown9C = remainder;
	if ( !weapon->CanFire() ) {
		weapon->StopFiring();
		return false;
	}
	if ( weapon->Fire( deltaTime, true ) ) {
		return true;
	}
	weapon->target.targetID = 0;
	NiTListIterator pos = targets.GetHeadPos();
	while ( pos != NULL ) {
		WeaponSystemTarget *record = targets.GetNext( pos );
		if ( DamageableObject::Cast( TGObject::GetTGObjectPtr( record->targetID ) ) != NULL ) {
			weapon->SetTarget( *record );
			if ( weapon->Fire( deltaTime, true ) == true ) {
				return true;
			}
		}
	}
	return false;
}

/*
================
WeaponSystem::UpdateWeapons

Steps the weapons of the current firing group, starting with the weapon
fired last (a single-fire system starts at it): each that is due and can
fire does. When none of the group fired, the next group of the firing chain
is tried, around to the one that started. Returns the last weapon looked at;
fired says whether one fired.
================
*/
// FUNCTION: STBC 0x00584930
// DRAFT: register/schedule: end of the first loop is loaded before lastWeaponFired in retail (edx/ecx swaps after the target list update), a few spills ordered differently
Weapon *WeaponSystem::UpdateWeapons( float deltaTime, bool *fired ) {
	*fired = false;
	Weapon *weapon = NULL;
	if ( GetParentShip()->IsDoingInSystemWarp() == true ) {
		return NULL;
	}
	UpdateTargetList();
	FiringChain *chain = GetFiringChain( firingChainMode );
	int group = lastGroupFired;
	if ( group == -1 || chain == NULL || !chain->HasGroup( group ) ) {
		if ( chain != NULL ) {
			group = chain->GetFirstGroup();
		} else {
			group = 0;
		}
	}
	int startGroup = group;
	int end = lastWeaponFired + numChildSubsystems + 1;
	int first = lastWeaponFired + 1;
	if ( IsSingleFire() == true ) {
		first = ( lastWeaponFired != -1 ) ? lastWeaponFired : 0;
	}
	for ( ; ; ) {
		NiTList<Weapon *> candidates( 2 );
		for ( int i = first; i < end; i++ ) {
			weapon = (Weapon *)GetChildSubsystem( i % numChildSubsystems );
			if ( group == 0 || weapon->IsMemberOfGroup( group ) == true ) {
				candidates.AddTail( weapon );
			}
		}
		SetGroupFireMode( group );
		NiTListIterator pos = candidates.GetHeadPos();
		while ( pos != NULL ) {
			weapon = candidates.GetNext( pos );
			bool updated = UpdateWeapon( weapon, deltaTime );
			if ( updated == true ) {
				if ( !*fired ) {
					*fired = true;
				}
			}
			if ( updated ) {
				lastWeaponFired = GetChildSubsystemIndex( weapon );
				lastGroupFired = group;
				if ( IsSingleFire() == true ) {
					break;
				}
			} else {
				weapon->target.targetID = 0;
				if ( targets.GetSize() == 0 && weapon->IsDumbFire() == true && weapon->FireDumb( deltaTime, true ) == true ) {
					lastWeaponFired = GetChildSubsystemIndex( weapon );
					lastGroupFired = group;
				}
			}
		}
		if ( chain == NULL || *fired == true ) {
			break;
		}
		group = chain->GetNextGroup( group );
		if ( group == 0 ) {
			group = chain->GetFirstGroup();
		}
		if ( group == startGroup ) {
			if ( !*fired ) {
				lastGroupFired = -1;
			}
			break;
		}
	}
	return weapon;
}

/*
================
WeaponSystem::StopFiring

Forgets the targets and stops every weapon.
================
*/
// FUNCTION: STBC 0x00584560
void WeaponSystem::StopFiring( void ) {
	tryingToFire = false;
	ClearTargetList();
	for ( int i = 0; i < numChildSubsystems; i++ ) {
		GetWeapon( i )->StopFiring();
	}
}

/*
================
WeaponSystem::StopFiringAtTarget

The weapons aiming at the target stop, and the target leaves the list;
with no target left the system stops trying to fire.
================
*/
// FUNCTION: STBC 0x005845a0
void WeaponSystem::StopFiringAtTarget( ObjectClass *target ) {
	if ( target != NULL ) {
		for ( int i = 0; i < numChildSubsystems; i++ ) {
			Weapon *weapon = GetWeapon( i );
			if ( weapon->GetTargetID() == target->GetObjID() ) {
				weapon->StopFiring();
			}
		}
		RemoveFromTargetList( target );
	}
	if ( targets.GetSize() == 0 ) {
		tryingToFire = false;
	}
}

/*
================
WeaponSystem::ClearTargetList
================
*/
// FUNCTION: STBC 0x00584750
void WeaponSystem::ClearTargetList( void ) {
	NiTListIterator pos = targets.GetHeadPos();
	while ( pos ) {
		delete targets.GetNext( pos );
	}
	targets.RemoveAll();
}

/*
================
WeaponSystem::SetFiringChainMode

Only a chain the system has.
================
*/
// FUNCTION: STBC 0x00584fa0
void WeaponSystem::SetFiringChainMode( int mode ) {
	if ( mode < GetNumFiringChains() ) {
		firingChainMode = mode;
	}
}

/*
================
WeaponSystem::DeleteFiringChains
================
*/
// FUNCTION: STBC 0x00585200
void WeaponSystem::DeleteFiringChains( void ) {
	NiTListIterator pos = firingChains.GetHeadPos();
	while ( pos ) {
		delete firingChains.GetNext( pos );
	}
	firingChains.RemoveAll();
}

/*
================
WeaponSystem::FindTarget
================
*/
// FUNCTION: STBC 0x00585360
WeaponSystemTarget *WeaponSystem::FindTarget( long targetID ) {
	NiTListIterator pos = targets.GetHeadPos();
	while ( pos ) {
		WeaponSystemTarget *target = targets.GetNext( pos );
		if ( target->targetID == targetID ) {
			return target;
		}
	}
	return NULL;
}

/*
================
WeaponSystem::SetTargetOffset
================
*/
// FUNCTION: STBC 0x00585580
void WeaponSystem::SetTargetOffset( ObjectClass *target, const NiPoint3 &offset ) {
	if ( target == NULL ) {
		return;
	}

	WeaponSystemTarget *entry = IsInTargetList( target );
	long targetID = target->GetObjID();
	if ( entry != NULL ) {
		entry->offset = offset;
	}
	for ( int i = 0; i < numChildSubsystems; i++ ) {
		Weapon *weapon = GetWeapon( i );
		if ( weapon != NULL && weapon->GetTargetID() == targetID ) {
			weapon->SetTarget( 0, NiPoint3::ZERO );
		}
	}
}

/*
================
WeaponSystem::GetLastWeaponFired
================
*/
// FUNCTION: STBC 0x00585610
Weapon *WeaponSystem::GetLastWeaponFired( void ) {
	if ( lastWeaponFired == -1 ) {
		return NULL;
	}
	return GetWeapon( lastWeaponFired );
}

/*
================
WeaponSystem::GetNextWeapon

The weapon after the one fired last, wrapping around; the first one when
none has fired.
================
*/
// FUNCTION: STBC 0x00585630
Weapon *WeaponSystem::GetNextWeapon( void ) {
	if ( lastWeaponFired == -1 ) {
		return GetWeapon( 0 );
	}
	int next = lastWeaponFired + 1;
	return GetWeapon( next % GetNumChildSubsystems() );
}

/*
================
WeaponSystem::RegisterHandlerNames

The phaser and tractor beam systems' handlers too.
================
*/
// FUNCTION: STBC 0x00584360
void WeaponSystem::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&WeaponSystem::ObjectExitedSet, "WeaponSystem::ObjectExitedSet" );
	PhaserSystem::RegisterHandlerNames();
	TractorBeamSystem::RegisterHandlerNames();
}

/*
================
WeaponSystem::RegisterHandlers

The phaser and tractor beam systems' handlers; the system's own handler
is added by the constructor.
================
*/
// FUNCTION: STBC 0x00584380
void WeaponSystem::RegisterHandlers( void ) {
	PhaserSystem::RegisterHandlers();
	TractorBeamSystem::RegisterHandlers();
}

/*
================
WeaponSystem::ObjectExitedSet

A ship leaving the set is no longer fired at.
================
*/
// FUNCTION: STBC 0x00585680
void WeaponSystem::ObjectExitedSet( TGEvent *event ) {
	ShipClass *ship = ShipClass::Cast( event->GetDestination() );
	if ( ship != NULL ) {
		StopFiringAtTarget( ship );
	}
	CallNextHandler( event );
}
