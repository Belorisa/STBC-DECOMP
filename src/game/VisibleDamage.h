/*
===============================================================================

	VisibleDamage.h

	The damage drawn on a DamageableObject's model, and the list of
	damage volumes (MetaVolume) it is drawn with. Both class names are invented: no
	string or SWIG type names them. DamageableObject::RemoveVisibleDamage
	(0x5918F0) deletes the VisibleDamage objects and clears the list.

	Evidence: a VisibleDamage is 0x24 bytes, built by the constructor
	0x4B12E0 (vtable 0x88C5FC, 6 slots) and destroyed by the non-virtual
	destructor 0x4B13A0, which DamageableObject's destructor (0x591620)
	calls before freeing each one. The constructor clears six smart
	pointers (0x04-0x18), then creates the polygonizer (0x1C, 0x13C bytes)
	and the builder (0x20, 0x54 bytes); the destructor clears the
	OBBModel's callback before it deletes them. A MetaVolumePool is a NiObject of
	0x20 bytes (vtable 0x893D28: NiObject's slots, its own destructor
	0x590E40) holding an array of objects at 0x08 (vtable 0x88C760);
	DamageableObject's InitObject (0x590CB0) builds it inline; it is declared
	in MetaVolume.h.

===============================================================================
*/

#ifndef __VISIBLEDAMAGE_H__
#define __VISIBLEDAMAGE_H__

#include "MetaVolume.h"
#include "NiNode.h"
#include "NiProperty.h"
#include "NiTriShape.h"
#include "NiPointer.h"

class MetaVolumePolygonizer;
class OBBModel;
class MetaSurfacePolygonizer;
class DamageVoxelGrid;
struct DamageBitArray;
class NiBinaryVoxelExtraData;
class VisibleDamagePiece;
class MetaVolumePool;

// invented name
class VisibleDamage {
public:
					VisibleDamage();
					~VisibleDamage();

	// The OBBModel's callbacks. Slot 0 forwards to the builder: see
	// MetaSurfacePolygonizer::Unknown4B48C0. The other slots' parameter types
	// are unproven: only their counts are known, from ret N.
	virtual bool	VirtualSlot00( NiTriShape *shape, int triangle, const unsigned short *corner, unsigned short *corner1,
								   unsigned short *corner2, VisibleDamagePiece **piece );
	// Slots 1 and 2: 1 when the piece's volumes are too weak at point,
	// else 2 when the polygonizer's march from origin + t * direction meets
	// a set cell, else 0. The int parameters' types are unproven.
	virtual int		VirtualSlot01( int, int, int, const NiPoint3 *origin, const NiPoint3 *direction, const NiPoint3 *point,
								   const float *t, int, int, VisibleDamagePiece *piece );
	virtual int		VirtualSlot02( int, int, int, const NiPoint3 *origin, const NiPoint3 *direction, const NiPoint3 *point,
								   const float *t, int, int, VisibleDamagePiece *piece );
	// the builder's bits at 0x58 of the shape's piece
	virtual DamageBitArray *	VirtualSlot03( NiTriShape *shape );
	// the builder's bits at 0x70 of the shape's piece
	virtual DamageBitArray *	VirtualSlot04( NiTriShape *shape );
	// the triangles the shape is drawn with (the builder's GetTriList)
	virtual unsigned short *	VirtualSlot05( NiTriShape *shape );

	// True when the polygonizer has voxel data; DamageableObject's slots 82
	// and 83 test with the first damage for which it is.
	bool			Unknown4B1B50( void );
	// Moves a world point into the model's frame and tests it against the
	// polygonizer.
	bool			Unknown4B1940( const NiPoint3 *point, bool );

	// the builder's and the polygonizer's shared data, made at startup and
	// freed at shutdown (invented names)
	static void		Init( void );
	static void		Shutdown( void );

