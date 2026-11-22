// NiPosKey.cpp -- position keys and their per-type function tables

#include "NiRTLib.h"
#include "NiPosKey.h"
#include "NiStream.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x007919b0 _$E59
// FUNCTION: STBC 0x007919d0 _$E58
// FUNCTION: STBC 0x007919e0 _$E17
// FUNCTION: STBC 0x007919f0 _$E14
// FUNCTION: STBC 0x00791a00 _$E16
// FUNCTION: STBC 0x00791a10 _$E15
// FUNCTION: STBC 0x00791a20 _$E23
// FUNCTION: STBC 0x00791a30 _$E20
// FUNCTION: STBC 0x00791a40 _$E22
// FUNCTION: STBC 0x00791a50 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00791a60 _$E42
// FUNCTION: STBC 0x00791a70 _$E41

// DATA: STBC 0x99FEC8 ?curvatureFunctions@NiPosKey@@2PAP6AMPAPAV1@I@ZA
NiPosKey::CurvatureFunction NiPosKey::curvatureFunctions[NUMKEYTYPES];
// DATA: STBC 0x99FED8 ?copyFunctions@NiPosKey@@2PAP6APAV1@PAV1@@ZA
NiPosKey::CopyFunction NiPosKey::copyFunctions[NUMKEYTYPES];
// DATA: STBC 0x99FEE8 ?saveFunctions@NiPosKey@@2PAP6AXAAVNiStream@@PAV1@@ZA
NiPosKey::SaveFunction NiPosKey::saveFunctions[NUMKEYTYPES];
// DATA: STBC 0x99FEFC ?interpD1Functions@NiPosKey@@2PAP6A?AVNiPoint3@@MPAV1@0@ZA
NiPosKey::InterpFunction NiPosKey::interpD1Functions[NUMKEYTYPES];
// DATA: STBC 0x99FF10 ?interpD2Functions@NiPosKey@@2PAP6A?AVNiPoint3@@MPAV1@0@ZA
NiPosKey::InterpFunction NiPosKey::interpD2Functions[NUMKEYTYPES];
// DATA: STBC 0x99FF20 ?createFunctions@NiPosKey@@2PAP6APAV1@AAVNiStream@@@ZA
NiPosKey::CreateFunction NiPosKey::createFunctions[NUMKEYTYPES];
// DATA: STBC 0x99FF30 ?fillDerivedValuesFunctions@NiPosKey@@2PAP6AXPAPAV1@I@ZA
NiPosKey::FillDerivedValuesFunction NiPosKey::fillDerivedValuesFunctions[NUMKEYTYPES];
// DATA: STBC 0x99FF40 ?equalFunctions@NiPosKey@@2PAP6A_NPAV1@0@ZA
NiPosKey::EqualFunction NiPosKey::equalFunctions[NUMKEYTYPES];
// DATA: STBC 0x99FF54 ?interpFunctions@NiPosKey@@2PAP6A?AVNiPoint3@@MPAV1@0@ZA
NiPosKey::InterpFunction NiPosKey::interpFunctions[NUMKEYTYPES];

// DATA: STBC 0x975ED0 ?sampleSegments@NiPosKey@@2HA
int NiPosKey::sampleSegments = 20;
// DATA: STBC 0x99FF50 ?sampleDelta@NiPosKey@@2MA
float NiPosKey::sampleDelta = 1.0f / sampleSegments;

/*
================
NiPosKey::Register
================
*/
// FUNCTION: STBC 0x791420
int NiPosKey::Register() {
	static bool registered = false;
	if ( registered ) {
		return 0;
	}
	registered = true;
	createFunctions[NOINTERP] = Create;
	saveFunctions[NOINTERP] = Save;
	curvatureFunctions[NOINTERP] = ComputeMaxCurvature;
	equalFunctions[NOINTERP] = Equal;
	copyFunctions[NOINTERP] = Copy;
	interpFunctions[NOINTERP] = Interpolate;
	interpD1Functions[NOINTERP] = InterpolateD1;
	interpD2Functions[NOINTERP] = InterpolateD2;
	fillDerivedValuesFunctions[NOINTERP] = FillDerivedValues;
	return 1;
}

/*
================
NiPosKey::GenInterp

Times before the first key or after the last answer that key's position;
the search for the pair around time starts again at lastIndex unless time
went back before it.
================
*/
// FUNCTION: STBC 0x7914D0
NiPoint3 NiPosKey::GenInterp( float time, NiPosKey **keys, KeyType keyType, unsigned int numKeys, unsigned int &lastIndex ) {
	if ( keys[0]->time >= time ) {
		lastIndex = 0;
		return keys[0]->pos;
	}

	unsigned int last = numKeys - 1;
	if ( keys[last]->time <= time ) {
		lastIndex = last;
		return keys[last]->pos;
	}

	float lastTime = keys[lastIndex]->time;
	if ( time < lastTime ) {
		lastIndex = 0;
		lastTime = keys[0]->time;
	}

	unsigned int next;
	float nextTime;
	for ( next = lastIndex + 1; next <= last; next++ ) {
		nextTime = keys[next]->time;
		if ( time < nextTime ) {
			break;
		}
		lastIndex++;
		lastTime = nextTime;
	}

	float t = ( time - lastTime ) / ( nextTime - lastTime );
	InterpFunction interp = interpFunctions[keyType];
	return interp( t, keys[lastIndex], keys[next] );
}

