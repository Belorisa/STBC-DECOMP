/*
===============================================================================

	TGAnimTrack.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "TGAnimTrack.h"
#include "TGAnimBlender.h"
#include "NiLinRotKey.h"
#include "NiLinPosKey.h"
#include "NiLinFloatKey.h"
#include "NiKeyframeController.h"
#include "NiVisController.h"
#include "NiCamera.h"
#include "renderer/NiRender.h"
#include "NiScreenPolygon.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006ca620 _$E45
// FUNCTION: STBC 0x006ca630 _$E44
// FUNCTION: STBC 0x006ca640 _$E91
// FUNCTION: STBC 0x006ca650 _$E90
// FUNCTION: STBC 0x006ca660 _$E71
// FUNCTION: STBC 0x006ca670 _$E70
// FUNCTION: STBC 0x006ca680 _$E80
// FUNCTION: STBC 0x006ca690 _$E79
// FUNCTION: STBC 0x006ca6a0 _$E95
// FUNCTION: STBC 0x006ca6b0 _$E94
// FUNCTION: STBC 0x006ca6c0 _$E99
// FUNCTION: STBC 0x006ca6d0 _$E98
// FUNCTION: STBC 0x006ca6e0 _$E242
// FUNCTION: STBC 0x006ca6f0 _$E241
// FUNCTION: STBC 0x006ca700 _$E245
// FUNCTION: STBC 0x006ca710 _$E244
// FUNCTION: STBC 0x006ca720 _$E248
// FUNCTION: STBC 0x006ca730 _$E247
// FUNCTION: STBC 0x006ca740 _$E103
// FUNCTION: STBC 0x006ca750 _$E102
// FUNCTION: STBC 0x006ca760 _$E107
// FUNCTION: STBC 0x006ca770 _$E106
// FUNCTION: STBC 0x006ca780 _$E111
// FUNCTION: STBC 0x006ca790 _$E110
// FUNCTION: STBC 0x006ca7a0 _$E228
// FUNCTION: STBC 0x006ca7b0 _$E227
// FUNCTION: STBC 0x006ca7e0 _$E237
// FUNCTION: STBC 0x006ca7f0 _$E236
// FUNCTION: STBC 0x006ca800 _$E252
// FUNCTION: STBC 0x006ca810 _$E251
// FUNCTION: STBC 0x006ca820 _$E256
// FUNCTION: STBC 0x006ca830 _$E255
// FUNCTION: STBC 0x006ca840 _$E260
// FUNCTION: STBC 0x006ca850 _$E259

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006ca570 _$E32
// FUNCTION: STBC 0x006ca580 _$E29
// FUNCTION: STBC 0x006ca590 _$E31
// FUNCTION: STBC 0x006ca5a0 _$E30
// FUNCTION: STBC 0x006ca5b0 _$E38
// FUNCTION: STBC 0x006ca5c0 _$E35
// FUNCTION: STBC 0x006ca5d0 _$E37
// FUNCTION: STBC 0x006ca5e0 _$E36

/*
================
TGAnimTrack::TGAnimTrack
================
*/
// FUNCTION: STBC 0x006c9be0 ??0TGAnimTrack@@QAE@XZ
TGAnimTrack::TGAnimTrack( void ) {
	keyframeController = NULL;
	visController = NULL;
	data = NULL;
	rotKeys = NULL;
	posKeys = NULL;
	scaleKeys = NULL;
	frameCount = 0;
	time = 0.0f;
	duration = 0.0f;
	name = NULL;
	nextAnimation = NULL;
	flags = DONE | UNKNOWN_FLAG;
}

/*
================
TGAnimTrack::~TGAnimTrack

Frees the names and the three arrays of keys.
================
*/
// FUNCTION: STBC 0x006c9c60 ??1TGAnimTrack@@QAE@XZ
TGAnimTrack::~TGAnimTrack( void ) {
	int i;

	if ( name ) {
		delete [] name;
	}
	if ( nextAnimation ) {
		delete [] nextAnimation;
	}
	data->ReplaceRotAnim( NULL, 0, NiRotKey::NOINTERP );
	data->ReplacePosAnim( NULL, 0, NiPosKey::NOINTERP );
	data->ReplaceScaleAnim( NULL, 0, NiFloatKey::NOINTERP );
	data = NULL;
	if ( rotKeys ) {
		for ( i = 0; i < frameCount; i++ ) {
			delete rotKeys[i];
		}
		delete [] rotKeys;
	}
	if ( posKeys ) {
		for ( i = 0; i < frameCount; i++ ) {
			delete posKeys[i];
		}
		delete [] posKeys;
	}
	if ( scaleKeys ) {
		for ( i = 0; i < frameCount; i++ ) {
			delete scaleKeys[i];
		}
		delete [] scaleKeys;
	}
}

