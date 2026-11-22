/*
===============================================================================

	TractorBeamProjector.cpp

===============================================================================
*/

#include "TractorBeamProjector.h"
#include "TractorBeamProperty.h"
#include "PowerSubsystem.h"
#include "ShipClass.h"
#include "TGEventHandlerTable.h"
#include "TGStream.h"
#include <iostream>
#include "DamageableObject.h"
#include "Force.h"
#include "TractorBeamGraphic.h"
#include "TractorBeamSystem.h"
#include "ProximityManager.h"
#include "SetClass.h"
#include "WeaponSystem.h"
#include "NiMath.h"
#include "TGModelUtils.h"
#include "UtopiaApp.h"
#include "Game.h"
#include "TGSound.h"
#include "TGSoundRegion.h"
#include "ImpulseEngineSubsystem.h"
#include "TGEventManager.h"
#include "TGObjPtrEvent.h"
#include "TGSoundManager.h"
#include "NiNode.h"
#include "NiPointLight.h"
#include "NiScreenPolygon.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00581dd0 _$E502
// FUNCTION: STBC 0x00581df0 _$E501

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0057e930 _$E62
// FUNCTION: STBC 0x0057e940 _$E61
// FUNCTION: STBC 0x0057e950 _$E297
// FUNCTION: STBC 0x0057e960 _$E296
// FUNCTION: STBC 0x0057e970 _$E73
// FUNCTION: STBC 0x0057e980 _$E72
// FUNCTION: STBC 0x0057e990 _$E77
// FUNCTION: STBC 0x0057e9a0 _$E76
// FUNCTION: STBC 0x0057e9b0 _$E81
// FUNCTION: STBC 0x0057e9c0 _$E80
// FUNCTION: STBC 0x0057e9d0 _$E85
// FUNCTION: STBC 0x0057e9e0 _$E84
// FUNCTION: STBC 0x0057e9f0 _$E204
// FUNCTION: STBC 0x0057ea00 _$E203
// FUNCTION: STBC 0x0057f270 _$E146
// FUNCTION: STBC 0x0057f280 _$E145
// FUNCTION: STBC 0x0057f560 _$E159
// FUNCTION: STBC 0x0057f570 _$E158
// FUNCTION: STBC 0x0057f7c0 _$E209
// FUNCTION: STBC 0x0057f7d0 _$E208
// FUNCTION: STBC 0x005808f0 _$E302
// FUNCTION: STBC 0x00580900 _$E301
// FUNCTION: STBC 0x00580d50 _$E213
// FUNCTION: STBC 0x00580d60 _$E212
// FUNCTION: STBC 0x00581530 _$E306
// FUNCTION: STBC 0x00581540 _$E305
// FUNCTION: STBC 0x00581690 _$E313
// FUNCTION: STBC 0x005816a0 _$E312
// FUNCTION: STBC 0x005816b0 _$E354
// FUNCTION: STBC 0x005816c0 _$E353
// FUNCTION: STBC 0x005816d0 _$E358
// FUNCTION: STBC 0x005816e0 _$E357
// FUNCTION: STBC 0x005816f0 _$E104
// FUNCTION: STBC 0x00581700 _$E103
// FUNCTION: STBC 0x00581710 _$E221
// FUNCTION: STBC 0x00581720 _$E220
// FUNCTION: STBC 0x00581730 _$E228
// FUNCTION: STBC 0x00581740 _$E227
// FUNCTION: STBC 0x00581750 _$E232
// FUNCTION: STBC 0x00581760 _$E231
// FUNCTION: STBC 0x00581770 _$E236
// FUNCTION: STBC 0x00581780 _$E235
// FUNCTION: STBC 0x00581790 _$E240
// FUNCTION: STBC 0x005817a0 _$E239
// FUNCTION: STBC 0x005817b0 _$E246
// FUNCTION: STBC 0x005817c0 _$E245
// FUNCTION: STBC 0x005817d0 _$E340
// FUNCTION: STBC 0x005817e0 _$E339
// FUNCTION: STBC 0x005817f0 _$E362
// FUNCTION: STBC 0x00581800 _$E361
// FUNCTION: STBC 0x00581810 _$E366
// FUNCTION: STBC 0x00581820 _$E365
// FUNCTION: STBC 0x00581830 _$E370
// FUNCTION: STBC 0x00581840 _$E369
// FUNCTION: STBC 0x00581850 _$E374
// FUNCTION: STBC 0x00581860 _$E373
// FUNCTION: STBC 0x00581870 _$E378
// FUNCTION: STBC 0x00581880 _$E377
// FUNCTION: STBC 0x00581890 _$E250
// FUNCTION: STBC 0x005818a0 _$E249
// FUNCTION: STBC 0x005818b0 _$E382
// FUNCTION: STBC 0x005818c0 _$E381
// FUNCTION: STBC 0x005818d0 _$E386
// FUNCTION: STBC 0x005818e0 _$E385
// FUNCTION: STBC 0x005818f0 _$E390
// FUNCTION: STBC 0x00581900 _$E389
// FUNCTION: STBC 0x00581910 _$E109
// FUNCTION: STBC 0x00581920 _$E108
// FUNCTION: STBC 0x00581930 _$E394
// FUNCTION: STBC 0x00581940 _$E393
// FUNCTION: STBC 0x00581950 _$E114
// FUNCTION: STBC 0x00581960 _$E113
// FUNCTION: STBC 0x00581970 _$E398
// FUNCTION: STBC 0x00581980 _$E397
// FUNCTION: STBC 0x00581990 _$E402
// FUNCTION: STBC 0x005819a0 _$E401
// FUNCTION: STBC 0x005819b0 _$E406
// FUNCTION: STBC 0x005819c0 _$E405
// FUNCTION: STBC 0x005819d0 _$E410
// FUNCTION: STBC 0x005819e0 _$E409
// FUNCTION: STBC 0x005819f0 _$E123
// FUNCTION: STBC 0x00581a00 _$E122
// FUNCTION: STBC 0x00581a10 _$E414
// FUNCTION: STBC 0x00581a20 _$E413
// FUNCTION: STBC 0x00581a30 _$E418
// FUNCTION: STBC 0x00581a40 _$E417
// FUNCTION: STBC 0x00581a50 _$E422
// FUNCTION: STBC 0x00581a60 _$E421
// FUNCTION: STBC 0x00581a70 _$E129
// FUNCTION: STBC 0x00581a80 _$E128
// FUNCTION: STBC 0x00581a90 _$E134
// FUNCTION: STBC 0x00581aa0 _$E133
// FUNCTION: STBC 0x00581ab0 _$E164
// FUNCTION: STBC 0x00581ac0 _$E163
// FUNCTION: STBC 0x00581ad0 _$E344
// FUNCTION: STBC 0x00581ae0 _$E343
// FUNCTION: STBC 0x00581af0 _$E169
// FUNCTION: STBC 0x00581b00 _$E168
// FUNCTION: STBC 0x00581b10 _$E426
// FUNCTION: STBC 0x00581b20 _$E425
// FUNCTION: STBC 0x00581b30 _$E430
// FUNCTION: STBC 0x00581b40 _$E429
// FUNCTION: STBC 0x00581b50 _$E174
// FUNCTION: STBC 0x00581b60 _$E173
// FUNCTION: STBC 0x00581b70 _$E434
// FUNCTION: STBC 0x00581b80 _$E433
// FUNCTION: STBC 0x00581b90 _$E139
// FUNCTION: STBC 0x00581ba0 _$E138
// FUNCTION: STBC 0x00581bb0 _$E438
// FUNCTION: STBC 0x00581bc0 _$E437
// FUNCTION: STBC 0x00581bd0 _$E442
// FUNCTION: STBC 0x00581be0 _$E441
// FUNCTION: STBC 0x00581bf0 _$E179
// FUNCTION: STBC 0x00581c00 _$E178
// FUNCTION: STBC 0x00581c10 _$E184
// FUNCTION: STBC 0x00581c20 _$E183
// FUNCTION: STBC 0x00581c30 _$E446
// FUNCTION: STBC 0x00581c40 _$E445
// FUNCTION: STBC 0x00581c50 _$E450
// FUNCTION: STBC 0x00581c60 _$E449
// FUNCTION: STBC 0x00581c70 _$E454
// FUNCTION: STBC 0x00581c80 _$E453
// FUNCTION: STBC 0x00581c90 _$E458
// FUNCTION: STBC 0x00581ca0 _$E457
// FUNCTION: STBC 0x00581cb0 _$E462
// FUNCTION: STBC 0x00581cc0 _$E461
// FUNCTION: STBC 0x00581cd0 _$E466
// FUNCTION: STBC 0x00581ce0 _$E465
// FUNCTION: STBC 0x00581cf0 _$E470
// FUNCTION: STBC 0x00581d00 _$E469
// FUNCTION: STBC 0x00581d10 _$E474
// FUNCTION: STBC 0x00581d20 _$E473
// FUNCTION: STBC 0x00581d30 _$E189
// FUNCTION: STBC 0x00581d40 _$E188
// FUNCTION: STBC 0x00581d50 _$E478
// FUNCTION: STBC 0x00581d60 _$E477
// FUNCTION: STBC 0x00581d70 _$E194
// FUNCTION: STBC 0x00581d80 _$E193
// FUNCTION: STBC 0x00581d90 _$E199
// FUNCTION: STBC 0x00581da0 _$E198
// FUNCTION: STBC 0x00581e00 _$E482
// FUNCTION: STBC 0x00581e10 _$E481
// FUNCTION: STBC 0x00581e20 _$E487
// FUNCTION: STBC 0x00581e30 _$E486
// FUNCTION: STBC 0x00581e40 _$E329
// FUNCTION: STBC 0x00581e50 _$E328

