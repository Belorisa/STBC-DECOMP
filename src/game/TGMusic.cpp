/*
===============================================================================

	TGMusic.cpp

===============================================================================
*/

#include <string.h>
#include "TGEventManager.h"
#include "TGMusicFadeEvent.h"
#include "TGStringEvent.h"
#include "TGMusic.h"
#include "TGConfigMapping.h"
#include "TGSound.h"
#include "TGSoundManager.h"
#include "TGStream.h"
#include "TGTimer.h"
#include "TGTimerManager.h"
#include "UtopiaApp.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00715210 _$E57
// FUNCTION: STBC 0x00715220 _$E56
// FUNCTION: STBC 0x00715230 _$E219
// FUNCTION: STBC 0x00715240 _$E218
// FUNCTION: STBC 0x00715250 _$E68
// FUNCTION: STBC 0x00715260 _$E67
// FUNCTION: STBC 0x00715270 _$E72
// FUNCTION: STBC 0x00715280 _$E71
// FUNCTION: STBC 0x00715290 _$E76
// FUNCTION: STBC 0x007152a0 _$E75
// FUNCTION: STBC 0x007152b0 _$E80
// FUNCTION: STBC 0x007152c0 _$E79
// FUNCTION: STBC 0x007152d0 _$E197
// FUNCTION: STBC 0x007152e0 _$E196
// FUNCTION: STBC 0x007152f0 _$E139
// FUNCTION: STBC 0x00715300 _$E138
// FUNCTION: STBC 0x00715310 _$E152
// FUNCTION: STBC 0x00715320 _$E151
// FUNCTION: STBC 0x00715350 _$E224
// FUNCTION: STBC 0x00715360 _$E223
// FUNCTION: STBC 0x00715370 _$E206
// FUNCTION: STBC 0x00715380 _$E205
// FUNCTION: STBC 0x00715490 _$E228
// FUNCTION: STBC 0x007154a0 _$E227
// FUNCTION: STBC 0x00715590 _$E235
// FUNCTION: STBC 0x007155a0 _$E234
// FUNCTION: STBC 0x00715620 _$E97
// FUNCTION: STBC 0x00715630 _$E96
// FUNCTION: STBC 0x00715840 _$E102
// FUNCTION: STBC 0x00715850 _$E101
// FUNCTION: STBC 0x00715880 _$E107
// FUNCTION: STBC 0x00715890 _$E106
// FUNCTION: STBC 0x00715920 _$E116
// FUNCTION: STBC 0x00715930 _$E115
// FUNCTION: STBC 0x007159a0 _$E122
// FUNCTION: STBC 0x007159b0 _$E121
// FUNCTION: STBC 0x007159c0 _$E127
// FUNCTION: STBC 0x007159d0 _$E126
// FUNCTION: STBC 0x007159e0 _$E157
// FUNCTION: STBC 0x007159f0 _$E156
// FUNCTION: STBC 0x00715a20 _$E162
// FUNCTION: STBC 0x00715a30 _$E161
// FUNCTION: STBC 0x00715a80 _$E167
// FUNCTION: STBC 0x00715a90 _$E166
// FUNCTION: STBC 0x00715ac0 _$E132
// FUNCTION: STBC 0x00715ad0 _$E131
// FUNCTION: STBC 0x00715b20 _$E172
// FUNCTION: STBC 0x00715b30 _$E171
// FUNCTION: STBC 0x00715b40 _$E177
// FUNCTION: STBC 0x00715b50 _$E176
// FUNCTION: STBC 0x00715c60 _$E182
// FUNCTION: STBC 0x00715c70 _$E181
// FUNCTION: STBC 0x00715ca0 _$E187
// FUNCTION: STBC 0x00715cb0 _$E186
// FUNCTION: STBC 0x00715cc0 _$E192
// FUNCTION: STBC 0x00715cd0 _$E191
// FUNCTION: STBC 0x00715d20 _$E210
// FUNCTION: STBC 0x00715d30 _$E209

