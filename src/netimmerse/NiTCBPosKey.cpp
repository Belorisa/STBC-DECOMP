// NiTCBPosKey.cpp -- position keys on a Kochanek-Bartels spline

#include "NiRTLib.h"
#include "NiTCBPosKey.h"
#include "NiStream.h"
#include "NiExtraData.h"
#include "NiObject.h"
#include "NiObjectNET.h"
#include "NiSequenceStreamHelper.h"
#include "NiTimeController.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x007b53b0 _$E82
// FUNCTION: STBC 0x007b53d0 _$E81
// FUNCTION: STBC 0x007b53e0 _$E17
// FUNCTION: STBC 0x007b53f0 _$E14
// FUNCTION: STBC 0x007b5400 _$E16
// FUNCTION: STBC 0x007b5410 _$E15
// FUNCTION: STBC 0x007b5420 _$E23
// FUNCTION: STBC 0x007b5430 _$E20
// FUNCTION: STBC 0x007b5440 _$E22
// FUNCTION: STBC 0x007b5450 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007b5480 _$E45
// FUNCTION: STBC 0x007b5490 _$E44

/*
================
NiTCBPosKey::Register
================
*/
// FUNCTION: STBC 0x7B4900
int NiTCBPosKey::Register() {
	static bool registered = false;
	if ( registered ) {
		return 0;
	}
	registered = true;
	createFunctions[TCBKEY] = Create;
	saveFunctions[TCBKEY] = Save;
	curvatureFunctions[TCBKEY] = ComputeMaxCurvature;
	equalFunctions[TCBKEY] = Equal;
	copyFunctions[TCBKEY] = Copy;
	interpFunctions[TCBKEY] = Interpolate;
	interpD1Functions[TCBKEY] = InterpolateD1;
	interpD2Functions[TCBKEY] = InterpolateD2;
	fillDerivedValuesFunctions[TCBKEY] = FillDerivedValues;
	return 1;
}

/*
================
NiTCBPosKey::Equal
================
*/
// FUNCTION: STBC 0x7B4980
bool NiTCBPosKey::Equal( NiPosKey *key0, NiPosKey *key1 ) {
	if ( !NiPosKey::Equal( key0, key1 ) ) {
		return false;
	}

	NiTCBPosKey *tcbKey0 = (NiTCBPosKey *)key0;
	NiTCBPosKey *tcbKey1 = (NiTCBPosKey *)key1;
	return tcbKey0->tension == tcbKey1->tension
		&& tcbKey0->continuity == tcbKey1->continuity
		&& tcbKey0->bias == tcbKey1->bias
		&& tcbKey0->ds == tcbKey1->ds
		&& tcbKey0->dd == tcbKey1->dd
		&& tcbKey0->a == tcbKey1->a
		&& tcbKey0->b == tcbKey1->b;
}

/*
================
NiTCBPosKey::Copy

The copy is not checked for a failed allocation before its derived values
are set.
================
*/
// FUNCTION: STBC 0x7B4A80
NiPosKey *NiTCBPosKey::Copy( NiPosKey *key ) {
	NiTCBPosKey *tcbKey = (NiTCBPosKey *)key;
	NiTCBPosKey *copy = new NiTCBPosKey( tcbKey->time, tcbKey->pos, tcbKey->tension, tcbKey->continuity, tcbKey->bias );
	copy->ds = tcbKey->ds;
	copy->dd = tcbKey->dd;
	copy->a = tcbKey->a;
	copy->b = tcbKey->b;
	return copy;
}

/*
================
NiTCBPosKey::Interpolate
================
*/
// FUNCTION: STBC 0x7B4B50
NiPoint3 NiTCBPosKey::Interpolate( float t, NiPosKey *key0, NiPosKey *key1 ) {
	NiTCBPosKey *tcbKey0 = (NiTCBPosKey *)key0;
	return ( ( t * tcbKey0->b + tcbKey0->a ) * t + tcbKey0->dd ) * t + tcbKey0->pos;
}

/*
================
NiTCBPosKey::InterpolateD1
================
*/
// FUNCTION: STBC 0x7B4C00
NiPoint3 NiTCBPosKey::InterpolateD1( float t, NiPosKey *key0, NiPosKey *key1 ) {
	NiTCBPosKey *tcbKey0 = (NiTCBPosKey *)key0;
	return ( 2.0f * tcbKey0->a + 3.0f * t * tcbKey0->b ) * t + tcbKey0->dd;
}

/*
================
NiTCBPosKey::InterpolateD2
================
*/
// FUNCTION: STBC 0x7B4CA0
NiPoint3 NiTCBPosKey::InterpolateD2( float t, NiPosKey *key0, NiPosKey *key1 ) {
	NiTCBPosKey *tcbKey0 = (NiTCBPosKey *)key0;
	return 2.0f * tcbKey0->a + 6.0f * t * tcbKey0->b;
}

