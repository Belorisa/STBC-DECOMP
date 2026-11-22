/*
===============================================================================

	TGSoundManager.cpp

===============================================================================
*/

#include <iostream>
#include <stdio.h>
#include <string.h>

#include "NiMain.h"
#include "NiListener.h"
#include "NiProvider_Info.h"
#include "NiSoundSystem.h"
#include "PyEmbed.h"
#include "TGMilesImports.h"
#include "TGConfigMapping.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGObject.h"
#include "TGPointerMap.h"
#include "TGSound.h"
#include "TGSoundManager.h"
#include "TGStream.h"
#include "TGStringEvent.h"
#include "TGSystemWrapperClass.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x007120b0 _$E471
// FUNCTION: STBC 0x007120d0 _$E470

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00711bb0 _$E32
// FUNCTION: STBC 0x00711bc0 _$E29
// FUNCTION: STBC 0x00711bd0 _$E31
// FUNCTION: STBC 0x00711be0 _$E30
// FUNCTION: STBC 0x00711bf0 _$E38
// FUNCTION: STBC 0x00711c00 _$E35
// FUNCTION: STBC 0x00711c10 _$E37
// FUNCTION: STBC 0x00711c20 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00711c30 _$E45
// FUNCTION: STBC 0x00711c40 _$E44
// FUNCTION: STBC 0x00711c50 _$E49
// FUNCTION: STBC 0x00711c60 _$E48
// FUNCTION: STBC 0x00711c70 _$E60
// FUNCTION: STBC 0x00711c80 _$E59
// FUNCTION: STBC 0x00711c90 _$E64
// FUNCTION: STBC 0x00711ca0 _$E63
// FUNCTION: STBC 0x00711cb0 _$E68
// FUNCTION: STBC 0x00711cc0 _$E67
// FUNCTION: STBC 0x00711cd0 _$E72
// FUNCTION: STBC 0x00711ce0 _$E71
// FUNCTION: STBC 0x00711cf0 _$E196
// FUNCTION: STBC 0x00711d00 _$E195
// FUNCTION: STBC 0x00711d40 _$E138
// FUNCTION: STBC 0x00711d50 _$E137
// FUNCTION: STBC 0x00711d60 _$E151
// FUNCTION: STBC 0x00711d70 _$E150
// FUNCTION: STBC 0x00711d80 _$E201
// FUNCTION: STBC 0x00711d90 _$E200
// FUNCTION: STBC 0x00711da0 _$E206
// FUNCTION: STBC 0x00711db0 _$E205
// FUNCTION: STBC 0x00711dc0 _$E210
// FUNCTION: STBC 0x00711dd0 _$E209
// FUNCTION: STBC 0x00711e50 _$E214
// FUNCTION: STBC 0x00711e60 _$E213
// FUNCTION: STBC 0x00711e70 _$E221
// FUNCTION: STBC 0x00711e80 _$E220
// FUNCTION: STBC 0x00711e90 _$E246
// FUNCTION: STBC 0x00711ea0 _$E245
// FUNCTION: STBC 0x00711eb0 _$E250
// FUNCTION: STBC 0x00711ec0 _$E249
// FUNCTION: STBC 0x00711ed0 _$E96
// FUNCTION: STBC 0x00711ee0 _$E95
// FUNCTION: STBC 0x00711ef0 _$E232
// FUNCTION: STBC 0x00711f00 _$E231
// FUNCTION: STBC 0x00711f30 _$E239
// FUNCTION: STBC 0x00711f40 _$E238
// FUNCTION: STBC 0x00711f50 _$E262
// FUNCTION: STBC 0x00711f60 _$E261
// FUNCTION: STBC 0x00711f70 _$E266
// FUNCTION: STBC 0x00711f80 _$E265
// FUNCTION: STBC 0x00711f90 _$E306
// FUNCTION: STBC 0x00711fa0 _$E305
// FUNCTION: STBC 0x00712070 _$E312
// FUNCTION: STBC 0x00712080 _$E311
// FUNCTION: STBC 0x007120e0 _$E254
// FUNCTION: STBC 0x007120f0 _$E253
// FUNCTION: STBC 0x00712150 _$E258
// FUNCTION: STBC 0x00712160 _$E257
// FUNCTION: STBC 0x00712170 _$E270
// FUNCTION: STBC 0x00712180 _$E269
// FUNCTION: STBC 0x00712190 _$E274
// FUNCTION: STBC 0x007121a0 _$E273
// FUNCTION: STBC 0x007121b0 _$E278
// FUNCTION: STBC 0x007121c0 _$E277
// FUNCTION: STBC 0x007121d0 _$E282
// FUNCTION: STBC 0x007121e0 _$E281
// FUNCTION: STBC 0x007121f0 _$E286
// FUNCTION: STBC 0x00712200 _$E285
// FUNCTION: STBC 0x00712210 _$E290
// FUNCTION: STBC 0x00712220 _$E289
// FUNCTION: STBC 0x00712230 _$E294
// FUNCTION: STBC 0x00712240 _$E293
// FUNCTION: STBC 0x00712250 _$E298
// FUNCTION: STBC 0x00712260 _$E297
// FUNCTION: STBC 0x00712270 _$E101
// FUNCTION: STBC 0x00712280 _$E100
// FUNCTION: STBC 0x00712290 _$E302
// FUNCTION: STBC 0x007122a0 _$E301
// FUNCTION: STBC 0x007122b0 _$E106
// FUNCTION: STBC 0x007122c0 _$E105
// FUNCTION: STBC 0x007122d0 _$E316
// FUNCTION: STBC 0x007122e0 _$E315
// FUNCTION: STBC 0x007122f0 _$E320
// FUNCTION: STBC 0x00712300 _$E319
// FUNCTION: STBC 0x00712310 _$E324
// FUNCTION: STBC 0x00712320 _$E323
// FUNCTION: STBC 0x00712330 _$E328
// FUNCTION: STBC 0x00712340 _$E327
// FUNCTION: STBC 0x00712350 _$E115
// FUNCTION: STBC 0x00712360 _$E114
// FUNCTION: STBC 0x00712370 _$E332
// FUNCTION: STBC 0x00712380 _$E331
// FUNCTION: STBC 0x00712390 _$E336
// FUNCTION: STBC 0x007123a0 _$E335
// FUNCTION: STBC 0x007123b0 _$E340
// FUNCTION: STBC 0x007123c0 _$E339
// FUNCTION: STBC 0x007123d0 _$E121
// FUNCTION: STBC 0x007123e0 _$E120
// FUNCTION: STBC 0x007123f0 _$E126
// FUNCTION: STBC 0x00712400 _$E125
// FUNCTION: STBC 0x00712410 _$E156
// FUNCTION: STBC 0x00712420 _$E155
// FUNCTION: STBC 0x00712430 _$E344
// FUNCTION: STBC 0x00712440 _$E343
// FUNCTION: STBC 0x00712450 _$E161
// FUNCTION: STBC 0x00712460 _$E160
// FUNCTION: STBC 0x00712470 _$E348
// FUNCTION: STBC 0x00712480 _$E347
// FUNCTION: STBC 0x00712490 _$E352
// FUNCTION: STBC 0x007124a0 _$E351
// FUNCTION: STBC 0x007124b0 _$E166
// FUNCTION: STBC 0x007124c0 _$E165
// FUNCTION: STBC 0x007124d0 _$E356
// FUNCTION: STBC 0x007124e0 _$E355
// FUNCTION: STBC 0x007124f0 _$E131
// FUNCTION: STBC 0x00712500 _$E130
// FUNCTION: STBC 0x00712510 _$E360
// FUNCTION: STBC 0x00712520 _$E359
// FUNCTION: STBC 0x00712530 _$E364
// FUNCTION: STBC 0x00712540 _$E363
// FUNCTION: STBC 0x00712550 _$E171
// FUNCTION: STBC 0x00712560 _$E170
// FUNCTION: STBC 0x00712570 _$E176
// FUNCTION: STBC 0x00712580 _$E175
// FUNCTION: STBC 0x00712590 _$E368
// FUNCTION: STBC 0x007125a0 _$E367
// FUNCTION: STBC 0x007125b0 _$E372
// FUNCTION: STBC 0x007125c0 _$E371
// FUNCTION: STBC 0x007125d0 _$E376
// FUNCTION: STBC 0x007125e0 _$E375
// FUNCTION: STBC 0x007125f0 _$E380
// FUNCTION: STBC 0x00712600 _$E379
// FUNCTION: STBC 0x00712610 _$E384
// FUNCTION: STBC 0x00712620 _$E383
// FUNCTION: STBC 0x00712630 _$E388
// FUNCTION: STBC 0x00712640 _$E387
// FUNCTION: STBC 0x00712650 _$E392
// FUNCTION: STBC 0x00712660 _$E391
// FUNCTION: STBC 0x00712670 _$E396
// FUNCTION: STBC 0x00712680 _$E395
// FUNCTION: STBC 0x00712690 _$E181
// FUNCTION: STBC 0x007126a0 _$E180
// FUNCTION: STBC 0x007126b0 _$E400
// FUNCTION: STBC 0x007126c0 _$E399
// FUNCTION: STBC 0x007126d0 _$E186
// FUNCTION: STBC 0x007126e0 _$E185
// FUNCTION: STBC 0x007126f0 _$E191
// FUNCTION: STBC 0x00712700 _$E190
// FUNCTION: STBC 0x00712710 _$E404
// FUNCTION: STBC 0x00712720 _$E403
// FUNCTION: STBC 0x00712730 _$E410
// FUNCTION: STBC 0x00712740 _$E409
// FUNCTION: STBC 0x00712750 _$E446
// FUNCTION: STBC 0x00712760 _$E445

