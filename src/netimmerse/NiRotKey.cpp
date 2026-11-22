// NiRotKey.cpp -- rotation keys and their per-type function tables

#include "NiRTLib.h"
#include "NiRotKey.h"
#include "NiStream.h"
#include "NiAVObject.h"
#include "NiExtraData.h"
#include "NiListener.h"
#include "NiObject.h"
#include "NiObjectNET.h"
#include "NiProperty.h"
#include "NiSoundSystem.h"
#include "NiTimeController.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x00791340 _$E202
// FUNCTION: STBC 0x00791360 _$E201
// FUNCTION: STBC 0x00791370 _$E17
// FUNCTION: STBC 0x00791380 _$E14
// FUNCTION: STBC 0x00791390 _$E16
// FUNCTION: STBC 0x007913a0 _$E15
// FUNCTION: STBC 0x007913b0 _$E23
// FUNCTION: STBC 0x007913c0 _$E20
// FUNCTION: STBC 0x007913d0 _$E22
// FUNCTION: STBC 0x007913e0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00790f00 _$E53
// FUNCTION: STBC 0x00790f10 _$E52
// FUNCTION: STBC 0x00790f20 _$E57
// FUNCTION: STBC 0x00790f30 _$E56
// FUNCTION: STBC 0x00790f40 _$E61
// FUNCTION: STBC 0x00790f50 _$E60
// FUNCTION: STBC 0x00790f60 _$E65
// FUNCTION: STBC 0x00790f70 _$E64
// FUNCTION: STBC 0x00790f80 _$E69
// FUNCTION: STBC 0x00790f90 _$E68
// FUNCTION: STBC 0x00790fa0 _$E186
// FUNCTION: STBC 0x00790fb0 _$E185
// FUNCTION: STBC 0x00790fc0 _$E190
// FUNCTION: STBC 0x00790fd0 _$E189
// FUNCTION: STBC 0x00790fe0 _$E195
// FUNCTION: STBC 0x00790ff0 _$E194
// FUNCTION: STBC 0x007913f0 _$E39
// FUNCTION: STBC 0x00791400 _$E38

// DATA: STBC 0x99FE44 ?equalFunctions@NiRotKey@@2PAP6A_NPAV1@0@ZA
NiRotKey::EqualFunction NiRotKey::equalFunctions[NUMKEYTYPES];
// DATA: STBC 0x99FE58 ?interpFunctions@NiRotKey@@2PAP6A?AVNiQuaternion@@MPAV1@0@ZA
NiRotKey::InterpFunction NiRotKey::interpFunctions[NUMKEYTYPES];
// DATA: STBC 0x99FE6C ?copyFunctions@NiRotKey@@2PAP6APAV1@PAV1@@ZA
NiRotKey::CopyFunction NiRotKey::copyFunctions[NUMKEYTYPES];
// DATA: STBC 0x99FE84 ?createFunctions@NiRotKey@@2PAP6APAV1@AAVNiStream@@@ZA
NiRotKey::CreateFunction NiRotKey::createFunctions[NUMKEYTYPES];
// DATA: STBC 0x99FE9C ?fillDerivedValuesFunctions@NiRotKey@@2PAP6AXPAPAV1@I@ZA
NiRotKey::FillDerivedValuesFunction NiRotKey::fillDerivedValuesFunctions[NUMKEYTYPES];
// DATA: STBC 0x99FEB0 ?saveFunctions@NiRotKey@@2PAP6AXAAVNiStream@@PAV1@@ZA
NiRotKey::SaveFunction NiRotKey::saveFunctions[NUMKEYTYPES];

/*
================
NiRotKey::Register
================
*/
// FUNCTION: STBC 0x791010
int NiRotKey::Register() {
	static bool registered = false;
	if ( registered ) {
		return 0;
	}
	registered = true;
	createFunctions[NOINTERP] = Create;
	interpFunctions[NOINTERP] = Interpolate;
	fillDerivedValuesFunctions[NOINTERP] = FillDerivedValues;
	saveFunctions[NOINTERP] = Save;
	equalFunctions[NOINTERP] = Equal;
	copyFunctions[NOINTERP] = Copy;
	return 1;
}

