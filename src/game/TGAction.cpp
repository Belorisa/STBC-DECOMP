/*
===============================================================================

	TGAction.cpp

===============================================================================
*/

#include <iostream>
#include <windows.h>

// <windows.h>'s ANSI/wide macro would rename TGObject's method.
#undef GetClassName

#include "TGAction.h"
#include "TGActionManager.h"
#include "TGEvent.h"
#include "TGEventManager.h"
#include "TGSequence.h"
#include "TGSequenceEvent.h"
#include "TGEventHandlerTable.h"
#include "TGStream.h"
#include "TGTimer.h"
#include "TGTimerManager.h"
#include "UtopiaModule.h"
#include "NiTList.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006ff630 _$E447
// FUNCTION: STBC 0x006ff650 _$E446

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006fdb70 _$E32
// FUNCTION: STBC 0x006fdb80 _$E29
// FUNCTION: STBC 0x006fdb90 _$E31
// FUNCTION: STBC 0x006fdba0 _$E30
// FUNCTION: STBC 0x006fdbb0 _$E38
// FUNCTION: STBC 0x006fdbc0 _$E35
// FUNCTION: STBC 0x006fdbd0 _$E37
// FUNCTION: STBC 0x006fdbe0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006fdbf0 _$E69
// FUNCTION: STBC 0x006fdc00 _$E68
// FUNCTION: STBC 0x006fdc10 _$E225
// FUNCTION: STBC 0x006fdc20 _$E224
// FUNCTION: STBC 0x006fdc30 _$E80
// FUNCTION: STBC 0x006fdc40 _$E79
// FUNCTION: STBC 0x006fdc50 _$E84
// FUNCTION: STBC 0x006fdc60 _$E83
// FUNCTION: STBC 0x006fdc70 _$E88
// FUNCTION: STBC 0x006fdc80 _$E87
// FUNCTION: STBC 0x006fdc90 _$E92
// FUNCTION: STBC 0x006fdca0 _$E91
// FUNCTION: STBC 0x006fdcb0 _$E212
// FUNCTION: STBC 0x006fdcc0 _$E211
// FUNCTION: STBC 0x006fdcd0 _$E154
// FUNCTION: STBC 0x006fdce0 _$E153
// FUNCTION: STBC 0x006fdcf0 _$E167
// FUNCTION: STBC 0x006fdd00 _$E166
// FUNCTION: STBC 0x006fdd10 _$E217
// FUNCTION: STBC 0x006fdd20 _$E216
// FUNCTION: STBC 0x006fdd30 _$E230
// FUNCTION: STBC 0x006fdd40 _$E229
// FUNCTION: STBC 0x006fdd50 _$E221
// FUNCTION: STBC 0x006fdd60 _$E220
// FUNCTION: STBC 0x006fdd70 _$E234
// FUNCTION: STBC 0x006fdd80 _$E233
// FUNCTION: STBC 0x006fdd90 _$E241
// FUNCTION: STBC 0x006fdda0 _$E240
// FUNCTION: STBC 0x006fddb0 _$E274
// FUNCTION: STBC 0x006fddc0 _$E273
// FUNCTION: STBC 0x006fddd0 _$E278
// FUNCTION: STBC 0x006fdde0 _$E277
// FUNCTION: STBC 0x006fddf0 _$E116
// FUNCTION: STBC 0x006fde00 _$E115
// FUNCTION: STBC 0x006fde10 _$E260
// FUNCTION: STBC 0x006fde20 _$E259
// FUNCTION: STBC 0x006fde30 _$E267
// FUNCTION: STBC 0x006fde40 _$E266
// FUNCTION: STBC 0x006fde50 _$E290
// FUNCTION: STBC 0x006fde60 _$E289
// FUNCTION: STBC 0x006fde70 _$E294
// FUNCTION: STBC 0x006fde80 _$E293
// FUNCTION: STBC 0x006fde90 _$E334
// FUNCTION: STBC 0x006fdea0 _$E333
// FUNCTION: STBC 0x006fdeb0 _$E340
// FUNCTION: STBC 0x006fdec0 _$E339
// FUNCTION: STBC 0x006fded0 _$E282
// FUNCTION: STBC 0x006fdee0 _$E281
// FUNCTION: STBC 0x006fdef0 _$E286
// FUNCTION: STBC 0x006fdf00 _$E285
// FUNCTION: STBC 0x006fdf10 _$E298
// FUNCTION: STBC 0x006fdf20 _$E297
// FUNCTION: STBC 0x006fdf30 _$E302
// FUNCTION: STBC 0x006fdf40 _$E301
// FUNCTION: STBC 0x006fdf50 _$E306
// FUNCTION: STBC 0x006fdf60 _$E305
// FUNCTION: STBC 0x006fdf70 _$E310
// FUNCTION: STBC 0x006fdf80 _$E309
// FUNCTION: STBC 0x006fdf90 _$E314
// FUNCTION: STBC 0x006fdfa0 _$E313
// FUNCTION: STBC 0x006fdfb0 _$E318
// FUNCTION: STBC 0x006fdfc0 _$E317
// FUNCTION: STBC 0x006fdfd0 _$E322
// FUNCTION: STBC 0x006fdfe0 _$E321
// FUNCTION: STBC 0x006fdff0 _$E326
// FUNCTION: STBC 0x006fe000 _$E325
// FUNCTION: STBC 0x006fe010 _$E121
// FUNCTION: STBC 0x006fe020 _$E120
// FUNCTION: STBC 0x006fe030 _$E330
// FUNCTION: STBC 0x006fe040 _$E329
// FUNCTION: STBC 0x006fe050 _$E126
// FUNCTION: STBC 0x006fe060 _$E125
// FUNCTION: STBC 0x006fe070 _$E344
// FUNCTION: STBC 0x006fe080 _$E343
// FUNCTION: STBC 0x006fe090 _$E348
// FUNCTION: STBC 0x006fe0a0 _$E347
// FUNCTION: STBC 0x006fe0b0 _$E352
// FUNCTION: STBC 0x006fe0c0 _$E351
// FUNCTION: STBC 0x006fe0d0 _$E356
// FUNCTION: STBC 0x006fe0e0 _$E355
// FUNCTION: STBC 0x006fe0f0 _$E131
// FUNCTION: STBC 0x006fe100 _$E130
// FUNCTION: STBC 0x006fe110 _$E360
// FUNCTION: STBC 0x006fe120 _$E359
// FUNCTION: STBC 0x006fe130 _$E364
// FUNCTION: STBC 0x006fe140 _$E363
// FUNCTION: STBC 0x006fe150 _$E368
// FUNCTION: STBC 0x006fe160 _$E367
// FUNCTION: STBC 0x006fe170 _$E137
// FUNCTION: STBC 0x006fe180 _$E136
// FUNCTION: STBC 0x006fe190 _$E142
// FUNCTION: STBC 0x006fe1a0 _$E141
// FUNCTION: STBC 0x006fe1b0 _$E172
// FUNCTION: STBC 0x006fe1c0 _$E171
// FUNCTION: STBC 0x006fe1d0 _$E372
// FUNCTION: STBC 0x006fe1e0 _$E371
// FUNCTION: STBC 0x006fe1f0 _$E177
// FUNCTION: STBC 0x006fe200 _$E176
// FUNCTION: STBC 0x006fe210 _$E376
// FUNCTION: STBC 0x006fe220 _$E375
// FUNCTION: STBC 0x006fe230 _$E380
// FUNCTION: STBC 0x006fe240 _$E379
// FUNCTION: STBC 0x006fe250 _$E182
// FUNCTION: STBC 0x006fe260 _$E181
// FUNCTION: STBC 0x006fe270 _$E384
// FUNCTION: STBC 0x006fe280 _$E383
// FUNCTION: STBC 0x006fe290 _$E147
// FUNCTION: STBC 0x006fe2a0 _$E146
// FUNCTION: STBC 0x006fe2b0 _$E388
// FUNCTION: STBC 0x006fe2c0 _$E387
// FUNCTION: STBC 0x006fe2d0 _$E392
// FUNCTION: STBC 0x006fe2e0 _$E391
// FUNCTION: STBC 0x006fe2f0 _$E187
// FUNCTION: STBC 0x006fe300 _$E186
// FUNCTION: STBC 0x006fe310 _$E192
// FUNCTION: STBC 0x006fe320 _$E191
// FUNCTION: STBC 0x006fe330 _$E396
// FUNCTION: STBC 0x006fe340 _$E395
// FUNCTION: STBC 0x006fe350 _$E400
// FUNCTION: STBC 0x006fe360 _$E399
// FUNCTION: STBC 0x006fe370 _$E404
// FUNCTION: STBC 0x006fe380 _$E403
// FUNCTION: STBC 0x006fe390 _$E408
// FUNCTION: STBC 0x006fe3a0 _$E407
// FUNCTION: STBC 0x006fe3b0 _$E412
// FUNCTION: STBC 0x006fe3c0 _$E411
// FUNCTION: STBC 0x006fe3d0 _$E416
// FUNCTION: STBC 0x006fe3e0 _$E415
// FUNCTION: STBC 0x006fe3f0 _$E420
// FUNCTION: STBC 0x006fe400 _$E419
// FUNCTION: STBC 0x006fe410 _$E424
// FUNCTION: STBC 0x006fe420 _$E423
// FUNCTION: STBC 0x006fe430 _$E197
// FUNCTION: STBC 0x006fe440 _$E196
// FUNCTION: STBC 0x006fe450 _$E428
// FUNCTION: STBC 0x006fe460 _$E427
// FUNCTION: STBC 0x006fe470 _$E202
// FUNCTION: STBC 0x006fe480 _$E201
// FUNCTION: STBC 0x006fe490 _$E207
// FUNCTION: STBC 0x006fe4a0 _$E206

