/*
===============================================================================

	TGSound.cpp

===============================================================================
*/

#include <string.h>

#include "NiMain.h"
#include "NiSoundSystem.h"
#include "TGConfigMapping.h"
#include "TGEvent.h"
#include "TGEventManager.h"
#include "TGPointerMap.h"
#include "TGMilesImports.h"
#include "TGSound.h"
#include "TGSoundFile.h"
#include "TGSoundManager.h"
#include "TGSoundRegion.h"
#include "TGStream.h"
#include "TGStringEvent.h"
#include <iostream>
#include "NiListener.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0070d550 _$E447
// FUNCTION: STBC 0x0070d570 _$E446

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0070cf50 _$E21
// FUNCTION: STBC 0x0070cf60 _$E20
// FUNCTION: STBC 0x0070cfb0 _$E36
// FUNCTION: STBC 0x0070cfc0 _$E35
// FUNCTION: STBC 0x0070cfd0 _$E40
// FUNCTION: STBC 0x0070cfe0 _$E39
// FUNCTION: STBC 0x0070d230 _$E44
// FUNCTION: STBC 0x0070d240 _$E43
// FUNCTION: STBC 0x0070d250 _$E48
// FUNCTION: STBC 0x0070d260 _$E47
// FUNCTION: STBC 0x0070d270 _$E172
// FUNCTION: STBC 0x0070d280 _$E171
// FUNCTION: STBC 0x0070d290 _$E433
// FUNCTION: STBC 0x0070d2a0 _$E432
// FUNCTION: STBC 0x0070d2b0 _$E381
// FUNCTION: STBC 0x0070d2c0 _$E380
// FUNCTION: STBC 0x0070d2d0 _$E413
// FUNCTION: STBC 0x0070d2e0 _$E412
// FUNCTION: STBC 0x0070d2f0 _$E177
// FUNCTION: STBC 0x0070d300 _$E176
// FUNCTION: STBC 0x0070d310 _$E186
// FUNCTION: STBC 0x0070d320 _$E185
// FUNCTION: STBC 0x0070d330 _$E25
// FUNCTION: STBC 0x0070d340 _$E24
// FUNCTION: STBC 0x0070d350 _$E114
// FUNCTION: STBC 0x0070d360 _$E113
// FUNCTION: STBC 0x0070d410 _$E127
// FUNCTION: STBC 0x0070d420 _$E126
// FUNCTION: STBC 0x0070d4f0 _$E182
// FUNCTION: STBC 0x0070d500 _$E181
// FUNCTION: STBC 0x0070d510 _$E190
// FUNCTION: STBC 0x0070d520 _$E189
// FUNCTION: STBC 0x0070d530 _$E197
// FUNCTION: STBC 0x0070d540 _$E196
// FUNCTION: STBC 0x0070d5d0 _$E222
// FUNCTION: STBC 0x0070d5e0 _$E221
// FUNCTION: STBC 0x0070d5f0 _$E226
// FUNCTION: STBC 0x0070d600 _$E225
// FUNCTION: STBC 0x0070d610 _$E72
// FUNCTION: STBC 0x0070d620 _$E71
// FUNCTION: STBC 0x0070d630 _$E208
// FUNCTION: STBC 0x0070d640 _$E207
// FUNCTION: STBC 0x0070d650 _$E215
// FUNCTION: STBC 0x0070d660 _$E214
// FUNCTION: STBC 0x0070d670 _$E238
// FUNCTION: STBC 0x0070d680 _$E237
// FUNCTION: STBC 0x0070d690 _$E242
// FUNCTION: STBC 0x0070d6a0 _$E241
// FUNCTION: STBC 0x0070d6b0 _$E282
// FUNCTION: STBC 0x0070d6c0 _$E281
// FUNCTION: STBC 0x0070d6d0 _$E288
// FUNCTION: STBC 0x0070d6e0 _$E287
// FUNCTION: STBC 0x0070d6f0 _$E230
// FUNCTION: STBC 0x0070d700 _$E229
// FUNCTION: STBC 0x0070d710 _$E234
// FUNCTION: STBC 0x0070d720 _$E233
// FUNCTION: STBC 0x0070d730 _$E246
// FUNCTION: STBC 0x0070d740 _$E245
// FUNCTION: STBC 0x0070d750 _$E250
// FUNCTION: STBC 0x0070d760 _$E249
// FUNCTION: STBC 0x0070d770 _$E254
// FUNCTION: STBC 0x0070d780 _$E253
// FUNCTION: STBC 0x0070d790 _$E258
// FUNCTION: STBC 0x0070d7a0 _$E257
// FUNCTION: STBC 0x0070d7b0 _$E262
// FUNCTION: STBC 0x0070d7c0 _$E261
// FUNCTION: STBC 0x0070d7d0 _$E266
// FUNCTION: STBC 0x0070d7e0 _$E265
// FUNCTION: STBC 0x0070d7f0 _$E270
// FUNCTION: STBC 0x0070d800 _$E269
// FUNCTION: STBC 0x0070d810 _$E274
// FUNCTION: STBC 0x0070d820 _$E273
// FUNCTION: STBC 0x0070d830 _$E77
// FUNCTION: STBC 0x0070d840 _$E76
// FUNCTION: STBC 0x0070d850 _$E278
// FUNCTION: STBC 0x0070d860 _$E277
// FUNCTION: STBC 0x0070d870 _$E82
// FUNCTION: STBC 0x0070d880 _$E81
// FUNCTION: STBC 0x0070d890 _$E292
// FUNCTION: STBC 0x0070d8a0 _$E291
// FUNCTION: STBC 0x0070d8b0 _$E296
// FUNCTION: STBC 0x0070d8c0 _$E295
// FUNCTION: STBC 0x0070d8d0 _$E300
// FUNCTION: STBC 0x0070d8e0 _$E299
// FUNCTION: STBC 0x0070d8f0 _$E304
// FUNCTION: STBC 0x0070d900 _$E303
// FUNCTION: STBC 0x0070d910 _$E91
// FUNCTION: STBC 0x0070d920 _$E90
// FUNCTION: STBC 0x0070d930 _$E308
// FUNCTION: STBC 0x0070d940 _$E307
// FUNCTION: STBC 0x0070d950 _$E312
// FUNCTION: STBC 0x0070d960 _$E311
// FUNCTION: STBC 0x0070d970 _$E316
// FUNCTION: STBC 0x0070d980 _$E315
// FUNCTION: STBC 0x0070d990 _$E97
// FUNCTION: STBC 0x0070d9a0 _$E96
// FUNCTION: STBC 0x0070d9b0 _$E102
// FUNCTION: STBC 0x0070d9c0 _$E101
// FUNCTION: STBC 0x0070d9d0 _$E132
// FUNCTION: STBC 0x0070d9e0 _$E131
// FUNCTION: STBC 0x0070d9f0 _$E320
// FUNCTION: STBC 0x0070da00 _$E319
// FUNCTION: STBC 0x0070da10 _$E137
// FUNCTION: STBC 0x0070da20 _$E136
// FUNCTION: STBC 0x0070da30 _$E324
// FUNCTION: STBC 0x0070da40 _$E323
// FUNCTION: STBC 0x0070da50 _$E328
// FUNCTION: STBC 0x0070da60 _$E327
// FUNCTION: STBC 0x0070da70 _$E142
// FUNCTION: STBC 0x0070da80 _$E141
// FUNCTION: STBC 0x0070da90 _$E332
// FUNCTION: STBC 0x0070daa0 _$E331
// FUNCTION: STBC 0x0070dab0 _$E107
// FUNCTION: STBC 0x0070dac0 _$E106
// FUNCTION: STBC 0x0070dad0 _$E336
// FUNCTION: STBC 0x0070dae0 _$E335
// FUNCTION: STBC 0x0070daf0 _$E340
// FUNCTION: STBC 0x0070db00 _$E339
// FUNCTION: STBC 0x0070db10 _$E147
// FUNCTION: STBC 0x0070db20 _$E146
// FUNCTION: STBC 0x0070db30 _$E152
// FUNCTION: STBC 0x0070db40 _$E151
// FUNCTION: STBC 0x0070db50 _$E344
// FUNCTION: STBC 0x0070db60 _$E343
// FUNCTION: STBC 0x0070db70 _$E348
// FUNCTION: STBC 0x0070db80 _$E347
// FUNCTION: STBC 0x0070db90 _$E352
// FUNCTION: STBC 0x0070dba0 _$E351
// FUNCTION: STBC 0x0070dbb0 _$E356
// FUNCTION: STBC 0x0070dbc0 _$E355
// FUNCTION: STBC 0x0070dbd0 _$E360
// FUNCTION: STBC 0x0070dbe0 _$E359
// FUNCTION: STBC 0x0070dbf0 _$E364
// FUNCTION: STBC 0x0070dc00 _$E363
// FUNCTION: STBC 0x0070dc10 _$E368
// FUNCTION: STBC 0x0070dc20 _$E367
// FUNCTION: STBC 0x0070dc30 _$E372
// FUNCTION: STBC 0x0070dc40 _$E371
// FUNCTION: STBC 0x0070dc50 _$E157
// FUNCTION: STBC 0x0070dc60 _$E156
// FUNCTION: STBC 0x0070dc70 _$E376
// FUNCTION: STBC 0x0070dc80 _$E375
// FUNCTION: STBC 0x0070dc90 _$E162
// FUNCTION: STBC 0x0070dca0 _$E161
// FUNCTION: STBC 0x0070dcb0 _$E167
// FUNCTION: STBC 0x0070dcc0 _$E166

