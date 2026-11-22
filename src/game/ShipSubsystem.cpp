/*
===============================================================================

	ShipSubsystem.cpp

===============================================================================
*/

#include <iostream>
#include "ShipSubsystem.h"
#include "ShipClass.h"
#include "ShipProperty.h"
#include "SubsystemProperty.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGFloatEvent.h"
#include "TGLocalizationDatabase.h"
#include "TGLocalizationManager.h"
#include "TGModelUtils.h"
#include "TGObjPtrEvent.h"
#include "TGStream.h"
#include "UtopiaModule.h"
#include "Weapon.h"

#include <stdlib.h>
#include "NiMain.h"
#include "NiListener.h"
#include "NiMath.h"
#include "NiPick.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0056dca0 _$E478
// FUNCTION: STBC 0x0056dcc0 _$E477

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0056b820 _$E32
// FUNCTION: STBC 0x0056b830 _$E29
// FUNCTION: STBC 0x0056b840 _$E31
// FUNCTION: STBC 0x0056b850 _$E30
// FUNCTION: STBC 0x0056b860 _$E38
// FUNCTION: STBC 0x0056b870 _$E35
// FUNCTION: STBC 0x0056b880 _$E37
// FUNCTION: STBC 0x0056b890 _$E36

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x0056b8a0 _$E469
// FUNCTION: STBC 0x0056b8b0 _$E465
// FUNCTION: STBC 0x0056b8c0 _$E468
// FUNCTION: STBC 0x0056b8d0 _$E466

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0056c030 _$E72
// FUNCTION: STBC 0x0056c040 _$E71
// FUNCTION: STBC 0x0056c110 _$E278
// FUNCTION: STBC 0x0056c120 _$E277
// FUNCTION: STBC 0x0056d420 _$E83
// FUNCTION: STBC 0x0056d430 _$E82
// FUNCTION: STBC 0x0056d440 _$E87
// FUNCTION: STBC 0x0056d450 _$E86
// FUNCTION: STBC 0x0056d460 _$E91
// FUNCTION: STBC 0x0056d470 _$E90
// FUNCTION: STBC 0x0056d480 _$E95
// FUNCTION: STBC 0x0056d490 _$E94
// FUNCTION: STBC 0x0056d4a0 _$E212
// FUNCTION: STBC 0x0056d4b0 _$E211
// FUNCTION: STBC 0x0056d4c0 _$E154
// FUNCTION: STBC 0x0056d4d0 _$E153
// FUNCTION: STBC 0x0056d4e0 _$E167
// FUNCTION: STBC 0x0056d4f0 _$E166
// FUNCTION: STBC 0x0056d500 _$E217
// FUNCTION: STBC 0x0056d510 _$E216
// FUNCTION: STBC 0x0056d520 _$E283
// FUNCTION: STBC 0x0056d530 _$E282
// FUNCTION: STBC 0x0056d540 _$E221
// FUNCTION: STBC 0x0056d550 _$E220
// FUNCTION: STBC 0x0056d560 _$E287
// FUNCTION: STBC 0x0056d570 _$E286
// FUNCTION: STBC 0x0056d580 _$E294
// FUNCTION: STBC 0x0056d590 _$E293
// FUNCTION: STBC 0x0056d5a0 _$E314
// FUNCTION: STBC 0x0056d5b0 _$E313
// FUNCTION: STBC 0x0056d5c0 _$E318
// FUNCTION: STBC 0x0056d5d0 _$E317
// FUNCTION: STBC 0x0056d5e0 _$E112
// FUNCTION: STBC 0x0056d5f0 _$E111
// FUNCTION: STBC 0x0056d600 _$E240
// FUNCTION: STBC 0x0056d610 _$E239
// FUNCTION: STBC 0x0056d620 _$E247
// FUNCTION: STBC 0x0056d630 _$E246
// FUNCTION: STBC 0x0056d640 _$E251
// FUNCTION: STBC 0x0056d650 _$E250
// FUNCTION: STBC 0x0056d660 _$E255
// FUNCTION: STBC 0x0056d670 _$E254
// FUNCTION: STBC 0x0056d680 _$E259
// FUNCTION: STBC 0x0056d690 _$E258
// FUNCTION: STBC 0x0056d6a0 _$E265
// FUNCTION: STBC 0x0056d6b0 _$E264
// FUNCTION: STBC 0x0056d6c0 _$E322
// FUNCTION: STBC 0x0056d6d0 _$E321
// FUNCTION: STBC 0x0056d6e0 _$E326
// FUNCTION: STBC 0x0056d6f0 _$E325
// FUNCTION: STBC 0x0056d700 _$E330
// FUNCTION: STBC 0x0056d710 _$E329
// FUNCTION: STBC 0x0056d720 _$E334
// FUNCTION: STBC 0x0056d730 _$E333
// FUNCTION: STBC 0x0056d740 _$E338
// FUNCTION: STBC 0x0056d750 _$E337
// FUNCTION: STBC 0x0056d760 _$E342
// FUNCTION: STBC 0x0056d770 _$E341
// FUNCTION: STBC 0x0056d780 _$E346
// FUNCTION: STBC 0x0056d790 _$E345
// FUNCTION: STBC 0x0056d7a0 _$E350
// FUNCTION: STBC 0x0056d7b0 _$E349
// FUNCTION: STBC 0x0056d7c0 _$E354
// FUNCTION: STBC 0x0056d7d0 _$E353
// FUNCTION: STBC 0x0056d7e0 _$E358
// FUNCTION: STBC 0x0056d7f0 _$E357
// FUNCTION: STBC 0x0056d800 _$E117
// FUNCTION: STBC 0x0056d810 _$E116
// FUNCTION: STBC 0x0056d820 _$E362
// FUNCTION: STBC 0x0056d830 _$E361
// FUNCTION: STBC 0x0056d840 _$E122
// FUNCTION: STBC 0x0056d850 _$E121
// FUNCTION: STBC 0x0056d860 _$E366
// FUNCTION: STBC 0x0056d870 _$E365
// FUNCTION: STBC 0x0056d880 _$E370
// FUNCTION: STBC 0x0056d890 _$E369
// FUNCTION: STBC 0x0056d8a0 _$E374
// FUNCTION: STBC 0x0056d8b0 _$E373
// FUNCTION: STBC 0x0056d8c0 _$E378
// FUNCTION: STBC 0x0056d8d0 _$E377
// FUNCTION: STBC 0x0056d8e0 _$E131
// FUNCTION: STBC 0x0056d8f0 _$E130
// FUNCTION: STBC 0x0056d900 _$E382
// FUNCTION: STBC 0x0056d910 _$E381
// FUNCTION: STBC 0x0056d920 _$E386
// FUNCTION: STBC 0x0056d930 _$E385
// FUNCTION: STBC 0x0056d940 _$E390
// FUNCTION: STBC 0x0056d950 _$E389
// FUNCTION: STBC 0x0056d960 _$E137
// FUNCTION: STBC 0x0056d970 _$E136
// FUNCTION: STBC 0x0056d980 _$E142
// FUNCTION: STBC 0x0056d990 _$E141
// FUNCTION: STBC 0x0056d9a0 _$E172
// FUNCTION: STBC 0x0056d9b0 _$E171
// FUNCTION: STBC 0x0056d9c0 _$E394
// FUNCTION: STBC 0x0056d9d0 _$E393
// FUNCTION: STBC 0x0056d9e0 _$E177
// FUNCTION: STBC 0x0056d9f0 _$E176
// FUNCTION: STBC 0x0056da00 _$E398
// FUNCTION: STBC 0x0056da10 _$E397
// FUNCTION: STBC 0x0056da20 _$E402
// FUNCTION: STBC 0x0056da30 _$E401
// FUNCTION: STBC 0x0056da40 _$E182
// FUNCTION: STBC 0x0056da50 _$E181
// FUNCTION: STBC 0x0056da60 _$E406
// FUNCTION: STBC 0x0056da70 _$E405
// FUNCTION: STBC 0x0056da80 _$E147
// FUNCTION: STBC 0x0056da90 _$E146
// FUNCTION: STBC 0x0056daa0 _$E410
// FUNCTION: STBC 0x0056dab0 _$E409
// FUNCTION: STBC 0x0056dac0 _$E414
// FUNCTION: STBC 0x0056dad0 _$E413
// FUNCTION: STBC 0x0056dae0 _$E187
// FUNCTION: STBC 0x0056daf0 _$E186
// FUNCTION: STBC 0x0056db00 _$E192
// FUNCTION: STBC 0x0056db10 _$E191
// FUNCTION: STBC 0x0056db20 _$E418
// FUNCTION: STBC 0x0056db30 _$E417
// FUNCTION: STBC 0x0056db40 _$E422
// FUNCTION: STBC 0x0056db50 _$E421
// FUNCTION: STBC 0x0056db60 _$E426
// FUNCTION: STBC 0x0056db70 _$E425
// FUNCTION: STBC 0x0056db80 _$E430
// FUNCTION: STBC 0x0056db90 _$E429
// FUNCTION: STBC 0x0056dba0 _$E434
// FUNCTION: STBC 0x0056dbb0 _$E433
// FUNCTION: STBC 0x0056dbc0 _$E438
// FUNCTION: STBC 0x0056dbd0 _$E437
// FUNCTION: STBC 0x0056dbe0 _$E442
// FUNCTION: STBC 0x0056dbf0 _$E441
// FUNCTION: STBC 0x0056dc00 _$E446
// FUNCTION: STBC 0x0056dc10 _$E445
// FUNCTION: STBC 0x0056dc20 _$E197
// FUNCTION: STBC 0x0056dc30 _$E196
// FUNCTION: STBC 0x0056dc40 _$E450
// FUNCTION: STBC 0x0056dc50 _$E449
// FUNCTION: STBC 0x0056dc60 _$E202
// FUNCTION: STBC 0x0056dc70 _$E201
// FUNCTION: STBC 0x0056dc80 _$E207
// FUNCTION: STBC 0x0056dc90 _$E206
// FUNCTION: STBC 0x0056dcd0 _$E454
// FUNCTION: STBC 0x0056dce0 _$E453
// FUNCTION: STBC 0x0056dcf0 _$E459
// FUNCTION: STBC 0x0056dd00 _$E458
// FUNCTION: STBC 0x0056dd10 _$E463
// FUNCTION: STBC 0x0056dd20 _$E462

