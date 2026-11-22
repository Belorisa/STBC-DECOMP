// NiTriShapeData.cpp -- triangle list geometry data

#include "NiRTLib.h"
#include <assert.h>
#include <string.h>

#include "NiTriShapeData.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007f1da0 _$E79
// FUNCTION: STBC 0x007f1dc0 _$E17
// FUNCTION: STBC 0x007f1de0 _$E15
// FUNCTION: STBC 0x007f1df0 _$E23
// FUNCTION: STBC 0x007f1e10 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007f1e20 _$E48
// FUNCTION: STBC 0x007f1e30 _$E56
// FUNCTION: STBC 0x007f1e40 _$E60
// FUNCTION: STBC 0x007f1e80 _$E64

// FUNCTION: STBC 0x007f1270 _$E70
// FUNCTION: STBC 0x007f1290 _$E67
// DATA: STBC 0x9A1B40 ?rtti@NiTriShapeData@@2VNiRTTI@@B
const NiRTTI NiTriShapeData::rtti( "NiTriShapeData", &NiTriBasedGeomData::rtti );

/*
================
NiTriShapeData::Register
================
*/
// FUNCTION: STBC 0x7F12B0 ?Register@NiTriShapeData@@SAHXZ
NiImplementRegister( NiTriShapeData )

/*
================
NiTriShapeData::NiTriShapeData

Takes ownership of the arrays.
================
*/
// FUNCTION: STBC 0x7F1400 ??0NiTriShapeData@@QAE@GPAVNiPoint3@@0PAVNiColorA@@PAVNiPoint2@@GPAG@Z
NiTriShapeData::NiTriShapeData( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, NiPoint2 *textures_, unsigned short triangleCount_, unsigned short *triList_ )
	: NiTriBasedGeomData( vertexCount_, vertices_, normals_, colors_, textures_, triangleCount_ ) {
	triListLength = 3 * triangleCount;
	triList = triList_;
	sharedNormals = 0;
	sharedNormalsArraySize = 0;
}

/*
================
NiTriShapeData::NiTriShapeData
================
*/
// FUNCTION: STBC 0x7F1450 ??0NiTriShapeData@@QAE@XZ
NiTriShapeData::NiTriShapeData() {
	triListLength = 0;
	triList = 0;
	sharedNormals = 0;
	sharedNormalsArraySize = 0;
}

/*
================
NiTriShapeData::~NiTriShapeData
================
*/
// FUNCTION: STBC 0x7F1480 ??1NiTriShapeData@@UAE@XZ
NiTriShapeData::~NiTriShapeData() {
	delete [] triList;
	delete [] sharedNormals;
}

/*
================
NiTriShapeData::Replace
================
*/
// FUNCTION: STBC 0x7F1560
void NiTriShapeData::Replace( unsigned short *triList_ ) {
	triListLength = 3 * triangleCount;
	delete [] triList;
	triList = triList_;
}

/*
================
NiTriShapeData::SetData
================
*/
// FUNCTION: STBC 0x7F1590
void NiTriShapeData::SetData( unsigned short *triList_ ) {
	triListLength = 3 * triangleCount;
	triList = triList_;
}

/*
================
NiTriShapeData::LetVerticesShareNormal

Joins the two vertices' sharing groups: every vertex of the joined group
gets a new array of the others. Nothing changes when the two already
share. The shared normal array is rebuilt, empty, when the vertex count
changed (Gamebryo lineage; this version allocates each vertex's array on
its own).
================
*/
// FUNCTION: STBC 0x7F15B0
void NiTriShapeData::LetVerticesShareNormal( unsigned short index0, unsigned short index1 ) {
	if ( index0 == index1 ) {
		return;
	}

	if ( sharedNormalsArraySize != vertexCount ) {
		delete [] sharedNormals;
		sharedNormals = 0;
	}

	if ( sharedNormals == 0 ) {
		sharedNormals = new SharedNormalArray[vertexCount];
		sharedNormalsArraySize = vertexCount;
	}

	if ( index0 > sharedNormalsArraySize || index1 > sharedNormalsArraySize ) {
		return;
	}

	// both groups in one array
	unsigned short maxCount0 = sharedNormals[index0].GetCount();
	unsigned short maxCount1 = sharedNormals[index1].GetCount();
	unsigned short *result = new unsigned short[maxCount0 + maxCount1 + 2];

	unsigned short *array0 = sharedNormals[index0].GetArray();
	unsigned short i, count = 0;
	for ( i = 0; i < maxCount0; i++ ) {
		if ( array0[i] == index1 ) {
			delete [] result;
			return;
		}
		result[count++] = array0[i];
	}

	unsigned short *array1 = sharedNormals[index1].GetArray();
	for ( i = 0; i < maxCount1; i++ ) {
		if ( array1[i] == index0 ) {
			delete [] result;
			return;
		}
		result[count++] = array1[i];
	}

	result[count++] = index0;
	result[count++] = index1;

	// each member shares with the others
	for ( i = 0; i < count; i++ ) {
		unsigned short *newArray = new unsigned short[count - 1];
		unsigned short index = result[i];
		unsigned short newCount = 0;
		for ( unsigned short j = 0; j < count; j++ ) {
			if ( result[j] != index ) {
				newArray[newCount++] = result[j];
			}
		}
		sharedNormals[index].SetArray( newCount, newArray );
	}

	delete [] result;
}

