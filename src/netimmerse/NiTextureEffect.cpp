// NiTextureEffect.cpp -- projected texture effects

#include "NiRTLib.h"
#include "NiCloneExtraData.h"
#include "NiStream.h"
#include "NiTextureEffect.h"
#include "renderer/NiRender.h"
#include <stdio.h>
#include <string.h>

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00809bd0 _$E209
// FUNCTION: STBC 0x00809b50 _$E17
// FUNCTION: STBC 0x00809b70 _$E15
// FUNCTION: STBC 0x00809b80 _$E23
// FUNCTION: STBC 0x00809ba0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00809bb0 _$E30
// FUNCTION: STBC 0x00809bc0 _$E41
// FUNCTION: STBC 0x00809bf0 _$E191
// FUNCTION: STBC 0x00809c00 _$E120
// FUNCTION: STBC 0x00809c10 _$E133
// FUNCTION: STBC 0x00809c20 _$E46
// FUNCTION: STBC 0x00809c30 _$E50
// FUNCTION: STBC 0x00809c40 _$E54
// FUNCTION: STBC 0x00809c50 _$E178
// FUNCTION: STBC 0x00809c60 _$E183
// FUNCTION: STBC 0x00809c70 _$E196
// FUNCTION: STBC 0x00809c80 _$E108
// FUNCTION: STBC 0x00809cc0 _$E187

// FUNCTION: STBC 0x00808a20 _$E202
// FUNCTION: STBC 0x00808a40 _$E199
// DATA: STBC 0x9A2498 ?rtti@NiTextureEffect@@2VNiRTTI@@B
const NiRTTI NiTextureEffect::rtti( "NiTextureEffect", &NiDynamicEffect::rtti );

/*
================
NiTextureEffect::Register
================
*/
// FUNCTION: STBC 0x808A60 ?Register@NiTextureEffect@@SAHXZ
NiImplementRegister( NiTextureEffect )

/*
================
NiTextureEffect::NiTextureEffect

An identity projection without an image, wrapping in both directions.
================
*/
// FUNCTION: STBC 0x808BB0 ??0NiTextureEffect@@QAE@XZ
NiTextureEffect::NiTextureEffect()
	: modelProjection( NiMatrix3::IDENTITY ), modelProjectionTranslate( 0.0f, 0.0f, 0.0f ),
	worldProjection( NiMatrix3::IDENTITY ), worldProjectionTranslate( 0.0f, 0.0f, 0.0f ),
	modelPlane( NiPoint3::UNIT_X, 0.0f ), worldPlane( NiPoint3::UNIT_X, 0.0f ) {
	filter = NiTextureModeProperty::FILTER_NEAREST;
	clamp = NiTextureModeProperty::WRAP_S_WRAP_T;
	textureType = PROJECTED_LIGHT;
	coordMode = WORLD_PARALLEL;
	planeEnabled = false;
	l = 0;
	k = -75;
	unknown1BC = false;
	unknown1BD = false;
}

/*
================
NiTextureEffect::~NiTextureEffect

The renderers drop what they cache for the effect.
================
*/
// FUNCTION: STBC 0x808CB0 ??1NiTextureEffect@@UAE@XZ
// FUNCTION: STBC 0x809CA0 ??_GNiTextureEffect@@UAEPAXI@Z
NiTextureEffect::~NiTextureEffect() {
	NiRender::RemoveEffect( this );
}

/*
================
NiTextureEffect::UpdateProjection

A sphere map projects the view through the world rotation's last two
columns onto [0,1]; every other mode carries the model projection into
world space. The world clipping plane follows the world transform, and the
revision tells the renderers the effect moved.
================
*/
// FUNCTION: STBC 0x808D30
void NiTextureEffect::UpdateProjection() {
	if ( coordMode == SPHERE_MAP ) {
		worldProjectionTranslate.x = worldProjectionTranslate.y = 0.5f;
		worldProjectionTranslate.z = 0.0f;
		worldProjection.SetEntry( 0, 0, worldRotate.GetEntry( 0, 2 ) * 0.5f );
		worldProjection.SetEntry( 0, 1, worldRotate.GetEntry( 1, 2 ) * 0.5f );
		worldProjection.SetEntry( 0, 2, worldRotate.GetEntry( 2, 2 ) * 0.5f );
		worldProjection.SetEntry( 1, 0, worldRotate.GetEntry( 0, 1 ) * -0.5f );
		worldProjection.SetEntry( 1, 1, worldRotate.GetEntry( 1, 1 ) * -0.5f );
		worldProjection.SetEntry( 1, 2, worldRotate.GetEntry( 2, 1 ) * -0.5f );
		worldProjection.SetEntry( 2, 0, 0.0f );
		worldProjection.SetEntry( 2, 1, 0.0f );
		worldProjection.SetEntry( 2, 2, 0.0f );
	} else {
		float invScale = 1.0f / worldScale;
		worldProjection = modelProjection * worldRotate.Transpose() * invScale;
		worldProjectionTranslate = modelProjectionTranslate - worldProjection * worldTranslate;
	}

	NiPoint3 worldNormal = worldRotate * modelPlane.GetNormal();
	float worldConstant = modelPlane.GetConstant() * worldScale + worldNormal.Dot( worldTranslate );
	worldPlane.SetNormal( worldNormal );
	worldPlane.SetConstant( worldConstant );
	IncRevisionID();
}