/*
================
TGAnimTrack::SetFrameCount

Resizes the three arrays of keys to the frame count: the keys of the frames
that remain are kept, the others are dropped and the new frames start at the
time of the frame before them.
================
*/
// FUNCTION: STBC 0x006c9dd0 ?SetFrameCount@TGAnimTrack@@QAEXH@Z
void TGAnimTrack::SetFrameCount( int frames ) {
	int i;

	if ( frames == frameCount ) {
		return;
	}
	if ( data == NULL ) {
		data = new NiKeyframeData;
	} else {
		data->ReplaceRotAnim( NULL, 0, NiRotKey::NOINTERP );
		data->ReplacePosAnim( NULL, 0, NiPosKey::NOINTERP );
		data->ReplaceScaleAnim( NULL, 0, NiFloatKey::NOINTERP );
	}

	NiRotKey **newRotKeys = new NiRotKey *[frames];
	if ( data ) {
		i = 0;
		while ( i < frames && i < frameCount ) {
			newRotKeys[i] = rotKeys[i];
			i++;
		}
		while ( i < frameCount ) {
			delete rotKeys[i];
			i++;
		}
	} else {
		i = 0;
	}
	for ( ; i < frames; i++ ) {
		NiRotKey *key = new NiRotKey;
		newRotKeys[i] = key;
		if ( i > 0 ) {
			key->time = newRotKeys[i - 1]->GetTime();
		} else {
			key->time = 0.0f;
		}
	}
	if ( rotKeys ) {
		delete [] rotKeys;
	}
	rotKeys = newRotKeys;

	NiPosKey **newPosKeys = new NiPosKey *[frames];
	if ( data ) {
		i = 0;
		while ( i < frames && i < frameCount ) {
			newPosKeys[i] = posKeys[i];
			i++;
		}
		while ( i < frameCount ) {
			delete posKeys[i];
			i++;
		}
	} else {
		i = 0;
	}
	for ( ; i < frames; i++ ) {
		NiPosKey *key = new NiPosKey;
		newPosKeys[i] = key;
		if ( i > 0 ) {
			key->time = newPosKeys[i - 1]->GetTime();
		} else {
			key->time = 0.0f;
		}
	}
	if ( posKeys ) {
		delete [] posKeys;
	}
	posKeys = newPosKeys;

	NiFloatKey **newScaleKeys = new NiFloatKey *[frames];
	if ( data ) {
		i = 0;
		while ( i < frames && i < frameCount ) {
			newScaleKeys[i] = scaleKeys[i];
			i++;
		}
		while ( i < frameCount ) {
			delete scaleKeys[i];
			i++;
		}
	} else {
		i = 0;
	}
	for ( ; i < frames; i++ ) {
		NiFloatKey *key = new NiFloatKey;
		newScaleKeys[i] = key;
		if ( i > 0 ) {
			key->time = newScaleKeys[i - 1]->GetTime();
		} else {
			key->time = 0.0f;
		}
	}
	if ( scaleKeys ) {
		delete [] scaleKeys;
	}
	scaleKeys = newScaleKeys;
	frameCount = frames;
}

/*
================
TGAnimTrack::Update

Once the animation has run out: stops the controllers, then starts the next
animation, or loops, or lets the controllers go.
================
*/
// FUNCTION: STBC 0x006ca0b0 ?Update@TGAnimTrack@@QAEXMPAVTGAnimBlender@@@Z
void TGAnimTrack::Update( float currentTime, TGAnimBlender *blender ) {
	if ( duration - ( currentTime - time ) >= 0.0f ) {
		flags &= ~DONE;
		return;
	}
	if ( IsDone() ) {
		return;
	}
	if ( keyframeController ) {
		keyframeController->Update( currentTime );
	}
	if ( visController ) {
		visController->Update( currentTime );
	}
	if ( nextAnimation ) {
		blender->UseAnimation( nextAnimation, ( flags >> 2 ) & 1, time + duration, duration );
	} else if ( flags & LOOPING ) {
		time = time + duration;
		if ( keyframeController ) {
			keyframeController->phase = -time;
		}
		if ( visController ) {
			visController->phase = -time;
		}
	} else {
		flags |= DONE;
		if ( keyframeController ) {
			keyframeController->ClearData();
		}
	}
}

/*
================
TGAnimTrack::SetLooping
================
*/
// FUNCTION: STBC 0x006ca1a0 ?SetLooping@TGAnimTrack@@QAEX_N@Z
void TGAnimTrack::SetLooping( bool looping ) {
	if ( looping ) {
		flags |= LOOPING;
	} else {
		flags &= ~LOOPING;
	}
}

