// NiBezRotKey.cpp -- rotation keys on a squad curve

#include "NiRTLib.h"
#include "NiBezRotKey.h"
#include "NiStream.h"
#include "NiBezPosKey.h"
#include "NiPosKey.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x0079d3b0 _$E73
// FUNCTION: STBC 0x0079d3d0 _$E72
// FUNCTION: STBC 0x0079d3e0 _$E17
// FUNCTION: STBC 0x0079d3f0 _$E14
// FUNCTION: STBC 0x0079d400 _$E16
// FUNCTION: STBC 0x0079d410 _$E15
// FUNCTION: STBC 0x0079d420 _$E23
// FUNCTION: STBC 0x0079d430 _$E20
// FUNCTION: STBC 0x0079d440 _$E22
// FUNCTION: STBC 0x0079d450 _$E21

/*
================
NiBezRotKey::Register
================
*/
// FUNCTION: STBC 0x79D0C0
int NiBezRotKey::Register() {
	static bool registered = false;
	if ( registered ) {
		return 0;
	}
	registered = true;
	createFunctions[BEZKEY] = Create;
	interpFunctions[BEZKEY] = Interpolate;
	fillDerivedValuesFunctions[BEZKEY] = FillDerivedValues;
	saveFunctions[BEZKEY] = Save;
	equalFunctions[BEZKEY] = Equal;
	copyFunctions[BEZKEY] = Copy;
	return 1;
}

/*
================
NiBezRotKey::Equal
================
*/
// FUNCTION: STBC 0x79D120
bool NiBezRotKey::Equal( NiRotKey *key0, NiRotKey *key1 ) {
	if ( !NiRotKey::Equal( key0, key1 ) ) {
		return false;
	}

	NiBezRotKey *bezKey0 = (NiBezRotKey *)key0;
	NiBezRotKey *bezKey1 = (NiBezRotKey *)key1;
	return bezKey0->intQuat.w == bezKey1->intQuat.w
		&& bezKey0->intQuat.x == bezKey1->intQuat.x
		&& bezKey0->intQuat.y == bezKey1->intQuat.y
		&& bezKey0->intQuat.z == bezKey1->intQuat.z;
}

/*
================
NiBezRotKey::Copy

The copy is not checked for a failed allocation before its control point is
set.
================
*/
// FUNCTION: STBC 0x79D180
NiRotKey *NiBezRotKey::Copy( NiRotKey *key ) {
	NiBezRotKey *bezKey = (NiBezRotKey *)key;
	NiBezRotKey *copy = new NiBezRotKey( bezKey->time, bezKey->quat );
	copy->intQuat = bezKey->intQuat;
	return copy;
}

/*
================
NiBezRotKey::Interpolate
================
*/
// FUNCTION: STBC 0x79D200
NiQuaternion NiBezRotKey::Interpolate( float t, NiRotKey *key0, NiRotKey *key1 ) {
	NiBezRotKey *bezKey0 = (NiBezRotKey *)key0;
	NiBezRotKey *bezKey1 = (NiBezRotKey *)key1;
	return NiQuaternion::Squad( t, bezKey0->quat, bezKey0->intQuat, bezKey1->intQuat, bezKey1->quat );
}

/*
================
NiBezRotKey::FillDerivedValues

The end keys take themselves for the missing neighbour.
================
*/
// FUNCTION: STBC 0x79D230
void NiBezRotKey::FillDerivedValues( NiRotKey **keys, unsigned int numKeys ) {
	if ( numKeys < 2 ) {
		return;
	}

	NiRotKey::FillDerivedValues( keys, numKeys );

	NiBezRotKey **bezKeys = (NiBezRotKey **)keys;
	bezKeys[0]->intQuat = NiQuaternion::Intermediate( bezKeys[0]->quat, bezKeys[0]->quat, bezKeys[1]->quat );

	unsigned int last = numKeys - 1;
	for ( unsigned int i = 1; i < last; i++ ) {
		bezKeys[i]->intQuat = NiQuaternion::Intermediate( bezKeys[i - 1]->quat, bezKeys[i]->quat, bezKeys[i + 1]->quat );
	}

	bezKeys[last]->intQuat = NiQuaternion::Intermediate( bezKeys[numKeys - 2]->quat, bezKeys[last]->quat, bezKeys[last]->quat );
}

/*
================
NiBezRotKey::Create
================
*/
// FUNCTION: STBC 0x79D300
NiRotKey *NiBezRotKey::Create( NiStream &stream ) {
	NiBezRotKey *key = new NiBezRotKey;
	key->LoadBinary( stream );
	return key;
}

/*
================
NiBezRotKey::LoadBinary
================
*/
// FUNCTION: STBC 0x79D380
void NiBezRotKey::LoadBinary( NiStream &stream ) {
	NiRotKey::LoadBinary( stream );
}

/*
================
NiBezRotKey::Save
================
*/
// FUNCTION: STBC 0x79D390
void NiBezRotKey::Save( NiStream &stream, NiRotKey *key ) {
	NiRotKey::Save( stream, key );
}
