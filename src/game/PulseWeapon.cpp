/*
===============================================================================

	PulseWeapon.cpp

===============================================================================
*/

#include <math.h>

#include "PulseWeapon.h"
#include "MultiplayerGame.h"
#include "TGBufferStream.h"
#include "TGMessage.h"
#include "TGNetwork.h"
#include "PulseWeaponProperty.h"
#include "PoweredSubsystem.h"
#include "PyEmbed.h"
#include "ShipClass.h"
#include "TGEventManager.h"
#include "TGObjPtrEvent.h"
#include "Torpedo.h"
#include "TGEventHandlerTable.h"
#include "TGStream.h"
#include <iostream>
#include "UtopiaModule.h"
#include "WeaponSystem.h"
#include "Game.h"
#include "SetClass.h"
#include "TGSound.h"
#include "TGSoundManager.h"
#include "TGSoundRegion.h"
#include "NiMath.h"
#include "TGMatrix3.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "TGModelUtils.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00577220 _$E526
// FUNCTION: STBC 0x00577240 _$E525

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00574dd0 _$E101
// FUNCTION: STBC 0x00574de0 _$E100
// FUNCTION: STBC 0x00574df0 _$E317
// FUNCTION: STBC 0x00574e00 _$E316
// FUNCTION: STBC 0x00574e10 _$E112
// FUNCTION: STBC 0x00574e20 _$E111
// FUNCTION: STBC 0x00574e30 _$E116
// FUNCTION: STBC 0x00574e40 _$E115
// FUNCTION: STBC 0x00574e50 _$E120
// FUNCTION: STBC 0x00574e60 _$E119
// FUNCTION: STBC 0x00574e70 _$E124
// FUNCTION: STBC 0x00574e80 _$E123
// FUNCTION: STBC 0x00574e90 _$E241
// FUNCTION: STBC 0x00574ea0 _$E240
// FUNCTION: STBC 0x00576a40 _$E183
// FUNCTION: STBC 0x00576a50 _$E182
// FUNCTION: STBC 0x00576a60 _$E196
// FUNCTION: STBC 0x00576a70 _$E195
// FUNCTION: STBC 0x00576a80 _$E246
// FUNCTION: STBC 0x00576a90 _$E245
// FUNCTION: STBC 0x00576aa0 _$E322
// FUNCTION: STBC 0x00576ab0 _$E321
// FUNCTION: STBC 0x00576ac0 _$E250
// FUNCTION: STBC 0x00576ad0 _$E249
// FUNCTION: STBC 0x00576ae0 _$E326
// FUNCTION: STBC 0x00576af0 _$E325
// FUNCTION: STBC 0x00576b00 _$E333
// FUNCTION: STBC 0x00576b10 _$E332
// FUNCTION: STBC 0x00576b20 _$E366
// FUNCTION: STBC 0x00576b30 _$E365
// FUNCTION: STBC 0x00576b40 _$E370
// FUNCTION: STBC 0x00576b50 _$E369
// FUNCTION: STBC 0x00576b60 _$E141
// FUNCTION: STBC 0x00576b70 _$E140
// FUNCTION: STBC 0x00576b80 _$E266
// FUNCTION: STBC 0x00576b90 _$E265
// FUNCTION: STBC 0x00576ba0 _$E273
// FUNCTION: STBC 0x00576bb0 _$E272
// FUNCTION: STBC 0x00576bc0 _$E277
// FUNCTION: STBC 0x00576bd0 _$E276
// FUNCTION: STBC 0x00576be0 _$E281
// FUNCTION: STBC 0x00576bf0 _$E280
// FUNCTION: STBC 0x00576c00 _$E285
// FUNCTION: STBC 0x00576c10 _$E284
// FUNCTION: STBC 0x00576c20 _$E291
// FUNCTION: STBC 0x00576c30 _$E290
// FUNCTION: STBC 0x00576c40 _$E374
// FUNCTION: STBC 0x00576c50 _$E373
// FUNCTION: STBC 0x00576c60 _$E378
// FUNCTION: STBC 0x00576c70 _$E377
// FUNCTION: STBC 0x00576c80 _$E382
// FUNCTION: STBC 0x00576c90 _$E381
// FUNCTION: STBC 0x00576ca0 _$E386
// FUNCTION: STBC 0x00576cb0 _$E385
// FUNCTION: STBC 0x00576cc0 _$E390
// FUNCTION: STBC 0x00576cd0 _$E389
// FUNCTION: STBC 0x00576ce0 _$E394
// FUNCTION: STBC 0x00576cf0 _$E393
// FUNCTION: STBC 0x00576d00 _$E398
// FUNCTION: STBC 0x00576d10 _$E397
// FUNCTION: STBC 0x00576d20 _$E402
// FUNCTION: STBC 0x00576d30 _$E401
// FUNCTION: STBC 0x00576d40 _$E406
// FUNCTION: STBC 0x00576d50 _$E405
// FUNCTION: STBC 0x00576d60 _$E410
// FUNCTION: STBC 0x00576d70 _$E409
// FUNCTION: STBC 0x00576d80 _$E146
// FUNCTION: STBC 0x00576d90 _$E145
// FUNCTION: STBC 0x00576da0 _$E414
// FUNCTION: STBC 0x00576db0 _$E413
// FUNCTION: STBC 0x00576dc0 _$E151
// FUNCTION: STBC 0x00576dd0 _$E150
// FUNCTION: STBC 0x00576de0 _$E418
// FUNCTION: STBC 0x00576df0 _$E417
// FUNCTION: STBC 0x00576e00 _$E422
// FUNCTION: STBC 0x00576e10 _$E421
// FUNCTION: STBC 0x00576e20 _$E426
// FUNCTION: STBC 0x00576e30 _$E425
// FUNCTION: STBC 0x00576e40 _$E430
// FUNCTION: STBC 0x00576e50 _$E429
// FUNCTION: STBC 0x00576e60 _$E160
// FUNCTION: STBC 0x00576e70 _$E159
// FUNCTION: STBC 0x00576e80 _$E434
// FUNCTION: STBC 0x00576e90 _$E433
// FUNCTION: STBC 0x00576ea0 _$E438
// FUNCTION: STBC 0x00576eb0 _$E437
// FUNCTION: STBC 0x00576ec0 _$E442
// FUNCTION: STBC 0x00576ed0 _$E441
// FUNCTION: STBC 0x00576ee0 _$E166
// FUNCTION: STBC 0x00576ef0 _$E165
// FUNCTION: STBC 0x00576f00 _$E171
// FUNCTION: STBC 0x00576f10 _$E170
// FUNCTION: STBC 0x00576f20 _$E201
// FUNCTION: STBC 0x00576f30 _$E200
// FUNCTION: STBC 0x00576f40 _$E446
// FUNCTION: STBC 0x00576f50 _$E445
// FUNCTION: STBC 0x00576f60 _$E206
// FUNCTION: STBC 0x00576f70 _$E205
// FUNCTION: STBC 0x00576f80 _$E450
// FUNCTION: STBC 0x00576f90 _$E449
// FUNCTION: STBC 0x00576fa0 _$E454
// FUNCTION: STBC 0x00576fb0 _$E453
// FUNCTION: STBC 0x00576fc0 _$E211
// FUNCTION: STBC 0x00576fd0 _$E210
// FUNCTION: STBC 0x00576fe0 _$E458
// FUNCTION: STBC 0x00576ff0 _$E457
// FUNCTION: STBC 0x00577000 _$E176
// FUNCTION: STBC 0x00577010 _$E175
// FUNCTION: STBC 0x00577020 _$E462
// FUNCTION: STBC 0x00577030 _$E461
// FUNCTION: STBC 0x00577040 _$E466
// FUNCTION: STBC 0x00577050 _$E465
// FUNCTION: STBC 0x00577060 _$E216
// FUNCTION: STBC 0x00577070 _$E215
// FUNCTION: STBC 0x00577080 _$E221
// FUNCTION: STBC 0x00577090 _$E220
// FUNCTION: STBC 0x005770a0 _$E470
// FUNCTION: STBC 0x005770b0 _$E469
// FUNCTION: STBC 0x005770c0 _$E474
// FUNCTION: STBC 0x005770d0 _$E473
// FUNCTION: STBC 0x005770e0 _$E478
// FUNCTION: STBC 0x005770f0 _$E477
// FUNCTION: STBC 0x00577100 _$E482
// FUNCTION: STBC 0x00577110 _$E481
// FUNCTION: STBC 0x00577120 _$E486
// FUNCTION: STBC 0x00577130 _$E485
// FUNCTION: STBC 0x00577140 _$E490
// FUNCTION: STBC 0x00577150 _$E489
// FUNCTION: STBC 0x00577160 _$E494
// FUNCTION: STBC 0x00577170 _$E493
// FUNCTION: STBC 0x00577180 _$E498
// FUNCTION: STBC 0x00577190 _$E497
// FUNCTION: STBC 0x005771a0 _$E226
// FUNCTION: STBC 0x005771b0 _$E225
// FUNCTION: STBC 0x005771c0 _$E502
// FUNCTION: STBC 0x005771d0 _$E501
// FUNCTION: STBC 0x005771e0 _$E231
// FUNCTION: STBC 0x005771f0 _$E230
// FUNCTION: STBC 0x00577200 _$E236
// FUNCTION: STBC 0x00577210 _$E235
// FUNCTION: STBC 0x00577250 _$E506
// FUNCTION: STBC 0x00577260 _$E505
// FUNCTION: STBC 0x00577270 _$E511
// FUNCTION: STBC 0x00577280 _$E510
// FUNCTION: STBC 0x00577290 _$E352
// FUNCTION: STBC 0x005772a0 _$E351