// The class handler table's initialiser and exit-time destructor.
// FUNCTION: STBC 0x006fe4b0 _$E434
// FUNCTION: STBC 0x006fe4c0 _$E430
// FUNCTION: STBC 0x006fe4d0 _$E433
// FUNCTION: STBC 0x006fe4e0 _$E431
// DATA: STBC 0x0099af78
TGEventHandlerTable TGActionManager::classHandlers;

/*
================
TGAction::Cast
================
*/
// FUNCTION: STBC 0x006fe500
TGAction *TGAction::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TGACTION_TYPE ) ) {
		return (TGAction *)object;
	}
	return NULL;
}

// DATA: STBC 0x0099b010
TGActionManager g_kTGActionManager;

/*
================
TGAction::TGAction
================
*/
// FUNCTION: STBC 0x006fe5a0
TGAction::TGAction( long id ) : TGObject( id ) {
	g_kTGActionManager.AddAction( this );
	numCompletedEvents = 0;
	skippable = false;
	completedEvents = NULL;
	sequence = NULL;
	startTime = ACTION_NOT_PLAYING;
	useRealTime = false;
	surviveGlobalAbort = false;
}

/*
================
TGAction::GetObjType
================
*/
// FUNCTION: STBC 0x006fe620
int TGAction::GetObjType( void ) {
	return TGACTION_TYPE;
}

