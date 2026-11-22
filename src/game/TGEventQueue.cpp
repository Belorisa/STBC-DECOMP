/*
===============================================================================

	TGEventQueue.cpp

===============================================================================
*/

#include <iostream>
#include "TGEventManager.h"
#include "TGEvent.h"
#include "TGStream.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006dd840 _$E422
// FUNCTION: STBC 0x006dd860 _$E421
// FUNCTION: STBC 0x006dd870 _$E32
// FUNCTION: STBC 0x006dd880 _$E29
// FUNCTION: STBC 0x006dd890 _$E31
// FUNCTION: STBC 0x006dd8a0 _$E30
// FUNCTION: STBC 0x006dd8b0 _$E38
// FUNCTION: STBC 0x006dd8c0 _$E35
// FUNCTION: STBC 0x006dd8d0 _$E37
// FUNCTION: STBC 0x006dd8e0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006dd8f0 _$E59
// FUNCTION: STBC 0x006dd900 _$E58
// FUNCTION: STBC 0x006dd910 _$E63
// FUNCTION: STBC 0x006dd920 _$E62
// FUNCTION: STBC 0x006dd930 _$E74
// FUNCTION: STBC 0x006dd940 _$E73
// FUNCTION: STBC 0x006dd950 _$E78
// FUNCTION: STBC 0x006dd960 _$E77
// FUNCTION: STBC 0x006dd970 _$E82
// FUNCTION: STBC 0x006dd980 _$E81
// FUNCTION: STBC 0x006dd990 _$E86
// FUNCTION: STBC 0x006dd9a0 _$E85
// FUNCTION: STBC 0x006dd9b0 _$E210
// FUNCTION: STBC 0x006dd9c0 _$E209
// FUNCTION: STBC 0x006dd9d0 _$E152
// FUNCTION: STBC 0x006dd9e0 _$E151
// FUNCTION: STBC 0x006dd9f0 _$E165
// FUNCTION: STBC 0x006dda00 _$E164
// FUNCTION: STBC 0x006dda10 _$E215
// FUNCTION: STBC 0x006dda20 _$E214
// FUNCTION: STBC 0x006dda30 _$E220
// FUNCTION: STBC 0x006dda40 _$E219
// FUNCTION: STBC 0x006dda50 _$E224
// FUNCTION: STBC 0x006dda60 _$E223
// FUNCTION: STBC 0x006dda70 _$E228
// FUNCTION: STBC 0x006dda80 _$E227
// FUNCTION: STBC 0x006dda90 _$E235
// FUNCTION: STBC 0x006ddaa0 _$E234
// FUNCTION: STBC 0x006ddab0 _$E260
// FUNCTION: STBC 0x006ddac0 _$E259
// FUNCTION: STBC 0x006ddad0 _$E264
// FUNCTION: STBC 0x006ddae0 _$E263
// FUNCTION: STBC 0x006ddaf0 _$E110
// FUNCTION: STBC 0x006ddb00 _$E109
// FUNCTION: STBC 0x006ddb10 _$E246
// FUNCTION: STBC 0x006ddb20 _$E245
// FUNCTION: STBC 0x006ddb30 _$E253
// FUNCTION: STBC 0x006ddb40 _$E252
// FUNCTION: STBC 0x006ddb50 _$E276
// FUNCTION: STBC 0x006ddb60 _$E275
// FUNCTION: STBC 0x006ddb70 _$E280
// FUNCTION: STBC 0x006ddb80 _$E279
// FUNCTION: STBC 0x006ddb90 _$E320
// FUNCTION: STBC 0x006ddba0 _$E319
// FUNCTION: STBC 0x006ddbb0 _$E326
// FUNCTION: STBC 0x006ddbc0 _$E325
// FUNCTION: STBC 0x006ddbd0 _$E268
// FUNCTION: STBC 0x006ddbe0 _$E267
// FUNCTION: STBC 0x006ddbf0 _$E272
// FUNCTION: STBC 0x006ddc00 _$E271
// FUNCTION: STBC 0x006ddc10 _$E284
// FUNCTION: STBC 0x006ddc20 _$E283
// FUNCTION: STBC 0x006ddc30 _$E288
// FUNCTION: STBC 0x006ddc40 _$E287
// FUNCTION: STBC 0x006ddc50 _$E292
// FUNCTION: STBC 0x006ddc60 _$E291
// FUNCTION: STBC 0x006ddc70 _$E296
// FUNCTION: STBC 0x006ddc80 _$E295
// FUNCTION: STBC 0x006ddc90 _$E300
// FUNCTION: STBC 0x006ddca0 _$E299
// FUNCTION: STBC 0x006ddcb0 _$E304
// FUNCTION: STBC 0x006ddcc0 _$E303
// FUNCTION: STBC 0x006ddcd0 _$E308
// FUNCTION: STBC 0x006ddce0 _$E307
// FUNCTION: STBC 0x006ddcf0 _$E312
// FUNCTION: STBC 0x006ddd00 _$E311
// FUNCTION: STBC 0x006ddd10 _$E115
// FUNCTION: STBC 0x006ddd20 _$E114
// FUNCTION: STBC 0x006ddd30 _$E316
// FUNCTION: STBC 0x006ddd40 _$E315
// FUNCTION: STBC 0x006ddd50 _$E120
// FUNCTION: STBC 0x006ddd60 _$E119
// FUNCTION: STBC 0x006ddd70 _$E330
// FUNCTION: STBC 0x006ddd80 _$E329
// FUNCTION: STBC 0x006ddd90 _$E334
// FUNCTION: STBC 0x006ddda0 _$E333
// FUNCTION: STBC 0x006dddb0 _$E338
// FUNCTION: STBC 0x006dddc0 _$E337
// FUNCTION: STBC 0x006dddd0 _$E342
// FUNCTION: STBC 0x006ddde0 _$E341
// FUNCTION: STBC 0x006dddf0 _$E129
// FUNCTION: STBC 0x006dde00 _$E128
// FUNCTION: STBC 0x006dde10 _$E346
// FUNCTION: STBC 0x006dde20 _$E345
// FUNCTION: STBC 0x006dde30 _$E350
// FUNCTION: STBC 0x006dde40 _$E349
// FUNCTION: STBC 0x006dde50 _$E354
// FUNCTION: STBC 0x006dde60 _$E353
// FUNCTION: STBC 0x006dde70 _$E135
// FUNCTION: STBC 0x006dde80 _$E134
// FUNCTION: STBC 0x006dde90 _$E140
// FUNCTION: STBC 0x006ddea0 _$E139
// FUNCTION: STBC 0x006ddeb0 _$E170
// FUNCTION: STBC 0x006ddec0 _$E169
// FUNCTION: STBC 0x006dded0 _$E358
// FUNCTION: STBC 0x006ddee0 _$E357
// FUNCTION: STBC 0x006ddef0 _$E175
// FUNCTION: STBC 0x006ddf00 _$E174
// FUNCTION: STBC 0x006ddf10 _$E362
// FUNCTION: STBC 0x006ddf20 _$E361
// FUNCTION: STBC 0x006ddf30 _$E366
// FUNCTION: STBC 0x006ddf40 _$E365
// FUNCTION: STBC 0x006ddf50 _$E180
// FUNCTION: STBC 0x006ddf60 _$E179
// FUNCTION: STBC 0x006ddf70 _$E370
// FUNCTION: STBC 0x006ddf80 _$E369
// FUNCTION: STBC 0x006ddf90 _$E145
// FUNCTION: STBC 0x006ddfa0 _$E144
// FUNCTION: STBC 0x006ddfb0 _$E374
// FUNCTION: STBC 0x006ddfc0 _$E373
// FUNCTION: STBC 0x006ddfd0 _$E378
// FUNCTION: STBC 0x006ddfe0 _$E377
// FUNCTION: STBC 0x006ddff0 _$E185
// FUNCTION: STBC 0x006de000 _$E184
// FUNCTION: STBC 0x006de010 _$E190
// FUNCTION: STBC 0x006de020 _$E189
// FUNCTION: STBC 0x006de030 _$E382
// FUNCTION: STBC 0x006de040 _$E381
// FUNCTION: STBC 0x006de050 _$E386
// FUNCTION: STBC 0x006de060 _$E385
// FUNCTION: STBC 0x006de070 _$E390
// FUNCTION: STBC 0x006de080 _$E389
// FUNCTION: STBC 0x006de090 _$E394
// FUNCTION: STBC 0x006de0a0 _$E393
// FUNCTION: STBC 0x006de0b0 _$E398
// FUNCTION: STBC 0x006de0c0 _$E397
// FUNCTION: STBC 0x006de0d0 _$E402
// FUNCTION: STBC 0x006de0e0 _$E401
// FUNCTION: STBC 0x006de0f0 _$E406
// FUNCTION: STBC 0x006de100 _$E405
// FUNCTION: STBC 0x006de110 _$E410
// FUNCTION: STBC 0x006de120 _$E409
// FUNCTION: STBC 0x006de130 _$E195
// FUNCTION: STBC 0x006de140 _$E194
// FUNCTION: STBC 0x006de150 _$E414
// FUNCTION: STBC 0x006de160 _$E413
// FUNCTION: STBC 0x006de170 _$E200
// FUNCTION: STBC 0x006de180 _$E199
// FUNCTION: STBC 0x006de190 _$E205
// FUNCTION: STBC 0x006de1a0 _$E204

