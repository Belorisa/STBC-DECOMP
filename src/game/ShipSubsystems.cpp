/*
===============================================================================

	ShipSubsystems.cpp

	A ship's subsystem lists and main subsystems (ShipSubsystems.h): their
	lifetime, streaming and the checksum of their figures. Retail keeps
	them as a unit of their own (0x5B5B60-0x5B7B20, bounded by its
	<iostream> quad and locale-id pair, its statics in .bss
	0x98EA34-0x98EB58).

===============================================================================
*/

#include <iostream>
#include "ShipSubsystems.h"
#include "CloakingSubsystem.h"
#include "HullClass.h"
#include "ImpulseEngineProperty.h"
#include "ImpulseEngineSubsystem.h"
#include "PhaserBank.h"
#include "PhaserSystem.h"
#include "PowerProperty.h"
#include "PowerSubsystem.h"
#include "PulseWeapon.h"
#include "PulseWeaponSystem.h"
#include "RepairSubsystem.h"
#include "RepairSubsystemProperty.h"
#include "SensorSubsystem.h"
#include "ShieldClass.h"
#include "TGStream.h"
#include "TorpedoSystem.h"
#include "TorpedoSystemProperty.h"
#include "TorpedoTube.h"
#include "TractorBeamProjector.h"
#include "TractorBeamSystem.h"
#include "WarpEngineSubsystem.h"
#include "Weapon.h"
#include "WeaponSystem.h"
#include "WeaponSystemProperty.h"
#include "EnergyWeapon.h"
#include "PhaserProperty.h"
#include "PulseWeaponProperty.h"
#include "TorpedoTubeProperty.h"
#include "TractorBeamProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x005b5b60 _$E32
// FUNCTION: STBC 0x005b5b70 _$E29
// FUNCTION: STBC 0x005b5b80 _$E31
// FUNCTION: STBC 0x005b5b90 _$E30
// FUNCTION: STBC 0x005b5ba0 _$E38
// FUNCTION: STBC 0x005b5bb0 _$E35
// FUNCTION: STBC 0x005b5bc0 _$E37
// FUNCTION: STBC 0x005b5bd0 _$E36

