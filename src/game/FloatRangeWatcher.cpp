/*
===============================================================================

	FloatRangeWatcher.cpp

===============================================================================
*/

#include <iostream>
#include <stdio.h>

#include "FloatRangeWatcher.h"
#include "TGEventManager.h"
#include "TGStream.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The disc data's key (DiscCheck.h's GetDiscDataKey), hidden at the head of
// this unrelated unit's data; it repeats every 400 bytes, as BezierCurve's
// DecodeDiscData reads it.
#define DISC_DATA_KEY_SIZE	400

// DATA: STBC 0x008dad08
static unsigned char discDataKey[DISC_DATA_KEY_SIZE] = {
	0xB9, 0x61, 0x88, 0x96, 0x41, 0x91, 0x99, 0x37, 0x8D, 0x8A, 0xB9, 0x9C,
	0xB6, 0x5F, 0x11, 0x50, 0xE7, 0x1D, 0x22, 0x1A, 0xBF, 0xFE, 0x0A, 0x8B,
	0xF6, 0xDF, 0x02, 0xD3, 0x44, 0xF4, 0x2C, 0xD9, 0x7E, 0xBB, 0x8C, 0x8A,
	0x03, 0x2E, 0x53, 0x70, 0x56, 0x3D, 0x64, 0x87, 0x0C, 0x7B, 0x11, 0x4D,
	0xE9, 0x18, 0x25, 0x19, 0x46, 0x0F, 0x28, 0xAD, 0xB5, 0xA0, 0xDE, 0x0E,
	0xE8, 0x01, 0x11, 0x94, 0xCF, 0x51, 0x90, 0x3D, 0x01, 0xCD, 0x78, 0x46,
	0x5D, 0x46, 0xB2, 0xFD, 0xF1, 0xD3, 0xBD, 0xD1, 0x1B, 0xC3, 0xAE, 0xAB,
	0xED, 0xD5, 0x75, 0x81, 0xD6, 0xAC, 0x61, 0x28, 0x7F, 0x7D, 0xE7, 0x69,
	0xF4, 0x0B, 0x9F, 0x58, 0x04, 0xD5, 0x8F, 0xE1, 0xEA, 0x8D, 0xAC, 0xA4,
	0x2D, 0xCD, 0x9E, 0x03, 0xC4, 0x06, 0xC4, 0x78, 0x7E, 0xB9, 0x78, 0x2C,
	0xA1, 0xE9, 0x94, 0xC6, 0xD1, 0xD0, 0x34, 0x81, 0x34, 0xD1, 0xBE, 0x82,
	0xD4, 0xAE, 0x20, 0x68, 0x45, 0xFB, 0x59, 0x24, 0x87, 0xD2, 0x3B, 0x0B,
	0x2D, 0xC9, 0x6F, 0x29, 0xBF, 0x21, 0xB9, 0xD9, 0x5E, 0x40, 0x7D, 0x93,
	0xA6, 0x61, 0x83, 0x04, 0xD9, 0x8B, 0xF7, 0x64, 0x39, 0xBF, 0xB4, 0x04,
	0xB5, 0x76, 0xC2, 0x26, 0xC8, 0x4B, 0x1C, 0x12, 0x9F, 0xF4, 0xB7, 0x66,
	0x7A, 0x76, 0xC1, 0xAE, 0x54, 0x99, 0x26, 0x34, 0xC6, 0x9A, 0x59, 0x1A,
	0x29, 0x21, 0x51, 0xA5, 0xFB, 0x72, 0xD2, 0xDC, 0x83, 0xE7, 0xED, 0x52,
	0xB8, 0x9E, 0xCA, 0x40, 0x60, 0x6F, 0xA5, 0xD6, 0x75, 0x21, 0x17, 0xD3,
	0xCC, 0xDD, 0x97, 0x54, 0xF9, 0xE2, 0x40, 0xE9, 0x6D, 0x7B, 0xD5, 0xEE,
	0xE1, 0x2E, 0x02, 0x19, 0xF8, 0x37, 0xE4, 0x4F, 0x1F, 0x35, 0xCC, 0xBC,
	0xB8, 0x22, 0x40, 0x22, 0x79, 0x88, 0x43, 0x71, 0x0E, 0xF2, 0xD7, 0x99,
	0x06, 0xA1, 0xBF, 0x9C, 0xEC, 0x81, 0x89, 0xE7, 0xB4, 0x5C, 0xCC, 0xE2,
	0x5A, 0x4C, 0xAF, 0xC6, 0xC5, 0x76, 0xAA, 0xAF, 0xF1, 0xF5, 0x91, 0xF2,
	0x4D, 0x15, 0xCE, 0xB0, 0x62, 0xC2, 0xED, 0xAB, 0xB6, 0x40, 0x5F, 0x59,
	0xF0, 0x1C, 0x77, 0x37, 0x3C, 0x62, 0xB9, 0x60, 0xF1, 0x10, 0x54, 0x5E,
	0x72, 0xCB, 0xED, 0x40, 0x52, 0xD1, 0x9E, 0xEC, 0xBB, 0x2F, 0x3F, 0xB7,
	0x10, 0x34, 0xE4, 0x33, 0xD1, 0x25, 0xA6, 0x48, 0xBF, 0x32, 0xA7, 0x87,
	0x41, 0xA9, 0x50, 0xBA, 0x07, 0x6A, 0xDD, 0xC0, 0xED, 0x9B, 0x1C, 0x98,
	0x22, 0x9C, 0x71, 0xBB, 0x89, 0x3D, 0x1E, 0xAF, 0x5F, 0x2F, 0xC1, 0xD9,
	0x1F, 0xB9, 0x1E, 0x93, 0xA2, 0xAC, 0x1F, 0x7E, 0x8B, 0x99, 0x19, 0x19,
	0xE2, 0x41, 0x51, 0x93, 0xFF, 0x4D, 0xBF, 0xDE, 0xAA, 0x40, 0x12, 0x05,
	0x80, 0xA9, 0xF4, 0xBF, 0x99, 0x9D, 0x8E, 0x44, 0x67, 0x64, 0x4F, 0x5F,
	0xE1, 0x73, 0xE9, 0xC5, 0xE1, 0x9A, 0x9D, 0xA5, 0xCB, 0x7F, 0xB9, 0x80,
	0x70, 0x4B, 0x5F, 0x3B
};

