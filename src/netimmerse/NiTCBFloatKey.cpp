// NiTCBFloatKey.cpp -- float keys on a Kochanek-Bartels spline

#include "NiRTLib.h"
#include "NiTCBFloatKey.h"
#include "NiInterpolation.h"
#include "NiStream.h"
#include "NiAnimBlender.h"
#include "NiEulerRotKey.h"
#include "NiKeyframeController.h"
#include "NiKeyframeData.h"
#include "NiLinPosKey.h"
#include "NiLinRotKey.h"
#include "NiTextKeyExtraData.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x0079bcd0 _$E245
// FUNCTION: STBC 0x0079bcf0 _$E244
// FUNCTION: STBC 0x0079bd00 _$E17
// FUNCTION: STBC 0x0079bd10 _$E14
// FUNCTION: STBC 0x0079bd20 _$E16
// FUNCTION: STBC 0x0079bd30 _$E15
// FUNCTION: STBC 0x0079bd40 _$E23
// FUNCTION: STBC 0x0079bd50 _$E20
// FUNCTION: STBC 0x0079bd60 _$E22
// FUNCTION: STBC 0x0079bd70 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0079bcb0 _$E32
// FUNCTION: STBC 0x0079bcc0 _$E31
// FUNCTION: STBC 0x0079bd80 _$E35
// FUNCTION: STBC 0x0079bd90 _$E34

/*
================
NiTCBFloatKey::Register
================
*/
// FUNCTION: STBC 0x79B920
int NiTCBFloatKey::Register() {
	static bool registered = false;
	if ( registered ) {
		return 0;
	}
	registered = true;
	createFunctions[TCBKEY] = Create;
	saveFunctions[TCBKEY] = Save;
	interpFunctions[TCBKEY] = Interpolate;
	equalFunctions[TCBKEY] = Equal;
	copyFunctions[TCBKEY] = Copy;
	fillDerivedValuesFunctions[TCBKEY] = FillDerivedValues;
	return 1;
}

/*
================
NiTCBFloatKey::Equal
================
*/
// FUNCTION: STBC 0x79B980
bool NiTCBFloatKey::Equal( NiFloatKey *key0, NiFloatKey *key1, int unknown ) {
	if ( !NiFloatKey::Equal( key0, key1, unknown ) ) {
		return false;
	}

	NiTCBFloatKey *tcbKey0 = (NiTCBFloatKey *)key0;
	NiTCBFloatKey *tcbKey1 = (NiTCBFloatKey *)key1;
	return tcbKey0->tension == tcbKey1->tension
		&& tcbKey0->continuity == tcbKey1->continuity
		&& tcbKey0->bias == tcbKey1->bias
		&& tcbKey0->ds == tcbKey1->ds
		&& tcbKey0->dd == tcbKey1->dd;
}

/*
================
NiTCBFloatKey::Interpolate

The Hermite curve from key0's value leaving along its outgoing tangent to
key1's value arriving along its incoming one.
================
*/
// FUNCTION: STBC 0x79B9F0
float NiTCBFloatKey::Interpolate( float t, NiFloatKey *key0, NiFloatKey *key1 ) {
	NiTCBFloatKey *tcbKey0 = (NiTCBFloatKey *)key0;
	NiTCBFloatKey *tcbKey1 = (NiTCBFloatKey *)key1;
	return NiHermiteInterp( t, tcbKey0->GetValue(), tcbKey0->dd, tcbKey1->GetValue(), tcbKey1->ds );
}

/*
================
NiTCBFloatKey::Copy

The copy is not checked for a failed allocation before its tangents are set.
================
*/
// FUNCTION: STBC 0x79BA30
NiFloatKey *NiTCBFloatKey::Copy( NiFloatKey *key, int unknown ) {
	NiTCBFloatKey *tcbKey = (NiTCBFloatKey *)key;
	NiTCBFloatKey *copy = new NiTCBFloatKey( tcbKey->time, tcbKey->value, tcbKey->tension, tcbKey->continuity, tcbKey->bias );
	copy->ds = tcbKey->ds;
	copy->dd = tcbKey->dd;
	return copy;
}