/*
================
TGEventQueue::TGEventQueue
================
*/
// FUNCTION: STBC 0x006de1b0
TGEventQueue::TGEventQueue( void ) {
	head = NULL;
	tail = NULL;
	count = 0;
}

/*
================
TGEventQueue::~TGEventQueue
================
*/
// FUNCTION: STBC 0x006de1c0
TGEventQueue::~TGEventQueue( void ) {
	Unknown6DE310();
}

/*
================
TGEventQueue::WriteToStream

The first event's ID, the last saved event's ID and the number of events that are saved.
================
*/
// FUNCTION: STBC 0x006de1d0
bool TGEventQueue::WriteToStream( TGStream *stream ) {
	int saved = 0;
	TGEvent *first = head;
	// BUG: spins forever when the first event is not saved; it was meant to skip them
	while ( first != NULL && first->IsNotSaved() ) {
	}
	TGEvent *last = first;
	TGEvent *event;
	for ( event = first; event != NULL; event = event->unknown24 ) {
		if ( !event->IsNotSaved() ) {
			last = event;
			saved++;
		}
	}
	stream->WriteID( first != NULL ? first->GetObjID() : 0 );
	stream->WriteID( last != NULL ? last->GetObjID() : 0 );
	stream->WriteInt( saved );
	return true;
}

