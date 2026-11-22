/*
===============================================================================

	ShieldClass.cpp

===============================================================================
*/

#include <iostream>
#include "ShieldClass.h"
#include "CloakingSubsystem.h"
#include "PowerSubsystem.h"
#include "ShipClass.h"
#include "ShieldProperty.h"
#include "TGEventHandlerTable.h"
#include "TGBoolEvent.h"
#include "TGEventManager.h"
#include "TGStream.h"
#include "NiAccumulator.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0056b730 _$E473
// FUNCTION: STBC 0x0056b750 _$E472

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00569f00 _$E32
// FUNCTION: STBC 0x00569f10 _$E29
// FUNCTION: STBC 0x00569f20 _$E31
// FUNCTION: STBC 0x00569f30 _$E30
// FUNCTION: STBC 0x00569f40 _$E38
// FUNCTION: STBC 0x00569f50 _$E35
// FUNCTION: STBC 0x00569f60 _$E37
// FUNCTION: STBC 0x00569f70 _$E36

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00569f80 _$E464
// FUNCTION: STBC 0x00569f90 _$E460
// FUNCTION: STBC 0x00569fa0 _$E463
// FUNCTION: STBC 0x00569fb0 _$E461

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0056a400 _$E79
// FUNCTION: STBC 0x0056a410 _$E78
// FUNCTION: STBC 0x0056a4d0 _$E291
// FUNCTION: STBC 0x0056a4e0 _$E290
// FUNCTION: STBC 0x0056af10 _$E90
// FUNCTION: STBC 0x0056af20 _$E89
// FUNCTION: STBC 0x0056af30 _$E94
// FUNCTION: STBC 0x0056af40 _$E93
// FUNCTION: STBC 0x0056af50 _$E98
// FUNCTION: STBC 0x0056af60 _$E97
// FUNCTION: STBC 0x0056af70 _$E102
// FUNCTION: STBC 0x0056af80 _$E101
// FUNCTION: STBC 0x0056af90 _$E217
// FUNCTION: STBC 0x0056afa0 _$E216
// FUNCTION: STBC 0x0056afb0 _$E159
// FUNCTION: STBC 0x0056afc0 _$E158
// FUNCTION: STBC 0x0056afd0 _$E172
// FUNCTION: STBC 0x0056afe0 _$E171
// FUNCTION: STBC 0x0056aff0 _$E222
// FUNCTION: STBC 0x0056b000 _$E221
// FUNCTION: STBC 0x0056b010 _$E296
// FUNCTION: STBC 0x0056b020 _$E295
// FUNCTION: STBC 0x0056b030 _$E239
// FUNCTION: STBC 0x0056b040 _$E238
// FUNCTION: STBC 0x0056b050 _$E300
// FUNCTION: STBC 0x0056b060 _$E299
// FUNCTION: STBC 0x0056b070 _$E307
// FUNCTION: STBC 0x0056b080 _$E306
// FUNCTION: STBC 0x0056b090 _$E317
// FUNCTION: STBC 0x0056b0a0 _$E316
// FUNCTION: STBC 0x0056b0b0 _$E321
// FUNCTION: STBC 0x0056b0c0 _$E320
// FUNCTION: STBC 0x0056b0d0 _$E121
// FUNCTION: STBC 0x0056b0e0 _$E120
// FUNCTION: STBC 0x0056b0f0 _$E255
// FUNCTION: STBC 0x0056b100 _$E254
// FUNCTION: STBC 0x0056b110 _$E262
// FUNCTION: STBC 0x0056b120 _$E261
// FUNCTION: STBC 0x0056b130 _$E266
// FUNCTION: STBC 0x0056b140 _$E265
// FUNCTION: STBC 0x0056b150 _$E270
// FUNCTION: STBC 0x0056b160 _$E269
// FUNCTION: STBC 0x0056b170 _$E274
// FUNCTION: STBC 0x0056b180 _$E273
// FUNCTION: STBC 0x0056b190 _$E280
// FUNCTION: STBC 0x0056b1a0 _$E279
// FUNCTION: STBC 0x0056b1b0 _$E226
// FUNCTION: STBC 0x0056b1c0 _$E225
// FUNCTION: STBC 0x0056b1d0 _$E230
// FUNCTION: STBC 0x0056b1e0 _$E229
// FUNCTION: STBC 0x0056b1f0 _$E325
// FUNCTION: STBC 0x0056b200 _$E324
// FUNCTION: STBC 0x0056b210 _$E329
// FUNCTION: STBC 0x0056b220 _$E328
// FUNCTION: STBC 0x0056b230 _$E333
// FUNCTION: STBC 0x0056b240 _$E332
// FUNCTION: STBC 0x0056b250 _$E337
// FUNCTION: STBC 0x0056b260 _$E336
// FUNCTION: STBC 0x0056b270 _$E341
// FUNCTION: STBC 0x0056b280 _$E340
// FUNCTION: STBC 0x0056b290 _$E345
// FUNCTION: STBC 0x0056b2a0 _$E344
// FUNCTION: STBC 0x0056b2b0 _$E349
// FUNCTION: STBC 0x0056b2c0 _$E348
// FUNCTION: STBC 0x0056b2d0 _$E353
// FUNCTION: STBC 0x0056b2e0 _$E352
// FUNCTION: STBC 0x0056b2f0 _$E126
// FUNCTION: STBC 0x0056b300 _$E125
// FUNCTION: STBC 0x0056b310 _$E357
// FUNCTION: STBC 0x0056b320 _$E356
// FUNCTION: STBC 0x0056b330 _$E131
// FUNCTION: STBC 0x0056b340 _$E130
// FUNCTION: STBC 0x0056b350 _$E361
// FUNCTION: STBC 0x0056b360 _$E360
// FUNCTION: STBC 0x0056b370 _$E365
// FUNCTION: STBC 0x0056b380 _$E364
// FUNCTION: STBC 0x0056b390 _$E369
// FUNCTION: STBC 0x0056b3a0 _$E368
// FUNCTION: STBC 0x0056b3b0 _$E373
// FUNCTION: STBC 0x0056b3c0 _$E372
// FUNCTION: STBC 0x0056b3d0 _$E136
// FUNCTION: STBC 0x0056b3e0 _$E135
// FUNCTION: STBC 0x0056b3f0 _$E377
// FUNCTION: STBC 0x0056b400 _$E376
// FUNCTION: STBC 0x0056b410 _$E381
// FUNCTION: STBC 0x0056b420 _$E380
// FUNCTION: STBC 0x0056b430 _$E385
// FUNCTION: STBC 0x0056b440 _$E384
// FUNCTION: STBC 0x0056b450 _$E142
// FUNCTION: STBC 0x0056b460 _$E141
// FUNCTION: STBC 0x0056b470 _$E147
// FUNCTION: STBC 0x0056b480 _$E146
// FUNCTION: STBC 0x0056b490 _$E177
// FUNCTION: STBC 0x0056b4a0 _$E176
// FUNCTION: STBC 0x0056b4b0 _$E389
// FUNCTION: STBC 0x0056b4c0 _$E388
// FUNCTION: STBC 0x0056b4d0 _$E182
// FUNCTION: STBC 0x0056b4e0 _$E181
// FUNCTION: STBC 0x0056b4f0 _$E393
// FUNCTION: STBC 0x0056b500 _$E392
// FUNCTION: STBC 0x0056b510 _$E397
// FUNCTION: STBC 0x0056b520 _$E396
// FUNCTION: STBC 0x0056b530 _$E187
// FUNCTION: STBC 0x0056b540 _$E186
// FUNCTION: STBC 0x0056b550 _$E401
// FUNCTION: STBC 0x0056b560 _$E400
// FUNCTION: STBC 0x0056b570 _$E152
// FUNCTION: STBC 0x0056b580 _$E151
// FUNCTION: STBC 0x0056b590 _$E405
// FUNCTION: STBC 0x0056b5a0 _$E404
// FUNCTION: STBC 0x0056b5b0 _$E409
// FUNCTION: STBC 0x0056b5c0 _$E408
// FUNCTION: STBC 0x0056b5d0 _$E192
// FUNCTION: STBC 0x0056b5e0 _$E191
// FUNCTION: STBC 0x0056b5f0 _$E197
// FUNCTION: STBC 0x0056b600 _$E196
// FUNCTION: STBC 0x0056b610 _$E413
// FUNCTION: STBC 0x0056b620 _$E412
// FUNCTION: STBC 0x0056b630 _$E417
// FUNCTION: STBC 0x0056b640 _$E416
// FUNCTION: STBC 0x0056b650 _$E421
// FUNCTION: STBC 0x0056b660 _$E420
// FUNCTION: STBC 0x0056b670 _$E425
// FUNCTION: STBC 0x0056b680 _$E424
// FUNCTION: STBC 0x0056b690 _$E429
// FUNCTION: STBC 0x0056b6a0 _$E428
// FUNCTION: STBC 0x0056b6b0 _$E433
// FUNCTION: STBC 0x0056b6c0 _$E432
// FUNCTION: STBC 0x0056b6d0 _$E437
// FUNCTION: STBC 0x0056b6e0 _$E436
// FUNCTION: STBC 0x0056b6f0 _$E441
// FUNCTION: STBC 0x0056b700 _$E440
// FUNCTION: STBC 0x0056b710 _$E202
// FUNCTION: STBC 0x0056b720 _$E201
// FUNCTION: STBC 0x0056b760 _$E445
// FUNCTION: STBC 0x0056b770 _$E444
// FUNCTION: STBC 0x0056b780 _$E207
// FUNCTION: STBC 0x0056b790 _$E206
// FUNCTION: STBC 0x0056b7a0 _$E212
// FUNCTION: STBC 0x0056b7b0 _$E211
// FUNCTION: STBC 0x0056b7c0 _$E449
// FUNCTION: STBC 0x0056b7d0 _$E448
// FUNCTION: STBC 0x0056b7e0 _$E454
// FUNCTION: STBC 0x0056b7f0 _$E453
// FUNCTION: STBC 0x0056b800 _$E458
// FUNCTION: STBC 0x0056b810 _$E457

