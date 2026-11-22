// NiBezierTriangle3.cpp -- triangular Bezier patches of degree 3

#include "NiRTLib.h"
#include "NiStream.h"
#include "NiTriShapeDynamicData.h"
#include "NiBezierTriangle2.h"
#include "NiBezierTriangle3.h"
#include "NiBezierTriangle4.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x0083d660 _$E17
// FUNCTION: STBC 0x0083d680 _$E15
// FUNCTION: STBC 0x0083d690 _$E23
// FUNCTION: STBC 0x0083d6b0 _$E21
// FUNCTION: STBC 0x0083d6c0 _$E238

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0083a780 _$E229
// FUNCTION: STBC 0x0083a7a0 _$E226
// FUNCTION: STBC 0x0083a770 _$E223
// FUNCTION: STBC 0x0083d6e0 _$E48
// FUNCTION: STBC 0x0083d6f0 _$E72
// FUNCTION: STBC 0x0083d700 _$E76
// FUNCTION: STBC 0x0083d710 _$E80
// FUNCTION: STBC 0x0083d720 _$E84
// FUNCTION: STBC 0x0083d730 _$E197
// FUNCTION: STBC 0x0083d740 _$E56
// FUNCTION: STBC 0x0083d750 _$E60
// FUNCTION: STBC 0x0083d760 _$E64
// FUNCTION: STBC 0x0083d770 _$E68
// FUNCTION: STBC 0x0083d780 _$E205
// FUNCTION: STBC 0x0083d790 _$E211
// FUNCTION: STBC 0x0083d7a0 _$E215
// FUNCTION: STBC 0x0083d7f0 _$E219

// DATA: STBC 0x9B3120 ?rtti@NiBezierTriangle3@@2VNiRTTI@@B
const NiRTTI NiBezierTriangle3::rtti( "NiBezierTriangle3", &NiBezierTriangle::rtti );

/*
================
NiBezierTriangle3::Register
================
*/
// FUNCTION: STBC 0x83A7C0 ?Register@NiBezierTriangle3@@SAHXZ
NiImplementRegister( NiBezierTriangle3 )

/*
================
NiBezierTriangle3::NiBezierTriangle3
================
*/
// FUNCTION: STBC 0x83A910 ??0NiBezierTriangle3@@QAE@PAVNiPoint3@@PAVNiPoint2@@PAVNiColorA@@_N@Z
NiBezierTriangle3::NiBezierTriangle3( NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08 )
	: NiBezierTriangle( 3, vertices_, textures_, colors_, unknown08 ) {
}

/*
================
NiBezierTriangle3::NiBezierTriangle3
================
*/
// FUNCTION: STBC 0x83A940 ??0NiBezierTriangle3@@QAE@PAGPAVNiPoint3@@PAVNiPoint2@@PAVNiColorA@@_NG@Z
NiBezierTriangle3::NiBezierTriangle3( unsigned short *unknownE4_, NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08, unsigned short unknownD6_ )
	: NiBezierTriangle( 3, unknownE4_, vertices_, textures_, colors_, unknown08, unknownD6_ ) {
}

