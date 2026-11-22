// NiQuaternion.cpp -- rotations as quaternions

#include "NiRTLib.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "NiMath.h"
#include "NiMatrix3.h"
#include "NiQuaternion.h"
#include "NiStream.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x008165d0 _$E49
// FUNCTION: STBC 0x008165f0 _$E17
// FUNCTION: STBC 0x00816610 _$E15
// FUNCTION: STBC 0x00816620 _$E23
// FUNCTION: STBC 0x00816640 _$E21

// DATA: STBC 0x9A2950 ?IDENTITY@NiQuaternion@@2V1@B
const NiQuaternion NiQuaternion::IDENTITY( 1.0f, 0.0f, 0.0f, 0.0f );

// DATA: STBC 0x89A748 ?epsilon@NiQuaternion@@2MB
const float NiQuaternion::epsilon = 0.001f;

/*
================
NiQuaternion::NiQuaternion
================
*/
// FUNCTION: STBC 0x815920 ??0NiQuaternion@@QAE@MMMM@Z
NiQuaternion::NiQuaternion( float w_, float x_, float y_, float z_ ) {
	w = w_;
	x = x_;
	y = y_;
	z = z_;
}

/*
================
NiQuaternion::NiQuaternion

The rotation by angle about the unit axis.
================
*/
// FUNCTION: STBC 0x815940 ??0NiQuaternion@@QAE@MABVNiPoint3@@@Z
NiQuaternion::NiQuaternion( float angle, const NiPoint3 &axis ) {
	float halfAngle = angle * 0.5f;
	float sn = (float)sin( halfAngle );
	w = (float)cos( halfAngle );
	x = sn * axis.x;
	y = sn * axis.y;
	z = sn * axis.z;
}

/*
================
NiQuaternion::operator+
================
*/
// FUNCTION: STBC 0x815980 ??HNiQuaternion@@QBE?AV0@ABV0@@Z
NiQuaternion NiQuaternion::operator+( const NiQuaternion &q ) const {
	return NiQuaternion( w + q.w, x + q.x, y + q.y, z + q.z );
}

/*
================
NiQuaternion::operator-
================
*/
// FUNCTION: STBC 0x8159B0 ??GNiQuaternion@@QBE?AV0@ABV0@@Z
NiQuaternion NiQuaternion::operator-( const NiQuaternion &q ) const {
	return NiQuaternion( w - q.w, x - q.x, y - q.y, z - q.z );
}

/*
================
NiQuaternion::operator-
================
*/
// FUNCTION: STBC 0x8159E0 ??GNiQuaternion@@QBE?AV0@XZ
NiQuaternion NiQuaternion::operator-() const {
	return NiQuaternion( -w, -x, -y, -z );
}

/*
================
NiQuaternion::operator*

The product applies q first, then this rotation.
================
*/
// FUNCTION: STBC 0x815A10 ??DNiQuaternion@@QBE?AV0@ABV0@@Z
NiQuaternion NiQuaternion::operator*( const NiQuaternion &q ) const {
	return NiQuaternion(
		w * q.w - x * q.x - y * q.y - z * q.z,
		w * q.x + x * q.w + y * q.z - z * q.y,
		w * q.y + y * q.w + z * q.x - x * q.z,
		w * q.z + z * q.w + x * q.y - y * q.x );
}

/*
================
NiQuaternion::UnitInverse

The inverse of a unit quaternion: its conjugate.
================
*/
// FUNCTION: STBC 0x815AA0
NiQuaternion NiQuaternion::UnitInverse( const NiQuaternion &q ) {
	return NiQuaternion( q.w, -q.x, -q.y, -q.z );
}

/*
================
NiQuaternion::Log

The pure quaternion of the half angle along the axis; near the identity the
sine is too small to divide by and the axis is taken as it is.
================
*/
// FUNCTION: STBC 0x815AD0
NiQuaternion NiQuaternion::Log( const NiQuaternion &q ) {
	float angle = NiACos( q.w );
	float sn = (float)sin( angle );
	float coeff;
	if ( fabsf( sn ) < epsilon ) {
		coeff = 1.0f;
	} else {
		coeff = angle / sn;
	}
	return NiQuaternion( 0.0f, coeff * q.x, coeff * q.y, coeff * q.z );
}

/*
================
NiQuaternion::Slerp

Answers p itself when the two are too close for the sine to divide by.
================
*/
// FUNCTION: STBC 0x815B80
NiQuaternion NiQuaternion::Slerp( float t, const NiQuaternion &p, const NiQuaternion &q ) {
	float angle = NiACos( Dot( p, q ) );
	float sn = (float)sin( angle );
	if ( sn < epsilon ) {
		return p;
	}
	float invSn = 1.0f / sn;
	float coeff0 = (float)sin( ( 1.0f - t ) * angle ) * invSn;
	float coeff1 = (float)sin( t * angle ) * invSn;
	return coeff0 * p + coeff1 * q;
}

/*
================
NiQuaternion::Intermediate

The inner control point at q1 that makes a squad curve through q0, q1 and
q2 smooth at q1.

Gamebryo 1.2.2: NiQuaternion::Intermediate names the exponential.
================
*/
// FUNCTION: STBC 0x815C90
NiQuaternion NiQuaternion::Intermediate( const NiQuaternion &q0, const NiQuaternion &q1, const NiQuaternion &q2 ) {
	NiQuaternion inv = UnitInverse( q1 );
	NiQuaternion exp = Exp( -0.25f * ( Log( inv * q0 ) + Log( inv * q2 ) ) );
	return q1 * exp;
}