/*
================
ShieldClass::Cast

The object as a ShieldClass; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00569fd0
ShieldClass *ShieldClass::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( SHIELDCLASS_TYPE ) ) {
		return (ShieldClass *)object;
	}
	return NULL;
}

/*
================
ShieldClass::ShieldClass

Every facing at 1000 and undamaged, its watcher on its percentage; the
last watcher watches the overall percentage.
================
*/
// FUNCTION: STBC 0x0056a000
ShieldClass::ShieldClass( long id ) : PoweredSubsystem( id ) {
	for ( int i = 0; i < NUM_SHIELDS; i++ ) {
		curShields[i] = 1000.0f;
		shieldPercentages[i] = 1.0f;
		unknown130[i] = 1.0f;
		shieldDamaged[i] = false;
		shieldWatchers[i].SetVariable( &shieldPercentages[i] );
	}
	shieldWatchers[NUM_SHIELDS].SetVariable( &shieldPercentages[NUM_SHIELDS] );
	chargeTime = 0.0f;
	shieldPercentages[NUM_SHIELDS] = 1.0f;
	chargePower = 0.0f;
}

// DATA: STBC 0x0098c180
TGEventHandlerTable ShieldClass::classHandlers;

/*
================
ShieldClass::GetObjType
================
*/
// FUNCTION: STBC 0x0056a0b0
int ShieldClass::GetObjType( void ) {
	return SHIELDCLASS_TYPE;
}