/*
================
TGAction::GetClassName
================
*/
// FUNCTION: STBC 0x006fe650
char *TGAction::GetClassName( void ) {
	return "TGAction";
}

/*
================
TGAction::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x006fe660
char *TGAction::GetSwigTypeName( void ) {
	return "_p_TGAction";
}

/*
================
TGAction::GetPythonClassName
================
*/
// FUNCTION: STBC 0x006fe670
char *TGAction::GetPythonClassName( void ) {
	return "TGActionPtr";
}

/*
================
TGAction::~TGAction
================
*/
// FUNCTION: STBC 0x006fe6b0
TGAction::~TGAction( void ) {
	g_kTGActionManager.RemoveAction( this );
	if ( completedEvents != NULL ) {
		for ( int i = 0; i < numCompletedEvents; i++ ) {
			completedEvents[i]->DecRefCount();
		}
		free( completedEvents );
	}
	completedEvents = NULL;
	numCompletedEvents = 0;
}

/*
================
TGAction::AddCompletedEvent

The event is sent when the action ends; the action holds a reference.
================
*/
// FUNCTION: STBC 0x006fe760
void TGAction::AddCompletedEvent( TGEvent *event ) {
	completedEvents = (TGEvent **)realloc( completedEvents, ( numCompletedEvents + 1 ) * sizeof( TGEvent * ) );
	event->IncRefCount();
	completedEvents[numCompletedEvents] = event;
	numCompletedEvents++;
}

/*
================
TGAction::Completed

Sends the completion events, tells the sequence, and deletes the action.
================
*/
// FUNCTION: STBC 0x006fe7c0
void TGAction::Completed( void ) {
	SendCompletedEvents();
	if ( sequence != NULL && g_kTGActionManager.FindAction( sequence ) != -1 ) {
		sequence->ActionCompleted( this );
	}
	delete this;
}