// DATA: STBC 0x0098c318
TGEventHandlerTable ShipSubsystem::classHandlers;

// DATA: STBC 0x00892fc0
const float ShipSubsystem::UPDATE_INTERVAL = 0.33f;

/*
================
ShipSubsystem::Cast

The object as a ShipSubsystem; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0056b8f0
ShipSubsystem *ShipSubsystem::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( SHIPSUBSYSTEM_TYPE ) ) {
		return (ShipSubsystem *)object;
	}
	return NULL;
}

/*
================
ShipSubsystem::GetPosition

The property's position, in the model's frame.
================
*/
// FUNCTION: STBC 0x0056b920
NiPoint3 ShipSubsystem::GetPosition( void ) {
	return property->GetPosition();
}

/*
================
ShipSubsystem::GetRadius
================
*/
// FUNCTION: STBC 0x0056b940
float ShipSubsystem::GetRadius( void ) {
	return property->GetRadius();
}

/*
================
ShipSubsystem::GetRepairComplexity
================
*/
// FUNCTION: STBC 0x0056b950
float ShipSubsystem::GetRepairComplexity( void ) {
	return property->GetRepairComplexity();
}

/*
================
ShipSubsystem::GetDisabledPercentage
================
*/
// FUNCTION: STBC 0x0056b960
float ShipSubsystem::GetDisabledPercentage( void ) {
	return property->GetDisabledPercentage();
}

