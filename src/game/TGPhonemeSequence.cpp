/*
===============================================================================

	TGPhonemeSequence.cpp

	TGPhonemeSequence and TGPhonemeAction.

===============================================================================
*/

#include <iostream>
#include <windows.h>

// <windows.h>'s ANSI/wide macro would rename TGObject's method.
#undef GetClassName

#include "TGPhonemeSequence.h"
#include "TGPhonemeAction.h"
#include "TGPhoneme.h"
#include "TGAnimNode.h"
#include "TGPointerMap.h"
#include "TGSequence.h"
#include "TGObjPtrEvent.h"
#include "TGActionManager.h"
#include "TGStream.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00708000 _$E32
// FUNCTION: STBC 0x00708010 _$E29
// FUNCTION: STBC 0x00708020 _$E31
// FUNCTION: STBC 0x00708030 _$E30
// FUNCTION: STBC 0x00708040 _$E38
// FUNCTION: STBC 0x00708050 _$E35
// FUNCTION: STBC 0x00708060 _$E37
// FUNCTION: STBC 0x00708070 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00708080 _$E52
// FUNCTION: STBC 0x00708090 _$E51
// FUNCTION: STBC 0x007080a0 _$E144
// FUNCTION: STBC 0x007080b0 _$E143
// FUNCTION: STBC 0x007080c0 _$E157
// FUNCTION: STBC 0x007080d0 _$E156
// FUNCTION: STBC 0x007080e0 _$E66
// FUNCTION: STBC 0x007080f0 _$E65
// FUNCTION: STBC 0x00708100 _$E70
// FUNCTION: STBC 0x00708110 _$E69
// FUNCTION: STBC 0x00708120 _$E74
// FUNCTION: STBC 0x00708130 _$E73
// FUNCTION: STBC 0x00708140 _$E78
// FUNCTION: STBC 0x00708150 _$E77
// FUNCTION: STBC 0x00708160 _$E177
// FUNCTION: STBC 0x00708170 _$E176
// FUNCTION: STBC 0x00708180 _$E202
// FUNCTION: STBC 0x00708190 _$E201
// FUNCTION: STBC 0x007081c0 _$E211
// FUNCTION: STBC 0x007081d0 _$E210

// The playing sequences' list: its dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x007081e0 _$E224
// FUNCTION: STBC 0x007081f0 _$E220
// FUNCTION: STBC 0x00708220 _$E223
// FUNCTION: STBC 0x00708230 _$E221

// DATA: STBC 0x0099b698
NiTList<TGPhonemeSequence *> TGPhonemeSequence::sequences( 2 );

/*
================
TGPhonemeAction::TGPhonemeAction

Shows a copy of the phoneme; a duration of -1 takes the phoneme's.
================
*/
// FUNCTION: STBC 0x007082a0
TGPhonemeAction::TGPhonemeAction( TGOverlayManager *manager_, TGPhoneme *phoneme_, float duration_ ) {
	phoneme = phoneme_->Clone();
	manager = manager_;
	if ( duration_ != -1.0f ) {
		SetDuration( duration_ );
	} else {
		SetDuration( phoneme->duration );
	}
}

/*
================
TGPhonemeAction::~TGPhonemeAction
================
*/
// FUNCTION: STBC 0x00708350
TGPhonemeAction::~TGPhonemeAction( void ) {
	delete phoneme;
	manager = NULL;
}

/*
================
TGPhonemeAction::Play

A phoneme of one entry is shown at once, and the action ends with its
duration. A longer one becomes a sequence of one action per entry,
squeezed into the action's duration when the phoneme lasts longer; the
action ends when the sequence does. Every action of the sequence is
delayed by the first entry's time, not its own (the original's slip).
================
*/
// FUNCTION: STBC 0x00708400
void TGPhonemeAction::Play( void ) {
	unsigned int count = phoneme->entries.GetSize();
	if ( count == 0 ) {
		Completed();
		return;
	}
	if ( count == 1 ) {
		float phonemeDuration = phoneme->duration;
		manager->ShowOverlay( phoneme->GetController(), phoneme->GetOverlay(), phonemeDuration );
		if ( duration > 0.0f ) {
			TGTimedAction::Play();
		} else {
			Completed();
		}
		return;
	}

	float scale = 1.0f;
	if ( phoneme->duration < duration ) {
		scale = phoneme->duration / duration;
	}
	TGSequence *sequence = new TGSequence;
	for ( unsigned int i = 0; i < count; i++ ) {
		TGPhoneme *entryPhoneme = phoneme->GetEntryPhoneme( i );
		entryPhoneme->SetDuration( entryPhoneme->GetDuration() * scale );
		TGPhonemeAction *action = new TGPhonemeAction( manager, entryPhoneme, -1.0f );
		sequence->AddAction( action, NULL, phoneme->GetTime() * scale );	// the first entry's time for every action
		delete entryPhoneme;
	}

	TGObjPtrEvent *event = new TGObjPtrEvent;
	event->SetDestination( &g_kTGActionManager );
	event->SetObjPtr( this );
	event->SetEventType( ET_ACTION_COMPLETED );
	sequence->AddCompletedEvent( event );
	sequence->Play();
}

