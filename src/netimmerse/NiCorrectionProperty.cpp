// NiCorrectionProperty.cpp -- gamma correction

#include "NiRTLib.h"
#include "NiCorrectionProperty.h"
#include "NiStream.h"
#include "NiAVObject.h"
#include "NiCollisionSwitch.h"
#include "NiDynamicEffect.h"
#include "NiNode.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007f9ab0 _$E214
// FUNCTION: STBC 0x007f9ad0 _$E17
// FUNCTION: STBC 0x007f9af0 _$E15
// FUNCTION: STBC 0x007f9b00 _$E23
// FUNCTION: STBC 0x007f9b20 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007f9b30 _$E30
// FUNCTION: STBC 0x007f9b40 _$E41
// FUNCTION: STBC 0x007f9b50 _$E45
// FUNCTION: STBC 0x007f9b60 _$E49
// FUNCTION: STBC 0x007f9b70 _$E53
// FUNCTION: STBC 0x007f9bc0 _$E58

// FUNCTION: STBC 0x007f9470 _$E196
// FUNCTION: STBC 0x007f9490 _$E193
// DATA: STBC 0x9A1ED0 ?rtti@NiCorrectionProperty@@2VNiRTTI@@B
const NiRTTI NiCorrectionProperty::rtti( "NiCorrectionProperty", &NiProperty::rtti );

/*
================
NiCorrectionProperty::Register
================
*/
// FUNCTION: STBC 0x7F94B0 ?Register@NiCorrectionProperty@@SAHXZ
NiImplementRegister( NiCorrectionProperty )

// FUNCTION: STBC 0x007f9600 _$E202
// FUNCTION: STBC 0x007f9620 _$E199
// DATA: STBC 0x9A1EEC
NiPointer<NiCorrectionProperty> NiCorrectionProperty::defaultProperty;

/*
===============================================================================

	NiCorrectionPropertyInit (name invented)

	A file-scope object whose constructor creates the default property and
	whose destructor releases it.

===============================================================================
*/
class NiCorrectionPropertyInit {
public:
	// FUNCTION: STBC 0x7F9680 ??0NiCorrectionPropertyInit@@QAE@XZ
	NiCorrectionPropertyInit() { NiCorrectionProperty::defaultProperty = new NiCorrectionProperty; }
	~NiCorrectionPropertyInit() { NiCorrectionProperty::defaultProperty = 0; }
};

// FUNCTION: STBC 0x007f9660 _$E207
// FUNCTION: STBC 0x007f9720 _$E205
// DATA: STBC 0x9A1ED8
static NiCorrectionPropertyInit init;

/*
================
NiCorrectionProperty::CreateClone

NiCorrectionProperty has no members of its own to copy.
================
*/
// FUNCTION: STBC 0x7F9750
NiObject *NiCorrectionProperty::CreateClone() {
	NiCorrectionProperty *clone = new NiCorrectionProperty;
	CopyMembers( clone );
	return clone;
}

/*
================
NiCorrectionProperty::CreateObject

Creates the property and loads it, with a record for NiObjectNET's link
IDs.
================
*/
// FUNCTION: STBC 0x7F97D0
void NiCorrectionProperty::CreateObject( NiStream &stream, NiObject *&object ) {
	NiCorrectionProperty *correction = new NiCorrectionProperty;
	stream.SetLoadRecord( new NiCorrectionPropertyLoadRecord );
	correction->LoadBinary( stream );
	object = correction;
}

/*
================
NiCorrectionProperty::LoadBinary
================
*/
// FUNCTION: STBC 0x7F98A0
void NiCorrectionProperty::LoadBinary( NiStream &stream ) {
	NiProperty::LoadBinary( stream );
}

/*
================
NiCorrectionProperty::LinkObject
================
*/
// FUNCTION: STBC 0x7F98B0
void NiCorrectionProperty::LinkObject( NiStream &stream ) {
	NiProperty::LinkObject( stream );
}

/*
================
NiCorrectionProperty::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7F98C0
bool NiCorrectionProperty::RegisterStreamables( NiStream &stream ) {
	return NiProperty::RegisterStreamables( stream );
}

/*
================
NiCorrectionProperty::SaveBinary
================
*/
// FUNCTION: STBC 0x7F98D0
void NiCorrectionProperty::SaveBinary( NiStream &stream ) {
	NiProperty::SaveBinary( stream );
}

/*
================
NiCorrectionProperty::IsEqual
================
*/
// FUNCTION: STBC 0x7F98E0
bool NiCorrectionProperty::IsEqual( NiObject *object ) {
	if ( !NiProperty::IsEqual( object ) ) {
		return false;
	}

	NiCorrectionProperty *correction = (NiCorrectionProperty *)object;
	if ( GetCorrect() != correction->GetCorrect() ) {
		return false;
	}
	return true;
}

/*
================
NiCorrectionProperty::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7F9910
void NiCorrectionProperty::GetViewerStrings( NiTArray<char *> *strings ) {
	NiProperty::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_bCorrect", GetCorrect() ) );
}
