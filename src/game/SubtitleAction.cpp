/*
===============================================================================

	SubtitleAction.cpp

===============================================================================
*/

#include <iostream>
#include "SubtitleAction.h"
#include "SubtitleWindow.h"
#include "TGLocalizationDatabase.h"
#include "TGParagraph.h"
#include "TGStream.h"
#include "TopWindow.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0055caf0 _$E58
// FUNCTION: STBC 0x0055cb00 _$E57
// FUNCTION: STBC 0x0055cb10 _$E62
// FUNCTION: STBC 0x0055cb20 _$E61
// FUNCTION: STBC 0x0055cb30 _$E73
// FUNCTION: STBC 0x0055cb40 _$E72
// FUNCTION: STBC 0x0055cb50 _$E77
// FUNCTION: STBC 0x0055cb60 _$E76
// FUNCTION: STBC 0x0055cb70 _$E81
// FUNCTION: STBC 0x0055cb80 _$E80
// FUNCTION: STBC 0x0055cb90 _$E85
// FUNCTION: STBC 0x0055cba0 _$E84
// FUNCTION: STBC 0x0055cbb0 _$E209
// FUNCTION: STBC 0x0055cbc0 _$E208
// FUNCTION: STBC 0x0055cbd0 _$E151
// FUNCTION: STBC 0x0055cbe0 _$E150
// FUNCTION: STBC 0x0055cbf0 _$E164
// FUNCTION: STBC 0x0055cc00 _$E163
// FUNCTION: STBC 0x0055cc10 _$E214
// FUNCTION: STBC 0x0055cc20 _$E213
// FUNCTION: STBC 0x0055cc30 _$E219
// FUNCTION: STBC 0x0055cc40 _$E218
// FUNCTION: STBC 0x0055cc50 _$E223
// FUNCTION: STBC 0x0055cc60 _$E222
// FUNCTION: STBC 0x0055cc70 _$E227
// FUNCTION: STBC 0x0055cc80 _$E226
// FUNCTION: STBC 0x0055cc90 _$E234
// FUNCTION: STBC 0x0055cca0 _$E233
// FUNCTION: STBC 0x0055ccb0 _$E285
// FUNCTION: STBC 0x0055ccc0 _$E284
// FUNCTION: STBC 0x0055ccd0 _$E289
// FUNCTION: STBC 0x0055cce0 _$E288
// FUNCTION: STBC 0x0055ccf0 _$E109
// FUNCTION: STBC 0x0055cd00 _$E108
// FUNCTION: STBC 0x0055cd10 _$E271
// FUNCTION: STBC 0x0055cd20 _$E270
// FUNCTION: STBC 0x0055cd30 _$E278
// FUNCTION: STBC 0x0055cd40 _$E277
// FUNCTION: STBC 0x0055cd50 _$E301
// FUNCTION: STBC 0x0055cd60 _$E300
// FUNCTION: STBC 0x0055cd70 _$E305
// FUNCTION: STBC 0x0055cd80 _$E304
// FUNCTION: STBC 0x0055cd90 _$E345
// FUNCTION: STBC 0x0055cda0 _$E344
// FUNCTION: STBC 0x0055cdb0 _$E351
// FUNCTION: STBC 0x0055cdc0 _$E350
// FUNCTION: STBC 0x0055cdd0 _$E293
// FUNCTION: STBC 0x0055cde0 _$E292
// FUNCTION: STBC 0x0055cdf0 _$E297
// FUNCTION: STBC 0x0055ce00 _$E296
// FUNCTION: STBC 0x0055ce10 _$E309
// FUNCTION: STBC 0x0055ce20 _$E308
// FUNCTION: STBC 0x0055ce30 _$E313
// FUNCTION: STBC 0x0055ce40 _$E312
// FUNCTION: STBC 0x0055ce50 _$E317
// FUNCTION: STBC 0x0055ce60 _$E316
// FUNCTION: STBC 0x0055ce70 _$E321
// FUNCTION: STBC 0x0055ce80 _$E320
// FUNCTION: STBC 0x0055ce90 _$E325
// FUNCTION: STBC 0x0055cea0 _$E324
// FUNCTION: STBC 0x0055ceb0 _$E329
// FUNCTION: STBC 0x0055cec0 _$E328
// FUNCTION: STBC 0x0055ced0 _$E333
// FUNCTION: STBC 0x0055cee0 _$E332
// FUNCTION: STBC 0x0055cef0 _$E337
// FUNCTION: STBC 0x0055cf00 _$E336
// FUNCTION: STBC 0x0055cf10 _$E114
// FUNCTION: STBC 0x0055cf20 _$E113
// FUNCTION: STBC 0x0055cf60 _$E341
// FUNCTION: STBC 0x0055cf70 _$E340
// FUNCTION: STBC 0x0055cf80 _$E119
// FUNCTION: STBC 0x0055cf90 _$E118
// FUNCTION: STBC 0x0055cfa0 _$E355
// FUNCTION: STBC 0x0055cfb0 _$E354
// FUNCTION: STBC 0x0055cfc0 _$E359
// FUNCTION: STBC 0x0055cfd0 _$E358
// FUNCTION: STBC 0x0055cfe0 _$E363
// FUNCTION: STBC 0x0055cff0 _$E362
// FUNCTION: STBC 0x0055d000 _$E367
// FUNCTION: STBC 0x0055d010 _$E366
// FUNCTION: STBC 0x0055d020 _$E128
// FUNCTION: STBC 0x0055d030 _$E127
// FUNCTION: STBC 0x0055d040 _$E371
// FUNCTION: STBC 0x0055d050 _$E370
// FUNCTION: STBC 0x0055d060 _$E375
// FUNCTION: STBC 0x0055d070 _$E374
// FUNCTION: STBC 0x0055d080 _$E379
// FUNCTION: STBC 0x0055d090 _$E378
// FUNCTION: STBC 0x0055d0a0 _$E134
// FUNCTION: STBC 0x0055d0b0 _$E133
// FUNCTION: STBC 0x0055d0c0 _$E139
// FUNCTION: STBC 0x0055d0d0 _$E138
// FUNCTION: STBC 0x0055d0e0 _$E169
// FUNCTION: STBC 0x0055d0f0 _$E168
// FUNCTION: STBC 0x0055d100 _$E383
// FUNCTION: STBC 0x0055d110 _$E382
// FUNCTION: STBC 0x0055d120 _$E174
// FUNCTION: STBC 0x0055d130 _$E173
// FUNCTION: STBC 0x0055d140 _$E387
// FUNCTION: STBC 0x0055d150 _$E386
// FUNCTION: STBC 0x0055d160 _$E391
// FUNCTION: STBC 0x0055d170 _$E390
// FUNCTION: STBC 0x0055d180 _$E179
// FUNCTION: STBC 0x0055d190 _$E178
// FUNCTION: STBC 0x0055d1a0 _$E395
// FUNCTION: STBC 0x0055d1b0 _$E394
// FUNCTION: STBC 0x0055d1c0 _$E144
// FUNCTION: STBC 0x0055d1d0 _$E143
// FUNCTION: STBC 0x0055d1e0 _$E399
// FUNCTION: STBC 0x0055d1f0 _$E398
// FUNCTION: STBC 0x0055d200 _$E403
// FUNCTION: STBC 0x0055d210 _$E402
// FUNCTION: STBC 0x0055d220 _$E184
// FUNCTION: STBC 0x0055d230 _$E183
// FUNCTION: STBC 0x0055d240 _$E189
// FUNCTION: STBC 0x0055d250 _$E188
// FUNCTION: STBC 0x0055d260 _$E407
// FUNCTION: STBC 0x0055d270 _$E406
// FUNCTION: STBC 0x0055d280 _$E411
// FUNCTION: STBC 0x0055d290 _$E410
// FUNCTION: STBC 0x0055d2a0 _$E415
// FUNCTION: STBC 0x0055d2b0 _$E414
// FUNCTION: STBC 0x0055d2c0 _$E419
// FUNCTION: STBC 0x0055d2d0 _$E418
// FUNCTION: STBC 0x0055d2e0 _$E423
// FUNCTION: STBC 0x0055d2f0 _$E422
// FUNCTION: STBC 0x0055d300 _$E427
// FUNCTION: STBC 0x0055d310 _$E426
// FUNCTION: STBC 0x0055d320 _$E431
// FUNCTION: STBC 0x0055d330 _$E430
// FUNCTION: STBC 0x0055d340 _$E435
// FUNCTION: STBC 0x0055d350 _$E434
// FUNCTION: STBC 0x0055d360 _$E194
// FUNCTION: STBC 0x0055d370 _$E193
// FUNCTION: STBC 0x0055d380 _$E439
// FUNCTION: STBC 0x0055d390 _$E438
// FUNCTION: STBC 0x0055d3a0 _$E199
// FUNCTION: STBC 0x0055d3b0 _$E198
// FUNCTION: STBC 0x0055d3c0 _$E204
// FUNCTION: STBC 0x0055d3d0 _$E203
// FUNCTION: STBC 0x0055d3e0 _$E443
// FUNCTION: STBC 0x0055d3f0 _$E442
// FUNCTION: STBC 0x0055d400 _$E448
// FUNCTION: STBC 0x0055d410 _$E447
// FUNCTION: STBC 0x0055d420 _$E452
// FUNCTION: STBC 0x0055d430 _$E451

