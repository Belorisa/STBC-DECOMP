// NiKeyType4FloatKey.cpp -- TCB-laid-out float keys that do not interpolate

#include "NiRTLib.h"
#include "NiKeyType4FloatKey.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x0079bed0 _$E45
// FUNCTION: STBC 0x0079bef0 _$E44
// FUNCTION: STBC 0x0079bf00 _$E17
// FUNCTION: STBC 0x0079bf10 _$E14
// FUNCTION: STBC 0x0079bf20 _$E16
// FUNCTION: STBC 0x0079bf30 _$E15
// FUNCTION: STBC 0x0079bf40 _$E23
// FUNCTION: STBC 0x0079bf50 _$E20
// FUNCTION: STBC 0x0079bf60 _$E22
// FUNCTION: STBC 0x0079bf70 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0079beb0 _$E32
// FUNCTION: STBC 0x0079bec0 _$E31
// FUNCTION: STBC 0x0079bf80 _$E35
// FUNCTION: STBC 0x0079bf90 _$E34
// FUNCTION: STBC 0x0079bfa0 _$E38
// FUNCTION: STBC 0x0079bfb0 _$E37

/*
================
NiKeyType4FloatKey::Register
================
*/
// FUNCTION: STBC 0x79BDA0
int NiKeyType4FloatKey::Register() {
	static bool registered = false;
	if ( registered ) {
		return 0;
	}
	registered = true;
	createFunctions[KEYTYPE4] = Create;
	saveFunctions[KEYTYPE4] = Save;
	interpFunctions[KEYTYPE4] = Interpolate;
	equalFunctions[KEYTYPE4] = Equal;
	copyFunctions[KEYTYPE4] = Copy;
	fillDerivedValuesFunctions[KEYTYPE4] = FillDerivedValues;
	return 1;
}

/*
================
NiKeyType4FloatKey::FillDerivedValues
================
*/
// FUNCTION: STBC 0x79BE00
void NiKeyType4FloatKey::FillDerivedValues( NiFloatKey **keys, unsigned int numKeys, int unknown ) {
}

/*
================
NiKeyType4FloatKey::Copy
================
*/
// FUNCTION: STBC 0x79BE10
NiFloatKey *NiKeyType4FloatKey::Copy( NiFloatKey *key, int unknown ) {
	return NiTCBFloatKey::Copy( key, unknown );
}

/*
================
NiKeyType4FloatKey::Equal
================
*/
// FUNCTION: STBC 0x79BE30
bool NiKeyType4FloatKey::Equal( NiFloatKey *key0, NiFloatKey *key1, int unknown ) {
	return NiTCBFloatKey::Equal( key0, key1, unknown );
}

/*
================
NiKeyType4FloatKey::Interpolate

These keys stay at zero.
================
*/
// FUNCTION: STBC 0x79BE50
float NiKeyType4FloatKey::Interpolate( float t, NiFloatKey *key0, NiFloatKey *key1 ) {
	return 0.0f;
}

/*
================
NiKeyType4FloatKey::Create

These keys are not read from a stream.
================
*/
// FUNCTION: STBC 0x79BE60
NiFloatKey *NiKeyType4FloatKey::Create( NiStream &stream, int unknown ) {
	return 0;
}

/*
================
NiKeyType4FloatKey::LoadBinary
================
*/
// FUNCTION: STBC 0x79BE70
void NiKeyType4FloatKey::LoadBinary( NiStream &stream, int unknown ) {
	NiTCBFloatKey::LoadBinary( stream, unknown );
}

/*
================
NiKeyType4FloatKey::Save
================
*/
// FUNCTION: STBC 0x79BE90
void NiKeyType4FloatKey::Save( NiStream &stream, NiFloatKey *key, int unknown ) {
	NiTCBFloatKey::Save( stream, key, unknown );
}
