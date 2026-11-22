/*
===============================================================================

	SensorSubsystem.cpp

===============================================================================
*/

#include <iostream>
#include <stdio.h>
#include <string.h>
#include "SensorSubsystem.h"
#include "SensorProperty.h"
#include "CloakingSubsystem.h"
#include "DamageableObject.h"
#include "Game.h"
#include "Nebula.h"
#include "ObjectClass.h"
#include "PhysicsObjectClass.h"
#include "PlacementObject.h"
#include "SetClass.h"
#include "ShipClass.h"
#include "TGBoolEvent.h"
#include "TGFloatEvent.h"
#include "TGObjPtrEvent.h"
#include "TGTimer.h"
#include "UtopiaModule.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGStream.h"
#include "TGTimerManager.h"
#include "TGSequence.h"
#include "TGScriptAction.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00569e70 _$E505
// FUNCTION: STBC 0x00569e90 _$E504

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00566c00 _$E80
// FUNCTION: STBC 0x00566c10 _$E79
// FUNCTION: STBC 0x00566c20 _$E307
// FUNCTION: STBC 0x00566c30 _$E306
// FUNCTION: STBC 0x00567420 _$E91
// FUNCTION: STBC 0x00567430 _$E90
// FUNCTION: STBC 0x00567860 _$E95
// FUNCTION: STBC 0x00567870 _$E94
// FUNCTION: STBC 0x00567cb0 _$E99
// FUNCTION: STBC 0x00567cc0 _$E98
// FUNCTION: STBC 0x00567fd0 _$E103
// FUNCTION: STBC 0x00567fe0 _$E102
// FUNCTION: STBC 0x00568360 _$E218
// FUNCTION: STBC 0x00568370 _$E217
// FUNCTION: STBC 0x005694f0 _$E160
// FUNCTION: STBC 0x00569500 _$E159
// FUNCTION: STBC 0x00569510 _$E173
// FUNCTION: STBC 0x00569520 _$E172
// FUNCTION: STBC 0x00569530 _$E223
// FUNCTION: STBC 0x00569540 _$E222
// FUNCTION: STBC 0x00569550 _$E312
// FUNCTION: STBC 0x00569560 _$E311
// FUNCTION: STBC 0x00569570 _$E238
// FUNCTION: STBC 0x00569580 _$E237
// FUNCTION: STBC 0x00569590 _$E316
// FUNCTION: STBC 0x005695a0 _$E315
// FUNCTION: STBC 0x005695b0 _$E323
// FUNCTION: STBC 0x005695c0 _$E322
// FUNCTION: STBC 0x005695d0 _$E343
// FUNCTION: STBC 0x005695e0 _$E342
// FUNCTION: STBC 0x005695f0 _$E347
// FUNCTION: STBC 0x00569600 _$E346
// FUNCTION: STBC 0x00569610 _$E122
// FUNCTION: STBC 0x00569620 _$E121
// FUNCTION: STBC 0x00569630 _$E254
// FUNCTION: STBC 0x00569640 _$E253
// FUNCTION: STBC 0x00569650 _$E261
// FUNCTION: STBC 0x00569660 _$E260
// FUNCTION: STBC 0x00569670 _$E265
// FUNCTION: STBC 0x00569680 _$E264
// FUNCTION: STBC 0x00569690 _$E269
// FUNCTION: STBC 0x005696a0 _$E268
// FUNCTION: STBC 0x005696b0 _$E273
// FUNCTION: STBC 0x005696c0 _$E272
// FUNCTION: STBC 0x005696d0 _$E279
// FUNCTION: STBC 0x005696e0 _$E278
// FUNCTION: STBC 0x005696f0 _$E227
// FUNCTION: STBC 0x00569700 _$E226
// FUNCTION: STBC 0x00569710 _$E231
// FUNCTION: STBC 0x00569720 _$E230
// FUNCTION: STBC 0x00569730 _$E351
// FUNCTION: STBC 0x00569740 _$E350
// FUNCTION: STBC 0x00569750 _$E355
// FUNCTION: STBC 0x00569760 _$E354
// FUNCTION: STBC 0x00569770 _$E359
// FUNCTION: STBC 0x00569780 _$E358
// FUNCTION: STBC 0x00569790 _$E363
// FUNCTION: STBC 0x005697a0 _$E362
// FUNCTION: STBC 0x005697b0 _$E367
// FUNCTION: STBC 0x005697c0 _$E366
// FUNCTION: STBC 0x005697d0 _$E371
// FUNCTION: STBC 0x005697e0 _$E370
// FUNCTION: STBC 0x005697f0 _$E375
// FUNCTION: STBC 0x00569800 _$E374
// FUNCTION: STBC 0x00569810 _$E379
// FUNCTION: STBC 0x00569820 _$E378
// FUNCTION: STBC 0x00569830 _$E127
// FUNCTION: STBC 0x00569840 _$E126
// FUNCTION: STBC 0x00569850 _$E383
// FUNCTION: STBC 0x00569860 _$E382
// FUNCTION: STBC 0x00569870 _$E132
// FUNCTION: STBC 0x00569880 _$E131
// FUNCTION: STBC 0x00569890 _$E387
// FUNCTION: STBC 0x005698a0 _$E386
// FUNCTION: STBC 0x005698b0 _$E391
// FUNCTION: STBC 0x005698c0 _$E390
// FUNCTION: STBC 0x005698d0 _$E395
// FUNCTION: STBC 0x005698e0 _$E394
// FUNCTION: STBC 0x005698f0 _$E399
// FUNCTION: STBC 0x00569900 _$E398
// FUNCTION: STBC 0x00569910 _$E137
// FUNCTION: STBC 0x00569920 _$E136
// FUNCTION: STBC 0x00569930 _$E403
// FUNCTION: STBC 0x00569940 _$E402
// FUNCTION: STBC 0x00569950 _$E407
// FUNCTION: STBC 0x00569960 _$E406
// FUNCTION: STBC 0x00569970 _$E411
// FUNCTION: STBC 0x00569980 _$E410
// FUNCTION: STBC 0x00569990 _$E143
// FUNCTION: STBC 0x005699a0 _$E142
// FUNCTION: STBC 0x005699b0 _$E148
// FUNCTION: STBC 0x005699c0 _$E147
// FUNCTION: STBC 0x005699d0 _$E178
// FUNCTION: STBC 0x005699e0 _$E177
// FUNCTION: STBC 0x00569a50 _$E415
// FUNCTION: STBC 0x00569a60 _$E414
// FUNCTION: STBC 0x00569a70 _$E183
// FUNCTION: STBC 0x00569a80 _$E182
// FUNCTION: STBC 0x00569a90 _$E419
// FUNCTION: STBC 0x00569aa0 _$E418
// FUNCTION: STBC 0x00569ab0 _$E423
// FUNCTION: STBC 0x00569ac0 _$E422
// FUNCTION: STBC 0x00569ad0 _$E188
// FUNCTION: STBC 0x00569ae0 _$E187
// FUNCTION: STBC 0x00569af0 _$E427
// FUNCTION: STBC 0x00569b00 _$E426
// FUNCTION: STBC 0x00569b10 _$E153
// FUNCTION: STBC 0x00569b20 _$E152
// FUNCTION: STBC 0x00569b70 _$E431
// FUNCTION: STBC 0x00569b80 _$E430
// FUNCTION: STBC 0x00569b90 _$E435
// FUNCTION: STBC 0x00569ba0 _$E434
// FUNCTION: STBC 0x00569bb0 _$E193
// FUNCTION: STBC 0x00569bc0 _$E192
// FUNCTION: STBC 0x00569bd0 _$E198
// FUNCTION: STBC 0x00569be0 _$E197
// FUNCTION: STBC 0x00569bf0 _$E439
// FUNCTION: STBC 0x00569c00 _$E438
// FUNCTION: STBC 0x00569c10 _$E443
// FUNCTION: STBC 0x00569c20 _$E442
// FUNCTION: STBC 0x00569c30 _$E447
// FUNCTION: STBC 0x00569c40 _$E446
// FUNCTION: STBC 0x00569c50 _$E451
// FUNCTION: STBC 0x00569c60 _$E450
// FUNCTION: STBC 0x00569c70 _$E455
// FUNCTION: STBC 0x00569c80 _$E454
// FUNCTION: STBC 0x00569c90 _$E459
// FUNCTION: STBC 0x00569ca0 _$E458
// FUNCTION: STBC 0x00569cb0 _$E463
// FUNCTION: STBC 0x00569cc0 _$E462
// FUNCTION: STBC 0x00569cd0 _$E467
// FUNCTION: STBC 0x00569ce0 _$E466
// FUNCTION: STBC 0x00569cf0 _$E203
// FUNCTION: STBC 0x00569d00 _$E202
// FUNCTION: STBC 0x00569d10 _$E471
// FUNCTION: STBC 0x00569d20 _$E470
// FUNCTION: STBC 0x00569d30 _$E208
// FUNCTION: STBC 0x00569d40 _$E207
// FUNCTION: STBC 0x00569d50 _$E213
// FUNCTION: STBC 0x00569d60 _$E212
// FUNCTION: STBC 0x00569ea0 _$E475
// FUNCTION: STBC 0x00569eb0 _$E474
// FUNCTION: STBC 0x00569ec0 _$E480
// FUNCTION: STBC 0x00569ed0 _$E479
// FUNCTION: STBC 0x00569ee0 _$E484
// FUNCTION: STBC 0x00569ef0 _$E483

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00566b80 _$E32
// FUNCTION: STBC 0x00566b90 _$E29
// FUNCTION: STBC 0x00566ba0 _$E31
// FUNCTION: STBC 0x00566bb0 _$E30
// FUNCTION: STBC 0x00566bc0 _$E38
// FUNCTION: STBC 0x00566bd0 _$E35
// FUNCTION: STBC 0x00566be0 _$E37
// FUNCTION: STBC 0x00566bf0 _$E36

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00566c40 _$E490
// FUNCTION: STBC 0x00566c50 _$E486
// FUNCTION: STBC 0x00566c60 _$E489
// FUNCTION: STBC 0x00566c70 _$E487