/*
================
ShieldClass::GetClassName
================
*/
// FUNCTION: STBC 0x0056a120
char *ShieldClass::GetClassName( void ) {
	return "ShieldClass";
}

/*
================
ShieldClass::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x0056a130
char *ShieldClass::GetSwigTypeName( void ) {
	return "_p_ShieldClass";
}

/*
================
ShieldClass::GetPythonClassName
================
*/
// FUNCTION: STBC 0x0056a140
char *ShieldClass::GetPythonClassName( void ) {
	return "ShieldClassPtr";
}

/*
================
ShieldClass::GetClassHandlers
================
*/
// FUNCTION: STBC 0x0056a150
TGEventHandlerTable *ShieldClass::GetClassHandlers( void ) {
	return &classHandlers;
}
/*
================
ShieldClass::~ShieldClass
================
*/
// FUNCTION: STBC 0x0056a190
// FUNCTION: STBC 0x0056a160 ??_GShieldClass@@UAEPAXI@Z
ShieldClass::~ShieldClass( void ) {
}

/*
================
ShieldClass::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x0056a1f0
void ShieldClass::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ShieldClass::HandleSetShieldState, "ShieldClass::HandleSetShieldState" );
}

/*
================
ShieldClass::RegisterHandlers
================
*/
// FUNCTION: STBC 0x0056a210
void ShieldClass::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_SHIELDS_SET_STATE, "ShieldClass::HandleSetShieldState" );
}