// Standard streams, handler tables and other file-scope statics.
// FUNCTION: STBC 0x0057e8b0 _$E276
// FUNCTION: STBC 0x0057e8c0 _$E273
// FUNCTION: STBC 0x0057e8d0 _$E275
// FUNCTION: STBC 0x0057e8e0 _$E274
// FUNCTION: STBC 0x0057e8f0 _$E282
// FUNCTION: STBC 0x0057e900 _$E279
// FUNCTION: STBC 0x0057e910 _$E281
// FUNCTION: STBC 0x0057e920 _$E280
// FUNCTION: STBC 0x0057ea10 _$E493
// FUNCTION: STBC 0x0057ea20 _$E489
// FUNCTION: STBC 0x0057ea30 _$E492
// FUNCTION: STBC 0x0057ea40 _$E490

/*
================
TractorBeamProjector::Cast
The object as a TractorBeamProjector; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0057ea60
TractorBeamProjector *TractorBeamProjector::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TRACTORBEAMPROJECTOR_TYPE ) ) {
		return (TractorBeamProjector *)object;
	}
	return NULL;
}

/*
================
TractorBeamProjector::GetProperty
================
*/
// FUNCTION: STBC 0x0057ea90
TractorBeamProperty *TractorBeamProjector::GetProperty( void ) {
	return (TractorBeamProperty *)property;
}

/*
================
TractorBeamProjector::GetOrientationForward
================
*/
// FUNCTION: STBC 0x0057eaa0
TGPoint3 TractorBeamProjector::GetOrientationForward( void ) {
	return GetProperty()->GetOrientationForward();
}

/*
================
TractorBeamProjector::GetOrientationUp
================
*/
// FUNCTION: STBC 0x0057eb00
TGPoint3 TractorBeamProjector::GetOrientationUp( void ) {
	return GetProperty()->GetOrientationUp();
}

/*
================
TractorBeamProjector::GetOrientation
================
*/
// FUNCTION: STBC 0x0057eb60
void TractorBeamProjector::GetOrientation( TGPoint3 *forward, TGPoint3 *up ) {
	*forward = GetProperty()->GetOrientationForward();
	*up = GetProperty()->GetOrientationUp();
}

/*
================
TractorBeamProjector::GetArcWidthAngles
================
*/
// FUNCTION: STBC 0x0057ebc0
void TractorBeamProjector::GetArcWidthAngles( float *min, float *max ) {
	*min = GetProperty()->GetArcWidthAngleMin();
	*max = GetProperty()->GetArcWidthAngleMax();
}

/*
================
TractorBeamProjector::GetArcHeightAngles
================
*/
// FUNCTION: STBC 0x0057ebf0
void TractorBeamProjector::GetArcHeightAngles( float *min, float *max ) {
	*min = GetProperty()->GetArcHeightAngleMin();
	*max = GetProperty()->GetArcHeightAngleMax();
}