// The global object's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00566cc0 _$E496
// FUNCTION: STBC 0x00566cd0 _$E492
// FUNCTION: STBC 0x00566ce0 _$E495
// FUNCTION: STBC 0x00566cf0 _$E493

// DATA: STBC 0x0098c0b0
TGEventHandlerTable SensorSubsystem::classHandlers;

/*
================
SensorSubsystem::Cast

The object as a SensorSubsystem; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00566c90
SensorSubsystem *SensorSubsystem::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( SENSORSUBSYSTEM_TYPE ) ) {
		return (SensorSubsystem *)object;
	}
	return NULL;
}

// DATA: STBC 0x0098c000
SensorData SensorSubsystem::sensorData;

/*
================
SensorSubsystem::SensorSubsystem
================
*/
// FUNCTION: STBC 0x00566d10
SensorSubsystem::SensorSubsystem( long id ) : PoweredSubsystem( id ), probes( 2 ) {
	lastSensorRange = 0.0f;
	scanTimerID = 0;
	numProbes = 0;
}

/*
================
SensorSubsystem::GetObjType
================
*/
// FUNCTION: STBC 0x00566d70
int SensorSubsystem::GetObjType( void ) {
	return SENSORSUBSYSTEM_TYPE;
}

/*
================
SensorSubsystem::GetClassName
================
*/
// FUNCTION: STBC 0x00566de0
char *SensorSubsystem::GetClassName( void ) {
	return "SensorSubsystem";
}

/*
================
SensorSubsystem::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00566df0
char *SensorSubsystem::GetSwigTypeName( void ) {
	return "_p_SensorSubsystem";
}

/*
================
SensorSubsystem::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00566e00
char *SensorSubsystem::GetPythonClassName( void ) {
	return "SensorSubsystemPtr";
}

/*
================
SensorSubsystem::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00566e10
TGEventHandlerTable *SensorSubsystem::GetClassHandlers( void ) {
	return &classHandlers;
}
/*
================
SensorSubsystem::~SensorSubsystem
================
*/
// FUNCTION: STBC 0x00566e50
// FUNCTION: STBC 0x00566e20 ??_GSensorSubsystem@@UAEPAXI@Z
SensorSubsystem::~SensorSubsystem( void ) {
	probes.RemoveAll();
	if ( scanTimerID != 0 ) {
		g_kTimerManager.DeleteTimer( scanTimerID );
	}
}

