/*
===============================================================================

	DamageVoxelGrid.h

	A box of voxel cells, one bit each, for MetaVolumePolygonizer. Its own
	header: DamageVoxelGrid.cpp registers only the NetImmerse classes its
	debug lines use, so it can't see DamagePolygonizer.h's includes (the
	file name is invented).

===============================================================================
*/

#ifndef __DAMAGEVOXELGRID_H__
#define __DAMAGEVOXELGRID_H__

#include "DamageBitArray.h"
#include "NiPoint3.h"

class NiAVObject;
class NiColor;
class NiNode;

/*
================
DamageVoxelGrid

A box of cells of the polygonizer's grid, a bit per cell of the whole grid
(invented name). Evidence: 0x40 bytes, built by 0x4C5A50 from the grid's
size, a source bit block or a count of set cells, the box's lower and upper
cells, the grid's origin and cell size and the debug node; destroyed by
0x4BD790, which only frees the bits. The polygonizer keeps the current one
at 0xA0 and a list of them at 0x120.
================
*/
// FUNCTION: STBC 0x004bd790 ??1DamageVoxelGrid@@QAE@XZ @game/DamagePolygonizer
class DamageVoxelGrid {
public:
						DamageVoxelGrid( const unsigned char *source, const unsigned short *size_, unsigned int setCount_,
										 const unsigned short *lower_, const unsigned short *upper_, const NiPoint3 &origin_,
										 float cellSize_, NiNode *debugNode_ );

	unsigned int		GetCellCount() const { return size[0] * size[1] * size[2]; }
	unsigned int		GetIndex( unsigned short x, unsigned short y, unsigned short z ) const { return ( size[1] * z + y ) * size[0] + x; }
	// FUNCTION: STBC 0x004c4c10 ?IsSet@DamageVoxelGrid@@QBE_NGGG@Z @game/DamagePolygonizer
	bool				IsSet( unsigned short x, unsigned short y, unsigned short z ) const { return cells.IsSet( ( size[1] * z + y ) * size[0] + x ); }
	// clears the cell's bit, counting it out when it was set
	void				Clear( unsigned short x, unsigned short y, unsigned short z ) {
							if ( cells.Clear( ( size[1] * z + y ) * size[0] + x ) ) {
								setCount--;
							}
						}
	// the lines of the grid's box, in color, for the debug node (invented
	// name)
	NiAVObject *		CreateLines( const NiColor *color );
	// a new grid of the cells set in both, over this grid's size and box
	// corner; NULL when they share none (invented name)
	DamageVoxelGrid *	Intersect( DamageVoxelGrid *other );
	// whether a cell is set in both (invented name)
	bool				Overlaps( DamageVoxelGrid *other );
	// a new grid of the cells set in only one of them; NULL when none
	// (invented name)
	DamageVoxelGrid *	Difference( DamageVoxelGrid *other );
	// the box's corners in the model's frame (invented name)
	void				GetBox( NiPoint3 &boxMin, NiPoint3 &boxMax ) const {
							boxMin = origin + NiPoint3( lower[0] * cellSize, lower[1] * cellSize, lower[2] * cellSize );
							boxMax = origin + NiPoint3( upper[0] * cellSize, upper[1] * cellSize, upper[2] * cellSize );
						}
	// the node the grid draws its lines under (invented: name and helper;
	// MetaVolumePolygonizer::CreateDebugNode's inline budget counts the site)
	void				SetDebugNode( NiNode *debugNode_ ) { debugNode = debugNode_; }
	bool				IsInBox( unsigned short x, unsigned short y, unsigned short z ) const {
							return x >= lower[0] && x < upper[0] && y >= lower[1] && y < upper[1] && z >= lower[2] && z < upper[2];
						}

	unsigned short		size[3];			// 0x00 the whole grid's cells along each axis
	unsigned short		lower[3];			// 0x06 the box's first cell along each axis
	unsigned short		upper[3];			// 0x0C past the box's last cell
	NiPoint3			origin;				// 0x14 the grid's corner
	float				cellSize;			// 0x20
	NiNode *			debugNode;			// 0x24
	NiPoint3			center;				// 0x28 the box's center
	DamageBitArray		cells;				// 0x34 a bit per cell of the whole grid
	unsigned int		setCount;			// 0x3C the cells set
};

#endif /* !__DAMAGEVOXELGRID_H__ */
