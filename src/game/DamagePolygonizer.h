/*
===============================================================================

	DamagePolygonizer.h

	The part of a VisibleDamage (at 0x1C) that turns the damage volumes
	into triangles.

	0x13C bytes, no vtable. Evidence: the constructor (0x4BD2D0) clears a
	smart pointer (0x2C), builds a list with two items per block (0x34),
	an array of smart pointers (0x4C, vtable 0x88C760), three 37-bucket
	maps keyed by 64-bit values (0x68, 0x78 and 0x88, vtables 0x88C74C,
	0x88C738 and 0x88C724), two more smart pointers (0x98, 0x9C), a
	list with 50 items per block (0xAC), a list with two (0x120, through
	its out-of-line constructor 0x4C41F0) and a last smart pointer
	(0x138), then clears 0x08, 0x09, 0x30, 0xA0, 0xC4 and 0x108. The
	destructor (0x4BD4D0) deletes the values of the map at 0x88, frees
	0x108 and the block at 0xA0, calls 0x4BD710 and destroys the members.
	The class name is from DamageTool.exe (the BC SDK's damage tool, built
	with RTTI and a COFF symbol table), an earlier build of the same class:
	its ??0MetaVolumePolygonizer (0x413BE0) builds the same three 37-bucket
	maps keyed by __int64, whose RTTI names their values (TriIndex, three
	dwords, as the map at 0x68 here; VertexInfo, one dword, as 0x78;
	IndexChunk *, as 0x88), and the array of NiPointer<MetaVolume>;
	Damage's constructor news it where VisibleDamage's news this class
	(0x4B133F), and its AddVolume (0x414160), CollectFeatures (0x414A10) and
	two Polygonize overloads (0x415660, 0x415D80) have the instructions of
	the methods of those names here (0x4BD9F0, 0x4BE640, 0x4BF700,
	0x4BFE80), which take their names from them; Refresh is its only public
	entry point without parameters, as 0x4BEFA0 here. Its layout differs (an
	array of VisitedVolume *, no list at 0x34), so the member names stay
	placeholders.

===============================================================================
*/

#ifndef __DAMAGEPOLYGONIZER_H__
#define __DAMAGEPOLYGONIZER_H__

#include "DamageBitArray.h"
#include "MetaVolume.h"
#include "DamageMesh.h"
#include "NiBinaryVoxelExtraData.h"
#include "NiNode.h"
#include "NiObject.h"
#include "NiPoint3.h"
#include "NiPointer.h"
#include "NiTArray.h"
#include "NiTList.h"
#include "NiTMap.h"
#include "NiTriShape.h"
#include "NiColor.h"
#include "DamageVoxelGrid.h"

#include <math.h>
#include <string.h>


/*
================
Voxel

One cell's eight corners and their weights, as the marching works on it.
The name is DamageTool.exe's: MetaVolumePolygonizer's UpdateVoxel and its
two Polygonize overloads (0x4BDC30, 0x4BF700 and 0x4BFE80) take a Voxel &
(a class). Evidence: the 0x150-byte local of Unknown4BE1B0 that
UpdateVoxel fills: the corners (0x00) of the cell (0x148), lower layer
first, their weights (0x60) and, with the second kind of damage, the
cell's key (0x140). 0x80-0x13F are filled by 0x4BF700 and 0x4BFE80.
================
*/
class Voxel {
public:
	NiPoint3			corners[8];			// 0x00 0-3 at z, 4-7 at z + 1
	float				weights[8];			// 0x60
	NiPoint3			unknown80[12];		// 0x80 TODO identity (filled by 0x4BF700)
	int					edgeVertices[12];	// 0x110 the mesh vertex of each edge's crossing, as the edge table numbers them
	__int64				key;				// 0x140 x << 32 | y << 16 | z
	unsigned short		x;					// 0x148
	unsigned short		y;					// 0x14A
	unsigned short		z;					// 0x14C
};

// a cell of the grid (invented name); lists hold it by value
struct DamageCell {
	unsigned short		x;
	unsigned short		y;
	unsigned short		z;

						DamageCell() {}
						// NiTList::DeleteItem stores 0 into the freed item: retail copies an unwritten
						// DamageCell over it, so the conversion leaves the cell as it is
						DamageCell( int ) {}
};

