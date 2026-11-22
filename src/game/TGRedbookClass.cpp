/*
===============================================================================

	TGRedbookClass.cpp

===============================================================================
*/

#include <iostream>
#include <windows.h>
#include <mmsystem.h>
#include <string.h>

#include "TGRedbookClass.h"
#include "TGSystemWrapperClass.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00712e80 _$E421
// FUNCTION: STBC 0x00712ea0 _$E420

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00712d60 _$E32
// FUNCTION: STBC 0x00712d70 _$E29
// FUNCTION: STBC 0x00712d80 _$E31
// FUNCTION: STBC 0x00712d90 _$E30
// FUNCTION: STBC 0x00712da0 _$E38
// FUNCTION: STBC 0x00712db0 _$E35
// FUNCTION: STBC 0x00712dc0 _$E37
// FUNCTION: STBC 0x00712dd0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00712de0 _$E45
// FUNCTION: STBC 0x00712df0 _$E44
// FUNCTION: STBC 0x00712e00 _$E49
// FUNCTION: STBC 0x00712e10 _$E48
// FUNCTION: STBC 0x00712e20 _$E60
// FUNCTION: STBC 0x00712e30 _$E59
// FUNCTION: STBC 0x00712e40 _$E64
// FUNCTION: STBC 0x00712e50 _$E63
// FUNCTION: STBC 0x00712e60 _$E68
// FUNCTION: STBC 0x00712e70 _$E67
// FUNCTION: STBC 0x00712eb0 _$E72
// FUNCTION: STBC 0x00712ec0 _$E71
// FUNCTION: STBC 0x00712ed0 _$E196
// FUNCTION: STBC 0x00712ee0 _$E195
// FUNCTION: STBC 0x00712ef0 _$E138
// FUNCTION: STBC 0x00712f00 _$E137
// FUNCTION: STBC 0x00712f10 _$E151
// FUNCTION: STBC 0x00712f20 _$E150
// FUNCTION: STBC 0x00712f30 _$E201
// FUNCTION: STBC 0x00712f40 _$E200
// FUNCTION: STBC 0x00712f50 _$E206
// FUNCTION: STBC 0x00712f60 _$E205
// FUNCTION: STBC 0x00712f70 _$E210
// FUNCTION: STBC 0x00712f80 _$E209
// FUNCTION: STBC 0x00712f90 _$E214
// FUNCTION: STBC 0x00712fa0 _$E213
// FUNCTION: STBC 0x00712fb0 _$E221
// FUNCTION: STBC 0x00712fc0 _$E220
// FUNCTION: STBC 0x00712fd0 _$E246
// FUNCTION: STBC 0x00712fe0 _$E245
// FUNCTION: STBC 0x00712ff0 _$E250
// FUNCTION: STBC 0x00713000 _$E249
// FUNCTION: STBC 0x00713010 _$E96
// FUNCTION: STBC 0x00713020 _$E95
// FUNCTION: STBC 0x00713030 _$E232
// FUNCTION: STBC 0x00713040 _$E231
// FUNCTION: STBC 0x00713050 _$E239
// FUNCTION: STBC 0x00713060 _$E238
// FUNCTION: STBC 0x00713070 _$E262
// FUNCTION: STBC 0x00713080 _$E261
// FUNCTION: STBC 0x00713090 _$E266
// FUNCTION: STBC 0x007130a0 _$E265
// FUNCTION: STBC 0x007130b0 _$E306
// FUNCTION: STBC 0x007130c0 _$E305
// FUNCTION: STBC 0x007130d0 _$E312
// FUNCTION: STBC 0x007130e0 _$E311
// FUNCTION: STBC 0x007130f0 _$E254
// FUNCTION: STBC 0x00713100 _$E253
// FUNCTION: STBC 0x00713110 _$E258
// FUNCTION: STBC 0x00713120 _$E257
// FUNCTION: STBC 0x00713130 _$E270
// FUNCTION: STBC 0x00713140 _$E269
// FUNCTION: STBC 0x00713150 _$E274
// FUNCTION: STBC 0x00713160 _$E273
// FUNCTION: STBC 0x00713170 _$E278
// FUNCTION: STBC 0x00713180 _$E277
// FUNCTION: STBC 0x00713190 _$E282
// FUNCTION: STBC 0x007131a0 _$E281
// FUNCTION: STBC 0x007131b0 _$E286
// FUNCTION: STBC 0x007131c0 _$E285
// FUNCTION: STBC 0x007131d0 _$E290
// FUNCTION: STBC 0x007131e0 _$E289
// FUNCTION: STBC 0x007131f0 _$E294
// FUNCTION: STBC 0x00713200 _$E293
// FUNCTION: STBC 0x00713210 _$E298
// FUNCTION: STBC 0x00713220 _$E297
// FUNCTION: STBC 0x00713230 _$E101
// FUNCTION: STBC 0x00713240 _$E100
// FUNCTION: STBC 0x00713250 _$E302
// FUNCTION: STBC 0x00713260 _$E301
// FUNCTION: STBC 0x00713270 _$E106
// FUNCTION: STBC 0x00713280 _$E105
// FUNCTION: STBC 0x00713290 _$E316
// FUNCTION: STBC 0x007132a0 _$E315
// FUNCTION: STBC 0x007132b0 _$E320
// FUNCTION: STBC 0x007132c0 _$E319
// FUNCTION: STBC 0x007132d0 _$E324
// FUNCTION: STBC 0x007132e0 _$E323
// FUNCTION: STBC 0x007132f0 _$E328
// FUNCTION: STBC 0x00713300 _$E327
// FUNCTION: STBC 0x00713310 _$E115
// FUNCTION: STBC 0x00713320 _$E114
// FUNCTION: STBC 0x00713330 _$E332
// FUNCTION: STBC 0x00713340 _$E331
// FUNCTION: STBC 0x00713350 _$E336
// FUNCTION: STBC 0x00713360 _$E335
// FUNCTION: STBC 0x00713370 _$E340
// FUNCTION: STBC 0x00713380 _$E339
// FUNCTION: STBC 0x00713390 _$E121
// FUNCTION: STBC 0x007133a0 _$E120
// FUNCTION: STBC 0x007133b0 _$E126
// FUNCTION: STBC 0x007133c0 _$E125
// FUNCTION: STBC 0x007133d0 _$E156
// FUNCTION: STBC 0x007133e0 _$E155
// FUNCTION: STBC 0x007133f0 _$E344
// FUNCTION: STBC 0x00713400 _$E343
// FUNCTION: STBC 0x00713410 _$E161
// FUNCTION: STBC 0x00713420 _$E160
// FUNCTION: STBC 0x00713430 _$E348
// FUNCTION: STBC 0x00713440 _$E347
// FUNCTION: STBC 0x00713450 _$E352
// FUNCTION: STBC 0x00713460 _$E351
// FUNCTION: STBC 0x00713470 _$E166
// FUNCTION: STBC 0x00713480 _$E165
// FUNCTION: STBC 0x00713490 _$E356
// FUNCTION: STBC 0x007134a0 _$E355
// FUNCTION: STBC 0x007134b0 _$E131
// FUNCTION: STBC 0x007134c0 _$E130
// FUNCTION: STBC 0x007134d0 _$E360
// FUNCTION: STBC 0x007134e0 _$E359
// FUNCTION: STBC 0x007134f0 _$E364
// FUNCTION: STBC 0x00713500 _$E363
// FUNCTION: STBC 0x00713510 _$E171
// FUNCTION: STBC 0x00713520 _$E170
// FUNCTION: STBC 0x00713530 _$E176
// FUNCTION: STBC 0x00713540 _$E175
// FUNCTION: STBC 0x00713550 _$E368
// FUNCTION: STBC 0x00713560 _$E367
// FUNCTION: STBC 0x00713570 _$E372
// FUNCTION: STBC 0x00713580 _$E371
// FUNCTION: STBC 0x00713590 _$E376
// FUNCTION: STBC 0x007135a0 _$E375
// FUNCTION: STBC 0x007135b0 _$E380
// FUNCTION: STBC 0x007135c0 _$E379
// FUNCTION: STBC 0x007135d0 _$E384
// FUNCTION: STBC 0x007135e0 _$E383
// FUNCTION: STBC 0x007135f0 _$E388
// FUNCTION: STBC 0x00713600 _$E387
// FUNCTION: STBC 0x00713610 _$E392
// FUNCTION: STBC 0x00713620 _$E391
// FUNCTION: STBC 0x00713630 _$E396
// FUNCTION: STBC 0x00713640 _$E395
// FUNCTION: STBC 0x00713650 _$E181
// FUNCTION: STBC 0x00713660 _$E180
// FUNCTION: STBC 0x00713670 _$E400
// FUNCTION: STBC 0x00713680 _$E399
// FUNCTION: STBC 0x00713690 _$E186
// FUNCTION: STBC 0x007136a0 _$E185
// FUNCTION: STBC 0x007136b0 _$E191
// FUNCTION: STBC 0x007136c0 _$E190