// A static of an included header: its initialiser with a guard byte, and the exit-time registration.
// FUNCTION: STBC 0x005b7a90 _$E484
// FUNCTION: STBC 0x005b7ab0 _$E483

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x005b5be0 _$E78
// FUNCTION: STBC 0x005b5bf0 _$E77
// FUNCTION: STBC 0x005b5c00 _$E313
// FUNCTION: STBC 0x005b5c10 _$E312
// FUNCTION: STBC 0x005b5c20 _$E89
// FUNCTION: STBC 0x005b5c30 _$E88
// FUNCTION: STBC 0x005b5c40 _$E93
// FUNCTION: STBC 0x005b5c50 _$E92
// FUNCTION: STBC 0x005b5c60 _$E97
// FUNCTION: STBC 0x005b5c70 _$E96
// FUNCTION: STBC 0x005b5c80 _$E101
// FUNCTION: STBC 0x005b5c90 _$E100
// FUNCTION: STBC 0x005b5ca0 _$E216
// FUNCTION: STBC 0x005b5cb0 _$E215
// FUNCTION: STBC 0x005b5cc0 _$E158
// FUNCTION: STBC 0x005b5cd0 _$E157
// FUNCTION: STBC 0x005b5ce0 _$E171
// FUNCTION: STBC 0x005b5cf0 _$E170
// FUNCTION: STBC 0x005b5d60 _$E221
// FUNCTION: STBC 0x005b5d70 _$E220
// FUNCTION: STBC 0x005b6bd0 _$E318
// FUNCTION: STBC 0x005b6be0 _$E317
// FUNCTION: STBC 0x005b6bf0 _$E241
// FUNCTION: STBC 0x005b6c00 _$E240
// FUNCTION: STBC 0x005b7140 _$E322
// FUNCTION: STBC 0x005b7150 _$E321
// FUNCTION: STBC 0x005b7370 _$E329
// FUNCTION: STBC 0x005b7380 _$E328
// FUNCTION: STBC 0x005b7390 _$E339
// FUNCTION: STBC 0x005b73a0 _$E338
// FUNCTION: STBC 0x005b73b0 _$E343
// FUNCTION: STBC 0x005b73c0 _$E342
// FUNCTION: STBC 0x005b73d0 _$E120
// FUNCTION: STBC 0x005b73e0 _$E119
// FUNCTION: STBC 0x005b73f0 _$E246
// FUNCTION: STBC 0x005b7400 _$E245
// FUNCTION: STBC 0x005b7410 _$E253
// FUNCTION: STBC 0x005b7420 _$E252
// FUNCTION: STBC 0x005b7430 _$E257
// FUNCTION: STBC 0x005b7440 _$E256
// FUNCTION: STBC 0x005b7450 _$E261
// FUNCTION: STBC 0x005b7460 _$E260
// FUNCTION: STBC 0x005b7470 _$E265
// FUNCTION: STBC 0x005b7480 _$E264
// FUNCTION: STBC 0x005b7490 _$E271
// FUNCTION: STBC 0x005b74a0 _$E270
// FUNCTION: STBC 0x005b74b0 _$E225
// FUNCTION: STBC 0x005b74c0 _$E224
// FUNCTION: STBC 0x005b74d0 _$E229
// FUNCTION: STBC 0x005b74e0 _$E228
// FUNCTION: STBC 0x005b74f0 _$E347
// FUNCTION: STBC 0x005b7500 _$E346
// FUNCTION: STBC 0x005b7510 _$E351
// FUNCTION: STBC 0x005b7520 _$E350
// FUNCTION: STBC 0x005b7530 _$E355
// FUNCTION: STBC 0x005b7540 _$E354
// FUNCTION: STBC 0x005b7550 _$E359
// FUNCTION: STBC 0x005b7560 _$E358
// FUNCTION: STBC 0x005b7570 _$E275
// FUNCTION: STBC 0x005b7580 _$E274
// FUNCTION: STBC 0x005b7590 _$E363
// FUNCTION: STBC 0x005b75a0 _$E362
// FUNCTION: STBC 0x005b75b0 _$E367
// FUNCTION: STBC 0x005b75c0 _$E366
// FUNCTION: STBC 0x005b75d0 _$E371
// FUNCTION: STBC 0x005b75e0 _$E370
// FUNCTION: STBC 0x005b75f0 _$E125
// FUNCTION: STBC 0x005b7600 _$E124
// FUNCTION: STBC 0x005b7610 _$E375
// FUNCTION: STBC 0x005b7620 _$E374
// FUNCTION: STBC 0x005b7630 _$E130
// FUNCTION: STBC 0x005b7640 _$E129
// FUNCTION: STBC 0x005b7650 _$E379
// FUNCTION: STBC 0x005b7660 _$E378
// FUNCTION: STBC 0x005b7670 _$E383
// FUNCTION: STBC 0x005b7680 _$E382
// FUNCTION: STBC 0x005b7690 _$E387
// FUNCTION: STBC 0x005b76a0 _$E386
// FUNCTION: STBC 0x005b76b0 _$E391
// FUNCTION: STBC 0x005b76c0 _$E390
// FUNCTION: STBC 0x005b76d0 _$E135
// FUNCTION: STBC 0x005b76e0 _$E134
// FUNCTION: STBC 0x005b76f0 _$E395
// FUNCTION: STBC 0x005b7700 _$E394
// FUNCTION: STBC 0x005b7710 _$E399
// FUNCTION: STBC 0x005b7720 _$E398
// FUNCTION: STBC 0x005b7730 _$E403
// FUNCTION: STBC 0x005b7740 _$E402
// FUNCTION: STBC 0x005b7750 _$E141
// FUNCTION: STBC 0x005b7760 _$E140
// FUNCTION: STBC 0x005b7770 _$E146
// FUNCTION: STBC 0x005b7780 _$E145
// FUNCTION: STBC 0x005b7790 _$E176
// FUNCTION: STBC 0x005b77a0 _$E175
// FUNCTION: STBC 0x005b77b0 _$E407
// FUNCTION: STBC 0x005b77c0 _$E406
// FUNCTION: STBC 0x005b77d0 _$E181
// FUNCTION: STBC 0x005b77e0 _$E180
// FUNCTION: STBC 0x005b77f0 _$E411
// FUNCTION: STBC 0x005b7800 _$E410
// FUNCTION: STBC 0x005b7810 _$E415
// FUNCTION: STBC 0x005b7820 _$E414
// FUNCTION: STBC 0x005b7830 _$E186
// FUNCTION: STBC 0x005b7840 _$E185
// FUNCTION: STBC 0x005b7850 _$E419
// FUNCTION: STBC 0x005b7860 _$E418
// FUNCTION: STBC 0x005b7870 _$E151
// FUNCTION: STBC 0x005b7880 _$E150
// FUNCTION: STBC 0x005b7890 _$E423
// FUNCTION: STBC 0x005b78a0 _$E422
// FUNCTION: STBC 0x005b78b0 _$E427
// FUNCTION: STBC 0x005b78c0 _$E426
// FUNCTION: STBC 0x005b78d0 _$E191
// FUNCTION: STBC 0x005b78e0 _$E190
// FUNCTION: STBC 0x005b78f0 _$E196
// FUNCTION: STBC 0x005b7900 _$E195
// FUNCTION: STBC 0x005b7910 _$E431
// FUNCTION: STBC 0x005b7920 _$E430
// FUNCTION: STBC 0x005b7930 _$E435
// FUNCTION: STBC 0x005b7940 _$E434
// FUNCTION: STBC 0x005b7950 _$E439
// FUNCTION: STBC 0x005b7960 _$E438
// FUNCTION: STBC 0x005b7970 _$E443
// FUNCTION: STBC 0x005b7980 _$E442
// FUNCTION: STBC 0x005b7990 _$E447
// FUNCTION: STBC 0x005b79a0 _$E446
// FUNCTION: STBC 0x005b79b0 _$E451
// FUNCTION: STBC 0x005b79c0 _$E450
// FUNCTION: STBC 0x005b79d0 _$E455
// FUNCTION: STBC 0x005b79e0 _$E454
// FUNCTION: STBC 0x005b79f0 _$E459
// FUNCTION: STBC 0x005b7a00 _$E458
// FUNCTION: STBC 0x005b7a10 _$E201
// FUNCTION: STBC 0x005b7a20 _$E200
// FUNCTION: STBC 0x005b7a30 _$E463
// FUNCTION: STBC 0x005b7a40 _$E462
// FUNCTION: STBC 0x005b7a50 _$E206
// FUNCTION: STBC 0x005b7a60 _$E205
// FUNCTION: STBC 0x005b7a70 _$E211
// FUNCTION: STBC 0x005b7a80 _$E210
// FUNCTION: STBC 0x005b7ac0 _$E467
// FUNCTION: STBC 0x005b7ad0 _$E466
// FUNCTION: STBC 0x005b7ae0 _$E472
// FUNCTION: STBC 0x005b7af0 _$E471
// FUNCTION: STBC 0x005b7b00 _$E476
// FUNCTION: STBC 0x005b7b10 _$E475

