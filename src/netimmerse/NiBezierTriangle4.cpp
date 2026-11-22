// NiBezierTriangle4.cpp -- triangular Bezier patches of degree 4

#include "NiRTLib.h"
#include "NiStream.h"
#include "NiTriShapeDynamicData.h"
#include "NiBezierTriangle2.h"
#include "NiBezierTriangle3.h"
#include "NiBezierTriangle4.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00842120 _$E17
// FUNCTION: STBC 0x00842140 _$E15
// FUNCTION: STBC 0x00842150 _$E23
// FUNCTION: STBC 0x00842170 _$E21
// FUNCTION: STBC 0x008421a0 _$E238

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0083d810 _$E229
// FUNCTION: STBC 0x0083d830 _$E226
// FUNCTION: STBC 0x0083d800 _$E223
// FUNCTION: STBC 0x00842180 _$E48
// FUNCTION: STBC 0x00842190 _$E72
// FUNCTION: STBC 0x008421c0 _$E76
// FUNCTION: STBC 0x008421d0 _$E80
// FUNCTION: STBC 0x008421e0 _$E84
// FUNCTION: STBC 0x008421f0 _$E197
// FUNCTION: STBC 0x00842200 _$E56
// FUNCTION: STBC 0x00842210 _$E60
// FUNCTION: STBC 0x00842220 _$E64
// FUNCTION: STBC 0x00842230 _$E68
// FUNCTION: STBC 0x00842240 _$E205
// FUNCTION: STBC 0x00842250 _$E211
// FUNCTION: STBC 0x00842260 _$E215
// FUNCTION: STBC 0x00842270 _$E219

// DATA: STBC 0x9B3160 ?rtti@NiBezierTriangle4@@2VNiRTTI@@B
const NiRTTI NiBezierTriangle4::rtti( "NiBezierTriangle4", &NiBezierTriangle::rtti );

/*
================
NiBezierTriangle4::Register
================
*/
// FUNCTION: STBC 0x83D850 ?Register@NiBezierTriangle4@@SAHXZ
NiImplementRegister( NiBezierTriangle4 )

/*
================
NiBezierTriangle4::NiBezierTriangle4
================
*/
// FUNCTION: STBC 0x83D9A0 ??0NiBezierTriangle4@@QAE@PAVNiPoint3@@PAVNiPoint2@@PAVNiColorA@@_N@Z
NiBezierTriangle4::NiBezierTriangle4( NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08 )
	: NiBezierTriangle( 4, vertices_, textures_, colors_, unknown08 ) {
}

/*
================
NiBezierTriangle4::NiBezierTriangle4
================
*/
// FUNCTION: STBC 0x83DA10 ??0NiBezierTriangle4@@QAE@PAGPAVNiPoint3@@PAVNiPoint2@@PAVNiColorA@@_NG@Z
NiBezierTriangle4::NiBezierTriangle4( unsigned short *unknownE4_, NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08, unsigned short unknownD6_ )
	: NiBezierTriangle( 4, unknownE4_, vertices_, textures_, colors_, unknown08, unknownD6_ ) {
}

