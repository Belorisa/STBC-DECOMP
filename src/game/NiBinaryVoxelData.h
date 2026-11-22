/*
===============================================================================

	NiBinaryVoxelData.h

	A model's solid volume as a grid of voxels, with flag bits per cell and
	a tree of cell data in one block, for the visible damage system. The
	class is the game's own, written in NetImmerse's style: it streams in
	NIF files under the name "NiBinaryVoxelData" (0x8DD2A8).

	0x6C bytes. Evidence: the NiRTTI at 0x983930 is named
	"NiBinaryVoxelData" with NiObject's as its base; the vtable 0x88C57C
	(12 slots) replaces GetRTTI (0x4AC350), the six streaming slots
	(0x4A5890 LoadBinary, 0x4A5880 LinkObject, 0x4A5B10
	RegisterStreamables, 0x4A5B20 SaveBinary, 0x4A5CE0 IsEqual) and the
	destructor (0x4A7B40); the constructor is 0x4A7AE0, the destructor
	0x4A7B60. LoadBinary and SaveBinary give the layout: three 16-bit grid
	sizes, a float, two points, a count of planes and the planes, seven bit
	arrays of one bit per cell, and a block of the given size that five
	pointers point into (stored in the file as offsets from its start).
	The voxelizer (0x4B0940) fills the members directly and then calls
	BuildBits (0x4A7BD0) for the bit arrays. The member names are invented.

	BuildBits and the line geometry give their meaning: the grid's cells
	are unknown10 wide and start at unknown14, and unknown20 is the grid's
	far corner (CreateGridLines, 0x4AABE0); cellBits says a cell's lowest
	corner is inside the model, unknown4C[axis] that the cell's edge from
	that corner along the axis meets it, unknown58 the same for the three
	faces at that corner.

	Retail's layout shows more than one original file: an <iostream> pair
	of initialisers at 0x4AA770 starts a second object, which holds the
	line geometry, GetTree and the row and slice functions up to 0x4ABEA0.
	This unit compiles them together.

===============================================================================
*/

#ifndef __NIBINARYVOXELDATA_H__
#define __NIBINARYVOXELDATA_H__

#include "NiObject.h"
#include "NiPoint3.h"
#include "NiPlane.h"
#include "NiStreamMacros.h"
#include "NiTList.h"

class NiStream;
class NiAVObject;
class NiNode;
class NiLines;

/*
	The block of a NiBinaryVoxelData holds the grid as runs of cells. Each
	plane of the grid along z is a VoxelSlice; a slice lists its runs of
	rows along y, and each row lists its runs of cells along x. The rows of
	cells inside the model (VoxelRow) come first, then the rows of cells on
	its surface (VoxelSurfaceRow), whose runs point to data per cell. In the
	file every pointer is an offset from the start of its part of the block
	(PointersToOffsets, OffsetsToPointers). All the names are invented.
*/

// A convex polygon cut against the trees (ClipInside): the visible damage
// code hands over triangles and gets back the parts inside the model. 0xC
// bytes; the constructor is 0x4AC220. The names are invented.
struct VoxelPolygon {
	int					count;			// vertices in use
	int					capacity;
	NiPoint3 *			vertices;		// capacity of them

						VoxelPolygon( int capacity_ );
						~VoxelPolygon() { delete [] vertices; }
};

typedef NiTList<VoxelPolygon *> VoxelPolygonList;

// A node of the solid BSP trees that model the surface inside each surface
// cell: the nodes on the positive and the negative side of the node's
// plane, as indices into the data's nodes (0 for none). A point past a
// missing positive side is outside the model, past a missing negative
// side inside it.
struct VoxelBSPNode {
	unsigned short		front;			// the positive side
	unsigned short		back;			// the negative side
	unsigned short		plane;			// index into the data's planes

