/*
===============================================================================

	Torpedo.cpp

===============================================================================
*/

#include <string.h>

#include "Torpedo.h"
#include "DamageableObject.h"
#include "DisruptorGraphic.h"
#include "NiMath.h"
#include "NiPointLight.h"
#include "Planet.h"
#include "ShipClass.h"
#include "WeaponHitEvent.h"
#include "PyEmbed.h"
#include "SetClass.h"
#include "SetManager.h"
#include "TGEvent.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGStream.h"
#include <iostream>
#include "TGTimer.h"
#include "TGTimerManager.h"
#include "TorpedoGraphic.h"
#include "UtopiaModule.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "TGModelUtils.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0057ad80 _$E511
// FUNCTION: STBC 0x0057ada0 _$E510

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00577ec0 _$E55
// FUNCTION: STBC 0x00577ed0 _$E54
// FUNCTION: STBC 0x00577ee0 _$E316
// FUNCTION: STBC 0x00577ef0 _$E315
// FUNCTION: STBC 0x005794a0 _$E66
// FUNCTION: STBC 0x005794b0 _$E65
// FUNCTION: STBC 0x00579a70 _$E70
// FUNCTION: STBC 0x00579a80 _$E69
// FUNCTION: STBC 0x0057a250 _$E74
// FUNCTION: STBC 0x0057a260 _$E73
// FUNCTION: STBC 0x0057a3e0 _$E78
// FUNCTION: STBC 0x0057a3f0 _$E77
// FUNCTION: STBC 0x0057a580 _$E195
// FUNCTION: STBC 0x0057a590 _$E194
// FUNCTION: STBC 0x0057a5a0 _$E137
// FUNCTION: STBC 0x0057a5b0 _$E136
// FUNCTION: STBC 0x0057a5c0 _$E150
// FUNCTION: STBC 0x0057a5d0 _$E149
// FUNCTION: STBC 0x0057a5e0 _$E200
// FUNCTION: STBC 0x0057a5f0 _$E199
// FUNCTION: STBC 0x0057a600 _$E321
// FUNCTION: STBC 0x0057a610 _$E320
// FUNCTION: STBC 0x0057a620 _$E204
// FUNCTION: STBC 0x0057a630 _$E203
// FUNCTION: STBC 0x0057a640 _$E325
// FUNCTION: STBC 0x0057a650 _$E324
// FUNCTION: STBC 0x0057a660 _$E332
// FUNCTION: STBC 0x0057a670 _$E331
// FUNCTION: STBC 0x0057a680 _$E350
// FUNCTION: STBC 0x0057a690 _$E349
// FUNCTION: STBC 0x0057a6a0 _$E354
// FUNCTION: STBC 0x0057a6b0 _$E353
// FUNCTION: STBC 0x0057a6c0 _$E95
// FUNCTION: STBC 0x0057a6d0 _$E94
// FUNCTION: STBC 0x0057a6e0 _$E225
// FUNCTION: STBC 0x0057a6f0 _$E224
// FUNCTION: STBC 0x0057a700 _$E232
// FUNCTION: STBC 0x0057a710 _$E231
// FUNCTION: STBC 0x0057a720 _$E236
// FUNCTION: STBC 0x0057a730 _$E235
// FUNCTION: STBC 0x0057a740 _$E240
// FUNCTION: STBC 0x0057a750 _$E239
// FUNCTION: STBC 0x0057a760 _$E244
// FUNCTION: STBC 0x0057a770 _$E243
// FUNCTION: STBC 0x0057a780 _$E250
// FUNCTION: STBC 0x0057a790 _$E249
// FUNCTION: STBC 0x0057a7a0 _$E260
// FUNCTION: STBC 0x0057a7b0 _$E259
// FUNCTION: STBC 0x0057a7c0 _$E358
// FUNCTION: STBC 0x0057a7d0 _$E357
// FUNCTION: STBC 0x0057a7e0 _$E362
// FUNCTION: STBC 0x0057a7f0 _$E361
// FUNCTION: STBC 0x0057a800 _$E366
// FUNCTION: STBC 0x0057a810 _$E365
// FUNCTION: STBC 0x0057a820 _$E370
// FUNCTION: STBC 0x0057a830 _$E369
// FUNCTION: STBC 0x0057a840 _$E374
// FUNCTION: STBC 0x0057a850 _$E373
// FUNCTION: STBC 0x0057a860 _$E378
// FUNCTION: STBC 0x0057a870 _$E377
// FUNCTION: STBC 0x0057a880 _$E382
// FUNCTION: STBC 0x0057a890 _$E381
// FUNCTION: STBC 0x0057a8a0 _$E386
// FUNCTION: STBC 0x0057a8b0 _$E385
// FUNCTION: STBC 0x0057a8c0 _$E390
// FUNCTION: STBC 0x0057a8d0 _$E389
// FUNCTION: STBC 0x0057a8e0 _$E100
// FUNCTION: STBC 0x0057a8f0 _$E99
// FUNCTION: STBC 0x0057a900 _$E394
// FUNCTION: STBC 0x0057a910 _$E393
// FUNCTION: STBC 0x0057a920 _$E105
// FUNCTION: STBC 0x0057a930 _$E104
// FUNCTION: STBC 0x0057a940 _$E398
// FUNCTION: STBC 0x0057a950 _$E397
// FUNCTION: STBC 0x0057a960 _$E402
// FUNCTION: STBC 0x0057a970 _$E401
// FUNCTION: STBC 0x0057a980 _$E406
// FUNCTION: STBC 0x0057a990 _$E405
// FUNCTION: STBC 0x0057a9a0 _$E410
// FUNCTION: STBC 0x0057a9b0 _$E409
// FUNCTION: STBC 0x0057a9c0 _$E114
// FUNCTION: STBC 0x0057a9d0 _$E113
// FUNCTION: STBC 0x0057a9e0 _$E414
// FUNCTION: STBC 0x0057a9f0 _$E413
// FUNCTION: STBC 0x0057aa00 _$E418
// FUNCTION: STBC 0x0057aa10 _$E417
// FUNCTION: STBC 0x0057aa20 _$E422
// FUNCTION: STBC 0x0057aa30 _$E421
// FUNCTION: STBC 0x0057aa40 _$E120
// FUNCTION: STBC 0x0057aa50 _$E119
// FUNCTION: STBC 0x0057aa60 _$E125
// FUNCTION: STBC 0x0057aa70 _$E124
// FUNCTION: STBC 0x0057aa80 _$E155
// FUNCTION: STBC 0x0057aa90 _$E154
// FUNCTION: STBC 0x0057aaa0 _$E264
// FUNCTION: STBC 0x0057aab0 _$E263
// FUNCTION: STBC 0x0057aac0 _$E160
// FUNCTION: STBC 0x0057aad0 _$E159
// FUNCTION: STBC 0x0057aae0 _$E426
// FUNCTION: STBC 0x0057aaf0 _$E425
// FUNCTION: STBC 0x0057ab00 _$E430
// FUNCTION: STBC 0x0057ab10 _$E429
// FUNCTION: STBC 0x0057ab20 _$E165
// FUNCTION: STBC 0x0057ab30 _$E164
// FUNCTION: STBC 0x0057ab40 _$E434
// FUNCTION: STBC 0x0057ab50 _$E433
// FUNCTION: STBC 0x0057ab60 _$E130
// FUNCTION: STBC 0x0057ab70 _$E129
// FUNCTION: STBC 0x0057ab80 _$E438
// FUNCTION: STBC 0x0057ab90 _$E437
// FUNCTION: STBC 0x0057aba0 _$E442
// FUNCTION: STBC 0x0057abb0 _$E441
// FUNCTION: STBC 0x0057abc0 _$E170
// FUNCTION: STBC 0x0057abd0 _$E169
// FUNCTION: STBC 0x0057abe0 _$E175
// FUNCTION: STBC 0x0057abf0 _$E174
// FUNCTION: STBC 0x0057ac00 _$E446
// FUNCTION: STBC 0x0057ac10 _$E445
// FUNCTION: STBC 0x0057ac20 _$E450
// FUNCTION: STBC 0x0057ac30 _$E449
// FUNCTION: STBC 0x0057ac40 _$E454
// FUNCTION: STBC 0x0057ac50 _$E453
// FUNCTION: STBC 0x0057ac60 _$E458
// FUNCTION: STBC 0x0057ac70 _$E457
// FUNCTION: STBC 0x0057ac80 _$E462
// FUNCTION: STBC 0x0057ac90 _$E461
// FUNCTION: STBC 0x0057aca0 _$E466
// FUNCTION: STBC 0x0057acb0 _$E465
// FUNCTION: STBC 0x0057acc0 _$E470
// FUNCTION: STBC 0x0057acd0 _$E469
// FUNCTION: STBC 0x0057ace0 _$E474
// FUNCTION: STBC 0x0057acf0 _$E473
// FUNCTION: STBC 0x0057ad00 _$E180
// FUNCTION: STBC 0x0057ad10 _$E179
// FUNCTION: STBC 0x0057ad20 _$E478
// FUNCTION: STBC 0x0057ad30 _$E477
// FUNCTION: STBC 0x0057ad40 _$E185
// FUNCTION: STBC 0x0057ad50 _$E184
// FUNCTION: STBC 0x0057ad60 _$E190
// FUNCTION: STBC 0x0057ad70 _$E189
// FUNCTION: STBC 0x0057ade0 _$E482
// FUNCTION: STBC 0x0057adf0 _$E481
// FUNCTION: STBC 0x0057ae00 _$E487
// FUNCTION: STBC 0x0057ae10 _$E486
// FUNCTION: STBC 0x0057ae20 _$E491
// FUNCTION: STBC 0x0057ae30 _$E490

