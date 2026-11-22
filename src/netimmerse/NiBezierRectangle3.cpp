// NiBezierRectangle3.cpp -- triangular Bezier patches of degree 3

#include "NiRTLib.h"
#include "NiStream.h"
#include "NiTriShapeDynamicData.h"
#include "NiBezierRectangle2.h"
#include "NiBezierRectangle3.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x0084c8d0 _$E17
// FUNCTION: STBC 0x0084c8f0 _$E15
// FUNCTION: STBC 0x0084c900 _$E23
// FUNCTION: STBC 0x0084c920 _$E21
// FUNCTION: STBC 0x0084c930 _$E233

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00849310 _$E225
// FUNCTION: STBC 0x00849330 _$E222
// FUNCTION: STBC 0x00849300 _$E219
// FUNCTION: STBC 0x0084c950 _$E48
// FUNCTION: STBC 0x0084c960 _$E72
// FUNCTION: STBC 0x0084c970 _$E76
// FUNCTION: STBC 0x0084c980 _$E80
// FUNCTION: STBC 0x0084c990 _$E84
// FUNCTION: STBC 0x0084c9a0 _$E197
// FUNCTION: STBC 0x0084c9b0 _$E56
// FUNCTION: STBC 0x0084c9c0 _$E60
// FUNCTION: STBC 0x0084c9d0 _$E64
// FUNCTION: STBC 0x0084c9e0 _$E68
// FUNCTION: STBC 0x0084c9f0 _$E205
// FUNCTION: STBC 0x0084ca00 _$E211
// FUNCTION: STBC 0x0084ca10 _$E215

// DATA: STBC 0x9B3268 ?rtti@NiBezierRectangle3@@2VNiRTTI@@B
const NiRTTI NiBezierRectangle3::rtti( "NiBezierRectangle3", &NiBezierRectangle::rtti );

/*
================
NiBezierRectangle3::Register
================
*/
// FUNCTION: STBC 0x849350 ?Register@NiBezierRectangle3@@SAHXZ
NiImplementRegister( NiBezierRectangle3 )

/*
================
NiBezierRectangle3::NiBezierRectangle3
================
*/
// FUNCTION: STBC 0x8494A0 ??0NiBezierRectangle3@@QAE@PAVNiPoint3@@PAVNiPoint2@@PAVNiColorA@@_N@Z
NiBezierRectangle3::NiBezierRectangle3( NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08 )
	: NiBezierRectangle( 3, vertices_, textures_, colors_, unknown08 ) {
}

/*
================
NiBezierRectangle3::NiBezierRectangle3
================
*/
// FUNCTION: STBC 0x849510 ??0NiBezierRectangle3@@QAE@PAGPAVNiPoint3@@PAVNiPoint2@@PAVNiColorA@@_NG@Z
NiBezierRectangle3::NiBezierRectangle3( unsigned short *unknownE4_, NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08, unsigned short unknownD6_ )
	: NiBezierRectangle( 3, unknownE4_, vertices_, textures_, colors_, unknown08, unknownD6_ ) {
}

