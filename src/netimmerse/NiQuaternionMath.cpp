// NiQuaternionMath.cpp -- the quaternion functions compiled apart from NiQuaternion.cpp
//
// The functions after NiQuaternion.cpp's static initializers (0x816650 to
// 0x8167DC) form an object of their own, with its own iostream
// initializers: they call the four-float constructor out of line, which
// NiQuaternion.cpp's own functions expand. The file name is invented.

#include "NiRTLib.h"
#include <math.h>

#include "NiQuaternion.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x008167e0 _$E37
// FUNCTION: STBC 0x00816800 _$E17
// FUNCTION: STBC 0x00816820 _$E15
// FUNCTION: STBC 0x00816830 _$E23
// FUNCTION: STBC 0x00816850 _$E21

/*
================
NiQuaternion::NiQuaternion
================
*/
// FUNCTION: STBC 0x816650 ??0NiQuaternion@@QAE@ABV0@@Z
NiQuaternion::NiQuaternion( const NiQuaternion &q ) {
	w = q.w;
	x = q.x;
	y = q.y;
	z = q.z;
}

/*
================
NiQuaternion::operator=
================
*/
// FUNCTION: STBC 0x816670 ??4NiQuaternion@@QAEAAV0@ABV0@@Z
NiQuaternion &NiQuaternion::operator=( const NiQuaternion &q ) {
	w = q.w;
	x = q.x;
	y = q.y;
	z = q.z;
	return *this;
}

/*
================
NiQuaternion::operator*
================
*/
// FUNCTION: STBC 0x816690 ??DNiQuaternion@@QBE?AV0@M@Z
NiQuaternion NiQuaternion::operator*( float c ) const {
	return NiQuaternion( c * w, c * x, c * y, c * z );
}

/*
================
operator*
================
*/
// FUNCTION: STBC 0x8166D0 ??D@YA?AVNiQuaternion@@MABV0@@Z
NiQuaternion operator*( float c, const NiQuaternion &q ) {
	return NiQuaternion( c * q.w, c * q.x, c * q.y, c * q.z );
}

/*
================
NiQuaternion::Dot
================
*/
// FUNCTION: STBC 0x816710
float NiQuaternion::Dot( const NiQuaternion &p, const NiQuaternion &q ) {
	return p.w * q.w + p.x * q.x + p.y * q.y + p.z * q.z;
}

/*
================
NiQuaternion::Exp

The rotation by twice the length of the vector part about it; near zero
the sine is too small to divide by and the vector is taken as it is.
================
*/
// FUNCTION: STBC 0x816740
NiQuaternion NiQuaternion::Exp( const NiQuaternion &q ) {
	float angle = sqrtf( q.x * q.x + q.y * q.y + q.z * q.z );
	float cs = (float)cos( angle );
	float sn = (float)sin( angle );
	float coeff;
	if ( fabsf( sn ) < epsilon ) {
		coeff = 1.0f;
	} else {
		coeff = sn / angle;
	}
	NiQuaternion result( cs, coeff * q.x, coeff * q.y, coeff * q.z );
	return result;
}
