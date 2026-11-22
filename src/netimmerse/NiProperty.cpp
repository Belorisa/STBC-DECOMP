// NiProperty.cpp -- the base of the rendering properties

#include "NiRTLib.h"
#include <assert.h>
#include "NiProperty.h"
#include "NiStream.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007dbe60 _$E66
// FUNCTION: STBC 0x007dbe80 _$E17
// FUNCTION: STBC 0x007dbea0 _$E15
// FUNCTION: STBC 0x007dbeb0 _$E23
// FUNCTION: STBC 0x007dbed0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007dbee0 _$E30
// FUNCTION: STBC 0x007dbef0 _$E41
// FUNCTION: STBC 0x007dbf00 _$E45
// FUNCTION: STBC 0x007dbf10 _$E49
// FUNCTION: STBC 0x007dbf20 _$E53

// FUNCTION: STBC 0x007dbac0 _$E59
// FUNCTION: STBC 0x007dbae0 _$E56
// DATA: STBC 0x9A1528 ?rtti@NiProperty@@2VNiRTTI@@B
const NiRTTI NiProperty::rtti( "NiProperty", &NiObjectNET::rtti );

/*
================
NiProperty::Register
================
*/
// FUNCTION: STBC 0x7DBB00 ?Register@NiProperty@@SAHXZ
NiImplementRegister( NiProperty )

/*
================
NiProperty::Update

Runs the property's controllers.
================
*/
// FUNCTION: STBC 0x7DBC50
void NiProperty::Update( float time ) {
	for ( NiTimeController *controller = GetControllers(); controller; controller = controller->GetNext() ) {
		controller->Update( time );
	}
}

/*
================
NiProperty::CreateClone

NiProperty itself is never cloned; the derived classes create the clone.
================
*/
// FUNCTION: STBC 0x7DBC80
NiObject *NiProperty::CreateClone() {
	assert( false );
	return 0;
}

/*
================
NiProperty::CopyMembers
================
*/
// FUNCTION: STBC 0x7DBC90
void NiProperty::CopyMembers( NiProperty *dest ) {
	NiObjectNET::CopyMembers( dest );
	dest->flags = flags;
}

/*
================
NiProperty::VirtualSlot03
================
*/
// FUNCTION: STBC 0x7DBCB0
void NiProperty::VirtualSlot03() {
	NiObjectNET::VirtualSlot03();
}

/*
================
NiProperty::CreateObject

NiProperty is abstract: nothing to create.
================
*/
// FUNCTION: STBC 0x7DBCC0
void NiProperty::CreateObject( NiStream &stream, NiObject *&object ) {
}

/*
================
NiProperty::LoadBinary
================
*/
// FUNCTION: STBC 0x7DBCD0
void NiProperty::LoadBinary( NiStream &stream ) {
	NiObjectNET::LoadBinary( stream );
	NiStreamLoadBinary( stream, flags );
}

/*
================
NiProperty::LinkObject
================
*/
// FUNCTION: STBC 0x7DBD00
void NiProperty::LinkObject( NiStream &stream ) {
	NiObjectNET::LinkObject( stream );
}

/*
================
NiProperty::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7DBD10
bool NiProperty::RegisterStreamables( NiStream &stream ) {
	return NiObjectNET::RegisterStreamables( stream );
}

/*
================
NiProperty::SaveBinary
================
*/
// FUNCTION: STBC 0x7DBD20
void NiProperty::SaveBinary( NiStream &stream ) {
	NiObjectNET::SaveBinary( stream );
	NiStreamSaveBinary( stream, flags );
}

/*
================
NiProperty::IsEqual

The flags take no part in the comparison.
================
*/
// FUNCTION: STBC 0x7DBD50
bool NiProperty::IsEqual( NiObject *object ) {
	return NiObjectNET::IsEqual( object );
}

/*
================
NiProperty::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7DBD60
void NiProperty::GetViewerStrings( NiTArray<char *> *strings ) {
	NiObjectNET::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
}
