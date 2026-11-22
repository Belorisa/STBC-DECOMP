// NiFogProperty.cpp -- distance fog

#include "NiRTLib.h"
#include <stdio.h>
#include <string.h>

#include "NiFogProperty.h"
#include "NiStream.h"
#include "NiAVObject.h"
#include "NiAccumulator.h"
#include "NiCamera.h"
#include "NiDynamicEffect.h"
#include "NiFltAnimationNode.h"
#include "renderer/NiImage.h"
#include "NiNode.h"
#include "NiRawImageData.h"
#include "renderer/NiRender.h"
#include "NiScreenPolygon.h"
#include "NiSwitchNode.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007fe190 _$E34
// FUNCTION: STBC 0x007fe1a0 _$E45
// FUNCTION: STBC 0x007fe1b0 _$E49
// FUNCTION: STBC 0x007fe1c0 _$E53
// FUNCTION: STBC 0x007fe1d0 _$E57
// FUNCTION: STBC 0x007fe1e0 _$E62

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007fe110 _$E238
// FUNCTION: STBC 0x007fe130 _$E17
// FUNCTION: STBC 0x007fe150 _$E15
// FUNCTION: STBC 0x007fe160 _$E23
// FUNCTION: STBC 0x007fe180 _$E21

// FUNCTION: STBC 0x007fd890 _$E220
// FUNCTION: STBC 0x007fd8b0 _$E217
// DATA: STBC 0x9A20C8 ?rtti@NiFogProperty@@2VNiRTTI@@B
const NiRTTI NiFogProperty::rtti( "NiFogProperty", &NiProperty::rtti );

/*
================
NiFogProperty::Register
================
*/
// FUNCTION: STBC 0x7FD8D0 ?Register@NiFogProperty@@SAHXZ
NiImplementRegister( NiFogProperty )

// FUNCTION: STBC 0x007fda20 _$E226
// FUNCTION: STBC 0x007fda40 _$E223
// DATA: STBC 0x9A20D0
NiPointer<NiFogProperty> NiFogProperty::defaultProperty;

/*
===============================================================================

	NiFogPropertyInit (name invented)

	A file-scope object whose constructor creates the default property and
	whose destructor releases it.

===============================================================================
*/
class NiFogPropertyInit {
public:
	// FUNCTION: STBC 0x7FDAA0 ??0NiFogPropertyInit@@QAE@XZ
	NiFogPropertyInit() { NiFogProperty::defaultProperty = new NiFogProperty; }
	~NiFogPropertyInit() { NiFogProperty::defaultProperty = 0; }
};

// FUNCTION: STBC 0x007fda80 _$E231
// FUNCTION: STBC 0x007fdb60 _$E229
static NiFogPropertyInit init;

/*
================
NiFogProperty::CreateClone
================
*/
// FUNCTION: STBC 0x7FDB90
NiObject *NiFogProperty::CreateClone() {
	NiFogProperty *clone = new NiFogProperty;
	CopyMembers( clone );
	return clone;
}

/*
================
NiFogProperty::CopyMembers
================
*/
// FUNCTION: STBC 0x7FDC40
void NiFogProperty::CopyMembers( NiFogProperty *dest ) {
	NiProperty::CopyMembers( dest );
	dest->depth = depth;
	dest->color = color;
}

/*
================
NiFogProperty::CreateObject

Creates the property and loads it, with a record for NiObjectNET's link
IDs.
================
*/
// FUNCTION: STBC 0x7FDC70
void NiFogProperty::CreateObject( NiStream &stream, NiObject *&object ) {
	NiFogProperty *fog = new NiFogProperty;
	stream.SetLoadRecord( new NiFogPropertyLoadRecord );
	fog->LoadBinary( stream );
	object = fog;
}

/*
================
NiFogProperty::LoadBinary

================
*/
// FUNCTION: STBC 0x7FDD70
void NiFogProperty::LoadBinary( NiStream &stream ) {
	NiProperty::LoadBinary( stream );
	NiStreamLoadBinary( stream, depth );
	color.LoadBinary( stream );
}

/*
================
NiFogProperty::LinkObject
================
*/
// FUNCTION: STBC 0x7FDDA0
void NiFogProperty::LinkObject( NiStream &stream ) {
	NiProperty::LinkObject( stream );
}

/*
================
NiFogProperty::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7FDDB0
bool NiFogProperty::RegisterStreamables( NiStream &stream ) {
	return NiProperty::RegisterStreamables( stream );
}

/*
================
NiFogProperty::SaveBinary
================
*/
// FUNCTION: STBC 0x7FDDC0
void NiFogProperty::SaveBinary( NiStream &stream ) {
	NiProperty::SaveBinary( stream );
	NiStreamSaveBinary( stream, depth );
	color.SaveBinary( stream );
}

/*
================
NiFogProperty::IsEqual
================
*/
// FUNCTION: STBC 0x7FDDF0
bool NiFogProperty::IsEqual( NiObject *object ) {
	if ( !NiProperty::IsEqual( object ) ) {
		return false;
	}

	NiFogProperty *fog = (NiFogProperty *)object;
	if ( GetFog() != fog->GetFog()
		|| depth != fog->depth
		|| GetFogFunction() != fog->GetFogFunction()
		|| color != fog->color ) {
		return false;
	}
	return true;
}

/*
================
NiFogProperty::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7FDE70
void NiFogProperty::GetViewerStrings( NiTArray<char *> *strings ) {
	NiProperty::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_bFog", GetFog() ) );
	strings->Add( NiGetViewerString( "m_fDepth", depth ) );
	strings->Add( GetViewerString( "m_uiFunc", GetFogFunction() ) );
	strings->Add( color.GetViewerString( "m_color" ) );
}

/*
================
NiFogProperty::GetViewerString
================
*/
// FUNCTION: STBC 0x7FE0A0
char *NiFogProperty::GetViewerString( const char *prefix, FogFunction function ) {
	char *string = new char[strlen( prefix ) + 20];
	switch ( function ) {
	case FOG_Z_LINEAR:
		sprintf( string, "%s = FOG_Z_LINEAR", prefix );
		break;
	case FOG_RANGE_SQ:
		sprintf( string, "%s = FOG_RANGE_SQ", prefix );
		break;
	case FOG_VERTEX_ALPHA:
		sprintf( string, "%s = FOG_VERTEX_ALPHA", prefix );
		break;
	}
	return string;
}
