/*
===============================================================================

	TGTimerList.cpp

	A unit of its own: retail's <iostream> initialisers (0x6DE4F0) and class
	registrations (0x6DE5A0) open it, between TGEventQueue's code and
	TGTimerList's.

===============================================================================
*/

#include <iostream>
#include "TGTimerManager.h"
#include "TGTimer.h"
#include "TGEvent.h"
#include "TGEventManager.h"
#include "TGStream.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006de4f0 _$E424
// FUNCTION: STBC 0x006de510 _$E423
// FUNCTION: STBC 0x006de520 _$E32
// FUNCTION: STBC 0x006de530 _$E29
// FUNCTION: STBC 0x006de540 _$E31
// FUNCTION: STBC 0x006de550 _$E30
// FUNCTION: STBC 0x006de560 _$E38
// FUNCTION: STBC 0x006de570 _$E35
// FUNCTION: STBC 0x006de580 _$E37
// FUNCTION: STBC 0x006de590 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006de5a0 _$E61
// FUNCTION: STBC 0x006de5b0 _$E60
// FUNCTION: STBC 0x006de5c0 _$E65
// FUNCTION: STBC 0x006de5d0 _$E64
// FUNCTION: STBC 0x006de5e0 _$E76
// FUNCTION: STBC 0x006de5f0 _$E75
// FUNCTION: STBC 0x006de600 _$E80
// FUNCTION: STBC 0x006de610 _$E79
// FUNCTION: STBC 0x006de620 _$E84
// FUNCTION: STBC 0x006de630 _$E83
// FUNCTION: STBC 0x006de640 _$E88
// FUNCTION: STBC 0x006de650 _$E87
// FUNCTION: STBC 0x006de660 _$E212
// FUNCTION: STBC 0x006de670 _$E211
// FUNCTION: STBC 0x006de680 _$E154
// FUNCTION: STBC 0x006de690 _$E153
// FUNCTION: STBC 0x006de6a0 _$E167
// FUNCTION: STBC 0x006de6b0 _$E166
// FUNCTION: STBC 0x006de6c0 _$E217
// FUNCTION: STBC 0x006de6d0 _$E216
// FUNCTION: STBC 0x006de6e0 _$E222
// FUNCTION: STBC 0x006de6f0 _$E221
// FUNCTION: STBC 0x006de700 _$E226
// FUNCTION: STBC 0x006de710 _$E225
// FUNCTION: STBC 0x006de720 _$E230
// FUNCTION: STBC 0x006de730 _$E229
// FUNCTION: STBC 0x006de740 _$E237
// FUNCTION: STBC 0x006de750 _$E236
// FUNCTION: STBC 0x006de760 _$E262
// FUNCTION: STBC 0x006de770 _$E261
// FUNCTION: STBC 0x006de780 _$E266
// FUNCTION: STBC 0x006de790 _$E265
// FUNCTION: STBC 0x006de7a0 _$E112
// FUNCTION: STBC 0x006de7b0 _$E111
// FUNCTION: STBC 0x006de7c0 _$E248
// FUNCTION: STBC 0x006de7d0 _$E247
// FUNCTION: STBC 0x006de7e0 _$E255
// FUNCTION: STBC 0x006de7f0 _$E254
// FUNCTION: STBC 0x006de800 _$E278
// FUNCTION: STBC 0x006de810 _$E277
// FUNCTION: STBC 0x006de820 _$E282
// FUNCTION: STBC 0x006de830 _$E281
// FUNCTION: STBC 0x006de840 _$E322
// FUNCTION: STBC 0x006de850 _$E321
// FUNCTION: STBC 0x006de860 _$E328
// FUNCTION: STBC 0x006de870 _$E327
// FUNCTION: STBC 0x006de880 _$E270
// FUNCTION: STBC 0x006de890 _$E269
// FUNCTION: STBC 0x006de8a0 _$E274
// FUNCTION: STBC 0x006de8b0 _$E273
// FUNCTION: STBC 0x006de8c0 _$E286
// FUNCTION: STBC 0x006de8d0 _$E285
// FUNCTION: STBC 0x006de8e0 _$E290
// FUNCTION: STBC 0x006de8f0 _$E289
// FUNCTION: STBC 0x006de900 _$E294
// FUNCTION: STBC 0x006de910 _$E293
// FUNCTION: STBC 0x006de920 _$E298
// FUNCTION: STBC 0x006de930 _$E297
// FUNCTION: STBC 0x006de940 _$E302
// FUNCTION: STBC 0x006de950 _$E301
// FUNCTION: STBC 0x006de960 _$E306
// FUNCTION: STBC 0x006de970 _$E305
// FUNCTION: STBC 0x006de980 _$E310
// FUNCTION: STBC 0x006de990 _$E309
// FUNCTION: STBC 0x006de9a0 _$E314
// FUNCTION: STBC 0x006de9b0 _$E313
// FUNCTION: STBC 0x006de9c0 _$E117
// FUNCTION: STBC 0x006de9d0 _$E116
// FUNCTION: STBC 0x006de9e0 _$E318
// FUNCTION: STBC 0x006de9f0 _$E317
// FUNCTION: STBC 0x006dea00 _$E122
// FUNCTION: STBC 0x006dea10 _$E121
// FUNCTION: STBC 0x006dea20 _$E332
// FUNCTION: STBC 0x006dea30 _$E331
// FUNCTION: STBC 0x006dea40 _$E336
// FUNCTION: STBC 0x006dea50 _$E335
// FUNCTION: STBC 0x006dea60 _$E340
// FUNCTION: STBC 0x006dea70 _$E339
// FUNCTION: STBC 0x006dea80 _$E344
// FUNCTION: STBC 0x006dea90 _$E343
// FUNCTION: STBC 0x006deaa0 _$E131
// FUNCTION: STBC 0x006deab0 _$E130
// FUNCTION: STBC 0x006deac0 _$E348
// FUNCTION: STBC 0x006dead0 _$E347
// FUNCTION: STBC 0x006deae0 _$E352
// FUNCTION: STBC 0x006deaf0 _$E351
// FUNCTION: STBC 0x006deb00 _$E356
// FUNCTION: STBC 0x006deb10 _$E355
// FUNCTION: STBC 0x006deb20 _$E137
// FUNCTION: STBC 0x006deb30 _$E136
// FUNCTION: STBC 0x006deb40 _$E142
// FUNCTION: STBC 0x006deb50 _$E141
// FUNCTION: STBC 0x006deb60 _$E172
// FUNCTION: STBC 0x006deb70 _$E171
// FUNCTION: STBC 0x006deb80 _$E360
// FUNCTION: STBC 0x006deb90 _$E359
// FUNCTION: STBC 0x006deba0 _$E177
// FUNCTION: STBC 0x006debb0 _$E176
// FUNCTION: STBC 0x006debc0 _$E364
// FUNCTION: STBC 0x006debd0 _$E363
// FUNCTION: STBC 0x006debe0 _$E368
// FUNCTION: STBC 0x006debf0 _$E367
// FUNCTION: STBC 0x006dec00 _$E182
// FUNCTION: STBC 0x006dec10 _$E181
// FUNCTION: STBC 0x006dec20 _$E372
// FUNCTION: STBC 0x006dec30 _$E371
// FUNCTION: STBC 0x006dec40 _$E147
// FUNCTION: STBC 0x006dec50 _$E146
// FUNCTION: STBC 0x006dec60 _$E376
// FUNCTION: STBC 0x006dec70 _$E375
// FUNCTION: STBC 0x006dec80 _$E380
// FUNCTION: STBC 0x006dec90 _$E379
// FUNCTION: STBC 0x006deca0 _$E187
// FUNCTION: STBC 0x006decb0 _$E186
// FUNCTION: STBC 0x006decc0 _$E192
// FUNCTION: STBC 0x006decd0 _$E191
// FUNCTION: STBC 0x006dece0 _$E384
// FUNCTION: STBC 0x006decf0 _$E383
// FUNCTION: STBC 0x006ded00 _$E388
// FUNCTION: STBC 0x006ded10 _$E387
// FUNCTION: STBC 0x006ded20 _$E392
// FUNCTION: STBC 0x006ded30 _$E391
// FUNCTION: STBC 0x006ded40 _$E396
// FUNCTION: STBC 0x006ded50 _$E395
// FUNCTION: STBC 0x006ded60 _$E400
// FUNCTION: STBC 0x006ded70 _$E399
// FUNCTION: STBC 0x006ded80 _$E404
// FUNCTION: STBC 0x006ded90 _$E403
// FUNCTION: STBC 0x006deda0 _$E408
// FUNCTION: STBC 0x006dedb0 _$E407
// FUNCTION: STBC 0x006dedc0 _$E412
// FUNCTION: STBC 0x006dedd0 _$E411
// FUNCTION: STBC 0x006dede0 _$E197
// FUNCTION: STBC 0x006dedf0 _$E196
// FUNCTION: STBC 0x006dee00 _$E416
// FUNCTION: STBC 0x006dee10 _$E415
// FUNCTION: STBC 0x006dee20 _$E202
// FUNCTION: STBC 0x006dee30 _$E201
// FUNCTION: STBC 0x006dee40 _$E207
// FUNCTION: STBC 0x006dee50 _$E206