/*
================
NiBezierTriangle3::VirtualSlot46

As NiBezierTriangle2::VirtualSlot46 for the cubic patch: the second
derivative along the segment's side direction, taken at the segment's
midpoint, scaled by its parameter length squared, against unknownF8 times
that side's squared length. The weights are the second derivatives of the
cubic Bernstein basis along that direction.
================
*/
// FUNCTION: STBC 0x83A980 ?VirtualSlot46@NiBezierTriangle3@@UAE_NGGGG@Z
// The statements are the debug copy's (AnimOnly.exe 0x4E1AE1).
bool NiBezierTriangle3::VirtualSlot46( unsigned short u0, unsigned short v0, unsigned short u1, unsigned short v1 ) {
	unsigned short u = ( u1 + u0 ) >> 1;
	unsigned short v = ( v1 + v0 ) >> 1;
	float sideLength;
	unsigned short start;
	unsigned short end;
	float weight[10];	// the second difference along the side, per control point
	if ( v0 == v1 ) {
		// along the first row
		sideLength = unknown160;
		end = u1;
		start = u0;
		float scaledU = u * 6.0f * GRID_STEP;	// (names invented)
		float scaledV = v0 * 6.0f * GRID_STEP;
		float scaledW = 6.0f - scaledV - scaledU;
		weight[0] = scaledW;
		weight[1] = scaledU - 2.0f * scaledW;
		weight[2] = scaledW - 2.0f * scaledU;
		weight[3] = scaledU;
		weight[4] = scaledV;
		weight[5] = -2.0f * scaledV;
		weight[6] = scaledV;
		weight[7] = 0.0f;
		weight[8] = 0.0f;
		weight[9] = 0.0f;
	} else if ( u0 == u1 ) {
		// along the first column
		sideLength = unknown164;
		end = v1;
		start = v0;
		float scaledU = u0 * 6.0f * GRID_STEP;
		float scaledV = v * 6.0f * GRID_STEP;
		float scaledW = 6.0f - scaledV - scaledU;
		weight[0] = scaledW;
		weight[1] = scaledU;
		weight[2] = 0.0f;
		weight[3] = 0.0f;
		weight[4] = scaledV - 2.0f * scaledW;
		weight[5] = -2.0f * scaledU;
		weight[6] = 0.0f;
		weight[7] = scaledW - 2.0f * scaledV;
		weight[8] = scaledU;
		weight[9] = scaledV;
	} else {
		// along the diagonal
		sideLength = unknown168;
		end = v1;
		start = v0;
		float scaledU = u * 6.0f * GRID_STEP;
		float scaledV = v * 6.0f * GRID_STEP;
		float scaledW = 6.0f - scaledV - scaledU;
		weight[0] = 0.0f;
		weight[1] = 0.0f;
		weight[2] = scaledW;
		weight[3] = scaledU;
		weight[4] = 0.0f;
		weight[5] = -2.0f * scaledW;
		weight[6] = scaledV - 2.0f * scaledU;
		weight[7] = scaledW;
		weight[8] = scaledU - 2.0f * scaledV;
		weight[9] = scaledV;
	}
	float step = ( end - start ) * GRID_STEP;

	NiPoint3 bend;
	if ( unknownE4 ) {
		bend = vertices[unknownE4[0]] * weight[0] + vertices[unknownE4[1]] * weight[1] + vertices[unknownE4[2]] * weight[2] +
			vertices[unknownE4[3]] * weight[3] + vertices[unknownE4[4]] * weight[4] + vertices[unknownE4[5]] * weight[5] +
			vertices[unknownE4[6]] * weight[6] + vertices[unknownE4[7]] * weight[7] + vertices[unknownE4[8]] * weight[8] +
			vertices[unknownE4[9]] * weight[9];
	} else {
		bend = vertices[0] * weight[0] + vertices[1] * weight[1] + vertices[2] * weight[2] +
			vertices[3] * weight[3] + vertices[4] * weight[4] + vertices[5] * weight[5] +
			vertices[6] * weight[6] + vertices[7] * weight[7] + vertices[8] * weight[8] +
			vertices[9] * weight[9];
	}
	float bendLength = bend.x * bend.x + bend.y * bend.y + bend.z * bend.z;	// squared
	if ( bendLength * step * step > sideLength * unknownF8 ) {
		return true;
	}
	return false;
}

/*
================
NiBezierTriangle3::Unknown83B370

Lowers the cubic patch to degree 2: the corners stay, and each edge's
quadratic point is the one that reproduces the cubic edge's midpoint,
( 3 ( P1 + P2 ) - P0 - P3 ) / 4. The texture coordinates and colours of
the quadratic's control points are those of the patch evaluated at the
quadratic's control parameters (multiples of a half), through slot 41
into fresh sets.
================
*/
// FUNCTION: STBC 0x83B370
NiBezierTriangle2 *NiBezierTriangle3::Unknown83B370() {
	NiPoint3 *newVertices = new NiPoint3[6];
	CreateSets();

	if ( unknownE4 ) {
		newVertices[0] = vertices[unknownE4[0]];
		newVertices[2] = vertices[unknownE4[3]];
		newVertices[5] = vertices[unknownE4[9]];
		newVertices[1] = 0.25f * ( -vertices[unknownE4[0]] + 3.0f * vertices[unknownE4[1]] + 3.0f * vertices[unknownE4[2]] - vertices[unknownE4[3]] );
		newVertices[3] = 0.25f * ( -vertices[unknownE4[0]] + 3.0f * vertices[unknownE4[4]] + 3.0f * vertices[unknownE4[7]] - vertices[unknownE4[9]] );
		newVertices[4] = 0.25f * ( -vertices[unknownE4[3]] + 3.0f * vertices[unknownE4[6]] + 3.0f * vertices[unknownE4[8]] - vertices[unknownE4[9]] );
	} else {
		newVertices[0] = vertices[0];
		newVertices[2] = vertices[3];
		newVertices[5] = vertices[9];
		newVertices[1] = 0.25f * ( -vertices[0] + 3.0f * vertices[1] + 3.0f * vertices[2] - vertices[3] );
		newVertices[3] = 0.25f * ( -vertices[0] + 3.0f * vertices[4] + 3.0f * vertices[7] - vertices[9] );
		newVertices[4] = 0.25f * ( -vertices[3] + 3.0f * vertices[6] + 3.0f * vertices[8] - vertices[9] );
	}

	int row;
	int column;
	int index;
	for ( row = 0; row <= 2; row++ ) {
		for ( column = 0; column <= 2 - row; column++ ) {
			VirtualSlot41( column * 0.5f, row * 0.5f );
		}
	}

	NiPoint2 *newTextures;
	if ( textures ) {
		newTextures = new NiPoint2[textureSets * 6];
		for ( unsigned short set = 0; set < textureSets; set++ ) {
			for ( row = 0, index = 0; row <= 2; row++ ) {
				for ( column = 0; column <= 2 - row; column++ ) {
					newTextures[set * 6 + index] = textureCoordinates->GetAt( set, index );
					index++;
				}
			}
		}
	} else {
		newTextures = 0;
	}

	NiColorA *newColors;
	if ( colors ) {
		newColors = new NiColorA[6];
		for ( row = 0, index = 0; row <= 2; row++ ) {
			for ( column = 0; column <= 2 - row; column++ ) {
				newColors[index] = vertexColors->GetAt( index );
				index++;
			}
		}
	} else {
		newColors = 0;
	}

	NiBezierTriangle2 *patch = new NiBezierTriangle2( newVertices, newTextures, newColors, GetUnknown08() );
	patch->Unknown83A650( textureSets );
	return patch;
}

