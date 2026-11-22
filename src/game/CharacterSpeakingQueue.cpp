/*
===============================================================================

	CharacterSpeakingQueue.cpp

	A character's queue of lines to speak.

===============================================================================
*/

#include <iostream>
#include "NiMain.h"
#include "CharacterSpeakingQueue.h"
#include "CharacterClass.h"
#include "TGAction.h"
#include "TGActionManager.h"
#include "TGEvent.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGStream.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00672aa0 _$E32
// FUNCTION: STBC 0x00672ab0 _$E29
// FUNCTION: STBC 0x00672ac0 _$E31
// FUNCTION: STBC 0x00672ad0 _$E30
// FUNCTION: STBC 0x00672ae0 _$E38
// FUNCTION: STBC 0x00672af0 _$E35
// FUNCTION: STBC 0x00672b00 _$E37
// FUNCTION: STBC 0x00672b10 _$E36

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00673000 _$E463
// FUNCTION: STBC 0x00673020 _$E462

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00672b20 _$E45
// FUNCTION: STBC 0x00672b30 _$E44
// FUNCTION: STBC 0x00672b40 _$E49
// FUNCTION: STBC 0x00672b50 _$E48
// FUNCTION: STBC 0x00672b60 _$E60
// FUNCTION: STBC 0x00672b70 _$E59
// FUNCTION: STBC 0x00672b80 _$E64
// FUNCTION: STBC 0x00672b90 _$E63
// FUNCTION: STBC 0x00672ba0 _$E68
// FUNCTION: STBC 0x00672bb0 _$E67
// FUNCTION: STBC 0x00672bc0 _$E72
// FUNCTION: STBC 0x00672bd0 _$E71
// FUNCTION: STBC 0x00672be0 _$E196
// FUNCTION: STBC 0x00672bf0 _$E195
// FUNCTION: STBC 0x00672c00 _$E138
// FUNCTION: STBC 0x00672c10 _$E137
// FUNCTION: STBC 0x00672c20 _$E151
// FUNCTION: STBC 0x00672c30 _$E150
// FUNCTION: STBC 0x00672c40 _$E201
// FUNCTION: STBC 0x00672c50 _$E200
// FUNCTION: STBC 0x00672c60 _$E206
// FUNCTION: STBC 0x00672c70 _$E205
// FUNCTION: STBC 0x00672c80 _$E210
// FUNCTION: STBC 0x00672c90 _$E209
// FUNCTION: STBC 0x00672ca0 _$E214
// FUNCTION: STBC 0x00672cb0 _$E213
// FUNCTION: STBC 0x00672cc0 _$E221
// FUNCTION: STBC 0x00672cd0 _$E220
// FUNCTION: STBC 0x00672ce0 _$E246
// FUNCTION: STBC 0x00672cf0 _$E245
// FUNCTION: STBC 0x00672d00 _$E250
// FUNCTION: STBC 0x00672d10 _$E249
// FUNCTION: STBC 0x00672d20 _$E96
// FUNCTION: STBC 0x00672d30 _$E95
// FUNCTION: STBC 0x00672d40 _$E232
// FUNCTION: STBC 0x00672d50 _$E231
// FUNCTION: STBC 0x00672d60 _$E239
// FUNCTION: STBC 0x00672d70 _$E238
// FUNCTION: STBC 0x00672d80 _$E262
// FUNCTION: STBC 0x00672d90 _$E261
// FUNCTION: STBC 0x00672da0 _$E266
// FUNCTION: STBC 0x00672db0 _$E265
// FUNCTION: STBC 0x00672dc0 _$E306
// FUNCTION: STBC 0x00672dd0 _$E305
// FUNCTION: STBC 0x00672de0 _$E312
// FUNCTION: STBC 0x00672df0 _$E311
// FUNCTION: STBC 0x00672e00 _$E254
// FUNCTION: STBC 0x00672e10 _$E253
// FUNCTION: STBC 0x00672e20 _$E258
// FUNCTION: STBC 0x00672e30 _$E257
// FUNCTION: STBC 0x00672e40 _$E270
// FUNCTION: STBC 0x00672e50 _$E269
// FUNCTION: STBC 0x00672e60 _$E274
// FUNCTION: STBC 0x00672e70 _$E273
// FUNCTION: STBC 0x00672e80 _$E278
// FUNCTION: STBC 0x00672e90 _$E277
// FUNCTION: STBC 0x00672ea0 _$E282
// FUNCTION: STBC 0x00672eb0 _$E281
// FUNCTION: STBC 0x00672ec0 _$E286
// FUNCTION: STBC 0x00672ed0 _$E285
// FUNCTION: STBC 0x00672ee0 _$E290
// FUNCTION: STBC 0x00672ef0 _$E289
// FUNCTION: STBC 0x00672f00 _$E294
// FUNCTION: STBC 0x00672f10 _$E293
// FUNCTION: STBC 0x00672f20 _$E298
// FUNCTION: STBC 0x00672f30 _$E297
// FUNCTION: STBC 0x00673030 _$E101
// FUNCTION: STBC 0x00673040 _$E100
// FUNCTION: STBC 0x00673050 _$E302
// FUNCTION: STBC 0x00673060 _$E301
// FUNCTION: STBC 0x00673070 _$E106
// FUNCTION: STBC 0x00673080 _$E105
// FUNCTION: STBC 0x00673090 _$E316
// FUNCTION: STBC 0x006730a0 _$E315
// FUNCTION: STBC 0x006730b0 _$E320
// FUNCTION: STBC 0x006730c0 _$E319
// FUNCTION: STBC 0x006730d0 _$E324
// FUNCTION: STBC 0x006730e0 _$E323
// FUNCTION: STBC 0x006730f0 _$E328
// FUNCTION: STBC 0x00673100 _$E327
// FUNCTION: STBC 0x00673110 _$E115
// FUNCTION: STBC 0x00673120 _$E114
// FUNCTION: STBC 0x00673130 _$E332
// FUNCTION: STBC 0x00673140 _$E331
// FUNCTION: STBC 0x00673150 _$E336
// FUNCTION: STBC 0x00673160 _$E335
// FUNCTION: STBC 0x00673170 _$E340
// FUNCTION: STBC 0x00673180 _$E339
// FUNCTION: STBC 0x00673190 _$E121
// FUNCTION: STBC 0x006731a0 _$E120
// FUNCTION: STBC 0x006731b0 _$E126
// FUNCTION: STBC 0x006731c0 _$E125
// FUNCTION: STBC 0x006731d0 _$E156
// FUNCTION: STBC 0x006731e0 _$E155
// FUNCTION: STBC 0x006731f0 _$E344
// FUNCTION: STBC 0x00673200 _$E343
// FUNCTION: STBC 0x00673210 _$E161
// FUNCTION: STBC 0x00673220 _$E160
// FUNCTION: STBC 0x00673230 _$E348
// FUNCTION: STBC 0x00673240 _$E347
// FUNCTION: STBC 0x00673250 _$E352
// FUNCTION: STBC 0x00673260 _$E351
// FUNCTION: STBC 0x00673270 _$E166
// FUNCTION: STBC 0x00673280 _$E165
// FUNCTION: STBC 0x00673290 _$E356
// FUNCTION: STBC 0x006732a0 _$E355
// FUNCTION: STBC 0x006732b0 _$E131
// FUNCTION: STBC 0x006732c0 _$E130
// FUNCTION: STBC 0x006732d0 _$E360
// FUNCTION: STBC 0x006732e0 _$E359
// FUNCTION: STBC 0x006732f0 _$E364
// FUNCTION: STBC 0x00673300 _$E363
// FUNCTION: STBC 0x00673310 _$E171
// FUNCTION: STBC 0x00673320 _$E170
// FUNCTION: STBC 0x00673330 _$E176
// FUNCTION: STBC 0x00673340 _$E175
// FUNCTION: STBC 0x00673350 _$E368
// FUNCTION: STBC 0x00673360 _$E367
// FUNCTION: STBC 0x00673370 _$E372
// FUNCTION: STBC 0x00673380 _$E371
// FUNCTION: STBC 0x00673390 _$E376
// FUNCTION: STBC 0x006733a0 _$E375
// FUNCTION: STBC 0x006733b0 _$E380
// FUNCTION: STBC 0x006733c0 _$E379
// FUNCTION: STBC 0x006733d0 _$E384
// FUNCTION: STBC 0x006733e0 _$E383
// FUNCTION: STBC 0x006733f0 _$E388
// FUNCTION: STBC 0x00673400 _$E387
// FUNCTION: STBC 0x00673410 _$E392
// FUNCTION: STBC 0x00673420 _$E391
// FUNCTION: STBC 0x00673430 _$E396
// FUNCTION: STBC 0x00673440 _$E395
// FUNCTION: STBC 0x00673450 _$E181
// FUNCTION: STBC 0x00673460 _$E180
// FUNCTION: STBC 0x00673470 _$E400
// FUNCTION: STBC 0x00673480 _$E399
// FUNCTION: STBC 0x00673490 _$E186
// FUNCTION: STBC 0x006734a0 _$E185
// FUNCTION: STBC 0x006734b0 _$E191
// FUNCTION: STBC 0x006734c0 _$E190
// FUNCTION: STBC 0x006734d0 _$E440
// FUNCTION: STBC 0x006734e0 _$E439
// FUNCTION: STBC 0x006734f0 _$E445
// FUNCTION: STBC 0x00673500 _$E444
// FUNCTION: STBC 0x00673510 _$E449
// FUNCTION: STBC 0x00673520 _$E448

