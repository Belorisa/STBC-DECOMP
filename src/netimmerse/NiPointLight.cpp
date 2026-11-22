// NiPointLight.cpp -- lights from one point

#include "NiRTLib.h"
#include <assert.h>
#include "NiCloneExtraData.h"
#include "NiPointLight.h"
#include "NiStream.h"
#include "renderer/NiRender.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00803fb0 _$E213
// FUNCTION: STBC 0x00803f50 _$E17
// FUNCTION: STBC 0x00803f70 _$E15
// FUNCTION: STBC 0x00803f80 _$E23
// FUNCTION: STBC 0x00803fa0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00803fd0 _$E30
// FUNCTION: STBC 0x00803fe0 _$E41
// FUNCTION: STBC 0x00803ff0 _$E50
// FUNCTION: STBC 0x00804000 _$E54
// FUNCTION: STBC 0x00804010 _$E58
// FUNCTION: STBC 0x00804020 _$E178
// FUNCTION: STBC 0x00804030 _$E183
// FUNCTION: STBC 0x00804040 _$E187
// FUNCTION: STBC 0x00804050 _$E191
// FUNCTION: STBC 0x00804060 _$E195
// FUNCTION: STBC 0x00804070 _$E120
// FUNCTION: STBC 0x00804080 _$E133
// FUNCTION: STBC 0x00804090 _$E200

// FUNCTION: STBC 0x00803760 _$E206
// FUNCTION: STBC 0x00803780 _$E203
// DATA: STBC 0x9A22A0 ?rtti@NiPointLight@@2VNiRTTI@@B
const NiRTTI NiPointLight::rtti( "NiPointLight", &NiLight::rtti );

/*
================
NiPointLight::Register
================
*/
// FUNCTION: STBC 0x8037A0 ?Register@NiPointLight@@SAHXZ
NiImplementRegister( NiPointLight )

/*
================
NiPointLight::~NiPointLight

Tells the renderers to drop the light first.
================
*/
// FUNCTION: STBC 0x8038F0
NiPointLight::~NiPointLight() {
	NiRender::RemoveEffect( this );
}

/*
================
NiPointLight::UpdateWorldData
================
*/
// FUNCTION: STBC 0x803950
void NiPointLight::UpdateWorldData() {
	NiAVObject::UpdateWorldData();
	IncRevisionID();
}

/*
================
NiPointLight::CreateClone

Marks the original with its clone unless it already carries the mark.
================
*/
// FUNCTION: STBC 0x803970
NiObject *NiPointLight::CreateClone() {
	NiPointLight *result = new NiPointLight;	// (name invented; fits the /Od frame order)
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
NiPointLight::CopyMembers
================
*/
// FUNCTION: STBC 0x803A90
void NiPointLight::CopyMembers( NiPointLight *dest ) {
	NiLight::CopyMembers( dest );
	dest->constantAttenuation = constantAttenuation;
	dest->linearAttenuation = linearAttenuation;
	dest->quadraticAttenuation = quadraticAttenuation;
}

/*
================
NiPointLight::CreateObject

Creates the light and loads it.
================
*/
// FUNCTION: STBC 0x803AD0
void NiPointLight::CreateObject( NiStream &stream, NiObject *&object ) {
	NiPointLight *light = new NiPointLight;
	stream.SetLoadRecord( new NiPointLightLoadRecord );
	light->LoadBinary( stream );
	object = light;
}

/*
================
NiPointLight::LoadBinary
================
*/
// FUNCTION: STBC 0x803C40
void NiPointLight::LoadBinary( NiStream &stream ) {
	NiLight::LoadBinary( stream );
	NiStreamLoadBinary( stream, constantAttenuation );
	NiStreamLoadBinary( stream, linearAttenuation );
	NiStreamLoadBinary( stream, quadraticAttenuation );
}

/*
================
NiPointLight::LinkObject
================
*/
// FUNCTION: STBC 0x803C90
void NiPointLight::LinkObject( NiStream &stream ) {
	NiLight::LinkObject( stream );
}

/*
================
NiPointLight::RegisterStreamables
================
*/
// FUNCTION: STBC 0x803CA0
bool NiPointLight::RegisterStreamables( NiStream &stream ) {
	return NiLight::RegisterStreamables( stream );
}

/*
================
NiPointLight::SaveBinary
================
*/
// FUNCTION: STBC 0x803CB0
void NiPointLight::SaveBinary( NiStream &stream ) {
	NiLight::SaveBinary( stream );
	NiStreamSaveBinary( stream, constantAttenuation );
	NiStreamSaveBinary( stream, linearAttenuation );
	NiStreamSaveBinary( stream, quadraticAttenuation );
}

/*
================
NiPointLight::IsEqual
================
*/
// FUNCTION: STBC 0x803D00
bool NiPointLight::IsEqual( NiObject *object ) {
	if ( !NiLight::IsEqual( object ) ) {
		return false;
	}
	NiPointLight *light = (NiPointLight *)object;
	if ( constantAttenuation != light->constantAttenuation
		|| linearAttenuation != light->linearAttenuation
		|| quadraticAttenuation != light->quadraticAttenuation ) {
		return false;
	}
	return true;
}

/*
================
NiPointLight::GetViewerStrings
================
*/
// FUNCTION: STBC 0x803D60
void NiPointLight::GetViewerStrings( NiTArray<char *> *strings ) {
	NiLight::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_fAtten0", constantAttenuation ) );
	strings->Add( NiGetViewerString( "m_fAtten1", linearAttenuation ) );
	strings->Add( NiGetViewerString( "m_fAtten2", quadraticAttenuation ) );
}
