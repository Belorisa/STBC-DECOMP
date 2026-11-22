// NiBezierRectangle.cpp -- rectangular Bezier patches

#include "NiRTLib.h"
#include "NiStream.h"
#include "NiBezierRectangle.h"
#include "NiCamera.h"
#include "renderer/NiRender.h"
#include "NiBezierTriangle.h"
#include "NiBezierTriangle2.h"
#include "NiBezierTriangle3.h"
#include "NiNode.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00847da0 _$E17
// FUNCTION: STBC 0x00847dc0 _$E15
// FUNCTION: STBC 0x00847dd0 _$E23
// FUNCTION: STBC 0x00847df0 _$E21
// FUNCTION: STBC 0x00847e90 _$E267

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00842280 _$E260
// FUNCTION: STBC 0x008422a0 _$E257
// FUNCTION: STBC 0x00847e00 _$E37
// FUNCTION: STBC 0x00847e10 _$E41
// FUNCTION: STBC 0x00847e20 _$E45
// FUNCTION: STBC 0x00847e30 _$E49
// FUNCTION: STBC 0x00847e40 _$E53
// FUNCTION: STBC 0x00847e50 _$E177
// FUNCTION: STBC 0x00847e60 _$E220
// FUNCTION: STBC 0x00847e70 _$E229
// FUNCTION: STBC 0x00847e80 _$E215
// FUNCTION: STBC 0x00847eb0 _$E119
// FUNCTION: STBC 0x00847ec0 _$E132
// FUNCTION: STBC 0x00847ed0 _$E225
// FUNCTION: STBC 0x00847ee0 _$E233
// FUNCTION: STBC 0x00847ef0 _$E240
// FUNCTION: STBC 0x00847f00 _$E185
// FUNCTION: STBC 0x00847f10 _$E189
// FUNCTION: STBC 0x00847f20 _$E193
// FUNCTION: STBC 0x00847f30 _$E197
// FUNCTION: STBC 0x00847f40 _$E205
// FUNCTION: STBC 0x00847f80 _$E211

// DATA: STBC 0x9B3208 ?current@NiBezierRectangle@@1PAV1@A
NiBezierRectangle *NiBezierRectangle::current;

// DATA: STBC 0x89AD2C ?GRID_STEP@NiBezierRectangle@@2MB
const float NiBezierRectangle::GRID_STEP = 1.0f / GRID_SEGMENTS;

// DATA: STBC 0x9B31B8 ?rtti@NiBezierRectangle@@2VNiRTTI@@B
const NiRTTI NiBezierRectangle::rtti( "NiBezierRectangle", &NiBezierPatch::rtti );

/*
================
NiBezierRectangle::Register
================
*/
// FUNCTION: STBC 0x8422C0 ?Register@NiBezierRectangle@@SAHXZ
NiImplementRegister( NiBezierRectangle )

/*
================
NiBezierRectangle::NiBezierRectangle

A patch of the given degree on the mesh's arrays.
================
*/
// FUNCTION: STBC 0x842410 ??0NiBezierRectangle@@QAE@GPAVNiPoint3@@PAVNiPoint2@@PAVNiColorA@@_N@Z
NiBezierRectangle::NiBezierRectangle( unsigned short degree_, NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08 )
	: NiBezierPatch( degree_, ( degree_ + 1 ) * ( degree_ + 1 ), vertices_, textures_, colors_, unknown08 ) {
	neighborEdges[0] = 0;
	neighborEdges[1] = 0;
	neighborEdges[2] = 0;
	neighborEdges[3] = 0;
	unknown164 = 0;
	sideBits[0] = new char[9];
	sideBits[1] = new char[9];
	sideBits[2] = new char[9];
	sideBits[3] = new char[9];
}

/*
================
NiBezierRectangle::NiBezierRectangle

A patch of the given degree that owns unknownE4 (TODO identity).
================
*/
// FUNCTION: STBC 0x8424D0 ??0NiBezierRectangle@@QAE@GPAGPAVNiPoint3@@PAVNiPoint2@@PAVNiColorA@@_NG@Z
NiBezierRectangle::NiBezierRectangle( unsigned short degree_, unsigned short *unknownE4_, NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08, unsigned short unknownD6_ )
	: NiBezierPatch( degree_, ( degree_ + 1 ) * ( degree_ + 1 ), unknownE4_, vertices_, textures_, colors_, unknown08, unknownD6_ ) {
	neighborEdges[0] = 0;
	neighborEdges[1] = 0;
	neighborEdges[2] = 0;
	neighborEdges[3] = 0;
	unknown164 = 0;
	sideBits[0] = new char[9];
	sideBits[1] = new char[9];
	sideBits[2] = new char[9];
	sideBits[3] = new char[9];
}

/*
================
NiBezierRectangle::NiBezierRectangle
================
*/
// FUNCTION: STBC 0x8425A0 ??0NiBezierRectangle@@QAE@XZ
NiBezierRectangle::NiBezierRectangle() {
	neighborEdges[0] = 0;
	neighborEdges[1] = 0;
	neighborEdges[2] = 0;
	neighborEdges[3] = 0;
	unknown164 = 0;
	sideBits[0] = new char[9];
	sideBits[1] = new char[9];
	sideBits[2] = new char[9];
	sideBits[3] = new char[9];
}

/*
================
NiBezierRectangle::~NiBezierRectangle
================
*/
// FUNCTION: STBC 0x842640
NiBezierRectangle::~NiBezierRectangle() {
	delete [] sideBits[0];
	delete [] sideBits[1];
	delete [] sideBits[2];
	delete [] sideBits[3];
}

