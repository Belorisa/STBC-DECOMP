/*
===============================================================================

	ProximityCheck.cpp

===============================================================================
*/

#include <iostream>
#include "ProximityCheck.h"
#include "TGEventHandlerTable.h"
#include "SetClass.h"
#include "SetManager.h"
#include "ProximityManager.h"
#include "ProximityEvent.h"
#include "TGEvent.h"
#include "TGEventManager.h"
#include "TGEventHandlerObject.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x005a65c0 _$E32
// FUNCTION: STBC 0x005a65d0 _$E29
// FUNCTION: STBC 0x005a65e0 _$E31
// FUNCTION: STBC 0x005a65f0 _$E30
// FUNCTION: STBC 0x005a6770 _$E38
// FUNCTION: STBC 0x005a6780 _$E35
// FUNCTION: STBC 0x005a6790 _$E37
// FUNCTION: STBC 0x005a67a0 _$E36

// A static of an included header: its initialiser with a guard byte, and the exit-time registration.
// FUNCTION: STBC 0x005a7250 _$E468
// FUNCTION: STBC 0x005a7270 _$E467

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x005a6840 _$E66
// FUNCTION: STBC 0x005a6850 _$E65
// FUNCTION: STBC 0x005a6860 _$E241
// FUNCTION: STBC 0x005a6870 _$E240
// FUNCTION: STBC 0x005a6880 _$E77
// FUNCTION: STBC 0x005a6890 _$E76
// FUNCTION: STBC 0x005a68a0 _$E81
// FUNCTION: STBC 0x005a68b0 _$E80
// FUNCTION: STBC 0x005a68c0 _$E85
// FUNCTION: STBC 0x005a68d0 _$E84
// FUNCTION: STBC 0x005a6920 _$E89
// FUNCTION: STBC 0x005a6930 _$E88
// FUNCTION: STBC 0x005a6940 _$E206
// FUNCTION: STBC 0x005a6950 _$E205
// FUNCTION: STBC 0x005a6960 _$E148
// FUNCTION: STBC 0x005a6970 _$E147
// FUNCTION: STBC 0x005a6980 _$E161
// FUNCTION: STBC 0x005a6990 _$E160
// FUNCTION: STBC 0x005a69a0 _$E211
// FUNCTION: STBC 0x005a69b0 _$E210
// FUNCTION: STBC 0x005a69c0 _$E246
// FUNCTION: STBC 0x005a69d0 _$E245
// FUNCTION: STBC 0x005a69e0 _$E215
// FUNCTION: STBC 0x005a69f0 _$E214
// FUNCTION: STBC 0x005a6a00 _$E250
// FUNCTION: STBC 0x005a6a10 _$E249
// FUNCTION: STBC 0x005a6a20 _$E257
// FUNCTION: STBC 0x005a6a30 _$E256
// FUNCTION: STBC 0x005a6a40 _$E279
// FUNCTION: STBC 0x005a6a50 _$E278
// FUNCTION: STBC 0x005a6a60 _$E283
// FUNCTION: STBC 0x005a6a70 _$E282
// FUNCTION: STBC 0x005a6a80 _$E106
// FUNCTION: STBC 0x005a6a90 _$E105
// FUNCTION: STBC 0x005a6aa0 _$E265
// FUNCTION: STBC 0x005a6ab0 _$E264
// FUNCTION: STBC 0x005a6ac0 _$E272
// FUNCTION: STBC 0x005a6ad0 _$E271
// FUNCTION: STBC 0x005a6ae0 _$E295
// FUNCTION: STBC 0x005a6af0 _$E294
// FUNCTION: STBC 0x005a6b00 _$E299
// FUNCTION: STBC 0x005a6b10 _$E298
// FUNCTION: STBC 0x005a6b20 _$E339
// FUNCTION: STBC 0x005a6b30 _$E338
// FUNCTION: STBC 0x005a6b40 _$E345
// FUNCTION: STBC 0x005a6b50 _$E344
// FUNCTION: STBC 0x005a6b60 _$E287
// FUNCTION: STBC 0x005a6b70 _$E286
// FUNCTION: STBC 0x005a6b80 _$E291
// FUNCTION: STBC 0x005a6b90 _$E290
// FUNCTION: STBC 0x005a6ba0 _$E303
// FUNCTION: STBC 0x005a6bb0 _$E302
// FUNCTION: STBC 0x005a6bc0 _$E307
// FUNCTION: STBC 0x005a6bd0 _$E306
// FUNCTION: STBC 0x005a6be0 _$E311
// FUNCTION: STBC 0x005a6bf0 _$E310
// FUNCTION: STBC 0x005a6c00 _$E315
// FUNCTION: STBC 0x005a6c10 _$E314
// FUNCTION: STBC 0x005a6c20 _$E319
// FUNCTION: STBC 0x005a6c30 _$E318
// FUNCTION: STBC 0x005a6c40 _$E323
// FUNCTION: STBC 0x005a6c50 _$E322
// FUNCTION: STBC 0x005a6c60 _$E327
// FUNCTION: STBC 0x005a6c70 _$E326
// FUNCTION: STBC 0x005a6c80 _$E331
// FUNCTION: STBC 0x005a6c90 _$E330
// FUNCTION: STBC 0x005a6ca0 _$E111
// FUNCTION: STBC 0x005a6cb0 _$E110
// FUNCTION: STBC 0x005a6cc0 _$E335
// FUNCTION: STBC 0x005a6cd0 _$E334
// FUNCTION: STBC 0x005a6ce0 _$E116
// FUNCTION: STBC 0x005a6cf0 _$E115
// FUNCTION: STBC 0x005a6d00 _$E349
// FUNCTION: STBC 0x005a6d10 _$E348
// FUNCTION: STBC 0x005a6d20 _$E353
// FUNCTION: STBC 0x005a6d30 _$E352
// FUNCTION: STBC 0x005a6d40 _$E357
// FUNCTION: STBC 0x005a6d50 _$E356
// FUNCTION: STBC 0x005a6d60 _$E361
// FUNCTION: STBC 0x005a6d70 _$E360
// FUNCTION: STBC 0x005a6d80 _$E125
// FUNCTION: STBC 0x005a6d90 _$E124
// FUNCTION: STBC 0x005a6da0 _$E365
// FUNCTION: STBC 0x005a6db0 _$E364
// FUNCTION: STBC 0x005a6dc0 _$E369
// FUNCTION: STBC 0x005a6dd0 _$E368
// FUNCTION: STBC 0x005a6de0 _$E373
// FUNCTION: STBC 0x005a6df0 _$E372
// FUNCTION: STBC 0x005a6e00 _$E131
// FUNCTION: STBC 0x005a6e10 _$E130
// FUNCTION: STBC 0x005a6e90 _$E136
// FUNCTION: STBC 0x005a6ea0 _$E135
// FUNCTION: STBC 0x005a6eb0 _$E166
// FUNCTION: STBC 0x005a6ec0 _$E165
// FUNCTION: STBC 0x005a6ed0 _$E377
// FUNCTION: STBC 0x005a6ee0 _$E376
// FUNCTION: STBC 0x005a6ef0 _$E171
// FUNCTION: STBC 0x005a6f00 _$E170
// FUNCTION: STBC 0x005a6f10 _$E381
// FUNCTION: STBC 0x005a6f20 _$E380
// FUNCTION: STBC 0x005a6f30 _$E385
// FUNCTION: STBC 0x005a6f40 _$E384
// FUNCTION: STBC 0x005a6f50 _$E176
// FUNCTION: STBC 0x005a6f60 _$E175
// FUNCTION: STBC 0x005a6f70 _$E389
// FUNCTION: STBC 0x005a6f80 _$E388
// FUNCTION: STBC 0x005a6f90 _$E141
// FUNCTION: STBC 0x005a6fa0 _$E140
// FUNCTION: STBC 0x005a6fb0 _$E393
// FUNCTION: STBC 0x005a6fc0 _$E392
// FUNCTION: STBC 0x005a6fd0 _$E397
// FUNCTION: STBC 0x005a6fe0 _$E396
// FUNCTION: STBC 0x005a6ff0 _$E181
// FUNCTION: STBC 0x005a7000 _$E180
// FUNCTION: STBC 0x005a7010 _$E186
// FUNCTION: STBC 0x005a7020 _$E185
// FUNCTION: STBC 0x005a7030 _$E401
// FUNCTION: STBC 0x005a7040 _$E400
// FUNCTION: STBC 0x005a7050 _$E405
// FUNCTION: STBC 0x005a7060 _$E404
// FUNCTION: STBC 0x005a7070 _$E409
// FUNCTION: STBC 0x005a7080 _$E408
// FUNCTION: STBC 0x005a7090 _$E413
// FUNCTION: STBC 0x005a70a0 _$E412
// FUNCTION: STBC 0x005a70b0 _$E417
// FUNCTION: STBC 0x005a70c0 _$E416
// FUNCTION: STBC 0x005a70d0 _$E421
// FUNCTION: STBC 0x005a70e0 _$E420
// FUNCTION: STBC 0x005a70f0 _$E425
// FUNCTION: STBC 0x005a7100 _$E424
// FUNCTION: STBC 0x005a7110 _$E429
// FUNCTION: STBC 0x005a7120 _$E428
// FUNCTION: STBC 0x005a71f0 _$E191
// FUNCTION: STBC 0x005a7200 _$E190
// FUNCTION: STBC 0x005a7210 _$E433
// FUNCTION: STBC 0x005a7220 _$E432
// FUNCTION: STBC 0x005a7230 _$E196
// FUNCTION: STBC 0x005a7240 _$E195
// FUNCTION: STBC 0x005a7280 _$E201
// FUNCTION: STBC 0x005a7290 _$E200
// FUNCTION: STBC 0x005a72a0 _$E437
// FUNCTION: STBC 0x005a72b0 _$E436
// FUNCTION: STBC 0x005a72c0 _$E442
// FUNCTION: STBC 0x005a72d0 _$E441
// FUNCTION: STBC 0x005a72e0 _$E446
// FUNCTION: STBC 0x005a72f0 _$E445

