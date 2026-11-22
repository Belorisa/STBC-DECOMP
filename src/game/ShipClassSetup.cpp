/*
===============================================================================

	ShipClassSetup.cpp

	ShipClass's setup from its model's properties: the subsystems built
	from their properties, and their attachment to the ship's systems.
	The unit's name is invented: retail keeps it as a unit of its own
	(0x5B3C20-0x5B5B40, between ShipClass's and ShipSubsystems' units,
	bounded by its <iostream> quad and locale-id pair, its statics in
	.bss 0x98E90C-0x98EA30), and no string names it.

===============================================================================
*/

#include <iostream>
#include "ShipClass.h"
#include "CloakingSubsystem.h"
#include "EngineProperty.h"
#include "ImpulseEngineSubsystem.h"
#include "PhaserBank.h"
#include "PhaserSystem.h"
#include "PoweredSubsystem.h"
#include "PulseWeapon.h"
#include "PulseWeaponSystem.h"
#include "ShipProperty.h"
#include "TorpedoSystem.h"
#include "TorpedoTube.h"
#include "TractorBeamProjector.h"
#include "TractorBeamSystem.h"
#include "WarpEngineSubsystem.h"
#include "Weapon.h"
#include "WeaponSystem.h"
#include "HullClass.h"
#include "PowerSubsystem.h"
#include "RepairSubsystem.h"
#include "SensorSubsystem.h"
#include "ShieldClass.h"
#include "PhaserProperty.h"
#include "PulseWeaponProperty.h"
#include "TractorBeamProperty.h"
#include "TorpedoTubeProperty.h"
#include "TorpedoSystemProperty.h"
#include "WeaponSystemProperty.h"
#include "SubsystemProperty.h"
#include "PowerProperty.h"
#include "RepairSubsystemProperty.h"
#include "CloakingSubsystemProperty.h"
#include "ShieldProperty.h"
#include "ImpulseEngineProperty.h"
#include "WarpEngineProperty.h"
#include "HullProperty.h"
#include "SensorProperty.h"
#include "BlinkingLightProperty.h"
#include "BlinkingLightUnit.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x005b3c20 _$E32
// FUNCTION: STBC 0x005b3c30 _$E29
// FUNCTION: STBC 0x005b3c40 _$E31
// FUNCTION: STBC 0x005b3c50 _$E30
// FUNCTION: STBC 0x005b3c60 _$E38
// FUNCTION: STBC 0x005b3c70 _$E35
// FUNCTION: STBC 0x005b3c80 _$E37
// FUNCTION: STBC 0x005b3c90 _$E36