/*
================
TGTimerList::TGTimerList
================
*/
// FUNCTION: STBC 0x006dee60 ??0TGTimerList@@QAE@XZ
TGTimerList::TGTimerList( void ) {
	head = NULL;
	tail = NULL;
	count = 0;
}

/*
================
TGTimerList::~TGTimerList
================
*/
// FUNCTION: STBC 0x006dee70 ??1TGTimerList@@QAE@XZ
TGTimerList::~TGTimerList( void ) {
	DeleteAll();
}
/*
================
TGTimerList::WriteToStream

The first timer's ID, the last saved timer's ID and the number of timers
that are saved; a timer is saved when its event is.
================
*/
// FUNCTION: STBC 0x006dee80
bool TGTimerList::WriteToStream( TGStream *stream ) {
	TGTimer *first = head;
	while ( first != NULL && first->event->IsNotSaved() ) {
		first = first->next;
	}
	int saved = 0;
	TGTimer *timer = first;
	TGTimer *last = first;
	for ( ; timer != NULL; timer = timer->next ) {
		if ( !timer->event->IsNotSaved() ) {
			saved++;
			last = timer;
		}
	}
	stream->WriteID( first != NULL ? first->GetObjID() : 0 );
	stream->WriteID( last != NULL ? last->GetObjID() : 0 );
	stream->WriteLong( saved );
	return true;
}