/*
================
NiBezierRectangle::SetNeighbor
================
*/
// FUNCTION: STBC 0x842690
void NiBezierRectangle::SetNeighbor( unsigned short side, unsigned int neighbor, short edge ) {
	neighbors[side] = neighbor;
	neighborEdges[side] = edge;
}

/*
================
NiBezierRectangle::VirtualSlot44

Tessellates the patch as a regular grid of 2 ^ level segments a side, two
triangles to a cell.
================
*/
// FUNCTION: STBC 0x8426C0
// The statements are the debug copy's (AnimOnly.exe 0x4E8AFE).
void NiBezierRectangle::VirtualSlot44( unsigned char level_ ) {
	unsigned short segments = 1 << level_;
	unsigned short i;
	unsigned short j;

	vertexCount = ( segments + 1 ) * ( segments + 1 );
	triangleCount = 2 * segments * segments;
	CreateSets();

	float step = 1.0f / segments;
	for ( i = 0; i <= segments; i++ ) {
		float v = i * step;
		for ( j = 0; j <= segments; j++ ) {
			float u = j * step;
			VirtualSlot41( u, v );
		}
	}

	unsigned short rowLength = segments + 1;
	unsigned short rowStart = 0;
	for ( i = 0; i < segments; i++ ) {
		unsigned short lower = rowStart;
		unsigned short lowerNext = lower + 1;
		rowStart += rowLength;
		unsigned short upper = rowStart;
		for ( j = 0; j < segments; j++ ) {
			triangles->Add( lower );
			triangles->Add( lowerNext );
			triangles->Add( upper );
			triangles->Add( lowerNext );
			triangles->Add( upper + 1 );
			triangles->Add( upper );
			lower++;
			lowerNext++;
			upper++;
		}
	}

	modelBound.ComputeFromData( vertexCount, positions ? positions->GetBase() : 0 );
	worldSetsStale = true;
}

/*
================
NiBezierRectangle::VirtualSlot45

Tessellates the patch adaptively: evaluates the four corners into the
vertex cache, has slot 47 refine from them, then records per side which
points of its edge the refinement made.
================
*/
// FUNCTION: STBC 0x842A50
// The statements before the side loop are the debug copy's (AnimOnly.exe 0x4E8DDA).
void NiBezierRectangle::VirtualSlot45( unsigned char level_ ) {
	unsigned short segments = 1 << level_;
	unsigned int i;
	unsigned int j;

	vertexCount = ( segments + 1 ) * ( segments + 1 );
	triangleCount = 2 * segments * segments;
	CreateSets();

	for ( i = 0; i < 4; i++ ) {
		float u = ( i & 1 ) ? 1.0f : 0.0f;
		float v = ( i / 2 ) ? 1.0f : 0.0f;
		VirtualSlot41( u, v );
	}

	vertexCache.cache->NextStamp();
	vertexCache.cache->Set( 0, 0 );
	vertexCache.cache->Set( GRID_SEGMENTS, 1 );
	vertexCache.cache->Set( GRID_SEGMENTS * GRID_ROW, 2 );
	vertexCache.cache->Set( GRID_SEGMENTS * GRID_ROW + GRID_SEGMENTS, 3 );
	current = this;

	unsigned char corners[4];
	corners[0] = 1;
	corners[1] = 1;
	corners[2] = 1;
	corners[3] = 1;
	unsigned char sideBytes = 9;	// (name invented)
	memset( sideBits[0], 0, sideBytes );
	memset( sideBits[1], 0, sideBytes );
	memset( sideBits[2], 0, sideBytes );
	memset( sideBits[3], 0, sideBytes );
	VirtualSlot47( 0, level_, 0, corners );

	for ( i = 0; i < sideBytes; i++ ) {
		unsigned char firstRow = 0;
		unsigned char firstColumn = 0;
		unsigned char lastColumn = 0;
		unsigned char lastRow = 0;
		for ( j = 0; j < 8; j++ ) {
			unsigned short point = i * 8 + j;
			if ( point < GRID_ROW ) {
				if ( vertexCache.cache->IsSet( point ) ) {
					firstRow |= 1 << j;
				}
				if ( vertexCache.cache->IsSet( point * GRID_ROW ) ) {
					firstColumn |= 1 << j;
				}
				if ( vertexCache.cache->IsSet( point * GRID_ROW + GRID_SEGMENTS ) ) {
					lastColumn |= 1 << j;
				}
				if ( vertexCache.cache->IsSet( GRID_SEGMENTS * GRID_ROW + point ) ) {
					lastRow |= 1 << j;
				}
			}
		}
		sideBits[0][i] = firstRow;
		sideBits[1][i] = firstColumn;
		sideBits[2][i] = lastColumn;
		sideBits[3][i] = lastRow;
	}

	unknown164 = !unknown164;
	vertexCount = positions->GetSize();
	triangleCount = triangles->GetSize() / 3;
	modelBound.ComputeFromData( vertexCount, positions ? positions->GetBase() : 0 );
	worldSetsStale = true;
}

