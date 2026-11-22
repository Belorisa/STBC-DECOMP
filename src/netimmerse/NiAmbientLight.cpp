// NiAmbientLight.cpp -- lights without direction or position

#include "NiRTLib.h"
#include <assert.h>
#include "NiAmbientLight.h"
#include "NiCloneExtraData.h"
#include "NiStream.h"
#include "renderer/NiRender.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007f4430 _$E213
// FUNCTION: STBC 0x007f43d0 _$E17
// FUNCTION: STBC 0x007f43f0 _$E15
// FUNCTION: STBC 0x007f4400 _$E23
// FUNCTION: STBC 0x007f4420 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007f4450 _$E34
// FUNCTION: STBC 0x007f4460 _$E45
// FUNCTION: STBC 0x007f4470 _$E49
// FUNCTION: STBC 0x007f4480 _$E53
// FUNCTION: STBC 0x007f4490 _$E57
// FUNCTION: STBC 0x007f44a0 _$E177
// FUNCTION: STBC 0x007f44b0 _$E182
// FUNCTION: STBC 0x007f44c0 _$E186
// FUNCTION: STBC 0x007f4500 _$E190
// FUNCTION: STBC 0x007f4520 _$E195
// FUNCTION: STBC 0x007f4530 _$E119
// FUNCTION: STBC 0x007f4540 _$E132
// FUNCTION: STBC 0x007f4550 _$E200

// FUNCTION: STBC 0x007f3e30 _$E206
// FUNCTION: STBC 0x007f3e50 _$E203
// DATA: STBC 0x9A1C30 ?rtti@NiAmbientLight@@2VNiRTTI@@B
const NiRTTI NiAmbientLight::rtti( "NiAmbientLight", &NiLight::rtti );

/*
================
NiAmbientLight::Register
================
*/
// FUNCTION: STBC 0x7F3E70 ?Register@NiAmbientLight@@SAHXZ
NiImplementRegister( NiAmbientLight )

/*
================
NiAmbientLight::NiAmbientLight
================
*/
// FUNCTION: STBC 0x7F3FC0
NiAmbientLight::NiAmbientLight() {
}

/*
================
NiAmbientLight::~NiAmbientLight

Tells the renderers to drop the light first.
================
*/
// FUNCTION: STBC 0x7F3FE0
NiAmbientLight::~NiAmbientLight() {
	NiRender::RemoveEffect( this );
}

/*
================
NiAmbientLight::CreateClone

Marks the original with its clone unless it already carries the mark.
================
*/
// FUNCTION: STBC 0x7F4040
NiObject *NiAmbientLight::CreateClone() {
	NiAmbientLight *result = new NiAmbientLight;	// (name invented; fits the /Od frame order)
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
NiAmbientLight::CopyMembers
================
*/
// FUNCTION: STBC 0x7F4120
void NiAmbientLight::CopyMembers( NiAmbientLight *dest ) {
	NiLight::CopyMembers( dest );
}

/*
================
NiAmbientLight::CreateObject

Creates the light and loads it.
================
*/
// FUNCTION: STBC 0x7F4130
void NiAmbientLight::CreateObject( NiStream &stream, NiObject *&object ) {
	NiAmbientLight *light = new NiAmbientLight;
	stream.SetLoadRecord( new NiAmbientLightLoadRecord );
	light->LoadBinary( stream );
	object = light;
}

/*
================
NiAmbientLight::LoadBinary
================
*/
// FUNCTION: STBC 0x7F4280
void NiAmbientLight::LoadBinary( NiStream &stream ) {
	NiLight::LoadBinary( stream );
}

/*
================
NiAmbientLight::LinkObject
================
*/
// FUNCTION: STBC 0x7F4290
void NiAmbientLight::LinkObject( NiStream &stream ) {
	NiLight::LinkObject( stream );
}

/*
================
NiAmbientLight::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7F42A0
bool NiAmbientLight::RegisterStreamables( NiStream &stream ) {
	return NiLight::RegisterStreamables( stream );
}

/*
================
NiAmbientLight::SaveBinary
================
*/
// FUNCTION: STBC 0x7F42B0
void NiAmbientLight::SaveBinary( NiStream &stream ) {
	NiLight::SaveBinary( stream );
}

/*
================
NiAmbientLight::IsEqual
================
*/
// FUNCTION: STBC 0x7F42C0
bool NiAmbientLight::IsEqual( NiObject *object ) {
	return NiLight::IsEqual( object );
}

/*
================
NiAmbientLight::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7F42D0
void NiAmbientLight::GetViewerStrings( NiTArray<char *> *strings ) {
	NiLight::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
}