// Standard streams, handler tables and other file-scope statics.
// FUNCTION: STBC 0x00574d50 _$E305
// FUNCTION: STBC 0x00574d60 _$E302
// FUNCTION: STBC 0x00574d70 _$E304
// FUNCTION: STBC 0x00574d80 _$E303
// FUNCTION: STBC 0x00574d90 _$E311
// FUNCTION: STBC 0x00574da0 _$E308
// FUNCTION: STBC 0x00574db0 _$E310
// FUNCTION: STBC 0x00574dc0 _$E309
// FUNCTION: STBC 0x00574eb0 _$E517
// FUNCTION: STBC 0x00574ec0 _$E513
// FUNCTION: STBC 0x00574ed0 _$E516
// FUNCTION: STBC 0x00574ee0 _$E514

// How urgently a launch sound plays.
#define PULSE_LAUNCH_PRIORITY			0.5f
#define PLAYER_PULSE_LAUNCH_PRIORITY	0.9f

// What a pulse with guidance gets: a sharper turn and a longer guided life.
#define PULSE_TURN_SCALE				1.5f
#define PULSE_GUIDANCE_SCALE			1.25f

/*
================
PulseWeapon::Cast

The object as a PulseWeapon; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00574f00
PulseWeapon *PulseWeapon::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( PULSEWEAPON_TYPE ) ) {
		return (PulseWeapon *)object;
	}
	return NULL;
}

/*
================
PulseWeapon::GetProperty
================
*/
// FUNCTION: STBC 0x00574f30
PulseWeaponProperty *PulseWeapon::GetProperty( void ) {
	return (PulseWeaponProperty *)property;
}