/*
================
ShipSubsystems::ShipSubsystems
================
*/
// FUNCTION: STBC 0x005b5d00
ShipSubsystems::ShipSubsystems( void ) : list( 2 ), unknown1C( 2 ) {
	powerSubsystem = NULL;
	hull = NULL;
	shields = NULL;
	torpedoSystem = NULL;
	phaserSystem = NULL;
	pulseWeaponSystem = NULL;
	sensorSubsystem = NULL;
	impulseEngineSubsystem = NULL;
	warpEngineSubsystem = NULL;
	tractorBeamSystem = NULL;
	repairSubsystem = NULL;
	cloakingSubsystem = NULL;
}

/*
================
ShipSubsystems::~ShipSubsystems

Deletes every subsystem the ship has and empties both lists.
================
*/
// FUNCTION: STBC 0x005b5d80
ShipSubsystems::~ShipSubsystems( void ) {
	NiTListIterator pos = list.GetHeadPos();
	while ( pos ) {
		ShipSubsystem *subsystem = list.GetNext( pos );
		if ( subsystem != NULL ) {
			delete subsystem;
		}
	}
	list.RemoveAll();
	unknown1C.RemoveAll();
}

// The tag written before each subsystem in the stream; the reader skips it.
#define SUBSYSTEM_STREAM_TAG		0xEA

