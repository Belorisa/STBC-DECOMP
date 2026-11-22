// NiColorKey.cpp -- colour keys and their per-type function tables

#include "NiRTLib.h"
#include "NiColorKey.h"
#include "NiStream.h"
#include "NiBezRotKey.h"
#include "NiRotKey.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x0079d6f0 _$E68
// FUNCTION: STBC 0x0079d710 _$E67
// FUNCTION: STBC 0x0079d720 _$E17
// FUNCTION: STBC 0x0079d730 _$E14
// FUNCTION: STBC 0x0079d740 _$E16
// FUNCTION: STBC 0x0079d750 _$E15
// FUNCTION: STBC 0x0079d760 _$E23
// FUNCTION: STBC 0x0079d770 _$E20
// FUNCTION: STBC 0x0079d780 _$E22
// FUNCTION: STBC 0x0079d790 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0079d460 _$E58
// FUNCTION: STBC 0x0079d470 _$E57
// FUNCTION: STBC 0x0079d480 _$E61
// FUNCTION: STBC 0x0079d490 _$E60
// FUNCTION: STBC 0x0079d7a0 _$E35
// FUNCTION: STBC 0x0079d7b0 _$E34

// DATA: STBC 0x9A02A0 ?interpFunctions@NiColorKey@@2PAP6A?AVNiColorA@@MPAV1@0@ZA
NiColorKey::InterpFunction NiColorKey::interpFunctions[NUMKEYTYPES];
// DATA: STBC 0x9A02A8 ?copyFunctions@NiColorKey@@2PAP6APAV1@PAV1@@ZA
NiColorKey::CopyFunction NiColorKey::copyFunctions[NUMKEYTYPES];
// DATA: STBC 0x9A02B4 ?equalFunctions@NiColorKey@@2PAP6A_NPAV1@0@ZA
NiColorKey::EqualFunction NiColorKey::equalFunctions[NUMKEYTYPES];
// DATA: STBC 0x9A02C0 ?createFunctions@NiColorKey@@2PAP6APAV1@AAVNiStream@@@ZA
NiColorKey::CreateFunction NiColorKey::createFunctions[NUMKEYTYPES];
// DATA: STBC 0x9A02C8 ?saveFunctions@NiColorKey@@2PAP6AXAAVNiStream@@PAV1@@ZA
NiColorKey::SaveFunction NiColorKey::saveFunctions[NUMKEYTYPES];

/*
================
NiColorKey::Register
================
*/
// FUNCTION: STBC 0x79D4A0
int NiColorKey::Register() {
	static bool registered = false;
	if ( registered ) {
		return 0;
	}
	registered = true;
	createFunctions[NOINTERP] = Create;
	saveFunctions[NOINTERP] = Save;
	equalFunctions[NOINTERP] = Equal;
	copyFunctions[NOINTERP] = Copy;
	interpFunctions[NOINTERP] = Interpolate;
	return 1;
}

/*
================
NiColorKey::GenInterp

Times before the first key or after the last answer that key's colour;
the search for the pair around time starts again at lastIndex unless time
went back before it.
================
*/
// FUNCTION: STBC 0x79D4F0
NiColorA NiColorKey::GenInterp( float time, NiColorKey **keys, KeyType keyType, unsigned int numKeys, unsigned int &lastIndex ) {
	if ( keys[0]->time >= time ) {
		lastIndex = 0;
		return keys[0]->color;
	}

	unsigned int last = numKeys - 1;
	if ( keys[last]->time <= time ) {
		lastIndex = last;
		return keys[last]->color;
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
NiColorKey::Equal
================
*/
// FUNCTION: STBC 0x79D630
bool NiColorKey::Equal( NiColorKey *key0, NiColorKey *key1 ) {
	return key0->time == key1->time && key0->color == key1->color;
}

/*
================
NiColorKey::Create
================
*/
// FUNCTION: STBC 0x79D680
NiColorKey *NiColorKey::Create( NiStream &stream ) {
	return 0;
}

/*
================
NiColorKey::LoadBinary
================
*/
// FUNCTION: STBC 0x79D690
void NiColorKey::LoadBinary( NiStream &stream ) {
	NiStreamLoadBinary( stream, time );
	color.LoadBinary( stream );
}

/*
================
NiColorKey::Save
================
*/
// FUNCTION: STBC 0x79D6C0
void NiColorKey::Save( NiStream &stream, NiColorKey *key ) {
	NiStreamSaveBinary( stream, key->time );
	key->color.SaveBinary( stream );
}

/*
================
NiColorKey::Copy
================
*/
// FUNCTION: STBC 0x79D7C0
NiColorKey *NiColorKey::Copy( NiColorKey *key ) {
	return 0;
}

/*
================
NiColorKey::Interpolate
================
*/
// FUNCTION: STBC 0x79D7D0
NiColorA NiColorKey::Interpolate( float t, NiColorKey *key0, NiColorKey *key1 ) {
	return NiColorA::BLACK;
}
