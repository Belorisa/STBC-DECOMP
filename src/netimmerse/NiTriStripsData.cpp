// NiTriStripsData.cpp -- triangle strip geometry data

#include "NiRTLib.h"
#include <assert.h>
#include "NiTriStripsData.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x0080eb00 _$E77
// FUNCTION: STBC 0x0080eb20 _$E17
// FUNCTION: STBC 0x0080eb40 _$E15
// FUNCTION: STBC 0x0080eb50 _$E23
// FUNCTION: STBC 0x0080eb70 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0080eb80 _$E48
// FUNCTION: STBC 0x0080eb90 _$E56
// FUNCTION: STBC 0x0080eba0 _$E60
// FUNCTION: STBC 0x0080ebe0 _$E64

// FUNCTION: STBC 0x0080e450 _$E70
// FUNCTION: STBC 0x0080e470 _$E67
// DATA: STBC 0x9A26C8 ?rtti@NiTriStripsData@@2VNiRTTI@@B
const NiRTTI NiTriStripsData::rtti( "NiTriStripsData", &NiTriBasedGeomData::rtti );

/*
================
NiTriStripsData::Register
================
*/
// FUNCTION: STBC 0x80E490 ?Register@NiTriStripsData@@SAHXZ
NiImplementRegister( NiTriStripsData )

/*
================
NiTriStripsData::NiTriStripsData

Takes ownership of the arrays.
================
*/
// FUNCTION: STBC 0x80E5E0 ??0NiTriStripsData@@QAE@GPAVNiPoint3@@0PAVNiColorA@@PAVNiPoint2@@GGPAG3@Z
NiTriStripsData::NiTriStripsData( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, NiPoint2 *textures_, unsigned short triangleCount_, unsigned short stripCount_, unsigned short *stripLengths_, unsigned short *stripLists_ )
	: NiTriBasedGeomData( vertexCount_, vertices_, normals_, colors_, textures_, triangleCount_ ) {
	stripCount = stripCount_;
	stripLengths = stripLengths_;
	stripLists = stripLists_;
}

/*
================
NiTriStripsData::NiTriStripsData
================
*/
// FUNCTION: STBC 0x80E630 ??0NiTriStripsData@@QAE@XZ
NiTriStripsData::NiTriStripsData() {
	stripCount = 0;
	stripLengths = 0;
	stripLists = 0;
}

/*
================
NiTriStripsData::~NiTriStripsData
================
*/
// FUNCTION: STBC 0x80E650 ??1NiTriStripsData@@UAE@XZ
// FUNCTION: STBC 0x80EBC0 ??_GNiTriStripsData@@UAEPAXI@Z
NiTriStripsData::~NiTriStripsData() {
	delete [] stripLengths;
	delete [] stripLists;
}

/*
================
NiTriStripsData::GetStripLengthSum
================
*/
// FUNCTION: STBC 0x80E680
unsigned short NiTriStripsData::GetStripLengthSum() const {
	return triangleCount + 2 * stripCount;
}

/*
================
NiTriStripsData::CreateClone

The data is shared, not copied: the clone is the data itself.
================
*/
// FUNCTION: STBC 0x80E690
NiObject *NiTriStripsData::CreateClone() {
	return this;
}

/*
================
NiTriStripsData::CopyMembers
================
*/
// FUNCTION: STBC 0x80E6A0
void NiTriStripsData::CopyMembers( NiTriStripsData *dest ) {
	assert( false );
}

/*
================
NiTriStripsData::CreateObject

Creates the data and loads it.
================
*/
// FUNCTION: STBC 0x80E6B0
void NiTriStripsData::CreateObject( NiStream &stream, NiObject *&object ) {
	NiTriStripsData *data = new NiTriStripsData;
	stream.SetLoadRecord( new NiTriStripsDataLoadRecord );
	data->LoadBinary( stream );
	object = data;
}

/*
================
NiTriStripsData::LoadBinary

NiTriBasedGeomData's members, the strip count, then the lengths and the
strip lists.

Gamebryo 1.2.2: NiTriStripsData::LoadBinary holds the list length in an
unsigned int.
================
*/
// FUNCTION: STBC 0x80E780
void NiTriStripsData::LoadBinary( NiStream &stream ) {
	NiTriBasedGeomData::LoadBinary( stream );

	NiStreamLoadBinary( stream, stripCount );
	if ( stripCount > 0 ) {
		stripLengths = new unsigned short[stripCount];
		assert( stripLengths != NULL );
		NiStreamLoadBinary( stream, stripLengths, stripCount );
		unsigned int listLength = GetStripLengthSum();
		stripLists = new unsigned short[listLength];
		assert( stripLists != NULL );
		NiStreamLoadBinary( stream, stripLists, listLength );
	}
}

/*
================
NiTriStripsData::LinkObject
================
*/
// FUNCTION: STBC 0x80E800
void NiTriStripsData::LinkObject( NiStream &stream ) {
	NiTriBasedGeomData::LinkObject( stream );
}

/*
================
NiTriStripsData::RegisterStreamables
================
*/
// FUNCTION: STBC 0x80E810
bool NiTriStripsData::RegisterStreamables( NiStream &stream ) {
	return NiTriBasedGeomData::RegisterStreamables( stream );
}

/*
================
NiTriStripsData::SaveBinary
================
*/
// FUNCTION: STBC 0x80E820
void NiTriStripsData::SaveBinary( NiStream &stream ) {
	NiTriBasedGeomData::SaveBinary( stream );

	NiStreamSaveBinary( stream, stripCount );
	if ( stripCount > 0 ) {
		NiStreamSaveBinary( stream, stripLengths, stripCount );
		NiStreamSaveBinary( stream, stripLists, GetStripLengthSum() );
	}
}

/*
================
NiTriStripsData::IsEqual

Gamebryo 1.2.2: NiTriStripsData::IsEqual sums the strip lengths once,
before the list loop.
================
*/
// FUNCTION: STBC 0x80E880
bool NiTriStripsData::IsEqual( NiObject *object ) {
	if ( !NiTriBasedGeomData::IsEqual( object ) ) {
		return false;
	}

	NiTriStripsData *data = (NiTriStripsData *)object;
	if ( stripCount != data->stripCount ) {
		return false;
	}
	unsigned int i, listLength;
	for ( i = 0; i < stripCount; i++ ) {
		if ( stripLengths[i] != data->stripLengths[i] ) {
			return false;
		}
	}

	listLength = GetStripLengthSum();
	for ( i = 0; i < listLength; i++ ) {
		if ( stripLists[i] != data->stripLists[i] ) {
			return false;
		}
	}
	return true;
}

/*
================
NiTriStripsData::GetViewerStrings
================
*/
// FUNCTION: STBC 0x80E910
void NiTriStripsData::GetViewerStrings( NiTArray<char *> *strings ) {
	NiTriBasedGeomData::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_usStrips", stripCount ) );
	strings->Add( NiGetViewerString( "m_pStripLengths", stripLengths ) );
	strings->Add( NiGetViewerString( "m_pStripLists", stripLists ) );
}