// The handler table's and the global manager's dynamic initialisers and exit-time destructors.
// FUNCTION: STBC 0x0070edf0 _$E458
// FUNCTION: STBC 0x0070ee00 _$E454
// FUNCTION: STBC 0x0070ee10 _$E457
// FUNCTION: STBC 0x0070ee20 _$E455
// FUNCTION: STBC 0x0070ee40 _$E463
// FUNCTION: STBC 0x0070ee50 _$E460
// FUNCTION: STBC 0x0070ee60 _$E462
// FUNCTION: STBC 0x0070ee70 _$E461

// DATA: STBC 0x0099bae0
TGEventHandlerTable TGSoundManager::classHandlers;

// DATA: STBC 0x0099ba00
TGSoundManager g_kSoundManager;

// Set when a manager is built or read from a stream; nothing reads it
// (invented name).
static bool soundManagerReady;

// The file Reload saves the sounds to while the providers reopen.
#define SOUND_TEMP_FILE		"SoundTmp.sav"

// The options file and its section.
#define OPTIONS_FILE		"options.cfg"
#define SOUND_SECTION		"Sound"

/*
================
TGSoundManager::TGSoundManager
================
*/
// FUNCTION: STBC 0x0070ee80
TGSoundManager::TGSoundManager( void ) :
	redistDirectory( NULL ),
	hWnd( NULL ),
	unknown1C( false ),
	enabled( false ),
	providersOpen( false ),
	masterVolume( 1.0f ),
	sfxVolume( 1.0f ),
	voiceVolume( 1.0f ),
	interfaceVolume( 1.0f ),
	sfxEnabled( true ),
	voiceEnabled( true ),
	sfxAdjustment( 1.0f ),
	sfxAdjustmentMinimum( 0.8f ),
	maxSoundsAtOnce( 0 ),
	configMaxSounds( unknown896AA4 ),
	playingSounds( NULL ),
	slotReserved( NULL ),
	slotIDBase( 0 ),
	outputRate( 0 ),
	outputBits( 0 ),
	a3dTestProvider( NULL ),
	eaxTestProvider( NULL ),
	paused( false ),
	reserves( 2 ),
	stoppedSounds( 2 ) {
	soundManagerReady = true;
	SetMaxSoundsAtOnce( unknown896AA4 );
}

/*
================
TGSoundManager::~TGSoundManager
================
*/
// FUNCTION: STBC 0x0070f010
TGSoundManager::~TGSoundManager( void ) {
	if ( playingSounds != NULL ) {
		delete playingSounds;
		playingSounds = NULL;
	}
	if ( slotReserved != NULL ) {
		delete slotReserved;
		slotReserved = NULL;
	}
	if ( redistDirectory != NULL ) {
		delete redistDirectory;
		redistDirectory = NULL;
	}
	if ( a3dTestProvider != NULL ) {
		delete a3dTestProvider;
	}
	if ( eaxTestProvider != NULL ) {
		delete eaxTestProvider;
	}
}

/*
================
TGSoundManager::Startup

Once started, the manager stays started. The driver's configuration is
read for a report the release build does not make.
================
*/
// FUNCTION: STBC 0x0070f240
bool TGSoundManager::Startup( void *hWnd_, char *redistDirectory_ ) {
	if ( enabled ) {
		return enabled;
	}

	hWnd = hWnd_;
	if ( redistDirectory_ != redistDirectory ) {
		delete redistDirectory;
		redistDirectory = new char[strlen( redistDirectory_ ) + 1];
		strcpy( redistDirectory, redistDirectory_ );
	}

	if ( !LoadConfig( "SoundConfig" ) ) {
		return false;
	}

	NiSoundSystem *soundSystem = NiSoundSystem::GetSoundSystem();
	soundSystem->SetHWnd( hWnd_ );
	bool result = soundSystem->Startup( redistDirectory_, outputRate, outputBits );
	if ( result ) {
		HDIGDRIVER driver = soundSystem->GetDriver();
		if ( driver != NULL ) {
			int rate;
			int format;
			char configuration[128];
			AIL_digital_configuration( driver, &rate, &format, configuration );
		}
		result &= OpenProviders();
	}

	unknown1C = result;
	enabled = result;
	return result;
}

