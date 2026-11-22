/*
===============================================================================

	OBBNode.h

	A node of an OBBRoot's tree of oriented bounding boxes.

===============================================================================
*/

#ifndef __OBBNODE_H__
#define __OBBNODE_H__

#include "NiBox.h"
#include "NiMatrix3.h"
#include "NiPoint3.h"
#include "NiPointer.h"
#include "NiTArray.h"
#include "NiTriShape.h"
#include "NiTriIntersect.h"

class NiAVObject;
class NiPick;
class NiPlane;
class VisibleDamage;
class CollisionRecorder;
class VisibleDamagePiece;

// The triangles of one shape that a sphere test touched (invented name).
// 0x0C bytes. Evidence: the leaf node's sphere test (0x469FC0) finds the
// shape's list in the array it is given by the smart pointer at 0x00, else
// makes one with room for every triangle of the shape; its slot 4
// (0x469F80) appends its own triangles and adds to the count.
// MetaSurfacePolygonizer::AddVolume (0x4B4B40) reads the lists and deletes
// them: the triangles, then the smart pointer, then the block.
struct OBBTriangleList {
						OBBTriangleList( NiTriShape *shape_ ) {
							shape = shape_;
							triangles = new unsigned short[shape_->GetShapeData()->GetTriangleCount()];
							count = 0;
						}
						~OBBTriangleList() { delete [] triangles; }

	// the shape's list in lists, or NULL (invented name)
	static OBBTriangleList *	Find( NiTArray<OBBTriangleList *> *lists, NiTriShape *shape ) {
							for ( unsigned int i = 0; i < lists->GetSize(); i++ ) {
								OBBTriangleList *list = lists->GetAt( i );
								if ( list->shape == shape ) {
									return list;
								}
							}
							return NULL;
						}

	NiPointer<NiTriShape>	shape;		// 0x00
	unsigned short		count;			// 0x04
	unsigned short *	triangles;		// 0x08 owned: room for every triangle of the shape
};

// A node of the tree (DamageTool.exe's RTTI names OBBNode and OBBLeaf :
// OBBNode, with 8-slot vtables in the same order): a box around a range of the shape's
// triangles. 0x48 bytes: the constructor (0x4654F0) takes the shape, the
// leaf size and the tree's index and plane arrays; the vtable 0x88BA94
// has 7 slots, slot 0 the deleting destructor (0x4657B0); the next
// vtable, 0x88BAB0, is the leaf node's (stored by 0x467EE0; its
// destructor 0x468760 jumps to this one's, 0x4657F0). The destructor
// deletes the two subnodes at 0x40 and 0x44. The slots' purposes are
// known only from OBBRoot's calls: 1 and 2 test a ray in the shape's
// model space and fill a pick, calling back into a VisibleDamage when one
// is given; 3 tests a sphere, gathering the triangles it touches; 5 tests
// another tree.
class OBBNode {
public:
						OBBNode();
						OBBNode( NiTriShape *shape, int leafSize, unsigned short *indices, NiPlane *planes );
	virtual				~OBBNode();
	// The slots' names are DamageTool.exe's: there OBBNode's vtable holds
	// the destructor, FindRayIntersections, FindSegmentIntersections,
	// FindSphereIntersections, FindRoughOBBIntersections,
	// FindBoxIntersections, Update and AddLeafVolumes, with the parameter
	// lists of these slots before the damage callback was added. Slot 4 has
	// no counterpart there.
	virtual bool		FindRayIntersections( NiTriShape *shape, const NiPoint3 &localOrigin, const NiPoint3 &localDirection,
								const NiPoint3 &origin, const NiPoint3 &direction, NiPick &pick, VisibleDamage *callback, VisibleDamagePiece *&piece );
	virtual bool		FindSegmentIntersections( NiTriShape *shape, const NiPoint3 &localOrigin, const NiPoint3 &localDirection,
								const NiPoint3 &origin, const NiPoint3 &direction, NiPick &pick, VisibleDamage *callback, VisibleDamagePiece *&piece );
	virtual bool		FindSphereIntersections( NiTriShape *shape, const NiPoint3 &localCenter, float radius, float radiusSquared, NiTArray<OBBTriangleList *> *lists );
	// adds the triangles under the node to the list (each subnode's slot 4)
	virtual void		VirtualSlot04( OBBTriangleList *list );
	virtual bool		FindRoughOBBIntersections( NiTriShape *shape, NiTriShape *otherShape, OBBNode *other, CollisionRecorder *recorder, VisibleDamage *damage, VisibleDamage *otherDamage );
	// Gathers the triangles that meet box into the shape's list, through
	// the callback when one is given. With depth above 0 the node's box is
	// tested first (0x466850) and the walk stops when it fails; the
	// subnodes get depth - 1; at depth 0 every triangle below is taken.
	virtual bool		FindBoxIntersections( NiTriShape *shape, const NiBox &box, NiTArray<OBBTriangleList *> *lists, int depth,
								VisibleDamage *callback, VisibleDamagePiece *&piece );

