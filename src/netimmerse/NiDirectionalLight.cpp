// NiDirectionalLight.cpp -- lights from one direction

#include "NiRTLib.h"
#include <assert.h>
#include "NiDirectionalLight.h"
#include "NiCloneExtraData.h"
#include "NiStream.h"
#include "renderer/NiRender.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007fa2d0 _$E213
// FUNCTION: STBC 0x007fa270 _$E17
// FUNCTION: STBC 0x007fa290 _$E15
// FUNCTION: STBC 0x007fa2a0 _$E23
// FUNCTION: STBC 0x007fa2c0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007fa2f0 _$E34
// FUNCTION: STBC 0x007fa300 _$E45
// FUNCTION: STBC 0x007fa310 _$E49
// FUNCTION: STBC 0x007fa320 _$E53
// FUNCTION: STBC 0x007fa330 _$E57
// FUNCTION: STBC 0x007fa340 _$E177
// FUNCTION: STBC 0x007fa350 _$E182
// FUNCTION: STBC 0x007fa360 _$E186
// FUNCTION: STBC 0x007fa3a0 _$E190
// FUNCTION: STBC 0x007fa3c0 _$E195
// FUNCTION: STBC 0x007fa3d0 _$E119
// FUNCTION: STBC 0x007fa3e0 _$E132
// FUNCTION: STBC 0x007fa3f0 _$E200

// FUNCTION: STBC 0x007f9be0 _$E206
// FUNCTION: STBC 0x007f9c00 _$E203
// DATA: STBC 0x9A1F18 ?rtti@NiDirectionalLight@@2VNiRTTI@@B
const NiRTTI NiDirectionalLight::rtti( "NiDirectionalLight", &NiLight::rtti );

/*
================
NiDirectionalLight::Register
================
*/
// FUNCTION: STBC 0x7F9C20 ?Register@NiDirectionalLight@@SAHXZ
NiImplementRegister( NiDirectionalLight )

/*
================
NiDirectionalLight::NiDirectionalLight
================
*/
// FUNCTION: STBC 0x7F9D70
NiDirectionalLight::NiDirectionalLight() {
	worldDirection = NiPoint3( 1.0f, 0.0f, 0.0f );
}

/*
================
NiDirectionalLight::~NiDirectionalLight

Tells the renderers to drop the light first.
================
*/
// FUNCTION: STBC 0x7F9DA0
NiDirectionalLight::~NiDirectionalLight() {
	NiRender::RemoveEffect( this );
}

/*
================
NiDirectionalLight::UpdateWorldData

The light shines down its local x axis.
================
*/
// FUNCTION: STBC 0x7F9E00
void NiDirectionalLight::UpdateWorldData() {
	NiAVObject::UpdateWorldData();
	worldDirection = worldRotate * NiPoint3::UNIT_X;
	IncRevisionID();
}

/*
================
NiDirectionalLight::CreateClone

Marks the original with its clone unless it already carries the mark.
================
*/
// FUNCTION: STBC 0x7F9E50
NiObject *NiDirectionalLight::CreateClone() {
	NiDirectionalLight *result = new NiDirectionalLight;	// (name invented; fits the /Od frame order)
	assert( result );
	CopyMembers( result );
	NiExtraData *extraData = GetExtraData();
	if ( !extraData || !NiIsExactKindOf( NiCloneExtraData, extraData ) ) {
		extraData = new NiCloneExtraData( result );
		assert( extraData );
		AddExtraData( extraData );
	}
	return result;
}

/*
================
NiDirectionalLight::CopyMembers
================
*/
// FUNCTION: STBC 0x7F9F70
void NiDirectionalLight::CopyMembers( NiDirectionalLight *dest ) {
	NiLight::CopyMembers( dest );
	dest->worldDirection = worldDirection;
}

/*
================
NiDirectionalLight::CreateObject

Creates the light and loads it.
================
*/
// FUNCTION: STBC 0x7F9FB0
void NiDirectionalLight::CreateObject( NiStream &stream, NiObject *&object ) {
	NiDirectionalLight *light = new NiDirectionalLight;
	stream.SetLoadRecord( new NiDirectionalLightLoadRecord );
	light->LoadBinary( stream );
	object = light;
}

/*
================
NiDirectionalLight::LoadBinary
================
*/
// FUNCTION: STBC 0x7FA120
void NiDirectionalLight::LoadBinary( NiStream &stream ) {
	NiLight::LoadBinary( stream );
}

/*
================
NiDirectionalLight::LinkObject
================
*/
// FUNCTION: STBC 0x7FA130
void NiDirectionalLight::LinkObject( NiStream &stream ) {
	NiLight::LinkObject( stream );
}

/*
================
NiDirectionalLight::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7FA140
bool NiDirectionalLight::RegisterStreamables( NiStream &stream ) {
	return NiLight::RegisterStreamables( stream );
}

/*
================
NiDirectionalLight::SaveBinary
================
*/
// FUNCTION: STBC 0x7FA150
void NiDirectionalLight::SaveBinary( NiStream &stream ) {
	NiLight::SaveBinary( stream );
}

/*
================
NiDirectionalLight::IsEqual
================
*/
// FUNCTION: STBC 0x7FA160
bool NiDirectionalLight::IsEqual( NiObject *object ) {
	return NiLight::IsEqual( object );
}

/*
================
NiDirectionalLight::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7FA170
void NiDirectionalLight::GetViewerStrings( NiTArray<char *> *strings ) {
	NiLight::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
}