// Standard streams, handler tables and other file-scope statics.
// FUNCTION: STBC 0x00577e40 _$E301
// FUNCTION: STBC 0x00577e50 _$E298
// FUNCTION: STBC 0x00577e60 _$E300
// FUNCTION: STBC 0x00577e70 _$E299
// FUNCTION: STBC 0x00577e80 _$E307
// FUNCTION: STBC 0x00577e90 _$E304
// FUNCTION: STBC 0x00577ea0 _$E306
// FUNCTION: STBC 0x00577eb0 _$E305
// A pick the unit constructs at startup and destroys at exit; no code in retail
// reads it (only its initialiser 0x577F10 and exit destructor 0x577F30 name
// 0x98CBB8). File scope: the exit destructor has no guard (invented name).
// FUNCTION: STBC 0x00577f00 _$E496
// FUNCTION: STBC 0x00577f10 _$E493
// FUNCTION: STBC 0x00577f20 _$E495
// FUNCTION: STBC 0x00577f30 _$E494
// DATA: STBC 0x0098cbb8
static NiPick unusedPick;

// FUNCTION: STBC 0x00577f40 _$E502
// FUNCTION: STBC 0x00577f50 _$E498
// FUNCTION: STBC 0x00577f60 _$E501
// FUNCTION: STBC 0x00577f70 _$E499

