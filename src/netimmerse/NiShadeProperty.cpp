// NiShadeProperty.cpp -- smooth or flat shading

#include "NiRTLib.h"
#include "NiShadeProperty.h"
#include "NiStream.h"
#include "NiAVObject.h"
#include "NiAccumulator.h"
#include "NiDynamicEffect.h"
#include "renderer/NiImage.h"
#include "NiLight.h"
#include "NiPointLight.h"
#include "NiRawImageData.h"
#include "renderer/NiRender.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x008046e0 _$E223
// FUNCTION: STBC 0x00804700 _$E17
// FUNCTION: STBC 0x00804720 _$E15
// FUNCTION: STBC 0x00804730 _$E23
// FUNCTION: STBC 0x00804750 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00804760 _$E30
// FUNCTION: STBC 0x00804770 _$E41
// FUNCTION: STBC 0x00804780 _$E45
// FUNCTION: STBC 0x00804790 _$E49
// FUNCTION: STBC 0x008047a0 _$E53
// FUNCTION: STBC 0x008047f0 _$E58

// FUNCTION: STBC 0x008040a0 _$E205
// FUNCTION: STBC 0x008040c0 _$E202
// DATA: STBC 0x9A22F0 ?rtti@NiShadeProperty@@2VNiRTTI@@B
const NiRTTI NiShadeProperty::rtti( "NiShadeProperty", &NiProperty::rtti );

/*
================
NiShadeProperty::Register
================
*/
// FUNCTION: STBC 0x8040E0 ?Register@NiShadeProperty@@SAHXZ
NiImplementRegister( NiShadeProperty )

// FUNCTION: STBC 0x00804230 _$E211
// FUNCTION: STBC 0x00804250 _$E208
// DATA: STBC 0x9A22DC
NiPointer<NiShadeProperty> NiShadeProperty::defaultProperty;

/*
===============================================================================

	NiShadePropertyInit (name invented)

	A file-scope object whose constructor creates the default property and
	whose destructor releases it.

===============================================================================
*/
class NiShadePropertyInit {
public:
	// FUNCTION: STBC 0x8042B0 ??0NiShadePropertyInit@@QAE@XZ
	NiShadePropertyInit() { NiShadeProperty::defaultProperty = new NiShadeProperty; }
	~NiShadePropertyInit() { NiShadeProperty::defaultProperty = 0; }
};

// FUNCTION: STBC 0x00804290 _$E216
// FUNCTION: STBC 0x00804350 _$E214
static NiShadePropertyInit init;

/*
================
NiShadeProperty::CreateClone

NiShadeProperty has no members of its own to copy.
================
*/
// FUNCTION: STBC 0x804380
NiObject *NiShadeProperty::CreateClone() {
	NiShadeProperty *clone = new NiShadeProperty;
	CopyMembers( clone );
	return clone;
}

/*
================
NiShadeProperty::CreateObject

Creates the property and loads it, with a record for NiObjectNET's link
IDs.
================
*/
// FUNCTION: STBC 0x804400
void NiShadeProperty::CreateObject( NiStream &stream, NiObject *&object ) {
	NiShadeProperty *shade = new NiShadeProperty;
	stream.SetLoadRecord( new NiShadePropertyLoadRecord );
	shade->LoadBinary( stream );
	object = shade;
}

/*
================
NiShadeProperty::LoadBinary
================
*/
// FUNCTION: STBC 0x8044D0
void NiShadeProperty::LoadBinary( NiStream &stream ) {
	NiProperty::LoadBinary( stream );
}

/*
================
NiShadeProperty::LinkObject
================
*/
// FUNCTION: STBC 0x8044E0
void NiShadeProperty::LinkObject( NiStream &stream ) {
	NiProperty::LinkObject( stream );
}

/*
================
NiShadeProperty::RegisterStreamables
================
*/
// FUNCTION: STBC 0x8044F0
bool NiShadeProperty::RegisterStreamables( NiStream &stream ) {
	return NiProperty::RegisterStreamables( stream );
}

/*
================
NiShadeProperty::SaveBinary
================
*/
// FUNCTION: STBC 0x804500
void NiShadeProperty::SaveBinary( NiStream &stream ) {
	NiProperty::SaveBinary( stream );
}

/*
================
NiShadeProperty::IsEqual
================
*/
// FUNCTION: STBC 0x804510
bool NiShadeProperty::IsEqual( NiObject *object ) {
	if ( !NiProperty::IsEqual( object ) ) {
		return false;
	}

	NiShadeProperty *shade = (NiShadeProperty *)object;
	if ( GetSmooth() != shade->GetSmooth() ) {
		return false;
	}
	return true;
}

/*
================
NiShadeProperty::GetViewerStrings
================
*/
// FUNCTION: STBC 0x804540
void NiShadeProperty::GetViewerStrings( NiTArray<char *> *strings ) {
	NiProperty::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_bSmooth", GetSmooth() ) );
}
