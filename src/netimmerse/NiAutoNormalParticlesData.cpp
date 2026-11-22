// NiAutoNormalParticlesData.cpp -- particle data with camera-facing normals

#include "NiRTLib.h"
#include <assert.h>
#include "NiAutoNormalParticlesData.h"
#include "NiParticles.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007f5be0 _$E225
// FUNCTION: STBC 0x007f5b80 _$E17
// FUNCTION: STBC 0x007f5ba0 _$E15
// FUNCTION: STBC 0x007f5bb0 _$E23
// FUNCTION: STBC 0x007f5bd0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007f5c00 _$E41
// FUNCTION: STBC 0x007f5c10 _$E72
// FUNCTION: STBC 0x007f5c20 _$E76
// FUNCTION: STBC 0x007f5c30 _$E80
// FUNCTION: STBC 0x007f5c40 _$E84
// FUNCTION: STBC 0x007f5c50 _$E197
// FUNCTION: STBC 0x007f5c60 _$E49
// FUNCTION: STBC 0x007f5c70 _$E204
// FUNCTION: STBC 0x007f5c80 _$E53
// FUNCTION: STBC 0x007f5c90 _$E208
// FUNCTION: STBC 0x007f5ca0 _$E64
// FUNCTION: STBC 0x007f5cb0 _$E212
// FUNCTION: STBC 0x007f5d00 _$E68

// FUNCTION: STBC 0x007f5740 _$E218
// FUNCTION: STBC 0x007f5760 _$E215
// DATA: STBC 0x9A1D18 ?rtti@NiAutoNormalParticlesData@@2VNiRTTI@@B
const NiRTTI NiAutoNormalParticlesData::rtti( "NiAutoNormalParticlesData", &NiParticlesData::rtti );

/*
================
NiAutoNormalParticlesData::Register
================
*/
// FUNCTION: STBC 0x7F5780 ?Register@NiAutoNormalParticlesData@@SAHXZ
NiImplementRegister( NiAutoNormalParticlesData )

/*
================
NiAutoNormalParticlesData::NiAutoNormalParticlesData

Takes ownership of the arrays, and creates the normals the shape sets.
================
*/
// FUNCTION: STBC 0x7F58D0 ??0NiAutoNormalParticlesData@@QAE@GPAVNiPoint3@@PAVNiColorA@@@Z
NiAutoNormalParticlesData::NiAutoNormalParticlesData( unsigned short vertexCount_, NiPoint3 *vertices_, NiColorA *colors_ )
	: NiParticlesData( vertexCount_, vertices_, 0, colors_ ) {
	CreateNormals( false );
}

/*
================
NiAutoNormalParticlesData::NiAutoNormalParticlesData
================
*/
// FUNCTION: STBC 0x7F5930 ??0NiAutoNormalParticlesData@@QAE@XZ
NiAutoNormalParticlesData::NiAutoNormalParticlesData() {
}

/*
================
NiAutoNormalParticlesData::CreateClone

The data is shared, not copied: the clone is the data itself.
================
*/
// FUNCTION: STBC 0x7F5950
NiObject *NiAutoNormalParticlesData::CreateClone() {
	return this;
}

/*
================
NiAutoNormalParticlesData::CopyMembers
================
*/
// FUNCTION: STBC 0x7F5960
void NiAutoNormalParticlesData::CopyMembers( NiAutoNormalParticlesData *dest ) {
	assert( false );
}

/*
================
NiAutoNormalParticlesData::CreateObject

Creates the data and loads it.
================
*/
// FUNCTION: STBC 0x7F5970
void NiAutoNormalParticlesData::CreateObject( NiStream &stream, NiObject *&object ) {
	NiAutoNormalParticlesData *data = new NiAutoNormalParticlesData;
	stream.SetLoadRecord( new NiAutoNormalParticlesDataLoadRecord );
	data->LoadBinary( stream );
	object = data;
}

/*
================
NiAutoNormalParticlesData::LoadBinary
================
*/
// FUNCTION: STBC 0x7F5A30
void NiAutoNormalParticlesData::LoadBinary( NiStream &stream ) {
	NiParticlesData::LoadBinary( stream );
}

/*
================
NiAutoNormalParticlesData::LinkObject
================
*/
// FUNCTION: STBC 0x7F5A40
void NiAutoNormalParticlesData::LinkObject( NiStream &stream ) {
	NiParticlesData::LinkObject( stream );
}

/*
================
NiAutoNormalParticlesData::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7F5A50
bool NiAutoNormalParticlesData::RegisterStreamables( NiStream &stream ) {
	return NiParticlesData::RegisterStreamables( stream );
}

/*
================
NiAutoNormalParticlesData::SaveBinary
================
*/
// FUNCTION: STBC 0x7F5A60
void NiAutoNormalParticlesData::SaveBinary( NiStream &stream ) {
	NiParticlesData::SaveBinary( stream );
}

/*
================
NiAutoNormalParticlesData::IsEqual
================
*/
// FUNCTION: STBC 0x7F5A70
bool NiAutoNormalParticlesData::IsEqual( NiObject *object ) {
	return NiParticlesData::IsEqual( object );
}

/*
================
NiAutoNormalParticlesData::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7F5A80
void NiAutoNormalParticlesData::GetViewerStrings( NiTArray<char *> *strings ) {
	NiParticlesData::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
}