// DATA: STBC 0x0099bcc0
TGRedbookClass g_kRedbook;

/*
================
TGRedbookClass::OpenRedbookCD

Closes the device it had, then opens the CD audio device in the track,
minute, second, frame time format and reads the length of every track.
False when MCI refuses or there is no window for it to notify.
================
*/
// FUNCTION: STBC 0x00712770 ?OpenRedbookCD@TGRedbookClass@@QAE_NXZ
bool TGRedbookClass::OpenRedbookCD( void ) {
	if ( g_kSystemWrapper.GetWindowHandle() != 0 ) {
		if ( device != 0 ) {
			mciSendCommandA( device, MCI_CLOSE, 0, 0 );
			device = 0;
			memset( trackLengths, 0, sizeof( trackLengths ) );
			currentTrack = 0;
			finished = false;
		}
		savedVolume = -1;
		UINT numDevices = auxGetNumDevs();
		for ( UINT deviceNumber = 0; deviceNumber < numDevices; deviceNumber++ ) {
			AUXCAPS caps;
			memset( &caps, 0, sizeof( caps ) );
			auxGetDevCapsA( deviceNumber, &caps, sizeof( caps ) );
			DWORD volume;
			if ( caps.wTechnology == AUXCAPS_CDAUDIO && ( caps.dwSupport & AUXCAPS_VOLUME ) && auxGetVolume( deviceNumber, &volume ) == 0 ) {
				savedVolume = volume & 0xffff;
				break;
			}
		}

		MCI_OPEN_PARMS open;
		open.lpstrDeviceType = "cdaudio";
		if ( mciSendCommandA( 0, MCI_OPEN, MCI_OPEN_TYPE, (DWORD)&open ) != 0 ) {
			device = 0;
			return false;
		}
		device = open.wDeviceID;
		MCI_SET_PARMS set;
		set.dwTimeFormat = MCI_FORMAT_TMSF;
		if ( mciSendCommandA( device, MCI_SET, MCI_SET_TIME_FORMAT, (DWORD)&set ) != 0 ) {
			mciSendCommandA( device, MCI_CLOSE, 0, 0 );
			device = 0;
			return false;
		}
		MCI_STATUS_PARMS status;
		status.dwItem = MCI_STATUS_NUMBER_OF_TRACKS;
		if ( mciSendCommandA( device, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)&status ) != 0 ) {
			mciSendCommandA( device, MCI_CLOSE, 0, 0 );
			device = 0;
			return false;
		}
		numTracks = status.dwReturn;
		for ( unsigned int track = 1; track <= (unsigned int)numTracks; track++ ) {
			status.dwItem = MCI_STATUS_LENGTH;
			status.dwTrack = track;
			if ( mciSendCommandA( device, MCI_STATUS, MCI_STATUS_ITEM | MCI_TRACK, (DWORD)&status ) != 0 ) {
				mciSendCommandA( device, MCI_CLOSE, 0, 0 );
				device = 0;
				return false;
			}
			trackLengths[track - 1] = status.dwReturn;
		}
		return true;
	}
	return false;
}

