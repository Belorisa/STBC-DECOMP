/*
===============================================================================

	Voxelizer.h

	Builds a model's NiBinaryVoxelData: slices the model's triangles along
	z, one grid plane per step, and gathers the runs of solid and surface
	cells of each row (InsideData, BoundaryData) and the rows of each plane
	(SliceData), then writes them into the data's block.

	The names are from DamageTool.exe (the BC SDK's damage tool, built with
	RTTI and a COFF symbol table), an earlier build of the same code: its
	RTTI names BinaryVoxelizer::InsideData and BinaryVoxelizer::BoundaryData
	: InsideData, whose two-slot vtables (the deleting destructor, GetSize)
	are the rows' here, and its symbols name the rest of the voxelizer in
	the order retail keeps them, with the same instructions: InsideData's
	constructor, destructor, Refer, Unrefer, operator== and AddRange
	(0x406C80-0x406E20) against 0x4AD320-0x4AD4A0, InsideChunk's,
	BspChunk's, VoxelBspRoot's, SliceChunk's and ClipPoly's members, the
	SliceData counters (0x4080C0, 0x4080D0) and BinaryVoxelizer's
	constructor (0x408BF0). Retail's debug log is "VoxelizerDebug.txt"
	(0x88C5CC), or "<model>_VoxelizerDebug.txt" (0x88C5B8) beside the model
	file. The method names and VoxelizerCellBlock are invented.

	A BinaryVoxelizer is 0x5C bytes, built by 0x4AE900 and destroyed by
	0x4AE940; the model viewer's voxelizing routine (0x450580) creates one,
	steps it until it is done, and saves its data as NiBinaryVoxelExtraData.

===============================================================================
*/

#ifndef __VOXELIZER_H__
#define __VOXELIZER_H__

#include <stdio.h>
#include <string.h>

#include "NiBinaryVoxelData.h"
#include "NiPointer.h"
#include "NiPoint3.h"
#include "OBBModel.h"

class NiAVObject;
class NiNode;
class NiPlane;

struct VoxelizerCellBlock;

/*
===============================================================================

	BinaryVoxelizer

===============================================================================
*/

class BinaryVoxelizer {
public:
	// One row's runs of cells along x, gathered while a plane is sliced. Rows
	// that come out equal are shared between slices, so they are counted.
	// FUNCTION: STBC 0x004ad370 ??_GInsideData@BinaryVoxelizer@@UAEPAXI@Z
	class InsideData {
	public:
		// eight runs of a row being built, as start and end pairs, and the
		// next eight
		// FUNCTION: STBC 0x004b1180 ??_GInsideChunk@InsideData@BinaryVoxelizer@@QAEPAXI@Z
		struct InsideChunk {
			unsigned short		values[16];
			InsideChunk *		next;

								InsideChunk() { next = NULL; values[0] = 0xFFFF; }
								~InsideChunk() { delete next; }
		};

							InsideData();
		virtual				~InsideData();

		// the bytes the row takes in the data's block
		// FUNCTION: STBC 0x004b1120 ?GetSize@InsideData@BinaryVoxelizer@@UBEIXZ
		virtual unsigned int	GetSize() const { return GetSize( count ); }

		void				AddRef();
		void				Release();

		// whether the rows hold the same runs (DamageTool.exe's operator==,
		// 0x406D70)
		bool				operator==( const InsideData &row );
		// appends the run from start up to end
		void				Add( unsigned short start, unsigned short end );
		// the bytes a VoxelRow of count runs takes
		unsigned int		GetSize( int spanCount ) const;
		// writes the row as a VoxelRow at offset in the block, and moves offset past it
		void				Write( char *block, int &offset );

		int					count;			// 0x04 runs
		InsideChunk *		head;			// 0x08 owned
		InsideChunk *		tail;			// 0x0C the block being filled
		bool				written;		// 0x10 Write has placed it
		int					offset;			// 0x14 where Write placed it; -1 before
		int					refs;			// 0x18
	};

	// A triangle of the model, or a piece of one, while a cell's tree is
	// built: the plane it lies in and its corners. Arrays of them are built
	// and destroyed through the vector iterators, so the constructor and
	// destructor have out-of-line copies.
	// FUNCTION: STBC 0x004af440 ??_EClipPoly@BinaryVoxelizer@@QAEPAXI@Z
	struct ClipPoly {
		int					plane;			// 0x00 index into the voxelizer's planes
		int					count;			// 0x04 corners
		int					capacity;		// 0x08 corners the array holds
		float				minX;			// 0x0C the least x of the corners
		float				maxX;			// 0x10 the greatest
		NiPoint3 *			corners;		// 0x14 owned