/*
================
NiBezierTriangle4::VirtualSlot46

As NiBezierTriangle3::VirtualSlot46 for the quartic patch: the weights are
the second derivatives of the quartic Bernstein basis along the segment's
side direction at its midpoint, built from the squares (times 12) and the
cross products (times 24) of the barycentric coordinates.
================
*/
// FUNCTION: STBC 0x83DA50 ?VirtualSlot46@NiBezierTriangle4@@UAE_NGGGG@Z
// The statements are the debug copy's (AnimOnly.exe 0x4E4821).
bool NiBezierTriangle4::VirtualSlot46( unsigned short u0, unsigned short v0, unsigned short u1, unsigned short v1 ) {
	unsigned short u = ( u1 + u0 ) >> 1;
	unsigned short v = ( v1 + v0 ) >> 1;
	float sideLength;
	unsigned short start;
	unsigned short end;
	float weight[15];	// the second difference along the side, per control point
	if ( v0 == v1 ) {
		// along the first row
		sideLength = unknown160;
		end = u1;
		start = u0;
		float s = u * GRID_STEP;
		float t = v0 * GRID_STEP;
		float w = 1.0f - s - t;
		float ss = 12.0f * s * s;	// (names invented)
		float tt = 12.0f * t * t;
		float ww = 12.0f * w * w;
		float st = 24.0f * s * t;
		float sw = 24.0f * s * w;
		float tw = 24.0f * t * w;
		weight[0] = ww;
		weight[1] = sw - 2.0f * ww;
		weight[2] = ss + ww - 2.0f * sw;
		weight[3] = sw - 2.0f * ss;
		weight[4] = ss;
		weight[5] = tw;
		weight[6] = st - 2.0f * tw;
		weight[7] = tw - 2.0f * st;
		weight[8] = st;
		weight[9] = tt;
		weight[10] = -2.0f * tt;
		weight[11] = tt;
		weight[12] = 0.0f;
		weight[13] = 0.0f;
		weight[14] = 0.0f;
	} else if ( u0 == u1 ) {
		// along the first column
		sideLength = unknown164;
		end = v1;
		start = v0;
		float s = u0 * GRID_STEP;
		float t = v * GRID_STEP;
		float w = 1.0f - s - t;
		float ss = 12.0f * s * s;	// (names invented)
		float tt = 12.0f * t * t;
		float ww = 12.0f * w * w;
		float st = 24.0f * s * t;
		float sw = 24.0f * s * w;
		float tw = 24.0f * t * w;
		weight[0] = ww;
		weight[1] = sw;
		weight[2] = ss;
		weight[3] = 0.0f;
		weight[4] = 0.0f;
		weight[5] = tw - 2.0f * ww;
		weight[6] = st - 2.0f * sw;
		weight[7] = -2.0f * ss;
		weight[8] = 0.0f;
		weight[9] = tt + ww - 2.0f * tw;
		weight[10] = sw - 2.0f * st;
		weight[11] = ss;
		weight[12] = tw - 2.0f * tt;
		weight[13] = st;
		weight[14] = tt;
	} else {
		// along the diagonal
		sideLength = unknown168;
		end = v1;
		start = v0;
		float s = u * GRID_STEP;
		float t = v * GRID_STEP;
		float w = 1.0f - s - t;
		float ss = 12.0f * s * s;	// (names invented)
		float tt = 12.0f * t * t;
		float ww = 12.0f * w * w;
		float st = 24.0f * s * t;
		float sw = 24.0f * s * w;
		float tw = 24.0f * t * w;
		weight[0] = 0.0f;
		weight[1] = 0.0f;
		weight[2] = ww;
		weight[3] = sw;
		weight[4] = ss;
		weight[5] = 0.0f;
		weight[6] = -2.0f * ww;
		weight[7] = tw - 2.0f * sw;
		weight[8] = st - 2.0f * ss;
		weight[9] = ww;
		weight[10] = sw - 2.0f * tw;
		weight[11] = ss + tt - 2.0f * st;
		weight[12] = tw;
		weight[13] = st - 2.0f * tt;
		weight[14] = tt;
	}
	float step = ( end - start ) * GRID_STEP;

	NiPoint3 bend;
	if ( unknownE4 ) {
		bend = vertices[unknownE4[0]] * weight[0] + vertices[unknownE4[1]] * weight[1] + vertices[unknownE4[2]] * weight[2] +
			vertices[unknownE4[3]] * weight[3] + vertices[unknownE4[4]] * weight[4] + vertices[unknownE4[5]] * weight[5] +
			vertices[unknownE4[6]] * weight[6] + vertices[unknownE4[7]] * weight[7] + vertices[unknownE4[8]] * weight[8] +
			vertices[unknownE4[9]] * weight[9] + vertices[unknownE4[10]] * weight[10] + vertices[unknownE4[11]] * weight[11] +
			vertices[unknownE4[12]] * weight[12] + vertices[unknownE4[13]] * weight[13] + vertices[unknownE4[14]] * weight[14];
	} else {
		bend = vertices[0] * weight[0] + vertices[1] * weight[1] + vertices[2] * weight[2] +
			vertices[3] * weight[3] + vertices[4] * weight[4] + vertices[5] * weight[5] +
			vertices[6] * weight[6] + vertices[7] * weight[7] + vertices[8] * weight[8] +
			vertices[9] * weight[9] + vertices[10] * weight[10] + vertices[11] * weight[11] +
			vertices[12] * weight[12] + vertices[13] * weight[13] + vertices[14] * weight[14];
	}
	float bendLength = bend.x * bend.x + bend.y * bend.y + bend.z * bend.z;	// squared
	if ( bendLength * step * step > sideLength * unknownF8 ) {
		return true;
	}
	return false;
}

