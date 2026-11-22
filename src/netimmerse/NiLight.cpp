// NiLight.cpp -- the base of the light effects

#include "NiRTLib.h"
#include <assert.h>
#include "NiLight.h"
#include "NiStream.h"
#include "NiCamera.h"
#include "NiGeometry.h"
#include "NiGeometryData.h"
#include "NiNode.h"
#include "renderer/NiRender.h"
#include "NiScreenPolygon.h"
#include "NiSkinController.h"
#include "NiTriBasedGeom.h"
#include "NiTriBasedGeomData.h"
#include "NiTriShape.h"
#include "NiTriShapeData.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007f3d30 _$E34
// FUNCTION: STBC 0x007f3d40 _$E45
// FUNCTION: STBC 0x007f3d50 _$E49
// FUNCTION: STBC 0x007f3d60 _$E53
// FUNCTION: STBC 0x007f3d70 _$E57
// FUNCTION: STBC 0x007f3d80 _$E177
// FUNCTION: STBC 0x007f3d90 _$E182
// FUNCTION: STBC 0x007f3dd0 _$E186
// FUNCTION: STBC 0x007f3de0 _$E199
// FUNCTION: STBC 0x007f3df0 _$E190
// FUNCTION: STBC 0x007f3e00 _$E119
// FUNCTION: STBC 0x007f3e10 _$E132
// FUNCTION: STBC 0x007f3e20 _$E195

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007f3d10 _$E260
// FUNCTION: STBC 0x007f3cb0 _$E17
// FUNCTION: STBC 0x007f3cd0 _$E15
// FUNCTION: STBC 0x007f3ce0 _$E23
// FUNCTION: STBC 0x007f3d00 _$E21

// FUNCTION: STBC 0x007f3610 _$E253
// FUNCTION: STBC 0x007f3630 _$E250
// DATA: STBC 0x9A1BF8 ?rtti@NiLight@@2VNiRTTI@@B
const NiRTTI NiLight::rtti( "NiLight", &NiDynamicEffect::rtti );

/*
================
NiLight::Register
================
*/
// FUNCTION: STBC 0x7F3650 ?Register@NiLight@@SAHXZ
NiImplementRegister( NiLight )

/*
================
NiLight::NiLight

A light has no extent: its world bound keeps a zero radius.
================
*/
// FUNCTION: STBC 0x7F37A0
NiLight::NiLight() {
	dimmer = 1.0f;
	ambient = NiColor( 1.0f, 1.0f, 1.0f );
	diffuse = NiColor( 1.0f, 1.0f, 1.0f );
	specular = NiColor( 1.0f, 1.0f, 1.0f );
	worldBound.radius = 0.0f;
}

/*
================
NiLight::~NiLight
================
*/
// FUNCTION: STBC 0x7F3830
NiLight::~NiLight() {
}

/*
================
NiLight::CreateClone

NiLight itself is never cloned; the derived classes create the clone.
================
*/
// FUNCTION: STBC 0x7F3840
NiObject *NiLight::CreateClone() {
	assert( false );
	return 0;
}

/*
================
NiLight::CopyMembers
================
*/
// FUNCTION: STBC 0x7F3850
void NiLight::CopyMembers( NiLight *dest ) {
	NiDynamicEffect::CopyMembers( dest );
	dest->dimmer = dimmer;
	dest->ambient = ambient;
	dest->diffuse = diffuse;
	dest->specular = specular;
}

/*
================
NiLight::CreateObject

The class is abstract: the stream creates nothing for it.
================
*/
// FUNCTION: STBC 0x7F38E0
void NiLight::CreateObject( NiStream &stream, NiObject *&object ) {
}

/*
================
NiLight::LoadBinary
================
*/
// FUNCTION: STBC 0x7F38F0
void NiLight::LoadBinary( NiStream &stream ) {
	NiDynamicEffect::LoadBinary( stream );
	NiStreamLoadBinary( stream, dimmer );
	ambient.LoadBinary( stream );
	diffuse.LoadBinary( stream );
	specular.LoadBinary( stream );
}

/*
================
NiLight::LinkObject
================
*/
// FUNCTION: STBC 0x7F3940
void NiLight::LinkObject( NiStream &stream ) {
	NiDynamicEffect::LinkObject( stream );
}

/*
================
NiLight::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7F3950
bool NiLight::RegisterStreamables( NiStream &stream ) {
	return NiDynamicEffect::RegisterStreamables( stream );
}

/*
================
NiLight::SaveBinary
================
*/
// FUNCTION: STBC 0x7F3960
void NiLight::SaveBinary( NiStream &stream ) {
	NiDynamicEffect::SaveBinary( stream );
	NiStreamSaveBinary( stream, dimmer );
	ambient.SaveBinary( stream );
	diffuse.SaveBinary( stream );
	specular.SaveBinary( stream );
}

/*
================
NiLight::IsEqual
================
*/
// FUNCTION: STBC 0x7F39B0
bool NiLight::IsEqual( NiObject *object ) {
	if ( !NiDynamicEffect::IsEqual( object ) ) {
		return false;
	}
	NiLight *light = (NiLight *)object;
	if ( dimmer != light->dimmer
		|| ambient != light->ambient || diffuse != light->diffuse
		|| specular != light->specular ) {
		return false;
	}
	return true;
}

/*
================
NiLight::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7F3AA0
void NiLight::GetViewerStrings( NiTArray<char *> *strings ) {
	NiDynamicEffect::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_fDimmer", dimmer ) );
	strings->Add( ambient.GetViewerString( "m_amb" ) );
	strings->Add( diffuse.GetViewerString( "m_diff" ) );
	strings->Add( specular.GetViewerString( "m_spec" ) );
}
