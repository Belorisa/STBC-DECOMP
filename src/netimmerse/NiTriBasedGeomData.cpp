// NiTriBasedGeomData.cpp -- geometry data drawn as triangles

#include "NiRTLib.h"
#include <assert.h>
#include "NiTriBasedGeomData.h"
#include "NiStream.h"
#include "NiGeometry.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007eefa0 _$E209
// FUNCTION: STBC 0x007eefc0 _$E17
// FUNCTION: STBC 0x007eefe0 _$E15
// FUNCTION: STBC 0x007eeff0 _$E23
// FUNCTION: STBC 0x007ef010 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007ef020 _$E41
// FUNCTION: STBC 0x007ef030 _$E49
// FUNCTION: STBC 0x007ef070 _$E53

// FUNCTION: STBC 0x007eeae0 _$E202
// FUNCTION: STBC 0x007eeb00 _$E199
// DATA: STBC 0x9A1AD0 ?rtti@NiTriBasedGeomData@@2VNiRTTI@@B
const NiRTTI NiTriBasedGeomData::rtti( "NiTriBasedGeomData", &NiGeometryData::rtti );

/*
================
NiTriBasedGeomData::Register
================
*/
// FUNCTION: STBC 0x7EEB20 ?Register@NiTriBasedGeomData@@SAHXZ
NiImplementRegister( NiTriBasedGeomData )

/*
================
NiTriBasedGeomData::NiTriBasedGeomData

Takes ownership of the arrays.
================
*/
// FUNCTION: STBC 0x7EEC70 ??0NiTriBasedGeomData@@QAE@GPAVNiPoint3@@0PAVNiColorA@@PAVNiPoint2@@G@Z
NiTriBasedGeomData::NiTriBasedGeomData( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, NiPoint2 *textures_, unsigned short triangleCount_ )
	: NiGeometryData( vertexCount_, vertices_, normals_, colors_, textures_ ) {
	triangleCount = triangleCount_;
}

/*
================
NiTriBasedGeomData::NiTriBasedGeomData
================
*/
// FUNCTION: STBC 0x7EECB0 ??0NiTriBasedGeomData@@QAE@XZ
NiTriBasedGeomData::NiTriBasedGeomData() {
	triangleCount = 0;
}

/*
================
NiTriBasedGeomData::~NiTriBasedGeomData
================
*/
// FUNCTION: STBC 0x7EECD0
NiTriBasedGeomData::~NiTriBasedGeomData() {
}

/*
================
NiTriBasedGeomData::CreateClone

NiTriBasedGeomData itself is never cloned; the derived classes create
the clone.
================
*/
// FUNCTION: STBC 0x7EECE0
NiObject *NiTriBasedGeomData::CreateClone() {
	assert( false );
	return 0;
}

/*
================
NiTriBasedGeomData::CopyMembers

A clone copies nothing of NiTriBasedGeomData's own.
================
*/
// FUNCTION: STBC 0x7EECF0
void NiTriBasedGeomData::CopyMembers( NiTriBasedGeomData *dest ) {
	assert( false );
}

/*
================
NiTriBasedGeomData::CreateObject

The class is abstract: the stream creates nothing for it.
================
*/
// FUNCTION: STBC 0x7EED00
void NiTriBasedGeomData::CreateObject( NiStream &stream, NiObject *&object ) {
}

/*
================
NiTriBasedGeomData::LoadBinary
================
*/
// FUNCTION: STBC 0x7EED10
void NiTriBasedGeomData::LoadBinary( NiStream &stream ) {
	NiGeometryData::LoadBinary( stream );
	NiStreamLoadBinary( stream, triangleCount );
}

/*
================
NiTriBasedGeomData::LinkObject
================
*/
// FUNCTION: STBC 0x7EED40
void NiTriBasedGeomData::LinkObject( NiStream &stream ) {
	NiGeometryData::LinkObject( stream );
}

/*
================
NiTriBasedGeomData::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7EED50
bool NiTriBasedGeomData::RegisterStreamables( NiStream &stream ) {
	return NiGeometryData::RegisterStreamables( stream );
}

/*
================
NiTriBasedGeomData::SaveBinary
================
*/
// FUNCTION: STBC 0x7EED60
void NiTriBasedGeomData::SaveBinary( NiStream &stream ) {
	NiGeometryData::SaveBinary( stream );
	NiStreamSaveBinary( stream, triangleCount );
}

/*
================
NiTriBasedGeomData::IsEqual
================
*/
// FUNCTION: STBC 0x7EED90
bool NiTriBasedGeomData::IsEqual( NiObject *object ) {
	if ( !NiGeometryData::IsEqual( object ) ) {
		return false;
	}
	NiTriBasedGeomData *data = (NiTriBasedGeomData *)object;
	if ( triangleCount != data->triangleCount ) {
		return false;
	}
	return true;
}

/*
================
NiTriBasedGeomData::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7EEDC0
void NiTriBasedGeomData::GetViewerStrings( NiTArray<char *> *strings ) {
	NiGeometryData::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_usTriangles", triangleCount ) );
}