/*
================
NiTextureEffect::UpdateWorldData
================
*/
// FUNCTION: STBC 0x808F00
void NiTextureEffect::UpdateWorldData() {
	NiAVObject::UpdateWorldData();
	UpdateProjection();
}

/*
================
NiTextureEffect::CreateClone

Marks the original with its clone unless it already carries the mark.
================
*/
// FUNCTION: STBC 0x808F20
NiObject *NiTextureEffect::CreateClone() {
	NiTextureEffect *result = new NiTextureEffect;	// (name invented; fits the /Od frame order)
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
NiTextureEffect::CopyMembers

The clone shares the image; the world projection and plane follow at its
next update.
================
*/
// FUNCTION: STBC 0x809000
void NiTextureEffect::CopyMembers( NiTextureEffect *dest ) {
	NiDynamicEffect::CopyMembers( dest );
	dest->image = image;
	dest->modelProjection = modelProjection;
	dest->modelProjectionTranslate = modelProjectionTranslate;
	dest->filter = filter;
	dest->clamp = clamp;
	dest->textureType = textureType;
	dest->coordMode = coordMode;
	dest->planeEnabled = planeEnabled;
	dest->modelPlane = modelPlane;
	dest->worldPlane = worldPlane;
	dest->l = l;
	dest->k = k;
	dest->unknown1BC = unknown1BC;
	dest->unknown1BD = unknown1BD;
}

/*
================
NiTextureEffect::CreateObject

Creates the effect and loads it.
================
*/
// FUNCTION: STBC 0x809120
void NiTextureEffect::CreateObject( NiStream &stream, NiObject *&object ) {
	NiTextureEffect *effect = new NiTextureEffect;
	stream.SetLoadRecord( new NiTextureEffectLoadRecord );
	effect->LoadBinary( stream );
	object = effect;
}

/*
================
NiTextureEffect::LoadBinary

The model projection and the modes; the image's link ID and the clipping
plane from "3.03" on (older files get no image and the disabled default
plane), the remaining members from "3.04" on.
================
*/
// FUNCTION: STBC 0x809270
void NiTextureEffect::LoadBinary( NiStream &stream ) {
	NiDynamicEffect::LoadBinary( stream );
	NiTextureEffectLoadRecord *record = (NiTextureEffectLoadRecord *)stream.GetLoadRecord();

	modelProjection.LoadBinary( stream );
	modelProjectionTranslate.LoadBinary( stream );
	NiStreamLoadEnum( stream, filter );
	NiStreamLoadEnum( stream, clamp );
	NiStreamLoadEnum( stream, textureType );
	NiStreamLoadEnum( stream, coordMode );

	if ( strcmp( stream.GetFileVersion(), "3.03" ) >= 0 ) {
		NiStreamLoadBinary( stream, record->imageLinkID );
		NiStreamLoadBinary( stream, planeEnabled );
		modelPlane.LoadBinary( stream );
		if ( strcmp( stream.GetFileVersion(), "3.04" ) >= 0 ) {
			NiStreamLoadBinary( stream, l );
			NiStreamLoadBinary( stream, k );
			NiStreamLoadBinary( stream, unknown1BC );
			NiStreamLoadBinary( stream, unknown1BD );
		}
	} else {
		record->imageLinkID = 0;
		planeEnabled = false;
		modelPlane = NiPlane( NiPoint3::UNIT_X, 0.0f );
	}
}

/*
================
NiTextureEffect::LinkObject
================
*/
// FUNCTION: STBC 0x809430
void NiTextureEffect::LinkObject( NiStream &stream ) {
	NiDynamicEffect::LinkObject( stream );
	NiTextureEffectLoadRecord *record = (NiTextureEffectLoadRecord *)stream.GetLoadRecord();
	image = (NiImage *)stream.GetObjectFromLinkID( record->imageLinkID );
}

/*
================
NiTextureEffect::RegisterStreamables

Registers the image after the effect.
================
*/
// FUNCTION: STBC 0x809490
bool NiTextureEffect::RegisterStreamables( NiStream &stream ) {
	if ( !NiDynamicEffect::RegisterStreamables( stream ) ) {
		return false;
	}
	if ( image ) {
		image->RegisterStreamables( stream );
	}
	return true;
}

/*
================
NiTextureEffect::SaveBinary

The model projection, the modes, the image's link ID, the clipping plane
and the remaining members; the world projection and plane are not saved.
================
*/
// FUNCTION: STBC 0x8094C0
void NiTextureEffect::SaveBinary( NiStream &stream ) {
	NiDynamicEffect::SaveBinary( stream );
	modelProjection.SaveBinary( stream );
	modelProjectionTranslate.SaveBinary( stream );
	NiStreamSaveEnum( stream, filter );
	NiStreamSaveEnum( stream, clamp );
	NiStreamSaveEnum( stream, textureType );
	NiStreamSaveEnum( stream, coordMode );
	NiStreamSaveBinary( stream, (NiImage *)image );
	NiStreamSaveBinary( stream, planeEnabled );
	modelPlane.SaveBinary( stream );
	NiStreamSaveBinary( stream, l );
	NiStreamSaveBinary( stream, k );
	NiStreamSaveBinary( stream, unknown1BC );
	NiStreamSaveBinary( stream, unknown1BD );
}

/*
================
NiTextureEffect::IsEqual

The images are not compared, nor the world projection and plane.
================
*/
// FUNCTION: STBC 0x8095B0
bool NiTextureEffect::IsEqual( NiObject *object ) {
	if ( !NiDynamicEffect::IsEqual( object ) ) {
		return false;
	}

	NiTextureEffect *effect = (NiTextureEffect *)object;
	if ( effect->modelProjection != modelProjection ||
		effect->modelProjectionTranslate != modelProjectionTranslate ||
		effect->filter != filter ||
		effect->clamp != clamp ||
		effect->textureType != textureType ||
		effect->coordMode != coordMode ||
		effect->planeEnabled != planeEnabled ||
		effect->modelPlane != modelPlane ||
		effect->unknown1BC != unknown1BC ||
		effect->unknown1BD != unknown1BD ||
		effect->l != l ||
		effect->k != k ) {
		return false;
	}
	return true;
}

/*
================
NiTextureEffect::GetViewerStrings

The texture type is listed twice under the same name, once before the
filter and once after the clamp mode (a quirk of the original).
================
*/
// FUNCTION: STBC 0x8096F0
// PARKED: inline budget: retail calls SetAtGrow (0x7DDDF0) out of line for the coordinate mode's Add and expands the repeated texture type's; this build does the reverse
void NiTextureEffect::GetViewerStrings( NiTArray<char *> *strings ) {
	NiDynamicEffect::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( modelProjection.GetViewerString( "m_modelProjMat" ) );
	strings->Add( modelProjectionTranslate.GetViewerString( "m_modelProjTrans" ) );
	strings->Add( GetViewerString( "m_eTextureMode", textureType ) );
	strings->Add( NiTextureModeProperty::GetViewerString( "m_eFilter", filter ) );
	strings->Add( NiTextureModeProperty::GetViewerString( "m_eClamp", clamp ) );
	strings->Add( GetViewerString( "m_eTextureMode", textureType ) );
	strings->Add( GetViewerString( "m_eCoordMode", coordMode ) );
	strings->Add( NiGetViewerString( "m_bPlaneEnable", planeEnabled ) );
	strings->Add( modelPlane.GetViewerString( "m_modelPlane" ) );
	strings->Add( NiGetViewerString( "m_sL", l ) );
	strings->Add( NiGetViewerString( "m_sK", k ) );
}

/*
================
NiTextureEffect::GetViewerString

The texture type by name.
================
*/
// FUNCTION: STBC 0x809A40 ?GetViewerString@NiTextureEffect@@SAPADPBDW4TextureType@1@@Z
char *NiTextureEffect::GetViewerString( const char *prefix, TextureType type ) {
	char *string = new char[strlen( prefix ) + 19];
	switch ( type ) {
	case PROJECTED_LIGHT:
		sprintf( string, "%s = PROJECTED_LIGHT", prefix );
		break;
	case PROJECTED_SHADOW:
		sprintf( string, "%s = PROJECTED_SHADOW", prefix );
		break;
	case ENVIRONMENT_MAP:
		sprintf( string, "%s = ENVIRONMENT_MAP", prefix );
		break;
	case FOG_MAP:
		sprintf( string, "%s = FOG_MAP", prefix );
		break;
	}
	return string;
}

/*
================
NiTextureEffect::GetViewerString

The coordinate mode by name.
================
*/
// FUNCTION: STBC 0x809AE0 ?GetViewerString@NiTextureEffect@@SAPADPBDW4CoordGenType@1@@Z
char *NiTextureEffect::GetViewerString( const char *prefix, CoordGenType mode ) {
	char *string = new char[strlen( prefix ) + 19];
	switch ( mode ) {
	case WORLD_PARALLEL:
		sprintf( string, "%s = WORLD_PARALLEL", prefix );
		break;
	case WORLD_PERSPECTIVE:
		sprintf( string, "%s = WORLD_PERSPECTIVE", prefix );
		break;
	case SPHERE_MAP:
		sprintf( string, "%s = SPHERE_MAP", prefix );
		break;
	}
	return string;
}