// The unit's static initialisers and exit-time registrations (<iostream> and header statics).
// FUNCTION: STBC 0x0070ceb0 _$E423
// FUNCTION: STBC 0x0070cec0 _$E420
// FUNCTION: STBC 0x0070ced0 _$E422
// FUNCTION: STBC 0x0070cee0 _$E421
// FUNCTION: STBC 0x0070cef0 _$E429
// FUNCTION: STBC 0x0070cf00 _$E426
// FUNCTION: STBC 0x0070cf10 _$E428
// FUNCTION: STBC 0x0070cf20 _$E427

// The file cache's dynamic initialiser and exit-time destructor (a class static: guarded).
// FUNCTION: STBC 0x0070afe0 _$E439
// FUNCTION: STBC 0x0070aff0 _$E435
// FUNCTION: STBC 0x0070b000 _$E438
// FUNCTION: STBC 0x0070b010 _$E436

// The memory a built sound adds to the rough memory usage.
#define ROUGH_MEMORY_PER_SOUND	0x198

// The options file's section and the size of a stream's buffer in it.
#define SOUND_SECTION			"Sound"

// DATA: STBC 0x0099b95c
int TGSound::numAllocated;

// DATA: STBC 0x0099b960
int TGSound::roughMemoryUsage;

/*
================
TGSound::TGSound
================
*/
// FUNCTION: STBC 0x0070b030 ??0TGSound@@QAE@PAD0J@Z
TGSound::TGSound( char *fileName_, char *name_, long flags ) :
	source( NULL ),
	node( NULL ),
	fadePoints( 2 ) {
	silenced = false;
	endEventID = 0;
	region = NULL;
	savedPlayPosition = -1.0f;
	Init( fileName_, name_, flags, false );
}

