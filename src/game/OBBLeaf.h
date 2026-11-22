/*
===============================================================================

	OBBLeaf.h

	A leaf of an OBBRoot's tree: the box around a run of the shape's
	triangles, with the triangles themselves.

	0x54 bytes. Evidence: OBBNode::CreateRecursive (0x465820) builds one for
	a range shorter than the leaf size; the constructor (0x467EE0) runs
	OBBNode's, stores the run's slices of the tree's index and plane arrays
	at 0x48 and 0x4C, then the vtable 0x88BAB0 (7 slots, all overridden)
	and the count at 0x50; it copies the run's triangles, fits the box (one
	triangle on its own, more through ComputeOBB) and stores each
	triangle's plane. The destructor is implicit (0x468760 jumps to
	OBBNode's). The name is from DamageTool.exe's RTTI (OBBLeaf : OBBNode;
	the Gamebryo lineage's NiOBBLeaf plays the same part).

===============================================================================
*/

#ifndef __OBBLEAF_H__
#define __OBBLEAF_H__

#include "OBBNode.h"

// FUNCTION: STBC 0x00468740 ??_GOBBLeaf@@UAEPAXI@Z
// FUNCTION: STBC 0x00468760 ??1OBBLeaf@@UAE@XZ
class OBBLeaf : public OBBNode {
public:
						OBBLeaf( const unsigned short *triList, const NiPoint3 *vertices, const float *areas,
								 const NiPoint3 *centroids, int i0, int i1, const unsigned short *order,
								 unsigned short *indices_, NiPlane *planes_ );
	virtual bool		FindRayIntersections( NiTriShape *shape, const NiPoint3 &localOrigin, const NiPoint3 &localDirection,
								const NiPoint3 &origin, const NiPoint3 &direction, NiPick &pick, VisibleDamage *callback, VisibleDamagePiece *&piece );
	virtual bool		FindSegmentIntersections( NiTriShape *shape, const NiPoint3 &localOrigin, const NiPoint3 &localDirection,
								const NiPoint3 &origin, const NiPoint3 &direction, NiPick &pick, VisibleDamage *callback, VisibleDamagePiece *&piece );
	virtual bool		FindSphereIntersections( NiTriShape *shape, const NiPoint3 &localCenter, float radius, float radiusSquared, NiTArray<OBBTriangleList *> *lists );
	// appends the leaf's triangles to the list
	virtual void		VirtualSlot04( OBBTriangleList *list );
	virtual bool		FindRoughOBBIntersections( NiTriShape *shape, NiTriShape *otherShape, OBBNode *other, CollisionRecorder *recorder, VisibleDamage *damage, VisibleDamage *otherDamage );
	virtual bool		FindBoxIntersections( NiTriShape *shape, const NiBox &box, NiTArray<OBBTriangleList *> *lists, int depth,
								VisibleDamage *callback, VisibleDamagePiece *&piece );

protected:
	// The helpers are protected, as DamageTool.exe's symbols have them
	// (IAE, and KA for BoxIntersectsTri).

	// Where the ray from origin along direction (RayIntersectsTriangle) or the
	// segment from origin to origin + direction (SegmentIntersectsTriangle) crosses the
	// triangle v0 v1 v2 in plane: the point, its distance in direction's
	// lengths and its parameters along the edges v0 v1 and v0 v2. With
	// frontOnly a triangle facing away is missed. The leaf's ray and segment
	// tests (slots 1 and 2) call them. The names are from DamageTool.exe's
	// symbols, with these parameter lists; their bodies (0x41BC40, 0x41BED0)
	// have these instructions.
	bool				RayIntersectsTriangle( const NiPoint3 &origin, const NiPoint3 &direction, const NiPlane &plane,
								const NiPoint3 &v0, const NiPoint3 &v1, const NiPoint3 &v2, bool frontOnly,
								NiPoint3 &point, float &distance, float &s, float &t );
	bool				SegmentIntersectsTriangle( const NiPoint3 &origin, const NiPoint3 &direction, const NiPlane &plane,
								const NiPoint3 &v0, const NiPoint3 &v1, const NiPoint3 &v2, bool frontOnly,
								NiPoint3 &point, float &distance, float &s, float &t );

	// The squared distance from point to the triangle v0 v1 v2, with the
	// closest point's parameters along the edges v0 v1 and v0 v2 (the
	// point-triangle distance of Eberly's Magic Software library). The
	// leaf's sphere test (slot 3) calls it. The name is from DamageTool.exe's
	// symbols (0x41D6E0, the same instructions).
	float				SquaredDistanceToTri( const NiPoint3 &point, const NiPoint3 &v0, const NiPoint3 &v1, const NiPoint3 &v2,
								float &s, float &t );

	// Whether the triangle v0 v1 v2 meets box: a separating axis test over
	// the box's axes, the triangle's normal and the edges crossed with the
	// axes. cdecl, after the leaf's slots (0x46B5A0). The name is from
	// DamageTool.exe's symbols (a static member there too, 0x41DC20, the
	// same instructions).
	static bool			BoxIntersectsTri( const NiBox &box, const NiPoint3 &v0, const NiPoint3 &v1, const NiPoint3 &v2 );

	unsigned short *	indices;			// 0x48 the leaf's run of the tree's array: its triangles
	NiPlane *			planes;				// 0x4C the leaf's run of the tree's array: one per triangle
	unsigned int		count;				// 0x50 triangles
};

#endif /* !__OBBLEAF_H__ */