/*
================
NiTCBPosKey::CalculateDVals

The Kochanek-Bartels tangents: each weighs the differences to the previous
and the next position by the tension, continuity and bias, and is scaled
for the length of its own interval against the mean of the two.
================
*/
// FUNCTION: STBC 0x7B4D10
void NiTCBPosKey::CalculateDVals( const NiPoint3 &prevPos, const NiPoint3 &nextPos, float prevLength, float nextLength ) {
	NiPoint3 prevDiff = pos - prevPos;
	NiPoint3 nextDiff = nextPos - pos;
	float onePlusBias = 1.0f + bias;
	float oneMinusBias = 1.0f - bias;
	float tensionWeight = ( 1.0f - tension ) * 0.5f;
	float plusContinuityWeight = ( 1.0f + continuity ) * tensionWeight;
	float minusContinuityWeight = ( 1.0f - continuity ) * tensionWeight;

	ds = ( minusContinuityWeight * onePlusBias ) * prevDiff + ( plusContinuityWeight * oneMinusBias ) * nextDiff;
	dd = ( plusContinuityWeight * onePlusBias ) * prevDiff + ( minusContinuityWeight * oneMinusBias ) * nextDiff;

	float scale = 2.0f / ( prevLength + nextLength );
	ds *= scale * prevLength;
	dd *= scale * nextLength;
}

/*
================
NiTCBPosKey::FillDerivedValues

The end keys take a mirrored neighbour and unit intervals. The cubic of
each key to the next then meets the next key's position with its incoming
tangent.
================
*/
// FUNCTION: STBC 0x7B4EF0
void NiTCBPosKey::FillDerivedValues( NiPosKey **keys, unsigned int numKeys ) {
	NiTCBPosKey **tcbKeys = (NiTCBPosKey **)keys;
	unsigned int last = numKeys - 1;
	unsigned int i;

	if ( numKeys >= 2 ) {
		tcbKeys[0]->CalculateDVals( 2.0f * tcbKeys[0]->pos - tcbKeys[1]->pos, tcbKeys[1]->pos, 1.0f, 1.0f );
		for ( i = 1; i < last; i++ ) {
			NiTCBPosKey *prev = tcbKeys[i - 1];
			NiTCBPosKey *key = tcbKeys[i];
			NiTCBPosKey *next = tcbKeys[i + 1];
			key->CalculateDVals( prev->pos, next->pos, key->time - prev->time, next->time - key->time );
		}
		tcbKeys[last]->CalculateDVals( tcbKeys[numKeys - 2]->pos, 2.0f * tcbKeys[last]->pos - tcbKeys[numKeys - 2]->pos, 1.0f, 1.0f );
	}

	for ( i = 0; i < last; i++ ) {
		NiTCBPosKey *key0 = tcbKeys[i];
		NiTCBPosKey *key1 = tcbKeys[i + 1];
		key0->a = 3.0f * ( key1->pos - key0->pos ) - ( 2.0f * key0->dd + key1->ds );
		key0->b = key0->dd + key1->ds - 2.0f * ( key1->pos - key0->pos );
	}
}

/*
================
NiTCBPosKey::ComputeMaxCurvature

Samples the curvature of every segment at sampleSegments + 1 points.
================
*/
// FUNCTION: STBC 0x7B51C0
float NiTCBPosKey::ComputeMaxCurvature( NiPosKey **keys, unsigned int numKeys ) {
	float maxCurvature = 0.0f;
	for ( unsigned int i = 0; i < numKeys - 1; i++ ) {
		NiPosKey *key0 = keys[i];
		NiPosKey *key1 = keys[i + 1];
		for ( int j = 0; j <= sampleSegments; j++ ) {
			float t = j * sampleDelta;
			NiPoint3 d1 = InterpolateD1( t, key0, key1 );
			NiPoint3 d2 = InterpolateD2( t, key0, key1 );
			float curvature = d1.Cross( d2 ).Length() / d1.SqrLength();
			if ( curvature > maxCurvature ) {
				maxCurvature = curvature;
			}
		}
	}
	return maxCurvature;
}

/*
================
NiTCBPosKey::Create
================
*/
// FUNCTION: STBC 0x7B5300
NiPosKey *NiTCBPosKey::Create( NiStream &stream ) {
	NiTCBPosKey *key = new NiTCBPosKey;
	key->LoadBinary( stream );
	return key;
}

/*
================
NiTCBPosKey::LoadBinary
================
*/
// FUNCTION: STBC 0x7B5320
void NiTCBPosKey::LoadBinary( NiStream &stream ) {
	NiPosKey::LoadBinary( stream );
	NiStreamLoadBinary( stream, tension );
	NiStreamLoadBinary( stream, continuity );
	NiStreamLoadBinary( stream, bias );
}

/*
================
NiTCBPosKey::Save
================
*/
// FUNCTION: STBC 0x7B5360
void NiTCBPosKey::Save( NiStream &stream, NiPosKey *key ) {
	NiPosKey::Save( stream, key );

	NiTCBPosKey *tcbKey = (NiTCBPosKey *)key;
	NiStreamSaveBinary( stream, tcbKey->tension );
	NiStreamSaveBinary( stream, tcbKey->continuity );
	NiStreamSaveBinary( stream, tcbKey->bias );
}
