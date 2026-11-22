/*
===============================================================================

	OBBModel.h

	The triangle shapes of a model, each given an OBBRoot tree, gathered for
	line and collision tests against the whole model; also the box around
	all of the model's vertices, in the model's own space.

	0x7C bytes. Evidence: the constructor (0x463FC0) runs NiObject's, holds
	the model in an NiPointer at 0x5C, sets up an NiTArray at 0x60 (vtable
	0x88BA2C), stores the vtable 0x88B9FC, then walks the model (0x464140)
	and computes the box (0x464330). The vtable keeps NiObject's slots and
	replaces only the destructor (0x46C380, which the model viewer's unit
	keeps). The voxelizer (0x4AECE0) builds one with a leaf size of 8 and
	tests its segments with 0x4648E0; the damage geometry's clone
	(0x4B1550) builds one too. No string names the class: OBBModel and the
	method names are invented.

===============================================================================
*/

#ifndef __OBBMODEL_H__
#define __OBBMODEL_H__

#include "NiObject.h"
#include "NiPointer.h"
#include "NiTArray.h"
#include "NiPoint3.h"
#include "NiBox.h"

class NiAVObject;
class NiPick;
class NiTriShape;
class VisibleDamage;
class CollisionRecorder;
struct OBBTriangleList;

// FUNCTION: STBC 0x0046c380 ??_GOBBModel@@UAEPAXI@Z
class OBBModel : public NiObject {
public:
	// leafSize is passed to each new OBBRoot; with 0 the shapes that have
	// none are left out
						OBBModel( NiAVObject *root_, unsigned int leafSize );
	// the copy that the model viewer's unit keeps
	// FUNCTION: STBC 0x0046c3a0 ??1OBBModel@@UAE@XZ
	virtual				~OBBModel() {}

	// gathers the shapes again, keeping only those that have an OBBRoot
	void				RefreshShapes();

	// The tests of OBBRoot over the shapes, true when any shape was hit.
	// The ray tests pass the callback object down and sort the pick's
	// results; the sphere test gathers the triangles it touches into
	// lists, one per shape. The names are DamageTool.exe's (OBBForest's
	// FindRayIntersections, FindSegmentIntersections, FindSphereIntersections,
	// FindRoughOBBIntersections and FindBoxIntersections, with these
	// parameters but the pair test's NiPick), each one forwarding to the
	// OBB node slot of that name.
	bool				FindRayIntersections( const NiPoint3 &origin, const NiPoint3 &direction, NiPick &pick );
	bool				FindSegmentIntersections( const NiPoint3 &origin, const NiPoint3 &direction, NiPick &pick );
	bool				FindSphereIntersections( const NiPoint3 &center, float radius, NiTArray<OBBTriangleList *> *lists );
	// the shape pairs of this model and other: OBBRoot's pair test for each
	// pair of shapes that have one, with both models' callback objects
	bool				FindRoughOBBIntersections( OBBModel *other, CollisionRecorder *recorder );
	// the box test: the triangles that meet the world box, gathered with
	// depth - 1 (the damage's segment tests call it)
	bool				FindBoxIntersections( const NiBox &box, NiTArray<OBBTriangleList *> *lists, int depth );

	// true when every triangle shape under object has an OBBRoot
	static bool			HasOBBRoots( NiAVObject *object );

	// the axis-aligned box around the shapes (invented names)
	const NiPoint3 &	GetCenter() const { return center; }
	const NiPoint3 &	GetHalfExtent() const { return halfExtent; }

protected:
	// VisibleDamage sets and clears the callback directly (0x4B1F20,
	// 0x4B13A0)
	friend class VisibleDamage;
	// and the damage collision test walks the shapes' vertices (0x588280)
	friend class DamageCollisionTest;
	// the LOD loader hands the level's visible damage to its collision
	// model (0x451119)
	friend class LODModelPart;

	// adds the triangle shapes under object, giving those without an
	// OBBRoot one when leafSize is not 0
	void				AddShapes( NiAVObject *object, unsigned int leafSize );
	// orders the pick's results nearest first, or keeps only the nearest
	// for a FIND_FIRST pick, as NiPick does after its own search
	// (Gamebryo 1.2.2: the end of NiPick::PickObjects; invented name)
	void				SortResults( NiPick &pick );
	// fits the boxes around the shapes' world vertices, taken into root_'s
	// model space
	void				ComputeBound( NiAVObject *root_ );

	NiBox				box;					// 0x08 the oriented box around the vertices
	NiPoint3			center;					// 0x44 the axis-aligned box around them
	NiPoint3			halfExtent;				// 0x50
	NiPointer<NiAVObject>	root;				// 0x5C the model
	NiTArray<NiTriShape *>	shapes;				// 0x60 the shapes that have an OBBRoot
	VisibleDamage *		callback;				// 0x78 the ray tests' callback object, or NULL (invented name)
};

#endif /* !__OBBMODEL_H__ */