/*
================
ShipSubsystems::Unknown5B5EB0
================
*/
// FUNCTION: STBC 0x005b5eb0
// PARKED: register allocation: retail rotates the scratch register that forms &checksum (ecx/edx/eax) differently from the sensors on, and loads the impulse engines before the property's register move; calls, loads and branches match
int ShipSubsystems::Unknown5B5EB0( void ) {
	int checksum = 0;
	if ( hull != NULL ) {
		Unknown5B6C10( Unknown5B6170( hull ), &checksum );
	}
	if ( shields != NULL ) {
		ShieldProperty *shieldProperty = shields->GetProperty();
		Unknown5B6C10( Unknown5B6170( shields ), &checksum );
		for ( int i = 0; i < ShieldClass::NUM_SHIELDS; i++ ) {
			Unknown5B6C10( shieldProperty->GetMaxShields( i ), &checksum );
			Unknown5B6C10( shieldProperty->GetShieldChargePerSecond( i ), &checksum );
		}
	}
	if ( powerSubsystem != NULL ) {
		PowerProperty *powerProperty = powerSubsystem->GetProperty();
		Unknown5B6C10( Unknown5B6170( powerSubsystem ), &checksum );
		Unknown5B6C10( powerProperty->GetBackupBatteryLimit(), &checksum );
		Unknown5B6C10( powerProperty->GetMainBatteryLimit(), &checksum );
		Unknown5B6C10( powerProperty->GetBackupConduitCapacity(), &checksum );
		Unknown5B6C10( powerProperty->GetMainConduitCapacity(), &checksum );
		Unknown5B6C10( powerProperty->GetPowerOutput(), &checksum );
	}
	if ( sensorSubsystem != NULL ) {
		SensorProperty *sensorProperty = sensorSubsystem->GetProperty();
		Unknown5B6C10( Unknown5B6170( sensorSubsystem ), &checksum );
		Unknown5B6C10( sensorProperty->GetBaseSensorRange(), &checksum );
	}
	if ( impulseEngineSubsystem != NULL ) {
		ImpulseEngineProperty *impulseProperty = impulseEngineSubsystem->GetProperty();
		Unknown5B6C10( Unknown5B6170( impulseEngineSubsystem ), &checksum );
		Unknown5B6C10( impulseProperty->GetMaxAccel(), &checksum );
		Unknown5B6C10( impulseProperty->GetMaxAngularAccel(), &checksum );
		Unknown5B6C10( impulseProperty->GetMaxAngularVelocity(), &checksum );
		Unknown5B6C10( impulseProperty->GetMaxSpeed(), &checksum );
	}
	if ( warpEngineSubsystem != NULL ) {
		Unknown5B6C10( Unknown5B6170( warpEngineSubsystem ), &checksum );
	}
	if ( repairSubsystem != NULL ) {
		RepairSubsystemProperty *repairProperty = repairSubsystem->GetProperty();
		Unknown5B6C10( Unknown5B6170( repairSubsystem ), &checksum );
		Unknown5B6C10( repairProperty->GetMaxRepairPoints(), &checksum );
	}
	if ( cloakingSubsystem != NULL ) {
		// BUG: redundant: the cloak's property is fetched and never used
		cloakingSubsystem->GetProperty();
		Unknown5B6C10( Unknown5B6170( cloakingSubsystem ), &checksum );
	}
	if ( torpedoSystem != NULL ) {
		Unknown5B6C10( Unknown5B6330( torpedoSystem ), &checksum );
	}
	if ( phaserSystem != NULL ) {
		Unknown5B6C10( Unknown5B6330( phaserSystem ), &checksum );
	}
	if ( pulseWeaponSystem != NULL ) {
		Unknown5B6C10( Unknown5B6330( pulseWeaponSystem ), &checksum );
	}
	if ( tractorBeamSystem != NULL ) {
		Unknown5B6C10( Unknown5B6330( tractorBeamSystem ), &checksum );
	}
	return checksum;
}

/*
================
ShipSubsystems::Unknown5B6170
================
*/
// FUNCTION: STBC 0x005b6170
// PARKED: register allocation: retail keeps zero in EBX from the entry (for the checksum's first store and the child count's test) and so reuses it as the loop counter; this build stores an immediate. A counter declared at the top drops the second count test. Calls, constants and branches match
float ShipSubsystems::Unknown5B6170( ShipSubsystem *subsystem ) {
	float checksum = 0.0f;
	SubsystemProperty *property = subsystem->GetProperty();
	NiPoint3 position = property->GetPosition();
	Unknown5B6C10( property->GetMaxCondition(), &checksum );
	Unknown5B6C10( property->GetDisabledPercentage(), &checksum );
	Unknown5B6C10( position.x, &checksum );
	Unknown5B6C10( position.y, &checksum );
	Unknown5B6C10( position.z, &checksum );
	Unknown5B6C10( property->GetRadius(), &checksum );
	Unknown5B6C10( property->GetRepairComplexity(), &checksum );
	if ( subsystem->GetNumChildSubsystems() > 0 ) {
		for ( int i = 0; i < subsystem->GetNumChildSubsystems(); i++ ) {
			Unknown5B6C10( Unknown5B6170( subsystem->GetChildSubsystem( i ) ), &checksum );
		}
	}
	Unknown5B6C10( subsystem->IsCritical() == true ? 64.001f : 76.6f, &checksum );
	Unknown5B6C10( subsystem->IsInvincible() == true ? 98.6f : 100.0f, &checksum );
	Unknown5B6C10( subsystem->IsTargetable() == true ? 14.3f : 456.1f, &checksum );
	Unknown5B6C10( property->IsPrimary() == true ? 27.3f : 16.1f, &checksum );
	PoweredSubsystem *powered = PoweredSubsystem::Cast( subsystem );
	if ( powered != NULL ) {
		PoweredSubsystemProperty *poweredProperty = powered->GetProperty();
		Unknown5B6C10( poweredProperty->GetNormalPowerPerSecond(), &checksum );
	}
	return checksum;
}

