/*
===============================================================================

	Voxelizer.cpp

===============================================================================
*/

#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "Voxelizer.h"
#include "NiMath.h"
#include "NiNode.h"
#include "NiPlane.h"
#include "NiTriShape.h"
#include "NiTriShapeData.h"
#include "NiBinaryVoxelExtraData.h"
#include "OBBNode.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004b0fa0 _$E427
// FUNCTION: STBC 0x004b0fc0 _$E426

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004b08c0 _$E32
// FUNCTION: STBC 0x004b08d0 _$E29
// FUNCTION: STBC 0x004b08e0 _$E31
// FUNCTION: STBC 0x004b08f0 _$E30
// FUNCTION: STBC 0x004b0900 _$E38
// FUNCTION: STBC 0x004b0910 _$E35
// FUNCTION: STBC 0x004b0920 _$E37
// FUNCTION: STBC 0x004b0930 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004b0e10 _$E45
// FUNCTION: STBC 0x004b0e20 _$E44
// FUNCTION: STBC 0x004b0e30 _$E67
// FUNCTION: STBC 0x004b0e40 _$E66
// FUNCTION: STBC 0x004b0e50 _$E71
// FUNCTION: STBC 0x004b0e60 _$E70
// FUNCTION: STBC 0x004b0e70 _$E75
// FUNCTION: STBC 0x004b0e80 _$E74
// FUNCTION: STBC 0x004b0e90 _$E79
// FUNCTION: STBC 0x004b0ea0 _$E78
// FUNCTION: STBC 0x004b0eb0 _$E198
// FUNCTION: STBC 0x004b0ec0 _$E197
// FUNCTION: STBC 0x004b0ed0 _$E203
// FUNCTION: STBC 0x004b0ee0 _$E202
// FUNCTION: STBC 0x004b0ef0 _$E207
// FUNCTION: STBC 0x004b0f00 _$E206
// FUNCTION: STBC 0x004b0f30 _$E215
// FUNCTION: STBC 0x004b0f40 _$E214
// FUNCTION: STBC 0x004b0fd0 _$E222
// FUNCTION: STBC 0x004b0fe0 _$E221
// FUNCTION: STBC 0x004b0ff0 _$E226
// FUNCTION: STBC 0x004b1000 _$E225
// FUNCTION: STBC 0x004b1010 _$E230
// FUNCTION: STBC 0x004b1020 _$E229
// FUNCTION: STBC 0x004b1030 _$E234
// FUNCTION: STBC 0x004b1040 _$E233
// FUNCTION: STBC 0x004b1050 _$E240
// FUNCTION: STBC 0x004b1060 _$E239
// FUNCTION: STBC 0x004b1070 _$E54
// FUNCTION: STBC 0x004b1080 _$E53
// FUNCTION: STBC 0x004b1090 _$E244
// FUNCTION: STBC 0x004b10a0 _$E243
// FUNCTION: STBC 0x004b1280 _$E343
// FUNCTION: STBC 0x004b1290 _$E342
// FUNCTION: STBC 0x004b12a0 _$E347
// FUNCTION: STBC 0x004b12b0 _$E346

char *BinaryVoxelizer::fileName;
FILE *BinaryVoxelizer::debugFile;

/*
================
BinaryVoxelizer::GetDebugFileName

The model file's name with its extension replaced by
"_VoxelizerDebug.txt", or "VoxelizerDebug.txt" when there is no model
file.
================
*/
// FUNCTION: STBC 0x004ac830
char *BinaryVoxelizer::GetDebugFileName() {
	static char name[_MAX_PATH];
	if ( fileName != NULL ) {
		strcpy( name, fileName );
		*strrchr( name, '.' ) = '\0';
		strcat( name, "_VoxelizerDebug.txt" );
		return name;
	}
	return "VoxelizerDebug.txt";
}

/*
================
BinaryVoxelizer::GetDebugFile
================
*/
// FUNCTION: STBC 0x004ac8b0
FILE *BinaryVoxelizer::GetDebugFile() {
	if ( debugFile == NULL ) {
		debugFile = fopen( GetDebugFileName(), "w+" );
	}
	return debugFile;
}

/*
================
BinaryVoxelizer::CloseDebugFile

Closes the log and forgets the model file.
================
*/
// FUNCTION: STBC 0x004ac8e0
void BinaryVoxelizer::CloseDebugFile() {
	if ( debugFile != NULL ) {
		fclose( debugFile );
		debugFile = NULL;
	}
	fileName = NULL;
}

/*
================
BinaryVoxelizer::SetFileName
================
*/
// FUNCTION: STBC 0x004ac910
void BinaryVoxelizer::SetFileName( char *name ) {
	fileName = name;
}

/*
===============================================================================

	Surface cell trees

===============================================================================
*/

// a corner this close to a splitting plane lies on it
static const float SPLIT_EPSILON = 0.002f;

/*
================
BinaryVoxelizer::VoxelBspNode::Split

Splits a polygon by the node's plane into the corners in front of it and
the corners behind it, adding the points where its edges cross the plane
to both sides. A corner on the plane goes to the side of the corner
before it.
================
*/
// FUNCTION: STBC 0x004ac920
// PARKED: control flow: the call set is retail's (the single-statement bodies unbraced: own cost -12 leaves the first crossing's NiPoint3 constructor out of line); the corner in front is tested first as retail's branch sense shows (jne on distance < 0); left the crossing's x87 product order and which single copies join the shared tail
void BinaryVoxelizer::VoxelBspNode::Split( NiPoint3 *corners, unsigned short count, NiPoint3 *front, unsigned short &frontCount,
							  NiPoint3 *back, unsigned short &backCount, NiPlane *planes ) {
	frontCount = 0;
	backCount = 0;
	NiPlane &splitter = planes[plane];
	NiPoint3 origin = splitter.GetConstant() * splitter.GetNormal();

	unsigned short previous = count - 1;
	for ( unsigned short i = 0; i < count; i++ ) {
		float distance = splitter.Distance( corners[i] );
		float previousDistance = splitter.Distance( corners[previous] );
		bool on = (float)fabs( distance ) < SPLIT_EPSILON;
		bool previousOn = (float)fabs( previousDistance ) < SPLIT_EPSILON;
		if ( !on && !previousOn ) {
			if ( distance >= 0.0f ) {
				if ( previousDistance < 0.0f ) {
					// the edge crosses from the back to the front
					NiPoint3 direction = corners[i] - corners[previous];
					float t = splitter.GetNormal().Dot( corners[previous] - origin ) / -splitter.GetNormal().Dot( direction );
					NiPoint3 crossing = direction * t + corners[previous];
					front[frontCount++] = crossing;
					front[frontCount++] = corners[i];
					back[backCount++] = crossing;
				} else
					front[frontCount++] = corners[i];
			} else if ( previousDistance < 0.0f )
				back[backCount++] = corners[i];
			else {
				// the edge crosses from the front to the back
				NiPoint3 direction = corners[i] - corners[previous];
				float t = splitter.GetNormal().Dot( corners[previous] - origin ) / -splitter.GetNormal().Dot( direction );
				NiPoint3 crossing = direction * t + corners[previous];
				front[frontCount++] = crossing;
				back[backCount++] = crossing;
				back[backCount++] = corners[i];
			}
		} else if ( on && !previousOn ) {
			// a corner on the plane goes to the side of the corner before it
			if ( previousDistance > 0.0f )
				front[frontCount++] = corners[i];
			else
				back[backCount++] = corners[i];
		} else if ( !on && previousOn ) {
			if ( distance > 0.0f )
				front[frontCount++] = corners[i];
			else
				back[backCount++] = corners[i];
		}
		previous = i;
	}
}

/*
================
BinaryVoxelizer::VoxelBspNode::Write
================
*/
// FUNCTION: STBC 0x004acd50
void BinaryVoxelizer::VoxelBspNode::Write( VoxelBSPNode *node, NiPlane *planes_ ) {
	node->front = frontIndex;
	node->back = backIndex;
	node->plane = plane;
}

/*
================
BinaryVoxelizer::VoxelBspNode::Number
================
*/
// FUNCTION: STBC 0x004acd70
void BinaryVoxelizer::VoxelBspNode::Number( VoxelBSPNode *nodes, int &next, NiPlane *planes_ ) {
	if ( front != NULL ) {
		front->Number( nodes, next, planes_ );
		front->Write( &nodes[next], planes_ );
		frontIndex = next;
		next++;
	} else {
		frontIndex = 0;
	}
	if ( back != NULL ) {
		back->Number( nodes, next, planes_ );
		back->Write( &nodes[next], planes_ );
		backIndex = next;
		next++;
	} else {
		backIndex = 0;
	}
}

