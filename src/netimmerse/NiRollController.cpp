// NiRollController.cpp -- roll animation

#include "NiRTLib.h"
#include "NiRollController.h"
#include "NiObjectNET.h"
#include "NiAutoNormalParticles.h"
#include "NiAutoNormalParticlesData.h"
#include "NiColorData.h"
#include "NiColorKey.h"
#include "NiDynamicEffect.h"
#include "NiForce.h"
#include "NiGeometry.h"
#include "NiGeometryData.h"
#include "NiGravity.h"
#include "NiLinColorKey.h"
#include "NiNode.h"
#include "NiParticleBomb.h"
#include "NiParticleSystemController.h"
#include "NiParticles.h"
#include "NiParticlesData.h"
#include "NiPlanarCollider.h"
#include "NiPosKey.h"
#include "NiSphericalCollider.h"
#include "NiTriBasedGeom.h"
#include "NiTriBasedGeomData.h"
#include "NiTriShape.h"
#include "NiTriShapeData.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007b39e0 _$E233
// FUNCTION: STBC 0x007b39f0 _$E232
// FUNCTION: STBC 0x007b3a00 _$E237
// FUNCTION: STBC 0x007b3a10 _$E236
// FUNCTION: STBC 0x007b3a20 _$E304
// FUNCTION: STBC 0x007b3a30 _$E303
// FUNCTION: STBC 0x007b3a40 _$E310
// FUNCTION: STBC 0x007b3a50 _$E309
// FUNCTION: STBC 0x007b3bf0 _$E262
// FUNCTION: STBC 0x007b3c00 _$E261
// FUNCTION: STBC 0x007b3c10 _$E273
// FUNCTION: STBC 0x007b3c20 _$E272
// FUNCTION: STBC 0x007b3c30 _$E300
// FUNCTION: STBC 0x007b3c40 _$E299
// FUNCTION: STBC 0x007b3c50 _$E282
// FUNCTION: STBC 0x007b3c60 _$E281
// FUNCTION: STBC 0x007b3c70 _$E296
// FUNCTION: STBC 0x007b3c80 _$E295
// FUNCTION: STBC 0x007b3c90 _$E265
// FUNCTION: STBC 0x007b3ca0 _$E264
// FUNCTION: STBC 0x007b42f0 _$E30
// FUNCTION: STBC 0x007b4300 _$E29
// FUNCTION: STBC 0x007b4310 _$E62
// FUNCTION: STBC 0x007b4320 _$E61
// FUNCTION: STBC 0x007b4330 _$E41
// FUNCTION: STBC 0x007b4340 _$E40
// FUNCTION: STBC 0x007b4350 _$E66
// FUNCTION: STBC 0x007b4360 _$E65
// FUNCTION: STBC 0x007b4370 _$E70
// FUNCTION: STBC 0x007b4380 _$E69
// FUNCTION: STBC 0x007b4390 _$E194
// FUNCTION: STBC 0x007b43a0 _$E193
// FUNCTION: STBC 0x007b43b0 _$E50
// FUNCTION: STBC 0x007b43c0 _$E49
// FUNCTION: STBC 0x007b43d0 _$E54
// FUNCTION: STBC 0x007b43e0 _$E53
// FUNCTION: STBC 0x007b43f0 _$E58
// FUNCTION: STBC 0x007b4400 _$E57
// FUNCTION: STBC 0x007b4450 _$E198
// FUNCTION: STBC 0x007b4460 _$E197

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x007b4240 _$E323
// FUNCTION: STBC 0x007b4260 _$E322
// FUNCTION: STBC 0x007b4270 _$E17
// FUNCTION: STBC 0x007b4280 _$E14
// FUNCTION: STBC 0x007b4290 _$E16
// FUNCTION: STBC 0x007b42a0 _$E15
// FUNCTION: STBC 0x007b42b0 _$E23
// FUNCTION: STBC 0x007b42c0 _$E20
// FUNCTION: STBC 0x007b42d0 _$E22
// FUNCTION: STBC 0x007b42e0 _$E21

