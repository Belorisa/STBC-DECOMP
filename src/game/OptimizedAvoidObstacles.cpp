/*
===============================================================================

	OptimizedAvoidObstacles.cpp

===============================================================================
*/

#include <iostream>
#include <math.h>

#include "OptimizedAvoidObstacles.h"
#include "NiPoint3.h"
#include "AsteroidField.h"
#include "ImpulseEngineSubsystem.h"
#include "Nebula.h"
#include "NiMath.h"
#include "NiMatrix3.h"
#include "ObjectClass.h"
#include "Planet.h"
#include "PhysicsObjectClass.h"
#include "ProximityCheck.h"
#include "ProximityManager.h"
#include "SetClass.h"
#include "ShipClass.h"
#include "Torpedo.h"
#include "TGStream.h"
#include "UtopiaModule.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004817c0 _$E514
// FUNCTION: STBC 0x004817e0 _$E513

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0047f080 _$E32
// FUNCTION: STBC 0x0047f090 _$E29
// FUNCTION: STBC 0x0047f0a0 _$E31
// FUNCTION: STBC 0x0047f0b0 _$E30
// FUNCTION: STBC 0x0047f0c0 _$E38
// FUNCTION: STBC 0x0047f0d0 _$E35
// FUNCTION: STBC 0x0047f0e0 _$E37
// FUNCTION: STBC 0x0047f0f0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0047f100 _$E90
// FUNCTION: STBC 0x0047f110 _$E89
// FUNCTION: STBC 0x0047f120 _$E316
// FUNCTION: STBC 0x0047f130 _$E315
// FUNCTION: STBC 0x0047f5d0 _$E101
// FUNCTION: STBC 0x0047f5e0 _$E100
// FUNCTION: STBC 0x0047fa20 _$E105
// FUNCTION: STBC 0x0047fa30 _$E104
// FUNCTION: STBC 0x004800d0 _$E109
// FUNCTION: STBC 0x004800e0 _$E108
// FUNCTION: STBC 0x004806d0 _$E113
// FUNCTION: STBC 0x004806e0 _$E112
// FUNCTION: STBC 0x00480b00 _$E230
// FUNCTION: STBC 0x00480b10 _$E229
// FUNCTION: STBC 0x00480de0 _$E172
// FUNCTION: STBC 0x00480df0 _$E171
// FUNCTION: STBC 0x00480e00 _$E185
// FUNCTION: STBC 0x00480e10 _$E184
// FUNCTION: STBC 0x00480e20 _$E235
// FUNCTION: STBC 0x00480e30 _$E234
// FUNCTION: STBC 0x00480e40 _$E321
// FUNCTION: STBC 0x00480e50 _$E320
// FUNCTION: STBC 0x00480e60 _$E239
// FUNCTION: STBC 0x00480e70 _$E238
// FUNCTION: STBC 0x00480e80 _$E325
// FUNCTION: STBC 0x00480e90 _$E324
// FUNCTION: STBC 0x00480ea0 _$E332
// FUNCTION: STBC 0x00480eb0 _$E331
// FUNCTION: STBC 0x00480ec0 _$E350
// FUNCTION: STBC 0x00480ed0 _$E349
// FUNCTION: STBC 0x00480ee0 _$E354
// FUNCTION: STBC 0x00480ef0 _$E353
// FUNCTION: STBC 0x00480f00 _$E130
// FUNCTION: STBC 0x00480f10 _$E129
// FUNCTION: STBC 0x00480f20 _$E278
// FUNCTION: STBC 0x00480f30 _$E277
// FUNCTION: STBC 0x00480f40 _$E285
// FUNCTION: STBC 0x00480f50 _$E284
// FUNCTION: STBC 0x00480f60 _$E289
// FUNCTION: STBC 0x00480f70 _$E288
// FUNCTION: STBC 0x00480f80 _$E293
// FUNCTION: STBC 0x00480f90 _$E292
// FUNCTION: STBC 0x00480fa0 _$E297
// FUNCTION: STBC 0x00480fb0 _$E296
// FUNCTION: STBC 0x00480fc0 _$E303
// FUNCTION: STBC 0x00480fd0 _$E302
// FUNCTION: STBC 0x00480fe0 _$E358
// FUNCTION: STBC 0x00480ff0 _$E357
// FUNCTION: STBC 0x00481000 _$E362
// FUNCTION: STBC 0x00481010 _$E361
// FUNCTION: STBC 0x00481020 _$E366
// FUNCTION: STBC 0x00481030 _$E365
// FUNCTION: STBC 0x00481040 _$E370
// FUNCTION: STBC 0x00481050 _$E369
// FUNCTION: STBC 0x00481060 _$E374
// FUNCTION: STBC 0x00481070 _$E373
// FUNCTION: STBC 0x00481080 _$E378
// FUNCTION: STBC 0x00481090 _$E377
// FUNCTION: STBC 0x004810a0 _$E382
// FUNCTION: STBC 0x004810b0 _$E381
// FUNCTION: STBC 0x004810c0 _$E386
// FUNCTION: STBC 0x004810d0 _$E385
// FUNCTION: STBC 0x004810e0 _$E390
// FUNCTION: STBC 0x004810f0 _$E389
// FUNCTION: STBC 0x00481100 _$E394
// FUNCTION: STBC 0x00481110 _$E393
// FUNCTION: STBC 0x00481120 _$E135
// FUNCTION: STBC 0x00481130 _$E134
// FUNCTION: STBC 0x00481140 _$E398
// FUNCTION: STBC 0x00481150 _$E397
// FUNCTION: STBC 0x00481160 _$E140
// FUNCTION: STBC 0x00481170 _$E139
// FUNCTION: STBC 0x00481180 _$E402
// FUNCTION: STBC 0x00481190 _$E401
// FUNCTION: STBC 0x004811a0 _$E406
// FUNCTION: STBC 0x004811b0 _$E405
// FUNCTION: STBC 0x004811c0 _$E410
// FUNCTION: STBC 0x004811d0 _$E409
// FUNCTION: STBC 0x004811e0 _$E414
// FUNCTION: STBC 0x004811f0 _$E413
// FUNCTION: STBC 0x00481200 _$E149
// FUNCTION: STBC 0x00481210 _$E148
// FUNCTION: STBC 0x00481220 _$E418
// FUNCTION: STBC 0x00481230 _$E417
// FUNCTION: STBC 0x00481240 _$E422
// FUNCTION: STBC 0x00481250 _$E421
// FUNCTION: STBC 0x00481260 _$E426
// FUNCTION: STBC 0x00481270 _$E425
// FUNCTION: STBC 0x00481280 _$E155
// FUNCTION: STBC 0x00481290 _$E154
// FUNCTION: STBC 0x004812a0 _$E160
// FUNCTION: STBC 0x004812b0 _$E159
// FUNCTION: STBC 0x004812c0 _$E190
// FUNCTION: STBC 0x004812d0 _$E189
// FUNCTION: STBC 0x004812e0 _$E430
// FUNCTION: STBC 0x004812f0 _$E429
// FUNCTION: STBC 0x00481300 _$E195
// FUNCTION: STBC 0x00481310 _$E194
// FUNCTION: STBC 0x00481320 _$E434
// FUNCTION: STBC 0x00481330 _$E433
// FUNCTION: STBC 0x00481340 _$E438
// FUNCTION: STBC 0x00481350 _$E437
// FUNCTION: STBC 0x00481360 _$E200
// FUNCTION: STBC 0x00481370 _$E199
// FUNCTION: STBC 0x00481380 _$E442
// FUNCTION: STBC 0x00481390 _$E441
// FUNCTION: STBC 0x004813a0 _$E165
// FUNCTION: STBC 0x004813b0 _$E164
// FUNCTION: STBC 0x00481400 _$E446
// FUNCTION: STBC 0x00481410 _$E445
// FUNCTION: STBC 0x00481420 _$E450
// FUNCTION: STBC 0x00481430 _$E449
// FUNCTION: STBC 0x00481440 _$E205
// FUNCTION: STBC 0x00481450 _$E204
// FUNCTION: STBC 0x00481460 _$E210
// FUNCTION: STBC 0x00481470 _$E209
// FUNCTION: STBC 0x004814c0 _$E454
// FUNCTION: STBC 0x004814d0 _$E453
// FUNCTION: STBC 0x004814e0 _$E458
// FUNCTION: STBC 0x004814f0 _$E457
// FUNCTION: STBC 0x00481500 _$E462
// FUNCTION: STBC 0x00481510 _$E461
// FUNCTION: STBC 0x00481520 _$E466
// FUNCTION: STBC 0x00481530 _$E465
// FUNCTION: STBC 0x00481540 _$E470
// FUNCTION: STBC 0x00481550 _$E469
// FUNCTION: STBC 0x00481560 _$E474
// FUNCTION: STBC 0x00481570 _$E473
// FUNCTION: STBC 0x00481580 _$E478
// FUNCTION: STBC 0x00481590 _$E477
// FUNCTION: STBC 0x004815a0 _$E482
// FUNCTION: STBC 0x004815b0 _$E481
// FUNCTION: STBC 0x004815c0 _$E215
// FUNCTION: STBC 0x004815d0 _$E214
// FUNCTION: STBC 0x004815e0 _$E486
// FUNCTION: STBC 0x004815f0 _$E485
// FUNCTION: STBC 0x00481600 _$E220
// FUNCTION: STBC 0x00481610 _$E219
// FUNCTION: STBC 0x00481620 _$E225
// FUNCTION: STBC 0x00481630 _$E224
// FUNCTION: STBC 0x00481820 _$E490
// FUNCTION: STBC 0x00481830 _$E489
// FUNCTION: STBC 0x00481840 _$E495
// FUNCTION: STBC 0x00481850 _$E494
// FUNCTION: STBC 0x00481860 _$E499
// FUNCTION: STBC 0x00481870 _$E498