/*
================
DamageVertexIndices

Up to six vertex indices of a damage key, 0xFFFF past the last (invented
name). Evidence: the polygonizer's map at 0x68 holds them as values: its
SetValue (0x4C3D60) copies three dwords, and CollectFeatures reads them as
unsigned shorts up to the first 0xFFFF and writes the vertices' new
indices back over them.
================
*/
struct DamageVertexIndices {
	unsigned short		indices[6];			// 0x00
};

/*
================
DamageIndexBlock

Twelve vertex indices of a damage key, 0xFFFF past the last, and the next
twelve (invented name). Evidence: the polygonizer's map at 0x88 holds them
as values (0x1C bytes, the link at 0x18); its destructor deletes them.
================
*/
// FUNCTION: STBC 0x004c4de0 ??_GDamageIndexBlock@@QAEPAXI@Z @game/DamagePolygonizer
struct DamageIndexBlock {
	unsigned short		indices[12];		// 0x00
	DamageIndexBlock *	next;				// 0x18 owned

						DamageIndexBlock() { next = NULL; }
	// FUNCTION: STBC 0x004c4d50 ??1DamageIndexBlock@@QAE@XZ @game/DamagePolygonizer
						~DamageIndexBlock() { delete next; }
};

class MetaVolumePolygonizer {
public:
						MetaVolumePolygonizer();
						~MetaVolumePolygonizer();

	// Tests a point in the model's frame; with nothing built, the result is
	// the flag's opposite.
	bool				Unknown4C2AF0( const NiPoint3 *point, bool );

	// sets both counters (invented name)
	void				SetUnknown00( float value ) { unknown00 = value; unknown04 = value; }

	// hangs a new debug node under parent, drawn with property, with the
	// texture mode, vertex colour and white emissive material the grid lines
	// need (invented name)
	void				CreateDebugNode( NiNode *parent, NiProperty *property );

	// the grid point's bit in unknown108 (invented name)
	bool				IsPointSet( unsigned short x, unsigned short y, unsigned short z ) const {
							unsigned int index = ( z * ( size[1] + 1 ) + y ) * ( size[0] + 1 ) + x;
							return ( unknown108[index >> 3] & (unsigned char)( 1 << ( index & 7 ) ) ) != 0;
						}
	// clears the grid point's bit in unknown108 (invented name)
	void				ClearPoint( unsigned short x, unsigned short y, unsigned short z ) {
							unsigned int index = ( ( size[1] + 1 ) * z + y ) * ( size[0] + 1 ) + x;
							unknown108[index >> 3] &= ~( 1 << ( index & 7 ) );
						}
	// the edges of the cell, set in the current grid, that both the voxel
	// data and the corners' pattern give; 0 otherwise (invented name)
	unsigned int		Unknown4C0650( unsigned short x, unsigned short y, unsigned short z );
	// the bits of the cell's eight corners in unknown108, x fastest (invented
	// name)
	int					GetCornerBits( unsigned short x, unsigned short y, unsigned short z );

