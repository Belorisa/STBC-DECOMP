/*
===============================================================================

	TGTimerManager.cpp

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
// FUNCTION: STBC 0x006db8d0 _$E424
// FUNCTION: STBC 0x006db8f0 _$E423
// FUNCTION: STBC 0x006db900 _$E32
// FUNCTION: STBC 0x006db910 _$E29
// FUNCTION: STBC 0x006db920 _$E31
// FUNCTION: STBC 0x006db930 _$E30
// FUNCTION: STBC 0x006db940 _$E38
// FUNCTION: STBC 0x006db950 _$E35
// FUNCTION: STBC 0x006db960 _$E37
// FUNCTION: STBC 0x006db970 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006db980 _$E61
// FUNCTION: STBC 0x006db990 _$E60
// FUNCTION: STBC 0x006db9a0 _$E65
// FUNCTION: STBC 0x006db9b0 _$E64
// FUNCTION: STBC 0x006db9c0 _$E76
// FUNCTION: STBC 0x006db9d0 _$E75
// FUNCTION: STBC 0x006db9e0 _$E80
// FUNCTION: STBC 0x006db9f0 _$E79
// FUNCTION: STBC 0x006dba00 _$E84
// FUNCTION: STBC 0x006dba10 _$E83
// FUNCTION: STBC 0x006dba20 _$E88
// FUNCTION: STBC 0x006dba30 _$E87
// FUNCTION: STBC 0x006dba40 _$E212
// FUNCTION: STBC 0x006dba50 _$E211
// FUNCTION: STBC 0x006dba60 _$E154
// FUNCTION: STBC 0x006dba70 _$E153
// FUNCTION: STBC 0x006dba80 _$E167
// FUNCTION: STBC 0x006dba90 _$E166
// FUNCTION: STBC 0x006dbaa0 _$E217
// FUNCTION: STBC 0x006dbab0 _$E216
// FUNCTION: STBC 0x006dbac0 _$E222
// FUNCTION: STBC 0x006dbad0 _$E221
// FUNCTION: STBC 0x006dbae0 _$E226
// FUNCTION: STBC 0x006dbaf0 _$E225
// FUNCTION: STBC 0x006dbb00 _$E230
// FUNCTION: STBC 0x006dbb10 _$E229
// FUNCTION: STBC 0x006dbb20 _$E237
// FUNCTION: STBC 0x006dbb30 _$E236
// FUNCTION: STBC 0x006dbb40 _$E262
// FUNCTION: STBC 0x006dbb50 _$E261
// FUNCTION: STBC 0x006dbb60 _$E266
// FUNCTION: STBC 0x006dbb70 _$E265
// FUNCTION: STBC 0x006dbb80 _$E112
// FUNCTION: STBC 0x006dbb90 _$E111
// FUNCTION: STBC 0x006dbba0 _$E248
// FUNCTION: STBC 0x006dbbb0 _$E247
// FUNCTION: STBC 0x006dbbc0 _$E255
// FUNCTION: STBC 0x006dbbd0 _$E254
// FUNCTION: STBC 0x006dbbe0 _$E278
// FUNCTION: STBC 0x006dbbf0 _$E277
// FUNCTION: STBC 0x006dbc00 _$E282
// FUNCTION: STBC 0x006dbc10 _$E281
// FUNCTION: STBC 0x006dbc20 _$E322
// FUNCTION: STBC 0x006dbc30 _$E321
// FUNCTION: STBC 0x006dbc40 _$E328
// FUNCTION: STBC 0x006dbc50 _$E327
// FUNCTION: STBC 0x006dbc60 _$E270
// FUNCTION: STBC 0x006dbc70 _$E269
// FUNCTION: STBC 0x006dbc80 _$E274
// FUNCTION: STBC 0x006dbc90 _$E273
// FUNCTION: STBC 0x006dbca0 _$E286
// FUNCTION: STBC 0x006dbcb0 _$E285
// FUNCTION: STBC 0x006dbcc0 _$E290
// FUNCTION: STBC 0x006dbcd0 _$E289
// FUNCTION: STBC 0x006dbce0 _$E294
// FUNCTION: STBC 0x006dbcf0 _$E293
// FUNCTION: STBC 0x006dbd00 _$E298
// FUNCTION: STBC 0x006dbd10 _$E297
// FUNCTION: STBC 0x006dbd20 _$E302
// FUNCTION: STBC 0x006dbd30 _$E301
// FUNCTION: STBC 0x006dbd40 _$E306
// FUNCTION: STBC 0x006dbd50 _$E305
// FUNCTION: STBC 0x006dbd60 _$E310
// FUNCTION: STBC 0x006dbd70 _$E309
// FUNCTION: STBC 0x006dbd80 _$E314
// FUNCTION: STBC 0x006dbd90 _$E313
// FUNCTION: STBC 0x006dbda0 _$E117
// FUNCTION: STBC 0x006dbdb0 _$E116
// FUNCTION: STBC 0x006dbdc0 _$E318
// FUNCTION: STBC 0x006dbdd0 _$E317
// FUNCTION: STBC 0x006dbde0 _$E122
// FUNCTION: STBC 0x006dbdf0 _$E121
// FUNCTION: STBC 0x006dbe00 _$E332
// FUNCTION: STBC 0x006dbe10 _$E331
// FUNCTION: STBC 0x006dbe20 _$E336
// FUNCTION: STBC 0x006dbe30 _$E335
// FUNCTION: STBC 0x006dbe40 _$E340
// FUNCTION: STBC 0x006dbe50 _$E339
// FUNCTION: STBC 0x006dbe60 _$E344
// FUNCTION: STBC 0x006dbe70 _$E343
// FUNCTION: STBC 0x006dbe80 _$E131
// FUNCTION: STBC 0x006dbe90 _$E130
// FUNCTION: STBC 0x006dbea0 _$E348
// FUNCTION: STBC 0x006dbeb0 _$E347
// FUNCTION: STBC 0x006dbec0 _$E352
// FUNCTION: STBC 0x006dbed0 _$E351
// FUNCTION: STBC 0x006dbee0 _$E356
// FUNCTION: STBC 0x006dbef0 _$E355
// FUNCTION: STBC 0x006dbf00 _$E137
// FUNCTION: STBC 0x006dbf10 _$E136
// FUNCTION: STBC 0x006dbf20 _$E142
// FUNCTION: STBC 0x006dbf30 _$E141
// FUNCTION: STBC 0x006dbf40 _$E172
// FUNCTION: STBC 0x006dbf50 _$E171
// FUNCTION: STBC 0x006dbf60 _$E360
// FUNCTION: STBC 0x006dbf70 _$E359
// FUNCTION: STBC 0x006dbf80 _$E177
// FUNCTION: STBC 0x006dbf90 _$E176
// FUNCTION: STBC 0x006dbfa0 _$E364
// FUNCTION: STBC 0x006dbfb0 _$E363
// FUNCTION: STBC 0x006dbfc0 _$E368
// FUNCTION: STBC 0x006dbfd0 _$E367
// FUNCTION: STBC 0x006dbfe0 _$E182
// FUNCTION: STBC 0x006dbff0 _$E181
// FUNCTION: STBC 0x006dc000 _$E372
// FUNCTION: STBC 0x006dc010 _$E371
// FUNCTION: STBC 0x006dc020 _$E147
// FUNCTION: STBC 0x006dc030 _$E146
// FUNCTION: STBC 0x006dc040 _$E376
// FUNCTION: STBC 0x006dc050 _$E375
// FUNCTION: STBC 0x006dc060 _$E380
// FUNCTION: STBC 0x006dc070 _$E379
// FUNCTION: STBC 0x006dc080 _$E187
// FUNCTION: STBC 0x006dc090 _$E186
// FUNCTION: STBC 0x006dc0a0 _$E192
// FUNCTION: STBC 0x006dc0b0 _$E191
// FUNCTION: STBC 0x006dc0c0 _$E384
// FUNCTION: STBC 0x006dc0d0 _$E383
// FUNCTION: STBC 0x006dc0e0 _$E388
// FUNCTION: STBC 0x006dc0f0 _$E387
// FUNCTION: STBC 0x006dc100 _$E392
// FUNCTION: STBC 0x006dc110 _$E391
// FUNCTION: STBC 0x006dc120 _$E396
// FUNCTION: STBC 0x006dc130 _$E395
// FUNCTION: STBC 0x006dc140 _$E400
// FUNCTION: STBC 0x006dc150 _$E399
// FUNCTION: STBC 0x006dc160 _$E404
// FUNCTION: STBC 0x006dc170 _$E403
// FUNCTION: STBC 0x006dc180 _$E408
// FUNCTION: STBC 0x006dc190 _$E407
// FUNCTION: STBC 0x006dc1a0 _$E412
// FUNCTION: STBC 0x006dc1b0 _$E411
// FUNCTION: STBC 0x006dc1c0 _$E197
// FUNCTION: STBC 0x006dc1d0 _$E196
// FUNCTION: STBC 0x006dc1e0 _$E416
// FUNCTION: STBC 0x006dc1f0 _$E415
// FUNCTION: STBC 0x006dc200 _$E202
// FUNCTION: STBC 0x006dc210 _$E201
// FUNCTION: STBC 0x006dc220 _$E207
// FUNCTION: STBC 0x006dc230 _$E206

/*
================
TGTimerManager::TGTimerManager
================
*/
// FUNCTION: STBC 0x006dc240
TGTimerManager::TGTimerManager( void ) {
	eventManager = &g_kEventManager;
	timers = new TGTimerList;
	ResetTime();
}

