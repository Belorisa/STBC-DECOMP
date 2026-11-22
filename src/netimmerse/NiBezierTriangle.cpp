// NiBezierTriangle.cpp -- triangular Bezier patches

#include "NiRTLib.h"
#include "NiStream.h"
#include "NiBezierTriangle.h"
#include "NiCamera.h"
#include "renderer/NiRender.h"
#include "NiBezierMesh.h"
#include "NiBezierSkinController.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00838b60 _$E17
// FUNCTION: STBC 0x00838b80 _$E15
// FUNCTION: STBC 0x00838b90 _$E23
// FUNCTION: STBC 0x00838bb0 _$E21
// FUNCTION: STBC 0x00838d20 _$E265

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x008351b0 _$E258
// FUNCTION: STBC 0x008351d0 _$E255
// FUNCTION: STBC 0x00838bc0 _$E37
// FUNCTION: STBC 0x00838bd0 _$E41
// FUNCTION: STBC 0x00838be0 _$E45
// FUNCTION: STBC 0x00838bf0 _$E49
// FUNCTION: STBC 0x00838c00 _$E53
// FUNCTION: STBC 0x00838c90 _$E177
// FUNCTION: STBC 0x00838ca0 _$E220
// FUNCTION: STBC 0x00838cb0 _$E229
// FUNCTION: STBC 0x00838d10 _$E215
// FUNCTION: STBC 0x00838d40 _$E119
// FUNCTION: STBC 0x00838d50 _$E132
// FUNCTION: STBC 0x00838d60 _$E225
// FUNCTION: STBC 0x00838d70 _$E233
// FUNCTION: STBC 0x00838d80 _$E240
// FUNCTION: STBC 0x00838dc0 _$E185
// FUNCTION: STBC 0x00838dd0 _$E189
// FUNCTION: STBC 0x00838de0 _$E193
// FUNCTION: STBC 0x00838df0 _$E197
// FUNCTION: STBC 0x00838e00 _$E205
// FUNCTION: STBC 0x00838e40 _$E211

// DATA: STBC 0x9B3098 ?current@NiBezierTriangle@@1PAV1@A
NiBezierTriangle *NiBezierTriangle::current;

// DATA: STBC 0x89A9C8 ?GRID_STEP@NiBezierTriangle@@1MB
const float NiBezierTriangle::GRID_STEP = 1.0f / GRID_SEGMENTS;

// DATA: STBC 0x9B3068 ?rtti@NiBezierTriangle@@2VNiRTTI@@B
const NiRTTI NiBezierTriangle::rtti( "NiBezierTriangle", &NiBezierPatch::rtti );

/*
================
NiBezierTriangle::Register
================
*/
// FUNCTION: STBC 0x8351F0 ?Register@NiBezierTriangle@@SAHXZ
NiImplementRegister( NiBezierTriangle )

/*
================
NiBezierTriangle::NiBezierTriangle

A patch of the given degree on the mesh's arrays.
================
*/
// FUNCTION: STBC 0x835340 ??0NiBezierTriangle@@QAE@GPAVNiPoint3@@PAVNiPoint2@@PAVNiColorA@@_N@Z
NiBezierTriangle::NiBezierTriangle( unsigned short degree_, NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08 )
	: NiBezierPatch( degree_, ( degree_ + 1 ) * ( degree_ + 2 ) / 2, vertices_, textures_, colors_, unknown08 ) {
	neighborEdges[0] = 0;
	neighborEdges[1] = 0;
	neighborEdges[2] = 0;
	unknown160 = 0;
	unknown164 = 0;
	unknown168 = 0;
	unknown16C = 0;
	sideBits[0] = new char[17];
	sideBits[1] = new char[17];
	sideBits[2] = new char[17];
}

/*
================
NiBezierTriangle::NiBezierTriangle

A patch of the given degree that owns unknownE4 (TODO identity).
================
*/
// FUNCTION: STBC 0x835410 ??0NiBezierTriangle@@QAE@GPAGPAVNiPoint3@@PAVNiPoint2@@PAVNiColorA@@_NG@Z
NiBezierTriangle::NiBezierTriangle( unsigned short degree_, unsigned short *unknownE4_, NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08, unsigned short unknownD6_ )
	: NiBezierPatch( degree_, ( degree_ + 1 ) * ( degree_ + 2 ) / 2, unknownE4_, vertices_, textures_, colors_, unknown08, unknownD6_ ) {
	neighborEdges[0] = 0;
	neighborEdges[1] = 0;
	neighborEdges[2] = 0;
	unknown160 = 0;
	unknown164 = 0;
	unknown168 = 0;
	unknown16C = 0;
	sideBits[0] = new char[17];
	sideBits[1] = new char[17];
	sideBits[2] = new char[17];
}

/*
================
NiBezierTriangle::NiBezierTriangle
================
*/
// FUNCTION: STBC 0x8354F0 ??0NiBezierTriangle@@QAE@XZ
NiBezierTriangle::NiBezierTriangle() {
	neighborEdges[0] = 0;
	neighborEdges[1] = 0;
	neighborEdges[2] = 0;
	unknown160 = 0;
	unknown164 = 0;
	unknown168 = 0;
	unknown16C = 0;
	sideBits[0] = new char[17];
	sideBits[1] = new char[17];
	sideBits[2] = new char[17];
}