// The music player's construction and exit-time destruction.
// FUNCTION: STBC 0x007136d0 _$E245
// FUNCTION: STBC 0x007136e0 _$E242
// FUNCTION: STBC 0x007136f0 _$E244
// FUNCTION: STBC 0x00713700 _$E243
// DATA: STBC 0x0099bda0
TGMusic g_kMusicManager;

// The class handler table's initialiser and exit-time destructor.
// FUNCTION: STBC 0x00713710 _$E251
// FUNCTION: STBC 0x00713720 _$E247
// FUNCTION: STBC 0x00713730 _$E250
// FUNCTION: STBC 0x00713740 _$E248
// DATA: STBC 0x0099be88
TGEventHandlerTable TGMusic::classHandlers;

/*
================
TGMusic::TGMusic
================
*/
// FUNCTION: STBC 0x00713760 ??0TGMusic@@QAE@XZ
TGMusic::TGMusic( void ) : fades( 2 ), fadeEvents( 2 ) {
	musicName = NULL;
	soundID = unknown896A9C;
	volume = 1.0f;
	lastUpdateTime = 0.0f;
	enabled = true;
	fanfarePlaying = false;
	resumeName = NULL;
	resumeLooping = true;
	resumePosition = 0.0f;
}

/*
================
TGMusic::~TGMusic
================
*/
// FUNCTION: STBC 0x00713840 ??1TGMusic@@UAE@XZ
TGMusic::~TGMusic( void ) {
	ClearFanfare();
	DeletePeriods();
	if ( musicName != NULL ) {
		delete musicName;
	}
}

/*
================
TGMusic::DeletePeriods
================
*/
// FUNCTION: STBC 0x007139b0 ?DeletePeriods@TGMusic@@IAEXXZ
void TGMusic::DeletePeriods( void ) {
	NiTMapIterator pos = periods.GetFirstPos();
	while ( pos ) {
		const char *name;
		TGMusicPeriod *period;
		periods.GetNext( pos, name, period );
		periods.RemoveAt( name );
		delete period;
	}
}

/*
================
TGMusic::LoadMusic

Loads a piece of music as a sound of the Music group, streamed when the
options ask for it, and files the period of its fanfare under the name.
================
*/
// FUNCTION: STBC 0x00713ad0
bool TGMusic::LoadMusic( char *fileName, char *name, float period ) {
	long flags = 0;
	if ( g_kConfigMapping.HasValue( "Sound", "StreamMusic" ) && g_kConfigMapping.GetIntValue( "Sound", "StreamMusic" ) != 0 ) {
		flags = LS_STREAMED;
	}
	TGSound *sound = new TGSound( fileName, name, flags );
	if ( !sound->loaded ) {
		delete sound;
		return false;
	}
	sound->SetGroup( "Music" );
	sound->sfx = false;
	sound->SetPriority( 1.0f );
	TGMusicPeriod *fanfarePeriod = new TGMusicPeriod;
	fanfarePeriod->period = period;
	periods.SetAt( name, fanfarePeriod );
	return true;
}

/*
================
TGMusic::UnloadMusic

Deletes the piece's sound and its fanfare period; with no name, every
piece of the Music group. Always true.
================
*/
// FUNCTION: STBC 0x00713c50 ?UnloadMusic@TGMusic@@QAE_NPAD@Z
bool TGMusic::UnloadMusic( char *name ) {
	if ( name != NULL ) {
		TGMusicPeriod *period;
		if ( periods.GetAt( name, period ) ) {
			periods.RemoveAt( name );
			delete period;
			g_kSoundManager.DeleteSound( name );
		}
	} else {
		g_kSoundManager.DeleteAllSoundsInGroup( "Music" );
		DeletePeriods();
	}
	return true;
}