/*
================
NiBezierRectangle::VirtualSlot47

Refines one square of the grid with its first corner at the grid point
first, 2 ^ ( finest level - depth ) grid steps a side. For each of its four
sides ( the first row, the first column, the last column and the last row )
it decides whether the side is split at its midpoint: while the refinement
is above the wanted level, and for a side the parent split, when the
segment is in view, long enough and not already decided by the next size
up; the decision is kept in the vertex cache, with the midpoint's vertex
and the centre's. A quarter whose two outer sides are split recurses; the
rest is filled with triangles that fan to the centre, or, with no side
split, with two triangles.
================
*/
// FUNCTION: STBC 0x842D50 ?VirtualSlot47@NiBezierRectangle@@UAEXEEGPAE@Z
// PARKED: inline budget: /Od identical to the debug copy (AnimOnly.exe 0x4E922E) but for frame slots; retail calls Get and Add out of line from site 196 on (2 Gets, 3 Adds earlier than this build), an own cost 191-227 tuples lower
void NiBezierRectangle::VirtualSlot47( unsigned char depth, unsigned char level_, unsigned short first, unsigned char *corners ) {
	unsigned short size = 1 << ( VirtualSlot40() - depth );
	unsigned short half = size >> 1;
	unsigned short index[9];		// the corners, the midpoints of the sides and the centre, by rows
	index[0] = first;
	index[1] = first + half;
	index[2] = first + size;
	index[3] = first + half * GRID_ROW;
	index[4] = index[3] + half;
	index[5] = index[3] + size;
	index[6] = first + size * GRID_ROW;
	index[7] = index[6] + half;
	index[8] = index[6] + size;

	// the grid coordinates of the first corner
	unsigned short v = first >> VirtualSlot40();
	short column = first - v * GRID_ROW;	// (name invented)
	while ( column < 0 ) {
		column += GRID_ROW;
		v--;
	}
	unsigned short u = column;

	// the sides run from point k to point k + 2
	unsigned short us[6];
	us[0] = u;
	us[1] = u;
	us[2] = u + size;
	us[3] = u;
	us[4] = u + size;
	us[5] = u + size;
	unsigned short vs[6];
	vs[0] = v;
	vs[1] = v;
	vs[2] = v;
	vs[3] = v + size;
	vs[4] = v + size;
	vs[5] = v + size;

	unsigned char split[4];
	split[0] = 0;
	split[1] = 0;
	split[2] = 0;
	split[3] = 0;

	unsigned short vertex;
	if ( depth < VirtualSlot40() ) {
		for ( unsigned short k = 0; k < 4; k++ ) {
			unsigned short mid = k + k + 1;
			if ( depth < level_ ) {
				split[k] = 1;
			} else if ( corners[k] ) {
				bool cached = vertexCache.cache->Get( index[mid], vertex );	// (name invented)
				if ( !cached ) {
					split[k] = VirtualSlot50( us[k], vs[k], us[k + 2], vs[k + 2] )
						&& VirtualSlot46( us[k], vs[k], us[k + 2], vs[k + 2] )
						&& VirtualSlot48( us[k], vs[k], us[k + 2], vs[k + 2], level_, k );
				} else {
					split[k] = vertex != 0xFFFF;
				}
			}

			if ( split[k] ) {
				if ( !vertexCache.cache->Get( index[4], vertex ) ) {
					float s = ( u + half ) * GRID_STEP;
					float t = ( v + half ) * GRID_STEP;
					vertexCache.cache->Set( index[4], VirtualSlot41( s, t ) );
				}
				if ( !vertexCache.cache->Get( index[mid], vertex ) ) {
					float sideS = ( ( us[k] + us[k + 2] ) >> 1 ) * GRID_STEP;	// (name invented)
					float sideT = ( ( vs[k] + vs[k + 2] ) >> 1 ) * GRID_STEP;	// (name invented)
					vertexCache.cache->Set( index[mid], VirtualSlot41( sideS, sideT ) );
				}
			} else {
				vertexCache.cache->Set( index[mid], 0xFFFF );
			}
		}
	}

	if ( split[0] ) {
		if ( split[1] ) {
			if ( split[2] ) {
				if ( split[3] ) {
					VirtualSlot47( depth + 1, level_, index[0], split );
					VirtualSlot47( depth + 1, level_, index[1], split );
					VirtualSlot47( depth + 1, level_, index[3], split );
					VirtualSlot47( depth + 1, level_, index[4], split );
				} else {
					VirtualSlot47( depth + 1, level_, index[0], split );
					VirtualSlot47( depth + 1, level_, index[1], split );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[6], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[3], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[8], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[6], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[5], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[8], vertex );
					triangles->Add( vertex );
				}
			} else {
				if ( split[3] ) {
					VirtualSlot47( depth + 1, level_, index[0], split );
					VirtualSlot47( depth + 1, level_, index[3], split );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[8], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[7], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[2], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[8], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[1], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[2], vertex );
					triangles->Add( vertex );
				} else {
					VirtualSlot47( depth + 1, level_, index[0], split );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[6], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[3], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[8], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[6], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[2], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[8], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[1], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[2], vertex );
					triangles->Add( vertex );
				}
			}
		} else {
			if ( split[2] ) {
				if ( split[3] ) {
					VirtualSlot47( depth + 1, level_, index[1], split );
					VirtualSlot47( depth + 1, level_, index[4], split );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[0], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[1], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[6], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[0], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[7], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[6], vertex );
					triangles->Add( vertex );
				} else {
					VirtualSlot47( depth + 1, level_, index[1], split );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[0], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[1], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[6], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[0], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[8], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[6], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[5], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[8], vertex );
					triangles->Add( vertex );
				}
			} else {
				if ( split[3] ) {
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[0], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[1], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[6], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[0], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[7], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[6], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[8], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[7], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[2], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[8], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[1], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[2], vertex );
					triangles->Add( vertex );
				} else {
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[0], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[1], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[6], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[0], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[8], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[6], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[2], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[8], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[1], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[2], vertex );
					triangles->Add( vertex );
				}
			}
		}
	} else {
		if ( split[1] ) {
			if ( split[2] ) {
				if ( split[3] ) {
					VirtualSlot47( depth + 1, level_, index[3], split );
					VirtualSlot47( depth + 1, level_, index[4], split );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[3], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[0], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[0], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[2], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[2], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[5], vertex );
					triangles->Add( vertex );
				} else {
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[3], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[0], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[0], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[2], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[2], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[5], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[5], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[8], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[8], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[6], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[6], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[3], vertex );
					triangles->Add( vertex );
				}
			} else {
				if ( split[3] ) {
					VirtualSlot47( depth + 1, level_, index[3], split );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[3], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[0], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[0], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[2], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[2], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[8], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[8], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[7], vertex );
					triangles->Add( vertex );
				} else {
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[3], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[0], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[0], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[2], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[2], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[8], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[8], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[6], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[6], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[3], vertex );
					triangles->Add( vertex );
				}
			}
		} else {
			if ( split[2] ) {
				if ( split[3] ) {
					VirtualSlot47( depth + 1, level_, index[4], split );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[7], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[6], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[6], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[0], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[0], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[2], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[2], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[5], vertex );
					triangles->Add( vertex );
				} else {
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[5], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[8], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[8], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[6], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[6], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[0], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[0], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[2], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[2], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[5], vertex );
					triangles->Add( vertex );
				}
			} else {
				if ( split[3] ) {
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[7], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[6], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[6], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[0], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[0], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[2], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[2], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[8], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[4], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[8], vertex );
					triangles->Add( vertex );
					vertexCache.cache->Get( index[7], vertex );
					triangles->Add( vertex );
				} else {
					// no side split: two triangles, their diagonal alternating with the
					// square's position on the grid
					if ( ( first >> ( VirtualSlot40() - depth ) ) & 1 ) {
						vertexCache.cache->Get( index[0], vertex );
						triangles->Add( vertex );
						vertexCache.cache->Get( index[2], vertex );
						triangles->Add( vertex );
						vertexCache.cache->Get( index[6], vertex );
						triangles->Add( vertex );
						vertexCache.cache->Get( index[2], vertex );
						triangles->Add( vertex );
						vertexCache.cache->Get( index[8], vertex );
						triangles->Add( vertex );
						vertexCache.cache->Get( index[6], vertex );
						triangles->Add( vertex );
					} else {
						vertexCache.cache->Get( index[0], vertex );
						triangles->Add( vertex );
						vertexCache.cache->Get( index[8], vertex );
						triangles->Add( vertex );
						vertexCache.cache->Get( index[6], vertex );
						triangles->Add( vertex );
						vertexCache.cache->Get( index[0], vertex );
						triangles->Add( vertex );
						vertexCache.cache->Get( index[2], vertex );
						triangles->Add( vertex );
						vertexCache.cache->Get( index[8], vertex );
						triangles->Add( vertex );
					}
				}
			}
		}
	}
}

