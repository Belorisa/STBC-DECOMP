/*
===============================================================================

	TGEvent.cpp

===============================================================================
*/

#include <iostream>
#include <stdio.h>

#include "TGEvent.h"
#include "TGBoolEvent.h"
#include "TGCharEvent.h"
#include "TGShortEvent.h"
#include "TGIntEvent.h"
#include "TGFloatEvent.h"
#include "TGStringEvent.h"
#include "TGVoidPtrEvent.h"
#include "TGObjPtrEvent.h"
#include "TGEventHandlerObject.h"
#include "TGStream.h"
#include "PyEmbed.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006d4d60 _$E441
// FUNCTION: STBC 0x006d4d80 _$E440
// FUNCTION: STBC 0x006d4d90 _$E32
// FUNCTION: STBC 0x006d4da0 _$E29
// FUNCTION: STBC 0x006d4db0 _$E31
// FUNCTION: STBC 0x006d4dc0 _$E30
// FUNCTION: STBC 0x006d4dd0 _$E38
// FUNCTION: STBC 0x006d4de0 _$E35
// FUNCTION: STBC 0x006d4df0 _$E37
// FUNCTION: STBC 0x006d4e00 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006d4e10 _$E78
// FUNCTION: STBC 0x006d4e20 _$E77
// FUNCTION: STBC 0x006d4e30 _$E82
// FUNCTION: STBC 0x006d4e40 _$E81
// FUNCTION: STBC 0x006d4e50 _$E93
// FUNCTION: STBC 0x006d4e60 _$E92
// FUNCTION: STBC 0x006d4e70 _$E97
// FUNCTION: STBC 0x006d4e80 _$E96
// FUNCTION: STBC 0x006d4e90 _$E101
// FUNCTION: STBC 0x006d4ea0 _$E100
// FUNCTION: STBC 0x006d4eb0 _$E105
// FUNCTION: STBC 0x006d4ec0 _$E104
// FUNCTION: STBC 0x006d4ed0 _$E229
// FUNCTION: STBC 0x006d4ee0 _$E228
// FUNCTION: STBC 0x006d4ef0 _$E171
// FUNCTION: STBC 0x006d4f00 _$E170
// FUNCTION: STBC 0x006d4f10 _$E184
// FUNCTION: STBC 0x006d4f20 _$E183
// FUNCTION: STBC 0x006d4f30 _$E234
// FUNCTION: STBC 0x006d4f40 _$E233
// FUNCTION: STBC 0x006d4f50 _$E239
// FUNCTION: STBC 0x006d4f60 _$E238
// FUNCTION: STBC 0x006d4f70 _$E243
// FUNCTION: STBC 0x006d4f80 _$E242
// FUNCTION: STBC 0x006d4f90 _$E247
// FUNCTION: STBC 0x006d4fa0 _$E246
// FUNCTION: STBC 0x006d4fb0 _$E254
// FUNCTION: STBC 0x006d4fc0 _$E253
// FUNCTION: STBC 0x006d4fd0 _$E279
// FUNCTION: STBC 0x006d4fe0 _$E278
// FUNCTION: STBC 0x006d4ff0 _$E283
// FUNCTION: STBC 0x006d5000 _$E282
// FUNCTION: STBC 0x006d5010 _$E129
// FUNCTION: STBC 0x006d5020 _$E128
// FUNCTION: STBC 0x006d5030 _$E265
// FUNCTION: STBC 0x006d5040 _$E264
// FUNCTION: STBC 0x006d5050 _$E272
// FUNCTION: STBC 0x006d5060 _$E271
// FUNCTION: STBC 0x006d5070 _$E295
// FUNCTION: STBC 0x006d5080 _$E294
// FUNCTION: STBC 0x006d5090 _$E299
// FUNCTION: STBC 0x006d50a0 _$E298
// FUNCTION: STBC 0x006d50b0 _$E339
// FUNCTION: STBC 0x006d50c0 _$E338
// FUNCTION: STBC 0x006d50d0 _$E345
// FUNCTION: STBC 0x006d50e0 _$E344
// FUNCTION: STBC 0x006d50f0 _$E287
// FUNCTION: STBC 0x006d5100 _$E286
// FUNCTION: STBC 0x006d5110 _$E291
// FUNCTION: STBC 0x006d5120 _$E290
// FUNCTION: STBC 0x006d5130 _$E303
// FUNCTION: STBC 0x006d5140 _$E302
// FUNCTION: STBC 0x006d5150 _$E307
// FUNCTION: STBC 0x006d5160 _$E306
// FUNCTION: STBC 0x006d5170 _$E311
// FUNCTION: STBC 0x006d5180 _$E310
// FUNCTION: STBC 0x006d5190 _$E315
// FUNCTION: STBC 0x006d51a0 _$E314
// FUNCTION: STBC 0x006d51b0 _$E319
// FUNCTION: STBC 0x006d51c0 _$E318
// FUNCTION: STBC 0x006d51d0 _$E323
// FUNCTION: STBC 0x006d51e0 _$E322
// FUNCTION: STBC 0x006d51f0 _$E327
// FUNCTION: STBC 0x006d5200 _$E326
// FUNCTION: STBC 0x006d5210 _$E331
// FUNCTION: STBC 0x006d5220 _$E330
// FUNCTION: STBC 0x006d5230 _$E134
// FUNCTION: STBC 0x006d5240 _$E133
// FUNCTION: STBC 0x006d5250 _$E335
// FUNCTION: STBC 0x006d5260 _$E334
// FUNCTION: STBC 0x006d5270 _$E139
// FUNCTION: STBC 0x006d5280 _$E138
// FUNCTION: STBC 0x006d5290 _$E349
// FUNCTION: STBC 0x006d52a0 _$E348
// FUNCTION: STBC 0x006d52b0 _$E353
// FUNCTION: STBC 0x006d52c0 _$E352
// FUNCTION: STBC 0x006d52d0 _$E357
// FUNCTION: STBC 0x006d52e0 _$E356
// FUNCTION: STBC 0x006d52f0 _$E361
// FUNCTION: STBC 0x006d5300 _$E360
// FUNCTION: STBC 0x006d5310 _$E148
// FUNCTION: STBC 0x006d5320 _$E147
// FUNCTION: STBC 0x006d5330 _$E365
// FUNCTION: STBC 0x006d5340 _$E364
// FUNCTION: STBC 0x006d5350 _$E369
// FUNCTION: STBC 0x006d5360 _$E368
// FUNCTION: STBC 0x006d5370 _$E373
// FUNCTION: STBC 0x006d5380 _$E372
// FUNCTION: STBC 0x006d5390 _$E154
// FUNCTION: STBC 0x006d53a0 _$E153
// FUNCTION: STBC 0x006d53b0 _$E159
// FUNCTION: STBC 0x006d53c0 _$E158
// FUNCTION: STBC 0x006d53d0 _$E189
// FUNCTION: STBC 0x006d53e0 _$E188
// FUNCTION: STBC 0x006d53f0 _$E377
// FUNCTION: STBC 0x006d5400 _$E376
// FUNCTION: STBC 0x006d5410 _$E194
// FUNCTION: STBC 0x006d5420 _$E193
// FUNCTION: STBC 0x006d5430 _$E381
// FUNCTION: STBC 0x006d5440 _$E380
// FUNCTION: STBC 0x006d5450 _$E385
// FUNCTION: STBC 0x006d5460 _$E384
// FUNCTION: STBC 0x006d5470 _$E199
// FUNCTION: STBC 0x006d5480 _$E198
// FUNCTION: STBC 0x006d5490 _$E389
// FUNCTION: STBC 0x006d54a0 _$E388
// FUNCTION: STBC 0x006d54b0 _$E164
// FUNCTION: STBC 0x006d54c0 _$E163
// FUNCTION: STBC 0x006d54d0 _$E393
// FUNCTION: STBC 0x006d54e0 _$E392
// FUNCTION: STBC 0x006d54f0 _$E397
// FUNCTION: STBC 0x006d5500 _$E396
// FUNCTION: STBC 0x006d5510 _$E204
// FUNCTION: STBC 0x006d5520 _$E203
// FUNCTION: STBC 0x006d5530 _$E209
// FUNCTION: STBC 0x006d5540 _$E208
// FUNCTION: STBC 0x006d5550 _$E401
// FUNCTION: STBC 0x006d5560 _$E400
// FUNCTION: STBC 0x006d5570 _$E405
// FUNCTION: STBC 0x006d5580 _$E404
// FUNCTION: STBC 0x006d5590 _$E409
// FUNCTION: STBC 0x006d55a0 _$E408
// FUNCTION: STBC 0x006d55b0 _$E413
// FUNCTION: STBC 0x006d55c0 _$E412
// FUNCTION: STBC 0x006d55d0 _$E417
// FUNCTION: STBC 0x006d55e0 _$E416
// FUNCTION: STBC 0x006d55f0 _$E421
// FUNCTION: STBC 0x006d5600 _$E420
// FUNCTION: STBC 0x006d5610 _$E425
// FUNCTION: STBC 0x006d5620 _$E424
// FUNCTION: STBC 0x006d5630 _$E429
// FUNCTION: STBC 0x006d5640 _$E428
// FUNCTION: STBC 0x006d5650 _$E214
// FUNCTION: STBC 0x006d5660 _$E213
// FUNCTION: STBC 0x006d5670 _$E433
// FUNCTION: STBC 0x006d5680 _$E432
// FUNCTION: STBC 0x006d5690 _$E219
// FUNCTION: STBC 0x006d56a0 _$E218
// FUNCTION: STBC 0x006d56b0 _$E224
// FUNCTION: STBC 0x006d56c0 _$E223