// The handler table's dynamic initialiser and its exit-time destructor.
// FUNCTION: STBC 0x006721c0 _$E455
// FUNCTION: STBC 0x006721d0 _$E451
// FUNCTION: STBC 0x006721e0 _$E454
// FUNCTION: STBC 0x006721f0 _$E452
// DATA: STBC 0x00991e48
TGEventHandlerTable CharacterSpeakingQueue::classHandlers;

// DATA: STBC 0x00991f34
int CharacterSpeakingQueue::numSpeaking;

/*
================
CharacterSpeakingQueue::CreateSoundDoneEvent

The event a line sends the queue when it has been spoken.
================
*/
// FUNCTION: STBC 0x00672210
TGEvent *CharacterSpeakingQueue::CreateSoundDoneEvent( void ) {
	TGEvent *event = new TGEvent;
	event->SetEventType( ET_CHARACTER_SOUND_DONE );
	event->SetDestination( this );
	return event;
}

/*
================
CharacterSpeakingQueue::GetNextSound

Takes the first queued line, from the first list that has one, and
returns its action while the action manager still knows it.
================
*/
// FUNCTION: STBC 0x00672270
TGAction *CharacterSpeakingQueue::GetNextSound( void ) {
	QueuedSound *sound = NULL;
	if ( missionCriticalLines.GetSize() != 0 ) {
		sound = missionCriticalLines.RemoveHead();
	} else if ( normalLines.GetSize() != 0 ) {
		sound = normalLines.RemoveHead();
	} else if ( spontaneousLines.GetSize() != 0 ) {
		sound = spontaneousLines.RemoveHead();
	}
	if ( sound == NULL ) {
		return NULL;
	}
	TGAction *action = sound->action;
	delete sound;
	return g_kTGActionManager.FindAction( action ) != -1 ? action : NULL;
}