// The handler table's dynamic initialiser, the registration of its exit-time
// destructor and the guarded destructor.
// FUNCTION: STBC 0x005a5160 _$E452
// FUNCTION: STBC 0x005a5170 _$E448
// FUNCTION: STBC 0x005a5180 _$E451
// FUNCTION: STBC 0x005a5190 _$E449
// DATA: STBC 0x0098e518
TGEventHandlerTable ProximityCheck::classHandlers;

/*
================
ProximityCheck::AddToSet

Adds the check to the set's list of its class type, and to those of its
base classes.
================
*/
// FUNCTION: STBC 0x005a51b0
void ProximityCheck::AddToSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Add( this, PROXIMITYCHECK_TYPE );
		ObjectClass::AddToSet( set );
	}
}

/*
================
ProximityCheck::RemoveFromSet
================
*/
// FUNCTION: STBC 0x005a51e0
void ProximityCheck::RemoveFromSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Remove( this, PROXIMITYCHECK_TYPE );
		ObjectClass::RemoveFromSet( set );
	}
}

/*
================
ProximityCheck::GetClassObjects

The set's proximity checks and their number; NULL and 0 when there are
none.
================
*/
// FUNCTION: STBC 0x005a5210
ProximityCheck **ProximityCheck::GetClassObjects( SetClass *set, int *count ) {
	if ( set != NULL ) {
		return (ProximityCheck **)set->objectTable.GetObjects( PROXIMITYCHECK_TYPE, count );
	}
	*count = 0;
	return NULL;
}

