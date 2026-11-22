/*
===============================================================================

	NiOBBNode.h

	A node of a triangle shape's oriented bounding box tree: a box around a
	range of the shape's triangles and the subtrees of the two halves the
	range splits into. NiOBBLeaf holds a single triangle.

	Evidence: NiOBBRoot's constructor allocates 0x8C bytes and calls the
	constructor (0x828030) for more than one triangle; the vtable 0x89A7C0
	holds the scalar deleting destructor (0x829720), IsLeaf (0x829760,
	false), the overlap test (0x829130), the collision search (0x8292B0) and
	the ray search (0x829560). The box is at 0x04, the subtrees at 0x80 and
	0x84 (the destructor deletes both through slot 0) and the change stamp
	of the last world transform at 0x88. The constructor measures each
	triangle's area and centroid and builds the tree (0x8282F0), which fits
	each node's box to the area-weighted covariance of its triangles
	(0x828500, 0x828A40) and splits the range along the box's axes
	(0x828D90). The class and method names are Gamebryo 1.1.2's (NiOBBNode.h).

===============================================================================
*/

#ifndef __NIOBBNODE_H__
#define __NIOBBNODE_H__

#include "NiAVObject.h"
#include "NiOBBox.h"

class NiAVObject;
class NiPick;
struct NiCollisionIntersect;

// FUNCTION: STBC 0x829720 ??_GNiOBBNode@@UAEPAXI@Z
class NiOBBNode {
public:
							NiOBBNode( NiOBBRoot *root, unsigned short triangleCount, const unsigned short *triList,
									   const NiPoint3 *vertices, const NiPoint3 *worldVertices, int binSize );
	virtual					~NiOBBNode();

	// FUNCTION: STBC 0x829760 ?IsLeaf@NiOBBNode@@UAE_NXZ @netimmerse/NiOBBNode
	virtual bool			IsLeaf() { return false; }

	// whether the trees overlap, descending at most depth levels of this
	// tree and otherDepth levels of the other
	virtual bool			TestCollision( NiOBBNode *other, int depth, int otherDepth );

	// reports the leaf pairs that collide while the objects move; 1 when a
	// callback asks to stop
	virtual int				FindCollisions( NiOBBNode *other, NiAVObject *root0, NiAVObject *root1,
											NiAVObject *object0, NiAVObject *object1, int depth, int otherDepth,
											unsigned int stamp, unsigned int otherStamp );

	// adds the triangles a ray hits to the pick; found is set when the
	// last triangle tested is hit
	virtual int				FindIntersections( const NiPoint3 &origin, const NiPoint3 &direction, NiPick &pick,
											   bool &found, NiAVObject *object, unsigned int stamp );

	// moves the box into world space
	void					Transform( const NiMatrix3 &rotate, const NiPoint3 &translate, float scale );

	// Gamebryo 1.2.2's accessors (NiOBBNode.inl)
	bool					HasChildren() const { return left || right; }
	NiOBBNode *				GetLeft() { return left; }
	NiOBBNode *				GetRight() { return right; }
	void					TransformBox( const NiMatrix3 &rotate, const NiPoint3 &translate, float scale ) { box.Transform( rotate, translate, scale ); }
	void					SetChangeStamp( unsigned int stamp_ ) { stamp = stamp_; }
	unsigned int			GetChangeStamp() const { return stamp; }

protected:
	friend class NiOBBLeaf;

							NiOBBNode() { left = 0; right = 0; }

	// fits the box to the triangles indices[i0] to indices[i1] and builds
	// the subtrees; the halves are partitioned into scratch, which the
	// subtrees use as their index array
	void					CreateRecursive( NiOBBRoot *root, const unsigned short *triList, const NiPoint3 *vertices,
								   const NiPoint3 *worldVertices, const float *areas, const NiPoint3 *centroids, int i0,
								   int i1, int *indices, int *scratch, int binSize );

	// fits the box to the triangles: the axes are the eigenvectors of their
	// covariance, the extents span their vertices
	void					ComputeOBB( NiOBBRoot *root, const unsigned short *triList, const NiPoint3 *vertices,
									const float *areas, const NiPoint3 *centroids, int i0, int i1, const int *indices );

	// the area-weighted covariance of the triangles; their mean becomes the
	// box's center
	NiMatrix3				ComputeCovarianceAndMean( const unsigned short *triList, const NiPoint3 *vertices,
											   const float *areas, const NiPoint3 *centroids, int i0, int i1,
											   const int *indices );

	// partitions the triangles by the side of the box's center their
	// centroids lie on along the first axis that separates them, into
	// scratch[i0] to scratch[j0] and scratch[j1] to scratch[i1]; halves at
	// the middle when no axis does
	void					DivideTriangles( const NiPoint3 *centroids, int i0, int i1, const int *indices, int &j0, int &j1,
								   int *scratch );

	// reports a collision to both objects' callbacks; nonzero when either
	// asks to stop
	int						CollisionCallback( NiCollisionIntersect &intersect );

	// tests two leaves' boxes, moving or at rest, and reports a contact
	int						BoxBoxCollision( NiOBBNode *other, NiAVObject *root0, NiAVObject *root1, NiAVObject *object0,
										NiAVObject *object1 );

	NiOBBox					box;
	NiOBBNode *				left;			// owned
	NiOBBNode *				right;			// owned
	unsigned int			stamp;			// the root's change stamp the world box was last made for
};

#endif /* !__NIOBBNODE_H__ */