	// the subnodes, NULL in a leaf (invented names)
	OBBNode *			GetLeft( void ) const { return left; }
	OBBNode *			GetRight( void ) const { return right; }

protected:
	// A triangle and the distance of its centroid along the box's longest
	// axis, sorted to split a node's triangles in half. 8 bytes:
	// DivideTriangles (0x467D70) sorts them with qsort. The names are
	// DamageTool.exe's (DivideTriangles' last parameter is an
	// OBBNode::ProjectionInfo *, the order ?Compare@OBBNode@@KAHPBX0@Z).
	struct ProjectionInfo {
		unsigned short		triangle;		// 0x00
		float				distance;		// 0x04
	};

	// qsort's order for ProjectionInfo: nearest first
	static int			Compare( const void *a, const void *b );

	// Building the tree (the Gamebryo lineage's NiOBBNode functions, adapted;
	// the names are theirs). order holds the triangles of the range i0..i1;
	// scratch receives them split in two halves; indices and planes are the
	// tree's arrays, each leaf taking the next run of them; keys is room for
	// the sort.
	void				CreateRecursive( const unsigned short *triList, const NiPoint3 *vertices, const float *areas,
								const NiPoint3 *centroids, int i0, int i1, unsigned short *order, unsigned short *scratch,
								int leafSize, unsigned short *indices, NiPlane *planes, ProjectionInfo *keys );
	void				ComputeOBB( const unsigned short *triList, const NiPoint3 *vertices, const float *areas,
								const NiPoint3 *centroids, int i0, int i1, const unsigned short *order );
	NiMatrix3			ComputeCovarianceAndMean( const unsigned short *triList, const NiPoint3 *vertices,
								const float *areas, const NiPoint3 *centroids, int i0, int i1, const unsigned short *order );
	void				DivideTriangles( const NiPoint3 *centroids, int i0, int i1, unsigned short *order,
								int &j0, int &j1, unsigned short *scratch, ProjectionInfo *keys );

	// Separating axis tests of the node's box against the ray from origin
	// along direction (slot 1) and against the segment from origin to
	// origin + direction (slot 2); the slots descend only when they pass.
	// The names are DamageTool.exe's symbols (the BC SDK's damage tool): its RayIntersectsBox
	// (0x419FC0) has 0x4662C0's instructions, and its SegmentIntersectsBox
	// is the same body, which retail's segment slot calls in its own form.
	bool				RayIntersectsBox( const NiPoint3 &localOrigin, const NiPoint3 &localDirection );
	bool				SegmentIntersectsBox( const NiPoint3 &localOrigin, const NiPoint3 &localDirection );

	// The sphere tests of the node's box (slot 3): whether the sphere
	// meets the box (the squared distance from the center to the box is
	// summed axis by axis), and whether the box lies inside the sphere,
	// which always answers false. SphereIntersectsBox is named from
	// DamageTool.exe's symbols (the BC SDK's damage tool) (0x41A460, 0x4666F0's
	// instructions); the second name is a placeholder.
	bool				SphereIntersectsBox( const NiPoint3 &localCenter, float radiusSquared );
	bool				Unknown4666E0( const NiPoint3 &localCenter, float radiusSquared );

	// Separating axis tests of the node's box against a box in the same
	// model space (slot 6) and against another node's box, carried from
	// otherShape's model space into shape's (slot 5). Both compare the
	// absolute value of the radii's sum, not of the centers' distance,
	// with the distance. The name is from DamageTool.exe's symbols (the BC SDK's damage tool):
	// BoxIntersectsBox( const NiBox & ) (0x41A5A0) has 0x466850's
	// instructions, and BoxIntersectsBox( const NiAVObject *, const
	// NiAVObject *, OBBNode * ) (0x41AB90) is the pair test's overload.
	bool				BoxIntersectsBox( const NiBox &box );
	bool				BoxIntersectsBox( NiAVObject *shape, NiAVObject *otherShape, OBBNode *other );

	// Whether two still triangles meet (the leaf pair test of slot 5).
	// The name is from DamageTool.exe's symbols (the BC SDK's damage tool): its
	// TriIntersectsTri (0x41AC10) has 0x467630's instructions.
	bool				TriIntersectsTri( NiPoint3 *triangle0[3], NiPoint3 *triangle1[3] );

	// The box, in the shape's model space; the constructor (0x4654F0) fits
	// it around the node's triangles.
	NiPoint3			center;				// 0x04
	NiPoint3			axis[3];			// 0x10 unit length
	float				extent[3];			// 0x34 half extents along the axes
	OBBNode *			left;				// 0x40 owned subnode (invented name)
	OBBNode *			right;				// 0x44 owned subnode (invented name)
};

#endif /* !__OBBNODE_H__ */