/*
================
ShipSubsystem::ShipSubsystem

A subsystem at full condition, without a property, a ship or children.
In multiplayer a subsystem created without an ID takes the next client ID.
================
*/
// FUNCTION: STBC 0x0056b970
ShipSubsystem::ShipSubsystem( long id ) : TGEventHandlerObject( id ) {
	unknown28 = 0xFFFF;
	unknown2A = 0xFFFF;
	unknown2C = 0xFFFF;
	condition = 1.0f;
	conditionPercentage = 1.0f;
	combinedConditionPercentage = 1.0f;
	parentShip = NULL;
	property = NULL;
	lastUpdateLength = 0.001f;
	name = NULL;
	invincible = false;
	disabled = false;
	unknown48 = rand() * UPDATE_INTERVAL * ( 1.0f / RAND_MAX );
	unknown84 = 0.0f;
	numChildSubsystems = 0;
	childSubsystems = NULL;
	parentSubsystem = NULL;
	disabledCheck = FloatRangeWatcher::RID_INVALID;
	operationalCheck = FloatRangeWatcher::RID_INVALID;
	destroyedCheck = FloatRangeWatcher::RID_INVALID;
	rebuiltCheck = FloatRangeWatcher::RID_INVALID;
	completelyDestroyedCheck = FloatRangeWatcher::RID_INVALID;
	conditionPercentage = 1.0f;
	conditionWatcher.SetVariable( &conditionPercentage );
	combinedPercentageWatcher.SetVariable( &combinedConditionPercentage );
	damagePoint = NiPoint3::ZERO;

	if ( g_kUtopiaModule.IsMultiplayer() && id == 0 && !g_kUtopiaModule.IsIgnoreClientIDForObjectCreation() ) {
		SetObjID( g_kUtopiaModule.GetUnusedClientID() );
		g_kUtopiaModule.IncrementClientID();
	}
}

/*
================
ShipSubsystem::GetObjType
================
*/
// FUNCTION: STBC 0x0056bad0
int ShipSubsystem::GetObjType( void ) {
	return SHIPSUBSYSTEM_TYPE;
}

/*
================
ShipSubsystem::GetClassName
================
*/
// FUNCTION: STBC 0x0056bb20
char *ShipSubsystem::GetClassName( void ) {
	return "ShipSubsystem";
}

/*
================
ShipSubsystem::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x0056bb30
char *ShipSubsystem::GetSwigTypeName( void ) {
	return "_p_ShipSubsystem";
}

/*
================
ShipSubsystem::GetPythonClassName
================
*/
// FUNCTION: STBC 0x0056bb40
char *ShipSubsystem::GetPythonClassName( void ) {
	return "ShipSubsystemPtr";
}

/*
================
ShipSubsystem::GetClassHandlers
================
*/
// FUNCTION: STBC 0x0056bb50
TGEventHandlerTable *ShipSubsystem::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
ShipSubsystem::~ShipSubsystem

Deletes the children with their array, and the name.
================
*/
// FUNCTION: STBC 0x0056bb90
// FUNCTION: STBC 0x0056bb60 ??_GShipSubsystem@@UAEPAXI@Z
ShipSubsystem::~ShipSubsystem( void ) {
	if ( numChildSubsystems > 0 ) {
		for ( int i = 0; i < numChildSubsystems; i++ ) {
			ShipSubsystem *child = GetChildSubsystem( i );
			if ( child != NULL ) {
				delete child;
			}
		}
		delete [] childSubsystems;
		childSubsystems = NULL;
	}
	if ( name != NULL ) {
		delete [] name;
		name = NULL;
	}
}

/*
================
ShipSubsystem::SetParentShip

Moves the range checks, whose events go to the ship, to the new ship.
================
*/
// FUNCTION: STBC 0x0056bc50
void ShipSubsystem::SetParentShip( ShipClass *ship ) {
	parentShip = ship;
	SetupRangeChecks();
}

