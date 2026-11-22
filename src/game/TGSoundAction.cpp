/*
===============================================================================

	TGSoundAction.cpp

===============================================================================
*/

#include <windows.h>
#include <string.h>

// <windows.h>'s ANSI/wide macro would rename TGObject's method.
#undef GetClassName

#include "TGSoundAction.h"
#include "TGActionManager.h"
#include "TGObjPtrEvent.h"
#include "TGPointerMap.h"
#include "TGSound.h"
#include "TGSoundManager.h"
#include "TGSoundRegion.h"
#include "TGStream.h"
#include <iostream>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00702600 _$E216
// FUNCTION: STBC 0x00702610 _$E213
// FUNCTION: STBC 0x00702620 _$E215
// FUNCTION: STBC 0x00702630 _$E214
// FUNCTION: STBC 0x00702640 _$E222
// FUNCTION: STBC 0x00702650 _$E219
// FUNCTION: STBC 0x00702660 _$E221
// FUNCTION: STBC 0x00702670 _$E220

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00702680 _$E28
// FUNCTION: STBC 0x00702690 _$E27
// FUNCTION: STBC 0x007026a0 _$E39
// FUNCTION: STBC 0x007026b0 _$E38
// FUNCTION: STBC 0x007026c0 _$E43
// FUNCTION: STBC 0x007026d0 _$E42
// FUNCTION: STBC 0x007026e0 _$E47
// FUNCTION: STBC 0x007026f0 _$E46
// FUNCTION: STBC 0x00702700 _$E51
// FUNCTION: STBC 0x00702710 _$E50
// FUNCTION: STBC 0x00702720 _$E175
// FUNCTION: STBC 0x00702730 _$E174
// FUNCTION: STBC 0x00702740 _$E180
// FUNCTION: STBC 0x00702750 _$E179
// FUNCTION: STBC 0x00702760 _$E184
// FUNCTION: STBC 0x00702770 _$E183
// FUNCTION: STBC 0x00702780 _$E425
// FUNCTION: STBC 0x00702790 _$E424
// FUNCTION: STBC 0x007027a0 _$E430
// FUNCTION: STBC 0x007027b0 _$E429
// FUNCTION: STBC 0x007027c0 _$E194
// FUNCTION: STBC 0x007027d0 _$E193
// FUNCTION: STBC 0x007027e0 _$E226
// FUNCTION: STBC 0x007027f0 _$E225
// FUNCTION: STBC 0x00702800 _$E117
// FUNCTION: STBC 0x00702810 _$E116
// FUNCTION: STBC 0x00702820 _$E130
// FUNCTION: STBC 0x00702830 _$E129
// FUNCTION: STBC 0x00702840 _$E231
// FUNCTION: STBC 0x00702850 _$E230
// FUNCTION: STBC 0x00702860 _$E235
// FUNCTION: STBC 0x00702870 _$E234
// FUNCTION: STBC 0x00702880 _$E242
// FUNCTION: STBC 0x00702890 _$E241
// FUNCTION: STBC 0x007028a0 _$E267
// FUNCTION: STBC 0x007028b0 _$E266
// FUNCTION: STBC 0x007028c0 _$E271
// FUNCTION: STBC 0x007028d0 _$E270
// FUNCTION: STBC 0x007028e0 _$E75
// FUNCTION: STBC 0x007028f0 _$E74
// FUNCTION: STBC 0x00702900 _$E253
// FUNCTION: STBC 0x00702910 _$E252
// FUNCTION: STBC 0x00702920 _$E260
// FUNCTION: STBC 0x00702930 _$E259
// FUNCTION: STBC 0x00702940 _$E283
// FUNCTION: STBC 0x00702950 _$E282
// FUNCTION: STBC 0x00702960 _$E287
// FUNCTION: STBC 0x00702970 _$E286
// FUNCTION: STBC 0x00702980 _$E327
// FUNCTION: STBC 0x00702990 _$E326
// FUNCTION: STBC 0x007029a0 _$E333
// FUNCTION: STBC 0x007029b0 _$E332
// FUNCTION: STBC 0x007029c0 _$E275
// FUNCTION: STBC 0x007029d0 _$E274
// FUNCTION: STBC 0x007029e0 _$E279
// FUNCTION: STBC 0x007029f0 _$E278
// FUNCTION: STBC 0x00702a00 _$E291
// FUNCTION: STBC 0x00702a10 _$E290
// FUNCTION: STBC 0x00702a20 _$E295
// FUNCTION: STBC 0x00702a30 _$E294
// FUNCTION: STBC 0x00702a40 _$E299
// FUNCTION: STBC 0x00702a50 _$E298
// FUNCTION: STBC 0x00702a60 _$E303
// FUNCTION: STBC 0x00702a70 _$E302
// FUNCTION: STBC 0x00702a80 _$E307
// FUNCTION: STBC 0x00702a90 _$E306
// FUNCTION: STBC 0x00702aa0 _$E311
// FUNCTION: STBC 0x00702ab0 _$E310
// FUNCTION: STBC 0x00702ac0 _$E315
// FUNCTION: STBC 0x00702ad0 _$E314
// FUNCTION: STBC 0x00702ae0 _$E319
// FUNCTION: STBC 0x00702af0 _$E318
// FUNCTION: STBC 0x00702b00 _$E80
// FUNCTION: STBC 0x00702b10 _$E79
// FUNCTION: STBC 0x00702b20 _$E323
// FUNCTION: STBC 0x00702b30 _$E322
// FUNCTION: STBC 0x00702b40 _$E85
// FUNCTION: STBC 0x00702b50 _$E84
// FUNCTION: STBC 0x00702b60 _$E337
// FUNCTION: STBC 0x00702b70 _$E336
// FUNCTION: STBC 0x00702b80 _$E341
// FUNCTION: STBC 0x00702b90 _$E340
// FUNCTION: STBC 0x00702ba0 _$E345
// FUNCTION: STBC 0x00702bb0 _$E344
// FUNCTION: STBC 0x00702bc0 _$E349
// FUNCTION: STBC 0x00702bd0 _$E348
// FUNCTION: STBC 0x00702be0 _$E94
// FUNCTION: STBC 0x00702bf0 _$E93
// FUNCTION: STBC 0x00702c00 _$E353
// FUNCTION: STBC 0x00702c10 _$E352
// FUNCTION: STBC 0x00702c20 _$E357
// FUNCTION: STBC 0x00702c30 _$E356
// FUNCTION: STBC 0x00702c40 _$E361
// FUNCTION: STBC 0x00702c50 _$E360
// FUNCTION: STBC 0x00702c60 _$E100
// FUNCTION: STBC 0x00702c70 _$E99
// FUNCTION: STBC 0x00702c80 _$E105
// FUNCTION: STBC 0x00702c90 _$E104
// FUNCTION: STBC 0x00702ca0 _$E135
// FUNCTION: STBC 0x00702cb0 _$E134
// FUNCTION: STBC 0x00702cc0 _$E365
// FUNCTION: STBC 0x00702cd0 _$E364
// FUNCTION: STBC 0x00702ce0 _$E140
// FUNCTION: STBC 0x00702cf0 _$E139
// FUNCTION: STBC 0x00702d00 _$E369
// FUNCTION: STBC 0x00702d10 _$E368
// FUNCTION: STBC 0x00702d20 _$E373
// FUNCTION: STBC 0x00702d30 _$E372
// FUNCTION: STBC 0x00702d40 _$E145
// FUNCTION: STBC 0x00702d50 _$E144
// FUNCTION: STBC 0x00702d60 _$E377
// FUNCTION: STBC 0x00702d70 _$E376
// FUNCTION: STBC 0x00702d80 _$E110
// FUNCTION: STBC 0x00702d90 _$E109
// FUNCTION: STBC 0x00702da0 _$E381
// FUNCTION: STBC 0x00702db0 _$E380
// FUNCTION: STBC 0x00702dc0 _$E385
// FUNCTION: STBC 0x00702dd0 _$E384
// FUNCTION: STBC 0x00702de0 _$E150
// FUNCTION: STBC 0x00702df0 _$E149
// FUNCTION: STBC 0x00702e00 _$E155
// FUNCTION: STBC 0x00702e10 _$E154
// FUNCTION: STBC 0x00702e20 _$E389
// FUNCTION: STBC 0x00702e30 _$E388
// FUNCTION: STBC 0x00702e40 _$E393
// FUNCTION: STBC 0x00702e50 _$E392
// FUNCTION: STBC 0x00702e60 _$E397
// FUNCTION: STBC 0x00702e70 _$E396
// FUNCTION: STBC 0x00702e80 _$E401
// FUNCTION: STBC 0x00702e90 _$E400
// FUNCTION: STBC 0x00702ea0 _$E405
// FUNCTION: STBC 0x00702eb0 _$E404
// FUNCTION: STBC 0x00702ec0 _$E409
// FUNCTION: STBC 0x00702ed0 _$E408
// FUNCTION: STBC 0x00702ee0 _$E413
// FUNCTION: STBC 0x00702ef0 _$E412
// FUNCTION: STBC 0x00702f00 _$E417
// FUNCTION: STBC 0x00702f10 _$E416
// FUNCTION: STBC 0x00702f20 _$E160
// FUNCTION: STBC 0x00702f30 _$E159
// FUNCTION: STBC 0x00702f40 _$E421
// FUNCTION: STBC 0x00702f50 _$E420
// FUNCTION: STBC 0x00702f60 _$E165
// FUNCTION: STBC 0x00702f70 _$E164
// FUNCTION: STBC 0x00702f80 _$E170
// FUNCTION: STBC 0x00702f90 _$E169

