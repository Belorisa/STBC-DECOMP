// NiLinRotKey.cpp -- linearly interpolated rotation keys

#include "NiRTLib.h"
#include "NiLinRotKey.h"
#include "NiStream.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x007963a0 _$E56
// FUNCTION: STBC 0x007963c0 _$E55
// FUNCTION: STBC 0x007963d0 _$E17
// FUNCTION: STBC 0x007963e0 _$E14
// FUNCTION: STBC 0x007963f0 _$E16
// FUNCTION: STBC 0x00796400 _$E15
// FUNCTION: STBC 0x00796410 _$E23
// FUNCTION: STBC 0x00796420 _$E20
// FUNCTION: STBC 0x00796430 _$E22
// FUNCTION: STBC 0x00796440 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00796450 _$E39
// FUNCTION: STBC 0x00796460 _$E38
// FUNCTION: STBC 0x00796470 _$E42
// FUNCTION: STBC 0x00796480 _$E41

/*
================
NiLinRotKey::Register
================
*/
// FUNCTION: STBC 0x7961E0
int NiLinRotKey::Register() {
	static bool registered = false;
	if ( registered ) {
		return 0;
	}
	registered = true;
	createFunctions[LINKEY] = Create;
	interpFunctions[LINKEY] = Interpolate;
	fillDerivedValuesFunctions[LINKEY] = FillDerivedValues;
	saveFunctions[LINKEY] = Save;
	equalFunctions[LINKEY] = Equal;
	copyFunctions[LINKEY] = Copy;
	return 1;
}

/*
================
NiLinRotKey::FillDerivedValues
================
*/
// FUNCTION: STBC 0x796240
void NiLinRotKey::FillDerivedValues( NiRotKey **keys, unsigned int numKeys ) {
	NiRotKey::FillDerivedValues( keys, numKeys );
}

/*
================
NiLinRotKey::Copy
================
*/
// FUNCTION: STBC 0x796260
NiRotKey *NiLinRotKey::Copy( NiRotKey *key ) {
	return new NiLinRotKey( key->time, key->quat );
}

/*
================
NiLinRotKey::Interpolate
================
*/
// FUNCTION: STBC 0x7962C0
NiQuaternion NiLinRotKey::Interpolate( float t, NiRotKey *key0, NiRotKey *key1 ) {
	return NiQuaternion::Slerp( t, key0->quat, key1->quat );
}

/*
================
NiLinRotKey::Create
================
*/
// FUNCTION: STBC 0x7962F0
NiRotKey *NiLinRotKey::Create( NiStream &stream ) {
	NiLinRotKey *key = new NiLinRotKey;
	key->LoadBinary( stream );
	return key;
}

/*
================
NiLinRotKey::LoadBinary
================
*/
// FUNCTION: STBC 0x796370
void NiLinRotKey::LoadBinary( NiStream &stream ) {
	NiRotKey::LoadBinary( stream );
}

/*
================
NiLinRotKey::Save
================
*/
// FUNCTION: STBC 0x796380
void NiLinRotKey::Save( NiStream &stream, NiRotKey *key ) {
	NiRotKey::Save( stream, key );
}

/*
================
NiLinRotKey::Equal
================
*/
// FUNCTION: STBC 0x796490
bool NiLinRotKey::Equal( NiRotKey *key0, NiRotKey *key1 ) {
	return NiRotKey::Equal( key0, key1 );
}