							// FUNCTION: STBC 0x004b10b0 ??0ClipPoly@BinaryVoxelizer@@QAE@XZ
							ClipPoly() { count = 0; capacity = 9; corners = new NiPoint3[9]; }
							// FUNCTION: STBC 0x004b10e0 ??1ClipPoly@BinaryVoxelizer@@QAE@XZ
							~ClipPoly() { delete [] corners; }

		// qsort's order for polygons: by their least x
		static int			CompareMinX( const void *a, const void *b );
	};

	// A node of a surface cell's tree while it is built: the plane that
	// splits the cell's triangles and the subtrees on either side. Write
	// turns it into the data's VoxelBSPNode, with the subtrees as indices.
	// FUNCTION: STBC 0x004ad9d0 ??1VoxelBspRoot@BinaryVoxelizer@@QAE@XZ
	struct VoxelBspNode {
		int					plane;			// 0x00 index into the voxelizer's planes
		int					frontIndex;		// 0x04 the front subtree's index once numbered; 0 for none
		VoxelBspNode *		front;			// 0x08 owned
		int					backIndex;		// 0x0C
		VoxelBspNode *		back;			// 0x10 owned

							VoxelBspNode() { back = NULL; front = NULL; }
							// FUNCTION: STBC 0x004b1130 ??1VoxelBspNode@BinaryVoxelizer@@QAE@XZ
							~VoxelBspNode() { delete front; delete back; }

		// stores the node as a VoxelBSPNode
		void				Write( VoxelBSPNode *node, NiPlane *planes_ );
		// numbers the subtrees from next on, children before their parents,
		// and writes them into nodes
		void				Number( VoxelBSPNode *nodes, int &next, NiPlane *planes_ );
		// the nodes of the subtree, this one included
		int					CountNodes();
		// whether the point is behind the planes down to an empty side
		bool				IsInside( const NiPoint3 &point, NiPlane *planes );
		// whether some part of the segment is behind the planes down to an
		// empty side (invented name)
		bool				IsSegmentInside( const NiPoint3 &start, const NiPoint3 &end, NiPlane *planes );
		// splits a polygon by the node's plane into the corners in front and
		// behind; front and back hold count + 1 corners at least
		void				Split( NiPoint3 *corners, unsigned short count, NiPoint3 *front, unsigned short &frontCount,
								   NiPoint3 *back, unsigned short &backCount, NiPlane *planes );
	};

	// a cell's tree, with the cell's center
	struct VoxelBspRoot : public VoxelBspNode {
		NiPoint3			point;			// 0x14
	};

	// The trees of the cells of one surface run, in order.
	struct BoundaryBspsData {
		// eight trees of a run being built, and the next eight
		struct BspChunk {
			VoxelBspRoot *		trees[8];		// 0x00 owned
			BspChunk *			next;			// 0x20 owned

								BspChunk();
								~BspChunk();
		};

		int					count;			// 0x00 cells
		int					offset;			// 0x04 the first root's index once written; -1 before
		int					unknown08;		// 0x08
		BspChunk *			head;			// 0x0C owned
		BspChunk *			tail;			// 0x10 the block being filled

							BoundaryBspsData();
							~BoundaryBspsData() { delete head; }

		void				Add( VoxelBspRoot *tree );
		// the nodes below the roots
		int					CountNodes();
		// the bytes the trees take in the data's nodes
		int					GetNodeSize() { int nodes = count; nodes += CountNodes(); return nodes * sizeof( VoxelBSPNode ); }
		// writes the roots from next on and their subtrees from nextChild on
		void				Write( VoxelBSPNode *nodes, int &next, int &nextChild, NiPlane *planes_ );
	};

	// One row's runs of surface cells, with the trees of each run's cells.
	// FUNCTION: STBC 0x004b11b0 ??_GBoundaryData@BinaryVoxelizer@@UAEPAXI@Z
	class BoundaryData : public InsideData {
	public:
							BoundaryData();
		virtual				~BoundaryData();