// Update charges the facings this often, in seconds. A variable, not a constant:
// retail reads it from the unit's .data, beside its strings (invented name).
// DATA: STBC 0x008e529c
static float shieldChargeInterval = 0.5f;

/*
================
ShieldClass::Update

Every half second, charges the facings with the power received since
the last charge, less the losses; switched off, the facings draw power
from the ship's power subsystem themselves, and disabled they drop.
Then the facings' percentages and their watchers are updated.
================
*/
// FUNCTION: STBC 0x0056a230
// PARKED: register residue: retail swaps the power subsystem and watcher registers in the switched-off loop and addresses the facing maxima from the property base; calls and branches match
void ShieldClass::Update( float deltaTime ) {
	chargeTime += deltaTime;
	chargePower += powerReceived;
	if ( chargeTime < shieldChargeInterval ) {
		PoweredSubsystem::Update( deltaTime );
		return;
	}

	float facingPower = chargePower / NUM_SHIELDS;
	chargeTime = 0.0f;
	ShipClass *ship = GetParentShip();
	int i;
	if ( !IsDisabled() && IsOn() ) {
		float boost = facingPower * SHIELD_CHARGE_EFFICIENCY;
		for ( i = 0; i < NUM_SHIELDS; i++ ) {
			BoostShield( i, boost );
			if ( GetMaxShields( i ) > 0.0f ) {
				shieldPercentages[i] = curShields[i] / GetMaxShields( i );
			} else {
				shieldPercentages[i] = 0.0f;
			}
			shieldWatchers[i].CheckRanges();
		}
		shieldPercentages[NUM_SHIELDS] = GetShieldPercentage();
	} else {
		for ( i = 0; i < NUM_SHIELDS; i++ ) {
			if ( IsDisabled() ) {
				curShields[i] = 0.0f;
			} else {
				PowerSubsystem *power = ship->GetPowerSubsystem();
				if ( power != NULL ) {
					float drawn = power->StealPower( GetProperty()->GetNormalPowerPerSecond() / NUM_SHIELDS );
					power->AddPower( BoostShield( i, drawn ) );
				}
			}
			shieldPercentages[i] = 0.0f;
			shieldWatchers[i].CheckRanges();
		}
		shieldPercentages[NUM_SHIELDS] = 0.0f;
	}
	shieldWatchers[NUM_SHIELDS].CheckRanges();
	chargePower = 0.0f;
	PoweredSubsystem::Update( deltaTime );
}

/*
================
ShieldClass::BoostShield

Charges a facing with the power, at the facing's share of the normal
power; returns the power left over once the facing is full (all of it
when the shields draw no power).
================
*/
// FUNCTION: STBC 0x0056a420
float ShieldClass::BoostShield( int shield, float power ) {
	float facingPower = GetProperty()->GetNormalPowerPerSecond() / NUM_SHIELDS;
	if ( facingPower <= 0.0f ) {
		return power;
	}

	float charge = curShields[shield] + GetShieldChargePerSecond( shield ) * power / facingPower;
	curShields[shield] = charge;
	if ( GetMaxShields( shield ) < charge ) {
		float chargePerPower = GetShieldChargePerSecond( shield ) / facingPower;
		float leftOver = 0.0f;
		if ( chargePerPower > 0.0f ) {
			leftOver = ( charge - GetMaxShields( shield ) ) / chargePerPower;
		}
		curShields[shield] = GetMaxShields( shield );
		return leftOver;
	}
	return 0.0f;
}

/*
================
ShieldClass::SetProperty

Charges every facing to the property's maximum.
================
*/
// FUNCTION: STBC 0x0056a4f0
void ShieldClass::SetProperty( SubsystemProperty *property_ ) {
	ShipSubsystem::SetProperty( property_ );
	for ( int i = 0; i < NUM_SHIELDS; i++ ) {
		curShields[i] = GetMaxShields( i );
		unknown130[i] = 1.0f;
		shieldDamaged[i] = false;
	}
}