/*
================
CharacterSpeakingQueue::IsReadyToSpeak

No line is being spoken and one is queued.
================
*/
// FUNCTION: STBC 0x00672330
bool CharacterSpeakingQueue::IsReadyToSpeak( void ) {
	if ( speakingAction != NULL ) {
		return false;
	}
	if ( missionCriticalLines.GetSize() != 0 || normalLines.GetSize() != 0 || spontaneousLines.GetSize() != 0 ) {
		return true;
	}
	return false;
}

/*
================
CharacterSpeakingQueue::SpeakNextLine

With no line being spoken, plays the next queued one; it tells the queue
when it is done. False when nothing was started.
================
*/
// FUNCTION: STBC 0x00672360
bool CharacterSpeakingQueue::SpeakNextLine( void ) {
	if ( speakingAction == NULL ) {
		TGAction *action = GetNextSound();
		if ( action != NULL ) {
			action->AddCompletedEvent( CreateSoundDoneEvent() );
			speakingAction = action;
			action->Play();
			numSpeaking++;
			return true;
		}
	}
	return false;
}

/*
================
CharacterSpeakingQueue::ClearQueue

Skips and drops every queued line and the one being spoken.
================
*/
// FUNCTION: STBC 0x006723E0
void CharacterSpeakingQueue::ClearQueue( void ) {
	while ( missionCriticalLines.GetSize() != 0 ) {
		QueuedSound *sound = missionCriticalLines.RemoveHead();
		// BUG: a line whose action is no longer registered is never freed
		if ( g_kTGActionManager.FindAction( sound->action ) != -1 ) {
			sound->action->Skip();
			delete sound;
		}
	}
	while ( normalLines.GetSize() != 0 ) {
		QueuedSound *sound = normalLines.RemoveHead();
		// BUG: as above
		if ( g_kTGActionManager.FindAction( sound->action ) != -1 ) {
			sound->action->Skip();
			delete sound;
		}
	}
	while ( spontaneousLines.GetSize() != 0 ) {
		QueuedSound *sound = spontaneousLines.RemoveHead();
		// BUG: as above
		if ( g_kTGActionManager.FindAction( sound->action ) != -1 ) {
			sound->action->Skip();
			delete sound;
		}
	}

	if ( g_kTGActionManager.FindAction( speakingAction ) != -1 ) {
		speakingAction->Skip();
	}
	speakingAction = NULL;
}