// The seconds between two looks while the ship steers around an object,
// and while the way is clear (the names are invented).
static const float AVOIDING_UPDATE_INTERVAL = 0.0f;
static const float UPDATE_INTERVAL = 0.0f;

// How far ahead the ship's motion is predicted, in seconds, and the step the
// object tests advance by (invented names).
static const float LOOK_AHEAD_TIME = 10.0f;
static const float LOOK_AHEAD_STEP = 0.5f;

// The look-ahead radius, in ship radii: for a ship slower than the first
// speed, and for one faster than the second, with the blend between them
// (invented names).
static const float SLOW_RADIUS_SCALE = 2.5f;
static const float FAST_RADIUS_SCALE = 4.0f;
static const float SLOW_SPEED = 0.1f;
static const float FAST_SPEED = 2.0f;

// The look-ahead radius is never smaller than this (invented name).
static const float MIN_LOOK_AHEAD_RADIUS = 225.0f;

// The weight a candidate gets over its distance (invented name).
static const float CANDIDATE_WEIGHT = 50.0f;

// Objects larger than the first radius are taken for the smaller of
// the scale of it and the radius plus the extra (invented names).
static const float MARGIN_RADIUS_THRESHOLD = 25.0f;
static const float MARGIN_EXTRA = 125.0f;
static const float MARGIN_SCALE = 1.25f;