	// whether a cell lies inside the voxel data's grid (invented)
	bool				IsInGrid( int x, int y, int z ) const {
							return x >= 0 && x < size[0] && y >= 0 && y < size[1] && z >= 0 && z < size[2];
						}
	// the cell's bit in the cell bit arrays
	unsigned int		GetCellIndex( unsigned short x, unsigned short y, unsigned short z ) const { return ( size[1] * z + y ) * size[0] + x; }
	// sets the cell's bit in unknownE4, counts it and grows the box of the
	// cells set around it (invented name)
	// FUNCTION: STBC 0x004c4c70 ?Unknown4C4C70@MetaVolumePolygonizer@@QAEXGGG@Z @game/DamagePolygonizer
	void				Unknown4C4C70( unsigned short x, unsigned short y, unsigned short z ) {
							unsigned int index = ( size[1] * z + y ) * size[0] + x;
							unsigned int byteIndex = index >> 3;
							unknownEC++;
							unknownE4[byteIndex] |= 1 << ( index & 7 );
							if ( x < unknownF0[0] ) {
								unknownF0[0] = x;
							} else if ( x > unknownF6[0] ) {
								unknownF6[0] = x;
							}
							if ( y < unknownF0[1] ) {
								unknownF0[1] = y;
							} else if ( y > unknownF6[1] ) {
								unknownF6[1] = y;
							}
							if ( z < unknownF0[2] ) {
								unknownF0[2] = z;
							} else if ( z > unknownF6[2] ) {
								unknownF6[2] = z;
							}
						}
	// clears the cell's bit in unknownC8, counting it out when it was set
	// (invented name)
	// FUNCTION: STBC 0x004c4d70 ?Unknown4C4D70@MetaVolumePolygonizer@@QAEXGGG@Z @game/DamagePolygonizer
	void				Unknown4C4D70( unsigned short x, unsigned short y, unsigned short z ) {
							if ( unknownC8.Clear( ( size[1] * z + y ) * size[0] + x ) ) {
								unknownC4--;
							}
						}
	// a run of the grid's cells along x through the cell (invented name)
	void				Unknown4C2550( unsigned short x, unsigned short y, unsigned short z, unsigned int *start, unsigned int *end );

	// the first cell of the search box whose bit is set in unknownC8, z
	// slowest; the outputs stay as they were when there is none (invented
	// name)
	void				FindSetCell( unsigned short *x, unsigned short *y, unsigned short *z );

	// set the grid points of the other grid's cells in the shared point
	// buffer and keep only those in unknown108 (invented names)
	void				Unknown4C2C40( unsigned short x, unsigned short y, unsigned short z );
	void				Unknown4C2CC0( DamageVoxelGrid *other );

	// a new node at 0x138 that tests and writes the z-buffer (invented name)
	NiNode *			CreateUnknown138( void );

	// takes the model's voxel data and builds a grid over all of it; with
	// clear, the cells that are neither solid nor on the surface are left
	// out (invented name)
	void				SetVoxelData( NiBinaryVoxelExtraData *data, bool clear );
	// the voxel data's corners and cells along each axis (DamageTool.exe
	// 0x415380, ?SetExtents@MetaVolumePolygonizer@@QAEXABVNiPoint3@@0GGG@Z)
	void				SetExtents( const NiPoint3 &min, const NiPoint3 &max, unsigned short sizeX, unsigned short sizeY,
									unsigned short sizeZ );

	// keeps the volume when its box meets the model's (invented name)
	void				AddVolume( MetaVolume *volume );

	// turn the two kinds of damage on; each needs voxel data, the second
	// also the debug node (invented names)
	void				SetUnknown08( bool enable );
	void				SetUnknown09( bool enable );
	// the cell holding a point in the model's frame, clamped to the grid
	// (invented name)
	void				GetCell( const NiPoint3 *point, unsigned short *x, unsigned short *y, unsigned short *z );

	// With the second kind of damage on, marches from origin + t * direction
	// along direction in quarter cells through the current grid, in the
	// debug node's frame: true at the first cell set, false on leaving the
	// grid; true when that kind is off. The two are the callbacks of
	// VisibleDamage's slots 1 and 2 and have the same body. The unused
	// parameters' types are unproven (invented names).
	bool				Unknown4C3680( const NiPoint3 *origin, const NiPoint3 *direction, const NiPoint3 *point, int, const float *t, int, int );
	bool				Unknown4C38C0( const NiPoint3 *origin, const NiPoint3 *direction, const NiPoint3 *point, int, const float *t, int, int );

	// the shared mesh and the marching-cube tables, made at startup and
	// freed at shutdown (invented names)
	static void			Init();
	static void			Shutdown();

	// rebuilds the damage from the volumes on the list: the first kind into
	// the shared mesh, the second as one grid per connected group of the
	// cells set in unknownC8, each drawn under the node at 0x138 in the
	// next of five colours (invented name)
	void				Refresh( void );
	// keeps only the current grid's cells that the other grid has set and
	// narrows the box to the other's; the triangles built in the dropped
	// cells go out of both maps and collapse in the shape (0x4C2F80;
	// VisibleDamage::Unknown4B1B90 calls it; invented name)
	void				Unknown4C2F80( DamageVoxelGrid *other );
	// the steps of Refresh
	void				Unknown4BE1B0( void );
	void				CollectFeatures( void );
	// floods the cells set in unknownC8 from a cell into unknownE4 (invented
	// name): the run of cells along x through a cell is found by
	// Unknown4C2550, and the cells of each neighbouring row that the run's
	// masks name and unknownE4 doesn't hold yet are queued in unknownAC
	void				Unknown4C0830( unsigned short x, unsigned short y, unsigned short z );