// A static of an included header: its initialiser with a guard byte, and the exit-time registration.
// FUNCTION: STBC 0x005b5ab0 _$E501
// FUNCTION: STBC 0x005b5ad0 _$E500

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x005b3ca0 _$E66
// FUNCTION: STBC 0x005b3cb0 _$E65
// FUNCTION: STBC 0x005b3cc0 _$E329
// FUNCTION: STBC 0x005b3cd0 _$E328
// FUNCTION: STBC 0x005b3ce0 _$E77
// FUNCTION: STBC 0x005b3cf0 _$E76
// FUNCTION: STBC 0x005b3d00 _$E81
// FUNCTION: STBC 0x005b3d10 _$E80
// FUNCTION: STBC 0x005b3d20 _$E85
// FUNCTION: STBC 0x005b3d30 _$E84
// FUNCTION: STBC 0x005b3d40 _$E89
// FUNCTION: STBC 0x005b3d50 _$E88
// FUNCTION: STBC 0x005b3d60 _$E206
// FUNCTION: STBC 0x005b3d70 _$E205
// FUNCTION: STBC 0x005b3d80 _$E148
// FUNCTION: STBC 0x005b3d90 _$E147
// FUNCTION: STBC 0x005b3da0 _$E161
// FUNCTION: STBC 0x005b3db0 _$E160
// FUNCTION: STBC 0x005b3dc0 _$E211
// FUNCTION: STBC 0x005b3dd0 _$E210
// FUNCTION: STBC 0x005b3de0 _$E334
// FUNCTION: STBC 0x005b3df0 _$E333
// FUNCTION: STBC 0x005b3e00 _$E215
// FUNCTION: STBC 0x005b3e10 _$E214
// FUNCTION: STBC 0x005b4ff0 _$E338
// FUNCTION: STBC 0x005b5000 _$E337
// FUNCTION: STBC 0x005b5010 _$E345
// FUNCTION: STBC 0x005b5020 _$E344
// FUNCTION: STBC 0x005b53b0 _$E355
// FUNCTION: STBC 0x005b53c0 _$E354
// FUNCTION: STBC 0x005b53d0 _$E359
// FUNCTION: STBC 0x005b53e0 _$E358
// FUNCTION: STBC 0x005b53f0 _$E106
// FUNCTION: STBC 0x005b5400 _$E105
// FUNCTION: STBC 0x005b5410 _$E234
// FUNCTION: STBC 0x005b5420 _$E233
// FUNCTION: STBC 0x005b5430 _$E241
// FUNCTION: STBC 0x005b5440 _$E240
// FUNCTION: STBC 0x005b5450 _$E245
// FUNCTION: STBC 0x005b5460 _$E244
// FUNCTION: STBC 0x005b5470 _$E249
// FUNCTION: STBC 0x005b5480 _$E248
// FUNCTION: STBC 0x005b5490 _$E253
// FUNCTION: STBC 0x005b54a0 _$E252
// FUNCTION: STBC 0x005b54b0 _$E259
// FUNCTION: STBC 0x005b54c0 _$E258
// FUNCTION: STBC 0x005b54d0 _$E275
// FUNCTION: STBC 0x005b54e0 _$E274
// FUNCTION: STBC 0x005b54f0 _$E279
// FUNCTION: STBC 0x005b5500 _$E278
// FUNCTION: STBC 0x005b5510 _$E363
// FUNCTION: STBC 0x005b5520 _$E362
// FUNCTION: STBC 0x005b5530 _$E367
// FUNCTION: STBC 0x005b5540 _$E366
// FUNCTION: STBC 0x005b5550 _$E371
// FUNCTION: STBC 0x005b5560 _$E370
// FUNCTION: STBC 0x005b5570 _$E375
// FUNCTION: STBC 0x005b5580 _$E374
// FUNCTION: STBC 0x005b5590 _$E290
// FUNCTION: STBC 0x005b55a0 _$E289
// FUNCTION: STBC 0x005b55b0 _$E379
// FUNCTION: STBC 0x005b55c0 _$E378
// FUNCTION: STBC 0x005b55d0 _$E383
// FUNCTION: STBC 0x005b55e0 _$E382
// FUNCTION: STBC 0x005b55f0 _$E387
// FUNCTION: STBC 0x005b5600 _$E386
// FUNCTION: STBC 0x005b5610 _$E111
// FUNCTION: STBC 0x005b5620 _$E110
// FUNCTION: STBC 0x005b5630 _$E391
// FUNCTION: STBC 0x005b5640 _$E390
// FUNCTION: STBC 0x005b5650 _$E116
// FUNCTION: STBC 0x005b5660 _$E115
// FUNCTION: STBC 0x005b5670 _$E395
// FUNCTION: STBC 0x005b5680 _$E394
// FUNCTION: STBC 0x005b5690 _$E399
// FUNCTION: STBC 0x005b56a0 _$E398
// FUNCTION: STBC 0x005b56b0 _$E403
// FUNCTION: STBC 0x005b56c0 _$E402
// FUNCTION: STBC 0x005b56d0 _$E407
// FUNCTION: STBC 0x005b56e0 _$E406
// FUNCTION: STBC 0x005b56f0 _$E125
// FUNCTION: STBC 0x005b5700 _$E124
// FUNCTION: STBC 0x005b5710 _$E411
// FUNCTION: STBC 0x005b5720 _$E410
// FUNCTION: STBC 0x005b5730 _$E415
// FUNCTION: STBC 0x005b5740 _$E414
// FUNCTION: STBC 0x005b5750 _$E419
// FUNCTION: STBC 0x005b5760 _$E418
// FUNCTION: STBC 0x005b5770 _$E131
// FUNCTION: STBC 0x005b5780 _$E130
// FUNCTION: STBC 0x005b5790 _$E136
// FUNCTION: STBC 0x005b57a0 _$E135
// FUNCTION: STBC 0x005b57b0 _$E166
// FUNCTION: STBC 0x005b57c0 _$E165
// FUNCTION: STBC 0x005b57d0 _$E423
// FUNCTION: STBC 0x005b57e0 _$E422
// FUNCTION: STBC 0x005b57f0 _$E171
// FUNCTION: STBC 0x005b5800 _$E170
// FUNCTION: STBC 0x005b5810 _$E427
// FUNCTION: STBC 0x005b5820 _$E426
// FUNCTION: STBC 0x005b5830 _$E431
// FUNCTION: STBC 0x005b5840 _$E430
// FUNCTION: STBC 0x005b5850 _$E176
// FUNCTION: STBC 0x005b5860 _$E175
// FUNCTION: STBC 0x005b5870 _$E435
// FUNCTION: STBC 0x005b5880 _$E434
// FUNCTION: STBC 0x005b5890 _$E141
// FUNCTION: STBC 0x005b58a0 _$E140
// FUNCTION: STBC 0x005b58b0 _$E439
// FUNCTION: STBC 0x005b58c0 _$E438
// FUNCTION: STBC 0x005b58d0 _$E443
// FUNCTION: STBC 0x005b58e0 _$E442
// FUNCTION: STBC 0x005b58f0 _$E181
// FUNCTION: STBC 0x005b5900 _$E180
// FUNCTION: STBC 0x005b5910 _$E186
// FUNCTION: STBC 0x005b5920 _$E185
// FUNCTION: STBC 0x005b5930 _$E447
// FUNCTION: STBC 0x005b5940 _$E446
// FUNCTION: STBC 0x005b5950 _$E451
// FUNCTION: STBC 0x005b5960 _$E450
// FUNCTION: STBC 0x005b5970 _$E455
// FUNCTION: STBC 0x005b5980 _$E454
// FUNCTION: STBC 0x005b5990 _$E459
// FUNCTION: STBC 0x005b59a0 _$E458
// FUNCTION: STBC 0x005b59b0 _$E463
// FUNCTION: STBC 0x005b59c0 _$E462
// FUNCTION: STBC 0x005b59d0 _$E467
// FUNCTION: STBC 0x005b59e0 _$E466
// FUNCTION: STBC 0x005b59f0 _$E471
// FUNCTION: STBC 0x005b5a00 _$E470
// FUNCTION: STBC 0x005b5a10 _$E475
// FUNCTION: STBC 0x005b5a20 _$E474
// FUNCTION: STBC 0x005b5a30 _$E191
// FUNCTION: STBC 0x005b5a40 _$E190
// FUNCTION: STBC 0x005b5a50 _$E479
// FUNCTION: STBC 0x005b5a60 _$E478
// FUNCTION: STBC 0x005b5a70 _$E196
// FUNCTION: STBC 0x005b5a80 _$E195
// FUNCTION: STBC 0x005b5a90 _$E201
// FUNCTION: STBC 0x005b5aa0 _$E200
// FUNCTION: STBC 0x005b5ae0 _$E483
// FUNCTION: STBC 0x005b5af0 _$E482
// FUNCTION: STBC 0x005b5b00 _$E488
// FUNCTION: STBC 0x005b5b10 _$E487
// FUNCTION: STBC 0x005b5b20 _$E492
// FUNCTION: STBC 0x005b5b30 _$E491

