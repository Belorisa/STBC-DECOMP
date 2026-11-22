/*
===============================================================================

	Force.h

	A force applied to a physics object at a point (the class
	PhysicsObjectClass::ApplyForce, 0x5A0790, takes; its SWIG wrapper
	PhysicsObjectClass_ApplyForce names the type). 0x24 bytes: the point,
	the force and the torque it adds. Evidence: the constructor (0x5965F0)
	copies three points, the copy constructor (0x596640) nine floats, the
	destructor (0x596680) is empty, and 0x596690 moves the point, adding
	the torque of the force about the new point.

===============================================================================
*/

#ifndef __FORCE_H__
#define __FORCE_H__

#include "NiPoint3.h"

class Force {
public:
					Force( void );
					Force( const NiPoint3 &position_, const NiPoint3 &force_, const NiPoint3 &torque_ );
					Force( const Force &other );
					~Force( void );

	// Moves the point the force acts at, adding the torque the force has
	// about the new point (0x596690; invented name).
	void			MoveTo( const NiPoint3 &position_ );

	// Adding a force adds the torque it has about this force's point.
	void			operator+=( const Force &other );
	void			operator-=( const Force &other );
	void			operator*=( float scale );
	Force			operator+( const Force &other ) const;
	Force			operator-( const Force &other ) const;
	Force			operator*( float scale ) const;
	void			SetPosition( const NiPoint3 &position_ ) { position = position_; }	// invented name
	const NiPoint3 &	GetForce( void ) const { return force; }		// invented name
	void			SetForce( const NiPoint3 &force_ ) { force = force_; }		// invented name
	const NiPoint3 &	GetTorque( void ) const { return torque; }		// invented name
	void			SetTorque( const NiPoint3 &torque_ ) { torque = torque_; }	// invented name

protected:
	NiPoint3		position;			// 0x00 where the force acts, in world space (invented name)
	NiPoint3		force;				// 0x0C (invented name)
	NiPoint3		torque;				// 0x18 (invented name)
};

#endif /* !__FORCE_H__ */
