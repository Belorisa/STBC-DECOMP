// NiDitherProperty.cpp -- dithering

#include "NiRTLib.h"
#include "NiDitherProperty.h"
#include "NiStream.h"
#include "NiAVObject.h"
#include "NiAccumulator.h"
#include "NiDirectionalLight.h"
#include "NiDynamicEffect.h"
#include "renderer/NiImage.h"
#include "NiLight.h"
#include "NiRawImageData.h"
#include "renderer/NiRender.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007faa40 _$E223
// FUNCTION: STBC 0x007faa60 _$E17
// FUNCTION: STBC 0x007faa80 _$E15
// FUNCTION: STBC 0x007faa90 _$E23
// FUNCTION: STBC 0x007faab0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007faac0 _$E30
// FUNCTION: STBC 0x007faad0 _$E41
// FUNCTION: STBC 0x007faae0 _$E45
// FUNCTION: STBC 0x007faaf0 _$E49
// FUNCTION: STBC 0x007fab00 _$E53
// FUNCTION: STBC 0x007fab10 _$E58

// FUNCTION: STBC 0x007fa400 _$E205
// FUNCTION: STBC 0x007fa420 _$E202
// DATA: STBC 0x9A1F70 ?rtti@NiDitherProperty@@2VNiRTTI@@B
const NiRTTI NiDitherProperty::rtti( "NiDitherProperty", &NiProperty::rtti );

/*
================
NiDitherProperty::Register
================
*/
// FUNCTION: STBC 0x7FA440 ?Register@NiDitherProperty@@SAHXZ
NiImplementRegister( NiDitherProperty )

// FUNCTION: STBC 0x007fa590 _$E211
// FUNCTION: STBC 0x007fa5b0 _$E208
// DATA: STBC 0x9A1F54
NiPointer<NiDitherProperty> NiDitherProperty::defaultProperty;

/*
===============================================================================

	NiDitherPropertyInit (name invented)

	A file-scope object whose constructor creates the default property and
	whose destructor releases it.

===============================================================================
*/
class NiDitherPropertyInit {
public:
	// FUNCTION: STBC 0x7FA610 ??0NiDitherPropertyInit@@QAE@XZ
	NiDitherPropertyInit() { NiDitherProperty::defaultProperty = new NiDitherProperty; }
	~NiDitherPropertyInit() { NiDitherProperty::defaultProperty = 0; }
};

// FUNCTION: STBC 0x007fa5f0 _$E216
// FUNCTION: STBC 0x007fa6b0 _$E214
static NiDitherPropertyInit init;

/*
================
NiDitherProperty::CreateClone

NiDitherProperty has no members of its own to copy.
================
*/
// FUNCTION: STBC 0x7FA6E0
NiObject *NiDitherProperty::CreateClone() {
	NiDitherProperty *clone = new NiDitherProperty;
	CopyMembers( clone );
	return clone;
}

/*
================
NiDitherProperty::CreateObject

Creates the property and loads it, with a record for NiObjectNET's link
IDs.
================
*/
// FUNCTION: STBC 0x7FA760
void NiDitherProperty::CreateObject( NiStream &stream, NiObject *&object ) {
	NiDitherProperty *dither = new NiDitherProperty;
	stream.SetLoadRecord( new NiDitherPropertyLoadRecord );
	dither->LoadBinary( stream );
	object = dither;
}

/*
================
NiDitherProperty::LoadBinary
================
*/
// FUNCTION: STBC 0x7FA830
void NiDitherProperty::LoadBinary( NiStream &stream ) {
	NiProperty::LoadBinary( stream );
}

/*
================
NiDitherProperty::LinkObject
================
*/
// FUNCTION: STBC 0x7FA840
void NiDitherProperty::LinkObject( NiStream &stream ) {
	NiProperty::LinkObject( stream );
}

/*
================
NiDitherProperty::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7FA850
bool NiDitherProperty::RegisterStreamables( NiStream &stream ) {
	return NiProperty::RegisterStreamables( stream );
}

/*
================
NiDitherProperty::SaveBinary
================
*/
// FUNCTION: STBC 0x7FA860
void NiDitherProperty::SaveBinary( NiStream &stream ) {
	NiProperty::SaveBinary( stream );
}

/*
================
NiDitherProperty::IsEqual
================
*/
// FUNCTION: STBC 0x7FA870
bool NiDitherProperty::IsEqual( NiObject *object ) {
	if ( !NiProperty::IsEqual( object ) ) {
		return false;
	}

	NiDitherProperty *dither = (NiDitherProperty *)object;
	if ( GetDithering() != dither->GetDithering() ) {
		return false;
	}
	return true;
}

/*
================
NiDitherProperty::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7FA8A0
void NiDitherProperty::GetViewerStrings( NiTArray<char *> *strings ) {
	NiProperty::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_bDither", GetDithering() ) );
}
