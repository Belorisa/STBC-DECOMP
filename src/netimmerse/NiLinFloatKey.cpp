// NiLinFloatKey.cpp -- linearly interpolated float keys

#include "NiRTLib.h"
#include "NiLinFloatKey.h"
#include "NiInterpolation.h"
#include "NiStream.h"
#include "NiAVObject.h"
#include "NiColorKey.h"
#include "NiDynamicEffect.h"
#include "NiExtraData.h"
#include "NiLight.h"
#include "NiLightColorController.h"
#include "NiLinColorKey.h"
#include "NiObject.h"
#include "NiObjectNET.h"
#include "NiPosData.h"
#include "NiPosKey.h"
#include "NiProperty.h"
#include "NiTimeController.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x007a7540 _$E238
// FUNCTION: STBC 0x007a7560 _$E237
// FUNCTION: STBC 0x007a7570 _$E17
// FUNCTION: STBC 0x007a7580 _$E14
// FUNCTION: STBC 0x007a7590 _$E16
// FUNCTION: STBC 0x007a75a0 _$E15
// FUNCTION: STBC 0x007a75b0 _$E23
// FUNCTION: STBC 0x007a75c0 _$E20
// FUNCTION: STBC 0x007a75d0 _$E22
// FUNCTION: STBC 0x007a75e0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007a7370 _$E197
// FUNCTION: STBC 0x007a7380 _$E196
// FUNCTION: STBC 0x007a7520 _$E32
// FUNCTION: STBC 0x007a7530 _$E31
// FUNCTION: STBC 0x007a75f0 _$E35
// FUNCTION: STBC 0x007a7600 _$E34

/*
================
NiLinFloatKey::Register
================
*/
// FUNCTION: STBC 0x7A73D0
int NiLinFloatKey::Register() {
	static bool registered = false;
	if ( registered ) {
		return 0;
	}
	registered = true;
	createFunctions[LINKEY] = Create;
	saveFunctions[LINKEY] = Save;
	interpFunctions[LINKEY] = Interpolate;
	equalFunctions[LINKEY] = Equal;
	copyFunctions[LINKEY] = Copy;
	fillDerivedValuesFunctions[LINKEY] = FillDerivedValues;
	return 1;
}

/*
================
NiLinFloatKey::FillDerivedValues
================
*/
// FUNCTION: STBC 0x7A7430
void NiLinFloatKey::FillDerivedValues( NiFloatKey **keys, unsigned int numKeys, int unknown ) {
}

/*
================
NiLinFloatKey::Interpolate
================
*/
// FUNCTION: STBC 0x7A7440
float NiLinFloatKey::Interpolate( float t, NiFloatKey *key0, NiFloatKey *key1 ) {
	return NiLinearInterp( t, key0->GetValue(), key1->GetValue() );
}

/*
================
NiLinFloatKey::Equal
================
*/
// FUNCTION: STBC 0x7A7470
bool NiLinFloatKey::Equal( NiFloatKey *key0, NiFloatKey *key1, int unknown ) {
	return NiFloatKey::Equal( key0, key1, unknown );
}

/*
================
NiLinFloatKey::Copy
================
*/
// FUNCTION: STBC 0x7A7490
NiFloatKey *NiLinFloatKey::Copy( NiFloatKey *key, int unknown ) {
	return new NiLinFloatKey( key->time, key->value );
}

/*
================
NiLinFloatKey::Create
================
*/
// FUNCTION: STBC 0x7A74B0
NiFloatKey *NiLinFloatKey::Create( NiStream &stream, int unknown ) {
	NiLinFloatKey *key = new NiLinFloatKey;
	key->LoadBinary( stream, unknown );
	return key;
}

/*
================
NiLinFloatKey::LoadBinary
================
*/
// FUNCTION: STBC 0x7A74E0
void NiLinFloatKey::LoadBinary( NiStream &stream, int unknown ) {
	NiFloatKey::LoadBinary( stream, unknown );
}

/*
================
NiLinFloatKey::Save
================
*/
// FUNCTION: STBC 0x7A7500
void NiLinFloatKey::Save( NiStream &stream, NiFloatKey *key, int unknown ) {
	NiFloatKey::Save( stream, key, unknown );
}
