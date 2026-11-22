// NiTriStripData.cpp -- triangle strip data

#include "NiRTLib.h"
#include "NiTriStripData.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x0080d7f0 _$E77
// FUNCTION: STBC 0x0080d810 _$E17
// FUNCTION: STBC 0x0080d830 _$E15
// FUNCTION: STBC 0x0080d840 _$E23
// FUNCTION: STBC 0x0080d860 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0080d900 _$E41
// FUNCTION: STBC 0x0080d910 _$E49
// FUNCTION: STBC 0x0080d920 _$E53
// FUNCTION: STBC 0x0080d970 _$E64

// FUNCTION: STBC 0x0080cfc0 _$E70
// FUNCTION: STBC 0x0080cfe0 _$E67
// DATA: STBC 0x9A2638 ?rtti@NiTriStripData@@2VNiRTTI@@B
const NiRTTI NiTriStripData::rtti( "NiTriStripData", &NiTriBasedGeomData::rtti );

/*
================
NiTriStripData::Register
================
*/
// FUNCTION: STBC 0x80D000 ?Register@NiTriStripData@@SAHXZ
NiImplementRegister( NiTriStripData )

/*
================
NiTriStripData::NiTriStripData

Takes ownership of the arrays, the strip's vertices two more than its
triangles; every triangle starts active.
================
*/
// FUNCTION: STBC 0x80D150 ??0NiTriStripData@@QAE@PAVNiPoint3@@0PAVNiColorA@@PAVNiPoint2@@G@Z
NiTriStripData::NiTriStripData( NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, NiPoint2 *textures_, unsigned short triangleCount_ )
	: NiTriBasedGeomData( triangleCount_ + 2, vertices_, normals_, colors_, textures_, triangleCount_ ) {
	activeTriangleCount = triangleCount_;
}

/*
================
NiTriStripData::NiTriStripData
================
*/
// FUNCTION: STBC 0x80D190 ??0NiTriStripData@@QAE@XZ
NiTriStripData::NiTriStripData() {
	activeTriangleCount = 0;
}

/*
================
NiTriStripData::CalculateNormals

Each triangle of the strip adds its face normal to its three vertices, two
triangles a pass (the second turned the other way, as a strip alternates),
then the odd one left; the sums are then made unit length.

Gamebryo 1.2.2's NiTriStripsData::CalculateNormals names e1, e2 and normal;
AnimOnly.exe 0x4524F4's frame order fits them with pVertex, pNormal and
usTris.
================
*/
// FUNCTION: STBC 0x80D1B0 ?CalculateNormals@NiTriStripData@@UAEXXZ
// PARKED: x87 code selection at the first inline UnitizeVector: x * length with length in st(0) after the join (0.0f or 1.0f / length) loads x and multiplies st(1) here, retail duplicates length and multiplies by [x]; the out-of-line copy (0x80D870) matches and the census of byte-neutral spellings before the site (cards) never moves it
void NiTriStripData::CalculateNormals() {
	CreateNormals( true );

	NiPoint3 *pVertex = vertices;	// (name invented; fits the /Od frame order)
	NiPoint3 *pNormal = normals;	// (name invented; fits the /Od frame order)
	unsigned short usTris = GetActiveTriangleCount() >> 1;	// (name invented; fits the /Od frame order)
	NiPoint3 e1;
	NiPoint3 e2;
	NiPoint3 normal;

	for ( unsigned short i = 0; i < usTris; i++ ) {
		e1 = pVertex[1] - pVertex[0];
		e2 = pVertex[2] - pVertex[1];
		normal = e1.Cross( e2 );
		NiPoint3::UnitizeVector( normal );
		pNormal[0] += normal;
		pNormal[1] += normal;
		pNormal[2] += normal;
		pVertex++;
		pNormal++;

		e1 = pVertex[1] - pVertex[0];
		e2 = pVertex[2] - pVertex[1];
		normal = e2.Cross( e1 );
		NiPoint3::UnitizeVector( normal );
		pNormal[0] += normal;
		pNormal[1] += normal;
		pNormal[2] += normal;
		pVertex++;
		pNormal++;
	}

	if ( GetActiveTriangleCount() & 1 ) {
		e1 = pVertex[1] - pVertex[0];
		e2 = pVertex[2] - pVertex[1];
		normal = e1.Cross( e2 );
		NiPoint3::UnitizeVector( normal );
		pNormal[0] += normal;
		pNormal[1] += normal;
		pNormal[2] += normal;
	}

	NiPoint3::UnitizeVectors( pNormal, GetActiveVertexCount() );	// BUG: unitizes from the walked pointer, not from the first normal
}

