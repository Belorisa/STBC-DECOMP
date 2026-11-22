/*
===============================================================================

	CollisionTestDisplay.h

	The debugging display of the collision tests: while it is on, the
	collision code draws the points it tests under a node of its own in the
	rendered set's scene. The unit's name is invented.

	Evidence: the SWIG wrapper DisplayCollisionTests (0x5BC990) calls
	0x588000 with its argument as a bool. 0x588000 creates an NiNode (0xE8
	bytes, constructor 0x7E37C0) into the static NiPointer at 0x98D5E4
	(cleared by the static initialiser 0x587FB0 and released at exit by
	0x587FD0) and attaches it under the rendered set's scene root (SetClass
	0xA0); turning the display off detaches it from its parent and releases
	it. The collision tests (0x588380) add a particle system of each tested
	point under that node.

===============================================================================
*/

#ifndef __COLLISIONTESTDISPLAY_H__
#define __COLLISIONTESTDISPLAY_H__

// Shows or hides the points the collision tests try.
void	DisplayCollisionTests( bool show );

class CollisionRecorder;
class DamageCollisionTest;
class DamageableObject;
class OBBModel;
class VisibleDamage;

// One side of a collision test, which the two sides run by double
// dispatch: slot 0 hands the other side this one as its concrete type, and
// the other side's slot for that type runs the test (invented names).
// Evidence: the vtable 0x893F10 (three _purecall slots) follows
// DamageCollisionTest's, and DamageCollisionTest's implicit destructor
// (0x594530, kept in DamageableObject's unit, where the unwinding of slots
// 82 and 83 calls it) only stores it; the constructor's store of the
// object comes before the derived vtable's.
class CollisionTest {
public:
	CollisionTest( DamageableObject *object_ ) { object = object_; }
	~CollisionTest() {}

	virtual bool	VirtualSlot00( CollisionTest *other, CollisionRecorder *recorder ) = 0;
	virtual bool	VirtualSlot01( DamageCollisionTest *other, CollisionRecorder *recorder ) = 0;
	virtual bool	VirtualSlot02( DamageCollisionTest *other, CollisionRecorder *recorder ) = 0;

	DamageableObject *	object;			// 0x04
};

// A DamageableObject's side of a collision test (invented name):
// DamageableObject's slots 82 and 83 (0x594440, 0x5945B0) build one on the
// stack for the object, with its first OBB model (0x594540), the first of
// its visible damages whose polygonizer is set up, and its unknown1B4, and
// hand it to the other object. 0x14 bytes; vtable 0x893F04: slot 0
// (0x594510, kept in DamageableObject's unit) dispatches to the other
// side's slot 1, which tests the vertices of one side's model against the
// other side's damage; slot 2 (0x589710) finds nothing.
// FUNCTION: STBC 0x00594530 ??1DamageCollisionTest@@QAE@XZ @game/DamageableObject
class DamageCollisionTest : public CollisionTest {
public:
	DamageCollisionTest( DamageableObject *object_, OBBModel *obbModel_, VisibleDamage *damage_, int unknown10_ ) : CollisionTest( object_ ) {
		obbModel = obbModel_;
		damage = damage_;
		unknown10 = unknown10_;
	}

	// Lets the other side test itself against this one.
	// FUNCTION: STBC 0x00594510 ?VirtualSlot00@DamageCollisionTest@@UAE_NPAVCollisionTest@@PAVCollisionRecorder@@@Z @game/DamageableObject
	virtual bool	VirtualSlot00( CollisionTest *other, CollisionRecorder *recorder ) { return other->VirtualSlot01( this, recorder ); }
	// Tests the model of the side with the smaller bound against the damage
	// of the other, recording the points that hit as type 2.
	virtual bool	VirtualSlot01( DamageCollisionTest *other, CollisionRecorder *recorder );
	virtual bool	VirtualSlot02( DamageCollisionTest *other, CollisionRecorder *recorder );

	// Test the model's side against the damaged side: by vertex when the
	// model's side has unknown10 clear, else by 0x5884A0's test.
	bool			TestVertices( DamageableObject *modelObject, OBBModel *model, DamageCollisionTest *modelSide, DamageableObject *damagedObject, OBBModel *damagedModel, VisibleDamage *damage, CollisionRecorder *recorder );
	bool			Unknown5884A0( DamageableObject *modelObject, OBBModel *model, DamageCollisionTest *modelSide, DamageableObject *damagedObject, OBBModel *damagedModel, VisibleDamage *damage, CollisionRecorder *recorder );

	OBBModel *		obbModel;			// 0x08
	VisibleDamage *	damage;				// 0x0C NULL when no damage is set up
	int				unknown10;			// 0x10 the object's unknown1B4
};

#endif /* !__COLLISIONTESTDISPLAY_H__ */
