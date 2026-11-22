// NiEulerRotKey.cpp -- rotation keys as three animated Euler angles

#include "NiRTLib.h"
#include <string.h>

#include "NiEulerRotKey.h"
#include "NiStream.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x00796aa0 _$E66
// FUNCTION: STBC 0x00796ac0 _$E65
// FUNCTION: STBC 0x00796ad0 _$E17
// FUNCTION: STBC 0x00796ae0 _$E14
// FUNCTION: STBC 0x00796af0 _$E16
// FUNCTION: STBC 0x00796b00 _$E15
// FUNCTION: STBC 0x00796b10 _$E23
// FUNCTION: STBC 0x00796b20 _$E20
// FUNCTION: STBC 0x00796b30 _$E22
// FUNCTION: STBC 0x00796b40 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00796a80 _$E32
// FUNCTION: STBC 0x00796a90 _$E31
// FUNCTION: STBC 0x00796b50 _$E48
// FUNCTION: STBC 0x00796b60 _$E47
// FUNCTION: STBC 0x00796b70 _$E52
// FUNCTION: STBC 0x00796b80 _$E51

// DATA: STBC 0x975FD0 ?axisOrderMap@NiEulerRotKey@@2PAY02HA
int NiEulerRotKey::axisOrderMap[NUMAXISORDERS][3] = {
	{ 0, 1, 2 },
	{ 0, 2, 1 },
	{ 1, 2, 0 },
	{ 1, 0, 2 },
	{ 2, 0, 1 },
	{ 2, 1, 0 },
	{ 0, 1, 0 },
	{ 1, 2, 1 },
	{ 2, 0, 2 }
};

/*
================
NiEulerRotKey::Register
================
*/
// FUNCTION: STBC 0x7964B0
int NiEulerRotKey::Register() {
	static bool registered = false;
	if ( registered ) {
		return 0;
	}
	registered = true;
	createFunctions[EULERKEY] = Create;
	interpFunctions[EULERKEY] = Interpolate;
	fillDerivedValuesFunctions[EULERKEY] = FillDerivedValues;
	saveFunctions[EULERKEY] = Save;
	equalFunctions[EULERKEY] = Equal;
	copyFunctions[EULERKEY] = Copy;
	return 1;
}

/*
================
NiEulerRotKey::Cleanup
================
*/
// FUNCTION: STBC 0x796510
void NiEulerRotKey::Cleanup() {
	for ( int i = 0; i < 3; i++ ) {
		for ( unsigned int j = 0; j < numKeys[i]; j++ ) {
			delete keys[i][j];
		}
		delete [] keys[i];
		keys[i] = 0;
		numKeys[i] = 0;
		keyTypes[i] = NiFloatKey::NOINTERP;
	}
}

/*
================
NiEulerRotKey::Interpolate

The one key holds the whole animation: t is the time itself and key1 is
unused (asserted). An axis without keys keeps a zero angle. As the debug
copy has it (AnimOnly.exe 0x47D725, NiEulerRotKey.cpp), which reads the
axis order through an accessor this tree's class doesn't declare yet.
================
*/
// FUNCTION: STBC 0x796560
NiQuaternion NiEulerRotKey::Interpolate( float t, NiRotKey *key0, NiRotKey *key1 ) {
	assert( key1 == 0 );
	NiEulerRotKey *key = (NiEulerRotKey *)key0;

	float angles[3];
	unsigned int i;
	for ( i = 0; i < 3; i++ ) {
		if ( key->numKeys[i] == 0 ) {
			angles[i] = 0.0f;
		} else {
			angles[i] = NiFloatKey::GenInterp( t, key->keys[i], key->keyTypes[i], key->numKeys[i], key->lastIndex[i] );
		}
	}

	NiQuaternion axisQuats[3];
	axisQuats[0].FromAngleAxis( angles[0], NiPoint3::UNIT_X );
	axisQuats[1].FromAngleAxis( angles[1], NiPoint3::UNIT_Y );
	axisQuats[2].FromAngleAxis( angles[2], NiPoint3::UNIT_Z );

	NiQuaternion result( 1.0f, 0.0f, 0.0f, 0.0f );
	for ( i = 0; i < 3; i++ ) {
		int axis = axisOrderMap[key->GetOrder()][i];
		result = axisQuats[axis] * result;
	}
	return result;
}