/*
================
TractorBeamProjector::GetArcWidthAngleMin
================
*/
// FUNCTION: STBC 0x0057ec20
float TractorBeamProjector::GetArcWidthAngleMin( void ) {
	return GetProperty()->GetArcWidthAngleMin();
}

/*
================
TractorBeamProjector::GetArcWidthAngleMax
================
*/
// FUNCTION: STBC 0x0057ec30
float TractorBeamProjector::GetArcWidthAngleMax( void ) {
	return GetProperty()->GetArcWidthAngleMax();
}

/*
================
TractorBeamProjector::GetArcHeightAngleMin
================
*/
// FUNCTION: STBC 0x0057ec40
float TractorBeamProjector::GetArcHeightAngleMin( void ) {
	return GetProperty()->GetArcHeightAngleMin();
}

/*
================
TractorBeamProjector::GetArcHeightAngleMax
================
*/
// FUNCTION: STBC 0x0057ec50
float TractorBeamProjector::GetArcHeightAngleMax( void ) {
	return GetProperty()->GetArcHeightAngleMax();
}

/*
================
TractorBeamProjector::GetMaxDamageDistance
================
*/
// FUNCTION: STBC 0x0057ec60
float TractorBeamProjector::GetMaxDamageDistance( void ) {
	return GetProperty()->GetMaxDamageDistance();
}

/*
================
TractorBeamProjector::TractorBeamProjector
================
*/
// FUNCTION: STBC 0x0057ec70
TractorBeamProjector::TractorBeamProjector( long id ) : EnergyWeapon( id ) {
	unknownFC = true;
}

/*
================
TractorBeamProjector::GetObjType
================
*/
// FUNCTION: STBC 0x0057ecd0
int TractorBeamProjector::GetObjType( void ) {
	return TRACTORBEAMPROJECTOR_TYPE;
}

// DATA: STBC 0x0098cf58
TGEventHandlerTable TractorBeamProjector::classHandlers;

/*
================
TractorBeamProjector::GetClassName
================
*/
// FUNCTION: STBC 0x0057ed40
char *TractorBeamProjector::GetClassName( void ) {
	return "TractorBeamProjector";
}

/*
================
TractorBeamProjector::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x0057ed50
char *TractorBeamProjector::GetSwigTypeName( void ) {
	return "_p_TractorBeamProjector";
}

/*
================
TractorBeamProjector::GetPythonClassName
================
*/
// FUNCTION: STBC 0x0057ed60
char *TractorBeamProjector::GetPythonClassName( void ) {
	return "TractorBeamProjectorPtr";
}

/*
================
TractorBeamProjector::GetClassHandlers
================
*/
// FUNCTION: STBC 0x0057ed70
TGEventHandlerTable *TractorBeamProjector::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
TractorBeamProjector::~TractorBeamProjector
================
*/
// FUNCTION: STBC 0x0057edb0
// FUNCTION: STBC 0x0057ed80 ??_GTractorBeamProjector@@UAEPAXI@Z
TractorBeamProjector::~TractorBeamProjector( void ) {
}

/*
================
TractorBeamProjector::SetProperty
The payload takes the weapon's damage radius factor.
================
*/
// FUNCTION: STBC 0x0057ee10
void TractorBeamProjector::SetProperty( SubsystemProperty *property_ ) {
	EnergyWeapon::SetProperty( property_ );
	payload.SetDamageRadiusFactor( GetDamageRadiusFactor() );
}

/*
================
TractorBeamProjector::Fire

Holds the target the weapon system aims at: the projector stops when the
ship is in no set, when there is no target position, when the ship's
sensors don't see the target, when the target is out of reach or out of the
arcs, or when another object is nearer along the beam. Returns whether the
projector is firing.
================
*/
// FUNCTION: STBC 0x0057ee30
// PARKED: inline budget and tail merge: return IsFiring() puts the first LocalToWorldPoint's operator+ out of line as retail; both constructors of the second need one more inline site after it (share 477/2 -> 477/3); retail emits each early return with its own epilogue and sends the reach and hit failures back to the IsVisibleTarget failure block, where this build shares one trailing StopFiring block
bool TractorBeamProjector::Fire( float time, bool playSound ) {
	ShipClass *ship = parentSubsystem->GetParentShip();
	if ( ship->GetContainingSet() == NULL ) {
		return false;
	}

	ObjectClass *targetObject = NULL;
	NiPoint3 targetPosition;
	if ( !( (WeaponSystem *)parentSubsystem )->GetTargetPosition( &target, &targetPosition, &targetObject ) ) {
		StopFiring();
		return false;
	}
	if ( !WeaponSystem::IsVisibleTarget( targetObject ) ) {
		StopFiring();
		return false;
	}

	NiPoint3 localTarget = TGModelUtils::WorldToLocalPoint( ship->GetNode(), targetPosition );
	NiPoint3 position = GetDamagePoint();
	if ( CanReach( localTarget - position ) == true ) {
		NiPoint3 start = TGModelUtils::LocalToWorldPoint( ship->GetNode(), position );
		float distance;
		if ( FindHitObject( &start, &targetPosition, &distance ) == targetObject ) {
			UpdateBeam( TGModelUtils::LocalToWorldPoint( ship->GetNode(), position ), targetObject->GetWorldBound().GetCenter(), time, playSound );
			return IsFiring();
		}
	}
	StopFiring();
	return false;
}