/*
================
ShieldClass::GetShieldPercentage

The weakest facing's charge over its maximum; none while the shields are
disabled or off.
================
*/
// FUNCTION: STBC 0x0056a540
float ShieldClass::GetShieldPercentage( void ) {
	float lowest = 1.0f;
	if ( IsDisabled() || !IsOn() ) {
		return 0.0f;
	}
	for ( int i = 0; i < NUM_SHIELDS; i++ ) {
		float percentage = 0.0f;
		if ( GetMaxShields( i ) > 0.0f ) {
			percentage = curShields[i] / GetMaxShields( i );
		}
		if ( percentage < lowest ) {
			lowest = percentage;
		}
	}
	return lowest;
}

/*
================
ShieldClass::SetCurShields

Sets a facing's charge, kept between none and the property's maximum.
================
*/
// FUNCTION: STBC 0x0056a5c0
void ShieldClass::SetCurShields( int shield, float charge ) {
	if ( GetMaxShields( shield ) < charge ) {
		charge = GetMaxShields( shield );
	}
	curShields[shield] = ( charge < 0.0f ) ? 0.0f : charge;
}

/*
================
ShieldClass::IsShieldBreached

A facing is breached when it holds less than one point or is damaged, and
every facing is while the shields are disabled or off.
================
*/
// FUNCTION: STBC 0x0056a620
bool ShieldClass::IsShieldBreached( int shield ) {
	if ( IsDisabled() || !IsOn() ) {
		return true;
	}
	return curShields[shield] < BREACHED_SHIELD_CHARGE || shieldDamaged[shield];
}

/*
================
ShieldClass::IsAnyShieldBreached
================
*/
// FUNCTION: STBC 0x0056a670
bool ShieldClass::IsAnyShieldBreached( void ) {
	for ( int i = 0; i < NUM_SHIELDS; i++ ) {
		if ( IsShieldBreached( i ) ) {
			return true;
		}
	}
	return false;
}

/*
================
ShieldClass::Unknown56A690

The shield facing the segment from start to end, world points, first meets:
the segment is taken into the frame of the ship's box scaled to a unit
sphere, and the facing is the one of the meeting point pushed out along its
normal on the box's ellipsoid; -1 when the segment misses.
================
*/
// FUNCTION: STBC 0x0056a690
int ShieldClass::Unknown56A690( const NiPoint3 *start, const NiPoint3 *end ) {
	ShipClass *ship = parentShip;
	NiPoint3 offset = ship->GetWorldLocation() - ship->boxCenter;
	NiPoint3 localStart = ( *start - offset ) * ship->GetNode()->GetWorldRotate();
	localStart.x /= ship->boxExtent.x;
	localStart.y /= ship->boxExtent.y;
	localStart.z /= ship->boxExtent.z;
	NiPoint3 localEnd = ( *end - offset ) * ship->GetNode()->GetWorldRotate();
	localEnd.x /= ship->boxExtent.x;
	localEnd.y /= ship->boxExtent.y;
	localEnd.z /= ship->boxExtent.z;
	NiPoint3 hit;
	NiPoint3 exit;
	if ( Unknown4570D0( localStart, localEnd - localStart, NiPoint3::ZERO, 1.0f, &hit, &exit ) != 0 ) {
		NiPoint3 normal;
		normal.x = hit.x * ship->boxExtent.x;
		normal.y = hit.y * ship->boxExtent.y;
		normal.z = hit.z * ship->boxExtent.z;
		normal.Unitize();
		NiPoint3 facing = normal + ( hit + hit );
		return Unknown56A8D0( &facing );
	}
	return -1;
}

/*
================
ShieldClass::Unknown56A8D0
================
*/
// FUNCTION: STBC 0x0056a8d0
int ShieldClass::Unknown56A8D0( const NiPoint3 *direction ) {
	int shield = 0;
	float axes[3];
	axes[0] = direction->y;
	axes[1] = direction->z;
	axes[2] = direction->x;
	float largest = axes[0];
	int i;
	for ( i = 1; i < 3; i++ ) {
		if ( largest < axes[i] ) {
			largest = axes[i];
			shield = i;
		}
	}
	for ( ; i < 6; i++ ) {
		float negative = -axes[i - 3];
		if ( negative > largest ) {
			largest = negative;
			shield = i;
		}
	}
	switch ( shield ) {
	case 0:
		shield = FRONT_SHIELDS;
		break;
	case 1:
		shield = TOP_SHIELDS;
		break;
	case 2:
		shield = RIGHT_SHIELDS;
		break;
	case 3:
		shield = REAR_SHIELDS;
		break;
	case 4:
		shield = BOTTOM_SHIELDS;
		break;
	case 5:
		shield = LEFT_SHIELDS;
		break;
	}
	return shield;
}

