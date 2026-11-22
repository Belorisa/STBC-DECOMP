// NiBezierRectangle2.cpp -- rectangular Bezier patches of degree 2

#include "NiRTLib.h"
#include "NiStream.h"
#include "NiBezierRectangle2.h"
#include "NiBezierTriangle.h"
#include "NiBezierTriangle2.h"
#include "NiBezierTriangle3.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00849170 _$E17
// FUNCTION: STBC 0x00849190 _$E15
// FUNCTION: STBC 0x008491a0 _$E23
// FUNCTION: STBC 0x008491c0 _$E21
// FUNCTION: STBC 0x008491d0 _$E242

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00847fa0 _$E235
// FUNCTION: STBC 0x00847fc0 _$E232
// FUNCTION: STBC 0x008491f0 _$E37
// FUNCTION: STBC 0x00849200 _$E41
// FUNCTION: STBC 0x00849210 _$E45
// FUNCTION: STBC 0x00849220 _$E49
// FUNCTION: STBC 0x00849230 _$E53
// FUNCTION: STBC 0x00849240 _$E177
// FUNCTION: STBC 0x00849250 _$E185
// FUNCTION: STBC 0x00849260 _$E189
// FUNCTION: STBC 0x00849270 _$E193
// FUNCTION: STBC 0x00849280 _$E197
// FUNCTION: STBC 0x00849290 _$E205
// FUNCTION: STBC 0x008492a0 _$E211
// FUNCTION: STBC 0x008492f0 _$E215

// DATA: STBC 0x9B3218 ?rtti@NiBezierRectangle2@@2VNiRTTI@@B
const NiRTTI NiBezierRectangle2::rtti( "NiBezierRectangle2", &NiBezierRectangle::rtti );

/*
================
NiBezierRectangle2::Register
================
*/
// FUNCTION: STBC 0x847FE0 ?Register@NiBezierRectangle2@@SAHXZ
NiImplementRegister( NiBezierRectangle2 )

/*
================
NiBezierRectangle2::NiBezierRectangle2
================
*/
// FUNCTION: STBC 0x848130 ??0NiBezierRectangle2@@QAE@PAVNiPoint3@@PAVNiPoint2@@PAVNiColorA@@_N@Z
NiBezierRectangle2::NiBezierRectangle2( NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08 )
	: NiBezierRectangle( 2, vertices_, textures_, colors_, unknown08 ) {
}

/*
================
NiBezierRectangle2::NiBezierRectangle2
================
*/
// FUNCTION: STBC 0x848160 ??0NiBezierRectangle2@@QAE@PAGPAVNiPoint3@@PAVNiPoint2@@PAVNiColorA@@_NG@Z
NiBezierRectangle2::NiBezierRectangle2( unsigned short *unknownE4_, NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08, unsigned short unknownD6_ )
	: NiBezierRectangle( 2, unknownE4_, vertices_, textures_, colors_, unknown08, unknownD6_ ) {
}

