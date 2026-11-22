// NiSpotLight.cpp -- point lights limited to a cone

#include "NiRTLib.h"
#include <assert.h>
#include "NiCloneExtraData.h"
#include "NiSpotLight.h"
#include "NiStream.h"
#include "renderer/NiRender.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x008073f0 _$E217
// FUNCTION: STBC 0x00807390 _$E17
// FUNCTION: STBC 0x008073b0 _$E15
// FUNCTION: STBC 0x008073c0 _$E23
// FUNCTION: STBC 0x008073e0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00807410 _$E30
// FUNCTION: STBC 0x00807420 _$E41
// FUNCTION: STBC 0x00807430 _$E50
// FUNCTION: STBC 0x00807440 _$E54
// FUNCTION: STBC 0x00807450 _$E58
// FUNCTION: STBC 0x00807460 _$E178
// FUNCTION: STBC 0x00807470 _$E183
// FUNCTION: STBC 0x00807480 _$E187
// FUNCTION: STBC 0x00807490 _$E191
// FUNCTION: STBC 0x008074d0 _$E195
// FUNCTION: STBC 0x008074f0 _$E199
// FUNCTION: STBC 0x00807500 _$E120
// FUNCTION: STBC 0x00807510 _$E133
// FUNCTION: STBC 0x00807520 _$E204

// FUNCTION: STBC 0x00806ae0 _$E210
// FUNCTION: STBC 0x00806b00 _$E207
// DATA: STBC 0x9A23F8 ?rtti@NiSpotLight@@2VNiRTTI@@B
const NiRTTI NiSpotLight::rtti( "NiSpotLight", &NiPointLight::rtti );

/*
================
NiSpotLight::Register
================
*/
// FUNCTION: STBC 0x806B20 ?Register@NiSpotLight@@SAHXZ
NiImplementRegister( NiSpotLight )

/*
================
NiSpotLight::NiSpotLight
================
*/
// FUNCTION: STBC 0x806C70
NiSpotLight::NiSpotLight() {
	worldDirection = NiPoint3( 1.0f, 0.0f, 0.0f );
	spotExponent = 0.0f;
	spotAngle = 0.0f;
}

/*
================
NiSpotLight::~NiSpotLight

Tells the renderers to drop the light first.
================
*/
// FUNCTION: STBC 0x806CC0
NiSpotLight::~NiSpotLight() {
	NiRender::RemoveEffect( this );
}

/*
================
NiSpotLight::UpdateWorldData

The light shines down its local x axis.
================
*/
// FUNCTION: STBC 0x806D20
void NiSpotLight::UpdateWorldData() {
	NiPointLight::UpdateWorldData();
	worldDirection = worldRotate * NiPoint3::UNIT_X;
	IncRevisionID();
}

/*
================
NiSpotLight::CreateClone

Marks the original with its clone unless it already carries the mark.
================
*/
// FUNCTION: STBC 0x806D70
NiObject *NiSpotLight::CreateClone() {
	NiSpotLight *result = new NiSpotLight;	// (name invented; fits the /Od frame order)
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
NiSpotLight::CopyMembers
================
*/
// FUNCTION: STBC 0x806EC0
void NiSpotLight::CopyMembers( NiSpotLight *dest ) {
	NiPointLight::CopyMembers( dest );
	dest->worldDirection = worldDirection;
	dest->spotAngle = spotAngle;
	dest->spotExponent = spotExponent;
}

/*
================
NiSpotLight::CreateObject

Creates the light and loads it.
================
*/
// FUNCTION: STBC 0x806F10 ?CreateObject@NiSpotLight@@SAXAAVNiStream@@AAPAVNiObject@@@Z
void NiSpotLight::CreateObject( NiStream &stream, NiObject *&object ) {
	NiSpotLight *light = new NiSpotLight;
	stream.SetLoadRecord( new NiSpotLightLoadRecord );
	light->LoadBinary( stream );
	object = light;
}

/*
================
NiSpotLight::LoadBinary
================
*/
// FUNCTION: STBC 0x807070
void NiSpotLight::LoadBinary( NiStream &stream ) {
	NiPointLight::LoadBinary( stream );
	NiStreamLoadBinary( stream, spotAngle );
	NiStreamLoadBinary( stream, spotExponent );
}

/*
================
NiSpotLight::LinkObject
================
*/
// FUNCTION: STBC 0x8070B0
void NiSpotLight::LinkObject( NiStream &stream ) {
	NiPointLight::LinkObject( stream );
}

/*
================
NiSpotLight::RegisterStreamables
================
*/
// FUNCTION: STBC 0x8070C0
bool NiSpotLight::RegisterStreamables( NiStream &stream ) {
	return NiPointLight::RegisterStreamables( stream );
}

/*
================
NiSpotLight::SaveBinary
================
*/
// FUNCTION: STBC 0x8070D0
void NiSpotLight::SaveBinary( NiStream &stream ) {
	NiPointLight::SaveBinary( stream );
	NiStreamSaveBinary( stream, spotAngle );
	NiStreamSaveBinary( stream, spotExponent );
}

/*
================
NiSpotLight::IsEqual
================
*/
// FUNCTION: STBC 0x807110
bool NiSpotLight::IsEqual( NiObject *object ) {
	if ( !NiPointLight::IsEqual( object ) ) {
		return false;
	}
	NiSpotLight *light = (NiSpotLight *)object;
	if ( spotAngle != light->spotAngle
		|| spotExponent != light->spotExponent ) {
		return false;
	}
	return true;
}

/*
================
NiSpotLight::GetViewerStrings
================
*/
// FUNCTION: STBC 0x807160
void NiSpotLight::GetViewerStrings( NiTArray<char *> *strings ) {
	NiPointLight::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( worldDirection.GetViewerString( "m_wDir" ) );
	strings->Add( NiGetViewerString( "m_fSpotAngle", spotAngle ) );
	strings->Add( NiGetViewerString( "m_fSpotExponent", spotExponent ) );
}