/*
================
TractorBeamProjector::StopFiring

Lets go of the target and ends the beam: the graphic stops and leaves the
scene, the sound stops, and the ship, the target and the tractored object are
told.
================
*/
// FUNCTION: STBC 0x0057f080 ?StopFiring@TractorBeamProjector@@UAEXXZ
void TractorBeamProjector::StopFiring( void ) {
	if ( firing ) {
		ShipClass *ship = parentShip;
		int mode = TractorBeamSystem::Cast( parentSubsystem )->GetMode();
		firing = false;

		DamageableObject *targetObject = DamageableObject::Cast( GetTGObjectPtr( GetTargetID() ) );
		if ( targetObject != NULL && ship != NULL && unknownFC && ( mode == 4 || mode == 5 ) ) {
			ship->EnableCollisionsWith( targetObject, true );
		}

		if ( payload.graphic != NULL ) {
			payload.graphic->Stop();
			NiNode *parent = payload.graphic->GetParent();
			if ( parent != NULL ) {
				parent->DetachChild( payload.graphic );
			}
		}

		if ( unknownA4 != unknown896A9C ) {
			g_kSoundManager.StopSound( unknownA4 );
			unknownA4 = unknown896A9C;
		}

		TGObjPtrEvent *event = new TGObjPtrEvent;
		event->SetEventType( ET_TRACTOR_BEAM_STOPPED_FIRING );
		event->SetSource( this );
		event->SetDestination( ship );
		event->SetObjPtr( GetTGObjectPtr( GetTargetID() ) );
		g_kEventManager.AddEvent( event );

		if ( payload.targetID != 0 ) {
			DamageableObject *held = DamageableObject::GetObjectByID( NULL, payload.targetID );
			if ( held != NULL ) {
				held->RemoveLight( payload.graphic->GetEndLight() );
			}
			TGEvent *stopped = new TGEvent;
			stopped->SetSource( this );
			stopped->SetDestination( held );
			stopped->SetEventType( ET_TRACTOR_BEAM_STOPPED_HITTING );
			g_kEventManager.AddEvent( stopped );
		}
		payload.targetID = 0;
	}
	Weapon::StopFiring();
}

/*
================
TractorBeamProjector::CanFire

Not while the ship is cloaked, nor while the projector is disabled. The
beam draws on the main battery: starting it needs 5% of the battery's
limit, keeping it 1%.
================
*/
// FUNCTION: STBC 0x0057f2b0
// PARKED: register: the ship local gives retail's data flow (no reload after IsCloaked, the power subsystem in the ship's register), but its priority (26) beats this's (21), so ours keeps the ship in esi and this in edi where retail swaps them
bool TractorBeamProjector::CanFire( void ) {
	ShipClass *ship = parentShip;
	if ( ship == NULL || ship->IsCloaked() != true ) {
		// The original reads the power subsystem even without a ship.
		PowerSubsystem *power = ship->GetPowerSubsystem();
		if ( power != NULL && parentSubsystem->GetConditionPercentage() * conditionPercentage >= GetDisabledPercentage() ) {
			if ( firing ) {
				float battery = power->GetMainBatteryPower();
				if ( battery >= power->GetMainBatteryLimit() * TRACTOR_KEEP_FIRING_BATTERY ) {
					return true;
				}
			} else {
				float battery = power->GetMainBatteryPower();
				if ( battery >= power->GetMainBatteryLimit() * TRACTOR_START_FIRING_BATTERY ) {
					return true;
				}
			}
		}
	}
	return false;
}

// The charge that gives the beam its full range, and that range, which is
// also the speed at which the graphic reaches out (invented names).
// DATA: STBC 0x008936e0
static const float FULL_CHARGE = 1.0f;
// DATA: STBC 0x008936cc
static const float FULL_CHARGE_RANGE = 500.0f;

// How far a docking beam draws its target in a second (invented name).
// DATA: STBC 0x008936ec
static const float DOCK_SPEED = 1.0f;

// How far a towing beam draws its target in a second (invented name).
// DATA: STBC 0x008936e8
static const float TOW_SPEED = 3.0f;

// The share of a half turn within which a docking target is lined up with
// the beam: twenty degrees (invented name).
#define DOCK_ANGLE_SHARE		( 1.0f / 9.0f )

// How a beam that moves its target splits its damage: the share that becomes
// the force and the share returned as damage done (invented names).
#define TRACTOR_FORCE_SHARE		0.7f
#define TRACTOR_DAMAGE_SHARE	0.3f

/*
================
TractorBeamProjector::CanReach

The beam reaches as far as the charge allows (FULL_CHARGE_RANGE at full
charge) and no further than the maximum damage distance; then the offset
has to be inside the arcs.
================
*/
// FUNCTION: STBC 0x0057f350
bool TractorBeamProjector::CanReach( NiPoint3 offset ) {
	float distanceSquared = offset.SqrLength();
	float charge = GetChargeLevel();
	float range = charge / Unknown581010() * FULL_CHARGE_RANGE;
	float maxDistance = GetMaxDamageDistance();
	if ( distanceSquared <= range * range && maxDistance * maxDistance >= distanceSquared ) {
		return IsInArc( offset ) ? true : false;
	}
	return false;
}

/*
================
TractorBeamProjector::IsInArc

Whether the offset, in the ship's frame, lies ahead along the projector's
forward axis and within its width and height arcs.
================
*/
// FUNCTION: STBC 0x0057f400
bool TractorBeamProjector::IsInArc( NiPoint3 offset ) {
	float ahead = GetProperty()->GetOrientationForward().Dot( offset );
	if ( ahead > 0.0f ) {
		float side = GetProperty()->GetOrientationRight().Dot( offset );
		float above = GetProperty()->GetOrientationUp().Dot( offset );
		float horizontal = NiFastATan2( side, ahead );
		if ( GetArcWidthAngleMin() <= horizontal && horizontal <= GetArcWidthAngleMax() ) {
			float vertical = NiFastATan2( above, ahead );
			if ( GetArcHeightAngleMin() <= vertical && vertical <= GetArcHeightAngleMax() ) {
				return true;
			}
		}
	}
	return false;
}

// The fire sound's priority, and the player's ship's (invented names).
#define FIRE_SOUND_PRIORITY			0.5f
#define PLAYER_FIRE_SOUND_PRIORITY	0.9f