/*
================
NiBezierTriangle::~NiBezierTriangle
================
*/
// FUNCTION: STBC 0x835590
NiBezierTriangle::~NiBezierTriangle() {
	delete [] sideBits[0];
	delete [] sideBits[1];
	delete [] sideBits[2];
}

/*
================
NiBezierTriangle::SetNeighbor
================
*/
// FUNCTION: STBC 0x8355D0
void NiBezierTriangle::SetNeighbor( unsigned short side, unsigned int neighbor, short edge ) {
	neighbors[side] = neighbor;
	neighborEdges[side] = edge;
}

/*
================
NiBezierTriangle::VirtualSlot44

Tessellates the patch as a regular triangular grid of 2 ^ level segments
a side: row i holds segments + 1 - i points, and each cell between two
rows makes an upward triangle and, but for the last, a downward one.
================
*/
// FUNCTION: STBC 0x835600
// The statements are the debug copy's (AnimOnly.exe 0x4DB4C9).
void NiBezierTriangle::VirtualSlot44( unsigned char level_ ) {
	unsigned short segments = 1 << level_;
	unsigned short i;
	unsigned short j;

	vertexCount = ( segments + 1 ) * ( segments + 2 ) >> 1;
	triangleCount = segments * segments;
	CreateSets();

	float step = 1.0f / segments;
	for ( i = 0; i <= segments; i++ ) {
		float v = i * step;
		for ( j = 0; j + i <= segments; j++ ) {
			float u = j * step;
			VirtualSlot41( u, v );
		}
	}

	unsigned short rowSpan = 2 * segments + 2;	// (name invented)
	unsigned short rowStart = 0;
	for ( i = 0; i < segments; i++ ) {
		unsigned short lower = rowStart;
		unsigned short lowerNext = lower + 1;
		rowStart = ( i + 1 ) * ( rowSpan - i ) / 2;
		unsigned short upper = rowStart;
		unsigned short column;
		for ( j = 0; ( column = j + i ) < segments; j++ ) {
			triangles->Add( lower );
			triangles->Add( lowerNext );
			triangles->Add( upper );
			if ( column + 1 < segments ) {
				triangles->Add( lowerNext );
				triangles->Add( upper + 1 );
				triangles->Add( upper );
			}
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
NiBezierTriangle::VirtualSlot45

Tessellates the patch adaptively: evaluates the three corners into the
vertex cache, keeps the squared lengths of the control triangle's sides,
has slot 47 refine from the corners, then records per side which points of
its edge the refinement made. The statements before the side loop are the
debug copy's (AnimOnly.exe 0x4DB803).
================
*/
// FUNCTION: STBC 0x8359F0
void NiBezierTriangle::VirtualSlot45( unsigned char level_ ) {
	unsigned short segments = 1 << level_;
	unsigned int i;
	unsigned int j;

	vertexCount = ( segments + 1 ) * ( segments + 2 ) >> 1;
	triangleCount = segments * segments;
	CreateSets();

	for ( i = 0; i < 3; i++ ) {
		float u = ( i & 1 ) ? 1.0f : 0.0f;
		float v = ( i / 2 ) ? 1.0f : 0.0f;
		VirtualSlot41( u, v );
	}

	vertexCache.cache->NextStamp();
	vertexCache.cache->Set( 0, 0 );
	vertexCache.cache->Set( GRID_SEGMENTS, 1 );
	vertexCache.cache->Set( GRID_LAST_ROW, 2 );
	current = this;

	NiPoint3 corner0 = positions->GetAt( 0 );
	NiPoint3 side0 = positions->GetAt( 1 ) - corner0;
	NiPoint3 side1 = positions->GetAt( 2 ) - corner0;
	NiPoint3 side2 = side0 - side1;
	unknown160 = side0.x * side0.x + side0.y * side0.y + side0.z * side0.z;
	unknown164 = side1.x * side1.x + side1.y * side1.y + side1.z * side1.z;
	unknown168 = side2.x * side2.x + side2.y * side2.y + side2.z * side2.z;

	unsigned char corners[3];
	corners[0] = 1;
	corners[1] = 1;
	corners[2] = 1;
	unsigned char sideBytes = 17;	// (name invented)
	memset( sideBits[0], 0, sideBytes );
	memset( sideBits[1], 0, sideBytes );
	memset( sideBits[2], 0, sideBytes );
	VirtualSlot47( 0, level_, 0, 0, 0, corners );

	unsigned short rowStart = 0;
	unsigned short rowEnd = GRID_SEGMENTS;
	for ( i = 0; i < sideBytes; i++ ) {
		unsigned char firstRow = 0;
		unsigned char firstColumn = 0;
		unsigned char diagonal = 0;
		for ( j = 0; j < 8; j++ ) {
			unsigned short point = i * 8 + j;
			if ( point < GRID_ROW ) {
				if ( vertexCache.cache->IsSet( point ) ) {
					firstRow |= 1 << j;
				}
				if ( vertexCache.cache->IsSet( rowStart ) ) {
					firstColumn |= 1 << j;
				}
				if ( vertexCache.cache->IsSet( rowEnd ) ) {
					diagonal |= 1 << j;
				}
				rowStart = rowEnd + 1;
				rowEnd += GRID_SEGMENTS - point;
			}
		}
		sideBits[0][i] = firstRow;
		sideBits[1][i] = firstColumn;
		sideBits[2][i] = diagonal;
	}

	unknown16C = !unknown16C;
	vertexCount = positions->GetSize();
	triangleCount = triangles->GetSize() / 3;
	modelBound.ComputeFromData( vertexCount, positions ? positions->GetBase() : 0 );
	worldSetsStale = true;
}

/*
================
NiBezierTriangle::VirtualSlot47

Refines one triangle of the grid, an upright one with its first corner at
( u, v ) or, when inverted, a downside-down one, 2 ^ ( finest level - depth )
grid steps a side. For each of its three sides ( the row, the column and the
diagonal ) it decides whether the side is split at its midpoint: while the
refinement is above the wanted level, and for a side the parent split, when
the segment is in view, long enough and not already decided by the next
size up; the decision is kept in the vertex cache, with the midpoint's
vertex. A split side recurses into the triangles it cuts off; the rest is
filled with triangles that fan to the midpoints.
================
*/
// FUNCTION: STBC 0x835D90 ?VirtualSlot47@NiBezierTriangle@@UAEXEEGG_NPAE@Z
// PARKED: inline budget and frame: retail expands one more Get/Add pair late (more budget left), packs us below vs (frame packing key), and one load is scheduled earlier; the /Od body equals AnimOnly.exe 0x4DBD41 but for frame slots
// The statements are the debug copy's (AnimOnly.exe 0x4DBD41).
void NiBezierTriangle::VirtualSlot47( unsigned char depth, unsigned char level_, unsigned short u, unsigned short v, bool inverted, unsigned char *corners ) {
	unsigned short size = 1 << ( VirtualSlot40() - depth );
	unsigned short half = size >> 1;
	short sideFar;		// from the first corner to the far side's corner
	short sideHalf;		// from the first corner to the far side's midpoint
	short sizeStep;
	short halfStep;
	if ( inverted ) {
		sideFar = -size * ( 2 * ( GRID_ROW - v ) + size + 1 ) >> 1;
		sideHalf = -half * ( 2 * ( GRID_ROW - v ) + half + 1 ) >> 1;
		sizeStep = -size;
		halfStep = -half;
	} else {
		sideFar = size * ( 2 * ( GRID_ROW - v ) - size + 1 ) >> 1;
		sideHalf = half * ( 2 * ( GRID_ROW - v ) - half + 1 ) >> 1;
		sizeStep = size;
		halfStep = half;
	}

	unsigned short first = ( v * ( 2 * GRID_ROW - v + 1 ) >> 1 ) + u;
	unsigned short index[6];		// the three corners and the midpoints of the sides between them
	index[0] = first;
	index[1] = first + halfStep;
	index[2] = first + sizeStep;
	index[3] = first + sideHalf;
	index[4] = first + sideFar;
	index[5] = first + sideHalf + halfStep;

	// the segments run from point k to point k + 2
	unsigned short us[5];
	us[0] = u;
	us[1] = u;
	us[2] = u + sizeStep;
	us[3] = u;
	us[4] = u;
	unsigned short vs[5];
	vs[0] = v;
	vs[1] = v;
	vs[2] = v;
	vs[3] = v + sizeStep;
	vs[4] = v + sizeStep;

	unsigned char split[3];
	split[0] = 0;
	split[1] = 0;
	split[2] = 0;

	if ( depth < VirtualSlot40() ) {
		unsigned short vertex;
		for ( unsigned short k = 0; k < 3; k++ ) {
			unsigned short mid = k + k + 1;
			if ( depth < level_ ) {
				split[k] = 1;
			} else if ( corners[k] ) {
				bool cached = vertexCache.cache->Get( index[mid], vertex );	// (name invented)
				if ( !cached ) {
					split[k] = Unknown8381F0( us[k], vs[k], us[k + 2], vs[k + 2] )
						&& VirtualSlot46( us[k], vs[k], us[k + 2], vs[k + 2] )
						&& VirtualSlot48( us[k], vs[k], us[k + 2], vs[k + 2], level_, k );
				} else {
					split[k] = vertex != 0xFFFF;
				}
			}

			if ( split[k] ) {
				if ( !vertexCache.cache->Get( index[mid], vertex ) ) {
					float s = ( ( us[k] + us[k + 2] ) >> 1 ) * GRID_STEP;
					float t = ( ( vs[k] + vs[k + 2] ) >> 1 ) * GRID_STEP;
					vertexCache.cache->Set( index[mid], VirtualSlot41( s, t ) );
				}
			} else {
				vertexCache.cache->Set( index[mid], 0xFFFF );
			}
		}
	}

	unsigned short vertex;
	if ( split[0] ) {
		if ( split[1] ) {
			if ( split[2] ) {
				VirtualSlot47( depth + 1, level_, u, v, inverted, split );
				VirtualSlot47( depth + 1, level_, u + halfStep, v, inverted, split );
				VirtualSlot47( depth + 1, level_, u, v + halfStep, inverted, split );
				VirtualSlot47( depth + 1, level_, u + halfStep, v + halfStep, !inverted, split );
			} else {
				VirtualSlot47( depth + 1, level_, u, v, inverted, split );
				vertexCache.cache->Get( index[1], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( index[2], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( index[3], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( index[2], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( index[4], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( index[3], vertex );
				triangles->Add( vertex );
			}
		} else if ( split[2] ) {
			VirtualSlot47( depth + 1, level_, u + halfStep, v, inverted, split );
			vertexCache.cache->Get( index[0], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( index[1], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( index[4], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( index[5], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( index[4], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( index[1], vertex );
			triangles->Add( vertex );
		} else {
			vertexCache.cache->Get( index[0], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( index[1], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( index[4], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( index[1], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( index[2], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( index[4], vertex );
			triangles->Add( vertex );
		}
	} else if ( split[1] ) {
		if ( split[2] ) {
			VirtualSlot47( depth + 1, level_, u, v + halfStep, inverted, split );
			vertexCache.cache->Get( index[0], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( index[2], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( index[5], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( index[0], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( index[5], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( index[3], vertex );
			triangles->Add( vertex );
		} else {
			vertexCache.cache->Get( index[0], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( index[2], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( index[3], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( index[2], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( index[4], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( index[3], vertex );
			triangles->Add( vertex );
		}
	} else if ( split[2] ) {
		vertexCache.cache->Get( index[0], vertex );
		triangles->Add( vertex );
		vertexCache.cache->Get( index[2], vertex );
		triangles->Add( vertex );
		vertexCache.cache->Get( index[5], vertex );
		triangles->Add( vertex );
		vertexCache.cache->Get( index[0], vertex );
		triangles->Add( vertex );
		vertexCache.cache->Get( index[5], vertex );
		triangles->Add( vertex );
		vertexCache.cache->Get( index[4], vertex );
		triangles->Add( vertex );
	} else {
		vertexCache.cache->Get( index[0], vertex );
		triangles->Add( vertex );
		vertexCache.cache->Get( index[2], vertex );
		triangles->Add( vertex );
		vertexCache.cache->Get( index[4], vertex );
		triangles->Add( vertex );
	}
}

/*
================
NiBezierTriangle::VirtualSlot46

Triangles have no such intersection.
================
*/
// FUNCTION: STBC 0x836F20
bool NiBezierTriangle::VirtualSlot46( unsigned short, unsigned short, unsigned short, unsigned short ) {
	return false;
}

/*
================
NiBezierTriangle::VirtualSlot48

For a segment on a grid line in the direction side (0 a row, 1 a column,
2 a diagonal): unless it is already at the finest level or lies on the odd
points of its own size, tests the segment of twice its length that
completes the triangle of the next size toward the inside of the patch,
and keeps the vertex cache's midpoint as slot 42 does. A segment on the
patch's own edge asks the neighbour across it instead (slot 42 with the
shared edge); one with no neighbour splits. The ends come in either order;
reversed tells which way the next segment runs.
================
*/
// FUNCTION: STBC 0x836F30 ?VirtualSlot48@NiBezierTriangle@@UAE_NGGGGEG@Z
// The statements are the debug copy's (AnimOnly.exe 0x4DCB74).
bool NiBezierTriangle::VirtualSlot48( unsigned short u0, unsigned short v0, unsigned short u1, unsigned short v1, unsigned char level_, unsigned short side ) {
	bool reversed = v0 > v1 || ( side == 0 && u0 > u1 );
	if ( reversed ) {
		unsigned short swap = u0;
		u0 = u1;
		u1 = swap;
		swap = v0;
		v0 = v1;
		v1 = swap;
	}

	bool interior;
	bool split;
	unsigned short nextU0;
	unsigned short nextV0;
	unsigned short nextU1;
	unsigned short nextV1;
	unsigned short nextSide;
	if ( side == 0 ) {
		// along a row
		unsigned short length = u1 - u0;
		unsigned short shift = 0;
		for ( unsigned short rest = length; rest != 1; rest >>= 1 ) {
			shift++;
		}
		if ( shift == VirtualSlot40() - level_ ) {
			return true;
		}
		if ( ( v0 >> shift ) & 1 ) {
			return true;
		}
		if ( reversed ) {
			if ( ( u0 >> shift ) & 1 ) {
				nextU0 = u1;
				nextV0 = v1;
				nextU1 = u0 - length;
				nextV1 = v0 + 2 * length;
				nextSide = 2;
				interior = ( ( GRID_SEGMENTS - nextU0 - nextV0 ) >> ( shift + 1 ) ) & 1;
			} else {
				nextU0 = u0;
				nextV0 = v0;
				nextU1 = u0;
				nextV1 = v0 + 2 * length;
				nextSide = 1;
				interior = ( nextU0 >> ( shift + 1 ) ) & 1;
			}
		} else if ( v0 == 0 ) {
			if ( neighborEdges[0] == 0 ) {
				return true;
			}
			NiPointer<NiBezierPatch> patch = NiDynamicCast( NiBezierPatch, GetParent()->GetAt( neighbors[0] ) );
			assert( patch );
			split = patch->VirtualSlot42( u0, u1, VirtualSlot40(), level_, unknown16C, neighborEdges[0] );
			patch = 0;
			return split;
		} else if ( ( u0 >> shift ) & 1 ) {
			nextU0 = u1;
			nextV0 = v1 - 2 * length;
			nextU1 = u1;
			nextV1 = v1;
			nextSide = 1;
			interior = ( nextU0 >> ( shift + 1 ) ) & 1;
		} else {
			nextU0 = u1 + length;
			nextV0 = v1 - 2 * length;
			nextU1 = u0;
			nextV1 = v0;
			nextSide = 2;
			interior = ( ( GRID_SEGMENTS - nextU0 - nextV0 ) >> ( shift + 1 ) ) & 1;
		}
	} else if ( side == 1 ) {
		// along a column
		unsigned short length = v1 - v0;
		unsigned short shift = 0;
		for ( unsigned short rest = length; rest != 1; rest >>= 1 ) {
			shift++;
		}
		if ( shift == VirtualSlot40() - level_ ) {
			return true;
		}
		if ( ( u0 >> shift ) & 1 ) {
			return true;
		}
		if ( reversed ) {
			if ( ( v0 >> shift ) & 1 ) {
				nextU0 = u0 + 2 * length;
				nextV0 = v0 - length;
				nextU1 = u1;
				nextV1 = v1;
				nextSide = 2;
				interior = ( ( GRID_SEGMENTS - nextU0 - nextV0 ) >> ( shift + 1 ) ) & 1;
			} else {
				nextU0 = u0;
				nextV0 = v0;
				nextU1 = u0 + 2 * length;
				nextV1 = v0;
				nextSide = 0;
				interior = ( nextV0 >> ( shift + 1 ) ) & 1;
			}
		} else if ( u0 == 0 ) {
			if ( neighborEdges[1] == 0 ) {
				return true;
			}
			NiPointer<NiBezierPatch> patch = NiDynamicCast( NiBezierPatch, GetParent()->GetAt( neighbors[1] ) );
			assert( patch );
			split = patch->VirtualSlot42( v0, v1, VirtualSlot40(), level_, unknown16C, neighborEdges[1] );
			patch = 0;
			return split;
		} else if ( ( v0 >> shift ) & 1 ) {
			nextU0 = u1 - 2 * length;
			nextV0 = v1;
			nextU1 = u1;
			nextV1 = v1;
			nextSide = 0;
			interior = ( nextV0 >> ( shift + 1 ) ) & 1;
		} else {
			nextU0 = u0;
			nextV0 = v0;
			nextU1 = u1 - 2 * length;
			nextV1 = v1 + length;
			nextSide = 2;
			interior = ( ( GRID_SEGMENTS - nextU0 - nextV0 ) >> ( shift + 1 ) ) & 1;
		}
	} else {
		// along the diagonal
		unsigned short length = v1 - v0;
		unsigned short shift = 0;
		for ( unsigned short rest = length; rest != 1; rest >>= 1 ) {
			shift++;
		}
		if ( shift == VirtualSlot40() - level_ ) {
			return true;
		}
		if ( ( ( GRID_SEGMENTS - u0 - v0 ) >> shift ) & 1 ) {
			return true;
		}
		if ( reversed ) {
			if ( ( v0 >> shift ) & 1 ) {
				nextU0 = u1;
				nextV0 = v1 - 2 * length;
				nextU1 = u1;
				nextV1 = v1;
				nextSide = 1;
				interior = ( nextU0 >> ( shift + 1 ) ) & 1;
			} else {
				nextU0 = u0 - 2 * length;
				nextV0 = v0;
				nextU1 = u0;
				nextV1 = v0;
				nextSide = 0;
				interior = ( nextV0 >> ( shift + 1 ) ) & 1;
			}
		} else if ( v0 + u0 + 1 == GRID_ROW ) {
			if ( neighborEdges[2] == 0 ) {
				return true;
			}
			NiPointer<NiBezierPatch> patch = NiDynamicCast( NiBezierPatch, GetParent()->GetAt( neighbors[2] ) );
			assert( patch );
			split = patch->VirtualSlot42( v0, v1, VirtualSlot40(), level_, unknown16C, neighborEdges[2] );
			patch = 0;
			return split;
		} else if ( ( v0 >> shift ) & 1 ) {
			nextU0 = u1;
			nextV0 = v1;
			nextU1 = u1 + 2 * length;
			nextV1 = v1;
			nextSide = 0;
			interior = ( nextV0 >> ( shift + 1 ) ) & 1;
		} else {
			nextU0 = u0;
			nextV0 = v0;
			nextU1 = u0;
			nextV1 = v0 + 2 * length;
			nextSide = 1;
			interior = ( nextU0 >> ( shift + 1 ) ) & 1;
		}
	}

	unsigned short u = ( nextU1 + nextU0 ) >> 1;
	unsigned short v = ( nextV1 + nextV0 ) >> 1;
	unsigned short index = u + ( v * ( 2 * GRID_ROW - v + 1 ) >> 1 );
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

	split = Unknown8381F0( nextU0, nextV0, nextU1, nextV1 ) && VirtualSlot46( nextU0, nextV0, nextU1, nextV1 );
	if ( split ) {
		if ( interior ) {
			split = VirtualSlot49( nextU0, nextV0, nextU1, nextV1, level_, !reversed );
		} else if ( reversed ) {
			split = VirtualSlot48( nextU0, nextV0, nextU1, nextV1, level_, nextSide );
		} else {
			split = VirtualSlot48( nextU1, nextV1, nextU0, nextV0, level_, nextSide );
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
NiBezierTriangle::VirtualSlot42

As NiBezierRectangle::VirtualSlot42, for the triangle's three sides: a
neighbour asks whether the stretch of the shared edge from start to end,
on its grid of 2 ^ gridLevel segments, is split.
================
*/
// FUNCTION: STBC 0x8379B0 ?VirtualSlot42@NiBezierTriangle@@UAE_NGGEEEF@Z
// The statements are the debug copy's (AnimOnly.exe 0x4DDB40).
bool NiBezierTriangle::VirtualSlot42( unsigned short start, unsigned short end, unsigned char gridLevel, unsigned char level_, unsigned char pass, short edge ) {
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
	unsigned short gridShift = shift;	// (name invented)

	if ( gridSegments != segments ) {
		char levelDiff = VirtualSlot40() - gridLevel;
		if ( levelDiff > 0 ) {
			end <<= levelDiff;
			start <<= levelDiff;
			gridLength = length << levelDiff;
		} else {
			levelDiff = -levelDiff;
			end >>= levelDiff;
			start >>= levelDiff;
			gridLength = length >> levelDiff;
		}
		// BUG: the shift grows by the level difference either way; a finer
		// neighbour grid should shrink it
		gridShift += levelDiff;
	}

	if ( pass != unknown16C ) {
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
	if ( edge == 0 ) {
		if ( odd ) {
			u0 = end;
			v0 = 0;
			u1 = start - gridLength;
			v1 = 2 * gridLength;
			side = 2;
			interior = ( ( GRID_SEGMENTS - u0 - v0 ) >> ( gridShift + 1 ) ) & 1;
		} else {
			u0 = start;
			v0 = 0;
			u1 = start;
			v1 = 2 * gridLength;
			side = 1;
			interior = ( u0 >> ( gridShift + 1 ) ) & 1;
		}
	} else if ( edge == 1 ) {
		if ( odd ) {
			u0 = 2 * gridLength;
			v0 = start - gridLength;
			u1 = 0;
			v1 = end;
			side = 2;
			interior = ( ( GRID_SEGMENTS - u0 - v0 ) >> ( gridShift + 1 ) ) & 1;
		} else {
			u0 = 0;
			v0 = start;
			u1 = 2 * gridLength;
			v1 = start;
			side = 0;
			interior = ( v0 >> ( gridShift + 1 ) ) & 1;
		}
	} else {
		if ( odd ) {
			u0 = gridSegments - end;
			v0 = start - gridLength;
			u1 = gridSegments - end;
			v1 = end;
			side = 1;
			interior = ( u0 >> ( gridShift + 1 ) ) & 1;
		} else {
			u0 = gridSegments - end - gridLength;
			v0 = start;
			u1 = gridSegments - start;
			v1 = start;
			side = 0;
			interior = ( v0 >> ( gridShift + 1 ) ) & 1;
		}
	}

	unsigned short u = ( u1 + u0 ) >> 1;
	unsigned short v = ( v1 + v0 ) >> 1;
	unsigned short index = u + ( v * ( 2 * GRID_ROW - v + 1 ) >> 1 );
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

	bool split = Unknown8381F0( u0, v0, u1, v1 ) && VirtualSlot46( u0, v0, u1, v1 );
	if ( split ) {
		if ( interior ) {
			split = VirtualSlot49( u0, v0, u1, v1, level_, false );
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
NiBezierTriangle::VirtualSlot49

As NiBezierRectangle::VirtualSlot49, for the triangle's three directions:
unless the segment is already at the finest level or its line's parity
differs from odd, tests the segment of twice its length one step to the
side and keeps the vertex cache's midpoint as slot 42 does. A segment
whose line is odd at the next size goes on to slot 49 again; one on an
even line to slot 48 with its side.
================
*/
// FUNCTION: STBC 0x837DD0 ?VirtualSlot49@NiBezierTriangle@@UAE_NGGGGE_N@Z
// The statements are the debug copy's (AnimOnly.exe 0x4DE28C).
bool NiBezierTriangle::VirtualSlot49( unsigned short u0, unsigned short v0, unsigned short u1, unsigned short v1, unsigned char level_, bool odd ) {
	unsigned short nextU0;
	unsigned short nextV0;
	unsigned short nextU1;
	unsigned short nextV1;
	unsigned short side;
	bool interior;
	if ( v0 == v1 ) {
		// along a row
		unsigned short length = u1 - u0;
		unsigned short shift = 0;
		for ( unsigned short rest = length; rest != 1; rest >>= 1 ) {
			shift++;
		}
		if ( shift == VirtualSlot40() - level_ ) {
			return true;
		}
		if ( ( ( u0 >> shift ) & 1 ) != odd ) {
			return true;
		}
		if ( odd ) {
			nextU0 = u0 - length;
			nextV0 = v0 + length;
			nextU1 = u1;
			nextV1 = v0 + length;
		} else {
			nextU0 = u0;
			nextV0 = v0 - length;
			nextU1 = u1 + length;
			nextV1 = v0 - length;
		}
		interior = ( nextV0 >> ( shift + 1 ) ) & 1;
		side = 0;
	} else if ( u0 == u1 ) {
		// along a column
		unsigned short length = v1 - v0;
		unsigned short shift = 0;
		for ( unsigned short rest = length; rest != 1; rest >>= 1 ) {
			shift++;
		}
		if ( shift == VirtualSlot40() - level_ ) {
			return true;
		}
		if ( ( ( v0 >> shift ) & 1 ) != odd ) {
			return true;
		}
		if ( odd ) {
			nextU0 = u0 + length;
			nextV0 = v0 - length;
			nextU1 = u0 + length;
			nextV1 = v1;
		} else {
			nextU0 = u0 - length;
			nextV0 = v0;
			nextU1 = u0 - length;
			nextV1 = v1 + length;
		}
		interior = ( nextU0 >> ( shift + 1 ) ) & 1;
		side = 1;
	} else {
		// along a diagonal
		unsigned short length = v1 - v0;
		unsigned short shift = 0;
		for ( unsigned short rest = length; rest != 1; rest >>= 1 ) {
			shift++;
		}
		if ( shift == VirtualSlot40() - level_ ) {
			return true;
		}
		if ( ( ( v0 >> shift ) & 1 ) != odd ) {
			return true;
		}
		if ( odd ) {
			nextU0 = u0;
			nextV0 = v0 - length;
			nextU1 = u1 - length;
			nextV1 = v1;
		} else {
			nextU0 = u0 + length;
			nextV0 = v0;
			nextU1 = u1;
			nextV1 = v1 + length;
		}
		interior = ( ( GRID_SEGMENTS - nextU0 - nextV0 ) >> ( shift + 1 ) ) & 1;
		side = 2;
	}

	unsigned short u = ( nextU1 + nextU0 ) >> 1;
	unsigned short v = ( nextV1 + nextV0 ) >> 1;
	unsigned short index = u + ( v * ( 2 * GRID_ROW - v + 1 ) >> 1 );
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

	bool split = Unknown8381F0( nextU0, nextV0, nextU1, nextV1 ) && VirtualSlot46( nextU0, nextV0, nextU1, nextV1 );
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
NiBezierTriangle::Unknown8381F0

Evaluates the segment's end points, or takes the vertices the cache holds
for them, carries them into world space and through the camera's
projection, and splits the segment when either end is in view and its
length across the port, in pixels, exceeds unknownFC times the summed
depths.
================
*/
// FUNCTION: STBC 0x8381F0
bool NiBezierTriangle::Unknown8381F0( unsigned short u0, unsigned short v0, unsigned short u1, unsigned short v1 ) {
	if ( !unknown128 ) {
		return false;
	}

	int i;
	unsigned short index0 = 0;
	unsigned short index1 = 0;
	if ( current == this ) {
		vertexCache.cache->Get( ( ( 2 * GRID_ROW + 1 - v0 ) * v0 >> 1 ) + u0, index0 );
		vertexCache.cache->Get( ( ( 2 * GRID_ROW + 1 - v1 ) * v1 >> 1 ) + u1, index1 );
	}

	NiPoint3 point0;
	if ( index0 != 0xFFFF && ( index0 != 0 || u0 + v0 == 0 ) ) {
		point0 = positions->GetAt( index0 );
	} else {
		VirtualSlot43( u0 * GRID_STEP, v0 * GRID_STEP );
		if ( unknownE4 ) {
			NiPoint3 vertex = vertices[unknownE4[0]];
			float weight = weights[0];
			point0.x = vertex.x * weight;
			point0.y = vertex.y * weight;
			point0.z = vertex.z * weight;
			for ( i = 1; i < controlPointCount; i++ ) {
				vertex = vertices[unknownE4[i]];
				weight = weights[i];
				point0.x += vertex.x * weight;
				point0.y += vertex.y * weight;
				point0.z += vertex.z * weight;
			}
		} else {
			NiPoint3 vertex = vertices[0];
			float weight = weights[0];
			point0.x = vertex.x * weight;
			point0.y = vertex.y * weight;
			point0.z = vertex.z * weight;
			for ( i = 1; i < controlPointCount; i++ ) {
				vertex = vertices[i];
				weight = weights[i];
				point0.x += vertex.x * weight;
				point0.y += vertex.y * weight;
				point0.z += vertex.z * weight;
			}
		}
	}
	NiPoint3 point1;
	if ( index1 != 0xFFFF && ( index1 != 0 || u1 + v1 == 0 ) ) {
		point1 = positions->GetAt( index1 );
	} else {
		VirtualSlot43( u1 * GRID_STEP, v1 * GRID_STEP );
		if ( unknownE4 ) {
			NiPoint3 vertex = vertices[unknownE4[0]];
			float weight = weights[0];
			point1.x = vertex.x * weight;
			point1.y = vertex.y * weight;
			point1.z = vertex.z * weight;
			for ( i = 1; i < controlPointCount; i++ ) {
				vertex = vertices[unknownE4[i]];
				weight = weights[i];
				point1.x += vertex.x * weight;
				point1.y += vertex.y * weight;
				point1.z += vertex.z * weight;
			}
		} else {
			NiPoint3 vertex = vertices[0];
			float weight = weights[0];
			point1.x = vertex.x * weight;
			point1.y = vertex.y * weight;
			point1.z = vertex.z * weight;
			for ( i = 1; i < controlPointCount; i++ ) {
				vertex = vertices[i];
				weight = weights[i];
				point1.x += vertex.x * weight;
				point1.y += vertex.y * weight;
				point1.z += vertex.z * weight;
			}
		}
	}

	NiPoint3 world0 = GetWorldScale() * ( GetWorldRotate() * point0 ) + GetWorldTranslate();
	NiPoint3 world1 = GetWorldScale() * ( GetWorldRotate() * point1 ) + GetWorldTranslate();

	// the depth sums its products z, x, y: the order retail's FPU code has
	NiCamera *camera = unknown128;
	float w0 = world0.z * camera->worldToCam[3][2] + world0.x * camera->worldToCam[3][0] + world0.y * camera->worldToCam[3][1] + camera->worldToCam[3][3];
	float x0 = world0.x * camera->worldToCam[0][0] + world0.y * camera->worldToCam[0][1] + world0.z * camera->worldToCam[0][2] + camera->worldToCam[0][3];
	float y0 = world0.x * camera->worldToCam[1][0] + world0.y * camera->worldToCam[1][1] + world0.z * camera->worldToCam[1][2] + camera->worldToCam[1][3];
	float w1 = world1.z * camera->worldToCam[3][2] + world1.x * camera->worldToCam[3][0] + world1.y * camera->worldToCam[3][1] + camera->worldToCam[3][3];
	float x1 = world1.x * camera->worldToCam[0][0] + world1.y * camera->worldToCam[0][1] + world1.z * camera->worldToCam[0][2] + camera->worldToCam[0][3];
	float y1 = world1.x * camera->worldToCam[1][0] + world1.y * camera->worldToCam[1][1] + world1.z * camera->worldToCam[1][2] + camera->worldToCam[1][3];

	if ( ( x0 < -w0 || x0 > w0 || y0 < -w0 || y0 > w0 ) && ( x1 < -w1 || x1 > w1 || y1 < -w1 || y1 > w1 ) ) {
		return false;
	}

	NiRect<float> port = camera->GetViewPort();
	float dx = ( x1 - x0 ) * ( ( port.right - port.left ) * camera->GetRenderer()->VirtualSlot19() );
	if ( dx < 0.0f ) {
		dx = -dx;
	}
	float dy = ( y1 - y0 ) * ( ( port.top - port.bottom ) * unknown128->GetRenderer()->VirtualSlot20() );
	if ( dy < 0.0f ) {
		dy = -dy;
	}
	float threshold = ( w1 + w0 ) * unknownFC;
	return dx > threshold || dy > threshold;
}

/*
================
NiBezierTriangle::CreateClone
================
*/
// FUNCTION: STBC 0x838A00
NiObject *NiBezierTriangle::CreateClone() {
	assert( false );
	return 0;
}

/*
================
NiBezierTriangle::CopyMembers

The clone shares the neighbours of the original.
================
*/
// FUNCTION: STBC 0x838A10
void NiBezierTriangle::CopyMembers( NiBezierTriangle *dest ) {
	NiBezierPatch::CopyMembers( dest );

	for ( unsigned int i = 0; i < 3; i++ ) {
		dest->neighbors[i] = neighbors[i];
		dest->neighborEdges[i] = neighborEdges[i];
	}
}

/*
================
NiBezierTriangle::CreateObject

The stream gets nothing from this class; its derived classes create the
patches.
================
*/
// FUNCTION: STBC 0x838A50
void NiBezierTriangle::CreateObject( NiStream &stream, NiObject *&object ) {
}

/*
================
NiBezierTriangle::LoadBinary

The neighbour pointers are read back as they were saved, not as link IDs
(a quirk of the original: they are never relinked).
================
*/
// FUNCTION: STBC 0x838A60
void NiBezierTriangle::LoadBinary( NiStream &stream ) {
	NiBezierPatch::LoadBinary( stream );

	NiStreamLoadBinary( stream, neighbors, 3 );
	NiStreamLoadBinary( stream, neighborEdges, 3 );
}

/*
================
NiBezierTriangle::LinkObject
================
*/
// FUNCTION: STBC 0x838AA0
void NiBezierTriangle::LinkObject( NiStream &stream ) {
	NiBezierPatch::LinkObject( stream );
}

/*
================
NiBezierTriangle::RegisterStreamables
================
*/
// FUNCTION: STBC 0x838AB0
bool NiBezierTriangle::RegisterStreamables( NiStream &stream ) {
	return NiBezierPatch::RegisterStreamables( stream );
}

/*
================
NiBezierTriangle::SaveBinary

The neighbour pointers are written as raw addresses.
================
*/
// FUNCTION: STBC 0x838AC0
void NiBezierTriangle::SaveBinary( NiStream &stream ) {
	NiBezierPatch::SaveBinary( stream );

	NiStreamSaveBinary( stream, neighbors, 3 );
	NiStreamSaveBinary( stream, neighborEdges, 3 );
}

/*
================
NiBezierTriangle::IsEqual

The neighbours are compared by address.
================
*/
// FUNCTION: STBC 0x838B00
bool NiBezierTriangle::IsEqual( NiObject *object ) {
	if ( !NiBezierPatch::IsEqual( object ) ) {
		return false;
	}

	NiBezierTriangle *triangle = (NiBezierTriangle *)object;
	for ( unsigned int i = 0; i < 3; i++ ) {
		if ( neighbors[i] != triangle->neighbors[i] || neighborEdges[i] != triangle->neighborEdges[i] ) {
			return false;
		}
	}
	return true;
}