	// a new VisibleDamage of the grid's cells of this one: copies of the
	// properties, a new model, the builder's pieces cut to the grid and the
	// volumes whose box meets the grid's; list receives those volumes
	// (invented name)
	VisibleDamage *	CloneGrid( DamageVoxelGrid *grid, MetaVolumePool *list );
	// the model's OBBModel; DamageableObject::BreakOffPiece calls its
	// out-of-line copy (0x592510) to hand the clone's to the new object
	// (invented name)
	// FUNCTION: STBC 0x00592510 ?GetOBBModel@VisibleDamage@@QAEPAVOBBModel@@XZ @game/DamageableObject
	OBBModel *		GetOBBModel( void ) { return obbModel; }
	// the damaged model; DamageableObject::BreakOffPiece attaches the
	// clone's under the new object's node (invented name)
	NiNode *		GetModel( void ) { return model; }
	// frees the polygonizer's list of grids without deleting the grids
	// (invented name)
	void			ClearGrids( void );
	// the polygonizer's list of grids (invented name)
	NiTList<DamageVoxelGrid *> *	GetGrids( void );
	// the cells of the polygonizer's grid that are set (invented name)
	unsigned int	GetCellCount( void );
	// whether the cell lies in the current grid's box and is set (invented name)
	bool			IsCellSet( unsigned short x, unsigned short y, unsigned short z );
	// whether the voxel data has the cell solid or on the surface (invented name)
	bool			IsCellFilled( unsigned short x, unsigned short y, unsigned short z );

	// the object's damage volumes, shared with the polygonizer and the
	// builder (invented name)
	void			SetVolumeList( MetaVolumePool *list );
	void			SetUnknown10( NiProperty *property );
	void			SetUnknown14( NiProperty *property );
	void			SetUnknown18( NiProperty *property );
	// the model's OBBModel; the OBBModel calls back into this object
	// (invented name)
	void			SetOBBModel( OBBModel *model_ );
	// the polygonizer's two counters at 0x00 and 0x04 (invented name)
	void			SetUnknown00( float value );
	// gives the polygonizer the model's voxel data and the builder the grid
	// built from it (invented name)
	void			SetVoxelData( NiBinaryVoxelExtraData *data );
	// turn the kinds of damage on (invented names)
	void			SetBuilderUnknown18( bool enable );
	void			SetPolygonizerUnknown09( bool enable );
	void			SetPolygonizerUnknown08( bool enable );
	// the polygonizer's cell holding a world point (invented name)
	void			GetCell( const NiPoint3 *point, unsigned short *x, unsigned short *y, unsigned short *z );
	// gives a new damage volume to the kinds of damage that are on
	// (invented name)
	void			AddVolume( MetaVolume *volume );
	// the damaged model: the polygonizer's debug node and the builder's
	// pieces hang under it (invented name)
	void			SetModel( NiNode *model_ );
	// rebuilds the polygonizer's damage when a kind of it is on (invented
	// name)
	void			Polygonize( void );
	// narrows the polygonizer's grid to another one's cells and resorts
	// the builder's pieces (invented name)
	void			Unknown4B1B90( DamageVoxelGrid *grid );
	// rebuilds the builder's damage when that kind of damage is on (invented
	// name)
	void			Unknown4B2050( void );
	// the builder's settings (invented names)
	void			SetBuilderUnknown0C( float value );
	void			SetBuilderUnknown08( float value );
	void			SetBuilderUnknown10( float value );
	void			SetBuilderUnknown19( bool value );

protected:
	NiPointer<NiNode>		model;			// 0x04 the damaged model (the clone 0x4B1550 builds an OBBModel from it)
	NiPointer<OBBModel>		obbModel;		// 0x08
	NiPointer<MetaVolumePool>	volumeList;	// 0x0C the object's damage volumes, shared with the polygonizer and the builder
	NiPointer<NiProperty>	unknown10;		// 0x10 a property the damage is drawn with (TODO identity)
	NiPointer<NiProperty>	unknown14;		// 0x14 a property the damage is drawn with (TODO identity)
	NiPointer<NiProperty>	unknown18;		// 0x18 a property the damage is drawn with (TODO identity)
	MetaVolumePolygonizer *		polygonizer;	// 0x1C owned
	MetaSurfacePolygonizer *	builder;		// 0x20 owned
};

#endif /* !__VISIBLEDAMAGE_H__ */
