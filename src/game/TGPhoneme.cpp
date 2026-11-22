/*
===============================================================================

	TGPhoneme.cpp

===============================================================================
*/

#include "TGPhoneme.h"
#include "TGObject.h"
#include "TGStream.h"

#include <stdio.h>
#include <string.h>
#include <iostream>

// The unit's static initialisers and exit-time registrations (<iostream> and header statics).

// FUNCTION: STBC 0x00707fd0 _$E215
// FUNCTION: STBC 0x00707ff0 _$E214
#include "NiAVObject.h"
#include "NiDynamicEffect.h"
#include "NiExtraData.h"
#include "renderer/NiImage.h"
#include "NiNode.h"
#include "NiObject.h"
#include "NiObjectNET.h"
#include "NiProperty.h"
#include "NiRawImageData.h"
#include "NiTextureProperty.h"
#include "NiTimeController.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00707730 _$E39
// FUNCTION: STBC 0x00707740 _$E36
// FUNCTION: STBC 0x00707750 _$E38
// FUNCTION: STBC 0x00707760 _$E37
// FUNCTION: STBC 0x00707770 _$E45
// FUNCTION: STBC 0x00707780 _$E42
// FUNCTION: STBC 0x00707790 _$E44
// FUNCTION: STBC 0x007077a0 _$E43

// The standard streams' initialisers and exit-time destructors (<iostream>).

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007077b0 _$E52
// FUNCTION: STBC 0x007077c0 _$E51
// FUNCTION: STBC 0x007077d0 _$E141
// FUNCTION: STBC 0x007077e0 _$E140
// FUNCTION: STBC 0x007077f0 _$E154
// FUNCTION: STBC 0x00707800 _$E153
// FUNCTION: STBC 0x00707810 _$E63
// FUNCTION: STBC 0x00707820 _$E62
// FUNCTION: STBC 0x00707830 _$E67
// FUNCTION: STBC 0x00707840 _$E66
// FUNCTION: STBC 0x00707850 _$E71
// FUNCTION: STBC 0x00707860 _$E70
// FUNCTION: STBC 0x00707870 _$E75
// FUNCTION: STBC 0x00707880 _$E74
// FUNCTION: STBC 0x00707890 _$E174
// FUNCTION: STBC 0x007078a0 _$E173
// FUNCTION: STBC 0x007078b0 _$E199
// FUNCTION: STBC 0x007078c0 _$E198
// FUNCTION: STBC 0x007078d0 _$E204
// FUNCTION: STBC 0x007078e0 _$E203
// FUNCTION: STBC 0x007078f0 _$E208
// FUNCTION: STBC 0x00707900 _$E207

/*
================
TGPhoneme::TGPhoneme
================
*/
// FUNCTION: STBC 0x00707910
TGPhoneme::TGPhoneme( void ) : entries( 2 ) {
	duration = 0.0f;
}

/*
================
TGPhoneme::~TGPhoneme

Deletes the entries and their names.
================
*/
// FUNCTION: STBC 0x00707930
TGPhoneme::~TGPhoneme( void ) {
	NiTListIterator pos = entries.GetHeadPos();
	while ( pos != NULL ) {
		TGPhonemeEntry *entry = entries.GetNext( pos );
		delete entry->controller;
		delete entry->overlay;
		delete entry;
	}
}

/*
================
TGPhoneme::AddEntry
================
*/
// FUNCTION: STBC 0x007079b0
void TGPhoneme::AddEntry( char *controller, char *overlay, float time, float unknown ) {
	if ( controller == NULL || overlay == NULL ) {
		return;
	}
	TGPhonemeEntry *entry = new TGPhonemeEntry;
	entry->controller = new char[strlen( controller ) + 1];
	strcpy( entry->controller, controller );
	entry->overlay = new char[strlen( overlay ) + 1];
	strcpy( entry->overlay, overlay );
	entry->time = time;
	entry->unknown0C = unknown;
	entries.AddTail( entry );
	if ( time > duration ) {
		duration = time;
	}
}

/*
================
TGPhoneme::AddPhonemes

The phonemes the group hands out one entry at a time are added entry by
entry; the copies are not deleted.
================
*/
// FUNCTION: STBC 0x00707ac0
void TGPhoneme::AddPhonemes( TGPhoneme *group ) {
	if ( group == NULL ) {
		return;
	}
	for ( unsigned int i = 0; i < group->entries.GetSize(); i++ ) {
		TGPhoneme *phoneme = group->GetEntryPhoneme( i );
		if ( phoneme != NULL ) {
			if ( phoneme->entries.GetSize() == 1 ) {
				float unknown = phoneme->duration;
				AddEntry( phoneme->GetController(), phoneme->GetOverlay(), phoneme->GetTime(), unknown );
			} else {
				AddPhonemes( phoneme );
			}
		}
	}
}