/*
================
TGSound::TGSound

A copy plays: the filed sound's settings are given to it.
================
*/
// FUNCTION: STBC 0x0070b0c0 ??0TGSound@@QAE@PAV0@@Z
// PARKED: register allocation: retail keeps the zero in ebp and the copied fade point in ebx (ours swaps them; ebx carries a 400 penalty in c2trace --regs), and its byte copies go through al
TGSound::TGSound( TGSound *original ) :
	source( NULL ),
	node( NULL ),
	fadePoints( 2 ) {
	savedPlayPosition = -1.0f;
	endEventID = 0;
	region = NULL;

	long flags = original->Is3D() ? LS_3D : 0;
	flags |= original->IsStreamed() ? LS_STREAMED : 0;
	silenced = original->silenced;
	unknown51 = original->unknown51;
	Init( original->fileName, original->name, flags, true );
	SetGroup( original->group );
	sfx = original->sfx;
	voice = original->voice;
	interfaceSound = original->interfaceSound;
	if ( original->GetParentNode() != NULL ) {
		AttachToNode( original->GetParentNode() );
	}

	{
		float inner, outer, outerVolume;
		original->GetConeData( &inner, &outer, &outerVolume );
		SetConeData( inner, outer, outerVolume );
	}
	{
		float minimum, maximum;
		original->GetMinMaxDistance( &minimum, &maximum );
		SetMinMaxDistance( minimum, maximum );
	}
	looping = original->looping;
	SetVolume( original->GetVolume() );
	priority = original->priority;
	SetPitch( original->GetPitch() );
	SetPlayPositionSeconds( original->GetPlayPositionSeconds() );

	NiTListIterator pos = original->fadePoints.GetHeadPos();
	while ( pos != NULL ) {
		TGFadePoint *fade = original->fadePoints.GetNext( pos );
		fadePoints.AddTail( new TGFadePoint( *fade ) );
	}

	singleShot = original->IsSingleShot();
	NiPoint3 forward, up;
	original->GetOrientation( &forward, &up );
	// the address of GetPosition's returned temporary (retail passes the returned pointer on)
	SetPosition( &original->GetPosition() );
	SetOrientation( &forward, &up );
	SetEndEvent( original->GetEndEvent() );
	if ( original->region != NULL ) {
		original->region->AddSound( this );
	}
}

/*
================
TGSound::Init

Copies the names, sets the members and, for a sound that is not a copy,
files it with the manager. The sound is loaded unless it is to wait.
================
*/
// FUNCTION: STBC 0x0070b360 ?Init@TGSound@@IAEXPAD0K_N@Z
void TGSound::Init( char *fileName_, char *name_, unsigned long flags, bool copy ) {
	unknown11 = copy;
	group = NULL;
	loaded = false;
	unknown10 = false;
	looping = false;
	streamed = ( flags >> 1 ) & 1 ? true : false;
	is3D = flags & LS_3D;
	source = NULL;
	sample = NULL;
	node = new NiNode;
	facing = NiPoint3::ZERO;
	upDirection = NiPoint3::ZERO;
	innerConeAngle = 360.0f;
	outerConeAngle = 360.0f;
	hasPlayPosition = false;
	outerConeVolume = 1.0f;
	pitch = 1.0f;
	playPosition = 0.0f;
	minDistance = 50.0f;
	maxDistance = 700.0f;
	volume = 1.0f;
	priority = 0.5f;
	fileName = NULL;
	name = NULL;
	file = NULL;
	id = unknown896A9C;
	endEventID = 0;
	region = NULL;
	unknown51 = false;
	paused = false;
	singleShot = false;
	sfx = true;
	voice = false;
	interfaceSound = false;
	numAllocated++;
	roughMemoryUsage += ROUGH_MEMORY_PER_SOUND;

	fileName = new char[strlen( fileName_ ) + 1];
	strcpy( fileName, fileName_ );
	name = new char[strlen( name_ ) + 1];
	strcpy( name, name_ );

	if ( !( flags & LS_DELAY_LOADING ) ) {
		loaded = Load();
	}
	if ( !unknown11 ) {
		g_kSoundManager.AddSound( this );
	}
}

/*
================
TGSound::~TGSound
================
*/
// FUNCTION: STBC 0x0070b590 ??1TGSound@@QAE@XZ
TGSound::~TGSound( void ) {
	if ( id != unknown896A9C ) {
		g_kSoundManager.StopSound( id );
	}
	numAllocated--;
	Unload();
	if ( region != NULL ) {
		region->RemoveSound( this );
	}
	SetGroup( NULL );
	if ( GetParentNode() != NULL ) {
		DetachFromNode();
	}
	if ( !unknown11 ) {
		g_kSoundManager.RemoveSound( this );
	}
	SetEndEvent( NULL );
	node = NULL;
	if ( file != NULL ) {
		TGSoundFile::Release( fileName, file );
		file = NULL;
	}
	if ( fileName != NULL ) {
		delete fileName;
		fileName = NULL;
	}
	if ( name != NULL ) {
		delete name;
		name = NULL;
	}
}