// What a torpedo with guidance gets when it is read from a stream: a sharper turn
// and a longer guided life.
#define GUIDED_TURN_SCALE	1.5f
#define GUIDED_LIFE_SCALE	1.25f

// DATA: STBC 0x0098cb58
TGEventHandlerTable Torpedo::classHandlers;

/*
================
Torpedo::AddToSet

Adds the object to the set's list of its class type, and to those of its
base classes.
================
*/
// FUNCTION: STBC 0x00577f90
void Torpedo::AddToSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Add( this, TORPEDO_TYPE );
		PhysicsObjectClass::AddToSet( set );
	}
}

/*
================
Torpedo::RemoveFromSet
================
*/
// FUNCTION: STBC 0x00577fc0
void Torpedo::RemoveFromSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Remove( this, TORPEDO_TYPE );
		PhysicsObjectClass::RemoveFromSet( set );
	}
}

/*
================
Torpedo::GetClassObjects
================
*/
// FUNCTION: STBC 0x00577ff0
Torpedo **Torpedo::GetClassObjects( SetClass *set, int *count ) {
	if ( set != NULL ) {
		return (Torpedo **)set->objectTable.GetObjects( TORPEDO_TYPE, count );
	}
	*count = 0;
	return NULL;
}

/*
================
Torpedo::GetNumClassObjects

The number of the set's torpedoes.
================
*/
// FUNCTION: STBC 0x00578020
int Torpedo::GetNumClassObjects( SetClass *set ) {
	if ( set != NULL ) {
		return set->objectTable.GetNumObjects( TORPEDO_TYPE );
	}
	return 0;
}

/*
================
Torpedo::GetObjectByID

The torpedo with the given ID in the set, or among every object when the
set is NULL; NULL when it is none or not a Torpedo.
================
*/
// FUNCTION: STBC 0x00578040
Torpedo *Torpedo::GetObjectByID( SetClass *set, long id ) {
	if ( set == NULL ) {
		TGObject *object;
		if ( TGObject::objectTable->GetAt( id, object ) && object->IsTypeOf( TORPEDO_TYPE ) ) {
			return (Torpedo *)object;
		}
		return NULL;
	}
	return (Torpedo *)set->GetClassObject( TORPEDO_TYPE, id );
}

/*
================
Torpedo::GetObject

The torpedo of the name in the set, or in any set when the set is NULL,
the last set first.
================
*/
// FUNCTION: STBC 0x005780b0
Torpedo *Torpedo::GetObject( SetClass *set, char *name ) {
	if ( set == NULL ) {
		int count = g_kSetManager.GetNumSets();
		SetClass **sets = g_kSetManager.GetSets();
		BaseObjectClass *object = NULL;
		for ( int i = count - 1; i >= 0 && object == NULL; i-- ) {
			object = sets[i]->GetClassObject( TORPEDO_TYPE, name );
		}
		return (Torpedo *)object;
	}
	return (Torpedo *)set->GetClassObject( TORPEDO_TYPE, name );
}

/*
================
Torpedo::Cast

The object as a Torpedo; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00578110
Torpedo *Torpedo::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TORPEDO_TYPE ) ) {
		return (Torpedo *)object;
	}
	return NULL;
}

/*
================
Torpedo::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00578140
void Torpedo::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&Torpedo::HandleStartHomingEvent, "Torpedo::HandleStartHomingEvent" );
}

/*
================
Torpedo::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00578160
void Torpedo::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_TORPEDO_START_HOMING, "Torpedo::HandleStartHomingEvent" );
}

/*
================
Torpedo::Create

A new torpedo that runs the module's Create on itself, placed at the
position. With a homing delay it does not home until a timer sends it
ET_TORPEDO_START_HOMING that many seconds later.
================
*/
// FUNCTION: STBC 0x00578180
Torpedo *Torpedo::Create( char *moduleName, const TGPoint3 &position, unsigned char homingDelay ) {
	Torpedo *torpedo = new Torpedo;
	torpedo->moduleName = new char[strlen( moduleName ) + 1];
	strcpy( torpedo->moduleName, moduleName );

	PyObject *self = torpedo->VirtualSlot08();
	int result;
	PyEmbed::CallFunction( torpedo->moduleName, "Create", "i", &result, "(O)", self );
	Py_DECREF( self );

	torpedo->SetTranslate( position );
	torpedo->lastLocation = position;
	torpedo->homingDelay = homingDelay;
	if ( homingDelay > 0 ) {
		torpedo->homing = false;
		TGEvent *event = new TGEvent;
		event->SetSource( torpedo );
		event->SetDestination( torpedo );
		event->SetEventType( ET_TORPEDO_START_HOMING );
		TGTimer *timer = new TGTimer;
		timer->SetTimerStart( g_kUtopiaModule.GetGameTime() + torpedo->homingDelay );
		timer->SetEvent( event );
		g_kTimerManager.AddTimer( timer );
	}
	return torpedo;
}

/*
================
Torpedo::SetParent
================
*/
// FUNCTION: STBC 0x00578320
void Torpedo::SetParent( ObjectClass *parent ) {
	if ( parent != NULL ) {
		parentID = parent->GetObjID();
	}
}

/*
================
Torpedo::GetLaunchSpeed

The script's GetLaunchSpeed.
================
*/
// FUNCTION: STBC 0x00578340
float Torpedo::GetLaunchSpeed( void ) {
	float launchSpeed;
	PyEmbed::CallFunction( moduleName, "GetLaunchSpeed", "f", &launchSpeed, "" );
	return launchSpeed;
}