/*
================
NiBezierRectangle::VirtualSlot46

The class does not do this.
================
*/
// FUNCTION: STBC 0x846410
bool NiBezierRectangle::VirtualSlot46( unsigned short, unsigned short, unsigned short, unsigned short ) {
	return false;
}

/*
================
NiBezierRectangle::VirtualSlot48

For a segment on a grid line parallel to side (sides 0 and 3 are the
first and last rows, 1 and 2 the first and last columns): unless it is
already at the finest level or lies on the odd points of its own size,
tests the segment of twice its length that leads away from the side,
and keeps the vertex cache's midpoint as slot 42 does. A segment on the
side itself asks the neighbour across it instead (slot 42 with the shared
edge); one with no neighbour splits.
================
*/
// FUNCTION: STBC 0x846420 ?VirtualSlot48@NiBezierRectangle@@UAE_NGGGGEG@Z
// PARKED: two-register addresses: the vertex cache stores at the end take [esi+edx] in retail, [edx+esi] here (c2trace --addr: 3 or 4 symbols more or fewer numbered before both); the call set is retail's with the single-statement if and for bodies unbraced (own cost 10 to 26 lower puts ~NiPointer<NiAVObject> [20.4] out of line)
// The statements are the debug copy's (AnimOnly.exe 0x4EBA5E).
bool NiBezierRectangle::VirtualSlot48( unsigned short u0, unsigned short v0, unsigned short u1, unsigned short v1, unsigned char level_, unsigned short side ) {
	bool odd;
	bool interior;
	bool split;
	unsigned short nextU0;
	unsigned short nextV0;
	unsigned short nextU1;
	unsigned short nextV1;
	unsigned short nextSide;
	if ( ( ( side >> 1 ) & 1 ) == ( side & 1 ) ) {
		// a row
		unsigned short length = u1 - u0;
		unsigned short shift = 0;
		for ( unsigned short rest = length; rest != 1; rest >>= 1 )
			shift++;
		if ( shift == VirtualSlot40() - level_ )
			return true;
		if ( ( v0 >> shift ) & 1 )
			return true;
		odd = ( u0 >> shift ) & 1;
		if ( side == 0 ) {
			if ( v0 == 0 ) {
				// the first row: the neighbour's shared edge decides
				if ( neighborEdges[0] == 0 )
					return true;
				NiPointer<NiBezierPatch> patch = NiDynamicCast( NiBezierPatch, GetParent()->GetAt( neighbors[0] ) );
				assert( patch );
				split = patch->VirtualSlot42( u0, u1, VirtualSlot40(), level_, unknown164, neighborEdges[0] );
				patch = 0;
				return split;
			}
			if ( odd ) {
				nextU0 = u1;
				nextV0 = v1 - 2 * length;
				nextU1 = u1;
				nextV1 = v1;
				nextSide = 2;
			} else {
				nextU0 = u0;
				nextV0 = v0 - 2 * length;
				nextU1 = u0;
				nextV1 = v0;
				nextSide = 1;
			}
		} else {
			if ( v0 == GRID_SEGMENTS ) {
				// the last row
				if ( neighborEdges[3] == 0 )
					return true;
				NiPointer<NiBezierPatch> patch = NiDynamicCast( NiBezierPatch, GetParent()->GetAt( neighbors[3] ) );
				assert( patch );
				split = patch->VirtualSlot42( u0, u1, VirtualSlot40(), level_, unknown164, neighborEdges[3] );
				patch = 0;
				return split;
			}
			if ( odd ) {
				nextU0 = u1;
				nextV0 = v1;
				nextU1 = u1;
				nextV1 = v1 + 2 * length;
				nextSide = 2;
			} else {
				nextU0 = u0;
				nextV0 = v0;
				nextU1 = u0;
				nextV1 = v0 + 2 * length;
				nextSide = 1;
			}
		}
		interior = ( nextU0 >> ( shift + 1 ) ) & 1;
	} else {
		// a column
		unsigned short length = v1 - v0;
		unsigned short shift = 0;
		for ( unsigned short rest = length; rest != 1; rest >>= 1 )
			shift++;
		if ( shift == VirtualSlot40() - level_ )
			return true;
		if ( ( u0 >> shift ) & 1 )
			return true;
		odd = ( v0 >> shift ) & 1;
		if ( side == 1 ) {
			if ( u0 == 0 ) {
				// the first column
				if ( neighborEdges[1] == 0 )
					return true;
				NiPointer<NiBezierPatch> patch = NiDynamicCast( NiBezierPatch, GetParent()->GetAt( neighbors[1] ) );
				assert( patch );
				split = patch->VirtualSlot42( v0, v1, VirtualSlot40(), level_, unknown164, neighborEdges[1] );
				patch = 0;
				return split;
			}
			if ( odd ) {
				nextU0 = u1 - 2 * length;
				nextV0 = v1;
				nextU1 = u1;
				nextV1 = v1;
				nextSide = 3;
			} else {
				nextU0 = u0 - 2 * length;
				nextV0 = v0;
				nextU1 = u0;
				nextV1 = v0;
				nextSide = 0;
			}
		} else {
			if ( u0 == GRID_SEGMENTS ) {
				// the last column
				if ( neighborEdges[2] == 0 )
					return true;
				NiPointer<NiBezierPatch> patch = NiDynamicCast( NiBezierPatch, GetParent()->GetAt( neighbors[2] ) );
				assert( patch );
				split = patch->VirtualSlot42( v0, v1, VirtualSlot40(), level_, unknown164, neighborEdges[2] );
				patch = 0;
				return split;
			}
			if ( odd ) {
				nextU0 = u1;
				nextV0 = v1;
				nextU1 = u1 + 2 * length;
				nextV1 = v1;
				nextSide = 3;
			} else {
				nextU0 = u0;
				nextV0 = v0;
				nextU1 = u0 + 2 * length;
				nextV1 = v0;
				nextSide = 0;
			}
		}
		interior = ( nextV0 >> ( shift + 1 ) ) & 1;
	}

	unsigned short u = ( nextU1 + nextU0 ) >> 1;
	unsigned short v = ( nextV1 + nextV0 ) >> 1;
	unsigned short index = v * GRID_ROW + u;
	bool tessellating = current == this;
	unsigned short value;
	if ( tessellating ) {
		bool cached = vertexCache.cache->Get( index, value );	// (name invented)
		if ( !cached )
			return true;
		if ( value == 0xFFFF )
			return false;
	}

	split = VirtualSlot50( nextU0, nextV0, nextU1, nextV1 ) && VirtualSlot46( nextU0, nextV0, nextU1, nextV1 );
	if ( split ) {
		if ( interior ) {
			split = VirtualSlot49( nextU0, nextV0, nextU1, nextV1, level_, odd );
		} else {
			split = VirtualSlot48( nextU0, nextV0, nextU1, nextV1, level_, nextSide );
		}
	}

	if ( tessellating && !vertexCache.cache->Get( index, value ) ) {
		if ( split ) {
			float s = u * GRID_STEP;
			float t = v * GRID_STEP;
			vertexCache.cache->Set( index, VirtualSlot41( s, t ) );
		} else {
			vertexCache.cache->Set( index, 0xFFFF );
		}
	}
	return split;
}