/*
================
NiBezierRectangle3::VirtualSlot46

As NiBezierRectangle2::VirtualSlot46 for the cubic patch: the row (or
column) through the segment is a cubic curve whose four control points
blend the patch's columns (or rows) at the segment's parameter; its second
derivative at the segment's midpoint, scaled by the segment's parameter
length squared, is tested against unknownF8 times the squared chord of
that curve.
================
*/
// FUNCTION: STBC 0x849550 ?VirtualSlot46@NiBezierRectangle3@@UAE_NGGGG@Z
// DRAFT: inline budget: the /Od body equals AnimOnly 0x4EFAF1 but for frame slots; retail expands two more operator* and two more operator+ and calls four more NiPoint3 constructors out of line
bool NiBezierRectangle3::VirtualSlot46( unsigned short u0, unsigned short v0, unsigned short u1, unsigned short v1 ) {
	NiPoint3 points[4];
	unsigned short start;
	unsigned short end;
	if ( v0 == v1 ) {
		// along a row: the curve in u at v
		end = u1;
		start = u0;
		float t = v0 * GRID_STEP;
		float s = 1.0f - t;
		float st = t * s;
		float weight0 = s * s * s;
		float weight1 = 3.0f * st * s;
		float weight2 = 3.0f * t * st;
		float weight3 = t * t * t;
		if ( unknownE4 ) {
			points[0] = weight0 * vertices[unknownE4[0]] + weight1 * vertices[unknownE4[4]] + weight2 * vertices[unknownE4[8]] + weight3 * vertices[unknownE4[12]];
			points[1] = weight0 * vertices[unknownE4[1]] + weight1 * vertices[unknownE4[5]] + weight2 * vertices[unknownE4[9]] + weight3 * vertices[unknownE4[13]];
			points[2] = weight0 * vertices[unknownE4[2]] + weight1 * vertices[unknownE4[6]] + weight2 * vertices[unknownE4[10]] + weight3 * vertices[unknownE4[14]];
			points[3] = weight0 * vertices[unknownE4[3]] + weight1 * vertices[unknownE4[7]] + weight2 * vertices[unknownE4[11]] + weight3 * vertices[unknownE4[15]];
		} else {
			points[0] = weight0 * vertices[0] + weight1 * vertices[4] + weight2 * vertices[8] + weight3 * vertices[12];
			points[1] = weight0 * vertices[1] + weight1 * vertices[5] + weight2 * vertices[9] + weight3 * vertices[13];
			points[2] = weight0 * vertices[2] + weight1 * vertices[6] + weight2 * vertices[10] + weight3 * vertices[14];
			points[3] = weight0 * vertices[3] + weight1 * vertices[7] + weight2 * vertices[11] + weight3 * vertices[15];
		}
	} else {
		// along a column: the curve in v at u
		end = v1;
		start = v0;
		float t = u0 * GRID_STEP;
		float s = 1.0f - t;
		float st = t * s;
		float weight0 = s * s * s;
		float weight1 = 3.0f * st * s;
		float weight2 = 3.0f * t * st;
		float weight3 = t * t * t;
		if ( unknownE4 ) {
			points[0] = weight0 * vertices[unknownE4[0]] + weight1 * vertices[unknownE4[1]] + weight2 * vertices[unknownE4[2]] + weight3 * vertices[unknownE4[3]];
			points[1] = weight0 * vertices[unknownE4[4]] + weight1 * vertices[unknownE4[5]] + weight2 * vertices[unknownE4[6]] + weight3 * vertices[unknownE4[7]];
			points[2] = weight0 * vertices[unknownE4[8]] + weight1 * vertices[unknownE4[9]] + weight2 * vertices[unknownE4[10]] + weight3 * vertices[unknownE4[11]];
			points[3] = weight0 * vertices[unknownE4[12]] + weight1 * vertices[unknownE4[13]] + weight2 * vertices[unknownE4[14]] + weight3 * vertices[unknownE4[15]];
		} else {
			points[0] = weight0 * vertices[0] + weight1 * vertices[1] + weight2 * vertices[2] + weight3 * vertices[3];
			points[1] = weight0 * vertices[4] + weight1 * vertices[5] + weight2 * vertices[6] + weight3 * vertices[7];
			points[2] = weight0 * vertices[8] + weight1 * vertices[9] + weight2 * vertices[10] + weight3 * vertices[11];
			points[3] = weight0 * vertices[12] + weight1 * vertices[13] + weight2 * vertices[14] + weight3 * vertices[15];
		}
	}

	NiPoint3 chord = points[3] - points[0];
	float chordLength = chord.x * chord.x + chord.y * chord.y + chord.z * chord.z;
	float step = ( end - start ) * GRID_STEP;
	float middle = ( ( end + start ) >> 1 ) * GRID_STEP;
	float oneMinusMiddle = 1.0f - middle;
	NiPoint3 bend = 6.0f * ( oneMinusMiddle * ( points[2] - 2.0f * points[1] + points[0] ) + middle * ( points[3] - 2.0f * points[2] + points[1] ) );
	float bendLength = bend.x * bend.x + bend.y * bend.y + bend.z * bend.z;
	if ( bendLength * step * step > chordLength * unknownF8 ) {
		return true;
	} else {
		return false;
	}
}

