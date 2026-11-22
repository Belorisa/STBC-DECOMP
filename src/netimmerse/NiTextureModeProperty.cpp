// NiTextureModeProperty.cpp -- how a texture is applied, filtered and clamped

#include "NiRTLib.h"
#include "NiTextureModeProperty.h"
#include "NiStream.h"
#include <stdio.h>
#include <string.h>
#include "NiMaterialProperty.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00801c10 _$E92
// FUNCTION: STBC 0x00801c30 _$E17
// FUNCTION: STBC 0x00801c50 _$E15
// FUNCTION: STBC 0x00801c60 _$E23
// FUNCTION: STBC 0x00801c80 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00801c90 _$E30
// FUNCTION: STBC 0x00801ca0 _$E41
// FUNCTION: STBC 0x00801cb0 _$E45
// FUNCTION: STBC 0x00801cc0 _$E49
// FUNCTION: STBC 0x00801cd0 _$E53
// FUNCTION: STBC 0x00801ce0 _$E58

// FUNCTION: STBC 0x008010e0 _$E74
// FUNCTION: STBC 0x00801100 _$E71
// DATA: STBC 0x9A2228 ?rtti@NiTextureModeProperty@@2VNiRTTI@@B
const NiRTTI NiTextureModeProperty::rtti( "NiTextureModeProperty", &NiProperty::rtti );

/*
================
NiTextureModeProperty::Register
================
*/
// FUNCTION: STBC 0x801120 ?Register@NiTextureModeProperty@@SAHXZ
NiImplementRegister( NiTextureModeProperty )

// FUNCTION: STBC 0x00801270 _$E80
// FUNCTION: STBC 0x00801290 _$E77
// DATA: STBC 0x9A2214
NiPointer<NiTextureModeProperty> NiTextureModeProperty::defaultProperty;

/*
===============================================================================

	NiTextureModePropertyInit (name invented)

	A file-scope object whose constructor creates the default property and
	whose destructor releases it.

===============================================================================
*/
class NiTextureModePropertyInit {
public:
	// FUNCTION: STBC 0x8012F0 ??0NiTextureModePropertyInit@@QAE@XZ
	NiTextureModePropertyInit() { NiTextureModeProperty::defaultProperty = new NiTextureModeProperty; }
	~NiTextureModePropertyInit() { NiTextureModeProperty::defaultProperty = 0; }
};

// FUNCTION: STBC 0x008012d0 _$E85
// FUNCTION: STBC 0x008013a0 _$E83
static NiTextureModePropertyInit init;

/*
================
NiTextureModeProperty::CreateClone
================
*/
// FUNCTION: STBC 0x8013D0
NiObject *NiTextureModeProperty::CreateClone() {
	NiTextureModeProperty *clone = new NiTextureModeProperty;
	CopyMembers( clone );
	return clone;
}

/*
================
NiTextureModeProperty::CopyMembers
================
*/
// FUNCTION: STBC 0x801460
void NiTextureModeProperty::CopyMembers( NiTextureModeProperty *dest ) {
	NiProperty::CopyMembers( dest );
	dest->l = l;
	dest->k = k;
}

/*
================
NiTextureModeProperty::CreateObject

Creates the property and loads it, with a record for NiObjectNET's link
IDs.
================
*/
// FUNCTION: STBC 0x801490
void NiTextureModeProperty::CreateObject( NiStream &stream, NiObject *&object ) {
	NiTextureModeProperty *mode = new NiTextureModeProperty;
	stream.SetLoadRecord( new NiTextureModePropertyLoadRecord );
	mode->LoadBinary( stream );
	object = mode;
}

/*
================
NiTextureModeProperty::LoadBinary

Files older than "3.02" number the apply modes differently: three of
their modes became APPLY_MODULATE. The level-of-detail values are stored
from "3.04" on.
================
*/
// FUNCTION: STBC 0x801570
void NiTextureModeProperty::LoadBinary( NiStream &stream ) {
	NiProperty::LoadBinary( stream );

	if ( strcmp( stream.GetFileVersion(), "3.02" ) < 0 ) {
		ApplyMode mode = GetApplyMode();
		switch ( mode ) {
		case 0:
			SetApplyMode( APPLY_REPLACE );
			break;
		case 1:
			SetApplyMode( APPLY_DECAL );
			break;
		case 2:
		case 3:
		case 4:
			SetApplyMode( APPLY_MODULATE );
			break;
		case 5:
			SetApplyMode( APPLY_HILIGHT );
			break;
		case 6:
			SetApplyMode( APPLY_HILIGHT2 );
			break;
		}
	}

	if ( strcmp( stream.GetFileVersion(), "3.04" ) >= 0 ) {
		NiStreamLoadBinary( stream, l );
		NiStreamLoadBinary( stream, k );
	}
}

/*
================
NiTextureModeProperty::LinkObject
================
*/
// FUNCTION: STBC 0x801680
void NiTextureModeProperty::LinkObject( NiStream &stream ) {
	NiProperty::LinkObject( stream );
}

/*
================
NiTextureModeProperty::RegisterStreamables
================
*/
// FUNCTION: STBC 0x801690
bool NiTextureModeProperty::RegisterStreamables( NiStream &stream ) {
	return NiProperty::RegisterStreamables( stream );
}

