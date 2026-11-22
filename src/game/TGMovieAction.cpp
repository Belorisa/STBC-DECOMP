/*
===============================================================================

	TGMovieAction.cpp

===============================================================================
*/

#include <iostream>
#include <stdio.h>
#include <string.h>
#include "TGMovieAction.h"
#include "TGMovieManager.h"
#include "TGStream.h"
#include "NiAVObject.h"
#include "NiAccumulator.h"
#include "NiDynamicEffect.h"
#include "NiExtraData.h"
#include "renderer/NiImage.h"
#include "NiObject.h"
#include "NiObjectNET.h"
#include "NiProperty.h"
#include "NiRawImageData.h"
#include "renderer/NiRender.h"
#include "NiTimeController.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006b1130 _$E220
// FUNCTION: STBC 0x006b1150 _$E219

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006b0440 _$E32
// FUNCTION: STBC 0x006b0450 _$E29
// FUNCTION: STBC 0x006b0460 _$E31
// FUNCTION: STBC 0x006b0470 _$E30
// FUNCTION: STBC 0x006b0480 _$E38
// FUNCTION: STBC 0x006b0490 _$E35
// FUNCTION: STBC 0x006b04a0 _$E37
// FUNCTION: STBC 0x006b04b0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006b04c0 _$E52
// FUNCTION: STBC 0x006b04d0 _$E51
// FUNCTION: STBC 0x006b04e0 _$E203
// FUNCTION: STBC 0x006b04f0 _$E202
// FUNCTION: STBC 0x006b0500 _$E141
// FUNCTION: STBC 0x006b0510 _$E140
// FUNCTION: STBC 0x006b0520 _$E154
// FUNCTION: STBC 0x006b0530 _$E153
// FUNCTION: STBC 0x006b0540 _$E63
// FUNCTION: STBC 0x006b0550 _$E62
// FUNCTION: STBC 0x006b0560 _$E67
// FUNCTION: STBC 0x006b0570 _$E66
// FUNCTION: STBC 0x006b0580 _$E71
// FUNCTION: STBC 0x006b0590 _$E70
// FUNCTION: STBC 0x006b05a0 _$E75
// FUNCTION: STBC 0x006b05b0 _$E74
// FUNCTION: STBC 0x006b05c0 _$E199
// FUNCTION: STBC 0x006b05d0 _$E198
// FUNCTION: STBC 0x006b05e0 _$E208
// FUNCTION: STBC 0x006b05f0 _$E207
// FUNCTION: STBC 0x006b0600 _$E213
// FUNCTION: STBC 0x006b0610 _$E212

/*
================
TGMovieAction::TGMovieAction

The script's: the movie plays centred until SetOffset moves it.
================
*/
// FUNCTION: STBC 0x006b0620
TGMovieAction::TGMovieAction( char *movie_, bool switchToMovieMode_, bool stayInMovieMode_, bool repeats_, bool skipFrames_ ) : TGAction( 0 ) {
	movie = new char[strlen( movie_ ) + 1];
	strcpy( movie, movie_ );
	stayInMovieMode = stayInMovieMode_;
	switchToMovieMode = switchToMovieMode_;
	offsetX = -1.0f;
	offsetY = -1.0f;
	repeats = repeats_;
	skipped = false;
	skipFrames = skipFrames_;
}

/*
================
TGMovieAction::~TGMovieAction
================
*/
// FUNCTION: STBC 0x006b06f0
TGMovieAction::~TGMovieAction( void ) {
	if ( movie != NULL ) {
		delete [] movie;
		movie = NULL;
	}
	ClearFrameActions();
}

/*
================
TGMovieAction::Play
================
*/
// FUNCTION: STBC 0x006b0820
void TGMovieAction::Play( void ) {
	if ( !skipped ) {
		if ( switchToMovieMode ) {
			g_kMovieManager.SwitchToMovieMode();
		}
		g_kMovieManager.AddMovie( movie, this, skipFrames );
		TGAction::Play();
	}
}

/*
================
TGMovieAction::Skip
================
*/
// FUNCTION: STBC 0x006b0860
void TGMovieAction::Skip( void ) {
	if ( !skipped ) {
		skipped = true;
		ClearFrameActions();
		g_kMovieManager.StopMovie( this );
		if ( !stayInMovieMode ) {
			g_kMovieManager.SwitchOutOfMovieMode();
		}
		TGAction::Skip();
	}
}

/*
================
TGMovieAction::Completed

A repeating movie plays again.
================
*/
// FUNCTION: STBC 0x006b08a0
void TGMovieAction::Completed( void ) {
	if ( !skipped ) {
		ClearFrameActions();
		g_kMovieManager.StopMovie( this );
		if ( repeats ) {
			Play();
			return;
		}
		if ( !stayInMovieMode ) {
			g_kMovieManager.SwitchOutOfMovieMode();
		}
		TGAction::Completed();
	}
}

/*
================
TGMovieAction::SetOffset
================
*/
// FUNCTION: STBC 0x006b08f0
void TGMovieAction::SetOffset( float x, float y ) {
	offsetX = x;
	offsetY = y;
}