/*
================
Torpedo::GetLaunchSound

The script's GetLaunchSound; NULL for None.
================
*/
// FUNCTION: STBC 0x00578370
char *Torpedo::GetLaunchSound( void ) {
	char *launchSound;
	PyEmbed::CallFunction( moduleName, "GetLaunchSound", "z", &launchSound, "" );
	return launchSound;
}

/*
================
Torpedo::GetPowerCost

The script's GetPowerCost.
================
*/
// FUNCTION: STBC 0x005783a0
float Torpedo::GetPowerCost( void ) {
	float powerCost;
	PyEmbed::CallFunction( moduleName, "GetPowerCost", "f", &powerCost, "" );
	return powerCost;
}

/*
================
Torpedo::Torpedo

A torpedo with full guidance and no target, in collision group 2 only,
that cannot be hailed or scanned.
================
*/
// FUNCTION: STBC 0x005783d0
Torpedo::Torpedo( long id ) : PhysicsObjectClass( id ) {
	moduleName = NULL;
	maxAngularAccel = 0.125f;
	lifetime = 60.0f;
	guidanceLifetime = 4.0f;
	guidanceTimeLeft = 4.0f;
	isDisruptor = false;
	homing = true;
	collisionFlags &= ~( CFB_MEMBER_GROUP_1 | CFB_COLLIDES_WITH_GROUP_2 | CFB_MEMBER_GROUP_3 );
	targetID = 0;
	damage = 100.0f;
	lastLocation = NiPoint3::ZERO;
	unknown15C = NiPoint3::ZERO;
	playerID = 0;
	mass = 0.01f;
	payloadType = WP_TORPEDO;
	light = NULL;
	SetHailable( false );
	SetScannable( false );
}

/*
================
Torpedo::GetObjType
================
*/
// FUNCTION: STBC 0x00578530
int Torpedo::GetObjType( void ) {
	return TORPEDO_TYPE;
}

/*
================
Torpedo::IsTypeOf
================
*/
// FUNCTION: STBC 0x00578540
bool Torpedo::IsTypeOf( int type ) {
	if ( type == TORPEDO_TYPE ) {
		return true;
	}
	return PhysicsObjectClass::IsTypeOf( type );
}

/*
================
Torpedo::GetClassName
================
*/
// FUNCTION: STBC 0x005785a0
char *Torpedo::GetClassName( void ) {
	return "Torpedo";
}

/*
================
Torpedo::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x005785b0
char *Torpedo::GetSwigTypeName( void ) {
	return "_p_Torpedo";
}

/*
================
Torpedo::GetPythonClassName
================
*/
// FUNCTION: STBC 0x005785c0
char *Torpedo::GetPythonClassName( void ) {
	return "TorpedoPtr";
}

/*
================
Torpedo::GetClassHandlers
================
*/
// FUNCTION: STBC 0x005785d0
TGEventHandlerTable *Torpedo::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
Torpedo::TurnTowardOrientation
================
*/
// FUNCTION: STBC 0x005785e0
float Torpedo::TurnTowardOrientation( TGPoint3 *forward, TGPoint3 *up ) {
	NiPoint3 upCopy = *up;
	NiPoint3 forwardCopy = *forward;
	return Unknown579530( &forwardCopy, &upCopy );
}

/*
================
Torpedo::~Torpedo
================
*/
// FUNCTION: STBC 0x00578660
// FUNCTION: STBC 0x00578630 ??_GTorpedo@@UAEPAXI@Z
Torpedo::~Torpedo( void ) {
	delete[] moduleName;
}

/*
================
Torpedo::CreateDisruptorModel

Gives the torpedo a disruptor bolt's model, in the script's shell and core
colours (the SWIG wrapper Torpedo_CreateDisruptorModel).
================
*/
// FUNCTION: STBC 0x005786a0
void Torpedo::CreateDisruptorModel( TGColorA *shellColor, TGColorA *coreColor, float length, float radius, int segments,
									float coreRadiusScale, float coreLengthScale ) {
	isDisruptor = true;
	SetNiObject( new DisruptorGraphic( shellColor, coreColor, segments, length, radius, coreRadiusScale, coreLengthScale ) );
}

/*
================
Torpedo::CreateTorpedoModel

Gives the torpedo a photon-style model of core, glow and flares (the SWIG
wrapper Torpedo_CreateTorpedoModel); the model's light becomes the
torpedo's.
================
*/
// FUNCTION: STBC 0x00578730
void Torpedo::CreateTorpedoModel( char *coreTexture, TGColorA *coreColor, float coreSize, float coreRotationSpeed,
								  char *glowTexture, TGColorA *glowColor, float glowPulseRate, float glowMinScale,
								  float glowMaxScale, char *flareTexture, TGColorA *flareColor, int numFlares,
								  float flareLife, float flareLifeVariance ) {
	TorpedoGraphic *graphic = new TorpedoGraphic( coreTexture, coreColor, coreSize, coreRotationSpeed, glowTexture, glowColor,
												  glowPulseRate, glowMinScale, glowMaxScale, flareTexture, flareColor,
												  numFlares, flareLife, flareLifeVariance );
	SetNiObject( graphic );
	isDisruptor = false;
	light = graphic->GetLight();
}

/*
================
Torpedo::Unknown578800

Turns the disruptor model so that its up axis lies along the flight: the
rotation about the axis square to both, by the angle between them.
================
*/
// FUNCTION: STBC 0x00578800
void Torpedo::Unknown578800( void ) {
	NiPoint3 up = NiPoint3::UNIT_Y;
	NiNode *model = node;
	NiPoint3 direction = GetVelocity();
	direction.Unitize();
	NiPoint3 axis;
	axis = direction.Cross( up );
	axis.Unitize();
	NiMatrix3 rotation;
	rotation.MakeRotation( NiACos( direction.y ), axis );
	model->SetRotate( rotation );
}

