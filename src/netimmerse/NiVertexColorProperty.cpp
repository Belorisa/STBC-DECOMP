// NiVertexColorProperty.cpp -- how vertex colours feed the lighting

#include "NiRTLib.h"
#include <stdio.h>
#include <string.h>

#include "NiVertexColorProperty.h"
#include "NiStream.h"
#include "NiAVObject.h"
#include "NiAccumulator.h"
#include "NiCamera.h"
#include "NiDynamicEffect.h"
#include "NiGeometry.h"
#include "NiGeometryData.h"
#include "renderer/NiImage.h"
#include "NiNode.h"
#include "NiRawImageData.h"
#include "renderer/NiRender.h"
#include "NiScreenPolygon.h"
#include "NiTriBasedGeom.h"
#include "NiTriBasedGeomData.h"
#include "NiTriStrips.h"
#include "NiTriStripsData.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x0080fe80 _$E261
// FUNCTION: STBC 0x0080fea0 _$E17
// FUNCTION: STBC 0x0080fec0 _$E15
// FUNCTION: STBC 0x0080fed0 _$E23
// FUNCTION: STBC 0x0080fef0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0080ff00 _$E30
// FUNCTION: STBC 0x0080ff10 _$E41
// FUNCTION: STBC 0x0080ff20 _$E45
// FUNCTION: STBC 0x0080ff30 _$E49
// FUNCTION: STBC 0x0080ff40 _$E53
// FUNCTION: STBC 0x0080ff50 _$E58

// FUNCTION: STBC 0x0080f690 _$E243
// FUNCTION: STBC 0x0080f6b0 _$E240
// DATA: STBC 0x9A2758 ?rtti@NiVertexColorProperty@@2VNiRTTI@@B
const NiRTTI NiVertexColorProperty::rtti( "NiVertexColorProperty", &NiProperty::rtti );

/*
================
NiVertexColorProperty::Register
================
*/
// FUNCTION: STBC 0x80F6D0 ?Register@NiVertexColorProperty@@SAHXZ
NiImplementRegister( NiVertexColorProperty )

// FUNCTION: STBC 0x0080f820 _$E249
// FUNCTION: STBC 0x0080f840 _$E246
// DATA: STBC 0x9A2750
NiPointer<NiVertexColorProperty> NiVertexColorProperty::defaultProperty;

/*
===============================================================================

	NiVertexColorPropertyInit (name invented)

	A file-scope object whose constructor creates the default property and
	whose destructor releases it.

===============================================================================
*/
class NiVertexColorPropertyInit {
public:
	// FUNCTION: STBC 0x80F8A0 ??0NiVertexColorPropertyInit@@QAE@XZ
	NiVertexColorPropertyInit() { NiVertexColorProperty::defaultProperty = new NiVertexColorProperty; }
	~NiVertexColorPropertyInit() { NiVertexColorProperty::defaultProperty = 0; }
};

// FUNCTION: STBC 0x0080f880 _$E254
// FUNCTION: STBC 0x0080f950 _$E252
static NiVertexColorPropertyInit init;

/*
================
NiVertexColorProperty::CreateClone
================
*/
// FUNCTION: STBC 0x80F980
NiObject *NiVertexColorProperty::CreateClone() {
	NiVertexColorProperty *clone = new NiVertexColorProperty;
	CopyMembers( clone );
	return clone;
}

/*
================
NiVertexColorProperty::CopyMembers
================
*/
// FUNCTION: STBC 0x80FA10
void NiVertexColorProperty::CopyMembers( NiVertexColorProperty *dest ) {
	NiProperty::CopyMembers( dest );
	dest->source = source;
	dest->lighting = lighting;
}

/*
================
NiVertexColorProperty::CreateObject

Creates the property and loads it, with a record for NiObjectNET's link
IDs.
================
*/
// FUNCTION: STBC 0x80FA30
void NiVertexColorProperty::CreateObject( NiStream &stream, NiObject *&object ) {
	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	stream.SetLoadRecord( new NiVertexColorPropertyLoadRecord );
	vertexColor->LoadBinary( stream );
	object = vertexColor;
}

