/*
===============================================================================

	NiCollisionRecord.h

	One side of a collision test: the root of the tree being tested and the
	object of that tree the test has reached. The scene-graph objects pass
	a record per side down their children. The class name is invented
	after the Gamebryo lineage's NiCollisionGroup::Record; the member names
	are open.

	Evidence: NiNode's slot 32 (0x7E4B00) and slot 33 (0x7E4BD0) build a
	0x14-byte record for their children from the first record's words at 0,
	8 and 0x0C, with no object and 0 at 0x10, and store each child at 4;
	NiSwitchNode's (0x7FC480) builds it with the active child and -1 at
	0x10. The object at 4 of the other record receives the swapped test
	when this object is a leaf of its tree.

===============================================================================
*/

#ifndef __NICOLLISIONRECORD_H__
#define __NICOLLISIONRECORD_H__

#include "NiPoint3.h"

class NiAVObject;

// what the collision searches return: a callback's nonzero return stops
// the search (Gamebryo lineage names)
enum {
	CONTINUE_COLLISIONS,
	TERMINATE_COLLISIONS
};

class NiCollisionRecord {
public:
							NiCollisionRecord( NiAVObject *root_, NiAVObject *object_, int unknown08_, int unknown0C_, int unknown10_ ) {
								root = root_;
								object = object_;
								unknown08 = unknown08_;
								unknown0C = unknown0C_;
								unknown10 = unknown10_;
							}

	// the asserts name the object's getter ("record0.GetAVObject() ==
	// this", AnimOnly.exe 0x41DAD9)
	NiAVObject *			GetRoot() const { return root; }
	NiAVObject *			GetAVObject() const { return object; }
	void					SetObject( NiAVObject *object_ ) { object = object_; }
	// TODO identity: the getters of the two fields the children's records
	// take over (AnimOnly.exe 0x41FD70, 0x41FD90)
	int						GetUnknown08() const { return unknown08; }
	int						GetUnknown0C() const { return unknown0C; }

	NiAVObject *			root;
	NiAVObject *			object;
	int						unknown08;		// TODO identity: passed on to the children's records
	int						unknown0C;		// TODO identity: passed on to the children's records
	int						unknown10;		// TODO identity: 0 for a node's children, -1 for a switch's active child
};

/*
================
NiCollisionIntersect

A collision as the collision callbacks of the two objects receive it: the
roots of both trees, the two objects that collide, the time of contact
within the step, the contact point and each object's normal at it. The name
is invented after the Gamebryo lineage's NiCollisionGroup::Intersect, whose
layout it keeps.

Evidence: the OBB tree's leaf-pair test (0x828FB0) builds it on the stack
from its root and object arguments, the time and point of the box test and
the boxes' normals; its report (0x828F10) calls the callback at 0xB8 of the
first object and, with the sides swapped (the objects at 0 and 4, the
normals at 0x20 and 0x2C), of the second.
================
*/
struct NiCollisionIntersect {
	NiAVObject *			root0;
	NiAVObject *			root1;
	NiAVObject *			object0;
	NiAVObject *			object1;
	float					time;
	NiPoint3				point;
	NiPoint3				normal0;
	NiPoint3				normal1;
};

#endif /* !__NICOLLISIONRECORD_H__ */