/*
================
BinaryVoxelizer::VoxelBspNode::CountNodes
================
*/
// FUNCTION: STBC 0x004ace00
int BinaryVoxelizer::VoxelBspNode::CountNodes() {
	int nodes = 0;
	if ( front != NULL ) {
		nodes = front->CountNodes();
	}
	if ( back != NULL ) {
		nodes += back->CountNodes();
	}
	return nodes + 1;
}

/*
================
BinaryVoxelizer::VoxelBspNode::IsInside

A point on a plane counts as behind it. Where the plane has subtrees on
both sides, a point on it is tried down both.
================
*/
// FUNCTION: STBC 0x004ace30
bool BinaryVoxelizer::VoxelBspNode::IsInside( const NiPoint3 &point, NiPlane *planes ) {
	bool inside = false;
	float distance = planes[plane].GetNormal().Dot( point ) - planes[plane].GetConstant();
	if ( back != NULL && front != NULL ) {
		if ( distance >= 0.0f ) {
			inside = front->IsInside( point, planes );
		}
		if ( !inside && distance <= 0.0f ) {
			inside = back->IsInside( point, planes );
		}
	} else if ( front != NULL ) {
		if ( distance <= 0.0f ) {
			inside = true;
		} else {
			inside = front->IsInside( point, planes );
		}
	} else if ( distance <= 0.0f ) {
		if ( back != NULL ) {
			inside = back->IsInside( point, planes );
		} else {
			inside = true;
		}
	}
	return inside;
}

/*
================
BinaryVoxelizer::VoxelBspNode::IsSegmentInside

IsInside for a segment: a segment that crosses a plane with subtrees on
both sides is split where it crosses, and each half is tried on its side.
================
*/
// FUNCTION: STBC 0x004acf00
// DRAFT: control flow and calls from the asm; retail keeps 10 recursive calls (ours merges two pairs of tail calls), reuses the start and end argument slots for the distances, and builds the second crossing through NiPoint3's out-of-line constructor (0x414540)
bool BinaryVoxelizer::VoxelBspNode::IsSegmentInside( const NiPoint3 &start, const NiPoint3 &end, NiPlane *planes ) {
	bool inside = false;
	NiPlane &splitter = planes[plane];
	float startDistance = splitter.GetNormal().Dot( start ) - splitter.GetConstant();
	float endDistance = splitter.GetNormal().Dot( end ) - splitter.GetConstant();
	if ( back != NULL && front != NULL ) {
		if ( startDistance >= 0.0f && endDistance >= 0.0f ) {
			inside = front->IsSegmentInside( start, end, planes );
		}
		if ( !inside ) {
			if ( startDistance <= 0.0f && endDistance <= 0.0f ) {
				inside = back->IsSegmentInside( start, end, planes );
			}
			if ( !inside && ( startDistance <= 0.0f ) != ( endDistance <= 0.0f ) ) {
				NiPoint3 origin = splitter.GetConstant() * splitter.GetNormal();
				NiPoint3 direction = end - start;
				float t = splitter.GetNormal().Dot( start - origin ) / -splitter.GetNormal().Dot( direction );
				NiPoint3 crossing = direction * t + start;
				if ( startDistance < 0.0f ) {
					inside = front->IsSegmentInside( end, crossing, planes );
					if ( !inside ) {
						inside = back->IsSegmentInside( crossing, start, planes );
					}
				} else {
					inside = front->IsSegmentInside( start, crossing, planes );
					if ( !inside ) {
						inside = back->IsSegmentInside( crossing, end, planes );
					}
				}
			}
		}
	} else if ( front != NULL ) {
		if ( startDistance <= 0.0f || endDistance <= 0.0f ) {
			inside = true;
		} else {
			inside = front->IsSegmentInside( start, end, planes );
		}
	} else if ( startDistance <= 0.0f || endDistance <= 0.0f ) {
		if ( back == NULL ) {
			inside = true;
		} else if ( startDistance <= 0.0f && endDistance <= 0.0f ) {
			inside = back->IsSegmentInside( start, end, planes );
		} else {
			NiPoint3 origin = splitter.GetConstant() * splitter.GetNormal();
			NiPoint3 direction = end - start;
			float t = splitter.GetNormal().Dot( start - origin ) / -splitter.GetNormal().Dot( direction );
			NiPoint3 crossing = direction * t + start;
			if ( startDistance <= 0.0f ) {
				inside = back->IsSegmentInside( start, crossing, planes );
			} else {
				inside = back->IsSegmentInside( end, crossing, planes );
			}
		}
	}
	return inside;
}

/*
===============================================================================

	InsideData

===============================================================================
*/

/*
================
BinaryVoxelizer::InsideData::InsideData
================
*/
// FUNCTION: STBC 0x004ad320
BinaryVoxelizer::InsideData::InsideData() {
	count = 0;
	head = new InsideData::InsideChunk;
	tail = head;
	written = false;
	refs = 0;
	offset = -1;
}

/*
================
BinaryVoxelizer::InsideData::~InsideData
================
*/
// FUNCTION: STBC 0x004ad390
BinaryVoxelizer::InsideData::~InsideData() {
	delete head;
}

/*
================
BinaryVoxelizer::InsideData::AddRef
================
*/
// FUNCTION: STBC 0x004ad3c0
void BinaryVoxelizer::InsideData::AddRef() {
	refs++;
}

/*
================
BinaryVoxelizer::InsideData::Release
================
*/
// FUNCTION: STBC 0x004ad3d0
void BinaryVoxelizer::InsideData::Release() {
	if ( --refs < 1 ) {
		delete this;
	}
}

/*
================
BinaryVoxelizer::InsideData::operator==

Whether the rows hold the same runs.
================
*/
// FUNCTION: STBC 0x004ad3f0 ??8InsideData@BinaryVoxelizer@@QAE_NABV01@@Z
// PARKED: register: with the unit's rest local the doubling is lea edx, [eax + eax] where retail doubles in place (add eax, eax), and the tail loop walks a copy of the block pointer where retail walks it in place
bool BinaryVoxelizer::InsideData::operator==( const InsideData &row ) {
	int spanCount = count;
	if ( spanCount != row.count ) {
		return false;
	}
	InsideData::InsideChunk *block = head;
	InsideData::InsideChunk *other = row.head;
	int i;
	int j;
	for ( i = 0; i < spanCount / 8; i++ ) {
		for ( j = 0; j < 16; j++ ) {
			if ( block->values[j] != other->values[j] ) {
				return false;
			}
		}
		block = block->next;
		other = other->next;
	}
	int rest = spanCount % 8;
	for ( j = 0; j < rest * 2; j++ ) {
		if ( block->values[j] != other->values[j] ) {
			return false;
		}
	}
	return true;
}

/*
================
BinaryVoxelizer::InsideData::Add
================
*/
// FUNCTION: STBC 0x004ad4a0
void BinaryVoxelizer::InsideData::Add( unsigned short start, unsigned short end ) {
	tail->values[count % 8 * 2] = start;
	tail->values[count % 8 * 2 + 1] = end;
	count++;
	if ( count % 8 <= 0 ) {
		tail->next = new InsideData::InsideChunk;
		tail = tail->next;
	}
}

/*
================
BinaryVoxelizer::InsideData::GetSize
================
*/
// FUNCTION: STBC 0x004ad540 ?GetSize@InsideData@BinaryVoxelizer@@QBEIH@Z
unsigned int BinaryVoxelizer::InsideData::GetSize( int spanCount ) const {
	return spanCount * sizeof( VoxelSpan ) + sizeof( int );
}

/*
================
BinaryVoxelizer::InsideData::Write
================
*/
// FUNCTION: STBC 0x004ad550
// DamageTool.exe 0x406ED0 (InsideData::Collect)
void BinaryVoxelizer::InsideData::Write( char *block, int &offset_ ) {
	VoxelRow *row = (VoxelRow *)( block + offset_ );
	InsideData::InsideChunk *chunk = head;
	int n = 0;
	int i;
	int j;
	int blocks = count / 8;
	for ( i = 0; i < blocks; i++ ) {
		for ( j = 0; j < 8; j++ ) {
			row->spans[n].start = chunk->values[j * 2];
			row->spans[n].end = chunk->values[j * 2 + 1];
			n++;
		}
		chunk = chunk->next;
	}
	int rest = count % 8;
	for ( j = 0; j < rest; j++ ) {
		row->spans[n].start = chunk->values[j * 2];
		row->spans[n].end = chunk->values[j * 2 + 1];
		n++;
	}
	row->count = count;
	written = true;
	offset = offset_;
	offset_ += row->count * sizeof( VoxelSpan ) + sizeof( int );
}