/*
================
SensorSubsystem::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00566f50
void SensorSubsystem::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerFunction( SensorSubsystem::HandleSetPlayer, "SensorSubsystem::HandleSetPlayer" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&SensorSubsystem::HandleEnterSet, "SensorSubsystem::HandleEnterSet" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&SensorSubsystem::HandleExitSet, "SensorSubsystem::HandleExitSet" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&SensorSubsystem::HandleShipIdentified, "SensorSubsystem::HandleShipIdentified" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&SensorSubsystem::HandleShipDecloaked, "SensorSubsystem::HandleShipDecloaked" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&SensorSubsystem::HandlePeriodicScanEvent, "SensorSubsystem::HandlePeriodicScanEvent" );
}

/*
================
SensorSubsystem::RegisterHandlers

Besides the class's periodic scan, every change of player reaches
HandleSetPlayer.
================
*/
// FUNCTION: STBC 0x00566fd0
void SensorSubsystem::RegisterHandlers( void ) {
	g_kEventManager.AddBroadcastFuncHandler( ET_SET_PLAYER, NULL, "SensorSubsystem::HandleSetPlayer" );
	AddMethodHandlerForClass( &classHandlers, ET_SENSORS_PERIODIC_SCAN, "SensorSubsystem::HandlePeriodicScanEvent" );
}

/*
================
SensorSubsystem::AddProbe

The sensors also see what the probe's sensors see.
================
*/
// FUNCTION: STBC 0x00567010
void SensorSubsystem::AddProbe( long probeID ) {
	if ( probeID != 0 ) {
		probes.AddTail( probeID );
	}
}

/*
================
SensorSubsystem::SetProperty

Takes the number of probes from a sensor property.
================
*/
// FUNCTION: STBC 0x00567080
void SensorSubsystem::SetProperty( SubsystemProperty *property_ ) {
	ShipSubsystem::SetProperty( property_ );

	SensorProperty *sensorProperty = SensorProperty::Cast( property_ );
	if ( sensorProperty != NULL ) {
		numProbes = sensorProperty->GetMaxProbes();
	}
}

/*
================
SensorSubsystem::Update

On the player's ship, tells the ship when the sensor range changed, and
drops a target the sensors no longer see.
================
*/
// FUNCTION: STBC 0x005670b0
void SensorSubsystem::Update( float deltaTime ) {
	PoweredSubsystem::Update( deltaTime );

	float range = GetSensorRange();
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player == parentShip ) {
		if ( range != lastSensorRange ) {
			TGFloatEvent *event = new TGFloatEvent;
			event->SetSource( this );
			event->SetDestination( parentShip );
			event->SetEventType( ET_SENSORS_RANGE_CHANGED );
			event->SetFloat( range );
			g_kEventManager.AddEvent( event );
		}

		ObjectClass *target = parentShip->GetTarget();
		if ( target != NULL && !target->IsTypeOf( PLACEMENTOBJECT_TYPE ) && !IsObjectVisible( target ) ) {
			parentShip->SetTargetByID( 0 );
		}
	}
	lastSensorRange = range;
}

/*
================
SensorSubsystem::GetSensorRange

The property's range, scaled by the power setting and the sensors'
condition; none while they are disabled or off.
================
*/
// FUNCTION: STBC 0x00567190
float SensorSubsystem::GetSensorRange( void ) {
	if ( !IsDisabled() && IsOn() ) {
		return GetBaseSensorRange() * normalPowerPercentage * conditionPercentage;
	}
	return 0.0f;
}

/*
================
SensorSubsystem::GetIdentificationTime

How long the sensors take to identify an object, in seconds.
================
*/
// FUNCTION: STBC 0x005671c0
float SensorSubsystem::GetIdentificationTime( void ) {
	return IDENTIFICATION_TIME;
}

/*
================
SensorSubsystem::IsObjectVisible

Whether the sensors see the object: never while they are disabled or off,
a cloaked ship or an object in another set. Above 120% power they see
everything outside nebulae; otherwise what is in far range, what a probe
sees, and, outside nebulae, what they know.
================
*/
// FUNCTION: STBC 0x005671d0
bool SensorSubsystem::IsObjectVisible( BaseObjectClass *object ) {
	if ( IsDisabled() || !on ) {
		return false;
	}

	ShipClass *ship = ShipClass::Cast( object );
	if ( ship != NULL && ship->GetCloakingSubsystem() != NULL && ship->GetCloakingSubsystem()->IsCloaked() == true ) {
		return false;
	}

	ShipClass *parent = parentShip;
	SetClass *set = parent->GetContainingSet();
	if ( set != object->GetContainingSet() ) {
		return false;
	}

	bool inNebula = false;
	int count = 0;
	Nebula **nebulae = (Nebula **)set->objectTable.GetObjects( NEBULA_TYPE, &count );
	for ( int i = 0; i < count; i++ ) {
		inNebula = nebulae[i]->IsObjectInNebula( parent );
		if ( inNebula == true ) {
			break;
		}
		inNebula = nebulae[i]->IsObjectInNebula( ObjectClass::Cast( object ) );
		if ( inNebula == true ) {
			break;
		}
	}

	if ( normalPowerPercentage > OVERPOWERED_SENSORS && !inNebula ) {
		return true;
	}
	if ( IsObjectFar( object, false ) == true ) {
		return true;
	}

	NiTListIterator pos = probes.GetHeadPos();
	while ( pos ) {
		long probeID = probes.GetNext( pos );
		ShipClass *probeShip = ShipClass::GetObjectByID( NULL, probeID );
		if ( probeShip != NULL ) {
			if ( probeShip->GetSensorSubsystem() != NULL && probeShip->GetSensorSubsystem()->IsObjectVisible( object ) == true ) {
				return true;
			}
		} else {
			probes.Remove( probeID );
		}
	}

	if ( inNebula ) {
		return false;
	}
	return IsObjectKnown( object );
}