	bool				IsInside( VoxelBSPNode *nodes, NiPlane *planes, const NiPoint3 &point );
	// whether any part of the segment is inside the model
	bool				IsSegmentInside( VoxelBSPNode *nodes, NiPlane *planes, const NiPoint3 *start, const NiPoint3 *end );
	// whether any part of the triangle is inside the model
	bool				IsTriangleInside( VoxelBSPNode *nodes, NiPlane *planes, const NiPoint3 *point0, const NiPoint3 *point1, const NiPoint3 *point2 );

	// the part of the polygon behind the node's plane; NULL when there is none
	void				ClipBack( VoxelBSPNode *nodes, NiPlane *planes, VoxelPolygon *polygon, VoxelPolygon **back );
	// the parts of the polygon in front of and behind the node's plane
	void				Split( VoxelBSPNode *nodes, NiPlane *planes, VoxelPolygon *polygon, VoxelPolygon **front, VoxelPolygon **back );
	// adds the parts of the polygon inside the model to the list and
	// deletes the polygon
	void				ClipInside( VoxelBSPNode *nodes, NiPlane *planes, VoxelPolygon *polygon, VoxelPolygonList *inside );
};

// one cell's tree: the root among the data's nodes
struct VoxelBSPTree {
	VoxelBSPNode *		nodes;
	NiPlane *			planes;
	unsigned short		root;

	bool				IsInside( const NiPoint3 &point );
	bool				IsSegmentInside( const NiPoint3 *start, const NiPoint3 *end );
	bool				IsTriangleInside( const NiPoint3 *point0, const NiPoint3 *point1, const NiPoint3 *point2 );
	// whether any part of the quad's two triangles is inside the model
	bool				IsQuadInside( const NiPoint3 *point0, const NiPoint3 *point1, const NiPoint3 *point2, const NiPoint3 *point3 );
	void				ClipInside( VoxelPolygon *polygon, VoxelPolygonList *inside );
};

// a run of cells along x, from start up to end (not included)
struct VoxelSpan {
	unsigned short		start;
	unsigned short		end;
};

// the runs of cells of one row that are inside the model
struct VoxelRow {
	int					count;
	VoxelSpan			spans[1];		// count of them

	// the run holding cell x (the signatures are DamageTool.exe's
	// BinaryVoxel::Inside::IsInside, 0x4053E0 and 0x405440)
	bool				GetSpan( unsigned short x, unsigned short &start, unsigned short &end ) const;
	bool				IsSolid( unsigned short x ) const;

	// DamageTool.exe's BinaryVoxel::Inside::GetVisualRep (0x4055E0)
	NiAVObject *		CreateLines( const NiPoint3 &origin, float size ) const;
};

// a run of surface cells along x, with the data of each
struct VoxelSurfaceSpan {
	unsigned short		start;
	unsigned short		end;
	VoxelBSPNode *		cells;			// the root of each cell's tree, end - start of them; in the file an index into the nodes
};

// the runs of cells of one row that are on the model's surface
struct VoxelSurfaceRow {
	int					count;
	VoxelSurfaceSpan	spans[1];		// count of them

	// the run holding cell x; the cell's data too when cell is set (the
	// signatures are DamageTool.exe's BinaryVoxel::Boundary::IsInside,
	// 0x405490 and 0x405510)
	bool				GetSpan( unsigned short x, unsigned short &start, unsigned short &end, const VoxelBSPNode *&cell ) const;
	bool				GetCell( unsigned short x, const VoxelBSPNode *&cell ) const;

	void				ToPointers( VoxelBSPNode *nodes );
	void				ToOffsets( VoxelBSPNode *nodes );

	// DamageTool.exe's BinaryVoxel::Boundary::GetVisualRep (0x405A80)
	NiAVObject *		CreateLines( const NiPoint3 &origin, float size ) const;
};

// a run of rows along y, from start up to end (not included); the rows
// are firstRow onwards in the slice's list
struct VoxelSliceSpan {
	unsigned int		start;
	unsigned int		end;
	unsigned int		firstRow;

	// one past the last of the run's rows in the slice's list
	unsigned short		GetEndRow( void ) const { return firstRow + end - start; }
};