/*
================
TGSound::Load

A copy takes a source: the sample of a 3D sound is one the manager
reserved, a stream is opened from its file. A sound that fails to load is
not tried again.
================
*/
// FUNCTION: STBC 0x0070b730 ?Load@TGSound@@QAE_NXZ
// PARKED: register picks only: retail loads the three file arguments of LoadSample/Load3DSample into registers (the image in edi) before pushing them, and the source for RemoveSource into eax
bool TGSound::Load( void ) {
	if ( !g_kSoundManager.IsEnabled() ) {
		return false;
	}
	if ( silenced && unknown11 ) {
		return false;
	}
	if ( loaded || unknown10 ) {
		return loaded;
	}

	loaded = true;
	if ( !streamed ) {
		file = TGSoundFile::Get( fileName, true );
		if ( file == NULL ) {
			loaded = false;
		}
	}
	if ( loaded && unknown11 ) {
		source = new NiSource;
		if ( streamed ) {
			int bufferSize = g_kConfigMapping.GetIntValue( SOUND_SECTION, "StreamBufferSize" );
			loaded = source->LoadStream( fileName, bufferSize );
		} else if ( Is3D() ) {
			NiProvider_Info *provider = NULL;
			g_kSoundManager.GetSample( &sample, &provider );
			if ( sample != NULL ) {
				loaded = source->Load3DSample( file->size, file->image, provider, sample );
			} else {
				loaded = false;
			}
		} else {
			loaded = source->LoadSample( file->size, file->image, file->fileName );
		}
		if ( !loaded ) {
			NiSoundSystem::GetSoundSystem()->RemoveSource( source );
			source = NULL;
			if ( sample != NULL ) {
				g_kSoundManager.ReleaseSample( sample );
				sample = NULL;
			}
		}
	}
	unknown10 = !loaded;
	return loaded;
}

/*
================
TGSound::Unload
================
*/
// FUNCTION: STBC 0x0070b930 ?Unload@TGSound@@QAEXXZ
void TGSound::Unload( void ) {
	if ( loaded ) {
		roughMemoryUsage -= ROUGH_MEMORY_PER_SOUND;
		if ( source != NULL ) {
			source->Unload();
		}
		if ( sample != NULL ) {
			g_kSoundManager.ReleaseSample( sample );
			sample = NULL;
		}
		if ( source != NULL ) {
			NiSoundSystem::GetSoundSystem()->RemoveSource( source );
		}
		source = NULL;
		loaded = false;
	}
}

/*
================
TGSound::SetGroup

The sound keeps a copy of the group's name; NULL puts it in none.
================
*/
// FUNCTION: STBC 0x0070b9b0 ?SetGroup@TGSound@@QAEXPAD@Z
void TGSound::SetGroup( char *group_ ) {
	if ( group != NULL ) {
		delete group;
		group = NULL;
	}
	if ( group_ != NULL ) {
		group = new char[strlen( group_ ) + 1];
		strcpy( group, group_ );
	}
}

/*
================
TGSound::Play

The sound plays as a copy; the ID of the copy comes back.
================
*/
// FUNCTION: STBC 0x0070ba10 ?Play@TGSound@@QAEH_N@Z
int TGSound::Play( bool rewind ) {
	return PlayAndNotify( NULL, rewind );
}

/*
================
TGSound::CanPlay

Only a filed sound plays, and only once it is loaded (loading it if need
be): a copy is the played one. Nothing plays while the manager is down.
================
*/
// FUNCTION: STBC 0x0070ba20 ?CanPlay@TGSound@@IAE_NXZ
bool TGSound::CanPlay( void ) {
	bool cannot = false;
	if ( !g_kSoundManager.IsEnabled() ) {
		cannot = true;
	}
	if ( unknown11 ) {
		cannot = true;
	} else if ( !cannot && !loaded && !Load() ) {
		cannot = true;
	}
	return !cannot;
}

/*
================
TGSound::PlayAndNotify

Plays a copy of the sound; the event, if any, is sent when the copy ends.
When the copy cannot play, the event is sent at once.
================
*/
// FUNCTION: STBC 0x0070ba60 ?PlayAndNotify@TGSound@@QAEHPAVTGEvent@@_N@Z
// PARKED: retail lays the failure block out before the success block; every spelling tried keeps it last
int TGSound::PlayAndNotify( TGEvent *event, bool rewind ) {
	long playingID = unknown896A9C;
	bool playable = CanPlay();
	if ( playable && ( playingID = g_kSoundManager.AllocateSoundID( this ) ) == unknown896A9C ) {
		playable = false;
	}

	TGSound *copy = NULL;
	if ( playable ) {
		copy = new TGSound( this );
	}
	if ( copy == NULL || !copy->loaded ) {
		if ( event != NULL ) {
			g_kEventManager.AddEvent( event );
		}
		if ( copy != NULL ) {
			delete copy;
		}
		if ( unknown11 ) {
			delete this;
		}
		return unknown896A9C;
	} else {
		copy->SetEndEvent( event );
		g_kSoundManager.SetPlayingSound( copy, playingID );
		if ( !copy->Unknown70BB80( rewind ) ) {
			copy->Stop();
			playingID = unknown896A9C;
		}
		return playingID;
	}
}

/*
================
TGSound::Unknown70BB80

Starts the sound's source, from the start when asked to. The source loops
when the sound does and is not streamed. While the manager pauses the
sounds, a sound that pauses with them is only marked paused.
================
*/
// FUNCTION: STBC 0x0070bb80 ?Unknown70BB80@TGSound@@QAE_N_N@Z
bool TGSound::Unknown70BB80( bool rewind ) {
	if ( rewind ) {
		Rewind();
	}
	if ( !looping || streamed ) {
		source->SetLoop( false );
	} else {
		source->SetLoop( true );
	}
	Update();
	if ( !g_kSoundManager.paused || ( !sfx && !voice ) ) {
		if ( !source->Play() ) {
			return false;
		}
	} else {
		paused = true;
	}
	return true;
}

