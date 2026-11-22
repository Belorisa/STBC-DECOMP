// NiMatrix3.cpp -- 3x3 matrices

#include "NiRTLib.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "NiMatrix3.h"
#include "NiMath.h"
#include "NiStream.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x008158d0 _$E50
// FUNCTION: STBC 0x00815870 _$E17
// FUNCTION: STBC 0x00815890 _$E15
// FUNCTION: STBC 0x008158a0 _$E23
// FUNCTION: STBC 0x008158c0 _$E21

// FUNCTION: STBC 0x00813360 _$E40
// FUNCTION: STBC 0x008133c0 _$E43
// DATA: STBC 0x9A2900 ?ZERO@NiMatrix3@@2V1@B
const NiMatrix3 NiMatrix3::ZERO( NiPoint3( 0.0f, 0.0f, 0.0f ), NiPoint3( 0.0f, 0.0f, 0.0f ), NiPoint3( 0.0f, 0.0f, 0.0f ) );
// DATA: STBC 0x9A2928 ?IDENTITY@NiMatrix3@@2V1@B
const NiMatrix3 NiMatrix3::IDENTITY( NiPoint3( 1.0f, 0.0f, 0.0f ), NiPoint3( 0.0f, 1.0f, 0.0f ), NiPoint3( 0.0f, 0.0f, 1.0f ) );

/*
================
NiMatrix3::NiMatrix3

Builds the matrix from its three columns.
================
*/
// FUNCTION: STBC 0x813420 ??0NiMatrix3@@QAE@ABVNiPoint3@@00@Z
NiMatrix3::NiMatrix3( const NiPoint3 &col0, const NiPoint3 &col1, const NiPoint3 &col2 ) {
	SetCol( 0, col0 );
	SetCol( 1, col1 );
	SetCol( 2, col2 );
}

/*
================
NiMatrix3::NiMatrix3
================
*/
// FUNCTION: STBC 0x813470 ??0NiMatrix3@@QAE@ABV0@@Z
NiMatrix3::NiMatrix3( const NiMatrix3 &mat ) {
	memcpy( entry, mat.entry, sizeof( entry ) );
}

/*
================
NiMatrix3::MakeZero
================
*/
// FUNCTION: STBC 0x813490
void NiMatrix3::MakeZero() {
	memset( entry, 0, sizeof( entry ) );
}

/*
================
NiMatrix3::MakeIdentity
================
*/
// FUNCTION: STBC 0x8134A0
void NiMatrix3::MakeIdentity() {
	entry[0][0] = 1.0f;
	entry[0][1] = 0.0f;
	entry[0][2] = 0.0f;
	entry[1][0] = 0.0f;
	entry[1][1] = 1.0f;
	entry[1][2] = 0.0f;
	entry[2][0] = 0.0f;
	entry[2][1] = 0.0f;
	entry[2][2] = 1.0f;
}

/*
================
NiMatrix3::MakeDiagonal
================
*/
// FUNCTION: STBC 0x8134D0
void NiMatrix3::MakeDiagonal( float d0, float d1, float d2 ) {
	entry[0][0] = d0;
	entry[0][1] = 0.0f;
	entry[0][2] = 0.0f;
	entry[1][0] = 0.0f;
	entry[1][1] = d1;
	entry[1][2] = 0.0f;
	entry[2][0] = 0.0f;
	entry[2][1] = 0.0f;
	entry[2][2] = d2;
}

/*
================
NiMatrix3::MakeXRotation
================
*/
// FUNCTION: STBC 0x813500
void NiMatrix3::MakeXRotation( float angle ) {
	float cs = (float)cos( angle );
	float sn = (float)sin( angle );

	entry[0][0] = 1.0f;
	entry[0][1] = 0.0f;
	entry[0][2] = 0.0f;
	entry[1][0] = 0.0f;
	entry[1][1] = cs;
	entry[1][2] = sn;
	entry[2][0] = 0.0f;
	entry[2][1] = -sn;
	entry[2][2] = cs;
}

/*
================
NiMatrix3::MakeYRotation
================
*/
// FUNCTION: STBC 0x813540
void NiMatrix3::MakeYRotation( float angle ) {
	float cs = (float)cos( angle );
	float sn = (float)sin( angle );

	entry[0][0] = cs;
	entry[0][1] = 0.0f;
	entry[0][2] = -sn;
	entry[1][0] = 0.0f;
	entry[1][1] = 1.0f;
	entry[1][2] = 0.0f;
	entry[2][0] = sn;
	entry[2][1] = 0.0f;
	entry[2][2] = cs;
}

/*
================
NiMatrix3::MakeZRotation
================
*/
// FUNCTION: STBC 0x813580
void NiMatrix3::MakeZRotation( float angle ) {
	float cs = (float)cos( angle );
	float sn = (float)sin( angle );

	entry[0][0] = cs;
	entry[0][1] = sn;
	entry[0][2] = 0.0f;
	entry[1][0] = -sn;
	entry[1][1] = cs;
	entry[1][2] = 0.0f;
	entry[2][0] = 0.0f;
	entry[2][1] = 0.0f;
	entry[2][2] = 1.0f;
}