// FUNCTION: STBC 0x007b3cb0 _$E316
// FUNCTION: STBC 0x007b3cc0 _$E312
// FUNCTION: STBC 0x007b3ce0 _$E315
// FUNCTION: STBC 0x007b3cf0 _$E313
// DATA: STBC 0x9A07D8 ?rtti@NiRollController@@2VNiRTTI@@B
const NiRTTI NiRollController::rtti( "NiRollController", &NiFloatController::rtti );

/*
================
NiRollController::Register
================
*/
// FUNCTION: STBC 0x7B3D10 ?Register@NiRollController@@SAHXZ
NiImplementRegister( NiRollController )

/*
================
NiRollController::FindController
================
*/
// FUNCTION: STBC 0x7B3E60
NiRollController *NiRollController::FindController( NiObjectNET *target ) {
	for ( NiTimeController *controller = target->GetControllers(); controller; controller = controller->GetNext() ) {
		NiRollController *rollController = NiDynamicCast( NiRollController, controller );
		if ( rollController ) {
			return rollController;
		}
	}
	return 0;
}

/*
================
NiRollController::Update

Rolls the target's local rotation by the keys' angle at the scaled time.
================
*/
// FUNCTION: STBC 0x7B3EB0
void NiRollController::Update( float time ) {
	if ( !target || DontDoUpdate( time ) ) {
		return;
	}

	unsigned int keysCount;	// (name invented; fits the /Od frame order)
	NiFloatKey::KeyType type;	// (name invented; fits the /Od frame order)
	NiFloatKey **anim = GetAnim( keysCount, type );	// (name invented; fits the /Od frame order)
	float sTime = ComputeScaledTime( time );	// (name invented; fits the /Od frame order)
	float rollAngle = NiFloatKey::GenInterp( sTime, anim, type, keysCount, lastIndex );	// (name invented; fits the /Od frame order)

	NiMatrix3 rotate = ( (NiAVObject *)target )->GetRotate();
	NiMatrix3 rollMat;	// (name invented; fits the /Od frame order)
	rollMat.MakeZRotation( -rollAngle );
	rotate = rotate * rollMat;
	( (NiAVObject *)target )->SetRotate( rotate );
}

/*
================
NiRollController::CreateClone
================
*/
// FUNCTION: STBC 0x7B3F90
NiObject *NiRollController::CreateClone() {
	NiRollController *clone = new NiRollController;
	CopyMembers( clone );
	return clone;
}

/*
================
NiRollController::CopyMembers
================
*/
// FUNCTION: STBC 0x7B4010
void NiRollController::CopyMembers( NiRollController *dest ) {
	NiFloatController::CopyMembers( dest );
}

/*
================
NiRollController::CreateObject
================
*/
// FUNCTION: STBC 0x7B4020
void NiRollController::CreateObject( NiStream &stream, NiObject *&object ) {
	NiRollController *controller = new NiRollController;
	stream.SetLoadRecord( new NiRollControllerLoadRecord );
	controller->LoadBinary( stream );
	object = controller;
}

/*
================
NiRollController::LoadBinary
================
*/
// FUNCTION: STBC 0x7B40F0
void NiRollController::LoadBinary( NiStream &stream ) {
	NiFloatController::LoadBinary( stream );
}

/*
================
NiRollController::LinkObject
================
*/
// FUNCTION: STBC 0x7B4100
void NiRollController::LinkObject( NiStream &stream ) {
	NiFloatController::LinkObject( stream );
}

/*
================
NiRollController::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7B4110
bool NiRollController::RegisterStreamables( NiStream &stream ) {
	return NiFloatController::RegisterStreamables( stream );
}

/*
================
NiRollController::SaveBinary
================
*/
// FUNCTION: STBC 0x7B4120
void NiRollController::SaveBinary( NiStream &stream ) {
	NiFloatController::SaveBinary( stream );
}

/*
================
NiRollController::IsEqual
================
*/
// FUNCTION: STBC 0x7B4130
bool NiRollController::IsEqual( NiObject *object ) {
	return NiFloatController::IsEqual( object );
}

/*
================
NiRollController::GetViewerStrings

Skips NiFloatController's strings: NiTimeController's come first, then
the class banner.
================
*/
// FUNCTION: STBC 0x7B4140
void NiRollController::GetViewerStrings( NiTArray<char *> *strings ) {
	NiTimeController::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
}
