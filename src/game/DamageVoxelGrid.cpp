/*
===============================================================================

	DamageVoxelGrid.cpp

	A box of voxel cells, one bit each, that MetaVolumePolygonizer turns into
	triangles: the set operations between two grids and the debug lines.
	Its own unit: retail lays its code out after MetaVolumePolygonizer's and
	follows it with its own <iostream> bytes, locale pair and registrations
	(NiLines and the three properties CreateLines uses, without
	MetaVolumePolygonizer's texture properties). The file name is invented.

===============================================================================
*/

#include <iostream>
#include <stdlib.h>
#include "DamageVoxelGrid.h"
#include "NiLines.h"
#include "NiMaterialProperty.h"
#include "NiVertexColorProperty.h"
#include "NiZBufferProperty.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004c64d0 _$E32
// FUNCTION: STBC 0x004c64e0 _$E29
// FUNCTION: STBC 0x004c64f0 _$E31
// FUNCTION: STBC 0x004c6500 _$E30
// FUNCTION: STBC 0x004c6510 _$E38
// FUNCTION: STBC 0x004c6520 _$E35
// FUNCTION: STBC 0x004c6530 _$E37
// FUNCTION: STBC 0x004c6540 _$E36

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004c6550 _$E223
// FUNCTION: STBC 0x004c6570 _$E222

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004c6580 _$E50
// FUNCTION: STBC 0x004c6590 _$E49
// FUNCTION: STBC 0x004c65a0 _$E61
// FUNCTION: STBC 0x004c65b0 _$E60
// FUNCTION: STBC 0x004c65c0 _$E65
// FUNCTION: STBC 0x004c65d0 _$E64
// FUNCTION: STBC 0x004c65e0 _$E69
// FUNCTION: STBC 0x004c65f0 _$E68
// FUNCTION: STBC 0x004c6600 _$E73
// FUNCTION: STBC 0x004c6610 _$E72
// FUNCTION: STBC 0x004c6620 _$E192
// FUNCTION: STBC 0x004c6630 _$E191
// FUNCTION: STBC 0x004c6640 _$E200
// FUNCTION: STBC 0x004c6650 _$E199
// FUNCTION: STBC 0x004c6660 _$E207
// FUNCTION: STBC 0x004c6670 _$E206
// FUNCTION: STBC 0x004c6680 _$E211
// FUNCTION: STBC 0x004c6690 _$E210
// FUNCTION: STBC 0x004c66a0 _$E215
// FUNCTION: STBC 0x004c66b0 _$E214
// FUNCTION: STBC 0x004c66c0 _$E117
// FUNCTION: STBC 0x004c66d0 _$E116
// FUNCTION: STBC 0x004c66e0 _$E177
// FUNCTION: STBC 0x004c66f0 _$E176
// FUNCTION: STBC 0x004c6700 _$E92
// FUNCTION: STBC 0x004c6710 _$E91
// FUNCTION: STBC 0x004c6720 _$E187
// FUNCTION: STBC 0x004c6730 _$E186