/*
================
TGMusic::StartMusic

Plays the piece, fading it in over the piece playing, which fades out. While
a fanfare plays, the piece is only remembered to play after it. The piece
already playing only takes the looping flag. The fade events sent before
are sent nowhere. False when the piece has no sound.
================
*/
// FUNCTION: STBC 0x00713d60 ?StartMusic@TGMusic@@QAE_NPAD_N@Z
bool TGMusic::StartMusic( char *name, bool looping ) {
	bool result = true;
	TGSound *sound = g_kSoundManager.GetSound( name );
	if ( sound == NULL ) {
		result = false;
	}

	if ( fanfarePlaying ) {
		if ( resumeName != NULL ) {
			delete resumeName;
			resumeName = NULL;
		}
		resumeName = new char[strlen( name ) + 1];
		strcpy( resumeName, name );
		resumeLooping = looping;
		resumePosition = 0.0f;
		return result;
	}

	TGSound *playing = g_kSoundManager.GetPlayingSound( soundID );
	if ( musicName != NULL && strcmp( musicName, name ) == 0 && playing != NULL ) {
		playing->SetLooping( looping );
		return true;
	}

	float time = UtopiaApp::GetApp()->GetGameTime();
	float fadeTime = 0.0f;
	float fromVolume = 1.0f;
	if ( musicName != NULL && playing != NULL ) {
		FadeOutMusic( &time, &fadeTime );
		fromVolume = 0.0f;
	}

	while ( fadeEvents.GetSize() != 0 ) {
		TGEvent *fadeEvent = TGEvent::GetTGEventPtr( fadeEvents.RemoveHead() );
		if ( fadeEvent != NULL ) {
			fadeEvent->SetDestination( INVALID_DESTINATION );
		}
	}

	if ( sound != NULL ) {
		TGTimer *timer = new TGTimer;
		timer->SetTimerStart( time );
		TGMusicFadeEvent *fadeIn = new TGMusicFadeEvent;
		fadeIn->SetEventType( ET_MUSIC_FADE );
		fadeIn->SetDestination( this );
		fadeIn->SetMusicName( name );
		fadeIn->SetFadeTime( fadeTime );
		fadeIn->fromVolume = fromVolume;
		fadeIn->toVolume = 1.0f;
		fadeIn->unknown38 = 0.0f;
		fadeIn->looping = looping;
		fadeEvents.AddTail( fadeIn->GetObjID() );
		timer->SetEvent( fadeIn );
		g_kTimerManager.AddTimer( timer );
	}
	return result;
}

/*
================
TGMusic::FadeOutMusic

Fades the piece playing out over two seconds, or over its fanfare period
from the next multiple of the period, and forgets it. time and fadeTime
come back with the fade's start and length (the game time and 0 when
nothing plays). The name is invented.
================
*/
// FUNCTION: STBC 0x00714050
void TGMusic::FadeOutMusic( float *time, float *fadeTime ) {
	*time = UtopiaApp::GetApp()->GetGameTime();
	*fadeTime = 0.0f;
	TGSound *playing = g_kSoundManager.GetPlayingSound( soundID );
	if ( playing == NULL ) {
		return;
	}

	float position = playing->GetPlayPositionSeconds();
	*fadeTime = 2.0f;
	TGMusicPeriod *period;
	if ( periods.GetAt( playing->name, period ) && period->period >= 0.0f ) {
		*time += (float)( (int)( position / period->period ) + 1 ) * period->period - position;
		*fadeTime = period->period;
	}

	TGTimer *timer = new TGTimer;
	timer->SetTimerStart( *time );
	TGMusicFadeEvent *fadeOut = new TGMusicFadeEvent;
	fadeOut->SetEventType( ET_MUSIC_FADE );
	fadeOut->SetDestination( this );
	fadeOut->SetMusicName( playing->name );
	fadeOut->SetFadeTime( *fadeTime );
	fadeOut->fromVolume = -1.0f;
	fadeOut->toVolume = 0.0f;
	fadeOut->unknown38 = 0.0f;
	fadeOut->looping = playing->GetLooping();
	timer->SetEvent( fadeOut );
	g_kTimerManager.AddTimer( timer );

	if ( musicName != NULL ) {
		delete musicName;
		musicName = NULL;
	}
	musicName = NULL;	// BUG: redundant: the test above already cleared it
	soundID = unknown896A9C;
}