/*
================
TGSoundManager::OpenProviders

Reads SoundConfig and opens the providers it reserves samples on, once.
================
*/
// FUNCTION: STBC 0x0070f330
bool TGSoundManager::OpenProviders( void ) {
	if ( providersOpen ) {
		return true;
	}
	if ( !LoadConfig( "SoundConfig" ) ) {
		return false;
	}
	providersOpen = OpenReservedProviders();
	return providersOpen;
}

/*
================
TGSoundManager::CloseProviders

Deletes every sound and frees the sample handles of every provider.
================
*/
// FUNCTION: STBC 0x0070f360
bool TGSoundManager::CloseProviders( void ) {
	if ( providersOpen ) {
		DeleteAllSounds();
		while ( reserves.GetSize() != 0 ) {
			TGProviderReserve *reserve = reserves.RemoveHead();
			NiProvider_Info *provider = FindProvider( reserve->providerName );
			if ( provider != NULL ) {
				FreeSamples( provider );
			}
			delete reserve->providerName;
			delete reserve;
		}
		FreeSamples( NULL );
		NiSoundSystem::GetSoundSystem()->GetListener();
		providersOpen = false;
	}
	return true;
}

/*
================
TGSoundManager::Reload

Closes and reopens the providers. With keepSounds, the sounds are saved
first and read back after, and the playing ones play on.
================
*/
// FUNCTION: STBC 0x0070f400
bool TGSoundManager::Reload( bool keepSounds ) {
	if ( !enabled ) {
		return false;
	}
	if ( !providersOpen ) {
		return false;
	}

	if ( keepSounds ) {
		TGStream stream;
		stream.VirtualSlot00( SOUND_TEMP_FILE, "wb" );
		if ( WriteToStream( &stream ) ) {
			for ( int i = 0; i < maxSoundsAtOnce; i++ ) {
				TGSound *sound = playingSounds[i];
				if ( sound != NULL ) {
					sound->endEventID = 0;
					if ( sound->GetParentNode() != NULL ) {
						pointerMap.Add( (long)sound->GetParentNode(), sound->GetParentNode() );
					}
					if ( sound->region != NULL ) {
						pointerMap.Add( (long)sound->region, sound->region );
					}
				}
			}
		}
		stream.Close();
	}

	CloseProviders();
	bool result = OpenProviders();

	if ( keepSounds ) {
		if ( result ) {
			TGStream stream;
			stream.VirtualSlot00( SOUND_TEMP_FILE, "rb" );
			if ( TGObject::LoadObject( &stream, 0 ) == NULL ) {
				result = false;
			}
			stream.Close();
			if ( result ) {
				if ( !FixUpPointers() ) {
					result = false;
				} else {
					PostFixUp();
				}
			}
		}
		pointerMap.RemoveAll();
		remove( SOUND_TEMP_FILE );
		RestorePlayingSounds();
	}
	return result;
}

/*
================
TGSoundManager::Shutdown
================
*/
// FUNCTION: STBC 0x0070f780
void TGSoundManager::Shutdown( void ) {
	if ( enabled ) {
		CloseProviders();
		NiSoundSystem::GetSoundSystem()->Shutdown();
		enabled = false;
	}
}

/*
================
TGSoundManager::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x0070f7a0
void TGSoundManager::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TGSoundManager::HandleDeleteSoundEvent, "TGSoundManager::HandleDeleteSoundEvent" );
}

/*
================
TGSoundManager::RegisterHandlers
================
*/
// FUNCTION: STBC 0x0070f7c0
void TGSoundManager::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_DELETE_SOUND, "TGSoundManager::HandleDeleteSoundEvent" );
}

/*
================
TGSoundManager::Update

A playing sound that has stopped is stopped for good, unless it loops
from a stream: that one starts over. While a voice plays, the effects'
factor falls toward its minimum; after, it rises back to 1.
================
*/
// FUNCTION: STBC 0x0070f7e0
void TGSoundManager::Update( bool updateAll ) {
	if ( !enabled ) {
		return;
	}

	NiSoundSystem *soundSystem = NiSoundSystem::GetSoundSystem();
	bool voicePlaying = false;
	for ( int i = 0; i < maxSoundsAtOnce; i++ ) {
		TGSound *sound = playingSounds[i];
		if ( sound == NULL ) {
			continue;
		}
		if ( sound->streamed && sound->looping ) {
			if ( !sound->paused && sound->GetStatus() != 0 ) {
				sound->Unknown70BB80( true );
			}
		} else if ( !sound->paused && sound->GetStatus() != 0 ) {
			sound->Stop();
		} else {
			if ( updateAll ) {
				sound->Update();
			}
			if ( sound->voice ) {
				voicePlaying = true;
			}
		}
	}

	if ( voicePlaying ) {
		if ( sfxAdjustment > sfxAdjustmentMinimum ) {
			float perSecond = ( 1.0f - sfxAdjustmentMinimum ) * unknown896A98;
			sfxAdjustment -= perSecond * g_kSystemWrapper.GetTimeElapsedInSeconds();
			if ( sfxAdjustment < sfxAdjustmentMinimum ) {
				sfxAdjustment = sfxAdjustmentMinimum;
			}
			UpdateVolumes();
		}
	} else if ( sfxAdjustment < 1.0f ) {
		float perSecond = ( 1.0f - sfxAdjustmentMinimum ) * unknown896A98;
		sfxAdjustment += perSecond * g_kSystemWrapper.GetTimeElapsedInSeconds();
		if ( sfxAdjustment > 1.0f ) {
			sfxAdjustment = 1.0f;
		}
		UpdateVolumes();
	}

	DeleteStoppedSounds();
	soundSystem->Update( false, 0.0f );
}

/*
================
TGSoundManager::SetMaxSoundsAtOnce

Stops every sound and rebuilds the slots. The IDs of the new slots start
past every ID handed out so far.
================
*/
// FUNCTION: STBC 0x0070f920
void TGSoundManager::SetMaxSoundsAtOnce( int count ) {
	slotIDBase += maxSoundsAtOnce;
	StopAllSounds();
	if ( maxSoundsAtOnce != 0 ) {
		delete playingSounds;
		playingSounds = NULL;
		delete slotReserved;
		slotReserved = NULL;
	}

	maxSoundsAtOnce = count;
	if ( maxSoundsAtOnce > 0 ) {
		playingSounds = new TGSound *[maxSoundsAtOnce];
		memset( playingSounds, 0, maxSoundsAtOnce * sizeof( TGSound * ) );
		slotReserved = new bool[maxSoundsAtOnce];
		memset( slotReserved, 0, maxSoundsAtOnce * sizeof( bool ) );
		if ( slotIDBase % maxSoundsAtOnce != 0 ) {
			slotIDBase += maxSoundsAtOnce - slotIDBase % maxSoundsAtOnce;
		}
	}
}

/*
================
TGSoundManager::SetUnitsPerMeter
================
*/
// FUNCTION: STBC 0x0070f9d0
bool TGSoundManager::SetUnitsPerMeter( float unitsPerMeter ) {
	return NiSoundSystem::GetSoundSystem()->SetUnitsPerMeter( unitsPerMeter );
}