/*
================
NiTextureModeProperty::SaveBinary
================
*/
// FUNCTION: STBC 0x8016A0
void NiTextureModeProperty::SaveBinary( NiStream &stream ) {
	NiProperty::SaveBinary( stream );
	NiStreamSaveBinary( stream, l );
	NiStreamSaveBinary( stream, k );
}

/*
================
NiTextureModeProperty::IsEqual
================
*/
// FUNCTION: STBC 0x8016D0
bool NiTextureModeProperty::IsEqual( NiObject *object ) {
	if ( !NiProperty::IsEqual( object ) ) {
		return false;
	}

	NiTextureModeProperty *mode = (NiTextureModeProperty *)object;
	if ( GetApplyMode() != mode->GetApplyMode()
		|| GetFilterMode() != mode->GetFilterMode()
		|| GetClampMode() != mode->GetClampMode()
		|| l != mode->l
		|| k != mode->k ) {
		return false;
	}
	return true;
}

/*
================
NiTextureModeProperty::GetViewerStrings
================
*/
// FUNCTION: STBC 0x801710
void NiTextureModeProperty::GetViewerStrings( NiTArray<char *> *strings ) {
	NiProperty::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( GetViewerString( "m_apply", GetApplyMode() ) );
	strings->Add( GetViewerString( "m_filter", GetFilterMode() ) );
	strings->Add( GetViewerString( "m_clamp", GetClampMode() ) );
	strings->Add( NiGetViewerString( "m_sL", l ) );
	strings->Add( NiGetViewerString( "m_sK", k ) );
}

/*
================
NiTextureModeProperty::GetViewerString

The apply mode by name. The block leaves room for 15 characters of mode
name, and APPLY_MODULATE_VERTEX overruns it by six bytes (a shipped bug).
================
*/
// FUNCTION: STBC 0x8019D0 ?GetViewerString@NiTextureModeProperty@@SAPADPBDW4ApplyMode@1@@Z
char *NiTextureModeProperty::GetViewerString( const char *prefix, ApplyMode mode ) {
	char *string = new char[strlen( prefix ) + 19];
	switch ( mode ) {
	case APPLY_REPLACE:
		sprintf( string, "%s = APPLY_REPLACE", prefix );
		break;
	case APPLY_DECAL:
		sprintf( string, "%s = APPLY_DECAL", prefix );
		break;
	case APPLY_MODULATE:
		sprintf( string, "%s = APPLY_MODULATE", prefix );
		break;
	case APPLY_MODULATE_VERTEX:
		sprintf( string, "%s = APPLY_MODULATE_VERTEX", prefix );
		break;
	case APPLY_HILIGHT:
		sprintf( string, "%s = APPLY_HILIGHT", prefix );
		break;
	case APPLY_HILIGHT2:
		sprintf( string, "%s = APPLY_HILIGHT2", prefix );
		break;
	}
	return string;
}

/*
================
NiTextureModeProperty::GetViewerString

The filter mode by name.
================
*/
// FUNCTION: STBC 0x801AA0 ?GetViewerString@NiTextureModeProperty@@SAPADPBDW4FilterMode@1@@Z
char *NiTextureModeProperty::GetViewerString( const char *prefix, FilterMode mode ) {
	char *string = new char[strlen( prefix ) + 29];
	switch ( mode ) {
	case FILTER_NEAREST:
		sprintf( string, "%s = FILTER_NEAREST", prefix );
		break;
	case FILTER_BILERP:
		sprintf( string, "%s = FILTER_BILERP", prefix );
		break;
	case FILTER_TRILERP:
		sprintf( string, "%s = FILTER_TRILERP", prefix );
		break;
	case FILTER_NEAREST_MIPNEAREST:
		sprintf( string, "%s = FILTER_NEAREST_MIPNEAREST", prefix );
		break;
	case FILTER_NEAREST_MIPLERP:
		sprintf( string, "%s = FILTER_NEAREST_MIPLERP", prefix );
		break;
	case FILTER_BILERP_MIPNEAREST:
		sprintf( string, "%s = FILTER_BILERP_MIPNEAREST", prefix );
		break;
	}
	return string;
}

/*
================
NiTextureModeProperty::GetViewerString

The clamp mode by name.
================
*/
// FUNCTION: STBC 0x801B70 ?GetViewerString@NiTextureModeProperty@@SAPADPBDW4ClampMode@1@@Z
char *NiTextureModeProperty::GetViewerString( const char *prefix, ClampMode mode ) {
	char *string = new char[strlen( prefix ) + 19];
	switch ( mode ) {
	case CLAMP_S_CLAMP_T:
		sprintf( string, "%s = CLAMP_S_CLAMP_T", prefix );
		break;
	case CLAMP_S_WRAP_T:
		sprintf( string, "%s = CLAMP_S_WRAP_T", prefix );
		break;
	case WRAP_S_CLAMP_T:
		sprintf( string, "%s = WRAP_S_CLAMP_T", prefix );
		break;
	case WRAP_S_WRAP_T:
		sprintf( string, "%s = WRAP_S_WRAP_T", prefix );
		break;
	}
	return string;
}
