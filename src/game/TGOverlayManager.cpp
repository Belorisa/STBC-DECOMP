/*
===============================================================================

	TGOverlayManager.cpp

===============================================================================
*/

#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "TGOverlayManager.h"
#include "TGPhonemeAction.h"
#include "TGOverlayAction.h"
#include "TGAnimAction.h"
#include "TGAnimNode.h"
#include "TGPhoneme.h"
#include "TGPhonemeSequence.h"
#include "TGSequence.h"
#include "TGStream.h"
#include "TGSystemWrapperClass.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00704fb0 _$E32
// FUNCTION: STBC 0x00704fc0 _$E29
// FUNCTION: STBC 0x00704fd0 _$E31
// FUNCTION: STBC 0x00704fe0 _$E30
// FUNCTION: STBC 0x00704ff0 _$E38
// FUNCTION: STBC 0x00705000 _$E35
// FUNCTION: STBC 0x00705010 _$E37
// FUNCTION: STBC 0x00705020 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00705030 _$E45
// FUNCTION: STBC 0x00705040 _$E44
// FUNCTION: STBC 0x00705050 _$E65
// FUNCTION: STBC 0x00705060 _$E64
// FUNCTION: STBC 0x00705070 _$E69
// FUNCTION: STBC 0x00705080 _$E68
// FUNCTION: STBC 0x00705090 _$E73
// FUNCTION: STBC 0x007050a0 _$E72
// FUNCTION: STBC 0x007050b0 _$E77
// FUNCTION: STBC 0x007050c0 _$E76
// FUNCTION: STBC 0x007050d0 _$E201
// FUNCTION: STBC 0x007050e0 _$E200
// FUNCTION: STBC 0x007050f0 _$E206
// FUNCTION: STBC 0x00705100 _$E205
// FUNCTION: STBC 0x00705110 _$E210
// FUNCTION: STBC 0x00705120 _$E209
// FUNCTION: STBC 0x00705130 _$E143
// FUNCTION: STBC 0x00705140 _$E142
// FUNCTION: STBC 0x00705150 _$E156
// FUNCTION: STBC 0x00705160 _$E155
// FUNCTION: STBC 0x00705170 _$E176
// FUNCTION: STBC 0x00705180 _$E175
// FUNCTION: STBC 0x00705190 _$E216
// FUNCTION: STBC 0x007051a0 _$E215
// FUNCTION: STBC 0x007051b0 _$E221
// FUNCTION: STBC 0x007051c0 _$E220
// FUNCTION: STBC 0x007051d0 _$E225
// FUNCTION: STBC 0x007051e0 _$E224
// FUNCTION: STBC 0x007051f0 _$E232
// FUNCTION: STBC 0x00705200 _$E231
// FUNCTION: STBC 0x00705210 _$E257
// FUNCTION: STBC 0x00705220 _$E256
// FUNCTION: STBC 0x00705230 _$E261
// FUNCTION: STBC 0x00705240 _$E260
// FUNCTION: STBC 0x00705250 _$E101
// FUNCTION: STBC 0x00705260 _$E100
// FUNCTION: STBC 0x00705270 _$E243
// FUNCTION: STBC 0x00705280 _$E242
// FUNCTION: STBC 0x00705290 _$E250
// FUNCTION: STBC 0x007052a0 _$E249
// FUNCTION: STBC 0x007052b0 _$E273
// FUNCTION: STBC 0x007052c0 _$E272
// FUNCTION: STBC 0x007052d0 _$E277
// FUNCTION: STBC 0x007052e0 _$E276
// FUNCTION: STBC 0x007052f0 _$E317
// FUNCTION: STBC 0x00705300 _$E316
// FUNCTION: STBC 0x00705310 _$E323
// FUNCTION: STBC 0x00705320 _$E322
// FUNCTION: STBC 0x00705330 _$E265
// FUNCTION: STBC 0x00705340 _$E264
// FUNCTION: STBC 0x00705350 _$E269
// FUNCTION: STBC 0x00705360 _$E268
// FUNCTION: STBC 0x00705370 _$E281
// FUNCTION: STBC 0x00705380 _$E280
// FUNCTION: STBC 0x00705390 _$E285
// FUNCTION: STBC 0x007053a0 _$E284
// FUNCTION: STBC 0x007053b0 _$E289
// FUNCTION: STBC 0x007053c0 _$E288
// FUNCTION: STBC 0x007053d0 _$E293
// FUNCTION: STBC 0x007053e0 _$E292
// FUNCTION: STBC 0x007053f0 _$E297
// FUNCTION: STBC 0x00705400 _$E296
// FUNCTION: STBC 0x00705410 _$E301
// FUNCTION: STBC 0x00705420 _$E300
// FUNCTION: STBC 0x00705430 _$E305
// FUNCTION: STBC 0x00705440 _$E304
// FUNCTION: STBC 0x00705450 _$E309
// FUNCTION: STBC 0x00705460 _$E308
// FUNCTION: STBC 0x00705470 _$E106
// FUNCTION: STBC 0x00705480 _$E105
// FUNCTION: STBC 0x00705490 _$E313
// FUNCTION: STBC 0x007054a0 _$E312
// FUNCTION: STBC 0x007054b0 _$E111
// FUNCTION: STBC 0x007054c0 _$E110
// FUNCTION: STBC 0x007054d0 _$E327
// FUNCTION: STBC 0x007054e0 _$E326
// FUNCTION: STBC 0x007054f0 _$E331
// FUNCTION: STBC 0x00705500 _$E330
// FUNCTION: STBC 0x00705510 _$E335
// FUNCTION: STBC 0x00705520 _$E334
// FUNCTION: STBC 0x00705530 _$E339
// FUNCTION: STBC 0x00705540 _$E338
// FUNCTION: STBC 0x00705550 _$E120
// FUNCTION: STBC 0x00705560 _$E119
// FUNCTION: STBC 0x00705570 _$E343
// FUNCTION: STBC 0x00705580 _$E342
// FUNCTION: STBC 0x00705590 _$E347
// FUNCTION: STBC 0x007055a0 _$E346
// FUNCTION: STBC 0x007055b0 _$E351
// FUNCTION: STBC 0x007055c0 _$E350
// FUNCTION: STBC 0x007055d0 _$E126
// FUNCTION: STBC 0x007055e0 _$E125
// FUNCTION: STBC 0x007055f0 _$E131
// FUNCTION: STBC 0x00705600 _$E130
// FUNCTION: STBC 0x00705610 _$E161
// FUNCTION: STBC 0x00705620 _$E160
// FUNCTION: STBC 0x00705630 _$E355
// FUNCTION: STBC 0x00705640 _$E354
// FUNCTION: STBC 0x00705650 _$E166
// FUNCTION: STBC 0x00705660 _$E165
// FUNCTION: STBC 0x00705670 _$E359
// FUNCTION: STBC 0x00705680 _$E358
// FUNCTION: STBC 0x00705690 _$E363
// FUNCTION: STBC 0x007056a0 _$E362
// FUNCTION: STBC 0x007056b0 _$E171
// FUNCTION: STBC 0x007056c0 _$E170
// FUNCTION: STBC 0x007056d0 _$E367
// FUNCTION: STBC 0x007056e0 _$E366
// FUNCTION: STBC 0x007056f0 _$E136
// FUNCTION: STBC 0x00705700 _$E135
// FUNCTION: STBC 0x00705710 _$E371
// FUNCTION: STBC 0x00705720 _$E370
// FUNCTION: STBC 0x00705730 _$E375
// FUNCTION: STBC 0x00705740 _$E374
// FUNCTION: STBC 0x00705750 _$E181
// FUNCTION: STBC 0x00705760 _$E180
// FUNCTION: STBC 0x00705770 _$E379
// FUNCTION: STBC 0x00705780 _$E378
// FUNCTION: STBC 0x00705790 _$E383
// FUNCTION: STBC 0x007057a0 _$E382
// FUNCTION: STBC 0x007057b0 _$E387
// FUNCTION: STBC 0x007057c0 _$E386
// FUNCTION: STBC 0x007057d0 _$E391
// FUNCTION: STBC 0x007057e0 _$E390
// FUNCTION: STBC 0x007057f0 _$E395
// FUNCTION: STBC 0x00705800 _$E394
// FUNCTION: STBC 0x00705810 _$E399
// FUNCTION: STBC 0x00705820 _$E398
// FUNCTION: STBC 0x00705830 _$E403
// FUNCTION: STBC 0x00705840 _$E402
// FUNCTION: STBC 0x00705850 _$E407
// FUNCTION: STBC 0x00705860 _$E406
// FUNCTION: STBC 0x00705870 _$E186
// FUNCTION: STBC 0x00705880 _$E185
// FUNCTION: STBC 0x00705890 _$E411
// FUNCTION: STBC 0x007058a0 _$E410
// FUNCTION: STBC 0x007058b0 _$E191
// FUNCTION: STBC 0x007058c0 _$E190
// FUNCTION: STBC 0x007058d0 _$E196
// FUNCTION: STBC 0x007058e0 _$E195