/*
================
ShipClass::VirtualSlot89

Calls Unknown5B5030 for each of the ship's subsystems.
================
*/
// FUNCTION: STBC 0x005b3e20
void ShipClass::VirtualSlot89( void ) {
	NiTListIterator pos = subsystems.list.GetHeadPos();
	while ( pos ) {
		Unknown5B5030( subsystems.list.GetNext( pos ) );
	}
}

/*
================
ShipClass::AddSubsystem

Adds the subsystem to the ship's list. Unless it is a weapon system, a
weapon, the warp engines or the cloak, it also goes in the second list.
================
*/
// FUNCTION: STBC 0x005b3e50
void ShipClass::AddSubsystem( ShipSubsystem *subsystem ) {
	subsystems.list.AddTail( subsystem );
	if ( subsystem->IsTypeOf( PHASERSYSTEM_TYPE )
		|| subsystem->IsTypeOf( TRACTORBEAMSYSTEM_TYPE )
		|| subsystem->IsTypeOf( PHASERBANK_TYPE )
		|| subsystem->IsTypeOf( TORPEDOTUBE_TYPE )
		|| subsystem->IsTypeOf( TRACTORBEAMPROJECTOR_TYPE )
		|| subsystem->IsTypeOf( PULSEWEAPON_TYPE )
		|| subsystem->IsTypeOf( WARPENGINESUBSYSTEM_TYPE )
		|| subsystem->IsTypeOf( CLOAKINGSUBSYSTEM_TYPE ) ) {
		return;
	}
	subsystems.unknown1C.AddTail( subsystem );
}