/*
================
BinaryVoxelizer::BoundaryData::GetNodeSize
================
*/
// FUNCTION: STBC 0x004ad630
int BinaryVoxelizer::BoundaryData::GetNodeSize() {
	VoxelizerCellBlock *block = cellHead;
	int size = 0;
	int blocks = count / 8;
	int i;
	int j;
	for ( i = 0; i < blocks; i++ ) {
		for ( j = 0; j < 8; j++ ) {
			size += block->lists[j]->GetNodeSize();
		}
		block = block->next;
	}
	int rest = count % 8;
	for ( j = 0; j < rest; j++ ) {
		size += block->lists[j]->GetNodeSize();
	}
	return size;
}

/*
================
BinaryVoxelizer::BoundaryData::Add
================
*/
// FUNCTION: STBC 0x004ad6d0
void BinaryVoxelizer::BoundaryData::Add( unsigned short start, unsigned short end, BoundaryBspsData *cells ) {
	InsideData::Add( start, end );
	int index = count - 1;
	cellTail->lists[index % 8] = cells;
	if ( ( index + 1 ) % 8 == 0 ) {
		cellTail->next = new VoxelizerCellBlock;
		cellTail = cellTail->next;
	}
}

/*
================
BinaryVoxelizer::BoundaryData::WriteNodes
================
*/
// FUNCTION: STBC 0x004ad760
void BinaryVoxelizer::BoundaryData::WriteNodes( VoxelBSPNode *nodes, int &next, NiPlane *planes_ ) {
	VoxelizerCellBlock *block = cellHead;
	int blocks = count / 8;
	int i;
	int j;
	for ( i = 0; i < blocks; i++ ) {
		for ( j = 0; j < 8; j++ ) {
			int first = next;
			int nextChild = next + block->lists[j]->count;
			// the count is not used
			block->lists[j]->CountNodes();
			block->lists[j]->Write( nodes, first, nextChild, planes_ );
			next = nextChild;
		}
		block = block->next;
	}
	int rest = count % 8;
	for ( j = 0; j < rest; j++ ) {
		int first = next;
		int nextChild = next + block->lists[j]->count;
		block->lists[j]->CountNodes();
		block->lists[j]->Write( nodes, first, nextChild, planes_ );
		next = nextChild;
	}
}

/*
================
BinaryVoxelizer::BoundaryData::Write
================
*/
// FUNCTION: STBC 0x004ad850
// DamageTool.exe 0x4075F0 (BoundaryData::Collect)
void BinaryVoxelizer::BoundaryData::Write( char *block, int &offset_ ) {
	VoxelizerCellBlock *cellBlock = cellHead;
	VoxelSurfaceRow *row = (VoxelSurfaceRow *)( block + offset_ );
	InsideData::InsideChunk *chunk = head;
	int n = 0;
	int i;
	int j;
	int blocks = count / 8;
	for ( i = 0; i < blocks; i++ ) {
		for ( j = 0; j < 8; j++ ) {
			row->spans[n].start = chunk->values[j * 2];
			row->spans[n].end = chunk->values[j * 2 + 1];
			row->spans[n].cells = (VoxelBSPNode *)cellBlock->lists[j]->offset;
			n++;
		}
		cellBlock = cellBlock->next;
		chunk = chunk->next;
	}
	int rest = count % 8;
	for ( j = 0; j < rest; j++ ) {
		row->spans[n].start = chunk->values[j * 2];
		row->spans[n].end = chunk->values[j * 2 + 1];
		row->spans[n].cells = (VoxelBSPNode *)cellBlock->lists[j]->offset;
		n++;
	}
	row->count = count;
	written = true;
	offset = offset_;
	offset_ += row->count * sizeof( VoxelSurfaceSpan ) + sizeof( int );
}

/*
================
BinaryVoxelizer::BoundaryBspsData::BspChunk::BspChunk
================
*/
// FUNCTION: STBC 0x004ad960
BinaryVoxelizer::BoundaryBspsData::BspChunk::BspChunk() {
	memset( trees, 0, sizeof( trees ) );
	next = NULL;
}

/*
================
BinaryVoxelizer::BoundaryBspsData::BspChunk::~BspChunk
================
*/
// FUNCTION: STBC 0x004ad980
BinaryVoxelizer::BoundaryBspsData::BspChunk::~BspChunk() {
	for ( int i = 0; i < 8; i++ ) {
		delete trees[i];
	}
	delete next;
}

/*
================
BinaryVoxelizer::BoundaryBspsData::BoundaryBspsData
================
*/
// FUNCTION: STBC 0x004ada10
BinaryVoxelizer::BoundaryBspsData::BoundaryBspsData() {
	offset = -1;
	count = 0;
	head = new BoundaryBspsData::BspChunk;
	tail = head;
}

/*
================
BinaryVoxelizer::BoundaryBspsData::Add
================
*/
// FUNCTION: STBC 0x004ada50
void BinaryVoxelizer::BoundaryBspsData::Add( VoxelBspRoot *tree ) {
	tail->trees[count % 8] = tree;
	count++;
	if ( count % 8 == 0 ) {
		tail->next = new BoundaryBspsData::BspChunk;
		tail = tail->next;
	}
}

/*
================
BinaryVoxelizer::BoundaryBspsData::CountNodes
================
*/
// FUNCTION: STBC 0x004adac0
int BinaryVoxelizer::BoundaryBspsData::CountNodes() {
	BoundaryBspsData::BspChunk *block = head;
	int nodes = 0;
	int blocks = count / 8;
	int i;
	int j;
	for ( i = 0; i < blocks; i++ ) {
		for ( j = 0; j < 8; j++ ) {
			nodes += block->trees[j]->CountNodes() - 1;
		}
		block = block->next;
	}
	int rest = count % 8;
	for ( j = 0; j < rest; j++ ) {
		nodes += block->trees[j]->CountNodes() - 1;
	}
	return nodes;
}

/*
================
BinaryVoxelizer::BoundaryBspsData::Write
================
*/
// FUNCTION: STBC 0x004adb40
void BinaryVoxelizer::BoundaryBspsData::Write( VoxelBSPNode *nodes, int &next, int &nextChild, NiPlane *planes_ ) {
	offset = next;
	BoundaryBspsData::BspChunk *block = head;
	int blocks = count / 8;
	int i;
	int j;
	for ( i = 0; i < blocks; i++ ) {
		for ( j = 0; j < 8; j++ ) {
			block->trees[j]->Number( nodes, nextChild, planes_ );
			block->trees[j]->Write( &nodes[next], planes_ );
			next++;
		}
		block = block->next;
	}
	int rest = count % 8;
	for ( j = 0; j < rest; j++ ) {
		block->trees[j]->Number( nodes, nextChild, planes_ );
		block->trees[j]->Write( &nodes[next], planes_ );
		next++;
	}
}

/*
===============================================================================

	BoundaryData

===============================================================================
*/

/*
===============================================================================

	SliceData

===============================================================================
*/

/*
================
BinaryVoxelizer::SliceData::SliceChunk::SliceChunk
================
*/
// FUNCTION: STBC 0x004adc30
BinaryVoxelizer::SliceData::SliceChunk::SliceChunk() {
	for ( int i = 0; i < 32; i++ ) {
		rows[i] = NULL;
	}
	next = NULL;
	ys[0] = 0xFFFF;
}

/*
================
BinaryVoxelizer::SliceData::SliceChunk::~SliceChunk
================
*/
// FUNCTION: STBC 0x004adc50
BinaryVoxelizer::SliceData::SliceChunk::~SliceChunk() {
	for ( int i = 0; i < 32; i++ ) {
		if ( rows[i] != NULL ) {
			rows[i]->Release();
		}
	}
	delete next;
}

/*
================
BinaryVoxelizer::SliceData::SliceData
================
*/
// FUNCTION: STBC 0x004adc90
BinaryVoxelizer::SliceData::SliceData() {
	written = false;
	refs = 0;
	offset = -1;
	solidCount = 0;
	solidTail = solidHead = new SliceData::SliceChunk;
	surfaceCount = 0;
	surfaceTail = surfaceHead = new SliceData::SliceChunk;
}

/*
================
BinaryVoxelizer::SliceData::~SliceData
================
*/
// FUNCTION: STBC 0x004add10
BinaryVoxelizer::SliceData::~SliceData() {
	delete solidHead;
	delete surfaceHead;
}

/*
================
BinaryVoxelizer::SliceData::AddRef
================
*/
// FUNCTION: STBC 0x004add50
void BinaryVoxelizer::SliceData::AddRef() {
	refs++;
}

/*
================
BinaryVoxelizer::SliceData::Release
================
*/
// FUNCTION: STBC 0x004add60
void BinaryVoxelizer::SliceData::Release() {
	if ( --refs < 1 ) {
		delete this;
	}
}