/*
================
PulseWeapon::GetArcHeightAngleMin
================
*/
// FUNCTION: STBC 0x00574f40
float PulseWeapon::GetArcHeightAngleMin( void ) {
	return GetProperty()->GetArcHeightAngleMin();
}

/*
================
PulseWeapon::GetArcHeightAngleMax
================
*/
// FUNCTION: STBC 0x00574f50
float PulseWeapon::GetArcHeightAngleMax( void ) {
	return GetProperty()->GetArcHeightAngleMax();
}

/*
================
PulseWeapon::GetArcWidthAngleMin
================
*/
// FUNCTION: STBC 0x00574f60
float PulseWeapon::GetArcWidthAngleMin( void ) {
	return GetProperty()->GetArcWidthAngleMin();
}

/*
================
PulseWeapon::GetArcWidthAngleMax
================
*/
// FUNCTION: STBC 0x00574f70
float PulseWeapon::GetArcWidthAngleMax( void ) {
	return GetProperty()->GetArcWidthAngleMax();
}

/*
================
PulseWeapon::GetArcHeightAngles
================
*/
// FUNCTION: STBC 0x00574f80
void PulseWeapon::GetArcHeightAngles( float *min, float *max ) {
	*min = GetArcHeightAngleMin();
	*max = GetArcHeightAngleMax();
}