		// the bytes the row takes in the data's block
		// FUNCTION: STBC 0x004b1170 ?GetSize@BoundaryData@BinaryVoxelizer@@UBEIXZ
		virtual unsigned int	GetSize() const { return count * sizeof( VoxelSurfaceSpan ) + sizeof( int ); }

		// appends the run from start up to end with the trees of its cells
		void				Add( unsigned short start, unsigned short end, BoundaryBspsData *cells );
		// the bytes the trees of all runs take in the data's nodes
		int					GetNodeSize();
		// writes the trees of all runs, the roots of each run from next on
		void				WriteNodes( VoxelBSPNode *nodes, int &next, NiPlane *planes_ );
		// writes the row as a VoxelSurfaceRow at offset in the block, and
		// moves offset past it
		void				Write( char *block, int &offset );

		VoxelizerCellBlock *	cellHead;	// 0x1C owned
		VoxelizerCellBlock *	cellTail;	// 0x20 the block being filled
	};

	// One grid plane's rows along y, gathered while the plane is sliced: the
	// rows of solid cells and the rows of surface cells. Planes that come out
	// equal are shared, so they are counted.
	class SliceData {
	public:
		// 32 rows of a slice being built, with the y of each, and the next 32
		struct SliceChunk {
			unsigned short		ys[32];			// 0x00
			InsideData *		rows[32];		// 0x40 counted
			SliceChunk *		next;			// 0xC0 owned

								SliceChunk();
								~SliceChunk();
		};

							SliceData();
							~SliceData();

		void				AddRef();
		void				Release();

		void				AddSolidRow( unsigned short y, InsideData *row );
		void				AddSurfaceRow( unsigned short y, InsideData *row );

		// the written rows' offsets in the data's block, in order; returns
		// their count
		int					GetSolidRowOffsets( int *offsets ) const;
		int					GetSurfaceRowOffsets( int *offsets ) const;
		// the runs of consecutive y among count rows from block on
		int					CountRuns( const SliceChunk *block, int count ) const;
		int					CountSolidRuns() const;
		int					CountSurfaceRuns() const;
		// fills runs with the runs of consecutive y; returns their count
		// (DamageTool.exe's CollectInsideSliceRanges and
		// CollectBoundarySliceRanges take the slice's own runs,
		// BinaryVoxel::Slice::Range *)
		int					GetSolidRuns( VoxelSliceSpan *runs ) const;
		int					GetSurfaceRuns( VoxelSliceSpan *runs ) const;

		// writes the trees of every surface row
		void				WriteSurfaceNodes( VoxelBSPNode *nodes, int &next, NiPlane *planes_ );
		// the bytes of the solid rows not counted yet, which are then marked
		unsigned int		CountSolidRowSize() const;
		// the same for the surface rows, with the bytes of their trees
		void				CountSurfaceRowSize( unsigned int &rowSize, unsigned int &nodeSize ) const;
		// clears the marks of count rows from block on
		void				ClearWritten( const SliceChunk *block, int count ) const;
		void				ClearWritten() const;
		// the bytes the slice takes in the data's block
		unsigned int		GetSize() const;
		// writes the rows not written yet at offset in the block
		void				WriteSolidRows( char *block, int &offset_ );
		void				WriteSurfaceRows( char *block, int &offset_ );
		// writes the slice's runs and row offsets; returns the bytes written
		unsigned int		Write( VoxelSlice *slice ) const;

		bool				written;			// 0x00
		int					refs;				// 0x04
		int					offset;				// 0x08 where it was written; -1 before
		int					solidCount;			// 0x0C
		SliceChunk *		solidTail;			// 0x10 the block being filled
		SliceChunk *		solidHead;			// 0x14 owned
		int					surfaceCount;		// 0x18
		SliceChunk *		surfaceTail;		// 0x1C the block being filled
		SliceChunk *		surfaceHead;		// 0x20 owned
	};

						BinaryVoxelizer();
						~BinaryVoxelizer();

	// releases the model and everything built from it, and goes back to a
	// cell size of 1 (invented name)
	void				Reset();

	// detaches the model from its parent, builds its OBBModel and fits the
	// grid around it: cells of cellSize, centered on the model's bound
	// (invented name)
	void				Begin();

	// slices the next row of cells along x, beginning with Begin when
	// nothing is under way; progress is the share of the rows done. False
	// once the last row is done and BuildData has built the data
	// (invented name)
	bool				Step( float *progress );