/*
================
ShipSubsystems::Unknown5B6330
================
*/
// FUNCTION: STBC 0x005b6330
float ShipSubsystems::Unknown5B6330( WeaponSystem *system ) {
	float checksum = 0.0f;
	WeaponSystemProperty *property = system->GetProperty();
	Unknown5B6C10( Unknown5B6170( system ), &checksum );
	Unknown5B6C10( property->IsSingleFire() ? 0.4f : 99.1f, &checksum );
	Unknown5B6C10( property->IsAimedWeapon() ? 32.6f : 487.1f, &checksum );
	for ( int i = 0; i < system->GetNumChildSubsystems(); i++ ) {
		Weapon *weapon = Weapon::Cast( system->GetChildSubsystem( i ) );
		Unknown5B6C10( Unknown5B6560( weapon ), &checksum );
	}

	TorpedoSystem *torpedoes = TorpedoSystem::Cast( system );
	if ( torpedoes != NULL ) {
		TorpedoSystemProperty *torpedoProperty = torpedoes->GetProperty();
		for ( int type = 0; type < torpedoProperty->GetNumAmmoTypes(); type++ ) {
			Unknown5B6C10( (float)torpedoProperty->GetMaxTorpedoes( type ), &checksum );

			char *script = torpedoProperty->GetTorpedoScript( type );
			int length = strlen( script );
			for ( int j = 0; j < length; j++ ) {
				int product = script[j] * script[length - 1 - j];
				Unknown5B6C10( (float)product, &checksum );
			}

			TorpedoAmmoType *ammo = torpedoProperty->GetAmmoType( type );
			char *name = ammo->GetAmmoName();
			length = strlen( name );
			for ( int k = 0; k < length; k++ ) {
				int product = name[k] * name[length - 1 - k];
				Unknown5B6C10( (float)product, &checksum );
			}

			Unknown5B6C10( ammo->GetLaunchSpeed() * ammo->GetMaxTorpedoes(), &checksum );
		}
	}
	return checksum;
}