/*
================
CharacterSpeakingQueue::IsSpeaking

The line being spoken is still a registered action.
================
*/
// FUNCTION: STBC 0x00672510
int CharacterSpeakingQueue::IsSpeaking( void ) {
	return g_kTGActionManager.FindAction( speakingAction ) != -1;
}

/*
================
CharacterSpeakingQueue::AddSoundToQueue

Files the line in the list of its kind, after dropping every line of the
last kind.
================
*/
// FUNCTION: STBC 0x00672530
void CharacterSpeakingQueue::AddSoundToQueue( TGAction *action, int type, int subPriority ) {
	QueuedSound *sound = new QueuedSound;
	sound->action = action;
	sound->type = type;
	sound->subPriority = subPriority;

	while ( spontaneousLines.GetSize() != 0 ) {
		QueuedSound *dropped = spontaneousLines.RemoveHead();
		if ( dropped != NULL && dropped->action != NULL ) {
			dropped->action->Skip();
			delete dropped;
		}
	}

	if ( type == 0 ) {
		missionCriticalLines.AddTail( sound );
	} else if ( type == 1 ) {
		normalLines.AddTail( sound );
	} else {
		spontaneousLines.AddTail( sound );
	}
}

/*
================
CharacterSpeakingQueue::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x006726e0
void CharacterSpeakingQueue::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&CharacterSpeakingQueue::SoundDoneHandler, "CharacterSpeakingQueue::SoundDoneHandler" );
}

/*
================
CharacterSpeakingQueue::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00672700
void CharacterSpeakingQueue::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_CHARACTER_SOUND_DONE, "CharacterSpeakingQueue::SoundDoneHandler" );
}

/*
================
CharacterSpeakingQueue::SoundDoneHandler

The line has been spoken: the next one starts.
================
*/
// FUNCTION: STBC 0x00672720
void CharacterSpeakingQueue::SoundDoneHandler( TGEvent *event ) {
	speakingAction = NULL;
	numSpeaking--;
	SpeakNextLine();
}

