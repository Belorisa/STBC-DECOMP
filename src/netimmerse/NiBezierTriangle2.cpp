// NiBezierTriangle2.cpp -- triangular Bezier patches of degree 2

#include "NiRTLib.h"
#include "NiStream.h"
#include "NiTriShapeDynamicData.h"
#include "NiBezierTriangle2.h"
#include "NiBezierTriangle3.h"
#include "NiBezierTriangle4.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x0083a4c0 _$E17
// FUNCTION: STBC 0x0083a4e0 _$E15
// FUNCTION: STBC 0x0083a4f0 _$E23
// FUNCTION: STBC 0x0083a510 _$E21
// FUNCTION: STBC 0x0083a580 _$E237

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00838e60 _$E229
// FUNCTION: STBC 0x00838e80 _$E226
// FUNCTION: STBC 0x0083a5a0 _$E48
// FUNCTION: STBC 0x0083a5b0 _$E72
// FUNCTION: STBC 0x0083a5c0 _$E76
// FUNCTION: STBC 0x0083a5d0 _$E80
// FUNCTION: STBC 0x0083a5e0 _$E84
// FUNCTION: STBC 0x0083a5f0 _$E197
// FUNCTION: STBC 0x0083a600 _$E56
// FUNCTION: STBC 0x0083a610 _$E60
// FUNCTION: STBC 0x0083a620 _$E64
// FUNCTION: STBC 0x0083a630 _$E68
// FUNCTION: STBC 0x0083a640 _$E205
// FUNCTION: STBC 0x0083a700 _$E211
// FUNCTION: STBC 0x0083a750 _$E215
// FUNCTION: STBC 0x0083a760 _$E219

// DATA: STBC 0x9B30D8 ?rtti@NiBezierTriangle2@@2VNiRTTI@@B
const NiRTTI NiBezierTriangle2::rtti( "NiBezierTriangle2", &NiBezierTriangle::rtti );

/*
================
NiBezierTriangle2::Register
================
*/
// FUNCTION: STBC 0x838EA0 ?Register@NiBezierTriangle2@@SAHXZ
NiImplementRegister( NiBezierTriangle2 )

/*
================
NiBezierTriangle2::NiBezierTriangle2
================
*/
// FUNCTION: STBC 0x838FF0 ??0NiBezierTriangle2@@QAE@PAVNiPoint3@@PAVNiPoint2@@PAVNiColorA@@_N@Z
NiBezierTriangle2::NiBezierTriangle2( NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08 )
	: NiBezierTriangle( 2, vertices_, textures_, colors_, unknown08 ) {
}

/*
================
NiBezierTriangle2::NiBezierTriangle2
================
*/
// FUNCTION: STBC 0x839020 ??0NiBezierTriangle2@@QAE@PAGPAVNiPoint3@@PAVNiPoint2@@PAVNiColorA@@_NG@Z
NiBezierTriangle2::NiBezierTriangle2( unsigned short *unknownE4_, NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08, unsigned short unknownD6_ )
	: NiBezierTriangle( 2, unknownE4_, vertices_, textures_, colors_, unknown08, unknownD6_ ) {
}

