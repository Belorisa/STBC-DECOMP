// NiKeyType6FloatKey.cpp -- TCB float keys that pick a morph target

#include "NiRTLib.h"
#include <string.h>

#include "NiKeyType6FloatKey.h"
#include "NiInterpolation.h"
#include "NiColorData.h"
#include "NiColorKey.h"
#include "NiObject.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x0079e200 _$E78
// FUNCTION: STBC 0x0079e220 _$E77
// FUNCTION: STBC 0x0079e230 _$E17
// FUNCTION: STBC 0x0079e240 _$E14
// FUNCTION: STBC 0x0079e250 _$E16
// FUNCTION: STBC 0x0079e260 _$E15
// FUNCTION: STBC 0x0079e270 _$E23
// FUNCTION: STBC 0x0079e280 _$E20
// FUNCTION: STBC 0x0079e290 _$E22
// FUNCTION: STBC 0x0079e2a0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0079e1e0 _$E32
// FUNCTION: STBC 0x0079e1f0 _$E31
// FUNCTION: STBC 0x0079e2b0 _$E35
// FUNCTION: STBC 0x0079e2c0 _$E34
// FUNCTION: STBC 0x0079e2d0 _$E38
// FUNCTION: STBC 0x0079e2e0 _$E37
// FUNCTION: STBC 0x0079e2f0 _$E41
// FUNCTION: STBC 0x0079e300 _$E40

/*
================
NiKeyType6FloatKey::Register
================
*/
// FUNCTION: STBC 0x79DFD0
int NiKeyType6FloatKey::Register() {
	static bool registered = false;
	if ( registered ) {
		return 0;
	}
	registered = true;
	createFunctions[KEYTYPE6] = Create;
	saveFunctions[KEYTYPE6] = Save;
	interpFunctions[KEYTYPE6] = Interpolate;
	equalFunctions[KEYTYPE6] = Equal;
	copyFunctions[KEYTYPE6] = Copy;
	fillDerivedValuesFunctions[KEYTYPE6] = FillDerivedValues;
	return 1;
}

/*
================
NiKeyType6FloatKey::FillDerivedValues

The TCB tangents of the target indices.
================
*/
// FUNCTION: STBC 0x79E030
void NiKeyType6FloatKey::FillDerivedValues( NiFloatKey **keys, unsigned int numKeys, int unknown ) {
	NiTCBFloatKey::FillDerivedValues( keys, numKeys, unknown );
}

/*
================
NiKeyType6FloatKey::NiKeyType6FloatKey
================
*/
// FUNCTION: STBC 0x79E050
NiKeyType6FloatKey::NiKeyType6FloatKey( float time_, float tension_, float continuity_, float bias_, int target ) {
	time = time_;
	tension = tension_;
	value = (float)target;
	continuity = continuity_;
	bias = bias_;
}

/*
================
NiKeyType6FloatKey::Equal
================
*/
// FUNCTION: STBC 0x79E080
bool NiKeyType6FloatKey::Equal( NiFloatKey *key0, NiFloatKey *key1, int unknown ) {
	return NiKeyType4FloatKey::Equal( key0, key1, unknown );
}

/*
================
NiKeyType6FloatKey::Copy
================
*/
// FUNCTION: STBC 0x79E0A0
NiFloatKey *NiKeyType6FloatKey::Copy( NiFloatKey *key, int unknown ) {
	NiKeyType6FloatKey *source = (NiKeyType6FloatKey *)key;
	return new NiKeyType6FloatKey( source->time, source->tension, source->continuity, source->bias, (int)source->value );
}

/*
================
NiKeyType6FloatKey::Interpolate

The target index does not interpolate as a float; InterpolateValues
splits the weight instead.
================
*/
// FUNCTION: STBC 0x79E0E0
float NiKeyType6FloatKey::Interpolate( float t, NiFloatKey *key0, NiFloatKey *key1 ) {
	return 0.0f;
}

/*
================
NiKeyType6FloatKey::InterpolateValues
================
*/
// FUNCTION: STBC 0x79E0F0
float *NiKeyType6FloatKey::InterpolateValues( float t, NiFloatKey *key0, NiFloatKey *key1, unsigned int numValues ) {
	float *values = new float[numValues];
	memset( values, 0, numValues * sizeof( float ) );
	NiKeyType6FloatKey *tcb0 = (NiKeyType6FloatKey *)key0;
	NiKeyType6FloatKey *tcb1 = (NiKeyType6FloatKey *)key1;
	float target = NiHermiteInterp( t, tcb0->value, tcb0->dd, tcb1->value, tcb1->ds );
	float weight = ( target - tcb0->value ) / ( tcb1->value - tcb0->value );
	values[(int)tcb0->value] = 1.0f - weight;
	values[(int)tcb1->value] = weight;
	return values;
}

/*
================
NiKeyType6FloatKey::Create
================
*/
// FUNCTION: STBC 0x79E170
NiFloatKey *NiKeyType6FloatKey::Create( NiStream &stream, int unknown ) {
	NiKeyType6FloatKey *key = new NiKeyType6FloatKey;
	key->LoadBinary( stream, unknown );
	return key;
}

/*
================
NiKeyType6FloatKey::LoadBinary
================
*/
// FUNCTION: STBC 0x79E1A0
void NiKeyType6FloatKey::LoadBinary( NiStream &stream, int unknown ) {
	NiKeyType4FloatKey::LoadBinary( stream, unknown );
}

/*
================
NiKeyType6FloatKey::Save
================
*/
// FUNCTION: STBC 0x79E1C0
void NiKeyType6FloatKey::Save( NiStream &stream, NiFloatKey *key, int unknown ) {
	NiKeyType4FloatKey::Save( stream, key, unknown );
}
