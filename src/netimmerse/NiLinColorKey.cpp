// NiLinColorKey.cpp -- linearly interpolated colour keys

#include "NiRTLib.h"
#include "NiLinColorKey.h"
#include "NiInterpolation.h"
#include "NiStream.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x007a72c0 _$E52
// FUNCTION: STBC 0x007a72e0 _$E51
// FUNCTION: STBC 0x007a72f0 _$E17
// FUNCTION: STBC 0x007a7300 _$E14
// FUNCTION: STBC 0x007a7310 _$E16
// FUNCTION: STBC 0x007a7320 _$E15
// FUNCTION: STBC 0x007a7330 _$E23
// FUNCTION: STBC 0x007a7340 _$E20
// FUNCTION: STBC 0x007a7350 _$E22
// FUNCTION: STBC 0x007a7360 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007a7390 _$E38
// FUNCTION: STBC 0x007a73a0 _$E37

/*
================
NiLinColorKey::Register
================
*/
// FUNCTION: STBC 0x7A70F0
int NiLinColorKey::Register() {
	static bool registered = false;
	if ( registered ) {
		return 0;
	}
	registered = true;
	createFunctions[LINKEY] = Create;
	saveFunctions[LINKEY] = Save;
	equalFunctions[LINKEY] = Equal;
	copyFunctions[LINKEY] = Copy;
	interpFunctions[LINKEY] = Interpolate;
	return 1;
}

/*
================
NiLinColorKey::Copy
================
*/
// FUNCTION: STBC 0x7A7140
NiColorKey *NiLinColorKey::Copy( NiColorKey *key ) {
	return new NiLinColorKey( key->time, key->color );
}

/*
================
NiLinColorKey::Interpolate

The colour a fraction t of the way from the first key's to the second's.
================
*/
// FUNCTION: STBC 0x7A7180
NiColorA NiLinColorKey::Interpolate( float t, NiColorKey *key0, NiColorKey *key1 ) {
	NiColorA from = key0->color * ( 1.0f - t );
	NiColorA to = key1->color * t;
	return from + to;
}

/*
================
NiLinColorKey::Create
================
*/
// FUNCTION: STBC 0x7A7260
NiColorKey *NiLinColorKey::Create( NiStream &stream ) {
	NiLinColorKey *key = new NiLinColorKey;
	key->LoadBinary( stream );
	return key;
}

/*
================
NiLinColorKey::LoadBinary
================
*/
// FUNCTION: STBC 0x7A7290
void NiLinColorKey::LoadBinary( NiStream &stream ) {
	NiColorKey::LoadBinary( stream );
}

/*
================
NiLinColorKey::Save
================
*/
// FUNCTION: STBC 0x7A72A0
void NiLinColorKey::Save( NiStream &stream, NiColorKey *key ) {
	NiColorKey::Save( stream, key );
}
