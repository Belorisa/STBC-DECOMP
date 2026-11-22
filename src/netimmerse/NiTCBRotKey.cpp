// NiTCBRotKey.cpp -- rotation keys on a Kochanek-Bartels spline

#include "NiRTLib.h"
#include "NiTCBRotKey.h"
#include "NiStream.h"
#include "NiPosKey.h"
#include "NiTCBPosKey.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x007b5a50 _$E73
// FUNCTION: STBC 0x007b5a70 _$E72
// FUNCTION: STBC 0x007b5a80 _$E17
// FUNCTION: STBC 0x007b5a90 _$E14
// FUNCTION: STBC 0x007b5aa0 _$E16
// FUNCTION: STBC 0x007b5ab0 _$E15
// FUNCTION: STBC 0x007b5ac0 _$E23
// FUNCTION: STBC 0x007b5ad0 _$E20
// FUNCTION: STBC 0x007b5ae0 _$E22
// FUNCTION: STBC 0x007b5af0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007b5460 _$E63
// FUNCTION: STBC 0x007b5470 _$E62
// FUNCTION: STBC 0x007b5b00 _$E39
// FUNCTION: STBC 0x007b5b10 _$E38
// FUNCTION: STBC 0x007b5b20 _$E42
// FUNCTION: STBC 0x007b5b30 _$E41

/*
================
NiTCBRotKey::Register
================
*/
// FUNCTION: STBC 0x7B54A0
int NiTCBRotKey::Register() {
	static bool registered = false;
	if ( registered ) {
		return 0;
	}
	registered = true;
	createFunctions[TCBKEY] = Create;
	interpFunctions[TCBKEY] = Interpolate;
	fillDerivedValuesFunctions[TCBKEY] = FillDerivedValues;
	saveFunctions[TCBKEY] = Save;
	equalFunctions[TCBKEY] = Equal;
	copyFunctions[TCBKEY] = Copy;
	return 1;
}

/*
================
NiTCBRotKey::Equal
================
*/
// FUNCTION: STBC 0x7B5500
bool NiTCBRotKey::Equal( NiRotKey *key0, NiRotKey *key1 ) {
	if ( !NiRotKey::Equal( key0, key1 ) ) {
		return false;
	}

	NiTCBRotKey *tcbKey0 = (NiTCBRotKey *)key0;
	NiTCBRotKey *tcbKey1 = (NiTCBRotKey *)key1;
	return tcbKey0->tension == tcbKey1->tension
		&& tcbKey0->continuity == tcbKey1->continuity
		&& tcbKey0->bias == tcbKey1->bias
		&& tcbKey0->a.w == tcbKey1->a.w
		&& tcbKey0->a.x == tcbKey1->a.x
		&& tcbKey0->a.y == tcbKey1->a.y
		&& tcbKey0->a.z == tcbKey1->a.z
		&& tcbKey0->b.w == tcbKey1->b.w
		&& tcbKey0->b.x == tcbKey1->b.x
		&& tcbKey0->b.y == tcbKey1->b.y
		&& tcbKey0->b.z == tcbKey1->b.z;
}

/*
================
NiTCBRotKey::Copy

The copy is not checked for a failed allocation before the rest of the key
is set.
================
*/
// FUNCTION: STBC 0x7B55C0
NiRotKey *NiTCBRotKey::Copy( NiRotKey *key ) {
	NiTCBRotKey *tcbKey = (NiTCBRotKey *)key;
	NiTCBRotKey *copy = new NiTCBRotKey( tcbKey->time, tcbKey->quat );
	copy->tension = tcbKey->tension;
	copy->continuity = tcbKey->continuity;
	copy->bias = tcbKey->bias;
	copy->a = tcbKey->a;
	copy->b = tcbKey->b;
	return copy;
}

/*
================
NiTCBRotKey::Interpolate
================
*/
// FUNCTION: STBC 0x7B5660
NiQuaternion NiTCBRotKey::Interpolate( float t, NiRotKey *key0, NiRotKey *key1 ) {
	NiTCBRotKey *tcbKey0 = (NiTCBRotKey *)key0;
	NiTCBRotKey *tcbKey1 = (NiTCBRotKey *)key1;
	return NiQuaternion::Squad( t, tcbKey0->quat, tcbKey0->a, tcbKey1->b, tcbKey1->quat );
}