/*
================
TGTimerManager::~TGTimerManager
================
*/
// FUNCTION: STBC 0x006dc2b0
TGTimerManager::~TGTimerManager( void ) {
	if ( timers != NULL ) {
		delete timers;
		timers = NULL;
	}
}

/*
================
TGTimerManager::WriteToStream
================
*/
// FUNCTION: STBC 0x006dc2e0
bool TGTimerManager::WriteToStream( TGStream *stream ) {
	stream->WriteFloat( unknown04 );
	return timers->WriteToStream( stream ) ? true : false;
}

/*
================
TGTimerManager::ReadFromStream
================
*/
// FUNCTION: STBC 0x006dc310
bool TGTimerManager::ReadFromStream( TGStream *stream ) {
	if ( timers != NULL ) {
		delete timers;
		timers = new TGTimerList;
		if ( timers == NULL ) {
			return false;
		}
	}
	unknown04 = stream->ReadFloat();
	return timers->ReadFromStream( stream ) ? true : false;
}

/*
================
TGTimerManager::FixUpPointers
================
*/
// FUNCTION: STBC 0x006dc3c0
bool TGTimerManager::FixUpPointers( void ) {
	return timers->FixUpPointers() ? true : false;
}

/*
================
TGTimerManager::PostFixUp
================
*/
// FUNCTION: STBC 0x006dc3d0
void TGTimerManager::PostFixUp( void ) {
	timers->PostFixUp();
}