/*
================
TractorBeamProjector::UpdateBeam

A projector that isn't firing creates its beam, plays the looping fire sound
when asked, and tells the ship it started firing; one that is moves its
beam. A docking beam stops the ship and its target from colliding,
remembering whether they could. Then the beam works on its target and the
graphic follows the beam.
================
*/
// FUNCTION: STBC 0x0057f580
// DRAFT: register allocation only: after the docking test retail loads the time into eax and the game time's application into ecx (ours ecx and edx); calls, branches and constants match
void TractorBeamProjector::UpdateBeam( const NiPoint3 &start, const NiPoint3 &target, float time, bool playSound ) {
	ShipClass *ship = parentShip;
	if ( !firing ) {
		CreateBeam( start, target );
		if ( playSound ) {
			SetClass *set = ship->GetContainingSet();
			if ( set != NULL ) {
				TGSound *sound = g_kSoundManager.GetSound( GetFireSound() );
				if ( sound != NULL ) {
					sound->AttachToNode( parentShip->GetNode() );
					sound->SetLooping( true );
					TGSoundRegion *region = TGSoundRegion::GetRegion( set->GetName() );
					if ( region != NULL ) {
						region->AddSound( sound );
						float priority = FIRE_SOUND_PRIORITY;
						if ( ship == Game::GetCurrentPlayer() ) {
							priority = PLAYER_FIRE_SOUND_PRIORITY;
						}
						sound->SetPriority( priority );
						unknownA4 = sound->Play( true );
					}
				}
			}
		}

		TGObjPtrEvent *started = new TGObjPtrEvent;
		started->SetEventType( ET_TRACTOR_BEAM_STARTED_FIRING );
		started->SetSource( this );
		started->SetDestination( ship );
		started->SetObjPtr( GetTGObjectPtr( GetTargetID() ) );
		g_kEventManager.AddEvent( started );

		TGObjPtrEvent *fired = new TGObjPtrEvent;
		fired->SetSource( this );
		fired->SetDestination( ship );
		fired->SetEventType( ET_WEAPON_FIRED );
		fired->SetObjPtr( GetTGObjectPtr( GetTargetID() ) );
		g_kEventManager.AddEvent( fired );
	} else {
		SetBeamEnds( start, target );
	}

	DamageableObject *targetObject = DamageableObject::Cast( GetTGObjectPtr( GetTargetID() ) );
	int mode = TractorBeamSystem::Cast( parentSubsystem )->GetMode();
	if ( ( mode == TractorBeamSystem::TBS_DOCK_STAGE_1 || mode == TractorBeamSystem::TBS_DOCK_STAGE_2 ) && targetObject != NULL && ship != NULL ) {
		unknownFC = ship->CanCollide( targetObject );
		ship->EnableCollisionsWith( targetObject, false );
	}

	UpdateTarget( time );
	if ( firing == true ) {
		payload.graphic->Update( payload.start, payload.direction, payload.length, UtopiaApp::GetApp()->GetGameTime() );
	}
}

/*
================
TractorBeamProjector::GetBeamOrigin

In the world, the ship's rotation and scale applied to the damage point
moved ten units along the forward axis, and the ship's translation added.
================
*/
// FUNCTION: STBC 0x0057f7e0
// DRAFT: EH frame: the call set is retail's, but the forward's TGPoint3 temporary lives across the transform's calls here, where retail has no EH frame; with the offset in a statement of its own (no frame) the scaled point's constructor needs a share under 47 (one more site after the transform)
NiPoint3 TractorBeamProjector::GetBeamOrigin( void ) {
	ShipClass *ship = parentShip;
	NiPoint3 origin = TGModelUtils::LocalToWorldPoint( ship->GetNode(), GetDamagePoint() + GetProperty()->GetOrientationForward() * 10.0f );
	return origin;
}

/*
================
TractorBeamProjector::UpdateTarget

Finds what the beam hits along its length and shortens the beam to it. A new
target takes the beam's end light and hears ET_TRACTOR_BEAM_STARTED_HITTING,
the old one loses the light and hears ET_TRACTOR_BEAM_STOPPED_HITTING. The
system's mode then works on the target with the damage the beam can do, and
the system adds what the mode used to its load. A beam that hits nothing
lets go of its target.
================
*/
// FUNCTION: STBC 0x0057f8c0
void TractorBeamProjector::UpdateTarget( float time ) {
	TractorBeamSystem *system = (TractorBeamSystem *)parentSubsystem;
	if ( parentShip->GetContainingSet() == NULL ) {
		return;
	}

	float distance = 0.0f;
	NiPoint3 end = payload.length * payload.direction + payload.start;
	DamageableObject *hit = FindHitObject( &payload.start, &end, &distance );
	if ( hit != NULL ) {
		payload.length = distance;
		if ( hit->GetObjID() != payload.targetID ) {
			DamageableObject *old = DamageableObject::GetObjectByID( NULL, payload.targetID );
			if ( old != NULL ) {
				old->RemoveLight( payload.graphic->GetEndLight() );
				TGEvent *stopped = new TGEvent;
				stopped->SetSource( this );
				stopped->SetDestination( old );
				stopped->SetEventType( ET_TRACTOR_BEAM_STOPPED_HITTING );
				g_kEventManager.AddEvent( stopped );
			}
			payload.targetID = hit->GetObjID();
			payload.NextID();
			hit->AddLight( payload.graphic->GetEndLight() );
			TGEvent *started = new TGEvent;
			started->SetSource( this );
			started->SetDestination( hit );
			started->SetEventType( ET_TRACTOR_BEAM_STARTED_HITTING );
			g_kEventManager.AddEvent( started );
		}

		float damage = CalculateDamage( time, distance );
		float unused;
		switch ( system->GetMode() ) {
		case TractorBeamSystem::TBS_HOLD:
			unused = HoldTarget( hit, damage, time );
			break;
		case TractorBeamSystem::TBS_TOW:
		case TractorBeamSystem::TBS_DOCK_STAGE_1:
			unused = TowTarget( hit, damage, time );
			break;
		case TractorBeamSystem::TBS_PULL:
			unused = PullTarget( hit, damage, time );
			break;
		case TractorBeamSystem::TBS_PUSH:
			unused = PushTarget( hit, damage, time );
			break;
		case TractorBeamSystem::TBS_DOCK_STAGE_2:
			unused = DockTarget( hit, damage, time );
			break;
		default:
			unused = damage;
			break;
		}
		system->AddLoad( damage - unused );
	} else if ( payload.targetID != 0 ) {
		DamageableObject *old = DamageableObject::GetObjectByID( NULL, payload.targetID );
		if ( old != NULL ) {
			old->RemoveLight( payload.graphic->GetEndLight() );
		}
		// BUG: the event goes out even when the old target is gone, to no destination.
		TGEvent *stopped = new TGEvent;
		stopped->SetSource( this );
		stopped->SetDestination( old );
		stopped->SetEventType( ET_TRACTOR_BEAM_STOPPED_HITTING );
		g_kEventManager.AddEvent( stopped );
		payload.targetID = 0;
	}
}