// How long a noted obstacle is kept without a new sighting, in seconds
// (invented name).
static const float OBSTACLE_MEMORY = 1.0f;

// The impulse engines' power setting while the ship steers around an
// object (invented name).
static const float AVOIDING_POWER_PERCENTAGE = 1.25f;

// savedPowerPercentage when no setting is saved (invented name).
#define NO_POWER_PERCENTAGE		-1.0f

// FUNCTION: STBC 0x0047f140 _$E502
// FUNCTION: STBC 0x0047f150 _$E501
static AICreateFunctionRegistration avoidObstaclesRegistration( AI_TYPE_AVOID_OBSTACLES, OptimizedAvoidObstacles::Create );

// FUNCTION: STBC 0x0047f1a0 _$E505
// FUNCTION: STBC 0x0047f1b0 _$E504
static OptimizedVersionRegistration avoidObstaclesVersion( "AvoidObstacles", OptimizedAvoidObstacles::CreateFromScript );

/*
================
OptimizedAvoidObstacles::OptimizedAvoidObstacles
================
*/
// FUNCTION: STBC 0x0047f270 ??0OptimizedAvoidObstacles@@QAE@PAVPreprocessingAI@@@Z
OptimizedAvoidObstacles::OptimizedAvoidObstacles( PreprocessingAI *ai ) : PreprocessingAI( ai->GetObject(), ai->name ),
	obstacles( 2 ) {
	containedAI = ai->containedAI;
	ai->containedAI = NULL;
	containedAI->SetParentID( GetID() );
	interruptable = ai->interruptable;
	savedPowerPercentage = NO_POWER_PERCENTAGE;
}

/*
================
OptimizedAvoidObstacles::GetAIType
================
*/
// FUNCTION: STBC 0x0047f2e0
int OptimizedAvoidObstacles::GetAIType( void ) {
	return AI_TYPE_AVOID_OBSTACLES;
}

