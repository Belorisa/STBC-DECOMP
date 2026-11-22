/*
===============================================================================

	TGSequence.cpp

===============================================================================
*/

#include "TGSequence.h"
#include "TGSequenceEvent.h"
#include "TGActionManager.h"
#include "TGStream.h"
#include <iostream>
#include "TGTimer.h"
#include "TGTimerManager.h"
#include "UtopiaModule.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006ffb80 _$E45
// FUNCTION: STBC 0x006ffb90 _$E42
// FUNCTION: STBC 0x006ffba0 _$E44
// FUNCTION: STBC 0x006ffbb0 _$E43
// FUNCTION: STBC 0x006ffbc0 _$E51
// FUNCTION: STBC 0x006ffbd0 _$E48
// FUNCTION: STBC 0x006ffbe0 _$E50
// FUNCTION: STBC 0x006ffbf0 _$E49

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006ffc00 _$E67
// FUNCTION: STBC 0x006ffc10 _$E66
// FUNCTION: STBC 0x006ffc20 _$E223
// FUNCTION: STBC 0x006ffc30 _$E222
// FUNCTION: STBC 0x006ffc40 _$E78
// FUNCTION: STBC 0x006ffc50 _$E77
// FUNCTION: STBC 0x006ffc60 _$E82
// FUNCTION: STBC 0x006ffc70 _$E81
// FUNCTION: STBC 0x006ffc80 _$E86
// FUNCTION: STBC 0x006ffc90 _$E85
// FUNCTION: STBC 0x006ffca0 _$E90
// FUNCTION: STBC 0x006ffcb0 _$E89
// FUNCTION: STBC 0x006ffcc0 _$E210
// FUNCTION: STBC 0x006ffcd0 _$E209
// FUNCTION: STBC 0x006ffce0 _$E152
// FUNCTION: STBC 0x006ffcf0 _$E151
// FUNCTION: STBC 0x006ffd00 _$E165
// FUNCTION: STBC 0x006ffd10 _$E164
// FUNCTION: STBC 0x006ffd20 _$E215
// FUNCTION: STBC 0x006ffd30 _$E214
// FUNCTION: STBC 0x006ffd40 _$E228
// FUNCTION: STBC 0x006ffd50 _$E227
// FUNCTION: STBC 0x006ffd60 _$E219
// FUNCTION: STBC 0x006ffd70 _$E218
// FUNCTION: STBC 0x006ffd80 _$E232
// FUNCTION: STBC 0x006ffd90 _$E231
// FUNCTION: STBC 0x006ffda0 _$E239
// FUNCTION: STBC 0x006ffdb0 _$E238
// FUNCTION: STBC 0x006ffdc0 _$E272
// FUNCTION: STBC 0x006ffdd0 _$E271
// FUNCTION: STBC 0x006ffde0 _$E276
// FUNCTION: STBC 0x006ffdf0 _$E275
// FUNCTION: STBC 0x006ffe00 _$E114
// FUNCTION: STBC 0x006ffe10 _$E113
// FUNCTION: STBC 0x006ffe20 _$E258
// FUNCTION: STBC 0x006ffe30 _$E257
// FUNCTION: STBC 0x006ffe40 _$E265
// FUNCTION: STBC 0x006ffe50 _$E264
// FUNCTION: STBC 0x006ffe60 _$E288
// FUNCTION: STBC 0x006ffe70 _$E287
// FUNCTION: STBC 0x006ffe80 _$E292
// FUNCTION: STBC 0x006ffe90 _$E291
// FUNCTION: STBC 0x006ffea0 _$E332
// FUNCTION: STBC 0x006ffeb0 _$E331
// FUNCTION: STBC 0x006ffec0 _$E338
// FUNCTION: STBC 0x006ffed0 _$E337
// FUNCTION: STBC 0x006ffee0 _$E280
// FUNCTION: STBC 0x006ffef0 _$E279
// FUNCTION: STBC 0x006fff00 _$E284
// FUNCTION: STBC 0x006fff10 _$E283
// FUNCTION: STBC 0x006fff20 _$E296
// FUNCTION: STBC 0x006fff30 _$E295
// FUNCTION: STBC 0x006fff40 _$E300
// FUNCTION: STBC 0x006fff50 _$E299
// FUNCTION: STBC 0x006fff60 _$E304
// FUNCTION: STBC 0x006fff70 _$E303
// FUNCTION: STBC 0x006fff80 _$E308
// FUNCTION: STBC 0x006fff90 _$E307
// FUNCTION: STBC 0x006fffa0 _$E312
// FUNCTION: STBC 0x006fffb0 _$E311
// FUNCTION: STBC 0x006fffc0 _$E316
// FUNCTION: STBC 0x006fffd0 _$E315
// FUNCTION: STBC 0x006fffe0 _$E320
// FUNCTION: STBC 0x006ffff0 _$E319
// FUNCTION: STBC 0x00700000 _$E324
// FUNCTION: STBC 0x00700010 _$E323
// FUNCTION: STBC 0x00700020 _$E119
// FUNCTION: STBC 0x00700030 _$E118
// FUNCTION: STBC 0x00700040 _$E328
// FUNCTION: STBC 0x00700050 _$E327
// FUNCTION: STBC 0x00700060 _$E124
// FUNCTION: STBC 0x00700070 _$E123
// FUNCTION: STBC 0x00700080 _$E342
// FUNCTION: STBC 0x00700090 _$E341
// FUNCTION: STBC 0x007000a0 _$E346
// FUNCTION: STBC 0x007000b0 _$E345
// FUNCTION: STBC 0x007000c0 _$E350
// FUNCTION: STBC 0x007000d0 _$E349
// FUNCTION: STBC 0x007000e0 _$E354
// FUNCTION: STBC 0x007000f0 _$E353
// FUNCTION: STBC 0x00700100 _$E129
// FUNCTION: STBC 0x00700110 _$E128
// FUNCTION: STBC 0x00700120 _$E358
// FUNCTION: STBC 0x00700130 _$E357
// FUNCTION: STBC 0x00700140 _$E362
// FUNCTION: STBC 0x00700150 _$E361
// FUNCTION: STBC 0x00700160 _$E366
// FUNCTION: STBC 0x00700170 _$E365
// FUNCTION: STBC 0x00700180 _$E135
// FUNCTION: STBC 0x00700190 _$E134
// FUNCTION: STBC 0x007001a0 _$E140
// FUNCTION: STBC 0x007001b0 _$E139
// FUNCTION: STBC 0x007001c0 _$E170
// FUNCTION: STBC 0x007001d0 _$E169
// FUNCTION: STBC 0x007001e0 _$E370
// FUNCTION: STBC 0x007001f0 _$E369
// FUNCTION: STBC 0x00700200 _$E175
// FUNCTION: STBC 0x00700210 _$E174
// FUNCTION: STBC 0x00700220 _$E374
// FUNCTION: STBC 0x00700230 _$E373
// FUNCTION: STBC 0x00700240 _$E378
// FUNCTION: STBC 0x00700250 _$E377
// FUNCTION: STBC 0x00700260 _$E180
// FUNCTION: STBC 0x00700270 _$E179
// FUNCTION: STBC 0x00700280 _$E382
// FUNCTION: STBC 0x00700290 _$E381
// FUNCTION: STBC 0x007002a0 _$E145
// FUNCTION: STBC 0x007002b0 _$E144
// FUNCTION: STBC 0x007002c0 _$E386
// FUNCTION: STBC 0x007002d0 _$E385
// FUNCTION: STBC 0x007002e0 _$E390
// FUNCTION: STBC 0x007002f0 _$E389
// FUNCTION: STBC 0x00700300 _$E185
// FUNCTION: STBC 0x00700310 _$E184
// FUNCTION: STBC 0x00700320 _$E190
// FUNCTION: STBC 0x00700330 _$E189
// FUNCTION: STBC 0x00700340 _$E394
// FUNCTION: STBC 0x00700350 _$E393
// FUNCTION: STBC 0x00700360 _$E398
// FUNCTION: STBC 0x00700370 _$E397
// FUNCTION: STBC 0x00700380 _$E402
// FUNCTION: STBC 0x00700390 _$E401
// FUNCTION: STBC 0x007003a0 _$E406
// FUNCTION: STBC 0x007003b0 _$E405
// FUNCTION: STBC 0x007003c0 _$E410
// FUNCTION: STBC 0x007003d0 _$E409
// FUNCTION: STBC 0x007003e0 _$E414
// FUNCTION: STBC 0x007003f0 _$E413
// FUNCTION: STBC 0x00700400 _$E418
// FUNCTION: STBC 0x00700410 _$E417
// FUNCTION: STBC 0x00700420 _$E422
// FUNCTION: STBC 0x00700430 _$E421
// FUNCTION: STBC 0x00700440 _$E195
// FUNCTION: STBC 0x00700450 _$E194
// FUNCTION: STBC 0x00700460 _$E426
// FUNCTION: STBC 0x00700470 _$E425
// FUNCTION: STBC 0x00700480 _$E200
// FUNCTION: STBC 0x00700490 _$E199
// FUNCTION: STBC 0x007004a0 _$E205
// FUNCTION: STBC 0x007004b0 _$E204