/*
================
ProximityCheck::GetNumClassObjects

The number of the set's proximity checks.
================
*/
// FUNCTION: STBC 0x005a5240
int ProximityCheck::GetNumClassObjects( SetClass *set ) {
	if ( set != NULL ) {
		return set->objectTable.GetNumObjects( PROXIMITYCHECK_TYPE );
	}
	return 0;
}

/*
================
ProximityCheck::GetObjectByID

The proximity check with the given ID in the set, or among every object
when the set is NULL; NULL when it is none or not a ProximityCheck.
================
*/
// FUNCTION: STBC 0x005a5260
ProximityCheck *ProximityCheck::GetObjectByID( SetClass *set, long id ) {
	if ( set == NULL ) {
		TGObject *object;
		if ( TGObject::objectTable->GetAt( id, object ) && object->IsTypeOf( PROXIMITYCHECK_TYPE ) ) {
			return (ProximityCheck *)object;
		}
		return NULL;
	}
	return (ProximityCheck *)set->GetClassObject( PROXIMITYCHECK_TYPE, id );
}

/*
================
ProximityCheck::GetObject

The proximity check of the name in the set, or in any set when the set is
NULL, the last set first.
================
*/
// FUNCTION: STBC 0x005a52d0
ProximityCheck *ProximityCheck::GetObject( SetClass *set, char *name ) {
	if ( set == NULL ) {
		int count = g_kSetManager.GetNumSets();
		SetClass **sets = g_kSetManager.GetSets();
		BaseObjectClass *object = NULL;
		for ( int i = count - 1; i >= 0 && object == NULL; i-- ) {
			object = sets[i]->GetClassObject( PROXIMITYCHECK_TYPE, name );
		}
		return (ProximityCheck *)object;
	}
	return (ProximityCheck *)set->GetClassObject( PROXIMITYCHECK_TYPE, name );
}