/*
================
Torpedo::SetContainingSet

Leaving its set, the torpedo's glow stops lighting the firing object and
the target.
================
*/
// FUNCTION: STBC 0x005789b0
void Torpedo::SetContainingSet( SetClass *set ) {
	if ( set == NULL && light != NULL ) {
		if ( parentID != 0 ) {
			ObjectClass *parent = ObjectClass::GetObjectByID( GetContainingSet(), parentID );
			if ( parent != NULL ) {
				parent->RemoveLight( light );
			}
		}
		if ( targetID != 0 ) {
			ObjectClass *target = ObjectClass::GetObjectByID( GetContainingSet(), targetID );
			if ( target != NULL ) {
				target->RemoveLight( light );
			}
		}
	}
	PhysicsObjectClass::SetContainingSet( set );
}

/*
================
Torpedo::Update

Steers the torpedo while its guidance lasts, burns its lifetime down
(deleting it when none is left), turns a disruptor along its flight, and
lets its glow light the firing object and the target when they are near
enough.
================
*/
// FUNCTION: STBC 0x00578a30
// PARKED: callee-saved pushes: retail saves EBX/EDI at entry (so the no-light guard return merges into the final one), this build saves them after the guard; the clamp's 0.001 is stored through the FPU in retail
void Torpedo::Update( float time ) {
	float elapsed;
	if ( lastUpdateTime < 0.0f ) {
		elapsed = 0.001f;
	} else {
		float delta = time - lastUpdateTime;
		elapsed = delta < 0.001f ? 0.001f : delta;
	}

	lastLocation = GetWorldLocation();
	if ( usePhysics && homing && guidanceTimeLeft > 0.0f ) {
		Unknown578CB0( elapsed );
		guidanceTimeLeft -= elapsed;
		if ( guidanceTimeLeft < 0.0f ) {
			guidanceTimeLeft = 0.0f;
		}
	}

	lifetime -= elapsed;
	if ( lifetime <= 0.0f ) {
		deleteMe = true;
		PhysicsObjectClass::Update( time );
		return;
	}

	if ( isDisruptor ) {
		Unknown578800();
	}

	if ( light == NULL ) {
		PhysicsObjectClass::Update( time );
		return;
	}

	const TGPoint3 &location = GetWorldLocation();
	if ( parentID != 0 ) {
		ObjectClass *parent = ObjectClass::GetObjectByID( GetContainingSet(), parentID );
		if ( parent != NULL ) {
			NiPoint3 offset = parent->GetWorldLocation() - location;
			float range = light->GetLinearAttenuation() * 100.0f;
			if ( offset.SqrLength() < range * range ) {
				if ( !parent->IsLitBy( light ) ) {
					parent->AddLight( light );
				}
			} else {
				parent->RemoveLight( light );
			}
		}
	}
	if ( targetID != 0 ) {
		ObjectClass *target = ObjectClass::GetObjectByID( GetContainingSet(), targetID );
		if ( target != NULL ) {
			NiPoint3 offset = target->GetWorldLocation() - location;
			float range = light->GetLinearAttenuation();
			if ( offset.SqrLength() < range * range ) {
				if ( !target->IsLitBy( light ) ) {
					target->AddLight( light );
				}
			} else {
				target->RemoveLight( light );
			}
		}
	}
	PhysicsObjectClass::Update( time );
}

/*
================
Torpedo::Unknown578CB0

Turns the velocity toward the point it aims at, by at most the angle the
guidance still left allows in the elapsed time. The aim is the target's
offset point, predicted forward by the time to reach it; a cloaked ship is
aimed at where it was last seen.
================
*/
// FUNCTION: STBC 0x00578cb0
// DRAFT: calls and branches agree; the frame is 0xC larger than retail's and the x87 schedule differs in places
void Torpedo::Unknown578CB0( float elapsed ) {
	if ( elapsed > 0.0f ) {
		ObjectClass *target = ObjectClass::GetObjectByID( containingSet, targetID );
		if ( target != NULL ) {
			ShipClass *ship = ShipClass::Cast( target );
			NiPoint3 aim;
			if ( ship != NULL && ship->IsCloaked() ) {
				aim = unknown15C;
			} else {
				NiNode *node = target->GetNode();
				aim = TGModelUtils::LocalToWorldPoint( node, targetOffset );
				unknown15C = aim;
			}

			NiPoint3 position = GetWorldLocation();
			float speed = GetVelocity().Length();
			if ( speed != 0.0f ) {
				float time = ( aim - position ).Length() / speed;
				if ( time > 0.0f ) {
					PhysicsObjectClass *physical = PhysicsObjectClass::Cast( target );
					NiPoint3 predicted;
					if ( physical == NULL || ( ship != NULL && ship->IsCloaked() ) ) {
						predicted = aim;
					} else {
						predicted = physical->PredictPosition( aim, physical->GetVelocity(), physical->GetAcceleration(), time );
					}
					NiPoint3 direction = predicted - position;
					direction.Unitize();

					NiPoint3 heading = GetVelocity() * ( 1.0f / speed );
					float angle;
					NiPoint3 axis;
					GetTurnBetween( angle, axis, heading, direction );

					float limit = guidanceTimeLeft * maxAngularAccel / guidanceLifetime * elapsed;
					if ( angle > limit ) {
						angle = limit;
					} else if ( angle < -limit ) {
						angle = -limit;
					}

					NiMatrix3 rotation;
					rotation.MakeRotation( angle, axis );
					NiPoint3 velocity = speed * ( rotation * heading );
					SetVelocity( &velocity );
				}
			}
		}
	}
}