/*
================
NiBezierTriangle3::Unknown83BFB0

Raises the cubic patch to degree 4: the corners stay, and each quartic
point is the blend of the cubic's neighbouring points by their
parameter distances (quarters). The texture coordinates and colours of
the quartic's control points are those of the patch evaluated at the
quartic's control parameters, through slot 41 into fresh sets.
================
*/
// FUNCTION: STBC 0x83BFB0
NiBezierTriangle4 *NiBezierTriangle3::Unknown83BFB0() {
	NiPoint3 *newVertices = new NiPoint3[15];
	CreateSets();

	if ( unknownE4 ) {
		newVertices[0] = vertices[unknownE4[0]];
		newVertices[4] = vertices[unknownE4[3]];
		newVertices[14] = vertices[unknownE4[9]];
		newVertices[1] = 0.25f * ( vertices[unknownE4[0]] + 3.0f * vertices[unknownE4[1]] );
		newVertices[2] = 0.25f * ( 2.0f * vertices[unknownE4[1]] + 2.0f * vertices[unknownE4[2]] );
		newVertices[3] = 0.25f * ( 3.0f * vertices[unknownE4[2]] + vertices[unknownE4[3]] );
		newVertices[5] = 0.25f * ( vertices[unknownE4[0]] + 3.0f * vertices[unknownE4[4]] );
		newVertices[9] = 0.25f * ( 2.0f * vertices[unknownE4[4]] + 2.0f * vertices[unknownE4[7]] );
		newVertices[12] = 0.25f * ( 3.0f * vertices[unknownE4[7]] + vertices[unknownE4[9]] );
		newVertices[8] = 0.25f * ( vertices[unknownE4[3]] + 3.0f * vertices[unknownE4[6]] );
		newVertices[11] = 0.25f * ( 2.0f * vertices[unknownE4[6]] + 2.0f * vertices[unknownE4[8]] );
		newVertices[13] = 0.25f * ( 3.0f * vertices[unknownE4[8]] + vertices[unknownE4[9]] );
		newVertices[6] = 0.25f * ( vertices[unknownE4[4]] + vertices[unknownE4[1]] + 2.0f * vertices[unknownE4[5]] );
		newVertices[7] = 0.25f * ( 2.0f * vertices[unknownE4[5]] + vertices[unknownE4[2]] + vertices[unknownE4[6]] );
		newVertices[10] = 0.25f * ( vertices[unknownE4[7]] + 2.0f * vertices[unknownE4[5]] + vertices[unknownE4[8]] );
	} else {
		newVertices[0] = vertices[0];
		newVertices[4] = vertices[3];
		newVertices[14] = vertices[9];
		newVertices[1] = 0.25f * ( vertices[0] + 3.0f * vertices[1] );
		newVertices[2] = 0.25f * ( 2.0f * vertices[1] + 2.0f * vertices[2] );
		newVertices[3] = 0.25f * ( 3.0f * vertices[2] + vertices[3] );
		newVertices[5] = 0.25f * ( vertices[0] + 3.0f * vertices[4] );
		newVertices[9] = 0.25f * ( 2.0f * vertices[4] + 2.0f * vertices[7] );
		newVertices[12] = 0.25f * ( 3.0f * vertices[7] + vertices[9] );
		newVertices[8] = 0.25f * ( vertices[3] + 3.0f * vertices[6] );
		newVertices[11] = 0.25f * ( 2.0f * vertices[6] + 2.0f * vertices[8] );
		newVertices[13] = 0.25f * ( 3.0f * vertices[8] + vertices[9] );
		newVertices[6] = 0.25f * ( vertices[4] + vertices[1] + 2.0f * vertices[5] );
		newVertices[7] = 0.25f * ( 2.0f * vertices[5] + vertices[2] + vertices[6] );
		newVertices[10] = 0.25f * ( vertices[7] + 2.0f * vertices[5] + vertices[8] );
	}

	int row;
	int column;
	int index;
	for ( row = 0; row <= 4; row++ ) {
		for ( column = 0; column <= 4 - row; column++ ) {
			VirtualSlot41( column * 0.25f, row * 0.25f );
		}
	}

	NiPoint2 *newTextures;
	if ( textures ) {
		newTextures = new NiPoint2[textureSets * 15];
		for ( unsigned short set = 0; set < textureSets; set++ ) {
			for ( row = 0, index = 0; row <= 4; row++ ) {
				for ( column = 0; column <= 4 - row; column++ ) {
					newTextures[set * 15 + index] = textureCoordinates->GetAt( set, index );
					index++;
				}
			}
		}
	} else {
		newTextures = 0;
	}

	NiColorA *newColors;
	if ( colors ) {
		newColors = new NiColorA[15];
		for ( row = 0, index = 0; row <= 4; row++ ) {
			for ( column = 0; column <= 4 - row; column++ ) {
				newColors[index] = vertexColors->GetAt( index );
				index++;
			}
		}
	} else {
		newColors = 0;
	}

	NiBezierTriangle4 *patch = new NiBezierTriangle4( newVertices, newTextures, newColors, GetUnknown08() );
	patch->Unknown83A650( textureSets );
	return patch;
}