/*
================
TGRedbookClass::PlayRedbookCDTrack

Plays the track from the time given to its end. The end is the game time
plus the track's length and a margin of two seconds.
================
*/
// FUNCTION: STBC 0x00712920 ?PlayRedbookCDTrack@TGRedbookClass@@QAE_NHHH_N@Z
bool TGRedbookClass::PlayRedbookCDTrack( int track, int minutes, int seconds, bool looping_ ) {
	if ( track <= numTracks && track > 0 && device != 0 ) {
		MCI_PLAY_PARMS play;
		memset( &play, 0, sizeof( play ) );
		play.dwFrom = MCI_MAKE_TMSF( track, minutes, seconds, 0 );
		DWORD length = trackLengths[track - 1];
		play.dwTo = MCI_MAKE_TMSF( track, MCI_MSF_MINUTE( length ), MCI_MSF_SECOND( length ), MCI_MSF_FRAME( length ) );
		play.dwCallback = (DWORD)g_kSystemWrapper.GetWindowHandle();
		if ( mciSendCommandA( device, MCI_PLAY, MCI_NOTIFY | MCI_FROM | MCI_TO, (DWORD)&play ) == 0 ) {
			currentTrack = track;
			int now = g_kSystemWrapper.GetTimeInMilliseconds();
			endTime = now + GetCDTrackLengthInMilliseconds( track ) + 2000;
			finished = false;
			looping = looping_;
			return true;
		}
	}
	return false;
}