/*
================
NiBezierRectangle::VirtualSlot42

A neighbour asks whether the stretch of the shared edge from start to end,
on the neighbour's grid of 2 ^ gridLevel segments, is split. A negative
edge numbers the side from 1 and runs the stretch the other way. When the
neighbour's pass differs from this patch's, the answer is the side's
recorded bit at the midpoint; otherwise the segment across the edge at the
stretch's end point is tested and, while this patch is the one being
tessellated, its midpoint is evaluated into the vertex cache or marked as
not split.
================
*/
// FUNCTION: STBC 0x846D00 ?VirtualSlot42@NiBezierRectangle@@UAE_NGGEEEF@Z
// The statements are the debug copy's (AnimOnly.exe 0x4EC6EB).
bool NiBezierRectangle::VirtualSlot42( unsigned short start, unsigned short end, unsigned char gridLevel, unsigned char level_, unsigned char pass, short edge ) {
	unsigned short segments = 1 << gridLevel;
	if ( edge < 0 ) {
		unsigned short oldEnd = end;
		end = segments - start;
		start = segments - oldEnd;
		edge = -edge;
	}
	edge--;

	unsigned short length = end - start;
	unsigned short shift = 0;
	for ( unsigned short rest = length; rest != 1; rest >>= 1 ) {
		shift++;
	}
	unsigned short gridSegments = GRID_SEGMENTS;	// (name invented)
	bool odd = ( start >> shift ) & 1;
	unsigned short gridLength = length;	// (name invented)

	if ( segments != gridSegments ) {
		char levelDiff = gridLevel - VirtualSlot40();
		if ( levelDiff > 0 ) {
			end >>= levelDiff;
			start >>= levelDiff;
			gridLength = length >> levelDiff;
		} else {
			levelDiff = -levelDiff;
			end <<= levelDiff;
			start <<= levelDiff;
			gridLength = length << levelDiff;
		}
	}

	if ( pass != unknown164 ) {
		unsigned short point = ( end + start ) >> 1;
		if ( sideBits[edge][point >> 3] & ( 1 << ( point & 7 ) ) ) {
			return true;
		}
		return false;
	}

	unsigned short u0;
	unsigned short v0;
	unsigned short u1;
	unsigned short v1;
	unsigned short side;
	bool interior;
	if ( ( ( edge >> 1 ) & 1 ) == ( edge & 1 ) ) {
		if ( odd ) {
			u0 = u1 = end;
			side = 2;
		} else {
			u0 = u1 = start;
			side = 1;
		}
		if ( edge == 3 ) {
			v0 = gridSegments - 2 * gridLength;
			v1 = gridSegments;
		} else {
			v0 = 0;
			v1 = 2 * gridLength;
		}
		interior = u0 != 0 && u0 != gridSegments;
	} else {
		if ( odd ) {
			v0 = v1 = end;
			side = 3;
		} else {
			v0 = v1 = start;
			side = 0;
		}
		if ( edge == 1 ) {
			u0 = 0;
			u1 = 2 * gridLength;
		} else {
			u0 = gridSegments - 2 * gridLength;
			u1 = gridSegments;
		}
		interior = v0 != 0 && v0 != gridSegments;
	}

	unsigned short u = ( u1 + u0 ) >> 1;
	unsigned short v = ( v1 + v0 ) >> 1;
	unsigned short index = v * GRID_ROW + u;
	bool tessellating = current == this;
	unsigned short value;
	if ( tessellating ) {
		bool cached = vertexCache.cache->Get( index, value );	// (name invented)
		if ( !cached ) {
			return true;
		}
		if ( value == 0xFFFF ) {
			return false;
		}
	}

	bool split = VirtualSlot50( u0, v0, u1, v1 ) && VirtualSlot46( u0, v0, u1, v1 );
	if ( split ) {
		if ( interior ) {
			split = VirtualSlot49( u0, v0, u1, v1, level_, odd );
		} else {
			split = VirtualSlot48( u0, v0, u1, v1, level_, side );
		}
	}

	if ( tessellating && !vertexCache.cache->Get( index, value ) ) {
		if ( split ) {
			float s = u * GRID_STEP;
			float t = v * GRID_STEP;
			vertexCache.cache->Set( index, VirtualSlot41( s, t ) );
		} else {
			vertexCache.cache->Set( index, 0xFFFF );
		}
	}
	return split;
}