/*
================
SensorSubsystem::IsObjectNear

Whether the object is within half the sensor range of the ship, or near
one of its probes. For the player's ship the proximity lists answer,
unless forced to measure.
================
*/
// FUNCTION: STBC 0x00567440
bool SensorSubsystem::IsObjectNear( BaseObjectClass *object, bool measure ) {
	ShipClass *ship = parentShip;
	if ( !measure && ship == Game::GetCurrentPlayer() ) {
		return sensorData.IsNear( object );
	}

	SetClass *set = ship->GetContainingSet();
	if ( set != object->GetContainingSet() ) {
		return false;
	}

	NiPoint3 shipLocation = ship->GetWorldLocation();
	NiPoint3 objectLocation = object->GetWorldLocation();
	float distance = set->GetDistance( &shipLocation, &objectLocation );
	if ( GetSensorRange() * NEAR_RANGE_FRACTION >= distance ) {
		return true;
	}

	NiTListIterator pos = probes.GetHeadPos();
	while ( pos ) {
		long probeID = probes.GetNext( pos );
		ShipClass *probeShip = ShipClass::GetObjectByID( NULL, probeID );
		if ( probeShip != NULL ) {
			if ( probeShip->GetSensorSubsystem() != NULL && probeShip->GetSensorSubsystem()->IsObjectNear( object, false ) == true ) {
				return true;
			}
		} else {
			probes.Remove( probeID );
		}
	}
	return false;
}

/*
================
SensorSubsystem::IsObjectFar

Whether the object is within the sensor range of the ship, or of one of
its probes. For the player's ship the proximity lists answer, unless
forced to measure.
================
*/
// FUNCTION: STBC 0x00567640
bool SensorSubsystem::IsObjectFar( BaseObjectClass *object, bool measure ) {
	ShipClass *ship = parentShip;
	if ( !measure && ship == Game::GetCurrentPlayer() ) {
		return sensorData.IsInRange( object );
	}

	SetClass *set = ship->GetContainingSet();
	if ( set != object->GetContainingSet() ) {
		return false;
	}

	NiPoint3 shipLocation = ship->GetWorldLocation();
	NiPoint3 objectLocation = object->GetWorldLocation();
	float distance = set->GetDistance( &shipLocation, &objectLocation );
	if ( GetSensorRange() >= distance ) {
		return true;
	}

	NiTListIterator pos = probes.GetHeadPos();
	while ( pos ) {
		long probeID = probes.GetNext( pos );
		ShipClass *probeShip = ShipClass::GetObjectByID( NULL, probeID );
		if ( probeShip != NULL ) {
			if ( probeShip->GetSensorSubsystem() != NULL && probeShip->GetSensorSubsystem()->IsObjectFar( object, false ) == true ) {
				return true;
			}
		} else {
			probes.Remove( probeID );
		}
	}
	return false;
}

/*
================
SensorSubsystem::IsObjectKnown

Only the player's sensors know objects: the ones identified.
================
*/
// FUNCTION: STBC 0x00567830
bool SensorSubsystem::IsObjectKnown( BaseObjectClass *object ) {
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player != NULL && parentShip == player ) {
		return sensorData.IsIdentified( object );
	}
	return false;
}

/*
================
SensorSubsystem::IdentifyObject

The player's sensors start identifying the object.
================
*/
// FUNCTION: STBC 0x00567880
void SensorSubsystem::IdentifyObject( BaseObjectClass *object ) {
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player != NULL && player == parentShip ) {
		sensorData.StartIdentification( object );
	}
}

/*
================
SensorSubsystem::ForceObjectIdentified

The player's sensors identify the object at once, and the object hears
ET_SENSORS_SHIP_IDENTIFIED; the event is not saved when the object is a
damageable object without a model yet.
================
*/
// FUNCTION: STBC 0x005678b0
void SensorSubsystem::ForceObjectIdentified( BaseObjectClass *object ) {
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player != NULL && player == parentShip ) {
		TGObjPtrEvent *event = new TGObjPtrEvent;
		event->SetSource( object );
		event->SetDestination( object );
		event->SetObjPtr( object );
		event->SetEventType( ET_SENSORS_SHIP_IDENTIFIED );
		g_kEventManager.AddEvent( event );
		if ( object->IsTypeOf( DAMAGEABLEOBJECT_TYPE ) && ( (DamageableObject *)object )->GetLODModelName() == NULL ) {
			event->SetNotSaved( true );
		}
		sensorData.AddIdentified( object );
	}
}

/*
================
SensorSubsystem::ScanAllObjects

A sequence that scans the objects of the ship's set the sensors do not
know yet, one after the other: the sensors are boosted first, each scan
waits for the one before (the identification time; a second for the
first), and the sensors' power setting is restored at the end. Asteroids
are left out, unless they are ships. NULL when the ship is in no set.
================
*/
// FUNCTION: STBC 0x00567960
TGSequence *SensorSubsystem::ScanAllObjects( void ) {
	ShipClass *ship = parentShip;
	if ( ship == NULL ) {
		return NULL;
	}
	SetClass *set = ship->GetContainingSet();
	if ( set == NULL ) {
		return NULL;
	}

	int numObjects;
	BaseObjectClass **objects = set->objectTable.GetObjects( DAMAGEABLEOBJECT_TYPE, &numObjects );
	TGSequence *sequence = new TGSequence;
	long shipID = ship->GetObjID();
	long sensorsID = GetObjID();

	TGScriptAction *boost = new TGScriptAction( "Actions.ShipScriptActions", "BoostSubsystemAction" );
	boost->AddArgument( Py_BuildValue( "l", shipID ) );
	boost->AddArgument( Py_BuildValue( "l", sensorsID ) );
	sequence->AddAction( boost );
	TGAction *previous = boost;

	for ( int i = 0; i < numObjects; i++ ) {
		long objectID = objects[i]->GetObjID();
		DamageableObject *damageable = DamageableObject::Cast( objects[i] );
		if ( IsObjectKnown( objects[i] ) || objects[i] == ship ) {
			continue;
		}
		if ( damageable != NULL ) {
			char *modelName = damageable->GetLODModelName();
			if ( ( modelName == NULL || strstr( modelName, "Asteroid" ) != NULL ) && ShipClass::Cast( damageable ) == NULL ) {
				continue;
			}
		}

		TGScriptAction *scan = new TGScriptAction( "Actions.ShipScriptActions", "ScanObject" );
		scan->AddArgument( Py_BuildValue( "l", shipID ) );
		scan->AddArgument( Py_BuildValue( "l", objectID ) );
		if ( i > 0 ) {
			sequence->AddAction( scan, previous, GetIdentificationTime() );
		} else {
			sequence->AddAction( scan, previous, 1.0f );
		}
		previous = scan;
	}

	TGScriptAction *restore = new TGScriptAction( "Actions.ShipScriptActions", "BoostSubsystemAction" );
	restore->AddArgument( Py_BuildValue( "l", shipID ) );
	restore->AddArgument( Py_BuildValue( "l", sensorsID ) );
	restore->AddArgument( Py_BuildValue( "f", powerPercentageWanted ) );
	sequence->AddAction( restore, previous, 1.0f );
	return sequence;
}