/*
================
TGSoundManager::GetUnitsPerMeter
================
*/
// FUNCTION: STBC 0x0070f9f0
float TGSoundManager::GetUnitsPerMeter( void ) {
	return NiSoundSystem::GetSoundSystem()->GetUnitsPerMeter();
}

/*
================
TGSoundManager::SetMasterVolume
================
*/
// FUNCTION: STBC 0x0070fa00
void TGSoundManager::SetMasterVolume( float volume ) {
	if ( !enabled ) {
		return;
	}
	if ( volume < 0.0f ) {
		volume = 0.0f;
	} else if ( volume > 1.0f ) {
		volume = 1.0f;
	}
	masterVolume = volume;
	UpdateVolumes();
}

/*
================
TGSoundManager::SetSFXVolume
================
*/
// FUNCTION: STBC 0x0070fa50
void TGSoundManager::SetSFXVolume( float volume ) {
	if ( !enabled ) {
		return;
	}
	if ( volume < 0.0f ) {
		volume = 0.0f;
	} else if ( volume > 1.0f ) {
		volume = 1.0f;
	}
	sfxVolume = volume;
	UpdateVolumes();
}

/*
================
TGSoundManager::UpdateVolumes
================
*/
// FUNCTION: STBC 0x0070faa0
void TGSoundManager::UpdateVolumes( void ) {
	for ( int i = 0; i < maxSoundsAtOnce; i++ ) {
		TGSound *sound = playingSounds[i];
		if ( sound != NULL ) {
			sound->SetVolume( sound->GetVolume() );
		}
	}
}

/*
================
TGSoundManager::SetSFXEnabled
================
*/
// FUNCTION: STBC 0x0070fad0
void TGSoundManager::SetSFXEnabled( bool enable ) {
	if ( enable != sfxEnabled ) {
		sfxEnabled = enable;
		UpdateVolumes();
	}
}

/*
================
TGSoundManager::SetVoiceEnabled
================
*/
// FUNCTION: STBC 0x0070faf0
void TGSoundManager::SetVoiceEnabled( bool enable ) {
	if ( enable != voiceEnabled ) {
		voiceEnabled = enable;
		UpdateVolumes();
	}
}

/*
================
TGSoundManager::SetVoiceVolume
================
*/
// FUNCTION: STBC 0x0070fb10
void TGSoundManager::SetVoiceVolume( float volume ) {
	if ( !enabled ) {
		return;
	}
	if ( volume < 0.0f ) {
		volume = 0.0f;
	} else if ( volume > 1.0f ) {
		volume = 1.0f;
	}
	voiceVolume = volume;
	UpdateVolumes();
}

/*
================
TGSoundManager::SetInterfaceVolume
================
*/
// FUNCTION: STBC 0x0070fb60
void TGSoundManager::SetInterfaceVolume( float volume ) {
	if ( !enabled ) {
		return;
	}
	if ( volume < 0.0f ) {
		volume = 0.0f;
	} else if ( volume > 1.0f ) {
		volume = 1.0f;
	}
	interfaceVolume = volume;
	UpdateVolumes();
}

/*
================
TGSoundManager::GetSound

The filed sound of the name; NULL when there is none.
================
*/
// FUNCTION: STBC 0x0070fbb0
TGSound *TGSoundManager::GetSound( char *name ) {
	if ( !enabled ) {
		return NULL;
	}
	TGSound *sound = NULL;
	sounds.GetAt( name, sound );
	return sound;
}

/*
================
TGSoundManager::GetPlayingSound

The sound playing under the ID; NULL when none is.
================
*/
// FUNCTION: STBC 0x0070fc10 ?GetPlayingSound@TGSoundManager@@QAEPAVTGSound@@J@Z
TGSound *TGSoundManager::GetPlayingSound( long id ) {
	if ( enabled ) {
		int slot = GetSlot( id );
		if ( slot >= 0 ) {
			TGSound *sound = playingSounds[slot];
			if ( sound != NULL && id == sound->id ) {
				return sound;
			}
		}
	}
	return NULL;
}

/*
================
TGSoundManager::GetPlayingSound
================
*/
// FUNCTION: STBC 0x0070fc50 ?GetPlayingSound@TGSoundManager@@QAEPAVTGSound@@PAD@Z
TGSound *TGSoundManager::GetPlayingSound( char *name ) {
	if ( enabled ) {
		for ( int i = 0; i < maxSoundsAtOnce; i++ ) {
			TGSound *sound = playingSounds[i];
			if ( sound != NULL && strcmp( sound->name, name ) == 0 ) {
				return sound;
			}
		}
	}
	return NULL;
}

/*
================
TGSoundManager::PlaySound

Plays a copy of the filed sound of the name; the ID it plays under, the
invalid ID when there is no such sound.
================
*/
// FUNCTION: STBC 0x0070fcd0
int TGSoundManager::PlaySound( char *name ) {
	if ( !enabled ) {
		return 0;
	}
	TGSound *sound = GetSound( name );
	if ( sound != NULL ) {
		return sound->Play( true );
	}
	return unknown896A9C;
}

/*
================
TGSoundManager::StopSound

Stops every playing sound of the name.
================
*/
// FUNCTION: STBC 0x0070fd00 ?StopSound@TGSoundManager@@QAE_NPAD@Z
bool TGSoundManager::StopSound( char *name ) {
	if ( !enabled ) {
		return true;
	}
	for ( int i = 0; i < maxSoundsAtOnce; i++ ) {
		TGSound *sound = playingSounds[i];
		if ( sound != NULL && strcmp( sound->name, name ) == 0 ) {
			sound->Stop();
		}
	}
	return true;
}

/*
================
TGSoundManager::StopSound
================
*/
// FUNCTION: STBC 0x0070fd80 ?StopSound@TGSoundManager@@QAE_NJ@Z
bool TGSoundManager::StopSound( long id ) {
	if ( enabled ) {
		TGSound *sound = GetPlayingSound( id );
		if ( sound != NULL ) {
			sound->Stop();
		}
	}
	return true;
}

/*
================
TGSoundManager::PauseAllSounds
================
*/
// FUNCTION: STBC 0x0070fdb0
void TGSoundManager::PauseAllSounds( void ) {
	for ( int i = 0; i < maxSoundsAtOnce; i++ ) {
		TGSound *sound = playingSounds[i];
		if ( sound != NULL && ( sound->sfx || sound->voice ) ) {
			sound->Pause();
		}
	}
	paused = true;
}

/*
================
TGSoundManager::UnpauseAllSounds
================
*/
// FUNCTION: STBC 0x0070fdf0
void TGSoundManager::UnpauseAllSounds( void ) {
	for ( int i = 0; i < maxSoundsAtOnce; i++ ) {
		TGSound *sound = playingSounds[i];
		if ( sound != NULL && sound->paused && ( sound->sfx || sound->voice ) ) {
			sound->Unpause();
		}
	}
	paused = false;
}