	// fills the marching-cube tables (0x98406C, 0x983C68) (invented name)
	static void			BuildTables();

	// build the first kind of damage's triangles for a weighed cell: from
	// the surface cell's tree, or from the corner pattern alone (DamageTool.exe
	// 0x415660 and 0x415D80)
	void				Polygonize( Voxel &cube, VoxelBSPTree *tree );
	void				Polygonize( Voxel &cube );

	// weighs the cell's corners by the volumes near the current one, clears
	// the grid points the damage reaches and marks the cell in unknownC8
	// when the surface crosses it (DamageTool.exe's UpdateVoxel( Voxel &,
	// unsigned short, unsigned short, unsigned short ) at 0x414410, the same
	// parameters)
	void				UpdateVoxel( Voxel &cube, unsigned short x, unsigned short y, unsigned short z );

	// takes the next volume off the list and gathers the volumes near it;
	// false when the list is empty (invented name)
	bool				NextVolume( void );

	// deletes the grids of the list at 0x120 and empties it (invented name)
	void				DeleteGrids();

	// where the surface crosses the edge between two of a cell's corners:
	// the corner it passes through, or the point between them where the
	// weights reach unknown00 (invented name)
	NiPoint3			GetEdgePoint( const Voxel &cube, int from, int to ) const;

protected:
	// An edge vertex of the damage mesh, the value of the map at 0x78. The
	// name is DamageTool.exe's (NiTMap<__int64, MetaVolumePolygonizer::
	// VertexInfo>, SetAt 0x4166A0: the value passed as one dword). Polygonize
	// sets unknown02 to 1 when it places the vertex, CollectFeatures to 0
	// when it renumbers it.
	class VertexInfo {
	public:
		unsigned short	index;				// 0x00 the vertex in the mesh
		unsigned short	unknown02;			// 0x02 1 once placed
	};

	// deletes the index blocks of a cell's clipped triangles and forgets
	// them (DamageTool.exe 0x415480,
	// ?PopVoxelClipedTris@MetaVolumePolygonizer@@IAEX_J@Z, protected; retail
	// keeps no copy: its one caller expands it)
	void				PopVoxelClipedTris( __int64 key ) {
							DamageIndexBlock *chunk;
							if ( unknown88.GetAt( key, chunk ) ) {
								delete chunk;
								unknown88.RemoveAt( key );
							}
						}

	bool				IsCellMarked( unsigned short x, unsigned short y, unsigned short z ) const {
							unsigned int index = ( size[1] * z + y ) * size[0] + x;
							return ( unknownE4[index >> 3] & (unsigned char)( 1 << ( index & 7 ) ) ) != 0;
						}

	friend class VisibleDamage;		// forwards to the polygonizer and reads its members