// DATA: STBC 0x0099b664
bool TGOverlayManager::showPhonemeDebugging;

/*
================
TGOverlayManager::TGOverlayManager
================
*/
// FUNCTION: STBC 0x007058f0
TGOverlayManager::TGOverlayManager( void ) {
	nextPhonemeID = 1;
	unknown2C = true;
}

/*
================
TGOverlayManager::~TGOverlayManager

Deletes the entries and the phonemes the maps own.
================
*/
// FUNCTION: STBC 0x007059f0
TGOverlayManager::~TGOverlayManager( void ) {
	NiTMapIterator pos = controllers.GetFirstPos();
	while ( pos != NULL ) {
		const char *name;
		TGOverlayEntry *entry;
		controllers.GetNext( pos, name, entry );
		NiTListIterator item = entry->shown.GetHeadPos();
		while ( item != NULL ) {
			delete entry->shown.GetNext( item );
		}
		item = entry->families.GetHeadPos();
		while ( item != NULL ) {
			delete entry->families.GetNext( item );
		}
		delete entry;
	}
	pos = phonemes.GetFirstPos();
	while ( pos != NULL ) {
		long id;
		TGPhoneme *phoneme;
		phonemes.GetNext( pos, id, phoneme );
		delete phoneme;
	}
}

/*
================
TGOverlayManager::AddController
================
*/
// FUNCTION: STBC 0x00705c20
void TGOverlayManager::AddController( NiObject *controller, char *name ) {
	if ( controller != NULL && name != NULL ) {
		TGOverlayEntry *entry;
		if ( controllers.GetAt( name, entry ) ) {
			return;
		}
		entry = new TGOverlayEntry;
		entry->controller = controller;
		controllers.SetAt( name, entry );
	}
}