/*
================
NiTriShapeData::CreateClone

The data is shared, not copied: the clone is the data itself.
================
*/
// FUNCTION: STBC 0x7F1840
NiObject *NiTriShapeData::CreateClone() {
	return this;
}

/*
================
NiTriShapeData::CopyMembers
================
*/
// FUNCTION: STBC 0x7F1850
void NiTriShapeData::CopyMembers( NiTriShapeData *dest ) {
	assert( false );
}

/*
================
NiTriShapeData::CreateObject

Creates the data and loads it.
================
*/
// FUNCTION: STBC 0x7F1860
void NiTriShapeData::CreateObject( NiStream &stream, NiObject *&object ) {
	NiTriShapeData *data = new NiTriShapeData;
	stream.SetLoadRecord( new NiTriShapeDataLoadRecord );
	data->LoadBinary( stream );
	object = data;
}

/*
================
NiTriShapeData::LoadBinary

NiTriBasedGeomData's members, the triangle list, then, in files newer
than 3.05, the shared normal arrays.
================
*/
// FUNCTION: STBC 0x7F1930
void NiTriShapeData::LoadBinary( NiStream &stream ) {
	NiTriBasedGeomData::LoadBinary( stream );

	NiStreamLoadBinary( stream, triListLength );
	if ( triListLength > 0 ) {
		triList = new unsigned short[triListLength];
		assert( triList );
		NiStreamLoadBinary( stream, triList, triListLength );
	}

	if ( strcmp( stream.GetFileVersion(), "3.05" ) > 0 ) {
		NiStreamLoadBinary( stream, sharedNormalsArraySize );
		if ( sharedNormalsArraySize ) {
			sharedNormals = new SharedNormalArray[sharedNormalsArraySize];
			unsigned short count;
			for ( unsigned short i = 0; i < sharedNormalsArraySize; i++ ) {
				NiStreamLoadBinary( stream, count );
				unsigned short *indices;
				if ( count ) {
					indices = new unsigned short[count];
					NiStreamLoadBinary( stream, indices, count );
				} else {
					indices = 0;
				}
				sharedNormals[i].SetArray( count, indices );
			}
		}
	}
}

/*
================
NiTriShapeData::LinkObject
================
*/
// FUNCTION: STBC 0x7F1AC0
void NiTriShapeData::LinkObject( NiStream &stream ) {
	NiTriBasedGeomData::LinkObject( stream );
}

/*
================
NiTriShapeData::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7F1AD0
bool NiTriShapeData::RegisterStreamables( NiStream &stream ) {
	return NiTriBasedGeomData::RegisterStreamables( stream );
}

/*
================
NiTriShapeData::SaveBinary

The order LoadBinary reads; the shared normal arrays are always written.
================
*/
// FUNCTION: STBC 0x7F1AE0
void NiTriShapeData::SaveBinary( NiStream &stream ) {
	NiTriBasedGeomData::SaveBinary( stream );

	NiStreamSaveBinary( stream, triListLength );
	if ( triListLength > 0 ) {
		NiStreamSaveBinary( stream, triList, triListLength );
	}

	NiStreamSaveBinary( stream, sharedNormalsArraySize );
	for ( unsigned short i = 0; i < sharedNormalsArraySize; i++ ) {
		unsigned short count = sharedNormals[i].GetCount();
		NiStreamSaveBinary( stream, count );
		if ( count ) {
			NiStreamSaveBinary( stream, sharedNormals[i].GetArray(), count );
		}
	}
}

/*
================
NiTriShapeData::IsEqual
================
*/
// FUNCTION: STBC 0x7F1B80
bool NiTriShapeData::IsEqual( NiObject *object ) {
	if ( !NiTriBasedGeomData::IsEqual( object ) ) {
		return false;
	}
	NiTriShapeData *data = (NiTriShapeData *)object;
	if ( triListLength != data->triListLength ) {
		return false;
	}
	unsigned int bytes = triListLength * sizeof( unsigned short );	// (name invented; fits the /Od frame order)
	if ( memcmp( triList, data->triList, bytes ) != 0 ) {
		return false;
	}
	return true;
}

/*
================
NiTriShapeData::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7F1BC0
void NiTriShapeData::GetViewerStrings( NiTArray<char *> *strings ) {
	NiTriBasedGeomData::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_uiTriListLength", triListLength ) );
	strings->Add( NiGetViewerString( "m_pTriList", triList ) );
}