// DATA: STBC 0x009983a4
NiTMap<long, TGEvent *> *TGEvent::eventTable;

// DATA: STBC 0x009983a8
NiTMap<TGObject *, NiTList<TGEvent *> *> *TGEvent::objectEventTable;

// DATA: STBC 0x0095ad6c
bool TGEvent::keepEventTable = true;

/*
================
TGEvent::Cast

The object as an event; NULL when it is none.
================
*/
// FUNCTION: STBC 0x006d56d0
TGEvent *TGEvent::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TGEVENT_TYPE ) ) {
		return (TGEvent *)object;
	}
	return NULL;
}

/*
================
TGBoolEvent::Cast
================
*/
// FUNCTION: STBC 0x006d5700
TGBoolEvent *TGBoolEvent::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TGBOOLEVENT_TYPE ) ) {
		return (TGBoolEvent *)object;
	}
	return NULL;
}

/*
================
TGCharEvent::Cast
================
*/
// FUNCTION: STBC 0x006d5730
TGCharEvent *TGCharEvent::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TGCHAREVENT_TYPE ) ) {
		return (TGCharEvent *)object;
	}
	return NULL;
}

/*
================
TGShortEvent::Cast
================
*/
// FUNCTION: STBC 0x006d5760
TGShortEvent *TGShortEvent::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TGSHORTEVENT_TYPE ) ) {
		return (TGShortEvent *)object;
	}
	return NULL;
}