/*
================
TGTimerManager::ForgetAll

Forgets the timers without deleting them.
================
*/
// FUNCTION: STBC 0x006dc3e0
void TGTimerManager::ForgetAll( void ) {
	timers->Unknown6DEFC0();
}

/*
================
TGTimerManager::AddTimer
================
*/
// FUNCTION: STBC 0x006dc3f0 ?AddTimer@TGTimerManager@@QAE_NPAVTGTimer@@@Z
bool TGTimerManager::AddTimer( TGTimer *timer ) {
	if ( timers != NULL ) {
		return timers->AddTimer( timer );
	}
	return false;
}

/*
================
TGTimerManager::RemoveTimer
================
*/
// FUNCTION: STBC 0x006dc410 ?RemoveTimer@TGTimerManager@@QAE_NPAVTGTimer@@@Z
bool TGTimerManager::RemoveTimer( TGTimer *timer ) {
	if ( timers != NULL ) {
		return timers->RemoveTimer( timer );
	}
	return false;
}

/*
================
TGTimerManager::RemoveTimer
================
*/
// FUNCTION: STBC 0x006dc430 ?RemoveTimer@TGTimerManager@@QAE_NJ@Z
bool TGTimerManager::RemoveTimer( long id ) {
	if ( timers != NULL ) {
		return timers->RemoveTimer( id );
	}
	return false;
}

