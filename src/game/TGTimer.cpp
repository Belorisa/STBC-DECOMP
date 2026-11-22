/*
===============================================================================

	TGTimer.cpp

===============================================================================
*/

#include <iostream>
#include "TGTimer.h"
#include "TGEvent.h"
#include "TGStream.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006dc5e0 _$E420
// FUNCTION: STBC 0x006dc600 _$E419
// FUNCTION: STBC 0x006dc610 _$E32
// FUNCTION: STBC 0x006dc620 _$E29
// FUNCTION: STBC 0x006dc630 _$E31
// FUNCTION: STBC 0x006dc640 _$E30
// FUNCTION: STBC 0x006dc650 _$E38
// FUNCTION: STBC 0x006dc660 _$E35
// FUNCTION: STBC 0x006dc670 _$E37
// FUNCTION: STBC 0x006dc680 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006dc690 _$E57
// FUNCTION: STBC 0x006dc6a0 _$E56
// FUNCTION: STBC 0x006dc6b0 _$E61
// FUNCTION: STBC 0x006dc6c0 _$E60
// FUNCTION: STBC 0x006dc6d0 _$E72
// FUNCTION: STBC 0x006dc6e0 _$E71
// FUNCTION: STBC 0x006dc6f0 _$E76
// FUNCTION: STBC 0x006dc700 _$E75
// FUNCTION: STBC 0x006dc710 _$E80
// FUNCTION: STBC 0x006dc720 _$E79
// FUNCTION: STBC 0x006dc730 _$E84
// FUNCTION: STBC 0x006dc740 _$E83
// FUNCTION: STBC 0x006dc750 _$E208
// FUNCTION: STBC 0x006dc760 _$E207
// FUNCTION: STBC 0x006dc770 _$E150
// FUNCTION: STBC 0x006dc780 _$E149
// FUNCTION: STBC 0x006dc790 _$E163
// FUNCTION: STBC 0x006dc7a0 _$E162
// FUNCTION: STBC 0x006dc7b0 _$E213
// FUNCTION: STBC 0x006dc7c0 _$E212
// FUNCTION: STBC 0x006dc7d0 _$E218
// FUNCTION: STBC 0x006dc7e0 _$E217
// FUNCTION: STBC 0x006dc7f0 _$E222
// FUNCTION: STBC 0x006dc800 _$E221
// FUNCTION: STBC 0x006dc810 _$E226
// FUNCTION: STBC 0x006dc820 _$E225
// FUNCTION: STBC 0x006dc830 _$E233
// FUNCTION: STBC 0x006dc840 _$E232
// FUNCTION: STBC 0x006dc850 _$E258
// FUNCTION: STBC 0x006dc860 _$E257
// FUNCTION: STBC 0x006dc870 _$E262
// FUNCTION: STBC 0x006dc880 _$E261
// FUNCTION: STBC 0x006dc890 _$E108
// FUNCTION: STBC 0x006dc8a0 _$E107
// FUNCTION: STBC 0x006dc8b0 _$E244
// FUNCTION: STBC 0x006dc8c0 _$E243
// FUNCTION: STBC 0x006dc8d0 _$E251
// FUNCTION: STBC 0x006dc8e0 _$E250
// FUNCTION: STBC 0x006dc8f0 _$E274
// FUNCTION: STBC 0x006dc900 _$E273
// FUNCTION: STBC 0x006dc910 _$E278
// FUNCTION: STBC 0x006dc920 _$E277
// FUNCTION: STBC 0x006dc930 _$E318
// FUNCTION: STBC 0x006dc940 _$E317
// FUNCTION: STBC 0x006dc950 _$E324
// FUNCTION: STBC 0x006dc960 _$E323
// FUNCTION: STBC 0x006dc970 _$E266
// FUNCTION: STBC 0x006dc980 _$E265
// FUNCTION: STBC 0x006dc990 _$E270
// FUNCTION: STBC 0x006dc9a0 _$E269
// FUNCTION: STBC 0x006dc9b0 _$E282
// FUNCTION: STBC 0x006dc9c0 _$E281
// FUNCTION: STBC 0x006dc9d0 _$E286
// FUNCTION: STBC 0x006dc9e0 _$E285
// FUNCTION: STBC 0x006dc9f0 _$E290
// FUNCTION: STBC 0x006dca00 _$E289
// FUNCTION: STBC 0x006dca10 _$E294
// FUNCTION: STBC 0x006dca20 _$E293
// FUNCTION: STBC 0x006dca30 _$E298
// FUNCTION: STBC 0x006dca40 _$E297
// FUNCTION: STBC 0x006dca50 _$E302
// FUNCTION: STBC 0x006dca60 _$E301
// FUNCTION: STBC 0x006dca70 _$E306
// FUNCTION: STBC 0x006dca80 _$E305
// FUNCTION: STBC 0x006dca90 _$E310
// FUNCTION: STBC 0x006dcaa0 _$E309
// FUNCTION: STBC 0x006dcab0 _$E113
// FUNCTION: STBC 0x006dcac0 _$E112
// FUNCTION: STBC 0x006dcad0 _$E314
// FUNCTION: STBC 0x006dcae0 _$E313
// FUNCTION: STBC 0x006dcaf0 _$E118
// FUNCTION: STBC 0x006dcb00 _$E117
// FUNCTION: STBC 0x006dcb10 _$E328
// FUNCTION: STBC 0x006dcb20 _$E327
// FUNCTION: STBC 0x006dcb30 _$E332
// FUNCTION: STBC 0x006dcb40 _$E331
// FUNCTION: STBC 0x006dcb50 _$E336
// FUNCTION: STBC 0x006dcb60 _$E335
// FUNCTION: STBC 0x006dcb70 _$E340
// FUNCTION: STBC 0x006dcb80 _$E339
// FUNCTION: STBC 0x006dcb90 _$E127
// FUNCTION: STBC 0x006dcba0 _$E126
// FUNCTION: STBC 0x006dcbb0 _$E344
// FUNCTION: STBC 0x006dcbc0 _$E343
// FUNCTION: STBC 0x006dcbd0 _$E348
// FUNCTION: STBC 0x006dcbe0 _$E347
// FUNCTION: STBC 0x006dcbf0 _$E352
// FUNCTION: STBC 0x006dcc00 _$E351
// FUNCTION: STBC 0x006dcc10 _$E133
// FUNCTION: STBC 0x006dcc20 _$E132
// FUNCTION: STBC 0x006dcc30 _$E138
// FUNCTION: STBC 0x006dcc40 _$E137
// FUNCTION: STBC 0x006dcc50 _$E168
// FUNCTION: STBC 0x006dcc60 _$E167
// FUNCTION: STBC 0x006dcc70 _$E356
// FUNCTION: STBC 0x006dcc80 _$E355
// FUNCTION: STBC 0x006dcc90 _$E173
// FUNCTION: STBC 0x006dcca0 _$E172
// FUNCTION: STBC 0x006dccb0 _$E360
// FUNCTION: STBC 0x006dccc0 _$E359
// FUNCTION: STBC 0x006dccd0 _$E364
// FUNCTION: STBC 0x006dcce0 _$E363
// FUNCTION: STBC 0x006dccf0 _$E178
// FUNCTION: STBC 0x006dcd00 _$E177
// FUNCTION: STBC 0x006dcd10 _$E368
// FUNCTION: STBC 0x006dcd20 _$E367
// FUNCTION: STBC 0x006dcd30 _$E143
// FUNCTION: STBC 0x006dcd40 _$E142
// FUNCTION: STBC 0x006dcd50 _$E372
// FUNCTION: STBC 0x006dcd60 _$E371
// FUNCTION: STBC 0x006dcd70 _$E376
// FUNCTION: STBC 0x006dcd80 _$E375
// FUNCTION: STBC 0x006dcd90 _$E183
// FUNCTION: STBC 0x006dcda0 _$E182
// FUNCTION: STBC 0x006dcdb0 _$E188
// FUNCTION: STBC 0x006dcdc0 _$E187
// FUNCTION: STBC 0x006dcdd0 _$E380
// FUNCTION: STBC 0x006dcde0 _$E379
// FUNCTION: STBC 0x006dcdf0 _$E384
// FUNCTION: STBC 0x006dce00 _$E383
// FUNCTION: STBC 0x006dce10 _$E388
// FUNCTION: STBC 0x006dce20 _$E387
// FUNCTION: STBC 0x006dce30 _$E392
// FUNCTION: STBC 0x006dce40 _$E391
// FUNCTION: STBC 0x006dce50 _$E396
// FUNCTION: STBC 0x006dce60 _$E395
// FUNCTION: STBC 0x006dce70 _$E400
// FUNCTION: STBC 0x006dce80 _$E399
// FUNCTION: STBC 0x006dce90 _$E404
// FUNCTION: STBC 0x006dcea0 _$E403
// FUNCTION: STBC 0x006dceb0 _$E408
// FUNCTION: STBC 0x006dcec0 _$E407
// FUNCTION: STBC 0x006dced0 _$E193
// FUNCTION: STBC 0x006dcee0 _$E192
// FUNCTION: STBC 0x006dcef0 _$E412
// FUNCTION: STBC 0x006dcf00 _$E411
// FUNCTION: STBC 0x006dcf10 _$E198
// FUNCTION: STBC 0x006dcf20 _$E197
// FUNCTION: STBC 0x006dcf30 _$E203
// FUNCTION: STBC 0x006dcf40 _$E202

