/*
===============================================================================

	VisibleDamageBuilder.h

	The part of a VisibleDamage (at 0x20) that keeps, for each triangle
	shape of the damaged model, the data the damage is built from
	(VisibleDamagePiece), and answers which triangles a shape is drawn
	with: its piece's, or the shape's own.

	0x54 bytes. Evidence: the constructor (0x4B46D0) clears seven smart
	pointers (0x1C-0x34) and a list built with two items per block (0x3C),
	stores the vtable 0x88C698 (one slot), then sets four floats (0.8,
	0.5, NI_INFINITY, 0.1 at 0x08-0x14) and three flags (0x38, 0x18,
	0x19). The non-virtual destructor (0x4B4730) deletes every piece of
	the list (0x4B3C00 then the allocator's free) and frees the list's
	blocks. 0x4B8E80 finds a shape's piece by the smart pointer at its
	start; slot 0 (0x4B4980) returns the piece's triangle list (0x78), or
	the shape data's own. The polygonizer's name is from DamageTool.exe
	(the BC SDK's damage tool, built with RTTI and a COFF symbol table), an
	earlier build of the same class: ??0MetaSurfacePolygonizer (0x40D3E0)
	clears the same members and sets the same floats (0.8, 0.5, NI_INFINITY,
	0.1), and its ProcessMesh, ProcessTri, ProcessDamageAndHoleTri,
	ProcessDamageTri and SetUpSceneGraph (0x40EDC0-0x4119A0) have the
	instructions of 0x4BAB90, 0x4B71E0, 0x4B79C0, 0x4B8420 and SetModel
	here, and its CollectFeatures (0x40E4D0) those of 0x4B5640; the methods
	here take those names. The piece's name and every member name
	are invented.

===============================================================================
*/

#ifndef __VISIBLEDAMAGEBUILDER_H__
#define __VISIBLEDAMAGEBUILDER_H__

#include "DamageBitArray.h"
#include "NiObject.h"
#include "NiPointer.h"
#include "NiTList.h"
#include "NiTMap.h"
#include "NiTriShape.h"
#include "NiNode.h"
#include "MetaVolume.h"
#include "OBBModel.h"

class DamageVoxelGrid;
class SimpleFeatureCollector;

/*
================
EdgeMap

The vertices the cutting makes on the shape's edges, keyed by the edge's two
vertex indices. Evidence: the vtable 0x88C69C, whose slots
1-4 are NiTMap<__int64, unsigned short>'s (0x88C6B0) and whose slot 0 is
its own deleting destructor (0x4B71C0); the constructor (0x4B7100) is
NiTMap's with 37 buckets and the vtable stored after it. The name is from
DamageTool.exe's RTTI, which names EdgeMap : NiTMap<__int64, unsigned
short> with the same vtable (its own deleting destructor, NiTMap's four
slots), and its symbol table: ProcessTri and the other edge cuts take an
EdgeMap *.
================
*/
// FUNCTION: STBC 0x004b71c0 ??_GEdgeMap@@UAEPAXI@Z @game/VisibleDamageBuilder
// FUNCTION: STBC 0x004b7150 ??1EdgeMap@@UAE@XZ @game/VisibleDamageBuilder
class EdgeMap : public NiTMap<__int64, unsigned short> {
public:
	// (retail keeps an out-of-line copy at 0x4B7100 that no unit emits yet)
						EdgeMap() {}

	// (invented) records the mesh index of an edge's cut point
	void				AddEdge( __int64 key, unsigned short index ) { SetAt( key, index ); }
};