/*
================
TGSound::Stop

A copy is unloaded when it stops. The end event is sent, and a sound that
was to be deleted when it stopped is deleted by the event the manager gets.
================
*/
// FUNCTION: STBC 0x0070bbf0 ?Stop@TGSound@@QAE_NXZ
bool TGSound::Stop( void ) {
	if ( GetStatus() == SS_PLAYING ) {
		source->Stop();
	}
	if ( unknown11 ) {
		Unload();
	}
	if ( endEventID != 0 ) {
		g_kEventManager.AddEvent( TGEvent::GetTGEventPtr( endEventID ) );
		endEventID = 0;
	}
	g_kSoundManager.ReleaseSoundID( this );
	if ( singleShot ) {
		TGStringEvent *event = new TGStringEvent;
		event->SetEventType( ET_DELETE_SOUND );
		event->SetString( name );
		event->SetDestination( &g_kSoundManager );
		g_kEventManager.AddEvent( event );
	}
	return true;
}

/*
================
TGSound::Rewind
================
*/
// FUNCTION: STBC 0x0070bcb0 ?Rewind@TGSound@@QAE_NXZ
bool TGSound::Rewind( void ) {
	if ( source != NULL ) {
		source->Rewind();
	}
	return true;
}

/*
================
TGSound::Pause
================
*/
// FUNCTION: STBC 0x0070bcc0 ?Pause@TGSound@@QAEXXZ
void TGSound::Pause( void ) {
	if ( unknown11 && source != NULL ) {
		paused = true;
		source->Stop();
	}
}

/*
================
TGSound::Unpause
================
*/
// FUNCTION: STBC 0x0070bce0 ?Unpause@TGSound@@QAEXXZ
void TGSound::Unpause( void ) {
	if ( unknown11 && source != NULL ) {
		paused = false;
		source->Resume();
	}
}

/*
================
TGSound::GetStatus
================
*/
// FUNCTION: STBC 0x0070bd00 ?GetStatus@TGSound@@QAEHXZ
int TGSound::GetStatus( void ) {
	if ( !loaded ) {
		return SS_UNLOADED;
	}
	if ( !unknown11 ) {
		return SS_STOPPED;
	}
	switch ( source->GetStatus() ) {
	case NiSource::STATUS_PLAYING:
	case NiSource::STATUS_LOOPING:
	case NiSource::STATUS_2:
		return SS_PLAYING;
	case NiSource::STATUS_DONE:
		return SS_STOPPED;
	}
	return SS_UNKNOWN;
}

/*
================
TGSound::Is3D
================
*/
// FUNCTION: STBC 0x0070bd40 ?Is3D@TGSound@@QAE_NXZ
bool TGSound::Is3D( void ) {
	return is3D;
}

/*
================
TGSound::AttachToNode

The sound's own node becomes a child of the node; it leaves its old parent.
================
*/
// FUNCTION: STBC 0x0070bd50 ?AttachToNode@TGSound@@QAE_NPAVNiNode@@@Z
bool TGSound::AttachToNode( NiNode *parent ) {
	if ( node != NULL ) {
		if ( GetParentNode() != NULL ) {
			DetachFromNode();
		}
		parent->AttachChild( node, false );
	}
	return true;
}

/*
================
TGSound::DetachFromNode
================
*/
// FUNCTION: STBC 0x0070bd90 ?DetachFromNode@TGSound@@QAE_NXZ
bool TGSound::DetachFromNode( void ) {
	if ( node != NULL ) {
		NiNode *parent = GetParentNode();
		if ( parent != NULL ) {
			parent->DetachChild( node );
			return true;
		}
	}
	return false;
}

/*
================
TGSound::GetParentNode
================
*/
// FUNCTION: STBC 0x0070bdf0 ?GetParentNode@TGSound@@QAEPAVNiNode@@XZ
NiNode *TGSound::GetParentNode( void ) {
	if ( node != NULL ) {
		return node->GetParent();
	}
	return NULL;
}

/*
================
TGSound::SetOrientation
================
*/
// FUNCTION: STBC 0x0070be00 ?SetOrientation@TGSound@@QAE_NPAVNiPoint3@@0@Z
bool TGSound::SetOrientation( NiPoint3 *forward, NiPoint3 *up ) {
	if ( source != NULL ) {
		source->SetDirection( *forward );
		source->SetUp( *up );
		return true;
	}
	facing = *forward;
	upDirection = *up;
	return true;
}

/*
================
TGSound::GetOrientation
================
*/
// FUNCTION: STBC 0x0070be80 ?GetOrientation@TGSound@@QAE_NPAVNiPoint3@@0@Z
bool TGSound::GetOrientation( NiPoint3 *forward, NiPoint3 *up ) {
	if ( source != NULL ) {
		*forward = source->GetDirection();
		*up = source->GetUp();
		return true;
	}
	*forward = facing;
	*up = upDirection;
	return true;
}

/*
================
TGSound::SetPosition

The source is placed; a sound without one places its node.
================
*/
// FUNCTION: STBC 0x0070bf00 ?SetPosition@TGSound@@QAEXPAVNiPoint3@@@Z
void TGSound::SetPosition( NiPoint3 *position ) {
	NiAVObject *object = source;
	if ( object == NULL ) {
		object = node;
	}
	if ( object != NULL ) {
		object->SetTranslate( *position );
	}
}

/*
================
TGSound::GetPosition
================
*/
// FUNCTION: STBC 0x0070bf30 ?GetPosition@TGSound@@QAE?AVNiPoint3@@XZ
NiPoint3 TGSound::GetPosition( void ) {
	if ( source != NULL ) {
		return source->GetTranslate();
	}
	if ( node != NULL ) {
		return node->GetTranslate();
	}
	return NiPoint3::ZERO;
}

/*
================
TGSound::SetConeData

The cone is passed to the source only when it changes.
================
*/
// FUNCTION: STBC 0x0070bf90 ?SetConeData@TGSound@@QAE_NMMM@Z
bool TGSound::SetConeData( float inner, float outer, float outerVolume ) {
	if ( source != NULL && ( innerConeAngle != inner || outerConeAngle != outer || outerConeVolume != outerVolume ) ) {
		source->SetConeData( inner, outer, outerVolume );
	}
	innerConeAngle = inner;
	outerConeAngle = outer;
	outerConeVolume = outerVolume;
	return true;
}

