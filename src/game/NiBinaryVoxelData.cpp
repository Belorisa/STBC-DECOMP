/*
===============================================================================

	NiBinaryVoxelData.cpp

===============================================================================
*/

#include <iostream>

#include "NiBinaryVoxelData.h"
#include "NiBinaryVoxelExtraData.h"
#include "NiStream.h"
#include "NiNode.h"
#include "NiLines.h"
#include "NiMaterialProperty.h"
#include "NiAlphaProperty.h"
#include "NiVertexColorProperty.h"
#include "NiMain.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004abeb0 _$E45
// FUNCTION: STBC 0x004abec0 _$E44
// FUNCTION: STBC 0x004ac1e0 _$E65
// FUNCTION: STBC 0x004ac1f0 _$E64
// FUNCTION: STBC 0x004ac530 _$E54
// FUNCTION: STBC 0x004ac540 _$E53
// FUNCTION: STBC 0x004ac580 _$E69
// FUNCTION: STBC 0x004ac590 _$E68
// FUNCTION: STBC 0x004ac5a0 _$E73
// FUNCTION: STBC 0x004ac5b0 _$E72
// FUNCTION: STBC 0x004ac5c0 _$E77
// FUNCTION: STBC 0x004ac5d0 _$E76
// FUNCTION: STBC 0x004ac5e0 _$E81
// FUNCTION: STBC 0x004ac5f0 _$E80
// FUNCTION: STBC 0x004ac600 _$E200
// FUNCTION: STBC 0x004ac610 _$E199
// FUNCTION: STBC 0x004ac620 _$E217
// FUNCTION: STBC 0x004ac630 _$E216
// FUNCTION: STBC 0x004ac640 _$E224
// FUNCTION: STBC 0x004ac650 _$E223
// FUNCTION: STBC 0x004ac750 _$E228
// FUNCTION: STBC 0x004ac760 _$E227
// FUNCTION: STBC 0x004ac770 _$E232
// FUNCTION: STBC 0x004ac780 _$E231
// FUNCTION: STBC 0x004ac790 _$E205
// FUNCTION: STBC 0x004ac7a0 _$E204
// FUNCTION: STBC 0x004ac7b0 _$E209
// FUNCTION: STBC 0x004ac7c0 _$E208
// FUNCTION: STBC 0x004ac7d0 _$E100
// FUNCTION: STBC 0x004ac7e0 _$E99
// FUNCTION: STBC 0x004ac7f0 _$E125
// FUNCTION: STBC 0x004ac800 _$E124
// FUNCTION: STBC 0x004ac810 _$E185
// FUNCTION: STBC 0x004ac820 _$E184

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004aa770 _$E32
// FUNCTION: STBC 0x004aa780 _$E29
// FUNCTION: STBC 0x004aa790 _$E31
// FUNCTION: STBC 0x004aa7a0 _$E30
// FUNCTION: STBC 0x004aa7b0 _$E38
// FUNCTION: STBC 0x004aa7c0 _$E35
// FUNCTION: STBC 0x004aa7d0 _$E37
// FUNCTION: STBC 0x004aa7e0 _$E36
// and the locale facet id's pair that <iostream> brings
// FUNCTION: STBC 0x004ac720 _$E435
// FUNCTION: STBC 0x004ac740 _$E434

// The initialisers and exit-time destructors of the two classes' NiRTTI.
// FUNCTION: STBC 0x004a7a80 _$E414
// FUNCTION: STBC 0x004a7a90 _$E410
// FUNCTION: STBC 0x004a7ab0 _$E413
// FUNCTION: STBC 0x004a7ac0 _$E411
// FUNCTION: STBC 0x004abed0 _$E426
// FUNCTION: STBC 0x004abee0 _$E422
// FUNCTION: STBC 0x004abf00 _$E425
// FUNCTION: STBC 0x004abf10 _$E423

// How far from a tree's plane a point still counts as on it.
static const float EPSILON = 0.0001f;

// DATA: STBC 0x983930 ?rtti@NiBinaryVoxelData@@2VNiRTTI@@B
const NiRTTI NiBinaryVoxelData::rtti( "NiBinaryVoxelData", &NiObject::rtti );

/*
================
NiBinaryVoxelData::Register
================
*/
// FUNCTION: STBC 0x004a56a0 ?Register@NiBinaryVoxelData@@SAHXZ
NiImplementRegister( NiBinaryVoxelData )

/*
================
NiBinaryVoxelData::CreateObject

Creates the data and loads it.
================
*/
// FUNCTION: STBC 0x004a57f0
void NiBinaryVoxelData::CreateObject( NiStream &stream, NiObject *&object ) {
	NiBinaryVoxelData *data = new NiBinaryVoxelData;
	stream.SetLoadRecord( new NiObjectLoadRecord );
	data->LoadBinary( stream );
	object = data;
}

/*
================
NiBinaryVoxelData::LinkObject
================
*/
// FUNCTION: STBC 0x004a5880
void NiBinaryVoxelData::LinkObject( NiStream &stream ) {
	NiObject::LinkObject( stream );
}

/*
================
NiBinaryVoxelData::LoadBinary

The bit arrays hold one bit per cell. The block's pointers are saved as
offsets from its start.
================
*/
// FUNCTION: STBC 0x004a5890
void NiBinaryVoxelData::LoadBinary( NiStream &stream ) {
	NiObject::LoadBinary( stream );

	stream.GetIstr()->Read( &sizeX, sizeof( sizeX ) );
	stream.GetIstr()->Read( &sizeY, sizeof( sizeY ) );
	stream.GetIstr()->Read( &sizeZ, sizeof( sizeZ ) );
	stream.GetIstr()->Read( &unknown10, sizeof( unknown10 ) );
	stream.GetIstr()->Read( &unknown14, sizeof( unknown14 ) );
	stream.GetIstr()->Read( &unknown20, sizeof( unknown20 ) );

	int bitBytes = GetCellCount() / 8 + 1;
	cellBits = new unsigned char[bitBytes];
	stream.GetIstr()->Read( cellBits, bitBytes );
	unknown4C[0] = new unsigned char[bitBytes];
	stream.GetIstr()->Read( unknown4C[0], bitBytes );
	unknown4C[1] = new unsigned char[bitBytes];
	stream.GetIstr()->Read( unknown4C[1], bitBytes );
	unknown4C[2] = new unsigned char[bitBytes];
	stream.GetIstr()->Read( unknown4C[2], bitBytes );
	unknown58[0] = new unsigned char[bitBytes];
	stream.GetIstr()->Read( unknown58[0], bitBytes );
	unknown58[1] = new unsigned char[bitBytes];
	stream.GetIstr()->Read( unknown58[1], bitBytes );
	unknown58[2] = new unsigned char[bitBytes];
	stream.GetIstr()->Read( unknown58[2], bitBytes );

	stream.GetIstr()->Read( &planeCount, sizeof( planeCount ) );
	planes = new NiPlane[planeCount];
	stream.GetIstr()->Read( planes, planeCount * sizeof( NiPlane ) );

	stream.GetIstr()->Read( &blockSize, sizeof( blockSize ) );
	block = new char[blockSize];
	stream.GetIstr()->Read( block, blockSize );

	unsigned int offset;
	stream.GetIstr()->Read( &offset, sizeof( offset ) );
	nodes = (VoxelBSPNode *)( block + offset );
	stream.GetIstr()->Read( &offset, sizeof( offset ) );
	rows = block + offset;
	stream.GetIstr()->Read( &offset, sizeof( offset ) );
	surfaceRows = block + offset;
	stream.GetIstr()->Read( &offset, sizeof( offset ) );
	slices = (VoxelSlice *)( block + offset );
	stream.GetIstr()->Read( &offset, sizeof( offset ) );
	sliceRefs = (VoxelSliceRef *)( block + offset );

	OffsetsToPointers();
}

/*
================
NiBinaryVoxelData::RegisterStreamables
================
*/
// FUNCTION: STBC 0x004a5b10
bool NiBinaryVoxelData::RegisterStreamables( NiStream &stream ) {
	return NiObject::RegisterStreamables( stream );
}

/*
================
NiBinaryVoxelData::SaveBinary

Writes the block with its pointers turned into offsets, then turns them
back.
================
*/
// FUNCTION: STBC 0x004a5b20
void NiBinaryVoxelData::SaveBinary( NiStream &stream ) {
	NiObject::SaveBinary( stream );

	stream.GetOstr()->Write( &sizeX, sizeof( sizeX ) );
	stream.GetOstr()->Write( &sizeY, sizeof( sizeY ) );
	stream.GetOstr()->Write( &sizeZ, sizeof( sizeZ ) );
	stream.GetOstr()->Write( &unknown10, sizeof( unknown10 ) );
	stream.GetOstr()->Write( &unknown14, sizeof( unknown14 ) );
	stream.GetOstr()->Write( &unknown20, sizeof( unknown20 ) );

	int bitBytes = GetCellCount() / 8 + 1;
	stream.GetOstr()->Write( cellBits, bitBytes );
	stream.GetOstr()->Write( unknown4C[0], bitBytes );
	stream.GetOstr()->Write( unknown4C[1], bitBytes );
	stream.GetOstr()->Write( unknown4C[2], bitBytes );
	stream.GetOstr()->Write( unknown58[0], bitBytes );
	stream.GetOstr()->Write( unknown58[1], bitBytes );
	stream.GetOstr()->Write( unknown58[2], bitBytes );

	stream.GetOstr()->Write( &planeCount, sizeof( planeCount ) );
	stream.GetOstr()->Write( planes, planeCount * sizeof( NiPlane ) );

	stream.GetOstr()->Write( &blockSize, sizeof( blockSize ) );
	PointersToOffsets();
	stream.GetOstr()->Write( block, blockSize );

	unsigned int offset;
	offset = (char *)nodes - block;
	stream.GetOstr()->Write( &offset, sizeof( offset ) );
	offset = rows - block;
	stream.GetOstr()->Write( &offset, sizeof( offset ) );
	offset = surfaceRows - block;
	stream.GetOstr()->Write( &offset, sizeof( offset ) );
	offset = (char *)slices - block;
	stream.GetOstr()->Write( &offset, sizeof( offset ) );
	offset = (char *)sliceRefs - block;
	stream.GetOstr()->Write( &offset, sizeof( offset ) );

	OffsetsToPointers();
}