/*
================
NiBezierRectangle::VirtualSlot49

For a segment inside the patch: unless it is already at the finest level
or its line lies on the odd points of its own size, tests the segment of
twice its length one step to the side (toward the odd neighbour) and keeps
the vertex cache's midpoint as slot 42 does. An edge of the patch met that
way goes to slot 48 with its side.
================
*/
// FUNCTION: STBC 0x8470C0 ?VirtualSlot49@NiBezierRectangle@@UAE_NGGGGE_N@Z
// The statements are the debug copy's (AnimOnly.exe 0x4ECD15).
bool NiBezierRectangle::VirtualSlot49( unsigned short u0, unsigned short v0, unsigned short u1, unsigned short v1, unsigned char level_, bool odd ) {
	bool interior = false;
	unsigned short nextU0;
	unsigned short nextV0;
	unsigned short nextU1;
	unsigned short nextV1;
	unsigned short side;
	if ( v0 == v1 ) {
		unsigned short length = u1 - u0;
		unsigned short shift = 0;
		for ( unsigned short rest = length; rest != 1; rest >>= 1 ) {
			shift++;
		}
		if ( shift == VirtualSlot40() - level_ ) {
			return true;
		}
		if ( !( ( v0 >> shift ) & 1 ) ) {
			return true;
		}
		if ( ( u0 >> shift ) & 1 ) {
			nextU0 = u0 - length;
			nextU1 = u1;
		} else {
			nextU0 = u0;
			nextU1 = u1 + length;
		}
		if ( odd ) {
			nextV0 = nextV1 = v0 + length;
		} else {
			nextV0 = nextV1 = v0 - length;
		}
		if ( nextV0 == GRID_SEGMENTS ) {
			side = 3;
		} else if ( nextV0 == 0 ) {
			side = 0;
		} else {
			interior = true;
		}
	} else {
		unsigned short length = v1 - v0;
		unsigned short shift = 0;
		for ( unsigned short rest = length; rest != 1; rest >>= 1 ) {
			shift++;
		}
		if ( shift == VirtualSlot40() - level_ ) {
			return true;
		}
		if ( !( ( u0 >> shift ) & 1 ) ) {
			return true;
		}
		if ( ( v0 >> shift ) & 1 ) {
			nextV0 = v0 - length;
			nextV1 = v1;
		} else {
			nextV0 = v0;
			nextV1 = v1 + length;
		}
		if ( odd ) {
			nextU0 = nextU1 = u0 + length;
		} else {
			nextU0 = nextU1 = u0 - length;
		}
		if ( nextU0 == GRID_SEGMENTS ) {
			side = 2;
		} else if ( nextU0 == 0 ) {
			side = 1;
		} else {
			interior = true;
		}
	}

	unsigned short u = ( nextU1 + nextU0 ) >> 1;
	unsigned short v = ( nextV1 + nextV0 ) >> 1;
	unsigned short index = v * GRID_ROW + u;
	bool tessellating = current == this;
	unsigned short value;
	if ( tessellating ) {
		bool cached = vertexCache.cache->Get( index, value );	// (name invented)
		if ( !cached ) {
			return true;
		}
		if ( value == 0xFFFF ) {
			return false;
		}
	}

	bool split = VirtualSlot50( nextU0, nextV0, nextU1, nextV1 ) && VirtualSlot46( nextU0, nextV0, nextU1, nextV1 );
	if ( split ) {
		if ( interior ) {
			split = VirtualSlot49( nextU0, nextV0, nextU1, nextV1, level_, odd );
		} else {
			split = VirtualSlot48( nextU0, nextV0, nextU1, nextV1, level_, side );
		}
	}

	if ( tessellating && !vertexCache.cache->Get( index, value ) ) {
		if ( split ) {
			float s = u * GRID_STEP;
			float t = v * GRID_STEP;
			vertexCache.cache->Set( index, VirtualSlot41( s, t ) );
		} else {
			vertexCache.cache->Set( index, 0xFFFF );
		}
	}
	return split;
}