/*
================
TGSound::GetConeData
================
*/
// FUNCTION: STBC 0x0070c000 ?GetConeData@TGSound@@QAE_NPAM00@Z
bool TGSound::GetConeData( float *inner, float *outer, float *outerVolume ) {
	if ( source != NULL ) {
		source->GetConeData( *inner, *outer, *outerVolume );
		return true;
	}
	*inner = innerConeAngle;
	*outer = outerConeAngle;
	*outerVolume = outerConeVolume;
	return true;
}

/*
================
TGSound::SetMinMaxDistance
================
*/
// FUNCTION: STBC 0x0070c050 ?SetMinMaxDistance@TGSound@@QAE_NMM@Z
bool TGSound::SetMinMaxDistance( float minimum, float maximum ) {
	minDistance = minimum;
	maxDistance = maximum;
	if ( source != NULL ) {
		return source->SetMinMaxDistance( minimum, maximum );
	}
	return false;
}

/*
================
TGSound::GetMinMaxDistance
================
*/
// FUNCTION: STBC 0x0070c080 ?GetMinMaxDistance@TGSound@@QAE_NPAM0@Z
bool TGSound::GetMinMaxDistance( float *minimum, float *maximum ) {
	if ( source != NULL ) {
		source->GetMinMaxDistance( minDistance, maxDistance );
	}
	*minimum = minDistance;
	*maximum = maxDistance;
	return true;
}

/*
================
TGSound::SetVolume

The volume the source plays at is the sound's, scaled by the manager's
master volume and by that of the category the sound is in; the effects
also by the factor that falls while a voice plays.
================
*/
// FUNCTION: STBC 0x0070c0b0 ?SetVolume@TGSound@@QAE_NM@Z
bool TGSound::SetVolume( float volume_ ) {
	volume = volume_;
	float scaled = g_kSoundManager.GetMasterVolume() * volume_;
	if ( voice ) {
		if ( g_kSoundManager.IsVoiceEnabled() ) {
			scaled = g_kSoundManager.GetVoiceVolume() * scaled;
		} else {
			scaled = 0.0f;
		}
	} else if ( sfx ) {
		if ( g_kSoundManager.IsSFXEnabled() ) {
			scaled = g_kSoundManager.GetSFXVolume() * scaled;
		} else {
			scaled = 0.0f;
		}
	}
	if ( interfaceSound ) {
		if ( g_kSoundManager.IsSFXEnabled() ) {
			scaled = g_kSoundManager.GetInterfaceVolume() * scaled;
		} else {
			scaled = 0.0f;
		}
	}
	if ( !voice ) {
		scaled = g_kSoundManager.sfxAdjustment * scaled;
	}
	if ( source != NULL ) {
		source->SetVolume( scaled );
		return true;
	}
	return false;
}

/*
================
TGSound::SetPitch

The pitch is a factor of the rate the sound was recorded at.
================
*/
// FUNCTION: STBC 0x0070c170 ?SetPitch@TGSound@@QAE_NM@Z
bool TGSound::SetPitch( float pitch_ ) {
	pitch = pitch_;
	if ( source != NULL ) {
		return source->SetPlaybackRate( (int)( source->GetDefaultPlaybackRate() * pitch_ ) );
	}
	return false;
}

/*
================
TGSound::GetPitch
================
*/
// FUNCTION: STBC 0x0070c1b0 ?GetPitch@TGSound@@QAEMXZ
float TGSound::GetPitch( void ) {
	if ( source != NULL ) {
		pitch = (float)source->GetPlaybackRate() / (float)source->GetDefaultPlaybackRate();
	}
	return pitch;
}

/*
================
TGSound::SetPlayPositionSeconds

Without a source the position is kept for later.
================
*/
// FUNCTION: STBC 0x0070c1f0 ?SetPlayPositionSeconds@TGSound@@QAE_NM@Z
bool TGSound::SetPlayPositionSeconds( float seconds ) {
	if ( source != NULL ) {
		return source->Unknown78FCC0( seconds );
	}
	hasPlayPosition = true;
	playPosition = seconds;
	return false;
}

/*
================
TGSound::GetPlayPositionSeconds
================
*/
// FUNCTION: STBC 0x0070c220 ?GetPlayPositionSeconds@TGSound@@QAEMXZ
float TGSound::GetPlayPositionSeconds( void ) {
	if ( source != NULL ) {
		return source->GetPlayTime();
	}
	if ( hasPlayPosition ) {
		return playPosition;
	}
	return 0.0f;
}

/*
================
TGSound::SetPlayPosition

Hands the position to the source. Without one it cancels a position
SetPlayPositionSeconds left pending and returns false.
================
*/
// FUNCTION: STBC 0x0070c240
bool TGSound::SetPlayPosition( unsigned long position ) {
	if ( source != NULL ) {
		return source->SetPosition( position );
	}
	hasPlayPosition = false;
	return false;
}

/*
================
TGSound::GetPlayPosition

The source's play position; 0 without a source.
================
*/
// FUNCTION: STBC 0x0070c260
unsigned long TGSound::GetPlayPosition( void ) {
	if ( source != NULL ) {
		return source->GetPosition();
	}
	return 0;
}

/*
================
TGSound::GetDurationSeconds

-1 when there is no source to ask.
================
*/
// FUNCTION: STBC 0x0070c270 ?GetDurationSeconds@TGSound@@QAEMXZ
float TGSound::GetDurationSeconds( void ) {
	if ( source != NULL ) {
		return source->GetPlayLength();
	}
	// BUG: redundant: both answers are -1, so the 3D and streamed test decides nothing
	if ( Is3D() || streamed ) {
		return -1.0f;
	}
	return -1.0f;
}