/*
================
ShieldClass::RedistributeShields

Pools the charge of every facing and deals it out again in equal shares,
facing by facing, until it is gone or every facing is full. A facing is
full when one share exceeds its maximum: it is set to the maximum and
the excess goes back to the pool. The original compares the share alone,
not the facing's new charge, with the maximum, and sums the maxima
without using them.
================
*/
// FUNCTION: STBC 0x0056a9c0
// PARKED: schedule: retail loads the two 0.0f sums after the pushes and zeroes full (memset) through ecx once this is in edi; calls, branches and frame agree
void ShieldClass::RedistributeShields( void ) {
	float total = 0.0f;
	float totalMax = 0.0f;
	bool full[NUM_SHIELDS];
	int i;
	memset( full, 0, sizeof( full ) );

	for ( i = 0; i < NUM_SHIELDS; i++ ) {
		total += curShields[i];
		totalMax += GetMaxShields( i );
		curShields[i] = 0.0f;
	}

	float remaining = total;
	int facingsLeft = NUM_SHIELDS;
	bool done = false;
	while ( facingsLeft > 0 && remaining > 0.0 && !done ) {
		done = true;
		float shareFraction = 1.0f / facingsLeft;
		for ( i = 0; i < NUM_SHIELDS && remaining > 0.0; i++ ) {
			if ( full[i] != true ) {
				float share = shareFraction * total;
				remaining -= share;
				float maxCharge = GetMaxShields( i );
				if ( share <= maxCharge ) {
					curShields[i] += share;
					done = false;
				} else {
					// this facing is full: share the rest among the others afresh
					curShields[i] = maxCharge;
					remaining += share - maxCharge;
					facingsLeft--;
					full[i] = true;
					done = false;
					break;
				}
			}
		}
		total = remaining;
	}
}

/*
================
ShieldClass::GetOppositeShield

The original returns the facing before the given one, which is the
opposite facing only for the rear, bottom and right shields.
================
*/
// FUNCTION: STBC 0x0056aad0
int ShieldClass::GetOppositeShield( int shield ) {
	return shield - 1;
}

/*
================
ShieldClass::HandleSetShieldState

Raises or lowers the shields as the event says. The event goes no
further.
================
*/
// FUNCTION: STBC 0x0056aae0
void ShieldClass::HandleSetShieldState( TGEvent *event ) {
	if ( ( (TGBoolEvent *)event )->GetBool() == true ) {
		TurnOn();
	} else {
		TurnOff();
	}
}

/*
================
ShieldClass::TurnOn

The shields stay down while the ship cloaks, is cloaked or decloaks.
================
*/
// FUNCTION: STBC 0x0056ab10
void ShieldClass::TurnOn( void ) {
	ShipClass *ship = parentShip;
	bool cloaked = false;
	CloakingSubsystem *cloak = ship->GetCloakingSubsystem();
	if ( cloak != NULL ) {
		cloaked = cloak->IsCloaking() || cloak->IsDecloaking() || cloak->IsCloaked() == true;
	}
	if ( ship == NULL || !cloaked ) {
		PoweredSubsystem::TurnOn();
	}
}

/*
================
ShieldClass::WriteToStream

Each facing's strength and damage flag, then each watcher between its
percentage and unknown130's entry.
================
*/
// FUNCTION: STBC 0x0056ab60
bool ShieldClass::WriteToStream( TGStream *stream ) {
	if ( !PoweredSubsystem::WriteToStream( stream ) ) {
		return false;
	}

	int i;
	for ( i = 0; i < NUM_SHIELDS; i++ ) {
		stream->WriteFloat( curShields[i] );
		stream->WriteChar( shieldDamaged[i] );
	}
	for ( i = 0; i < NUM_SHIELDS + 1; i++ ) {
		stream->WriteFloat( shieldPercentages[i] );
		shieldWatchers[i].WriteToStream( stream );
		stream->WriteFloat( unknown130[i] );
	}
	stream->WriteFloat( chargeTime );
	stream->WriteFloat( chargePower );
	return true;
}