// one plane of the grid along z: the runs of rows, then the row pointers
struct VoxelSlice {
	unsigned short		solidCount;		// runs of rows of solid cells
	unsigned short		surfaceCount;	// runs of rows of surface cells, after them
	VoxelSliceSpan		spans[1];		// solidCount + surfaceCount of them, then the rows

	// the run counts; retail extends each read of them on its own, as an
	// inline's unsigned short result (names invented, after DamageTool's
	// GetNumBoundaryEntries below)
	unsigned short		GetSolidCount( void ) const { return solidCount; }
	unsigned short		GetSurfaceCount( void ) const { return surfaceCount; }

	VoxelSliceSpan *	GetSurfaceSpans( void ) { return &spans[GetSolidCount()]; }
	const VoxelSliceSpan *	GetSurfaceSpans( void ) const { return &spans[GetSolidCount()]; }
	const VoxelSliceSpan *	GetSolidRun( unsigned short index ) const { return &spans[index]; }
	const VoxelSliceSpan *	GetSurfaceRun( unsigned short index ) const { return &GetSurfaceSpans()[index]; }
	// the number of row pointers the runs use
	unsigned short		GetRowCount( void ) const {
		if ( GetSolidCount() > 0 ) {
			return GetSolidRun( GetSolidCount() - 1 )->GetEndRow();
		}
		return 0;
	}
	// DamageTool.exe's ?GetNumBoundaryEntries@Slice@BinaryVoxel@@QBEGXZ (0x40B8E0):
	// the surface runs' row count, the same body
	unsigned short		GetNumBoundaryEntries( void ) const { return GetSurfaceCount() > 0 ? GetSurfaceRun( GetSurfaceCount() - 1 )->GetEndRow() : 0; }
	VoxelRow **			GetRows( void ) const { return (VoxelRow **)&GetSurfaceSpans()[GetSurfaceCount()]; }
	VoxelSurfaceRow **	GetSurfaceRows( void ) const { return (VoxelSurfaceRow **)( GetRows() + GetRowCount() ); }

	// the queries' signatures are DamageTool.exe's BinaryVoxel::Slice::IsInside
	// and IsBoundary (0x405BB0, 0x405C90, 0x405D50, 0x405E60)
	bool				GetSpan( unsigned short x, unsigned short y, unsigned short &start, unsigned short &end ) const;
	bool				IsSolid( unsigned short x, unsigned short y ) const;
	bool				GetSurfaceSpan( unsigned short x, unsigned short y, unsigned short &start, unsigned short &end, const VoxelBSPNode *&cell ) const;
	bool				GetSurfaceCell( unsigned short x, unsigned short y, const VoxelBSPNode *&cell ) const;

	// each returns the slice that follows this one in the block
	VoxelSlice *		ToPointers( char *rows, char *surfaceRows, VoxelBSPNode *nodes );
	VoxelSlice *		ToOffsets( char *rows, char *surfaceRows, VoxelBSPNode *nodes );
};

// the slice of one plane along z; NULL (-1 in the file) when it is empty
struct VoxelSliceRef {
	VoxelSlice *		slice;

	void				ToPointer( char *slices );
	void				ToOffset( char *slices );
};

// FUNCTION: STBC 0x004a7b40 ??_GNiBinaryVoxelData@@UAEPAXI@Z
class NiBinaryVoxelData : public NiObject {
public:
	// FUNCTION: STBC 0x004ac350 ?GetRTTI@NiBinaryVoxelData@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );

	virtual					~NiBinaryVoxelData();

							NiBinaryVoxelData();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

	// the number of cells in the grid: one fewer than the grid points on
	// each axis
	int						GetCellCount() const { return ( sizeZ - 1 ) * ( sizeX - 1 ) * ( sizeY - 1 ); }

	// The block's pointers as offsets from the start of their parts, for
	// saving, and back.
	void					PointersToOffsets();
	void					OffsetsToPointers();