/*
================
DamageVoxelGrid::Overlaps

Whether a cell is set in both grids, searched over the boxes' common range
the way Intersect searches it (invented name; retail has no caller).
================
*/
// FUNCTION: STBC 0x004c5210
// PARKED: reload order at the x loop's exit: the five spilled values reload in their web numbers' order (ours z #17, toY #19, y #21; ties at 0x14000 go by input), retail's last three come y, toY, z; no declaration or loop spelling tried renumbers the webs
bool DamageVoxelGrid::Overlaps( DamageVoxelGrid *other ) {
	if ( other->lower[0] < upper[0] && other->lower[1] < upper[1] && other->lower[2] < upper[2]
	  && other->upper[0] > lower[0] && other->upper[1] > lower[1] && other->upper[2] > lower[2] ) {
		unsigned short fromX = __min( other->lower[0], lower[0] );
		unsigned short fromY = __min( other->lower[1], lower[1] );
		unsigned short fromZ = __min( other->lower[2], lower[2] );
		unsigned short toX = __min( other->upper[0], upper[0] );
		unsigned short toY = __min( other->upper[1], upper[1] );
		unsigned short toZ = __min( other->upper[2], upper[2] );
		for ( unsigned short z = fromZ; z < toZ; z++ ) {
			for ( unsigned short y = fromY; y < toY; y++ ) {
				for ( unsigned short x = fromX; x < toX; x++ ) {
					if ( IsSet( x, y, z ) && other->IsSet( x, y, z ) ) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

/*
================
DamageVoxelGrid::Intersect
================
*/
// FUNCTION: STBC 0x004c53e0
// PARKED: register allocation and the branch layout of the six __min selections (retail spills each bound to its own frame slot); the body is complete
DamageVoxelGrid *DamageVoxelGrid::Intersect( DamageVoxelGrid *other ) {
	DamageVoxelGrid *result = NULL;
	if ( other->lower[0] < upper[0] && other->lower[1] < upper[1] && other->lower[2] < upper[2]
	  && other->upper[0] > lower[0] && other->upper[1] > lower[1] && other->upper[2] > lower[2] ) {
		unsigned short fromX = __min( other->lower[0], lower[0] );
		unsigned short fromY = __min( other->lower[1], lower[1] );
		unsigned short fromZ = __min( other->lower[2], lower[2] );
		unsigned short toX = __min( other->upper[0], upper[0] );
		unsigned short toY = __min( other->upper[1], upper[1] );
		unsigned short toZ = __min( other->upper[2], upper[2] );
		for ( unsigned short z = fromZ; z < toZ; z++ ) {
			for ( unsigned short y = fromY; y < toY; y++ ) {
				for ( unsigned short x = fromX; x < toX; x++ ) {
					if ( IsSet( x, y, z ) && other->IsSet( x, y, z ) ) {
						if ( result == NULL ) {
							unsigned short cell[3];
							unsigned short cellUpper[3];
							cellUpper[0] = x;
							cellUpper[1] = y;
							cellUpper[2] = z;
							cell[0] = x;
							cell[1] = y;
							cell[2] = z;
							result = new DamageVoxelGrid( NULL, size, 0, cell, cellUpper, origin, cellSize, debugNode );
						}
						if ( !result->cells.TestAndSet( result->GetIndex( x, y, z ) ) ) {
							result->setCount++;
						}
						if ( x < result->lower[0] ) {
							result->lower[0] = x;
						}
						if ( x > result->upper[0] ) {
							result->upper[0] = x;
						}
						if ( y < result->lower[1] ) {
							result->lower[1] = y;
						}
						if ( y > result->upper[1] ) {
							result->upper[1] = y;
						}
						if ( z < result->lower[2] ) {
							result->lower[2] = z;
						}
						if ( z > result->upper[2] ) {
							result->upper[2] = z;
						}
					}
				}
			}
		}
	}
	return result;
}

/*
================
DamageVoxelGrid::Difference

A new grid of the cells set in one grid but not the other, over the boxes'
common range; NULL when there are none. Intersect with the test turned
around (invented name; retail has no caller).
================
*/
// FUNCTION: STBC 0x004c5700
// PARKED: register/frame: the test is retail's a ? !b : b layout; retail keeps inThis in a byte slot and inOther in AL (ours slots inOther), shares fromX's slot with another value (frame 0x40 against 0x44) and keeps this in EBP with a stack copy; register picks differ throughout
DamageVoxelGrid *DamageVoxelGrid::Difference( DamageVoxelGrid *other ) {
	DamageVoxelGrid *result = NULL;
	if ( other->lower[0] < upper[0] && other->lower[1] < upper[1] && other->lower[2] < upper[2]
	  && other->upper[0] > lower[0] && other->upper[1] > lower[1] && other->upper[2] > lower[2] ) {
		unsigned short fromX = __min( other->lower[0], lower[0] );
		unsigned short fromY = __min( other->lower[1], lower[1] );
		unsigned short fromZ = __min( other->lower[2], lower[2] );
		unsigned short toX = __min( other->upper[0], upper[0] );
		unsigned short toY = __min( other->upper[1], upper[1] );
		unsigned short toZ = __min( other->upper[2], upper[2] );
		for ( unsigned short z = fromZ; z < toZ; z++ ) {
			for ( unsigned short y = fromY; y < toY; y++ ) {
				for ( unsigned short x = fromX; x < toX; x++ ) {
					bool inThis = IsSet( x, y, z );
					bool inOther = other->IsSet( x, y, z );
					if ( inThis ? !inOther : inOther ) {
						if ( result == NULL ) {
							unsigned short cell[3];
							unsigned short cellUpper[3];
							cellUpper[0] = x;
							cellUpper[1] = y;
							cellUpper[2] = z;
							cell[0] = x;
							cell[1] = y;
							cell[2] = z;
							result = new DamageVoxelGrid( NULL, size, 0, cell, cellUpper, origin, cellSize, debugNode );
						}
						if ( !result->cells.TestAndSet( result->GetIndex( x, y, z ) ) ) {
							result->setCount++;
						}
						if ( x < result->lower[0] ) {
							result->lower[0] = x;
						}
						if ( x > result->upper[0] ) {
							result->upper[0] = x;
						}
						if ( y < result->lower[1] ) {
							result->lower[1] = y;
						}
						if ( y > result->upper[1] ) {
							result->upper[1] = y;
						}
						if ( z < result->lower[2] ) {
							result->lower[2] = z;
						}
						if ( z > result->upper[2] ) {
							result->upper[2] = z;
						}
					}
				}
			}
		}
	}
	return result;
}

/*
================
DamageVoxelGrid::DamageVoxelGrid

The box runs from lower to upper, both included. The bits are copied
from source when there is one; otherwise they start clear, or all set
when setCount_ covers the whole grid.
================
*/
// FUNCTION: STBC 0x004c5a50
DamageVoxelGrid::DamageVoxelGrid( const unsigned char *source, const unsigned short *size_, unsigned int setCount_,
								  const unsigned short *lower_, const unsigned short *upper_, const NiPoint3 &origin_,
								  float cellSize_, NiNode *debugNode_ ) {
	center = ( NiPoint3( lower_[0] * cellSize_, lower_[1] * cellSize_, lower_[2] * cellSize_ )
			 + NiPoint3( upper_[0] * cellSize_, upper_[1] * cellSize_, upper_[2] * cellSize_ ) ) * 0.5f + origin_;
	origin = origin_;
	cellSize = cellSize_;
	debugNode = debugNode_;
	setCount = setCount_;
	size[0] = size_[0];
	size[1] = size_[1];
	size[2] = size_[2];
	lower[0] = lower_[0];
	lower[1] = lower_[1];
	lower[2] = lower_[2];
	upper[0] = upper_[0] + 1;
	upper[1] = upper_[1] + 1;
	upper[2] = upper_[2] + 1;
	cells.SetSize( GetCellCount() );
	if ( source != NULL ) {
		memcpy( cells.bits, source, cells.GetByteCount() );
	} else if ( setCount_ == 0 ) {
		if ( cells.bits != NULL ) {
			memset( cells.bits, 0, cells.GetByteCount() );
		}
	} else if ( setCount_ == GetCellCount() ) {
		if ( cells.bits != NULL ) {
			memset( cells.bits, 0xFF, cells.GetByteCount() );
		}
	}
}

/*
================
DamageVoxelGrid::CreateLines

Three crossed diagonals inside each set cell, half a cell long, and the
twelve edges of the box, as one NiLines in an unlit, depth-tested colour.
A vertex's flag joins it to the next one (invented name).
================
*/
// FUNCTION: STBC 0x004c5d00
// DRAFT: calls, branches and stores as retail; the counting loop keeps its inner trip count in a register where retail spills it, and the cell loop rotates its entry differently (ours jumps into the x loop and keeps vertices in ebp, retail in ebx)
NiAVObject *DamageVoxelGrid::CreateLines( const NiColor *color ) {
	unsigned int setCells = 0;
	unsigned short x, y, z;
	for ( z = 0; z < size[2]; z++ ) {
		for ( y = 0; y < size[1]; y++ ) {
			for ( x = 0; x < size[0]; x++ ) {
				if ( cells.IsSet( GetIndex( x, y, z ) ) ) {
					setCells++;
				}
			}
		}
	}

	// six vertices per cell, sixteen for the box
	unsigned int vertexCount = setCells * 6 + 16;
	NiPoint3 *vertices = new NiPoint3[vertexCount];
	bool *connect = new bool[vertexCount];
	unsigned int n = 0;

	for ( z = 0; z < size[2]; z++ ) {
		for ( y = 0; y < size[1]; y++ ) {
			for ( x = 0; x < size[0]; x++ ) {
				if ( cells.IsSet( GetIndex( x, y, z ) ) ) {
					NiPoint3 cellCenter( ( x + 0.5f ) * cellSize + origin.x, ( y + 0.5f ) * cellSize + origin.y,
										 ( z + 0.5f ) * cellSize + origin.z );
					float half = cellSize * 0.25f;
					vertices[n] = cellCenter - NiPoint3( half, half, half );
					connect[n++] = true;
					vertices[n] = cellCenter + NiPoint3( half, half, half );
					connect[n++] = false;
					vertices[n] = cellCenter - NiPoint3( half, -half, half );
					connect[n++] = true;
					vertices[n] = cellCenter + NiPoint3( half, -half, half );
					connect[n++] = false;
					vertices[n] = cellCenter - NiPoint3( -half, half, half );
					connect[n++] = true;
					vertices[n] = cellCenter + NiPoint3( -half, half, half );
					connect[n++] = false;
				}
			}
		}
	}

	// the bottom face as a loop
	NiPoint3 bottom0( lower[0] * cellSize + origin.x, lower[1] * cellSize + origin.y, lower[2] * cellSize + origin.z );
	NiPoint3 bottom1( upper[0] * cellSize + origin.x, lower[1] * cellSize + origin.y, lower[2] * cellSize + origin.z );
	NiPoint3 bottom2( upper[0] * cellSize + origin.x, upper[1] * cellSize + origin.y, lower[2] * cellSize + origin.z );
	NiPoint3 bottom3( lower[0] * cellSize + origin.x, upper[1] * cellSize + origin.y, lower[2] * cellSize + origin.z );
	vertices[n] = bottom0;
	connect[n++] = true;
	vertices[n] = bottom1;
	connect[n++] = true;
	vertices[n] = bottom2;
	connect[n++] = true;
	vertices[n] = bottom3;
	connect[n++] = true;
	vertices[n] = bottom0;
	connect[n++] = true;

	// up to the top face, as a loop
	NiPoint3 top0( lower[0] * cellSize + origin.x, lower[1] * cellSize + origin.y, upper[2] * cellSize + origin.z );
	NiPoint3 top1( upper[0] * cellSize + origin.x, lower[1] * cellSize + origin.y, upper[2] * cellSize + origin.z );
	NiPoint3 top2( upper[0] * cellSize + origin.x, upper[1] * cellSize + origin.y, upper[2] * cellSize + origin.z );
	NiPoint3 top3( lower[0] * cellSize + origin.x, upper[1] * cellSize + origin.y, upper[2] * cellSize + origin.z );
	vertices[n] = top0;
	connect[n++] = true;
	vertices[n] = top1;
	connect[n++] = true;
	vertices[n] = top2;
	connect[n++] = true;
	vertices[n] = top3;
	connect[n++] = true;
	vertices[n] = top0;
	connect[n++] = false;

	// the other three upright edges
	vertices[n] = top1;
	connect[n++] = true;
	vertices[n] = bottom1;
	connect[n++] = false;
	vertices[n] = top2;
	connect[n++] = true;
	vertices[n] = bottom2;
	connect[n++] = false;
	vertices[n] = top3;
	connect[n++] = true;
	vertices[n] = bottom3;
	connect[n++] = false;

	NiLines *lines = new NiLines( n, vertices, NULL, NULL, connect );

	NiMaterialProperty *material = new NiMaterialProperty;
	material->SetEmittance( *color );
	lines->AttachProperty( material );

	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_IGNORE );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	lines->AttachProperty( vertexColor );

	NiZBufferProperty *zBuffer = new NiZBufferProperty;
	zBuffer->SetZBufferTest( true );
	zBuffer->SetZBufferWrite( true );
	lines->AttachProperty( zBuffer );

	lines->UpdateProperties();
	return lines;
}