/*
================
NiBezierTriangle2::VirtualSlot46

Whether the grid segment bends enough to split: the patch's second
derivative along the segment's side direction (constant for a quadratic
patch), scaled by the segment's parameter length squared, against
unknownF8 times that side's squared length.
================
*/
// FUNCTION: STBC 0x839060
// The statements are the debug copy's (AnimOnly.exe 0x4E005A).
bool NiBezierTriangle2::VirtualSlot46( unsigned short u0, unsigned short v0, unsigned short u1, unsigned short v1 ) {
	float sideLength;
	unsigned short start;
	unsigned short end;
	float weight[6];	// the second difference along the side, per control point
	if ( v0 == v1 ) {
		// along the first row
		sideLength = unknown160;
		end = u1;
		start = u0;
		weight[0] = 2.0f;
		weight[1] = -4.0f;
		weight[2] = 2.0f;
		weight[3] = 0.0f;
		weight[4] = 0.0f;
		weight[5] = 0.0f;
	} else if ( u0 == u1 ) {
		// along the first column
		sideLength = unknown164;
		end = v1;
		start = v0;
		weight[0] = 2.0f;
		weight[1] = 0.0f;
		weight[2] = 0.0f;
		weight[3] = -4.0f;
		weight[4] = 0.0f;
		weight[5] = 2.0f;
	} else {
		// along the diagonal
		sideLength = unknown168;
		end = v1;
		start = v0;
		weight[0] = 0.0f;
		weight[1] = 0.0f;
		weight[2] = 2.0f;
		weight[3] = 0.0f;
		weight[4] = -4.0f;
		weight[5] = 2.0f;
	}
	float step = ( end - start ) * GRID_STEP;

	NiPoint3 bend;
	if ( unknownE4 ) {
		bend = vertices[unknownE4[0]] * weight[0] + vertices[unknownE4[1]] * weight[1] + vertices[unknownE4[2]] * weight[2] +
			vertices[unknownE4[3]] * weight[3] + vertices[unknownE4[4]] * weight[4] + vertices[unknownE4[5]] * weight[5];
	} else {
		bend = vertices[0] * weight[0] + vertices[1] * weight[1] + vertices[2] * weight[2] + vertices[3] * weight[3] +
			vertices[4] * weight[4] + vertices[5] * weight[5];
	}
	float bendLength = bend.x * bend.x + bend.y * bend.y + bend.z * bend.z;	// squared
	if ( bendLength * step * step > sideLength * unknownF8 ) {
		return true;
	}
	return false;
}

/*
================
NiBezierTriangle2::Unknown8395C0

Raises the quadratic patch to degree 3: the corners stay, each edge's
cubic points are a third of the way between a corner and twice the edge's
quadratic point, and the centre is the mean of the three interior
quadratic points. The texture coordinates and colours of the cubic's
control points are those of the patch evaluated at the cubic's control
parameters (multiples of a third), through slot 41 into fresh sets.
================
*/
// FUNCTION: STBC 0x8395C0
// DRAFT: inline budget: retail expands the texture array's vector constructor iterator (site 64, 31 units short here) and calls one more NiPoint3::operator+ out of line; the /Od body equals AnimOnly 0x4E04A5 but for frame slots (the third as a literal reaches 98.3 but drops the debug copy's local)
NiBezierTriangle3 *NiBezierTriangle2::Unknown8395C0() {
	NiPoint3 *newVertices = new NiPoint3[10];
	float oneThird = 1.0f / 3.0f;	// the step between the cubic's control parameters
	CreateSets();

	if ( unknownE4 ) {
		newVertices[0] = vertices[unknownE4[0]];
		newVertices[3] = vertices[unknownE4[2]];
		newVertices[9] = vertices[unknownE4[5]];
		newVertices[1] = oneThird * ( vertices[unknownE4[0]] + 2.0f * vertices[unknownE4[1]] );
		newVertices[2] = oneThird * ( 2.0f * vertices[unknownE4[1]] + vertices[unknownE4[2]] );
		newVertices[4] = oneThird * ( vertices[unknownE4[0]] + 2.0f * vertices[unknownE4[3]] );
		newVertices[7] = oneThird * ( 2.0f * vertices[unknownE4[3]] + vertices[unknownE4[5]] );
		newVertices[6] = oneThird * ( vertices[unknownE4[2]] + 2.0f * vertices[unknownE4[4]] );
		newVertices[8] = oneThird * ( 2.0f * vertices[unknownE4[4]] + vertices[unknownE4[5]] );
		newVertices[5] = oneThird * ( vertices[unknownE4[3]] + vertices[unknownE4[1]] + vertices[unknownE4[4]] );
	} else {
		newVertices[0] = vertices[0];
		newVertices[3] = vertices[2];
		newVertices[9] = vertices[5];
		newVertices[1] = oneThird * ( vertices[0] + 2.0f * vertices[1] );
		newVertices[2] = oneThird * ( 2.0f * vertices[1] + vertices[2] );
		newVertices[4] = oneThird * ( vertices[0] + 2.0f * vertices[3] );
		newVertices[7] = oneThird * ( 2.0f * vertices[3] + vertices[5] );
		newVertices[6] = oneThird * ( vertices[2] + 2.0f * vertices[4] );
		newVertices[8] = oneThird * ( 2.0f * vertices[4] + vertices[5] );
		newVertices[5] = oneThird * ( vertices[3] + vertices[1] + vertices[4] );
	}

	int row;
	int column;
	int index;
	for ( row = 0; row <= 3; row++ ) {
		for ( column = 0; column <= 3 - row; column++ ) {
			VirtualSlot41( column * oneThird, row * oneThird );
		}
	}

	NiPoint2 *newTextures;
	if ( textures ) {
		newTextures = new NiPoint2[textureSets * 10];
		for ( unsigned short set = 0; set < textureSets; set++ ) {
			for ( row = 0, index = 0; row <= 3; row++ ) {
				for ( column = 0; column <= 3 - row; column++ ) {
					newTextures[set * 10 + index] = textureCoordinates->GetAt( set, index );
					index++;
				}
			}
		}
	} else {
		newTextures = 0;
	}

	NiColorA *newColors;
	if ( colors ) {
		newColors = new NiColorA[10];
		for ( row = 0, index = 0; row <= 3; row++ ) {
			for ( column = 0; column <= 3 - row; column++ ) {
				newColors[index] = vertexColors->GetAt( index );
				index++;
			}
		}
	} else {
		newColors = 0;
	}

	NiBezierTriangle3 *patch = new NiBezierTriangle3( newVertices, newTextures, newColors, GetUnknown08() );
	patch->Unknown83A650( textureSets );
	return patch;
}