/*
================
NiVertexColorProperty::LoadBinary

The modes are saved as ints.
================
*/
// FUNCTION: STBC 0x80FB00
void NiVertexColorProperty::LoadBinary( NiStream &stream ) {
	NiProperty::LoadBinary( stream );
	NiStreamLoadEnum( stream, source );
	NiStreamLoadEnum( stream, lighting );
}

/*
================
NiVertexColorProperty::LinkObject
================
*/
// FUNCTION: STBC 0x80FB40
void NiVertexColorProperty::LinkObject( NiStream &stream ) {
	NiProperty::LinkObject( stream );
}

/*
================
NiVertexColorProperty::RegisterStreamables
================
*/
// FUNCTION: STBC 0x80FB50
bool NiVertexColorProperty::RegisterStreamables( NiStream &stream ) {
	return NiProperty::RegisterStreamables( stream );
}

/*
================
NiVertexColorProperty::SaveBinary
================
*/
// FUNCTION: STBC 0x80FB60
void NiVertexColorProperty::SaveBinary( NiStream &stream ) {
	NiProperty::SaveBinary( stream );
	NiStreamSaveEnum( stream, source );
	NiStreamSaveEnum( stream, lighting );
}

/*
================
NiVertexColorProperty::IsEqual
================
*/
// FUNCTION: STBC 0x80FB90
bool NiVertexColorProperty::IsEqual( NiObject *object ) {
	if ( !NiProperty::IsEqual( object ) ) {
		return false;
	}

	NiVertexColorProperty *vertexColor = (NiVertexColorProperty *)object;
	if ( GetSourceMode() != vertexColor->GetSourceMode() || GetLightingMode() != vertexColor->GetLightingMode() ) {
		return false;
	}
	return true;
}

/*
================
NiVertexColorProperty::GetViewerStrings
================
*/
// FUNCTION: STBC 0x80FBD0
void NiVertexColorProperty::GetViewerStrings( NiTArray<char *> *strings ) {
	NiProperty::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( GetViewerString( "m_eSource", GetSourceMode() ) );
	strings->Add( GetViewerString( "m_eLighting", GetLightingMode() ) );
}

/*
================
NiVertexColorProperty::GetViewerString

The block is left unwritten for a mode out of range.
================
*/
// FUNCTION: STBC 0x80FDB0 ?GetViewerString@NiVertexColorProperty@@KAPADPBDW4SourceVertexMode@1@@Z
char *NiVertexColorProperty::GetViewerString( const char *prefix, SourceVertexMode mode ) {
	char *string = new char[strlen( prefix ) + 26];
	switch ( mode ) {
	case SOURCE_IGNORE:
		sprintf( string, "%s = SOURCE_IGNORE", prefix );
		break;
	case SOURCE_EMISSIVE:
		sprintf( string, "%s = SOURCE_EMISSIVE", prefix );
		break;
	case SOURCE_AMB_DIFF:
		sprintf( string, "%s = SOURCE_AMB_DIFF", prefix );
		break;
	}
	return string;
}

/*
================
NiVertexColorProperty::GetViewerString

The block is left unwritten for a mode out of range.
================
*/
// FUNCTION: STBC 0x80FE20 ?GetViewerString@NiVertexColorProperty@@KAPADPBDW4LightingMode@1@@Z
char *NiVertexColorProperty::GetViewerString( const char *prefix, LightingMode mode ) {
	char *string = new char[strlen( prefix ) + 26];
	switch ( mode ) {
	case LIGHTING_E:
		sprintf( string, "%s = LIGHTING_E", prefix );
		break;
	case LIGHTING_E_A_D:
		sprintf( string, "%s = LIGHTING_E_A_D", prefix );
		break;
	}
	return string;
}