/*
================
GetDiscDataKey

The key DecodeDiscData XORs "texture.dat" with.
================
*/
// FUNCTION: STBC 0x0044c1a0
unsigned char *GetDiscDataKey( void ) {
	return discDataKey;
}

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0044cc10 _$E436
// FUNCTION: STBC 0x0044cc30 _$E435

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0044c7d0 _$E32
// FUNCTION: STBC 0x0044c7e0 _$E29
// FUNCTION: STBC 0x0044c7f0 _$E31
// FUNCTION: STBC 0x0044c800 _$E30
// FUNCTION: STBC 0x0044c810 _$E38
// FUNCTION: STBC 0x0044c820 _$E35
// FUNCTION: STBC 0x0044c830 _$E37
// FUNCTION: STBC 0x0044c840 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0044c850 _$E60
// FUNCTION: STBC 0x0044c860 _$E59
// FUNCTION: STBC 0x0044c870 _$E64
// FUNCTION: STBC 0x0044c880 _$E63
// FUNCTION: STBC 0x0044c890 _$E75
// FUNCTION: STBC 0x0044c8a0 _$E74
// FUNCTION: STBC 0x0044c8b0 _$E79
// FUNCTION: STBC 0x0044c8c0 _$E78
// FUNCTION: STBC 0x0044c8d0 _$E83
// FUNCTION: STBC 0x0044c8e0 _$E82
// FUNCTION: STBC 0x0044c8f0 _$E87
// FUNCTION: STBC 0x0044c900 _$E86
// FUNCTION: STBC 0x0044c910 _$E211
// FUNCTION: STBC 0x0044c920 _$E210
// FUNCTION: STBC 0x0044c930 _$E153
// FUNCTION: STBC 0x0044c940 _$E152
// FUNCTION: STBC 0x0044c950 _$E166
// FUNCTION: STBC 0x0044c960 _$E165
// FUNCTION: STBC 0x0044c970 _$E216
// FUNCTION: STBC 0x0044c980 _$E215
// FUNCTION: STBC 0x0044c990 _$E221
// FUNCTION: STBC 0x0044c9a0 _$E220
// FUNCTION: STBC 0x0044c9b0 _$E225
// FUNCTION: STBC 0x0044c9c0 _$E224
// FUNCTION: STBC 0x0044c9d0 _$E229
// FUNCTION: STBC 0x0044c9e0 _$E228
// FUNCTION: STBC 0x0044c9f0 _$E236
// FUNCTION: STBC 0x0044ca00 _$E235
// FUNCTION: STBC 0x0044ca10 _$E261
// FUNCTION: STBC 0x0044ca20 _$E260
// FUNCTION: STBC 0x0044ca30 _$E265
// FUNCTION: STBC 0x0044ca40 _$E264
// FUNCTION: STBC 0x0044ca50 _$E111
// FUNCTION: STBC 0x0044ca60 _$E110
// FUNCTION: STBC 0x0044ca70 _$E247
// FUNCTION: STBC 0x0044ca80 _$E246
// FUNCTION: STBC 0x0044ca90 _$E254
// FUNCTION: STBC 0x0044caa0 _$E253
// FUNCTION: STBC 0x0044cab0 _$E277
// FUNCTION: STBC 0x0044cac0 _$E276
// FUNCTION: STBC 0x0044cad0 _$E281
// FUNCTION: STBC 0x0044cae0 _$E280
// FUNCTION: STBC 0x0044caf0 _$E321
// FUNCTION: STBC 0x0044cb00 _$E320
// FUNCTION: STBC 0x0044cb10 _$E327
// FUNCTION: STBC 0x0044cb20 _$E326
// FUNCTION: STBC 0x0044cb30 _$E269
// FUNCTION: STBC 0x0044cb40 _$E268
// FUNCTION: STBC 0x0044cb50 _$E273
// FUNCTION: STBC 0x0044cb60 _$E272
// FUNCTION: STBC 0x0044cb70 _$E285
// FUNCTION: STBC 0x0044cb80 _$E284
// FUNCTION: STBC 0x0044cb90 _$E289
// FUNCTION: STBC 0x0044cba0 _$E288
// FUNCTION: STBC 0x0044cbb0 _$E293
// FUNCTION: STBC 0x0044cbc0 _$E292
// FUNCTION: STBC 0x0044cbd0 _$E297
// FUNCTION: STBC 0x0044cbe0 _$E296
// FUNCTION: STBC 0x0044cbf0 _$E301
// FUNCTION: STBC 0x0044cc00 _$E300
// FUNCTION: STBC 0x0044cc40 _$E305
// FUNCTION: STBC 0x0044cc50 _$E304
// FUNCTION: STBC 0x0044cc60 _$E309
// FUNCTION: STBC 0x0044cc70 _$E308
// FUNCTION: STBC 0x0044cc80 _$E313
// FUNCTION: STBC 0x0044cc90 _$E312
// FUNCTION: STBC 0x0044cca0 _$E116
// FUNCTION: STBC 0x0044ccb0 _$E115
// FUNCTION: STBC 0x0044ccc0 _$E317
// FUNCTION: STBC 0x0044ccd0 _$E316
// FUNCTION: STBC 0x0044cce0 _$E121
// FUNCTION: STBC 0x0044ccf0 _$E120
// FUNCTION: STBC 0x0044cd00 _$E331
// FUNCTION: STBC 0x0044cd10 _$E330
// FUNCTION: STBC 0x0044cd20 _$E335
// FUNCTION: STBC 0x0044cd30 _$E334
// FUNCTION: STBC 0x0044cd40 _$E339
// FUNCTION: STBC 0x0044cd50 _$E338
// FUNCTION: STBC 0x0044cd60 _$E343
// FUNCTION: STBC 0x0044cd70 _$E342
// FUNCTION: STBC 0x0044cd80 _$E130
// FUNCTION: STBC 0x0044cd90 _$E129
// FUNCTION: STBC 0x0044cda0 _$E347
// FUNCTION: STBC 0x0044cdb0 _$E346
// FUNCTION: STBC 0x0044cdc0 _$E351
// FUNCTION: STBC 0x0044cdd0 _$E350
// FUNCTION: STBC 0x0044cde0 _$E355
// FUNCTION: STBC 0x0044cdf0 _$E354
// FUNCTION: STBC 0x0044ce00 _$E136
// FUNCTION: STBC 0x0044ce10 _$E135
// FUNCTION: STBC 0x0044ce20 _$E141
// FUNCTION: STBC 0x0044ce30 _$E140
// FUNCTION: STBC 0x0044ce40 _$E171
// FUNCTION: STBC 0x0044ce50 _$E170
// FUNCTION: STBC 0x0044ce60 _$E359
// FUNCTION: STBC 0x0044ce70 _$E358
// FUNCTION: STBC 0x0044ce80 _$E176
// FUNCTION: STBC 0x0044ce90 _$E175
// FUNCTION: STBC 0x0044cea0 _$E363
// FUNCTION: STBC 0x0044ceb0 _$E362
// FUNCTION: STBC 0x0044cec0 _$E367
// FUNCTION: STBC 0x0044ced0 _$E366
// FUNCTION: STBC 0x0044cee0 _$E181
// FUNCTION: STBC 0x0044cef0 _$E180
// FUNCTION: STBC 0x0044cf00 _$E371
// FUNCTION: STBC 0x0044cf10 _$E370
// FUNCTION: STBC 0x0044cf20 _$E146
// FUNCTION: STBC 0x0044cf30 _$E145
// FUNCTION: STBC 0x0044cf40 _$E375
// FUNCTION: STBC 0x0044cf50 _$E374
// FUNCTION: STBC 0x0044cf60 _$E379
// FUNCTION: STBC 0x0044cf70 _$E378
// FUNCTION: STBC 0x0044cf80 _$E186
// FUNCTION: STBC 0x0044cf90 _$E185
// FUNCTION: STBC 0x0044cfa0 _$E191
// FUNCTION: STBC 0x0044cfb0 _$E190
// FUNCTION: STBC 0x0044cfc0 _$E383
// FUNCTION: STBC 0x0044cfd0 _$E382
// FUNCTION: STBC 0x0044cfe0 _$E387
// FUNCTION: STBC 0x0044cff0 _$E386
// FUNCTION: STBC 0x0044d000 _$E391
// FUNCTION: STBC 0x0044d010 _$E390
// FUNCTION: STBC 0x0044d020 _$E395
// FUNCTION: STBC 0x0044d030 _$E394
// FUNCTION: STBC 0x0044d040 _$E399
// FUNCTION: STBC 0x0044d050 _$E398
// FUNCTION: STBC 0x0044d060 _$E403
// FUNCTION: STBC 0x0044d070 _$E402
// FUNCTION: STBC 0x0044d080 _$E407
// FUNCTION: STBC 0x0044d090 _$E406
// FUNCTION: STBC 0x0044d0a0 _$E411
// FUNCTION: STBC 0x0044d0b0 _$E410
// FUNCTION: STBC 0x0044d0c0 _$E196
// FUNCTION: STBC 0x0044d0d0 _$E195
// FUNCTION: STBC 0x0044d0e0 _$E415
// FUNCTION: STBC 0x0044d0f0 _$E414
// FUNCTION: STBC 0x0044d100 _$E201
// FUNCTION: STBC 0x0044d110 _$E200
// FUNCTION: STBC 0x0044d120 _$E206
// FUNCTION: STBC 0x0044d130 _$E205
// FUNCTION: STBC 0x0044d140 _$E419
// FUNCTION: STBC 0x0044d150 _$E418
// FUNCTION: STBC 0x0044d160 _$E424
// FUNCTION: STBC 0x0044d170 _$E423
// FUNCTION: STBC 0x0044d180 _$E428
// FUNCTION: STBC 0x0044d190 _$E427