/*
================
NiBezierRectangle2::VirtualSlot46

Whether the grid segment bends enough to split: the patch's row (or
column) through the segment is a quadratic curve whose control points
blend the patch's columns (or rows) at the segment's parameter; its second
derivative, scaled by the segment's parameter length squared, is tested
against unknownF8 times the squared chord of that curve.
================
*/
// FUNCTION: STBC 0x8481A0 ?VirtualSlot46@NiBezierRectangle2@@UAE_NGGGG@Z
// DRAFT: inline budget: the /Od body equals AnimOnly 0x4EE6BA but for frame slots; retail expands two more operator* and one more NiPoint3::operator= (35 units short here)
bool NiBezierRectangle2::VirtualSlot46( unsigned short u0, unsigned short v0, unsigned short u1, unsigned short v1 ) {
	NiPoint3 points[3];
	unsigned short start;
	unsigned short end;
	if ( v0 == v1 ) {
		// along a row: the curve in u at v
		end = u1;
		start = u0;
		float t = v0 * GRID_STEP;
		float s = 1.0f - t;
		float weight0 = s * s;
		float weight1 = 2.0f * t * s;
		float weight2 = t * t;
		// BUG: the second and third curve points take control points 2
		// and 3 where 1 and 2 belong
		if ( unknownE4 ) {
			points[0] = weight0 * vertices[unknownE4[0]] + weight1 * vertices[unknownE4[3]] + weight2 * vertices[unknownE4[6]];
			points[1] = weight0 * vertices[unknownE4[2]] + weight1 * vertices[unknownE4[4]] + weight2 * vertices[unknownE4[7]];
			points[2] = weight0 * vertices[unknownE4[3]] + weight1 * vertices[unknownE4[5]] + weight2 * vertices[unknownE4[8]];
		} else {
			points[0] = weight0 * vertices[0] + weight1 * vertices[3] + weight2 * vertices[6];
			points[1] = weight0 * vertices[2] + weight1 * vertices[4] + weight2 * vertices[7];
			points[2] = weight0 * vertices[3] + weight1 * vertices[5] + weight2 * vertices[8];
		}
	} else {
		// along a column: the curve in v at u
		end = v1;
		start = v0;
		float t = u0 * GRID_STEP;
		float s = 1.0f - t;
		float weight0 = s * s;
		float weight1 = 2.0f * t * s;
		float weight2 = t * t;
		if ( unknownE4 ) {
			points[0] = weight0 * vertices[unknownE4[0]] + weight1 * vertices[unknownE4[1]] + weight2 * vertices[unknownE4[2]];
			points[1] = weight0 * vertices[unknownE4[3]] + weight1 * vertices[unknownE4[4]] + weight2 * vertices[unknownE4[5]];
			points[2] = weight0 * vertices[unknownE4[6]] + weight1 * vertices[unknownE4[7]] + weight2 * vertices[unknownE4[8]];
		} else {
			points[0] = weight0 * vertices[0] + weight1 * vertices[1] + weight2 * vertices[2];
			points[1] = weight0 * vertices[3] + weight1 * vertices[4] + weight2 * vertices[5];
			points[2] = weight0 * vertices[6] + weight1 * vertices[7] + weight2 * vertices[8];
		}
	}

	NiPoint3 chord = points[2] - points[0];
	float chordLength = chord.x * chord.x + chord.y * chord.y + chord.z * chord.z;
	float step = ( end - start ) * GRID_STEP;
	NiPoint3 bend = 2.0f * ( points[2] - 2.0f * points[1] + points[0] );
	float bendLength = bend.x * bend.x + bend.y * bend.y + bend.z * bend.z;
	if ( bendLength * step * step > chordLength * unknownF8 ) {
		return true;
	} else {
		return false;
	}
}

/*
================
NiBezierRectangle2::CreateClone
================
*/
// FUNCTION: STBC 0x848F20
NiObject *NiBezierRectangle2::CreateClone() {
	NiBezierRectangle2 *rect = new NiBezierRectangle2;
	assert( rect );
	CopyMembers( rect );
	return rect;
}

/*
================
NiBezierRectangle2::CopyMembers
================
*/
// FUNCTION: STBC 0x848FB0
void NiBezierRectangle2::CopyMembers( NiBezierRectangle2 *dest ) {
	NiBezierRectangle::CopyMembers( dest );

	if ( !dest->unknownE4 ) {
		dest->Unknown8330E0( dest->level );
	}
}

/*
================
NiBezierRectangle2::CreateObject

Creates the patch and loads it.
================
*/
// FUNCTION: STBC 0x848FE0
void NiBezierRectangle2::CreateObject( NiStream &stream, NiObject *&object ) {
	NiBezierRectangle2 *rectangle = new NiBezierRectangle2;
	stream.SetLoadRecord( new NiBezierRectangle2LoadRecord );
	rectangle->LoadBinary( stream );
	object = rectangle;
}

/*
================
NiBezierRectangle2::LoadBinary

Tessellates at the loaded level unless the patch owns unknownE4.
================
*/
// FUNCTION: STBC 0x849100
void NiBezierRectangle2::LoadBinary( NiStream &stream ) {
	NiBezierRectangle::LoadBinary( stream );

	if ( !unknownE4 ) {
		Unknown8330E0( level );
	}
}

/*
================
NiBezierRectangle2::LinkObject
================
*/
// FUNCTION: STBC 0x849130
void NiBezierRectangle2::LinkObject( NiStream &stream ) {
	NiBezierRectangle::LinkObject( stream );
}

/*
================
NiBezierRectangle2::RegisterStreamables
================
*/
// FUNCTION: STBC 0x849140
bool NiBezierRectangle2::RegisterStreamables( NiStream &stream ) {
	return NiBezierRectangle::RegisterStreamables( stream );
}

/*
================
NiBezierRectangle2::SaveBinary
================
*/
// FUNCTION: STBC 0x849150
void NiBezierRectangle2::SaveBinary( NiStream &stream ) {
	NiBezierRectangle::SaveBinary( stream );
}

/*
================
NiBezierRectangle2::IsEqual
================
*/
// FUNCTION: STBC 0x849160
bool NiBezierRectangle2::IsEqual( NiObject *object ) {
	return NiBezierRectangle::IsEqual( object );
}
