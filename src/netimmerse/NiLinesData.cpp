// NiLinesData.cpp -- line geometry data

#include "NiRTLib.h"
#include <assert.h>
#include "NiLinesData.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007fe860 _$E73
// FUNCTION: STBC 0x007fe880 _$E17
// FUNCTION: STBC 0x007fe8a0 _$E15
// FUNCTION: STBC 0x007fe8b0 _$E23
// FUNCTION: STBC 0x007fe8d0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007fe8e0 _$E41
// FUNCTION: STBC 0x007fe8f0 _$E49
// FUNCTION: STBC 0x007fe930 _$E60

// FUNCTION: STBC 0x007fe1f0 _$E66
// FUNCTION: STBC 0x007fe210 _$E63
// DATA: STBC 0x9A2100 ?rtti@NiLinesData@@2VNiRTTI@@B
const NiRTTI NiLinesData::rtti( "NiLinesData", &NiGeometryData::rtti );

/*
================
NiLinesData::Register
================
*/
// FUNCTION: STBC 0x7FE230 ?Register@NiLinesData@@SAHXZ
NiImplementRegister( NiLinesData )

/*
================
NiLinesData::NiLinesData

Takes ownership of the arrays. Lines have no normals. Without flags, the
vertices pair up: each even vertex connects to the next.
================
*/
// FUNCTION: STBC 0x7FE380 ??0NiLinesData@@QAE@GPAVNiPoint3@@PAVNiColorA@@PAVNiPoint2@@PA_N@Z
NiLinesData::NiLinesData( unsigned short vertexCount_, NiPoint3 *vertices_, NiColorA *colors_, NiPoint2 *textures_, bool *flags_ )
	: NiGeometryData( vertexCount_, vertices_, 0, colors_, textures_ ) {
	if ( flags_ ) {
		flags = flags_;
	} else {
		flags = new bool[vertexCount_];
		for ( unsigned short i = 0; i < vertexCount_; i++ ) {
			flags[i] = ( i & 1 ) == 0;
		}
	}
}

/*
================
NiLinesData::NiLinesData
================
*/
// FUNCTION: STBC 0x7FE430 ??0NiLinesData@@QAE@XZ
NiLinesData::NiLinesData() {
	flags = 0;
}

/*
================
NiLinesData::~NiLinesData
================
*/
// FUNCTION: STBC 0x7FE450 ??1NiLinesData@@UAE@XZ
// FUNCTION: STBC 0x7FE910 ??_GNiLinesData@@UAEPAXI@Z
NiLinesData::~NiLinesData() {
	delete [] flags;
}

/*
================
NiLinesData::Replace
================
*/
// FUNCTION: STBC 0x7FE470
void NiLinesData::Replace( bool *flags_ ) {
	delete [] flags;
	flags = flags_;
}

/*
================
NiLinesData::SetData
================
*/
// FUNCTION: STBC 0x7FE490
void NiLinesData::SetData( bool *flags_ ) {
	flags = flags_;
}

/*
================
NiLinesData::CreateClone

The data is shared, not copied: the clone is the data itself.
================
*/
// FUNCTION: STBC 0x7FE4A0
NiObject *NiLinesData::CreateClone() {
	return this;
}

/*
================
NiLinesData::CopyMembers
================
*/
// FUNCTION: STBC 0x7FE4B0
void NiLinesData::CopyMembers( NiLinesData *dest ) {
	assert( false );
}

/*
================
NiLinesData::CreateObject

Creates the data and loads it.
================
*/
// FUNCTION: STBC 0x7FE4C0
void NiLinesData::CreateObject( NiStream &stream, NiObject *&object ) {
	NiLinesData *data = new NiLinesData;
	stream.SetLoadRecord( new NiLinesDataLoadRecord );
	data->LoadBinary( stream );
	object = data;
}

/*
================
NiLinesData::LoadBinary

NiGeometryData's members, then the flags. The original reads the flags
into the pointer itself (four bytes over the new array's address) rather
than into the array SaveBinary writes; the bug is kept.
================
*/
// FUNCTION: STBC 0x7FE590
void NiLinesData::LoadBinary( NiStream &stream ) {
	NiGeometryData::LoadBinary( stream );

	flags = new bool[vertexCount];
	assert( flags );
	NiStreamLoadBinary( stream, flags );
}

/*
================
NiLinesData::LinkObject
================
*/
// FUNCTION: STBC 0x7FE5D0
void NiLinesData::LinkObject( NiStream &stream ) {
	NiGeometryData::LinkObject( stream );
}

/*
================
NiLinesData::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7FE5E0
bool NiLinesData::RegisterStreamables( NiStream &stream ) {
	return NiGeometryData::RegisterStreamables( stream );
}

/*
================
NiLinesData::SaveBinary
================
*/
// FUNCTION: STBC 0x7FE5F0
void NiLinesData::SaveBinary( NiStream &stream ) {
	NiGeometryData::SaveBinary( stream );
	assert( flags );
	NiStreamSaveBinary( stream, flags, vertexCount );
}

/*
================
NiLinesData::IsEqual
================
*/
// FUNCTION: STBC 0x7FE620
bool NiLinesData::IsEqual( NiObject *object ) {
	if ( !NiGeometryData::IsEqual( object ) ) {
		return false;
	}

	NiLinesData *data = (NiLinesData *)object;
	if ( ( flags && !data->flags ) || ( !flags && data->flags ) ) {
		return false;
	}
	if ( flags ) {
		for ( unsigned short i = 0; i < vertexCount; i++ ) {
			if ( flags[i] != data->flags[i] ) {
				return false;
			}
		}
	}
	return true;
}

/*
================
NiLinesData::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7FE690
void NiLinesData::GetViewerStrings( NiTArray<char *> *strings ) {
	NiGeometryData::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_pFlags", flags ) );
}

/*
================
NiLinesData::CalculateNormals

Lines have no normals.
================
*/
// FUNCTION: STBC 0x7FE940
void NiLinesData::CalculateNormals() {
}