/*
================
TGSoundManager::SetSFXAdjustmentMinimum
================
*/
// FUNCTION: STBC 0x0070fe30
void TGSoundManager::SetSFXAdjustmentMinimum( float minimum ) {
	if ( minimum < 0.0f ) {
		sfxAdjustmentMinimum = 0.0f;
	} else if ( minimum > 1.0f ) {
		sfxAdjustmentMinimum = 1.0f;
	} else {
		sfxAdjustmentMinimum = minimum;
	}
}

/*
================
TGSoundManager::StopAllSounds
================
*/
// FUNCTION: STBC 0x0070fe70
bool TGSoundManager::StopAllSounds( void ) {
	if ( !enabled ) {
		return true;
	}
	for ( int i = 0; i < maxSoundsAtOnce; i++ ) {
		TGSound *sound = playingSounds[i];
		if ( sound != NULL ) {
			sound->Stop();
		}
	}
	return true;
}

/*
================
TGSoundManager::StopAllSoundsInGroup

Stops the playing sounds of the group; a NULL group is the sounds in none.
================
*/
// FUNCTION: STBC 0x0070feb0
bool TGSoundManager::StopAllSoundsInGroup( char *group ) {
	if ( !enabled ) {
		return true;
	}
	for ( int i = 0; i < maxSoundsAtOnce; i++ ) {
		TGSound *sound = playingSounds[i];
		if ( sound != NULL ) {
			if ( sound->IsInGroup( group ) ) {
				sound->Stop();
			}
		}
	}
	return true;
}

/*
================
TGSoundManager::DeleteSound

Deletes the stopped copies of the sound, then stops and deletes the filed
sound itself; false when there is none.
================
*/
// FUNCTION: STBC 0x0070ff40
bool TGSoundManager::DeleteSound( char *name ) {
	if ( enabled ) {
		NiTListIterator pos = stoppedSounds.GetHeadPos();
		while ( pos != NULL ) {
			NiTListIterator itemPos = pos;
			TGSound *stopped = stoppedSounds.GetNext( pos );
			if ( strcmp( stopped->name, name ) == 0 ) {
				stoppedSounds.RemovePos( itemPos );
				delete stopped;
			}
		}

		TGSound *sound = GetSound( name );
		if ( sound != NULL ) {
			StopSound( name );
			delete sound;
			return true;
		}
	}
	return false;
}

/*
================
TGSoundManager::DeleteAllSounds
================
*/
// FUNCTION: STBC 0x007100a0
void TGSoundManager::DeleteAllSounds( void ) {
	if ( !enabled ) {
		return;
	}
	StopAllSounds();
	DeleteStoppedSounds();

	NiTMapIterator pos = sounds.GetFirstPos();
	while ( pos != NULL ) {
		const char *name;
		TGSound *sound;
		sounds.GetNext( pos, name, sound );
		delete sound;
	}
}

/*
================
TGSoundManager::DeleteAllSoundsInGroup

A NULL group is the sounds in none.
================
*/
// FUNCTION: STBC 0x00710140
void TGSoundManager::DeleteAllSoundsInGroup( char *group ) {
	if ( !enabled ) {
		return;
	}
	StopAllSoundsInGroup( group );
	DeleteStoppedSounds();

	NiTMapIterator pos = sounds.GetFirstPos();
	while ( pos != NULL ) {
		const char *name;
		TGSound *sound;
		sounds.GetNext( pos, name, sound );
		if ( sound->IsInGroup( group ) ) {
			delete sound;
		}
	}
}

/*
================
TGSoundManager::GetNumSoundsAllocated
================
*/
// FUNCTION: STBC 0x00710240
int TGSoundManager::GetNumSoundsAllocated( void ) {
	return TGSound::numAllocated;
}

/*
================
TGSoundManager::GetRoughMemoryUsage
================
*/
// FUNCTION: STBC 0x00710250
int TGSoundManager::GetRoughMemoryUsage( void ) {
	return TGSound::roughMemoryUsage;
}

/*
================
TGSoundManager::IsA3DSupported

Whether a provider has the A3D interface. The walk stops when the position
runs out, so the last provider is never asked.
================
*/
// FUNCTION: STBC 0x00710260
bool TGSoundManager::IsA3DSupported( void ) {
	NiSoundSystem *soundSystem = NiSoundSystem::GetSoundSystem();
	NiTListIterator pos = NULL;
	NiProvider_Info *provider = soundSystem->GetFirstProvider( pos );
	while ( pos != NULL ) {
		int value;
		AIL_3D_provider_attribute( provider->GetProvider(), "A3D technology interface", &value );
		if ( value != -1 ) {
			return true;
		}
		provider = soundSystem->GetNextProvider( pos );
	}
	return false;
}

/*
================
TGSoundManager::IsEAXSupported

Whether a provider has the EAX environments; the last provider is never
asked, as in IsA3DSupported.
================
*/
// FUNCTION: STBC 0x007102d0
bool TGSoundManager::IsEAXSupported( void ) {
	NiSoundSystem *soundSystem = NiSoundSystem::GetSoundSystem();
	NiTListIterator pos = NULL;
	NiProvider_Info *provider = soundSystem->GetFirstProvider( pos );
	while ( pos != NULL ) {
		int value;
		AIL_3D_provider_attribute( provider->GetProvider(), "EAX environment selection", &value );
		if ( value != -1 ) {
			return true;
		}
		provider = soundSystem->GetNextProvider( pos );
	}
	return false;
}