/*
================
SensorSubsystem::TargetableChanged

The ship became targetable or not: the player drops it as a target when
it no longer is; when it is near and unknown to the player's sensors,
they start identifying it.
================
*/
// FUNCTION: STBC 0x00567c20
void SensorSubsystem::TargetableChanged( ShipClass *ship ) {
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player == NULL || ship == player ) {
		return;
	}

	ObjectClass *object = ObjectClass::Cast( ship );
	if ( object != NULL && !object->IsTargetable() && player->GetTarget() == ship ) {
		player->SetTarget( NULL, NULL );
	}

	SensorSubsystem *sensors = player->GetSensorSubsystem();
	if ( ship->GetContainingSet() == player->GetContainingSet() && sensors != NULL
			&& sensors->IsObjectKnown( ship ) != true && sensors->IsObjectNear( ship, false ) == true ) {
		sensorData.StartIdentification( ship );
	}
}

/*
================
SensorSubsystem::TurnOff

Switched off, the sensors want no power.
================
*/
// FUNCTION: STBC 0x00567cd0
void SensorSubsystem::TurnOff( void ) {
	PoweredSubsystem::TurnOff();
	powerWanted = 0.0f;
}

// The seconds between periodic scans. A variable, not a constant: retail reads it from
// the unit's .data, beside its strings (invented name).
// DATA: STBC 0x008e50f4
static float periodicScanInterval = 1.0f;

/*
================
SensorSubsystem::HandleSetPlayer

The player's ship changed: the sensor data forgets what it knew and its
handlers move from the old player's sensors to the new player's, whose
periodic scan is started. Without a new player there are no player sensors.
================
*/
// FUNCTION: STBC 0x00567cf0
void SensorSubsystem::HandleSetPlayer( TGEventHandlerObject *object, TGEvent *event ) {
	sensorData.ClearIdentifications();
	sensorData.ClearProximity();
	if ( sensorData.playerSensors != NULL ) {
		g_kEventManager.RemoveQueuedEvents( sensorData.playerSensors, ET_SENSORS_PERIODIC_SCAN );
		g_kEventManager.RemoveBroadcastHandler( ET_ENTERED_SET, sensorData.playerSensors, "SensorSubsystem::HandleEnterSet" );
		g_kEventManager.RemoveBroadcastHandler( ET_EXITED_SET, sensorData.playerSensors, "SensorSubsystem::HandleExitSet" );
		g_kEventManager.RemoveBroadcastHandler( ET_SENSORS_SHIP_IDENTIFIED, sensorData.playerSensors, "SensorSubsystem::HandleShipIdentified" );
		g_kEventManager.RemoveBroadcastHandler( ET_DECLOAK_COMPLETED, sensorData.playerSensors, "SensorSubsystem::HandleShipDecloaked" );
	}
	ShipClass *ship = ShipClass::Cast( ( (TGObjPtrEvent *)event )->GetObjPtr() );
	if ( ship != NULL ) {
		sensorData.playerSensors = ship->GetSensorSubsystem();
		g_kEventManager.AddBroadcastMethodHandler( ET_ENTERED_SET, sensorData.playerSensors, "SensorSubsystem::HandleEnterSet" );
		g_kEventManager.AddBroadcastMethodHandler( ET_EXITED_SET, sensorData.playerSensors, "SensorSubsystem::HandleExitSet" );
		g_kEventManager.AddBroadcastMethodHandler( ET_SENSORS_SHIP_IDENTIFIED, sensorData.playerSensors, "SensorSubsystem::HandleShipIdentified" );
		g_kEventManager.AddBroadcastMethodHandler( ET_DECLOAK_COMPLETED, sensorData.playerSensors, "SensorSubsystem::HandleShipDecloaked" );

		TGEvent *scan = new TGEvent;
		scan->SetSource( ship->GetSensorSubsystem() );
		scan->SetDestination( ship->GetSensorSubsystem() );
		scan->SetEventType( ET_SENSORS_PERIODIC_SCAN );

		TGTimer *timer = new TGTimer;
		timer->SetTimerStart( g_kUtopiaModule.GetGameTime() + periodicScanInterval );
		timer->SetEvent( scan );
		ship->GetSensorSubsystem()->scanTimerID = timer->GetObjID();
		g_kTimerManager.AddTimer( timer );
	} else {
		sensorData.playerSensors = NULL;
	}
	sensorData.ClearIdentifications();
	sensorData.ClearIdentified();
}

/*
================
SensorSubsystem::HandleEnterSet

An object entered a set: when the player is the one, its proximity lists
start over; an object in the player's set is checked against them.
================
*/
// FUNCTION: STBC 0x00567f60
void SensorSubsystem::HandleEnterSet( TGEvent *event ) {
	ObjectClass *object = ObjectClass::Cast( event->GetDestination() );
	if ( object != NULL && sensorData.playerSensors != NULL ) {
		ShipClass *player = Game::GetCurrentPlayer();
		if ( player == object ) {
			sensorData.ClearProximity();
			return;
		}
		if ( player != NULL && player->GetContainingSet() == object->GetContainingSet() && object->IsTypeOf( PHYSICSOBJECTCLASS_TYPE ) ) {
			sensorData.UpdateObject( object );
		}
	}
}