	// the tree of a surface cell; the same static object every call
	VoxelBSPTree *			GetTree( const VoxelBSPNode *cell );

	// The cell's bit in each bit array; false outside the grid. The cells are
	// numbered from 1 on each axis (invented names).
	bool					TestCellBit( unsigned short x, unsigned short y, unsigned short z );
	// the edges of the cell at x, y, z that the model's surface crosses, a
	// bit per edge from the bit arrays (invented name)
	unsigned int			Unknown4A9190( unsigned short x, unsigned short y, unsigned short z );
	bool					TestUnknown4CBit( int array, unsigned short x, unsigned short y, unsigned short z );
	bool					TestUnknown58Bit( int array, unsigned short x, unsigned short y, unsigned short z );
	// clear the bit of the cell; nothing outside the grid (invented names)
	void					ClearCellBit( unsigned short x, unsigned short y, unsigned short z );
	void					ClearUnknown4CBit( int array, unsigned short x, unsigned short y, unsigned short z );
	void					ClearUnknown58Bit( int array, unsigned short x, unsigned short y, unsigned short z );

	// fills the bit arrays from the block (invented name)
	void					BuildBits();

	// queries of the cell at x, y, z (invented names; the signatures are
	// DamageTool.exe's BinaryVoxel::VoxelIsInside and VoxelIsBoundary,
	// 0x4048F0 to 0x404990)
	bool					IsSolid( unsigned short x, unsigned short y, unsigned short z ) const;
	bool					GetSpan( unsigned short x, unsigned short y, unsigned short z, unsigned short &start, unsigned short &end ) const;
	bool					GetSurfaceCell( unsigned short x, unsigned short y, unsigned short z, const VoxelBSPNode *&cell ) const;
	bool					GetSurfaceSpan( unsigned short x, unsigned short y, unsigned short z, unsigned short &start, unsigned short &end, const VoxelBSPNode *&cell ) const;

	// line geometry of the data, for looking at it (the first three are
	// DamageTool.exe's GetInsideVisualRep, GetBoundaryVisualRep and
	// GetVoxelGridVisualRep, 0x4049D0, 0x404BD0 and 0x404DC0)
	NiAVObject *			CreateSolidLines() const;
	NiAVObject *			CreateSurfaceLines() const;
	NiAVObject *			CreateGridLines() const;
	NiNode *				CreateEdgeLines();

	unsigned short			sizeX;			// 0x08 grid points along each axis
	unsigned short			sizeY;			// 0x0A
	unsigned short			sizeZ;			// 0x0C
	float					unknown10;		// 0x10 the voxelizer's first member
	NiPoint3				unknown14;		// 0x14
	NiPoint3				unknown20;		// 0x20
	unsigned int			planeCount;		// 0x2C
	NiPlane *				planes;			// 0x30
	VoxelBSPNode *			nodes;			// 0x34 into the block: the nodes of the surface cells' trees
	char *					rows;			// 0x38 into the block: the solid rows, after the slices
	char *					surfaceRows;	// 0x3C into the block: the surface rows
	VoxelSlice *			slices;			// 0x40 into the block: the slices, one after another
	VoxelSliceRef *			sliceRefs;		// 0x44 into the block: one per grid plane along z
	unsigned char *			cellBits;		// 0x48 one bit per cell
	unsigned char *			unknown4C[3];	// 0x4C one bit per cell each
	unsigned char *			unknown58[3];	// 0x58 one bit per cell each
	char *					block;			// 0x64
	unsigned int			blockSize;		// 0x68
};

/*
================
NiBinaryVoxelData::TestCellBit

The cell's bit in cellBits; false outside the grid. The cells are
numbered from 1 on each axis.
================
*/
// FUNCTION: STBC 0x004ac360
inline bool NiBinaryVoxelData::TestCellBit( unsigned short x, unsigned short y, unsigned short z ) {
	if ( x > 0 && y > 0 && z > 0 && x < sizeX && y < sizeY && z < sizeZ ) {
		unsigned int index = ( ( z - 1 ) * ( sizeY - 1 ) + y - 1 ) * ( sizeX - 1 ) + x - 1;
		unsigned int byteIndex = index >> 3;
		unsigned char mask = 1 << ( index & 7 );
		return ( cellBits[byteIndex] & mask ) != 0;
	}
	return false;
}