/*
================
TGTimerList::ReadFromStream

The timers are IDs until PostFixUp.
================
*/
// FUNCTION: STBC 0x006def00
bool TGTimerList::ReadFromStream( TGStream *stream ) {
	DeleteAll();
	head = (TGTimer *)stream->ReadID();
	tail = (TGTimer *)stream->ReadID();
	count = stream->ReadLong();
	return true;
}

/*
================
TGTimerList::FixUpPointers

False while an ID names no object yet.
================
*/
// FUNCTION: STBC 0x006def40
bool TGTimerList::FixUpPointers( void ) {
	if ( head != NULL && TGObject::GetTGObjectPtr( (long)head ) == NULL ) {
		return false;
	}
	if ( tail != NULL && TGObject::GetTGObjectPtr( (long)tail ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
TGTimerList::PostFixUp
================
*/
// FUNCTION: STBC 0x006def80
void TGTimerList::PostFixUp( void ) {
	head = head != NULL ? TGTimer::GetTGTimerPtr( (long)head ) : NULL;
	tail = tail != NULL ? TGTimer::GetTGTimerPtr( (long)tail ) : NULL;
}

/*
================
TGTimerList::Unknown6DEFC0

Forgets the timers without deleting them.
================
*/
// FUNCTION: STBC 0x006defc0
void TGTimerList::Unknown6DEFC0( void ) {
	head = NULL;
	tail = NULL;
	count = 0;
}

/*
================
TGTimerList::DeleteAll
================
*/
// FUNCTION: STBC 0x006defd0 ?DeleteAll@TGTimerList@@QAEXXZ
void TGTimerList::DeleteAll( void ) {
	while ( count != 0 ) {
		TGTimer *timer = head;
		RemoveHead();
		if ( timer != NULL ) {
			delete timer;
		}
	}
}

/*
================
TGTimerList::AddTimer

Links the timer in before the first one due at or after its time.
================
*/
// FUNCTION: STBC 0x006df000 ?AddTimer@TGTimerList@@QAE_NPAVTGTimer@@@Z
bool TGTimerList::AddTimer( TGTimer *timer ) {
	TGTimer *previous = NULL;
	TGTimer *current = head;
	while ( current != NULL && current->GetNextTime() < timer->GetNextTime() ) {
		previous = current;
		current = current->next;
	}
	if ( current == NULL ) {
		if ( previous != NULL ) {
			tail->next = timer;
			tail = timer;
			timer->next = NULL;
			count++;
			return true;
		}
		head = timer;
		tail = timer;
		timer->next = NULL;
		count++;
		return true;
	}
	if ( previous != NULL ) {
		timer->next = previous->next;
		previous->next = timer;
		count++;
		return true;
	}
	timer->next = head;
	head = timer;
	count++;
	return true;
}

/*
================
TGTimerList::RemoveHead

Unlinks the timer due first and returns it; NULL when the list is empty.
================
*/
// FUNCTION: STBC 0x006df0a0 ?RemoveHead@TGTimerList@@QAEPAVTGTimer@@XZ
TGTimer *TGTimerList::RemoveHead( void ) {
	TGTimer *timer = head;
	if ( timer != NULL ) {
		head = timer->next;
		count--;
		if ( count == 0 ) {
			tail = NULL;
		}
	}
	return timer;
}

/*
================
TGTimerList::DeleteHead
================
*/
// FUNCTION: STBC 0x006df0c0 ?DeleteHead@TGTimerList@@QAEXXZ
void TGTimerList::DeleteHead( void ) {
	TGTimer *timer = RemoveHead();
	if ( timer != NULL ) {
		delete timer;
	}
}

/*
================
TGTimerList::RemoveTimer

Unlinks the timer of the ID; false when the list has none.
================
*/
// FUNCTION: STBC 0x006df0e0 ?RemoveTimer@TGTimerList@@QAE_NJ@Z
// PARKED: the walk's last ID test: retail compares the ID in memory, this build loads it first
bool TGTimerList::RemoveTimer( long id ) {
	TGTimer *first = head;
	if ( first != NULL ) {
		TGTimer *current = first;
		TGTimer *previous = NULL;
		while ( current->GetObjID() != id && current->next != NULL ) {
			previous = current;
			current = current->next;
		}
		if ( current->GetObjID() == id ) {
			if ( first == current ) {
				head = current->next;
			}
			if ( tail == current ) {
				tail = previous;
			}
			if ( previous != NULL ) {
				previous->next = current->next;
			}
			current->next = NULL;
			count--;
			return true;
		}
	}
	return false;
}

/*
================
TGTimerList::RemoveTimer

Unlinks the timer; false for NULL or a timer that is not in the list.
================
*/
// FUNCTION: STBC 0x006df150 ?RemoveTimer@TGTimerList@@QAE_NPAVTGTimer@@@Z
bool TGTimerList::RemoveTimer( TGTimer *timer ) {
	if ( timer == NULL ) {
		return false;
	}
	return RemoveTimer( timer->GetObjID() );
}

/*
================
TGTimerList::DeleteTimer

Unlinks the timer of the ID and deletes it.
================
*/
// FUNCTION: STBC 0x006df170 ?DeleteTimer@TGTimerList@@QAE_NJ@Z
bool TGTimerList::DeleteTimer( long id ) {
	if ( RemoveTimer( id ) == true ) {
		TGTimer *timer = TGTimer::GetTGTimerPtr( id );
		if ( timer != NULL ) {
			delete timer;
			return true;
		}
	}
	return false;
}

/*
================
TGTimerList::DeleteTimer

Unlinks the timer and deletes it; false for NULL.
================
*/
// FUNCTION: STBC 0x006df1a0 ?DeleteTimer@TGTimerList@@QAE_NPAVTGTimer@@@Z
bool TGTimerList::DeleteTimer( TGTimer *timer ) {
	if ( timer != NULL ) {
		RemoveTimer( timer );
		delete timer;
		return true;
	}
	return false;
}

/*
================
TGTimerList::DeleteTimersForDestination
================
*/
// FUNCTION: STBC 0x006df1d0
bool TGTimerList::DeleteTimersForDestination( TGEventHandlerObject *destination, int eventType ) {
	bool deleted = false;
	TGTimer *previous = NULL;
	TGTimer *timer = head;
	while ( timer != NULL ) {
		TGEvent *event = timer->GetEvent();
		if ( event != NULL && event->GetDestination() == destination && ( eventType == 0 || eventType == event->GetEventType() ) ) {
			DeleteTimer( timer->GetObjID() );
			deleted = true;
			timer = previous;
		}
		previous = timer;
		timer = timer != NULL ? timer->next : head;
	}
	return deleted;
}

/*
================
TGTimerList::ClearSource
================
*/
// FUNCTION: STBC 0x006df230
bool TGTimerList::ClearSource( TGObject *source ) {
	bool found = false;
	TGTimer *timer = head;
	while ( timer != NULL ) {
		TGEvent *event = timer->GetEvent();
		if ( event != NULL && event->GetSource() == source ) {
			event->SetSource( NULL );
			found = true;
		}
		timer = timer != NULL ? timer->next : head;
	}
	return found;
}

/*
================
TGTimerList::IsTimerInList
================
*/
// FUNCTION: STBC 0x006df270
bool TGTimerList::IsTimerInList( TGTimer *timer ) {
	for ( TGTimer *current = head; current != NULL; current = current->next ) {
		if ( timer == current ) {
			return true;
		}
	}
	return false;
}

/*
================
TGTimerList::IsValid

Head and tail are both set or both clear, and the chain is not longer than
the count.
================
*/
// FUNCTION: STBC 0x006df290
bool TGTimerList::IsValid( void ) {
	if ( ( head != NULL && tail != NULL ) || ( head == NULL && tail == NULL ) ) {
		int numTimers = 0;
		for ( TGTimer *timer = head; timer != NULL && numTimers <= count; timer = timer->next ) {
			numTimers++;
		}
		return numTimers > count ? false : true;
	}
	return false;
}