/*
================
SensorSubsystem::HandleExitSet

An object left its set: its identification stops; when the player left,
everything under way and in range is forgotten.
================
*/
// FUNCTION: STBC 0x00567ff0
void SensorSubsystem::HandleExitSet( TGEvent *event ) {
	Game *game = Game::GetCurrentGame();
	if ( game != NULL ) {
		ShipClass *player = ShipClass::Cast( game->GetPlayer() );
		if ( player != NULL ) {
			sensorData.CancelIdentification( event->GetDestination() );
		}
		if ( event->GetDestination() == player ) {
			sensorData.ClearIdentifications();
			sensorData.ClearProximity();
		}
	}
}

/*
================
SensorSubsystem::HandleShipDecloaked

A ship decloaked: in near range, the sensors start identifying it.
================
*/
// FUNCTION: STBC 0x00568040
void SensorSubsystem::HandleShipDecloaked( TGEvent *event ) {
	ObjectClass *object = ObjectClass::Cast( event->GetDestination() );
	if ( IsObjectNear( object, false ) == true ) {
		sensorData.StartIdentification( object );
	}
	CallNextHandler( event );
}

/*
================
SensorSubsystem::HandleShipIdentified

An identification's timer ran out: the object is identified.
================
*/
// FUNCTION: STBC 0x00568080
void SensorSubsystem::HandleShipIdentified( TGEvent *event ) {
	TGObject *object = ( (TGObjPtrEvent *)event )->GetObjPtr();
	sensorData.EndIdentification( object );
	sensorData.AddIdentified( object );
	CallNextHandler( event );
}

/*
================
SensorSubsystem::HandlePeriodicScanEvent

Updates the player's proximity lists and schedules the next scan.
================
*/
// FUNCTION: STBC 0x005680c0
void SensorSubsystem::HandlePeriodicScanEvent( TGEvent *event ) {
	sensorData.UpdateObjects();

	TGEvent *scan = new TGEvent;
	scan->SetSource( this );
	scan->SetDestination( this );
	scan->SetEventType( ET_SENSORS_PERIODIC_SCAN );

	TGTimer *timer = new TGTimer;
	timer->SetTimerStart( g_kUtopiaModule.GetGameTime() + periodicScanInterval );
	timer->SetEvent( scan );
	scanTimerID = timer->GetObjID();
	g_kTimerManager.AddTimer( timer );

	CallNextHandler( event );
}

/*
================
SensorSubsystem::WriteToStream
================
*/
// FUNCTION: STBC 0x00568190
bool SensorSubsystem::WriteToStream( TGStream *stream ) {
	if ( !PoweredSubsystem::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteFloat( lastSensorRange );
	stream->WriteInt( probes.GetSize() );
	NiTListIterator pos = probes.GetHeadPos();
	while ( pos ) {
		stream->WriteID( probes.GetNext( pos ) );
	}
	stream->WriteID( scanTimerID );
	stream->WriteInt( numProbes );
	return true;
}

/*
================
SensorSubsystem::ReadFromStream
================
*/
// FUNCTION: STBC 0x00568210
bool SensorSubsystem::ReadFromStream( TGStream *stream ) {
	if ( !PoweredSubsystem::ReadFromStream( stream ) ) {
		return false;
	}

	lastSensorRange = stream->ReadFloat();
	probes.RemoveAll();
	int count = stream->ReadInt();
	for ( int i = 0; i < count; i++ ) {
		probes.AddTail( stream->ReadID() );
	}
	scanTimerID = stream->ReadID();
	numProbes = stream->ReadInt();
	return true;
}

/*
================
SensorSubsystem::WriteStatics
================
*/
// FUNCTION: STBC 0x00568320
bool SensorSubsystem::WriteStatics( TGStream *stream ) {
	return sensorData.Write( stream ) ? true : false;
}

/*
================
SensorSubsystem::ReadStatics
================
*/
// FUNCTION: STBC 0x00568340
bool SensorSubsystem::ReadStatics( TGStream *stream ) {
	return sensorData.Read( stream ) ? true : false;
}

/*
================
SensorSubsystem::CanFixUpStatics
================
*/
// FUNCTION: STBC 0x00568380
bool SensorSubsystem::CanFixUpStatics( void ) {
	return sensorData.CanFixUp() ? true : false;
}

/*
================
SensorSubsystem::FixUpStatics
================
*/
// FUNCTION: STBC 0x00568390
void SensorSubsystem::FixUpStatics( void ) {
	sensorData.FixUp();
}

/*
================
SensorData::SensorData
================
*/
// FUNCTION: STBC 0x005683a0
SensorData::SensorData( void ) : identifiedObjects( 2 ), identifications( 2 ), farObjects( 2 ), nearObjects( 2 ) {
	playerSensors = NULL;
	unpowered = true;
}

/*
================
SensorData::~SensorData
================
*/
// FUNCTION: STBC 0x00568400
SensorData::~SensorData( void ) {
	ClearIdentifications();
	ClearIdentified();
	ClearProximity();
}

/*
================
SensorData::UpdateObjects

Checks every physics object in the player's set against the ranges.
The first scan after the sensors lose their power is skipped; the
following ones run as usual.
================
*/
// FUNCTION: STBC 0x00568520
void SensorData::UpdateObjects( void ) {
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player == NULL ) {
		return;
	}
	SensorSubsystem *sensors = player->GetSensorSubsystem();
	SetClass *set = player->GetContainingSet();
	if ( sensors == NULL || set == NULL ) {
		return;
	}

	if ( sensors->GetPowerReceived() <= 0.0f ) {
		bool wasUnpowered = unpowered;
		unpowered = true;
		if ( !wasUnpowered ) {
			return;
		}
	} else {
		unpowered = false;
	}

	int count;
	BaseObjectClass **objects = set->objectTable.GetObjects( PHYSICSOBJECTCLASS_TYPE, &count );
	for ( int i = 0; i < count; i++ ) {
		if ( objects[i] != player ) {
			UpdateObject( objects[i] );
		}
	}
}