/*
================
OptimizedAvoidObstacles::VirtualSlot20
================
*/
// FUNCTION: STBC 0x0047f2f0
bool OptimizedAvoidObstacles::VirtualSlot20( int type ) {
	if ( type == AI_TYPE_AVOID_OBSTACLES ) {
		return true;
	}
	return PreprocessingAI::VirtualSlot20( type );
}

/*
================
OptimizedAvoidObstacles::~OptimizedAvoidObstacles
================
*/
// FUNCTION: STBC 0x0047f330
// FUNCTION: STBC 0x0047f310 ??_GOptimizedAvoidObstacles@@UAEPAXI@Z
OptimizedAvoidObstacles::~OptimizedAvoidObstacles( void ) {
}

/*
================
OptimizedAvoidObstacles::CreateFromScript
================
*/
// FUNCTION: STBC 0x0047f380
ArtificialIntelligence *OptimizedAvoidObstacles::CreateFromScript( PreprocessingAI *ai ) {
	ArtificialIntelligence *optimized = new OptimizedAvoidObstacles( ai );
	delete ai;
	return optimized;
}

/*
================
OptimizedAvoidObstacles::CallPreprocessingMethod

While an object is in the way the ship turns away from it at the
impulse found and the contained AI is skipped; the player's own ship
keeps its power setting.
================
*/
// FUNCTION: STBC 0x0047f3f0
// PARKED: register allocation: retail keeps this in EDI and the ship in ESI (swapped here) and pops EDI after the final interval store; the SetImpulse call shape matches
int OptimizedAvoidObstacles::CallPreprocessingMethod( double time ) {
	if ( !forceUpdate && g_kUtopiaModule.GetGameTime() < nextUpdateTime ) {
		return preprocessStatus;
	}
	preprocessStatus = PS_NORMAL;
	float now = g_kUtopiaModule.GetGameTime();
	nextUpdateTime = now + UPDATE_INTERVAL;

	ShipClass *ship = GetShip();
	if ( ship == NULL ) {
		return PS_DONE;
	}
	if ( ship->IsDoingInSystemWarp() ) {
		return PS_NORMAL;
	}
	ImpulseEngineSubsystem *impulseEngines = ship->GetImpulseEngineSubsystem();
	if ( impulseEngines == NULL || impulseEngines->IsDisabled() ) {
		return PS_NORMAL;
	}

	NiPoint3 direction;
	float impulse;
	if ( Unknown47F5F0( ship, &direction, &impulse ) ) {
		preprocessStatus = PS_SKIP_ACTIVE;
		ship->TurnTowardDirection( direction );
		ship->SetImpulse( impulse, TGPoint3::GetModelForward(), 0 );
		if ( !ship->IsPlayerShip() ) {
			if ( savedPowerPercentage == NO_POWER_PERCENTAGE ) {
				savedPowerPercentage = impulseEngines->GetPowerPercentageWanted();
			}
			impulseEngines->SetPowerPercentageWanted( AVOIDING_POWER_PERCENTAGE );
		}
		nextUpdateTime = g_kUtopiaModule.GetGameTime() + AVOIDING_UPDATE_INTERVAL;
		return preprocessStatus;
	}

	if ( savedPowerPercentage != NO_POWER_PERCENTAGE ) {
		impulseEngines->SetPowerPercentageWanted( savedPowerPercentage );
		savedPowerPercentage = NO_POWER_PERCENTAGE;
	}
	return preprocessStatus;
}

