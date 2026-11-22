/*
===============================================================================

	PriorityListAI.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "AILog.h"
#include "PriorityListAI.h"
#include "PyEmbed.h"
#include "TGStream.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0048f280 _$E32
// FUNCTION: STBC 0x0048f290 _$E29
// FUNCTION: STBC 0x0048f2a0 _$E31
// FUNCTION: STBC 0x0048f2b0 _$E30
// FUNCTION: STBC 0x0048f2c0 _$E38
// FUNCTION: STBC 0x0048f2d0 _$E35
// FUNCTION: STBC 0x0048f2e0 _$E37
// FUNCTION: STBC 0x0048f2f0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0048f420 _$E229
// FUNCTION: STBC 0x0048f430 _$E228
// FUNCTION: STBC 0x0048f4c0 _$E274
// FUNCTION: STBC 0x0048f4d0 _$E273
// FUNCTION: STBC 0x0048f4e0 _$E278
// FUNCTION: STBC 0x0048f4f0 _$E277
// FUNCTION: STBC 0x0048f520 _$E260
// FUNCTION: STBC 0x0048f530 _$E259
// FUNCTION: STBC 0x0048f540 _$E267
// FUNCTION: STBC 0x0048f550 _$E266
// FUNCTION: STBC 0x0048f560 _$E290
// FUNCTION: STBC 0x0048f570 _$E289
// FUNCTION: STBC 0x0048f580 _$E294
// FUNCTION: STBC 0x0048f590 _$E293
// FUNCTION: STBC 0x0048f5a0 _$E334
// FUNCTION: STBC 0x0048f5b0 _$E333
// FUNCTION: STBC 0x0048f5c0 _$E340
// FUNCTION: STBC 0x0048f5d0 _$E339
// FUNCTION: STBC 0x0048f5e0 _$E282
// FUNCTION: STBC 0x0048f5f0 _$E281
// FUNCTION: STBC 0x0048f600 _$E286
// FUNCTION: STBC 0x0048f610 _$E285
// FUNCTION: STBC 0x0048f620 _$E298
// FUNCTION: STBC 0x0048f630 _$E297
// FUNCTION: STBC 0x0048f640 _$E302
// FUNCTION: STBC 0x0048f650 _$E301
// FUNCTION: STBC 0x0048f660 _$E306
// FUNCTION: STBC 0x0048f670 _$E305
// FUNCTION: STBC 0x0048f680 _$E310
// FUNCTION: STBC 0x0048f690 _$E309
// FUNCTION: STBC 0x0048f6a0 _$E314
// FUNCTION: STBC 0x0048f6b0 _$E313
// FUNCTION: STBC 0x0048f6c0 _$E318
// FUNCTION: STBC 0x0048f6d0 _$E317
// FUNCTION: STBC 0x0048f6e0 _$E322
// FUNCTION: STBC 0x0048f6f0 _$E321
// FUNCTION: STBC 0x0048f700 _$E326
// FUNCTION: STBC 0x0048f710 _$E325
// FUNCTION: STBC 0x0048f740 _$E330
// FUNCTION: STBC 0x0048f750 _$E329
// FUNCTION: STBC 0x0048f780 _$E344
// FUNCTION: STBC 0x0048f790 _$E343
// FUNCTION: STBC 0x0048f7a0 _$E348
// FUNCTION: STBC 0x0048f7b0 _$E347
// FUNCTION: STBC 0x0048f7c0 _$E352
// FUNCTION: STBC 0x0048f7d0 _$E351
// FUNCTION: STBC 0x0048f7e0 _$E356
// FUNCTION: STBC 0x0048f7f0 _$E355
// FUNCTION: STBC 0x0048f820 _$E360
// FUNCTION: STBC 0x0048f830 _$E359
// FUNCTION: STBC 0x0048f840 _$E364
// FUNCTION: STBC 0x0048f850 _$E363
// FUNCTION: STBC 0x0048f860 _$E368
// FUNCTION: STBC 0x0048f870 _$E367
// FUNCTION: STBC 0x0048f910 _$E372
// FUNCTION: STBC 0x0048f920 _$E371
// FUNCTION: STBC 0x0048f950 _$E376
// FUNCTION: STBC 0x0048f960 _$E375
// FUNCTION: STBC 0x0048f970 _$E380
// FUNCTION: STBC 0x0048f980 _$E379
// FUNCTION: STBC 0x0048f9b0 _$E384
// FUNCTION: STBC 0x0048f9c0 _$E383
// FUNCTION: STBC 0x0048f9f0 _$E388
// FUNCTION: STBC 0x0048fa00 _$E387
// FUNCTION: STBC 0x0048fa10 _$E392
// FUNCTION: STBC 0x0048fa20 _$E391
// FUNCTION: STBC 0x0048fa70 _$E396
// FUNCTION: STBC 0x0048fa80 _$E395
// FUNCTION: STBC 0x0048fa90 _$E400
// FUNCTION: STBC 0x0048faa0 _$E399
// FUNCTION: STBC 0x0048fab0 _$E404
// FUNCTION: STBC 0x0048fac0 _$E403
// FUNCTION: STBC 0x0048fad0 _$E408
// FUNCTION: STBC 0x0048fae0 _$E407
// FUNCTION: STBC 0x0048faf0 _$E412
// FUNCTION: STBC 0x0048fb00 _$E411
// FUNCTION: STBC 0x0048fb10 _$E416
// FUNCTION: STBC 0x0048fb20 _$E415
// FUNCTION: STBC 0x0048fb30 _$E420
// FUNCTION: STBC 0x0048fb40 _$E419
// FUNCTION: STBC 0x0048fb50 _$E424
// FUNCTION: STBC 0x0048fb60 _$E423
// FUNCTION: STBC 0x0048fb90 _$E428
// FUNCTION: STBC 0x0048fba0 _$E427

// FUNCTION: STBC 0x0048fc50 _$E444
// FUNCTION: STBC 0x0048fc60 _$E443
static AICreateFunctionRegistration priorityListAIRegistration( AI_TYPE_PRIORITY_LIST, PriorityListAI::Create );

/*
================
PriorityListAI::PriorityListAI
================
*/
// FUNCTION: STBC 0x0048fcb0 ??0PriorityListAI@@QAE@PAVPhysicsObjectClass@@PAD@Z
PriorityListAI::PriorityListAI( PhysicsObjectClass *object, char *name_ ) : ArtificialIntelligence( object, name_ ) {
	currentAI = NULL;
	count = 0;
	capacity = 0;
	entries = NULL;
}