/*
================
TGOverlayManager::GetController

A controller only the manager holds is as good as gone.
================
*/
// FUNCTION: STBC 0x00705d70
NiObject *TGOverlayManager::GetController( char *name ) {
	if ( name == NULL ) {
		return NULL;
	}
	TGOverlayEntry *entry;
	if ( !controllers.GetAt( name, entry ) ) {
		return NULL;
	}
	NiObject *controller = entry->controller;
	if ( controller != NULL && controller->GetRefCount() == 1 ) {
		return NULL;
	}
	return controller;
}

/*
================
TGOverlayManager::RemoveController

Drops the controller filed under the name, with its families and shown
overlays (invented name).
================
*/
// FUNCTION: STBC 0x00705dd0
void TGOverlayManager::RemoveController( char *name ) {
	if ( name == NULL ) {
		return;
	}
	TGOverlayEntry *entry;
	if ( !controllers.GetAt( name, entry ) ) {
		return;
	}
	controllers.RemoveAt( name );
	delete entry;
}

/*
================
TGOverlayManager::AddFamily
================
*/
// FUNCTION: STBC 0x00705f50
void TGOverlayManager::AddFamily( char *controller, char *family ) {
	if ( family == NULL ) {
		return;
	}
	TGOverlayEntry *entry;
	if ( !controllers.GetAt( controller, entry ) ) {
		return;
	}
	NiTListIterator pos = entry->families.GetHeadPos();
	while ( pos != NULL ) {
		if ( strcmp( entry->families.GetNext( pos ), family ) == 0 ) {
			return;
		}
	}
	char *copy = new char[strlen( family ) + 1];
	strcpy( copy, family );
	entry->families.AddTail( copy );
}