/*
================
TGMusic::StopMusic

A fanfare in progress is dropped with the music.
================
*/
// FUNCTION: STBC 0x00714230 ?StopMusic@TGMusic@@QAEXXZ
void TGMusic::StopMusic( void ) {
	if ( fanfarePlaying ) {
		ClearFanfare();
	}
	if ( musicName != NULL ) {
		g_kSoundManager.StopSound( soundID );
		soundID = unknown896A9C;
	}
}

/*
================
TGMusic::PlayFanfare

Does nothing with the player off or a fanfare already playing, or when the
fanfare has no sound. Otherwise fades the piece playing out and the fanfare
in, both at the next multiple of the piece's fanfare period and over that
period (at once when it has none), and remembers the piece and its position
to resume after the fanfare.
================
*/
// FUNCTION: STBC 0x00714260 ?PlayFanfare@TGMusic@@QAEXPAD@Z
void TGMusic::PlayFanfare( char *name ) {
	if ( !enabled || fanfarePlaying ) {
		return;
	}
	if ( g_kSoundManager.GetSound( name ) == NULL ) {
		return;
	}

	float time = UtopiaApp::GetApp()->GetGameTime();
	float fadeTime = 0.0f;
	TGSound *playing = g_kSoundManager.GetPlayingSound( soundID );
	if ( playing != NULL && musicName != NULL ) {
		TGMusicPeriod *period;
		if ( periods.GetAt( playing->name, period ) && period->period >= 0.0f ) {
			float position = playing->GetPlayPositionSeconds();
			time += (float)( (int)( position / period->period ) + 1 ) * period->period - position;
			fadeTime = period->period;
		}

		TGTimer *timer = new TGTimer;
		timer->SetTimerStart( time );
		TGMusicFadeEvent *fadeOut = new TGMusicFadeEvent;
		fadeOut->SetEventType( ET_MUSIC_FADE );
		fadeOut->SetDestination( this );
		fadeOut->SetMusicName( playing->name );
		fadeOut->fadeTime = fadeTime;
		fadeOut->fromVolume = -1.0f;
		fadeOut->toVolume = 0.0f;
		fadeOut->unknown38 = 0.0f;
		fadeOut->looping = playing->GetLooping();
		timer->SetEvent( fadeOut );
		g_kTimerManager.AddTimer( timer );
	}

	TGTimer *timer = new TGTimer;
	timer->SetTimerStart( time );
	TGMusicFadeEvent *fadeIn = new TGMusicFadeEvent;
	fadeIn->SetEventType( ET_MUSIC_FADE );
	fadeIn->SetDestination( this );
	fadeIn->SetMusicName( name );
	fadeIn->fadeTime = fadeTime;
	fadeIn->fromVolume = 0.0f;
	fadeIn->toVolume = 1.0f;
	fadeIn->unknown38 = 0.0f;
	fadeIn->looping = false;
	fadeEvents.AddTail( fadeIn->GetObjID() );
	timer->SetEvent( fadeIn );
	g_kTimerManager.AddTimer( timer );

	fanfarePlaying = true;
	fanfareName = new char[strlen( name ) + 1];
	strcpy( fanfareName, name );
	if ( playing != NULL && musicName != NULL ) {
		resumeName = new char[strlen( musicName ) + 1];
		strcpy( resumeName, musicName );
	}
	resumeLooping = false;
	resumePosition = 0.0f;
	if ( playing != NULL && musicName != NULL ) {
		resumePosition = playing->GetPlayPositionSeconds();
		resumeLooping = playing->GetLooping();
	}
}
/*
================
TGMusic::SetEnabled

Turning the music off stops it; turning it on starts the piece that was
chosen, from the start.
================
*/
// FUNCTION: STBC 0x00714630 ?SetEnabled@TGMusic@@QAEX_N@Z
void TGMusic::SetEnabled( bool enabled_ ) {
	if ( enabled != enabled_ ) {
		enabled = enabled_;
		if ( enabled_ ) {
			if ( musicName != NULL ) {
				StartMusic( musicName, true );
			}
		} else {
			StopMusic();
		}
	}
}