// DATA: STBC 0x0098039c ?RID_INVALID@FloatRangeWatcher@@2HA
int FloatRangeWatcher::RID_INVALID;

/*
================
FloatRangeWatcher::FloatRangeWatcher

The list starts as a single interval over 0 to 1.
================
*/
// FUNCTION: STBC 0x0044c1b0 ??0FloatRangeWatcher@@QAE@XZ
FloatRangeWatcher::FloatRangeWatcher( void ) {
	variable = NULL;
	current = new FloatRangeCheck;
	nextID = RID_INVALID + 1;
}

/*
================
FloatRangeWatcher::~FloatRangeWatcher

Walks the list to its top interval and frees it downwards. An interval
whose event no longer resolves to an object is left out of the sweep.
================
*/
// FUNCTION: STBC 0x0044c220 ??1FloatRangeWatcher@@QAE@XZ
FloatRangeWatcher::~FloatRangeWatcher( void ) {
	while ( current->next != NULL ) {
		current = current->next;
	}
	while ( current != NULL ) {
		FloatRangeCheck *below = current->previous;
		if ( current->event == NULL || TGObject::GetTGObjectPtr( current->event->GetObjID() ) != NULL ) {
			delete current;
		}
		current = below;
	}
}

/*
================
FloatRangeCheck::FloatRangeCheck
================
*/
// FUNCTION: STBC 0x0044c290 ??0FloatRangeCheck@@QAE@XZ
FloatRangeCheck::FloatRangeCheck( void ) {
	low = 0.0f;
	high = 1.0f;
	event = NULL;
	mode = FloatRangeWatcher::FRW_NONE;
	id = FloatRangeWatcher::RID_INVALID;
	previous = NULL;
	next = NULL;
}