/*
================
ShipSubsystem::Update

Refreshes the condition percentages and lets their watchers send the
range check events. The ship hears when the subsystem becomes disabled
and when it works again.
================
*/
// FUNCTION: STBC 0x0056bc60
void ShipSubsystem::Update( float deltaTime ) {
	if ( GetMaxCondition() > 0.0f ) {
		conditionPercentage = condition / GetMaxCondition();
	} else {
		conditionPercentage = 0.0f;
	}
	lastUpdateLength = deltaTime;
	conditionWatcher.CheckRanges();

	combinedConditionPercentage = GetCombinedConditionPercentage();
	combinedPercentageWatcher.CheckRanges();

	bool nowDisabled = IsDisabled();
	if ( !disabled && nowDisabled == true ) {
		disabled = nowDisabled;
		TGFloatEvent *event = new TGFloatEvent;
		event->SetSource( this );
		event->SetDestination( parentShip );
		event->SetEventType( ET_SUBSYSTEM_COMPLETELY_DISABLED );
		event->SetFloat( combinedConditionPercentage );
		g_kEventManager.AddEvent( event );
	}
	if ( disabled == true && !nowDisabled ) {
		disabled = nowDisabled;
		TGFloatEvent *event = new TGFloatEvent;
		event->SetSource( this );
		event->SetDestination( parentShip );
		event->SetEventType( ET_SUBSYSTEM_PARTIALLY_OPERATIONAL );
		event->SetFloat( combinedConditionPercentage );
		g_kEventManager.AddEvent( event );
	}
}

/*
================
ShipSubsystem::Repair

Raises the condition by the repair points, less the more complex the
subsystem is to repair.
================
*/
// FUNCTION: STBC 0x0056bd90
void ShipSubsystem::Repair( float points ) {
	float repaired = points / GetRepairComplexity();
	SetCondition( GetCondition() + repaired );
}

/*
================
ShipSubsystem::SetProperty

The subsystem starts at the property's full condition.
================
*/
// FUNCTION: STBC 0x0056bdc0
void ShipSubsystem::SetProperty( SubsystemProperty *property_ ) {
	property = property_;
	SetCondition( GetMaxCondition() );
}

/*
================
ShipSubsystem::SetupRangeChecks

Replaces the range checks: the condition crossing the disabled percentage
either way, the subsystem being destroyed, and the subsystem with its
children being rebuilt or completely destroyed. Each sends its event to the
parent ship.
================
*/
// FUNCTION: STBC 0x0056bde0
void ShipSubsystem::SetupRangeChecks( void ) {
	if ( disabledCheck != FloatRangeWatcher::RID_INVALID ) {
		conditionWatcher.RemoveRangeCheck( disabledCheck );
	}
	if ( operationalCheck != FloatRangeWatcher::RID_INVALID ) {
		conditionWatcher.RemoveRangeCheck( operationalCheck );
	}
	if ( destroyedCheck != FloatRangeWatcher::RID_INVALID ) {
		conditionWatcher.RemoveRangeCheck( destroyedCheck );
	}
	// The original's bug: the rebuilt check is removed from the condition
	// watcher, though it is added to the combined percentage watcher below.
	if ( rebuiltCheck != FloatRangeWatcher::RID_INVALID ) {
		conditionWatcher.RemoveRangeCheck( rebuiltCheck );
	}
	if ( completelyDestroyedCheck != FloatRangeWatcher::RID_INVALID ) {
		combinedPercentageWatcher.RemoveRangeCheck( completelyDestroyedCheck );
	}

	TGFloatEvent *event = new TGFloatEvent;
	event->SetSource( this );
	event->SetDestination( parentShip );
	event->SetEventType( ET_SUBSYSTEM_DISABLED );
	disabledCheck = conditionWatcher.AddRangeCheck( GetDisabledPercentage(), FloatRangeWatcher::FRW_BELOW, event );

	event = new TGFloatEvent;
	event->SetSource( this );
	event->SetDestination( parentShip );
	event->SetEventType( ET_SUBSYSTEM_OPERATIONAL );
	operationalCheck = conditionWatcher.AddRangeCheck( GetDisabledPercentage(), FloatRangeWatcher::FRW_ABOVE, event );

	event = new TGFloatEvent;
	event->SetSource( this );
	event->SetDestination( parentShip );
	event->SetEventType( ET_SUBSYSTEM_DESTROYED );
	destroyedCheck = conditionWatcher.AddRangeCheck( SUBSYSTEM_DESTROYED_PERCENTAGE, FloatRangeWatcher::FRW_BELOW, event );

	event = new TGFloatEvent;
	event->SetSource( this );
	event->SetDestination( parentShip );
	event->SetEventType( ET_SUBSYSTEM_REBUILT );
	rebuiltCheck = combinedPercentageWatcher.AddRangeCheck( SUBSYSTEM_DESTROYED_PERCENTAGE, FloatRangeWatcher::FRW_ABOVE, event );

	event = new TGFloatEvent;
	event->SetSource( this );
	event->SetDestination( parentShip );
	event->SetEventType( ET_SUBSYSTEM_COMPLETELY_DESTROYED );
	completelyDestroyedCheck = combinedPercentageWatcher.AddRangeCheck( SUBSYSTEM_DESTROYED_PERCENTAGE, FloatRangeWatcher::FRW_BELOW, event );
}