/*
================
TGAction::Play
================
*/
// FUNCTION: STBC 0x006fe800
void TGAction::Play( void ) {
	if ( useRealTime ) {
		startTime = g_kUtopiaModule.GetRealTime();
	} else {
		startTime = g_kUtopiaModule.GetGameTime();
	}
}

/*
================
TGAction::Restart
================
*/
// FUNCTION: STBC 0x006fe830
void TGAction::Restart( float delay ) {
	if ( useRealTime ) {
		startTime = g_kUtopiaModule.GetRealTime() - delay;
	} else {
		startTime = g_kUtopiaModule.GetGameTime() - delay;
	}
}

/*
================
TGAction::Abort

Ends the action without its completion events; an action in a sequence
aborts the sequence instead.
================
*/
// FUNCTION: STBC 0x006fe860
void TGAction::Abort( void ) {
	ReleaseCompletedEvents();
	if ( sequence != NULL && g_kTGActionManager.FindAction( sequence ) != -1 ) {
		sequence->Abort();
	} else {
		sequence = NULL;
		Completed();
	}
}

/*
================
TGAction::Skip
================
*/
// FUNCTION: STBC 0x006fe8a0
void TGAction::Skip( void ) {
	SendCompletedEvents();
	if ( sequence != NULL && g_kTGActionManager.FindAction( sequence ) != -1 ) {
		sequence->ActionCompleted( this );
	}
	delete this;
}

/*
================
TGAction::SendCompletedEvents
================
*/
// FUNCTION: STBC 0x006fe8e0
void TGAction::SendCompletedEvents( void ) {
	for ( int i = 0; i < numCompletedEvents; i++ ) {
		g_kEventManager.AddEvent( completedEvents[i] );
		completedEvents[i]->DecRefCount();
	}
	numCompletedEvents = 0;
	if ( completedEvents != NULL ) {
		free( completedEvents );
	}
	completedEvents = NULL;
}

/*
================
TGAction::SetSequence
================
*/
// FUNCTION: STBC 0x006fe950
void TGAction::SetSequence( TGSequence *sequence_ ) {
	sequence = sequence_;
}

/*
================
TGAction::IsPartOfSequence

Whether the sequence plays the action, directly or through sequences
inside it.
================
*/
// FUNCTION: STBC 0x006fe960
bool TGAction::IsPartOfSequence( TGSequence *sequence ) {
	TGSequence *parent = this->sequence;
	while ( parent != sequence ) {
		if ( parent == NULL ) {
			return false;
		}
		parent = parent->GetSequence();
	}
	return true;
}

/*
================
TGAction::ReleaseCompletedEvents
================
*/
// FUNCTION: STBC 0x006fe980
void TGAction::ReleaseCompletedEvents( void ) {
	if ( completedEvents != NULL ) {
		for ( int i = 0; i < numCompletedEvents; i++ ) {
			completedEvents[i]->DecRefCount();
		}
		free( completedEvents );
	}
	completedEvents = NULL;
	numCompletedEvents = 0;
}

