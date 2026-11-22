// NiMorphKey.cpp -- TCB keys with a value per morph target

// <new> declares operator delete throw(): the destructor below has no EH
// frame for its base, which it would get with delete assumed to throw.
#include "NiRTLib.h"
#include <new>

#include <string.h>

#include "NiMorphKey.h"
#include "NiInterpolation.h"
#include "NiStream.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x0079c550 _$E55
// FUNCTION: STBC 0x0079c570 _$E54
// FUNCTION: STBC 0x0079c580 _$E17
// FUNCTION: STBC 0x0079c590 _$E14
// FUNCTION: STBC 0x0079c5a0 _$E16
// FUNCTION: STBC 0x0079c5b0 _$E15
// FUNCTION: STBC 0x0079c5c0 _$E23
// FUNCTION: STBC 0x0079c5d0 _$E20
// FUNCTION: STBC 0x0079c5e0 _$E22
// FUNCTION: STBC 0x0079c5f0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0079c530 _$E32
// FUNCTION: STBC 0x0079c540 _$E31
// FUNCTION: STBC 0x0079c600 _$E35
// FUNCTION: STBC 0x0079c610 _$E34
// FUNCTION: STBC 0x0079c620 _$E38
// FUNCTION: STBC 0x0079c630 _$E37
// FUNCTION: STBC 0x0079c640 _$E41
// FUNCTION: STBC 0x0079c650 _$E40

/*
================
NiMorphKey::Register
================
*/
// FUNCTION: STBC 0x79BFC0
int NiMorphKey::Register() {
	static bool registered = false;
	if ( registered ) {
		return 0;
	}
	registered = true;
	createFunctions[MORPHKEY] = Create;
	saveFunctions[MORPHKEY] = Save;
	interpFunctions[MORPHKEY] = Interpolate;
	equalFunctions[MORPHKEY] = Equal;
	copyFunctions[MORPHKEY] = Copy;
	fillDerivedValuesFunctions[MORPHKEY] = FillDerivedValues;
	return 1;
}

/*
================
NiMorphKey::NiMorphKey

The key's own value is unused; the tangents are derived later.
================
*/
// FUNCTION: STBC 0x79C020
NiMorphKey::NiMorphKey( float time_, float tension_, float continuity_, float bias_, float *values_ )
	: NiKeyType4FloatKey( time_, 0.0f, tension_, continuity_, bias_ ) {
	values = values_;
	dsValues = 0;
	ddValues = 0;
}

/*
================
NiMorphKey::~NiMorphKey
================
*/
// FUNCTION: STBC 0x79C060
NiMorphKey::~NiMorphKey() {
	delete [] values;
	delete [] dsValues;
	delete [] ddValues;
}

/*
================
NiMorphKey::Equal
================
*/
// FUNCTION: STBC 0x79C090
bool NiMorphKey::Equal( NiFloatKey *key0, NiFloatKey *key1, int numValues ) {
	if ( !NiKeyType4FloatKey::Equal( key0, key1, numValues ) ) {
		return false;
	}

	NiMorphKey *morphKey0 = (NiMorphKey *)key0;
	NiMorphKey *morphKey1 = (NiMorphKey *)key1;
	unsigned int size = numValues * sizeof( float );
	if ( memcmp( morphKey0->values, morphKey1->values, size ) != 0 ) {
		return false;
	}
	if ( memcmp( morphKey0->dsValues, morphKey1->dsValues, size ) != 0 ) {
		return false;
	}
	return memcmp( morphKey0->ddValues, morphKey1->ddValues, size ) == 0;
}

/*
================
NiMorphKey::SetValues
================
*/
// FUNCTION: STBC 0x79C100
void NiMorphKey::SetValues( float *values_ ) {
	delete [] values;
	values = values_;
}

/*
================
NiMorphKey::Copy

The copy is not checked for a failed allocation before its tangents are set.
================
*/
// FUNCTION: STBC 0x79C120
NiFloatKey *NiMorphKey::Copy( NiFloatKey *key, int numValues ) {
	NiMorphKey *morphKey = (NiMorphKey *)key;
	unsigned int size = numValues * sizeof( float );

	float *values = 0;
	if ( morphKey->values ) {
		values = new float[numValues];
		memcpy( values, morphKey->values, size );
	}

	NiMorphKey *copy = new NiMorphKey( morphKey->time, morphKey->tension, morphKey->continuity, morphKey->bias, values );
	copy->value = morphKey->value;
	copy->dd = morphKey->dd;
	copy->ds = morphKey->ds;

	if ( morphKey->dsValues ) {
		copy->dsValues = new float[numValues];
		memcpy( copy->dsValues, morphKey->dsValues, size );
	} else {
		copy->dsValues = 0;
	}

	if ( morphKey->ddValues ) {
		copy->ddValues = new float[numValues];
		memcpy( copy->ddValues, morphKey->ddValues, size );
	} else {
		copy->ddValues = 0;
	}

	return copy;
}