/*
================
TGRedbookClass::StopRedbookCDTrack
================
*/
// FUNCTION: STBC 0x00712a10 ?StopRedbookCDTrack@TGRedbookClass@@QAE_NXZ
bool TGRedbookClass::StopRedbookCDTrack( void ) {
	if ( device != 0 && currentTrack != 0 ) {
		MCI_GENERIC_PARMS stop;
		mciSendCommandA( device, MCI_STOP, 0, (DWORD)&stop );
		currentTrack = 0;
		finished = false;
		return true;
	}
	return false;
}

/*
================
TGRedbookClass::CloseRedbookCD

Stops the track, closes the device and puts back the volume the CD output
had when it was opened.
================
*/
// FUNCTION: STBC 0x00712a60 ?CloseRedbookCD@TGRedbookClass@@QAE_NXZ
bool TGRedbookClass::CloseRedbookCD( void ) {
	MCI_GENERIC_PARMS stop;
	if ( device == 0 ) {
		return false;
	}
	if ( currentTrack != 0 ) {
		mciSendCommandA( device, MCI_STOP, 0, (DWORD)&stop );
		currentTrack = 0;
		finished = false;
	}
	mciSendCommandA( device, MCI_CLOSE, 0, 0 );
	device = 0;
	memset( trackLengths, 0, sizeof( trackLengths ) );
	currentTrack = 0;
	finished = false;
	int numDevices = auxGetNumDevs();
	if ( savedVolume != -1 ) {
		DWORD bothChannels = savedVolume * 0x10001;
		for ( int deviceNumber = 0; deviceNumber < numDevices; deviceNumber++ ) {
			AUXCAPS caps;
			memset( &caps, 0, sizeof( caps ) );
			auxGetDevCapsA( deviceNumber, &caps, sizeof( caps ) );
			if ( caps.wTechnology == AUXCAPS_CDAUDIO && ( caps.dwSupport & AUXCAPS_VOLUME ) ) {
				auxSetVolume( deviceNumber, bothChannels );
			}
		}
	}
	savedVolume = -1;
	return false;
}

/*
================
TGRedbookClass::IsRedbookCDTrackFinishedPlaying
================
*/
// FUNCTION: STBC 0x00712b40 ?IsRedbookCDTrackFinishedPlaying@TGRedbookClass@@QAE_NXZ
bool TGRedbookClass::IsRedbookCDTrackFinishedPlaying( void ) {
	if ( device != 0 && currentTrack != 0 ) {
		return finished;
	}
	return false;
}