/*
================
NiMatrix3::MakeRotation

Rotation by angle about the unit axis (x, y, z).
================
*/
// FUNCTION: STBC 0x8135C0 ?MakeRotation@NiMatrix3@@QAEXMMMM@Z
void NiMatrix3::MakeRotation( float angle, float x, float y, float z ) {
	float cs = (float)cos( angle );
	float sn = (float)sin( angle );
	float omcs = 1.0f - cs;
	float x2 = x * x;
	float y2 = y * y;
	float z2 = z * z;
	float xym = x * y * omcs;
	float xzm = x * z * omcs;
	float yzm = y * z * omcs;
	float xsin = x * sn;
	float ysin = y * sn;
	float zsin = z * sn;

	entry[0][0] = x2 * omcs + cs;
	entry[0][1] = xym + zsin;
	entry[0][2] = xzm - ysin;
	entry[1][0] = xym - zsin;
	entry[1][1] = y2 * omcs + cs;
	entry[1][2] = yzm + xsin;
	entry[2][0] = xzm + ysin;
	entry[2][1] = yzm - xsin;
	entry[2][2] = z2 * omcs + cs;
}

/*
================
NiMatrix3::MakeRotation
================
*/
// FUNCTION: STBC 0x8136A0 ?MakeRotation@NiMatrix3@@QAEXMABVNiPoint3@@@Z
void NiMatrix3::MakeRotation( float angle, const NiPoint3 &axis ) {
	MakeRotation( angle, axis.x, axis.y, axis.z );
}

/*
================
NiMatrix3::operator=
================
*/
// FUNCTION: STBC 0x8136C0 ??4NiMatrix3@@QAEAAV0@ABV0@@Z
NiMatrix3 &NiMatrix3::operator=( const NiMatrix3 &mat ) {
	memcpy( entry, mat.entry, sizeof( entry ) );
	return *this;
}

/*
================
NiMatrix3::operator==
================
*/
// FUNCTION: STBC 0x8136E0 ??8NiMatrix3@@QBE_NABV0@@Z
bool NiMatrix3::operator==( const NiMatrix3 &mat ) const {
	return entry[0][0] == mat.entry[0][0]
		&& entry[0][1] == mat.entry[0][1]
		&& entry[0][2] == mat.entry[0][2]
		&& entry[1][0] == mat.entry[1][0]
		&& entry[1][1] == mat.entry[1][1]
		&& entry[1][2] == mat.entry[1][2]
		&& entry[2][0] == mat.entry[2][0]
		&& entry[2][1] == mat.entry[2][1]
		&& entry[2][2] == mat.entry[2][2];
}

/*
================
NiMatrix3::operator+
================
*/
// FUNCTION: STBC 0x813770 ??HNiMatrix3@@QBE?AV0@ABV0@@Z
NiMatrix3 NiMatrix3::operator+( const NiMatrix3 &mat ) const {
	NiMatrix3 result = *this;
	result.entry[0][0] += mat.entry[0][0];
	result.entry[0][1] += mat.entry[0][1];
	result.entry[0][2] += mat.entry[0][2];
	result.entry[1][0] += mat.entry[1][0];
	result.entry[1][1] += mat.entry[1][1];
	result.entry[1][2] += mat.entry[1][2];
	result.entry[2][0] += mat.entry[2][0];
	result.entry[2][1] += mat.entry[2][1];
	result.entry[2][2] += mat.entry[2][2];
	return result;
}

/*
================
NiMatrix3::operator-
================
*/
// FUNCTION: STBC 0x813810 ??GNiMatrix3@@QBE?AV0@ABV0@@Z
NiMatrix3 NiMatrix3::operator-( const NiMatrix3 &mat ) const {
	NiMatrix3 result = *this;
	result.entry[0][0] -= mat.entry[0][0];
	result.entry[0][1] -= mat.entry[0][1];
	result.entry[0][2] -= mat.entry[0][2];
	result.entry[1][0] -= mat.entry[1][0];
	result.entry[1][1] -= mat.entry[1][1];
	result.entry[1][2] -= mat.entry[1][2];
	result.entry[2][0] -= mat.entry[2][0];
	result.entry[2][1] -= mat.entry[2][1];
	result.entry[2][2] -= mat.entry[2][2];
	return result;
}

/*
================
NiMatrix3::operator*
================
*/
// FUNCTION: STBC 0x8138B0 ??DNiMatrix3@@QBE?AV0@ABV0@@Z
NiMatrix3 NiMatrix3::operator*( const NiMatrix3 &mat ) const {
	NiMatrix3 prod;
	prod.entry[0][0] = entry[0][0] * mat.entry[0][0] + entry[0][1] * mat.entry[1][0] + entry[0][2] * mat.entry[2][0];
	prod.entry[1][0] = entry[1][0] * mat.entry[0][0] + entry[1][1] * mat.entry[1][0] + entry[1][2] * mat.entry[2][0];
	prod.entry[2][0] = entry[2][0] * mat.entry[0][0] + entry[2][1] * mat.entry[1][0] + entry[2][2] * mat.entry[2][0];
	prod.entry[0][1] = entry[0][0] * mat.entry[0][1] + entry[0][1] * mat.entry[1][1] + entry[0][2] * mat.entry[2][1];
	prod.entry[1][1] = entry[1][0] * mat.entry[0][1] + entry[1][1] * mat.entry[1][1] + entry[1][2] * mat.entry[2][1];
	prod.entry[2][1] = entry[2][0] * mat.entry[0][1] + entry[2][1] * mat.entry[1][1] + entry[2][2] * mat.entry[2][1];
	prod.entry[0][2] = entry[0][0] * mat.entry[0][2] + entry[0][1] * mat.entry[1][2] + entry[0][2] * mat.entry[2][2];
	prod.entry[1][2] = entry[1][0] * mat.entry[0][2] + entry[1][1] * mat.entry[1][2] + entry[1][2] * mat.entry[2][2];
	prod.entry[2][2] = entry[2][0] * mat.entry[0][2] + entry[2][1] * mat.entry[1][2] + entry[2][2] * mat.entry[2][2];
	return prod;
}