/*
================
PulseWeapon::GetArcWidthAngles
================
*/
// FUNCTION: STBC 0x00574fa0
void PulseWeapon::GetArcWidthAngles( float *min, float *max ) {
	*min = GetArcWidthAngleMin();
	*max = GetArcWidthAngleMax();
}

/*
================
PulseWeapon::GetCooldownTime
================
*/
// FUNCTION: STBC 0x00574fc0
float PulseWeapon::GetCooldownTime( void ) {
	return GetProperty()->GetCooldownTime();
}

/*
================
PulseWeapon::PulseWeapon

Ready to fire at once.
================
*/
// FUNCTION: STBC 0x00574fd0
PulseWeapon::PulseWeapon( long id ) : EnergyWeapon( id ) {
	lastFireTime = -100000.0f;
	launchSpeed = 1.0f;
	launchSound = NULL;
}

/*
================
PulseWeapon::GetObjType
================
*/
// FUNCTION: STBC 0x00575010
int PulseWeapon::GetObjType( void ) {
	return PULSEWEAPON_TYPE;
}

// DATA: STBC 0x0098c918
TGEventHandlerTable PulseWeapon::classHandlers;

/*
================
PulseWeapon::GetClassName
================
*/
// FUNCTION: STBC 0x00575080
char *PulseWeapon::GetClassName( void ) {
	return "PulseWeapon";
}

/*
================
PulseWeapon::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00575090
char *PulseWeapon::GetSwigTypeName( void ) {
	return "_p_PulseWeapon";
}

/*
================
PulseWeapon::GetPythonClassName
================
*/
// FUNCTION: STBC 0x005750a0
char *PulseWeapon::GetPythonClassName( void ) {
	return "PulseWeaponPtr";
}

/*
================
PulseWeapon::GetClassHandlers
================
*/
// FUNCTION: STBC 0x005750b0
TGEventHandlerTable *PulseWeapon::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
PulseWeapon::CalculateWeaponAppeal

Full appeal while the weapon can fire, none otherwise.
================
*/
// FUNCTION: STBC 0x005750c0
float PulseWeapon::CalculateWeaponAppeal( void ) {
	return CanFire();
}

/*
================
PulseWeapon::~PulseWeapon

The launch sound's name is not freed.
================
*/
// FUNCTION: STBC 0x00575110
// FUNCTION: STBC 0x005750e0 ??_GPulseWeapon@@UAEPAXI@Z
PulseWeapon::~PulseWeapon( void ) {
}

/*
================
PulseWeapon::Update

The weapon recharges at its system's normal power while it is above its
disabled condition.
================
*/
// FUNCTION: STBC 0x00575120
void PulseWeapon::Update( float deltaTime ) {
	PoweredSubsystem *system = (PoweredSubsystem *)parentSubsystem;
	float condition = system->GetConditionPercentage();
	if ( condition > system->GetDisabledPercentage() ) {
		UpdateCharge( deltaTime, system->GetNormalPowerPercentage() );
	}
	EnergyWeapon::Update( deltaTime );
}

/*
================
PulseWeapon::VirtualSlot37
================
*/
// FUNCTION: STBC 0x00575170
float PulseWeapon::VirtualSlot37( void ) {
	return 1.0f;
}

/*
================
PulseWeapon::CanFire

Not while the ship is cloaked. A weapon that is not firing needs the
minimum firing charge, and every shot needs the charge of its power
setting; the cooldown must have passed since the last shot, and the
weapon must not be disabled.
================
*/
// FUNCTION: STBC 0x00575180
// PARKED: one register: retail loads the parent subsystem into eax for the disabled test, ours into edx
bool PulseWeapon::CanFire( void ) {
	if ( parentShip != NULL && parentShip->IsCloaked() == true ) {
		return false;
	}

	float now = UtopiaApp::GetApp()->GetGameTime();
	switch ( firing ) {
	case false:
		if ( GetMinFiringCharge() > chargeLevel ) {
			return false;
		}
	case true:
		if ( GetPowerScaled() > chargeLevel ) {
			return false;
		}
	default:
		if ( GetCooldownTime() + lastFireTime > now ) {
			return false;
		}
	}

	if ( GetOverallConditionPercentage() < GetDisabledPercentage() ) {
		return false;
	}
	return true;
}

