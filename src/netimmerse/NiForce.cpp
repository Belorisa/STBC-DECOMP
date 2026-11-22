// NiForce.cpp -- the base of the particle forces and colliders

#include "NiRTLib.h"
#include <assert.h>
#include "NiForce.h"
#include "NiStream.h"
#include "NiFloatKey.h"
#include "NiKeyType4FloatKey.h"
#include "NiKeyType6FloatKey.h"
#include "NiTCBFloatKey.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0079e620 _$E30
// FUNCTION: STBC 0x0079e630 _$E29
// FUNCTION: STBC 0x0079e640 _$E34
// FUNCTION: STBC 0x0079e650 _$E33

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x0079e570 _$E72
// FUNCTION: STBC 0x0079e590 _$E71
// FUNCTION: STBC 0x0079e5a0 _$E17
// FUNCTION: STBC 0x0079e5b0 _$E14
// FUNCTION: STBC 0x0079e5c0 _$E16
// FUNCTION: STBC 0x0079e5d0 _$E15
// FUNCTION: STBC 0x0079e5e0 _$E23
// FUNCTION: STBC 0x0079e5f0 _$E20
// FUNCTION: STBC 0x0079e600 _$E22
// FUNCTION: STBC 0x0079e610 _$E21

// FUNCTION: STBC 0x0079e310 _$E65
// FUNCTION: STBC 0x0079e320 _$E61
// FUNCTION: STBC 0x0079e340 _$E64
// FUNCTION: STBC 0x0079e350 _$E62
// DATA: STBC 0x9A0318 ?rtti@NiForce@@2VNiRTTI@@B
const NiRTTI NiForce::rtti( "NiForce", &NiObject::rtti );

/*
================
NiForce::Register
================
*/
// FUNCTION: STBC 0x79E370 ?Register@NiForce@@SAHXZ
NiImplementRegister( NiForce )

/*
================
NiForce::GetChainLength

Callers test the chain for NULL first.
================
*/
// FUNCTION: STBC 0x79E4C0
unsigned int NiForce::GetChainLength() {
	unsigned int length = 0;
	for ( NiForce *force = this; force; force = force->next ) {
		length++;
	}
	return length;
}

/*
================
NiForce::CreateClone

NiForce is abstract: nothing to clone.
================
*/
// FUNCTION: STBC 0x79E4D0
NiObject *NiForce::CreateClone() {
	assert( false );
	return 0;
}

/*
================
NiForce::CopyMembers

Clones the rest of the chain behind the clone.
================
*/
// FUNCTION: STBC 0x79E4E0
void NiForce::CopyMembers( NiForce *dest ) {
	NiObject::CopyMembers( dest );
	if ( next ) {
		dest->next = (NiForce *)next->CreateClone();
	} else {
		dest->next = 0;
	}
}

/*
================
NiForce::CreateObject

NiForce is abstract: nothing to create.
================
*/
// FUNCTION: STBC 0x79E510
void NiForce::CreateObject( NiStream &stream, NiObject *&object ) {
}

/*
================
NiForce::LoadBinary
================
*/
// FUNCTION: STBC 0x79E520
void NiForce::LoadBinary( NiStream &stream ) {
	NiObject::LoadBinary( stream );
}

/*
================
NiForce::LinkObject
================
*/
// FUNCTION: STBC 0x79E530
void NiForce::LinkObject( NiStream &stream ) {
	NiObject::LinkObject( stream );
}

/*
================
NiForce::RegisterStreamables
================
*/
// FUNCTION: STBC 0x79E540
bool NiForce::RegisterStreamables( NiStream &stream ) {
	return NiObject::RegisterStreamables( stream );
}

/*
================
NiForce::SaveBinary
================
*/
// FUNCTION: STBC 0x79E550
void NiForce::SaveBinary( NiStream &stream ) {
	NiObject::SaveBinary( stream );
}

/*
================
NiForce::IsEqual
================
*/
// FUNCTION: STBC 0x79E560
bool NiForce::IsEqual( NiObject *object ) {
	return NiObject::IsEqual( object );
}
