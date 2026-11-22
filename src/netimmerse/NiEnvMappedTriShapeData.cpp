// NiEnvMappedTriShapeData.cpp -- environment-mapped triangle list geometry data

#include "NiRTLib.h"
#include <assert.h>
#include "NiEnvMappedTriShapeData.h"
#include "NiDitherProperty.h"
#include "NiExtraData.h"
#include "NiObjectNET.h"
#include "NiProperty.h"
#include "NiTimeController.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007faf80 _$E102
// FUNCTION: STBC 0x007fafa0 _$E17
// FUNCTION: STBC 0x007fafc0 _$E15
// FUNCTION: STBC 0x007fafd0 _$E23
// FUNCTION: STBC 0x007faff0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007fb000 _$E48
// FUNCTION: STBC 0x007fb010 _$E56
// FUNCTION: STBC 0x007fb020 _$E60
// FUNCTION: STBC 0x007fb030 _$E64
// FUNCTION: STBC 0x007fb080 _$E68

// FUNCTION: STBC 0x007fab20 _$E95
// FUNCTION: STBC 0x007fab40 _$E92
// DATA: STBC 0x9A1F88 ?rtti@NiEnvMappedTriShapeData@@2VNiRTTI@@B
const NiRTTI NiEnvMappedTriShapeData::rtti( "NiEnvMappedTriShapeData", &NiTriShapeData::rtti );

/*
================
NiEnvMappedTriShapeData::Register
================
*/
// FUNCTION: STBC 0x7FAB60 ?Register@NiEnvMappedTriShapeData@@SAHXZ
NiImplementRegister( NiEnvMappedTriShapeData )

/*
================
NiEnvMappedTriShapeData::NiEnvMappedTriShapeData

Takes ownership of the arrays, and creates the one set of texture
coordinates the shape generates.
================
*/
// FUNCTION: STBC 0x7FACB0 ??0NiEnvMappedTriShapeData@@QAE@GPAVNiPoint3@@0PAVNiColorA@@GPAG@Z
NiEnvMappedTriShapeData::NiEnvMappedTriShapeData( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, unsigned short triangleCount_, unsigned short *triList_ )
	: NiTriShapeData( vertexCount_, vertices_, normals_, colors_, 0, triangleCount_, triList_ ) {
	CreateTextures( false, 1 );
}

/*
================
NiEnvMappedTriShapeData::NiEnvMappedTriShapeData
================
*/
// FUNCTION: STBC 0x7FAD30 ??0NiEnvMappedTriShapeData@@QAE@XZ
NiEnvMappedTriShapeData::NiEnvMappedTriShapeData() {
}

/*
================
NiEnvMappedTriShapeData::CreateClone

The data is shared, not copied: the clone is the data itself.
================
*/
// FUNCTION: STBC 0x7FAD50
NiObject *NiEnvMappedTriShapeData::CreateClone() {
	return this;
}

/*
================
NiEnvMappedTriShapeData::CopyMembers
================
*/
// FUNCTION: STBC 0x7FAD60
void NiEnvMappedTriShapeData::CopyMembers( NiEnvMappedTriShapeData *dest ) {
	assert( false );
}

/*
================
NiEnvMappedTriShapeData::CreateObject

Creates the data and loads it.
================
*/
// FUNCTION: STBC 0x7FAD70
void NiEnvMappedTriShapeData::CreateObject( NiStream &stream, NiObject *&object ) {
	NiEnvMappedTriShapeData *data = new NiEnvMappedTriShapeData;
	stream.SetLoadRecord( new NiEnvMappedTriShapeDataLoadRecord );
	data->LoadBinary( stream );
	object = data;
}

/*
================
NiEnvMappedTriShapeData::LoadBinary
================
*/
// FUNCTION: STBC 0x7FAE30
void NiEnvMappedTriShapeData::LoadBinary( NiStream &stream ) {
	NiTriShapeData::LoadBinary( stream );
}

/*
================
NiEnvMappedTriShapeData::LinkObject
================
*/
// FUNCTION: STBC 0x7FAE40
void NiEnvMappedTriShapeData::LinkObject( NiStream &stream ) {
	NiTriShapeData::LinkObject( stream );
}

/*
================
NiEnvMappedTriShapeData::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7FAE50
bool NiEnvMappedTriShapeData::RegisterStreamables( NiStream &stream ) {
	return NiTriShapeData::RegisterStreamables( stream );
}

/*
================
NiEnvMappedTriShapeData::SaveBinary
================
*/
// FUNCTION: STBC 0x7FAE60
void NiEnvMappedTriShapeData::SaveBinary( NiStream &stream ) {
	NiTriShapeData::SaveBinary( stream );
}

/*
================
NiEnvMappedTriShapeData::IsEqual
================
*/
// FUNCTION: STBC 0x7FAE70
bool NiEnvMappedTriShapeData::IsEqual( NiObject *object ) {
	return NiTriShapeData::IsEqual( object );
}

/*
================
NiEnvMappedTriShapeData::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7FAE80
void NiEnvMappedTriShapeData::GetViewerStrings( NiTArray<char *> *strings ) {
	NiTriShapeData::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
}