/*
================
NiMatrix3::operator*
================
*/
// FUNCTION: STBC 0x8139C0 ??DNiMatrix3@@QBE?AV0@M@Z
NiMatrix3 NiMatrix3::operator*( float scalar ) const {
	NiMatrix3 result;
	result.entry[0][0] = scalar * entry[0][0];
	result.entry[0][1] = scalar * entry[0][1];
	result.entry[0][2] = scalar * entry[0][2];
	result.entry[1][0] = scalar * entry[1][0];
	result.entry[1][1] = scalar * entry[1][1];
	result.entry[1][2] = scalar * entry[1][2];
	result.entry[2][0] = scalar * entry[2][0];
	result.entry[2][1] = scalar * entry[2][1];
	result.entry[2][2] = scalar * entry[2][2];
	return result;
}

/*
================
NiMatrix3::operator*
================
*/
// FUNCTION: STBC 0x813A40 ??DNiMatrix3@@QBE?AVNiPoint3@@ABV1@@Z
NiPoint3 NiMatrix3::operator*( const NiPoint3 &pt ) const {
	return NiPoint3(
		entry[0][0] * pt.x + entry[0][1] * pt.y + entry[0][2] * pt.z,
		entry[1][0] * pt.x + entry[1][1] * pt.y + entry[1][2] * pt.z,
		entry[2][0] * pt.x + entry[2][1] * pt.y + entry[2][2] * pt.z );
}

/*
================
operator*

The row vector pt times the matrix.
================
*/
// FUNCTION: STBC 0x813AA0 ??D@YA?AVNiPoint3@@ABV0@ABVNiMatrix3@@@Z
NiPoint3 operator*( const NiPoint3 &pt, const NiMatrix3 &mat ) {
	return NiPoint3(
		pt.x * mat.entry[0][0] + pt.y * mat.entry[1][0] + pt.z * mat.entry[2][0],
		pt.x * mat.entry[0][1] + pt.y * mat.entry[1][1] + pt.z * mat.entry[2][1],
		pt.x * mat.entry[0][2] + pt.y * mat.entry[1][2] + pt.z * mat.entry[2][2] );
}

/*
================
NiMatrix3::Inverse

Computes the inverse into inv. Returns false, leaving inv holding the
adjoint, when the matrix is singular.
================
*/
// FUNCTION: STBC 0x813B00 ?Inverse@NiMatrix3@@QBE_NAAV1@@Z
bool NiMatrix3::Inverse( NiMatrix3 &inv ) const {
	inv.entry[0][0] = entry[1][1] * entry[2][2] - entry[1][2] * entry[2][1];
	inv.entry[0][1] = entry[0][2] * entry[2][1] - entry[0][1] * entry[2][2];
	inv.entry[0][2] = entry[0][1] * entry[1][2] - entry[0][2] * entry[1][1];
	inv.entry[1][0] = entry[1][2] * entry[2][0] - entry[1][0] * entry[2][2];
	inv.entry[1][1] = entry[0][0] * entry[2][2] - entry[0][2] * entry[2][0];
	inv.entry[1][2] = entry[0][2] * entry[1][0] - entry[0][0] * entry[1][2];
	inv.entry[2][0] = entry[1][0] * entry[2][1] - entry[1][1] * entry[2][0];
	inv.entry[2][1] = entry[0][1] * entry[2][0] - entry[0][0] * entry[2][1];
	inv.entry[2][2] = entry[0][0] * entry[1][1] - entry[0][1] * entry[1][0];

	float det = entry[0][0] * inv.entry[0][0] + entry[0][1] * inv.entry[1][0] + entry[0][2] * inv.entry[2][0];
	if ( (float)fabs( det ) <= 1e-06f ) {
		return false;
	}

	float invDet = 1.0f / det;
	for ( int row = 0; row < 3; row++ ) {
		for ( int col = 0; col < 3; col++ ) {
			inv.entry[row][col] *= invDet;
		}
	}
	return true;
}

/*
================
NiMatrix3::Inverse

Returns the inverse, or the zero matrix when the matrix is singular.
================
*/
// FUNCTION: STBC 0x813BF0 ?Inverse@NiMatrix3@@QBE?AV1@XZ
NiMatrix3 NiMatrix3::Inverse() const {
	NiMatrix3 inv;
	if ( !Inverse( inv ) ) {
		inv.MakeZero();
	}
	return inv;
}

/*
================
NiMatrix3::Transpose
================
*/
// FUNCTION: STBC 0x813C30
NiMatrix3 NiMatrix3::Transpose() const {
	NiPoint3 row[3];
	GetRow( 0, row[0] );
	GetRow( 1, row[1] );
	GetRow( 2, row[2] );

	// the rows become the columns
	return NiMatrix3( row[0], row[1], row[2] );
}