/*
================
NiBinaryVoxelData::IsEqual

Any other object counts as equal.
================
*/
// FUNCTION: STBC 0x004a5ce0
bool NiBinaryVoxelData::IsEqual( NiObject *object ) {
	return object ? true : false;
}

/*
===============================================================================

	The surface cells' BSP trees

===============================================================================
*/

/*
================
VoxelBSPNode::IsInside

Walks down to the side of each plane the point lies on.
================
*/
// FUNCTION: STBC 0x004a5cf0
bool VoxelBSPNode::IsInside( VoxelBSPNode *nodes, NiPlane *planes, const NiPoint3 &point ) {
	float distance = planes[plane].Distance( point );
	if ( front != 0 && back != 0 ) {
		if ( distance > 0.0f ) {
			return nodes[front].IsInside( nodes, planes, point );
		}
		return nodes[back].IsInside( nodes, planes, point );
	}
	if ( front != 0 ) {
		if ( distance < 0.0f ) {
			return true;
		}
		return nodes[front].IsInside( nodes, planes, point );
	}
	if ( distance < 0.0f ) {
		if ( back != 0 ) {
			return nodes[back].IsInside( nodes, planes, point );
		}
		return true;
	}
	return false;
}


/*
================
VoxelBSPNode::IsSegmentInside

A segment wholly on one side of the plane goes down that side; one that
crosses it is split where it crosses, and each part goes down its side.
================
*/
// FUNCTION: STBC 0x004a5dc0
// PARKED: x87 order in the crossing's t: retail sums the numerator's dot as z, x, y (this build z, y, x); named numerator/toStart, operator*, normal.Dot( toStart ), a const splitter and onPlane as c * n change nothing
bool VoxelBSPNode::IsSegmentInside( VoxelBSPNode *nodes, NiPlane *planes, const NiPoint3 *start, const NiPoint3 *end ) {
	NiPlane &splitter = planes[plane];
	float startDistance = splitter.Distance( *start );
	float endDistance = splitter.Distance( *end );
	if ( startDistance >= -EPSILON && endDistance >= -EPSILON ) {
		if ( front != 0 && nodes[front].IsSegmentInside( nodes, planes, start, end ) ) {
			return true;
		}
		return false;
	}
	if ( startDistance <= EPSILON && endDistance <= EPSILON ) {
		if ( back == 0 ) {
			return true;
		}
		return nodes[back].IsSegmentInside( nodes, planes, start, end );
	}
	if ( ( startDistance <= EPSILON || endDistance <= EPSILON ) && back == 0 ) {
		return true;
	}

	// where the segment crosses the plane
	NiPoint3 onPlane = splitter.GetNormal() * splitter.GetConstant();
	NiPoint3 direction = *end - *start;
	float denominator = -direction.Dot( splitter.GetNormal() );
	float t = ( *start - onPlane ).Dot( splitter.GetNormal() ) / denominator;
	NiPoint3 crossing = direction * t + *start;

	if ( front != 0 && ( startDistance >= -EPSILON || endDistance >= -EPSILON ) ) {
		if ( startDistance >= 0.0f ) {
			if ( nodes[front].IsSegmentInside( nodes, planes, start, &crossing ) ) {
				return true;
			}
		} else {
			if ( nodes[front].IsSegmentInside( nodes, planes, &crossing, end ) ) {
				return true;
			}
		}
	}
	if ( back != 0 && ( startDistance <= EPSILON || endDistance <= EPSILON ) ) {
		if ( startDistance <= 0.0f ) {
			if ( nodes[back].IsSegmentInside( nodes, planes, start, &crossing ) ) {
				return true;
			}
		} else {
			if ( nodes[back].IsSegmentInside( nodes, planes, &crossing, end ) ) {
				return true;
			}
		}
	}
	return false;
}

/*
================
VoxelBSPNode::IsTriangleInside

A triangle wholly on one side of the plane goes down that side; one that
crosses it is cut along the plane into a front and a back polygon of
three or four corners, as Split cuts a polygon, and each goes down its
side as one or two triangles.
================
*/
// FUNCTION: STBC 0x004a6090
// DRAFT: inline set and frame: retail (frame 0xB0) keeps this in EBX, expands the first edge's vector math and calls NiPoint3's constructor, operators and Dot out of line for the second and third edges; this build expands differently and spills this; the control flow follows retail's
bool VoxelBSPNode::IsTriangleInside( VoxelBSPNode *nodes, NiPlane *planes, const NiPoint3 *point0, const NiPoint3 *point1, const NiPoint3 *point2 ) {
	NiPlane &splitter = planes[plane];
	float distance0 = splitter.Distance( *point0 );
	float distance1 = splitter.Distance( *point1 );
	float distance2 = splitter.Distance( *point2 );
	// BUG: the two tests for a triangle on one side leave out the third corner
	if ( distance0 >= -EPSILON && distance1 >= -EPSILON ) {
		if ( front == 0 ) {
			return false;
		}
		return nodes[front].IsTriangleInside( nodes, planes, point0, point1, point2 );
	}
	if ( distance0 <= EPSILON && distance1 <= EPSILON ) {
		if ( back == 0 ) {
			return true;
		}
		return nodes[back].IsTriangleInside( nodes, planes, point0, point1, point2 );
	}
	// BUG: redundant: the test above has returned for these distances
	if ( back == 0 && distance0 <= EPSILON && distance1 <= EPSILON ) {
		return true;
	}

	NiPoint3 onPlane = splitter.GetNormal() * splitter.GetConstant();
	int backCount = 0;
	int frontCount = 0;
	NiPoint3 frontPoints[4];
	NiPoint3 backPoints[4];
	bool isOnPlane0 = (float)fabs( distance0 ) <= EPSILON;
	bool isOnPlane1 = (float)fabs( distance1 ) <= EPSILON;
	bool isOnPlane2 = (float)fabs( distance2 ) <= EPSILON;

	// the edge from point0 to point1
	if ( isOnPlane0 || isOnPlane1 ) {
		if ( isOnPlane0 && isOnPlane1 ) {
			frontPoints[frontCount++] = *point1;
			backPoints[backCount++] = *point1;
		} else if ( ( isOnPlane0 ? distance1 : distance0 ) > 0.0f ) {
			frontPoints[frontCount++] = *point1;
		} else {
			backPoints[backCount++] = *point1;
		}
	} else if ( distance1 >= 0.0f ) {
		if ( distance0 >= 0.0f ) {
			frontPoints[frontCount++] = *point1;
		} else {
			NiPoint3 direction = *point1 - *point0;
			float t = ( *point0 - onPlane ).Dot( splitter.GetNormal() ) / -direction.Dot( splitter.GetNormal() );
			NiPoint3 crossing = direction * t + *point0;
			frontPoints[frontCount++] = crossing;
			frontPoints[frontCount++] = *point1;
			backPoints[backCount++] = crossing;
		}
	} else if ( distance0 >= 0.0f ) {
		NiPoint3 direction = *point1 - *point0;
		float t = ( *point0 - onPlane ).Dot( splitter.GetNormal() ) / -direction.Dot( splitter.GetNormal() );
		NiPoint3 crossing = direction * t + *point0;
		frontPoints[frontCount++] = crossing;
		backPoints[backCount++] = crossing;
		backPoints[backCount++] = *point1;
	} else {
		backPoints[backCount++] = *point1;
	}

	// the edge from point1 to point2
	if ( isOnPlane1 || isOnPlane2 ) {
		if ( isOnPlane1 && isOnPlane2 ) {
			frontPoints[frontCount++] = *point2;
			backPoints[backCount++] = *point2;
		} else if ( ( isOnPlane1 ? distance2 : distance1 ) > 0.0f ) {
			frontPoints[frontCount++] = *point2;
		} else {
			backPoints[backCount++] = *point2;
		}
	} else if ( distance2 >= 0.0f ) {
		if ( distance1 >= 0.0f ) {
			frontPoints[frontCount++] = *point2;
		} else {
			NiPoint3 direction = *point2 - *point1;
			float t = ( *point1 - onPlane ).Dot( splitter.GetNormal() ) / -direction.Dot( splitter.GetNormal() );
			NiPoint3 crossing = direction * t + *point1;
			frontPoints[frontCount++] = crossing;
			frontPoints[frontCount++] = *point2;
			backPoints[backCount++] = crossing;
		}
	} else if ( distance1 >= 0.0f ) {
		NiPoint3 direction = *point2 - *point1;
		float t = ( *point1 - onPlane ).Dot( splitter.GetNormal() ) / -direction.Dot( splitter.GetNormal() );
		NiPoint3 crossing = direction * t + *point1;
		frontPoints[frontCount++] = crossing;
		backPoints[backCount++] = crossing;
		backPoints[backCount++] = *point2;
	} else {
		backPoints[backCount++] = *point2;
	}

	// the edge from point2 to point0
	if ( isOnPlane2 || isOnPlane0 ) {
		if ( isOnPlane2 && isOnPlane0 ) {
			frontPoints[frontCount++] = *point0;
			backPoints[backCount++] = *point0;
		} else if ( ( isOnPlane2 ? distance0 : distance2 ) > 0.0f ) {
			frontPoints[frontCount++] = *point0;
		} else {
			backPoints[backCount++] = *point0;
		}
	} else if ( distance0 >= 0.0f ) {
		if ( distance2 >= 0.0f ) {
			frontPoints[frontCount++] = *point0;
		} else {
			NiPoint3 direction = *point0 - *point2;
			float t = ( *point2 - onPlane ).Dot( splitter.GetNormal() ) / -direction.Dot( splitter.GetNormal() );
			NiPoint3 crossing = direction * t + *point2;
			frontPoints[frontCount++] = crossing;
			frontPoints[frontCount++] = *point0;
			backPoints[backCount++] = crossing;
		}
	} else if ( distance2 >= 0.0f ) {
		NiPoint3 direction = *point0 - *point2;
		float t = ( *point2 - onPlane ).Dot( splitter.GetNormal() ) / -direction.Dot( splitter.GetNormal() );
		NiPoint3 crossing = direction * t + *point2;
		frontPoints[frontCount++] = crossing;
		backPoints[backCount++] = crossing;
		backPoints[backCount++] = *point0;
	} else {
		backPoints[backCount++] = *point0;
	}

	if ( frontCount >= 3 && front != 0 ) {
		if ( nodes[front].IsTriangleInside( nodes, planes, &frontPoints[0], &frontPoints[1], &frontPoints[2] ) ) {
			return true;
		}
		if ( frontCount == 4 && nodes[front].IsTriangleInside( nodes, planes, &frontPoints[0], &frontPoints[2], &frontPoints[3] ) ) {
			return true;
		}
	}
	if ( backCount >= 3 && back != 0 ) {
		if ( nodes[back].IsTriangleInside( nodes, planes, &backPoints[0], &backPoints[1], &backPoints[2] ) ) {
			return true;
		}
		if ( backCount == 4 && nodes[back].IsTriangleInside( nodes, planes, &backPoints[0], &backPoints[2], &backPoints[3] ) ) {
			return true;
		}
	}
	return false;
}