// The standard streams' initialisers and exit-time destructors (<iostream>).

// A static of an included header: its initialiser with a guard byte, and the exit-time registration.
// FUNCTION: STBC 0x00700f40 _$E434
// FUNCTION: STBC 0x00700f60 _$E433

/*
================
TGSequence::TGSequence
================
*/
// FUNCTION: STBC 0x007004c0
TGSequence::TGSequence( long id ) : TGAction( id ) {
	numActions = 0;
	actions = NULL;
	numTimers = 0;
	timerIDs = NULL;
	completing = false;
}

/*
================
TGSequence::GetObjType
================
*/
// FUNCTION: STBC 0x007004f0
int TGSequence::GetObjType( void ) {
	return TGSEQUENCE_TYPE;
}

/*
================
TGSequence::GetClassName
================
*/
// FUNCTION: STBC 0x00700530
char *TGSequence::GetClassName( void ) {
	return "TGSequence";
}

/*
================
TGSequence::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00700540
char *TGSequence::GetSwigTypeName( void ) {
	return "_p_TGSequence";
}

/*
================
TGSequence::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00700550
char *TGSequence::GetPythonClassName( void ) {
	return "TGSequencePtr";
}

/*
================
TGSequence::~TGSequence
================
*/
// FUNCTION: STBC 0x00700590
TGSequence::~TGSequence( void ) {
	if ( actions != NULL ) {
		free( actions );
	}
	KillTimers();
}