/*
================
PriorityListAI::VirtualSlot15
================
*/
// FUNCTION: STBC 0x0048fce0
PyObject *PriorityListAI::VirtualSlot15( void ) {
	return PyEmbed::Unknown6F9920( this, "PriorityListAI" );
}

/*
================
PriorityListAI::GetAIType
================
*/
// FUNCTION: STBC 0x0048fcf0
int PriorityListAI::GetAIType( void ) {
	return AI_TYPE_PRIORITY_LIST;
}

/*
================
PriorityListAI::VirtualSlot20
================
*/
// FUNCTION: STBC 0x0048fd00
bool PriorityListAI::VirtualSlot20( int type ) {
	return type == AI_TYPE_PRIORITY_LIST ? true : false;
}

/*
================
PriorityListAI::~PriorityListAI

Deletes the AIs.
================
*/
// FUNCTION: STBC 0x0048fd30
// FUNCTION: STBC 0x0048fd10 ??_GPriorityListAI@@UAEPAXI@Z
PriorityListAI::~PriorityListAI( void ) {
	for ( int i = 0; i < count; i++ ) {
		delete entries[i].ai;
		entries[i].ai = NULL;
	}
	delete [] entries;
	count = 0;
	capacity = 0;
	currentAI = NULL;
}

/*
================
PriorityListAI::AddAI

The AI's parent becomes this list.
================
*/
// FUNCTION: STBC 0x0048fdd0
void PriorityListAI::AddAI( ArtificialIntelligence *ai, int priority ) {
	int index;
	for ( index = 0; index < count; index++ ) {
		if ( entries[index].priority > priority ) {
			break;
		}
	}

	if ( count >= capacity ) {
		PriorityListEntry *newEntries = new PriorityListEntry[capacity + 1];
		memcpy( newEntries, entries, count * sizeof( PriorityListEntry ) );
		memset( &newEntries[capacity], 0, sizeof( PriorityListEntry ) );
		delete [] entries;
		entries = newEntries;
		capacity++;
	}

	memmove( &entries[index + 1], &entries[index], ( count - index ) * sizeof( PriorityListEntry ) );
	entries[index].ai = ai->VirtualSlot13();
	entries[index].ai->SetParentID( id );
	entries[index].priority = priority;
	count++;
}