/*
================
CharacterSpeakingQueue::WriteToStream

The line being spoken, then the three lists: each line's kind, its second
value and its action.
================
*/
// FUNCTION: STBC 0x00672740
bool CharacterSpeakingQueue::WriteToStream( TGStream *stream ) {
	if ( !TGEventHandlerObject::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteID( TGObject::GetSavedID( speakingAction ) );

	stream->WriteLong( missionCriticalLines.GetSize() );
	NiTListIterator pos;
	pos = missionCriticalLines.GetHeadPos();
	while ( pos ) {
		QueuedSound *sound = missionCriticalLines.GetNext( pos );
		stream->WriteLong( sound->type );
		stream->WriteLong( sound->subPriority );
		stream->WriteID( sound->action->GetObjID() );
	}

	stream->WriteLong( normalLines.GetSize() );
	pos = normalLines.GetHeadPos();
	while ( pos ) {
		QueuedSound *sound = normalLines.GetNext( pos );
		stream->WriteLong( sound->type );
		stream->WriteLong( sound->subPriority );
		stream->WriteID( sound->action->GetObjID() );
	}

	stream->WriteLong( spontaneousLines.GetSize() );
	pos = spontaneousLines.GetHeadPos();
	while ( pos ) {
		QueuedSound *sound = spontaneousLines.GetNext( pos );
		stream->WriteLong( sound->type );
		stream->WriteLong( sound->subPriority );
		stream->WriteID( sound->action->GetObjID() );
	}
	return true;
}

/*
================
CharacterSpeakingQueue::ReadFromStream

Reads what WriteToStream wrote; the actions stay IDs until FixUpPointers.
================
*/
// FUNCTION: STBC 0x00672850
bool CharacterSpeakingQueue::ReadFromStream( TGStream *stream ) {
	if ( !TGEventHandlerObject::ReadFromStream( stream ) ) {
		return false;
	}

	speakingAction = (TGAction *)stream->ReadID();

	int count;

	for ( count = stream->ReadLong(); count > 0; count-- ) {
		QueuedSound *sound = new QueuedSound;
		sound->type = stream->ReadLong();
		sound->subPriority = stream->ReadLong();
		sound->action = (TGAction *)stream->ReadID();
		AddSoundToQueue( sound->action, sound->type, sound->subPriority );
	}
	for ( count = stream->ReadLong(); count > 0; count-- ) {
		QueuedSound *sound = new QueuedSound;
		sound->type = stream->ReadLong();
		sound->subPriority = stream->ReadLong();
		sound->action = (TGAction *)stream->ReadID();
		AddSoundToQueue( sound->action, sound->type, sound->subPriority );
	}
	for ( count = stream->ReadLong(); count > 0; count-- ) {
		QueuedSound *sound = new QueuedSound;
		sound->type = stream->ReadLong();
		sound->subPriority = stream->ReadLong();
		sound->action = (TGAction *)stream->ReadID();
		AddSoundToQueue( sound->action, sound->type, sound->subPriority );
	}
	return true;
}

/*
================
CharacterSpeakingQueue::FixUpPointers

False while the line being spoken or a queued action is not loaded yet.
================
*/
// FUNCTION: STBC 0x00672970
bool CharacterSpeakingQueue::FixUpPointers( void ) {
	if ( !TGEventHandlerObject::FixUpPointers() ) {
		return false;
	}

	if ( speakingAction != NULL && GetTGObjectPtr( (long)speakingAction ) == NULL ) {
		return false;
	}

	NiTListIterator pos = missionCriticalLines.GetHeadPos();
	while ( pos ) {
		QueuedSound *sound = missionCriticalLines.GetNext( pos );
		if ( sound != NULL && GetTGObjectPtr( (long)sound->action ) == NULL ) {
			return false;
		}
	}
	pos = normalLines.GetHeadPos();
	while ( pos ) {
		QueuedSound *sound = normalLines.GetNext( pos );
		if ( sound != NULL && GetTGObjectPtr( (long)sound->action ) == NULL ) {
			return false;
		}
	}
	pos = spontaneousLines.GetHeadPos();
	while ( pos ) {
		QueuedSound *sound = spontaneousLines.GetNext( pos );
		if ( sound != NULL && GetTGObjectPtr( (long)sound->action ) == NULL ) {
			return false;
		}
	}
	return true;
}

/*
================
CharacterSpeakingQueue::PostFixUp

Turns the saved IDs back into pointers.
================
*/
// FUNCTION: STBC 0x00672a10
void CharacterSpeakingQueue::PostFixUp( void ) {
	TGEventHandlerObject::PostFixUp();

	if ( speakingAction != NULL ) {
		speakingAction = (TGAction *)GetTGObjectPtr( (long)speakingAction );
	}

	NiTListIterator pos = missionCriticalLines.GetHeadPos();
	while ( pos ) {
		QueuedSound *sound = missionCriticalLines.GetNext( pos );
		if ( sound != NULL ) {
			sound->action = (TGAction *)GetTGObjectPtr( (long)sound->action );
		}
	}
	pos = normalLines.GetHeadPos();
	while ( pos ) {
		QueuedSound *sound = normalLines.GetNext( pos );
		if ( sound != NULL ) {
			sound->action = (TGAction *)GetTGObjectPtr( (long)sound->action );
		}
	}
	pos = spontaneousLines.GetHeadPos();
	while ( pos ) {
		QueuedSound *sound = spontaneousLines.GetNext( pos );
		if ( sound != NULL ) {
			sound->action = (TGAction *)GetTGObjectPtr( (long)sound->action );
		}
	}
}