/*
================
NiRotKey::GenInterp

Euler keys interpolate inside their one key. Otherwise times before the
first key or after the last answer that key's rotation; the search for the
pair around time starts again at lastIndex unless time went back before
it.
================
*/
// FUNCTION: STBC 0x791070
NiQuaternion NiRotKey::GenInterp( float time, NiRotKey **keys, KeyType keyType, unsigned int numKeys, unsigned int &lastIndex ) {
	if ( keyType == EULERKEY ) {
		InterpFunction interp = interpFunctions[EULERKEY];
		return interp( time, keys[0], 0 );
	}

	if ( keys[0]->time >= time ) {
		lastIndex = 0;
		return keys[0]->quat;
	}

	unsigned int last = numKeys - 1;
	if ( keys[last]->time <= time ) {
		lastIndex = last;
		return keys[last]->quat;
	}

	float lastTime = keys[lastIndex]->time;
	if ( time < lastTime ) {
		lastIndex = 0;
		lastTime = keys[0]->time;
	}

	unsigned int next;
	float nextTime;
	for ( next = lastIndex + 1; next <= last; next++ ) {
		nextTime = keys[next]->time;
		if ( time < nextTime ) {
			break;
		}
		lastIndex++;
		lastTime = nextTime;
	}

	float t = ( time - lastTime ) / ( nextTime - lastTime );
	InterpFunction interp = interpFunctions[keyType];
	return interp( t, keys[lastIndex], keys[next] );
}

/*
================
NiRotKey::Interpolate

Keys that do not interpolate stay at the identity.
================
*/
// FUNCTION: STBC 0x7911C0
NiQuaternion NiRotKey::Interpolate( float t, NiRotKey *key0, NiRotKey *key1 ) {
	return NiQuaternion( 1.0f, 0.0f, 0.0f, 0.0f );
}

/*
================
NiRotKey::Equal
================
*/
// FUNCTION: STBC 0x7911E0
bool NiRotKey::Equal( NiRotKey *key0, NiRotKey *key1 ) {
	return key0->time == key1->time
		&& key0->quat.w == key1->quat.w
		&& key0->quat.x == key1->quat.x
		&& key0->quat.y == key1->quat.y
		&& key0->quat.z == key1->quat.z;
}

/*
================
NiRotKey::FillDerivedValues

Flips each rotation into the same hemisphere as the one before it, so that
the interpolation takes the short way round, then clamps w to [-1, 1].
================
*/
// FUNCTION: STBC 0x791230
void NiRotKey::FillDerivedValues( NiRotKey **keys, unsigned int numKeys ) {
	unsigned int i;
	for ( i = 0; i < numKeys - 1; i++ ) {
		NiRotKey *key0 = keys[i];
		NiRotKey *key1 = keys[i + 1];
		if ( NiQuaternion::Dot( key0->quat, key1->quat ) < 0.0f ) {
			key1->quat = -key1->quat;
		}
	}

	for ( i = 0; i < numKeys; i++ ) {
		float w = keys[i]->quat.w;
		if ( w < -1.0f ) {
			keys[i]->quat.w = -1.0f;
		} else if ( w > 1.0f ) {
			keys[i]->quat.w = 1.0f;
		}
	}
}

/*
================
NiRotKey::Create

There are no NOINTERP keys to read.
================
*/
// FUNCTION: STBC 0x7912D0
NiRotKey *NiRotKey::Create( NiStream &stream ) {
	return 0;
}

/*
================
NiRotKey::LoadBinary
================
*/
// FUNCTION: STBC 0x7912E0
void NiRotKey::LoadBinary( NiStream &stream ) {
	NiStreamLoadBinary( stream, time );
	quat.LoadBinary( stream );
}

/*
================
NiRotKey::Save
================
*/
// FUNCTION: STBC 0x791310
void NiRotKey::Save( NiStream &stream, NiRotKey *key ) {
	NiStreamSaveBinary( stream, key->time );
	key->quat.SaveBinary( stream );
}

/*
================
NiRotKey::Copy
================
*/
// FUNCTION: STBC 0x791410
NiRotKey *NiRotKey::Copy( NiRotKey *key ) {
	return 0;
}