/*
================
PriorityListAI::RemoveAIByPriority
================
*/
// FUNCTION: STBC 0x0048fec0
void PriorityListAI::RemoveAIByPriority( int priority ) {
	for ( int i = 0; i < count; i++ ) {
		if ( entries[i].priority == priority ) {
			ArtificialIntelligence *ai = entries[i].ai;
			if ( currentAI == ai ) {
				currentAI = NULL;
			}
			if ( ai->HasFocus() ) {
				ai->LostFocus();
			}
			if ( ai->IsActive() ) {
				ai->SetInactive();
			}
			delete ai;

			memmove( &entries[i], &entries[i + 1], ( count - ( i + 1 ) ) * sizeof( PriorityListEntry ) );
			count--;
			memset( &entries[count], 0, sizeof( PriorityListEntry ) );
			return;
		}
	}
}

/*
================
PriorityListAI::RemoveAI
================
*/
// FUNCTION: STBC 0x0048ff80
void PriorityListAI::RemoveAI( ArtificialIntelligence *ai ) {
	for ( int i = 0; i < count; i++ ) {
		if ( entries[i].ai == ai ) {
			if ( currentAI == ai ) {
				currentAI = NULL;
			}
			if ( ai->HasFocus() ) {
				ai->LostFocus();
			}
			if ( ai->IsActive() ) {
				ai->SetInactive();
			}
			delete ai;

			memmove( &entries[i], &entries[i + 1], ( count - ( i + 1 ) ) * sizeof( PriorityListEntry ) );
			count--;
			memset( &entries[count], 0, sizeof( PriorityListEntry ) );
			return;
		}
	}
}

/*
================
PriorityListAI::GetAllAIsInTree
================
*/
// FUNCTION: STBC 0x00490030
void PriorityListAI::GetAllAIsInTree( NiTList<ArtificialIntelligence *> &ais ) {
	ais.AddTail( this );
	for ( int i = 0; i < count; i++ ) {
		entries[i].ai->GetAllAIsInTree( ais );
	}
}

/*
================
PriorityListAI::GetFocusAIs
================
*/
// FUNCTION: STBC 0x004900b0
void PriorityListAI::GetFocusAIs( NiTList<ArtificialIntelligence *> &ais ) {
	if ( focus ) {
		ais.AddTail( this );
		for ( int i = 0; i < count; i++ ) {
			if ( entries[i].ai->HasFocus() ) {
				entries[i].ai->GetFocusAIs( ais );
			}
		}
	}
}

/*
================
PriorityListAI::SetActive

Activates the AIs up to the current one that are not done.
================
*/
// FUNCTION: STBC 0x00490140
void PriorityListAI::SetActive( void ) {
	AILog log( "PriorityListAI(%s)::SetActive", name );
	active = true;
	for ( int i = 0; i < count; i++ ) {
		PriorityListEntry *entry = &entries[i];
		ArtificialIntelligence *ai = entry->ai;
		if ( !entry->done && !ai->IsActive() ) {
			ai->SetActive();
		}
		if ( ai == currentAI ) {
			break;
		}
	}
}