/*
================
TGMovieAction::WriteToStream
================
*/
// FUNCTION: STBC 0x006b0910
bool TGMovieAction::WriteToStream( TGStream *stream ) {
	if ( !TGAction::WriteToStream( stream ) ) {
		return false;
	}

	stream->VirtualSlot11( movie );
	stream->WriteBool( stayInMovieMode );
	stream->WriteBool( switchToMovieMode );
	stream->WriteFloat( offsetX );
	stream->WriteFloat( offsetY );

	char message[256];
	sprintf( message, "Saving movie action %s\n", movie );
	TGObject::Log( message );

	stream->WriteBool( skipFrames );

	int count = frameActions.GetCount();
	MovieFrameAction **entries = frameActions.GetItems();
	stream->WriteInt( count );
	for ( int i = 0; i < count; i++ ) {
		stream->WriteID( entries[i]->actionID );
		stream->WriteInt( entries[i]->frame );
		stream->WriteBool( entries[i]->skipWhenStopped );
	}

	count = playedFrameActions.GetCount();
	entries = playedFrameActions.GetItems();
	stream->WriteInt( count );
	for ( i = 0; i < count; i++ ) {
		stream->WriteID( entries[i]->actionID );
		stream->WriteInt( entries[i]->frame );
		stream->WriteBool( entries[i]->skipWhenStopped );
	}
	return true;
}

/*
================
TGMovieAction::ReadFromStream
================
*/
// FUNCTION: STBC 0x006b0a30
bool TGMovieAction::ReadFromStream( TGStream *stream ) {
	if ( !TGAction::ReadFromStream( stream ) ) {
		return false;
	}

	movie = stream->VirtualSlot10( -1 );
	stayInMovieMode = stream->ReadBool();
	switchToMovieMode = stream->ReadBool();
	offsetX = stream->ReadFloat();
	offsetY = stream->ReadFloat();

	char message[256];
	sprintf( message, "Loading movie action %s\n", movie );
	TGObject::Log( message );

	skipFrames = stream->ReadBool();

	int count = stream->ReadInt();
	for ( int i = 0; i < count; i++ ) {
		MovieFrameAction *entry = new MovieFrameAction;
		entry->actionID = stream->ReadID();
		entry->frame = stream->ReadInt();
		entry->skipWhenStopped = stream->ReadBool();
		frameActions.Insert( entry );
	}

	count = stream->ReadInt();
	for ( i = 0; i < count; i++ ) {
		MovieFrameAction *entry = new MovieFrameAction;
		entry->actionID = stream->ReadID();
		entry->frame = stream->ReadInt();
		entry->skipWhenStopped = stream->ReadBool();
		playedFrameActions.Insert( entry );
	}
	return true;
}

/*
================
TGMovieAction::AddFrameAction
================
*/
// FUNCTION: STBC 0x006b0d10
void TGMovieAction::AddFrameAction( TGAction *action, int frame, bool skipWhenStopped ) {
	if ( action != NULL ) {
		MovieFrameAction *entry = new MovieFrameAction;
		entry->actionID = action->GetObjID();
		entry->frame = frame;
		entry->skipWhenStopped = skipWhenStopped;
		frameActions.Insert( entry );
	}
}

/*
================
TGMovieAction::DeleteFrameAction

Takes the action of the frame out of the movie's list and skips it.
================
*/
// FUNCTION: STBC 0x006b0e20
void TGMovieAction::DeleteFrameAction( int frame ) {
	frameActions.Delete( frame );
}

/*
================
TGMovieAction::PlayFrameActions

Plays the actions due by the frame, in frame order, and keeps them in the
played list.
================
*/
// FUNCTION: STBC 0x006b0ed0
void TGMovieAction::PlayFrameActions( int frame ) {
	int count = frameActions.GetCount();
	MovieFrameAction **entries = frameActions.GetItems();
	while ( count > 0 ) {
		MovieFrameAction *entry = entries[0];
		if ( entry->frame > frame ) {
			break;
		}
		TGAction *action = (TGAction *)TGObject::GetTGObjectPtr( entry->actionID );
		if ( action != NULL ) {
			action->Play();
		}
		frameActions.RemoveAt( 0 );
		playedFrameActions.Insert( entry );
		count = frameActions.GetCount();
		entries = frameActions.GetItems();
	}
}

/*
================
TGMovieAction::ClearFrameActions

Deleting an entry not played yet skips its action; a played one is
skipped when its flag says so.
================
*/
// FUNCTION: STBC 0x006b1010
void TGMovieAction::ClearFrameActions( void ) {
	frameActions.DeleteAll();
	int count = playedFrameActions.GetCount();
	MovieFrameAction **entries = playedFrameActions.GetItems();
	for ( int i = 0; i < count; i++ ) {
		MovieFrameAction *entry = entries[i];
		TGAction *action = (TGAction *)TGObject::GetTGObjectPtr( entry->actionID );
		if ( action != NULL && entry->skipWhenStopped ) {
			action->Skip();
		}
	}
	// BUG: the played entries are forgotten, not deleted
	playedFrameActions.RemoveAll();
}