/*
================
ShipClass::VirtualSlot88

Takes one of the model's properties: the ship's own property, a blinking
light, or a subsystem's property, for which it builds the subsystem, gives
it the property and adds it to the ship. A primary system becomes the
ship's system of its kind. The weapons' orientations are made unit vectors
first (a zero forward becomes the model's forward, a zero up or right the
model's up or right), and their arcs ordered.
================
*/
// FUNCTION: STBC 0x005b3fb0
// DRAFT: inline budget: retail calls TGPoolManager::AllocateObject (0x40F030) out of line for every new but the tube's and the projector's, Length (0x4145B0) out of line in the phaser and pulse cases and TGPoint3's operator= (0x5B5B40) in the tube case; this build expands them all and calls NiPoint3::operator= instead (frame 0xCC against 0xD8)
void ShipClass::VirtualSlot88( TGModelProperty *property ) {
	switch ( property->GetObjType() ) {
	case SHIPPROPERTY_TYPE:
		SetShipProperty( ShipProperty::Cast( property ) );
		return;
	case BLINKINGLIGHTPROPERTY_TYPE:
		Unknown5B5280( property );
		break;
	}

	SubsystemProperty *subsystemProperty = SubsystemProperty::Cast( property );
	if ( subsystemProperty == NULL ) {
		return;
	}

	ShipSubsystem *subsystem = NULL;
	switch ( property->GetObjType() ) {
	case POWERPROPERTY_TYPE:
		subsystem = new PowerSubsystem;
		subsystem->SetProperty( subsystemProperty );
		if ( subsystemProperty->IsPrimary() == true ) {
			subsystems.powerSubsystem = PowerSubsystem::Cast( subsystem );
		}
		break;
	case REPAIRSUBSYSTEMPROPERTY_TYPE:
		subsystem = new RepairSubsystem;
		subsystem->SetProperty( subsystemProperty );
		if ( subsystemProperty->IsPrimary() == true ) {
			subsystems.repairSubsystem = RepairSubsystem::Cast( subsystem );
		}
		break;
	case CLOAKINGSUBSYSTEMPROPERTY_TYPE:
		subsystem = new CloakingSubsystem;
		subsystem->SetProperty( subsystemProperty );
		if ( subsystemProperty->IsPrimary() == true ) {
			subsystems.cloakingSubsystem = CloakingSubsystem::Cast( subsystem );
		}
		break;
	case SHIELDPROPERTY_TYPE:
		subsystem = new ShieldClass;
		subsystem->SetProperty( subsystemProperty );
		if ( subsystemProperty->IsPrimary() == true ) {
			subsystems.shields = ShieldClass::Cast( subsystem );
		}
		break;
	case ENGINEPROPERTY_TYPE:
		subsystem = new ShipSubsystem;
		subsystem->SetProperty( subsystemProperty );
		break;
	case IMPULSEENGINEPROPERTY_TYPE:
		subsystem = new ImpulseEngineSubsystem;
		subsystem->SetProperty( subsystemProperty );
		if ( subsystemProperty->IsPrimary() == true ) {
			subsystems.impulseEngineSubsystem = ImpulseEngineSubsystem::Cast( subsystem );
		}
		break;
	case WARPENGINEPROPERTY_TYPE:
		subsystem = new WarpEngineSubsystem;
		subsystem->SetProperty( subsystemProperty );
		if ( subsystemProperty->IsPrimary() == true ) {
			subsystems.warpEngineSubsystem = WarpEngineSubsystem::Cast( subsystem );
		}
		break;
	case HULLPROPERTY_TYPE:
		subsystem = new HullClass;
		subsystem->SetProperty( subsystemProperty );
		if ( subsystemProperty->IsPrimary() == true ) {
			subsystems.hull = HullClass::Cast( subsystem );
		}
		break;
	case SENSORPROPERTY_TYPE:
		subsystem = new SensorSubsystem;
		subsystem->SetProperty( subsystemProperty );
		if ( subsystemProperty->IsPrimary() == true ) {
			subsystems.sensorSubsystem = SensorSubsystem::Cast( subsystem );
		}
		break;
	case TORPEDOSYSTEMPROPERTY_TYPE:
		subsystem = new TorpedoSystem;
		subsystem->SetProperty( subsystemProperty );
		if ( subsystemProperty->IsPrimary() == true ) {
			subsystems.torpedoSystem = TorpedoSystem::Cast( subsystem );
		}
		TorpedoSystemProperty::Cast( subsystemProperty )->SetAimedWeapon( true );
		WeaponSystem::Cast( subsystem )->SetFiringChains( &WeaponSystemProperty::Cast( subsystemProperty )->GetFiringChainString() );
		break;
	case WEAPONSYSTEMPROPERTY_TYPE:
		switch ( WeaponSystemProperty::Cast( subsystemProperty )->GetWeaponSystemType() ) {
		case WeaponSystemProperty::WST_PHASER:
			subsystem = new PhaserSystem;
			if ( subsystemProperty->IsPrimary() == true ) {
				subsystems.phaserSystem = PhaserSystem::Cast( subsystem );
			}
			break;
		case WeaponSystemProperty::WST_PULSE:
			subsystem = new PulseWeaponSystem;
			if ( subsystemProperty->IsPrimary() == true ) {
				subsystems.pulseWeaponSystem = PulseWeaponSystem::Cast( subsystem );
			}
			break;
		case WeaponSystemProperty::WST_TRACTOR:
			subsystem = new TractorBeamSystem;
			if ( subsystemProperty->IsPrimary() == true ) {
				subsystems.tractorBeamSystem = TractorBeamSystem::Cast( subsystem );
			}
			break;
		}
		WeaponSystem::Cast( subsystem )->SetFiringChains( &WeaponSystemProperty::Cast( subsystemProperty )->GetFiringChainString() );
		subsystem->SetProperty( subsystemProperty );
		break;
	case PHASERPROPERTY_TYPE: {
		subsystem = new PhaserBank;
		subsystem->SetProperty( subsystemProperty );
		PhaserProperty *phaser = PhaserProperty::Cast( subsystemProperty );
		if ( phaser->GetArcWidthAngleMax() < phaser->GetArcWidthAngleMin() ) {
			phaser->SetArcWidthAngles( phaser->GetArcWidthAngleMax(), phaser->GetArcWidthAngleMin() );
		}
		if ( phaser->GetArcHeightAngleMax() < phaser->GetArcHeightAngleMin() ) {
			phaser->SetArcHeightAngles( phaser->GetArcHeightAngleMax(), phaser->GetArcHeightAngleMin() );
		}
		NiPoint3 forward = phaser->GetOrientationForward();
		NiPoint3 up = phaser->GetOrientationUp();
		forward.Unitize();
		up.Unitize();
		if ( forward == NiPoint3::ZERO ) {
			forward = NiPoint3::UNIT_Y;
		}
		if ( up == NiPoint3::ZERO ) {
			up = NiPoint3::UNIT_Z;
		}
		phaser->SetOrientation( forward, up );
		PhaserBank::Cast( subsystem )->Unknown572F00();
		break;
	}
	case TORPEDOTUBEPROPERTY_TYPE: {
		subsystem = new TorpedoTube;
		subsystem->SetProperty( subsystemProperty );
		TorpedoTubeProperty *tube = TorpedoTubeProperty::Cast( subsystemProperty );
		NiPoint3 direction = tube->GetDirection();
		NiPoint3 right = tube->GetRight();
		direction.Unitize();
		right.Unitize();
		if ( direction == NiPoint3::ZERO ) {
			direction = NiPoint3::UNIT_Y;
		}
		if ( right == NiPoint3::ZERO ) {
			right = NiPoint3::UNIT_X;
		}
		TGPoint3 tubeDirection = direction;
		tube->SetDirection( &tubeDirection );
		TGPoint3 tubeRight = right;
		tube->SetRight( &tubeRight );
		break;
	}
	case PULSEWEAPONPROPERTY_TYPE: {
		subsystem = new PulseWeapon;
		subsystem->SetProperty( subsystemProperty );
		PulseWeaponProperty *pulse = PulseWeaponProperty::Cast( subsystemProperty );
		if ( pulse->GetArcWidthAngleMax() < pulse->GetArcWidthAngleMin() ) {
			pulse->SetArcWidthAngles( pulse->GetArcWidthAngleMax(), pulse->GetArcWidthAngleMin() );
		}
		if ( pulse->GetArcHeightAngleMax() < pulse->GetArcHeightAngleMin() ) {
			pulse->SetArcHeightAngles( pulse->GetArcHeightAngleMax(), pulse->GetArcHeightAngleMin() );
		}
		NiPoint3 forward = pulse->GetOrientationForward();
		NiPoint3 up = pulse->GetOrientationUp();
		forward.Unitize();
		up.Unitize();
		if ( forward == NiPoint3::ZERO ) {
			forward = NiPoint3::UNIT_Y;
		}
		if ( up == NiPoint3::ZERO ) {
			up = NiPoint3::UNIT_Z;
		}
		pulse->SetOrientation( forward, up );
		break;
	}
	case TRACTORBEAMPROPERTY_TYPE: {
		subsystem = new TractorBeamProjector;
		subsystem->SetProperty( subsystemProperty );
		TractorBeamProperty *tractor = TractorBeamProperty::Cast( subsystemProperty );
		if ( tractor->GetArcWidthAngleMax() < tractor->GetArcWidthAngleMin() ) {
			tractor->SetArcWidthAngles( tractor->GetArcWidthAngleMax(), tractor->GetArcWidthAngleMin() );
		}
		if ( tractor->GetArcHeightAngleMax() < tractor->GetArcHeightAngleMin() ) {
			tractor->SetArcHeightAngles( tractor->GetArcHeightAngleMax(), tractor->GetArcHeightAngleMin() );
		}
		NiPoint3 forward = tractor->GetOrientationForward();
		NiPoint3 up = tractor->GetOrientationUp();
		forward.Unitize();
		up.Unitize();
		if ( forward == NiPoint3::ZERO ) {
			forward = NiPoint3::UNIT_Y;
		}
		if ( up == NiPoint3::ZERO ) {
			up = NiPoint3::UNIT_Z;
		}
		tractor->SetOrientation( forward, up );
		break;
	}
	default:
		return;
	}

	if ( subsystem != NULL ) {
		AddSubsystem( subsystem );
		subsystem->SetParentShip( this );
	}
}
/*
================
ShipClass::Unknown5B5030

Attaches a subsystem to the ship's systems once they are all loaded: a
powered subsystem draws on the power plant, a weapon becomes a child of
its weapon system and an engine a child of the impulse or warp engines;
both then leave the ship's own list.
================
*/
// FUNCTION: STBC 0x005b5030
void ShipClass::Unknown5B5030( ShipSubsystem *subsystem ) {
	subsystem->UpdateDamagePoint();
	if ( subsystem->IsTypeOf( POWEREDSUBSYSTEM_TYPE ) ) {
		PoweredSubsystem *powered = PoweredSubsystem::Cast( subsystem );
		if ( GetPowerSubsystem() == NULL ) {
			return;
		}
		powered->SetPowerSource( GetPowerSubsystem() );
	}
	if ( subsystem->IsTypeOf( WEAPON_TYPE ) ) {
		WeaponSystem *system;
		switch ( subsystem->GetObjType() ) {
		case PHASERBANK_TYPE:
			system = GetPhaserSystem();
			break;
		case PULSEWEAPON_TYPE:
			system = GetPulseWeaponSystem();
			break;
		case TRACTORBEAMPROJECTOR_TYPE:
			system = GetTractorBeamSystem();
			break;
		case TORPEDOTUBE_TYPE:
			system = GetTorpedoSystem();
			break;
		default:
			return;
		}
		if ( system == NULL ) {
			return;
		}
		system->AddChildSubsystem( subsystem );
		subsystems.list.Remove( subsystem );
	}
	if ( subsystem->GetProperty()->IsTypeOf( ENGINEPROPERTY_TYPE ) ) {
		EngineProperty *property = EngineProperty::Cast( subsystem->GetProperty() );
		ShipSubsystem *engines;
		switch ( property->GetEngineType() ) {
		case EngineProperty::EP_IMPULSE:
			engines = GetImpulseEngineSubsystem();
			break;
		case EngineProperty::EP_WARP:
			engines = GetWarpEngineSubsystem();
			break;
		default:
			return;
		}
		if ( engines == NULL ) {
			return;
		}
		engines->AddChildSubsystem( subsystem );
		subsystems.list.Remove( subsystem );
	}
}

/*
================
ShipClass::SetShipProperty
================
*/
// FUNCTION: STBC 0x005b5240
void ShipClass::SetShipProperty( ShipProperty *property ) {
	shipProperty = property;
	SetAffiliation( property->GetAffiliation() );
	SetMass( property->GetMass() );
	SetRotationalInertia( property->GetRotationalInertia() );
	SetStatic( property->IsStationary() );
}

/*
================
ShipClass::Unknown5B5280

A blinking light property's light, attached under the ship's node.
================
*/
// FUNCTION: STBC 0x005b5280
// DRAFT: complete; schedule: retail loads the radius, period and duration through the FPU (fld/fstp) after GetPosition, this build copies them as integers before it and spills one (frame 0x50 against 0x4C)
void ShipClass::Unknown5B5280( TGModelProperty *property ) {
	BlinkingLightProperty *light = BlinkingLightProperty::Cast( property );
	if ( light != NULL ) {
		NiColor color( light->GetColor().r, light->GetColor().g, light->GetColor().b );
		if ( node != NULL ) {
			node->AttachChild( Unknown4DB1D0( light->GetPosition(), color, light->GetRadius(), light->GetPeriod(), light->GetDuration(), light->GetTextureName() ), false );
		}
	}
}