/*
================
ShieldClass::ReadFromStream

Reads what WriteToStream writes, pointing each watcher back at its
percentage.
================
*/
// FUNCTION: STBC 0x0056ac10
bool ShieldClass::ReadFromStream( TGStream *stream ) {
	if ( !PoweredSubsystem::ReadFromStream( stream ) ) {
		return false;
	}

	int i;
	for ( i = 0; i < NUM_SHIELDS; i++ ) {
		curShields[i] = stream->ReadFloat();
		shieldDamaged[i] = stream->ReadChar() != 0;
	}
	for ( i = 0; i < NUM_SHIELDS + 1; i++ ) {
		shieldPercentages[i] = stream->ReadFloat();
		shieldWatchers[i].ReadFromStream( stream );
		shieldWatchers[i].SetVariable( &shieldPercentages[i] );
		unknown130[i] = stream->ReadFloat();
	}
	chargeTime = stream->ReadFloat();
	chargePower = stream->ReadFloat();
	return true;
}

/*
================
ShieldClass::FixUpPointers
================
*/
// FUNCTION: STBC 0x0056acc0
bool ShieldClass::FixUpPointers( void ) {
	if ( !ShipSubsystem::FixUpPointers() ) {
		return false;
	}
	for ( int i = 0; i < NUM_SHIELDS + 1; i++ ) {
		if ( !shieldWatchers[i].FixUpPointers() ) {
			return false;
		}
	}
	return true;
}

/*
================
ShieldClass::PostFixUp
================
*/
// FUNCTION: STBC 0x0056ad00
void ShieldClass::PostFixUp( void ) {
	ShipSubsystem::PostFixUp();
	for ( int i = 0; i < NUM_SHIELDS + 1; i++ ) {
		shieldWatchers[i].PostFixUp();
	}
}

/*
================
ShieldClass::VirtualSlot26

After PoweredSubsystem's state, the strength of each facing, in 255ths of
its maximum.
================
*/
// FUNCTION: STBC 0x0056ad30
void ShieldClass::VirtualSlot26( TGStream *stream ) {
	PoweredSubsystem::VirtualSlot26( stream );

	for ( int i = 0; i < NUM_SHIELDS; i++ ) {
		stream->WriteChar( (char)( GetCurShields( i ) / GetMaxShields( i ) * 255.0f ) );
	}
	stream->GetPos();
}

/*
================
ShieldClass::VirtualSlot27

Reads what VirtualSlot26 writes.
================
*/
// FUNCTION: STBC 0x0056ada0
void ShieldClass::VirtualSlot27( TGStream *stream ) {
	PoweredSubsystem::VirtualSlot27( stream );

	for ( int i = 0; i < NUM_SHIELDS; i++ ) {
		unsigned char charge = stream->ReadChar();
		SetCurShields( i, GetMaxShields( i ) * ( charge * ( 1.0f / 255.0f ) ) );
	}
	stream->GetPos();
}

/*
================
ShieldClass::VirtualSlot28

After PoweredSubsystem's state, the strength of each facing, as
VirtualSlot26 writes it.
================
*/
// FUNCTION: STBC 0x0056ae10
void ShieldClass::VirtualSlot28( TGStream *stream, bool flag ) {
	PoweredSubsystem::VirtualSlot28( stream, flag );

	for ( int i = 0; i < NUM_SHIELDS; i++ ) {
		stream->WriteChar( (char)( GetCurShields( i ) / GetMaxShields( i ) * 255.0f ) );
	}
	stream->GetPos();
}

/*
================
ShieldClass::VirtualSlot29

Reads what VirtualSlot28 writes; the strengths are taken only when the
state is newer than the last one taken.
================
*/
// FUNCTION: STBC 0x0056ae80
// PARKED: schedule: retail loads the property before the charge read back from the stack, ours after
void ShieldClass::VirtualSlot29( TGStream *stream, float time ) {
	float lastTime = unknown84;
	PoweredSubsystem::VirtualSlot29( stream, time );

	for ( int i = 0; i < NUM_SHIELDS; i++ ) {
		unsigned char charge = stream->ReadChar();
		if ( time > lastTime ) {
			SetCurShields( i, GetMaxShields( i ) * ( charge * ( 1.0f / 255.0f ) ) );
		}
	}
	stream->GetPos();
}