/*
================
VoxelBSPNode::ClipBack

Walks the polygon's edges and keeps the vertices behind the plane, with
the points where the edges cross it.
================
*/
// FUNCTION: STBC 0x004a6d50
// DRAFT: calls and branches as retail; left is the x87 operand order of the two distances' products (the plane's normal against the vertex) and of the crossing's division
void VoxelBSPNode::ClipBack( VoxelBSPNode *nodes, NiPlane *planes, VoxelPolygon *polygon, VoxelPolygon **back ) {
	NiPlane &splitter = planes[plane];
	*back = NULL;
	NiPoint3 onPlane = splitter.GetNormal() * splitter.GetConstant();
	NiPoint3 *vertices = polygon->vertices;
	unsigned short previous = polygon->count - 1;
	for ( unsigned short i = 0; i < polygon->count; i++ ) {
		NiPoint3 &vertex = vertices[i];
		NiPoint3 &last = vertices[previous];
		float distance = splitter.Distance( vertex );
		float lastDistance = splitter.Distance( last );
		bool isOnPlane = NiAbs( distance ) <= EPSILON;
		bool lastIsOnPlane = NiAbs( lastDistance ) <= EPSILON;
		if ( isOnPlane || lastIsOnPlane ) {
			if ( ( isOnPlane ? lastDistance : distance ) < 0.0f || ( isOnPlane && lastIsOnPlane ) ) {
				if ( *back == NULL ) {
					*back = new VoxelPolygon( polygon->capacity + 1 );
				}
				( *back )->vertices[( *back )->count++] = vertex;
			}
		} else if ( distance >= 0.0f ) {
			if ( lastDistance < 0.0f ) {
				NiPoint3 direction = vertex - last;
				float t = ( last - onPlane ).Dot( splitter.GetNormal() ) / -direction.Dot( splitter.GetNormal() );
				NiPoint3 crossing = direction * t + last;
				if ( *back == NULL ) {
					*back = new VoxelPolygon( polygon->capacity + 1 );
				}
				( *back )->vertices[( *back )->count++] = crossing;
			}
		} else if ( lastDistance >= 0.0f ) {
			NiPoint3 direction = vertex - last;
			float t = ( last - onPlane ).Dot( splitter.GetNormal() ) / -direction.Dot( splitter.GetNormal() );
			NiPoint3 crossing = direction * t + last;
			if ( *back == NULL ) {
				*back = new VoxelPolygon( polygon->capacity + 1 );
			}
			( *back )->vertices[( *back )->count++] = crossing;
			( *back )->vertices[( *back )->count++] = vertex;
		} else {
			if ( *back == NULL ) {
				*back = new VoxelPolygon( polygon->capacity + 1 );
			}
			( *back )->vertices[( *back )->count++] = vertex;
		}
		previous = i;
	}
}

/*
================
VoxelBSPNode::Split

Walks the polygon's edges and sorts the vertices by their side of the
plane; a point where an edge crosses the plane goes to both parts, and
so does a vertex of an edge that lies in the plane.
================
*/
// FUNCTION: STBC 0x004a7150
// DRAFT: calls and branches as retail; left is the register order of the prologue (retail loads front into EBX, back into EBP and the planes into ESI before clearing the outputs) and the x87 operand order of the two distances' products
void VoxelBSPNode::Split( VoxelBSPNode *nodes, NiPlane *planes, VoxelPolygon *polygon, VoxelPolygon **front, VoxelPolygon **back ) {
	NiPlane &splitter = planes[plane];
	*front = NULL;
	*back = NULL;
	NiPoint3 onPlane = splitter.GetNormal() * splitter.GetConstant();
	NiPoint3 *vertices = polygon->vertices;
	unsigned short previous = polygon->count - 1;
	for ( unsigned short i = 0; i < polygon->count; i++ ) {
		NiPoint3 &vertex = vertices[i];
		NiPoint3 &last = vertices[previous];
		float distance = splitter.Distance( vertex );
		float lastDistance = splitter.Distance( last );
		bool isOnPlane = (float)fabs( distance ) <= EPSILON;
		bool lastIsOnPlane = (float)fabs( lastDistance ) <= EPSILON;
		if ( isOnPlane || lastIsOnPlane ) {
			if ( isOnPlane && lastIsOnPlane ) {
				if ( *front == NULL ) {
					*front = new VoxelPolygon( polygon->capacity + 1 );
				}
				( *front )->vertices[( *front )->count++] = vertex;
				if ( *back == NULL ) {
					*back = new VoxelPolygon( polygon->capacity + 1 );
				}
				( *back )->vertices[( *back )->count++] = vertex;
			} else if ( ( isOnPlane ? lastDistance : distance ) > 0.0f ) {
				if ( *front == NULL ) {
					*front = new VoxelPolygon( polygon->capacity + 1 );
				}
				( *front )->vertices[( *front )->count++] = vertex;
			} else {
				if ( *back == NULL ) {
					*back = new VoxelPolygon( polygon->capacity + 1 );
				}
				( *back )->vertices[( *back )->count++] = vertex;
			}
		} else if ( distance >= 0.0f ) {
			if ( lastDistance >= 0.0f ) {
				if ( *front == NULL ) {
					*front = new VoxelPolygon( polygon->capacity + 1 );
				}
				( *front )->vertices[( *front )->count++] = vertex;
			} else {
				NiPoint3 direction = vertex - last;
				float t = ( last - onPlane ).Dot( splitter.GetNormal() ) / -direction.Dot( splitter.GetNormal() );
				NiPoint3 crossing = direction * t + last;
				if ( *front == NULL ) {
					*front = new VoxelPolygon( polygon->capacity + 1 );
				}
				( *front )->vertices[( *front )->count++] = crossing;
				( *front )->vertices[( *front )->count++] = vertex;
				if ( *back == NULL ) {
					*back = new VoxelPolygon( polygon->capacity + 1 );
				}
				( *back )->vertices[( *back )->count++] = crossing;
			}
		} else if ( lastDistance >= 0.0f ) {
			NiPoint3 direction = vertex - last;
			float t = ( last - onPlane ).Dot( splitter.GetNormal() ) / -direction.Dot( splitter.GetNormal() );
			NiPoint3 crossing = direction * t + last;
			if ( *front == NULL ) {
				*front = new VoxelPolygon( polygon->capacity + 1 );
			}
			( *front )->vertices[( *front )->count++] = crossing;
			if ( *back == NULL ) {
				*back = new VoxelPolygon( polygon->capacity + 1 );
			}
			( *back )->vertices[( *back )->count++] = crossing;
			( *back )->vertices[( *back )->count++] = vertex;
		} else {
			if ( *back == NULL ) {
				*back = new VoxelPolygon( polygon->capacity + 1 );
			}
			( *back )->vertices[( *back )->count++] = vertex;
		}
		previous = i;
	}
}

/*
================
VoxelBSPNode::ClipInside

Sends the polygon's parts down the tree. What ends up behind a plane
with no node on its negative side is inside the model.
================
*/
// FUNCTION: STBC 0x004a7730
void VoxelBSPNode::ClipInside( VoxelBSPNode *nodes, NiPlane *planes, VoxelPolygon *polygon, VoxelPolygonList *inside ) {
	if ( back != 0 && front != 0 ) {
		VoxelPolygon *frontPart = NULL;
		VoxelPolygon *backPart = NULL;
		Split( nodes, planes, polygon, &frontPart, &backPart );
		delete polygon;
		if ( frontPart != NULL ) {
			nodes[front].ClipInside( nodes, planes, frontPart, inside );
		}
		if ( backPart != NULL ) {
			nodes[back].ClipInside( nodes, planes, backPart, inside );
		}
	} else if ( front != 0 ) {
		VoxelPolygon *frontPart = NULL;
		VoxelPolygon *backPart = NULL;
		Split( nodes, planes, polygon, &frontPart, &backPart );
		delete polygon;
		if ( frontPart != NULL ) {
			nodes[front].ClipInside( nodes, planes, frontPart, inside );
		}
		if ( backPart != NULL ) {
			inside->AddTail( backPart );
		}
	} else {
		VoxelPolygon *backPart = NULL;
		ClipBack( nodes, planes, polygon, &backPart );
		delete polygon;
		if ( backPart != NULL ) {
			if ( back != 0 ) {
				nodes[back].ClipInside( nodes, planes, backPart, inside );
			} else {
				inside->AddTail( backPart );
			}
		}
	}
}

/*
================
VoxelBSPTree::ClipInside
================
*/
// FUNCTION: STBC 0x004a7980
void VoxelBSPTree::ClipInside( VoxelPolygon *polygon, VoxelPolygonList *inside ) {
	nodes[root].ClipInside( nodes, planes, polygon, inside );
}

/*
================
VoxelBSPTree::IsInside
================
*/
// FUNCTION: STBC 0x004a79b0
bool VoxelBSPTree::IsInside( const NiPoint3 &point ) {
	return nodes[root].IsInside( nodes, planes, point );
}

/*
================
VoxelBSPTree::IsSegmentInside
================
*/
// FUNCTION: STBC 0x004a79d0
bool VoxelBSPTree::IsSegmentInside( const NiPoint3 *start, const NiPoint3 *end ) {
	return nodes[root].IsSegmentInside( nodes, planes, start, end );
}

/*
================
VoxelBSPTree::IsTriangleInside
================
*/
// FUNCTION: STBC 0x004a7a00
bool VoxelBSPTree::IsTriangleInside( const NiPoint3 *point0, const NiPoint3 *point1, const NiPoint3 *point2 ) {
	return nodes[root].IsTriangleInside( nodes, planes, point0, point1, point2 );
}

