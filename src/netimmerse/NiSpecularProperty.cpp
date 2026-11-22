// NiSpecularProperty.cpp -- specular lighting on or off

#include "NiRTLib.h"
#include "NiSpecularProperty.h"
#include "NiStream.h"
#include "NiAVObject.h"
#include "NiAccumulator.h"
#include "NiCamera.h"
#include "NiDynamicEffect.h"
#include "renderer/NiImage.h"
#include "NiNode.h"
#include "NiRawImageData.h"
#include "renderer/NiRender.h"
#include "NiScreenPolygon.h"
#include "NiSortAdjustNode.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00806a00 _$E234
// FUNCTION: STBC 0x00806a20 _$E17
// FUNCTION: STBC 0x00806a40 _$E15
// FUNCTION: STBC 0x00806a50 _$E23
// FUNCTION: STBC 0x00806a70 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00806a80 _$E30
// FUNCTION: STBC 0x00806a90 _$E41
// FUNCTION: STBC 0x00806aa0 _$E45
// FUNCTION: STBC 0x00806ab0 _$E49
// FUNCTION: STBC 0x00806ac0 _$E53
// FUNCTION: STBC 0x00806ad0 _$E58

// FUNCTION: STBC 0x008063c0 _$E216
// FUNCTION: STBC 0x008063e0 _$E213
// DATA: STBC 0x9A23B0 ?rtti@NiSpecularProperty@@2VNiRTTI@@B
const NiRTTI NiSpecularProperty::rtti( "NiSpecularProperty", &NiProperty::rtti );

/*
================
NiSpecularProperty::Register
================
*/
// FUNCTION: STBC 0x806400 ?Register@NiSpecularProperty@@SAHXZ
NiImplementRegister( NiSpecularProperty )

// FUNCTION: STBC 0x00806550 _$E222
// FUNCTION: STBC 0x00806570 _$E219
// DATA: STBC 0x9A23AC
NiPointer<NiSpecularProperty> NiSpecularProperty::defaultProperty;

/*
===============================================================================

	NiSpecularPropertyInit (name invented)

	A file-scope object whose constructor creates the default property and
	whose destructor releases it.

===============================================================================
*/
class NiSpecularPropertyInit {
public:
	// FUNCTION: STBC 0x8065D0 ??0NiSpecularPropertyInit@@QAE@XZ
	NiSpecularPropertyInit() { NiSpecularProperty::defaultProperty = new NiSpecularProperty; }
	~NiSpecularPropertyInit() { NiSpecularProperty::defaultProperty = 0; }
};

// FUNCTION: STBC 0x008065b0 _$E227
// FUNCTION: STBC 0x00806670 _$E225
static NiSpecularPropertyInit init;

/*
================
NiSpecularProperty::CreateClone

NiSpecularProperty has no members of its own to copy.
================
*/
// FUNCTION: STBC 0x8066A0
NiObject *NiSpecularProperty::CreateClone() {
	NiSpecularProperty *clone = new NiSpecularProperty;
	CopyMembers( clone );
	return clone;
}

/*
================
NiSpecularProperty::CreateObject

Creates the property and loads it, with a record for NiObjectNET's link
IDs.
================
*/
// FUNCTION: STBC 0x806720
void NiSpecularProperty::CreateObject( NiStream &stream, NiObject *&object ) {
	NiSpecularProperty *specular = new NiSpecularProperty;
	stream.SetLoadRecord( new NiSpecularPropertyLoadRecord );
	specular->LoadBinary( stream );
	object = specular;
}

/*
================
NiSpecularProperty::LoadBinary
================
*/
// FUNCTION: STBC 0x8067F0
void NiSpecularProperty::LoadBinary( NiStream &stream ) {
	NiProperty::LoadBinary( stream );
}

/*
================
NiSpecularProperty::LinkObject
================
*/
// FUNCTION: STBC 0x806800
void NiSpecularProperty::LinkObject( NiStream &stream ) {
	NiProperty::LinkObject( stream );
}

/*
================
NiSpecularProperty::RegisterStreamables
================
*/
// FUNCTION: STBC 0x806810
bool NiSpecularProperty::RegisterStreamables( NiStream &stream ) {
	return NiProperty::RegisterStreamables( stream );
}

/*
================
NiSpecularProperty::SaveBinary
================
*/
// FUNCTION: STBC 0x806820
void NiSpecularProperty::SaveBinary( NiStream &stream ) {
	NiProperty::SaveBinary( stream );
}

/*
================
NiSpecularProperty::IsEqual
================
*/
// FUNCTION: STBC 0x806830
bool NiSpecularProperty::IsEqual( NiObject *object ) {
	if ( !NiProperty::IsEqual( object ) ) {
		return false;
	}

	NiSpecularProperty *specular = (NiSpecularProperty *)object;
	if ( GetSpecular() != specular->GetSpecular() ) {
		return false;
	}
	return true;
}

/*
================
NiSpecularProperty::GetViewerStrings
================
*/
// FUNCTION: STBC 0x806860
void NiSpecularProperty::GetViewerStrings( NiTArray<char *> *strings ) {
	NiProperty::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_bSpec", GetSpecular() ) );
}