// <iostream>'s statics: the ios_base::Init and _Winit quads and the locale id pair.
// FUNCTION: STBC 0x0055ca70 _$E32
// FUNCTION: STBC 0x0055ca80 _$E29
// FUNCTION: STBC 0x0055ca90 _$E31
// FUNCTION: STBC 0x0055caa0 _$E30
// FUNCTION: STBC 0x0055cab0 _$E38
// FUNCTION: STBC 0x0055cac0 _$E35
// FUNCTION: STBC 0x0055cad0 _$E37
// FUNCTION: STBC 0x0055cae0 _$E36
// FUNCTION: STBC 0x0055cf30 _$E460
// FUNCTION: STBC 0x0055cf50 _$E459

/*
================
SubtitleAction::SubtitleAction

The text is the database's string for the key.
================
*/
// FUNCTION: STBC 0x0055c690 ??0SubtitleAction@@QAE@PAVTGLocalizationDatabase@@PADM@Z
SubtitleAction::SubtitleAction( TGLocalizationDatabase *database, char *key, float durationScale_ ) : text( "" ) {
	paragraph = NULL;
	text.Unknown6F4D90( database->GetString( key ) );
	lineID = 0;
	durationScale = durationScale_;
}

/*
================
SubtitleAction::SubtitleAction

Shows the paragraph instead of a text.
================
*/
// FUNCTION: STBC 0x0055c720 ??0SubtitleAction@@QAE@PAVTGParagraph@@M@Z
SubtitleAction::SubtitleAction( TGParagraph *paragraph_, float durationScale_ ) : text( "" ) {
	paragraph = paragraph_;
	durationScale = durationScale_;
	lineID = 0;
}

