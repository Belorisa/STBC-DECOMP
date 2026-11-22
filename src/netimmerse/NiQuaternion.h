/*
===============================================================================

	NiQuaternion.h

	A rotation as a quaternion w + xi + yj + zk.

	Evidence: NiQuaternion.cpp's viewer string names the components
	("(w=%f,x=%f,y=%f,z=%f)") in their storage order; the static
	initializer builds the identity (1, 0, 0, 0); the streaming functions
	read and write four floats.

===============================================================================
*/

#ifndef __NIQUATERNION_H__
#define __NIQUATERNION_H__

#include "NiPoint3.h"

class NiMatrix3;
class NiStream;

class NiQuaternion {
public:
	float			w;
	float			x;
	float			y;
	float			z;

	float			GetW( void ) const { return w; }
	float			GetX( void ) const { return x; }
	float			GetY( void ) const { return y; }
	float			GetZ( void ) const { return z; }
	void			SetW( float w_ ) { w = w_; }

					NiQuaternion() {}
					NiQuaternion( float w_, float x_, float y_, float z_ );
					NiQuaternion( float angle, const NiPoint3 &axis );
					// out of line, as is the assignment: the rotation keys
					// and game code call them (NiQuaternionMath.cpp)
					NiQuaternion( const NiQuaternion &q );

	NiQuaternion &	operator=( const NiQuaternion &q );

	NiQuaternion	operator+( const NiQuaternion &q ) const;
	NiQuaternion	operator-( const NiQuaternion &q ) const;
	NiQuaternion	operator-() const;
	NiQuaternion	operator*( const NiQuaternion &q ) const;
	NiQuaternion	operator*( float c ) const;
	friend NiQuaternion	operator*( float c, const NiQuaternion &q );

	static NiQuaternion	UnitInverse( const NiQuaternion &q );

	// the logarithm of a unit quaternion and the exponential of a pure
	// one (Gamebryo lineage names)
	static NiQuaternion	Log( const NiQuaternion &q );
	static NiQuaternion	Exp( const NiQuaternion &q );
	static float		Dot( const NiQuaternion &p, const NiQuaternion &q );

	// spherical linear interpolation from p (t = 0) to q (t = 1)
	static NiQuaternion	Slerp( float t, const NiQuaternion &p, const NiQuaternion &q );

	// the inner control point of a squad curve at q1 between q0 and q2,
	// and the squad curve itself from p (t = 0) through the inner points a
	// and b to q (t = 1) (Gamebryo lineage names)
	static NiQuaternion	Intermediate( const NiQuaternion &q0, const NiQuaternion &q1, const NiQuaternion &q2 );
	static NiQuaternion	Squad( float t, const NiQuaternion &p, const NiQuaternion &a, const NiQuaternion &b, const NiQuaternion &q );

	// the rotation by angle (radians) about the unit axis, and back
	void			FromAngleAxis( float angle, const NiPoint3 &axis );
	void			ToAngleAxis( float &angle, NiPoint3 &axis ) const;

	// the unit quaternion of a rotation matrix (Gamebryo lineage name)
	void			FromRotation( const NiMatrix3 &rot );

	// the rotation matrix of this unit quaternion (Gamebryo lineage name)
	void			ToRotation( NiMatrix3 &rot ) const;

	// streaming
	void			LoadBinary( NiStream &stream );
	void			SaveBinary( NiStream &stream );
	char *			GetViewerString( const char *prefix ) const;

	static const NiQuaternion	IDENTITY;

	// the cutoff for a sine near zero (Gamebryo 1.1.2: ms_fEpsilon)
	static const float	epsilon;
};

#endif /* !__NIQUATERNION_H__ */