/*
================
TGSoundManager::LoadConfig

Reads the output format from the options, and from the script the test
providers and the provider reserve list the options name.
================
*/
// FUNCTION: STBC 0x00710340
bool TGSoundManager::LoadConfig( char *module ) {
	PyEmbed::GetModule( module, false );

	g_kConfigMapping.LoadConfigFile( OPTIONS_FILE );
	if ( !g_kConfigMapping.HasValue( SOUND_SECTION, "StreamBufferSize" ) ) {
		g_kConfigMapping.SetIntValue( SOUND_SECTION, "StreamBufferSize", 0 );
	}
	if ( !g_kConfigMapping.HasValue( SOUND_SECTION, "StreamMusic" ) ) {
		g_kConfigMapping.SetIntValue( SOUND_SECTION, "StreamMusic", 1 );
	}
	if ( !g_kConfigMapping.HasValue( SOUND_SECTION, "StreamVoices" ) ) {
		g_kConfigMapping.SetIntValue( SOUND_SECTION, "StreamVoices", 0 );
	}

	outputRate = 22050;
	if ( g_kConfigMapping.HasValue( SOUND_SECTION, "OutputRate" ) ) {
		outputRate = g_kConfigMapping.GetIntValue( SOUND_SECTION, "OutputRate" );
	} else {
		g_kConfigMapping.SetIntValue( SOUND_SECTION, "OutputRate", outputRate );
	}
	outputBits = 16;
	if ( g_kConfigMapping.HasValue( SOUND_SECTION, "OutputBits" ) ) {
		outputBits = g_kConfigMapping.GetIntValue( SOUND_SECTION, "OutputBits" );
	} else {
		g_kConfigMapping.SetIntValue( SOUND_SECTION, "OutputBits", outputBits );
	}

	char *listName = "LowQualitySoftware";
	if ( g_kConfigMapping.HasValue( SOUND_SECTION, "ProviderReserveList" ) ) {
		listName = g_kConfigMapping.GetStringValue( SOUND_SECTION, "ProviderReserveList" );
	} else {
		g_kConfigMapping.SetStringValue( SOUND_SECTION, "ProviderReserveList", listName );
	}

	PyObject *lists = NULL;
	if ( PyEmbed::GetValue( module, "AllProviderReserveLists", "O", &lists ) == -1 || !PyDict_Check( lists ) ) {
		return false;
	}

	char *string = NULL;
	if ( PyEmbed::GetValue( module, "A3DTestProvider", "s", &string ) == -1 ) {
		return false;
	}
	a3dTestProvider = new char[strlen( string ) + 1];
	strcpy( a3dTestProvider, string );
	if ( PyEmbed::GetValue( module, "EAXTestProvider", "s", &string ) == -1 ) {
		return false;
	}
	eaxTestProvider = new char[strlen( string ) + 1];
	strcpy( eaxTestProvider, string );

	PyObject *list = PyDict_GetItemString( lists, listName );
	if ( list == NULL || !PyTuple_Check( list ) || PyTuple_Size( list ) < 2 ) {
		return false;
	}

	while ( reserves.GetSize() != 0 ) {
		TGProviderReserve *reserve = reserves.RemoveHead();
		delete reserve->providerName;
		delete reserve;
	}

	// The list's first entry is the number of slots, the rest the
	// providers to reserve samples on.
	if ( PyInt_Check( PyTuple_GetItem( list, 0 ) ) ) {
		configMaxSounds = PyInt_AsLong( PyTuple_GetItem( list, 0 ) );
	} else {
		configMaxSounds = 32;
	}
	SetMaxSoundsAtOnce( configMaxSounds );

	for ( int i = 1; i < PyTuple_Size( list ); i++ ) {
		PyObject *entry = PyTuple_GetItem( list, i );
		if ( !PyTuple_Check( entry ) ) {
			continue;
		}
		TGProviderReserve *reserve = new TGProviderReserve;
		char *providerName = NULL;
		if ( PyArg_ParseTuple( entry, "is", &reserve->numSamples, &providerName ) != -1 ) {
			reserve->providerName = new char[strlen( providerName ) + 1];
			strcpy( reserve->providerName, providerName );
			reserves.AddTail( reserve );
		} else {
			PyErr_Print();
		}
	}

	g_kConfigMapping.SaveConfigFile( OPTIONS_FILE );
	return true;
}

/*
================
TGSoundManager::OpenReservedProviders

Opens the providers of the reserve list in order, with the listener on
each, and reserves their samples until the slots are covered; false when
no sample was reserved.
================
*/
// FUNCTION: STBC 0x007107d0
// PARKED: register allocation: retail keeps this in edi and the reserve in ebx, this build the other way round; local order, the count's form and the loop's form tried
bool TGSoundManager::OpenReservedProviders( void ) {
	bool result = true;
	int numSamples = 0;
	NiListener *listener = NiSoundSystem::GetSoundSystem()->GetListener();
	NiTListIterator pos = reserves.GetHeadPos();
	while ( pos != NULL && numSamples < configMaxSounds ) {
		TGProviderReserve *reserve = reserves.GetNext( pos );
		NiProvider_Info *provider = FindProvider( reserve->providerName );
		if ( provider != NULL ) {
			listener->AddProvider( provider );
			int count = configMaxSounds - numSamples;
			if ( reserve->numSamples < count && reserve->numSamples != -1 ) {
				count = reserve->numSamples;
			}
			numSamples += AllocateSamples( provider, count );
		}
	}
	listener->NiAVObject::Update( 0.0f );
	listener->Update();
	result &= numSamples > 0;
	return result;
}

/*
================
TGSoundManager::FindProvider

The provider of the name; NULL when the sound system has none.
================
*/
// FUNCTION: STBC 0x00710880
NiProvider_Info *TGSoundManager::FindProvider( char *name ) {
	NiSoundSystem *soundSystem = NiSoundSystem::GetSoundSystem();
	NiTListIterator pos = NULL;
	NiProvider_Info *provider = soundSystem->GetFirstProvider( pos );
	while ( pos != NULL ) {
		if ( strcmp( name, provider->GetName() ) == 0 ) {
			return provider;
		}
		provider = soundSystem->GetNextProvider( pos );
	}
	return NULL;
}

/*
================
TGSoundManager::AddSound

Files the sound under its name.
================
*/
// FUNCTION: STBC 0x00710910
bool TGSoundManager::AddSound( TGSound *sound ) {
	sounds.SetAt( sound->name, sound );
	return true;
}

/*
================
TGSoundManager::RemoveSound
================
*/
// FUNCTION: STBC 0x007109b0
void TGSoundManager::RemoveSound( TGSound *sound ) {
	TGSound *filed;
	if ( sounds.GetAt( sound->name, filed ) ) {
		sounds.RemoveAt( sound->name );
	}
}

/*
================
TGSoundManager::AllocateSamples

Builds the provider's pool and fills it with up to count sample handles:
3D samples of the provider, or the driver's 2D samples for no provider.
Returns the handles allocated; 0 when the provider already has a pool.
================
*/
// FUNCTION: STBC 0x00710a80
int TGSoundManager::AllocateSamples( NiProvider_Info *provider, int count ) {
	if ( count < 1 ) {
		return 0;
	}

	TGSamplePool *pool;
	if ( samplePools.GetAt( provider, pool ) ) {
		return 0;
	}
	pool = new TGSamplePool;
	samplePools.SetAt( provider, pool );

	NiSoundSystem *soundSystem = NiSoundSystem::GetSoundSystem();
	int i = 0;
	do {
		i++;
		void *sample;
		if ( provider != NULL ) {
			sample = AIL_allocate_3D_sample_handle( provider->GetProvider() );
		} else {
			sample = AIL_allocate_sample_handle( soundSystem->GetDriver() );
		}
		if ( sample == NULL ) {
			break;
		}
		pool->freeSamples.AddTail( sample );
	} while ( i < count );
	return pool->freeSamples.GetSize();
}

/*
================
TGSoundManager::GetNumSamples

The provider's sample handles, free and handed out; 0 for a provider
without a pool.
================
*/
// FUNCTION: STBC 0x00710c80
int TGSoundManager::GetNumSamples( NiProvider_Info *provider ) {
	int count = 0;
	TGSamplePool *pool;
	if ( samplePools.GetAt( provider, pool ) ) {
		count = pool->usedSamples.GetSize() + pool->freeSamples.GetSize();
	}
	return count;
}