/*
================
TGOverlayManager::RemoveFamily

Takes the family name out of the controller's families (invented name).
================
*/
// FUNCTION: STBC 0x00706090
void TGOverlayManager::RemoveFamily( char *controller, char *family ) {
	if ( family == NULL ) {
		return;
	}
	TGOverlayEntry *entry;
	if ( !controllers.GetAt( controller, entry ) ) {
		return;
	}
	NiTListIterator pos = entry->families.GetHeadPos();
	while ( pos != NULL ) {
		char *name = entry->families.GetNext( pos );
		if ( strcmp( name, family ) == 0 ) {
			entry->families.Remove( name );
			delete [] name;
			return;
		}
	}
}

/*
================
TGOverlayManager::ShowOverlay

A controller shows the overlay at once without a duration and through an
action for the duration given; an animation node plays the overlay as an
animation, for the duration or a short while.
================
*/
// FUNCTION: STBC 0x00706220
void TGOverlayManager::ShowOverlay( char *controller, char *overlay, float duration ) {
	if ( controller == NULL || overlay == NULL ) {
		return;
	}
	NiObject *object = GetController( controller );
	if ( object == NULL ) {
		return;
	}
	if ( object->IsKindOf( &TGOverlayController::rtti ) ) {
		if ( duration > 0.0f ) {
			TGOverlayAction *action = new TGOverlayAction( this, controller, overlay, duration );
			action->Play();
		} else {
			AddOverlay( controller, overlay );
		}
	} else if ( object->IsKindOf( &TGAnimNode::rtti ) ) {
		TGAnimNode *node = NiDynamicCast( TGAnimNode, object );
		TGAnimAction *action = new TGAnimAction( node, overlay, 0, 0, 0, 0 );
		if ( duration > 0.0f ) {
			action->SetDuration( duration );
		} else {
			action->SetDuration( 0.05f );
		}
		action->Play();
	}
}

/*
================
TGOverlayManager::ShowPhoneme

Plays a copy of the phoneme for its own duration.
================
*/
// FUNCTION: STBC 0x00706390
void TGOverlayManager::ShowPhoneme( TGPhoneme *phoneme ) {
	if ( phoneme != NULL ) {
		TGPhonemeAction *action = new TGPhonemeAction( this, phoneme, -1.0f );
		action->Play();
	}
}

/*
================
TGOverlayManager::AddOverlay

An overlay of a family replaces the shown overlay of that family; the
family's own name is not shown.
================
*/
// FUNCTION: STBC 0x00706410
void TGOverlayManager::AddOverlay( char *controller, char *overlay ) {
	if ( overlay == NULL ) {
		return;
	}
	TGOverlayEntry *entry;
	if ( !controllers.GetAt( controller, entry ) ) {
		return;
	}
	char *family = FindFamily( controller, overlay );
	if ( family != NULL ) {
		RemoveFamilyOverlay( controller, family );
		if ( strcmp( overlay, family ) == 0 ) {
			return;
		}
	}
	char *copy = new char[strlen( overlay ) + 1];
	strcpy( copy, overlay );
	entry->shown.AddTail( copy );
	ApplyOverlays();
}