/*
================
VoxelBSPTree::IsQuadInside
================
*/
// FUNCTION: STBC 0x004a7a30
bool VoxelBSPTree::IsQuadInside( const NiPoint3 *point0, const NiPoint3 *point1, const NiPoint3 *point2, const NiPoint3 *point3 ) {
	return IsTriangleInside( point0, point1, point2 ) || IsTriangleInside( point0, point2, point3 );
}

/*
================
NiBinaryVoxelData::NiBinaryVoxelData
================
*/
// FUNCTION: STBC 0x004a7ae0
NiBinaryVoxelData::NiBinaryVoxelData() {
	rows = NULL;
	slices = NULL;
	sliceRefs = NULL;
	nodes = NULL;
	surfaceRows = NULL;
	unknown14.x = 0.0f;
	unknown14.y = 0.0f;
	unknown14.z = 0.0f;
	unknown20.x = 0.0f;
	unknown20.y = 0.0f;
	unknown20.z = 0.0f;
	unknown10 = 0.0f;
	block = NULL;
	planes = NULL;
	cellBits = NULL;
	unknown4C[0] = NULL;
	unknown4C[1] = NULL;
	unknown4C[2] = NULL;
	unknown58[0] = NULL;
	unknown58[1] = NULL;
	unknown58[2] = NULL;
}

/*
================
NiBinaryVoxelData::~NiBinaryVoxelData
================
*/
// FUNCTION: STBC 0x004a7b60
NiBinaryVoxelData::~NiBinaryVoxelData() {
	delete [] planes;
	delete [] cellBits;
	delete [] unknown4C[0];
	delete [] unknown4C[1];
	delete [] unknown4C[2];
	delete [] unknown58[0];
	delete [] unknown58[1];
	delete [] unknown58[2];
	delete [] block;
}

/*
===============================================================================

	Line geometry of the voxel data, for looking at it. Nothing in the game
	calls these functions. The names are invented.

===============================================================================
*/

// the rows CreateSolidLines and CreateSurfaceLines have drawn
// DATA: STBC 0x983978
static int solidRowLines;
// DATA: STBC 0x98397c
static int surfaceRowLines;

// how many line vertices a run of solid cells takes: the edges of its box
#define SOLID_SPAN_VERTICES		16
// the bits Unknown4A9190 returns: a cell's 26 neighbours
#define EDGE_BITS				26

/*
================
NiBinaryVoxelData::BuildBits

Fills the bit arrays from the block, for the voxelizer (0x4B0940), once
the block has its pointers. A bit of cellBits says the lowest corner of
the cell is inside the model, a bit of unknown4C[axis] that the edge of
the cell from that corner along the axis meets the model, and a bit of
unknown58 that the face of the cell from that corner meets it (0 the xz
face, 1 the xy face, 2 the yz face). The second pass clears the bits
that give edges to a cell with nothing in it. The name is invented.
================
*/
// FUNCTION: STBC 0x004a7bd0
// DRAFT: frame slots: the same frame size and the same slot groups, but retail puts tree's slot (refs 9 in our packer) before the group of the first pass's temporaries (refs 41), and swaps y, cell and index; in the second pass the bit clears diverge from the third on
void NiBinaryVoxelData::BuildBits() {
	int bitBytes = GetCellCount() / 8 + 1;
	cellBits = new unsigned char[bitBytes];
	memset( cellBits, 0, bitBytes );
	unknown4C[0] = new unsigned char[bitBytes];
	memset( unknown4C[0], 0, bitBytes );
	unknown4C[1] = new unsigned char[bitBytes];
	memset( unknown4C[1], 0, bitBytes );
	unknown4C[2] = new unsigned char[bitBytes];
	memset( unknown4C[2], 0, bitBytes );
	unknown58[0] = new unsigned char[bitBytes];
	memset( unknown58[0], 0, bitBytes );
	unknown58[1] = new unsigned char[bitBytes];
	memset( unknown58[1], 0, bitBytes );
	unknown58[2] = new unsigned char[bitBytes];
	memset( unknown58[2], 0, bitBytes );

	unsigned int index = 0;
	unsigned short x;
	unsigned short y;
	unsigned short z;
	const VoxelBSPNode *cell;
	for ( z = 1; z < sizeZ; z++ ) {
		for ( y = 1; y < sizeY; y++ ) {
			for ( x = 1; x < sizeX; x++ ) {
				unsigned char mask = 1 << ( index & 7 );
				unsigned int byte = index >> 3;
				if ( IsSolid( x, y, z ) ) {
					cellBits[byte] |= mask;
					unknown4C[0][byte] |= mask;
					unknown4C[1][byte] |= mask;
					unknown4C[2][byte] |= mask;
					unknown58[0][byte] |= mask;
					unknown58[1][byte] |= mask;
					unknown58[2][byte] |= mask;
				} else if ( GetSurfaceCell( x, y, z, cell ) ) {
					NiPoint3 corner( x * unknown10 + unknown14.x, y * unknown10 + unknown14.y, z * unknown10 + unknown14.z );
					VoxelBSPTree *tree = GetTree( cell );
					if ( tree->IsInside( corner ) ) {
						cellBits[byte] |= mask;
						unknown4C[0][byte] |= mask;
						unknown4C[1][byte] |= mask;
						unknown4C[2][byte] |= mask;
						unknown58[0][byte] |= mask;
						unknown58[1][byte] |= mask;
						unknown58[2][byte] |= mask;
					} else {
						NiPoint3 cornerX( ( x + 1 ) * unknown10 + unknown14.x, y * unknown10 + unknown14.y, z * unknown10 + unknown14.z );
						if ( tree->IsSegmentInside( &corner, &cornerX ) ) {
							unknown4C[0][byte] |= mask;
							unknown58[0][byte] |= mask;
							unknown58[1][byte] |= mask;
						}
						NiPoint3 cornerY( x * unknown10 + unknown14.x, ( y + 1 ) * unknown10 + unknown14.y, z * unknown10 + unknown14.z );
						if ( tree->IsSegmentInside( &corner, &cornerY ) ) {
							unknown4C[1][byte] |= mask;
							unknown58[1][byte] |= mask;
							unknown58[2][byte] |= mask;
						}
						NiPoint3 cornerZ( x * unknown10 + unknown14.x, y * unknown10 + unknown14.y, ( z + 1 ) * unknown10 + unknown14.z );
						if ( tree->IsSegmentInside( &corner, &cornerZ ) ) {
							unknown4C[2][byte] |= mask;
							unknown58[0][byte] |= mask;
							unknown58[2][byte] |= mask;
						}
						if ( !( unknown58[0][byte] & mask ) ) {
							NiPoint3 cornerXZ( ( x + 1 ) * unknown10 + unknown14.x, y * unknown10 + unknown14.y, ( z + 1 ) * unknown10 + unknown14.z );
							if ( tree->IsQuadInside( &corner, &cornerX, &cornerXZ, &cornerZ ) ) {
								unknown58[0][byte] |= mask;
							}
						}
						if ( !( unknown58[1][byte] & mask ) ) {
							NiPoint3 cornerXY( ( x + 1 ) * unknown10 + unknown14.x, ( y + 1 ) * unknown10 + unknown14.y, z * unknown10 + unknown14.z );
							if ( tree->IsQuadInside( &corner, &cornerY, &cornerXY, &cornerX ) ) {
								unknown58[1][byte] |= mask;
							}
						}
						if ( !( unknown58[2][byte] & mask ) ) {
							NiPoint3 cornerYZ( x * unknown10 + unknown14.x, ( y + 1 ) * unknown10 + unknown14.y, ( z + 1 ) * unknown10 + unknown14.z );
							if ( tree->IsQuadInside( &corner, &cornerYZ, &cornerZ, &cornerY ) ) {
								unknown58[2][byte] |= mask;
							}
						}
					}
				}
				index++;
			}
		}
	}

	for ( z = 0; z < sizeZ; z++ ) {
		for ( y = 0; y < sizeY; y++ ) {
			for ( x = 0; x < sizeX; x++ ) {
				unsigned int edges = Unknown4A9190( x, y, z );
				if ( edges != 0 ) {
					if ( !IsSolid( x, y, z ) && !GetSurfaceCell( x, y, z, cell ) ) {
						if ( edges & 0x1 ) {
							ClearCellBit( x, y, z );
						}
						if ( edges & 0x2 ) {
							ClearUnknown4CBit( 0, x, y, z );
						}
						if ( edges & 0x4 ) {
							ClearCellBit( x + 1, y, z );
						}
						if ( edges & 0x8 ) {
							ClearUnknown4CBit( 1, x, y, z );
						}
						if ( edges & 0x10 ) {
							ClearUnknown58Bit( 1, x, y, z );
						}
						if ( edges & 0x20 ) {
							ClearUnknown4CBit( 1, x + 1, y, z );
						}
						if ( edges & 0x40 ) {
							ClearCellBit( x, y + 1, z );
						}
						if ( edges & 0x80 ) {
							ClearUnknown4CBit( 0, x, y + 1, z );
						}
						if ( edges & 0x100 ) {
							ClearCellBit( x + 1, y + 1, z );
						}
						if ( edges & 0x200 ) {
							ClearUnknown4CBit( 2, x, y, z );
						}
						if ( edges & 0x400 ) {
							ClearUnknown58Bit( 0, x, y, z );
						}
						if ( edges & 0x800 ) {
							ClearUnknown4CBit( 2, x + 1, y, z );
						}
						if ( edges & 0x1000 ) {
							ClearUnknown58Bit( 2, x, y, z );
						}
						if ( edges & 0x2000 ) {
							ClearUnknown58Bit( 2, x + 1, y, z );
						}
						if ( edges & 0x4000 ) {
							ClearUnknown4CBit( 2, x, y + 1, z );
						}
						if ( edges & 0x8000 ) {
							ClearUnknown58Bit( 0, x, y + 1, z );
						}
						if ( edges & 0x10000 ) {
							ClearUnknown4CBit( 2, x + 1, y + 1, z );
						}
						if ( edges & 0x20000 ) {
							ClearCellBit( x, y, z + 1 );
						}
						if ( edges & 0x40000 ) {
							ClearUnknown4CBit( 0, x, y, z + 1 );
						}
						if ( edges & 0x80000 ) {
							ClearCellBit( x + 1, y, z + 1 );
						}
						if ( edges & 0x100000 ) {
							ClearUnknown4CBit( 1, x, y, z + 1 );
						}
						if ( edges & 0x200000 ) {
							ClearUnknown58Bit( 1, x, y, z + 1 );
						}
						if ( edges & 0x400000 ) {
							ClearUnknown4CBit( 1, x + 1, y, z + 1 );
						}
						if ( edges & 0x800000 ) {
							ClearCellBit( x, y + 1, z + 1 );
						}
						if ( edges & 0x1000000 ) {
							ClearUnknown4CBit( 0, x, y + 1, z + 1 );
						}
						if ( edges & 0x2000000 ) {
							ClearCellBit( x + 1, y + 1, z + 1 );
						}
						edges = Unknown4A9190( x, y, z );
					}
					// BUG: redundant: the edges of each neighbour are worked out and dropped
					if ( edges & 0x1 ) {
						Unknown4A9190( x - 1, y - 1, z - 1 );
					}
					if ( edges & 0x2 ) {
						Unknown4A9190( x, y - 1, z - 1 );
					}
					if ( edges & 0x4 ) {
						Unknown4A9190( x + 1, y - 1, z - 1 );
					}
					if ( edges & 0x8 ) {
						Unknown4A9190( x - 1, y, z - 1 );
					}
					if ( edges & 0x10 ) {
						Unknown4A9190( x, y, z - 1 );
					}
					if ( edges & 0x20 ) {
						Unknown4A9190( x + 1, y, z - 1 );
					}
					if ( edges & 0x40 ) {
						Unknown4A9190( x - 1, y + 1, z - 1 );
					}
					if ( edges & 0x80 ) {
						Unknown4A9190( x, y + 1, z - 1 );
					}
					if ( edges & 0x100 ) {
						Unknown4A9190( x + 1, y + 1, z - 1 );
					}
					if ( edges & 0x200 ) {
						Unknown4A9190( x - 1, y - 1, z );
					}
					if ( edges & 0x400 ) {
						Unknown4A9190( x, y - 1, z );
					}
					if ( edges & 0x800 ) {
						Unknown4A9190( x + 1, y - 1, z );
					}
					if ( edges & 0x1000 ) {
						Unknown4A9190( x - 1, y, z );
					}
					if ( edges & 0x2000 ) {
						Unknown4A9190( x + 1, y, z );
					}
					if ( edges & 0x4000 ) {
						Unknown4A9190( x - 1, y + 1, z );
					}
					if ( edges & 0x8000 ) {
						Unknown4A9190( x, y + 1, z );
					}
					if ( edges & 0x10000 ) {
						Unknown4A9190( x + 1, y + 1, z );
					}
					if ( edges & 0x20000 ) {
						Unknown4A9190( x - 1, y - 1, z + 1 );
					}
					if ( edges & 0x40000 ) {
						Unknown4A9190( x, y - 1, z + 1 );
					}
					if ( edges & 0x80000 ) {
						Unknown4A9190( x + 1, y - 1, z + 1 );
					}
					if ( edges & 0x100000 ) {
						Unknown4A9190( x - 1, y, z + 1 );
					}
					if ( edges & 0x200000 ) {
						Unknown4A9190( x, y, z + 1 );
					}
					if ( edges & 0x400000 ) {
						Unknown4A9190( x + 1, y, z + 1 );
					}
					if ( edges & 0x800000 ) {
						Unknown4A9190( x - 1, y + 1, z + 1 );
					}
					if ( edges & 0x1000000 ) {
						Unknown4A9190( x, y + 1, z + 1 );
					}
					if ( edges & 0x2000000 ) {
						Unknown4A9190( x + 1, y + 1, z + 1 );
					}
				}
			}
		}
	}
}

