// NiFloatKey.cpp -- float keys and their per-type function tables

#include "NiRTLib.h"
#include "NiFloatKey.h"
#include "NiStream.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x00791d30 _$E46
// FUNCTION: STBC 0x00791d50 _$E45
// FUNCTION: STBC 0x00791d60 _$E17
// FUNCTION: STBC 0x00791d70 _$E14
// FUNCTION: STBC 0x00791d80 _$E16
// FUNCTION: STBC 0x00791d90 _$E15
// FUNCTION: STBC 0x00791da0 _$E23
// FUNCTION: STBC 0x00791db0 _$E20
// FUNCTION: STBC 0x00791dc0 _$E22
// FUNCTION: STBC 0x00791dd0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00791b00 _$E32
// FUNCTION: STBC 0x00791b10 _$E31

// DATA: STBC 0x99FF68 ?interpFunctions@NiFloatKey@@2PAP6AMMPAV1@0@ZA
NiFloatKey::InterpFunction NiFloatKey::interpFunctions[NUMKEYTYPES];
// DATA: STBC 0x99FF88 ?fillDerivedValuesFunctions@NiFloatKey@@2PAP6AXPAPAV1@IH@ZA
NiFloatKey::FillDerivedValuesFunction NiFloatKey::fillDerivedValuesFunctions[NUMKEYTYPES];
// DATA: STBC 0x99FFA4 ?createFunctions@NiFloatKey@@2PAP6APAV1@AAVNiStream@@H@ZA
NiFloatKey::CreateFunction NiFloatKey::createFunctions[NUMKEYTYPES];
// DATA: STBC 0x99FFC4 ?equalFunctions@NiFloatKey@@2PAP6A_NPAV1@0H@ZA
NiFloatKey::EqualFunction NiFloatKey::equalFunctions[NUMKEYTYPES];
// DATA: STBC 0x99FFE0 ?copyFunctions@NiFloatKey@@2PAP6APAV1@PAV1@H@ZA
NiFloatKey::CopyFunction NiFloatKey::copyFunctions[NUMKEYTYPES];
// DATA: STBC 0x99FFFC ?saveFunctions@NiFloatKey@@2PAP6AXAAVNiStream@@PAV1@H@ZA
NiFloatKey::SaveFunction NiFloatKey::saveFunctions[NUMKEYTYPES];

/*
================
NiFloatKey::Register
================
*/
// FUNCTION: STBC 0x791B20
int NiFloatKey::Register() {
	static bool registered = false;
	if ( registered ) {
		return 0;
	}
	registered = true;
	createFunctions[NOINTERP] = Create;
	saveFunctions[NOINTERP] = Save;
	interpFunctions[NOINTERP] = Interpolate;
	equalFunctions[NOINTERP] = Equal;
	copyFunctions[NOINTERP] = Copy;
	fillDerivedValuesFunctions[NOINTERP] = FillDerivedValues;
	return 1;
}

/*
================
NiFloatKey::GenInterp

Times before the first key or after the last answer that key's value; the
search for the pair around time starts again at lastIndex unless time went
back before it.
================
*/
// FUNCTION: STBC 0x791B80
float NiFloatKey::GenInterp( float time, NiFloatKey **keys, KeyType keyType, unsigned int numKeys, unsigned int &lastIndex ) {
	if ( keys[0]->time >= time ) {
		lastIndex = 0;
		return keys[0]->value;
	}

	unsigned int last = numKeys - 1;
	if ( keys[last]->time <= time ) {
		lastIndex = last;
		return keys[last]->value;
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
NiFloatKey::FillDerivedValues
================
*/
// FUNCTION: STBC 0x791C60
void NiFloatKey::FillDerivedValues( NiFloatKey **keys, unsigned int numKeys, int unknown ) {
}

/*
================
NiFloatKey::Copy

There are no NOINTERP keys to copy.
================
*/
// FUNCTION: STBC 0x791C70
NiFloatKey *NiFloatKey::Copy( NiFloatKey *key, int unknown ) {
	return 0;
}

/*
================
NiFloatKey::Equal
================
*/
// FUNCTION: STBC 0x791C80
bool NiFloatKey::Equal( NiFloatKey *key0, NiFloatKey *key1, int unknown ) {
	return key0->time == key1->time && key0->value == key1->value;
}

/*
================
NiFloatKey::Interpolate

Keys that do not interpolate stay at zero.
================
*/
// FUNCTION: STBC 0x791CB0
float NiFloatKey::Interpolate( float t, NiFloatKey *key0, NiFloatKey *key1 ) {
	return 0.0f;
}

/*
================
NiFloatKey::Create

There are no NOINTERP keys to read.
================
*/
// FUNCTION: STBC 0x791CC0
NiFloatKey *NiFloatKey::Create( NiStream &stream, int unknown ) {
	return 0;
}

/*
================
NiFloatKey::LoadBinary
================
*/
// FUNCTION: STBC 0x791CD0
void NiFloatKey::LoadBinary( NiStream &stream, int unknown ) {
	NiStreamLoadBinary( stream, time );
	NiStreamLoadBinary( stream, value );
}

/*
================
NiFloatKey::Save
================
*/
// FUNCTION: STBC 0x791D00
void NiFloatKey::Save( NiStream &stream, NiFloatKey *key, int unknown ) {
	NiStreamSaveBinary( stream, key->time );
	NiStreamSaveBinary( stream, key->value );
}