/*
================
TGOverlayManager::RemoveOverlay

Takes every shown copy of the overlay back.
================
*/
// FUNCTION: STBC 0x00706560
void TGOverlayManager::RemoveOverlay( char *controller, char *overlay ) {
	TGOverlayEntry *entry;
	if ( !controllers.GetAt( controller, entry ) ) {
		return;
	}
	NiTListIterator pos = entry->shown.GetHeadPos();
	while ( pos != NULL ) {
		char *name = entry->shown.GetNext( pos );
		if ( strcmp( name, overlay ) == 0 ) {
			entry->shown.Remove( name );
			delete [] name;
		}
	}
	ApplyOverlays();
}

/*
================
TGOverlayManager::FindFamily
================
*/
// FUNCTION: STBC 0x007066e0
char *TGOverlayManager::FindFamily( char *controller, char *overlay ) {
	if ( overlay == NULL ) {
		return NULL;
	}
	TGOverlayEntry *entry;
	if ( !controllers.GetAt( controller, entry ) ) {
		return NULL;
	}
	NiTListIterator pos = entry->families.GetHeadPos();
	while ( pos != NULL ) {
		char *family = entry->families.GetNext( pos );
		if ( family != NULL && strstr( overlay, family ) != NULL ) {
			return family;
		}
	}
	return NULL;
}

/*
================
TGOverlayManager::RemoveFamilyOverlay
================
*/
// FUNCTION: STBC 0x00706760
void TGOverlayManager::RemoveFamilyOverlay( char *controller, char *family ) {
	if ( family == NULL ) {
		return;
	}
	TGOverlayEntry *entry;
	if ( !controllers.GetAt( controller, entry ) ) {
		return;
	}
	NiTListIterator pos = entry->shown.GetHeadPos();
	while ( pos != NULL ) {
		char *name = entry->shown.GetNext( pos );
		if ( name != NULL && strstr( name, family ) != NULL ) {
			RemoveOverlay( controller, name );
			return;
		}
	}
}

/*
================
TGOverlayManager::AddOverlayPhoneme
================
*/
// FUNCTION: STBC 0x007067f0
void TGOverlayManager::AddOverlayPhoneme( char *controller, char *overlay, float duration ) {
	TGPhoneme *phoneme = new TGPhoneme;
	phoneme->AddEntry( controller, overlay, 0.0f, -1.0f );
	phoneme->duration = duration;
	AddPhoneme( phoneme );
}

/*
================
TGOverlayManager::AddPhoneme

Files the phoneme under the next ID and returns it; 0 for no phoneme.
================
*/
// FUNCTION: STBC 0x00706880
long TGOverlayManager::AddPhoneme( TGPhoneme *phoneme ) {
	if ( phoneme == NULL ) {
		return 0;
	}
	long id = nextPhonemeID++;
	phonemes.SetAt( id, phoneme );
	return id;
}

/*
================
TGOverlayManager::GetPhoneme
================
*/
// FUNCTION: STBC 0x00706920
TGPhoneme *TGOverlayManager::GetPhoneme( long id ) {
	TGPhoneme *phoneme;
	if ( phonemes.GetAt( id, phoneme ) ) {
		return phoneme;
	}
	return NULL;
}