	float				unknown00;			// 0x00 (VisibleDamage::SetUnknown00 passes a float)
	float				unknown04;			// 0x04 the weight a grid point is cleared above
	bool				unknown08;			// 0x08
	bool				unknown09;			// 0x09
	unsigned short		size[3];			// 0x0A the voxel data's cells along each axis
	float				cellSize;			// 0x10
	NiPoint3			boundMin;			// 0x14 the voxel data's corners, model space
	NiPoint3			boundMax;			// 0x20
	NiPointer<NiBinaryVoxelExtraData>	voxelData;	// 0x2C
	unsigned int		cellCount;			// 0x30
	NiTList<MetaVolume *>	volumes;		// 0x34 the damage volumes whose box meets the model's
	NiTArray< NiPointer<MetaVolume> >	nearVolumes;	// 0x4C the volumes near the volume being worked on
	NiPointer<MetaVolumePool>	volumeList;	// 0x64 the object's damage volumes
	NiTMap<__int64, DamageVertexIndices>	unknown68;	// 0x68 TODO identity: the key
	NiTMap<__int64, VertexInfo>	unknown78;	// 0x78 TODO identity: the key
	NiTMap<__int64, DamageIndexBlock *>	unknown88;	// 0x88 TODO identity: the key; owned values
	NiPointer<NiNode>	debugNode;			// 0x98 the node the grids draw their lines under
	NiPointer<NiTriShape>	unknown9C;		// 0x9C TODO identity: the shape the first kind of damage is built into
	DamageVoxelGrid *	grid;				// 0xA0 owned
	DamageBitArray		unknownA4;			// 0xA4 a bit per cell
	NiTList<DamageCell>	unknownAC;			// 0xAC TODO identity: which cells
	unsigned int		unknownC4;			// 0xC4 the cells set in unknownC8
	DamageBitArray		unknownC8;			// 0xC8
	int					unknownD0;			// 0xD0
	unsigned short		unknownD4;			// 0xD4
	unsigned short		searchLower[3];		// 0xD6 the box FindSetCell searches
	unsigned short		searchUpper[3];		// 0xDC past it
	unsigned char *		unknownE4;			// 0xE4 a bit per cell (a shared static buffer)
	unsigned char *		unknownE8;			// 0xE8 a bit per cell (a shared static buffer)
	unsigned int		unknownEC;			// 0xEC the cells set in unknownE4
	unsigned short		unknownF0[3];		// 0xF0 the lowest cell set in unknownE4
	unsigned short		unknownF6[3];		// 0xF6 the highest
	unsigned short		boxLower[3];		// 0xFC the cells around the volume being worked on
	unsigned short		boxUpper[3];		// 0x102 past them
	unsigned char *		unknown108;			// 0x108 owned: a bit per grid point
	float *				layerWeights[2];	// 0x10C the weights of the box's grid points, a layer of z and the next (shared static blocks)
	unsigned char *		layerWeighed[2];	// 0x114 whether each is weighed yet
	unsigned short		layerSize[2];		// 0x11C the layers' points along x and y
	NiTList<DamageVoxelGrid *>	grids;		// 0x120 owned
	NiPointer<NiNode>	unknown138;			// 0x138 a node with z-buffered properties: the grids' lines hang under it

	static FeatureCollector *	mesh;	// the shared mesh the first kind of damage is built in
	static NiColor		gridColors[5];		// the grids' line colours, in turn
	static int			gridColorIndex;		// the next colour
	static unsigned char *	cellBuffers[2];	// shared bit-per-cell blocks for unknownE4 and unknownE8
	static unsigned int	cellBufferSize;		// their bytes
	static unsigned int	cubeEdges[256];		// the edges each corner pattern's surface crosses (BuildTables)
	static unsigned int	cubeTable983C68[256];	// the second marching-cube table (BuildTables)
	static DamageBitArray *	unknown984504;
	static unsigned int *	unknown984508;	// a mask per cell of a row
	static unsigned short	unknown98450C;	// its entries
	static unsigned char *	unknown984510;
	static unsigned char *	unknown984520;
	static unsigned int	unknown984524;	// its bytes
	static float *		layerWeightBlocks[2];	// the shared blocks layerWeights point into (invented name)
	static unsigned char *	layerWeighedBlocks[2];	// the shared blocks layerWeighed point into (invented name)
	static int			layerBlockSize;		// their points (invented name)
	static unsigned short *	remapBuffer;	// CollectFeatures's scratch: where each triangle, then each vertex, goes (invented name)
	static int			remapBufferSize;	// its bytes
	static unsigned char *	usedBuffer;		// CollectFeatures's scratch: whether each vertex is used (invented name)
	static int			usedBufferSize;		// its bytes
};

/*
================
MetaVolumePolygonizer::SetExtents
================
*/
inline void MetaVolumePolygonizer::SetExtents( const NiPoint3 &min, const NiPoint3 &max, unsigned short sizeX,
											   unsigned short sizeY, unsigned short sizeZ ) {
	boundMin = min;
	boundMax = max;
	size[0] = sizeX;
	size[1] = sizeY;
	size[2] = sizeZ;
}

#endif /* !__DAMAGEPOLYGONIZER_H__ */