/*
================
TGIntEvent::Cast
================
*/
// FUNCTION: STBC 0x006d5790
TGIntEvent *TGIntEvent::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TGINTEVENT_TYPE ) ) {
		return (TGIntEvent *)object;
	}
	return NULL;
}

/*
================
TGFloatEvent::Cast
================
*/
// FUNCTION: STBC 0x006d57c0
TGFloatEvent *TGFloatEvent::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TGFLOATEVENT_TYPE ) ) {
		return (TGFloatEvent *)object;
	}
	return NULL;
}

/*
================
TGStringEvent::Cast
================
*/
// FUNCTION: STBC 0x006d57f0
TGStringEvent *TGStringEvent::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TGSTRINGEVENT_TYPE ) ) {
		return (TGStringEvent *)object;
	}
	return NULL;
}

/*
================
TGObjPtrEvent::Cast
================
*/
// FUNCTION: STBC 0x006d5820
TGObjPtrEvent *TGObjPtrEvent::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TGOBJPTREVENT_TYPE ) ) {
		return (TGObjPtrEvent *)object;
	}
	return NULL;
}

/*
================
TGEvent::AddObjectReference

Records that the event names the object, creating the table and the
object's list on first use.
================
*/
// FUNCTION: STBC 0x006d5850
// PARKED: same calls and control flow; retail keeps the table pointer in ECX from the NULL test and copies it to EDI for the inlined GetAt, this build loads it straight into ESI (RemoveObjectReferences shows the same)
void TGEvent::AddObjectReference( TGObject *object, TGEvent *event ) {
	if ( objectEventTable == NULL ) {
		objectEventTable = new NiTMap<TGObject *, NiTList<TGEvent *> *>( OBJECT_EVENT_TABLE_SIZE );
	}

	NiTList<TGEvent *> *list;
	if ( !objectEventTable->GetAt( object, list ) ) {
		list = new NiTList<TGEvent *>( OBJECT_EVENT_LIST_BLOCK );
		objectEventTable->SetAt( object, list );
	}
	list->AddTail( event );
}

/*
================
TGEvent::RemoveObjectReference

Forgets that the event names the object. The object's list goes with its
last event, and the table with its last list.
================
*/
// FUNCTION: STBC 0x006d5a80
// PARKED: instructions match but for where VC6 loads the table pointer: retail reads it into ecx and copies it to the saved register, this build reads it into the saved register
void TGEvent::RemoveObjectReference( TGObject *object, TGEvent *event ) {
	NiTList<TGEvent *> *list;
	if ( objectEventTable->GetAt( object, list ) ) {
		list->Remove( event );
		if ( list->IsEmpty() ) {
			delete list;
			objectEventTable->RemoveAt( object );
		}
	}

	if ( objectEventTable->GetCount() == 0 ) {
		delete objectEventTable;
		objectEventTable = NULL;
	}
}

/*
================
TGEvent::TGEvent
================
*/
// FUNCTION: STBC 0x006d5c00
TGEvent::TGEvent( long id ) : TGObject( id ) {
	eventType = 0;
	source = NULL;
	destination = NULL;
	timestamp = -1.0f;
	flags = 0;
	refCount = 0;
	currentTable = NULL;
	currentHandler = NULL;
	unknown24 = NULL;

	if ( eventTable == NULL ) {
		eventTable = new NiTMap<long, TGEvent *>;
	}
	AddEvent( this );
}

/*
================
TGEvent::GetObjType
================
*/
// FUNCTION: STBC 0x006d5ce0
int TGEvent::GetObjType( void ) {
	return TGEVENT_TYPE;
}

/*
================
TGEvent::GetClassName
================
*/
// FUNCTION: STBC 0x006d5d10
char *TGEvent::GetClassName( void ) {
	return "TGEvent";
}

/*
================
TGEvent::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x006d5d20
char *TGEvent::GetSwigTypeName( void ) {
	return "_p_TGEvent";
}

/*
================
TGEvent::GetPythonClassName
================
*/
// FUNCTION: STBC 0x006d5d30
char *TGEvent::GetPythonClassName( void ) {
	return "TGEventPtr";
}