// The unit's static initialisers and exit-time registrations (<iostream> and header statics).

// FUNCTION: STBC 0x00703620 _$E438
// FUNCTION: STBC 0x00703640 _$E437

/*
================
TGSoundAction::TGSoundAction

A looping sound always stops with the action.
================
*/
// FUNCTION: STBC 0x00702fa0
TGSoundAction::TGSoundAction( char *sound_, int flags_, char *region_, float volume_ ) {
	sound = new char[strlen( sound_ ) + 1];
	strcpy( sound, sound_ );
	volume = volume_;
	region = NULL;
	if ( region_ != NULL ) {
		region = new char[strlen( region_ ) + 1];
		strcpy( region, region_ );
	}
	playingID = unknown896A9C;
	if ( ( flags_ & TGSAF_ALLOW_SOUND_TO_CONTINUE_PLAYING_WHEN_STOPPED ) && ( flags_ & TGSAF_LOOPING ) ) {
		flags_ &= ~TGSAF_ALLOW_SOUND_TO_CONTINUE_PLAYING_WHEN_STOPPED;
	}
	flags = flags_;
	usePriority = false;
	priority = 0.5f;
	node = NULL;
	savedNode = 0;
}

/*
================
TGSoundAction::~TGSoundAction
================
*/
// FUNCTION: STBC 0x007030c0
TGSoundAction::~TGSoundAction( void ) {
	delete [] sound;
	delete [] region;
}