/*
================
TractorBeamProjector::FindHitObject

Walks the objects of the set's proximity manager along the segment and
keeps the nearest damageable object, other than the ship, whose model the
segment hits.
================
*/
// FUNCTION: STBC 0x0057fbb0
DamageableObject *TractorBeamProjector::FindHitObject( const NiPoint3 *start, const NiPoint3 *end, float *distance ) {
	SetClass *set = parentShip->GetContainingSet();
	if ( set == NULL ) {
		return NULL;
	}
	ProximityManager *proximity = set->GetProximityManager();
	if ( proximity == NULL ) {
		return NULL;
	}

	ProximityManager::ObjectIterator *iterator = proximity->GetLineIntersectObjects( *start, *end, 0.0f, false );
	DamageableObject *nearest = NULL;
	float nearestSquared = NI_INFINITY;
	for ( ObjectClass *object = proximity->GetNextObject( iterator ); object != NULL; object = proximity->GetNextObject( iterator ) ) {
		if ( object != parentShip && object->IsTypeOf( DAMAGEABLEOBJECT_TYPE ) ) {
			NiPoint3 hitPoint;
			NiPoint3 hitNormal;
			DamageableObject *damageable = (DamageableObject *)object;
			if ( damageable->VirtualSlot80( start, end, &hitPoint, &hitNormal, false ) ) {
				float distanceSquared = ( hitPoint - *start ).SqrLength();
				if ( distanceSquared < nearestSquared ) {
					nearestSquared = distanceSquared;
					nearest = damageable;
				}
			}
		}
	}
	proximity->EndObjectIteration( iterator );

	if ( nearest != NULL ) {
		*distance = sqrtf( nearestSquared );
	}
	return nearest;
}

/*
================
TractorBeamProjector::HoldTarget

Pushes against the target's velocity relative to the ship, with as much of
the damage as stopping it needs, and slows its spin by the same share; the
damage left over is returned. A static target is not moved, and one already
at rest relative to the ship stops spinning and leaves all the damage.
================
*/
// FUNCTION: STBC 0x0057fcd0
float TractorBeamProjector::HoldTarget( PhysicsObjectClass *target, float damage, float time ) {
	if ( target->IsStatic() == true ) {
		return 0.0f;
	}
	NiPoint3 relative = target->GetVelocity() - parentShip->GetVelocity();
	float length = relative.Length();
	float leftover = 0.0f;
	float strength = target->GetMass() * length;
	float needed = strength;
	if ( strength > damage ) {
		strength = damage;
	} else {
		leftover = damage - strength;
	}
	NiPoint3 direction = -relative;
	direction.Unitize();
	if ( needed <= 0.0f ) {
		target->SetAngularVelocity( &NiPoint3::ZERO, target->GetAngularDirectionType() );
		return damage;
	}
	direction *= strength;
	Force force( NiPoint3::ZERO, direction, NiPoint3::ZERO );
	force.MoveTo( target->GetWorldLocation() );
	force.SetTorque( NiPoint3::ZERO );
	target->ApplyForce( &force );
	NiPoint3 spin = target->GetAngularVelocity() * ( 1.0f - strength / needed );
	target->SetAngularVelocity( &spin, target->GetAngularDirectionType() );
	return leftover;
}

/*
================
TractorBeamProjector::TowTarget

The damage first takes away the target's momentum (all of it, or slows
it down and does no more); what is left draws the target towards the
beam's origin at no more than TOW_SPEED a second, as far as the damage
left pays for at the target's mass, and gives it the step as its
velocity. While docking, a target within half a unit of the origin, or
one nearer the ship's damage point than the origin is and within twenty
degrees of the beam, moves the system to the second docking stage
instead. Returns the damage left over.
================
*/
// FUNCTION: STBC 0x0057ff60
// DRAFT: inline budget: retail calls operator*( float ) and operator= out of line for the untouched step, which ours expands; retail keeps the slowing factor on the FPU; frame slots differ
float TractorBeamProjector::TowTarget( PhysicsObjectClass *target, float damage, float time ) {
	if ( target->IsStatic() == true ) {
		return 0.0f;
	}
	NiPoint3 velocity = target->GetVelocity();
	float momentum = velocity.Length() * target->GetMass();
	TractorBeamSystem *system = TractorBeamSystem::Cast( parentSubsystem );
	if ( momentum > damage ) {
		float factor = 1.0f - damage / momentum;
		NiPoint3 slower = velocity * factor;
		target->SetVelocity( &slower );
		int directionType = target->GetAngularDirectionType();
		NiPoint3 angular = target->GetAngularVelocity() * factor;
		target->SetAngularVelocity( &angular, directionType );
		return 0.0f;
	}
	target->SetVelocity( &NiPoint3::ZERO );
	target->SetAngularVelocity( &NiPoint3::ZERO, target->GetAngularDirectionType() );
	damage -= momentum;
	if ( damage <= 0.0f ) {
		return 0.0f;
	}

	NiPoint3 origin = GetBeamOrigin();
	NiPoint3 position = target->GetWorldLocation();
	NiPoint3 offset = position - origin;
	float distance = offset.Length();
	float maxDistance = TOW_SPEED * time;
	if ( distance > maxDistance ) {
		distance = maxDistance;
	}
	offset.Unitize();
	float cost = target->GetMass() * distance;
	if ( system->GetMode() == TractorBeamSystem::TBS_DOCK_STAGE_1 ) {
		if ( distance < 0.5f ) {
			system->SetMode( TractorBeamSystem::TBS_DOCK_STAGE_2 );
			return damage;
		}
		NiPoint3 dockPoint = TGModelUtils::LocalToWorldPoint( parentShip->GetNode(), GetDamagePoint() );
		NiPoint3 toTarget = position - dockPoint;
		NiPoint3 toOrigin = origin - dockPoint;
		float targetSquared = toTarget.SqrLength();
		float originSquared = toOrigin.SqrLength();
		if ( targetSquared <= originSquared ) {
			float angle = NiASin( toTarget.Dot( offset ) / ( sqrtf( targetSquared ) * sqrtf( originSquared ) ) );
			if ( fabs( angle ) < NI_PI * DOCK_ANGLE_SHARE ) {
				system->SetMode( TractorBeamSystem::TBS_DOCK_STAGE_2 );
				return damage;
			}
		}
	}
	NiPoint3 step;
	if ( cost > damage ) {
		step = offset * distance * damage / cost;
		NiPoint3 moved = position - step;
		target->SetTranslate( &moved );
		damage = 0.0f;
	} else {
		step = offset * distance;
		target->SetTranslate( &( position - step ) );
		damage -= cost;
	}
	target->SetVelocity( &-step );
	return damage;
}