/*
================
PulseWeapon::GetPowerScaled

The normal discharge rate, scaled by the power setting.
================
*/
// FUNCTION: STBC 0x00575230
// PARKED: retail calls GetNormalDischargeRate and returns for power setting 1; ours tail-jumps to it
float PulseWeapon::GetPowerScaled( void ) {
	switch ( powerSetting ) {
	case 0:
		return GetNormalDischargeRate() * 0.5f;
	case 1:
		return GetNormalDischargeRate();
	case 2:
		return GetNormalDischargeRate() * 2.0f;
	default:
		return 0.0f;
	}
}

/*
================
PulseWeapon::GetDamageScale

The damage a pulse does at the power setting.
================
*/
// FUNCTION: STBC 0x00575270
float PulseWeapon::GetDamageScale( void ) {
	switch ( powerSetting ) {
	case 0:
		return 0.2f;
	case 1:
		return 0.5f;
	case 2:
		return 1.0f;
	}
	return 0.0f;
}

/*
================
PulseWeapon::Fire

Launches a pulse at the weapon's target: the weapon system finds where the
target is, and the weapon stops firing when it can't. The pulse needs a
direction within the weapon's arcs; it costs the charge of the power
setting, and the weapon's and the ship's handlers hear of the shot. In
multiplayer the shot is sent to the other players.
================
*/
// FUNCTION: STBC 0x005752a0
bool PulseWeapon::Fire( float, bool playSound ) {
	if ( !CanFire() ) {
		return false;
	}

	ShipClass *ship = parentSubsystem->GetParentShip();
	ObjectClass *targetObject = NULL;
	NiPoint3 targetPosition;
	if ( !( (WeaponSystem *)parentSubsystem )->GetTargetPosition( &target, &targetPosition, &targetObject ) ) {
		StopFiring();
		return false;
	}

	// The target in the ship's frame. Nothing uses it.
	NiNode *node = ship->GetNode();
	NiPoint3 localPosition = TGModelUtils::WorldToLocalPoint( node, targetPosition );

	NiPoint3 direction;
	if ( !CanHitTarget( &targetPosition, &direction ) ) {
		return false;
	}

	Torpedo *pulse = CreatePulse( &direction, playSound );
	pulse->SetTarget( target.targetID );
	pulse->SetTargetOffset( target.offset );
	chargeLevel -= GetPowerScaled();
	lastFireTime = g_kUtopiaModule.GetGameTime();
	firing = true;

	TGObjPtrEvent *event = new TGObjPtrEvent;
	event->SetSource( this );
	event->SetDestination( parentShip );
	event->SetEventType( ET_WEAPON_FIRED );
	event->SetObjPtr( targetObject );
	g_kEventManager.AddEvent( event );

	if ( g_kUtopiaModule.IsMultiplayer() ) {
		SendCreatePulseMessage( pulse, playSound, &direction );
	}
	return true;
}