/*
================
NiQuaternion::Squad
================
*/
// FUNCTION: STBC 0x815E60
NiQuaternion NiQuaternion::Squad( float t, const NiQuaternion &p, const NiQuaternion &a, const NiQuaternion &b, const NiQuaternion &q ) {
	return Slerp( 2.0f * t * ( 1.0f - t ), Slerp( t, p, q ), Slerp( t, a, b ) );
}

/*
================
NiQuaternion::FromAngleAxis
================
*/
// FUNCTION: STBC 0x8161A0
void NiQuaternion::FromAngleAxis( float angle, const NiPoint3 &axis ) {
	float halfAngle = angle * 0.5f;
	float sn = (float)sin( halfAngle );
	w = (float)cos( halfAngle );
	x = sn * axis.x;
	y = sn * axis.y;
	z = sn * axis.z;
}

/*
================
NiQuaternion::ToAngleAxis

A rotation too small to have an axis answers no angle about the zero axis.
================
*/
// FUNCTION: STBC 0x8161E0
void NiQuaternion::ToAngleAxis( float &angle, NiPoint3 &axis ) const {
	float length = (float)sqrt( x * x + y * y + z * z );
	if ( length < epsilon ) {
		angle = 0.0f;
		axis.x = 0.0f;
		axis.y = 0.0f;
		axis.z = 0.0f;
	} else {
		angle = 2.0f * NiACos( w );
		float invLength = 1.0f / length;
		axis.x = invLength * x;
		axis.y = invLength * y;
		axis.z = invLength * z;
	}
}

/*
================
NiQuaternion::ToRotation
================
*/
// FUNCTION: STBC 0x8162B0
void NiQuaternion::ToRotation( NiMatrix3 &rot ) const {
	float tx = 2.0f * x;
	float ty = 2.0f * y;
	float tz = 2.0f * z;
	float twx = tx * w;
	float twy = ty * w;
	float twz = tz * w;
	float txx = tx * x;
	float txy = ty * x;
	float txz = tz * x;
	float tyy = ty * y;
	float tyz = tz * y;
	float tzz = tz * z;

	rot.SetEntry( 0, 0, 1.0f - ( tyy + tzz ) );
	rot.SetEntry( 1, 0, txy + twz );
	rot.SetEntry( 2, 0, txz - twy );
	rot.SetEntry( 0, 1, txy - twz );
	rot.SetEntry( 1, 1, 1.0f - ( txx + tzz ) );
	rot.SetEntry( 2, 1, tyz + twx );
	rot.SetEntry( 0, 2, txz + twy );
	rot.SetEntry( 1, 2, tyz - twx );
	rot.SetEntry( 2, 2, 1.0f - ( txx + tyy ) );
}

/*
================
NiQuaternion::FromRotation

Ken Shoemake's method: from the trace when w is large, otherwise from the
largest diagonal entry.
================
*/
// FUNCTION: STBC 0x816390
void NiQuaternion::FromRotation( const NiMatrix3 &rot ) {
	float trace = rot.GetEntry( 0, 0 ) + rot.GetEntry( 1, 1 ) + rot.GetEntry( 2, 2 );
	float root;

	if ( trace > 0.0f ) {
		root = sqrtf( trace + 1.0f );
		w = 0.5f * root;
		root = 0.5f / root;
		x = ( rot.GetEntry( 2, 1 ) - rot.GetEntry( 1, 2 ) ) * root;
		y = ( rot.GetEntry( 0, 2 ) - rot.GetEntry( 2, 0 ) ) * root;
		z = ( rot.GetEntry( 1, 0 ) - rot.GetEntry( 0, 1 ) ) * root;
	} else {
		static int next[3] = { 1, 2, 0 };
		int i = 0;
		if ( rot.GetEntry( 1, 1 ) > rot.GetEntry( 0, 0 ) ) {
			i = 1;
		}
		if ( rot.GetEntry( 2, 2 ) > rot.GetEntry( i, i ) ) {
			i = 2;
		}
		int j = next[i];
		int k = next[j];

		root = sqrtf( rot.GetEntry( i, i ) - rot.GetEntry( j, j ) - rot.GetEntry( k, k ) + 1.0f );
		float *quat[3] = { &x, &y, &z };
		*quat[i] = 0.5f * root;
		root = 0.5f / root;
		w = ( rot.GetEntry( k, j ) - rot.GetEntry( j, k ) ) * root;
		*quat[j] = ( rot.GetEntry( j, i ) + rot.GetEntry( i, j ) ) * root;
		*quat[k] = ( rot.GetEntry( k, i ) + rot.GetEntry( i, k ) ) * root;
	}
}

/*
================
NiQuaternion::LoadBinary
================
*/
// FUNCTION: STBC 0x8164D0
void NiQuaternion::LoadBinary( NiStream &stream ) {
	NiStreamLoadBinary( stream, w );
	NiStreamLoadBinary( stream, x );
	NiStreamLoadBinary( stream, y );
	NiStreamLoadBinary( stream, z );
}

/*
================
NiQuaternion::SaveBinary
================
*/
// FUNCTION: STBC 0x816520
void NiQuaternion::SaveBinary( NiStream &stream ) {
	NiStreamSaveBinary( stream, w );
	NiStreamSaveBinary( stream, x );
	NiStreamSaveBinary( stream, y );
	NiStreamSaveBinary( stream, z );
}

/*
================
NiQuaternion::GetViewerString

Returns "<prefix>(w=..,x=..,y=..,z=..)" -- the prefix is ignored -- in a
new string that the caller deletes.
================
*/
// FUNCTION: STBC 0x816570
char *NiQuaternion::GetViewerString( const char *prefix ) const {
	char *string = new char[strlen( prefix ) + 64];
	sprintf( string, "(w=%f,x=%f,y=%f,z=%f)", w, x, y, z );
	return string;
}