/*
================
VisibleDamagePiece

One triangle shape's damage data (invented name). Evidence: the
destructor (0x4B3C00) frees the triangle lists at 0x78 and 0x7C, then
destroys six bit arrays (0x70-0x48), four smart pointers (0x44-0x38), a
list (0x04) and the shape (0x00). Its size and its other members are not
known yet. The OBB ray tests hand the piece that VisibleDamage's slot 0
found to its slots 1 and 2, which weigh its damage volumes at a point of
its shape (0x4B49B0); 0x4B4B40 adds a volume to the piece of every shape
the volume's sphere touches.
================
*/
class VisibleDamagePiece {
public:
						VisibleDamagePiece( NiTriShape *shape_, NiNode *model, float unknown );
						~VisibleDamagePiece();

	// whether the triangle's bit is set in unknown4C (invented name)
	bool				IsTriangleSet( unsigned short triangle ) const { return unknown48.IsSet( triangle ); }
	// clears the triangle's bit and takes it out of the list at 0x7C,
	// moving the last one into its place (invented name)
	void				Unknown4B45E0( unsigned short triangle );
	// folds the triangle into its first corner and sets its bit at 0x50,
	// first giving the shape a copy of its data when the data is shared
	// (invented name)
	void				Unknown4B4380( unsigned short triangle );
	// sets the triangle's bit at 0x58; a triangle newly set leaves the list
	// and is folded unless its bit at 0x50 is set (invented name)
	void				Unknown4B4660( unsigned short triangle );
	// moves the shape and its damage shapes under other's nodes, the shape
	// keeping its place in the world against model, and drops the volumes
	// that miss the box (no caller in retail; invented name)
	void				Unknown4B3D70( MetaSurfacePolygonizer *other, NiNode *model, const NiPoint3 *boxMin, const NiPoint3 *boxMax );
	// a new piece for other, of a copy of the shape hung under other's
	// model, with this piece's triangles and marks and the volumes that
	// meet the box; model is not used (invented name)
	VisibleDamagePiece *	Unknown4B3FE0( MetaSurfacePolygonizer *other, NiNode *model, const NiPoint3 *boxMin, const NiPoint3 *boxMax );

	NiPointer<NiTriShape>	shape;			// 0x00 the shape the piece belongs to
	NiTList<MetaVolume *>	volumes;		// 0x04 the damage volumes that touch the shape
	int					unknown1C;			// 0x1C
	int					unknown20;			// 0x20
	int					unknown24;			// 0x24
	int					unknown28;			// 0x28
	int					unknown2C;			// 0x2C
	int					unknown30;			// 0x30
	int					unknown34;			// 0x34
	NiPointer<NiTriShape>	unknown38;		// 0x38 the damage built from meshes[0], under the builder's unknown24
	NiPointer<NiTriShape>	unknown3C;		// 0x3C its back faces, under unknown30
	NiPointer<NiTriShape>	unknown40;		// 0x40 the damage built from meshes[1], under unknown28
	NiPointer<NiTriShape>	unknown44;		// 0x44 the damage built from meshes[2], under unknown2C
	DamageBitArray		unknown48;			// 0x48 a bit per triangle
	DamageBitArray		unknown50;			// 0x50
	DamageBitArray		unknown58;			// 0x58
	DamageBitArray		unknown60;			// 0x60
	DamageBitArray		unknown68;			// 0x68
	DamageBitArray		unknown70;			// 0x70
	unsigned short *	triList;			// 0x78 owned: the triangles the shape is drawn with
	unsigned short *	unknown7C;			// 0x7C owned: unknown80 triangles
	unsigned short		unknown80;			// 0x80
};

/*
================
MetaSurfacePolygonizer
================
*/
class MetaSurfacePolygonizer {
public:
						MetaSurfacePolygonizer();
						~MetaSurfacePolygonizer();

	// the triangles the shape is drawn with: its piece's, or its own
	virtual unsigned short *	GetTriList( NiTriShape *shape );

	// When the first corner of a triangle edge is the triangle's own and
	// the triangle's bit is set in its shape's piece, replaces the edge's
	// corners with the triangle's other two; the piece is looked up once
	// and kept in *piece (invented name)
	bool				Unknown4B48C0( NiTriShape *shape, int triangle, const unsigned short *corner, unsigned short *corner1,
									   unsigned short *corner2, VisibleDamagePiece **piece );