/*
================
TGSoundManager::GetNumFreeSamples
================
*/
// FUNCTION: STBC 0x00710ce0
int TGSoundManager::GetNumFreeSamples( NiProvider_Info *provider ) {
	int count = 0;
	TGSamplePool *pool;
	if ( samplePools.GetAt( provider, pool ) ) {
		count = pool->freeSamples.GetSize();
	}
	return count;
}

/*
================
TGSoundManager::GetSample

Hands out a free sample handle of the first provider of the reserve list
that has one, and the provider; nothing when none has.
================
*/
// FUNCTION: STBC 0x00710d40
void TGSoundManager::GetSample( void **sample, NiProvider_Info **provider ) {
	NiTListIterator pos = reserves.GetHeadPos();
	while ( pos != NULL ) {
		TGProviderReserve *reserve = reserves.GetNext( pos );
		*provider = FindProvider( reserve->providerName );
		if ( *provider == NULL ) {
			continue;
		}
		TGSamplePool *pool;
		if ( samplePools.GetAt( *provider, pool ) && pool->freeSamples.GetSize() != 0 ) {
			*sample = pool->freeSamples.RemoveHead();
			pool->usedSamples.AddTail( *sample );
			return;
		}
	}
}

/*
================
TGSoundManager::ReleaseSample

Takes a handed-out sample handle back; false when no pool handed it out.
================
*/
// FUNCTION: STBC 0x00710e60
bool TGSoundManager::ReleaseSample( void *sample ) {
	NiTListIterator pos = reserves.GetHeadPos();
	while ( pos != NULL ) {
		TGProviderReserve *reserve = reserves.GetNext( pos );
		NiProvider_Info *provider = FindProvider( reserve->providerName );
		if ( provider == NULL ) {
			continue;
		}
		TGSamplePool *pool;
		if ( !samplePools.GetAt( provider, pool ) ) {
			continue;
		}
		NiTListIterator samplePos = pool->usedSamples.FindPos( sample );
		if ( samplePos != NULL ) {
			pool->usedSamples.RemovePos( samplePos );
			pool->freeSamples.AddTail( sample );
			return true;
		}
	}
	return false;
}

/*
================
TGSoundManager::FreeSamples

Releases the provider's sample handles, free and handed out, and its pool.
The NULL provider is the pool of the 2D samples.
================
*/
// FUNCTION: STBC 0x00710ff0
void TGSoundManager::FreeSamples( NiProvider_Info *provider ) {
	TGSamplePool *pool;
	if ( !samplePools.GetAt( provider, pool ) ) {
		return;
	}
	while ( pool->freeSamples.GetSize() != 0 ) {
		if ( provider != NULL ) {
			AIL_release_3D_sample_handle( pool->freeSamples.RemoveHead() );
		} else {
			AIL_release_sample_handle( pool->freeSamples.RemoveHead() );
		}
	}
	while ( pool->usedSamples.GetSize() != 0 ) {
		if ( provider != NULL ) {
			AIL_release_3D_sample_handle( pool->usedSamples.RemoveHead() );
		} else {
			AIL_release_sample_handle( pool->usedSamples.RemoveHead() );
		}
	}
	delete pool;
	samplePools.RemoveAt( provider );
}

/*
================
TGSoundManager::AllocateSoundID

Finds a slot for the sound: a free one, else one of a stopped sound, else
the one of the lowest priority below the sound's, whose sound is stopped.
The ID of the slot; the invalid ID when every sound playing has a
priority at least the sound's.
================
*/
// FUNCTION: STBC 0x00711230
long TGSoundManager::AllocateSoundID( TGSound *sound ) {
	long id = unknown896A9C;
	float lowestPriority = 1.0f;
	int lowestSlot = -1;
	int slot = -1;
	bool free = true;
	int i;
	for ( i = 0; i < maxSoundsAtOnce; i++ ) {
		TGSound *playing = playingSounds[i];
		if ( playing == NULL ) {
			slot = i;
			free = IsSlotReserved( i );
			if ( !free ) {
				break;
			}
			continue;
		}
		if ( playing == sound ) {
			slot = i;
			free = IsSlotReserved( i );
			break;
		}
		if ( !playing->paused && playing->GetStatus() != 0 ) {
			slot = i;
			free = IsSlotReserved( i );
			if ( !free ) {
				break;
			}
		}
		if ( playing->priority < lowestPriority ) {
			lowestSlot = i;
			lowestPriority = playing->priority;
		}
	}

	if ( slot == -1 && sound->priority > lowestPriority ) {
		slot = lowestSlot;
		free = IsSlotReserved( slot );
	}

	if ( slot != -1 ) {
		if ( free ) {
			slotIDBase += maxSoundsAtOnce;
			memset( slotReserved, 0, maxSoundsAtOnce );
		}
		if ( playingSounds[slot] != NULL ) {
			playingSounds[slot]->Stop();
		}
		playingSounds[slot] = NULL;
		id = GetSlotID( slot );
		slotReserved[slot] = true;
	}
	return id;
}

/*
================
TGSoundManager::SetPlayingSound

Puts the sound in the slot of the ID.
================
*/
// FUNCTION: STBC 0x00711370
void TGSoundManager::SetPlayingSound( TGSound *sound, long id ) {
	int slot = GetSlot( id );
	if ( slot != -1 ) {
		playingSounds[slot] = sound;
		sound->id = id;
	}
}

/*
================
TGSoundManager::IsSlotReserved
================
*/
// FUNCTION: STBC 0x007113a0
bool TGSoundManager::IsSlotReserved( int slot ) {
	return slotReserved[slot] ? true : false;
}

/*
================
TGSoundManager::ReleaseSoundID

Empties the sound's slot; a copy goes on the list of the stopped copies.
================
*/
// FUNCTION: STBC 0x007113c0
void TGSoundManager::ReleaseSoundID( TGSound *sound ) {
	int slot = GetSlot( sound->id );
	if ( slot < 0 || playingSounds[slot] == NULL || playingSounds[slot] != sound ) {
		return;
	}
	if ( sound->unknown11 ) {
		stoppedSounds.AddTail( sound );
	}
	playingSounds[slot] = NULL;
	sound->id = unknown896A9C;
}

/*
================
TGSoundManager::HandleDeleteSoundEvent
================
*/
// FUNCTION: STBC 0x00711460
void TGSoundManager::HandleDeleteSoundEvent( TGStringEvent *event ) {
	char *name = event->GetCString();
	DeleteSound( name );
	delete name;
	CallNextHandler( event );
}

/*
================
TGSoundManager::DeleteStoppedSounds
================
*/
// FUNCTION: STBC 0x007114a0
void TGSoundManager::DeleteStoppedSounds( void ) {
	while ( stoppedSounds.GetSize() != 0 ) {
		TGSound *sound = stoppedSounds.RemoveHead();
		delete sound;
	}
}

/*
================
TGSoundManager::GetSlot

The slot of the ID; -1 for the invalid ID or when there are no slots.
================
*/
// FUNCTION: STBC 0x00711510
int TGSoundManager::GetSlot( long id ) {
	if ( id == unknown896A9C || maxSoundsAtOnce == 0 ) {
		return -1;
	}
	return ( id - unknown896AA0 ) % maxSoundsAtOnce;
}

