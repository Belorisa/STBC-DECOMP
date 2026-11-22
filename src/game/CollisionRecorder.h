/*
===============================================================================

	CollisionRecorder.h

	Collects the points of contact the collision tests find, one record
	per test of two objects; the proximity code then reads the points of
	the pair that collided. Both class names are invented: no string or
	SWIG type names them.

	Evidence: the collision check 0x5A88E0 builds a recorder on its stack
	(constructor 0x58A1A0, destructor 0x58A1C0, also its unwind action) and
	passes it to DamageableObject's slot 82; the collision tests begin a
	record (0x58A2D0) with a type and the two objects' DamageableObjects,
	add copies of the points that hit (0x58A430) and end it (0x58A330),
	which keeps it only when it holds points. DamageableObject's slot 84
	(0x594840) asks for the points of type 2 between itself and the other
	object (0x58A4A0). A recorder is 0x58 bytes: the constructor clears the
	byte at 0x00, the record at 0x3C and the list at 0x40 (two items per
	block). A record is 0x24 bytes (BeginRecord allocates it): the type,
	the two objects and a list of points at 0x0C (two items per block).

	Between 0x04 and 0x3C the recorder holds the motion of the two objects
	before the collision. The proximity code stores them (0x5A8C70, called
	twice from 0x5A88E0 with index 0 and 1, each with a 0x1C-byte argument
	built from the object's mass 0xD8, GetVelocity and angular axis 0xCC
	times angular speed 0xE0), and DamageableObject's slot 84 copies the
	state of index and of the other one (the copy constructor 0x595250) and
	weighs the masses against each other.

===============================================================================
*/

#ifndef __COLLISIONRECORDER_H__
#define __COLLISIONRECORDER_H__

#include "NiPoint3.h"
#include "NiTList.h"

class DamageableObject;

// The points of contact one test found between two objects (invented name).
// The recorder owns the points.
// FUNCTION: STBC 0x0058a3e0 ??1CollisionRecord@@QAE@XZ
class CollisionRecord {
public:
					CollisionRecord( void ) : points( 2 ) {}

	int				type;				// 0x00 the kind of test; the proximity code reads type 2
	DamageableObject *	objectA;		// 0x04
	DamageableObject *	objectB;		// 0x08
	NiTList<NiPoint3 *>	points;			// 0x0C
};

// An object's motion as a collision found it (invented name).
// FUNCTION: STBC 0x00595250 ??0CollisionState@@QAE@ABU0@@Z @game/DamageableObject
struct CollisionState {
					CollisionState( void ) {}
					CollisionState( float mass_, const NiPoint3 &velocity_, const NiPoint3 &angularVelocity_ ) : mass( mass_ ), velocity( velocity_ ), angularVelocity( angularVelocity_ ) {}

	float			mass;				// 0x00
	NiPoint3		velocity;			// 0x04
	NiPoint3		angularVelocity;	// 0x10 the angular axis times the angular speed
};

// invented name
class CollisionRecorder {
public:
					CollisionRecorder( void );
					~CollisionRecorder( void );

	// Starts the record of a test, ending the one in progress.
	void			BeginRecord( int type, DamageableObject *objectA, DamageableObject *objectB );
	// Keeps the record in progress if it found points, else deletes it.
	void			EndRecord( void );
	// Adds a point to the record in progress; the recorder owns it.
	void			AddPoint( NiPoint3 *point );
	// Adds the points of the records of the type between the two objects to
	// the list (records of other types than 1 match the pair either way
	// round); true when the list holds points.
	bool			GetPoints( NiTList<NiPoint3 *> *list, int type, DamageableObject *objectA, DamageableObject *objectB );
	// The motion of the pair's first (0) or second (1) object (invented
	// names). The proximity code's unit keeps SetState's out-of-line copy
	// (0x5A8C70).
	// FUNCTION: STBC 0x005a8c70 ?SetState@CollisionRecorder@@QAEXHUCollisionState@@@Z @game/ProximityManager
	void			SetState( int index, CollisionState state ) { states[index] = state; }
	CollisionState	GetState( int index ) { return states[index]; }

protected:
	bool			unknown00;			// 0x00 false at first
	CollisionState	states[2];			// 0x04 not touched by the recorder's own code
	CollisionRecord *	current;		// 0x3C the record in progress, or NULL
	NiTList<CollisionRecord *>	records;	// 0x40
};

#endif /* !__COLLISIONRECORDER_H__ */