/*
================
NiMatrix3::ExtractAngleAndAxis

The rotation angle and unit axis of a rotation matrix. At angle zero the
axis is arbitrary and (1,0,0) is returned; at angle pi the axis comes from
the diagonal, its signs found by trying the combinations until the matrix
leaves the axis fixed.
================
*/
// FUNCTION: STBC 0x813CB0
void NiMatrix3::ExtractAngleAndAxis( float &angle, float &x, float &y, float &z ) const {
	float trace = entry[0][0] + entry[1][1] + entry[2][2];
	float cs = 0.5f * ( trace - 1.0f );
	angle = NiACos( cs );

	x = entry[1][2] - entry[2][1];
	y = entry[2][0] - entry[0][2];
	z = entry[0][1] - entry[1][0];
	float length = sqrtf( x * x + y * y + z * z );
	const float epsilon = 1e-06f;
	if ( length > epsilon ) {
		float invLength = 1.0f / length;
		x *= invLength;
		y *= invLength;
		z *= invLength;
	} else if ( angle > 1.0f ) {
		// the angle is pi
		x = sqrtf( 0.5f * ( 1.0f + entry[0][0] ) );
		y = sqrtf( 0.5f * ( 1.0f + entry[1][1] ) );
		z = sqrtf( 0.5f * ( 1.0f + entry[2][2] ) );

		float tx, ty, tz;
		tx = x * entry[0][0] + y * entry[0][1] + z * entry[0][2] - x;
		ty = x * entry[1][0] + y * entry[1][1] + z * entry[1][2] - y;
		tz = x * entry[2][0] + y * entry[2][1] + z * entry[2][2] - z;
		length = tx * tx + ty * ty + tz * tz;
		if ( length < epsilon ) {
			return;
		}

		y = -y;
		tx = x * entry[0][0] + y * entry[0][1] + z * entry[0][2] - x;
		ty = x * entry[1][0] + y * entry[1][1] + z * entry[1][2] - y;
		tz = x * entry[2][0] + y * entry[2][1] + z * entry[2][2] - z;
		length = tx * tx + ty * ty + tz * tz;
		if ( length < epsilon ) {
			return;
		}

		z = -z;
		tx = x * entry[0][0] + y * entry[0][1] + z * entry[0][2] - x;
		ty = x * entry[1][0] + y * entry[1][1] + z * entry[1][2] - y;
		tz = x * entry[2][0] + y * entry[2][1] + z * entry[2][2] - z;
		length = tx * tx + ty * ty + tz * tz;
		if ( length < epsilon ) {
			return;
		}

		y = -y;
		tx = x * entry[0][0] + y * entry[0][1] + z * entry[0][2] - x;
		ty = x * entry[1][0] + y * entry[1][1] + z * entry[1][2] - y;
		tz = x * entry[2][0] + y * entry[2][1] + z * entry[2][2] - z;
		length = tx * tx + ty * ty + tz * tz;
		if ( length < epsilon ) {
			return;
		}
	} else {
		// the angle is zero: any axis will do
		x = 1.0f;
		y = 0.0f;
		z = 0.0f;
	}
}

/*
================
NiMatrix3::Reorthogonalize

Gram-Schmidt on the columns. False, leaving the matrix partly changed,
when a column degenerates.
================
*/
// FUNCTION: STBC 0x813FC0
bool NiMatrix3::Reorthogonalize() {
	const float epsilon = 1e-05f;

	// column 0
	float length = NiSqrt( entry[0][0] * entry[0][0] + entry[1][0] * entry[1][0] + entry[2][0] * entry[2][0] );
	if ( length < epsilon ) {
		return false;
	}
	float invLength = 1.0f / length;
	entry[0][0] *= invLength;
	entry[1][0] *= invLength;
	entry[2][0] *= invLength;

	// column 1, less its part along column 0
	float dot = entry[0][0] * entry[0][1] + entry[1][0] * entry[1][1] + entry[2][0] * entry[2][1];
	entry[0][1] -= dot * entry[0][0];
	entry[1][1] -= dot * entry[1][0];
	entry[2][1] -= dot * entry[2][0];
	length = NiSqrt( entry[0][1] * entry[0][1] + entry[1][1] * entry[1][1] + entry[2][1] * entry[2][1] );
	if ( length < epsilon ) {
		return false;
	}
	invLength = 1.0f / length;
	entry[0][1] *= invLength;
	entry[1][1] *= invLength;
	entry[2][1] *= invLength;

	// column 2, less its parts along columns 0 and 1
	dot = entry[0][0] * entry[0][2] + entry[1][0] * entry[1][2] + entry[2][0] * entry[2][2];
	entry[0][2] -= dot * entry[0][0];
	entry[1][2] -= dot * entry[1][0];
	entry[2][2] -= dot * entry[2][0];
	dot = entry[0][1] * entry[0][2] + entry[1][1] * entry[1][2] + entry[2][1] * entry[2][2];
	entry[0][2] -= dot * entry[0][1];
	entry[1][2] -= dot * entry[1][1];
	entry[2][2] -= dot * entry[2][1];
	length = NiSqrt( entry[0][2] * entry[0][2] + entry[1][2] * entry[1][2] + entry[2][2] * entry[2][2] );
	if ( length < epsilon ) {
		return false;
	}
	invLength = 1.0f / length;
	entry[0][2] *= invLength;
	entry[1][2] *= invLength;
	entry[2][2] *= invLength;
	return true;
}