/*
================
TGOverlayManager::ConvertLWVToLIP

Writes a lip-sync file from a .LWV sound: after the "PMRK" marker, each
marker's phoneme (its index in the code table, 0 when the table lacks it),
its start and its length in seconds, from the marker positions in samples.
================
*/
// FUNCTION: STBC 0x00706970
void TGOverlayManager::ConvertLWVToLIP( char *lwvFile, char *lipFile, PhonemeCode *codes, int numCodes ) {
	TGStream in;
	in.VirtualSlot00( lwvFile, "rb" );

	// The file starts as a WAVE file does; the sample rate is at 24.
	char header[256];
	in.Read( header, sizeof( header ) );
	unsigned int sampleRate = *(unsigned int *)&header[24];

	while ( !in.Eof() ) {
		if ( in.ReadChar() == 'P' && in.ReadChar() == 'M' && in.ReadChar() == 'R' && in.ReadChar() == 'K' ) {
			break;
		}
	}
	if ( in.Eof() ) {
		return;
	}

	TGStream out;
	out.VirtualSlot00( lipFile, "wb" );

	unsigned short lastPosition = 0;
	float start = 0.0f;
	while ( !in.Eof() ) {
		float rate = (float)sampleRate;
		char buffer[256];
		in.Read( buffer, 8 );
		buffer[8] = '\0';
		in.Read( buffer, 2 );
		buffer[2] = '\0';
		unsigned short position = *(unsigned short *)buffer;
		float length = (unsigned short)( position - lastPosition ) / rate;
		in.Read( buffer, 6 );
		in.Read( buffer, 1 );
		int nameLength = *(int *)buffer & 0xFF;
		in.Read( buffer, 3 );
		in.Read( buffer, nameLength );
		buffer[nameLength] = '\0';
		if ( strchr( buffer, '+' ) != NULL ) {
			*strchr( buffer, '+' ) = '\0';
		}

		int phoneme = 0;
		for ( int code = 0; code < numCodes; code++ ) {
			if ( strcmp( buffer, codes[code].code ) == 0 ) {
				phoneme = code;
				break;
			}
		}
		out.WriteID( phoneme );
		out.WriteFloat( start );
		out.WriteFloat( length );
		start += length;
		lastPosition = position;
	}
	in.Close();
	out.Close();
}

/*
================
TGOverlayManager::BuildSequenceFromStream

The first record starts the first entry at time 0, a later entry takes the
start of the record that opened it.
================
*/
// FUNCTION: STBC 0x00706c60
// DRAFT: structure and calls match; the compiler makes a second counter for the count + 1 prints and lays the merge block out after the flush block, retail the reverse
TGSequence *TGOverlayManager::BuildSequenceFromStream( TGStream *stream, PhonemeCode *codes, int unused, int endPos ) {
	char text[260];
	TGSequence *sequence = new TGSequence;
	TGPhonemeSequence *phonemes = new TGPhonemeSequence( this );
	if ( endPos <= 0 ) {
		endPos = stream->GetLength();
	}
	int pendingID = -1;
	float pendingDuration = 0.0f;
	float pendingStart = 0.0f;
	int count = 0;
	if ( showPhonemeDebugging ) {
		OutputDebugString( "\n\n" );
	}
	while ( stream->GetPos() < endPos && !stream->Eof() ) {
		long id = stream->ReadID();
		float start = stream->ReadFloat();
		float duration = stream->ReadFloat();
		int phonemeID = id;
		if ( codes != NULL ) {
			phonemeID = codes[id].phoneme;
		}
		if ( showPhonemeDebugging ) {
			sprintf( text, "%d -> %d, %f for %f\n", id, phonemeID, start, duration );
			OutputDebugString( text );
		}
		if ( pendingID != -1 && phonemeID != pendingID ) {
			phonemes->AddEntry( pendingID, pendingStart, pendingDuration );
			if ( showPhonemeDebugging ) {
				sprintf( text, "Adding %d, %f for %f\n", pendingID, pendingStart, pendingDuration );
				OutputDebugString( text );
				if ( showPhonemeDebugging ) {
					sprintf( text, "%d phonemes generated to sequence\n", count + 1 );
					OutputDebugString( text );
				}
			}
			pendingID = phonemeID;
			pendingDuration = duration;
			pendingStart = start;
			count++;
		} else {
			pendingID = phonemeID;
			pendingDuration += duration;
		}
	}
	phonemes->AddEntry( pendingID, pendingStart, pendingDuration );
	if ( showPhonemeDebugging ) {
		sprintf( text, "Adding %d, %f for %f\n", pendingID, pendingStart, pendingDuration );
		OutputDebugString( text );
		if ( showPhonemeDebugging ) {
			sprintf( text, "%d phonemes generated to sequence\n", count + 1 );
			OutputDebugString( text );
		}
	}
	sequence->AppendAction( phonemes, 0.0f );
	return sequence;
}