/*
================
OptimizedAvoidObstacles::Unknown47F5F0

Looks along the ship's predicted motion for objects in the way; true when
there is one, with the direction to steer and the impulse to use.
================
*/
// FUNCTION: STBC 0x0047f5f0
// DRAFT: retail keeps NiTList<ObjectClass *>::NewItem out of line inside AddTail (SetClass's copy, 0x417B30) where this build expands it (c2trace: share 141 must fall under 129: own cost 30 lower or one more inline site after AddTail), and keeps ship in ESI, the proximity manager in EBX and the iterator in EBP with no zero held in a register; the float temporaries sit in the dead ship argument slot
bool OptimizedAvoidObstacles::Unknown47F5F0( ShipClass *ship, NiPoint3 *direction, float *impulse ) {
	SetClass *set = ship->GetContainingSet();
	if ( set == NULL ) {
		return false;
	}

	const NiPoint3 &velocity = ship->GetVelocity();
	NiPoint3 predicted = ship->GetPredictedPosition( &ship->GetWorldLocation(), &velocity, &ship->GetAcceleration(), LOOK_AHEAD_TIME );
	float distance = NiPoint3::VectorLength( predicted - ship->GetWorldLocation() );

	float radius = ship->GetRadius() * SLOW_RADIUS_SCALE;
	float speed = ship->GetVelocity().Length();
	if ( speed >= SLOW_SPEED ) {
		if ( speed > FAST_SPEED ) {
			radius = ship->GetRadius() * FAST_RADIUS_SCALE;
		} else {
			float t = ( speed - SLOW_SPEED ) / ( FAST_SPEED - SLOW_SPEED );
			radius = ( ( 1.0f - t ) * SLOW_RADIUS_SCALE + FAST_RADIUS_SCALE * t ) * ship->GetRadius();
		}
	}
	float lookAhead = radius + distance;
	if ( lookAhead < MIN_LOOK_AHEAD_RADIUS ) {
		lookAhead = MIN_LOOK_AHEAD_RADIUS;
	}

	ProximityManager *proximity = set->GetProximityManager();
	if ( proximity == NULL ) {
		return false;
	}

	ProximityManager::ObjectIterator *iterator = proximity->GetNearObjects( predicted, lookAhead, false );
	ObjectClass *object = proximity->GetNextObject( iterator );
	NiTList<ObjectClass *> found( 2 );
	while ( object != NULL ) {
		if ( object != ship && Unknown47FCA0( ship, radius, object ) ) {
			found.AddTail( object );
			NoteObstacle( object );
		}
		object = proximity->GetNextObject( iterator );
	}
	proximity->EndObjectIteration( iterator );

	Unknown47FB00( &found );
	if ( found.GetSize() == 0 ) {
		return false;
	}
	Unknown480250( ship, &found, direction, impulse );
	return true;
}

/*
================
OptimizedAvoidObstacles::NoteObstacle

An object already noted gets the current time again; a new one is added.
================
*/
// FUNCTION: STBC 0x0047fa40
// DRAFT: structure from the disassembly; not yet compared
void OptimizedAvoidObstacles::NoteObstacle( TGObject *object ) {
	NiTListIterator pos = obstacles.GetHeadPos();
	while ( pos != NULL ) {
		AvoidedObstacle *obstacle = obstacles.GetNext( pos );
		if ( obstacle->objectID == object->GetObjID() ) {
			obstacle->time = g_kUtopiaModule.GetGameTime();
			return;
		}
	}
	AvoidedObstacle *obstacle = new AvoidedObstacle;
	obstacle->objectID = object->GetObjID();
	obstacle->time = g_kUtopiaModule.GetGameTime();
	obstacles.AddTail( obstacle );
}

/*
================
OptimizedAvoidObstacles::Unknown47FB00
================
*/
// FUNCTION: STBC 0x0047fb00
void OptimizedAvoidObstacles::Unknown47FB00( NiTList<ObjectClass *> *found ) {
	NiTListIterator pos = obstacles.GetHeadPos();
	while ( pos != NULL ) {
		NiTListIterator current = pos;
		AvoidedObstacle *obstacle = obstacles.GetNext( pos );

		bool isFound = false;
		NiTListIterator foundPos = found->GetHeadPos();
		while ( foundPos != NULL ) {
			ObjectClass *object = found->GetNext( foundPos );
			if ( object->GetObjID() == obstacle->objectID ) {
				isFound = true;
				break;
			}
		}
		if ( isFound ) {
			continue;
		}

		ObjectClass *object = ObjectClass::GetObjectByID( NULL, obstacle->objectID );
		if ( object == NULL || g_kUtopiaModule.GetGameTime() - obstacle->time > OBSTACLE_MEMORY ) {
			obstacles.RemovePos( current );
			delete obstacle;
		} else {
			found->AddTail( object );
		}
	}
}