/*
================
SubtitleAction::SubtitleAction
================
*/
// FUNCTION: STBC 0x0055c790 ??0SubtitleAction@@QAE@PADM@Z
SubtitleAction::SubtitleAction( char *text_, float durationScale_ ) : text( "" ) {
	paragraph = NULL;
	text.AssignC( text_ );
	lineID = 0;
	durationScale = durationScale_;
}

/*
================
SubtitleAction::Play

A duration that is not positive leaves the action running until it is
skipped or completed from outside.
================
*/
// FUNCTION: STBC 0x0055c810
void SubtitleAction::Play( void ) {
	SubtitleWindow *window = (SubtitleWindow *)TopWindow::topWindow->FindMainWindow( MWT_SUBTITLE );
	if ( paragraph != NULL ) {
		lineID = window->AddParagraph( paragraph );
	} else {
		lineID = window->AddLine( &text );
	}
	if ( duration > 0.0f ) {
		duration = durationScale * duration;
		TGTimedAction::Play();
	}
}

/*
================
SubtitleAction::Completed
================
*/
// FUNCTION: STBC 0x0055c860
void SubtitleAction::Completed( void ) {
	SubtitleWindow *window = (SubtitleWindow *)TopWindow::topWindow->FindMainWindow( MWT_SUBTITLE );
	window->RemoveParagraph( lineID );
	TGTimedAction::Completed();
}