/*
================
ProximityCheck::Cast

The object as a ProximityCheck; NULL when it is none.
================
*/
// FUNCTION: STBC 0x005a5330
ProximityCheck *ProximityCheck::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( PROXIMITYCHECK_TYPE ) ) {
		return (ProximityCheck *)object;
	}
	return NULL;
}

// The list's initialiser, constructor, exit-time registration and destructor.
// FUNCTION: STBC 0x005a5360 _$E458
// FUNCTION: STBC 0x005a5370 _$E454
// FUNCTION: STBC 0x005a53a0 _$E457
// FUNCTION: STBC 0x005a53b0 _$E455
// DATA: STBC 0x0098e4a0
NiTList<ProximityCheck *> ProximityCheck::checks( 2 );

// DATA: STBC 0x008e5f54
int ProximityCheck::currentFrame = 1;

/*
================
ProximityCheck::ProximityCheck

A check that sends ProximityEvents of the type to the destination (the
triggering object when it is NULL).
================
*/
// FUNCTION: STBC 0x005a5420 ??0ProximityCheck@@QAE@HPAVTGEventHandlerObject@@@Z
ProximityCheck::ProximityCheck( int triggerEventType_, TGEventHandlerObject *triggerDestination_ ) : manager( NULL ),
		triggerEventType( triggerEventType_ ), triggerDestination( triggerDestination_ ), triggerEvent( NULL ),
		radius( 0.0f ), ignoreObjectSize( false ), checkList( 37 ), objectFlags( 37 ), updateFrame( 0 ) {
	SetCollisionFlags( GetCollisionFlags() & ~( CFB_MEMBER_GROUP_1 | CFB_MEMBER_GROUP_2 ) );
	SetCollisionFlags( GetCollisionFlags() & ~CFB_COLLIDES_WITH_GROUP_3 );
	checks.AddTail( this );
}

/*
================
ProximityCheck::ProximityCheck

A check that sends a copy of the event when an object triggers it.
================
*/
// FUNCTION: STBC 0x005a5610 ??0ProximityCheck@@QAE@PAVTGEvent@@@Z
ProximityCheck::ProximityCheck( TGEvent *event ) : manager( NULL ), triggerEventType( 0 ), triggerDestination( NULL ),
		triggerEvent( event ), radius( 0.0f ), ignoreObjectSize( false ), checkList( 37 ), objectFlags( 37 ),
		updateFrame( 0 ) {
	SetCollisionFlags( GetCollisionFlags() & ~( CFB_MEMBER_GROUP_1 | CFB_MEMBER_GROUP_2 ) );
	SetCollisionFlags( GetCollisionFlags() & ~CFB_COLLIDES_WITH_GROUP_3 );
	checks.AddTail( this );
}

/*
================
ProximityCheck::~ProximityCheck

Leaves the list of checks and deletes the event it sends.
================
*/
// FUNCTION: STBC 0x005a5800
ProximityCheck::~ProximityCheck( void ) {
	checks.Remove( this );
	if ( triggerEvent != NULL ) {
		delete triggerEvent;
	}
}

