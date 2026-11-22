// NiBezFloatKey.cpp -- float keys on a Bezier curve

#include "NiRTLib.h"
#include "NiBezFloatKey.h"
#include "NiInterpolation.h"
#include "NiStream.h"
#include "NiKeyType4FloatKey.h"
#include "NiMorphKey.h"
#include "NiTCBFloatKey.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x0079c870 _$E58
// FUNCTION: STBC 0x0079c890 _$E57
// FUNCTION: STBC 0x0079c8a0 _$E17
// FUNCTION: STBC 0x0079c8b0 _$E14
// FUNCTION: STBC 0x0079c8c0 _$E16
// FUNCTION: STBC 0x0079c8d0 _$E15
// FUNCTION: STBC 0x0079c8e0 _$E23
// FUNCTION: STBC 0x0079c8f0 _$E20
// FUNCTION: STBC 0x0079c900 _$E22
// FUNCTION: STBC 0x0079c910 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0079c850 _$E32
// FUNCTION: STBC 0x0079c860 _$E31
// FUNCTION: STBC 0x0079c920 _$E35
// FUNCTION: STBC 0x0079c930 _$E34

/*
================
NiBezFloatKey::Register
================
*/
// FUNCTION: STBC 0x79C660
int NiBezFloatKey::Register() {
	static bool registered = false;
	if ( registered ) {
		return 0;
	}
	registered = true;
	createFunctions[BEZKEY] = Create;
	saveFunctions[BEZKEY] = Save;
	interpFunctions[BEZKEY] = Interpolate;
	equalFunctions[BEZKEY] = Equal;
	copyFunctions[BEZKEY] = Copy;
	fillDerivedValuesFunctions[BEZKEY] = FillDerivedValues;
	return 1;
}

/*
================
NiBezFloatKey::FillDerivedValues

The tangents are stored with the keys; nothing is derived.
================
*/
// FUNCTION: STBC 0x79C6C0
void NiBezFloatKey::FillDerivedValues( NiFloatKey **keys, unsigned int numKeys, int unknown ) {
}

/*
================
NiBezFloatKey::Equal
================
*/
// FUNCTION: STBC 0x79C6D0
bool NiBezFloatKey::Equal( NiFloatKey *key0, NiFloatKey *key1, int unknown ) {
	if ( !NiFloatKey::Equal( key0, key1, unknown ) ) {
		return false;
	}

	NiBezFloatKey *bezKey0 = (NiBezFloatKey *)key0;
	NiBezFloatKey *bezKey1 = (NiBezFloatKey *)key1;
	return bezKey0->inTangent == bezKey1->inTangent && bezKey0->outTangent == bezKey1->outTangent;
}

/*
================
NiBezFloatKey::Interpolate

The Bezier curve from key0's value leaving along its outgoing tangent to
key1's value arriving along its incoming one.
================
*/
// FUNCTION: STBC 0x79C720
float NiBezFloatKey::Interpolate( float t, NiFloatKey *key0, NiFloatKey *key1 ) {
	NiBezFloatKey *bezKey0 = (NiBezFloatKey *)key0;
	NiBezFloatKey *bezKey1 = (NiBezFloatKey *)key1;
	return NiBezierInterp( t, bezKey0->GetValue(), bezKey0->GetOutTangent(), bezKey1->GetValue(), bezKey1->GetInTangent() );
}

/*
================
NiBezFloatKey::Copy
================
*/
// FUNCTION: STBC 0x79C770
NiFloatKey *NiBezFloatKey::Copy( NiFloatKey *key, int unknown ) {
	NiBezFloatKey *bezKey = (NiBezFloatKey *)key;
	return new NiBezFloatKey( bezKey->time, bezKey->value, bezKey->inTangent, bezKey->outTangent );
}

/*
================
NiBezFloatKey::Create
================
*/
// FUNCTION: STBC 0x79C7A0
NiFloatKey *NiBezFloatKey::Create( NiStream &stream, int unknown ) {
	NiBezFloatKey *key = new NiBezFloatKey;
	key->LoadBinary( stream, unknown );
	return key;
}

/*
================
NiBezFloatKey::LoadBinary
================
*/
// FUNCTION: STBC 0x79C7D0
void NiBezFloatKey::LoadBinary( NiStream &stream, int unknown ) {
	NiFloatKey::LoadBinary( stream, unknown );
	NiStreamLoadBinary( stream, inTangent );
	NiStreamLoadBinary( stream, outTangent );
}

/*
================
NiBezFloatKey::Save
================
*/
// FUNCTION: STBC 0x79C810
void NiBezFloatKey::Save( NiStream &stream, NiFloatKey *key, int unknown ) {
	NiFloatKey::Save( stream, key, unknown );

	NiBezFloatKey *bezKey = (NiBezFloatKey *)key;
	NiStreamSaveBinary( stream, bezKey->inTangent );
	NiStreamSaveBinary( stream, bezKey->outTangent );
}