/*
================
NiMatrix3::TransposeTimes

Returns the transpose of this matrix times mat.
================
*/
// FUNCTION: STBC 0x814180
NiMatrix3 NiMatrix3::TransposeTimes( const NiMatrix3 &mat ) const {
	NiMatrix3 prod;
	prod.entry[0][0] = entry[0][0] * mat.entry[0][0] + entry[1][0] * mat.entry[1][0] + entry[2][0] * mat.entry[2][0];
	prod.entry[1][0] = entry[0][1] * mat.entry[0][0] + entry[1][1] * mat.entry[1][0] + entry[2][1] * mat.entry[2][0];
	prod.entry[2][0] = entry[0][2] * mat.entry[0][0] + entry[1][2] * mat.entry[1][0] + entry[2][2] * mat.entry[2][0];
	prod.entry[0][1] = entry[0][0] * mat.entry[0][1] + entry[1][0] * mat.entry[1][1] + entry[2][0] * mat.entry[2][1];
	prod.entry[1][1] = entry[0][1] * mat.entry[0][1] + entry[1][1] * mat.entry[1][1] + entry[2][1] * mat.entry[2][1];
	prod.entry[2][1] = entry[0][2] * mat.entry[0][1] + entry[1][2] * mat.entry[1][1] + entry[2][2] * mat.entry[2][1];
	prod.entry[0][2] = entry[0][0] * mat.entry[0][2] + entry[1][0] * mat.entry[1][2] + entry[2][0] * mat.entry[2][2];
	prod.entry[1][2] = entry[0][1] * mat.entry[0][2] + entry[1][1] * mat.entry[1][2] + entry[2][1] * mat.entry[2][2];
	prod.entry[2][2] = entry[0][2] * mat.entry[0][2] + entry[1][2] * mat.entry[1][2] + entry[2][2] * mat.entry[2][2];
	return prod;
}

/*
================
NiMatrix3::Congruence

Returns rot times this matrix times the transpose of rot (Gamebryo
lineage).
================
*/
// FUNCTION: STBC 0x814290
NiMatrix3 NiMatrix3::Congruence( const NiMatrix3 &rot ) const {
	NiMatrix3 prod;

	int row, col, mid;
	for ( row = 0; row < 3; row++ ) {
		for ( col = 0; col < 3; col++ ) {
			prod.entry[row][col] = 0.0f;
			for ( mid = 0; mid < 3; mid++ ) {
				prod.entry[row][col] += rot.entry[row][mid] * entry[mid][col];
			}
		}
	}

	NiMatrix3 cong;
	for ( row = 0; row < 3; row++ ) {
		for ( col = 0; col < 3; col++ ) {
			cong.entry[row][col] = 0.0f;
			for ( mid = 0; mid < 3; mid++ ) {
				cong.entry[row][col] += prod.entry[row][mid] * rot.entry[col][mid];
			}
		}
	}

	return cong;
}

/*
================
NiMatrix3::TransformVertices

out[i] = rot * in[i] + trans.
================
*/
// FUNCTION: STBC 0x814360
void NiMatrix3::TransformVertices( const NiMatrix3 &rot, const NiPoint3 &trans, unsigned int count, const NiPoint3 *in, NiPoint3 *out ) {
	for ( unsigned int i = 0; i < count; i++ ) {
		out[i].x = rot.entry[0][0] * in[i].x + rot.entry[0][1] * in[i].y + rot.entry[0][2] * in[i].z + trans.x;
		out[i].y = rot.entry[1][0] * in[i].x + rot.entry[1][1] * in[i].y + rot.entry[1][2] * in[i].z + trans.y;
		out[i].z = rot.entry[2][0] * in[i].x + rot.entry[2][1] * in[i].y + rot.entry[2][2] * in[i].z + trans.z;
	}
}

/*
================
NiMatrix3::TransformNormals

out[i] = in[i] * rot, the row vectors times the matrix.
================
*/
// FUNCTION: STBC 0x8143F0
void NiMatrix3::TransformNormals( const NiMatrix3 &rot, unsigned int count, const NiPoint3 *in, NiPoint3 *out ) {
	for ( unsigned int i = 0; i < count; i++ ) {
		out[i].x = rot.entry[0][0] * in[i].x + rot.entry[1][0] * in[i].y + rot.entry[2][0] * in[i].z;
		out[i].y = rot.entry[0][1] * in[i].x + rot.entry[1][1] * in[i].y + rot.entry[2][1] * in[i].z;
		out[i].z = rot.entry[0][2] * in[i].x + rot.entry[1][2] * in[i].y + rot.entry[2][2] * in[i].z;
	}
}

/*
================
NiMatrix3::TransformVerticesAndNormals

TransformVertices and TransformNormals in one pass. Gamebryo 1.1.2:
NiMatrix3::TransformVerticesAndNormals.
================
*/
// FUNCTION: STBC 0x814470
void NiMatrix3::TransformVerticesAndNormals( const NiMatrix3 &rot, const NiPoint3 &trans, unsigned int count,
											 const NiPoint3 *inVertex, NiPoint3 *outVertex, const NiPoint3 *inNormal,
											 NiPoint3 *outNormal ) {
	for ( unsigned int i = 0; i < count; i++ ) {
		outVertex[i].x = rot.entry[0][0] * inVertex[i].x + rot.entry[0][1] * inVertex[i].y + rot.entry[0][2] * inVertex[i].z + trans.x;
		outVertex[i].y = rot.entry[1][0] * inVertex[i].x + rot.entry[1][1] * inVertex[i].y + rot.entry[1][2] * inVertex[i].z + trans.y;
		outVertex[i].z = rot.entry[2][0] * inVertex[i].x + rot.entry[2][1] * inVertex[i].y + rot.entry[2][2] * inVertex[i].z + trans.z;
		outNormal[i].x = rot.entry[0][0] * inNormal[i].x + rot.entry[1][0] * inNormal[i].y + rot.entry[2][0] * inNormal[i].z;
		outNormal[i].y = rot.entry[0][1] * inNormal[i].x + rot.entry[1][1] * inNormal[i].y + rot.entry[2][1] * inNormal[i].z;
		outNormal[i].z = rot.entry[0][2] * inNormal[i].x + rot.entry[1][2] * inNormal[i].y + rot.entry[2][2] * inNormal[i].z;
	}
}