/*
================
ShipSubsystems::Unknown5B6560

The checksum of a weapon's figures: those of every subsystem, an energy
weapon's charge and damage figures, and the orientation, size, arcs and
timing of its kind's property.
================
*/
// FUNCTION: STBC 0x005b6560
// DRAFT: register allocation: retail keeps zero in EBP from the entry (the checksum's first store, the casts' NULL tests, the module name's sum) and so shifts the frame by a slot; calls, loads and branches match (as in Unknown5B6170)
float ShipSubsystems::Unknown5B6560( Weapon *weapon ) {
	float checksum = 0.0f;
	Unknown5B6C10( Unknown5B6170( weapon ), &checksum );

	EnergyWeapon *energyWeapon = EnergyWeapon::Cast( weapon );
	if ( energyWeapon != NULL ) {
		Unknown5B6C10( energyWeapon->GetDamageRadiusFactor(), &checksum );
		Unknown5B6C10( energyWeapon->GetMaxCharge(), &checksum );
		Unknown5B6C10( energyWeapon->GetMaxDamage(), &checksum );
		Unknown5B6C10( energyWeapon->GetMaxDamageDistance(), &checksum );
		Unknown5B6C10( energyWeapon->GetMinFiringCharge(), &checksum );
		Unknown5B6C10( energyWeapon->GetNormalDischargeRate(), &checksum );
		Unknown5B6C10( energyWeapon->GetRechargeRate(), &checksum );
	}

	PhaserBank *phaser = PhaserBank::Cast( weapon );
	if ( phaser != NULL ) {
		PhaserProperty *property = phaser->GetProperty();
		NiPoint3 forward = property->GetOrientationForward();
		NiPoint3 up = property->GetOrientationUp();
		Unknown5B6C10( forward.x, &checksum );
		Unknown5B6C10( forward.y, &checksum );
		Unknown5B6C10( forward.z, &checksum );
		Unknown5B6C10( up.x, &checksum );
		Unknown5B6C10( up.y, &checksum );
		Unknown5B6C10( up.z, &checksum );
		Unknown5B6C10( property->GetWidth(), &checksum );
		Unknown5B6C10( property->GetLength(), &checksum );
		Unknown5B6C10( property->GetArcHeightAngleMax(), &checksum );
		Unknown5B6C10( property->GetArcHeightAngleMin(), &checksum );
		Unknown5B6C10( property->GetArcWidthAngleMax(), &checksum );
		Unknown5B6C10( property->GetArcWidthAngleMin(), &checksum );
	}

	TorpedoTube *tube = TorpedoTube::Cast( weapon );
	if ( tube != NULL ) {
		TorpedoTubeProperty *property = tube->GetProperty();
		NiPoint3 direction = property->GetDirection();
		NiPoint3 right = property->GetRight();
		Unknown5B6C10( direction.x, &checksum );
		Unknown5B6C10( direction.y, &checksum );
		Unknown5B6C10( direction.z, &checksum );
		Unknown5B6C10( right.x, &checksum );
		Unknown5B6C10( right.y, &checksum );
		Unknown5B6C10( right.z, &checksum );
		Unknown5B6C10( property->GetImmediateDelay(), &checksum );
		Unknown5B6C10( property->GetReloadDelay(), &checksum );
		Unknown5B6C10( (float)property->GetMaxReady(), &checksum );
	}

	PulseWeapon *pulse = PulseWeapon::Cast( weapon );
	if ( pulse != NULL ) {
		PulseWeaponProperty *property = pulse->GetProperty();
		NiPoint3 forward = property->GetOrientationForward();
		NiPoint3 up = property->GetOrientationUp();
		NiPoint3 right = property->GetOrientationRight();
		Unknown5B6C10( forward.x, &checksum );
		Unknown5B6C10( forward.y, &checksum );
		Unknown5B6C10( forward.z, &checksum );
		Unknown5B6C10( up.x, &checksum );
		Unknown5B6C10( up.y, &checksum );
		Unknown5B6C10( up.z, &checksum );
		Unknown5B6C10( right.x, &checksum );
		Unknown5B6C10( right.y, &checksum );
		Unknown5B6C10( right.z, &checksum );
		Unknown5B6C10( property->GetArcHeightAngleMax(), &checksum );
		Unknown5B6C10( property->GetArcHeightAngleMin(), &checksum );
		Unknown5B6C10( property->GetArcWidthAngleMax(), &checksum );
		Unknown5B6C10( property->GetArcWidthAngleMin(), &checksum );
		Unknown5B6C10( property->GetCooldownTime(), &checksum );

		char *moduleName = property->GetModuleName();
		int length = strlen( moduleName );
		int sum = 0;
		for ( int i = 0; i < length; i++ ) {
			sum += moduleName[i] * moduleName[length - 1 - i];
		}
		Unknown5B6C10( (float)sum, &checksum );
	}

	TractorBeamProjector *projector = TractorBeamProjector::Cast( weapon );
	if ( projector != NULL ) {
		TractorBeamProperty *property = projector->GetProperty();
		NiPoint3 forward = property->GetOrientationForward();
		NiPoint3 up = property->GetOrientationUp();
		NiPoint3 right = property->GetOrientationRight();
		Unknown5B6C10( forward.x, &checksum );
		Unknown5B6C10( forward.y, &checksum );
		Unknown5B6C10( forward.z, &checksum );
		Unknown5B6C10( up.x, &checksum );
		Unknown5B6C10( up.y, &checksum );
		Unknown5B6C10( up.z, &checksum );
		Unknown5B6C10( right.x, &checksum );
		Unknown5B6C10( right.y, &checksum );
		Unknown5B6C10( right.z, &checksum );
		Unknown5B6C10( property->GetArcHeightAngleMax(), &checksum );
		Unknown5B6C10( property->GetArcHeightAngleMin(), &checksum );
		Unknown5B6C10( property->GetArcWidthAngleMax(), &checksum );
		Unknown5B6C10( property->GetArcWidthAngleMin(), &checksum );
	}
	return checksum;
}

// The scales Unknown5B6C10 applies to small figures before it truncates
// them.
// DATA: STBC 0x008944d8
static const float CHECKSUM_SCALE = 8192.0f;
// DATA: STBC 0x008944dc
static const float CHECKSUM_FRACTION_SCALE = 65536.0f;

/*
================
ShipSubsystems::Unknown5B6C10
================
*/
// FUNCTION: STBC 0x005b6c10
// ORIGINAL-ASM: the rotation is the original's inline `__asm rol` (an EBP frame and `rol dword ptr [ebp+8],1` that VC6 emits only for inline assembly), as in NetFile::GetFileChecksum
void ShipSubsystems::Unknown5B6C10( float value, void *checksum ) {
	bool negative = false;
	if ( value < 0.0f ) {
		value = -value;
		negative = true;
	}
	if ( value < 1.0f ) {
		value *= CHECKSUM_FRACTION_SCALE;
	} else if ( value < CHECKSUM_SCALE ) {
		value *= CHECKSUM_SCALE;
	}
	int bits = (int)value;
	if ( negative ) {
		bits = -bits;
	}
	char *bytes = (char *)checksum;
	char *valueBytes = (char *)&bits;
	for ( int i = 0; i < 4; i++ ) {
		bytes[i] ^= valueBytes[i];
	}
	int rotated = *(int *)checksum;
	__asm rol rotated, 1
	*(int *)checksum = rotated;
}