// DATA: STBC 0x00998aa0
NiTMap<long, TGTimer *> *TGTimer::timerTable;

// DATA: STBC 0x0095ae80
bool TGTimer::keepTimerTable = true;

// DATA: STBC 0x0095ae84
float TIMER_FOREVER = -1.0f;

/*
================
TGTimer::TGTimer
================
*/
// FUNCTION: STBC 0x006dcf50
TGTimer::TGTimer( long id ) : TGObject( id ) {
	delay = 0.0f;
	duration = 0.0f;
	nextTime = 0.0f;
	timerStart = 0.0f;
	event = NULL;
	next = NULL;

	if ( timerTable == NULL ) {
		timerTable = new NiTMap<long, TGTimer *>;
	}
	AddTimer( this );
}

/*
================
TGTimer::GetObjType
================
*/
// FUNCTION: STBC 0x006dd030
int TGTimer::GetObjType( void ) {
	return TGTIMER_TYPE;
}

/*
================
TGTimer::IsTypeOf
================
*/
// FUNCTION: STBC 0x006dd040
bool TGTimer::IsTypeOf( int type ) {
	if ( type == TGTIMER_TYPE ) {
		return true;
	}
	return TGObject::IsTypeOf( type );
}

/*
================
TGTimer::GetClassName
================
*/
// FUNCTION: STBC 0x006dd060
char *TGTimer::GetClassName( void ) {
	return "TGTimer";
}

