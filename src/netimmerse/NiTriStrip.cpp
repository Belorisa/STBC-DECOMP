// NiTriStrip.cpp -- single triangle strip geometry objects

#include "NiRTLib.h"
#include "NiOBBRoot.h"
#include "NiCamera.h"
#include "NiStream.h"
#include "NiTriStrip.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0080e280 _$E37
// FUNCTION: STBC 0x0080e290 _$E52
// FUNCTION: STBC 0x0080e2a0 _$E56
// FUNCTION: STBC 0x0080e2b0 _$E60
// FUNCTION: STBC 0x0080e2c0 _$E64
// FUNCTION: STBC 0x0080e2d0 _$E181
// FUNCTION: STBC 0x0080e2e0 _$E41
// FUNCTION: STBC 0x0080e2f0 _$E123
// FUNCTION: STBC 0x0080e300 _$E136
// FUNCTION: STBC 0x0080e310 _$E186
// FUNCTION: STBC 0x0080e320 _$E191
// FUNCTION: STBC 0x0080e330 _$E195
// FUNCTION: STBC 0x0080e340 _$E199
// FUNCTION: STBC 0x0080e350 _$E206
// FUNCTION: STBC 0x0080e360 _$E214
// FUNCTION: STBC 0x0080e370 _$E221
// FUNCTION: STBC 0x0080e380 _$E225
// FUNCTION: STBC 0x0080e390 _$E229
// FUNCTION: STBC 0x0080e3a0 _$E233
// FUNCTION: STBC 0x0080e3f0 _$E237

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x0080e260 _$E250
// FUNCTION: STBC 0x0080e200 _$E17
// FUNCTION: STBC 0x0080e220 _$E15
// FUNCTION: STBC 0x0080e230 _$E23
// FUNCTION: STBC 0x0080e250 _$E21

// FUNCTION: STBC 0x0080da00 _$E243
// FUNCTION: STBC 0x0080da20 _$E240
// DATA: STBC 0x9A2688 ?rtti@NiTriStrip@@2VNiRTTI@@B
const NiRTTI NiTriStrip::rtti( "NiTriStrip", &NiTriBasedGeom::rtti );

/*
================
NiTriStrip::Register
================
*/
// FUNCTION: STBC 0x80DA40 ?Register@NiTriStrip@@SAHXZ
NiImplementRegister( NiTriStrip )

/*
================
NiTriStrip::NiTriStrip

Creates the data, which takes ownership of the arrays.
================
*/
// FUNCTION: STBC 0x80DB90 ??0NiTriStrip@@QAE@PAVNiPoint3@@0PAVNiColorA@@PAVNiPoint2@@G@Z
NiTriStrip::NiTriStrip( NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, NiPoint2 *textures_, unsigned short triangleCount_ )
	: NiTriBasedGeom( new NiTriStripData( vertices_, normals_, colors_, textures_, triangleCount_ ) ) {
}

/*
================
NiTriStrip::NiTriStrip
================
*/
// FUNCTION: STBC 0x80DC20 ??0NiTriStrip@@QAE@PAVNiTriStripData@@@Z
NiTriStrip::NiTriStrip( NiTriStripData *data ) : NiTriBasedGeom( data ) {
}

/*
================
NiTriStrip::NiTriStrip
================
*/
// FUNCTION: STBC 0x80DC40 ??0NiTriStrip@@QAE@XZ
NiTriStrip::NiTriStrip() {
}

/*
================
NiTriStrip::GetWorldTriangle

The strip's next three world vertices; NULL vertices when there is no
world copy.
================
*/
// FUNCTION: STBC 0x80DC60
void NiTriStrip::GetWorldTriangle( unsigned short index, NiPoint3 *&vertex0, NiPoint3 *&vertex1, NiPoint3 *&vertex2 ) {
	assert( index < GetTriangleCount() );
	if ( worldVertices ) {
		vertex0 = &worldVertices[index++];
		vertex1 = &worldVertices[index++];
		vertex2 = &worldVertices[index];
	} else {
		vertex0 = 0;
		vertex1 = 0;
		vertex2 = 0;
	}
}

/*
================
NiTriStrip::GetModelTriangle

The strip's next three model vertices.
================
*/
// FUNCTION: STBC 0x80DCE0
void NiTriStrip::GetModelTriangle( unsigned short index, NiPoint3 *&vertex0, NiPoint3 *&vertex1, NiPoint3 *&vertex2 ) {
	// AnimOnly.exe 0x452F19 (NiTriStrip.cpp)
	NiTriStripData *data = NiSmartPointerCast( NiTriStripData, modelData );
	assert( index < data->GetTriangleCount() );
	NiPoint3 *vertices = data->GetVertices();
	vertex0 = &vertices[index++];
	vertex1 = &vertices[index++];
	vertex2 = &vertices[index];
}