/*
================
TGEvent::~TGEvent

Leaves the event table and the lists of the objects the event names.
================
*/
// FUNCTION: STBC 0x006d5d70
TGEvent::~TGEvent( void ) {
	RemoveEvent( this );

	eventType = 0;
	if ( source != NULL && destination != source ) {
		RemoveObjectReference( source, this );
	}
	source = NULL;
	if ( destination != NULL && destination != INVALID_DESTINATION ) {
		RemoveObjectReference( destination, this );
	}
	destination = NULL;
	timestamp = -1.0f;
	flags = 0;
	refCount = 0;
	unknown24 = NULL;
}

/*
================
TGEvent::SetLogged
================
*/
// FUNCTION: STBC 0x006d5e10
void TGEvent::SetLogged( bool logged ) {
	if ( logged ) {
		if ( !IsLogged() ) {
			flags |= EVENT_LOGGED;
		}
	} else if ( IsLogged() ) {
		flags &= ~EVENT_LOGGED;
	}
}

/*
================
TGEvent::SetPrivate
================
*/
// FUNCTION: STBC 0x006d5e40
void TGEvent::SetPrivate( bool isPrivate ) {
	if ( isPrivate ) {
		if ( !IsPrivate() ) {
			flags |= EVENT_PRIVATE;
		}
	} else if ( IsPrivate() ) {
		flags &= ~EVENT_PRIVATE;
	}
}

/*
================
TGEvent::SetNotSaved
================
*/
// FUNCTION: STBC 0x006d5e80
void TGEvent::SetNotSaved( bool notSaved ) {
	if ( notSaved ) {
		if ( !IsNotSaved() ) {
			flags |= EVENT_NOT_SAVED;
		}
	} else if ( IsNotSaved() ) {
		flags &= ~EVENT_NOT_SAVED;
	}
}