/*
================
Torpedo::DetectCollision

A live torpedo meeting an object other than the one that fired it: a
planet takes the hit through Unknown5791D0; a damageable object is tested
along the torpedo's last step, and a hit takes the damage it can. What is
left of the damage carries on to a second test along the same step; the
torpedo dies once nothing is left or the step hits again.
================
*/
// FUNCTION: STBC 0x00579010
void Torpedo::DetectCollision( BaseObjectClass *object ) {
	if ( object == NULL || deleteMe || object->IsDeleteMe() || object->GetObjID() == parentID ) {
		return;
	}
	if ( !object->IsTypeOf( DAMAGEABLEOBJECT_TYPE ) ) {
		if ( object->IsTypeOf( PLANET_TYPE ) ) {
			Unknown5791D0( (ObjectClass *)object );
		}
		return;
	}

	DamageableObject *target = (DamageableObject *)object;
	float damageLeft = damage;
	float startDamage = damage;
	NiPoint3 hitNormal;
	NiPoint3 hitPoint;
	bool hit = target->VirtualSlot80( &lastLocation, &GetWorldLocation(), &hitPoint, &hitNormal, 0 );
	for ( int i = 0; i < 2; i++ ) {
		if ( !hit || damageLeft <= 0.0f ) {
			return;
		}
		target->VirtualSlot81( &lastLocation, &GetWorldLocation(), &hitPoint, &hitNormal, &damageLeft, this,
			this, ObjectClass::GetObjectByID( GetContainingSet(), parentID ) );
		if ( damageLeft > 0.0f ) {
			SetDamage( damageLeft );
			SetDamageRadiusFactor( GetDamageRadiusFactor() * ( GetDamage() / startDamage ) );
			hit = target->VirtualSlot80( &lastLocation, &GetWorldLocation(), &hitPoint, &hitNormal, 1 );
			if ( hit ) {
				SetDeleteMe( true );
				SetHidden( true );
			}
		} else {
			SetDeleteMe( true );
			SetHidden( true );
		}
	}
}

/*
================
Torpedo::Unknown5791D0

The hit on a planet: the torpedo ends where the step it took meets the
planet's bound, or where it started when that is already inside. The planet
gets a weapon hit event with the point and the direction from the planet's
centre, in its model space, and Effects.TorpedoHullHit plays.
================
*/
// FUNCTION: STBC 0x005791d0
// PARKED: frame: instructions and calls agree; retail packs start first (its slot shared with normal at the frame's base) where this build's temp order puts normal (16 references) before start (9)
void Torpedo::Unknown5791D0( ObjectClass *planet ) {
	NiPoint3 start = lastLocation;
	NiPoint3 step = GetWorldLocation() - lastLocation;
	NiBound &bound = planet->GetWorldBound();
	float radius = bound.GetRadius();

	NiPoint3 end;
	NiPoint3 other;
	if ( ( start - bound.GetCenter() ).SqrLength() < radius * radius ) {
		end = start;
	} else if ( Unknown4570D0( GetWorldLocation(), step, bound.GetCenter(), radius, &end, &other ) == 0 ) {
		return;
	}
	SetTranslate( &end );
	SetDeleteMe( true );

	NiPoint3 point = TGModelUtils::WorldToLocalPoint( planet->GetNode(), end );
	NiPoint3 normal = point;
	normal.Unitize();

	WeaponHitEvent *event = new WeaponHitEvent;
	event->SetSource( this );
	event->SetDestination( planet );
	event->SetFiringObject( ObjectClass::GetObjectByID( containingSet, parentID ) );
	event->SetCondition( 0.0f );
	event->SetDamage( damage );
	event->SetObjectHitPoint( point );
	event->SetObjectHitNormal( normal );
	event->SetWeaponInstanceID( WeaponPayload::GetID() );
	event->SetRadius( GetDamageRadiusFactor() );

	PyObject *pyPlanet = planet->VirtualSlot08();
	PyObject *pyEvent = event->VirtualSlot08();
	PyEmbed::CallFunction( "Effects", "TorpedoHullHit", NULL, NULL, "OO", pyPlanet, pyEvent );
	Py_DECREF( pyPlanet );
	Py_DECREF( pyEvent );
	delete event;
}

/*
================
Torpedo::SetTargetName
================
*/
// FUNCTION: STBC 0x005794c0
void Torpedo::SetTargetName( char *name ) {
	ObjectClass *target = ObjectClass::GetObject( containingSet, name );
	if ( target != NULL ) {
		targetID = target->GetObjID();
	} else {
		targetID = 0;
	}
}

/*
================
Torpedo::SetTargetObject
================
*/
// FUNCTION: STBC 0x00579500
void Torpedo::SetTargetObject( ObjectClass *object ) {
	if ( object != NULL ) {
		targetID = object->GetObjID();
	} else {
		targetID = 0;
	}
}

/*
================
Torpedo::Unknown579530

Turns the torpedo toward the orientation: the turn between its frame and the
given one, as an axis whose length is the angle, which TurnTowardDifference
works toward. A turn without an axis is about the torpedo's up direction.
================
*/
// FUNCTION: STBC 0x00579530
float Torpedo::Unknown579530( NiPoint3 *forward, NiPoint3 *up ) {
	float angle;
	NiPoint3 axis;
	NiQuaternion turn = GetTurnBetweenFrames( GetWorldForward(), GetWorldUp(), *forward, *up );
	turn.ToAngleAxis( angle, axis );
	if ( axis == NiPoint3::ZERO ) {
		axis = GetWorldUp();
	}
	axis *= -angle;
	return TurnTowardDifference( &axis );
}