/*
================
TGMusic::SetVolume

The piece that plays follows at once.
================
*/
// FUNCTION: STBC 0x00714660 ?SetVolume@TGMusic@@QAEXM@Z
void TGMusic::SetVolume( float volume_ ) {
	volume = volume_;
	TGSound *sound = g_kSoundManager.GetPlayingSound( soundID );
	if ( sound != NULL ) {
		sound->SetVolume( volume );
	}
}

/*
================
TGMusic::Update

Steps the fades by the game time since the last call, setting each sound's
volume and stopping the sounds faded out to nothing; a finished fade is
deleted. With no fade left, the piece playing and no fanfare, stops every
other loaded piece.
================
*/
// FUNCTION: STBC 0x00714690 ?Update@TGMusic@@QAEXXZ
void TGMusic::Update( void ) {
	float now = UtopiaApp::GetApp()->GetGameTime();
	float elapsed = now - lastUpdateTime;
	lastUpdateTime = now;

	NiTListIterator pos = fades.GetHeadPos();
	while ( pos ) {
		NiTListIterator fadePos = pos;
		TGMusicFade *fade = fades.GetNext( pos );
		fade->timeLeft -= elapsed;
		if ( fade->timeLeft < 0.0f ) {
			fade->timeLeft = 0.0f;
		}
		TGSound *sound = g_kSoundManager.GetPlayingSound( fade->soundID );
		if ( sound != NULL ) {
			float level = fade->toVolume;
			if ( fade->duration > 0.0f ) {
				float t = fade->timeLeft / fade->duration;
				level = ( 1.0f - t ) * fade->toVolume + t * fade->fromVolume;
			}
			sound->SetVolume( level * volume );
			if ( fade->timeLeft <= 0.0f && fade->toVolume <= 0.0f ) {
				sound->Stop();
			}
		}
		if ( fade->timeLeft <= 0.0f ) {
			delete fades.RemovePos( fadePos );
		}
	}

	if ( fades.GetSize() == 0 && musicName != NULL && !fanfarePlaying ) {
		NiTMapIterator periodPos = periods.GetFirstPos();
		while ( periodPos ) {
			const char *name;
			TGMusicPeriod *period;
			periods.GetNext( periodPos, name, period );
			if ( name != NULL && strcmp( name, musicName ) != 0 ) {
				g_kSoundManager.StopSound( (char *)name );
			}
		}
	}
}

/*
================
TGMusic::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x007148e0
void TGMusic::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TGMusic::HandleFadeEvent, "TGMusic::HandleFadeEvent" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TGMusic::MusicDone, "TGMusic::MusicDone" );
}

/*
================
TGMusic::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00714910
void TGMusic::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_MUSIC_FADE, "TGMusic::HandleFadeEvent" );
	AddMethodHandlerForClass( &classHandlers, ET_MUSIC_DONE, "TGMusic::MusicDone" );
}

/*
================
TGMusic::HandleFadeEvent

Starts a fade of the event's piece: when it is not playing yet, starts it
(with the player enabled) at the event's position and volume, with an
event to MusicDone at its end, and makes it the piece playing. A fade
from -1 starts from the volume the piece has.
================
*/
// FUNCTION: STBC 0x00714940 ?HandleFadeEvent@TGMusic@@QAEXPAVTGEvent@@@Z
// PARKED: register: the call set, branches and frame agree; retail stores playingID before pushing esi and picks other scratch registers for the musicName, RemovePos and looping copies
void TGMusic::HandleFadeEvent( TGEvent *event ) {
	TGMusicFadeEvent *fadeEvent = (TGMusicFadeEvent *)event;
	long playingID = unknown896A9C;
	float currentVolume = 0.0f;

	TGSound *sound = g_kSoundManager.GetPlayingSound( fadeEvent->musicName );
	if ( sound == NULL ) {
		fadeEvents.Remove( fadeEvent->GetObjID() );
		sound = g_kSoundManager.GetSound( fadeEvent->musicName );
		if ( sound != NULL && IsEnabled() ) {
			sound->SetLooping( fadeEvent->looping );
			sound->SetVolume( fadeEvent->GetFromVolume() * GetVolume() );
			sound->SetPlayPositionSeconds( fadeEvent->GetStartPosition() );
			TGStringEvent *done = new TGStringEvent;
			done->SetEventType( ET_MUSIC_DONE );
			done->SetDestination( this );
			done->SetString( sound->GetSoundName() );
			playingID = sound->PlayAndNotify( done, true );
		}
		if ( musicName != NULL ) {
			delete musicName;
		}
		musicName = new char[strlen( fadeEvent->musicName ) + 1];
		strcpy( musicName, fadeEvent->musicName );
		soundID = playingID;
	} else {
		playingID = sound->id;
		currentVolume = sound->GetVolume();
		sound->SetLooping( fadeEvent->looping );
	}

	if ( playingID != unknown896A9C ) {
		TGMusicFade *fade = new TGMusicFade;
		fade->soundID = playingID;
		fade->timeLeft = fade->duration = fadeEvent->GetFadeTime();
		fade->toVolume = fadeEvent->GetToVolume();
		if ( fadeEvent->GetFromVolume() == -1.0f ) {
			fade->fromVolume = currentVolume;
		} else {
			fade->fromVolume = fadeEvent->GetFromVolume();
		}
		fades.AddTail( fade );
	}
}