/*
================
FloatRangeCheck::~FloatRangeCheck
================
*/
// FUNCTION: STBC 0x0044c2c0 ??1FloatRangeCheck@@QAE@XZ
FloatRangeCheck::~FloatRangeCheck( void ) {
	if ( event != NULL ) {
		delete event;
	}
}

/*
================
FloatRangeWatcher::AddRangeCheck

Splits the interval the value falls in: below the value the interval keeps
what it had, above it the new event and mode apply. Returns the new range
check's ID.
================
*/
// FUNCTION: STBC 0x0044c2d0 ?AddRangeCheck@FloatRangeWatcher@@QAEHMHPAVTGFloatEvent@@@Z
int FloatRangeWatcher::AddRangeCheck( float value, int mode, TGFloatEvent *event ) {
	FloatRangeCheck *check = current;
	while ( check->low > value && check->previous != NULL ) {
		check = check->previous;
	}
	while ( check->high < value && check->next != NULL ) {
		check = check->next;
	}
	FloatRangeCheck *below = new FloatRangeCheck;
	below->mode = check->mode;
	below->event = check->event;
	below->high = value;
	below->low = check->low;
	below->id = check->id;
	below->next = check;
	below->previous = check->previous;
	if ( below->previous != NULL ) {
		below->previous->next = below;
	}
	check->mode = mode;
	check->event = event;
	check->low = value;
	check->id = nextID;
	nextID++;
	check->previous = below;
	while ( *variable < current->low ) {
		if ( current->previous == NULL ) {
			break;
		}
		current = current->previous;
	}
	// BUG: the walk up compares with the new check's value, not the watched variable
	while ( current->high < value && current->next != NULL ) {
		current = current->next;
	}
	return check->id;
}