/*
================
TGSoundManager::GetSlotID

The ID the slot hands out in the current round.
================
*/
// FUNCTION: STBC 0x00711540
long TGSoundManager::GetSlotID( int slot ) {
	return slotIDBase + unknown896AA0 + slot;
}

/*
================
TGSoundManager::WriteToStream

The filed sounds, then the slots and the playing copies.
================
*/
// FUNCTION: STBC 0x00711560
bool TGSoundManager::WriteToStream( TGStream *stream ) {
	if ( !TGEventHandlerObject::WriteToStream( stream ) ) {
		return false;
	}

	NiTMapIterator pos = sounds.GetFirstPos();
	while ( pos != NULL ) {
		const char *name;
		TGSound *sound;
		sounds.GetNext( pos, name, sound );
		if ( sound == NULL ) {
			continue;
		}

		char text[256];
		sprintf( text, "Saving sound %s (filename %s)\n", name, sound->fileName );
		TGObject::Log( text );

		stream->WriteBool( true );
		stream->VirtualSlot11( (char *)name );
		stream->VirtualSlot11( sound->fileName );
		long flags = 0;
		if ( sound->Is3D() ) {
			flags = 1;
		}
		if ( sound->streamed ) {
			flags |= 2;
		} else if ( !sound->loaded ) {
			flags |= 4;
		}
		stream->WriteLong( flags );
		stream->VirtualSlot11( sound->group );
		stream->VirtualSlot17( (long)sound );
		stream->WriteBool( sound->paused );
		stream->WriteBool( sound->sfx );
		stream->WriteBool( sound->voice );
		stream->WriteBool( sound->interfaceSound );
		stream->WriteFloat( sound->priority );
	}
	stream->WriteBool( false );

	stream->WriteInt( maxSoundsAtOnce );
	stream->WriteFloat( sfxAdjustment );
	stream->WriteFloat( sfxAdjustmentMinimum );
	for ( int i = 0; i < maxSoundsAtOnce; i++ ) {
		if ( playingSounds[i] != NULL ) {
			stream->WriteBool( true );
			playingSounds[i]->WriteToStream( stream );
		}
	}
	stream->WriteBool( false );

	stream->WriteBool( paused );
	return true;
}

/*
================
TGSoundManager::ReadFromStream
================
*/
// FUNCTION: STBC 0x00711750
bool TGSoundManager::ReadFromStream( TGStream *stream ) {
	if ( !TGEventHandlerObject::ReadFromStream( stream ) ) {
		return false;
	}

	while ( stream->ReadBool() == true ) {
		char *name = stream->VirtualSlot10( -1 );
		char *fileName = stream->VirtualSlot10( -1 );
		long flags = stream->ReadLong();
		TGSound *sound = new TGSound( fileName, name, flags );
		char *group = stream->VirtualSlot10( -1 );
		if ( group != NULL ) {
			sound->SetGroup( group );
			delete group;
		}
		pointerMap.Add( stream->VirtualSlot16(), sound );
		if ( GetSound( name ) == NULL ) {
			TGObject::Log( "Error loading sound during " );
		}
		sound->paused = stream->ReadBool();
		sound->sfx = stream->ReadBool();
		sound->voice = stream->ReadBool();
		sound->interfaceSound = stream->ReadBool();
		sound->priority = stream->ReadFloat();

		char text[256];
		sprintf( text, "Loading sound %s (filename %s)\n", name, fileName );
		TGObject::Log( text );
	}

	maxSoundsAtOnce = stream->ReadInt();
	sfxAdjustment = stream->ReadFloat();
	sfxAdjustmentMinimum = stream->ReadFloat();
	playingSounds = new TGSound *[maxSoundsAtOnce];
	memset( playingSounds, 0, maxSoundsAtOnce * sizeof( TGSound * ) );
	for ( int i = 0; stream->ReadBool() == true; i++ ) {
		TGSound *sound = new TGSound( GetSound( stream->VirtualSlot10( -1 ) ) );
		sound->ReadFromStream( stream );
		playingSounds[i] = sound;
	}

	paused = stream->ReadBool();
	soundManagerReady = true;
	return true;
}

/*
================
TGSoundManager::FixUpPointers
================
*/
// FUNCTION: STBC 0x00711a50
bool TGSoundManager::FixUpPointers( void ) {
	for ( int i = 0; i < maxSoundsAtOnce; i++ ) {
		if ( playingSounds[i] != NULL && !playingSounds[i]->FixUpPointers() ) {
			return false;
		}
	}
	return true;
}

/*
================
TGSoundManager::PostFixUp
================
*/
// FUNCTION: STBC 0x00711a90
void TGSoundManager::PostFixUp( void ) {
	for ( int i = 0; i < maxSoundsAtOnce; i++ ) {
		if ( playingSounds[i] != NULL ) {
			playingSounds[i]->PostFixUp();
		}
	}
}

/*
================
TGSoundManager::RestorePlayingSounds

Moves the sounds read back to the slots of their IDs and restarts the
unpaused ones where they were; a sound whose ID has no slot is deleted.
================
*/
// FUNCTION: STBC 0x00711ac0
void TGSoundManager::RestorePlayingSounds( void ) {
	TGSound **saved = new TGSound *[maxSoundsAtOnce];
	memcpy( saved, playingSounds, maxSoundsAtOnce * sizeof( TGSound * ) );
	memset( playingSounds, 0, maxSoundsAtOnce * sizeof( TGSound * ) );

	for ( int i = 0; i < maxSoundsAtOnce; i++ ) {
		if ( saved[i] == NULL ) {
			continue;
		}
		TGSound *sound = saved[i];
		int slot = GetSlot( sound->id );
		if ( slot == -1 ) {
			delete sound;
			continue;
		}
		playingSounds[slot] = sound;
		if ( !sound->paused ) {
			float position = sound->GetPlayPositionSeconds();
			sound->Unknown70BB80( false );
			sound->SetPlayPositionSeconds( position );
		}
	}
	delete saved;
}

/*
================
TGSoundManager::ForgetAll

Forgets the sound slots without deleting them.
================
*/
// FUNCTION: STBC 0x00711ba0
void TGSoundManager::ForgetAll( void ) {
	maxSoundsAtOnce = 0;
	playingSounds = NULL;
	slotReserved = NULL;
	slotIDBase = 0;
}

// The four constants the sound code reads from memory (invented names): the
// rate at which the effects' factor moves, the ID of no playing sound, the
// ID of the first slot of the first round and the slots at first.
// DATA: STBC 0x00896a98
extern const float unknown896A98 = 5.0f;
// DATA: STBC 0x00896a9c
extern const int unknown896A9C = 0;
// DATA: STBC 0x00896aa0
extern const unsigned long unknown896AA0 = 1;
// DATA: STBC 0x00896aa4
extern const int unknown896AA4 = 32;