/*
================
PulseWeapon::SendCreatePulseMessage

Sends the other players the pulse just fired: the weapon, the firing
player, the direction and the flags, then the target when there is one.
The host sends it to every other player, a client to the host.
================
*/
// FUNCTION: STBC 0x00575480
void PulseWeapon::SendCreatePulseMessage( Torpedo *pulse, bool playSound, NiPoint3 *direction ) {
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network == NULL ) {
		return;
	}

	TGBufferStream stream;
	char buffer[1024];
	stream.VirtualSlot60( buffer, sizeof( buffer ) );
	stream.WriteChar( CREATE_PULSE_MESSAGE );
	stream.WriteID( GetObjID() );
	stream.WriteChar( pulse->GetPlayerID() );
	stream.VirtualSlot34( direction->x, direction->y, direction->z );

	char flags = 0;
	if ( playSound ) {
		flags |= CREATE_PULSE_PLAY_SOUND;
	}
	ShipClass *targetShip = ShipClass::GetObjectByID( NULL, GetTargetID() );
	if ( targetShip != NULL ) {
		flags |= CREATE_PULSE_HAS_TARGET;
	}
	stream.WriteChar( flags );
	if ( targetShip != NULL ) {
		stream.WriteID( GetTargetID() );
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
PulseWeapon::CanHitTarget

Whether a pulse fired now from the weapon could hit the target at the point,
and the direction to fire it in. The pulse closes on the target at the ship's
speed less the target's, both along the line to the point, plus the launch
speed; it has to arrive within the pulse script's lifetime (8 seconds unless
GetLifetime says otherwise). The target's predicted position, seen from the
moving ship, has to be within 30 seconds' closing distance and in the
weapon's arc.
================
*/
// FUNCTION: STBC 0x00575610
// DRAFT: retail calls GetWorldRotation before GetWorldLocation for the launch position yet adds them in this order; retail negates the target's closing speed (fchs) before adding the ship's; the frame is 0x18 bytes larger (retail shares the ship velocity's slot with the offset and copies PredictPosition's result)
bool PulseWeapon::CanHitTarget( NiPoint3 *targetPosition, NiPoint3 *direction ) {
	ShipClass *ship = parentSubsystem->GetParentShip();
	PhysicsObjectClass *targetObject = PhysicsObjectClass::GetObjectByID( ship->GetContainingSet(), GetTargetID() );
	if ( targetObject != NULL ) {
		NiPoint3 position = ship->GetWorldRotation() * TGPoint3( GetDamagePoint() ) + ship->GetWorldLocation();
		NiPoint3 target = *targetPosition;
		NiPoint3 toTarget = target - position;
		toTarget.Unitize();

		NiPoint3 shipVelocity = ship->GetVelocity();
		NiPoint3 targetVelocity = targetObject->GetVelocity();
		NiPoint3 targetAcceleration = targetObject->GetAcceleration();
		float speed = -targetVelocity.Dot( toTarget ) + shipVelocity.Dot( toTarget ) + launchSpeed;
		if ( speed > 0.0f ) {
			NiPoint3 distance = target - position;
			float time = distance.Length() / speed;
			if ( time > 0.0f ) {
				float lifetime = 8.0f;
				if ( PyEmbed::CallFunction( GetProperty()->GetModuleName(), "GetLifetime", "f", &lifetime, "()" ) == -1 ) {
					PyErr_Clear();
				}
				if ( lifetime >= time ) {
					NiPoint3 predicted;
					predicted = targetObject->PredictPosition( target, targetVelocity, targetAcceleration, time );
					predicted -= time * ship->GetVelocity();
					speed *= 30.0f;
					NiPoint3 offset = predicted - position;
					if ( speed * speed >= offset.SqrLength() && IsInArc( predicted ) == true ) {
						*direction = predicted - position;
						direction->Unitize();
						return true;
					}
				}
			}
		}
	}
	return false;
}