/*
================
NiTriStrip::Display

Draws the strip unless the camera's sorter takes it, and counts it.
================
*/
// FUNCTION: STBC 0x80DD30
void NiTriStrip::Display( NiCamera *camera ) {
	NiRender *renderer = camera->GetRenderer();
	assert( renderer );
	if ( !renderer->RegisterObject( this ) ) {
		camera->UpdateActivePlanes();
		renderer->VirtualSlot42( worldTranslate, worldRotate, worldScale );
		renderer->VirtualSlot47( NiSmartPointerCast( NiTriStripData, modelData ), GetWorldBound() );
		shapesDrawn++;
		trianglesDrawn += GetActiveTriangleCount();
		verticesDrawn += GetActiveVertexCount();
	}
}

/*
================
NiTriStrip::VirtualSlot23

Builds the bounding box tree once, from a triangle list spelled out of the
strip: the triangles of the whole strip, every other one wound the other
way. The list is built even when the tree already exists.
The counts and the world vertices come through NiGeometry's and
NiTriBasedGeom's accessors, as the SDK's debug NetImmerse 3.1 reads them
(AnimOnly.exe 0x4530B7).
================
*/
// FUNCTION: STBC 0x80DDE0
void NiTriStrip::VirtualSlot23( int unknown ) {
	unsigned short triangleCount = GetVertexCount() - 2;
	unsigned short listSize = 3 * triangleCount;
	unsigned short *triList = new unsigned short[listSize];
	assert( triList );
	unsigned short listIndex = 0;
	for ( unsigned short i = 0; i < triangleCount; i++ ) {
		triList[listIndex++] = i;
		if ( i % 2 == 0 ) {
			triList[listIndex++] = i + 1;
			triList[listIndex++] = i + 2;
		} else {
			triList[listIndex++] = i + 2;
			triList[listIndex++] = i + 1;
		}
	}

	if ( !obbRoot ) {
		obbRoot = new NiOBBRoot( GetTriangleCount(), triList, GetVertices(), GetWorldVertices(), unknown );
		assert( obbRoot );
	}
	delete [] triList;
}

/*
================
NiTriStrip::CreateClone
================
*/
// FUNCTION: STBC 0x80DF10
NiObject *NiTriStrip::CreateClone() {
	NiTriStrip *clone = new NiTriStrip;
	CopyMembers( clone );
	return clone;
}

/*
================
NiTriStrip::CopyMembers
================
*/
// FUNCTION: STBC 0x80DF80
void NiTriStrip::CopyMembers( NiTriStrip *dest ) {
	NiTriBasedGeom::CopyMembers( dest );
}

/*
================
NiTriStrip::CreateObject

Creates the strip and loads it.
================
*/
// FUNCTION: STBC 0x80DF90
void NiTriStrip::CreateObject( NiStream &stream, NiObject *&object ) {
	NiTriStrip *strip = new NiTriStrip;
	stream.SetLoadRecord( new NiTriStripLoadRecord );
	strip->LoadBinary( stream );
	object = strip;
}

/*
================
NiTriStrip::LoadBinary
================
*/
// FUNCTION: STBC 0x80E0B0
void NiTriStrip::LoadBinary( NiStream &stream ) {
	NiTriBasedGeom::LoadBinary( stream );
}

/*
================
NiTriStrip::LinkObject
================
*/
// FUNCTION: STBC 0x80E0C0
void NiTriStrip::LinkObject( NiStream &stream ) {
	NiTriBasedGeom::LinkObject( stream );
}

/*
================
NiTriStrip::RegisterStreamables
================
*/
// FUNCTION: STBC 0x80E0D0
bool NiTriStrip::RegisterStreamables( NiStream &stream ) {
	return NiTriBasedGeom::RegisterStreamables( stream );
}

/*
================
NiTriStrip::SaveBinary
================
*/
// FUNCTION: STBC 0x80E0E0
void NiTriStrip::SaveBinary( NiStream &stream ) {
	NiTriBasedGeom::SaveBinary( stream );
}

/*
================
NiTriStrip::IsEqual
================
*/
// FUNCTION: STBC 0x80E0F0
bool NiTriStrip::IsEqual( NiObject *object ) {
	return NiTriBasedGeom::IsEqual( object );
}

/*
================
NiTriStrip::GetViewerStrings
================
*/
// FUNCTION: STBC 0x80E100
void NiTriStrip::GetViewerStrings( NiTArray<char *> *strings ) {
	NiTriBasedGeom::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
}

/*
================
NiTriStrip::GetTriangleIndices

The strip's next three vertices, the odd triangles wound the other way.
================
*/
// FUNCTION: STBC 0x80E400
void NiTriStrip::GetTriangleIndices( unsigned short index, unsigned short &index0, unsigned short &index1, unsigned short &index2 ) const {
	index0 = index;
	if ( index % 2 == 0 ) {
		index1 = index + 1;
		index2 = index + 2;
	} else {
		index1 = index + 2;
		index2 = index + 1;
	}
}