/*
================
TGSequence::AddTimerID
================
*/
// FUNCTION: STBC 0x00700600
void TGSequence::AddTimerID( long id ) {
	timerIDs = (long *)realloc( timerIDs, ( numTimers + 1 ) * sizeof( long ) );
	timerIDs[numTimers] = id;
	numTimers++;
}

/*
================
TGSequence::AddEntry
================
*/
// FUNCTION: STBC 0x00700640
void TGSequence::AddEntry( TGAction *action, TGAction *prerequisite, float delay ) {
	actions = (TGSequenceEntry *)realloc( actions, ( numActions + 1 ) * sizeof( TGSequenceEntry ) );
	actions[numActions].action = action;
	actions[numActions].prerequisite = prerequisite;
	actions[numActions].delay = delay;
	numActions++;
}

/*
================
TGSequence::KillTimers

A timer that is not one of g_kTimerManager's is one of
g_kRealtimeTimerManager's.
================
*/
// FUNCTION: STBC 0x007006a0
void TGSequence::KillTimers( void ) {
	if ( timerIDs != NULL ) {
		for ( int i = 0; i < numTimers; i++ ) {
			if ( !g_kTimerManager.DeleteTimer( timerIDs[i] ) ) {
				g_kRealtimeTimerManager.DeleteTimer( timerIDs[i] );
			}
		}
		numTimers = 0;
		free( timerIDs );
		timerIDs = NULL;
	}
}

/*
================
TGSequence::AddAction
================
*/
// FUNCTION: STBC 0x00700710
void TGSequence::AddAction( TGAction *action, TGAction *prerequisite, float delay ) {
	if ( prerequisite != NULL ) {
		bool found = false;
		TGSequenceEntry *entry = actions;
		for ( int i = 0; i < numActions; entry++, i++ ) {
			if ( entry->action == prerequisite ) {
				found = true;
			}
		}
		if ( !found ) {
			if ( action != NULL ) {
				delete action;
			}
			return;
		}

		// The prerequisite's completion starts the action.
		TGSequenceEvent *event = new TGSequenceEvent;
		if ( delay > 0.0f ) {
			event->SetEventType( ET_PLAY_SEQUENCE_ACTION_DELAYED );
		} else {
			event->SetEventType( ET_PLAY_SEQUENCE_ACTION );
		}
		event->SetDestination( &g_kTGActionManager );
		event->SetObjPtr( action );
		event->SetDelay( delay );
		prerequisite->AddCompletedEvent( event );
	}

	AddEntry( action, prerequisite, delay );
	action->SetSequence( this );
}