/*
================
NiBinaryVoxelData::Unknown4A9190

The surface's crossings around the cell at x, y, z as a bit per edge,
from the bits of the cell and of its seven neighbours towards higher
x, y and z.
================
*/
// FUNCTION: STBC 0x004a9190
// DRAFT: inline budget: retail expands ten bit tests and calls the other sixteen, this build expands eleven (the eleventh needs 144 fewer units of budget)
unsigned int NiBinaryVoxelData::Unknown4A9190( unsigned short x, unsigned short y, unsigned short z ) {
	unsigned int edges = 0;
	if ( TestCellBit( x, y, z ) ) {
		edges = 0x161B;
	} else {
		if ( TestUnknown4CBit( 0, x, y, z ) ) {
			edges = 0x412;
		} else {
			if ( TestUnknown58Bit( 0, x, y, z ) ) {
				edges = 0x400;
			}
			if ( TestUnknown58Bit( 1, x, y, z ) ) {
				edges |= 0x10;
			}
		}
		if ( TestUnknown4CBit( 1, x, y, z ) ) {
			edges |= 0x1018;
		} else if ( TestUnknown58Bit( 2, x, y, z ) ) {
			edges |= 0x1000;
		}
		if ( TestUnknown4CBit( 2, x, y, z ) ) {
			edges |= 0x1600;
		}
	}
	if ( TestCellBit( x + 1, y, z ) ) {
		edges |= 0x2824;
	} else {
		if ( TestUnknown4CBit( 1, x + 1, y, z ) ) {
			edges |= 0x2020;
		} else if ( TestUnknown58Bit( 2, x + 1, y, z ) ) {
			edges |= 0x2000;
		}
		if ( TestUnknown4CBit( 2, x + 1, y, z ) ) {
			edges |= 0x2800;
		}
	}
	if ( TestCellBit( x, y + 1, z ) ) {
		edges |= 0xC0C0;
	} else {
		if ( TestUnknown4CBit( 0, x, y + 1, z ) ) {
			edges |= 0x8080;
		} else if ( TestUnknown58Bit( 0, x, y + 1, z ) ) {
			edges |= 0x8000;
		}
		if ( TestUnknown4CBit( 2, x, y + 1, z ) ) {
			edges |= 0xC000;
		}
	}
	if ( TestCellBit( x + 1, y + 1, z ) ) {
		edges |= 0x10100;
	} else if ( TestUnknown4CBit( 2, x + 1, y + 1, z ) ) {
		edges |= 0x10000;
	}
	if ( TestCellBit( x, y, z + 1 ) ) {
		edges |= 0x360000;
	} else {
		if ( TestUnknown4CBit( 0, x, y, z + 1 ) ) {
			edges |= 0x240000;
		} else if ( TestUnknown58Bit( 1, x, y, z + 1 ) ) {
			edges |= 0x200000;
		}
		if ( TestUnknown4CBit( 1, x, y, z + 1 ) ) {
			edges |= 0x100000;
		}
	}
	if ( TestCellBit( x + 1, y, z + 1 ) ) {
		edges |= 0x480000;
	} else if ( TestUnknown4CBit( 1, x + 1, y, z + 1 ) ) {
		edges |= 0x400000;
	}
	if ( TestCellBit( x, y + 1, z + 1 ) ) {
		edges |= 0x1800000;
	} else if ( TestUnknown4CBit( 0, x, y + 1, z + 1 ) ) {
		edges |= 0x1000000;
	}
	if ( TestCellBit( x + 1, y + 1, z + 1 ) ) {
		edges |= 0x2000000;
	}
	return edges;
}

/*
================
NiBinaryVoxelData::PointersToOffsets
================
*/
// FUNCTION: STBC 0x004a98c0
void NiBinaryVoxelData::PointersToOffsets() {
	int count = sizeZ;
	VoxelSlice *slice = slices;
	for ( int i = 0; i < count; i++ ) {
		sliceRefs[i].ToOffset( (char *)slice );
	}
	do {
		slice = slice->ToOffsets( rows, surfaceRows, nodes );
	} while ( (char *)slice < rows );
}

/*
================
NiBinaryVoxelData::OffsetsToPointers
================
*/
// FUNCTION: STBC 0x004a9910
void NiBinaryVoxelData::OffsetsToPointers() {
	int count = sizeZ;
	VoxelSlice *slice = slices;
	for ( int i = 0; i < count; i++ ) {
		sliceRefs[i].ToPointer( (char *)slice );
	}
	do {
		slice = slice->ToPointers( rows, surfaceRows, nodes );
	} while ( (char *)slice < rows );
}

/*
================
NiBinaryVoxelData::IsSolid

Whether the cell is inside the model.
================
*/
// FUNCTION: STBC 0x004a9960
bool NiBinaryVoxelData::IsSolid( unsigned short x, unsigned short y, unsigned short z ) const {
	VoxelSlice *slice = sliceRefs[z].slice;
	if ( slice != NULL ) {
		return slice->IsSolid( x, y );
	}
	return false;
}

/*
================
NiBinaryVoxelData::GetSpan

The run of solid cells along x that holds the cell.
================
*/
// FUNCTION: STBC 0x004a9990
bool NiBinaryVoxelData::GetSpan( unsigned short x, unsigned short y, unsigned short z, unsigned short &start, unsigned short &end ) const {
	VoxelSlice *slice = sliceRefs[z].slice;
	if ( slice != NULL ) {
		return slice->GetSpan( x, y, start, end );
	}
	return false;
}

/*
================
NiBinaryVoxelData::GetSurfaceCell

The data of the cell when it is on the model's surface.
================
*/
// FUNCTION: STBC 0x004a99d0
bool NiBinaryVoxelData::GetSurfaceCell( unsigned short x, unsigned short y, unsigned short z, const VoxelBSPNode *&cell ) const {
	VoxelSlice *slice = sliceRefs[z].slice;
	if ( slice != NULL ) {
		return slice->GetSurfaceCell( x, y, cell );
	}
	return false;
}

/*
================
NiBinaryVoxelData::GetSurfaceSpan

The run of surface cells along x that holds the cell.
================
*/
// FUNCTION: STBC 0x004a9a00
bool NiBinaryVoxelData::GetSurfaceSpan( unsigned short x, unsigned short y, unsigned short z, unsigned short &start, unsigned short &end, const VoxelBSPNode *&cell ) const {
	VoxelSlice *slice = sliceRefs[z].slice;
	if ( slice != NULL ) {
		return slice->GetSurfaceSpan( x, y, start, end, cell );
	}
	return false;
}

