// NiWireframeProperty.cpp -- wireframe rendering

#include "NiRTLib.h"
#include "NiWireframeProperty.h"
#include "NiStream.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00810d60 _$E82
// FUNCTION: STBC 0x00810d80 _$E17
// FUNCTION: STBC 0x00810da0 _$E15
// FUNCTION: STBC 0x00810db0 _$E23
// FUNCTION: STBC 0x00810dd0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00810de0 _$E30
// FUNCTION: STBC 0x00810df0 _$E41
// FUNCTION: STBC 0x00810e00 _$E45
// FUNCTION: STBC 0x00810e10 _$E49
// FUNCTION: STBC 0x00810e20 _$E53

// FUNCTION: STBC 0x00810720 _$E64
// FUNCTION: STBC 0x00810740 _$E61
// DATA: STBC 0x9A27A8 ?rtti@NiWireframeProperty@@2VNiRTTI@@B
const NiRTTI NiWireframeProperty::rtti( "NiWireframeProperty", &NiProperty::rtti );

/*
================
NiWireframeProperty::Register
================
*/
// FUNCTION: STBC 0x810760 ?Register@NiWireframeProperty@@SAHXZ
NiImplementRegister( NiWireframeProperty )

// FUNCTION: STBC 0x008108b0 _$E70
// FUNCTION: STBC 0x008108d0 _$E67
// DATA: STBC 0x9A27A4
NiPointer<NiWireframeProperty> NiWireframeProperty::defaultProperty;

/*
===============================================================================

	NiWireframePropertyInit (name invented)

	A file-scope object whose constructor creates the default property and
	whose destructor releases it.

===============================================================================
*/
class NiWireframePropertyInit {
public:
	// FUNCTION: STBC 0x810930 ??0NiWireframePropertyInit@@QAE@XZ
	NiWireframePropertyInit() { NiWireframeProperty::defaultProperty = new NiWireframeProperty; }
	~NiWireframePropertyInit() { NiWireframeProperty::defaultProperty = 0; }
};

// FUNCTION: STBC 0x00810910 _$E75
// FUNCTION: STBC 0x008109d0 _$E73
static NiWireframePropertyInit init;

/*
================
NiWireframeProperty::CreateClone

NiWireframeProperty has no members of its own to copy.
================
*/
// FUNCTION: STBC 0x810A00
NiObject *NiWireframeProperty::CreateClone() {
	NiWireframeProperty *clone = new NiWireframeProperty;
	CopyMembers( clone );
	return clone;
}

/*
================
NiWireframeProperty::CreateObject

Creates the property and loads it, with a record for NiObjectNET's link
IDs.
================
*/
// FUNCTION: STBC 0x810A80
void NiWireframeProperty::CreateObject( NiStream &stream, NiObject *&object ) {
	NiWireframeProperty *wireframe = new NiWireframeProperty;
	stream.SetLoadRecord( new NiWireframePropertyLoadRecord );
	wireframe->LoadBinary( stream );
	object = wireframe;
}

/*
================
NiWireframeProperty::LoadBinary
================
*/
// FUNCTION: STBC 0x810B50
void NiWireframeProperty::LoadBinary( NiStream &stream ) {
	NiProperty::LoadBinary( stream );
}

/*
================
NiWireframeProperty::LinkObject
================
*/
// FUNCTION: STBC 0x810B60
void NiWireframeProperty::LinkObject( NiStream &stream ) {
	NiProperty::LinkObject( stream );
}

/*
================
NiWireframeProperty::RegisterStreamables
================
*/
// FUNCTION: STBC 0x810B70
bool NiWireframeProperty::RegisterStreamables( NiStream &stream ) {
	return NiProperty::RegisterStreamables( stream );
}

/*
================
NiWireframeProperty::SaveBinary
================
*/
// FUNCTION: STBC 0x810B80
void NiWireframeProperty::SaveBinary( NiStream &stream ) {
	NiProperty::SaveBinary( stream );
}

/*
================
NiWireframeProperty::IsEqual
================
*/
// FUNCTION: STBC 0x810B90
bool NiWireframeProperty::IsEqual( NiObject *object ) {
	if ( !NiProperty::IsEqual( object ) ) {
		return false;
	}

	NiWireframeProperty *wireframe = (NiWireframeProperty *)object;
	if ( GetWireframe() != wireframe->GetWireframe() ) {
		return false;
	}
	return true;
}

/*
================
NiWireframeProperty::GetViewerStrings
================
*/
// FUNCTION: STBC 0x810BC0
void NiWireframeProperty::GetViewerStrings( NiTArray<char *> *strings ) {
	NiProperty::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_bWireframe", GetWireframe() ) );
}