/*
================
PriorityListAI::SetInactive
================
*/
// FUNCTION: STBC 0x004901e0
void PriorityListAI::SetInactive( void ) {
	AILog log( "PriorityListAI(%s)::SetInactive", name );
	active = false;
	for ( int i = 0; i < count; i++ ) {
		PriorityListEntry *entry = &entries[i];
		if ( entry->ai->IsActive() ) {
			entry->ai->SetInactive();
		}
	}
}

/*
================
PriorityListAI::GotFocus

The focus goes to the current AI only when it updates.
================
*/
// FUNCTION: STBC 0x00490270
void PriorityListAI::GotFocus( void ) {
	AILog log( "PriorityListAI(%s)::GotFocus", name );
	focus = true;
}

/*
================
PriorityListAI::LostFocus
================
*/
// FUNCTION: STBC 0x004902a0
void PriorityListAI::LostFocus( void ) {
	AILog log( "PriorityListAI(%s)::LostFocus", name );
	if ( currentAI != NULL && currentAI->HasFocus() ) {
		currentAI->LostFocus();
	}
	focus = false;
}

/*
================
PriorityListAI::IsInterruptable

Interruptable only when the current AI is too, while the list is active.
================
*/
// FUNCTION: STBC 0x00490310
bool PriorityListAI::IsInterruptable( void ) {
	bool result = true;
	result &= interruptable;
	if ( result && active && currentAI != NULL ) {
		result &= currentAI->IsInterruptable();
	}
	return result;
}

/*
================
PriorityListAI::Update

Runs the first AI that is not done and can take over: one that is not
dormant, while the current AI can be interrupted. An active AI ends the
update and deactivates the AIs after it; otherwise the next one is tried.
================
*/
// FUNCTION: STBC 0x00490340
int PriorityListAI::Update( double time ) {
	AILog log( "PriorityListAI(%s)::Update, contains %d", name, count );
	int dormantCount = 0;
	for ( int i = 0; i < count; i++ ) {
		if ( entries[i].done ) {
			continue;
		}
		ArtificialIntelligence *ai = entries[i].ai;
		if ( !ai->IsActive() ) {
			ai->SetActive();
		}
		if ( ai != currentAI ) {
			if ( currentAI != NULL && !currentAI->IsInterruptable() ) {
				continue;
			}
			if ( !ai->IsDormant() ) {
				if ( currentAI != NULL && currentAI->HasFocus() ) {
					currentAI->LostFocus();
				}
				currentAI = ai;
				if ( !ai->IsActive() ) {
					ai->SetActive();
				}
			} else {
				dormantCount++;
				continue;
			}
		}
		if ( !ai->HasFocus() ) {
			ai->GotFocus();
		}

		switch ( ai->Update( time ) ) {
		case US_ACTIVE: {
			for ( int j = i + 1; j < count; j++ ) {
				if ( entries[j].ai->IsActive() ) {
					entries[j].ai->SetInactive();
				}
			}
			return US_ACTIVE;
		}
		case US_DONE:
			entries[i].done = true;
			break;
		case US_DORMANT:
			dormantCount++;
			break;
		}

		if ( ai->HasFocus() ) {
			ai->LostFocus();
		}
		if ( ai == currentAI ) {
			currentAI = NULL;
		}
	}

	if ( dormantCount != 0 ) {
		return US_DORMANT;
	}
	return US_DONE;
}

/*
================
PriorityListAI::Reset

Resets every AI and activates the ones up to the current one again.
================
*/
// FUNCTION: STBC 0x004904f0
void PriorityListAI::Reset( void ) {
	bool beforeCurrent = true;
	for ( int i = 0; i < count; i++ ) {
		ArtificialIntelligence *ai = entries[i].ai;
		ai->Reset();
		entries[i].done = false;
		if ( active && beforeCurrent && !ai->IsActive() ) {
			ai->SetActive();
		}
		if ( currentAI == ai ) {
			beforeCurrent = false;
		}
	}
}