/*
================
SubtitleAction::Skip
================
*/
// FUNCTION: STBC 0x0055c890
void SubtitleAction::Skip( void ) {
	SubtitleWindow *window = (SubtitleWindow *)TopWindow::topWindow->FindMainWindow( MWT_SUBTITLE );
	window->RemoveParagraph( lineID );
	TGTimedAction::Skip();
}

/*
================
SubtitleAction::WriteToStream

A paragraph not shown yet is saved whole; a shown one by its ID, the
window holding it.
================
*/
// FUNCTION: STBC 0x0055c8c0
bool SubtitleAction::WriteToStream( TGStream *stream ) {
	if ( !TGTimedAction::WriteToStream( stream ) ) {
		return false;
	}
	TGObject::Log( "Saving SubtitleAction\n" );
	stream->WriteID( lineID );
	text.WriteToStream( stream );
	stream->WriteFloat( durationScale );
	if ( paragraph != NULL ) {
		stream->WriteBool( true );
		if ( lineID == 0 ) {
			paragraph->WriteToStream( stream );
		} else {
			stream->WriteID( paragraph->GetObjID() );
		}
	} else {
		stream->WriteBool( false );
	}
	return true;
}

/*
================
SubtitleAction::ReadFromStream
================
*/
// FUNCTION: STBC 0x0055c960
bool SubtitleAction::ReadFromStream( TGStream *stream ) {
	if ( !TGTimedAction::ReadFromStream( stream ) ) {
		return false;
	}
	TGObject::Log( "Loading SubtitleAction\n" );
	lineID = stream->ReadID();
	text.ReadFromStream( stream );
	durationScale = stream->ReadFloat();
	if ( stream->ReadBool() ) {
		if ( lineID == 0 ) {
			paragraph = (TGParagraph *)TGObject::LoadObject( stream, 0 );
		} else {
			paragraph = (TGParagraph *)stream->ReadID();
		}
	} else {
		paragraph = NULL;
	}
	return true;
}

/*
================
SubtitleAction::FixUpPointers

Waits for a shown paragraph's object.
================
*/
// FUNCTION: STBC 0x0055c9f0
bool SubtitleAction::FixUpPointers( void ) {
	if ( !TGTimedAction::FixUpPointers() ) {
		return false;
	}
	if ( paragraph != NULL && lineID != 0 && TGObject::GetTGObjectPtr( (long)paragraph ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
SubtitleAction::PostFixUp

A shown paragraph's ID becomes the object again; a paragraph not shown yet
was loaded whole.
================
*/
// FUNCTION: STBC 0x0055ca30
void SubtitleAction::PostFixUp( void ) {
	TGTimedAction::PostFixUp();
	if ( lineID != 0 ) {
		paragraph = (TGParagraph *)TGObject::GetSavedObject( (long)paragraph );
	} else if ( paragraph == NULL ) {
		// BUG: redundant: stores NULL into a paragraph already NULL
		paragraph = NULL;
	}
}