/*
================
FloatRangeWatcher::RemoveRangeCheck

Joins the interval of the ID to the one below it.
================
*/
// FUNCTION: STBC 0x0044c3f0 ?RemoveRangeCheck@FloatRangeWatcher@@QAEXH@Z
void FloatRangeWatcher::RemoveRangeCheck( int id ) {
	FloatRangeCheck *found = NULL;
	FloatRangeCheck *check = current;
	if ( check != NULL ) {
		do {
			if ( check->id == id ) {
				found = check;
			}
			check = check->previous;
		} while ( check != NULL );
	}
	if ( found == NULL ) {
		check = current->next;
		if ( check == NULL ) {
			return;
		}
		do {
			if ( check->id == id ) {
				found = check;
			}
			check = check->next;
		} while ( check != NULL );
		if ( found == NULL ) {
			return;
		}
	}
	FloatRangeCheck *above = found->previous;
	if ( above != NULL ) {
		above->high = found->high;
		above->next = found->next;
		if ( found->next != NULL ) {
			found->next->previous = above;
		}
		if ( current == found ) {
			current = above;
		}
		delete found;
	}
}

/*
================
FloatRangeWatcher::CheckRanges

Moves to the interval the variable is in, sending the event of each interval
it enters that is set to report the crossing.
================
*/
// FUNCTION: STBC 0x0044c470 ?CheckRanges@FloatRangeWatcher@@QAEXXZ
// PARKED: register/x87 copy: retail copies the variable through the FPU (fld/fstp: no single-use intermediate found; GetWatchedVariable() and *variable both give integer moves) and keeps the current interval in EAX in the downward loop, so it reloads it at the loop top where this build keeps ECX and tail-duplicates the previous test
void FloatRangeWatcher::CheckRanges( void ) {
	float value = GetWatchedVariable();
	if ( value < current->low ) {
		while ( current->previous != NULL ) {
			if ( current->mode == FRW_BELOW || current->mode == FRW_BOTH ) {
				SendEvent( value, current->event );
			}
			current = current->previous;
			if ( value > current->low ) {
				break;
			}
		}
	} else {
		while ( value >= current->high && current->next != NULL ) {
			current = current->next;
			if ( current->mode == FRW_ABOVE || current->mode == FRW_BOTH ) {
				SendEvent( value, current->event );
			}
		}
	}
}