/*
================
NiBezierTriangle3::CreateClone
================
*/
// FUNCTION: STBC 0x83D410
NiObject *NiBezierTriangle3::CreateClone() {
	NiBezierTriangle3 *tri = new NiBezierTriangle3;
	assert( tri );
	CopyMembers( tri );
	return tri;
}

/*
================
NiBezierTriangle3::CopyMembers
================
*/
// FUNCTION: STBC 0x83D4A0
void NiBezierTriangle3::CopyMembers( NiBezierTriangle3 *dest ) {
	NiBezierTriangle::CopyMembers( dest );

	if ( !dest->unknownE4 ) {
		dest->Unknown8330E0( dest->level );
	}
}

/*
================
NiBezierTriangle3::CreateObject

Creates the patch and loads it.
================
*/
// FUNCTION: STBC 0x83D4D0
void NiBezierTriangle3::CreateObject( NiStream &stream, NiObject *&object ) {
	NiBezierTriangle3 *triangle = new NiBezierTriangle3;
	stream.SetLoadRecord( new NiBezierTriangle3LoadRecord );
	triangle->LoadBinary( stream );
	object = triangle;
}

/*
================
NiBezierTriangle3::LoadBinary

Tessellates at the loaded level unless the patch owns unknownE4.
================
*/
// FUNCTION: STBC 0x83D5F0
void NiBezierTriangle3::LoadBinary( NiStream &stream ) {
	NiBezierTriangle::LoadBinary( stream );

	if ( !unknownE4 ) {
		Unknown8330E0( level );
	}
}

/*
================
NiBezierTriangle3::LinkObject
================
*/
// FUNCTION: STBC 0x83D620
void NiBezierTriangle3::LinkObject( NiStream &stream ) {
	NiBezierTriangle::LinkObject( stream );
}

/*
================
NiBezierTriangle3::RegisterStreamables
================
*/
// FUNCTION: STBC 0x83D630
bool NiBezierTriangle3::RegisterStreamables( NiStream &stream ) {
	return NiBezierTriangle::RegisterStreamables( stream );
}

/*
================
NiBezierTriangle3::SaveBinary
================
*/
// FUNCTION: STBC 0x83D640
void NiBezierTriangle3::SaveBinary( NiStream &stream ) {
	NiBezierTriangle::SaveBinary( stream );
}

/*
================
NiBezierTriangle3::IsEqual
================
*/
// FUNCTION: STBC 0x83D650
bool NiBezierTriangle3::IsEqual( NiObject *object ) {
	return NiBezierTriangle::IsEqual( object );
}