/*
================
OptimizedAvoidObstacles::Unknown47FCA0
================
*/
// FUNCTION: STBC 0x0047fca0
bool OptimizedAvoidObstacles::Unknown47FCA0( ShipClass *ship, float radius, ObjectClass *object ) {
	ShipClass *otherShip = ShipClass::Cast( object );
	if ( otherShip != NULL && otherShip->IsCloaked() ) {
		return false;
	}

	// 0x800A is the type of an object class not identified yet.
	int ignoredTypes[5] = { PROXIMITYCHECK_TYPE, 0x800A, TORPEDO_TYPE, ASTEROIDFIELD_TYPE, NEBULA_TYPE };
	for ( unsigned int i = 0; i < 5; i++ ) {
		if ( object->IsTypeOf( ignoredTypes[i] ) ) {
			return false;
		}
	}

	float margin = object->GetRadius();
	if ( margin > MARGIN_RADIUS_THRESHOLD ) {
		float scaled = MARGIN_SCALE * margin;
		float extended = margin + MARGIN_EXTRA;
		margin = ( scaled > extended ) ? extended : scaled;
	}

	float distance = NiPoint3::VectorLength( ship->GetWorldLocation() - object->GetWorldLocation() );
	if ( distance < margin + radius ) {
		return true;
	}

	NiPoint3 shipPosition, shipVelocity, shipAcceleration, shipAngularAxis, shipAngularAcceleration, shipUnknownC0;
	float shipAngularSpeed, shipUnknownE4;
	NiMatrix3 shipRotation;
	ship->Unknown5A08F0( &shipPosition, &shipVelocity, &shipAcceleration, &shipAngularAxis, &shipAngularSpeed, &shipAngularAcceleration, &shipUnknownC0, &shipUnknownE4, &shipRotation );

	PhysicsObjectClass *physical = PhysicsObjectClass::Cast( object );
	NiPoint3 objectPosition, objectVelocity, objectAcceleration, objectAngularAxis, objectAngularAcceleration, objectUnknownC0;
	float objectAngularSpeed, objectUnknownE4;
	NiMatrix3 objectRotation;
	if ( physical != NULL ) {
		physical->Unknown5A08F0( &objectPosition, &objectVelocity, &objectAcceleration, &objectAngularAxis, &objectAngularSpeed, &objectAngularAcceleration, &objectUnknownC0, &objectUnknownE4, &objectRotation );
	} else {
		objectPosition = object->GetWorldLocation();
		objectVelocity = NiPoint3::ZERO;
	}

	bool collides = false;
	for ( float elapsed = 0.0f; elapsed < LOOK_AHEAD_TIME; elapsed += LOOK_AHEAD_STEP ) {
		NiPoint3 shipBefore = shipPosition;
		NiPoint3 objectBefore = objectPosition;
		ship->Unknown5A09D0( LOOK_AHEAD_STEP, &shipPosition, &shipVelocity, &shipAcceleration, &shipAngularAxis, &shipAngularSpeed, &shipAngularAcceleration, &shipUnknownC0, &shipUnknownE4, &shipRotation );
		if ( physical != NULL ) {
			physical->Unknown5A09D0( LOOK_AHEAD_STEP, &objectPosition, &objectVelocity, &objectAcceleration, &objectAngularAxis, &objectAngularSpeed, &objectAngularAcceleration, &objectUnknownC0, &objectUnknownE4, &objectRotation );
		}
		object->IsTypeOf( PLANET_TYPE );	// BUG: redundant: the result is unused
		NiPoint3 objectStep = objectPosition - objectBefore;
		NiPoint3 shipStep = shipPosition - shipBefore;
		if ( WillCollide( &shipBefore, &shipStep, &objectBefore, &objectStep, radius, 1.0f ) ) {
			collides = true;
			break;
		}
	}
	return collides;
}

/*
================
OptimizedAvoidObstacles::WillCollide

Solves |( position - otherPosition ) + t * ( step - otherStep )| = radius for
the first t at or after 0 and tests it against maxTime.
================
*/
// FUNCTION: STBC 0x004800f0
// DRAFT: the quadratic, its root choice and the one false exit follow the disassembly; retail keeps the velocity and the offset's y in a 0x18 frame (a and the doubled dot product over argument slots), this build keeps the velocity in x87 registers with no frame (both as NiPoint3 locals: frame 0xC, 43.3)
bool OptimizedAvoidObstacles::WillCollide( NiPoint3 *position, NiPoint3 *step, NiPoint3 *otherPosition, NiPoint3 *otherStep, float radius, float maxTime ) {
	NiPoint3 velocity = *step - *otherStep;
	float rx = position->x - otherPosition->x;
	float ry = position->y - otherPosition->y;
	float rz = position->z - otherPosition->z;

	float a = velocity.x * velocity.x + velocity.z * velocity.z + velocity.y * velocity.y;
	if ( a > 0.0f ) {
		float b = rz * velocity.z + ry * velocity.y + rx * velocity.x;
		b = b + b;
		float discriminant = b * b - ( rx * rx + ( ( rz * rz + ry * ry ) - radius * radius ) ) * a * 4.0f;
		if ( discriminant >= 0.0f ) {
			float root = (float)sqrt( discriminant );
			float t1 = ( root - b ) / ( a + a );
			float t2 = ( -b - root ) / ( a + a );
			float t = t2;
			if ( t2 <= t1 ) {
				if ( t2 < 0.0f ) {
					t = t1;
				}
			} else if ( t1 >= 0.0f ) {
				t = t1;
			}
			if ( t >= 0.0f && t < maxTime ) {
				return true;
			}
		}
	}
	return false;
}