/*
================
ProximityCheck::UpdateChecks

Brings at most maxChecks checks up to the update frame, the ones that
aren't yet; once every check is, starts the next frame (invented name).
================
*/
// FUNCTION: STBC 0x005a59c0
void ProximityCheck::UpdateChecks( int maxChecks ) {
	int updated = 0;
	int upToDate = 0;
	if ( checks.GetSize() == 0 ) {
		return;
	}
	for ( ;; ) {
		NiTListIterator pos = checks.GetHeadPos();
		while ( pos ) {
			if ( updated >= maxChecks ) {
				return;
			}
			ProximityCheck *check = checks.GetNext( pos );
			if ( check->updateFrame != currentFrame ) {
				check->UpdateObjects( currentFrame );
				updated++;
			} else {
				upToDate++;
			}
		}
		if ( updated >= maxChecks || upToDate == 0 ) {
			return;
		}
		if ( upToDate == checks.GetSize() ) {
			currentFrame++;
		}
		upToDate = 0;
	}
}

/*
================
ProximityCheck::UpdateObjects

Checks each object on the list that no check flagged since the last
update, and clears the flags (invented name).
================
*/
// FUNCTION: STBC 0x005a5a30
void ProximityCheck::UpdateObjects( int frame ) {
	updateFrame = frame;
	if ( manager == NULL ) {
		return;
	}
	SetClass *set = manager->set;
	if ( set == NULL ) {
		return;
	}
	NiTMapIterator pos = objectFlags.GetFirstPos();
	while ( pos ) {
		long id;
		bool flagged;
		objectFlags.GetNext( pos, id, flagged );
		if ( !flagged ) {
			ObjectClass *object = ObjectClass::GetObjectByID( set, id );
			if ( object != NULL ) {
				CheckProximity( object );
			}
		}
		objectFlags.SetAt( id, false );
	}
}

// The exit-time destructor of GetWorldBound's bound.
// FUNCTION: STBC 0x005a5c00 _$E461

/*
================
ProximityCheck::GetWorldBound

The sphere of the check's radius around its world position.
================
*/
// FUNCTION: STBC 0x005a5b80
NiBound &ProximityCheck::GetWorldBound( void ) {
	static NiBound bound;
	bound.SetCenterAndRadius( GetWorldLocation(), radius );
	return bound;
}

/*
================
ProximityCheck::AddObjectToCheckList
================
*/
// FUNCTION: STBC 0x005a5c10
void ProximityCheck::AddObjectToCheckList( ObjectClass *object, int triggerType ) {
	AddObjectToCheckListByID( object->GetObjID(), triggerType );
}

/*
================
ProximityCheck::AddObjectToCheckListByID

Checks the object with the trigger type, and notes that it is not yet
flagged.
================
*/
// FUNCTION: STBC 0x005a5c30
void ProximityCheck::AddObjectToCheckListByID( long id, int triggerType ) {
	checkList.SetAt( id, triggerType );
	objectFlags.SetAt( id, false );
}

/*
================
ProximityCheck::AddObjectListToCheckList

Checks each of the count objects with the trigger type.
================
*/
// FUNCTION: STBC 0x005a5d50
void ProximityCheck::AddObjectListToCheckList( ObjectClass **objects, int count, int triggerType ) {
	for ( int i = 0; i < count; i++ ) {
		AddObjectToCheckList( objects[i], triggerType );
	}
}

/*
================
ProximityCheck::AddObjectTypeToCheckList

Checks every object of the class.
================
*/
// FUNCTION: STBC 0x005a5d80
void ProximityCheck::AddObjectTypeToCheckList( int type ) {
	typeCheckList.push_back( std::pair<int, int>( type, 0 ) );
}

/*
================
ProximityCheck::FindTriggerType

An object is on the list by its ID, or by being of a listed type.
================
*/
// FUNCTION: STBC 0x005a5de0
bool ProximityCheck::FindTriggerType( ObjectClass *object, int *triggerType ) {
	int value;
	if ( checkList.GetAt( object->GetObjID(), value ) ) {
		if ( triggerType != NULL ) {
			*triggerType = value;
		}
		return true;
	}
	std::list< std::pair<int, int> >::iterator it;
	for ( it = typeCheckList.begin(); it != typeCheckList.end(); ++it ) {
		if ( object->IsTypeOf( (*it).first ) ) {
			if ( triggerType != NULL ) {
				*triggerType = (*it).second;
			}
			return true;
		}
	}
	return false;
}