/*
================
TGOverlayManager::BuildRandomSequence

Picks phonemes at random, never the same twice in a row and none without
a duration, until the sequence lasts the duration.
================
*/
// FUNCTION: STBC 0x00706ef0
TGSequence *TGOverlayManager::BuildRandomSequence( float duration, char *saveFile, int firstID, int lastID ) {
	if ( duration <= 0.0f ) {
		return NULL;
	}
	if ( firstID == 0 ) {
		firstID = 1;
	}
	if ( lastID == 0 ) {
		lastID = nextPhonemeID - 1;
	}
	TGStream *stream = NULL;
	float time = 0.0f;
	TGSequence *sequence = new TGSequence;
	TGPhonemeSequence *phonemes = new TGPhonemeSequence( this );
	int lastPick = -1;
	if ( saveFile != NULL ) {
		stream = new TGStream;
		stream->VirtualSlot00( saveFile, "wb" );
		if ( stream->VirtualSlot58() != 0 ) {
			delete stream;
			stream = NULL;
		}
	}
	while ( time < duration ) {
		int pick = g_kSystemWrapper.GetRandomNumber( lastID - firstID + 1 ) + firstID;
		if ( pick == lastPick ) {
			continue;
		}
		TGPhoneme *phoneme = GetPhoneme( pick );
		if ( phoneme->GetDuration() <= 0.0f ) {
			continue;
		}
		if ( stream != NULL ) {
			stream->WriteID( pick );
			stream->WriteFloat( time );
			stream->WriteFloat( phoneme->GetDuration() );
		}
		phonemes->AddEntry( pick, time, phoneme->GetDuration() );
		time += phoneme->GetDuration();
		lastPick = pick;
	}
	if ( stream != NULL ) {
		stream->Close();
		delete stream;
	}
	phonemes->SetDuration( duration );
	sequence->AddAction( phonemes, NULL, 0.0f );
	return sequence;
}

/*
================
TGOverlayManager::ApplyOverlays

Restores every named controller's face image and shows the overlay names it
has kept since, then tells the image its pixels changed; only while
unknown2C is on (CharacterClass::Update sets it while the character speaks).
================
*/
// FUNCTION: STBC 0x00707120
void TGOverlayManager::ApplyOverlays( void ) {
	if ( !unknown2C ) {
		return;
	}
	NiTMapIterator pos = controllers.GetFirstPos();
	while ( pos != NULL ) {
		const char *name;
		TGOverlayEntry *entry;
		controllers.GetNext( pos, name, entry );
		if ( entry != NULL && entry->controller != NULL && entry->controller->IsKindOf( &TGOverlayController::rtti ) ) {
			TGOverlayController *overlay = NiDynamicCast( TGOverlayController, entry->controller );
			overlay->ResetImage();
			NiTListIterator item = entry->shown.GetHeadPos();
			while ( item != NULL ) {
				overlay->ShowOverlay( entry->shown.GetNext( item ) );
			}
			overlay->UpdateImage();
		}
	}
}

/*
================
TGOverlayManager::Unknown707240
================
*/
// FUNCTION: STBC 0x00707240
void TGOverlayManager::Unknown707240( bool value ) {
	bool previous = unknown2C;
	unknown2C = value;
	if ( value && !previous ) {
		ApplyOverlays();
	}
}