/*
================
SensorData::UpdateObject

Moves the object between the player's near and far ranges, telling the
player of each change, and starts identifying an unknown object that came
near.
================
*/
// FUNCTION: STBC 0x005685b0
// PARKED: register residue: retail keeps the object in edi and this in ebx after the identification check (ours swaps them), which stops its tail merge of the far-range leave event; calls and branches match
void SensorData::UpdateObject( BaseObjectClass *object ) {
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player == NULL ) {
		return;
	}
	SensorSubsystem *sensors = player->GetSensorSubsystem();
	if ( sensors == NULL || player->GetContainingSet() == NULL ) {
		return;
	}

	NiTListIterator farPos = farObjects.FindPos( object->GetObjID() );
	NiTListIterator nearPos = nearObjects.FindPos( object->GetObjID() );
	bool isNear = sensors->IsObjectNear( object, true );
	bool isFar = sensors->IsObjectFar( object, true );

	if ( !sensors->IsObjectKnown( object ) && isNear == true && !IsIdentifying( object ) ) {
		StartIdentification( object );
	}

	if ( isNear == true && nearPos == NULL ) {
		nearObjects.AddTail( object->GetObjID() );
		TGBoolEvent *enterEvent = new TGBoolEvent;
		enterEvent->SetSource( object );
		enterEvent->SetDestination( player );
		enterEvent->SetBool( true );
		enterEvent->SetEventType( ET_SENSORS_SHIP_NEAR_PROXIMITY );
		g_kEventManager.AddEvent( enterEvent );
		if ( farPos != NULL ) {
			farObjects.RemovePos( farPos );
			TGBoolEvent *leaveEvent = new TGBoolEvent;
			leaveEvent->SetSource( object );
			leaveEvent->SetDestination( player );
			leaveEvent->SetEventType( ET_SENSORS_SHIP_FAR_PROXIMITY );
			leaveEvent->SetBool( false );
			g_kEventManager.AddEvent( leaveEvent );
		}
	} else if ( isFar == true && farPos == NULL ) {
		if ( nearPos == NULL ) {
			farObjects.AddTail( object->GetObjID() );
			TGBoolEvent *event = new TGBoolEvent;
			event->SetSource( object );
			event->SetDestination( player );
			event->SetBool( true );
			event->SetEventType( ET_SENSORS_SHIP_FAR_PROXIMITY );
			g_kEventManager.AddEvent( event );
		}
	} else if ( ( nearPos != NULL || farPos != NULL ) && !isFar ) {
		if ( nearPos != NULL ) {
			nearObjects.RemovePos( nearPos );
			TGBoolEvent *event = new TGBoolEvent;
			event->SetSource( object );
			event->SetDestination( player );
			event->SetBool( false );
			event->SetEventType( ET_SENSORS_SHIP_NEAR_PROXIMITY );
			g_kEventManager.AddEvent( event );
		}
		if ( farPos != NULL ) {
			farObjects.RemovePos( farPos );
			TGBoolEvent *event = new TGBoolEvent;
			event->SetSource( object );
			event->SetDestination( player );
			event->SetEventType( ET_SENSORS_SHIP_FAR_PROXIMITY );
			event->SetBool( false );
			g_kEventManager.AddEvent( event );
		}
	}
}

/*
================
SensorData::ClearProximity
================
*/
// FUNCTION: STBC 0x005689d0
void SensorData::ClearProximity( void ) {
	farObjects.RemoveAll();
	nearObjects.RemoveAll();
}

/*
================
SensorData::IsNear
================
*/
// FUNCTION: STBC 0x00568a50
bool SensorData::IsNear( BaseObjectClass *object ) {
	return nearObjects.FindPos( object->GetObjID() ) ? true : false;
}

/*
================
SensorData::IsInRange

In far or in near range.
================
*/
// FUNCTION: STBC 0x00568a90
bool SensorData::IsInRange( BaseObjectClass *object ) {
	if ( farObjects.FindPos( object->GetObjID() ) != NULL ) {
		return true;
	}
	return IsNear( object );
}

/*
================
SensorData::StartIdentification

Schedules ET_SENSORS_SHIP_IDENTIFIED for the object after the player's
identification time.
================
*/
// FUNCTION: STBC 0x00568ad0
void SensorData::StartIdentification( BaseObjectClass *object ) {
	TGObjPtrEvent *event = new TGObjPtrEvent;
	event->SetSource( object );
	event->SetDestination( object );
	event->SetObjPtr( object );
	event->SetEventType( ET_SENSORS_SHIP_IDENTIFIED );

	TGTimer *timer = new TGTimer;
	timer->SetTimerStart( g_kUtopiaModule.GetGameTime() + playerSensors->GetIdentificationTime() );
	timer->SetEvent( event );

	SensorIdentification *identification = new SensorIdentification;
	identification->object = object;
	identification->timerID = timer->GetObjID();
	identifications.AddTail( identification );

	g_kTimerManager.AddTimer( timer );
}

/*
================
SensorData::EndIdentification

Forgets the object's identification; its timer has run.
================
*/
// FUNCTION: STBC 0x00568c00
void SensorData::EndIdentification( TGObject *object ) {
	NiTListIterator pos = identifications.GetHeadPos();
	while ( pos ) {
		SensorIdentification *identification = identifications.GetNext( pos );
		if ( identification != NULL && identification->object == object ) {
			identifications.Remove( identification );
			delete identification;
			return;
		}
	}
}

/*
================
SensorData::ClearIdentified
================
*/
// FUNCTION: STBC 0x00568d00
void SensorData::ClearIdentified( void ) {
	identifiedObjects.RemoveAll();
}

/*
================
SensorData::CancelIdentification

Stops the object's identification and deletes its timer.
================
*/
// FUNCTION: STBC 0x00568d40
void SensorData::CancelIdentification( TGObject *object ) {
	NiTListIterator pos = identifications.GetHeadPos();
	while ( pos ) {
		SensorIdentification *identification = identifications.GetNext( pos );
		if ( identification != NULL && identification->object == object ) {
			identifications.Remove( identification );
			g_kTimerManager.DeleteTimer( identification->timerID );
			delete identification;
			return;
		}
	}
}