/*
================
BinaryVoxelizer::SliceData::AddSolidRow
================
*/
// FUNCTION: STBC 0x004add90
void BinaryVoxelizer::SliceData::AddSolidRow( unsigned short y, InsideData *row ) {
	row->AddRef();
	SliceData::SliceChunk *block = solidTail;
	block->ys[solidCount % 32] = y;
	block->rows[solidCount % 32] = row;
	solidCount++;
	if ( solidCount % 32 <= 0 ) {
		solidTail = block->next = new SliceData::SliceChunk;
	}
}

/*
================
BinaryVoxelizer::SliceData::AddSurfaceRow
================
*/
// FUNCTION: STBC 0x004ade20
void BinaryVoxelizer::SliceData::AddSurfaceRow( unsigned short y, InsideData *row ) {
	row->AddRef();
	SliceData::SliceChunk *block = surfaceTail;
	block->ys[surfaceCount % 32] = y;
	block->rows[surfaceCount % 32] = row;
	surfaceCount++;
	if ( surfaceCount % 32 <= 0 ) {
		surfaceTail = block->next = new SliceData::SliceChunk;
	}
}

/*
================
BinaryVoxelizer::SliceData::GetSolidRowOffsets
================
*/
// FUNCTION: STBC 0x004adec0
int BinaryVoxelizer::SliceData::GetSolidRowOffsets( int *offsets ) const {
	const SliceData::SliceChunk *block = solidHead;
	int count = 0;
	int blocks = solidCount / 32;
	int i;
	int j;
	for ( i = 0; i < blocks; i++ ) {
		for ( j = 0; j < 32; j++ ) {
			offsets[count++] = block->rows[j]->offset;
		}
		block = block->next;
	}
	int remaining = solidCount % 32;
	for ( j = 0; j < remaining; j++ ) {
		offsets[count++] = block->rows[j]->offset;
	}
	return count;
}

/*
================
BinaryVoxelizer::SliceData::GetSurfaceRowOffsets
================
*/
// FUNCTION: STBC 0x004adf60
int BinaryVoxelizer::SliceData::GetSurfaceRowOffsets( int *offsets ) const {
	const SliceData::SliceChunk *block = surfaceHead;
	int count = 0;
	int blocks = surfaceCount / 32;
	int i;
	int j;
	for ( i = 0; i < blocks; i++ ) {
		for ( j = 0; j < 32; j++ ) {
			offsets[count++] = block->rows[j]->offset;
		}
		block = block->next;
	}
	int remaining = surfaceCount % 32;
	for ( j = 0; j < remaining; j++ ) {
		offsets[count++] = block->rows[j]->offset;
	}
	return count;
}

/*
================
BinaryVoxelizer::SliceData::CountRuns
================
*/
// FUNCTION: STBC 0x004ae000
int BinaryVoxelizer::SliceData::CountRuns( const SliceData::SliceChunk *block, int count ) const {
	int runs = 0;
	int last = block->ys[0] - 2;
	int i;
	int j;
	for ( i = 0; i < count / 32; i++ ) {
		for ( j = 0; j < 32; j++ ) {
			if ( last != block->ys[j] - 1 ) {
				runs++;
			}
			last = block->ys[j];
		}
		block = block->next;
	}
	for ( j = 0; j < count % 32; j++ ) {
		if ( last != block->ys[j] - 1 ) {
			runs++;
		}
		last = block->ys[j];
	}
	return runs;
}

/*
================
BinaryVoxelizer::SliceData::CountSolidRuns
================
*/
// FUNCTION: STBC 0x004ae0a0
int BinaryVoxelizer::SliceData::CountSolidRuns() const {
	return CountRuns( solidHead, solidCount );
}

/*
================
BinaryVoxelizer::SliceData::CountSurfaceRuns
================
*/
// FUNCTION: STBC 0x004ae0b0
int BinaryVoxelizer::SliceData::CountSurfaceRuns() const {
	return CountRuns( surfaceHead, surfaceCount );
}

/*
================
BinaryVoxelizer::SliceData::GetSolidRuns
================
*/
// FUNCTION: STBC 0x004ae0c0
// PARKED: register allocation: retail keeps the row count in a frame slot and the ys cursor in ebp; c2trace --regs: row (prio 185) and run (182) outrank the cursor (156), retail needs the cursor above row
int BinaryVoxelizer::SliceData::GetSolidRuns( VoxelSliceSpan *runs ) const {
	if ( solidCount < 1 ) {
		return 0;
	}

	const SliceData::SliceChunk *block = solidHead;
	int start = block->ys[0];
	int row = 0;
	int run = 0;
	int last = start - 1;
	int blocks = solidCount / 32;
	int i;
	int j;
	for ( i = 0; i < blocks; i++ ) {
		for ( j = 0; j < 32; j++ ) {
			if ( last != block->ys[j] - 1 ) {
				runs[run].start = start;
				runs[run].end = last + 1;
				runs[run].firstRow = row - ( last + 1 - start );
				start = block->ys[j];
				run++;
			}
			last = block->ys[j];
			row++;
		}
		block = block->next;
	}
	int remaining = solidCount % 32;
	for ( j = 0; j < remaining; j++ ) {
		if ( last != block->ys[j] - 1 ) {
			runs[run].start = start;
			runs[run].end = last + 1;
			runs[run].firstRow = row - ( last + 1 - start );
			start = block->ys[j];
			run++;
		}
		last = block->ys[j];
		row++;
	}
	runs[run].start = start;
	runs[run].end = last + 1;
	runs[run].firstRow = row - ( last + 1 - start );

	// the count is not used
	CountSolidRuns();
	return run + 1;
}

/*
================
BinaryVoxelizer::SliceData::GetSurfaceRuns
================
*/
// FUNCTION: STBC 0x004ae220
// PARKED: register allocation: retail keeps the row count in a frame slot and the ys cursor in ebp; c2trace --regs: row (prio 185) and run (182) outrank the cursor (156), retail needs the cursor above row
int BinaryVoxelizer::SliceData::GetSurfaceRuns( VoxelSliceSpan *runs ) const {
	if ( surfaceCount < 1 ) {
		return 0;
	}

	const SliceData::SliceChunk *block = surfaceHead;
	int start = block->ys[0];
	int row = 0;
	int run = 0;
	int last = start - 1;
	int blocks = surfaceCount / 32;
	int i;
	int j;
	for ( i = 0; i < blocks; i++ ) {
		for ( j = 0; j < 32; j++ ) {
			if ( last != block->ys[j] - 1 ) {
				runs[run].start = start;
				runs[run].end = last + 1;
				runs[run].firstRow = row - ( last + 1 - start );
				start = block->ys[j];
				run++;
			}
			last = block->ys[j];
			row++;
		}
		block = block->next;
	}
	int remaining = surfaceCount % 32;
	for ( j = 0; j < remaining; j++ ) {
		if ( last != block->ys[j] - 1 ) {
			runs[run].start = start;
			runs[run].end = last + 1;
			runs[run].firstRow = row - ( last + 1 - start );
			start = block->ys[j];
			run++;
		}
		last = block->ys[j];
		row++;
	}
	runs[run].start = start;
	runs[run].end = last + 1;
	runs[run].firstRow = row - ( last + 1 - start );

	// the count is not used
	CountSurfaceRuns();
	return run + 1;
}

/*
================
BinaryVoxelizer::SliceData::WriteSurfaceNodes
================
*/
// FUNCTION: STBC 0x004ae380
void BinaryVoxelizer::SliceData::WriteSurfaceNodes( VoxelBSPNode *nodes, int &next, NiPlane *planes_ ) {
	SliceData::SliceChunk *block = surfaceHead;
	int blocks = surfaceCount / 32;
	int i;
	int j;
	for ( i = 0; i < blocks; i++ ) {
		for ( j = 0; j < 32; j++ ) {
			static_cast<BoundaryData *>( block->rows[j] )->WriteNodes( nodes, next, planes_ );
		}
		block = block->next;
	}
	int rest = surfaceCount % 32;
	for ( j = 0; j < rest; j++ ) {
		static_cast<BoundaryData *>( block->rows[j] )->WriteNodes( nodes, next, planes_ );
	}
}

/*
================
BinaryVoxelizer::SliceData::CountSolidRowSize
================
*/
// FUNCTION: STBC 0x004ae430
unsigned int BinaryVoxelizer::SliceData::CountSolidRowSize() const {
	SliceData::SliceChunk *block = solidHead;
	int size = 0;
	int blocks = solidCount / 32;
	int i;
	int j;
	for ( i = 0; i < blocks; i++ ) {
		for ( j = 0; j < 32; j++ ) {
			if ( !block->rows[j]->written ) {
				size += block->rows[j]->GetSize();
				block->rows[j]->written = true;
			}
		}
		block = block->next;
	}
	int rest = solidCount % 32;
	for ( j = 0; j < rest; j++ ) {
		if ( !block->rows[j]->written ) {
			size += block->rows[j]->GetSize();
			block->rows[j]->written = true;
		}
	}
	return size;
}