/*
================
ShipSubsystems::WriteToStream

Each subsystem in the list is written whole, behind a continue flag and
the tag the reader skips; the second list and the main subsystems are
written as IDs.
================
*/
// FUNCTION: STBC 0x005b6ca0 ?WriteToStream@ShipSubsystems@@UAE_NPAVTGStream@@@Z
bool ShipSubsystems::WriteToStream( TGStream *stream ) {
	NiTListIterator pos = list.GetHeadPos();
	while ( pos ) {
		ShipSubsystem *subsystem = list.GetNext( pos );
		if ( subsystem != NULL ) {
			stream->WriteBool( true );
			stream->WriteLong( SUBSYSTEM_STREAM_TAG );
			subsystem->WriteToStream( stream );
		}
	}
	stream->WriteBool( false );

	pos = unknown1C.GetHeadPos();
	while ( pos ) {
		ShipSubsystem *subsystem = unknown1C.GetNext( pos );
		if ( subsystem != NULL ) {
			stream->WriteBool( true );
			stream->WriteID( subsystem->GetObjID() );
		}
	}
	stream->WriteBool( false );

	stream->WriteID( TGObject::GetSavedID( powerSubsystem ) );
	stream->WriteID( TGObject::GetSavedID( hull ) );
	stream->WriteID( TGObject::GetSavedID( shields ) );
	stream->WriteID( TGObject::GetSavedID( torpedoSystem ) );
	stream->WriteID( TGObject::GetSavedID( phaserSystem ) );
	stream->WriteID( TGObject::GetSavedID( pulseWeaponSystem ) );
	stream->WriteID( TGObject::GetSavedID( sensorSubsystem ) );
	stream->WriteID( TGObject::GetSavedID( impulseEngineSubsystem ) );
	stream->WriteID( TGObject::GetSavedID( warpEngineSubsystem ) );
	stream->WriteID( TGObject::GetSavedID( tractorBeamSystem ) );
	stream->WriteID( TGObject::GetSavedID( repairSubsystem ) );
	stream->WriteID( TGObject::GetSavedID( cloakingSubsystem ) );
	return true;
}

/*
================
ShipSubsystems::ReadFromStream

Reads what WriteToStream wrote. The second list and the main subsystems
hold the IDs until the fix-up pass (VirtualSlot02) checks them.
================
*/
// FUNCTION: STBC 0x005b6e40 ?ReadFromStream@ShipSubsystems@@UAE_NPAVTGStream@@@Z
bool ShipSubsystems::ReadFromStream( TGStream *stream ) {
	if ( stream->ReadBool() == true ) {
		do {
			stream->ReadLong();
			list.AddTail( (ShipSubsystem *)TGObject::LoadObject( stream, 0 ) );
		} while ( stream->ReadBool() == true );
	}
	if ( stream->ReadBool() == true ) {
		do {
			unknown1C.AddTail( (ShipSubsystem *)stream->ReadID() );
		} while ( stream->ReadBool() == true );
	}
	powerSubsystem = (PowerSubsystem *)stream->ReadID();
	hull = (HullClass *)stream->ReadID();
	shields = (ShieldClass *)stream->ReadID();
	torpedoSystem = (TorpedoSystem *)stream->ReadID();
	phaserSystem = (PhaserSystem *)stream->ReadID();
	pulseWeaponSystem = (PulseWeaponSystem *)stream->ReadID();
	sensorSubsystem = (SensorSubsystem *)stream->ReadID();
	impulseEngineSubsystem = (ImpulseEngineSubsystem *)stream->ReadID();
	warpEngineSubsystem = (WarpEngineSubsystem *)stream->ReadID();
	tractorBeamSystem = (TractorBeamSystem *)stream->ReadID();
	repairSubsystem = (RepairSubsystem *)stream->ReadID();
	cloakingSubsystem = (CloakingSubsystem *)stream->ReadID();
	return true;
}