/*
================
Torpedo::TurnTowardDifference

Sets the angular acceleration that turns the torpedo through the difference
(an axis whose length is the angle) and stops it there, in the shortest time
its maximum angular acceleration allows: the time is bisected between 0 and
30 seconds, from 5, to within 1/16 of a second. Returns the time.
================
*/
// FUNCTION: STBC 0x00579610
float Torpedo::TurnTowardDifference( NiPoint3 *difference ) {
	float time = 5.0f;
	float low = 0.0f;
	float high = 30.0f;
	float maxSquared = maxAngularAccel * maxAngularAccel;
	NiPoint3 accel;
	NiPoint3 change;
	do {
		accel = ( *difference * 6.0f / time - GetAngularVelocity() * 4.0f ) / time;
		change = GetAngularVelocity() * -2.0f / time - accel;
		float accelSquared = accel.SqrLength();
		float changeSquared = change.SqrLength();
		if ( accelSquared > maxSquared || changeSquared > maxSquared ) {
			// too fast: try a longer time
			low = time;
			time = ( high + time ) * 0.5f;
		} else if ( accelSquared < maxSquared || changeSquared < maxSquared ) {
			// slower than it can: try a shorter time
			high = time;
			time = ( low + time ) * 0.5f;
		} else {
			break;
		}
	} while ( high - low > 0.0625f );

	accel = ( *difference * 6.0f / high - GetAngularVelocity() * 4.0f ) / high;
	change = -2.0f * GetAngularVelocity() / high - accel;
	SetLimitedAngularAccelerationLinear( accel, change, high, 1 );
	return high;
}

/*
================
Torpedo::SetLimitedAngularAccelerationLinear

Sets the angular acceleration and its linear change over a time, each
shortened to the torpedo's maximum angular acceleration.
================
*/
// FUNCTION: STBC 0x00579a90
void Torpedo::SetLimitedAngularAccelerationLinear( const NiPoint3 &acceleration_, const NiPoint3 &change, float time, int directionType ) {
	SetAngularDirectionType( directionType );
	NiPoint3 limited = acceleration_;
	NiPoint3 limitedChange = change;
	float maxSquared = maxAngularAccel * maxAngularAccel;
	if ( limited.SqrLength() > maxSquared ) {
		limited.Unitize();
		limited *= GetMaxAngularAccel();
	}
	if ( limitedChange.SqrLength() > maxSquared ) {
		limitedChange.Unitize();
		limitedChange *= GetMaxAngularAccel();
	}
	SetAngularAccelerationLinear( &limited, &limitedChange, time );
}

/*
================
Torpedo::HandleStartHomingEvent
================
*/
// FUNCTION: STBC 0x00579ca0
void Torpedo::HandleStartHomingEvent( TGEvent *event ) {
	homing = true;
	CallNextHandler( event );
}

/*
================
Torpedo::VirtualSlot69

PhysicsObjectClass's state, then the firing object, the target, the
player, the homing delay and the point aimed at, packed against the
target's radius (the offset's length without a target). Returns the
stream position.
================
*/
// FUNCTION: STBC 0x00579cc0
int Torpedo::VirtualSlot69( TGStream *stream ) {
	PhysicsObjectClass::VirtualSlot69( stream );
	stream->WriteLong( parentID );
	stream->WriteLong( targetID );
	stream->WriteChar( playerID );
	stream->WriteChar( homingDelay );
	float maxLength;
	ObjectClass *target = ObjectClass::GetObjectByID( NULL, GetTargetID() );
	if ( target != NULL ) {
		maxLength = target->GetRadius();
	} else {
		maxLength = targetOffset.Length();
	}
	stream->VirtualSlot38( targetOffset.x, targetOffset.y, targetOffset.z, maxLength );
	return stream->GetPos();
}

/*
================
Torpedo::VirtualSlot70

After the net type, Multiplayer.SpeciesToTorp's InitObject sets the
torpedo up for it; returns 0 when the call fails, else the stream
position.
================
*/
// FUNCTION: STBC 0x00579d90
int Torpedo::VirtualSlot70( TGStream *stream ) {
	PhysicsObjectClass::VirtualSlot70( stream );
	PyObject *self = VirtualSlot08();
	int result;
	int status = PyEmbed::CallFunction( "Multiplayer.SpeciesToTorp", "InitObject", "i", &result, "(Oi)", self, netType );
	Py_DECREF( self );
	if ( status == -1 ) {
		PyErr_Print();
		return 0;
	}
	return stream->GetPos();
}

