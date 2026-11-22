// NiLinPosKey.cpp -- linearly interpolated position keys

#include "NiRTLib.h"
#include "NiLinPosKey.h"
#include "NiStream.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x007960c0 _$E59
// FUNCTION: STBC 0x007960e0 _$E58
// FUNCTION: STBC 0x007960f0 _$E17
// FUNCTION: STBC 0x00796100 _$E14
// FUNCTION: STBC 0x00796110 _$E16
// FUNCTION: STBC 0x00796120 _$E15
// FUNCTION: STBC 0x00796130 _$E23
// FUNCTION: STBC 0x00796140 _$E20
// FUNCTION: STBC 0x00796150 _$E22
// FUNCTION: STBC 0x00796160 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00796170 _$E42
// FUNCTION: STBC 0x00796180 _$E41
// FUNCTION: STBC 0x00796190 _$E45
// FUNCTION: STBC 0x007961a0 _$E44

/*
================
NiLinPosKey::Register
================
*/
// FUNCTION: STBC 0x795EF0
int NiLinPosKey::Register() {
	static bool registered = false;
	if ( registered ) {
		return 0;
	}
	registered = true;
	createFunctions[LINKEY] = Create;
	saveFunctions[LINKEY] = Save;
	curvatureFunctions[LINKEY] = ComputeMaxCurvature;
	equalFunctions[LINKEY] = Equal;
	copyFunctions[LINKEY] = Copy;
	interpFunctions[LINKEY] = Interpolate;
	interpD1Functions[LINKEY] = InterpolateD1;
	interpD2Functions[LINKEY] = InterpolateD2;
	fillDerivedValuesFunctions[LINKEY] = FillDerivedValues;
	return 1;
}

/*
================
NiLinPosKey::ComputeMaxCurvature

Straight segments do not bend.
================
*/
// FUNCTION: STBC 0x795F70
float NiLinPosKey::ComputeMaxCurvature( NiPosKey **keys, unsigned int numKeys ) {
	return 0.0f;
}

/*
================
NiLinPosKey::Copy
================
*/
// FUNCTION: STBC 0x795F80
NiPosKey *NiLinPosKey::Copy( NiPosKey *key ) {
	return new NiLinPosKey( key->time, key->pos );
}

/*
================
NiLinPosKey::Interpolate
================
*/
// FUNCTION: STBC 0x795FB0
NiPoint3 NiLinPosKey::Interpolate( float t, NiPosKey *key0, NiPosKey *key1 ) {
	return t * key1->pos + ( 1.0f - t ) * key0->pos;
}

/*
================
NiLinPosKey::InterpolateD1
================
*/
// FUNCTION: STBC 0x796020
NiPoint3 NiLinPosKey::InterpolateD1( float t, NiPosKey *key0, NiPosKey *key1 ) {
	return key1->pos - key0->pos;
}

/*
================
NiLinPosKey::InterpolateD2
================
*/
// FUNCTION: STBC 0x796050
NiPoint3 NiLinPosKey::InterpolateD2( float t, NiPosKey *key0, NiPosKey *key1 ) {
	return NiPoint3::ZERO;
}

/*
================
NiLinPosKey::Create
================
*/
// FUNCTION: STBC 0x796070
NiPosKey *NiLinPosKey::Create( NiStream &stream ) {
	NiLinPosKey *key = new NiLinPosKey;
	key->LoadBinary( stream );
	return key;
}

/*
================
NiLinPosKey::LoadBinary
================
*/
// FUNCTION: STBC 0x796090
void NiLinPosKey::LoadBinary( NiStream &stream ) {
	NiPosKey::LoadBinary( stream );
}

/*
================
NiLinPosKey::Save
================
*/
// FUNCTION: STBC 0x7960A0
void NiLinPosKey::Save( NiStream &stream, NiPosKey *key ) {
	NiPosKey::Save( stream, key );
}

/*
================
NiLinPosKey::Equal
================
*/
// FUNCTION: STBC 0x7961B0
bool NiLinPosKey::Equal( NiPosKey *key0, NiPosKey *key1 ) {
	return NiPosKey::Equal( key0, key1 );
}

/*
================
NiLinPosKey::FillDerivedValues
================
*/
// FUNCTION: STBC 0x7961D0
void NiLinPosKey::FillDerivedValues( NiPosKey **keys, unsigned int numKeys ) {
}
