// NiBezPosKey.cpp -- position keys on a cubic Bezier curve

#include "NiRTLib.h"
#include "NiBezPosKey.h"
#include "NiStream.h"
#include "NiBezFloatKey.h"
#include "NiFloatKey.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x0079cfd0 _$E71
// FUNCTION: STBC 0x0079cff0 _$E70
// FUNCTION: STBC 0x0079d000 _$E17
// FUNCTION: STBC 0x0079d010 _$E14
// FUNCTION: STBC 0x0079d020 _$E16
// FUNCTION: STBC 0x0079d030 _$E15
// FUNCTION: STBC 0x0079d040 _$E23
// FUNCTION: STBC 0x0079d050 _$E20
// FUNCTION: STBC 0x0079d060 _$E22
// FUNCTION: STBC 0x0079d070 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0079d080 _$E42
// FUNCTION: STBC 0x0079d090 _$E41
// FUNCTION: STBC 0x0079d0a0 _$E45
// FUNCTION: STBC 0x0079d0b0 _$E44

/*
================
NiBezPosKey::Register
================
*/
// FUNCTION: STBC 0x79C940
int NiBezPosKey::Register() {
	static bool registered = false;
	if ( registered ) {
		return 0;
	}
	registered = true;
	createFunctions[BEZKEY] = Create;
	saveFunctions[BEZKEY] = Save;
	curvatureFunctions[BEZKEY] = ComputeMaxCurvature;
	equalFunctions[BEZKEY] = Equal;
	copyFunctions[BEZKEY] = Copy;
	interpFunctions[BEZKEY] = Interpolate;
	interpD1Functions[BEZKEY] = InterpolateD1;
	interpD2Functions[BEZKEY] = InterpolateD2;
	fillDerivedValuesFunctions[BEZKEY] = FillDerivedValues;
	return 1;
}

/*
================
NiBezPosKey::Equal
================
*/
// FUNCTION: STBC 0x79C9C0
bool NiBezPosKey::Equal( NiPosKey *key0, NiPosKey *key1 ) {
	if ( !NiPosKey::Equal( key0, key1 ) ) {
		return false;
	}

	NiBezPosKey *bezKey0 = (NiBezPosKey *)key0;
	NiBezPosKey *bezKey1 = (NiBezPosKey *)key1;
	return bezKey0->inTangent == bezKey1->inTangent && bezKey0->outTangent == bezKey1->outTangent;
}

/*
================
NiBezPosKey::Copy

The copy is not checked for a failed allocation before its coefficients are
set.
================
*/
// FUNCTION: STBC 0x79CA40
NiPosKey *NiBezPosKey::Copy( NiPosKey *key ) {
	NiBezPosKey *bezKey = (NiBezPosKey *)key;
	NiBezPosKey *copy = new NiBezPosKey( bezKey->time, bezKey->pos, bezKey->inTangent, bezKey->outTangent );
	copy->a = bezKey->a;
	copy->b = bezKey->b;
	return copy;
}

/*
================
NiBezPosKey::Interpolate
================
*/
// FUNCTION: STBC 0x79CAF0
NiPoint3 NiBezPosKey::Interpolate( float t, NiPosKey *key0, NiPosKey *key1 ) {
	NiBezPosKey *bezKey0 = (NiBezPosKey *)key0;
	return ( ( t * bezKey0->b + bezKey0->a ) * t + bezKey0->outTangent ) * t + bezKey0->pos;
}

/*
================
NiBezPosKey::InterpolateD1
================
*/
// FUNCTION: STBC 0x79CBA0
NiPoint3 NiBezPosKey::InterpolateD1( float t, NiPosKey *key0, NiPosKey *key1 ) {
	NiBezPosKey *bezKey0 = (NiBezPosKey *)key0;
	return ( 2.0f * bezKey0->a + 3.0f * t * bezKey0->b ) * t + bezKey0->outTangent;
}

/*
================
NiBezPosKey::InterpolateD2
================
*/
// FUNCTION: STBC 0x79CC40
NiPoint3 NiBezPosKey::InterpolateD2( float t, NiPosKey *key0, NiPosKey *key1 ) {
	NiBezPosKey *bezKey0 = (NiBezPosKey *)key0;
	return 2.0f * bezKey0->a + 6.0f * t * bezKey0->b;
}

/*
================
NiBezPosKey::FillDerivedValues

The cubic of each key to the next meets the next key's position with its
incoming tangent.
================
*/
// FUNCTION: STBC 0x79CCB0
void NiBezPosKey::FillDerivedValues( NiPosKey **keys, unsigned int numKeys ) {
	NiBezPosKey **bezKeys = (NiBezPosKey **)keys;
	for ( unsigned int i = 0; i < numKeys - 1; i++ ) {
		NiBezPosKey *key0 = bezKeys[i];
		NiBezPosKey *key1 = bezKeys[i + 1];
		key0->a = ( key1->pos - key0->pos ) * 3.0f - ( key0->outTangent * 2.0f + key1->inTangent );
		key0->b = key0->outTangent + key1->inTangent - 2.0f * ( key1->pos - key0->pos );
	}
}

/*
================
NiBezPosKey::ComputeMaxCurvature

Samples the curvature of every segment at sampleSegments + 1 points.
================
*/
// FUNCTION: STBC 0x79CE10
float NiBezPosKey::ComputeMaxCurvature( NiPosKey **keys, unsigned int numKeys ) {
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
NiBezPosKey::Create
================
*/
// FUNCTION: STBC 0x79CF50
NiPosKey *NiBezPosKey::Create( NiStream &stream ) {
	NiBezPosKey *key = new NiBezPosKey;
	key->LoadBinary( stream );
	return key;
}

/*
================
NiBezPosKey::LoadBinary
================
*/
// FUNCTION: STBC 0x79CF70
void NiBezPosKey::LoadBinary( NiStream &stream ) {
	NiPosKey::LoadBinary( stream );
	inTangent.LoadBinary( stream );
	outTangent.LoadBinary( stream );
}

/*
================
NiBezPosKey::Save
================
*/
// FUNCTION: STBC 0x79CFA0
void NiBezPosKey::Save( NiStream &stream, NiPosKey *key ) {
	NiPosKey::Save( stream, key );

	NiBezPosKey *bezKey = (NiBezPosKey *)key;
	bezKey->inTangent.SaveBinary( stream );
	bezKey->outTangent.SaveBinary( stream );
}