/*
================
NiBezierTriangle4::Unknown83EA40

Lowers the quartic patch to degree 3: the corners stay, the edge points
are the cubic edge's least-squares fit of the quartic edge, and the
centre is a blend of the quartic's control points less those of the new
corners and edges. The texture coordinates and colours of the cubic's
control points are those of the patch evaluated at the cubic's control
parameters (multiples of a third), through slot 41 into fresh sets.
================
*/
// FUNCTION: STBC 0x83EA40
// DRAFT: referent: bytes identical, but retail reads its third at 0x89AB5C (a second copy, the back end's fold, also read by NiBezierTriangle2::Unknown8395C0) where `1.0f / 3.0f` names the literal's copy (0x88BA90)
NiBezierTriangle3 *NiBezierTriangle4::Unknown83EA40() {
	NiPoint3 *newVertices = new NiPoint3[10];
	// unused: the debug copy declares them (AnimOnly.exe 0x4E5584), and
	// their constructors are inline sites that spend the release budget
	NiPoint3 point0;	// (name invented)
	NiPoint3 point1;	// (name invented)
	NiPoint2 texture;	// (name invented)
	NiColorA color( 0.0f, 0.0f, 0.0f, 0.0f );	// (name invented)
	float oneThird = 1.0f / 3.0f;
	float scale;
	CreateSets();

	if ( unknownE4 ) {
		newVertices[0] = vertices[unknownE4[0]];
		newVertices[3] = vertices[unknownE4[4]];
		newVertices[9] = vertices[unknownE4[14]];
		scale = 1.0f / 42.0f;
		newVertices[1] = scale * ( -11.0f * vertices[unknownE4[0]] + 44.0f * vertices[unknownE4[1]] + 18.0f * vertices[unknownE4[2]] - 12.0f * vertices[unknownE4[3]] + 3.0f * vertices[unknownE4[4]] );
		newVertices[2] = scale * ( 3.0f * vertices[unknownE4[0]] - 12.0f * vertices[unknownE4[1]] + 18.0f * vertices[unknownE4[2]] + 44.0f * vertices[unknownE4[3]] - 11.0f * vertices[unknownE4[4]] );
		newVertices[4] = scale * ( -11.0f * vertices[unknownE4[0]] + 44.0f * vertices[unknownE4[5]] + 18.0f * vertices[unknownE4[9]] - 12.0f * vertices[unknownE4[12]] + 3.0f * vertices[unknownE4[14]] );
		newVertices[7] = scale * ( 3.0f * vertices[unknownE4[0]] - 12.0f * vertices[unknownE4[5]] + 18.0f * vertices[unknownE4[9]] + 44.0f * vertices[unknownE4[12]] - 11.0f * vertices[unknownE4[14]] );
		newVertices[6] = scale * ( -11.0f * vertices[unknownE4[4]] + 44.0f * vertices[unknownE4[8]] + 18.0f * vertices[unknownE4[11]] - 12.0f * vertices[unknownE4[13]] + 3.0f * vertices[unknownE4[14]] );
		newVertices[8] = scale * ( 3.0f * vertices[unknownE4[4]] - 12.0f * vertices[unknownE4[8]] + 18.0f * vertices[unknownE4[11]] + 44.0f * vertices[unknownE4[13]] - 11.0f * vertices[unknownE4[14]] );
		scale = 1.0f / 2520.0f;
		newVertices[5] = scale * ( 5.0f * ( vertices[unknownE4[0]] + vertices[unknownE4[4]] + vertices[unknownE4[14]] ) + 8.0f * ( vertices[unknownE4[1]] + vertices[unknownE4[3]] + vertices[unknownE4[5]] + vertices[unknownE4[8]] + vertices[unknownE4[12]] + vertices[unknownE4[13]] ) + 9.0f * ( vertices[unknownE4[2]] + vertices[unknownE4[9]] + vertices[unknownE4[11]] ) + 12.0f * ( vertices[unknownE4[6]] + vertices[unknownE4[7]] + vertices[unknownE4[10]] ) );
		scale = 1.0f / 280.0f;
		newVertices[5] -= scale * ( newVertices[0] + newVertices[3] + newVertices[9] );
		scale = 3.0f / 560.0f;
		newVertices[5] -= scale * ( newVertices[1] + newVertices[2] + newVertices[4] + newVertices[6] + newVertices[7] + newVertices[8] );
	} else {
		newVertices[0] = vertices[0];
		newVertices[3] = vertices[4];
		newVertices[9] = vertices[14];
		scale = 1.0f / 42.0f;
		newVertices[1] = scale * ( -11.0f * vertices[0] + 44.0f * vertices[1] + 18.0f * vertices[2] - 12.0f * vertices[3] + 3.0f * vertices[4] );
		newVertices[2] = scale * ( 3.0f * vertices[0] - 12.0f * vertices[1] + 18.0f * vertices[2] + 44.0f * vertices[3] - 11.0f * vertices[4] );
		newVertices[4] = scale * ( -11.0f * vertices[0] + 44.0f * vertices[5] + 18.0f * vertices[9] - 12.0f * vertices[12] + 3.0f * vertices[14] );
		newVertices[7] = scale * ( 3.0f * vertices[0] - 12.0f * vertices[5] + 18.0f * vertices[9] + 44.0f * vertices[12] - 11.0f * vertices[14] );
		newVertices[6] = scale * ( -11.0f * vertices[4] + 44.0f * vertices[8] + 18.0f * vertices[11] - 12.0f * vertices[13] + 3.0f * vertices[14] );
		newVertices[8] = scale * ( 3.0f * vertices[4] - 12.0f * vertices[8] + 18.0f * vertices[11] + 44.0f * vertices[13] - 11.0f * vertices[14] );
		scale = 1.0f / 2520.0f;
		newVertices[5] = scale * ( 5.0f * ( vertices[0] + vertices[4] + vertices[14] ) + 8.0f * ( vertices[1] + vertices[3] + vertices[5] + vertices[8] + vertices[12] + vertices[13] ) + 9.0f * ( vertices[2] + vertices[9] + vertices[11] ) + 12.0f * ( vertices[6] + vertices[7] + vertices[10] ) );
		scale = 1.0f / 280.0f;
		newVertices[5] -= scale * ( newVertices[0] + newVertices[3] + newVertices[9] );
		scale = 3.0f / 560.0f;
		newVertices[5] -= scale * ( newVertices[1] + newVertices[2] + newVertices[4] + newVertices[6] + newVertices[7] + newVertices[8] );
	}

	int row;
	int column;
	int index;
	// BUG: the cubic has four rows of points, but this walks five (the
	// quartic's), evaluating past the patch at parameters up to 4/3
	for ( row = 0; row <= 4; row++ ) {
		for ( column = 0; column <= 4 - row; column++ ) {
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
NiBezierTriangle4::Unknown840F10

Lowers the quartic patch to degree 2: the corners stay, and each edge's
quadratic point is the least-squares fit of the quartic edge. The
texture coordinates and colours of the quadratic's control points are
those of the patch evaluated at the quadratic's control parameters
(multiples of a half), through slot 41 into fresh sets.
================
*/
// FUNCTION: STBC 0x840F10
NiBezierTriangle2 *NiBezierTriangle4::Unknown840F10() {
	NiPoint3 *newVertices = new NiPoint3[6];
	// unused: the debug copy declares them (AnimOnly.exe 0x4E7130), and
	// their constructors are inline sites that spend the release budget
	NiPoint3 point0;	// (name invented)
	NiPoint3 point1;	// (name invented)
	NiPoint2 texture;	// (name invented)
	NiColorA color( 0.0f, 0.0f, 0.0f, 0.0f );	// (name invented)
	float scale = 1.0f / 28.0f;
	CreateSets();

	if ( unknownE4 ) {
		newVertices[0] = vertices[unknownE4[0]];
		newVertices[2] = vertices[unknownE4[4]];
		newVertices[5] = vertices[unknownE4[14]];
		newVertices[1] = scale * ( -11.0f * vertices[unknownE4[0]] + 16.0f * vertices[unknownE4[1]] + 18.0f * vertices[unknownE4[2]] + 16.0f * vertices[unknownE4[3]] - 11.0f * vertices[unknownE4[4]] );
		newVertices[3] = scale * ( -11.0f * vertices[unknownE4[0]] + 16.0f * vertices[unknownE4[5]] + 18.0f * vertices[unknownE4[9]] + 16.0f * vertices[unknownE4[12]] - 11.0f * vertices[unknownE4[14]] );
		newVertices[4] = scale * ( -11.0f * vertices[unknownE4[4]] + 16.0f * vertices[unknownE4[8]] + 18.0f * vertices[unknownE4[11]] + 16.0f * vertices[unknownE4[13]] - 11.0f * vertices[unknownE4[14]] );
	} else {
		newVertices[0] = vertices[0];
		newVertices[2] = vertices[4];
		newVertices[5] = vertices[14];
		newVertices[1] = scale * ( -11.0f * vertices[0] + 16.0f * vertices[1] + 18.0f * vertices[2] + 16.0f * vertices[3] - 11.0f * vertices[4] );
		newVertices[3] = scale * ( -11.0f * vertices[0] + 16.0f * vertices[5] + 18.0f * vertices[9] + 16.0f * vertices[12] - 11.0f * vertices[14] );
		newVertices[4] = scale * ( -11.0f * vertices[4] + 16.0f * vertices[8] + 18.0f * vertices[11] + 16.0f * vertices[13] - 11.0f * vertices[14] );
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
NiBezierTriangle4::CreateClone
================
*/
// FUNCTION: STBC 0x841ED0
NiObject *NiBezierTriangle4::CreateClone() {
	NiBezierTriangle4 *tri = new NiBezierTriangle4;
	assert( tri );
	CopyMembers( tri );
	return tri;
}

/*
================
NiBezierTriangle4::CopyMembers
================
*/
// FUNCTION: STBC 0x841F60
void NiBezierTriangle4::CopyMembers( NiBezierTriangle4 *dest ) {
	NiBezierTriangle::CopyMembers( dest );

	if ( !dest->unknownE4 ) {
		dest->Unknown8330E0( dest->level );
	}
}

/*
================
NiBezierTriangle4::CreateObject

Creates the patch and loads it.
================
*/
// FUNCTION: STBC 0x841F90
void NiBezierTriangle4::CreateObject( NiStream &stream, NiObject *&object ) {
	NiBezierTriangle4 *triangle = new NiBezierTriangle4;
	stream.SetLoadRecord( new NiBezierTriangle4LoadRecord );
	triangle->LoadBinary( stream );
	object = triangle;
}

/*
================
NiBezierTriangle4::LoadBinary

Tessellates at the loaded level unless the patch owns unknownE4.
================
*/
// FUNCTION: STBC 0x8420B0
void NiBezierTriangle4::LoadBinary( NiStream &stream ) {
	NiBezierTriangle::LoadBinary( stream );

	if ( !unknownE4 ) {
		Unknown8330E0( level );
	}
}

/*
================
NiBezierTriangle4::LinkObject
================
*/
// FUNCTION: STBC 0x8420E0
void NiBezierTriangle4::LinkObject( NiStream &stream ) {
	NiBezierTriangle::LinkObject( stream );
}

/*
================
NiBezierTriangle4::RegisterStreamables
================
*/
// FUNCTION: STBC 0x8420F0
bool NiBezierTriangle4::RegisterStreamables( NiStream &stream ) {
	return NiBezierTriangle::RegisterStreamables( stream );
}

/*
================
NiBezierTriangle4::SaveBinary
================
*/
// FUNCTION: STBC 0x842100
void NiBezierTriangle4::SaveBinary( NiStream &stream ) {
	NiBezierTriangle::SaveBinary( stream );
}

/*
================
NiBezierTriangle4::IsEqual
================
*/
// FUNCTION: STBC 0x842110
bool NiBezierTriangle4::IsEqual( NiObject *object ) {
	return NiBezierTriangle::IsEqual( object );
}