/*
================
NiMatrix3::EigenSolveSymmetric

The eigenvectors come out as the columns of the reduced matrix; the last
is negated when needed to make the three a right-handed system.
================
*/
// FUNCTION: STBC 0x8145B0
void NiMatrix3::EigenSolveSymmetric( float eigenvalue[3], NiPoint3 eigenvector[3] ) {
	NiMatrix3 mat = *this;
	float subd[3];
	mat.Tridiagonal( eigenvalue, subd );
	mat.QLAlgorithm( eigenvalue, subd );

	for ( unsigned int i = 0; i < 3; i++ ) {
		eigenvector[i].x = mat.entry[0][i];
		eigenvector[i].y = mat.entry[1][i];
		eigenvector[i].z = mat.entry[2][i];
	}

	if ( eigenvector[0].Dot( eigenvector[1].Cross( eigenvector[2] ) ) < 0.0f ) {
		eigenvector[2].x = -eigenvector[2].x;
		eigenvector[2].y = -eigenvector[2].y;
		eigenvector[2].z = -eigenvector[2].z;
	}
}

/*
================
NiMatrix3::Tridiagonal
================
*/
// FUNCTION: STBC 0x814680
void NiMatrix3::Tridiagonal( float diag[3], float subd[3] ) {
	float a = entry[0][0];
	float b = entry[0][1];
	float c = entry[0][2];
	float d = entry[1][1];
	float e = entry[1][2];
	float f = entry[2][2];

	diag[0] = a;
	subd[2] = 0.0f;
	if ( (float)fabs( c ) >= 1e-08f ) {
		float length = (float)sqrt( b * b + c * c );
		b /= length;
		c /= length;
		float q = 2.0f * b * e + c * ( f - d );
		diag[1] = d + c * q;
		diag[2] = f - c * q;
		subd[0] = length;
		subd[1] = e - b * q;
		entry[0][0] = 1.0f;
		entry[0][1] = 0.0f;
		entry[0][2] = 0.0f;
		entry[1][0] = 0.0f;
		entry[1][1] = b;
		entry[1][2] = c;
		entry[2][0] = 0.0f;
		entry[2][1] = c;
		entry[2][2] = -b;
	} else {
		diag[1] = d;
		diag[2] = f;
		subd[0] = b;
		subd[1] = e;
		entry[0][0] = 1.0f;
		entry[0][1] = 0.0f;
		entry[0][2] = 0.0f;
		entry[1][0] = 0.0f;
		entry[1][1] = 1.0f;
		entry[1][2] = 0.0f;
		entry[2][0] = 0.0f;
		entry[2][1] = 0.0f;
		entry[2][2] = 1.0f;
	}
}

/*
================
NiMatrix3::QLAlgorithm
================
*/
// FUNCTION: STBC 0x8147B0
// The debug NetImmerse 3.1's statements (AnimOnly 0x422DBD): Numerical Recipes' tqli, with its
// cs *= ( sn = 1.0f / r ) and NiAbs and NiSqrt for fabs and sqrt.
int NiMatrix3::QLAlgorithm( float diag[3], float subd[3] ) {
	const int maxIter = 32;

	for ( int i0 = 0; i0 < 3; i0++ ) {
		int i1;
		for ( i1 = 0; i1 < maxIter; i1++ ) {
			int i2;
			for ( i2 = i0; i2 <= 1; i2++ ) {
				float tmp = NiAbs( diag[i2] ) + NiAbs( diag[i2 + 1] );
				if ( NiAbs( subd[i2] ) + tmp == tmp ) {
					break;
				}
			}
			if ( i2 == i0 ) {
				break;
			}

			float g = ( diag[i0 + 1] - diag[i0] ) / ( 2.0f * subd[i0] );
			float r = NiSqrt( g * g + 1.0f );
			if ( g < 0.0f ) {
				g = diag[i2] - diag[i0] + subd[i0] / ( g - r );
			} else {
				g = diag[i2] - diag[i0] + subd[i0] / ( g + r );
			}
			float sn = 1.0f;
			float cs = 1.0f;
			float p = 0.0f;
			for ( int i3 = i2 - 1; i3 >= i0; i3-- ) {
				float f = sn * subd[i3];
				float b = cs * subd[i3];
				if ( NiAbs( f ) >= NiAbs( g ) ) {
					cs = g / f;
					r = NiSqrt( cs * cs + 1.0f );
					subd[i3 + 1] = f * r;
					cs *= ( sn = 1.0f / r );
				} else {
					sn = f / g;
					r = NiSqrt( sn * sn + 1.0f );
					subd[i3 + 1] = g * r;
					sn *= ( cs = 1.0f / r );
				}
				g = diag[i3 + 1] - p;
				r = ( diag[i3] - g ) * sn + 2.0f * b * cs;
				p = sn * r;
				diag[i3 + 1] = g + p;
				g = cs * r - b;

				for ( int i4 = 0; i4 < 3; i4++ ) {
					f = entry[i4][i3 + 1];
					entry[i4][i3 + 1] = sn * entry[i4][i3] + cs * f;
					entry[i4][i3] = cs * entry[i4][i3] - sn * f;
				}
			}
			diag[i0] -= p;
			subd[i0] = g;
			subd[i2] = 0.0f;
		}
		if ( i1 == maxIter ) {
			return 0;
		}
	}
	return 1;
}