/*
================
ProximityCheck::GetTriggerType

The trigger type the object is on the list with; 2 when it is not.
================
*/
// FUNCTION: STBC 0x005a5e90
int ProximityCheck::GetTriggerType( ObjectClass *object ) {
	int triggerType = 2;
	FindTriggerType( object, &triggerType );
	return triggerType;
}

/*
================
ProximityCheck::SetTriggerType

Changes the trigger type of an object that is on the list; an object that
is not is left off.
================
*/
// FUNCTION: STBC 0x005a5eb0
void ProximityCheck::SetTriggerType( ObjectClass *object, int triggerType ) {
	int current;
	if ( checkList.GetAt( object->GetObjID(), current ) ) {
		checkList.SetAt( object->GetObjID(), triggerType );
	}
}

/*
================
ProximityCheck::RemoveObjectFromCheckList
================
*/
// FUNCTION: STBC 0x005a5f80
void ProximityCheck::RemoveObjectFromCheckList( ObjectClass *object ) {
	RemoveObjectFromCheckListByID( object->GetObjID() );
}

/*
================
ProximityCheck::RemoveObjectFromCheckListByID
================
*/
// FUNCTION: STBC 0x005a5f90
void ProximityCheck::RemoveObjectFromCheckListByID( long id ) {
	checkList.RemoveAt( id );
	objectFlags.RemoveAt( id );
}

/*
================
ProximityCheck::RemoveObjectTypeFromCheckList
================
*/
// FUNCTION: STBC 0x005a60c0
void ProximityCheck::RemoveObjectTypeFromCheckList( int type ) {
	typeCheckList.remove( std::pair<int, int>( type, 0 ) );
}

/*
================
ProximityCheck::ClearCheckList

Neither the objects nor the classes are checked any more.
================
*/
// FUNCTION: STBC 0x005a6120
void ProximityCheck::ClearCheckList( void ) {
	checkList.RemoveAll();
	typeCheckList.clear();
}

/*
================
ProximityCheck::CheckProximity

For an object on the check list: notes it as flagged and, by its trigger
type, sends the trigger events when the gap between the two spheres is
negative (type 0) or positive (type 1). The object's own radius counts unless
the check ignores object sizes.
================
*/
// FUNCTION: STBC 0x005a61c0
void ProximityCheck::CheckProximity( ObjectClass *object ) {
	int triggerType;
	if ( FindTriggerType( object, &triggerType ) ) {
		objectFlags.SetAt( object->GetObjID(), true );

		NiPoint3 offset = object->GetWorldLocation() - GetWorldLocation();
		float gap = offset.Length() - GetRadius();
		if ( !ignoreObjectSize ) {
			gap -= object->GetRadius();
		}
		if ( ( gap < 0.0f && triggerType == 0 ) || ( gap > 0.0f && triggerType == 1 ) ) {
			SendTriggerEvents( object );
		}
	}
}

/*
================
ProximityCheck::RemoveAndDelete

Leaves the manager's set and deletes itself in two stages.
================
*/
// FUNCTION: STBC 0x005a6340
void ProximityCheck::RemoveAndDelete( void ) {
	if ( manager != NULL ) {
		manager->RemoveObject( this );
	}
	TGEvent *event = new TGEvent;
	event->SetEventType( ET_DELETE_ME_STAGE_1 );
	event->SetDestination( this );
	g_kEventManager.AddEvent( event );
}