/*
================
PulseWeapon::IsInArc

Whether the point is in front of the weapon and within its width and height
arcs, measured about the weapon's forward direction.
================
*/
// FUNCTION: STBC 0x00575a60
// PARKED: calls agree; retail copies each rotated direction into a second local and keeps the up axis on the FPU for its dot product, and lays the ahead <= 0 return directly after the test
bool PulseWeapon::IsInArc( NiPoint3 position ) {
	ShipClass *ship = parentShip;
	NiMatrix3 rotation( ship->GetWorldRotation() );

	NiPoint3 origin = ship->GetWorldLocation() + rotation * GetDamagePoint();
	NiPoint3 offset = position - origin;

	TGPoint3 forward = rotation * GetProperty()->GetOrientationForward();
	TGPoint3 right = rotation * GetProperty()->GetOrientationRight();
	TGPoint3 up = rotation * GetProperty()->GetOrientationUp();

	float ahead = forward * offset;
	if ( ahead > 0.0f ) {
		float side = right * offset;
		float above = up * offset;
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

/*
================
PulseWeapon::CalculateRoughDirection

The direction the weapon points, in the world: the centre of its arc, turned
from forward by the width angle and then the height angle.
================
*/
// FUNCTION: STBC 0x00575db0
TGPoint3 PulseWeapon::CalculateRoughDirection( void ) {
	// BUG: the centre of the arc is the minimum plus half the maximum, not the
	// mean of the two (the same in the height angle below).
	float width = GetArcWidthAngleMin() + GetArcWidthAngleMax() * 0.5f;
	float widthSin = sinf( width );
	NiPoint3 direction = cosf( width ) * GetProperty()->GetOrientationForward() + widthSin * GetProperty()->GetOrientationRight();
	direction.Unitize();

	float height = GetArcHeightAngleMin() + GetArcHeightAngleMax() * 0.5f;
	float heightSin = sinf( height );
	direction = direction * cosf( height ) + GetProperty()->GetOrientationUp() * heightSin;
	direction.Unitize();

	return TGPoint3( GetParentShip()->GetWorldRotation() * TGPoint3( direction ) );
}

/*
================
PulseWeapon::CreatePulse

Creates a pulse at the weapon, in the ship's set, moving with the ship plus
the launch speed along the direction; NULL when the ship is in no set. The
pulse does the weapon's damage scale of its damage. The launch sound plays
from the ship when asked.
================
*/
// FUNCTION: STBC 0x00576080
// PARKED: stack frame: retail's launch position shares its slot with the TGPoint3 temporary of the damage point (a 0x24 byte frame); this build keeps them apart (0x30), the same wall as TorpedoTube::CreateTorpedo; every instruction otherwise matches
Torpedo *PulseWeapon::CreatePulse( NiPoint3 *direction, bool playSound ) {
	ShipClass *ship = parentShip;
	SetClass *set = ship->GetContainingSet();
	if ( set == NULL ) {
		return NULL;
	}

	NiPoint3 position = ship->GetWorldRotation() * TGPoint3( GetDamagePoint() ) + ship->GetWorldLocation();
	Torpedo *pulse = Torpedo::Create( GetProperty()->GetModuleName(), position, 0 );

	NiPoint3 velocity = *direction * pulse->GetLaunchSpeed() + ship->GetVelocity();
	pulse->SetVelocity( &velocity );

	pulse->SetParent( ship );
	pulse->SetDamage( GetDamageScale() * pulse->GetDamage() );
	pulse->UpdateNodeOnly();
	set->AddObjectToSet( pulse, NULL );

	if ( g_kUtopiaModule.IsMultiplayer() ) {
		if ( ship != NULL ) {
			pulse->SetPlayerID( ship->GetNetPlayerID() );
		} else {
			pulse->SetPlayerID( 0 );
		}
	}

	if ( playSound == true && launchSound != NULL ) {
		TGSound *sound = g_kSoundManager.GetSound( launchSound );
		if ( sound != NULL ) {
			if ( sound->GetParentNode() != ship->GetNode() ) {
				sound->AttachToNode( ship->GetNode() );
			}
			TGSoundRegion *region = TGSoundRegion::GetRegion( set->GetName() );
			if ( region != NULL ) {
				region->AddSound( sound );
			}
			float priority = PULSE_LAUNCH_PRIORITY;
			if ( ship == Game::GetCurrentPlayer() ) {
				priority = PLAYER_PULSE_LAUNCH_PRIORITY;
			}
			sound->SetPriority( priority );
			sound->Play( true );
		}
	}
	return pulse;
}

/*
================
PulseWeapon::CreateNetPulse

The pulse another player fired: made as CreatePulse makes it, for the firing
player and the target the message names. A pulse with guidance gets a longer
life and a sharper turn; one without is set to fly at the target's predicted
position. NULL when the weapon has no parent ship in a set.
================
*/
// FUNCTION: STBC 0x005762b0
// PARKED: inline budget and x87 order: retail calls NiPoint3's copy constructor (0x423480) and constructor (0x414540) out of line for the launch position and the velocity, adds the guidance scale of the lifetime before the turn, and measures the pulse's speed before the target's velocity is read; this build expands them and defers the speed (the wall of CreatePulse)
Torpedo *PulseWeapon::CreateNetPulse( bool playSound, int playerID, long targetID, NiPoint3 *direction ) {
	ShipClass *ship = parentShip;
	if ( ship == NULL ) {
		return NULL;
	}
	SetClass *set = ship->GetContainingSet();
	if ( set == NULL ) {
		return NULL;
	}

	NiPoint3 position = ship->GetWorldRotation() * TGPoint3( GetDamagePoint() ) + ship->GetWorldLocation();
	Torpedo *pulse = Torpedo::Create( GetProperty()->GetModuleName(), position, 0 );

	NiPoint3 velocity = *direction * pulse->GetLaunchSpeed() + ship->GetVelocity();
	pulse->SetVelocity( &velocity );

	pulse->SetParent( ship );
	pulse->SetDamage( GetDamageScale() * pulse->GetDamage() );
	pulse->UpdateNodeOnly();
	set->AddObjectToSet( pulse, NULL );

	if ( g_kUtopiaModule.IsMultiplayer() ) {
		pulse->SetPlayerID( playerID );
	}
	pulse->SetTarget( targetID );

	if ( playSound == true && launchSound != NULL ) {
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

	float maxAngularAccel = pulse->GetMaxAngularAccel();
	float guidanceLifetime = pulse->GetGuidanceLifeTime();
	if ( maxAngularAccel != 0.0f && guidanceLifetime != 0.0f ) {
		pulse->SetMaxAngularAccel( maxAngularAccel * PULSE_TURN_SCALE );
		pulse->SetGuidanceLifetime( guidanceLifetime * PULSE_GUIDANCE_SCALE );
	} else {
		ShipClass *target = ShipClass::GetObjectByID( NULL, targetID );
		if ( target != NULL ) {
			NiPoint3 pulsePosition = pulse->GetTranslate();
			NiPoint3 targetPosition = target->GetWorldLocation();
			NiPoint3 pulseVelocity = pulse->GetVelocity();
			float speed = pulseVelocity.Length();
			NiPoint3 targetVelocity = target->GetVelocity();
			NiPoint3 targetAcceleration = target->GetAcceleration();
			if ( speed > 0.0f ) {
				float time = ( targetPosition - pulsePosition ).Length() / speed;
				if ( time > 0.0f ) {
					NiPoint3 aim = target->PredictPosition( targetPosition, targetVelocity, targetAcceleration, time ) - pulsePosition;
					aim.Unitize();
					NiPoint3 aimedVelocity = aim * speed;
					pulse->SetVelocity( &aimedVelocity );
				}
			}
		}
	}
	return pulse;
}

/*
================
PulseWeapon::FireDumb

Launches a pulse straight along the weapon's orientation, with no target
and no arc test; the shot costs the same, and the same handlers hear of it.
================
*/
// FUNCTION: STBC 0x005767d0
bool PulseWeapon::FireDumb( float, bool playSound ) {
	if ( !CanFire() ) {
		return false;
	}

	ShipClass *ship = parentShip;
	NiPoint3 direction = ship->GetWorldRotation() * GetProperty()->GetOrientationForward();
	Torpedo *pulse = CreatePulse( &direction, playSound );
	chargeLevel -= GetPowerScaled();
	lastFireTime = g_kUtopiaModule.GetGameTime();
	firing = true;

	TGObjPtrEvent *event = new TGObjPtrEvent;
	event->SetSource( this );
	event->SetDestination( parentShip );
	event->SetEventType( ET_WEAPON_FIRED );
	event->SetObjPtr( NULL );
	g_kEventManager.AddEvent( event );

	if ( g_kUtopiaModule.IsMultiplayer() ) {
		SendCreatePulseMessage( pulse, playSound, &direction );
	}
	return true;
}

/*
================
PulseWeapon::SetProperty

The launch speed and sound come from the property's script module.
================
*/
// FUNCTION: STBC 0x00576930
void PulseWeapon::SetProperty( SubsystemProperty *property_ ) {
	EnergyWeapon::SetProperty( property_ );

	PyEmbed::CallFunction( GetProperty()->GetModuleName(), "GetLaunchSpeed", "f", &launchSpeed, "" );
	PyEmbed::CallFunction( GetProperty()->GetModuleName(), "GetLaunchSound", "z", &launchSound, "" );
}

/*
================
PulseWeapon::WriteToStream
================
*/
// FUNCTION: STBC 0x005769a0
bool PulseWeapon::WriteToStream( TGStream *stream ) {
	if ( !EnergyWeapon::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteFloat( lastFireTime );
	stream->WriteFloat( launchSpeed );
	stream->VirtualSlot11( launchSound );
	return true;
}

/*
================
PulseWeapon::ReadFromStream
================
*/
// FUNCTION: STBC 0x005769f0
bool PulseWeapon::ReadFromStream( TGStream *stream ) {
	if ( !EnergyWeapon::ReadFromStream( stream ) ) {
		return false;
	}

	lastFireTime = stream->ReadFloat();
	launchSpeed = stream->ReadFloat();
	launchSound = stream->VirtualSlot10( -1 );
	return true;
}