/*
================
NiTCBFloatKey::CalculateDVals

The Kochanek-Bartels tangents: each weighs the differences to the previous
and the next value by the tension, continuity and bias, and is scaled for
the length of its own interval against the mean of the two.
================
*/
// FUNCTION: STBC 0x79BA80
void NiTCBFloatKey::CalculateDVals( NiTCBFloatKey *key, float prevValue, float nextValue, float prevLength, float nextLength ) {
	float prevDiff = key->GetValue() - prevValue;
	float nextDiff = nextValue - key->GetValue();
	float onePlusBias = 1.0f + key->GetBias();
	float oneMinusBias = 1.0f - key->GetBias();
	float tensionWeight = ( 1.0f - key->GetTension() ) * 0.5f;
	float plusContinuityWeight = ( 1.0f + key->GetContinuity() ) * tensionWeight;
	float minusContinuityWeight = ( 1.0f - key->GetContinuity() ) * tensionWeight;
	float scale = 2.0f / ( prevLength + nextLength );

	key->ds = ( plusContinuityWeight * oneMinusBias * nextDiff + minusContinuityWeight * onePlusBias * prevDiff ) * scale * prevLength;
	key->dd = ( minusContinuityWeight * oneMinusBias * nextDiff + plusContinuityWeight * onePlusBias * prevDiff ) * scale * nextLength;
}

/*
================
NiTCBFloatKey::FillDerivedValues

The end keys take a mirrored neighbour and unit intervals.
================
*/
// FUNCTION: STBC 0x79BB20
void NiTCBFloatKey::FillDerivedValues( NiFloatKey **keys, unsigned int numKeys, int unknown ) {
	if ( numKeys < 2 ) {
		return;
	}

	NiTCBFloatKey **tcbKeys = (NiTCBFloatKey **)keys;
	CalculateDVals( tcbKeys[0], 2.0f * tcbKeys[0]->value - tcbKeys[1]->value, tcbKeys[1]->value, 1.0f, 1.0f );

	unsigned int last = numKeys - 1;
	for ( unsigned int i = 1; i < last; i++ ) {
		CalculateDVals( tcbKeys[i], tcbKeys[i - 1]->value, tcbKeys[i + 1]->value,
			tcbKeys[i]->time - tcbKeys[i - 1]->time, tcbKeys[i + 1]->time - tcbKeys[i]->time );
	}

	NiTCBFloatKey *lastKey = tcbKeys[last];
	float prevValue = tcbKeys[numKeys - 2]->value;
	CalculateDVals( lastKey, prevValue, 2.0f * lastKey->value - prevValue, 1.0f, 1.0f );
}

/*
================
NiTCBFloatKey::Create
================
*/
// FUNCTION: STBC 0x79BBE0
NiFloatKey *NiTCBFloatKey::Create( NiStream &stream, int unknown ) {
	NiTCBFloatKey *key = new NiTCBFloatKey;
	key->LoadBinary( stream, unknown );
	return key;
}

/*
================
NiTCBFloatKey::LoadBinary
================
*/
// FUNCTION: STBC 0x79BC10
void NiTCBFloatKey::LoadBinary( NiStream &stream, int unknown ) {
	NiFloatKey::LoadBinary( stream, unknown );
	NiStreamLoadBinary( stream, tension );
	NiStreamLoadBinary( stream, continuity );
	NiStreamLoadBinary( stream, bias );
}

/*
================
NiTCBFloatKey::Save
================
*/
// FUNCTION: STBC 0x79BC60
void NiTCBFloatKey::Save( NiStream &stream, NiFloatKey *key, int unknown ) {
	NiFloatKey::Save( stream, key, unknown );

	NiTCBFloatKey *tcbKey = (NiTCBFloatKey *)key;
	NiStreamSaveBinary( stream, tcbKey->tension );
	NiStreamSaveBinary( stream, tcbKey->continuity );
	NiStreamSaveBinary( stream, tcbKey->bias );
}