/*
================
NiBezierRectangle3::Unknown84ACF0

Lowers the bicubic patch to degree 2: the corners stay, each edge's
quadratic point reproduces the cubic edge's midpoint,
( 3 ( P1 + P2 ) - P0 - P3 ) / 4, and the centre is the tensor product of
that rule. The texture coordinates and colours of the quadratic's control
points are those of the patch evaluated at the quadratic's control
parameters (multiples of a half), through slot 41 into fresh sets.
================
*/
// FUNCTION: STBC 0x84ACF0
NiBezierRectangle2 *NiBezierRectangle3::Unknown84ACF0() {
	NiPoint3 *newVertices = new NiPoint3[9];
	CreateSets();

	if ( unknownE4 ) {
		newVertices[0] = vertices[unknownE4[0]];
		newVertices[2] = vertices[unknownE4[3]];
		newVertices[6] = vertices[unknownE4[12]];
		newVertices[8] = vertices[unknownE4[15]];
		newVertices[3] = 0.25f * ( -vertices[unknownE4[0]] + 3.0f * vertices[unknownE4[4]] + 3.0f * vertices[unknownE4[8]] - vertices[unknownE4[12]] );
		newVertices[1] = 0.25f * ( -vertices[unknownE4[0]] + 3.0f * vertices[unknownE4[1]] + 3.0f * vertices[unknownE4[2]] - vertices[unknownE4[3]] );
		newVertices[7] = 0.25f * ( -vertices[unknownE4[12]] + 3.0f * vertices[unknownE4[13]] + 3.0f * vertices[unknownE4[14]] - vertices[unknownE4[15]] );
		newVertices[5] = 0.25f * ( -vertices[unknownE4[3]] + 3.0f * vertices[unknownE4[7]] + 3.0f * vertices[unknownE4[11]] - vertices[unknownE4[15]] );
		newVertices[4] = 0.0625f * ( vertices[unknownE4[0]] - 3.0f * vertices[unknownE4[4]] - 3.0f * vertices[unknownE4[8]] + vertices[unknownE4[12]] - 3.0f * vertices[unknownE4[1]] + 9.0f * vertices[unknownE4[5]] + 9.0f * vertices[unknownE4[9]] - 3.0f * vertices[unknownE4[13]] -
			3.0f * vertices[unknownE4[2]] + 9.0f * vertices[unknownE4[6]] + 9.0f * vertices[unknownE4[10]] - 3.0f * vertices[unknownE4[14]] + vertices[unknownE4[3]] - 3.0f * vertices[unknownE4[7]] - 3.0f * vertices[unknownE4[11]] + vertices[unknownE4[15]] );
	} else {
		newVertices[0] = vertices[0];
		newVertices[2] = vertices[3];
		newVertices[6] = vertices[12];
		newVertices[8] = vertices[15];
		newVertices[3] = 0.25f * ( -vertices[0] + 3.0f * vertices[4] + 3.0f * vertices[8] - vertices[12] );
		newVertices[1] = 0.25f * ( -vertices[0] + 3.0f * vertices[1] + 3.0f * vertices[2] - vertices[3] );
		newVertices[7] = 0.25f * ( -vertices[12] + 3.0f * vertices[13] + 3.0f * vertices[14] - vertices[15] );
		newVertices[5] = 0.25f * ( -vertices[3] + 3.0f * vertices[7] + 3.0f * vertices[11] - vertices[15] );
		newVertices[4] = 0.0625f * ( vertices[0] - 3.0f * vertices[4] - 3.0f * vertices[8] + vertices[12] - 3.0f * vertices[1] + 9.0f * vertices[5] + 9.0f * vertices[9] - 3.0f * vertices[13] -
			3.0f * vertices[2] + 9.0f * vertices[6] + 9.0f * vertices[10] - 3.0f * vertices[14] + vertices[3] - 3.0f * vertices[7] - 3.0f * vertices[11] + vertices[15] );
	}

	int row;
	int column;
	for ( row = 0; row < 3; row++ ) {
		for ( column = 0; column < 3; column++ ) {
			VirtualSlot41( column * 0.5f, row * 0.5f );
		}
	}

	NiPoint2 *newTextures;
	if ( textures ) {
		newTextures = new NiPoint2[textureSets * 9];
		for ( unsigned short set = 0; set < textureSets; set++ ) {
			for ( row = 0; row < 3; row++ ) {
				for ( column = 0; column < 3; column++ ) {
					newTextures[set * 9 + row * 3 + column] = textureCoordinates->GetAt( set, row * 3 + column );
				}
			}
		}
	} else {
		newTextures = 0;
	}

	NiColorA *newColors;
	if ( colors ) {
		newColors = new NiColorA[9];
		for ( row = 0; row < 3; row++ ) {
			for ( column = 0; column < 3; column++ ) {
				newColors[row * 3 + column] = vertexColors->GetAt( row * 3 + column );
			}
		}
	} else {
		newColors = 0;
	}

	NiBezierRectangle2 *patch = new NiBezierRectangle2( newVertices, newTextures, newColors, GetUnknown08() );
	patch->Unknown83A650( textureSets );
	return patch;
}