/*
================
ShipSubsystems::VirtualSlot02

Whether every ID read by ReadFromStream names a loaded object.
================
*/
// FUNCTION: STBC 0x005b6fe0 ?VirtualSlot02@ShipSubsystems@@UAE_NXZ
bool ShipSubsystems::VirtualSlot02( void ) {
	NiTListIterator pos = unknown1C.GetHeadPos();
	while ( pos ) {
		ShipSubsystem *subsystem = unknown1C.GetNext( pos );
		if ( subsystem != NULL && TGObject::GetTGObjectPtr( (long)subsystem ) == NULL ) {
			return false;
		}
	}
	if ( powerSubsystem != NULL && TGObject::GetTGObjectPtr( (long)powerSubsystem ) == NULL ) {
		return false;
	}
	if ( hull != NULL && TGObject::GetTGObjectPtr( (long)hull ) == NULL ) {
		return false;
	}
	if ( shields != NULL && TGObject::GetTGObjectPtr( (long)shields ) == NULL ) {
		return false;
	}
	if ( torpedoSystem != NULL && TGObject::GetTGObjectPtr( (long)torpedoSystem ) == NULL ) {
		return false;
	}
	if ( phaserSystem != NULL && TGObject::GetTGObjectPtr( (long)phaserSystem ) == NULL ) {
		return false;
	}
	if ( pulseWeaponSystem != NULL && TGObject::GetTGObjectPtr( (long)pulseWeaponSystem ) == NULL ) {
		return false;
	}
	if ( sensorSubsystem != NULL && TGObject::GetTGObjectPtr( (long)sensorSubsystem ) == NULL ) {
		return false;
	}
	if ( impulseEngineSubsystem != NULL && TGObject::GetTGObjectPtr( (long)impulseEngineSubsystem ) == NULL ) {
		return false;
	}
	if ( warpEngineSubsystem != NULL && TGObject::GetTGObjectPtr( (long)warpEngineSubsystem ) == NULL ) {
		return false;
	}
	if ( tractorBeamSystem != NULL && TGObject::GetTGObjectPtr( (long)tractorBeamSystem ) == NULL ) {
		return false;
	}
	if ( repairSubsystem != NULL && TGObject::GetTGObjectPtr( (long)repairSubsystem ) == NULL ) {
		return false;
	}
	if ( cloakingSubsystem != NULL && TGObject::GetTGObjectPtr( (long)cloakingSubsystem ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
ShipSubsystems::VirtualSlot03

Turns the IDs ReadFromStream read into the objects they name; the second
list is rebuilt from the subsystems among them.
================
*/
// FUNCTION: STBC 0x005b7160
void ShipSubsystems::VirtualSlot03( void ) {
	NiTListIterator pos = unknown1C.GetHeadPos();
	while ( pos ) {
		ShipSubsystem *id = unknown1C.Get( pos );
		NiTListIterator next = unknown1C.GetNextPos( pos );
		ShipSubsystem *subsystem = ShipSubsystem::Cast( TGObject::GetTGObjectPtr( (long)id ) );
		if ( subsystem != NULL ) {
			unknown1C.AddHead( subsystem );
		}
		unknown1C.RemovePos( pos );
		pos = next;
	}
	if ( powerSubsystem != NULL ) {
		powerSubsystem = (PowerSubsystem *)TGObject::GetTGObjectPtr( (long)powerSubsystem );
	}
	if ( hull != NULL ) {
		hull = (HullClass *)TGObject::GetTGObjectPtr( (long)hull );
	}
	if ( shields != NULL ) {
		shields = (ShieldClass *)TGObject::GetTGObjectPtr( (long)shields );
	}
	if ( torpedoSystem != NULL ) {
		torpedoSystem = (TorpedoSystem *)TGObject::GetTGObjectPtr( (long)torpedoSystem );
	}
	if ( phaserSystem != NULL ) {
		phaserSystem = (PhaserSystem *)TGObject::GetTGObjectPtr( (long)phaserSystem );
	}
	if ( pulseWeaponSystem != NULL ) {
		pulseWeaponSystem = (PulseWeaponSystem *)TGObject::GetTGObjectPtr( (long)pulseWeaponSystem );
	}
	if ( sensorSubsystem != NULL ) {
		sensorSubsystem = (SensorSubsystem *)TGObject::GetTGObjectPtr( (long)sensorSubsystem );
	}
	if ( impulseEngineSubsystem != NULL ) {
		impulseEngineSubsystem = (ImpulseEngineSubsystem *)TGObject::GetTGObjectPtr( (long)impulseEngineSubsystem );
	}
	if ( warpEngineSubsystem != NULL ) {
		warpEngineSubsystem = (WarpEngineSubsystem *)TGObject::GetTGObjectPtr( (long)warpEngineSubsystem );
	}
	if ( tractorBeamSystem != NULL ) {
		tractorBeamSystem = (TractorBeamSystem *)TGObject::GetTGObjectPtr( (long)tractorBeamSystem );
	}
	if ( repairSubsystem != NULL ) {
		repairSubsystem = (RepairSubsystem *)TGObject::GetTGObjectPtr( (long)repairSubsystem );
	}
	if ( cloakingSubsystem != NULL ) {
		cloakingSubsystem = (CloakingSubsystem *)TGObject::GetTGObjectPtr( (long)cloakingSubsystem );
	}
}