/*
================
TGPhoneme::Clone
================
*/
// FUNCTION: STBC 0x00707b30
TGPhoneme *TGPhoneme::Clone( void ) {
	TGPhoneme *phoneme = new TGPhoneme;
	NiTListIterator pos = entries.GetHeadPos();
	while ( pos != NULL ) {
		TGPhonemeEntry *entry = entries.GetNext( pos );
		phoneme->AddEntry( entry->controller, entry->overlay, entry->time, entry->unknown0C );
	}
	return phoneme;
}

/*
================
TGPhoneme::WriteToStream

Each entry after a true; a false ends the list.
================
*/
// FUNCTION: STBC 0x00707b80
bool TGPhoneme::WriteToStream( TGStream *stream ) {
	char buffer[256];

	NiTListIterator pos = entries.GetHeadPos();
	while ( pos != NULL ) {
		TGPhonemeEntry *entry = entries.GetNext( pos );
		if ( entry != NULL ) {
			stream->WriteBool( true );
			sprintf( buffer, "Saving Phoneme %s on %s\n", entry->overlay, entry->controller );
			TGObject::Log( buffer );
			stream->VirtualSlot11( entry->controller );
			stream->VirtualSlot11( entry->overlay );
			stream->WriteFloat( entry->time );
			stream->WriteFloat( entry->unknown0C );
		}
	}
	stream->WriteBool( false );
	return true;
}

/*
================
TGPhoneme::ReadFromStream

The names the stream hands out are copied by AddEntry and never freed.
================
*/
// FUNCTION: STBC 0x00707c20
bool TGPhoneme::ReadFromStream( TGStream *stream ) {
	char buffer[256];

	while ( stream->ReadBool() ) {
		char *controller = stream->VirtualSlot10( -1 );
		char *overlay = stream->VirtualSlot10( -1 );
		float time = stream->ReadFloat();
		float unknown = stream->ReadFloat();
		sprintf( buffer, "Loading Phoneme %s on %s\n", controller, overlay );
		TGObject::Log( buffer );
		AddEntry( controller, overlay, time, unknown );
	}
	return true;
}

/*
================
TGPhoneme::GetController
================
*/
// FUNCTION: STBC 0x00707cd0
char *TGPhoneme::GetController( void ) {
	TGPhonemeEntry *entry = entries.GetHead();
	if ( entry == NULL ) {
		return NULL;
	}
	return entry->controller;
}

/*
================
TGPhoneme::GetOverlay
================
*/
// FUNCTION: STBC 0x00707ce0
char *TGPhoneme::GetOverlay( void ) {
	TGPhonemeEntry *entry = entries.GetHead();
	if ( entry == NULL ) {
		return NULL;
	}
	return entry->overlay;
}

/*
================
TGPhoneme::GetTime
================
*/
// FUNCTION: STBC 0x00707cf0
float TGPhoneme::GetTime( void ) {
	TGPhonemeEntry *entry = entries.GetHead();
	if ( entry == NULL ) {
		return 0.0f;
	}
	return entry->time;
}

/*
================
TGPhoneme::GetEntryPhoneme
================
*/
// FUNCTION: STBC 0x00707d10
TGPhoneme *TGPhoneme::GetEntryPhoneme( int index ) {
	if ( (int)entries.GetSize() <= 0 || index >= (int)entries.GetSize() ) {
		return NULL;
	}
	TGPhonemeEntry *entry = NULL;
	NiTListIterator pos = entries.GetHeadPos();
	for ( int i = 0; i <= index; i++ ) {
		if ( pos == NULL ) {
			return NULL;
		}
		entry = entries.GetNext( pos );
	}
	TGPhoneme *phoneme = new TGPhoneme;
	phoneme->AddEntry( entry->controller, entry->overlay, entry->time, -1.0f );
	phoneme->SetDuration( entry->unknown0C );
	return phoneme;
}

/*
================
TGPhoneme::RemoveEntryPhoneme

The entry is unlinked from the list but neither it nor its names are
freed.
================
*/
// FUNCTION: STBC 0x00707d80
TGPhoneme *TGPhoneme::RemoveEntryPhoneme( int index ) {
	if ( (int)entries.GetSize() <= 0 || index >= (int)entries.GetSize() ) {
		return NULL;
	}
	TGPhonemeEntry *entry = NULL;
	NiTListIterator pos = entries.GetHeadPos();
	for ( int i = 0; i <= index; i++ ) {
		if ( pos == NULL ) {
			return NULL;
		}
		entry = entries.GetNext( pos );
	}
	TGPhoneme *phoneme = new TGPhoneme;
	phoneme->AddEntry( entry->controller, entry->overlay, entry->time, -1.0f );
	phoneme->SetDuration( entry->unknown0C );
	entries.Remove( entry );
	return phoneme;
}