/*
================
TractorBeamProjector::PullTarget

Pulls the target towards the projector, with seven tenths of the damage as
the force, through its centre so that it does not turn; the other three
tenths are returned as the damage done. A static target is not moved.
================
*/
// FUNCTION: STBC 0x00580590
float TractorBeamProjector::PullTarget( PhysicsObjectClass *target, float damage, float time ) {
	if ( target->IsStatic() == true ) {
		return 0.0f;
	}
	NiPoint3 direction = GetWorldLocation() - target->GetWorldLocation();
	direction.Unitize();
	float strength = damage * TRACTOR_FORCE_SHARE;
	float damageDone = damage * TRACTOR_DAMAGE_SHARE;
	direction *= strength;
	Force force( NiPoint3::ZERO, direction, NiPoint3::ZERO );
	force.MoveTo( target->GetWorldLocation() );
	force.SetTorque( NiPoint3::ZERO );
	target->ApplyForce( &force );
	return damageDone;
}

/*
================
TractorBeamProjector::PushTarget

Pushes the target away from the projector, as PullTarget pulls it.
================
*/
// FUNCTION: STBC 0x00580740
float TractorBeamProjector::PushTarget( PhysicsObjectClass *target, float damage, float time ) {
	if ( target->IsStatic() == true ) {
		return 0.0f;
	}
	NiPoint3 direction = target->GetWorldLocation() - GetWorldLocation();
	direction.Unitize();
	float strength = damage * TRACTOR_FORCE_SHARE;
	float damageDone = damage * TRACTOR_DAMAGE_SHARE;
	direction *= strength;
	Force force( NiPoint3::ZERO, direction, NiPoint3::ZERO );
	force.MoveTo( target->GetWorldLocation() );
	force.SetTorque( NiPoint3::ZERO );
	target->ApplyForce( &force );
	return damageDone;
}

/*
================
TractorBeamProjector::DockTarget

The last stage of docking: the damage first takes away the target's
momentum (all of it, or slows it down and does no more), then draws the
target towards the ship's docking point at no more than DOCK_SPEED a second,
as far as the damage left pays for at the target's mass. Once the target
gets there, or within half a unit, the beam lets go, the ship hears
ET_TRACTOR_TARGET_DOCKED and the system goes back to towing. Returns the
damage left over.
================
*/
// FUNCTION: STBC 0x00580910
// DRAFT: call set and frame agree; retail keeps the factor on the FPU stack, copies one float slot through the FPU and pushes ebx in the prologue
float TractorBeamProjector::DockTarget( PhysicsObjectClass *target, float damage, float time ) {
	if ( target->IsStatic() == true ) {
		return 0.0f;
	}
	NiPoint3 velocity = target->GetVelocity();
	float momentum = velocity.Length() * target->GetMass();
	if ( momentum > damage ) {
		float factor = 1.0f - damage / momentum;
		NiPoint3 slower = velocity * factor;
		target->SetVelocity( &slower );
		int directionType = target->GetAngularDirectionType();
		NiPoint3 angular = target->GetAngularVelocity() * factor;
		target->SetAngularVelocity( &angular, directionType );
		return 0.0f;
	}
	target->SetVelocity( &NiPoint3::ZERO );
	target->SetAngularVelocity( &NiPoint3::ZERO, target->GetAngularDirectionType() );
	damage -= momentum;
	if ( damage <= 0.0f ) {
		return 0.0f;
	}

	TractorBeamSystem *system = TractorBeamSystem::Cast( parentSubsystem );
	NiPoint3 position = target->GetWorldLocation();
	NiPoint3 offset = position - system->GetDockingPoint();
	bool moving = false;
	bool docked = false;
	float distance = offset.Length();
	if ( distance < 0.5f ) {
		docked = true;
	}
	float maxDistance = DOCK_SPEED * time;
	if ( distance > maxDistance ) {
		distance = maxDistance;
		moving = true;
	}
	offset.Unitize();
	float cost = target->GetMass() * distance;
	if ( cost > damage ) {
		NiPoint3 moved = position - offset * distance * damage / cost;
		target->SetTranslate( &moved );
		damage = 0.0f;
		moving = true;
	} else {
		NiPoint3 moved = position - offset * distance;
		target->SetTranslate( &moved );
		damage -= cost;
	}
	if ( !moving || docked == true ) {
		system->VirtualSlot38( target );
		TGObjPtrEvent *event = new TGObjPtrEvent;
		event->SetSource( this );
		event->SetDestination( GetParentShip() );
		event->SetObjPtr( target );
		event->SetEventType( ET_TRACTOR_TARGET_DOCKED );
		g_kEventManager.AddEvent( event );
		system->SetMode( TractorBeamSystem::TBS_TOW );
	}
	target->SetVelocity( &NiPoint3::ZERO );
	return damage;
}

/*
================
TractorBeamProjector::CreateBeam

Creates the graphic the first time and puts it in the effects of the ship's
set; the beam starts with no target, from start to end, and the graphic
takes a time in proportion to the beam's length to reach out.
================
*/
// FUNCTION: STBC 0x00580d70
void TractorBeamProjector::CreateBeam( const NiPoint3 &start, const NiPoint3 &end ) {
	ShipClass *ship = parentShip;
	if ( payload.graphic == NULL ) {
		payload.graphic = new TractorBeamGraphic( GetProperty(), ship );
	}
	ship->GetContainingSet()->GetEffectRoot()->AttachChild( payload.graphic, true );
	payload.graphic->UpdateProperties();
	payload.targetID = 0;
	firing = true;
	SetBeamEnds( start, end );
	payload.graphic->Start( UtopiaApp::GetApp()->GetGameTime(), payload.length / FULL_CHARGE_RANGE );
}

/*
================
TractorBeamProjector::SetBeamEnds
================
*/
// FUNCTION: STBC 0x00580e90
void TractorBeamProjector::SetBeamEnds( const NiPoint3 &start, const NiPoint3 &end ) {
	payload.start = start;
	payload.direction = end - start;
	payload.length = payload.direction.Length();
	payload.direction *= 1.0f / payload.length;
}