/*
================
TGAction::WriteToStream
================
*/
// FUNCTION: STBC 0x006fe9e0
bool TGAction::WriteToStream( TGStream *stream ) {
	if ( !TGObject::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteLong( numCompletedEvents );
	for ( int i = 0; i < numCompletedEvents; i++ ) {
		stream->WriteID( completedEvents[i]->GetObjID() );
	}
	stream->WriteBool( skippable );
	stream->WriteFloat( startTime );
	stream->WriteBool( useRealTime );
	stream->WriteBool( surviveGlobalAbort );
	return true;
}

/*
================
TGAction::ReadFromStream

The completion events are read as IDs; PostFixUp turns them into
pointers.
================
*/
// FUNCTION: STBC 0x006fea70
bool TGAction::ReadFromStream( TGStream *stream ) {
	numCompletedEvents = stream->ReadLong();
	completedEvents = (TGEvent **)malloc( numCompletedEvents * sizeof( TGEvent * ) );
	for ( int i = 0; i < numCompletedEvents; i++ ) {
		completedEvents[i] = (TGEvent *)stream->ReadID();
	}
	skippable = stream->ReadBool();
	startTime = stream->ReadFloat();
	useRealTime = stream->ReadBool();
	surviveGlobalAbort = stream->ReadBool();
	return true;
}

/*
================
TGAction::FixUpPointers

False while a completion event is not loaded yet.
================
*/
// FUNCTION: STBC 0x006feb20
bool TGAction::FixUpPointers( void ) {
	for ( int i = 0; i < numCompletedEvents; i++ ) {
		if ( completedEvents[i] != NULL && TGEvent::GetTGEventPtr( (long)completedEvents[i] ) == NULL ) {
			return false;
		}
	}
	return true;
}

/*
================
TGAction::PostFixUp
================
*/
// FUNCTION: STBC 0x006feb60
void TGAction::PostFixUp( void ) {
	for ( int i = 0; i < numCompletedEvents; i++ ) {
		completedEvents[i] = TGEvent::GetTGEventPtr( (long)completedEvents[i] );
	}
}

/*
================
TGActionManager::Unknown6FEBA0

Empty in this release; the actions module calls it after the manager is
set up and again after DeleteAllActions.
================
*/
// FUNCTION: STBC 0x006feba0
void TGActionManager::Unknown6FEBA0( void ) {
}

/*
================
TGActionManager::DeleteAllActions

Every action unregisters itself in its destructor, so the first record
is a new one each time.
================
*/
// FUNCTION: STBC 0x006febb0
void TGActionManager::DeleteAllActions( void ) {
	OutputDebugString( "Shutting down Action Manager, deleting all valid actions\n" );

	TGActionRecord **records = actions.GetItems();
	for ( int i = 0; i < actions.GetCount(); i++ ) {
		records[i]->action->ReleaseCompletedEvents();
	}
	while ( actions.GetCount() > 0 ) {
		delete actions.GetAt( 0 )->action;
	}
}

/*
================
TGActionManager::SkipPlayingActions

The actions are gathered first: skipping one ends it, which changes the
records.
================
*/
// FUNCTION: STBC 0x006fec00
void TGActionManager::SkipPlayingActions( void ) {
	TGActionRecord **records = actions.GetItems();
	int count = actions.GetCount();
	NiTList<TGAction *> skipped( 2 );

	for ( int i = 0; i < count; i++ ) {
		if ( records[i]->action->IsSkippable() == true && records[i]->action->IsPlaying() ) {
			skipped.AddTail( records[i]->action );
		}
	}

	NiTListIterator pos = skipped.GetHeadPos();
	while ( pos != NULL ) {
		skipped.GetNext( pos )->Skip();
	}
}

/*
================
TGActionManager::SkipEvents
================
*/
// FUNCTION: STBC 0x006fed50
void TGActionManager::SkipEvents( void ) {
	g_kTGActionManager.SkipPlayingActions();
}

/*
================
TGActionManager::SetActionName
================
*/
// FUNCTION: STBC 0x006fed60
void TGActionManager::SetActionName( TGAction *action, char *name ) {
	if ( action == NULL ) {
		return;
	}
	int index = actions.Find( action );
	if ( index < 0 ) {
		return;
	}
	TGActionRecord *record = actions.GetAt( index );
	if ( name != NULL ) {
		record->SetName( name );
	} else {
		record->SetName( ACTION_NAME_ALL );
	}
}

/*
================
TGActionManager::RegisterAction
================
*/
// FUNCTION: STBC 0x006fee60
void TGActionManager::RegisterAction( TGAction *action, char *name ) {
	g_kTGActionManager.SetActionName( action, name );
}

/*
================
TGActionManager::KillNamedActions

The actions lose their completion events first; then they are skipped,
each skip ending the action and removing its record.
================
*/
// FUNCTION: STBC 0x006fee80
void TGActionManager::KillNamedActions( char *name ) {
	char *killName;
	if ( name == NULL ) {
		killName = new char[sizeof( ACTION_NAME_ALL )];
		strcpy( killName, ACTION_NAME_ALL );
	} else {
		killName = new char[strlen( name ) + 1];
		strcpy( killName, name );
	}
	bool killAll = false;
	int numKilled = 0;
	if ( strcmp( killName, ACTION_NAME_ALL ) == 0 ) {
		killAll = true;
	}

	TGActionRecord **records = actions.GetItems();
	int i;
	for ( i = 0; i < actions.GetCount(); i++ ) {
		TGActionRecord *record = records[i];
		if ( record->name != NULL && ( strcmp( record->name, killName ) == 0 || killAll ) ) {
			record->action->ReleaseCompletedEvents();
			numKilled++;
		}
	}

	for ( i = 0; numKilled > 0 && i < actions.GetCount(); i++ ) {
		char *recordName = records[i]->name;
		if ( recordName != NULL && ( strcmp( recordName, killName ) == 0 || killAll ) ) {
			actions.GetAt( i )->action->Skip();
			numKilled--;
			records = actions.GetItems();
			i--;
		}
	}

	delete [] killName;
}

/*
================
TGActionManager::KillActions
================
*/
// FUNCTION: STBC 0x006ff030
void TGActionManager::KillActions( char *name ) {
	g_kTGActionManager.KillNamedActions( name );
}

/*
================
TGActionManager::FindAction
================
*/
// FUNCTION: STBC 0x006ff040
int TGActionManager::FindAction( TGAction *action ) {
	if ( action == NULL ) {
		return -1;
	}
	return actions.Find( action );
}

/*
================
TGActionManager::AddAction
================
*/
// FUNCTION: STBC 0x006ff090
void TGActionManager::AddAction( TGAction *action ) {
	actions.Insert( new TGActionRecord( action ) );
}

/*
================
TGActionManager::RemoveAction
================
*/
// FUNCTION: STBC 0x006ff170
void TGActionManager::RemoveAction( TGAction *action ) {
	actions.Delete( action );
}

/*
================
TGActionManager::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x006ff210
void TGActionManager::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TGActionManager::ActionCompletedHandler, "TGActionManager::ActionCompletedHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TGActionManager::ActionSkipHandler, "TGActionManager::ActionSkipHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TGActionManager::PlaySequenceActionHandler, "TGActionManager::PlaySequenceActionHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TGActionManager::PlaySequenceActionDelayedHandler, "TGActionManager::PlaySequenceActionDelayedHandler" );
}

/*
================
TGActionManager::RegisterHandlers
================
*/
// FUNCTION: STBC 0x006ff270
void TGActionManager::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_ACTION_COMPLETED, "TGActionManager::ActionCompletedHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_ACTION_SKIP, "TGActionManager::ActionSkipHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_PLAY_SEQUENCE_ACTION, "TGActionManager::PlaySequenceActionHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_PLAY_SEQUENCE_ACTION_DELAYED, "TGActionManager::PlaySequenceActionDelayedHandler" );
}