/*
================
Torpedo::VirtualSlot71

Reads what VirtualSlot69 wrote: the state, the firing object, the target,
the player, the homing delay (with the timer that ends it) and the point
aimed at. A torpedo with guidance gets a sharper turn and a longer guided
life; one without is set to fly at the target's predicted position. Returns
the stream position.
================
*/
// FUNCTION: STBC 0x00579e10
// PARKED: x87 order and inline budget: retail measures the speed from a copy of the velocity before the target's velocity is read and keeps the aim vector in frame slots; this build defers the speed and keeps the vectors on the FPU (the wall of PulseWeapon::CreateNetPulse, whose aim block this repeats)
int Torpedo::VirtualSlot71( TGStream *stream ) {
	PhysicsObjectClass::VirtualSlot71( stream );
	parentID = stream->ReadLong();
	targetID = stream->ReadLong();
	playerID = stream->ReadChar();
	homingDelay = stream->ReadChar();
	if ( homingDelay > 0 ) {
		homing = false;
		TGEvent *event = new TGEvent;
		event->SetSource( this );
		event->SetDestination( this );
		event->SetEventType( ET_TORPEDO_START_HOMING );
		TGTimer *timer = new TGTimer;
		timer->SetTimerStart( g_kUtopiaModule.GetGameTime() + homingDelay );
		timer->SetEvent( event );
		g_kTimerManager.AddTimer( timer );
	}

	float maxLength;
	ObjectClass *target = ObjectClass::GetObjectByID( NULL, targetID );
	if ( target != NULL ) {
		maxLength = target->GetRadius();
		if ( maxLength <= 0.0f ) {
			maxLength = 0.01f;
		}
	} else {
		maxLength = 1.0f;
	}
	stream->VirtualSlot39( &targetOffset.x, &targetOffset.y, &targetOffset.z, maxLength );
	lastLocation = GetWorldLocation();

	if ( maxAngularAccel != 0.0f && guidanceLifetime != 0.0f ) {
		maxAngularAccel *= GUIDED_TURN_SCALE;
		guidanceLifetime *= GUIDED_LIFE_SCALE;
		guidanceTimeLeft *= GUIDED_LIFE_SCALE;
		return stream->GetPos();
	}

	ShipClass *ship = ShipClass::GetObjectByID( NULL, targetID );
	if ( ship != NULL ) {
		NiPoint3 position = GetTranslate();
		NiPoint3 targetPosition = ship->GetWorldLocation();
		NiPoint3 velocity = GetVelocity();
		float speed = velocity.Length();
		NiPoint3 targetVelocity = ship->GetVelocity();
		NiPoint3 targetAcceleration = ship->GetAcceleration();
		if ( speed > 0.0f ) {
			float time = ( targetPosition - position ).Length() / speed;
			if ( time > 0.0f ) {
				NiPoint3 predicted = ship->PredictPosition( targetPosition, targetVelocity, targetAcceleration, time );
				NiPoint3 aim = predicted - position;
				aim.Unitize();
				NiPoint3 aimedVelocity = aim * speed;
				SetVelocity( &aimedVelocity );
			}
		}
	}
	return stream->GetPos();
}

/*
================
Torpedo::VirtualSlot72

A torpedo takes no state update.
================
*/
// FUNCTION: STBC 0x0057a270
TGMessage *Torpedo::VirtualSlot72( MultiplayerPlayer *player ) {
	return NULL;
}

/*
================
Torpedo::WriteToStream
================
*/
// FUNCTION: STBC 0x0057a280 ?WriteToStream@Torpedo@@UAE_NPAVTGStream@@@Z
bool Torpedo::WriteToStream( TGStream *stream ) {
	if ( !PhysicsObjectClass::WriteToStream( stream ) ) {
		return false;
	}
	if ( !WeaponPayload::WriteToStream( stream ) ) {
		return false;
	}
	stream->VirtualSlot11( moduleName );
	stream->WriteID( targetID );
	stream->WriteFloat( targetOffset.x );
	stream->WriteFloat( targetOffset.y );
	stream->WriteFloat( targetOffset.z );
	stream->WriteID( parentID );
	stream->WriteFloat( maxAngularAccel );
	stream->WriteFloat( lifetime );
	stream->WriteFloat( guidanceLifetime );
	stream->WriteFloat( guidanceTimeLeft );
	stream->WriteFloat( damage );
	stream->WriteChar( homingDelay );
	stream->WriteInt( playerID );
	stream->WriteFloat( lastLocation.x );
	stream->WriteFloat( lastLocation.y );
	stream->WriteFloat( lastLocation.z );
	stream->WriteFloat( unknown15C.x );
	stream->WriteFloat( unknown15C.y );
	stream->WriteFloat( unknown15C.z );
	stream->WriteBool( homing );
	return true;
}

/*
================
Torpedo::ReadFromStream

Reads what WriteToStream writes, and runs the module's Create on the
torpedo again.
================
*/
// FUNCTION: STBC 0x0057a400 ?ReadFromStream@Torpedo@@UAE_NPAVTGStream@@@Z
bool Torpedo::ReadFromStream( TGStream *stream ) {
	if ( !PhysicsObjectClass::ReadFromStream( stream ) ) {
		return false;
	}
	if ( !WeaponPayload::ReadFromStream( stream ) ) {
		return false;
	}
	moduleName = stream->VirtualSlot10( -1 );
	PyObject *self = VirtualSlot08();
	int result;
	PyEmbed::CallFunction( moduleName, "Create", "i", &result, "(O)", self );
	Py_DECREF( self );

	targetID = stream->ReadID();
	targetOffset.x = stream->ReadFloat();
	targetOffset.y = stream->ReadFloat();
	targetOffset.z = stream->ReadFloat();
	parentID = stream->ReadID();
	maxAngularAccel = stream->ReadFloat();
	lifetime = stream->ReadFloat();
	guidanceLifetime = stream->ReadFloat();
	guidanceTimeLeft = stream->ReadFloat();
	damage = stream->ReadFloat();
	homingDelay = stream->ReadChar();
	playerID = stream->ReadInt();
	lastLocation.x = stream->ReadFloat();
	lastLocation.y = stream->ReadFloat();
	lastLocation.z = stream->ReadFloat();
	unknown15C.x = stream->ReadFloat();
	unknown15C.y = stream->ReadFloat();
	unknown15C.z = stream->ReadFloat();
	homing = stream->ReadBool();
	return true;
}