/*
================
PriorityListAI::IsDormant

Dormant when every AI that is not done is, unless the current AI cannot
be interrupted; an empty list is not.
================
*/
// FUNCTION: STBC 0x00490560
bool PriorityListAI::IsDormant( void ) {
	AILog log( "PriorityListAI(%s)::IsDormant (no easy check)", name );
	if ( !active || currentAI == NULL || currentAI->IsInterruptable() ) {
		int dormantCount = 0;
		for ( int i = 0; i < count; i++ ) {
			PriorityListEntry *entry = &entries[i];
			if ( !entry->done ) {
				if ( !entry->ai->IsDormant() ) {
					return false;
				}
				dormantCount++;
			}
		}
		if ( dormantCount != 0 ) {
			return true;
		}
	}
	return false;
}

/*
================
PriorityListAI::WriteToStream

The current AI goes as its index (-1 for none), then each entry with its
AI after a flag.
================
*/
// FUNCTION: STBC 0x00490640
bool PriorityListAI::WriteToStream( TGStream *stream ) {
	bool result = true;
	result &= ArtificialIntelligence::WriteToStream( stream );
	if ( result ) {
		int current;
		for ( current = count - 1; current >= 0; current-- ) {
			if ( entries[current].ai == currentAI ) {
				break;
			}
		}
		stream->WriteInt( current );
		stream->WriteInt( count );
		for ( int i = 0; i < count; i++ ) {
			stream->WriteInt( entries[i].priority );
			stream->WriteChar( entries[i].done );
			if ( entries[i].ai != NULL ) {
				stream->WriteChar( true );
				result &= entries[i].ai->WriteToStream( stream );
			} else {
				stream->WriteChar( false );
			}
		}
	}
	return result;
}

/*
================
PriorityListAI::PriorityListAI
================
*/
// FUNCTION: STBC 0x00490710 ??0PriorityListAI@@QAE@PAVTGStream@@PA_N@Z
PriorityListAI::PriorityListAI( TGStream *stream, bool *ok ) : ArtificialIntelligence( stream, ok ) {
	currentAI = NULL;
	count = 0;
	capacity = 0;
	entries = NULL;
	if ( *ok ) {
		int current = stream->ReadInt();
		capacity = count = stream->ReadInt();
		entries = new PriorityListEntry[count];
		for ( int i = 0; i < count; i++ ) {
			entries[i].priority = stream->ReadInt();
			char done = stream->ReadChar();
			if ( done == true || done == false ) {
				entries[i].done = done != 0;
			} else {
				*ok = false;
			}
			if ( stream->ReadChar() ) {
				entries[i].ai = CreateFromStream( stream );
				if ( entries[i].ai == NULL ) {
					*ok = false;
				}
			}
		}
		// The bound admits an index one past the last entry.
		if ( *ok && current != -1 ) {
			if ( current >= 0 && current <= count ) {
				currentAI = entries[current].ai;
			} else {
				*ok = false;
			}
		}
	}
}

/*
================
PriorityListAI::Create
================
*/
// FUNCTION: STBC 0x00490850
ArtificialIntelligence *PriorityListAI::Create( TGStream *stream ) {
	bool ok = true;
	PriorityListAI *ai = new PriorityListAI( stream, &ok );
	if ( !ok ) {
		delete ai;
		ai = NULL;
	}
	return ai;
}

/*
================
PriorityListAI::FixUpPointers
================
*/
// FUNCTION: STBC 0x004908d0
bool PriorityListAI::FixUpPointers( void ) {
	for ( int i = 0; i < count; i++ ) {
		if ( entries[i].ai != NULL && !entries[i].ai->FixUpPointers() ) {
			return false;
		}
	}
	return true;
}

/*
================
PriorityListAI::PostFixUp
================
*/
// FUNCTION: STBC 0x00490910
void PriorityListAI::PostFixUp( void ) {
	for ( int i = 0; i < count; i++ ) {
		if ( entries[i].ai != NULL ) {
			entries[i].ai->PostFixUp();
		}
	}
}