/*
================
BinaryVoxelizer::SliceData::CountSurfaceRowSize
================
*/
// FUNCTION: STBC 0x004ae4d0
void BinaryVoxelizer::SliceData::CountSurfaceRowSize( unsigned int &rowSize, unsigned int &nodeSize ) const {
	rowSize = 0;
	nodeSize = 0;
	SliceData::SliceChunk *block = surfaceHead;
	int blocks = surfaceCount / 32;
	int i;
	int j;
	for ( i = 0; i < blocks; i++ ) {
		for ( j = 0; j < 32; j++ ) {
			if ( !block->rows[j]->written ) {
				rowSize += block->rows[j]->GetSize();
				nodeSize += static_cast<BoundaryData *>( block->rows[j] )->GetNodeSize();
				block->rows[j]->written = true;
			}
		}
		block = block->next;
	}
	int rest = surfaceCount % 32;
	for ( j = 0; j < rest; j++ ) {
		if ( !block->rows[j]->written ) {
			rowSize += block->rows[j]->GetSize();
			nodeSize += static_cast<BoundaryData *>( block->rows[j] )->GetNodeSize();
			block->rows[j]->written = true;
		}
	}
}

/*
================
BinaryVoxelizer::SliceData::ClearWritten
================
*/
// FUNCTION: STBC 0x004ae5c0 ?ClearWritten@SliceData@BinaryVoxelizer@@QBEXPBUSliceChunk@12@H@Z
// DamageTool.exe 0x408810 (SliceData::Unmark): const, the chunk list walked through a local
void BinaryVoxelizer::SliceData::ClearWritten( const SliceData::SliceChunk *first, int count ) const {
	const SliceChunk *chunk = first;
	int j;
	for ( int i = 0; i < count / 32; i++ ) {
		for ( j = 0; j < 32; j++ ) {
			chunk->rows[j]->written = false;
		}
		chunk = chunk->next;
	}
	for ( j = 0; j < count % 32; j++ ) {
		chunk->rows[j]->written = false;
	}
}

/*
================
BinaryVoxelizer::SliceData::ClearWritten
================
*/
// FUNCTION: STBC 0x004ae630 ?ClearWritten@SliceData@BinaryVoxelizer@@QBEXXZ
void BinaryVoxelizer::SliceData::ClearWritten() const {
	ClearWritten( solidHead, solidCount );
	ClearWritten( surfaceHead, surfaceCount );
}

/*
================
BinaryVoxelizer::SliceData::GetSize

Two counts, the runs, then a row pointer per row.
================
*/
// FUNCTION: STBC 0x004ae660
// PARKED: register allocation: which register holds the run sum (operand orders and run locals tried)
unsigned int BinaryVoxelizer::SliceData::GetSize() const {
	int solidRuns = CountSolidRuns();
	int surfaceRuns = CountSurfaceRuns();
	return ( surfaceRuns + solidRuns ) * sizeof( VoxelSliceSpan ) + ( surfaceCount + solidCount ) * sizeof( VoxelRow * ) + sizeof( int );
}

/*
================
BinaryVoxelizer::SliceData::WriteSolidRows
================
*/
// FUNCTION: STBC 0x004ae690
void BinaryVoxelizer::SliceData::WriteSolidRows( char *block, int &offset_ ) {
	SliceData::SliceChunk *rowBlock = solidHead;
	int blocks = solidCount / 32;
	int i;
	int j;
	for ( i = 0; i < blocks; i++ ) {
		for ( j = 0; j < 32; j++ ) {
			if ( !rowBlock->rows[j]->written ) {
				rowBlock->rows[j]->Write( block, offset_ );
			}
		}
		rowBlock = rowBlock->next;
	}
	int rest = solidCount % 32;
	for ( j = 0; j < rest; j++ ) {
		if ( !rowBlock->rows[j]->written ) {
			rowBlock->rows[j]->Write( block, offset_ );
		}
	}
}

/*
================
BinaryVoxelizer::SliceData::WriteSurfaceRows
================
*/
// FUNCTION: STBC 0x004ae730
void BinaryVoxelizer::SliceData::WriteSurfaceRows( char *block, int &offset_ ) {
	SliceData::SliceChunk *rowBlock = surfaceHead;
	int blocks = surfaceCount / 32;
	int i;
	int j;
	for ( i = 0; i < blocks; i++ ) {
		for ( j = 0; j < 32; j++ ) {
			if ( !rowBlock->rows[j]->written ) {
				static_cast<BoundaryData *>( rowBlock->rows[j] )->Write( block, offset_ );
			}
		}
		rowBlock = rowBlock->next;
	}
	int rest = surfaceCount % 32;
	for ( j = 0; j < rest; j++ ) {
		if ( !rowBlock->rows[j]->written ) {
			static_cast<BoundaryData *>( rowBlock->rows[j] )->Write( block, offset_ );
		}
	}
}

/*
================
BinaryVoxelizer::SliceData::Write

The runs are written over the slice's spans.
================
*/
// FUNCTION: STBC 0x004ae7d0
// PARKED: retail keeps &slice->spans in ebx across the calls and reads GetRowCount's run through it ([ebx + i*12 + 8]); this build folds the base into slice + 4 (a spans local tried); body complete
unsigned int BinaryVoxelizer::SliceData::Write( VoxelSlice *slice ) const {
	slice->solidCount = GetSolidRuns( slice->spans );
	slice->surfaceCount = GetSurfaceRuns( slice->GetSurfaceSpans() );
	GetSolidRowOffsets( reinterpret_cast<int *>( slice->GetRows() ) );
	GetSurfaceRowOffsets( reinterpret_cast<int *>( slice->GetSurfaceRows() ) );
	return (char *)( slice->GetSurfaceRows() + slice->GetNumBoundaryEntries() ) - (char *)slice;
}

/*
================
BinaryVoxelizer::BinaryVoxelizer
================
*/
// FUNCTION: STBC 0x004ae900
BinaryVoxelizer::BinaryVoxelizer() {
	unknown34 = 0;
	cellSize = 1.0f;
	cellsX = 0;
	cellsY = 0;
	cellsZ = 0;
	unknown3C = 0;
	slices = NULL;
	planeCount = 0;
	planes = NULL;
}

/*
================
BinaryVoxelizer::~BinaryVoxelizer
================
*/
// FUNCTION: STBC 0x004ae940
BinaryVoxelizer::~BinaryVoxelizer() {
	Reset();
}

/*
================
BinaryVoxelizer::Reset
================
*/
// FUNCTION: STBC 0x004aea30
void BinaryVoxelizer::Reset() {
	obbModel = NULL;
	model = NULL;

	for ( int i = 0; i < cellsZ; i++ ) {
		if ( slices[i] != NULL ) {
			delete slices[i];
		}
	}
	delete [] slices;
	slices = NULL;

	cellsX = 0;
	cellsY = 0;
	cellsZ = 0;
	unknown34 = 0;
	cellSize = 1.0f;

	delete [] planes;
	planes = NULL;
	planeCount = 0;
}

/*
================
BinaryVoxelizer::ExpandBound
================
*/
// FUNCTION: STBC 0x004aeae0
void BinaryVoxelizer::ExpandBound( NiAVObject *object ) {
	NiGeometry *geometry = NiDynamicCast( NiGeometry, object );
	if ( geometry != NULL ) {
		geometry->CreateWorldVertices();
		geometry->UpdateWorldVertices();
		NiPoint3 *vertices = geometry->GetWorldVertices();
		int count = geometry->GetVertexCount();
		for ( int i = 0; i < count; i++ ) {
			if ( vertices[i].x < boundMin.x ) {
				boundMin.x = vertices[i].x;
			}
			if ( vertices[i].x > boundMax.x ) {
				boundMax.x = vertices[i].x;
			}
			if ( vertices[i].y < boundMin.y ) {
				boundMin.y = vertices[i].y;
			}
			if ( vertices[i].y > boundMax.y ) {
				boundMax.y = vertices[i].y;
			}
			if ( vertices[i].z < boundMin.z ) {
				boundMin.z = vertices[i].z;
			}
			if ( vertices[i].z > boundMax.z ) {
				boundMax.z = vertices[i].z;
			}
		}
		return;
	}

	NiNode *node = NiDynamicCast( NiNode, object );
	if ( node != NULL ) {
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			ExpandBound( node->GetAt( i ) );
		}
	}
}