/*
================
TGPhonemeAction::PlaysOn

Whether the action shows its phoneme through the object.
================
*/
// FUNCTION: STBC 0x007085c0
bool TGPhonemeAction::PlaysOn( TGAnimNode *node ) {
	return ( (NiObject *)node == (NiObject *)manager ) ? true : false;
}

/*
================
TGPhonemeAction::WriteToStream
================
*/
// FUNCTION: STBC 0x007085d0
bool TGPhonemeAction::WriteToStream( TGStream *stream ) {
	if ( !TGTimedAction::WriteToStream( stream ) ) {
		return false;
	}
	stream->VirtualSlot17( (long)(TGOverlayManager *)manager );
	if ( !phoneme->WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteLong( PHONEME_ACTION_MARKER );
	return true;
}

/*
================
TGPhonemeAction::ReadFromStream
================
*/
// FUNCTION: STBC 0x00708620
bool TGPhonemeAction::ReadFromStream( TGStream *stream ) {
	if ( !TGTimedAction::ReadFromStream( stream ) ) {
		return false;
	}
	savedManager = stream->VirtualSlot16();
	phoneme = new TGPhoneme;
	if ( !phoneme->ReadFromStream( stream ) ) {
		return false;
	}
	stream->ReadLong();
	return true;
}

/*
================
TGPhonemeAction::FixUpPointers

Fails while the saved manager has not been loaded.
================
*/
// FUNCTION: STBC 0x007086d0
bool TGPhonemeAction::FixUpPointers( void ) {
	if ( !TGAction::FixUpPointers() ) {
		return false;
	}
	if ( savedManager != 0 && pointerMap.Find( savedManager ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
TGPhonemeAction::PostFixUp
================
*/
// FUNCTION: STBC 0x00708740
void TGPhonemeAction::PostFixUp( void ) {
	TGAction::PostFixUp();
	if ( savedManager != 0 ) {
		manager = (TGOverlayManager *)pointerMap.Find( savedManager );
	}
}

/*
================
TGPhonemeSequence::TGPhonemeSequence
================
*/
// FUNCTION: STBC 0x007087c0
TGPhonemeSequence::TGPhonemeSequence( TGOverlayManager *manager_ ) : entries( 128, 1 ) {
	elapsed = -1.0f;
	manager = manager_;
}

/*
================
TGPhonemeSequence::~TGPhonemeSequence
================
*/
// FUNCTION: STBC 0x00708880
TGPhonemeSequence::~TGPhonemeSequence( void ) {
	manager = NULL;
	sequences.Remove( this );
	int count = entries.GetSize();
	for ( int i = 0; i < count; i++ ) {
		TGPhonemeSequenceEntry *entry = entries.GetAt( i );
		if ( entry != NULL ) {
			delete entry;
		}
	}
}

/*
================
TGPhonemeSequence::AddEntry
================
*/
// FUNCTION: STBC 0x007089a0
void TGPhonemeSequence::AddEntry( long phonemeID, float time, float duration ) {
	TGPhonemeSequenceEntry *entry = new TGPhonemeSequenceEntry;
	entry->phonemeID = phonemeID;
	entry->time = time;
	entry->duration = duration;
	entries.Add( entry );
}

/*
================
TGPhonemeSequence::Play

Completes at once without phonemes, or when the first is unknown to the
manager.
================
*/
// FUNCTION: STBC 0x00708aa0
void TGPhonemeSequence::Play( void ) {
	if ( entries.GetEffectiveSize() == 0 ) {
		Completed();
		return;
	}
	TGAction::Play();
	elapsed = 0.0f;
	sequences.AddTail( this );
	TGPhonemeSequenceEntry *entry = entries.GetAt( 0 );
	if ( entry == NULL ) {
		Completed();
		return;
	}
	if ( manager->GetPhoneme( entry->phonemeID ) == NULL ) {
		Completed();
		return;
	}
	nextTime = entry->time;
}

/*
================
TGPhonemeSequence::Update

Once the next entry's time has come, the entry before it is shown unless
the time has already passed and dropped. The last entry holds the action
for the rest of its duration.
================
*/
// FUNCTION: STBC 0x00708b60
void TGPhonemeSequence::Update( float time ) {
	if ( entries.GetEffectiveSize() == 0 ) {
		Completed();
		return;
	}
	elapsed += time;
	while ( elapsed >= nextTime ) {
		if ( entries.GetEffectiveSize() == 1 ) {
			TGPhonemeSequenceEntry *entry = entries.GetAt( 0 );
			TGPhoneme *phoneme = manager->GetPhoneme( entry->phonemeID );
			if ( phoneme != NULL ) {
				manager->ShowOverlay( phoneme->GetController(), phoneme->GetOverlay(), entry->duration );
			}
			if ( duration != 0.0f ) {
				TGTimedAction::Play();
			} else {
				Completed();
			}
			entries.Compact();
			return;
		}
		TGPhonemeSequenceEntry *entry = entries.GetAt( 0 );
		TGPhonemeSequenceEntry *next = entries.GetAt( 1 );
		if ( next->time > elapsed ) {
			TGPhoneme *phoneme = manager->GetPhoneme( entry->phonemeID );
			if ( phoneme != NULL ) {
				manager->ShowPhoneme( phoneme );
			}
		}
		nextTime = next->time;
		entries.RemoveAt( 0 );
		entries.Compact();
		delete entry;
	}
}

/*
================
TGPhonemeSequence::UpdateAll
================
*/
// FUNCTION: STBC 0x00708d50
void TGPhonemeSequence::UpdateAll( float time ) {
	NiTListIterator pos = sequences.GetHeadPos();
	while ( pos != NULL ) {
		TGPhonemeSequence *sequence = sequences.GetNext( pos );
		if ( sequence != NULL ) {
			sequence->Update( time );
		}
	}
}

/*
================
TGPhonemeSequence::PlaysOn

Whether the sequence shows its phonemes through the object.
================
*/
// FUNCTION: STBC 0x00708d80
bool TGPhonemeSequence::PlaysOn( TGAnimNode *node ) {
	return ( (NiObject *)node == (NiObject *)manager ) ? true : false;
}

/*
================
TGPhonemeSequence::WriteToStream
================
*/
// FUNCTION: STBC 0x00708D90
bool TGPhonemeSequence::WriteToStream( TGStream *stream ) {
	if ( !TGTimedAction::WriteToStream( stream ) ) {
		return false;
	}
	stream->VirtualSlot17( (long)(TGOverlayManager *)manager );
	stream->WriteFloat( elapsed );
	entries.Compact();
	int count = entries.GetSize();
	stream->WriteInt( count );
	for ( int i = 0; i < count; i++ ) {
		TGPhonemeSequenceEntry *entry = entries.GetAt( i );
		stream->WriteID( entry->phonemeID );
		stream->WriteFloat( entry->time );
		stream->WriteFloat( entry->duration );
	}
	return true;
}

/*
================
TGPhonemeSequence::ReadFromStream

A sequence saved while playing (elapsed time set) is played on.
================
*/
// FUNCTION: STBC 0x00708e90
bool TGPhonemeSequence::ReadFromStream( TGStream *stream ) {
	if ( !TGTimedAction::ReadFromStream( stream ) ) {
		return false;
	}
	savedManager = stream->VirtualSlot16();
	elapsed = stream->ReadFloat();
	int count = stream->ReadInt();
	for ( int i = 0; i < count; i++ ) {
		TGPhonemeSequenceEntry *entry = new TGPhonemeSequenceEntry;
		entry->phonemeID = stream->ReadID();
		entry->time = stream->ReadFloat();
		entry->duration = stream->ReadFloat();
		entries.Add( entry );
	}
	if ( elapsed != -1.0f ) {
		sequences.AddTail( this );
	}
	return true;
}

/*
================
TGPhonemeSequence::FixUpPointers

Fails while the saved manager has not been loaded.
================
*/
// FUNCTION: STBC 0x00709050
bool TGPhonemeSequence::FixUpPointers( void ) {
	if ( !TGAction::FixUpPointers() ) {
		return false;
	}
	if ( savedManager != 0 && pointerMap.Find( savedManager ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
TGPhonemeSequence::PostFixUp
================
*/
// FUNCTION: STBC 0x007090c0
void TGPhonemeSequence::PostFixUp( void ) {
	TGAction::PostFixUp();
	if ( savedManager != 0 ) {
		manager = (TGOverlayManager *)pointerMap.Find( savedManager );
	}
}