/*
================
ShipSubsystem::SetInvincible

Also sets the children.
================
*/
// FUNCTION: STBC 0x0056c050
void ShipSubsystem::SetInvincible( bool invincible_ ) {
	invincible = invincible_;
	for ( int i = 0; i < numChildSubsystems; i++ ) {
		GetChildSubsystem( i )->SetInvincible( invincible_ );
	}
}

/*
================
ShipSubsystem::GetName

Builds the name again from the property's name on every call, freeing the
old one.
================
*/
// FUNCTION: STBC 0x0056c090
char *ShipSubsystem::GetName( void ) {
	if ( name != NULL ) {
		delete [] name;
		name = NULL;
	}
	if ( property != NULL ) {
		name = property->GetName().Unknown6F56F0();
	}
	return name;
}

/*
================
ShipSubsystem::GetDisplayName

The localised name of the subsystem, else its name.
================
*/
// FUNCTION: STBC 0x0056c130
TGString ShipSubsystem::GetDisplayName( void ) {
	TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Subsystems.tgl", true );
	TGString displayName;
	if ( database != NULL ) {
		if ( database->HasString( GetName() ) ) {
			displayName.Unknown6F4D90( database->GetString( GetName() ) );
		} else {
			displayName.AssignC( GetName() );
		}
	} else {
		displayName.AssignC( GetName() );
	}
	g_kLocalizationManager.Unload( database );
	return displayName;
}

/*
================
ShipSubsystem::GetMaxCondition

The property's; 1 without a property.
================
*/
// FUNCTION: STBC 0x0056c310
float ShipSubsystem::GetMaxCondition( void ) {
	if ( property != NULL ) {
		return property->GetMaxCondition();
	}
	return 1.0f;
}

/*
================
ShipSubsystem::IsCritical
================
*/
// FUNCTION: STBC 0x0056c330
bool ShipSubsystem::IsCritical( void ) {
	if ( property != NULL ) {
		return property->IsCritical();
	}
	return false;
}

/*
================
ShipSubsystem::IsTargetable
================
*/
// FUNCTION: STBC 0x0056c340
bool ShipSubsystem::IsTargetable( void ) {
	if ( property != NULL ) {
		return property->IsTargetable();
	}
	return false;
}

/*
================
ShipSubsystem::IsDisabled

Disabled when the condition has fallen to the disabled percentage, or when
every child is.
================
*/
// FUNCTION: STBC 0x0056c350
bool ShipSubsystem::IsDisabled( void ) {
	float condition = GetConditionPercentage();
	if ( GetDisabledPercentage() > condition ) {
		return true;
	}
	int count = numChildSubsystems;
	if ( count > 0 ) {
		for ( int i = 0; i < count; i++ ) {
			ShipSubsystem *child = GetChildSubsystem( i );
			if ( child != NULL && !child->IsDisabled() ) {
				return false;
			}
		}
		return true;
	}
	return false;
}

/*
================
ShipSubsystem::IsHittableFromLocation

Whether a ray from the location to the subsystem's position meets the
ship within the subsystem's radius of that position.
================
*/
// FUNCTION: STBC 0x0056c3b0
bool ShipSubsystem::IsHittableFromLocation( TGPoint3 *location ) {
	ShipClass *ship = parentShip;
	if ( ship != NULL ) {
		NiPoint3 worldLocation = GetWorldLocation();
		NiPoint3 hitPoint;
		NiPoint3 hitNormal;
		if ( ship->VirtualSlot80( location, &worldLocation, &hitPoint, &hitNormal, 1 ) ) {
			NiPoint3 offset = hitPoint - worldLocation;
			float radius = GetRadius();
			if ( offset.SqrLength() < radius * radius ) {
				return true;
			}
		}
	}
	return false;
}

/*
================
ShipSubsystem::SetCondition

The condition is kept within the maximum. A loss of condition that leaves
the subsystem damaged is reported, unless its ship is dying.
================
*/
// FUNCTION: STBC 0x0056c470
void ShipSubsystem::SetCondition( float condition_ ) {
	condition = condition_;
	if ( GetMaxCondition() < condition ) {
		condition = GetMaxCondition();
	}
	conditionPercentage = condition / GetMaxCondition();
	conditionWatcher.CheckRanges();

	ShipClass *ship = parentShip;
	if ( GetMaxCondition() > condition && ( ship == NULL || !ship->IsDying() ) ) {
		TGObjPtrEvent *event = new TGObjPtrEvent;
		event->SetSource( NULL );
		event->SetDestination( parentShip );
		event->SetEventType( ET_SUBSYSTEM_DAMAGED );
		event->SetObjPtr( this );
		g_kEventManager.AddEvent( event );
	}
}

/*
================
ShipSubsystem::SetConditionPercentage

Sets the condition as a fraction of the maximum.
================
*/
// FUNCTION: STBC 0x0056c550
void ShipSubsystem::SetConditionPercentage( float percentage ) {
	SetCondition( GetMaxCondition() * percentage );
}

/*
================
ShipSubsystem::GetChildSubsystem

NULL when the index is out of range.
================
*/
// FUNCTION: STBC 0x0056c570
ShipSubsystem *ShipSubsystem::GetChildSubsystem( int index ) {
	if ( index >= 0 && index < numChildSubsystems ) {
		return childSubsystems[index];
	}
	return NULL;
}