/*
================
TGActionManager::ActionCompletedHandler
================
*/
// FUNCTION: STBC 0x006ff2d0
void TGActionManager::ActionCompletedHandler( TGEvent *event ) {
	TGAction *action = (TGAction *)( (TGObjPtrEvent *)event )->GetObjPtr();
	if ( action != NULL ) {
		action->Completed();
	}
}

/*
================
TGActionManager::ActionSkipHandler
================
*/
// FUNCTION: STBC 0x006ff2f0
void TGActionManager::ActionSkipHandler( TGEvent *event ) {
	TGAction *action = (TGAction *)( (TGObjPtrEvent *)event )->GetObjPtr();
	if ( action != NULL ) {
		action->Skip();
	}
}

/*
================
TGActionManager::PlaySequenceActionHandler
================
*/
// FUNCTION: STBC 0x006ff310
void TGActionManager::PlaySequenceActionHandler( TGEvent *event ) {
	TGAction *action = (TGAction *)( (TGSequenceEvent *)event )->GetObjPtr();
	if ( action != NULL ) {
		action->Play();
	}
}

/*
================
TGActionManager::PlaySequenceActionDelayedHandler

Starts the action after the event's delay: a timer sends the manager an
ET_PLAY_SEQUENCE_ACTION for it then. The action's sequence keeps the
timer's ID, to kill it with the sequence.
================
*/
// FUNCTION: STBC 0x006ff330
void TGActionManager::PlaySequenceActionDelayedHandler( TGEvent *event ) {
	TGSequenceEvent *delayedEvent = (TGSequenceEvent *)event;
	TGAction *action = (TGAction *)delayedEvent->GetObjPtr();
	if ( action == NULL ) {
		return;
	}

	TGTimer *timer = new TGTimer;
	TGSequenceEvent *playEvent = new TGSequenceEvent;
	playEvent->SetEventType( ET_PLAY_SEQUENCE_ACTION );
	playEvent->SetDestination( this );
	playEvent->SetObjPtr( action );
	if ( action->IsUseRealTime() ) {
		timer->SetTimerStart( g_kUtopiaModule.GetRealTime() + delayedEvent->GetDelay() );
		timer->SetEvent( playEvent );
		g_kRealtimeTimerManager.AddTimer( timer );
	} else {
		timer->SetTimerStart( g_kUtopiaModule.GetGameTime() + delayedEvent->GetDelay() );
		timer->SetEvent( playEvent );
		g_kTimerManager.AddTimer( timer );
	}
	action->GetSequence()->AddTimerID( timer->GetObjID() );
}