/*
================
TGEventQueue::ReadFromStream

The events are IDs until PostFixUp.
================
*/
// FUNCTION: STBC 0x006de240
bool TGEventQueue::ReadFromStream( TGStream *stream ) {
	Unknown6DE310();
	head = (TGEvent *)stream->ReadID();
	tail = (TGEvent *)stream->ReadID();
	count = stream->ReadInt();
	return true;
}

/*
================
TGEventQueue::FixUpPointers

False while an ID names no event yet.
================
*/
// FUNCTION: STBC 0x006de280
bool TGEventQueue::FixUpPointers( void ) {
	if ( head != NULL && TGEvent::GetTGEventPtr( (long)head ) == NULL ) {
		return false;
	}
	if ( tail != NULL && TGEvent::GetTGEventPtr( (long)tail ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
TGEventQueue::PostFixUp
================
*/
// FUNCTION: STBC 0x006de2c0
void TGEventQueue::PostFixUp( void ) {
	head = head != NULL ? TGEvent::GetTGEventPtr( (long)head ) : NULL;
	tail = tail != NULL ? TGEvent::GetTGEventPtr( (long)tail ) : NULL;
}

/*
================
TGEventQueue::Unknown6DE300

Empties the queue without dropping the references.
================
*/
// FUNCTION: STBC 0x006de300
void TGEventQueue::Unknown6DE300( void ) {
	head = NULL;
	tail = NULL;
	count = 0;
}

/*
================
TGEventQueue::Unknown6DE310

Drops every event.
================
*/
// FUNCTION: STBC 0x006de310
void TGEventQueue::Unknown6DE310( void ) {
	while ( count != 0 ) {
		Unknown6DE370();
	}
}

/*
================
TGEventQueue::Unknown6DE330

Appends the event; the queue holds a reference to it.
================
*/
// FUNCTION: STBC 0x006de330
bool TGEventQueue::Unknown6DE330( TGEvent *event ) {
	event->IncRefCount();
	if ( tail != NULL ) {
		tail->unknown24 = event;
		tail = event;
		event->unknown24 = NULL;
		count++;
		return true;
	}
	tail = event;
	head = event;
	event->unknown24 = NULL;
	count++;
	return true;
}

/*
================
TGEventQueue::Unknown6DE370

Drops the first event and the queue's reference to it.
================
*/
// FUNCTION: STBC 0x006de370
void TGEventQueue::Unknown6DE370( void ) {
	if ( head != NULL ) {
		TGEvent *event = head;
		head = event->unknown24;
		count--;
		event->DecRefCount();
		if ( count == 0 ) {
			head = NULL;
			tail = NULL;
		}
	}
}

/*
================
TGEventQueue::Unknown6DE3A0

Takes the event with the ID out of the queue and drops the queue's
reference to it; false when the queue does not hold it.
================
*/
// FUNCTION: STBC 0x006de3a0
bool TGEventQueue::Unknown6DE3A0( long eventID ) {
	if ( head != NULL ) {
		TGEvent *previous = NULL;
		TGEvent *current = head;
		while ( current->GetObjID() != eventID && current->unknown24 != NULL ) {
			previous = current;
			current = current->unknown24;
		}
		if ( current->GetObjID() == eventID ) {
			if ( head == current ) {
				head = current->unknown24;
			}
			if ( tail == current ) {
				tail = previous;
			}
			if ( previous != NULL ) {
				previous->unknown24 = current->unknown24;
			}
			current->DecRefCount();
			count--;
			return true;
		}
	}
	return false;
}

/*
================
TGEventQueue::RemoveEvent
================
*/
// FUNCTION: STBC 0x006de410
bool TGEventQueue::RemoveEvent( TGEvent *event ) {
	return Unknown6DE3A0( event->GetObjID() );
}

/*
================
TGEventQueue::RemoveEvents

Takes out the events sent to the destination, of the type when it is not 0;
true when there was one.
================
*/
// FUNCTION: STBC 0x006de420
bool TGEventQueue::RemoveEvents( TGEventHandlerObject *destination, int eventType ) {
	bool removed = false;
	if ( count == 0 ) {
		return false;
	}
	TGEvent *previous = NULL;
	TGEvent *current = head;
	while ( current != NULL ) {
		while ( current != NULL && current->destination != destination ) {
			previous = current;
			current = current->unknown24;
		}
		if ( current != NULL ) {
			if ( eventType != 0 && current->eventType != eventType ) {
				// BUG: previous is not moved on, so a later removal unlinks from the wrong event
				current = current->unknown24;
			} else {
				if ( head == current ) {
					head = current->unknown24;
				}
				if ( tail == current ) {
					tail = previous;
				}
				if ( previous != NULL ) {
					previous->unknown24 = current->unknown24;
				}
				current->DecRefCount();
				count--;
				if ( previous != NULL ) {
					current = previous;
				} else {
					current = head;
				}
				removed = true;
			}
		}
	}
	return removed;
}

/*
================
TGEventQueue::ClearSource

Clears the source of every event that has this one; true when there was one.
================
*/
// FUNCTION: STBC 0x006de4b0
bool TGEventQueue::ClearSource( TGObject *source ) {
	bool found = false;
	TGEvent *event = head;
	while ( event != NULL ) {
		while ( event != NULL && event->source != source ) {
			event = event->unknown24;
		}
		if ( event != NULL ) {
			event->SetSource( NULL );
			found = true;
		}
	}
	return found;
}
