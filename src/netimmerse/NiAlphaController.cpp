// NiAlphaController.cpp -- material alpha animation

#include "NiRTLib.h"
#include <assert.h>
#include "NiAlphaController.h"
#include "NiObjectNET.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x00795d00 _$E97
// FUNCTION: STBC 0x00795d20 _$E96
// FUNCTION: STBC 0x00795d30 _$E17
// FUNCTION: STBC 0x00795d40 _$E14
// FUNCTION: STBC 0x00795d50 _$E16
// FUNCTION: STBC 0x00795d60 _$E15
// FUNCTION: STBC 0x00795d70 _$E23
// FUNCTION: STBC 0x00795d80 _$E20
// FUNCTION: STBC 0x00795d90 _$E22
// FUNCTION: STBC 0x00795da0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00795db0 _$E30
// FUNCTION: STBC 0x00795dc0 _$E29
// FUNCTION: STBC 0x00795dd0 _$E66
// FUNCTION: STBC 0x00795de0 _$E65
// FUNCTION: STBC 0x00795df0 _$E41
// FUNCTION: STBC 0x00795e00 _$E40
// FUNCTION: STBC 0x00795e10 _$E70
// FUNCTION: STBC 0x00795e20 _$E69
// FUNCTION: STBC 0x00795e30 _$E74
// FUNCTION: STBC 0x00795e40 _$E73
// FUNCTION: STBC 0x00795e50 _$E80
// FUNCTION: STBC 0x00795e60 _$E79
// FUNCTION: STBC 0x00795e70 _$E50
// FUNCTION: STBC 0x00795e80 _$E49
// FUNCTION: STBC 0x00795e90 _$E54
// FUNCTION: STBC 0x00795ea0 _$E53
// FUNCTION: STBC 0x00795eb0 _$E58
// FUNCTION: STBC 0x00795ec0 _$E57
// FUNCTION: STBC 0x00795ed0 _$E84
// FUNCTION: STBC 0x00795ee0 _$E83

// FUNCTION: STBC 0x007957d0 _$E90
// FUNCTION: STBC 0x007957e0 _$E86
// FUNCTION: STBC 0x00795800 _$E89
// FUNCTION: STBC 0x00795810 _$E87
// DATA: STBC 0x9A0138 ?rtti@NiAlphaController@@2VNiRTTI@@B
const NiRTTI NiAlphaController::rtti( "NiAlphaController", &NiFloatController::rtti );

/*
================
NiAlphaController::Register
================
*/
// FUNCTION: STBC 0x795830 ?Register@NiAlphaController@@SAHXZ
NiImplementRegister( NiAlphaController )

/*
================
NiAlphaController::FindController
================
*/
// FUNCTION: STBC 0x795980
NiAlphaController *NiAlphaController::FindController( NiObjectNET *target ) {
	for ( NiTimeController *controller = target->GetControllers(); controller; controller = controller->GetNext() ) {
		NiAlphaController *alphaController = NiDynamicCast( NiAlphaController, controller );
		if ( alphaController ) {
			return alphaController;
		}
	}
	return 0;
}

/*
================
NiAlphaController::Update

Sets the material's alpha to the keys' value at the scaled time.
================
*/
// FUNCTION: STBC 0x7959D0
void NiAlphaController::Update( float time ) {
	if ( !target || DontDoUpdate( time ) ) {
		return;
	}

	unsigned int num;	// (name invented; fits the /Od frame order)
	NiFloatKey::KeyType type;	// (name invented; fits the /Od frame order)
	NiFloatKey **anim = GetAnim( num, type );	// (name invented; fits the /Od frame order)
	float sTime = ComputeScaledTime( time );	// (name invented; fits the /Od frame order)
	float alphaValue = NiFloatKey::GenInterp( sTime, anim, type, num, lastIndex );	// (name invented; fits the /Od frame order)
	assert( NiIsKindOf( NiMaterialProperty, target ) );
	( (NiMaterialProperty *)target )->SetAlpha( alphaValue );
}

/*
================
NiAlphaController::CreateClone
================
*/
// FUNCTION: STBC 0x795A50
NiObject *NiAlphaController::CreateClone() {
	NiAlphaController *clone = new NiAlphaController;
	CopyMembers( clone );
	return clone;
}

/*
================
NiAlphaController::CopyMembers
================
*/
// FUNCTION: STBC 0x795AD0
void NiAlphaController::CopyMembers( NiAlphaController *dest ) {
	NiFloatController::CopyMembers( dest );
}

/*
================
NiAlphaController::CreateObject
================
*/
// FUNCTION: STBC 0x795AE0
void NiAlphaController::CreateObject( NiStream &stream, NiObject *&object ) {
	NiAlphaController *controller = new NiAlphaController;
	stream.SetLoadRecord( new NiAlphaControllerLoadRecord );
	controller->LoadBinary( stream );
	object = controller;
}

/*
================
NiAlphaController::LoadBinary
================
*/
// FUNCTION: STBC 0x795BB0
void NiAlphaController::LoadBinary( NiStream &stream ) {
	NiFloatController::LoadBinary( stream );
}

/*
================
NiAlphaController::LinkObject
================
*/
// FUNCTION: STBC 0x795BC0
void NiAlphaController::LinkObject( NiStream &stream ) {
	NiFloatController::LinkObject( stream );
}

/*
================
NiAlphaController::RegisterStreamables
================
*/
// FUNCTION: STBC 0x795BD0
bool NiAlphaController::RegisterStreamables( NiStream &stream ) {
	return NiFloatController::RegisterStreamables( stream );
}

/*
================
NiAlphaController::SaveBinary
================
*/
// FUNCTION: STBC 0x795BE0
void NiAlphaController::SaveBinary( NiStream &stream ) {
	NiFloatController::SaveBinary( stream );
}

/*
================
NiAlphaController::IsEqual
================
*/
// FUNCTION: STBC 0x795BF0
bool NiAlphaController::IsEqual( NiObject *object ) {
	return NiFloatController::IsEqual( object );
}

/*
================
NiAlphaController::GetViewerStrings

Skips NiFloatController's strings: NiTimeController's come first, then
the class banner.
================
*/
// FUNCTION: STBC 0x795C00
void NiAlphaController::GetViewerStrings( NiTArray<char *> *strings ) {
	NiTimeController::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
}