/*
================
TGSound::Update

Moves the volume along the fade points: the points the sound has passed
are dropped (all but the last), the volume between the last passed and the
next is interpolated, and a fade that has settled on the volume goes. Then
the source is placed where the node is, relative to the region's frame.
================
*/
// FUNCTION: STBC 0x0070c2a0 ?Update@TGSound@@QAEXXZ
void TGSound::Update( void ) {
	if ( fadePoints.GetSize() != 0 ) {
		float position = GetPlayPositionSeconds();
		NiTListIterator pos = fadePoints.GetHeadPos();
		float previousTime = 0.0f;
		float previousVolume = volume;
		int passed = -1;
		float nextTime, nextVolume;
		while ( pos != NULL ) {
			TGFadePoint *fade = fadePoints.GetNext( pos );
			if ( fade->time > position ) {
				nextTime = fade->time;
				nextVolume = fade->volume;
				break;
			}
			nextTime = previousTime = fade->time;
			nextVolume = previousVolume = fade->volume;
			passed++;
		}
		while ( --passed >= 0 ) {
			delete fadePoints.RemoveHead();
		}
		float newVolume = ( nextTime != previousTime && nextTime >= position ) ?
			( ( nextTime - position ) / ( nextTime - previousTime ) ) * previousVolume + ( 1.0f - ( nextTime - position ) / ( nextTime - previousTime ) ) * nextVolume :
			nextVolume;
		if ( fadePoints.GetSize() == 1 && fadePoints.GetHead()->volume == newVolume ) {
			delete fadePoints.RemoveHead();
		}
		SetVolume( newVolume );
	}

	if ( node != NULL ) {
		NiPoint3 regionPosition = NiPoint3::ZERO;
		NiMatrix3 regionRotation( NiMatrix3::IDENTITY );
		if ( region != NULL ) {
			regionPosition = region->position;
			regionRotation = region->orientation;
		} else {
			// retail calls Is3D here and drops the result
			Is3D();
		}
		node->NiAVObject::Update( 0.0f );
		source->SetTranslate( regionRotation * ( node->GetWorldTranslate() - regionPosition ) );
		source->SetRotate( regionRotation * node->GetWorldRotate() );
		source->NiAVObject::Update( 0.0f );
		source->Update();
	}
}

/*
================
TGSound::AddFadePoint

The point keeps the list sorted by time; the time is held to 0 or more and
the volume to 0 to 1.
================
*/
// FUNCTION: STBC 0x0070c4f0 ?AddFadePoint@TGSound@@QAEXMM@Z
void TGSound::AddFadePoint( float time, float fadeVolume ) {
	if ( time < 0.0f ) {
		time = 0.0f;
	}
	if ( fadeVolume < 0.0f ) {
		fadeVolume = 0.0f;
	} else if ( fadeVolume > 1.0f ) {
		fadeVolume = 1.0f;
	}

	TGFadePoint *fade = new TGFadePoint;
	fade->time = time;
	fade->volume = fadeVolume;

	NiTListIterator pos = fadePoints.GetHeadPos();
	while ( pos != NULL ) {
		NiTListIterator at = pos;
		if ( fadePoints.GetNext( pos )->time > time ) {
			fadePoints.InsertBefore( at, fade );
			return;
		}
	}
	fadePoints.AddTail( fade );
}

/*
================
TGSound::ClearFadePoints
================
*/
// FUNCTION: STBC 0x0070c640 ?ClearFadePoints@TGSound@@QAEXXZ
void TGSound::ClearFadePoints( void ) {
	while ( fadePoints.GetSize() != 0 ) {
		delete fadePoints.RemoveHead();
	}
}

/*
================
TGSound::SetEndEvent

The event the sound sends when it ends. The event it had is deleted unless
it is the same one; NULL takes the sound's event away.
================
*/
// FUNCTION: STBC 0x0070c690 ?SetEndEvent@TGSound@@QAE_NPAVTGEvent@@@Z
bool TGSound::SetEndEvent( TGEvent *event ) {
	if ( endEventID != 0 && ( event == NULL || event->GetObjID() != endEventID ) ) {
		TGEvent *old = TGEvent::GetTGEventPtr( endEventID );
		if ( old != NULL ) {
			delete old;
		}
		endEventID = 0;
	}
	if ( event != NULL ) {
		endEventID = event->GetObjID();
	}
	return true;
}

/*
================
TGSound::GetEndEvent
================
*/
// FUNCTION: STBC 0x0070c6e0 ?GetEndEvent@TGSound@@QAEPAVTGEvent@@XZ
TGEvent *TGSound::GetEndEvent( void ) {
	return TGEvent::GetTGEventPtr( endEventID );
}

// DATA: STBC 0x0099b858
NiTStringMap<TGSoundFile *> TGSoundFile::files;

/*
================
TGSoundFile::TGSoundFile

Reads the file whole through Miles; the memory it takes counts in the
sounds' rough memory usage.
================
*/
// FUNCTION: STBC 0x0070c6f0 ??0TGSoundFile@@QAE@PAD@Z
TGSoundFile::TGSoundFile( char *fileName_ ) {
	users = 0;
	size = AIL_file_size( fileName_ );
	image = AIL_file_read( fileName_, NULL );
	fileName = new char[strlen( fileName_ ) + 1];
	strcpy( fileName, fileName_ );
	if ( image != NULL ) {
		TGSound::roughMemoryUsage += size;
	}
}