	// the piece's bit arrays at 0x58 and 0x70, or NULL without a piece
	// (invented names)
	DamageBitArray *	GetUnknown58( NiTriShape *shape );
	DamageBitArray *	GetUnknown70( NiTriShape *shape );

	// whether the piece's damage volumes add up, at point in the frame of
	// the piece's shape, to more than unknown08 (invented name)
	bool				Unknown4B49B0( VisibleDamagePiece *piece, const NiPoint3 *point );

	// takes the damaged model: a piece for each of its triangle shapes, and
	// the nodes and properties the damage is drawn with; the three
	// properties come from VisibleDamage (invented name)
	void				SetModel( NiNode *model_, NiProperty *property0, NiProperty *property1, NiProperty *property2 );
	// adds to list the pieces whose shape's world bound, in model's frame,
	// meets the box (no caller in retail; invented name)
	void				Unknown4B93D0( NiTList<VisibleDamagePiece *> *list, NiNode *model, const NiPoint3 *boxMin, const NiPoint3 *boxMax );
	// a piece for each triangle shape under object (invented name)
	void				AddPieces( NiAVObject *object );

	// gives the volume to the piece of every shape its sphere touches, and
	// marks the touched triangles that are neither in unknown48 nor in
	// unknown58 in unknown48 and the list at 0x7C (invented name)
	void				AddVolume( MetaVolume *volume );

	// the shared meshes the pieces are built in, made at startup and freed at
	// shutdown (invented names)
	static void			Init();
	static void			Shutdown();

	// the settings (invented names)
	float				GetUnknown08() const { return unknown08; }
	float				GetUnknown0C() const { return unknown0C; }
	float				GetUnknown10() const { return unknown10; }
	bool				GetUnknown18() const { return unknown18; }
	bool				GetUnknown19() const { return unknown19; }
	// their setters, which VisibleDamage's SetBuilderUnknown* wrap (DamageTool.exe's
	// Damage::SetSurfaceDamageLevel, SetSurfaceScorchLevel, SetSurfaceDamageRes and
	// SetSurfaceGenerateHoleGeometry, 0x40C010-0x40C040) (invented names)
	void				SetUnknown08( float value ) { unknown08 = value; }
	void				SetUnknown0C( float value ) { unknown0C = value; }
	void				SetUnknown10( float value ) { unknown10 = value; }
	void				SetUnknown19( bool value ) { unknown19 = value; }

	// builds the pieces' damage only with the damaged model's data at 0x20
	// (invented name)
	void				SetUnknown18( bool enable );

	// sorts the pieces' triangles against the polygonizer's current grid
	// (invented name)
	void				Unknown4BA250( void );
	// gives other the pieces that meet the grid's box, cut to it (invented
	// name)
	void				Unknown4BA030( DamageVoxelGrid *grid_, MetaSurfacePolygonizer *other );
	// takes the piece off the model and deletes it (invented name)
	void				RemovePiece( VisibleDamagePiece *piece );
	// sample a triangle (three points) against the current grid: no
	// sample in a set cell; some sample in a cell not set (invented names)
	bool				Unknown4B98B0( const NiPoint3 *triangle );
	bool				Unknown4B9C80( const NiPoint3 *triangle );