/*
================
NiBezierTriangle2::CreateClone
================
*/
// FUNCTION: STBC 0x83A270
NiObject *NiBezierTriangle2::CreateClone() {
	NiBezierTriangle2 *tri = new NiBezierTriangle2;
	assert( tri );
	CopyMembers( tri );
	return tri;
}

/*
================
NiBezierTriangle2::CopyMembers
================
*/
// FUNCTION: STBC 0x83A300
void NiBezierTriangle2::CopyMembers( NiBezierTriangle2 *dest ) {
	NiBezierTriangle::CopyMembers( dest );

	if ( !dest->unknownE4 ) {
		dest->Unknown8330E0( dest->level );
	}
}

/*
================
NiBezierTriangle2::CreateObject

Creates the patch and loads it.
================
*/
// FUNCTION: STBC 0x83A330
void NiBezierTriangle2::CreateObject( NiStream &stream, NiObject *&object ) {
	NiBezierTriangle2 *triangle = new NiBezierTriangle2;
	stream.SetLoadRecord( new NiBezierTriangle2LoadRecord );
	triangle->LoadBinary( stream );
	object = triangle;
}

/*
================
NiBezierTriangle2::LoadBinary

Tessellates at the loaded level unless the patch owns unknownE4.
================
*/
// FUNCTION: STBC 0x83A450
void NiBezierTriangle2::LoadBinary( NiStream &stream ) {
	NiBezierTriangle::LoadBinary( stream );

	if ( !unknownE4 ) {
		Unknown8330E0( level );
	}
}

/*
================
NiBezierTriangle2::LinkObject
================
*/
// FUNCTION: STBC 0x83A480
void NiBezierTriangle2::LinkObject( NiStream &stream ) {
	NiBezierTriangle::LinkObject( stream );
}

/*
================
NiBezierTriangle2::RegisterStreamables
================
*/
// FUNCTION: STBC 0x83A490
bool NiBezierTriangle2::RegisterStreamables( NiStream &stream ) {
	return NiBezierTriangle::RegisterStreamables( stream );
}

/*
================
NiBezierTriangle2::SaveBinary
================
*/
// FUNCTION: STBC 0x83A4A0
void NiBezierTriangle2::SaveBinary( NiStream &stream ) {
	NiBezierTriangle::SaveBinary( stream );
}

/*
================
NiBezierTriangle2::IsEqual
================
*/
// FUNCTION: STBC 0x83A4B0
bool NiBezierTriangle2::IsEqual( NiObject *object ) {
	return NiBezierTriangle::IsEqual( object );
}