/*
================
NiMatrix3::ToEulerAnglesXYZ

Decomposes the matrix into rotations about X, Y and Z. Returns false at
gimbal lock, where the answer is not unique and zAngle is set to 0.
================
*/
// FUNCTION: STBC 0x814A30
bool NiMatrix3::ToEulerAnglesXYZ( float &xAngle, float &yAngle, float &zAngle ) const {
	yAngle = -NiASin( entry[0][2] );
	if ( yAngle < NI_HALF_PI ) {
		if ( yAngle > -NI_HALF_PI ) {
			xAngle = -NiFastATan2( -entry[1][2], entry[2][2] );
			zAngle = -NiFastATan2( -entry[0][1], entry[0][0] );
			return true;
		}
		// not unique: any zAngle works with the matching xAngle
		float angle = NiFastATan2( entry[1][0], entry[1][1] );
		zAngle = 0.0f;
		xAngle = angle;
		return false;
	}
	float angle = NiFastATan2( entry[1][0], entry[1][1] );
	zAngle = 0.0f;
	xAngle = -angle;
	return false;
}

/*
================
NiMatrix3::ToEulerAnglesXZY

Decomposes the matrix into rotations about X, Z and Y. Returns
false at gimbal lock, where the answer is not unique and yAngle is set to 0.
================
*/
// FUNCTION: STBC 0x814B40
bool NiMatrix3::ToEulerAnglesXZY( float &xAngle, float &zAngle, float &yAngle ) const {
	zAngle = NiASin( entry[0][1] );
	if ( zAngle < NI_HALF_PI ) {
		if ( zAngle > -NI_HALF_PI ) {
			xAngle = -NiFastATan2( entry[2][1], entry[1][1] );
			yAngle = -NiFastATan2( entry[0][2], entry[0][0] );
			return true;
		}
		// not unique: any yAngle works with the matching xAngle
		float angle = NiFastATan2( -entry[2][0], entry[2][2] );
		yAngle = 0.0f;
		xAngle = angle;
		return false;
	}
	float angle = NiFastATan2( -entry[2][0], entry[2][2] );
	yAngle = 0.0f;
	xAngle = -angle;
	return false;
}

/*
================
NiMatrix3::ToEulerAnglesYXZ

Decomposes the matrix into rotations about Y, X and Z. Returns
false at gimbal lock, where the answer is not unique and zAngle is set to 0.
================
*/
// FUNCTION: STBC 0x814C40
bool NiMatrix3::ToEulerAnglesYXZ( float &yAngle, float &xAngle, float &zAngle ) const {
	xAngle = NiASin( entry[1][2] );
	if ( xAngle < NI_HALF_PI ) {
		if ( xAngle > -NI_HALF_PI ) {
			yAngle = -NiFastATan2( entry[0][2], entry[2][2] );
			zAngle = -NiFastATan2( entry[1][0], entry[1][1] );
			return true;
		}
		// not unique: any zAngle works with the matching yAngle
		float angle = NiFastATan2( -entry[0][1], entry[0][0] );
		zAngle = 0.0f;
		yAngle = angle;
		return false;
	}
	float angle = NiFastATan2( -entry[0][1], entry[0][0] );
	zAngle = 0.0f;
	yAngle = -angle;
	return false;
}

/*
================
NiMatrix3::ToEulerAnglesYZX

Decomposes the matrix into rotations about Y, Z and X. Returns
false at gimbal lock, where the answer is not unique and xAngle is set to 0.
================
*/
// FUNCTION: STBC 0x814D40
bool NiMatrix3::ToEulerAnglesYZX( float &yAngle, float &zAngle, float &xAngle ) const {
	zAngle = -NiASin( entry[1][0] );
	if ( zAngle < NI_HALF_PI ) {
		if ( zAngle > -NI_HALF_PI ) {
			yAngle = -NiFastATan2( -entry[2][0], entry[0][0] );
			xAngle = -NiFastATan2( -entry[1][2], entry[1][1] );
			return true;
		}
		// not unique: any xAngle works with the matching yAngle
		float angle = NiFastATan2( entry[2][1], entry[2][2] );
		xAngle = 0.0f;
		yAngle = angle;
		return false;
	}
	float angle = NiFastATan2( entry[2][1], entry[2][2] );
	xAngle = 0.0f;
	yAngle = -angle;
	return false;
}

/*
================
NiMatrix3::ToEulerAnglesZXY

Decomposes the matrix into rotations about Z, X and Y. Returns
false at gimbal lock, where the answer is not unique and yAngle is set to 0.
================
*/
// FUNCTION: STBC 0x814E50
bool NiMatrix3::ToEulerAnglesZXY( float &zAngle, float &xAngle, float &yAngle ) const {
	xAngle = -NiASin( entry[2][1] );
	if ( xAngle < NI_HALF_PI ) {
		if ( xAngle > -NI_HALF_PI ) {
			zAngle = -NiFastATan2( -entry[0][1], entry[1][1] );
			yAngle = -NiFastATan2( -entry[2][0], entry[2][2] );
			return true;
		}
		// not unique: any yAngle works with the matching zAngle
		float angle = NiFastATan2( entry[0][2], entry[0][0] );
		yAngle = 0.0f;
		zAngle = angle;
		return false;
	}
	float angle = NiFastATan2( entry[0][2], entry[0][0] );
	yAngle = 0.0f;
	zAngle = -angle;
	return false;
}