/*
================
OptimizedAvoidObstacles::Unknown480250
================
*/
// FUNCTION: STBC 0x00480250
// DRAFT: frame as retail's (the forward vector a temporary); left: retail keeps the iterator in ebx and the distance in ebp (ours the reverse), the zero in edi, and calls NiTListBlock's scalar deleting destructor (0x4817F0) at the end where this build expands it (share 61 against 50)
void OptimizedAvoidObstacles::Unknown480250( ShipClass *ship, NiTList<ObjectClass *> *objects, NiPoint3 *direction, float *impulse ) {
	NiTList<AvoidanceCandidate *> candidates( 2 );

	NiTListIterator pos = objects->GetHeadPos();
	while ( pos != NULL ) {
		ObjectClass *object = objects->GetNext( pos );
		AvoidanceCandidate *candidate = new AvoidanceCandidate;
		candidate->direction = object->GetWorldLocation() - ship->GetWorldLocation();
		float distance = candidate->direction.Unitize();

		PhysicsObjectClass *physical = PhysicsObjectClass::Cast( object );
		if ( physical != NULL ) {
			candidate->velocity = physical->GetVelocity();
		} else {
			candidate->velocity = NiPoint3::ZERO;
		}

		if ( distance <= 0.0f ) {
			delete candidate;
		} else {
			float reach = object->GetRadius() + ship->GetRadius();
			if ( distance <= reach ) {
				candidate->cosine = 0.0f;
			} else {
				candidate->cosine = NiSqrt( distance * distance - reach * reach ) / distance;
			}
			candidate->weight = -( CANDIDATE_WEIGHT / distance );
			candidates.AddTail( candidate );
		}
	}

	float best = -NI_INFINITY;
	pos = candidates.GetHeadPos();
	while ( pos != NULL ) {
		AvoidanceCandidate *candidate = candidates.GetNext( pos );
		NiPoint3 away = -candidate->direction;
		float rating = Unknown4806F0( ship, &away, &candidates );
		if ( rating > best ) {
			best = rating;
			*direction = away;
		}
	}

	for ( int x = -1; x <= 1; x++ ) {
		for ( int y = -1; y <= 1; y++ ) {
			for ( int z = -1; z <= 1; z++ ) {
				if ( x == 0 && y == 0 && z == 0 ) {
					continue;
				}
				NiPoint3 cell( (float)x, (float)y, (float)z );
				cell.Unitize();
				float rating = Unknown4806F0( ship, &cell, &candidates );
				if ( rating > best ) {
					best = rating;
					*direction = cell;
				}
			}
		}
	}

	if ( Unknown480B20( ship->GetWorldForwardTG(), &candidates ) ) {
		*impulse = 1.0f;
	} else {
		*impulse = 0.0f;
	}

	while ( candidates.GetSize() != 0 ) {
		delete candidates.RemoveHead();
	}
}

// The weight a direction into a candidate's cone is penalised by, the speed
// above which an object counts as moving, and the factor the ship's own
// heading is weighed by (invented names).
static const float FORWARD_FACTOR = -0.25f;
static const float MIN_MOVING_SPEED = 0.0625f;
static const float CONE_PENALTY = 3.25f;