/*
================
TGTimerManager::DeleteTimer
================
*/
// FUNCTION: STBC 0x006dc450 ?DeleteTimer@TGTimerManager@@QAE_NPAVTGTimer@@@Z
bool TGTimerManager::DeleteTimer( TGTimer *timer ) {
	if ( timers != NULL ) {
		return timers->DeleteTimer( timer );
	}
	return false;
}

/*
================
TGTimerManager::DeleteTimer
================
*/
// FUNCTION: STBC 0x006dc470 ?DeleteTimer@TGTimerManager@@QAE_NJ@Z
bool TGTimerManager::DeleteTimer( long id ) {
	if ( timers != NULL ) {
		return timers->DeleteTimer( id );
	}
	return false;
}

/*
================
TGTimerManager::Update

Sends the event of every timer that is due by the time: a timer with a
duration sends a copy, and goes back in the list while it runs on.
False when any event was refused, or there is no list.
================
*/
// FUNCTION: STBC 0x006dc490 ?Update@TGTimerManager@@QAE_NM@Z
bool TGTimerManager::Update( float time ) {
	// MATCH-PENDING: R17, timer->next is cleared through the invented TGTimer::SetNext (TGTimer.h): only the schedule shows an inline setter there (retail keeps the store ahead of the duration's load)
	bool result = true;
	if ( timers != NULL ) {
		if ( time < unknown04 ) {
			unknown04 = time;
		}
		TGTimer *timer;
		for ( timer = timers->GetHead(); timer != NULL && timer->GetNextTime() <= time; timer = timers->GetHead() ) {
			timers->RemoveHead();
			timer->SetNext( NULL );
			TGEvent *event;
			if ( timer->GetDuration() != 0.0f ) {
				event = (TGEvent *)TGObject::CreateObject( timer->GetEvent()->GetObjType(), 0 );
				if ( event != NULL ) {
					event->Copy( timer->GetEvent() );
				}
			} else {
				event = timer->GetEvent();
			}
			event->SetTimestamp( timer->GetNextTime() );
			if ( !eventManager->AddEvent( event ) ) {
				result = false;
			}
			if ( timer->Update( time ) ) {
				timers->AddTimer( timer );
			} else {
				delete timer;
			}
		}
		unknown04 = time;
		return result;
	}
	return false;
}

/*
================
TGTimerManager::RebaseTime
================
*/
// FUNCTION: STBC 0x006dc580
void TGTimerManager::RebaseTime( float time ) {
	float delta = time - unknown04;
	for ( TGTimer *timer = timers->GetHead(); timer != NULL; timer = timer->next ) {
		timer->SetTimerStart( timer->GetTimerStart() + delta );
		timer->nextTime = timer->GetNextTime() + delta;	// BUG: SetTimerStart already moved the next send to the new start, so it moves twice
	}
	unknown04 = time;
}

/*
================
TGTimerManager::DeleteAllTimers
================
*/
// FUNCTION: STBC 0x006dc5c0 ?DeleteAllTimers@TGTimerManager@@QAEXXZ
void TGTimerManager::DeleteAllTimers( void ) {
	if ( timers != NULL ) {
		timers->DeleteAll();
	}
}

/*
================
TGTimerManager::ResetTime
================
*/
// FUNCTION: STBC 0x006dc5d0 ?ResetTime@TGTimerManager@@QAEXXZ
void TGTimerManager::ResetTime( void ) {
	unknown04 = 0.0f;
}