/*
================
SensorData::ClearIdentifications

Stops every identification and deletes the timers.
================
*/
// FUNCTION: STBC 0x00568e40
void SensorData::ClearIdentifications( void ) {
	NiTListIterator pos = identifications.GetHeadPos();
	while ( pos ) {
		SensorIdentification *identification = identifications.GetNext( pos );
		if ( identification != NULL ) {
			g_kTimerManager.DeleteTimer( identification->timerID );
			delete identification;
		}
	}
	identifications.RemoveAll();
}

/*
================
SensorData::AddIdentified
================
*/
// FUNCTION: STBC 0x00568eb0
void SensorData::AddIdentified( TGObject *object ) {
	long id = object->GetObjID();
	if ( identifiedObjects.FindPos( id ) == NULL ) {
		identifiedObjects.AddTail( id );
	}
}

/*
================
SensorData::IsIdentified
================
*/
// FUNCTION: STBC 0x00568f40
bool SensorData::IsIdentified( BaseObjectClass *object ) {
	return identifiedObjects.FindPos( object->GetObjID() ) ? true : false;
}

/*
================
SensorData::IsIdentifying
================
*/
// FUNCTION: STBC 0x00568f80
bool SensorData::IsIdentifying( BaseObjectClass *object ) {
	NiTListIterator pos = identifications.GetHeadPos();
	while ( pos ) {
		if ( identifications.GetNext( pos )->object == object ) {
			return true;
		}
	}
	return false;
}

/*
================
SensorData::Write

Saves the identifications whose event is saved and whose object is a
damageable object with a model, then the lists.
================
*/
// FUNCTION: STBC 0x00568fb0
bool SensorData::Write( TGStream *stream ) {
	char text[256];

	if ( playerSensors != NULL ) {
		stream->WriteID( playerSensors->GetObjID() );
	} else {
		stream->WriteID( 0 );
	}

	NiTListIterator pos = identifications.GetHeadPos();
	while ( pos ) {
		SensorIdentification *identification = identifications.GetNext( pos );
		TGTimer *timer = TGTimer::GetTGTimerPtr( identification->timerID );
		if ( timer != NULL && !timer->GetEvent()->IsNotSaved() && identification->object != NULL
				&& identification->object->IsTypeOf( DAMAGEABLEOBJECT_TYPE )
				&& ( (DamageableObject *)identification->object )->GetLODModelName() != NULL ) {
			stream->WriteBool( true );
			sprintf( text, "Saving sensor data desiring timer %d on object %d\n", identification->timerID, TGObject::GetSavedID( identification->object ) );
			TGObject::Log( text );
			stream->WriteID( identification->timerID );
			if ( identification->object != NULL ) {
				stream->WriteID( identification->object->GetObjID() );
			} else {
				stream->WriteID( 0 );
			}
		}
	}
	stream->WriteBool( false );

	stream->WriteInt( identifiedObjects.GetSize() );
	pos = identifiedObjects.GetHeadPos();
	while ( pos ) {
		stream->WriteID( identifiedObjects.GetNext( pos ) );
	}
	stream->WriteInt( farObjects.GetSize() );
	pos = farObjects.GetHeadPos();
	while ( pos ) {
		stream->WriteID( farObjects.GetNext( pos ) );
	}
	stream->WriteInt( nearObjects.GetSize() );
	pos = nearObjects.GetHeadPos();
	while ( pos ) {
		stream->WriteID( nearObjects.GetNext( pos ) );
	}
	stream->WriteBool( unpowered );
	return true;
}

/*
================
SensorData::Read
================
*/
// FUNCTION: STBC 0x00569140
bool SensorData::Read( TGStream *stream ) {
	char text[256];
	int count;
	int i;

	playerSensors = (SensorSubsystem *)stream->ReadID();

	identifications.RemoveAll();
	while ( stream->ReadBool() ) {
		SensorIdentification *identification = new SensorIdentification;
		identification->timerID = stream->ReadID();
		identification->object = (BaseObjectClass *)stream->ReadID();
		sprintf( text, "Loading sensor data desiring timer %d on object %d\n", identification->timerID, identification->object );
		TGObject::Log( text );
		identifications.AddTail( identification );
	}

	count = stream->ReadInt();
	identifiedObjects.RemoveAll();
	for ( i = 0; i < count; i++ ) {
		identifiedObjects.AddTail( stream->ReadID() );
	}
	count = stream->ReadInt();
	farObjects.RemoveAll();
	for ( i = 0; i < count; i++ ) {
		farObjects.AddTail( stream->ReadID() );
	}
	count = stream->ReadInt();
	nearObjects.RemoveAll();
	for ( i = 0; i < count; i++ ) {
		nearObjects.AddTail( stream->ReadID() );
	}
	unpowered = stream->ReadBool();
	return true;
}

/*
================
SensorData::CanFixUp

Whether every object Read named has been loaded.
================
*/
// FUNCTION: STBC 0x00569440
bool SensorData::CanFixUp( void ) {
	bool loaded = true;
	if ( playerSensors != NULL && TGObject::GetTGObjectPtr( (long)playerSensors ) == NULL ) {
		return false;
	}
	NiTListIterator pos = identifications.GetHeadPos();
	while ( pos && loaded == true ) {
		SensorIdentification *identification = identifications.GetNext( pos );
		if ( identification->object != NULL && TGObject::GetTGObjectPtr( (long)identification->object ) == NULL ) {
			loaded = false;
		}
	}
	return loaded;
}

/*
================
SensorData::FixUp

Turns the IDs Read stored into pointers.
================
*/
// FUNCTION: STBC 0x005694a0
void SensorData::FixUp( void ) {
	playerSensors = (SensorSubsystem *)TGObject::GetSavedObject( (long)playerSensors );
	NiTListIterator pos = identifications.GetHeadPos();
	while ( pos ) {
		SensorIdentification *identification = identifications.GetNext( pos );
		identification->object = (BaseObjectClass *)TGObject::GetSavedObject( (long)identification->object );
	}
}