/*
================
TGSoundFile::~TGSoundFile
================
*/
// FUNCTION: STBC 0x0070c770 ??1TGSoundFile@@QAE@XZ
TGSoundFile::~TGSoundFile( void ) {
	delete fileName;
	if ( image != NULL ) {
		AIL_mem_free_lock( image );
		image = NULL;
		TGSound::roughMemoryUsage -= size;
	}
}

/*
================
TGSoundFile::Get

The file is read the first time it is asked for with create set; it is
filed only if the read gave an image. Every call that returns the file
adds a user.
================
*/
// FUNCTION: STBC 0x0070c7a0 ?Get@TGSoundFile@@SAPAV1@PAD_N@Z
// PARKED: retail keeps a zero in ebx across the whole function; the source reading that gives it is not found
TGSoundFile *TGSoundFile::Get( char *fileName, bool create ) {
	TGSoundFile *file = NULL;
	if ( files.GetAt( fileName, file ) ) {
		file->users++;
		return file;
	}
	if ( create ) {
		file = new TGSoundFile( fileName );
		if ( file->image == NULL ) {
			delete file;
			return NULL;
		}
		files.SetAt( fileName, file );
		file->users++;
		return file;
	}
	return file;
}

/*
================
TGSoundFile::Release

The last user to let go drops the file from the cache and frees it.
================
*/
// FUNCTION: STBC 0x0070c940 ?Release@TGSoundFile@@SAXPADPAV1@@Z
void TGSoundFile::Release( char *fileName, TGSoundFile *file ) {
	if ( file != NULL ) {
		if ( file->users <= 1 ) {
			files.RemoveAt( fileName );
		}
		if ( --file->users <= 0 ) {
			delete file;
		}
	}
}

/*
================
TGSound::WriteToStream
================
*/
// FUNCTION: STBC 0x0070ca30 ?WriteToStream@TGSound@@QAE_NPAVTGStream@@@Z
bool TGSound::WriteToStream( TGStream *stream ) {
	stream->VirtualSlot11( name );
	stream->VirtualSlot17( (long)this );
	stream->VirtualSlot11( group );
	stream->WriteID( endEventID );
	stream->VirtualSlot17( (long)GetParentNode() );
	stream->WriteFloat( GetPlayPositionSeconds() );
	stream->WriteBool( paused );
	stream->WriteBool( sfx );
	stream->WriteBool( voice );
	stream->WriteBool( interfaceSound );
	stream->WriteLong( id );
	stream->WriteFloat( priority );
	stream->WriteInt( fadePoints.GetSize() );
	NiTListIterator pos = fadePoints.GetHeadPos();
	while ( pos != NULL ) {
		TGFadePoint *fade = fadePoints.GetNext( pos );
		stream->WriteFloat( fade->time );
		stream->WriteFloat( fade->volume );
	}
	stream->VirtualSlot17( (long)region );
	return true;
}

/*
================
TGSound::ReadFromStream

The addresses of the parent node and the region are those they had when
the game was saved; FixUpPointers and PostFixUp turn them back into the
objects.
================
*/
// FUNCTION: STBC 0x0070cb20 ?ReadFromStream@TGSound@@QAE_NPAVTGStream@@@Z
bool TGSound::ReadFromStream( TGStream *stream ) {
	pointerMap.Add( stream->VirtualSlot16(), this );
	// BUG: the group the copy has is not freed before it is replaced
	group = stream->VirtualSlot10( -1 );
	endEventID = stream->ReadID();
	savedParentNode = stream->VirtualSlot16();
	savedPlayPosition = stream->ReadFloat();
	if ( savedPlayPosition > 0.0f ) {
		SetPlayPositionSeconds( savedPlayPosition );
	}
	paused = stream->ReadBool();
	sfx = stream->ReadBool();
	voice = stream->ReadBool();
	interfaceSound = stream->ReadBool();
	id = stream->ReadLong();
	priority = stream->ReadFloat();
	int count = stream->ReadInt();
	for ( int i = count - 1; i >= 0; i-- ) {
		TGFadePoint *fade = new TGFadePoint;
		fade->time = stream->ReadFloat();
		fade->volume = stream->ReadFloat();
		fadePoints.AddTail( fade );
	}
	region = (TGSoundRegion *)stream->VirtualSlot16();
	return true;
}

/*
================
TGSound::FixUpPointers

False when the region or the end event did not come back with the game.
================
*/
// FUNCTION: STBC 0x0070cd10 ?FixUpPointers@TGSound@@QAE_NXZ
bool TGSound::FixUpPointers( void ) {
	if ( region != NULL && pointerMap.Find( (long)region ) == NULL ) {
		return false;
	}
	if ( endEventID != 0 && TGObject::GetTGObjectPtr( endEventID ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
TGSound::PostFixUp

The sound's node goes back under its parent and the sound back in its
region.
================
*/
// FUNCTION: STBC 0x0070cd90 ?PostFixUp@TGSound@@QAEXXZ
void TGSound::PostFixUp( void ) {
	if ( savedParentNode != 0 ) {
		NiNode *parent = (NiNode *)pointerMap.Find( savedParentNode );
		if ( parent != NULL ) {
			parent->AttachChild( node, false );
		}
	}
	if ( region != NULL ) {
		TGSoundRegion *saved = (TGSoundRegion *)pointerMap.Find( (long)region );
		region = NULL;
		saved->AddSound( this );
	}
}

/*
================
TGSound::Unknown70CE60

The sound regions' filter 1: a silenced copy stops and gives up its source.
================
*/
// FUNCTION: STBC 0x0070ce60 ?Unknown70CE60@TGSound@@QAEX_N@Z
void TGSound::Unknown70CE60( bool silenced_ ) {
	if ( silenced != silenced_ ) {
		if ( silenced_ ) {
			Stop();
			if ( loaded && unknown11 ) {
				Unload();
			}
		}
		silenced = silenced_;
	}
}
