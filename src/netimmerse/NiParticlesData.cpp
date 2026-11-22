// NiParticlesData.cpp -- particle data

#include "NiRTLib.h"
#include <assert.h>
#include "NiParticlesData.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007f4cc0 _$E77
// FUNCTION: STBC 0x007f4ce0 _$E17
// FUNCTION: STBC 0x007f4d00 _$E15
// FUNCTION: STBC 0x007f4d10 _$E23
// FUNCTION: STBC 0x007f4d30 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007f4d40 _$E41
// FUNCTION: STBC 0x007f4d50 _$E49
// FUNCTION: STBC 0x007f4d60 _$E53
// FUNCTION: STBC 0x007f4da0 _$E64

// FUNCTION: STBC 0x007f4560 _$E70
// FUNCTION: STBC 0x007f4580 _$E67
// DATA: STBC 0x9A1C80 ?rtti@NiParticlesData@@2VNiRTTI@@B
const NiRTTI NiParticlesData::rtti( "NiParticlesData", &NiTriBasedGeomData::rtti );

/*
================
NiParticlesData::Register
================
*/
// FUNCTION: STBC 0x7F45A0 ?Register@NiParticlesData@@SAHXZ
NiImplementRegister( NiParticlesData )

/*
================
NiParticlesData::NiParticlesData

Takes ownership of the arrays. Particles have no texture coordinates; the
base's triangle count is the particle count.
================
*/
// FUNCTION: STBC 0x7F46F0 ??0NiParticlesData@@QAE@GPAVNiPoint3@@0PAVNiColorA@@@Z
NiParticlesData::NiParticlesData( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_ )
	: NiTriBasedGeomData( vertexCount_, vertices_, normals_, colors_, 0, vertexCount_ ) {
	activeVertexCount = vertexCount_;
	radius = 1.0f;
	sizes = new float[vertexCount_];
	for ( unsigned int i = 0; i < vertexCount_; i++ ) {
		sizes[i] = 1.0f;
	}
}

/*
================
NiParticlesData::NiParticlesData
================
*/
// FUNCTION: STBC 0x7F4790 ??0NiParticlesData@@QAE@XZ
NiParticlesData::NiParticlesData() {
	radius = 1.0f;
	activeVertexCount = 0;
	sizes = 0;
}

/*
================
NiParticlesData::~NiParticlesData
================
*/
// FUNCTION: STBC 0x7F47C0 ??1NiParticlesData@@UAE@XZ
// FUNCTION: STBC 0x7F4D80 ??_GNiParticlesData@@UAEPAXI@Z
NiParticlesData::~NiParticlesData() {
	delete [] sizes;
}

/*
================
NiParticlesData::Replace
================
*/
// FUNCTION: STBC 0x7F47E0
void NiParticlesData::Replace( float *sizes_ ) {
	delete [] sizes;
	sizes = sizes_;
}

/*
================
NiParticlesData::SetData
================
*/
// FUNCTION: STBC 0x7F4800
void NiParticlesData::SetData( float *sizes_ ) {
	sizes = sizes_;
}

/*
================
NiParticlesData::CreateClone

The data is shared, not copied: the clone is the data itself.
================
*/
// FUNCTION: STBC 0x7F4810
NiObject *NiParticlesData::CreateClone() {
	return this;
}

/*
================
NiParticlesData::CopyMembers
================
*/
// FUNCTION: STBC 0x7F4820
void NiParticlesData::CopyMembers( NiParticlesData *dest ) {
	assert( false );
}

/*
================
NiParticlesData::CreateObject

Creates the data and loads it.
================
*/
// FUNCTION: STBC 0x7F4830
void NiParticlesData::CreateObject( NiStream &stream, NiObject *&object ) {
	NiParticlesData *data = new NiParticlesData;
	stream.SetLoadRecord( new NiParticlesDataLoadRecord );
	data->LoadBinary( stream );
	object = data;
}

/*
================
NiParticlesData::LoadBinary

The sizes pointer is saved as a flag: the sizes follow only when it was
set, and are all 1 otherwise.
================
*/
// FUNCTION: STBC 0x7F4900
void NiParticlesData::LoadBinary( NiStream &stream ) {
	NiTriBasedGeomData::LoadBinary( stream );

	NiStreamLoadBinary( stream, radius );
	NiStreamLoadBinary( stream, activeVertexCount );
	NiStreamLoadBinary( stream, sizes );
	if ( sizes ) {
		sizes = new float[vertexCount];
		NiStreamLoadBinary( stream, sizes, vertexCount );
	} else {
		sizes = new float[vertexCount];
		for ( unsigned short i = 0; i < vertexCount; i++ ) {
			sizes[i] = 1.0f;
		}
	}
}

/*
================
NiParticlesData::LinkObject
================
*/
// FUNCTION: STBC 0x7F49B0
void NiParticlesData::LinkObject( NiStream &stream ) {
	NiTriBasedGeomData::LinkObject( stream );
}

/*
================
NiParticlesData::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7F49C0
bool NiParticlesData::RegisterStreamables( NiStream &stream ) {
	return NiTriBasedGeomData::RegisterStreamables( stream );
}

/*
================
NiParticlesData::SaveBinary
================
*/
// FUNCTION: STBC 0x7F49D0
void NiParticlesData::SaveBinary( NiStream &stream ) {
	NiTriBasedGeomData::SaveBinary( stream );

	NiStreamSaveBinary( stream, radius );
	NiStreamSaveBinary( stream, activeVertexCount );
	NiStreamSaveBinary( stream, sizes );
	if ( sizes ) {
		NiStreamSaveBinary( stream, sizes, vertexCount );
	}
}

/*
================
NiParticlesData::IsEqual
================
*/
// FUNCTION: STBC 0x7F4A30
bool NiParticlesData::IsEqual( NiObject *object ) {
	if ( !NiTriBasedGeomData::IsEqual( object ) ) {
		return false;
	}

	NiParticlesData *data = (NiParticlesData *)object;
	if ( radius != data->radius ) {
		return false;
	}
	if ( activeVertexCount != data->activeVertexCount ) {
		return false;
	}
	if ( ( sizes && !data->sizes ) || ( !sizes && data->sizes ) ) {
		return false;
	}
	if ( sizes ) {
		for ( unsigned short i = 0; i < vertexCount; i++ ) {
			if ( sizes[i] != data->sizes[i] ) {
				return false;
			}
		}
	}
	return true;
}

/*
================
NiParticlesData::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7F4AD0
void NiParticlesData::GetViewerStrings( NiTArray<char *> *strings ) {
	NiTriBasedGeomData::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_fRadius", radius ) );
	strings->Add( NiGetViewerString( "m_usActive", activeVertexCount ) );
	strings->Add( NiGetViewerString( "m_pSizes", sizes ) );
}

/*
================
NiParticlesData::CalculateNormals

Particles have no normals to build.
================
*/
// FUNCTION: STBC 0x7F4DB0
void NiParticlesData::CalculateNormals() {
}