/*
================
NiMatrix3::ToEulerAnglesZYX

Decomposes the matrix into rotations about Z, Y and X. Returns
false at gimbal lock, where the answer is not unique and xAngle is set to 0.
================
*/
// FUNCTION: STBC 0x814F60
bool NiMatrix3::ToEulerAnglesZYX( float &zAngle, float &yAngle, float &xAngle ) const {
	yAngle = NiASin( entry[2][0] );
	if ( yAngle < NI_HALF_PI ) {
		if ( yAngle > -NI_HALF_PI ) {
			zAngle = -NiFastATan2( entry[1][0], entry[0][0] );
			xAngle = -NiFastATan2( entry[2][1], entry[2][2] );
			return true;
		}
		// not unique: any xAngle works with the matching zAngle
		float angle = NiFastATan2( -entry[0][1], entry[0][2] );
		xAngle = 0.0f;
		zAngle = angle;
		return false;
	}
	float angle = NiFastATan2( -entry[0][1], entry[0][2] );
	xAngle = 0.0f;
	zAngle = -angle;
	return false;
}

/*
================
NiMatrix3::FromEulerAnglesXYZ

The product of the rotations about X, Y and Z, in that order
of factors.
================
*/
// FUNCTION: STBC 0x815060
void NiMatrix3::FromEulerAnglesXYZ( float xAngle, float yAngle, float zAngle ) {
	NiMatrix3 rotX, rotY, rotZ;
	rotX.MakeXRotation( xAngle );
	rotY.MakeYRotation( yAngle );
	rotZ.MakeZRotation( zAngle );
	*this = rotX * ( rotY * rotZ );
}

/*
================
NiMatrix3::FromEulerAnglesXZY

The product of the rotations about X, Z and Y, in that order
of factors.
================
*/
// FUNCTION: STBC 0x815190
void NiMatrix3::FromEulerAnglesXZY( float xAngle, float zAngle, float yAngle ) {
	NiMatrix3 rotX, rotY, rotZ;
	rotX.MakeXRotation( xAngle );
	rotY.MakeYRotation( yAngle );
	rotZ.MakeZRotation( zAngle );
	*this = rotX * ( rotZ * rotY );
}

/*
================
NiMatrix3::FromEulerAnglesYXZ

The product of the rotations about Y, X and Z, in that order
of factors.
================
*/
// FUNCTION: STBC 0x8152C0
void NiMatrix3::FromEulerAnglesYXZ( float yAngle, float xAngle, float zAngle ) {
	NiMatrix3 rotX, rotY, rotZ;
	rotX.MakeXRotation( xAngle );
	rotY.MakeYRotation( yAngle );
	rotZ.MakeZRotation( zAngle );
	*this = rotY * ( rotX * rotZ );
}

/*
================
NiMatrix3::FromEulerAnglesYZX

The product of the rotations about Y, Z and X, in that order
of factors.
================
*/
// FUNCTION: STBC 0x8153F0
void NiMatrix3::FromEulerAnglesYZX( float yAngle, float zAngle, float xAngle ) {
	NiMatrix3 rotX, rotY, rotZ;
	rotX.MakeXRotation( xAngle );
	rotY.MakeYRotation( yAngle );
	rotZ.MakeZRotation( zAngle );
	*this = rotY * ( rotZ * rotX );
}

/*
================
NiMatrix3::FromEulerAnglesZXY

The product of the rotations about Z, X and Y, in that order
of factors.
================
*/
// FUNCTION: STBC 0x815520
void NiMatrix3::FromEulerAnglesZXY( float zAngle, float xAngle, float yAngle ) {
	NiMatrix3 rotX, rotY, rotZ;
	rotX.MakeXRotation( xAngle );
	rotY.MakeYRotation( yAngle );
	rotZ.MakeZRotation( zAngle );
	*this = rotZ * ( rotX * rotY );
}

/*
================
NiMatrix3::FromEulerAnglesZYX

The product of the rotations about Z, Y and X, in that order
of factors.
================
*/
// FUNCTION: STBC 0x815650
void NiMatrix3::FromEulerAnglesZYX( float zAngle, float yAngle, float xAngle ) {
	NiMatrix3 rotX, rotY, rotZ;
	rotX.MakeXRotation( xAngle );
	rotY.MakeYRotation( yAngle );
	rotZ.MakeZRotation( zAngle );
	*this = rotZ * ( rotY * rotX );
}

/*
================
NiMatrix3::LoadBinary
================
*/
// FUNCTION: STBC 0x815780
void NiMatrix3::LoadBinary( NiStream &stream ) {
	for ( int row = 0; row < 3; row++ ) {
		for ( int col = 0; col < 3; col++ ) {
			NiStreamLoadBinary( stream, entry[row][col] );
		}
	}
}

/*
================
NiMatrix3::SaveBinary
================
*/
// FUNCTION: STBC 0x8157B0
void NiMatrix3::SaveBinary( NiStream &stream ) {
	for ( int row = 0; row < 3; row++ ) {
		for ( int col = 0; col < 3; col++ ) {
			NiStreamSaveBinary( stream, entry[row][col] );
		}
	}
}

/*
================
NiMatrix3::GetViewerString

Returns "<prefix> = ((row0),(row1),(row2))" in a new string that the
caller deletes.
================
*/
// FUNCTION: STBC 0x8157E0
char *NiMatrix3::GetViewerString( const char *prefix ) const {
	char *string = new char[strlen( prefix ) + 128];
	sprintf( string, "%s = ((%f,%f,%f),(%f,%f,%f),(%f,%f,%f))", prefix,
		entry[0][0], entry[0][1], entry[0][2],
		entry[1][0], entry[1][1], entry[1][2],
		entry[2][0], entry[2][1], entry[2][2] );
	return string;
}