/*
================
TGSoundAction::Play

Plays the sound with the action's settings; its end sends the action
manager the action's completion event. A sound the manager does not know
completes the action at once.
================
*/
// FUNCTION: STBC 0x00703140
void TGSoundAction::Play( void ) {
	if ( playingID != unknown896A9C ) {
		g_kSoundManager.StopSound( playingID );
		playingID = unknown896A9C;
	}

	TGSound *playing = g_kSoundManager.GetSound( sound );
	if ( playing == NULL ) {
		Completed();
		return;
	}

	TGObjPtrEvent *event = new TGObjPtrEvent;
	event->SetEventType( ET_ACTION_COMPLETED );
	event->SetDestination( &g_kTGActionManager );
	event->SetObjPtr( this );

	if ( region != NULL ) {
		TGSoundRegion *soundRegion = TGSoundRegion::GetRegion( region );
		if ( soundRegion != NULL ) {
			soundRegion->AddSound( playing );
		}
	}

	bool restart = ( flags & TGSAF_RESTART ) != 0;
	bool allowDuplicates = ( flags & TGSAF_ALLOW_DUPLICATES ) != 0;
	playing->SetVolume( volume );
	playing->SetLooping( ( flags & TGSAF_LOOPING ) != 0 );
	playing->SetVoice( ( flags & TGSAF_VOICE ) != 0 );
	if ( usePriority ) {
		playing->SetPriority( priority );
	}
	if ( node != NULL ) {
		playing->AttachToNode( node );
	}
	if ( !allowDuplicates ) {
		g_kSoundManager.StopSound( sound );
	}
	playingID = playing->PlayAndNotify( event, restart );

	TGAction::Play();
}