/*
================
NiTriStripData::CreateClone

The data is shared, not copied: the clone is the data itself.
================
*/
// FUNCTION: STBC 0x80D570
NiObject *NiTriStripData::CreateClone() {
	return this;
}

/*
================
NiTriStripData::CopyMembers
================
*/
// FUNCTION: STBC 0x80D580
void NiTriStripData::CopyMembers( NiTriStripData *dest ) {
	assert( false );
}

/*
================
NiTriStripData::CreateObject

Creates the data and loads it.
================
*/
// FUNCTION: STBC 0x80D590
void NiTriStripData::CreateObject( NiStream &stream, NiObject *&object ) {
	NiTriStripData *data = new NiTriStripData;
	stream.SetLoadRecord( new NiTriStripDataLoadRecord );
	data->LoadBinary( stream );
	object = data;
}

/*
================
NiTriStripData::LoadBinary
================
*/
// FUNCTION: STBC 0x80D660
void NiTriStripData::LoadBinary( NiStream &stream ) {
	NiTriBasedGeomData::LoadBinary( stream );
	NiStreamLoadBinary( stream, activeTriangleCount );
}

/*
================
NiTriStripData::LinkObject
================
*/
// FUNCTION: STBC 0x80D690
void NiTriStripData::LinkObject( NiStream &stream ) {
	NiTriBasedGeomData::LinkObject( stream );
}

/*
================
NiTriStripData::RegisterStreamables
================
*/
// FUNCTION: STBC 0x80D6A0
bool NiTriStripData::RegisterStreamables( NiStream &stream ) {
	return NiTriBasedGeomData::RegisterStreamables( stream );
}

/*
================
NiTriStripData::SaveBinary
================
*/
// FUNCTION: STBC 0x80D6B0
void NiTriStripData::SaveBinary( NiStream &stream ) {
	NiTriBasedGeomData::SaveBinary( stream );
	NiStreamSaveBinary( stream, activeTriangleCount );
}

/*
================
NiTriStripData::IsEqual
================
*/
// FUNCTION: STBC 0x80D6E0
bool NiTriStripData::IsEqual( NiObject *object ) {
	return NiTriBasedGeomData::IsEqual( object );
}

/*
================
NiTriStripData::GetViewerStrings
================
*/
// FUNCTION: STBC 0x80D6F0
void NiTriStripData::GetViewerStrings( NiTArray<char *> *strings ) {
	NiTriBasedGeomData::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
}

/*
================
NiTriStripData::SetActiveVertexCount

A strip of fewer than three vertices has no triangles.
================
*/
// FUNCTION: STBC 0x80D980
void NiTriStripData::SetActiveVertexCount( unsigned short count ) {
	if ( count > 2 ) {
		if ( count < vertexCount ) {
			activeTriangleCount = count - 2;
		} else {
			activeTriangleCount = triangleCount;
		}
	} else {
		activeTriangleCount = 0;
	}
}

/*
================
NiTriStripData::GetActiveVertexCount
================
*/
// FUNCTION: STBC 0x80D9B0
unsigned short NiTriStripData::GetActiveVertexCount() const {
	if ( activeTriangleCount > 0 ) {
		return activeTriangleCount + 2;
	}
	return 0;
}

/*
================
NiTriStripData::SetActiveTriangleCount
================
*/
// FUNCTION: STBC 0x80D9D0
void NiTriStripData::SetActiveTriangleCount( unsigned short count ) {
	if ( count < triangleCount ) {
		activeTriangleCount = count;
	} else {
		activeTriangleCount = triangleCount;
	}
}