/*
================
FloatRangeWatcher::SendEvent
================
*/
// FUNCTION: STBC 0x0044c520 ?SendEvent@FloatRangeWatcher@@AAEXMPAVTGFloatEvent@@@Z
void FloatRangeWatcher::SendEvent( float value, TGFloatEvent *event ) {
	event->SetFloat( value );
	g_kEventManager.AddEvent( event->Duplicate() );
}

/*
================
FloatRangeWatcher::WriteToStream

The range checks from the lowest up, each after a true and with the ID of
its event; then a false, the ID of the current range check and the next ID.
================
*/
// FUNCTION: STBC 0x0044c540
bool FloatRangeWatcher::WriteToStream( TGStream *stream ) {
	char message[256];
	sprintf( message, "FloatRangeWatcher mem address = %d\n", this );
	TGObject::Log( message );

	FloatRangeCheck *check = current;
	while ( check != NULL && check->previous != NULL ) {
		check = check->previous;
	}
	while ( check != NULL ) {
		stream->WriteBool( true );
		stream->WriteFloat( check->low );
		stream->WriteFloat( check->high );
		stream->WriteLong( check->mode );
		stream->WriteID( check->id );
		stream->WriteID( TGObject::GetSavedID( check->event ) );
		check = check->next;
	}
	stream->WriteBool( false );
	// BUG: a watcher without a range check has no current one to read the ID from.
	stream->WriteID( current->id );
	stream->WriteID( nextID );
	return true;
}

/*
================
FloatRangeWatcher::ReadFromStream

Reads what WriteToStream wrote. The events stay IDs until PostFixUp.
================
*/
// FUNCTION: STBC 0x0044c620
bool FloatRangeWatcher::ReadFromStream( TGStream *stream ) {
	char message[256];
	sprintf( message, "FloatRangeWatcher mem address = %d\n", this );
	TGObject::Log( message );

	delete current;
	current = NULL;
	while ( stream->ReadBool() == true ) {
		FloatRangeCheck *previous = current;
		current = new FloatRangeCheck;
		current->low = stream->ReadFloat();
		current->high = stream->ReadFloat();
		current->mode = stream->ReadLong();
		current->id = stream->ReadID();
		current->event = (TGFloatEvent *)stream->ReadID();
		current->next = NULL;
		current->previous = previous;
		if ( previous != NULL ) {
			previous->next = current;
		}
	}
	long currentID = stream->ReadID();
	while ( current != NULL && current->id != currentID && current->previous != NULL ) {
		current = current->previous;
	}
	nextID = stream->ReadID();
	return true;
}

/*
================
FloatRangeWatcher::FixUpPointers

Whether every event a range check was saved with still exists.
================
*/
// FUNCTION: STBC 0x0044c750
bool FloatRangeWatcher::FixUpPointers( void ) {
	FloatRangeCheck *check = current;
	while ( check != NULL && check->previous != NULL ) {
		check = check->previous;
	}
	while ( check != NULL ) {
		if ( check->event != NULL && TGObject::GetTGObjectPtr( (long)check->event ) == NULL ) {
			return false;
		}
		check = check->next;
	}
	return true;
}

/*
================
FloatRangeWatcher::PostFixUp

The saved IDs become the events.
================
*/
// FUNCTION: STBC 0x0044c790
void FloatRangeWatcher::PostFixUp( void ) {
	FloatRangeCheck *check = current;
	while ( check != NULL && check->previous != NULL ) {
		check = check->previous;
	}
	while ( check != NULL ) {
		check->event = (TGFloatEvent *)TGObject::GetSavedObject( (long)check->event );
		check = check->next;
	}
}