/*
================
NiBezierRectangle::VirtualSlot50

Evaluates the segment's end points, or takes the vertices the cache holds
for them, carries them into world space and through the camera's
projection, and splits the segment when either end is in view and its
length across the port, in pixels, exceeds unknownFC times the summed
depths.
================
*/
// FUNCTION: STBC 0x847450 ?VirtualSlot50@NiBezierRectangle@@UAE_NGGGG@Z
// The statements are the debug copy's (AnimOnly.exe 0x4ED2D1).
bool NiBezierRectangle::VirtualSlot50( unsigned short u0, unsigned short v0, unsigned short u1, unsigned short v1 ) {
	if ( !unknown128 ) {
		return false;
	}

	NiPoint3 point0;
	NiPoint3 point1;
	unsigned short index0 = 0;
	unsigned short index1 = 0;
	if ( current == this ) {
		vertexCache.cache->Get( u0 + v0 * GRID_ROW, index0 );
		vertexCache.cache->Get( u1 + v1 * GRID_ROW, index1 );
	}

	if ( index0 != 0xFFFF && ( index0 != 0 || u0 + v0 == 0 ) ) {
		point0 = positions->GetAt( index0 );
	} else {
		float s = u0 * GRID_STEP;
		float t = v0 * GRID_STEP;
		VirtualSlot43( s, t );
		NiPoint3 vertex;
		float weight;
		if ( unknownE4 ) {
			vertex = vertices[unknownE4[0]];
			weight = weights[0];
			point0.x = weight * vertex.x;
			point0.y = weight * vertex.y;
			point0.z = weight * vertex.z;
			for ( int i = 1; i < controlPointCount; i++ ) {
				vertex = vertices[unknownE4[i]];
				weight = weights[i];
				point0.x += weight * vertex.x;
				point0.y += weight * vertex.y;
				point0.z += weight * vertex.z;
			}
		} else {
			vertex = vertices[0];
			weight = weights[0];
			point0.x = weight * vertex.x;
			point0.y = weight * vertex.y;
			point0.z = weight * vertex.z;
			for ( int i = 1; i < controlPointCount; i++ ) {
				vertex = vertices[i];
				weight = weights[i];
				point0.x += weight * vertex.x;
				point0.y += weight * vertex.y;
				point0.z += weight * vertex.z;
			}
		}
	}
	if ( index1 != 0xFFFF && index1 != 0 ) {
		point1 = positions->GetAt( index1 );
	} else {
		float s = u1 * GRID_STEP;
		float t = v1 * GRID_STEP;
		VirtualSlot43( s, t );
		NiPoint3 vertex;
		float weight;
		if ( unknownE4 ) {
			vertex = vertices[unknownE4[0]];
			weight = weights[0];
			point1.x = weight * vertex.x;
			point1.y = weight * vertex.y;
			point1.z = weight * vertex.z;
			for ( int i = 1; i < controlPointCount; i++ ) {
				vertex = vertices[unknownE4[i]];
				weight = weights[i];
				point1.x += weight * vertex.x;
				point1.y += weight * vertex.y;
				point1.z += weight * vertex.z;
			}
		} else {
			vertex = vertices[0];
			weight = weights[0];
			point1.x = weight * vertex.x;
			point1.y = weight * vertex.y;
			point1.z = weight * vertex.z;
			for ( int i = 1; i < controlPointCount; i++ ) {
				vertex = vertices[i];
				weight = weights[i];
				point1.x += weight * vertex.x;
				point1.y += weight * vertex.y;
				point1.z += weight * vertex.z;
			}
		}
	}

	NiPoint3 world0 = GetWorldScale() * ( GetWorldRotate() * point0 ) + GetWorldTranslate();
	NiPoint3 world1 = GetWorldScale() * ( GetWorldRotate() * point1 ) + GetWorldTranslate();

	// project both ends through the camera's world-to-camera rows 0, 1 and 3
	const float *matrix = unknown128->GetWorldToCameraMatrix();
	const float *row1 = matrix + 4;
	const float *row3 = matrix + 12;
	float px0 = world0.x;
	float py0 = world0.y;
	float pz0 = world0.z;
	float w0 = px0 * row3[0] + py0 * row3[1] + pz0 * row3[2] + row3[3];
	float x0 = px0 * matrix[0] + py0 * matrix[1] + pz0 * matrix[2] + matrix[3];
	float y0 = px0 * row1[0] + py0 * row1[1] + pz0 * row1[2] + row1[3];
	float px1 = world1.x;
	float py1 = world1.y;
	float pz1 = world1.z;
	float w1 = px1 * row3[0] + py1 * row3[1] + pz1 * row3[2] + row3[3];
	float x1 = px1 * matrix[0] + py1 * matrix[1] + pz1 * matrix[2] + matrix[3];
	float y1 = px1 * row1[0] + py1 * row1[1] + pz1 * row1[2] + row1[3];

	if ( ( x0 < -w0 || x0 > w0 || y0 < -w0 || y0 > w0 ) && ( x1 < -w1 || x1 > w1 || y1 < -w1 || y1 > w1 ) ) {
		return false;
	}

	NiRect<float> port = unknown128->GetViewPort();
	float dx = ( x1 - x0 ) * ( port.right - port.left ) * unknown128->GetRenderer()->VirtualSlot19();
	if ( dx < 0.0f ) {
		dx = -dx;
	}
	float dy = ( y1 - y0 ) * ( port.top - port.bottom ) * unknown128->GetRenderer()->VirtualSlot20();
	if ( dy < 0.0f ) {
		dy = -dy;
	}
	float threshold = ( w0 + w1 ) * unknownFC;
	return dx > threshold || dy > threshold;
}