/*
================
ProximityCheck::SendTriggerEvents

An object triggered the check: sends the check's ProximityEvent, to its
destination or else to the object, and a copy of the check's other event.
================
*/
// FUNCTION: STBC 0x005a63a0
void ProximityCheck::SendTriggerEvents( ObjectClass *object ) {
	if ( triggerEventType != 0 ) {
		ProximityEvent *event = new ProximityEvent;
		event->SetEventType( triggerEventType );
		event->SetProximityCheck( this );
		if ( triggerDestination != NULL ) {
			event->SetDestination( triggerDestination );
		} else {
			event->SetDestination( object );
		}
		event->SetObject( object );
		g_kEventManager.AddEvent( event );
	}
	if ( triggerEvent != NULL ) {
		g_kEventManager.AddEvent( triggerEvent->Duplicate() );
	}
}

/*
================
ProximityCheck::WriteToStream

Writes the trigger, the radius, the lists of objects by ID and by type,
each entry after a true and each list ended by a false.
================
*/
// FUNCTION: STBC 0x005a6460
bool ProximityCheck::WriteToStream( TGStream *stream ) {
	if ( !ObjectClass::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteLong( triggerEventType );
	stream->WriteID( GetSavedID( triggerDestination ) );
	stream->WriteID( GetSavedID( triggerEvent ) );
	stream->WriteFloat( radius );
	stream->WriteBool( ignoreObjectSize );
	stream->WriteInt( updateFrame );

	NiTMapIterator pos = checkList.GetFirstPos();
	while ( pos ) {
		long id;
		int triggerType;
		stream->WriteBool( true );
		checkList.GetNext( pos, id, triggerType );
		stream->WriteID( id );
		stream->WriteLong( triggerType );
	}
	stream->WriteBool( false );

	std::list< std::pair<int, int> >::iterator it;
	for ( it = typeCheckList.begin(); it != typeCheckList.end(); ++it ) {
		stream->WriteBool( true );
		stream->WriteLong( (*it).second );
		stream->WriteLong( (*it).first );
	}
	stream->WriteBool( false );
	return true;
}

/*
================
ProximityCheck::ReadFromStream

Reads what WriteToStream wrote; the destination and the event stay IDs
until PostFixUp. An object type read with a trigger type other than 0
fails the read.
================
*/
// FUNCTION: STBC 0x005a6600
bool ProximityCheck::ReadFromStream( TGStream *stream ) {
	manager = NULL;
	if ( !ObjectClass::ReadFromStream( stream ) ) {
		return false;
	}
	triggerEventType = stream->ReadLong();
	triggerDestination = (TGEventHandlerObject *)stream->ReadID();
	triggerEvent = (TGEvent *)stream->ReadID();
	radius = stream->ReadFloat();
	ignoreObjectSize = stream->ReadBool();
	updateFrame = stream->ReadInt();

	while ( stream->ReadBool() == true ) {
		long id = stream->ReadID();
		int triggerType = stream->ReadLong();
		checkList.SetAt( id, triggerType );
	}

	while ( stream->ReadBool() == true ) {
		int triggerType = stream->ReadLong();
		int type = stream->ReadLong();
		if ( triggerType != 0 ) {
			return false;
		}
		AddObjectTypeToCheckList( type );
	}
	return true;
}

/*
================
ProximityCheck::FixUpPointers

Whether the destination and the event ReadFromStream read as IDs are
loaded.
================
*/
// FUNCTION: STBC 0x005a67b0
bool ProximityCheck::FixUpPointers( void ) {
	if ( !ObjectClass::FixUpPointers() ) {
		return false;
	}
	if ( triggerDestination != NULL && GetTGObjectPtr( (long)triggerDestination ) == NULL ) {
		return false;
	}
	if ( triggerEvent != NULL && TGEvent::GetTGEventPtr( (long)triggerEvent ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
ProximityCheck::PostFixUp

Turns the destination's and the event's IDs back into pointers.
================
*/
// FUNCTION: STBC 0x005a6800
void ProximityCheck::PostFixUp( void ) {
	ObjectClass::PostFixUp();
	triggerDestination = (TGEventHandlerObject *)GetSavedObject( (long)triggerDestination );
	triggerEvent = TGEvent::GetSavedEvent( (long)triggerEvent );
}

// The <list> instance retail kept from this unit (the destructor erases the
// type list's nodes through it).
// FUNCTION: STBC 0x005a68e0 ?erase@?$list@U?$pair@HH@std@@V?$allocator@U?$pair@HH@std@@@2@@std@@QAE?AViterator@12@V312@@Z
