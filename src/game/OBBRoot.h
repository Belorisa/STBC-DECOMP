// the leaf pairs OBBNode's pair test has reached; OBBModel's model test
// resets it (defined in OBBNode.cpp; invented name)
extern int obbLeafTests;

/*
===============================================================================

	OBBRoot.h

	The extra data block that gives a triangle shape a tree of oriented
	bounding boxes around its triangles, for line and collision tests
	against the model (OBBModel).

	0x18 bytes. Evidence: its NiRTTI at 0x981330 is built with the name
	"OBBRoot" (0x8DB128) and NiExtraData's NiRTTI as its base (0x463F70).
	The constructor (0x464D60) runs NiExtraData's, builds the tree
	(0x464BD0, 0x14 bytes) from the shape and stores the vtable 0x88BA60;
	the vtable overrides GetRTTI (0x464DE0), CreateClone (0x464E80) and the
	destructor (0x464DF0). The tree is an NiObject (vtable 0x88BA30) that
	overrides only its destructor (0x464CD0). The tree's name is from
	DamageTool.exe (the BC SDK's damage tool, built with RTTI and a COFF
	symbol table): its RTTI names OBBRootData : NiObject with NiObject's slots and
	its own destructor, and its ??0OBBRootData(NiTriShape *, unsigned int)
	(0x418A90) and ??1OBBRootData (0x418BB0) have this class's constructor's
	and destructor's instructions.

===============================================================================
*/

#ifndef __OBBROOT_H__
#define __OBBROOT_H__

#include "NiExtraData.h"
#include "NiPointer.h"
#include "NiTArray.h"
#include "OBBNode.h"

class NiAVObject;
class NiPick;
class NiPlane;
class NiPoint3;
class NiTriShape;

// The tree of one shape's triangles. 0x14 bytes.
class OBBRootData : public NiObject {
public:
						OBBRootData( NiTriShape *shape, unsigned int leafSize );
	virtual				~OBBRootData();

	OBBNode *			GetRoot() { return root; }		// invented name
	unsigned short *	GetIndices() { return indices; }	// invented name

protected:
	OBBNode *			root;			// 0x08 owned: the top node
	unsigned short *	indices;		// 0x0C owned: one per triangle
	NiPlane *			planes;			// 0x10 owned: one per triangle
};

// FUNCTION: STBC 0x00464df0 ??_GOBBRoot@@UAEPAXI@Z
// FUNCTION: STBC 0x00464e10 ??1OBBRoot@@UAE@XZ
class OBBRoot : public NiExtraData {
public:
	// FUNCTION: STBC 0x00464de0 ?GetRTTI@OBBRoot@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

							OBBRoot( NiTriShape *shape, unsigned int leafSize );
							// an empty block for CreateClone to fill
							OBBRoot() {}

	// The tests against the shape's tree: the ray, the point and the
	// sphere are taken into the shape's model space first; true when
	// something was hit. The ray, segment and pair tests are named from
	// DamageTool.exe's OBBRoot (FindRayIntersections and
	// FindSegmentIntersections with an IntersectionModifier * callback,
	// FindRoughOBBIntersections; 0x418DA0-0x419140), which forward to the
	// node slots of the same names as these do; the sphere and box tests
	// have no counterpart there.
	bool					FindRayIntersections( NiTriShape *shape, const NiPoint3 &origin, const NiPoint3 &direction,
										NiPick &pick, VisibleDamage *callback );
	bool					FindSegmentIntersections( NiTriShape *shape, const NiPoint3 &origin, const NiPoint3 &direction,
										NiPick &pick, VisibleDamage *callback );
	bool					Unknown465150( NiTriShape *shape, const NiPoint3 &center, float radius, NiTArray<OBBTriangleList *> *lists );
	bool					FindRoughOBBIntersections( NiTriShape *shape, NiTriShape *otherShape, OBBRoot *other, CollisionRecorder *recorder, VisibleDamage *damage, VisibleDamage *otherDamage );
	// Gathers the triangles that meet a world box into the shape's list
	// (the tree's slot 6, with the box taken into the shape's model space);
	// with depth below 1, every triangle of the shape.
	bool					Unknown465250( NiTriShape *shape, const NiBox &box, NiTArray<OBBTriangleList *> *lists, int depth,
										VisibleDamage *callback );


	static const NiRTTI		rtti;

protected:
	void					CopyMembers( OBBRoot *dest );

	NiPointer<OBBRootData>		tree;		// 0x14
};

#endif /* !__OBBROOT_H__ */
