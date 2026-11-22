/*
===============================================================================

	CollisionEvent.h

	The event sent when two objects collide (ET_OBJECT_COLLISION): the
	points of contact and the force of the collision.

	0x44 bytes. Evidence: the constructor (0x586D00) runs TGEvent's, builds
	an NiTArray at 0x28 (one element, growing by one; its vtable 0x89399C)
	and clears the float at 0x40, then stores the vtable 0x89395C (16 slots:
	TGEvent's 15 and one new); the deleting destructor (0x586DF0) frees
	0x44 bytes to the pool, as the creator 0x43EE50 allocates. The class
	strings are "CollisionEvent", "_p_CollisionEvent" and
	"CollisionEventPtr"; GetObjType returns 0x8124. The SWIG inline
	accessors read GetNumPoints at 0x38 (the array's count of non-null
	elements) and GetCollisionForce at 0x40. The proximity code that sends
	it (0x594840) sets the event type 0x800050, the source, the destination,
	the force, and adds copies of the contact points.

===============================================================================
*/

#ifndef __COLLISIONEVENT_H__
#define __COLLISIONEVENT_H__

#include "TGEvent.h"
#include "NiPoint3.h"
#include "TGPoint3.h"
#include "NiTArray.h"

// GetObjType's value for a CollisionEvent (the slot 1 at 0x586D80).
#define COLLISIONEVENT_TYPE		0x8124

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00586df0 ??_GCollisionEvent@@UAEPAXI@Z
class CollisionEvent : public TGEvent {
public:
					CollisionEvent( long id = 0 );
	virtual			~CollisionEvent( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEvent
	virtual bool	Copy( TGEvent *event );
	virtual bool	WriteToNetStream( TGStream *stream );
	virtual bool	ReadFromNetStream( TGStream *stream );

	// Moves the points of contact from the destination's model space, as
	// the network carries them, back into world space (invented name).
	virtual bool	PointsToWorldSpace( void );

	int				GetNumPoints( void ) { return points.GetEffectiveSize(); }
	float			GetCollisionForce( void ) { return collisionForce; }
	void			SetCollisionForce( float force ) { collisionForce = force; }	// invented name
	// Adds a point of contact; the event owns it (invented name).
	void			AddPoint( NiPoint3 *point ) { points.Add( point ); }
	// FUNCTION: STBC 0x00595410 ?GetPoint@CollisionEvent@@QAE?AVTGPoint3@@H@Z @game/ShipClass
	TGPoint3		GetPoint( int index ) { return *points.GetAt( index ); }

protected:
	NiTArray<NiPoint3 *>	points;		// 0x28 owned, the points of contact (invented name)
	float			collisionForce;		// 0x40
};

#endif /* !__COLLISIONEVENT_H__ */