/*
================
NiTCBRotKey::CalculateDVals

The Kochanek-Bartels tangents in the logarithms of the steps from the
previous rotation and to the next one, each scaled for its side's share of
the time between the neighbours (one share at a time), turned into the
control points of the squad curves on either side.
================
*/
// FUNCTION: STBC 0x7B5690
void NiTCBRotKey::CalculateDVals( const NiTCBRotKey *prev, const NiTCBRotKey *next ) {
	NiQuaternion logPrev = NiQuaternion::Log( NiQuaternion::UnitInverse( prev->quat ) * quat );
	NiQuaternion logNext = NiQuaternion::Log( NiQuaternion::UnitInverse( quat ) * next->quat );

	float invDuration = 1.0f / ( next->time - prev->time );
	float oneMinusTension = 1.0f - tension;
	float oneMinusContinuity = 1.0f - continuity;
	float onePlusContinuity = 1.0f + continuity;
	float oneMinusBias = 1.0f - bias;
	float onePlusBias = 1.0f + bias;

	float share = ( time - prev->time ) * invDuration;
	// the weights of the two sides (Gamebryo 1.2.2: NiTCBRotKey::CalculateDVals)
	float d0 = share * onePlusBias * onePlusContinuity * oneMinusTension;
	float d1 = share * oneMinusBias * oneMinusContinuity * oneMinusTension;
	NiQuaternion outTangent = d1 * logNext + d0 * logPrev;
	a = quat * NiQuaternion::Exp( 0.5f * ( outTangent - logNext ) );

	share = ( next->time - time ) * invDuration;
	d0 = share * onePlusBias * oneMinusContinuity * oneMinusTension;
	d1 = share * oneMinusBias * onePlusContinuity * oneMinusTension;
	NiQuaternion inTangent = d1 * logNext + d0 * logPrev;
	b = quat * NiQuaternion::Exp( 0.5f * ( logPrev - inTangent ) );
}

/*
================
NiTCBRotKey::FillDerivedValues

The end keys take themselves for the missing neighbour.
================
*/
// FUNCTION: STBC 0x7B58D0
void NiTCBRotKey::FillDerivedValues( NiRotKey **keys, unsigned int numKeys ) {
	if ( numKeys < 2 ) {
		return;
	}

	NiRotKey::FillDerivedValues( keys, numKeys );

	NiTCBRotKey **tcbKeys = (NiTCBRotKey **)keys;
	tcbKeys[0]->CalculateDVals( tcbKeys[0], tcbKeys[1] );

	unsigned int last = numKeys - 1;
	for ( unsigned int i = 1; i < last; i++ ) {
		tcbKeys[i]->CalculateDVals( tcbKeys[i - 1], tcbKeys[i + 1] );
	}

	tcbKeys[last]->CalculateDVals( tcbKeys[numKeys - 2], tcbKeys[last] );
}

/*
================
NiTCBRotKey::Create
================
*/
// FUNCTION: STBC 0x7B5940
NiRotKey *NiTCBRotKey::Create( NiStream &stream ) {
	NiTCBRotKey *key = new NiTCBRotKey;
	key->LoadBinary( stream );
	return key;
}

/*
================
NiTCBRotKey::LoadBinary
================
*/
// FUNCTION: STBC 0x7B59C0
void NiTCBRotKey::LoadBinary( NiStream &stream ) {
	NiRotKey::LoadBinary( stream );
	NiStreamLoadBinary( stream, tension );
	NiStreamLoadBinary( stream, continuity );
	NiStreamLoadBinary( stream, bias );
}

/*
================
NiTCBRotKey::Save
================
*/
// FUNCTION: STBC 0x7B5A00
void NiTCBRotKey::Save( NiStream &stream, NiRotKey *key ) {
	NiRotKey::Save( stream, key );

	NiTCBRotKey *tcbKey = (NiTCBRotKey *)key;
	NiStreamSaveBinary( stream, tcbKey->tension );
	NiStreamSaveBinary( stream, tcbKey->continuity );
	NiStreamSaveBinary( stream, tcbKey->bias );
}