/*
================
TGSoundAction::Restart

As Play, from delay seconds into the sound; a sound the manager does not
know does nothing.
================
*/
// FUNCTION: STBC 0x00703270
void TGSoundAction::Restart( float delay ) {
	if ( playingID != unknown896A9C ) {
		g_kSoundManager.StopSound( playingID );
		playingID = unknown896A9C;
	}

	TGSound *playing = g_kSoundManager.GetSound( sound );
	if ( playing == NULL ) {
		return;
	}

	TGObjPtrEvent *event = new TGObjPtrEvent;
	event->SetEventType( ET_ACTION_COMPLETED );
	event->SetDestination( &g_kTGActionManager );
	event->SetObjPtr( this );

	if ( region != NULL ) {
		TGSoundRegion *soundRegion = TGSoundRegion::GetRegion( region );
		if ( soundRegion != NULL ) {
			soundRegion->AddSound( playing );
		}
	}

	bool restart = ( flags & TGSAF_RESTART ) != 0;
	bool allowDuplicates = ( flags & TGSAF_ALLOW_DUPLICATES ) != 0;
	playing->SetVolume( volume );
	playing->SetLooping( ( flags & TGSAF_LOOPING ) != 0 );
	if ( usePriority ) {
		playing->SetPriority( priority );
	}
	if ( node != NULL ) {
		playing->AttachToNode( node );
	}
	if ( !allowDuplicates ) {
		g_kSoundManager.StopSound( sound );
	}
	playingID = playing->PlayAndNotify( event, restart );
	TGSound *started = g_kSoundManager.GetPlayingSound( playingID );
	started->SetPlayPositionSeconds( delay );

	TGAction::Restart( delay );
}

/*
================
TGSoundAction::Completed

Stops the sound unless it is allowed to play on; its end event is
dropped first.
================
*/
// FUNCTION: STBC 0x007033b0
void TGSoundAction::Completed( void ) {
	if ( playingID != unknown896A9C && !( flags & TGSAF_ALLOW_SOUND_TO_CONTINUE_PLAYING_WHEN_STOPPED ) ) {
		TGSound *playing = g_kSoundManager.GetPlayingSound( playingID );
		if ( playing != NULL ) {
			playing->SetEndEvent( NULL );
		}
		g_kSoundManager.StopSound( playingID );
		playingID = unknown896A9C;
	}
	TGAction::Completed();
}

/*
================
TGSoundAction::ReleaseCompletedEvents

The playing sound's end event goes with the others.
================
*/
// FUNCTION: STBC 0x00703400
void TGSoundAction::ReleaseCompletedEvents( void ) {
	if ( playingID != unknown896A9C ) {
		TGSound *playing = g_kSoundManager.GetPlayingSound( playingID );
		if ( playing != NULL ) {
			playing->SetEndEvent( NULL );
		}
	}
	TGAction::ReleaseCompletedEvents();
}

/*
================
TGSoundAction::Skip

Stops the sound; the action does not complete.
================
*/
// FUNCTION: STBC 0x00703440
void TGSoundAction::Skip( void ) {
	g_kSoundManager.StopSound( playingID );
	playingID = unknown896A9C;
}

/*
================
TGSoundAction::WriteToStream
================
*/
// FUNCTION: STBC 0x00703460
bool TGSoundAction::WriteToStream( TGStream *stream ) {
	if ( !TGAction::WriteToStream( stream ) ) {
		return false;
	}
	stream->VirtualSlot11( sound );
	stream->VirtualSlot11( region );
	stream->WriteLong( flags );
	stream->WriteID( playingID );
	stream->WriteFloat( volume );
	stream->WriteBool( usePriority );
	stream->WriteFloat( priority );
	stream->VirtualSlot17( (long)(NiNode *)node );
	return true;
}

/*
================
TGSoundAction::ReadFromStream
================
*/
// FUNCTION: STBC 0x007034e0
bool TGSoundAction::ReadFromStream( TGStream *stream ) {
	if ( !TGAction::ReadFromStream( stream ) ) {
		return false;
	}
	sound = stream->VirtualSlot10( -1 );
	region = stream->VirtualSlot10( -1 );
	flags = stream->ReadLong();
	playingID = stream->ReadID();
	volume = stream->ReadFloat();
	usePriority = stream->ReadBool();
	priority = stream->ReadFloat();
	savedNode = stream->VirtualSlot16();
	return true;
}

/*
================
TGSoundAction::FixUpPointers

A node that was not loaded is left out.
================
*/
// FUNCTION: STBC 0x00703560
bool TGSoundAction::FixUpPointers( void ) {
	return TGAction::FixUpPointers() ? true : false;
}

/*
================
TGSoundAction::PostFixUp
================
*/
// FUNCTION: STBC 0x00703570
void TGSoundAction::PostFixUp( void ) {
	TGAction::PostFixUp();
	if ( savedNode != 0 ) {
		node = (NiNode *)pointerMap.Find( savedNode );
	}
	savedNode = 0;
}