/*
================
ShipSubsystem::GetLastChildSubsystemIndex

The index of the last child before the first empty slot; -1 without
children.
================
*/
// FUNCTION: STBC 0x0056c590
int ShipSubsystem::GetLastChildSubsystemIndex( void ) {
	if ( numChildSubsystems == 0 ) {
		return -1;
	}
	int i;
	for ( i = 0; i < numChildSubsystems; i++ ) {
		if ( GetChildSubsystem( i ) == NULL ) {
			break;
		}
	}
	return i - 1;
}

/*
================
ShipSubsystem::AddChildSubsystem

Grows the array of children by one, and gives the child this subsystem as
its parent and this subsystem's ship.
================
*/
// FUNCTION: STBC 0x0056c5c0
void ShipSubsystem::AddChildSubsystem( ShipSubsystem *child ) {
	ShipSubsystem **oldChildren = childSubsystems;
	childSubsystems = new ShipSubsystem *[numChildSubsystems + 1];
	if ( oldChildren != NULL ) {
		for ( int i = 0; i < numChildSubsystems; i++ ) {
			childSubsystems[i] = oldChildren[i];
		}
		delete [] oldChildren;
	}
	childSubsystems[numChildSubsystems] = child;
	numChildSubsystems++;
	child->SetParentSubsystem( this );
	child->SetParentShip( parentShip );
}

/*
================
ShipSubsystem::GetChildSubsystemIndex

-1 when the subsystem is not a child of this one.
================
*/
// FUNCTION: STBC 0x0056c630
int ShipSubsystem::GetChildSubsystemIndex( ShipSubsystem *child ) {
	if ( child->parentSubsystem == this ) {
		for ( int i = 0; i < numChildSubsystems; i++ ) {
			if ( GetChildSubsystem( i ) == child ) {
				return i;
			}
		}
	}
	return -1;
}

/*
================
ShipSubsystem::HasTargetableChildSubsystem

Whether a child, or a child of one, is targetable.
================
*/
// FUNCTION: STBC 0x0056c670
bool ShipSubsystem::HasTargetableChildSubsystem( void ) {
	bool result = false;
	for ( int i = 0; i < numChildSubsystems; i++ ) {
		ShipSubsystem *child = GetChildSubsystem( i );
		if ( child->IsTargetable() == true || child->HasTargetableChildSubsystem() == true ) {
			result = true;
			break;
		}
	}
	return result;
}

/*
================
ShipSubsystem::GetNextTargetableChildSubsystem

The next targetable child with condition left after previous (NULL starts at
the first), wrapping around; NULL when there is none.
================
*/
// FUNCTION: STBC 0x0056c6c0
ShipSubsystem *ShipSubsystem::GetNextTargetableChildSubsystem( ShipSubsystem *previous ) {
	if ( numChildSubsystems != 0 ) {
		int i = -1;
		if ( previous != NULL ) {
			i = GetChildSubsystemIndex( previous );
		}
		int start = i;
		i++;
		if ( i >= numChildSubsystems ) {
			i = 0;
		}
		while ( i != start ) {
			ShipSubsystem *child = GetChildSubsystem( i );
			if ( child != NULL && child->IsTargetable() && child->condition > 0.0f ) {
				return child;
			}
			i++;
			if ( i >= numChildSubsystems ) {
				i = 0;
			}
		}
	}
	return NULL;
}

/*
================
ShipSubsystem::GetCombinedConditionPercentage

The condition percentage averaged with the children's: the own percentage
counts when the subsystem is targetable or has no children.
================
*/
// FUNCTION: STBC 0x0056c740
float ShipSubsystem::GetCombinedConditionPercentage( void ) {
	int count = numChildSubsystems;
	if ( IsTargetable() == true || count <= 0 ) {
		combinedConditionPercentage = GetConditionPercentage();
	} else {
		combinedConditionPercentage = 0;
	}
	if ( count > 0 ) {
		for ( int i = 0; i < count; i++ ) {
			combinedConditionPercentage += GetChildSubsystem( i )->GetCombinedConditionPercentage();
		}
		if ( IsTargetable() == true ) {
			combinedConditionPercentage /= count + 1.0f;
		} else {
			combinedConditionPercentage /= count;
		}
	}
	return combinedConditionPercentage;
}

/*
================
ShipSubsystem::GetWorldLocation

The subsystem's position carried to the world by its ship's node; the
position in the model's frame when the ship is none.
================
*/
// FUNCTION: STBC 0x0056c7b0
TGPoint3 ShipSubsystem::GetWorldLocation( void ) {
	ObjectClass *ship = ObjectClass::Cast( GetParentShip() );
	if ( ship != NULL ) {
		return TGModelUtils::LocalToWorldPoint( ship->GetNode(), GetPositionTG() );
	}
	return GetPositionTG();
}

/*
================
ShipSubsystem::GetPositionTG
================
*/