/*
================
BinaryVoxelizer::CountTriangles
================
*/
// FUNCTION: STBC 0x004aec30
int BinaryVoxelizer::CountTriangles( NiAVObject *object ) {
	int count = 0;

	NiTriShape *shape = NiDynamicCast( NiTriShape, object );
	if ( shape != NULL ) {
		return shape->GetShapeData()->GetTriangleCount();
	}

	NiNode *node = NiDynamicCast( NiNode, object );
	if ( node != NULL ) {
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			count += CountTriangles( node->GetAt( i ) );
		}
	}
	return count;
}

/*
================
BinaryVoxelizer::Begin
================
*/
// FUNCTION: STBC 0x004aece0
// PARKED: x87 schedule: retail pushes the slice array's size between the center's halving multiplies
void BinaryVoxelizer::Begin() {
	parent = model->GetParent();
	if ( parent != NULL ) {
		parent->DetachChild( model );
	}
	model->Update( 0.0f );

	if ( obbModel == NULL ) {
		obbModel = new OBBModel( model, 8 );
	}

	boundMax = NiPoint3( -NI_INFINITY, -NI_INFINITY, -NI_INFINITY );
	boundMin = NiPoint3( NI_INFINITY, NI_INFINITY, NI_INFINITY );
	ExpandBound( model );

	float sizeX = boundMax.x - boundMin.x;
	float sizeY = boundMax.y - boundMin.y;
	float sizeZ = boundMax.z - boundMin.z;
	cellsX = (int)( sizeX / cellSize + 1.0f );
	cellsY = (int)( sizeY / cellSize + 1.0f );
	cellsZ = (int)( sizeZ / cellSize + 1.0f );
	cellDiagonal = sqrtf( cellSize * cellSize * 3.0f );

	float gridX = cellsX * cellSize;
	float gridY = cellsY * cellSize;
	float gridZ = cellsZ * cellSize;
	NiPoint3 center = ( boundMax + boundMin ) * 0.5f;
	boundMax.x = center.x + gridX * 0.5f;
	boundMax.y = center.y + gridY * 0.5f;
	boundMax.z = center.z + gridZ * 0.5f;
	boundMin.x = center.x - gridX * 0.5f;
	boundMin.y = center.y - gridY * 0.5f;
	boundMin.z = center.z - gridZ * 0.5f;

	slices = new SliceData *[cellsZ];
	memset( slices, 0, cellsZ * sizeof( SliceData * ) );
	unknown44 = 0;
	unknown40 = 0;

	planes = new NiPlane[CountTriangles( model )];
}

/*
================
BinaryVoxelizer::BuildTree

Builds the subtree of node from count polygons: the polygon whose plane
faces x most closely splits the others into the pieces in front of it and
behind it, which build the two subtrees. Polygons in the splitter's own
plane are dropped.
================
*/
// FUNCTION: STBC 0x004af000
void BinaryVoxelizer::BuildTree( VoxelBspNode *node, ClipPoly *polygons, int count, NiPlane *planes_ ) {
	if ( count > 1 ) {
		int best = 0;
		float bestValue = (float)fabs( planes_[polygons[0].plane].GetNormal().x );
		int i;
		unsigned short j;
		for ( i = 1; i < count; i++ ) {
			float value = (float)fabs( planes_[polygons[i].plane].GetNormal().x );
			if ( value > bestValue ) {
				bestValue = value;
				best = i;
			}
		}
		ClipPoly *splitter = &polygons[best];
		node->plane = splitter->plane;

		ClipPoly *front = new ClipPoly[count];
		ClipPoly *back = new ClipPoly[count];
		int frontCount = 0;
		int backCount = 0;
		for ( i = 0; i < count; i++ ) {
			if ( splitter->plane == polygons[i].plane ) {
				continue;
			}

			int capacity = polygons[i].count + 1;
			NiPoint3 *frontCorners = new NiPoint3[capacity];
			NiPoint3 *backCorners = new NiPoint3[capacity];
			unsigned short frontCornerCount;
			unsigned short backCornerCount;
			node->Split( polygons[i].corners, polygons[i].count, frontCorners, frontCornerCount, backCorners, backCornerCount, planes_ );

			if ( frontCornerCount > 0 ) {
				if ( frontCornerCount <= front[frontCount].capacity ) {
					for ( j = 0; j < frontCornerCount; j++ ) {
						front[frontCount].corners[j] = frontCorners[j];
					}
					delete [] frontCorners;
				} else {
					delete [] front[frontCount].corners;
					front[frontCount].corners = frontCorners;
					front[frontCount].capacity = capacity;
				}
				front[frontCount].count = frontCornerCount;
				front[frontCount].plane = polygons[i].plane;
				frontCount++;
			} else {
				delete [] frontCorners;
			}

			if ( backCornerCount > 0 ) {
				if ( backCornerCount <= back[backCount].capacity ) {
					for ( j = 0; j < backCornerCount; j++ ) {
						back[backCount].corners[j] = backCorners[j];
					}
					delete [] backCorners;
				} else {
					delete [] back[backCount].corners;
					back[backCount].corners = backCorners;
					back[backCount].capacity = capacity;
				}
				back[backCount].count = backCornerCount;
				back[backCount].plane = polygons[i].plane;
				backCount++;
			} else {
				delete [] backCorners;
			}
		}

		if ( frontCount != 0 ) {
			node->front = new VoxelBspNode;
			BuildTree( node->front, front, frontCount, planes_ );
		} else {
			node->front = NULL;
		}
		if ( backCount != 0 ) {
			node->back = new VoxelBspNode;
			BuildTree( node->back, back, backCount, planes_ );
		} else {
			node->back = NULL;
		}

		delete [] front;
		delete [] back;
	} else {
		node->plane = polygons[0].plane;
		node->front = NULL;
		node->back = NULL;
	}
}

/*
================
BinaryVoxelizer::BuildCellTree

The tree of a surface cell's polygons, with the cell's center (invented
name).
================
*/
// FUNCTION: STBC 0x004af4a0
BinaryVoxelizer::VoxelBspRoot *BinaryVoxelizer::BuildCellTree( ClipPoly *polygons, int count, const NiPoint3 &point ) {
	VoxelBspRoot *tree = new VoxelBspRoot;
	tree->point = point;
	BuildTree( tree, polygons, count, planes );
	return tree;
}

/*
================
BinaryVoxelizer::ClipPoly::CompareMinX

qsort's order for the row's polygons: by their least x
================
*/
// FUNCTION: STBC 0x004b10f0
int BinaryVoxelizer::ClipPoly::CompareMinX( const void *a, const void *b ) {
	const ClipPoly *first = (const ClipPoly *)a;
	const ClipPoly *second = (const ClipPoly *)b;
	if ( first->minX < second->minX ) {
		return -1;
	}
	if ( first->minX > second->minX ) {
		return 1;
	}
	return 0;
}

/*
================
BinaryVoxelizer::BoundaryData::~BoundaryData
================
*/
// FUNCTION: STBC 0x004b11d0
BinaryVoxelizer::BoundaryData::~BoundaryData() {
	delete cellHead;
}

