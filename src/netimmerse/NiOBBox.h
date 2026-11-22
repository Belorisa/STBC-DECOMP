/*
===============================================================================

	NiOBBox.h

	An oriented bounding box: a center, three unit axes and the half
	extents along them, in model space and in world space. The boxes of a
	triangle shape's OBB tree are tested against each other, swept by the
	shapes' velocities, and against rays.

	Evidence: NiOBBNode keeps the box at 0x04 and NiOBBLeaf fits it around
	its triangle; the transform (0x829770) rotates the model center and axes
	into the world ones at 0x3C and 0x48, scales the extents at 0x30 into
	0x6C and adds the translation; the tests read the world members only.
	The word at 0x78 is the tree's NiOBBRoot: the moving test (0x82A440)
	and the leaf-pair test (0x828FB0) read its velocity at 0x10. The class
	name follows the Gamebryo lineage (NiOBBox); member names are invented.

===============================================================================
*/

#ifndef __NIOBBOX_H__
#define __NIOBBOX_H__

#include "NiMatrix3.h"
#include "NiPoint3.h"

class NiOBBRoot;

class NiOBBox {
public:
	// FUNCTION: STBC 0x827FD0 ??0NiOBBox@@QAE@PAVNiOBBRoot@@@Z @netimmerse/NiOBBLeaf
							NiOBBox( NiOBBRoot *root_ = 0 ) { root = root_; }

	// the world box: the model box rotated, scaled and translated
	void					Transform( const NiMatrix3 &rotate, const NiPoint3 &translate, float scale );

	// whether the world boxes overlap (a separating axis test)
	int						TestIntersection( const NiOBBox &box );

	// whether the world boxes meet while both move by their roots'
	// velocities over the roots' time step; the first time of contact and
	// the contact point when they do (names invented)
	int						FindIntersection( const NiOBBox &box, float &time, NiPoint3 &point );

	// the box's outward normal at a world point: the axis whose face the
	// point lies on, else the normalised direction of the point scaled by the
	// extents (name invented)
	void					GetNormal( const NiPoint3 &point, NiPoint3 &normal ) const;

	// whether a ray meets the world box (a slab test) (name invented)
	int						TestRay( const NiPoint3 &origin, const NiPoint3 &direction );

	// where a ray first meets the world box: the distance along the ray
	// and the face's normal, turned against the ray; no callers (name
	// invented)
	int						FindRayIntersection( const NiPoint3 &origin, const NiPoint3 &direction, float &distance,
												 NiPoint3 &normal );

	NiOBBRoot *				GetRoot() const { return root; }
	void					SetRoot( NiOBBRoot *root_ ) { root = root_; }

	NiPoint3				center;			// model space
	NiPoint3				axis[3];		// model space, unit length
	float					extent[3];		// half extents along the axes
	NiPoint3				worldCenter;
	NiPoint3				worldAxis[3];
	float					worldExtent[3];

	// a ray direction closer than this to a face plane is parallel to it
	static const float		PARALLEL_EPSILON;
	// the bound of a ray's parameter interval before any slab narrows it
	static const float		MAX_DISTANCE;

protected:
	friend class NiOBBLeaf;

	NiOBBRoot *				root;			// the tree's root; NULL until the tree is built
};

#endif /* !__NIOBBOX_H__ */