/*
================
TGSequence::AppendAction
================
*/
// FUNCTION: STBC 0x007007f0
void TGSequence::AppendAction( TGAction *action, float delay ) {
	if ( delay < 0.0f ) {
		delay = 0.0f;
	}
	if ( action == NULL ) {
		return;
	}

	// The last action still in the sequence.
	TGAction *last = NULL;
	if ( numActions > 0 ) {
		for ( int i = numActions - 1; i >= 0; i-- ) {
			last = actions[i].action;
			if ( last != NULL ) {
				break;
			}
		}
	}

	if ( last != NULL ) {
		TGSequenceEvent *event = new TGSequenceEvent;
		if ( delay > 0.0f ) {
			event->SetEventType( ET_PLAY_SEQUENCE_ACTION_DELAYED );
		} else {
			event->SetEventType( ET_PLAY_SEQUENCE_ACTION );
		}
		event->SetDestination( &g_kTGActionManager );
		event->SetObjPtr( action );
		event->SetDelay( delay );
		last->AddCompletedEvent( event );
	}

	AddEntry( action, last, delay );
	action->SetSequence( this );
}

/*
================
TGSequence::Play

Starts the actions that wait for no other: at once, or after their delay
on a timer.
================
*/
// FUNCTION: STBC 0x007008e0
void TGSequence::Play( void ) {
	TGAction::Play();
	float gameTime = UtopiaApp::GetApp()->GetGameTime();

	int count = numActions;
	if ( count == 0 ) {
		Completed();
		return;
	}

	for ( int i = 0; i < count; i++ ) {
		TGSequenceEntry *entry = &actions[i];
		if ( entry->action == NULL ) {
			continue;
		}
		if ( g_kTGActionManager.FindAction( entry->action ) == -1 ) {
			entry->action = NULL;
			continue;
		}
		if ( entry->prerequisite != NULL ) {
			continue;
		}

		if ( entry->delay == 0.0f ) {
			entry->action->Play();
			continue;
		}

		TGTimer *timer = new TGTimer;
		TGSequenceEvent *event = new TGSequenceEvent;
		event->SetEventType( ET_PLAY_SEQUENCE_ACTION );
		event->SetDestination( &g_kTGActionManager );
		event->SetObjPtr( entry->action );
		if ( entry->action->IsUseRealTime() ) {
			timer->SetTimerStart( g_kUtopiaModule.GetRealTime() + entry->delay );
			timer->SetEvent( event );
			g_kRealtimeTimerManager.AddTimer( timer );
		} else {
			timer->SetTimerStart( gameTime + entry->delay );
			timer->SetEvent( event );
			g_kTimerManager.AddTimer( timer );
		}
		AddTimerID( timer->GetObjID() );
	}
}

/*
================
TGSequence::Completed

Skips the actions still running, then completes as an action.
================
*/
// FUNCTION: STBC 0x00700a70
void TGSequence::Completed( void ) {
	completing = true;

	int count = numActions;
	int remaining = 0;
	int i;
	for ( i = 0; i < count; i++ ) {
		if ( actions[i].action != NULL ) {
			remaining++;
		}
	}

	for ( i = 0; remaining != 0 && i < count; i++ ) {
		TGSequenceEntry entry = actions[i];
		if ( entry.action != NULL ) {
			remaining--;
			entry.action->Skip();
		}
	}

	KillTimers();
	TGAction::Completed();
}

/*
================
TGSequence::ActionCompleted
================
*/
// FUNCTION: STBC 0x00700af0
void TGSequence::ActionCompleted( TGAction *action ) {
	bool done = true;
	TGSequenceEntry *entry = actions;
	for ( int i = 0; i < numActions; entry++, i++ ) {
		if ( entry->action == action ) {
			entry->action = NULL;
		}
		if ( entry->action != NULL ) {
			done = false;
		}
	}

	if ( done && !completing ) {
		Completed();
	}
}

/*
================
TGSequence::Skip

Skips the actions still running without sending their completion events;
the last one to complete completes the sequence.
================
*/
// FUNCTION: STBC 0x00700b40
void TGSequence::Skip( void ) {
	KillTimers();

	int count = numActions;
	int remaining = 0;
	int i;
	for ( i = 0; i < count; i++ ) {
		if ( actions[i].action != NULL ) {
			remaining++;
		}
	}

	for ( i = 0; remaining != 0 && i < count; i++ ) {
		TGSequenceEntry entry = actions[i];
		if ( entry.action != NULL ) {
			remaining--;
			entry.action->ReleaseCompletedEvents();
			entry.action->Skip();
		}
	}
}