/*
================
OptimizedAvoidObstacles::Unknown4806F0
================
*/
// FUNCTION: STBC 0x004806f0
// DRAFT: structure from the disassembly (cone penalty, moving-object turn terms, forward term); retail calls Length (0x4145b0), the NiPoint3 constructor (0x414540) and NiPoint3::Dot (0x428160) out of line where this build expands or calls Unitize; the three constants after the vtable (0x88BDF8-0x88BE00) land before it here
float OptimizedAvoidObstacles::Unknown4806F0( ShipClass *ship, NiPoint3 *direction, NiTList<AvoidanceCandidate *> *candidates ) {
	float rating = 0.0f;
	NiTListIterator pos = candidates->GetHeadPos();
	while ( pos != NULL ) {
		AvoidanceCandidate *candidate = candidates->GetNext( pos );

		// a direction into the object's cone is penalised by its weight
		if ( candidate->direction.Dot( *direction ) >= candidate->cosine ) {
			rating += candidate->weight * CONE_PENALTY;
			continue;
		}

		NiPoint3 heading = candidate->velocity;
		float speed = heading.Unitize();
		if ( speed > MIN_MOVING_SPEED ) {
			// a moving object: the direction's alignment with its heading, and how
			// the turn about the heading compares with the object's own
			float side = heading.Dot( *direction );
			if ( side < 0.0f ) {
				side = -side;
			}
			rating += ( side - 0.5f ) * candidate->weight * 2.0f;

			NiPoint3 axis = candidate->direction.Cross( candidate->velocity );
			axis.Unitize();
			TGPoint3 around = TGPoint3( *direction ).GetPerpendicularComponent( TGPoint3( candidate->velocity ) );
			TGPoint3 objectAround = TGPoint3( candidate->direction ).GetPerpendicularComponent( TGPoint3( candidate->velocity ) );
			NiPoint3 turn( around );
			turn.Unitize();
			NiPoint3 objectTurn( objectAround );
			objectTurn.Unitize();
			float turnAlignment = turn.Dot( objectTurn );
			if ( turnAlignment >= 0.0f ) {
				rating += ( turnAlignment - 0.5f ) * candidate->weight * 2.0f;
			} else {
				rating += ( -1.0f - turnAlignment ) * candidate->weight;
			}
		} else {
			float alignment = candidate->direction.Dot( *direction );
			rating += ( ( alignment + 1.0f ) / ( candidate->cosine + 1.0f ) - 0.5f ) * 2.0f * candidate->weight;
		}

		// the ship's own heading is preferred
		rating += ship->GetWorldForward().Dot( *direction ) * candidate->weight * FORWARD_FACTOR;
	}
	return rating;
}

/*
================
OptimizedAvoidObstacles::Unknown480B20
================
*/
// FUNCTION: STBC 0x00480b20
bool OptimizedAvoidObstacles::Unknown480B20( const NiPoint3 &direction, NiTList<AvoidanceCandidate *> *candidates ) {
	NiTListIterator pos = candidates->GetHeadPos();
	while ( pos != NULL ) {
		AvoidanceCandidate *candidate = candidates->GetNext( pos );
		if ( candidate->direction.Dot( direction ) >= candidate->cosine ) {
			return false;
		}
	}
	return true;
}

/*
================
OptimizedAvoidObstacles::WriteToStream
================
*/
// FUNCTION: STBC 0x00480b70
bool OptimizedAvoidObstacles::WriteToStream( TGStream *stream ) {
	if ( !PreprocessingAI::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteInt( obstacles.GetSize() );
	NiTListIterator pos = obstacles.GetHeadPos();
	while ( pos != NULL ) {
		AvoidedObstacle *obstacle = obstacles.GetNext( pos );
		stream->WriteID( obstacle->objectID );
		stream->WriteFloat( obstacle->time );
	}
	stream->WriteFloat( savedPowerPercentage );
	return true;
}

/*
================
OptimizedAvoidObstacles::OptimizedAvoidObstacles
================
*/
// FUNCTION: STBC 0x00480be0 ??0OptimizedAvoidObstacles@@QAE@PAVTGStream@@PA_N@Z
OptimizedAvoidObstacles::OptimizedAvoidObstacles( TGStream *stream, bool *ok ) : PreprocessingAI( stream, ok ),
	obstacles( 2 ) {
	if ( *ok ) {
		for ( int i = stream->ReadInt() - 1; i >= 0; i-- ) {
			AvoidedObstacle *obstacle = new AvoidedObstacle;
			obstacle->objectID = stream->ReadID();
			obstacle->time = stream->ReadFloat();
			obstacles.AddTail( obstacle );
		}
		savedPowerPercentage = stream->ReadFloat();
	}
}

/*
================
OptimizedAvoidObstacles::Create
================
*/
// FUNCTION: STBC 0x00480d60
ArtificialIntelligence *OptimizedAvoidObstacles::Create( TGStream *stream ) {
	bool ok = true;
	OptimizedAvoidObstacles *ai = new OptimizedAvoidObstacles( stream, &ok );
	if ( !ok ) {
		delete ai;
		ai = NULL;
	}
	return ai;
}