/*
================
NiBinaryVoxelData::TestUnknown4CBit
================
*/
// FUNCTION: STBC 0x004ac3f0
inline bool NiBinaryVoxelData::TestUnknown4CBit( int array, unsigned short x, unsigned short y, unsigned short z ) {
	if ( x > 0 && y > 0 && z > 0 && x < sizeX && y < sizeY && z < sizeZ ) {
		unsigned int index = ( ( z - 1 ) * ( sizeY - 1 ) + y - 1 ) * ( sizeX - 1 ) + x - 1;
		unsigned int byteIndex = index >> 3;
		unsigned char mask = 1 << ( index & 7 );
		return ( unknown4C[array][byteIndex] & mask ) != 0;
	}
	return false;
}

/*
================
NiBinaryVoxelData::TestUnknown58Bit
================
*/
// FUNCTION: STBC 0x004ac490
inline bool NiBinaryVoxelData::TestUnknown58Bit( int array, unsigned short x, unsigned short y, unsigned short z ) {
	if ( x > 0 && y > 0 && z > 0 && x < sizeX && y < sizeY && z < sizeZ ) {
		unsigned int index = ( ( z - 1 ) * ( sizeY - 1 ) + y - 1 ) * ( sizeX - 1 ) + x - 1;
		unsigned int byteIndex = index >> 3;
		unsigned char mask = 1 << ( index & 7 );
		return ( unknown58[array][byteIndex] & mask ) != 0;
	}
	return false;
}

/*
================
NiBinaryVoxelData::ClearCellBit

Only BuildBits uses it, expanded.
================
*/
inline void NiBinaryVoxelData::ClearCellBit( unsigned short x, unsigned short y, unsigned short z ) {
	if ( x > 0 && y > 0 && z > 0 && x < sizeX && y < sizeY && z < sizeZ ) {
		unsigned int index = ( ( z - 1 ) * ( sizeY - 1 ) + y - 1 ) * ( sizeX - 1 ) + x - 1;
		unsigned int byteIndex = index >> 3;
		unsigned char mask = 1 << ( index & 7 );
		cellBits[byteIndex] &= ~mask;
	}
}

/*
================
NiBinaryVoxelData::ClearUnknown4CBit

Only BuildBits uses it, expanded.
================
*/
inline void NiBinaryVoxelData::ClearUnknown4CBit( int array, unsigned short x, unsigned short y, unsigned short z ) {
	if ( x > 0 && y > 0 && z > 0 && x < sizeX && y < sizeY && z < sizeZ ) {
		unsigned int index = ( ( z - 1 ) * ( sizeY - 1 ) + y - 1 ) * ( sizeX - 1 ) + x - 1;
		unsigned int byteIndex = index >> 3;
		unsigned char mask = 1 << ( index & 7 );
		unknown4C[array][byteIndex] &= ~mask;
	}
}

/*
================
NiBinaryVoxelData::ClearUnknown58Bit

Only BuildBits uses it, expanded.
================
*/
inline void NiBinaryVoxelData::ClearUnknown58Bit( int array, unsigned short x, unsigned short y, unsigned short z ) {
	if ( x > 0 && y > 0 && z > 0 && x < sizeX && y < sizeY && z < sizeZ ) {
		unsigned int index = ( ( z - 1 ) * ( sizeY - 1 ) + y - 1 ) * ( sizeX - 1 ) + x - 1;
		unsigned int byteIndex = index >> 3;
		unsigned char mask = 1 << ( index & 7 );
		unknown58[array][byteIndex] &= ~mask;
	}
}

NiRegisterStream( NiBinaryVoxelData );

#endif /* !__NIBINARYVOXELDATA_H__ */