/*
================
ShipSubsystem::UpdateDamagePoint

Moves damagePoint onto the ship's hull: a ray from the subsystem's
position, carried to the world, toward the point ten units inward along
the position's direction is picked against the ship, and the hit nearest
the position, pushed a little past the hull, becomes the damage point in
the model's frame. Up to nine more rays with randomly bent directions are
tried when the first hits nothing. The damage point stays the position
when the position is inside the ship, when the pick fails, or for a
station's subsystem that is not a weapon.
================
*/
// FUNCTION: STBC 0x0056c8f0
// DRAFT: call set as retail (the braceless break body gives own cost 710, the closing operator= out of line); left: ship and this swap ebp/edi, the offset's SqrLength reads the x87 stack where retail stores the difference, one branch fewer
void ShipSubsystem::UpdateDamagePoint( void ) {
	ShipClass *ship = parentShip;
	ShipProperty *shipProperty = ship->GetShipProperty();
	if ( ship->Unknown595440( &GetPosition() ) == true
		|| ( shipProperty != NULL && shipProperty->GetGenus() == GENUS_STATION && !IsTypeOf( WEAPON_TYPE ) ) ) {
		damagePoint = GetPosition();
		return;
	}

	NiPick pick;
	pick.SetPickType( NiPick::FIND_ALL );
	pick.SetSortType( NiPick::SORT );
	pick.SetIntersectType( NiPick::TRIANGLE_INTERSECT );
	pick.SetCoordinateType( NiPick::WORLD_COORDINATES );
	pick.SetReturnNormal( true );
	pick.SetReturnSmoothNormal( true );
	pick.SetFrontOnly( true );

	NiPoint3 position = GetPosition();
	NiPoint3 direction = position;
	direction.Unitize();
	NiPoint3 worldPosition = TGModelUtils::LocalToWorldPoint( ship->GetNode(), position );

	NiPoint3 hit = NiPoint3::ZERO;
	float nearest = NI_INFINITY;
	for ( int i = 0; i < 10; i++ ) {
		NiPoint3 rayDirection = direction;
		if ( i > 0 ) {
			rayDirection += TGPoint3::GetRandomUnitVector();
			rayDirection.Unitize();
		}
		rayDirection *= -10.0f;
		rayDirection += position;
		NiPoint3 worldEnd = TGModelUtils::LocalToWorldPoint( ship->GetNode(), rayDirection );
		if ( !ship->VirtualSlot63( &worldPosition, worldEnd, &pick ) ) {
			break;
		}

		for ( unsigned int j = 0; j < pick.GetResults().GetSize(); j++ ) {
			NiPick::Record *record = pick.GetResults().GetAt( j );
			if ( record != NULL ) {
				NiPoint3 offset = record->intersection - worldPosition;
				float distance = offset.SqrLength();
				if ( distance < nearest ) {
					nearest = distance;
					hit = record->intersection;
					NiPoint3 outward = hit - worldPosition;
					outward.Unitize();
					outward *= 0.05f;
					hit += outward;
				}
			}
		}

		if ( hit != NiPoint3::ZERO ) {
			damagePoint = TGModelUtils::WorldToLocalPoint( ship->GetNode(), hit );
			return;
		}
	}
	damagePoint = GetPosition();
}

/*
================
ShipSubsystem::WriteToStream

The name, the property's ID, the children (each writing itself), the
parent subsystem's ID, the condition figures, the ship's ID, the two
watchers, the flags, the range check IDs and damagePoint.
================
*/
// FUNCTION: STBC 0x0056ceb0
bool ShipSubsystem::WriteToStream( TGStream *stream ) {
	if ( !TGEventHandlerObject::WriteToStream( stream ) ) {
		return false;
	}

	stream->VirtualSlot11( name );
	stream->WriteID( GetSavedID( property ) );
	stream->WriteLong( numChildSubsystems );
	for ( int i = 0; i < numChildSubsystems; i++ ) {
		childSubsystems[i]->WriteToStream( stream );
	}
	stream->WriteID( GetSavedID( parentSubsystem ) );
	stream->WriteFloat( condition );
	stream->WriteFloat( conditionPercentage );
	stream->WriteFloat( combinedConditionPercentage );
	stream->WriteFloat( lastUpdateLength );
	stream->WriteFloat( unknown48 );
	stream->WriteID( GetSavedID( parentShip ) );
	conditionWatcher.WriteToStream( stream );
	combinedPercentageWatcher.WriteToStream( stream );
	stream->WriteBool( invincible );
	stream->WriteBool( disabled );
	stream->WriteID( disabledCheck );
	stream->WriteID( operationalCheck );
	stream->WriteID( destroyedCheck );
	stream->WriteID( completelyDestroyedCheck );
	stream->WriteID( rebuiltCheck );
	stream->WriteFloat( damagePoint.x );
	stream->WriteFloat( damagePoint.y );
	stream->WriteFloat( damagePoint.z );
	return true;
}