/*
================
TGMusic::WriteToStream
================
*/
// FUNCTION: STBC 0x00714b50 ?WriteToStream@TGMusic@@UAE_NPAVTGStream@@@Z
bool TGMusic::WriteToStream( TGStream *stream ) {
	if ( !TGEventHandlerObject::WriteToStream( stream ) ) {
		return false;
	}
	stream->VirtualSlot11( musicName );
	stream->WriteLong( soundID );
	stream->WriteFloat( volume );
	stream->WriteFloat( lastUpdateTime );
	stream->WriteBool( enabled );

	stream->WriteInt( fades.GetSize() );
	NiTListIterator fadePos = fades.GetHeadPos();
	while ( fadePos ) {
		TGMusicFade *fade = fades.GetNext( fadePos );
		if ( !fade->WriteToStream( stream ) ) {
			return false;
		}
	}

	stream->WriteInt( periods.GetCount() );
	NiTMapIterator periodPos = periods.GetFirstPos();
	while ( periodPos ) {
		const char *name;
		TGMusicPeriod *period;
		periods.GetNext( periodPos, name, period );
		stream->VirtualSlot11( (char *)name );
		if ( !period->WriteToStream( stream ) ) {
			return false;
		}
	}

	stream->WriteInt( fadeEvents.GetSize() );
	NiTListIterator eventPos = fadeEvents.GetHeadPos();
	while ( eventPos ) {
		stream->WriteID( fadeEvents.GetNext( eventPos ) );
	}

	stream->WriteBool( fanfarePlaying );
	stream->VirtualSlot11( fanfareName );
	stream->VirtualSlot11( resumeName );
	stream->WriteBool( resumeLooping );
	stream->WriteFloat( resumePosition );
	return true;
}

/*
================
TGMusic::ReadFromStream
================
*/
// FUNCTION: STBC 0x00714ce0 ?ReadFromStream@TGMusic@@UAE_NPAVTGStream@@@Z
bool TGMusic::ReadFromStream( TGStream *stream ) {
	int i;

	if ( !TGEventHandlerObject::ReadFromStream( stream ) ) {
		return false;
	}
	musicName = stream->VirtualSlot10( -1 );
	soundID = stream->ReadLong();
	volume = stream->ReadFloat();
	lastUpdateTime = stream->ReadFloat();
	enabled = stream->ReadBool();

	for ( i = stream->ReadInt() - 1; i >= 0; i-- ) {
		TGMusicFade *fade = new TGMusicFade;
		if ( !fade->ReadFromStream( stream ) ) {
			return false;
		}
		fades.AddTail( fade );
	}

	for ( i = stream->ReadInt() - 1; i >= 0; i-- ) {
		char *name = stream->VirtualSlot10( -1 );
		TGMusicPeriod *period = new TGMusicPeriod;
		if ( !period->ReadFromStream( stream ) ) {
			return false;
		}
		periods.SetAt( name, period );
		delete name;
	}

	for ( i = stream->ReadInt() - 1; i >= 0; i-- ) {
		fadeEvents.AddTail( stream->ReadID() );
	}

	fanfarePlaying = stream->ReadBool();
	fanfareName = stream->VirtualSlot10( -1 );
	resumeName = stream->VirtualSlot10( -1 );
	resumeLooping = stream->ReadBool();
	resumePosition = stream->ReadFloat();
	return true;
}