/*
================
TGAnimTrack::SetDone
================
*/
// FUNCTION: STBC 0x006ca1c0 ?SetDone@TGAnimTrack@@QAEX_N@Z
void TGAnimTrack::SetDone( bool done ) {
	if ( done ) {
		flags |= DONE;
	} else {
		flags &= ~DONE;
	}
}

/*
================
TGAnimTrack::SetUnknownFlag
================
*/
// FUNCTION: STBC 0x006ca1e0 ?SetUnknownFlag@TGAnimTrack@@QAEX_N@Z
void TGAnimTrack::SetUnknownFlag( bool flag ) {
	if ( flag ) {
		flags |= UNKNOWN_FLAG;
	} else {
		flags &= ~UNKNOWN_FLAG;
	}
}

/*
================
TGAnimTrack::CopyFrom

Makes this track's timing, flags and keys a copy of the track's.
================
*/
// FUNCTION: STBC 0x006ca200 ?CopyFrom@TGAnimTrack@@QAEXPAV1@@Z
void TGAnimTrack::CopyFrom( TGAnimTrack *track ) {
	int i;

	time = track->time;
	duration = track->duration;
	flags = track->flags;
	if ( track->data ) {
		if ( data == NULL ) {
			data = new NiKeyframeData;
		} else {
			data->ReplaceRotAnim( NULL, 0, NiRotKey::NOINTERP );
			data->ReplacePosAnim( NULL, 0, NiPosKey::NOINTERP );
			data->ReplaceScaleAnim( NULL, 0, NiFloatKey::NOINTERP );
		}
	}
	if ( track->rotKeys ) {
		if ( rotKeys ) {
			for ( i = 0; i < frameCount; i++ ) {
				delete rotKeys[i];
			}
			delete [] rotKeys;
		}
		NiRotKey::CopyFunction copyKey = NiRotKey::copyFunctions[NiRotKey::LINKEY];
		rotKeys = new NiRotKey *[track->frameCount];
		frameCount = track->frameCount;
		for ( i = 0; i < track->frameCount; i++ ) {
			rotKeys[i] = copyKey( track->rotKeys[i] );
		}
	} else {
		rotKeys = NULL;
		frameCount = 0;
	}
	if ( track->posKeys ) {
		if ( posKeys ) {
			for ( i = 0; i < frameCount; i++ ) {
				if ( posKeys[i] ) {
					delete posKeys[i];
				}
			}
			delete [] posKeys;
		}
		NiPosKey::CopyFunction copyKey = NiPosKey::copyFunctions[NiPosKey::LINKEY];
		posKeys = new NiPosKey *[track->frameCount];
		frameCount = track->frameCount;
		for ( i = 0; i < track->frameCount; i++ ) {
			posKeys[i] = copyKey( track->posKeys[i] );
		}
	} else {
		posKeys = NULL;
		frameCount = 0;
	}
	if ( track->scaleKeys ) {
		if ( scaleKeys ) {
			for ( i = 0; i < frameCount; i++ ) {
				if ( scaleKeys[i] ) {
					delete scaleKeys[i];
				}
			}
			delete [] scaleKeys;
		}
		NiFloatKey::CopyFunction copyKey = NiFloatKey::copyFunctions[NiFloatKey::LINKEY];
		scaleKeys = new NiFloatKey *[track->frameCount];
		frameCount = track->frameCount;
		for ( i = 0; i < track->frameCount; i++ ) {
			scaleKeys[i] = copyKey( track->scaleKeys[i], 0 );
		}
	} else {
		scaleKeys = NULL;
		frameCount = 0;
	}
}

/*
================
TGAnimTrack::SetName

Keeps a copy of the node's name.
================
*/
// FUNCTION: STBC 0x006ca470 ?SetName@TGAnimTrack@@QAEXPBD@Z
void TGAnimTrack::SetName( const char *name_ ) {
	if ( name ) {
		delete [] name;
		name = NULL;
	}
	if ( name_ ) {
		name = new char[strlen( name_ ) + 1];
		strcpy( name, name_ );
	}
}

/*
================
TGAnimTrack::SetNextAnimation

Keeps a copy of the animation to start when this one runs out.
================
*/
// FUNCTION: STBC 0x006ca4e0 ?SetNextAnimation@TGAnimTrack@@QAEXPBD@Z
void TGAnimTrack::SetNextAnimation( const char *animation ) {
	if ( nextAnimation ) {
		delete [] nextAnimation;
		nextAnimation = NULL;
	}
	if ( animation ) {
		nextAnimation = new char[strlen( animation ) + 1];
		strcpy( nextAnimation, animation );
	}
}