/*
================
ShipSubsystem::ReadFromStream

Reads what WriteToStream writes; the children are loaded whole, the
other objects are held as their IDs until PostFixUp.
================
*/
// FUNCTION: STBC 0x0056d010
bool ShipSubsystem::ReadFromStream( TGStream *stream ) {
	if ( !TGEventHandlerObject::ReadFromStream( stream ) ) {
		return false;
	}

	name = stream->VirtualSlot10( -1 );
	property = (SubsystemProperty *)stream->ReadID();
	int numChildren = stream->ReadLong();
	childSubsystems = new ShipSubsystem *[numChildren];
	numChildSubsystems = numChildren;
	for ( int i = 0; i < numChildren; i++ ) {
		TGObject *child = LoadObject( stream, 0 );
		if ( child == NULL ) {
			return false;
		}
		childSubsystems[i] = (ShipSubsystem *)child;
	}
	parentSubsystem = (ShipSubsystem *)stream->ReadID();
	condition = stream->ReadFloat();
	conditionPercentage = stream->ReadFloat();
	combinedConditionPercentage = stream->ReadFloat();
	lastUpdateLength = stream->ReadFloat();
	unknown48 = stream->ReadFloat();
	parentShip = (ShipClass *)stream->ReadID();
	conditionWatcher.ReadFromStream( stream );
	conditionWatcher.SetVariable( &conditionPercentage );
	combinedPercentageWatcher.ReadFromStream( stream );
	combinedPercentageWatcher.SetVariable( &combinedConditionPercentage );
	invincible = stream->ReadBool();
	disabled = stream->ReadBool();
	disabledCheck = stream->ReadID();
	operationalCheck = stream->ReadID();
	destroyedCheck = stream->ReadID();
	completelyDestroyedCheck = stream->ReadID();
	rebuiltCheck = stream->ReadID();
	damagePoint.x = stream->ReadFloat();
	damagePoint.y = stream->ReadFloat();
	damagePoint.z = stream->ReadFloat();
	return true;
}

/*
================
ShipSubsystem::FixUpPointers

Fails when the property, the ship or the parent subsystem the subsystem
was saved with has not been loaded.
================
*/
// FUNCTION: STBC 0x0056d170
bool ShipSubsystem::FixUpPointers( void ) {
	if ( !TGEventHandlerObject::FixUpPointers() ) {
		return false;
	}
	if ( property != NULL && GetTGObjectPtr( (long)property ) == NULL ) {
		return false;
	}
	if ( !conditionWatcher.FixUpPointers() ) {
		return false;
	}
	if ( !combinedPercentageWatcher.FixUpPointers() ) {
		return false;
	}
	if ( parentShip != NULL && GetTGObjectPtr( (long)parentShip ) == NULL ) {
		return false;
	}
	if ( parentSubsystem != NULL && GetTGObjectPtr( (long)parentSubsystem ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
ShipSubsystem::PostFixUp

Turns the IDs ReadFromStream read back into objects.
================
*/
// FUNCTION: STBC 0x0056d1f0
void ShipSubsystem::PostFixUp( void ) {
	TGEventHandlerObject::PostFixUp();
	property = (SubsystemProperty *)GetSavedObject( (long)property );
	conditionWatcher.PostFixUp();
	combinedPercentageWatcher.PostFixUp();
	parentShip = (ShipClass *)GetSavedObject( (long)parentShip );
	parentSubsystem = (ShipSubsystem *)GetSavedObject( (long)parentSubsystem );
}

/*
================
ShipSubsystem::VirtualSlot26

Writes the subsystem's condition as a fraction of its maximum, in 255ths,
then the children's.
================
*/
// FUNCTION: STBC 0x0056d250
void ShipSubsystem::VirtualSlot26( TGStream *stream ) {
	float maxCondition = GetMaxCondition();
	stream->WriteChar( (char)( condition / maxCondition * 255.0f ) );

	int numChildren = numChildSubsystems;
	for ( int i = 0; i < numChildren; i++ ) {
		ShipSubsystem *child = GetChildSubsystem( i );
		if ( child != NULL ) {
			child->VirtualSlot26( stream );
		}
	}
	stream->GetPos();
}

/*
================
ShipSubsystem::VirtualSlot27

Reads what VirtualSlot26 writes.
================
*/
// FUNCTION: STBC 0x0056d2b0
void ShipSubsystem::VirtualSlot27( TGStream *stream ) {
	unsigned char value = stream->ReadChar();
	condition = GetMaxCondition() * ( value * ( 1.0f / 255.0f ) );

	int numChildren = numChildSubsystems;
	for ( int i = 0; i < numChildren; i++ ) {
		ShipSubsystem *child = GetChildSubsystem( i );
		if ( child != NULL ) {
			child->VirtualSlot27( stream );
		}
	}
	stream->GetPos();
}

/*
================
ShipSubsystem::VirtualSlot28

Writes the condition as VirtualSlot26 does, then the children's.
================
*/
// FUNCTION: STBC 0x0056d320
void ShipSubsystem::VirtualSlot28( TGStream *stream, bool flag ) {
	float maxCondition = GetMaxCondition();
	stream->WriteChar( (char)( condition / maxCondition * 255.0f ) );

	int numChildren = numChildSubsystems;
	for ( int i = 0; i < numChildren; i++ ) {
		ShipSubsystem *child = GetChildSubsystem( i );
		if ( child != NULL ) {
			child->VirtualSlot28( stream, flag );
		}
	}
	stream->GetPos();
}

/*
================
ShipSubsystem::VirtualSlot29

Reads what VirtualSlot28 writes; the condition is taken only when the
state is newer than the last one taken.
================
*/
// FUNCTION: STBC 0x0056d390
void ShipSubsystem::VirtualSlot29( TGStream *stream, float time ) {
	unsigned char value = stream->ReadChar();
	if ( time > unknown84 ) {
		condition = GetMaxCondition() * ( value * ( 1.0f / 255.0f ) );
		unknown84 = time;
	}

	int numChildren = numChildSubsystems;
	for ( int i = 0; i < numChildren; i++ ) {
		ShipSubsystem *child = GetChildSubsystem( i );
		if ( child != NULL ) {
			child->VirtualSlot29( stream, time );
		}
	}
	stream->GetPos();
}