/*
================
TGSequence::WriteToStream

An action that has completed is written as 0 alone.
================
*/
// FUNCTION: STBC 0x00700bc0
// PARKED: same calls and blocks; retail cross-jumps the prerequisite ID write into the invalid-prerequisite write and keeps the previous-entry-NULL write apart, the build merges the two zero writes instead (if/else orders, ternary and flag forms tried)
bool TGSequence::WriteToStream( TGStream *stream ) {
	if ( !TGAction::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteLong( numTimers );
	stream->WriteLong( numActions );
	int i;
	for ( i = 0; i < numTimers; i++ ) {
		stream->WriteID( timerIDs[i] );
	}

	for ( i = 0; i < numActions; i++ ) {
		TGSequenceEntry entry = actions[i];
		if ( entry.action != NULL && g_kTGActionManager.FindAction( entry.action ) != -1 ) {
			stream->WriteID( entry.action->GetObjID() );
			if ( entry.prerequisite != NULL && g_kTGActionManager.FindAction( entry.prerequisite ) != -1 ) {
				// The original tests the entry before this one, not the
				// prerequisite: the first entry never has a prerequisite.
				if ( actions[i - 1].action != NULL ) {
					stream->WriteID( entry.prerequisite->GetObjID() );
					stream->WriteFloat( entry.delay );
				} else {
					stream->WriteID( 0 );
					stream->WriteFloat( entry.delay );
				}
			} else {
				stream->WriteID( 0 );
				stream->WriteFloat( entry.delay );
			}
		} else {
			stream->WriteID( 0 );
		}
	}
	return true;
}

/*
================
TGSequence::ReadFromStream

The actions are read as IDs; PostFixUp turns them into pointers.
================
*/
// FUNCTION: STBC 0x00700d00
bool TGSequence::ReadFromStream( TGStream *stream ) {
	if ( !TGAction::ReadFromStream( stream ) ) {
		return false;
	}

	int timerCount = stream->ReadLong();
	int actionCount = stream->ReadLong();
	int i;
	for ( i = 0; i < timerCount; i++ ) {
		AddTimerID( stream->ReadID() );
	}

	for ( i = 0; i < actionCount; i++ ) {
		TGAction *action = (TGAction *)stream->ReadID();
		if ( action != NULL ) {
			TGAction *prerequisite = (TGAction *)stream->ReadID();
			float delay = stream->ReadFloat();
			AddEntry( action, prerequisite, delay );
		} else {
			AddEntry( NULL, NULL, 0.0f );
		}
	}
	return true;
}

/*
================
TGSequence::FixUpPointers

False while an action or prerequisite is not loaded yet.
================
*/
// FUNCTION: STBC 0x00700da0
bool TGSequence::FixUpPointers( void ) {
	if ( !TGAction::FixUpPointers() ) {
		return false;
	}

	for ( int i = 0; i < numActions; i++ ) {
		if ( actions[i].action != NULL && GetTGObjectPtr( (long)actions[i].action ) == NULL ) {
			return false;
		}
		if ( actions[i].prerequisite != NULL && GetTGObjectPtr( (long)actions[i].prerequisite ) == NULL ) {
			return false;
		}
	}
	return true;
}

/*
================
TGSequence::PostFixUp
================
*/
// FUNCTION: STBC 0x00700e00
void TGSequence::PostFixUp( void ) {
	TGAction::PostFixUp();

	for ( int i = 0; i < numActions; i++ ) {
		TGSequenceEntry entry = actions[i];
		entry.action = (TGAction *)GetSavedObject( (long)entry.action );
		entry.prerequisite = (TGAction *)GetSavedObject( (long)entry.prerequisite );
		if ( entry.action != NULL ) {
			entry.action->SetSequence( this );
		}
		actions[i] = entry;
	}
}

/*
================
TGSequenceEvent::Copy
================
*/
// FUNCTION: STBC 0x00700e90
bool TGSequenceEvent::Copy( TGEvent *event ) {
	bool copied = TGObjPtrEvent::Copy( event );
	if ( copied == true ) {
		delay = ( (TGSequenceEvent *)event )->delay;
	}
	return copied;
}

/*
================
TGSequenceEvent::WriteToStream
================
*/
// FUNCTION: STBC 0x00700eb0
bool TGSequenceEvent::WriteToStream( TGStream *stream ) {
	if ( !TGObjPtrEvent::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( delay );
	return true;
}

/*
================
TGSequenceEvent::ReadFromStream
================
*/
// FUNCTION: STBC 0x00700ee0
bool TGSequenceEvent::ReadFromStream( TGStream *stream ) {
	if ( !TGObjPtrEvent::ReadFromStream( stream ) ) {
		return false;
	}
	delay = stream->ReadFloat();
	return true;
}

/*
================
TGSequence::Cast
================
*/
// FUNCTION: STBC 0x00700f10
TGSequence *TGSequence::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TGSEQUENCE_TYPE ) ) {
		return (TGSequence *)object;
	}
	return NULL;
}