/*
================
NiBezierRectangle3::CreateClone
================
*/
// FUNCTION: STBC 0x84C680
NiObject *NiBezierRectangle3::CreateClone() {
	NiBezierRectangle3 *rect = new NiBezierRectangle3;
	assert( rect );
	CopyMembers( rect );
	return rect;
}

/*
================
NiBezierRectangle3::CopyMembers
================
*/
// FUNCTION: STBC 0x84C710
void NiBezierRectangle3::CopyMembers( NiBezierRectangle3 *dest ) {
	NiBezierRectangle::CopyMembers( dest );

	if ( !dest->unknownE4 ) {
		dest->Unknown8330E0( dest->level );
	}
}

/*
================
NiBezierRectangle3::CreateObject

Creates the patch and loads it.
================
*/
// FUNCTION: STBC 0x84C740
void NiBezierRectangle3::CreateObject( NiStream &stream, NiObject *&object ) {
	NiBezierRectangle3 *rectangle = new NiBezierRectangle3;
	stream.SetLoadRecord( new NiBezierRectangle3LoadRecord );
	rectangle->LoadBinary( stream );
	object = rectangle;
}

/*
================
NiBezierRectangle3::LoadBinary

Tessellates at the loaded level unless the patch owns unknownE4.
================
*/
// FUNCTION: STBC 0x84C860
void NiBezierRectangle3::LoadBinary( NiStream &stream ) {
	NiBezierRectangle::LoadBinary( stream );

	if ( !unknownE4 ) {
		Unknown8330E0( level );
	}
}

/*
================
NiBezierRectangle3::LinkObject
================
*/
// FUNCTION: STBC 0x84C890
void NiBezierRectangle3::LinkObject( NiStream &stream ) {
	NiBezierRectangle::LinkObject( stream );
}

/*
================
NiBezierRectangle3::RegisterStreamables
================
*/
// FUNCTION: STBC 0x84C8A0
bool NiBezierRectangle3::RegisterStreamables( NiStream &stream ) {
	return NiBezierRectangle::RegisterStreamables( stream );
}

/*
================
NiBezierRectangle3::SaveBinary
================
*/
// FUNCTION: STBC 0x84C8B0
void NiBezierRectangle3::SaveBinary( NiStream &stream ) {
	NiBezierRectangle::SaveBinary( stream );
}

/*
================
NiBezierRectangle3::IsEqual
================
*/
// FUNCTION: STBC 0x84C8C0
bool NiBezierRectangle3::IsEqual( NiObject *object ) {
	return NiBezierRectangle::IsEqual( object );
}