/*
================
NiBinaryVoxelData::CreateEdgeLines

A node with a star of lines in every cell: one line from the cell's
centre towards each neighbour whose bit Unknown4A9190 sets.
================
*/
// FUNCTION: STBC 0x004a9a40
// DRAFT: inline set: retail expands NiPoint3's constructor and operator+ for more of the 26 directions than this build (42 calls against 36) and calls NiColor's constructor, SetEmittance and NiVertexColorProperty's constructor out of line
NiNode *NiBinaryVoxelData::CreateEdgeLines() {
	NiNode *node = new NiNode;
	float reach = unknown10 * 0.4f;
	for ( int z = 0; z < sizeZ; z++ ) {
		for ( int y = 0; y < sizeY; y++ ) {
			for ( int x = 0; x < sizeX; x++ ) {
				int edges = Unknown4A9190( x, y, z );
				if ( edges <= 0 ) {
					continue;
				}
				int count = 0;
				NiPoint3 centre( ( x + 0.5f ) * unknown10 + unknown14.x, ( y + 0.5f ) * unknown10 + unknown14.y, ( z + 0.5f ) * unknown10 + unknown14.z );
				for ( int bit = 0; bit < EDGE_BITS; bit++ ) {
					if ( edges & ( 1 << bit ) ) {
						count++;
					}
				}
				NiPoint3 *vertices = new NiPoint3[count * 2];
				bool *flags = new bool[count * 2];
				int vertex = 0;
				if ( edges & 0x1 ) {
					vertices[vertex] = centre;
					flags[vertex++] = true;
					vertices[vertex] = centre + NiPoint3( -reach, -reach, -reach );
					flags[vertex++] = false;
				}
				if ( edges & 0x2 ) {
					vertices[vertex] = centre;
					flags[vertex++] = true;
					vertices[vertex] = centre + NiPoint3( 0.0f, -reach, -reach );
					flags[vertex++] = false;
				}
				if ( edges & 0x4 ) {
					vertices[vertex] = centre;
					flags[vertex++] = true;
					vertices[vertex] = centre + NiPoint3( reach, -reach, -reach );
					flags[vertex++] = false;
				}
				if ( edges & 0x8 ) {
					vertices[vertex] = centre;
					flags[vertex++] = true;
					vertices[vertex] = centre + NiPoint3( -reach, 0.0f, -reach );
					flags[vertex++] = false;
				}
				if ( edges & 0x10 ) {
					vertices[vertex] = centre;
					flags[vertex++] = true;
					vertices[vertex] = centre + NiPoint3( 0.0f, 0.0f, -reach );
					flags[vertex++] = false;
				}
				if ( edges & 0x20 ) {
					vertices[vertex] = centre;
					flags[vertex++] = true;
					vertices[vertex] = centre + NiPoint3( reach, 0.0f, -reach );
					flags[vertex++] = false;
				}
				if ( edges & 0x40 ) {
					vertices[vertex] = centre;
					flags[vertex++] = true;
					vertices[vertex] = centre + NiPoint3( -reach, reach, -reach );
					flags[vertex++] = false;
				}
				if ( edges & 0x80 ) {
					vertices[vertex] = centre;
					flags[vertex++] = true;
					vertices[vertex] = centre + NiPoint3( 0.0f, reach, -reach );
					flags[vertex++] = false;
				}
				if ( edges & 0x100 ) {
					vertices[vertex] = centre;
					flags[vertex++] = true;
					vertices[vertex] = centre + NiPoint3( reach, reach, -reach );
					flags[vertex++] = false;
				}
				if ( edges & 0x200 ) {
					vertices[vertex] = centre;
					flags[vertex++] = true;
					vertices[vertex] = centre + NiPoint3( -reach, -reach, 0.0f );
					flags[vertex++] = false;
				}
				if ( edges & 0x400 ) {
					vertices[vertex] = centre;
					flags[vertex++] = true;
					vertices[vertex] = centre + NiPoint3( 0.0f, -reach, 0.0f );
					flags[vertex++] = false;
				}
				if ( edges & 0x800 ) {
					vertices[vertex] = centre;
					flags[vertex++] = true;
					vertices[vertex] = centre + NiPoint3( reach, -reach, 0.0f );
					flags[vertex++] = false;
				}
				if ( edges & 0x1000 ) {
					vertices[vertex] = centre;
					flags[vertex++] = true;
					vertices[vertex] = centre + NiPoint3( -reach, 0.0f, 0.0f );
					flags[vertex++] = false;
				}
				if ( edges & 0x2000 ) {
					vertices[vertex] = centre;
					flags[vertex++] = true;
					vertices[vertex] = centre + NiPoint3( reach, 0.0f, 0.0f );
					flags[vertex++] = false;
				}
				if ( edges & 0x4000 ) {
					vertices[vertex] = centre;
					flags[vertex++] = true;
					vertices[vertex] = centre + NiPoint3( -reach, reach, 0.0f );
					flags[vertex++] = false;
				}
				if ( edges & 0x8000 ) {
					vertices[vertex] = centre;
					flags[vertex++] = true;
					vertices[vertex] = centre + NiPoint3( 0.0f, reach, 0.0f );
					flags[vertex++] = false;
				}
				if ( edges & 0x10000 ) {
					vertices[vertex] = centre;
					flags[vertex++] = true;
					vertices[vertex] = centre + NiPoint3( reach, reach, 0.0f );
					flags[vertex++] = false;
				}
				if ( edges & 0x20000 ) {
					vertices[vertex] = centre;
					flags[vertex++] = true;
					vertices[vertex] = centre + NiPoint3( -reach, -reach, reach );
					flags[vertex++] = false;
				}
				if ( edges & 0x40000 ) {
					vertices[vertex] = centre;
					flags[vertex++] = true;
					vertices[vertex] = centre + NiPoint3( 0.0f, -reach, reach );
					flags[vertex++] = false;
				}
				if ( edges & 0x80000 ) {
					vertices[vertex] = centre;
					flags[vertex++] = true;
					vertices[vertex] = centre + NiPoint3( reach, -reach, reach );
					flags[vertex++] = false;
				}
				if ( edges & 0x100000 ) {
					vertices[vertex] = centre;
					flags[vertex++] = true;
					vertices[vertex] = centre + NiPoint3( -reach, 0.0f, reach );
					flags[vertex++] = false;
				}
				if ( edges & 0x200000 ) {
					vertices[vertex] = centre;
					flags[vertex++] = true;
					vertices[vertex] = centre + NiPoint3( 0.0f, 0.0f, reach );
					flags[vertex++] = false;
				}
				if ( edges & 0x400000 ) {
					vertices[vertex] = centre;
					flags[vertex++] = true;
					vertices[vertex] = centre + NiPoint3( reach, 0.0f, reach );
					flags[vertex++] = false;
				}
				if ( edges & 0x800000 ) {
					vertices[vertex] = centre;
					flags[vertex++] = true;
					vertices[vertex] = centre + NiPoint3( -reach, reach, reach );
					flags[vertex++] = false;
				}
				if ( edges & 0x1000000 ) {
					vertices[vertex] = centre;
					flags[vertex++] = true;
					vertices[vertex] = centre + NiPoint3( 0.0f, reach, reach );
					flags[vertex++] = false;
				}
				if ( edges & 0x2000000 ) {
					vertices[vertex] = centre;
					flags[vertex++] = true;
					vertices[vertex] = centre + NiPoint3( reach, reach, reach );
					flags[vertex++] = false;
				}
				node->AttachChild( new NiLines( count * 2, vertices, NULL, NULL, flags ), false );
			}
		}
	}

	NiMaterialProperty *material = new NiMaterialProperty;
	material->SetEmittance( NiColor( 1.0f, 1.0f, 1.0f ) );
	node->AttachProperty( material );
	NiVertexColorProperty *vertexColors = new NiVertexColorProperty;
	vertexColors->SetSourceMode( NiVertexColorProperty::SOURCE_IGNORE );
	vertexColors->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	node->AttachProperty( vertexColors );
	return node;
}

/*
================
NiBinaryVoxelData::CreateSolidLines

A node with the boxes of every row of solid cells.
================
*/
// FUNCTION: STBC 0x004aa7f0
NiAVObject *NiBinaryVoxelData::CreateSolidLines() const {
	NiNode *node = new NiNode;
	for ( int z = 0; z < sizeZ; z++ ) {
		VoxelSlice *slice = sliceRefs[z].slice;
		if ( slice != NULL ) {
			VoxelSliceSpan *spans = slice->spans;
			unsigned short runs = slice->GetSolidCount();
			for ( int run = 0; run < runs; run++ ) {
				VoxelSliceSpan *span = &spans[run];
				VoxelRow **rows = sliceRefs[z].slice->GetRows();
				for ( unsigned short y = span->start; y < span->end; y++ ) {
					float size = unknown10;
					NiPoint3 origin = unknown14 + NiPoint3( 0.0f, y * unknown10, z * size );
					node->AttachChild( rows[y - span->start + span->firstRow]->CreateLines( origin, size ), false );
					solidRowLines++;
				}
			}
		}
	}

	NiMaterialProperty *material = new NiMaterialProperty;
	material->SetEmittance( NiColor( 0.0f, 0.0f, 0.0f ) );
	material->SetDiffuseColor( NiColor( 0.0f, 0.0f, 0.0f ) );
	node->AttachProperty( material );
	return node;
}

/*
================
NiBinaryVoxelData::CreateSurfaceLines

A node with a line along every run of surface cells.
================
*/
// FUNCTION: STBC 0x004aa9f0
// PARKED: schedule: retail loads the run's start and the node's vtable after the origin's x copy (fld/fstp), this build before it; index spellings are byte-identical
NiAVObject *NiBinaryVoxelData::CreateSurfaceLines() const {
	NiNode *node = new NiNode;
	for ( int z = 0; z < sizeZ; z++ ) {
		VoxelSlice *slice = sliceRefs[z].slice;
		if ( slice != NULL ) {
			unsigned short runs = slice->GetSurfaceCount();
			const VoxelSliceSpan *spans = slice->GetSurfaceSpans();
			for ( int run = 0; run < runs; run++ ) {
				const VoxelSliceSpan *span = &spans[run];
				VoxelSurfaceRow **rows = sliceRefs[z].slice->GetSurfaceRows();
				for ( unsigned short y = span->start; y < span->end; y++ ) {
					float size = unknown10;
					NiPoint3 origin = unknown14 + NiPoint3( 0.0f, y * unknown10, z * size );
					node->AttachChild( rows[span->firstRow - span->start + y]->CreateLines( origin, size ), false );
					surfaceRowLines++;
				}
			}
		}
	}
	return node;
}