	// rebuilds the damage of the pieces new volumes touched; with force,
	// even when none did (invented name)
	void				Unknown4B4E50( bool force );
	// draws the piece's rebuilt damage from the shared meshes (invented name)
	void				CollectFeatures( VisibleDamagePiece *piece );
	// gives the damage shape the source shape's texture properties and the
	// vertex colour and white material it is drawn with (invented name)
	void				Unknown4B8AE0( NiTriShape *damage, NiTriShape *source );
	// tessellates a large triangle into a grid of small ones weighed one by
	// one, inside the polygonizer's grid when inGrid; false when the
	// triangle is small enough or the grid leaves it whole (invented name)
	bool				Unknown4B5ED0( unsigned short *remap0, unsigned short *remap1, unsigned short *remap2, bool inGrid,
									   unsigned short *triangle, unsigned short *flags, NiPoint3 *vertices, NiPoint2 *textures,
									   NiPoint3 *normals, int volumeCount, MetaVolume **volumes );
	// cuts one triangle by its corners' weights into the shared meshes and
	// returns the classes of its corners (invented name)
	unsigned short		ProcessTri( unsigned short *remap0, unsigned short *remap1, unsigned short *remap2, EdgeMap *edges0,
									   EdgeMap *edges1, EdgeMap *edges2, unsigned short *triangle, float *weights,
									   NiPoint3 *vertices, NiPoint2 *textures, NiPoint3 *normals );
	// the second cut of one polygon of a cut triangle's six points, by
	// unknown08: into meshes[1] only, or with unknown19 into meshes[1] and
	// meshes[2] (invented names)
	unsigned short		ProcessDamageTri( unsigned short *remap1, EdgeMap *edges1, const unsigned short *corners, const __int64 *keys,
									   const float *weights, const NiPoint3 *points, const NiPoint2 *textures );
	unsigned short		ProcessDamageAndHoleTri( unsigned short *remap1, unsigned short *remap2, EdgeMap *edges1, EdgeMap *edges2,
									   const unsigned short *corners, const __int64 *keys, const float *weights, const NiPoint3 *points,
									   const NiPoint2 *textures );
	// cuts the touched triangles by their corners' weights into the shared
	// meshes; the remaps take each shape vertex to its copy in the meshes
	// (invented name)
	void				ProcessMesh( unsigned short *remap0, unsigned short *remap1, unsigned short *remap2,
									   DamageBitArray *gridTriangles, DamageBitArray *largeTriangles, unsigned short triangleCount,
									   unsigned short *triangles, unsigned short *triangleFlags, float *weights, NiPoint3 *vertices,
									   NiPoint2 *textures, NiPoint3 *normals, int volumeCount, MetaVolume **volumes, bool );

protected:
	friend class VisibleDamage;		// sets the settings and shares the smart pointers
	friend class VisibleDamagePiece;	// hangs its shapes under another builder's nodes

	// the piece of the shape, or NULL
	VisibleDamagePiece *	FindPiece( NiTriShape *shape );

	DamageVoxelGrid *	grid;				// 0x04 the polygonizer's current grid
	float				unknown08;			// 0x08 0.8 at first
	float				unknown0C;			// 0x0C 0.5 at first
	float				unknown10;			// 0x10 NI_INFINITY at first
	float				unknown14;			// 0x14 0.1 at first
	bool				unknown18;			// 0x18
	bool				unknown19;			// 0x19
	NiPointer<OBBModel>	obbModel;			// 0x1C the damaged model's OBBModel (invented name)
	NiPointer<NiNode>	model;				// 0x20 the damaged model
	NiPointer<NiNode>	unknown24;			// 0x24 a node under the model (TODO identity: what hangs under it)
	NiPointer<NiNode>	unknown28;			// 0x28 a node under the model, with vertex colours from the emissive colour
	NiPointer<NiNode>	unknown2C;			// 0x2C a node under unknown28
	NiPointer<NiNode>	unknown30;			// 0x30 a node under the model, lit only by a white emissive material
	NiPointer<MetaVolumePool>	volumeList;	// 0x34 the object's damage volumes
	bool				unknown38;			// 0x38
	NiTList<VisibleDamagePiece *>	pieces;	// 0x3C owned: one per shape

	static SimpleFeatureCollector *	meshes[3];	// the first with texture coordinates, normals and colours
};

#endif /* !__VISIBLEDAMAGEBUILDER_H__ */
