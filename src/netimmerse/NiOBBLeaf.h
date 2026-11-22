/*
===============================================================================

	NiOBBLeaf.h

	A leaf of a triangle shape's oriented bounding box tree: one triangle,
	held as pointers to its three world vertices, and the box fitted around
	it.

	Evidence: NiOBBRoot's constructor and the node builder allocate 0x98
	bytes and call the constructor (0x825D60) with the root, the triangle
	list, the model and world vertices and the triangle's index; it builds
	the box through the out-of-line NiOBBox constructor (0x827FD0), stores
	the vtable 0x89A7AC, clears the subtrees and keeps the world vertex
	pointers at 0x8C, 0x90 and 0x94. The vtable holds the scalar deleting
	destructor (0x826530), whose destructor is NiOBBNode's (0x826550 jumps to
	0x8282C0), IsLeaf (0x827FE0, true) and the three tests (0x8279C0,
	0x827A50, 0x827C50). The class name follows the Gamebryo lineage
	(NiOBBLeaf).

===============================================================================
*/

#ifndef __NIOBBLEAF_H__
#define __NIOBBLEAF_H__

#include "NiOBBNode.h"

// FUNCTION: STBC 0x826530 ??_GNiOBBLeaf@@UAEPAXI@Z
// FUNCTION: STBC 0x826550 ??1NiOBBLeaf@@UAE@XZ
class NiOBBLeaf : public NiOBBNode {
public:
							NiOBBLeaf( NiOBBRoot *root, const unsigned short *triList, const NiPoint3 *vertices,
									   const NiPoint3 *worldVertices, unsigned short triangle );

	// FUNCTION: STBC 0x827FE0 ?IsLeaf@NiOBBLeaf@@UAE_NXZ @netimmerse/NiOBBLeaf
	virtual bool			IsLeaf() { return true; }
	virtual bool			TestCollision( NiOBBNode *other, int depth, int otherDepth );
	virtual int				FindCollisions( NiOBBNode *other, NiAVObject *root0, NiAVObject *root1,
											NiAVObject *object0, NiAVObject *object1, int depth, int otherDepth,
											unsigned int stamp, unsigned int otherStamp );
	virtual int				FindIntersections( const NiPoint3 &origin, const NiPoint3 &direction, NiPick &pick,
											   bool &found, NiAVObject *object, unsigned int stamp );

protected:
	// the barycentric coordinates of a point in the plane of a triangle
	static void				ComputeBarycenter( const NiPoint3 &v0, const NiPoint3 &v1, const NiPoint3 &v2, const NiPoint3 &point,
											   float barycenter[3] );
	// the first and last times a moving segment meets a plane, and the points
	// touching then (names are those NICOLLISION11VC60.LIB exports)
	static int				TentativeCollision( float deltaTime, const NiPoint3 &normal, float constant, const NiPoint3 &velocity,
												const NiPoint3 &end0, const NiPoint3 &end1, float &time0, NiPoint3 &point0,
												float &time1, NiPoint3 &point1 );
	// the first contact of a moving edge with a triangle
	static int				TestEdgeTri( const NiPoint3 &end0, const NiPoint3 &end1, const NiPoint3 *const *triangle,
										 const NiPoint3 &normal, float constant, const NiPoint3 &velocity, float deltaTime,
										 float &time, NiPoint3 &point );
	// the first contact of one triangle's edges, moving relative to the other triangle, with it
	static int				FindIntersection( const NiPoint3 *const *edges, const NiPoint3 *const *triangle,
											  const NiPoint3 &velocity, float deltaTime, float &time, NiPoint3 &point );
	// whether two leaves' triangles meet while their shapes move over the
	// step; the first contact's time and point when they do (name invented)
	static int				TestTriangles( NiOBBLeaf *leaf0, NiOBBLeaf *leaf1, float &time, NiPoint3 &point );
	// the triangles of this leaf and other, as moving; the callback's
	// verdict when they meet (Gamebryo 1.2.2 name)
	int						FaceFaceCollision( NiOBBLeaf *other, NiAVObject *root0, NiAVObject *root1, NiAVObject *object0,
											   NiAVObject *object1 );

	const NiPoint3 *		vertex[3];		// the triangle's world vertices
};

#endif /* !__NIOBBLEAF_H__ */