/*
================
TGAction::SetUseRealTime
================
*/
// FUNCTION: STBC 0x006ff460
void TGAction::SetUseRealTime( bool useRealTime ) {
	this->useRealTime = useRealTime;
}

/*
================
TGActionManager::WriteToStream

Each action is written whole, between two markers.
================
*/
// FUNCTION: STBC 0x006ff470
bool TGActionManager::WriteToStream( TGStream *stream ) {
	if ( !TGEventHandlerObject::WriteToStream( stream ) ) {
		return false;
	}

	int count = actions.GetCount();
	stream->WriteLong( count );
	TGActionRecord **records = actions.GetItems();
	for ( int i = 0; i < count; i++ ) {
		stream->WriteLong( ACTION_STREAM_START );
		// The class name is fetched and not used.
		records[i]->action->GetClassName();
		records[i]->action->WriteToStream( stream );
		stream->WriteLong( ACTION_STREAM_END );
	}
	return true;
}

/*
================
TGActionManager::ReadFromStream

The actions register themselves as they are created.
================
*/
// FUNCTION: STBC 0x006ff4e0
bool TGActionManager::ReadFromStream( TGStream *stream ) {
	if ( !TGEventHandlerObject::ReadFromStream( stream ) ) {
		return false;
	}

	int count = stream->ReadLong();
	for ( int i = 0; i < count; i++ ) {
		if ( stream->ReadLong() != ACTION_STREAM_START ) {
			return false;
		}
		LoadObject( stream, 0 );
		if ( stream->ReadLong() != ACTION_STREAM_END ) {
			return false;
		}
	}
	return true;
}

/*
================
TGActionManager::RestartLoadedActions
================
*/
// FUNCTION: STBC 0x006ff540
void TGActionManager::RestartLoadedActions( void ) {
	int count = actions.GetCount();
	TGActionRecord **records = actions.GetItems();
	for ( int i = 0; i < count; i++ ) {
		float startTime = records[i]->action->GetStartTime();
		if ( startTime > ACTION_NOT_PLAYING && !records[i]->action->IsTypeOf( TGSOUNDACTION_TYPE ) ) {
			records[i]->action->Restart( startTime );
		}
	}
}

/*
================
TGActionManager::AbortAllActions

Ending an action changes the records, so the scan starts over after each.
================
*/
// FUNCTION: STBC 0x006ff590
void TGActionManager::AbortAllActions( void ) {
	bool aborted = true;
	while ( aborted ) {
		aborted = false;
		for ( int i = 0; actions.GetCount() > i; i++ ) {
			TGAction *action = actions.GetAt( i )->action;
			if ( !action->IsGlobalAbortSurvivor() ) {
				action->ReleaseCompletedEvents();
				action->Completed();
				aborted = true;
				break;
			}
		}
	}
}

/*
================
TGActionManager::SkipActionsFor
================
*/
// FUNCTION: STBC 0x006ff5d0
void TGActionManager::SkipActionsFor( TGAnimNode *node ) {
	bool skipped = true;
	while ( skipped ) {
		skipped = false;
		int count = actions.GetCount();
		TGActionRecord **records = actions.GetItems();
		for ( int i = 0; i < count; i++ ) {
			if ( records[i]->action->PlaysOn( node ) ) {
				records[i]->action->Skip();
				skipped = true;
				break;
			}
		}
	}
}

/*
================
TGActionManager::ForgetAllActions
================
*/
// FUNCTION: STBC 0x006ff620
void TGActionManager::ForgetAllActions( void ) {
	actions.RemoveAll();
}