/*
================
TGTimer::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x006dd070
char *TGTimer::GetSwigTypeName( void ) {
	return "_p_TGTimer";
}

/*
================
TGTimer::GetPythonClassName
================
*/
// FUNCTION: STBC 0x006dd080
char *TGTimer::GetPythonClassName( void ) {
	return "TGTimerPtr";
}

/*
================
TGTimer::~TGTimer
================
*/
// FUNCTION: STBC 0x006dd0c0
TGTimer::~TGTimer( void ) {
	RemoveTimer( this );
	ReleaseEvent();
}

/*
================
TGTimer::WriteToStream

The event and the next timer are written by ID, 0 for one that is gone;
the next timer as the first one after it whose event is saved.
================
*/
// FUNCTION: STBC 0x006dd120
bool TGTimer::WriteToStream( TGStream *stream ) {
	if ( !TGObject::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteFloat( delay );
	stream->WriteFloat( duration );
	stream->WriteFloat( nextTime );
	stream->WriteFloat( timerStart );

	if ( event != NULL && GetTGObjectPtr( event->GetObjID() ) == event ) {
		stream->WriteID( GetSavedID( event ) );
	} else {
		stream->WriteID( 0 );
	}

	TGTimer *timer = next;
	while ( timer != NULL && timer->event->IsNotSaved() ) {
		timer = timer->next;
	}
	if ( timer != NULL && GetTGObjectPtr( timer->GetObjID() ) == timer ) {
		stream->WriteID( timer->GetObjID() );
	} else {
		stream->WriteID( 0 );
	}
	return true;
}

/*
================
TGTimer::ReadFromStream

Keeps the IDs in the pointers until PostFixUp.
================
*/
// FUNCTION: STBC 0x006dd210
bool TGTimer::ReadFromStream( TGStream *stream ) {
	delay = stream->ReadFloat();
	duration = stream->ReadFloat();
	nextTime = stream->ReadFloat();
	timerStart = stream->ReadFloat();
	event = (TGEvent *)stream->ReadID();
	next = (TGTimer *)stream->ReadID();
	return true;
}

/*
================
TGTimer::SetEvent

Takes a reference to the new event and releases the old one.
================
*/
// FUNCTION: STBC 0x006dd270
void TGTimer::SetEvent( TGEvent *event ) {
	if ( this->event != event ) {
		if ( this->event != NULL ) {
			ReleaseEvent();
		}
		event->IncRefCount();
		this->event = event;
	}
}

/*
================
TGTimer::ReleaseEvent
================
*/
// FUNCTION: STBC 0x006dd2a0
void TGTimer::ReleaseEvent( void ) {
	if ( event != NULL ) {
		event->DecRefCount();
	}
	event = NULL;
}

/*
================
TGTimer::Update

True when the event is due at this time, and moves the next send on by
the delay; never once the duration has passed. A timer of TIMER_FOREVER
is due at every call.
================
*/
// FUNCTION: STBC 0x006dd2c0
bool TGTimer::Update( float time ) {
	if ( duration != TIMER_FOREVER ) {
		if ( time <= nextTime || timerStart + duration <= time ) {
			return false;
		}
	}
	nextTime = delay + nextTime;
	return true;
}

/*
================
TGTimer::FixUpPointers

False while an ID names no object yet.
================
*/
// FUNCTION: STBC 0x006dd310
bool TGTimer::FixUpPointers( void ) {
	if ( event != NULL && GetTGObjectPtr( (long)event ) == NULL ) {
		return false;
	}
	if ( next != NULL && GetTGObjectPtr( (long)next ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
TGTimer::PostFixUp

Turns the IDs back into objects.
================
*/
// FUNCTION: STBC 0x006dd350
void TGTimer::PostFixUp( void ) {
	if ( event != NULL ) {
		event = (TGEvent *)GetTGObjectPtr( (long)event );
	}
	if ( next != NULL ) {
		next = (TGTimer *)GetTGObjectPtr( (long)next );
	}
}

/*
================
TGTimer::GetTGTimerPtr

The timer with this ID; NULL when there is none.
================
*/
// FUNCTION: STBC 0x006dd380
TGTimer *TGTimer::GetTGTimerPtr( long id ) {
	TGTimer *timer;
	if ( timerTable->GetAt( id, timer ) ) {
		return timer;
	}
	return NULL;
}

/*
================
TGTimer::AddTimer
================
*/
// FUNCTION: STBC 0x006dd3d0
bool TGTimer::AddTimer( TGTimer *timer ) {
	timerTable->SetAt( timer->id, timer );
	return true;
}

/*
================
TGTimer::RemoveTimer

False when the timer was not registered. Once shutdown has begun
(ReleaseTimerTable), the last removal deletes the table.

The table pointer is not cleared after the delete.
================
*/
// FUNCTION: STBC 0x006dd460
bool TGTimer::RemoveTimer( TGTimer *timer ) {
	bool removed = timerTable->RemoveAt( timer->id );
	if ( !keepTimerTable && timerTable != NULL && timerTable->GetCount() == 0 ) {
		delete timerTable;
	}
	return removed;
}

/*
================
TGTimer::ReleaseTimerTable

Shutdown: the table goes now if it is empty, else with its last timer.

The table pointer is not cleared after the delete.
================
*/
// FUNCTION: STBC 0x006dd520
void TGTimer::ReleaseTimerTable( void ) {
	keepTimerTable = false;
	if ( timerTable != NULL && timerTable->GetCount() == 0 ) {
		delete timerTable;
	}
}

/*
================
TGTimer::RemoveAllTimers

Forgets every timer without deleting any.
================
*/
// FUNCTION: STBC 0x006dd550
void TGTimer::RemoveAllTimers( void ) {
	timerTable->RemoveAll();
}

/*
================
TGTimer::SaveTimers

Every registered timer whose event is saved, each after a true; a false
ends the list. False when one of them failed.
================
*/
// FUNCTION: STBC 0x006dd5b0
bool TGTimer::SaveTimers( TGStream *stream ) {
	bool saved = true;

	NiTMapIterator pos = timerTable->GetFirstPos();
	while ( pos != NULL ) {
		long id;
		TGTimer *timer;
		timerTable->GetNext( pos, id, timer );
		if ( GetTGObjectPtr( id ) == timer && !timer->event->IsNotSaved() ) {
			stream->WriteBool( true );
			if ( !timer->WriteToStream( stream ) ) {
				saved = false;
			}
		}
	}

	stream->WriteBool( false );
	return saved;
}

/*
================
TGTimer::LoadTimers

Reads what SaveTimers wrote; false when an object read is no timer.
================
*/
// FUNCTION: STBC 0x006dd680
bool TGTimer::LoadTimers( TGStream *stream ) {
	bool loaded = true;
	while ( stream->ReadBool() == true && loaded == true ) {
		TGObject *object = LoadObject( stream, 0 );
		if ( object == NULL || object->GetObjType() != TGTIMER_TYPE ) {
			loaded = false;
		}
	}
	return loaded;
}

/*
================
TGTimer::DeleteAllTimers
================
*/
// FUNCTION: STBC 0x006dd6d0
void TGTimer::DeleteAllTimers( void ) {
	NiTMapIterator pos = timerTable->GetFirstPos();
	while ( pos != NULL ) {
		long id;
		TGTimer *timer;
		timerTable->GetNext( pos, id, timer );
		if ( timer != NULL ) {
			delete timer;
		}
	}
	RemoveAllTimers();
}
