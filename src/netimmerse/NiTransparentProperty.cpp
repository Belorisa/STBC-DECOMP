// NiTransparentProperty.cpp -- colour-key transparency

#include "NiRTLib.h"
#include "NiTransparentProperty.h"
#include "NiStream.h"
#include "NiTextureProperty.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x0080b000 _$E107
// FUNCTION: STBC 0x0080b020 _$E17
// FUNCTION: STBC 0x0080b040 _$E15
// FUNCTION: STBC 0x0080b050 _$E23
// FUNCTION: STBC 0x0080b070 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0080b080 _$E30
// FUNCTION: STBC 0x0080b090 _$E41
// FUNCTION: STBC 0x0080b0a0 _$E45
// FUNCTION: STBC 0x0080b0b0 _$E49
// FUNCTION: STBC 0x0080b0c0 _$E53
// FUNCTION: STBC 0x0080b110 _$E58

// FUNCTION: STBC 0x0080a8e0 _$E89
// FUNCTION: STBC 0x0080a900 _$E86
// DATA: STBC 0x9A2508 ?rtti@NiTransparentProperty@@2VNiRTTI@@B
const NiRTTI NiTransparentProperty::rtti( "NiTransparentProperty", &NiProperty::rtti );

/*
================
NiTransparentProperty::Register
================
*/
// FUNCTION: STBC 0x80A920 ?Register@NiTransparentProperty@@SAHXZ
NiImplementRegister( NiTransparentProperty )

// FUNCTION: STBC 0x0080aa70 _$E95
// FUNCTION: STBC 0x0080aa90 _$E92
// DATA: STBC 0x9A2510
NiPointer<NiTransparentProperty> NiTransparentProperty::defaultProperty;

/*
===============================================================================

	NiTransparentPropertyInit (name invented)

	A file-scope object whose constructor creates the default property and
	whose destructor releases it.

===============================================================================
*/
class NiTransparentPropertyInit {
public:
	// FUNCTION: STBC 0x80AAF0 ??0NiTransparentPropertyInit@@QAE@XZ
	NiTransparentPropertyInit() { NiTransparentProperty::defaultProperty = new NiTransparentProperty; }
	~NiTransparentPropertyInit() { NiTransparentProperty::defaultProperty = 0; }
};

// FUNCTION: STBC 0x0080aad0 _$E100
// FUNCTION: STBC 0x0080ab90 _$E98
static NiTransparentPropertyInit init;

/*
================
NiTransparentProperty::CreateClone

================
*/
// FUNCTION: STBC 0x80ABC0
NiObject *NiTransparentProperty::CreateClone() {
	NiTransparentProperty *clone = new NiTransparentProperty;
	CopyMembers( clone );
	return clone;
}

/*
================
NiTransparentProperty::CopyMembers
================
*/
// FUNCTION: STBC 0x80AC40
void NiTransparentProperty::CopyMembers( NiTransparentProperty *dest ) {
	NiProperty::CopyMembers( dest );
	dest->color = color;
}

/*
================
NiTransparentProperty::CreateObject

Creates the property and loads it, with a record for NiObjectNET's link
IDs.
================
*/
// FUNCTION: STBC 0x80AC60
void NiTransparentProperty::CreateObject( NiStream &stream, NiObject *&object ) {
	NiTransparentProperty *transparent = new NiTransparentProperty;
	stream.SetLoadRecord( new NiTransparentPropertyLoadRecord );
	transparent->LoadBinary( stream );
	object = transparent;
}

/*
================
NiTransparentProperty::LoadBinary
================
*/
// FUNCTION: STBC 0x80AD30
void NiTransparentProperty::LoadBinary( NiStream &stream ) {
	NiProperty::LoadBinary( stream );
	NiStreamLoadBinary( stream, color );
}

/*
================
NiTransparentProperty::LinkObject
================
*/
// FUNCTION: STBC 0x80AD60
void NiTransparentProperty::LinkObject( NiStream &stream ) {
	NiProperty::LinkObject( stream );
}

/*
================
NiTransparentProperty::RegisterStreamables
================
*/
// FUNCTION: STBC 0x80AD70
bool NiTransparentProperty::RegisterStreamables( NiStream &stream ) {
	return NiProperty::RegisterStreamables( stream );
}

/*
================
NiTransparentProperty::SaveBinary
================
*/
// FUNCTION: STBC 0x80AD80
void NiTransparentProperty::SaveBinary( NiStream &stream ) {
	NiProperty::SaveBinary( stream );
	NiStreamSaveBinary( stream, color );
}

/*
================
NiTransparentProperty::IsEqual
================
*/
// FUNCTION: STBC 0x80ADB0
bool NiTransparentProperty::IsEqual( NiObject *object ) {
	if ( !NiProperty::IsEqual( object ) ) {
		return false;
	}

	NiTransparentProperty *transparent = (NiTransparentProperty *)object;
	if ( GetTransparent() != transparent->GetTransparent()
		|| color != transparent->color ) {
		return false;
	}
	return true;
}

/*
================
NiTransparentProperty::GetViewerStrings
================
*/
// FUNCTION: STBC 0x80ADF0
// PARKED: inline budget; retail inlines the third Add's SetAt too, this build calls it out of line
void NiTransparentProperty::GetViewerStrings( NiTArray<char *> *strings ) {
	NiProperty::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_bTransparent", GetTransparent() ) );
	strings->Add( NiGetViewerString( "m_uiColor", color ) );
}