	// the model to voxelize, its OBBModel and the grid step, and the data
	// Step builds, as the LOD loader (0x450580) uses them; retail calls
	// the smart pointers' assignments out of line there (invented names);
	// the model's is NiPointer<NiNode>'s (0x416FA0), so the model is a node
	void				SetOBBModel( OBBModel *obbModel_ ) { obbModel = obbModel_; }
	void				SetModel( NiNode *model_ ) { model = model_; }
	void				SetCellSize( float cellSize_ ) { cellSize = cellSize_; }
	NiBinaryVoxelData *	GetVoxelData() const { return voxelData; }

	// the model file the debug log is named after (0x4AC910)
	void				SetFileName( char *name );

	// the debug log, opened on first use
	static FILE *		GetDebugFile();
	static void			CloseDebugFile();

protected:
	static char *		GetDebugFileName();

	// grows the bound around the world vertices of every shape under
	// object (invented name)
	void				ExpandBound( NiAVObject *object );
	// the triangles of every shape under object (invented name)
	int					CountTriangles( NiAVObject *object );
	// builds node's subtree from count polygons (invented name)
	void				BuildTree( VoxelBspNode *node, ClipPoly *polygons, int count, NiPlane *planes_ );
	// the index of a plane equal to plane, which is added when there is
	// none (invented name)
	int					AddPlane( const NiPlane &plane );
	// the index of the plane facing the other way in the same place, or -1
	// (invented name)
	int					FindOppositePlane( const NiPlane &plane );
	// the bytes the data's block takes: the slices, the solid rows, the
	// surface rows and the nodes, after a reference per grid plane
	// (invented name)
	int					GetDataSize( unsigned int &sliceSize, unsigned int &solidRowSize, unsigned int &surfaceRowSize ) const;
	// builds the NiBinaryVoxelData from the slices, gives the model back to
	// its parent and finishes (invented name)
	void				BuildData();

	// a surface cell's tree from its polygons (invented name)
	VoxelBspRoot *		BuildCellTree( ClipPoly *polygons, int count, const NiPoint3 &point );

	float				cellSize;			// 0x00 the grid step
	float				cellDiagonal;		// 0x04 sqrt( 3 ) times the step
	NiPoint3			boundMin;			// 0x08 the grid's corners, model space
	NiPoint3			boundMax;			// 0x14
	int					cellsX;				// 0x20 the grid's size in cells
	int					cellsY;				// 0x24
	int					cellsZ;				// 0x28
	NiPointer<OBBModel>	obbModel;			// 0x2C the model's shapes, for the line tests
	NiPointer<NiNode>	model;			// 0x30
	int					unknown34;			// 0x34 the step reached: 3 once BuildData is done
	SliceData **		slices;				// 0x38 owned, cellsZ of them
	SliceData *			unknown3C;			// 0x3C the slice of the plane being sliced; NULL until it has a row
	int					unknown40;			// 0x40 the row being sliced: its y
	int					unknown44;			// 0x44 its z
	int					planeCount;			// 0x48
	NiPlane *			planes;				// 0x4C owned, one per triangle
	NiPointer<NiNode>	parent;				// 0x50 the model's parent while it is detached
	NiPointer<NiBinaryVoxelData>	voxelData;	// 0x54 the data BuildData builds
	NiPointer<NiObject>	unknown58;			// 0x58

	static char *		fileName;
	static FILE *		debugFile;
};

// eight cell lists of a surface row being built, and the next eight
// (invented name)
struct VoxelizerCellBlock {
	BinaryVoxelizer::BoundaryBspsData *	lists[8];	// 0x00 owned
	VoxelizerCellBlock *	next;		// 0x20

						VoxelizerCellBlock() { next = NULL; memset( lists, 0, sizeof( lists ) ); }
						// BUG: the next block and its lists are never deleted
						~VoxelizerCellBlock() { for ( int i = 0; i < 8; i++ ) { delete lists[i]; } }
};

/*
================
BinaryVoxelizer::BoundaryData::BoundaryData

starts the row with an empty block of cell lists
================
*/
inline BinaryVoxelizer::BoundaryData::BoundaryData() {
	cellTail = new VoxelizerCellBlock;
	cellHead = cellTail;
}

#endif /* !__VOXELIZER_H__ */