/*
================
TGRedbookClass::GetCDTrackLengthInMilliseconds

0 for a track the disc does not have.
================
*/
// FUNCTION: STBC 0x00712b60 ?GetCDTrackLengthInMilliseconds@TGRedbookClass@@QAEHH@Z
int TGRedbookClass::GetCDTrackLengthInMilliseconds( int track ) {
	if ( device != 0 && track > 0 && track <= numTracks ) {
		return MCI_MSF_FRAME( trackLengths[track - 1] ) * 1000 / 75 + ( MCI_MSF_SECOND( trackLengths[track - 1] ) + MCI_MSF_MINUTE( trackLengths[track - 1] ) * 60 ) * 1000;
	}
	return 0;
}

/*
================
TGRedbookClass::SetRedbookCDVolume

Sets the volume (0 to 0xFFFF) of every CD audio output.
================
*/
// FUNCTION: STBC 0x00712be0 ?SetRedbookCDVolume@TGRedbookClass@@QAEXH@Z
void TGRedbookClass::SetRedbookCDVolume( int volume ) {
	UINT numDevices = auxGetNumDevs();
	if ( (unsigned int)volume > 0xffff ) {
		volume = 0xffff;
	}
	DWORD bothChannels = volume * 0x10001;
	for ( UINT deviceNumber = 0; deviceNumber < numDevices; deviceNumber++ ) {
		AUXCAPS caps;
		memset( &caps, 0, sizeof( caps ) );
		auxGetDevCapsA( deviceNumber, &caps, sizeof( caps ) );
		if ( caps.wTechnology == AUXCAPS_CDAUDIO && ( caps.dwSupport & AUXCAPS_VOLUME ) ) {
			auxSetVolume( deviceNumber, bothChannels );
		}
	}
}

/*
================
TGRedbookClass::FadeRedbookCDVolume

Steps the volume from one level to the other, 0x100 at a time, spread over
the time given. It waits for the game time to move, so it holds up the
caller for the whole fade.
================
*/
// FUNCTION: STBC 0x00712c60 ?FadeRedbookCDVolume@TGRedbookClass@@QAE_NHHH@Z
bool TGRedbookClass::FadeRedbookCDVolume( int from, int to, int milliseconds ) {
	if ( device != 0 && (unsigned int)from != (unsigned int)to ) {
		// milliseconds becomes the time between two steps of 0x100
		BOOL rising;
		if ( (unsigned int)from > (unsigned int)to ) {
			rising = FALSE;
			milliseconds = ( milliseconds << 8 ) / (unsigned int)( from - to );
		} else {
			rising = TRUE;
			milliseconds = ( milliseconds << 8 ) / (unsigned int)( to - from );
		}
		int last = g_kSystemWrapper.GetTimeInMilliseconds();
		do {
			int now = g_kSystemWrapper.GetTimeInMilliseconds();
			if ( now > last + milliseconds ) {
				if ( rising ) {
					from += 0x100;
					if ( (unsigned int)from > 0xffff ) {
						from = 0xffff;
					}
				} else if ( (unsigned int)from < 0x100 ) {
					from = 0;
				} else {
					from -= 0x100;
				}
				SetRedbookCDVolume( from );
				last = now;
			}
		} while ( ( rising && (unsigned int)from < (unsigned int)to ) || ( !rising && (unsigned int)from > (unsigned int)to ) );
	}
	return true;
}

/*
================
TGRedbookClass::UpdateRedbookCD

A track that has outlasted its end time starts over when it loops, else is
marked finished.
================
*/
// FUNCTION: STBC 0x00712d20 ?UpdateRedbookCD@TGRedbookClass@@QAEXXZ
void TGRedbookClass::UpdateRedbookCD( void ) {
	if ( currentTrack != 0 ) {
		if ( (unsigned long)g_kSystemWrapper.GetTimeInMilliseconds() > endTime ) {
			if ( looping ) {
				PlayRedbookCDTrack( currentTrack, 0, 0, looping );
				return;
			}
			finished = true;
		}
	}
}
