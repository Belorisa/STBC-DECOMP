/*
===============================================================================

	BoneState.cpp

===============================================================================
*/

#include <iostream>
#include "BoneState.h"
#include "NiAVObject.h"
#include "NiKeyframeController.h"
#include "NiBone.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0065f7b0 _$E56
// FUNCTION: STBC 0x0065f7c0 _$E55
// FUNCTION: STBC 0x0065f7d0 _$E63
// FUNCTION: STBC 0x0065f7e0 _$E62
// FUNCTION: STBC 0x0065f7f0 _$E74
// FUNCTION: STBC 0x0065f800 _$E73
// FUNCTION: STBC 0x0065f810 _$E78
// FUNCTION: STBC 0x0065f820 _$E77
// FUNCTION: STBC 0x0065f830 _$E82
// FUNCTION: STBC 0x0065f840 _$E81
// FUNCTION: STBC 0x0065f850 _$E86
// FUNCTION: STBC 0x0065f860 _$E85
// FUNCTION: STBC 0x0065f870 _$E201
// FUNCTION: STBC 0x0065f880 _$E200
// FUNCTION: STBC 0x0065f8b0 _$E241
// FUNCTION: STBC 0x0065f8c0 _$E240
// FUNCTION: STBC 0x0065f8d0 _$E245
// FUNCTION: STBC 0x0065f8e0 _$E244

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0065f700 _$E32
// FUNCTION: STBC 0x0065f710 _$E29
// FUNCTION: STBC 0x0065f720 _$E31
// FUNCTION: STBC 0x0065f730 _$E30
// FUNCTION: STBC 0x0065f740 _$E38
// FUNCTION: STBC 0x0065f750 _$E35
// FUNCTION: STBC 0x0065f760 _$E37
// FUNCTION: STBC 0x0065f770 _$E36

// The locale-id pair (std::ctype<unsigned short>::id's guard and exit-time registration).
// FUNCTION: STBC 0x0065f780 _$E252
// FUNCTION: STBC 0x0065f7a0 _$E251

/*
================
BoneState::BoneState
================
*/
// FUNCTION: STBC 0x65F0A0
BoneState::BoneState() {
	bone = NULL;
}

/*
================
BoneState::~BoneState
================
*/
// FUNCTION: STBC 0x65F0E0
BoneState::~BoneState() {
	bone = NULL;
	int count = tracks.GetSize();
	for ( int i = 0; i < count; i++ ) {
		BoneKeyList *track = tracks.GetAt( i );
		if ( track != NULL ) {
			tracks.RemoveAt( i );
			delete track;
		}
	}
}

/*
================
BoneState::Reset

Rewinds to the start of the first track.
================
*/
// FUNCTION: STBC 0x65F190
void BoneState::Reset() {
	time = 0.0f;
	delay = 0.0f;
	rotation = NiMatrix3::IDENTITY;
	lastRotate = NiMatrix3::IDENTITY;
	finished = false;
	currentTrack = 0;
}

/*
================
BoneState::Clone

A copy with copies of every track's keys.
================
*/
// FUNCTION: STBC 0x65F1C0
BoneState *BoneState::Clone() {
	BoneState *clone = new BoneState;
	clone->bone = bone;
	clone->time = time;

	int count = tracks.GetSize();
	clone->tracks.SetSize( count );
	for ( int i = 0; i < count; i++ ) {
		BoneKeyList *track = tracks.GetAt( i );
		if ( track != NULL ) {
			BoneKeyList *copy = new BoneKeyList;
			copy->keys = new NiRotKey *[track->numKeys];
			copy->numKeys = track->numKeys;
			NiRotKey::CopyFunction copyKey = NiRotKey::copyFunctions[NiRotKey::LINKEY];
			for ( int j = 0; j < track->numKeys; j++ ) {
				copy->keys[j] = copyKey( track->keys[j] );
			}
			clone->tracks.SetAt( i, copy );
		}
	}
	return clone;
}

/*
================
BoneState::Update

Steps the current track by delta seconds and applies its rotation to the
bone on top of the bone's own: the rotation applied last time is taken
back first while the bone still holds what this left there.
================
*/
// FUNCTION: STBC 0x65F390
void BoneState::Update( float delta ) {
	if ( delta < 0.0f ) {
		return;
	}

	if ( delay > 0.0f ) {
		delay -= delta;
		if ( delay < 0.0f ) {
			delay = 0.0f;
			time = 0.0f;
		}
		return;
	}

	BoneKeyList *track = tracks.GetAt( currentTrack );
	if ( track == NULL ) {
		return;
	}

	NiMatrix3 rotate;
	rotate = bone->GetRotate();
	if ( lastRotate == rotate ) {
		rotate = rotation.TransposeTimes( rotate );
	}

	time += delta;
	NiRotKey *lastKey = track->keys[track->numKeys - 1];
	if ( time > lastKey->GetTime() ) {
		time = lastKey->GetTime();
		finished = true;
	}

	// BUG: with a single key, neither key is set
	NiRotKey *key0;
	NiRotKey *key1;
	for ( int i = 1; i < track->numKeys; i++ ) {
		if ( track->keys[i]->GetTime() >= time ) {
			key0 = track->keys[i - 1];
			key1 = track->keys[i];
			break;
		}
	}

	float t = ( time - key0->GetTime() ) / ( key1->GetTime() - key0->GetTime() );
	if ( t > 1.0f ) {
		t = 1.0f;
	}

	NiRotKey::InterpFunction interp = NiRotKey::interpFunctions[NiRotKey::LINKEY];
	NiQuaternion quat;
	quat = interp( t, key0, key1 );
	quat.ToRotation( rotation );
	rotate = rotation * rotate;
	lastRotate = rotate;
	bone->SetRotate( rotate );
}

/*
================
BoneState::SetBone
================
*/
// FUNCTION: STBC 0x65F540
void BoneState::SetBone( NiAVObject *bone_ ) {
	bone = bone_;
}

/*
================
BoneState::SetTrack

Replaces the track at the index with the keys, taking ownership of them.
================
*/
// FUNCTION: STBC 0x65F550
void BoneState::SetTrack( int index, NiRotKey **keys, int numKeys ) {
	if ( index < (int)tracks.GetSize() ) {
		BoneKeyList *track = tracks.GetAt( index );
		if ( track != NULL ) {
			tracks.RemoveAt( index );
			delete track;
		}
	}

	BoneKeyList *track = new BoneKeyList;
	track->keys = keys;
	track->numKeys = numKeys;
	tracks.SetAtGrow( index, track );
}
