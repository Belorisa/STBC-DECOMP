// NiAccumulator.cpp -- the base of the renderer's sorters

#include "NiRTLib.h"
#include <assert.h>
#include "NiAccumulator.h"
#include "NiStream.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007d9010 _$E30
// FUNCTION: STBC 0x007d9020 _$E34

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007d8f90 _$E54
// FUNCTION: STBC 0x007d8fb0 _$E17
// FUNCTION: STBC 0x007d8fd0 _$E15
// FUNCTION: STBC 0x007d8fe0 _$E23
// FUNCTION: STBC 0x007d9000 _$E21

// FUNCTION: STBC 0x007d8d30 _$E47
// FUNCTION: STBC 0x007d8d50 _$E44
// DATA: STBC 0x9A1490 ?rtti@NiAccumulator@@2VNiRTTI@@B
const NiRTTI NiAccumulator::rtti( "NiAccumulator", &NiObject::rtti );

/*
================
NiAccumulator::Register
================
*/
// FUNCTION: STBC 0x7D8D70 ?Register@NiAccumulator@@SAHXZ
NiImplementRegister( NiAccumulator )

/*
================
NiAccumulator::NiAccumulator
================
*/
// FUNCTION: STBC 0x7D8EC0
NiAccumulator::NiAccumulator() {
	camera = 0;
}

/*
================
NiAccumulator::StartAccumulating

Fails while another camera is accumulating.
================
*/
// FUNCTION: STBC 0x7D8EE0
bool NiAccumulator::StartAccumulating( NiCamera *camera_ ) {
	assert( camera_ );
	if ( camera ) {
		return false;
	}
	camera = camera_;
	return true;
}

/*
================
NiAccumulator::FinishAccumulating
================
*/
// FUNCTION: STBC 0x7D8F00
void NiAccumulator::FinishAccumulating() {
	camera = 0;
}

/*
================
NiAccumulator::CreateClone

The sorters are not cloned.
================
*/
// FUNCTION: STBC 0x7D8F10
NiObject *NiAccumulator::CreateClone() {
	assert( false );
	return 0;
}

/*
================
NiAccumulator::CopyMembers

The camera is not copied.
================
*/
// FUNCTION: STBC 0x7D8F20
void NiAccumulator::CopyMembers( NiAccumulator *dest ) {
	NiObject::CopyMembers( dest );
}

/*
================
NiAccumulator::CreateObject

NiAccumulator is abstract: nothing to create.
================
*/
// FUNCTION: STBC 0x7D8F30
void NiAccumulator::CreateObject( NiStream &stream, NiObject *&object ) {
}

/*
================
NiAccumulator::LoadBinary
================
*/
// FUNCTION: STBC 0x7D8F40
void NiAccumulator::LoadBinary( NiStream &stream ) {
	NiObject::LoadBinary( stream );
}

/*
================
NiAccumulator::LinkObject
================
*/
// FUNCTION: STBC 0x7D8F50
void NiAccumulator::LinkObject( NiStream &stream ) {
	NiObject::LinkObject( stream );
}

/*
================
NiAccumulator::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7D8F60
bool NiAccumulator::RegisterStreamables( NiStream &stream ) {
	return NiObject::RegisterStreamables( stream );
}

/*
================
NiAccumulator::SaveBinary
================
*/
// FUNCTION: STBC 0x7D8F70
void NiAccumulator::SaveBinary( NiStream &stream ) {
	NiObject::SaveBinary( stream );
}

/*
================
NiAccumulator::IsEqual
================
*/
// FUNCTION: STBC 0x7D8F80
bool NiAccumulator::IsEqual( NiObject *object ) {
	return NiObject::IsEqual( object );
}