/*
================
NiMorphKey::FillDerivedValues

Derives the tangents of every value in turn: the keys' own value takes each
value of the keys around the one derived, and the TCB tangents are kept in
the arrays.
================
*/
// FUNCTION: STBC 0x79C220
void NiMorphKey::FillDerivedValues( NiFloatKey **keys, unsigned int numKeys, int numValues ) {
	if ( !keys || numKeys < 2 ) {
		return;
	}

	NiMorphKey **morphKeys = (NiMorphKey **)keys;
	unsigned int i;
	for ( i = 0; i < numKeys; i++ ) {
		delete [] morphKeys[i]->dsValues;
		delete [] morphKeys[i]->ddValues;
		morphKeys[i]->dsValues = new float[numValues];
		morphKeys[i]->ddValues = new float[numValues];
	}

	for ( unsigned int j = 0; j < (unsigned int)numValues; j++ ) {
		unsigned int last = numKeys - 1;

		morphKeys[0]->value = morphKeys[0]->values[j];
		morphKeys[1]->value = morphKeys[1]->values[j];
		CalculateDVals( morphKeys[0], 2.0f * morphKeys[0]->value - morphKeys[1]->value, morphKeys[1]->value, 1.0f, 1.0f );
		morphKeys[0]->ddValues[j] = morphKeys[0]->dd;
		morphKeys[0]->dsValues[j] = morphKeys[0]->ds;

		for ( i = 1; i < last; i++ ) {
			morphKeys[i - 1]->value = morphKeys[i - 1]->values[j];
			morphKeys[i]->value = morphKeys[i]->values[j];
			morphKeys[i + 1]->value = morphKeys[i + 1]->values[j];
			CalculateDVals( morphKeys[i], morphKeys[i - 1]->value, morphKeys[i + 1]->value,
				morphKeys[i]->time - morphKeys[i - 1]->time, morphKeys[i + 1]->time - morphKeys[i]->time );
			morphKeys[i]->ddValues[j] = morphKeys[i]->dd;
			morphKeys[i]->dsValues[j] = morphKeys[i]->ds;
		}

		morphKeys[numKeys - 2]->value = morphKeys[numKeys - 2]->values[j];
		morphKeys[last]->value = morphKeys[last]->values[j];
		CalculateDVals( morphKeys[last], morphKeys[numKeys - 2]->value, 2.0f * morphKeys[last]->value - morphKeys[numKeys - 2]->value, 1.0f, 1.0f );
		morphKeys[last]->ddValues[j] = morphKeys[last]->dd;
		morphKeys[last]->dsValues[j] = morphKeys[last]->ds;
	}
}

/*
================
NiMorphKey::Interpolate

A morph key's own value does not interpolate; its values do, through
InterpolateValues.
================
*/
// FUNCTION: STBC 0x79C3F0
float NiMorphKey::Interpolate( float t, NiFloatKey *key0, NiFloatKey *key1 ) {
	return 0.0f;
}

/*
================
NiMorphKey::InterpolateValues
================
*/
// FUNCTION: STBC 0x79C400
float *NiMorphKey::InterpolateValues( float t, NiMorphKey *key0, NiMorphKey *key1, unsigned int numValues ) {
	float *values = new float[numValues];
	for ( unsigned int i = 0; i < numValues; i++ ) {
		values[i] = NiHermiteInterp( t, key0->values[i], key0->ddValues[i], key1->values[i], key1->dsValues[i] );
	}
	return values;
}

/*
================
NiMorphKey::Create
================
*/
// FUNCTION: STBC 0x79C470
NiFloatKey *NiMorphKey::Create( NiStream &stream, int numValues ) {
	NiMorphKey *key = new NiMorphKey;
	key->LoadBinary( stream, numValues );
	return key;
}

/*
================
NiMorphKey::LoadBinary
================
*/
// FUNCTION: STBC 0x79C4B0
void NiMorphKey::LoadBinary( NiStream &stream, int numValues ) {
	NiKeyType4FloatKey::LoadBinary( stream, numValues );
	values = new float[numValues];
	NiStreamLoadBinary( stream, values, numValues );
}

/*
================
NiMorphKey::Save
================
*/
// FUNCTION: STBC 0x79C4F0
void NiMorphKey::Save( NiStream &stream, NiFloatKey *key, int numValues ) {
	NiKeyType4FloatKey::Save( stream, key, numValues );

	NiMorphKey *morphKey = (NiMorphKey *)key;
	stream.GetOstr()->Write( morphKey->values, numValues * sizeof( float ) );
}
