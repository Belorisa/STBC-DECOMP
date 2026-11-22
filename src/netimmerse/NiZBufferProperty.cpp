// NiZBufferProperty.cpp -- the depth test and depth writes

#include "NiRTLib.h"
#include "NiZBufferProperty.h"
#include "NiStream.h"
#include "NiWireframeProperty.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x008114c0 _$E87
// FUNCTION: STBC 0x008114e0 _$E17
// FUNCTION: STBC 0x00811500 _$E15
// FUNCTION: STBC 0x00811510 _$E23
// FUNCTION: STBC 0x00811530 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00810e30 _$E63
// FUNCTION: STBC 0x00811540 _$E30
// FUNCTION: STBC 0x00811550 _$E41
// FUNCTION: STBC 0x00811560 _$E45
// FUNCTION: STBC 0x00811570 _$E49
// FUNCTION: STBC 0x00811580 _$E53
// FUNCTION: STBC 0x00811590 _$E58

// FUNCTION: STBC 0x00810e40 _$E69
// FUNCTION: STBC 0x00810e60 _$E66
// DATA: STBC 0x9A27E8 ?rtti@NiZBufferProperty@@2VNiRTTI@@B
const NiRTTI NiZBufferProperty::rtti( "NiZBufferProperty", &NiProperty::rtti );

/*
================
NiZBufferProperty::Register
================
*/
// FUNCTION: STBC 0x810E80 ?Register@NiZBufferProperty@@SAHXZ
NiImplementRegister( NiZBufferProperty )

// FUNCTION: STBC 0x00810fd0 _$E75
// FUNCTION: STBC 0x00810ff0 _$E72
// DATA: STBC 0x9A27D4
NiPointer<NiZBufferProperty> NiZBufferProperty::defaultProperty;

/*
===============================================================================

	NiZBufferPropertyInit (name invented)

	A file-scope object whose constructor creates the default property and
	whose destructor releases it.

===============================================================================
*/
class NiZBufferPropertyInit {
public:
	// FUNCTION: STBC 0x811050 ??0NiZBufferPropertyInit@@QAE@XZ
	NiZBufferPropertyInit() { NiZBufferProperty::defaultProperty = new NiZBufferProperty; }
	~NiZBufferPropertyInit() { NiZBufferProperty::defaultProperty = 0; }
};

// FUNCTION: STBC 0x00811030 _$E80
// FUNCTION: STBC 0x008110f0 _$E78
static NiZBufferPropertyInit init;

/*
================
NiZBufferProperty::CreateClone

NiZBufferProperty has no members of its own to copy.
================
*/
// FUNCTION: STBC 0x811120
NiObject *NiZBufferProperty::CreateClone() {
	NiZBufferProperty *clone = new NiZBufferProperty;
	CopyMembers( clone );
	return clone;
}

/*
================
NiZBufferProperty::CreateObject

Creates the property and loads it, with a record for NiObjectNET's link
IDs.
================
*/
// FUNCTION: STBC 0x8111A0
void NiZBufferProperty::CreateObject( NiStream &stream, NiObject *&object ) {
	NiZBufferProperty *zbuffer = new NiZBufferProperty;
	stream.SetLoadRecord( new NiZBufferPropertyLoadRecord );
	zbuffer->LoadBinary( stream );
	object = zbuffer;
}

/*
================
NiZBufferProperty::LoadBinary
================
*/
// FUNCTION: STBC 0x811270
void NiZBufferProperty::LoadBinary( NiStream &stream ) {
	NiProperty::LoadBinary( stream );
}

/*
================
NiZBufferProperty::LinkObject
================
*/
// FUNCTION: STBC 0x811280
void NiZBufferProperty::LinkObject( NiStream &stream ) {
	NiProperty::LinkObject( stream );
}

/*
================
NiZBufferProperty::RegisterStreamables
================
*/
// FUNCTION: STBC 0x811290
bool NiZBufferProperty::RegisterStreamables( NiStream &stream ) {
	return NiProperty::RegisterStreamables( stream );
}

/*
================
NiZBufferProperty::SaveBinary
================
*/
// FUNCTION: STBC 0x8112A0
void NiZBufferProperty::SaveBinary( NiStream &stream ) {
	NiProperty::SaveBinary( stream );
}

/*
================
NiZBufferProperty::IsEqual
================
*/
// FUNCTION: STBC 0x8112B0
bool NiZBufferProperty::IsEqual( NiObject *object ) {
	if ( !NiProperty::IsEqual( object ) ) {
		return false;
	}

	NiZBufferProperty *zbuffer = (NiZBufferProperty *)object;
	if ( GetZBufferTest() != zbuffer->GetZBufferTest()
		|| GetZBufferWrite() != zbuffer->GetZBufferWrite() ) {
		return false;
	}
	return true;
}

/*
================
NiZBufferProperty::GetViewerStrings
================
*/
// FUNCTION: STBC 0x8112F0
void NiZBufferProperty::GetViewerStrings( NiTArray<char *> *strings ) {
	NiProperty::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_bZTest", GetZBufferTest() ) );
	strings->Add( NiGetViewerString( "m_bZWrite", GetZBufferWrite() ) );
}