/*
================
NiBezierRectangle::CreateClone
================
*/
// FUNCTION: STBC 0x847C40
NiObject *NiBezierRectangle::CreateClone() {
	assert( false );
	return 0;
}

/*
================
NiBezierRectangle::CopyMembers

The clone shares the neighbours of the original.
================
*/
// FUNCTION: STBC 0x847C50
void NiBezierRectangle::CopyMembers( NiBezierRectangle *dest ) {
	NiBezierPatch::CopyMembers( dest );

	for ( unsigned int i = 0; i < 4; i++ ) {
		dest->neighbors[i] = neighbors[i];
		dest->neighborEdges[i] = neighborEdges[i];
	}
}

/*
================
NiBezierRectangle::CreateObject

The stream gets nothing from this class; its derived classes create the
patches.
================
*/
// FUNCTION: STBC 0x847C90
void NiBezierRectangle::CreateObject( NiStream &stream, NiObject *&object ) {
}

/*
================
NiBezierRectangle::LoadBinary

The neighbour pointers are read back as they were saved, not as link IDs
(a quirk of the original: they are never relinked).
================
*/
// FUNCTION: STBC 0x847CA0
void NiBezierRectangle::LoadBinary( NiStream &stream ) {
	NiBezierPatch::LoadBinary( stream );

	NiStreamLoadBinary( stream, neighbors, 4 );
	NiStreamLoadBinary( stream, neighborEdges, 4 );
}

/*
================
NiBezierRectangle::LinkObject
================
*/
// FUNCTION: STBC 0x847CE0
void NiBezierRectangle::LinkObject( NiStream &stream ) {
	NiBezierPatch::LinkObject( stream );
}

/*
================
NiBezierRectangle::RegisterStreamables
================
*/
// FUNCTION: STBC 0x847CF0
bool NiBezierRectangle::RegisterStreamables( NiStream &stream ) {
	return NiBezierPatch::RegisterStreamables( stream );
}

/*
================
NiBezierRectangle::SaveBinary

The neighbour pointers are written as raw addresses.
================
*/
// FUNCTION: STBC 0x847D00
void NiBezierRectangle::SaveBinary( NiStream &stream ) {
	NiBezierPatch::SaveBinary( stream );

	NiStreamSaveBinary( stream, neighbors, 4 );
	NiStreamSaveBinary( stream, neighborEdges, 4 );
}

/*
================
NiBezierRectangle::IsEqual

The neighbours are compared by address.
================
*/
// FUNCTION: STBC 0x847D40
bool NiBezierRectangle::IsEqual( NiObject *object ) {
	if ( !NiBezierPatch::IsEqual( object ) ) {
		return false;
	}

	NiBezierRectangle *rectangle = (NiBezierRectangle *)object;
	for ( unsigned int i = 0; i < 4; i++ ) {
		if ( neighbors[i] != rectangle->neighbors[i] || neighborEdges[i] != rectangle->neighborEdges[i] ) {
			return false;
		}
	}
	return true;
}