/*
================
TGEvent::WriteToStream

Objects are written by ID, 0 for one that is gone. The next event is
written as the first one after it that is saved.
================
*/
// FUNCTION: STBC 0x006d5ec0
bool TGEvent::WriteToStream( TGStream *stream ) {
	if ( !TGObject::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteLong( eventType );

	if ( source != NULL && GetTGObjectPtr( source->GetObjID() ) == source ) {
		stream->WriteID( source->GetObjID() );
	} else {
		stream->WriteID( 0 );
	}

	if ( destination == INVALID_DESTINATION ) {
		stream->WriteID( INVALID_DESTINATION_ID );
	} else if ( destination != NULL && GetTGObjectPtr( destination->GetObjID() ) == destination ) {
		stream->WriteID( destination->GetObjID() );
	} else {
		stream->WriteID( 0 );
	}

	stream->WriteFloat( timestamp );
	stream->WriteShort( flags );
	stream->WriteShort( refCount );

	if ( unknown24 != NULL && GetTGEventPtr( unknown24->GetObjID() ) == unknown24 ) {
		TGEvent *event = unknown24;
		while ( event != NULL && event->IsNotSaved() ) {
			event = event->unknown24;
		}
		stream->WriteID( event != NULL ? event->GetObjID() : 0 );
	} else {
		stream->WriteID( 0 );
	}
	return true;
}

/*
================
TGEvent::ReadFromStream

Keeps the objects' IDs in the pointers until PostFixUp.
================
*/
// FUNCTION: STBC 0x006d5ff0
bool TGEvent::ReadFromStream( TGStream *stream ) {
	eventType = stream->ReadLong();
	source = (TGObject *)stream->ReadID();
	destination = (TGEventHandlerObject *)stream->ReadID();
	timestamp = stream->ReadFloat();
	flags = stream->ReadShort();
	refCount = stream->ReadShort();
	unknown24 = (TGEvent *)stream->ReadID();
	return true;
}

/*
================
TGEvent::FixUpPointers

A source or destination that names no object is dropped; false while the
next event's ID names no event yet.
================
*/
// FUNCTION: STBC 0x006d6050
bool TGEvent::FixUpPointers( void ) {
	if ( source != NULL && GetTGObjectPtr( (long)source ) == NULL ) {
		source = NULL;
	}
	if ( destination != INVALID_DESTINATION && destination != NULL && GetTGObjectPtr( (long)destination ) == NULL ) {
		destination = INVALID_DESTINATION;
	}
	if ( unknown24 != NULL && GetTGEventPtr( (long)unknown24 ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
TGEvent::PostFixUp

Turns the IDs back into objects and records the event with them.
================
*/
// FUNCTION: STBC 0x006d60b0
void TGEvent::PostFixUp( void ) {
	source = GetSavedObject( (long)source );
	if ( destination != INVALID_DESTINATION ) {
		destination = (TGEventHandlerObject *)GetSavedObject( (long)destination );
	}
	unknown24 = GetSavedEvent( (long)unknown24 );

	if ( source != NULL && source != destination ) {
		AddObjectReference( source, this );
	}
	if ( destination != NULL && destination != INVALID_DESTINATION ) {
		AddObjectReference( destination, this );
	}
}

/*
================
TGEvent::WriteToNetStream

The type first, so that CreateFromNetStream can make the right class.
================
*/
// FUNCTION: STBC 0x006d6130
bool TGEvent::WriteToNetStream( TGStream *stream ) {
	stream->WriteInt( GetObjType() );
	stream->WriteInt( eventType );
	stream->WriteID( GetSavedID( source ) );
	if ( destination != INVALID_DESTINATION ) {
		stream->WriteID( GetSavedID( destination ) );
	} else {
		stream->WriteID( INVALID_DESTINATION_ID );
	}
	return true;
}

/*
================
TGEvent::ReadFromNetStream

The source and destination stay IDs.
================
*/
// FUNCTION: STBC 0x006d61c0
bool TGEvent::ReadFromNetStream( TGStream *stream ) {
	eventType = stream->ReadInt();
	source = (TGObject *)stream->ReadID();
	destination = (TGEventHandlerObject *)stream->ReadID();
	return true;
}

/*
================
TGEvent::CreateFromNetStream

An event of the type WriteToNetStream wrote, read back.
================
*/
// FUNCTION: STBC 0x006d6200
TGEvent *TGEvent::CreateFromNetStream( TGStream *stream ) {
	int type = stream->ReadInt();
	TGEvent *event = (TGEvent *)CreateObject( type, 0 );
	event->ReadFromNetStream( stream );
	return event;
}

/*
================
TGEvent::Copy

Everything but the ID, the reference count and the next event.
================
*/
// FUNCTION: STBC 0x006d6230
bool TGEvent::Copy( TGEvent *event ) {
	eventType = event->eventType;
	SetSource( event->source );
	SetDestination( event->destination );
	timestamp = event->timestamp;
	flags = event->flags;
	return true;
}

/*
================
TGEvent::SetSource
================
*/
// FUNCTION: STBC 0x006d6270
void TGEvent::SetSource( TGObject *source ) {
	if ( this->source != NULL && this->source != destination ) {
		RemoveObjectReference( this->source, this );
	}
	this->source = source;
	if ( source != NULL && source != destination ) {
		AddObjectReference( source, this );
	}
}

/*
================
TGEvent::SetDestination
================
*/
// FUNCTION: STBC 0x006d62b0
void TGEvent::SetDestination( TGEventHandlerObject *destination ) {
	if ( this->destination != NULL && this->destination != INVALID_DESTINATION && this->destination != source ) {
		RemoveObjectReference( this->destination, this );
	}
	this->destination = destination;
	if ( destination != NULL && destination != INVALID_DESTINATION && destination != source ) {
		AddObjectReference( destination, this );
	}
}

/*
================
TGEvent::DecRefCount

The event deletes itself when the count reaches 0.
================
*/
// FUNCTION: STBC 0x006d6300
void TGEvent::DecRefCount( void ) {
	refCount--;
	if ( refCount <= 0 ) {
		refCount = 0;
		delete this;
	}
}

/*
================
TGEvent::RemoveObjectReferences

Called by the object's destructor: an event whose destination is the
object goes to INVALID_DESTINATION instead.
================
*/
// FUNCTION: STBC 0x006d6320
// PARKED: same calls and control flow; retail loads the table into ECX before saving registers and copies it to EDI for GetAt; early returns and a combined condition give the same EDI load
void TGEvent::RemoveObjectReferences( TGObject *object ) {
	NiTList<TGEvent *> *list;
	if ( objectEventTable != NULL && objectEventTable->GetAt( object, list ) ) {
		NiTListIterator pos = list->GetHeadPos();
		while ( pos != NULL ) {
			TGEvent *event = list->GetNext( pos );
			if ( event->source == object ) {
				event->SetSource( NULL );
			}
			if ( event->destination == object ) {
				event->SetDestination( INVALID_DESTINATION );
			}
		}
	}
}

/*
================
TGEvent::GetTGEventPtr

The event with this ID; NULL when there is none.
================
*/
// FUNCTION: STBC 0x006d63a0
TGEvent *TGEvent::GetTGEventPtr( long id ) {
	TGEvent *event;
	if ( eventTable->GetAt( id, event ) ) {
		return event;
	}
	return NULL;
}

/*
================
TGEvent::AddEvent
================
*/
// FUNCTION: STBC 0x006d63f0
bool TGEvent::AddEvent( TGEvent *event ) {
	eventTable->SetAt( event->id, event );
	return true;
}

/*
================
TGEvent::RemoveEvent

False when the event was not registered. Once shutdown has begun
(ReleaseEventTable), the last removal deletes the table.
================
*/
// FUNCTION: STBC 0x006d6480
bool TGEvent::RemoveEvent( TGEvent *event ) {
	bool removed = eventTable->RemoveAt( event->id );
	if ( !keepEventTable && eventTable != NULL && eventTable->GetCount() == 0 ) {
		delete eventTable;
		eventTable = NULL;
	}
	return removed;
}

/*
================
TGEvent::ReleaseEventTable

Shutdown: the table goes now if it is empty, else with its last event.
================
*/
// FUNCTION: STBC 0x006d6550
void TGEvent::ReleaseEventTable( void ) {
	keepEventTable = false;
	if ( eventTable != NULL && eventTable->GetCount() == 0 ) {
		delete eventTable;
		eventTable = NULL;
	}
}

/*
================
TGEvent::RemoveAllEvents

Forgets every event without deleting any.
================
*/
// FUNCTION: STBC 0x006d6580
void TGEvent::RemoveAllEvents( void ) {
	eventTable->RemoveAll();
}

/*
================
TGEvent::SaveEvents

Writes the count, then every event that is saved; false when one of them
failed.
================
*/
// FUNCTION: STBC 0x006d65e0
bool TGEvent::SaveEvents( TGStream *stream ) {
	char text[256];
	bool saved = true;

	int count = 0;
	NiTMapIterator pos = eventTable->GetFirstPos();
	while ( pos != NULL ) {
		long id;
		TGEvent *event;
		eventTable->GetNext( pos, id, event );
		if ( !event->IsNotSaved() ) {
			count++;
		}
	}

	stream->WriteInt( count );
	sprintf( text, "\nSaving %d events\n\n", count );
	Log( text );

	pos = eventTable->GetFirstPos();
	int number = 0;
	while ( pos != NULL ) {
		long id;
		TGEvent *event;
		eventTable->GetNext( pos, id, event );
		if ( !event->IsNotSaved() ) {
			sprintf( text, "Saving %d ", number++ );
			Log( text );
			if ( !event->WriteToStream( stream ) ) {
				saved = false;
			}
		}
	}

	Log( "\nDone saving events\n\n" );
	return saved;
}

/*
================
TGEvent::LoadEvents

Reads what SaveEvents wrote; false at the first event that fails.
================
*/
// FUNCTION: STBC 0x006d6780
bool TGEvent::LoadEvents( TGStream *stream ) {
	char text[256];

	int count = stream->ReadInt();
	sprintf( text, "\nLoading %d events\n\n", count );
	Log( text );

	for ( int i = 0; i < count; i++ ) {
		sprintf( text, "Loading %d ", i );
		Log( text );
		if ( LoadObject( stream, 0 ) == NULL ) {
			return false;
		}
	}

	Log( "\nDone loading events\n\n" );
	return true;
}

/*
================
TGEvent::Duplicate

A new event of the same class with the same contents.
================
*/
// FUNCTION: STBC 0x006d6810
TGEvent *TGEvent::Duplicate( void ) {
	TGEvent *event = (TGEvent *)CreateObject( GetObjType(), 0 );
	if ( event != NULL ) {
		event->Copy( this );
	}
	return event;
}

/*
================
TGBoolEvent::WriteToNetStream
================
*/
// FUNCTION: STBC 0x006d6840
bool TGBoolEvent::WriteToNetStream( TGStream *stream ) {
	TGEvent::WriteToNetStream( stream );
	stream->WriteChar( GetBool() == true );
	return true;
}

/*
================
TGBoolEvent::ReadFromNetStream
================
*/
// FUNCTION: STBC 0x006d6870
bool TGBoolEvent::ReadFromNetStream( TGStream *stream ) {
	TGEvent::ReadFromNetStream( stream );
	SetBool( stream->ReadChar() == true );
	return true;
}

/*
================
TGBoolEvent::WriteToStream
================
*/
// FUNCTION: STBC 0x006d68a0
bool TGBoolEvent::WriteToStream( TGStream *stream ) {
	if ( !TGEvent::WriteToStream( stream ) ) {
		return false;
	}
	if ( GetBool() ) {
		stream->WriteChar( true );
	} else {
		stream->WriteChar( false );
	}
	return true;
}

/*
================
TGBoolEvent::ReadFromStream
================
*/
// FUNCTION: STBC 0x006d68e0
bool TGBoolEvent::ReadFromStream( TGStream *stream ) {
	if ( !TGEvent::ReadFromStream( stream ) ) {
		return false;
	}
	SetBool( stream->ReadChar() == true );
	return true;
}

/*
================
TGCharEvent::Copy
================
*/
// FUNCTION: STBC 0x006d6920
bool TGCharEvent::Copy( TGEvent *event ) {
	bool copied = TGEvent::Copy( event );
	if ( copied == true ) {
		value = ( (TGCharEvent *)event )->value;
	}
	return copied;
}

/*
================
TGCharEvent::WriteToNetStream
================
*/
// FUNCTION: STBC 0x006d6940
bool TGCharEvent::WriteToNetStream( TGStream *stream ) {
	TGEvent::WriteToNetStream( stream );
	stream->WriteChar( value );
	return true;
}

/*
================
TGCharEvent::ReadFromNetStream
================
*/
// FUNCTION: STBC 0x006d6960
bool TGCharEvent::ReadFromNetStream( TGStream *stream ) {
	TGEvent::ReadFromNetStream( stream );
	value = stream->ReadChar();
	return true;
}

/*
================
TGCharEvent::WriteToStream
================
*/
// FUNCTION: STBC 0x006d6980
bool TGCharEvent::WriteToStream( TGStream *stream ) {
	if ( !TGEvent::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteChar( value );
	return true;
}

/*
================
TGCharEvent::ReadFromStream
================
*/
// FUNCTION: STBC 0x006d69b0
bool TGCharEvent::ReadFromStream( TGStream *stream ) {
	if ( !TGEvent::ReadFromStream( stream ) ) {
		return false;
	}
	value = stream->ReadChar();
	return true;
}

/*
================
TGShortEvent::Copy
================
*/
// FUNCTION: STBC 0x006d69e0
bool TGShortEvent::Copy( TGEvent *event ) {
	bool copied = TGEvent::Copy( event );
	if ( copied == true ) {
		value = ( (TGShortEvent *)event )->value;
	}
	return copied;
}

/*
================
TGShortEvent::WriteToNetStream
================
*/
// FUNCTION: STBC 0x006d6a00
bool TGShortEvent::WriteToNetStream( TGStream *stream ) {
	TGEvent::WriteToNetStream( stream );
	stream->WriteShort( value );
	return true;
}

/*
================
TGShortEvent::ReadFromNetStream
================
*/
// FUNCTION: STBC 0x006d6a30
bool TGShortEvent::ReadFromNetStream( TGStream *stream ) {
	TGEvent::ReadFromNetStream( stream );
	value = stream->ReadShort();
	return true;
}

/*
================
TGShortEvent::WriteToStream
================
*/
// FUNCTION: STBC 0x006d6a50
bool TGShortEvent::WriteToStream( TGStream *stream ) {
	if ( !TGEvent::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteShort( value );
	return true;
}

/*
================
TGShortEvent::ReadFromStream
================
*/
// FUNCTION: STBC 0x006d6a80
bool TGShortEvent::ReadFromStream( TGStream *stream ) {
	if ( !TGEvent::ReadFromStream( stream ) ) {
		return false;
	}
	value = stream->ReadShort();
	return true;
}

/*
================
TGIntEvent::Copy
================
*/
// FUNCTION: STBC 0x006d6ab0
bool TGIntEvent::Copy( TGEvent *event ) {
	bool copied = TGEvent::Copy( event );
	if ( copied == true ) {
		value = ( (TGIntEvent *)event )->value;
	}
	return copied;
}

/*
================
TGIntEvent::WriteToNetStream
================
*/
// FUNCTION: STBC 0x006d6ad0
bool TGIntEvent::WriteToNetStream( TGStream *stream ) {
	TGEvent::WriteToNetStream( stream );
	stream->WriteInt( value );
	return true;
}

/*
================
TGIntEvent::ReadFromNetStream
================
*/
// FUNCTION: STBC 0x006d6af0
bool TGIntEvent::ReadFromNetStream( TGStream *stream ) {
	TGEvent::ReadFromNetStream( stream );
	value = stream->ReadInt();
	return true;
}

/*
================
TGIntEvent::WriteToStream
================
*/
// FUNCTION: STBC 0x006d6b10
bool TGIntEvent::WriteToStream( TGStream *stream ) {
	if ( !TGEvent::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteLong( value );
	return true;
}

/*
================
TGIntEvent::ReadFromStream
================
*/
// FUNCTION: STBC 0x006d6b40
bool TGIntEvent::ReadFromStream( TGStream *stream ) {
	if ( !TGEvent::ReadFromStream( stream ) ) {
		return false;
	}
	value = stream->ReadLong();
	return true;
}

/*
================
TGFloatEvent::Copy
================
*/
// FUNCTION: STBC 0x006d6b70
bool TGFloatEvent::Copy( TGEvent *event ) {
	bool copied = TGEvent::Copy( event );
	if ( copied == true ) {
		value = ( (TGFloatEvent *)event )->value;
	}
	return copied;
}

/*
================
TGFloatEvent::WriteToNetStream
================
*/
// FUNCTION: STBC 0x006d6b90
bool TGFloatEvent::WriteToNetStream( TGStream *stream ) {
	TGEvent::WriteToNetStream( stream );
	stream->WriteFloat( GetFloat() );
	return true;
}

/*
================
TGFloatEvent::ReadFromNetStream
================
*/
// FUNCTION: STBC 0x006d6bc0
bool TGFloatEvent::ReadFromNetStream( TGStream *stream ) {
	TGEvent::ReadFromNetStream( stream );
	value = stream->ReadFloat();
	return true;
}

/*
================
TGFloatEvent::WriteToStream
================
*/
// FUNCTION: STBC 0x006d6be0
bool TGFloatEvent::WriteToStream( TGStream *stream ) {
	if ( !TGEvent::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( value );
	return true;
}

/*
================
TGFloatEvent::ReadFromStream
================
*/
// FUNCTION: STBC 0x006d6c10
bool TGFloatEvent::ReadFromStream( TGStream *stream ) {
	if ( !TGEvent::ReadFromStream( stream ) ) {
		return false;
	}
	value = stream->ReadFloat();
	return true;
}

/*
================
TGStringEvent::Copy
================
*/
// FUNCTION: STBC 0x006d6c40
bool TGStringEvent::Copy( TGEvent *event ) {
	bool copied = TGEvent::Copy( event );
	if ( copied == true ) {
		value = ( (TGStringEvent *)event )->value;
	}
	return copied;
}

/*
================
TGStringEvent::WriteToNetStream
================
*/
// FUNCTION: STBC 0x006d6c70
bool TGStringEvent::WriteToNetStream( TGStream *stream ) {
	TGEvent::WriteToNetStream( stream );
	value.WriteToStream( stream );
	return true;
}

/*
================
TGStringEvent::ReadFromNetStream
================
*/
// FUNCTION: STBC 0x006d6c90
bool TGStringEvent::ReadFromNetStream( TGStream *stream ) {
	TGEvent::ReadFromNetStream( stream );
	value.ReadFromStream( stream );
	return true;
}

/*
================
TGStringEvent::WriteToStream
================
*/
// FUNCTION: STBC 0x006d6cb0
bool TGStringEvent::WriteToStream( TGStream *stream ) {
	if ( !TGEvent::WriteToStream( stream ) ) {
		return false;
	}
	if ( !value.WriteToStream( stream ) ) {
		return false;
	}
	return true;
}

/*
================
TGStringEvent::ReadFromStream
================
*/
// FUNCTION: STBC 0x006d6ce0
bool TGStringEvent::ReadFromStream( TGStream *stream ) {
	if ( !TGEvent::ReadFromStream( stream ) ) {
		return false;
	}
	if ( !value.ReadFromStream( stream ) ) {
		return false;
	}
	return true;
}

/*
================
TGStringEvent_GetCString

The event in the tuple's first item; a new string, empty when the event
has no string. NULL when there is no event.
================
*/
// FUNCTION: STBC 0x006d6d10
PyObject *TGStringEvent_GetCString( PyObject *self, PyObject *args ) {
	PyObject *result = NULL;
	PyObject *eventInstance = PyTuple_GetItem( args, 0 );
	if ( eventInstance != NULL ) {
		TGStringEvent *event = NULL;
		PyEmbed::Unknown6F98E0( eventInstance, (void **)&event, "TGStringEvent" );
		if ( event != NULL ) {
			char *string = event->GetCString();
			if ( string != NULL ) {
				result = PyString_FromString( string );
			} else {
				result = PyString_FromString( "" );
			}
			Py_INCREF( result );	// BUG: the new string already holds the caller's reference; this one leaks
			delete string;
		}
	}
	return result;
}

/*
================
TGVoidPtrEvent::Copy
================
*/
// FUNCTION: STBC 0x006d6d80
bool TGVoidPtrEvent::Copy( TGEvent *event ) {
	bool copied = TGEvent::Copy( event );
	if ( copied == true ) {
		value = ( (TGVoidPtrEvent *)event )->value;
	}
	return copied;
}

/*
================
TGObjPtrEvent::Copy
================
*/
// FUNCTION: STBC 0x006d6da0
bool TGObjPtrEvent::Copy( TGEvent *event ) {
	bool copied = TGEvent::Copy( event );
	if ( copied == true ) {
		objectID = ( (TGObjPtrEvent *)event )->objectID;
	}
	return copied;
}

/*
================
TGObjPtrEvent::WriteToNetStream
================
*/
// FUNCTION: STBC 0x006d6dc0
bool TGObjPtrEvent::WriteToNetStream( TGStream *stream ) {
	TGEvent::WriteToNetStream( stream );
	stream->WriteID( objectID );
	return true;
}

/*
================
TGObjPtrEvent::ReadFromNetStream
================
*/
// FUNCTION: STBC 0x006d6df0
bool TGObjPtrEvent::ReadFromNetStream( TGStream *stream ) {
	TGEvent::ReadFromNetStream( stream );
	objectID = stream->ReadID();
	return true;
}

/*
================
TGObjPtrEvent::WriteToStream
================
*/
// FUNCTION: STBC 0x006d6e20
bool TGObjPtrEvent::WriteToStream( TGStream *stream ) {
	if ( !TGEvent::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( objectID );
	return true;
}

/*
================
TGObjPtrEvent::ReadFromStream
================
*/
// FUNCTION: STBC 0x006d6e50
bool TGObjPtrEvent::ReadFromStream( TGStream *stream ) {
	if ( !TGEvent::ReadFromStream( stream ) ) {
		return false;
	}
	objectID = stream->ReadID();
	return true;
}

/*
================
TGEvent::DeleteAllEvents

Releases every reference to every event, then empties the table.
================
*/
// FUNCTION: STBC 0x006d6e80
void TGEvent::DeleteAllEvents( void ) {
	NiTMapIterator pos = eventTable->GetFirstPos();
	while ( pos != NULL ) {
		long id;
		TGEvent *event;
		eventTable->GetNext( pos, id, event );
		if ( event != NULL ) {
			short refs;
			do {
				refs = event->refCount;
				event->DecRefCount();
			} while ( refs > 1 );
		}
	}
	RemoveAllEvents();
}