/*
================
TGMusicFade::WriteToStream
================
*/
// FUNCTION: STBC 0x00714f50 ?WriteToStream@TGMusicFade@@QAE_NPAVTGStream@@@Z
bool TGMusicFade::WriteToStream( TGStream *stream ) {
	stream->WriteLong( soundID );
	stream->WriteFloat( duration );
	stream->WriteFloat( timeLeft );
	stream->WriteFloat( fromVolume );
	stream->WriteFloat( toVolume );
	return true;
}

/*
================
TGMusicFade::ReadFromStream
================
*/
// FUNCTION: STBC 0x00714fa0 ?ReadFromStream@TGMusicFade@@QAE_NPAVTGStream@@@Z
bool TGMusicFade::ReadFromStream( TGStream *stream ) {
	soundID = stream->ReadLong();
	duration = stream->ReadFloat();
	timeLeft = stream->ReadFloat();
	fromVolume = stream->ReadFloat();
	toVolume = stream->ReadFloat();
	return true;
}

/*
================
TGMusicPeriod::WriteToStream
================
*/
// FUNCTION: STBC 0x00714fe0 ?WriteToStream@TGMusicPeriod@@QAE_NPAVTGStream@@@Z
bool TGMusicPeriod::WriteToStream( TGStream *stream ) {
	stream->WriteFloat( period );
	return true;
}

/*
================
TGMusicPeriod::ReadFromStream
================
*/
// FUNCTION: STBC 0x00715000 ?ReadFromStream@TGMusicPeriod@@QAE_NPAVTGStream@@@Z
bool TGMusicPeriod::ReadFromStream( TGStream *stream ) {
	period = stream->ReadFloat();
	return true;
}

/*
================
TGMusic::MusicDone

The fanfare's piece has ended: the piece that was playing before it fades
back in from where it was left, and the fanfare is forgotten. The event
names the piece that ended.
================
*/
// FUNCTION: STBC 0x00715020
void TGMusic::MusicDone( TGEvent *event ) {
	if ( fanfarePlaying ) {
		// BUG: the narrowed copy of the name is never freed.
		if ( strcmp( ( (TGStringEvent *)event )->GetCString(), fanfareName ) == 0 ) {
			if ( resumeName != NULL ) {
				TGMusicFadeEvent *fade = new TGMusicFadeEvent;
				fade->SetEventType( ET_MUSIC_FADE );
				fade->SetDestination( this );
				fade->SetMusicName( resumeName );
				fade->fadeTime = 0.0f;
				fade->fromVolume = 1.0f;
				fade->toVolume = 1.0f;
				fade->SetStartPosition( resumePosition );
				fade->looping = resumeLooping;
				ProcessEvent( fade );
				delete fade;
			}
			ClearFanfare();
		}
	}
	CallNextHandler( event );
}

/*
================
TGMusic::ClearFanfare
================
*/
// FUNCTION: STBC 0x00715150 ?ClearFanfare@TGMusic@@IAEXXZ
void TGMusic::ClearFanfare( void ) {
	fanfarePlaying = false;
	if ( fanfareName != NULL ) {
		delete fanfareName;
		fanfareName = NULL;
	}
	if ( resumeName != NULL ) {
		delete resumeName;
		resumeName = NULL;
	}
	resumePosition = 0.0f;
	resumeLooping = true;
}