/*
================
TractorBeamProjector::CalculateDamage

The most the projector can do, by its condition and its system's, scaled
down to the distance it reaches and by the condition of the engines the
system drags on, over the time.
================
*/
// FUNCTION: STBC 0x00580f50
float TractorBeamProjector::CalculateDamage( float time, float distance ) {
	float reach = EnergyWeapon::GetMaxDamageDistance() / distance;
	if ( reach > 1.0 ) {
		reach = 1.0f;
	}
	ImpulseEngineSubsystem *engines = ( (TractorBeamSystem *)parentSubsystem )->GetImpulseEngineSubsystem();
	float condition = parentSubsystem->GetConditionPercentage() * conditionPercentage;
	float damage = GetMaxDamage() * condition * reach;
	if ( engines != NULL ) {
		damage = engines->GetCombinedConditionPercentage() * damage;
	}
	damage *= time;
	return damage;
}

/*
================
TractorBeamProjector::Unknown580FC0

The damage per unit of time and of the projector's and its system's
conditions, over the beam's reach when the beam is longer than its
maximum damage distance (close to CalculateDamage run backwards, without
the maximum damage).
================
*/
// FUNCTION: STBC 0x00580fc0
float TractorBeamProjector::Unknown580FC0( float damage, float time ) {
	damage = damage / time / ( parentSubsystem->GetConditionPercentage() * conditionPercentage );
	float reach = EnergyWeapon::GetMaxDamageDistance() / payload.length;
	if ( reach < 1.0 ) {
		return damage / reach;
	}
	return damage;
}

/*
================
TractorBeamProjector::Unknown581010
================
*/
// FUNCTION: STBC 0x00581010
float TractorBeamProjector::Unknown581010( void ) {
	return FULL_CHARGE;
}

/*
================
TractorBeamProjector::Update
A tractor beam projector is always fully charged.
================
*/
// FUNCTION: STBC 0x00581020
void TractorBeamProjector::Update( float deltaTime ) {
	chargeLevel = GetMaxCharge();
	EnergyWeapon::Update( deltaTime );
}

/*
================
TractorBeamProjector::CalculateRoughDirection

The direction the projector points, in the world: the centre of its arc,
turned from forward toward the side by the width angle and then toward up by
the height angle.
================
*/
// FUNCTION: STBC 0x00581040
// DRAFT: inline budget: PulseWeapon's and PhaserBank's spelling gives retail's call set but for the second Unitize, which retail expands with the operator= copy before it (161 units needed where 49 are left: the late-rejected UnitCross charges 224); retail's frame is 0x18 larger
TGPoint3 TractorBeamProjector::CalculateRoughDirection( void ) {
	// BUG: the centre of the arc is the minimum plus half the maximum, not the
	// mean of the two (the same in the height angle below), as in PulseWeapon's.
	float width = GetArcWidthAngleMin() + GetArcWidthAngleMax() * 0.5f;
	NiPoint3 right = GetProperty()->GetOrientationForward().UnitCross( GetProperty()->GetOrientationUp() );
	NiPoint3 direction = cosf( width ) * GetProperty()->GetOrientationForward() + sinf( width ) * right;
	direction.Unitize();

	float height = GetArcHeightAngleMin() + GetArcHeightAngleMax() * 0.5f;
	float heightSin = sinf( height );
	direction = direction * cosf( height ) + GetProperty()->GetOrientationUp() * heightSin;
	direction.Unitize();

	return TGPoint3( GetParentShip()->GetWorldRotation() * TGPoint3( direction ) );
}

/*
================
TractorBeamPayload::TractorBeamPayload
================
*/
// FUNCTION: STBC 0x00581350
TractorBeamPayload::TractorBeamPayload( void ) {
	payloadType = WP_PHASER;
	start = NiPoint3::ZERO;
	direction = NiPoint3::UNIT_Y;
	length = 0.0f;
	graphic = NULL;
	targetID = 0;
}

/*
================
TractorBeamPayload::~TractorBeamPayload
Takes the graphic out of the scene and releases it.
================
*/
// FUNCTION: STBC 0x00581420
// FUNCTION: STBC 0x00581400 ??_GTractorBeamPayload@@UAEPAXI@Z
TractorBeamPayload::~TractorBeamPayload( void ) {
	if ( graphic != NULL ) {
		if ( graphic->GetParent() != NULL ) {
			graphic->GetParent()->DetachChild( graphic );
		}
		graphic = NULL;
	}
}

/*
================
TractorBeamProjector::WriteToStream
================
*/
// FUNCTION: STBC 0x005814f0
bool TractorBeamProjector::WriteToStream( TGStream *stream ) {
	if ( !EnergyWeapon::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteBool( unknownFC );
	if ( !payload.WriteToStream( stream ) ) {
		return false;
	}
	return true;
}

/*
================
TractorBeamProjector::ReadFromStream
================
*/
// FUNCTION: STBC 0x00581550
bool TractorBeamProjector::ReadFromStream( TGStream *stream ) {
	if ( !EnergyWeapon::ReadFromStream( stream ) ) {
		return false;
	}

	unknownFC = stream->ReadBool();
	if ( !payload.ReadFromStream( stream ) ) {
		return false;
	}
	return true;
}

/*
================
TractorBeamPayload::WriteToStream
================
*/
// FUNCTION: STBC 0x00581590
bool TractorBeamPayload::WriteToStream( TGStream *stream ) {
	if ( !WeaponPayload::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteFloat( start.x );
	stream->WriteFloat( start.y );
	stream->WriteFloat( start.z );
	stream->WriteFloat( direction.x );
	stream->WriteFloat( direction.y );
	stream->WriteFloat( direction.z );
	stream->WriteFloat( length );
	stream->WriteID( targetID );
	return true;
}

/*
================
TractorBeamPayload::ReadFromStream
================
*/
// FUNCTION: STBC 0x00581610
bool TractorBeamPayload::ReadFromStream( TGStream *stream ) {
	if ( !WeaponPayload::ReadFromStream( stream ) ) {
		return false;
	}

	start.x = stream->ReadFloat();
	start.y = stream->ReadFloat();
	start.z = stream->ReadFloat();
	direction.x = stream->ReadFloat();
	direction.y = stream->ReadFloat();
	direction.z = stream->ReadFloat();
	length = stream->ReadFloat();
	targetID = stream->ReadID();
	return true;
}