/*
================
BinaryVoxelizer::Step

Slices one row of cells, the one at unknown40 and unknown44: the model's
triangles that meet the row's box are clipped to it, and a triangle whose
plane faces another one's the other way is taken for double sided and
drops both. The polygons left are grouped into runs that overlap along x;
each run is a run of surface cells, each cell with the tree of its
polygons clipped to it, and the gap after a run that lies inside the model
is a run of solid cells. The rows go into the plane's slice, and the next
call takes the next row. Once the last plane is done the data is built and
false comes back (invented name).
================
*/
// FUNCTION: STBC 0x004af4f0
// DRAFT: calls and branches as retail but one (an extra jmp after the solid run's Add); the frame is 12 bytes larger (0x1B4 against 0x1A8) and the registers differ throughout (retail keeps this in ESI and spills it at 0x28)
bool BinaryVoxelizer::Step( float *progress ) {
	if ( unknown34 == 0 || unknown34 == 3 ) {
		Begin();
		unknown34 = 1;
	}
	int row = cellsY * unknown44 + unknown40;

	// the row's box: the grid's width, a cell high and deep
	float width = boundMax.x - boundMin.x;
	NiBox box;
	box.center = NiPoint3( width * 0.5f + boundMin.x, ( unknown40 + 0.5f ) * cellSize + boundMin.y,
						   ( unknown44 + 0.5f ) * cellSize + boundMin.z );
	box.axis[0] = NiPoint3::UNIT_X;
	box.axis[1] = NiPoint3::UNIT_Y;
	box.axis[2] = NiPoint3::UNIT_Z;
	box.extent[0] = width * 0.5f;
	float half = cellSize * 0.5f;
	box.extent[1] = half;
	box.extent[2] = half;

	NiTArray<OBBTriangleList *> lists;
	if ( obbModel->FindBoxIntersections( box, &lists, 64000 ) ) {
		int listCount = lists.GetSize();
		int triangleCount = 0;
		int l;
		for ( l = 0; l < listCount; l++ ) {
			triangleCount += lists.GetAt( l )->count;
		}

		// the triangles clipped to the box, in the model's frame
		ClipPoly *polygons = new ClipPoly[triangleCount];
		int polygonCount = 0;
		NiPoint3 corners[9];
		NiPoint3 clipped[9];
		int clippedCount;
		int cornerCount;
		int i;
		int prev;
		float t;
		for ( l = 0; l < listCount; l++ ) {
			int doubleSided = 0;
			OBBTriangleList *list = lists.GetAt( l );
			int count = list->count;
			NiTriShape *shape = list->shape;
			NiPoint3 *vertices = shape->GetWorldVertices();
			unsigned short *triList = shape->GetShapeData()->GetTriList();
			for ( int triangle = 0; triangle < count; triangle++ ) {
				unsigned short corner = list->triangles[triangle] * 3;
				corners[0] = vertices[triList[corner++]];
				corners[1] = vertices[triList[corner++]];
				corners[2] = vertices[triList[corner]];
				NiPlane plane( corners[0], corners[1], corners[2] );
				int opposite = FindOppositePlane( plane );
				if ( opposite != -1 ) {
					// both faces go: the last polygon takes the place of the
					// one dropped, its corner array swapped so that each
					// array keeps one owner
					doubleSided++;
					for ( int k = 0; k < polygonCount; k++ ) {
						if ( polygons[k].plane == opposite ) {
							polygonCount--;
							ClipPoly &last = polygons[polygonCount];
							NiPoint3 *keptCorners = polygons[k].corners;
							polygons[k].corners = last.corners;
							last.corners = keptCorners;
							polygons[k].plane = last.plane;
							polygons[k].count = last.count;
							int keptCapacity = polygons[k].capacity;
							polygons[k].capacity = last.capacity;
							last.capacity = keptCapacity;
							polygons[k].minX = last.minX;
							polygons[k].maxX = last.maxX;
						}
					}
				} else {
					ClipPoly *polygon = &polygons[polygonCount];
					polygon->plane = AddPlane( plane );
					corners[0] -= box.center;
					corners[1] -= box.center;
					corners[2] -= box.center;

					// clipped to the box's y range, then its z range; each
					// pass keeps the corners inside and adds where the
					// edges cross
					float low = -half;
					clippedCount = 0;
					for ( i = 0, prev = 3 - 1; i < 3; prev = i, i++ ) {
						if ( corners[i].y >= low ) {
							if ( corners[prev].y >= low ) {
								clipped[clippedCount++] = corners[i];
							} else {
								t = -( corners[prev].y - low ) / ( corners[i].y - corners[prev].y );
								clipped[clippedCount++] = ( corners[i] - corners[prev] ) * t + corners[prev];
								clipped[clippedCount++] = corners[i];
							}
						} else if ( corners[prev].y >= low ) {
							t = -( corners[prev].y - low ) / ( corners[i].y - corners[prev].y );
							clipped[clippedCount++] = ( corners[i] - corners[prev] ) * t + corners[prev];
						}
					}
					cornerCount = 0;
					for ( i = 0, prev = clippedCount - 1; i < clippedCount; prev = i, i++ ) {
						if ( clipped[i].y <= half ) {
							if ( clipped[prev].y <= half ) {
								corners[cornerCount++] = clipped[i];
							} else {
								t = ( clipped[prev].y - half ) / -( clipped[i].y - clipped[prev].y );
								corners[cornerCount++] = ( clipped[i] - clipped[prev] ) * t + clipped[prev];
								corners[cornerCount++] = clipped[i];
							}
						} else if ( clipped[prev].y <= half ) {
							t = ( clipped[prev].y - half ) / -( clipped[i].y - clipped[prev].y );
							corners[cornerCount++] = ( clipped[i] - clipped[prev] ) * t + clipped[prev];
						}
					}
					clippedCount = 0;
					for ( i = 0, prev = cornerCount - 1; i < cornerCount; prev = i, i++ ) {
						if ( corners[i].z >= low ) {
							if ( corners[prev].z >= low ) {
								clipped[clippedCount++] = corners[i];
							} else {
								t = -( corners[prev].z - low ) / ( corners[i].z - corners[prev].z );
								clipped[clippedCount++] = ( corners[i] - corners[prev] ) * t + corners[prev];
								clipped[clippedCount++] = corners[i];
							}
						} else if ( corners[prev].z >= low ) {
							t = -( corners[prev].z - low ) / ( corners[i].z - corners[prev].z );
							clipped[clippedCount++] = ( corners[i] - corners[prev] ) * t + corners[prev];
						}
					}
					cornerCount = 0;
					for ( i = 0, prev = clippedCount - 1; i < clippedCount; prev = i, i++ ) {
						if ( clipped[i].z <= half ) {
							if ( clipped[prev].z <= half ) {
								corners[cornerCount++] = clipped[i];
							} else {
								t = ( clipped[prev].z - half ) / -( clipped[i].z - clipped[prev].z );
								corners[cornerCount++] = ( clipped[i] - clipped[prev] ) * t + clipped[prev];
								corners[cornerCount++] = clipped[i];
							}
						} else if ( clipped[prev].z <= half ) {
							t = ( clipped[prev].z - half ) / -( clipped[i].z - clipped[prev].z );
							corners[cornerCount++] = ( clipped[i] - clipped[prev] ) * t + clipped[prev];
						}
					}
					if ( cornerCount >= 3 ) {
						polygon->minX = NI_INFINITY;
						polygon->maxX = -NI_INFINITY;
						polygon->count = cornerCount;
						for ( int c = 0; c < cornerCount; c++ ) {
							polygon->corners[c] = box.center + corners[c];
							if ( polygon->corners[c].x < polygon->minX ) {
								polygon->minX = polygon->corners[c].x;
							}
							if ( polygon->corners[c].x > polygon->maxX ) {
								polygon->maxX = polygon->corners[c].x;
							}
						}
						polygonCount++;
					}
				}
			}
			if ( doubleSided > 0 ) {
				FILE *file = GetDebugFile();
				if ( file != NULL ) {
					if ( list->shape->GetName() == NULL ) {
						fprintf( file, "%d double sided triangle(s) detected in unnamed geometry.\n", doubleSided );
					} else {
						fprintf( file, "%d double sided triangle(s) detected in %s.\n", doubleSided, list->shape->GetName() );
					}
				}
			}
			delete list;
		}

		qsort( polygons, polygonCount, sizeof( ClipPoly ), ClipPoly::CompareMinX );

		InsideData *solidRow = NULL;
		BoundaryData *surfaceRow = new BoundaryData;
		int first = 0;
		int x;
		do {
			// the run of cells the polygons from first on cover
			x = (int)( ( polygons[first].minX - boundMin.x ) / cellSize );
			int end = (int)( ( polygons[first].maxX - boundMin.x ) / cellSize ) + 1;
			int next;
			for ( next = first + 1; next < polygonCount; next++ ) {
				if ( (int)( ( polygons[next].minX - boundMin.x ) / cellSize ) > end ) {
					break;
				}
				int nextEnd = (int)( ( polygons[next].maxX - boundMin.x ) / cellSize ) + 1;
				if ( end < nextEnd ) {
					end = nextEnd;
				}
			}
			BoundaryBspsData *cells = new BoundaryBspsData;
			surfaceRow->Add( x, end, cells );

			// each cell's tree, from the run's polygons clipped to it
			int runCount = next - first;
			ClipPoly *cellPolygons = new ClipPoly[runCount];
			VoxelBspRoot *tree;
			for ( ; x < end; x++ ) {
				float cellMin = x * cellSize + boundMin.x;
				float cellMax = ( x + 1 ) * cellSize + boundMin.x;
				int cellCount = 0;
				for ( int k = first; k < next; k++ ) {
					if ( polygons[k].minX < cellMax && polygons[k].maxX > cellMin ) {
						clippedCount = 0;
						for ( i = 0, prev = polygons[k].count - 1; i < polygons[k].count; prev = i, i++ ) {
							if ( polygons[k].corners[i].x >= cellMin ) {
								if ( polygons[k].corners[prev].x >= cellMin ) {
									clipped[clippedCount++] = polygons[k].corners[i];
								} else {
									t = -( polygons[k].corners[prev].x - cellMin ) / ( polygons[k].corners[i].x - polygons[k].corners[prev].x );
									clipped[clippedCount++] = ( polygons[k].corners[i] - polygons[k].corners[prev] ) * t + polygons[k].corners[prev];
									clipped[clippedCount++] = polygons[k].corners[i];
								}
							} else if ( polygons[k].corners[prev].x >= cellMin ) {
								t = -( polygons[k].corners[prev].x - cellMin ) / ( polygons[k].corners[i].x - polygons[k].corners[prev].x );
								clipped[clippedCount++] = ( polygons[k].corners[i] - polygons[k].corners[prev] ) * t + polygons[k].corners[prev];
							}
						}
						cornerCount = 0;
						for ( i = 0, prev = clippedCount - 1; i < clippedCount; prev = i, i++ ) {
							if ( clipped[i].x <= cellMax ) {
								if ( clipped[prev].x <= cellMax ) {
									corners[cornerCount++] = clipped[i];
								} else {
									t = ( clipped[prev].x - cellMax ) / -( clipped[i].x - clipped[prev].x );
									corners[cornerCount++] = ( clipped[i] - clipped[prev] ) * t + clipped[prev];
									corners[cornerCount++] = clipped[i];
								}
							} else if ( clipped[prev].x <= cellMax ) {
								t = ( clipped[prev].x - cellMax ) / -( clipped[i].x - clipped[prev].x );
								corners[cornerCount++] = ( clipped[i] - clipped[prev] ) * t + clipped[prev];
							}
						}
						cellPolygons[cellCount].count = cornerCount;
						cellPolygons[cellCount].plane = polygons[k].plane;
						for ( int c = 0; c < cornerCount; c++ ) {
							cellPolygons[cellCount].corners[c] = corners[c];
						}
						cellCount++;
					}
				}
				tree = BuildCellTree( cellPolygons, cellCount, NiPoint3( x * cellSize, unknown40 * cellSize, unknown44 * cellSize ) + boundMin );
				cells->Add( tree );
			}

			// the cells up to the next run are solid when the corner past
			// the run is inside the last cell's tree
			if ( next < polygonCount ) {
				if ( tree->IsInside( NiPoint3( x * cellSize, unknown40 * cellSize, unknown44 * cellSize ) + boundMin, planes ) ) {
					if ( solidRow == NULL ) {
						solidRow = new InsideData;
					}
					solidRow->Add( x - 1, (int)( ( polygons[next].minX - boundMin.x ) / cellSize ) );
				}
			}
			delete [] cellPolygons;
			first = next;
		} while ( x < cellsX && first < polygonCount );
		delete [] polygons;

		if ( surfaceRow->count > 0 ) {
			if ( unknown3C == NULL ) {
				unknown3C = new SliceData;
				slices[unknown44] = unknown3C;
			}
			unknown3C->AddSurfaceRow( unknown40, surfaceRow );
			if ( solidRow != NULL && solidRow->count > 0 ) {
				unknown3C->AddSolidRow( unknown40, solidRow );
			}
		} else {
			delete surfaceRow;
			delete solidRow;
		}
	}

	if ( unknown40 < cellsY ) {
		unknown40++;
	} else {
		unknown3C = NULL;
		unknown40 = 0;
		unknown44++;
	}
	int rows = cellsZ * cellsY;
	*progress = (float)row / rows;
	if ( row == rows ) {
		unknown34 = 2;
		CloseDebugFile();
		BuildData();
		return false;
	}
	return true;
}

