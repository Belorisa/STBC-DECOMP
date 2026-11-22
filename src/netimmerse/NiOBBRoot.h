/*
===============================================================================

	NiOBBRoot.h

	The root of a triangle shape's oriented bounding box tree, used for
	collision tests between shapes and for ray picks: the tree, the shape's
	world transform and velocity, and a stamp that changes with the
	transform so that each node moves its box into world space only once
	per change.

	Evidence: NiTriShape's slot 23 (0x7F30D0) allocates 0x54 bytes and calls
	the constructor (0x8235F0) with the triangle count, the triangle list,
	the model and world vertices and its own argument, which the node
	constructor compares range sizes with (the bin size); the constructor
	stores the vtable 0x89A79C (4 slots) and builds the tree node at 0x08
	(an NiOBBNode, 0x8C bytes, for more than one triangle, else an
	NiOBBLeaf, 0x98 bytes), copies the zero point to 0x10 and 0x40 and the
	identity matrix to 0x1C and sets 1.0 at 0x4C; slot 0 (0x823710) deletes
	the node through its vtable and frees the root. NiTriBasedGeom keeps the
	root at 0xE0 ("m_pOBBRoot"), stores its time step at 0x04 and its world
	velocity at 0x10 before passing its world transform to 0x823760, which
	sets the flag at 0x0C and advances the stamp at 0x50 when the transform
	changed. The class name follows the Gamebryo lineage (NiOBBRoot,
	m_pOBBRoot); member and method names are invented after it.

===============================================================================
*/

#ifndef __NIOBBROOT_H__
#define __NIOBBROOT_H__

#include "NiMatrix3.h"
#include "NiPoint3.h"

class NiAVObject;
class NiOBBNode;
class NiPick;

// FUNCTION: STBC 0x823710 ??_GNiOBBRoot@@UAEPAXI@Z
class NiOBBRoot {
public:
							NiOBBRoot( unsigned short triangleCount, const unsigned short *triList, const NiPoint3 *vertices,
									   const NiPoint3 *worldVertices, int binSize );
	virtual					~NiOBBRoot();

	// whether the shapes' trees overlap, down to the given depths
	virtual bool			TestCollision( NiOBBRoot *other, int depth, int otherDepth );

	// reports the colliding triangle pairs of the shapes' trees
	virtual int				FindCollisions( NiOBBRoot *other, NiAVObject *root0, NiAVObject *root1,
											NiAVObject *object0, NiAVObject *object1, int depth, int otherDepth );

	// adds the triangles a ray hits to the pick; found is set when the
	// last triangle tested is hit
	virtual int				FindIntersections( const NiPoint3 &origin, const NiPoint3 &direction, NiPick &pick,
											   bool &found, NiAVObject *object, unsigned int stamp );

	// takes the shape's world transform; the tree follows it when next tested
	void					Transform( const NiMatrix3 &rotate_, const NiPoint3 &translate_, float scale_ );

	float					GetDeltaTime() const { return deltaTime; }
	void					SetDeltaTime( float deltaTime_ ) { deltaTime = deltaTime_; }
	const NiPoint3 &		GetVelocity() const { return velocity; }
	void					SetVelocity( const NiPoint3 &velocity_ ) { velocity = velocity_; }
	unsigned int			GetChangeStamp() const { return changeStamp; }

protected:
	// the leaves read the step and the velocity when they sweep their triangles
	friend class NiOBBLeaf;

	// moves the tree's boxes into world space if the transform changed
	// since they were last moved
	void					UpdateTree();

	float					deltaTime;			// the time step the velocity moves over
	NiOBBNode *				tree;				// owned
	bool					transformChanged;	// the tree's boxes lag the transform
	NiPoint3				velocity;			// the shape's world velocity
	NiMatrix3				rotate;				// the shape's world transform
	NiPoint3				translate;
	float					scale;
	unsigned int			changeStamp;		// advanced with each transform change
};

#endif /* !__NIOBBROOT_H__ */