/*
================
NiBinaryVoxelData::CreateGridLines

The lines of the grid along each axis, nearly transparent.
================
*/
// FUNCTION: STBC 0x004aabe0
NiAVObject *NiBinaryVoxelData::CreateGridLines() const {
	int linesYZ = ( sizeY + 1 ) * ( sizeZ + 1 );
	int linesXZ = ( sizeX + 1 ) * ( sizeZ + 1 );
	int linesXY = ( sizeX + 1 ) * ( sizeY + 1 );
	int count = linesXY + linesXZ + linesYZ;
	NiPoint3 *vertices = new NiPoint3[count * 2];
	bool *flags = new bool[count * 2];
	NiColorA *colors = new NiColorA[count * 2];
	int vertex = 0;
	int i;
	int j;

	float length = unknown20.x - unknown14.x;
	for ( i = 0; i <= sizeY; i++ ) {
		for ( j = 0; j <= sizeZ; j++ ) {
			vertices[vertex] = unknown14 + NiPoint3( 0.0f, i * unknown10, j * unknown10 );
			vertices[vertex + 1] = vertices[vertex];
			vertices[vertex + 1].x += length;
			vertex += 2;
		}
	}
	length = unknown20.y - unknown14.y;
	for ( i = 0; i <= sizeX; i++ ) {
		for ( j = 0; j <= sizeZ; j++ ) {
			vertices[vertex] = unknown14 + NiPoint3( i * unknown10, 0.0f, j * unknown10 );
			vertices[vertex + 1] = vertices[vertex];
			vertices[vertex + 1].y += length;
			vertex += 2;
		}
	}
	length = unknown20.z - unknown14.z;
	for ( i = 0; i <= sizeX; i++ ) {
		for ( j = 0; j <= sizeY; j++ ) {
			vertices[vertex] = unknown14 + NiPoint3( i * unknown10, j * unknown10, 0.0f );
			vertices[vertex + 1] = vertices[vertex];
			vertices[vertex + 1].z += length;
			vertex += 2;
		}
	}

	for ( i = 0; i < count; i++ ) {
		flags[i * 2] = true;
		flags[i * 2 + 1] = false;
		colors[i * 2] = NiColorA( 1.0f, 1.0f, 1.0f, 0.1f );
		colors[i * 2 + 1] = NiColorA( 1.0f, 1.0f, 1.0f, 0.1f );
	}

	NiLines *lines = new NiLines( count * 2, vertices, colors, NULL, flags );
	NiAlphaProperty *alpha = new NiAlphaProperty;
	alpha->SetAlphaBlending( true );
	lines->AttachProperty( alpha );
	lines->UpdateProperties();
	return lines;
}

/*
================
NiBinaryVoxelData::GetTree
================
*/
// FUNCTION: STBC 0x004aafc0
VoxelBSPTree *NiBinaryVoxelData::GetTree( const VoxelBSPNode *cell ) {
	static VoxelBSPTree tree;
	tree.nodes = nodes;
	tree.planes = planes;
	tree.root = cell - nodes;
	return &tree;
}

/*
===============================================================================

	The block's rows and slices

===============================================================================
*/

/*
================
VoxelRow::GetSpan
================
*/
// FUNCTION: STBC 0x004ab000
bool VoxelRow::GetSpan( unsigned short x, unsigned short &start, unsigned short &end ) const {
	int high = count - 1;
	int low = 0;
	while ( high >= low ) {
		int middle = ( low + high ) >> 1;
		if ( x >= spans[middle].start ) {
			if ( x < spans[middle].end ) {
				start = spans[middle].start;
				end = spans[middle].end;
				return true;
			}
			low = middle + 1;
		} else {
			high = middle - 1;
		}
	}
	return false;
}

/*
================
VoxelRow::IsSolid
================
*/
// FUNCTION: STBC 0x004ab060
bool VoxelRow::IsSolid( unsigned short x ) const {
	int high = count - 1;
	int low = 0;
	while ( high >= low ) {
		int middle = ( low + high ) >> 1;
		if ( x >= spans[middle].start ) {
			if ( x < spans[middle].end ) {
				return true;
			}
			low = middle + 1;
		} else {
			high = middle - 1;
		}
	}
	return false;
}

/*
================
VoxelSurfaceRow::GetSpan

The cell's data is filled in only when cell is not NULL on entry.
================
*/
// FUNCTION: STBC 0x004ab0b0
bool VoxelSurfaceRow::GetSpan( unsigned short x, unsigned short &start, unsigned short &end, const VoxelBSPNode *&cell ) const {
	int high = count - 1;
	int low = 0;
	while ( high >= low ) {
		int middle = ( low + high ) >> 1;
		if ( x >= spans[middle].start ) {
			if ( x < spans[middle].end ) {
				start = spans[middle].start;
				end = spans[middle].end;
				if ( cell != NULL ) {
					cell = &spans[middle].cells[x - spans[middle].start];
				}
				return true;
			}
			low = middle + 1;
		} else {
			high = middle - 1;
		}
	}
	return false;
}

/*
================
VoxelSurfaceRow::GetCell
================
*/
// FUNCTION: STBC 0x004ab130
bool VoxelSurfaceRow::GetCell( unsigned short x, const VoxelBSPNode *&cell ) const {
	int high = count - 1;
	int low = 0;
	while ( high >= low ) {
		int middle = ( low + high ) >> 1;
		if ( x >= spans[middle].start ) {
			if ( x < spans[middle].end ) {
				cell = &spans[middle].cells[x - spans[middle].start];
				return true;
			}
			low = middle + 1;
		} else {
			high = middle - 1;
		}
	}
	return false;
}

/*
================
VoxelSurfaceRow::ToPointers
================
*/
// FUNCTION: STBC 0x004ab190
void VoxelSurfaceRow::ToPointers( VoxelBSPNode *nodes ) {
	for ( int i = 0; i < count; i++ ) {
		spans[i].cells = nodes + (int)spans[i].cells;
	}
}

/*
================
VoxelSurfaceRow::ToOffsets
================
*/
// FUNCTION: STBC 0x004ab1c0
void VoxelSurfaceRow::ToOffsets( VoxelBSPNode *nodes ) {
	for ( int i = 0; i < count; i++ ) {
		spans[i].cells = (VoxelBSPNode *)( spans[i].cells - nodes );
	}
}

/*
================
VoxelRow::CreateLines

The edges of a box around each run of the row. The row starts at origin
and its cells are size wide.
================
*/
// FUNCTION: STBC 0x004ab200
NiAVObject *VoxelRow::CreateLines( const NiPoint3 &origin, float size ) const {
	NiPoint3 *vertices = new NiPoint3[count * SOLID_SPAN_VERTICES];
	bool *flags = new bool[count * SOLID_SPAN_VERTICES];
	int i;
	for ( i = 0; i < count; i++ ) {
		const unsigned short &start = spans[i].start;
		const unsigned short &end = spans[i].end;
		NiPoint3 *corner = &vertices[i * SOLID_SPAN_VERTICES];

		// the four edges along the row
		corner[0] = origin;
		corner[0].x += start * size;
		corner[1] = origin;
		corner[1].x += end * size;
		corner[2] = NiPoint3( origin.x, origin.y + size, origin.z );
		corner[2].x += start * size;
		corner[3] = NiPoint3( origin.x, origin.y + size, origin.z );
		corner[3].x += end * size;
		corner[4] = NiPoint3( origin.x, origin.y, origin.z + size );
		corner[4].x += start * size;
		corner[5] = NiPoint3( origin.x, origin.y, origin.z + size );
		corner[5].x += end * size;
		corner[6] = NiPoint3( origin.x, origin.y + size, origin.z + size );
		corner[6].x += start * size;
		corner[7] = NiPoint3( origin.x, origin.y + size, origin.z + size );
		corner[7].x += end * size;

		// the square at each end
		corner[8] = origin + NiPoint3( start * size, 0.0f, 0.0f );
		corner[9] = origin + NiPoint3( start * size, size, 0.0f );
		corner[10] = origin + NiPoint3( start * size, size, size );
		corner[11] = origin + NiPoint3( start * size, 0.0f, size );
		corner[12] = origin + NiPoint3( end * size, 0.0f, 0.0f );
		corner[13] = origin + NiPoint3( end * size, size, 0.0f );
		corner[14] = origin + NiPoint3( end * size, size, size );
		corner[15] = origin + NiPoint3( end * size, 0.0f, size );
	}
	for ( i = 0; i < count; i++ ) {
		bool *connect = &flags[i * SOLID_SPAN_VERTICES];
		connect[0] = true;
		connect[1] = false;
		connect[2] = true;
		connect[3] = false;
		connect[4] = true;
		connect[5] = false;
		connect[6] = true;
		connect[7] = false;
		connect[8] = true;
		connect[9] = true;
		connect[10] = true;
		connect[11] = false;
		connect[12] = true;
		connect[13] = true;
		connect[14] = true;
		connect[15] = false;
	}
	return new NiLines( count * SOLID_SPAN_VERTICES, vertices, NULL, NULL, flags );
}

/*
================
VoxelSurfaceRow::CreateLines

A line along each run of the row.
================
*/
// FUNCTION: STBC 0x004ab690
NiAVObject *VoxelSurfaceRow::CreateLines( const NiPoint3 &origin, float size ) const {
	NiPoint3 *vertices = new NiPoint3[count * 2];
	bool *flags = new bool[count * 2];
	int i;
	for ( i = 0; i < count; i++ ) {
		vertices[i * 2] = origin;
		vertices[i * 2].x += spans[i].start * size;
		vertices[i * 2 + 1] = origin;
		vertices[i * 2 + 1].x += spans[i].end * size;
	}
	for ( i = 0; i < count; i++ ) {
		flags[i * 2] = true;
		flags[i * 2 + 1] = false;
	}
	return new NiLines( count * 2, vertices, NULL, NULL, flags );
}