/*
================
BinaryVoxelizer::BuildData

Builds the NiBinaryVoxelData: the grid and the planes, then one block that
holds a reference per grid plane, the slices, the solid rows, the surface
rows and the surface cells' trees. Slices and rows shared between planes are
written once.
================
*/
// FUNCTION: STBC 0x004b0940
// DRAFT: retail keeps the counter in EBX and spills the reference pointer (ours: the counter stays in memory, spill cost 0 in c2trace --regs); the slice block's start is shl/add, not lea
void BinaryVoxelizer::BuildData() {
	voxelData = new NiBinaryVoxelData;
	voxelData->unknown14 = boundMin;
	voxelData->unknown20 = boundMax;
	voxelData->unknown10 = cellSize;

	unsigned int sliceSize;
	unsigned int solidRowSize;
	unsigned int surfaceRowSize;
	int size = GetDataSize( sliceSize, solidRowSize, surfaceRowSize );
	voxelData->sizeX = cellsX;
	voxelData->sizeY = cellsY;
	voxelData->sizeZ = cellsZ;

	voxelData->planeCount = planeCount;
	voxelData->planes = new NiPlane[planeCount];
	memcpy( voxelData->planes, planes, planeCount * sizeof( NiPlane ) );

	char *block = new char[size];
	voxelData->block = block;
	voxelData->blockSize = size;
	voxelData->sliceRefs = (VoxelSliceRef *)block;
	block += cellsZ * sizeof( VoxelSliceRef );
	voxelData->slices = (VoxelSlice *)block;
	voxelData->rows = block + sliceSize;
	voxelData->surfaceRows = block + sliceSize + solidRowSize;
	voxelData->nodes = (VoxelBSPNode *)( block + sliceSize + solidRowSize + surfaceRowSize );

	int offset = 0;
	int solidOffset = 0;
	int surfaceOffset = 0;
	int nextNode = 0;
	VoxelSliceRef *refs = voxelData->sliceRefs;
	char *sliceBlock = (char *)voxelData->slices;
	char *rows = voxelData->rows;
	SliceData **list = slices;
	for ( int i = 0; i < cellsZ; i++, refs++ ) {
		if ( list[i] != NULL ) {
			if ( !list[i]->written ) {
				list[i]->written = true;
				list[i]->offset = offset;
				list[i]->WriteSurfaceNodes( voxelData->nodes, nextNode, planes );
				list[i]->WriteSurfaceRows( voxelData->surfaceRows, surfaceOffset );
				list[i]->WriteSolidRows( rows, solidOffset );
				offset += list[i]->Write( (VoxelSlice *)( sliceBlock + offset ) );
			}
			refs->slice = (VoxelSlice *)list[i]->offset;
		} else {
			refs->slice = (VoxelSlice *)-1;
		}
	}
	voxelData->OffsetsToPointers();
	voxelData->BuildBits();

	if ( parent != NULL ) {
		parent->AttachChild( model, false );
	}
	unknown34 = 3;
}

/*
================
BinaryVoxelizer::AddPlane
================
*/
// FUNCTION: STBC 0x004b0bf0
int BinaryVoxelizer::AddPlane( const NiPlane &plane ) {
	float constant = plane.GetConstant();
	for ( int i = 0; i < planeCount; i++ ) {
		if ( (float)fabs( constant - planes[i].GetConstant() ) < 0.001f && planes[i].GetNormal() * plane.GetNormal() > 0.999f ) {
			return i;
		}
	}
	planes[planeCount++] = plane;
	return planeCount - 1;
}

/*
================
BinaryVoxelizer::FindOppositePlane
================
*/
// FUNCTION: STBC 0x004b0c80
int BinaryVoxelizer::FindOppositePlane( const NiPlane &plane ) {
	float constant = plane.GetConstant();
	for ( int i = 0; i < planeCount; i++ ) {
		if ( (float)fabs( planes[i].GetConstant() + constant ) < 0.001f && plane.GetNormal() * planes[i].GetNormal() < -0.999f ) {
			return i;
		}
	}
	return -1;
}

/*
================
BinaryVoxelizer::GetDataSize

Counts each shared slice and row once: the marks are cleared before and
after.
================
*/
// FUNCTION: STBC 0x004b0cf0
// DRAFT: register choice (retail keeps the count in EDI and the node bytes in EBX) and the total's order (retail adds count * 4 after the row bytes)
int BinaryVoxelizer::GetDataSize( unsigned int &sliceSize, unsigned int &solidRowSize, unsigned int &surfaceRowSize ) const {
	unsigned int count = cellsZ;
	sliceSize = 0;
	solidRowSize = 0;
	surfaceRowSize = 0;
	SliceData **list = slices;
	int nodeSize = 0;
	unsigned int i;
	for ( i = 0; i < count; i++ ) {
		if ( list[i] != NULL ) {
			list[i]->ClearWritten();
			list[i]->written = false;
		}
	}
	for ( i = 0; i < count; i++ ) {
		if ( list[i] != NULL ) {
			solidRowSize += list[i]->CountSolidRowSize();
			unsigned int rowSize;
			unsigned int nodes;
			list[i]->CountSurfaceRowSize( rowSize, nodes );
			surfaceRowSize += rowSize;
			nodeSize += nodes;
			sliceSize += list[i]->GetSize();
			list[i]->written = true;
		}
	}
	int size = sliceSize + solidRowSize + surfaceRowSize + count * sizeof( VoxelSliceRef ) + nodeSize;
	for ( i = 0; i < count; i++ ) {
		if ( list[i] != NULL ) {
			list[i]->ClearWritten();
			list[i]->written = false;
		}
	}
	return size;
}
