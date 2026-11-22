// NiStringExtraData.cpp -- string extra data

#include "NiRTLib.h"
#include <string.h>

#include "NiStream.h"
#include "NiStringExtraData.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00808920 _$E58
// FUNCTION: STBC 0x00808940 _$E17
// FUNCTION: STBC 0x00808960 _$E15
// FUNCTION: STBC 0x00808970 _$E23
// FUNCTION: STBC 0x00808990 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x008089a0 _$E37
// FUNCTION: STBC 0x008089b0 _$E41
// FUNCTION: STBC 0x008089f0 _$E45

// FUNCTION: STBC 0x008081b0 _$E51
// FUNCTION: STBC 0x008081d0 _$E48
// DATA: STBC 0x9A2458 ?rtti@NiStringExtraData@@2VNiRTTI@@B
const NiRTTI NiStringExtraData::rtti( "NiStringExtraData", &NiExtraData::rtti );

/*
================
NiStringExtraData::Register
================
*/
// FUNCTION: STBC 0x8081F0 ?Register@NiStringExtraData@@SAHXZ
NiImplementRegister( NiStringExtraData )

/*
================
NiStringExtraData::NiStringExtraData
================
*/
// FUNCTION: STBC 0x808340 ??0NiStringExtraData@@QAE@PBD@Z
NiStringExtraData::NiStringExtraData( const char *string_ ) {
	string = 0;
	SetValue( string_ );
}

/*
================
NiStringExtraData::SetValue

The size is the string's size in a file: its length, then the
characters.
================
*/
// FUNCTION: STBC 0x8083F0
void NiStringExtraData::SetValue( const char *string_ ) {
	delete [] string;
	if ( string_ ) {
		string = new char[strlen( string_ ) + 1];
		strcpy( string, string_ );
		size = strlen( string ) + sizeof( int );
	} else {
		string = 0;
		size = 0;
	}
}

/*
================
NiStringExtraData::CreateClone
================
*/
// FUNCTION: STBC 0x808470
NiObject *NiStringExtraData::CreateClone() {
	NiStringExtraData *clone = new NiStringExtraData;
	CopyMembers( clone );
	return clone;
}

/*
================
NiStringExtraData::CopyMembers
================
*/
// FUNCTION: STBC 0x808540
void NiStringExtraData::CopyMembers( NiStringExtraData *dest ) {
	NiExtraData::CopyMembers( dest );
	if ( string ) {
		dest->string = new char[strlen( string ) + 1];
		strcpy( dest->string, string );
	} else {
		dest->string = 0;
	}
}

/*
================
NiStringExtraData::CreateObject

Creates the data and loads it.
================
*/
// FUNCTION: STBC 0x8085A0
void NiStringExtraData::CreateObject( NiStream &stream, NiObject *&object ) {
	NiStringExtraData *data = new NiStringExtraData;
	stream.SetLoadRecord( new NiStringExtraDataLoadRecord );
	data->LoadBinary( stream );
	object = data;
}

/*
================
NiStringExtraData::LoadBinary
================
*/
// FUNCTION: STBC 0x808660
void NiStringExtraData::LoadBinary( NiStream &stream ) {
	NiExtraData::LoadBinary( stream );
	stream.LoadCString( string );
}

/*
================
NiStringExtraData::LinkObject
================
*/
// FUNCTION: STBC 0x808680
void NiStringExtraData::LinkObject( NiStream &stream ) {
	NiExtraData::LinkObject( stream );
}

/*
================
NiStringExtraData::RegisterStreamables
================
*/
// FUNCTION: STBC 0x808690
bool NiStringExtraData::RegisterStreamables( NiStream &stream ) {
	return NiExtraData::RegisterStreamables( stream );
}

/*
================
NiStringExtraData::SaveBinary
================
*/
// FUNCTION: STBC 0x8086A0
void NiStringExtraData::SaveBinary( NiStream &stream ) {
	NiExtraData::SaveBinary( stream );
	stream.SaveCString( string );
}

/*
================
NiStringExtraData::IsEqual

Compares the sizes and the strings; unlike its siblings it does not ask
NiExtraData's IsEqual first.
================
*/
// FUNCTION: STBC 0x8086C0
bool NiStringExtraData::IsEqual( NiObject *object ) {
	if ( !object ) {
		return false;
	}
	NiStringExtraData *other = (NiStringExtraData *)object;
	if ( size != other->size ) {
		return false;
	}
	if ( ( string && !other->string ) || ( !string && other->string ) ) {
		return false;
	}
	if ( string && strcmp( string, other->string ) ) {
		return false;
	}
	return true;
}

/*
================
NiStringExtraData::GetViewerStrings
================
*/
// FUNCTION: STBC 0x808750
void NiStringExtraData::GetViewerStrings( NiTArray<char *> *strings ) {
	NiExtraData::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_pString", string ) );
}

/*
================
NiStringExtraData::~NiStringExtraData
================
*/
// FUNCTION: STBC 0x808A00
NiStringExtraData::~NiStringExtraData() {
	delete [] string;
}