/*
================
NiPosKey::Copy

There are no NOINTERP keys to copy.
================
*/
// FUNCTION: STBC 0x791600
NiPosKey *NiPosKey::Copy( NiPosKey *key ) {
	return 0;
}

/*
================
NiPosKey::Equal
================
*/
// FUNCTION: STBC 0x791610
bool NiPosKey::Equal( NiPosKey *key0, NiPosKey *key1 ) {
	return key0->time == key1->time
		&& key0->pos.x == key1->pos.x
		&& key0->pos.y == key1->pos.y
		&& key0->pos.z == key1->pos.z;
}

/*
================
NiPosKey::CoordinateFrame

The Frenet frame of the path: the tangent is the unit first derivative,
the normal the part of the second derivative across it, and the binormal
completes the frame. Where the path does not bend the normal is any
direction across the tangent.
================
*/
// FUNCTION: STBC 0x791660
void NiPosKey::CoordinateFrame( float t, NiPosKey *key0, NiPosKey *key1, KeyType keyType, NiPoint3 &tangent, NiPoint3 &normal, NiPoint3 &binormal, float &curvature ) {
	InterpFunction interpD1 = interpD1Functions[keyType];
	InterpFunction interpD2 = interpD2Functions[keyType];
	NiPoint3 d1 = interpD1( t, key0, key1 );
	NiPoint3 d2 = interpD2( t, key0, key1 );

	// the unit tangent (Gamebryo 1.2.2: NiPosKey::CoordinateFrame)
	float lengthSqr = d1.SqrLength();
	float length = NiSqrt( lengthSqr );
	float invLength = 1.0f / length;
	tangent = invLength * d1;

	// the curvature times the speed, which is invariant to the units
	NiPoint3 cross = d1.Cross( d2 );
	curvature = cross.Length() * invLength * invLength;

	if ( NiAbs( curvature ) > 1e-06f ) {
		normal = lengthSqr * d2 - d1.Dot( d2 ) * d1;
	} else {
		curvature = 0.0f;
		if ( NiAbs( tangent.x ) > 1e-06f || NiAbs( tangent.y ) > 1e-06f ) {
			normal.x = tangent.y;
			normal.y = -tangent.x;
			normal.z = 0.0f;
		} else {
			normal.z = tangent.y;
			normal.y = -tangent.z;
			normal.x = 0.0f;
		}
	}
	normal.Unitize();
	binormal = tangent.Cross( normal );
}

/*
================
NiPosKey::Create

There are no NOINTERP keys to read.
================
*/
// FUNCTION: STBC 0x791940
NiPosKey *NiPosKey::Create( NiStream &stream ) {
	return 0;
}

/*
================
NiPosKey::LoadBinary
================
*/
// FUNCTION: STBC 0x791950
void NiPosKey::LoadBinary( NiStream &stream ) {
	NiStreamLoadBinary( stream, time );
	pos.LoadBinary( stream );
}

/*
================
NiPosKey::Save
================
*/
// FUNCTION: STBC 0x791980
void NiPosKey::Save( NiStream &stream, NiPosKey *key ) {
	NiStreamSaveBinary( stream, key->time );
	key->pos.SaveBinary( stream );
}

/*
================
NiPosKey::Interpolate

Keys that do not interpolate stay at the origin.
================
*/
// FUNCTION: STBC 0x791A80
NiPoint3 NiPosKey::Interpolate( float t, NiPosKey *key0, NiPosKey *key1 ) {
	return NiPoint3::ZERO;
}

/*
================
NiPosKey::InterpolateD1
================
*/
// FUNCTION: STBC 0x791AA0
NiPoint3 NiPosKey::InterpolateD1( float t, NiPosKey *key0, NiPosKey *key1 ) {
	return NiPoint3::ZERO;
}

/*
================
NiPosKey::InterpolateD2
================
*/
// FUNCTION: STBC 0x791AC0
NiPoint3 NiPosKey::InterpolateD2( float t, NiPosKey *key0, NiPosKey *key1 ) {
	return NiPoint3::ZERO;
}

/*
================
NiPosKey::FillDerivedValues
================
*/
// FUNCTION: STBC 0x791AE0
void NiPosKey::FillDerivedValues( NiPosKey **keys, unsigned int numKeys ) {
}

/*
================
NiPosKey::ComputeMaxCurvature
================
*/
// FUNCTION: STBC 0x791AF0
float NiPosKey::ComputeMaxCurvature( NiPosKey **keys, unsigned int numKeys ) {
	return 0.0f;
}