/*
================
VoxelSlice::GetSpan
================
*/
// FUNCTION: STBC 0x004ab7c0
bool VoxelSlice::GetSpan( unsigned short x, unsigned short y, unsigned short &start, unsigned short &end ) const {
	int high = GetSolidCount() - 1;
	const VoxelSliceSpan *solidSpans = spans;
	int low = 0;
	while ( high >= low ) {
		int middle = ( low + high ) >> 1;
		const VoxelSliceSpan *span = &solidSpans[middle];
		unsigned short first = span->start;
		if ( y >= first ) {
			if ( y < span->end ) {
				return GetRows()[solidSpans[middle].firstRow - first + y]->GetSpan( x, start, end );
			}
			low = middle + 1;
		} else {
			high = middle - 1;
		}
	}
	return false;
}

/*
================
VoxelSlice::IsSolid
================
*/
// FUNCTION: STBC 0x004ab870
bool VoxelSlice::IsSolid( unsigned short x, unsigned short y ) const {
	int high = GetSolidCount() - 1;
	const VoxelSliceSpan *solidSpans = spans;
	int low = 0;
	while ( high >= low ) {
		int middle = ( low + high ) >> 1;
		const VoxelSliceSpan *span = &solidSpans[middle];
		unsigned short first = span->start;
		if ( y >= first ) {
			if ( y < span->end ) {
				return GetRows()[solidSpans[middle].firstRow - first + y]->IsSolid( x );
			}
			low = middle + 1;
		} else {
			high = middle - 1;
		}
	}
	return false;
}

/*
================
VoxelSlice::GetSurfaceSpan
================
*/
// FUNCTION: STBC 0x004ab910
// PARKED: register allocation: solidCount (priority 105) and first (106) swap esi and edi against retail; the row pointer's address is taken before the pushes as retail
bool VoxelSlice::GetSurfaceSpan( unsigned short x, unsigned short y, unsigned short &start, unsigned short &end, const VoxelBSPNode *&cell ) const {
	int high = GetSurfaceCount() - 1;
	const VoxelSliceSpan *surfaceSpans = GetSurfaceSpans();
	int low = 0;
	while ( high >= low ) {
		int middle = ( low + high ) >> 1;
		const VoxelSliceSpan *span = &surfaceSpans[middle];
		unsigned short first = span->start;
		if ( y >= first ) {
			if ( y < span->end ) {
				VoxelSurfaceRow *const &row = GetSurfaceRows()[surfaceSpans[middle].firstRow - first + y];
				return row->GetSpan( x, start, end, cell );
			}
			low = middle + 1;
		} else {
			high = middle - 1;
		}
	}
	return false;
}

/*
================
VoxelSlice::GetSurfaceCell
================
*/
// FUNCTION: STBC 0x004aba20
// PARKED: register allocation as GetSurfaceSpan: solidCount and first swap esi and edi against retail
bool VoxelSlice::GetSurfaceCell( unsigned short x, unsigned short y, const VoxelBSPNode *&cell ) const {
	int high = GetSurfaceCount() - 1;
	const VoxelSliceSpan *surfaceSpans = GetSurfaceSpans();
	int low = 0;
	while ( high >= low ) {
		int middle = ( low + high ) >> 1;
		const VoxelSliceSpan *span = &surfaceSpans[middle];
		unsigned short first = span->start;
		if ( y >= first ) {
			if ( y < span->end ) {
				VoxelSurfaceRow *const &row = GetSurfaceRows()[surfaceSpans[middle].firstRow - first + y];
				return row->GetCell( x, cell );
			}
			low = middle + 1;
		} else {
			high = middle - 1;
		}
	}
	return false;
}

/*
================
VoxelSlice::ToPointers
================
*/
// FUNCTION: STBC 0x004abb20
// PARKED: retail's GetNumBoundaryEntries keeps &spans[solidCount] as its own base ([base + index*12]), this build folds solidCount + index; the first loop's registers swap (solid count ecx in retail)
VoxelSlice *VoxelSlice::ToPointers( char *rows, char *surfaceRows, VoxelBSPNode *nodes ) {
	VoxelRow **solid = GetRows();
	unsigned short count = GetRowCount();
	for ( int i = 0; i < count; i++ ) {
		solid[i] = (VoxelRow *)( rows + (int)solid[i] );
	}
	VoxelSurfaceRow **surface = GetSurfaceRows();
	count = GetNumBoundaryEntries();
	for ( i = 0; i < count; i++ ) {
		surface[i] = (VoxelSurfaceRow *)( surfaceRows + (int)surface[i] );
		surface[i]->ToPointers( nodes );
	}
	return (VoxelSlice *)( GetSurfaceRows() + GetNumBoundaryEntries() );
}

/*
================
VoxelSlice::ToOffsets
================
*/
// FUNCTION: STBC 0x004abcb0
// PARKED: as ToPointers: GetNumBoundaryEntries folds solidCount + index where retail keeps the surface runs' base apart
VoxelSlice *VoxelSlice::ToOffsets( char *rows, char *surfaceRows, VoxelBSPNode *nodes ) {
	VoxelRow **solid = GetRows();
	unsigned short count = GetRowCount();
	for ( int i = 0; i < count; i++ ) {
		solid[i] = (VoxelRow *)( (char *)solid[i] - rows );
	}
	VoxelSurfaceRow **surface = GetSurfaceRows();
	count = GetNumBoundaryEntries();
	for ( i = 0; i < count; i++ ) {
		surface[i]->ToOffsets( nodes );
		surface[i] = (VoxelSurfaceRow *)( (char *)surface[i] - surfaceRows );
	}
	return (VoxelSlice *)( GetSurfaceRows() + GetNumBoundaryEntries() );
}

/*
================
VoxelSliceRef::ToPointer
================
*/
// FUNCTION: STBC 0x004abe40
void VoxelSliceRef::ToPointer( char *slices ) {
	if ( (int)slice == -1 ) {
		slice = NULL;
	} else {
		slice = (VoxelSlice *)( slices + (int)slice );
	}
}

/*
================
VoxelSliceRef::ToOffset
================
*/
// FUNCTION: STBC 0x004abe80
void VoxelSliceRef::ToOffset( char *slices ) {
	if ( slice == NULL ) {
		slice = (VoxelSlice *)-1;
	} else {
		slice = (VoxelSlice *)( (char *)slice - slices );
	}
}

/*
===============================================================================

	NiBinaryVoxelExtraData

===============================================================================
*/

// DATA: STBC 0x983910 ?rtti@NiBinaryVoxelExtraData@@2VNiRTTI@@B
const NiRTTI NiBinaryVoxelExtraData::rtti( "NiBinaryVoxelExtraData", &NiExtraData::rtti );

/*
================
NiBinaryVoxelExtraData::CreateClone
================
*/
// FUNCTION: STBC 0x004abf30
NiObject *NiBinaryVoxelExtraData::CreateClone() {
	NiBinaryVoxelExtraData *clone = new NiBinaryVoxelExtraData;
	CopyMembers( clone );
	return clone;
}

/*
================
NiBinaryVoxelExtraData::CopyMembers

The clone shares the voxel data.
================
*/
// FUNCTION: STBC 0x004abfb0
void NiBinaryVoxelExtraData::CopyMembers( NiBinaryVoxelExtraData *dest ) {
	NiExtraData::CopyMembers( dest );
	dest->voxelData = voxelData;
}

/*
================
NiBinaryVoxelExtraData::Register
================
*/
// FUNCTION: STBC 0x004ac000 ?Register@NiBinaryVoxelExtraData@@SAHXZ
NiImplementRegister( NiBinaryVoxelExtraData )

/*
================
NiBinaryVoxelExtraData::CreateObject

Creates the block and loads it.
================
*/
// FUNCTION: STBC 0x004ac150
void NiBinaryVoxelExtraData::CreateObject( NiStream &stream, NiObject *&object ) {
	NiBinaryVoxelExtraData *data = new NiBinaryVoxelExtraData;
	stream.SetLoadRecord( new NiBinaryVoxelExtraDataLoadRecord );
	data->LoadBinary( stream );
	object = data;
}

/*
================
NiBinaryVoxelExtraData::LoadBinary
================
*/
// FUNCTION: STBC 0x004ac200
void NiBinaryVoxelExtraData::LoadBinary( NiStream &stream ) {
	NiExtraData::LoadBinary( stream );
	NiBinaryVoxelExtraDataLoadRecord *record = (NiBinaryVoxelExtraDataLoadRecord *)stream.GetLoadRecord();
	NiStreamLoadBinary( stream, record->voxelDataLinkID );
}

/*
================
VoxelPolygon::VoxelPolygon
================
*/
// FUNCTION: STBC 0x004ac220
VoxelPolygon::VoxelPolygon( int capacity_ ) {
	count = 0;
	capacity = capacity_;
	vertices = new NiPoint3[capacity_];
}

/*
================
NiBinaryVoxelExtraData::LinkObject
================
*/
// FUNCTION: STBC 0x004ac260
void NiBinaryVoxelExtraData::LinkObject( NiStream &stream ) {
	NiExtraData::LinkObject( stream );
	NiBinaryVoxelExtraDataLoadRecord *record = (NiBinaryVoxelExtraDataLoadRecord *)stream.GetLoadRecord();
	if ( record->voxelDataLinkID ) {
		voxelData = (NiBinaryVoxelData *)stream.GetObjectFromLinkID( record->voxelDataLinkID );
	}
}

/*
================
NiBinaryVoxelExtraData::RegisterStreamables
================
*/
// FUNCTION: STBC 0x004ac2b0
bool NiBinaryVoxelExtraData::RegisterStreamables( NiStream &stream ) {
	if ( !NiExtraData::RegisterStreamables( stream ) ) {
		return false;
	}
	if ( voxelData && !voxelData->RegisterStreamables( stream ) ) {
		return false;
	}
	return true;
}

/*
================
NiBinaryVoxelExtraData::SaveBinary
================
*/
// FUNCTION: STBC 0x004ac2f0
void NiBinaryVoxelExtraData::SaveBinary( NiStream &stream ) {
	NiExtraData::SaveBinary( stream );
	stream.SaveLinkID( voxelData );
}

/*
================
NiBinaryVoxelExtraData::IsEqual

Equal when both blocks share the same voxel data.
================
*/
// FUNCTION: STBC 0x004ac320
bool NiBinaryVoxelExtraData::IsEqual( NiObject *object ) {
	if ( !object ) {
		return false;
	}
	return ( (NiBinaryVoxelExtraData *)object )->voxelData == voxelData;
}
