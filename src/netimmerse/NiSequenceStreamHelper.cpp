// NiSequenceStreamHelper.cpp -- the root of a keyframe sequence file

#include "NiRTLib.h"
#include "NiSequenceStreamHelper.h"
#include "NiAVObject.h"
#include "NiFloatController.h"
#include "NiFloatData.h"
#include "NiFloatKey.h"
#include "NiProperty.h"
#include "NiRollController.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x007b4770 _$E215
// FUNCTION: STBC 0x007b4790 _$E214
// FUNCTION: STBC 0x007b47a0 _$E17
// FUNCTION: STBC 0x007b47b0 _$E14
// FUNCTION: STBC 0x007b47c0 _$E16
// FUNCTION: STBC 0x007b47d0 _$E15
// FUNCTION: STBC 0x007b47e0 _$E23
// FUNCTION: STBC 0x007b47f0 _$E20
// FUNCTION: STBC 0x007b4800 _$E22
// FUNCTION: STBC 0x007b4810 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007b4820 _$E30
// FUNCTION: STBC 0x007b4830 _$E29
// FUNCTION: STBC 0x007b4840 _$E41
// FUNCTION: STBC 0x007b4850 _$E40
// FUNCTION: STBC 0x007b4860 _$E45
// FUNCTION: STBC 0x007b4870 _$E44
// FUNCTION: STBC 0x007b4880 _$E49
// FUNCTION: STBC 0x007b4890 _$E48
// FUNCTION: STBC 0x007b48e0 _$E53
// FUNCTION: STBC 0x007b48f0 _$E52

// FUNCTION: STBC 0x007b44a0 _$E208
// FUNCTION: STBC 0x007b44b0 _$E204
// FUNCTION: STBC 0x007b44d0 _$E207
// FUNCTION: STBC 0x007b44e0 _$E205
// DATA: STBC 0x9A0808 ?rtti@NiSequenceStreamHelper@@2VNiRTTI@@B
const NiRTTI NiSequenceStreamHelper::rtti( "NiSequenceStreamHelper", &NiObjectNET::rtti );

/*
================
NiSequenceStreamHelper::Register
================
*/
// FUNCTION: STBC 0x7B4500 ?Register@NiSequenceStreamHelper@@SAHXZ
NiImplementRegister( NiSequenceStreamHelper )

/*
================
NiSequenceStreamHelper::CreateObject
================
*/
// FUNCTION: STBC 0x7B4650
void NiSequenceStreamHelper::CreateObject( NiStream &stream, NiObject *&object ) {
	NiSequenceStreamHelper *helper = new NiSequenceStreamHelper;
	stream.SetLoadRecord( new NiSequenceStreamHelperLoadRecord );
	helper->LoadBinary( stream );
	object = helper;
}

/*
================
NiSequenceStreamHelper::LoadBinary
================
*/
// FUNCTION: STBC 0x7B4720
void NiSequenceStreamHelper::LoadBinary( NiStream &stream ) {
	NiObjectNET::LoadBinary( stream );
}

/*
================
NiSequenceStreamHelper::LinkObject
================
*/
// FUNCTION: STBC 0x7B4730
void NiSequenceStreamHelper::LinkObject( NiStream &stream ) {
	NiObjectNET::LinkObject( stream );
}

/*
================
NiSequenceStreamHelper::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7B4740
bool NiSequenceStreamHelper::RegisterStreamables( NiStream &stream ) {
	return NiObjectNET::RegisterStreamables( stream );
}

/*
================
NiSequenceStreamHelper::SaveBinary
================
*/
// FUNCTION: STBC 0x7B4750
void NiSequenceStreamHelper::SaveBinary( NiStream &stream ) {
	NiObjectNET::SaveBinary( stream );
}

/*
================
NiSequenceStreamHelper::IsEqual
================
*/
// FUNCTION: STBC 0x7B4760
bool NiSequenceStreamHelper::IsEqual( NiObject *object ) {
	return NiObjectNET::IsEqual( object );
}