/*
================
NiEulerRotKey::Equal

Compares the axis order and the keys, not the key types. As the debug copy
has it (AnimOnly.exe 0x47D87B, NiEulerRotKey.cpp), whose inner loop also
stops at the first axis's count.
================
*/
// FUNCTION: STBC 0x796650
bool NiEulerRotKey::Equal( NiRotKey *key0, NiRotKey *key1 ) {
	NiEulerRotKey *euler0 = (NiEulerRotKey *)key0;
	NiEulerRotKey *euler1 = (NiEulerRotKey *)key1;
	if ( euler0->axisOrder != euler1->axisOrder ) {
		return false;
	}

	for ( unsigned int i = 0; i < 3; i++ ) {
		if ( euler0->numKeys[i] != euler1->numKeys[i] ) {
			return false;
		}
		if ( euler0->numKeys[i] > 0 ) {
			NiFloatKey::EqualFunction equal = NiFloatKey::GetEqualFunction( euler0->keyTypes[i] );
			assert( equal );
			// BUG: every axis's keys are compared up to the first axis's count
			for ( unsigned int j = 0; j < euler0->numKeys[0]; j++ ) {
				if ( !equal( euler0->keys[i][j], euler1->keys[i][j], 0 ) ) {
					return false;
				}
			}
		}
	}
	return true;
}

/*
================
NiEulerRotKey::Copy
================
*/
// FUNCTION: STBC 0x7966F0
NiRotKey *NiEulerRotKey::Copy( NiRotKey *key ) {
	NiEulerRotKey *euler = (NiEulerRotKey *)key;
	NiEulerRotKey *copy = new NiEulerRotKey;
	copy->axisOrder = euler->axisOrder;
	for ( int i = 0; i < 3; i++ ) {
		copy->numKeys[i] = euler->numKeys[i];
		copy->keyTypes[i] = euler->keyTypes[i];
		unsigned int count = euler->numKeys[i];
		if ( count ) {
			copy->keys[i] = new NiFloatKey *[count];
			NiFloatKey::CopyFunction copyKey = NiFloatKey::copyFunctions[euler->keyTypes[i]];
			for ( unsigned int j = 0; j < count; j++ ) {
				copy->keys[i][j] = copyKey( euler->keys[i][j], 0 );
			}
		}
	}
	return copy;
}

/*
================
NiEulerRotKey::FillDerivedValues
================
*/
// FUNCTION: STBC 0x796820
void NiEulerRotKey::FillDerivedValues( NiRotKey **keys, unsigned int numKeys ) {
	NiEulerRotKey *euler = (NiEulerRotKey *)keys[0];
	for ( int i = 0; i < 3; i++ ) {
		if ( euler->keys[i] ) {
			NiFloatKey::GetFillDerivedFunction( euler->keyTypes[i] )( euler->keys[i], euler->numKeys[i], 0 );
		}
	}
}

/*
================
NiEulerRotKey::Create
================
*/
// FUNCTION: STBC 0x796860
NiRotKey *NiEulerRotKey::Create( NiStream &stream ) {
	NiEulerRotKey *key = new NiEulerRotKey;
	key->LoadBinary( stream );
	return key;
}

/*
================
NiEulerRotKey::LoadBinary

Files up to version 2.60 store the axis order in two bytes.
================
*/
// FUNCTION: STBC 0x796900
void NiEulerRotKey::LoadBinary( NiStream &stream ) {
	if ( strcmp( stream.GetFileVersion(), "2.60" ) <= 0 ) {
		short order;
		NiStreamLoadBinary( stream, order );
		axisOrder = order;
	} else {
		int order;
		NiStreamLoadBinary( stream, order );
		axisOrder = order;
	}

	for ( int i = 0; i < 3; i++ ) {
		NiStreamLoadBinary( stream, numKeys[i] );
		if ( numKeys[i] > 0 ) {
			NiFloatKey::KeyType keyType;
			NiStreamLoadEnum( stream, keyType );
			keyTypes[i] = keyType;
			keys[i] = new NiFloatKey *[numKeys[i]];
			NiFloatKey::CreateFunction create = NiFloatKey::GetCreateFunction( keyType );
			for ( unsigned int j = 0; j < numKeys[i]; j++ ) {
				keys[i][j] = create( stream, 0 );
			}
		}
	}
}

/*
================
NiEulerRotKey::Save
================
*/
// FUNCTION: STBC 0x7969F0
void NiEulerRotKey::Save( NiStream &stream, NiRotKey *key ) {
	NiEulerRotKey *euler = (NiEulerRotKey *)key;
	NiStreamSaveBinary( stream, euler->axisOrder );
	for ( int i = 0; i < 3; i++ ) {
		NiStreamSaveBinary( stream, euler->numKeys[i] );
		if ( euler->numKeys[i] > 0 ) {
			NiStreamSaveBinary( stream, euler->keyTypes[i] );
			NiFloatKey::SaveFunction save = NiFloatKey::GetSaveFunction( euler->keyTypes[i] );
			for ( unsigned int j = 0; j < euler->numKeys[i]; j++ ) {
				save( stream, euler->keys[i][j], 0 );
			}
		}
	}
}
