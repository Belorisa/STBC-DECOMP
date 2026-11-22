/*
===============================================================================

	TGMatrix3.cpp

	TGMatrix3 repeats NiMatrix3's own methods: the same code, compiled again
	in this unit (0x459670..), not calls into NetImmerse (0x813490..).

===============================================================================
*/

#include <iostream>
#include <math.h>
#include <string.h>

#include "TGMatrix3.h"
#include "TGPoint3.h"
#include "NiMath.h"
#include "NiAVObject.h"
#include "NiAccumulator.h"
#include "NiDynamicEffect.h"
#include "NiExtraData.h"
#include "renderer/NiImage.h"
#include "NiObject.h"
#include "NiObjectNET.h"
#include "NiProperty.h"
#include "NiRawImageData.h"
#include "renderer/NiRender.h"
#include "NiTextureProperty.h"
#include "NiTimeController.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00459390 _$E32
// FUNCTION: STBC 0x004593a0 _$E29
// FUNCTION: STBC 0x004593b0 _$E31
// FUNCTION: STBC 0x004593c0 _$E30
// FUNCTION: STBC 0x004593d0 _$E38
// FUNCTION: STBC 0x004593e0 _$E35
// FUNCTION: STBC 0x004593f0 _$E37
// FUNCTION: STBC 0x00459400 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00459440 _$E56
// FUNCTION: STBC 0x00459450 _$E55
// FUNCTION: STBC 0x00459460 _$E200
// FUNCTION: STBC 0x00459470 _$E199
// FUNCTION: STBC 0x00459490 _$E138
// FUNCTION: STBC 0x004594a0 _$E137
// FUNCTION: STBC 0x004594b0 _$E151
// FUNCTION: STBC 0x004594c0 _$E150
// FUNCTION: STBC 0x004594d0 _$E67
// FUNCTION: STBC 0x004594e0 _$E66
// FUNCTION: STBC 0x004594f0 _$E71
// FUNCTION: STBC 0x00459500 _$E70
// FUNCTION: STBC 0x00459510 _$E75
// FUNCTION: STBC 0x00459520 _$E74
// FUNCTION: STBC 0x00459530 _$E79
// FUNCTION: STBC 0x00459540 _$E78
// FUNCTION: STBC 0x00459550 _$E196
// FUNCTION: STBC 0x00459560 _$E195
// FUNCTION: STBC 0x00459570 _$E205
// FUNCTION: STBC 0x00459580 _$E204
// FUNCTION: STBC 0x00459590 _$E210
// FUNCTION: STBC 0x004595a0 _$E209

/*
================
TGMatrix3::TGMatrix3

Builds the matrix from its three columns.
================
*/
// FUNCTION: STBC 0x004595b0 ??0TGMatrix3@@QAE@ABVTGPoint3@@00@Z
TGMatrix3::TGMatrix3( const TGPoint3 &col0, const TGPoint3 &col1, const TGPoint3 &col2 ) {
	entry[0][0] = col0.x;
	entry[1][0] = col0.y;
	entry[2][0] = col0.z;
	entry[0][1] = col1.x;
	entry[1][1] = col1.y;
	entry[2][1] = col1.z;
	entry[0][2] = col2.x;
	entry[1][2] = col2.y;
	entry[2][2] = col2.z;
}

/*
================
TGMatrix3::TGMatrix3

Copies a NetImmerse matrix, a column at a time.
================
*/
// FUNCTION: STBC 0x00459600 ??0TGMatrix3@@QAE@ABVNiMatrix3@@@Z
TGMatrix3::TGMatrix3( const NiMatrix3 &mat ) {
	TGPoint3 col;
	col.x = mat.entry[0][0];
	col.y = mat.entry[1][0];
	col.z = mat.entry[2][0];
	SetCol( 0, &col );
	col.x = mat.entry[0][1];
	col.y = mat.entry[1][1];
	col.z = mat.entry[2][1];
	SetCol( 1, &col );
	col.x = mat.entry[0][2];
	col.y = mat.entry[1][2];
	col.z = mat.entry[2][2];
	SetCol( 2, &col );
}

/*
================
TGMatrix3::MakeZero
================
*/
// FUNCTION: STBC 0x00459670
void TGMatrix3::MakeZero( void ) {
	memset( entry, 0, sizeof( entry ) );
}

/*
================
TGMatrix3::MakeIdentity
================
*/
// FUNCTION: STBC 0x00459680
void TGMatrix3::MakeIdentity( void ) {
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
TGMatrix3::MakeDiagonal
================
*/
// FUNCTION: STBC 0x004596b0
void TGMatrix3::MakeDiagonal( float d0, float d1, float d2 ) {
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
TGMatrix3::MakeXRotation
================
*/
// FUNCTION: STBC 0x004596e0
void TGMatrix3::MakeXRotation( float angle ) {
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
TGMatrix3::MakeYRotation
================
*/
// FUNCTION: STBC 0x00459720
void TGMatrix3::MakeYRotation( float angle ) {
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
TGMatrix3::MakeZRotation
================
*/
// FUNCTION: STBC 0x00459760
void TGMatrix3::MakeZRotation( float angle ) {
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
TGMatrix3::MakeRotation
================
*/
// FUNCTION: STBC 0x004597a0 ?MakeRotation@TGMatrix3@@QAEXMMMM@Z
void TGMatrix3::MakeRotation( float angle, float x, float y, float z ) {
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
TGMatrix3::MakeRotation
================
*/
// FUNCTION: STBC 0x00459880 ?MakeRotation@TGMatrix3@@QAEXMPAVTGPoint3@@@Z
void TGMatrix3::MakeRotation( float angle, TGPoint3 *axis ) {
	MakeRotation( angle, axis->x, axis->y, axis->z );
}

/*
================
TGMatrix3::operator=
================
*/
// FUNCTION: STBC 0x004598a0 ??4TGMatrix3@@QAEAAV0@ABV0@@Z
TGMatrix3 &TGMatrix3::operator=( const TGMatrix3 &matrix ) {
	memcpy( entry, matrix.entry, sizeof( entry ) );
	return *this;
}

/*
================
TGMatrix3::operator==
================
*/
// FUNCTION: STBC 0x004598c0 ??8TGMatrix3@@QBEHABV0@@Z
int TGMatrix3::operator==( const TGMatrix3 &mat ) const {
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
TGMatrix3::operator+
================
*/
// FUNCTION: STBC 0x00459950 ??HTGMatrix3@@QBE?AV0@ABV0@@Z
TGMatrix3 TGMatrix3::operator+( const TGMatrix3 &mat ) const {
	TGMatrix3 result = *this;
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
TGMatrix3::operator-
================
*/
// FUNCTION: STBC 0x00459a10 ??GTGMatrix3@@QBE?AV0@ABV0@@Z
TGMatrix3 TGMatrix3::operator-( const TGMatrix3 &mat ) const {
	TGMatrix3 result = *this;
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
TGMatrix3::operator*
================
*/
// FUNCTION: STBC 0x00459ad0 ??DTGMatrix3@@QBE?AV0@ABV0@@Z
TGMatrix3 TGMatrix3::operator*( const TGMatrix3 &mat ) const {
	TGMatrix3 prod;
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
TGMatrix3::operator*
================
*/
// FUNCTION: STBC 0x00459c00 ??DTGMatrix3@@QBE?AV0@M@Z
TGMatrix3 TGMatrix3::operator*( float scalar ) const {
	TGMatrix3 result;
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
TGMatrix3::operator*
================
*/
// FUNCTION: STBC 0x00459cb0 ??DTGMatrix3@@QBE?AVTGPoint3@@ABV1@@Z
TGPoint3 TGMatrix3::operator*( const TGPoint3 &pt ) const {
	return TGPoint3(
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
// FUNCTION: STBC 0x00459d10 ??D@YA?AVTGPoint3@@ABV0@ABVTGMatrix3@@@Z
TGPoint3 operator*( const TGPoint3 &pt, const TGMatrix3 &mat ) {
	return TGPoint3(
		pt.x * mat.entry[0][0] + pt.y * mat.entry[1][0] + pt.z * mat.entry[2][0],
		pt.x * mat.entry[0][1] + pt.y * mat.entry[1][1] + pt.z * mat.entry[2][1],
		pt.x * mat.entry[0][2] + pt.y * mat.entry[1][2] + pt.z * mat.entry[2][2] );
}

/*
================
TGMatrix3::Inverse
================
*/
// FUNCTION: STBC 0x00459d70 ?Inverse@TGMatrix3@@QBE_NAAV1@@Z
bool TGMatrix3::Inverse( TGMatrix3 &inv ) const {
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
TGMatrix3::Inverse
================
*/
// FUNCTION: STBC 0x00459e60 ?Inverse@TGMatrix3@@QBE?AV1@XZ
TGMatrix3 TGMatrix3::Inverse( void ) const {
	TGMatrix3 inv;
	if ( !Inverse( inv ) ) {
		inv.MakeZero();
	}
	return inv;
}

/*
================
TGMatrix3::Transpose
================
*/
// FUNCTION: STBC 0x00459ec0
TGMatrix3 TGMatrix3::Transpose( void ) const {
	TGPoint3 row[3];
	GetRow( 0, &row[0] );
	GetRow( 1, &row[1] );
	GetRow( 2, &row[2] );

	// the rows become the columns
	return TGMatrix3( row[0], row[1], row[2] );
}

/*
================
TGMatrix3::ExtractAngleAndAxis

The rotation angle and unit axis of a rotation matrix. At angle zero the
axis is arbitrary and (1,0,0) is returned; at angle pi the axis comes from
the diagonal, its signs found by trying the combinations until the matrix
leaves the axis fixed.
================
*/
// FUNCTION: STBC 0x00459f90
void TGMatrix3::ExtractAngleAndAxis( float *angle, float *x, float *y, float *z ) {
	float trace = entry[0][0] + entry[1][1] + entry[2][2];
	float cs = 0.5f * ( trace - 1.0f );
	*angle = NiACos( cs );

	*x = entry[1][2] - entry[2][1];
	*y = entry[2][0] - entry[0][2];
	*z = entry[0][1] - entry[1][0];
	float length = sqrtf( *x * *x + *y * *y + *z * *z );
	const float epsilon = 1e-06f;
	if ( length > epsilon ) {
		float invLength = 1.0f / length;
		*x *= invLength;
		*y *= invLength;
		*z *= invLength;
	} else if ( *angle > 1.0f ) {
		// the angle is pi
		*x = sqrtf( 0.5f * ( 1.0f + entry[0][0] ) );
		*y = sqrtf( 0.5f * ( 1.0f + entry[1][1] ) );
		*z = sqrtf( 0.5f * ( 1.0f + entry[2][2] ) );

		float tx, ty, tz;
		tx = *x * entry[0][0] + *y * entry[0][1] + *z * entry[0][2] - *x;
		ty = *x * entry[1][0] + *y * entry[1][1] + *z * entry[1][2] - *y;
		tz = *x * entry[2][0] + *y * entry[2][1] + *z * entry[2][2] - *z;
		length = tx * tx + ty * ty + tz * tz;
		if ( length < epsilon ) {
			return;
		}

		*y = -*y;
		tx = *x * entry[0][0] + *y * entry[0][1] + *z * entry[0][2] - *x;
		ty = *x * entry[1][0] + *y * entry[1][1] + *z * entry[1][2] - *y;
		tz = *x * entry[2][0] + *y * entry[2][1] + *z * entry[2][2] - *z;
		length = tx * tx + ty * ty + tz * tz;
		if ( length < epsilon ) {
			return;
		}

		*z = -*z;
		tx = *x * entry[0][0] + *y * entry[0][1] + *z * entry[0][2] - *x;
		ty = *x * entry[1][0] + *y * entry[1][1] + *z * entry[1][2] - *y;
		tz = *x * entry[2][0] + *y * entry[2][1] + *z * entry[2][2] - *z;
		length = tx * tx + ty * ty + tz * tz;
		if ( length < epsilon ) {
			return;
		}

		*y = -*y;
		tx = *x * entry[0][0] + *y * entry[0][1] + *z * entry[0][2] - *x;
		ty = *x * entry[1][0] + *y * entry[1][1] + *z * entry[1][2] - *y;
		tz = *x * entry[2][0] + *y * entry[2][1] + *z * entry[2][2] - *z;
		length = tx * tx + ty * ty + tz * tz;
		if ( length < epsilon ) {
			return;
		}
	} else {
		// the angle is zero: any axis will do
		*x = 1.0f;
		*y = 0.0f;
		*z = 0.0f;
	}
}

/*
================
TGMatrix3::Reorthogonalize

Gram-Schmidt on the columns. False, leaving the matrix partly changed,
when a column degenerates.
================
*/
// FUNCTION: STBC 0x0045a2a0
bool TGMatrix3::Reorthogonalize( void ) {
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
TGMatrix3::TransposeTimes
================
*/
// FUNCTION: STBC 0x0045a460
TGMatrix3 TGMatrix3::TransposeTimes( const TGMatrix3 &mat ) const {
	TGMatrix3 prod;
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
TGMatrix3::Congruence

Returns mat * this * mat^T.
================
*/
// FUNCTION: STBC 0x0045a590
TGMatrix3 TGMatrix3::Congruence( const TGMatrix3 &mat ) const {
	int row, col, k;

	TGMatrix3 prod;
	for ( row = 0; row < 3; row++ ) {
		for ( col = 0; col < 3; col++ ) {
			prod.entry[row][col] = 0.0f;
			for ( k = 0; k < 3; k++ ) {
				prod.entry[row][col] += mat.entry[row][k] * entry[k][col];
			}
		}
	}

	TGMatrix3 result;
	for ( row = 0; row < 3; row++ ) {
		for ( col = 0; col < 3; col++ ) {
			result.entry[row][col] = 0.0f;
			for ( k = 0; k < 3; k++ ) {
				result.entry[row][col] += prod.entry[row][k] * mat.entry[col][k];
			}
		}
	}
	return result;
}

/*
================
TGMatrix3::TransformVertices

out[i] = rot * in[i] + trans.
================
*/
// FUNCTION: STBC 0x0045a690
void TGMatrix3::TransformVertices( TGMatrix3 *rot, TGPoint3 *trans, int count, TGPoint3 *in, TGPoint3 *out ) {
	for ( unsigned int i = 0; i < count; i++ ) {
		out[i].x = rot->entry[0][0] * in[i].x + rot->entry[0][1] * in[i].y + rot->entry[0][2] * in[i].z + trans->x;
		out[i].y = rot->entry[1][0] * in[i].x + rot->entry[1][1] * in[i].y + rot->entry[1][2] * in[i].z + trans->y;
		out[i].z = rot->entry[2][0] * in[i].x + rot->entry[2][1] * in[i].y + rot->entry[2][2] * in[i].z + trans->z;
	}
}

/*
================
TGMatrix3::TransformNormals

out[i] = in[i] * rot, the row vectors times the matrix.
================
*/
// FUNCTION: STBC 0x0045a720
void TGMatrix3::TransformNormals( TGMatrix3 *rot, int count, TGPoint3 *in, TGPoint3 *out ) {
	for ( unsigned int i = 0; i < count; i++ ) {
		out[i].x = rot->entry[0][0] * in[i].x + rot->entry[1][0] * in[i].y + rot->entry[2][0] * in[i].z;
		out[i].y = rot->entry[0][1] * in[i].x + rot->entry[1][1] * in[i].y + rot->entry[2][1] * in[i].z;
		out[i].z = rot->entry[0][2] * in[i].x + rot->entry[1][2] * in[i].y + rot->entry[2][2] * in[i].z;
	}
}

/*
================
TGMatrix3::TransformVerticesAndNormals

TransformVertices and TransformNormals in one pass.
================
*/
// FUNCTION: STBC 0x0045a7a0
void TGMatrix3::TransformVerticesAndNormals( TGMatrix3 *rot, TGPoint3 *trans, int count, TGPoint3 *inVertices, TGPoint3 *outVertices, TGPoint3 *inNormals, TGPoint3 *outNormals ) {
	for ( unsigned int i = 0; i < count; i++ ) {
		outVertices[i].x = rot->entry[0][0] * inVertices[i].x + rot->entry[0][1] * inVertices[i].y + rot->entry[0][2] * inVertices[i].z + trans->x;
		outVertices[i].y = rot->entry[1][0] * inVertices[i].x + rot->entry[1][1] * inVertices[i].y + rot->entry[1][2] * inVertices[i].z + trans->y;
		outVertices[i].z = rot->entry[2][0] * inVertices[i].x + rot->entry[2][1] * inVertices[i].y + rot->entry[2][2] * inVertices[i].z + trans->z;
		outNormals[i].x = rot->entry[0][0] * inNormals[i].x + rot->entry[1][0] * inNormals[i].y + rot->entry[2][0] * inNormals[i].z;
		outNormals[i].y = rot->entry[0][1] * inNormals[i].x + rot->entry[1][1] * inNormals[i].y + rot->entry[2][1] * inNormals[i].z;
		outNormals[i].z = rot->entry[0][2] * inNormals[i].x + rot->entry[1][2] * inNormals[i].y + rot->entry[2][2] * inNormals[i].z;
	}
}

/*
================
TGMatrix3::EigenSolveSymmetric

The eigenvalues and unit eigenvectors of a symmetric matrix: Householder
reduction to tridiagonal form, then the QL algorithm. The eigenvectors come
out as a right-handed system.
================
*/
// FUNCTION: STBC 0x0045a8e0
void TGMatrix3::EigenSolveSymmetric( float *eigenvalues, TGPoint3 *eigenvectors ) {
	TGMatrix3 mat = *this;
	float subDiag[3];
	mat.Tridiagonal( eigenvalues, subDiag );
	mat.QLAlgorithm( eigenvalues, subDiag );

	for ( int i = 0; i < 3; i++ ) {
		eigenvectors[i].x = mat.entry[0][i];
		eigenvectors[i].y = mat.entry[1][i];
		eigenvectors[i].z = mat.entry[2][i];
	}

	// make the eigenvectors a right-handed system
	if ( eigenvectors[1].Cross( eigenvectors[2] ).Dot( eigenvectors[0] ) < 0.0f ) {
		eigenvectors[2].x = -eigenvectors[2].x;
		eigenvectors[2].y = -eigenvectors[2].y;
		eigenvectors[2].z = -eigenvectors[2].z;
	}
}

/*
================
TGMatrix3::Tridiagonal

Householder reduction of a symmetric matrix to tridiagonal form: diag and
subDiag receive the tridiagonal matrix, and the matrix becomes the
orthogonal transform.
================
*/
// FUNCTION: STBC 0x0045a9c0
void TGMatrix3::Tridiagonal( float *diag, float *subDiag ) {
	float a = entry[0][0];
	float b = entry[0][1];
	float c = entry[0][2];
	float d = entry[1][1];
	float e = entry[1][2];
	float f = entry[2][2];

	diag[0] = a;
	subDiag[2] = 0.0f;
	if ( (float)fabs( c ) >= 1e-08f ) {
		float length = sqrtf( b * b + c * c );
		b /= length;
		c /= length;
		float q = 2.0f * b * e + c * ( f - d );
		diag[1] = d + c * q;
		diag[2] = f - c * q;
		subDiag[0] = length;
		subDiag[1] = e - b * q;
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
		subDiag[0] = b;
		subDiag[1] = e;
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
TGMatrix3::QLAlgorithm

QL iteration with implicit shifting, from tridiagonal (diag, subDiag) to
diagonal form, accumulating the rotations into the matrix. False when an
eigenvalue fails to converge in 32 iterations.
================
*/
// FUNCTION: STBC 0x0045aaf0
// PARKED: two FP schedules differ (cs *= sn loads sn instead of using it from memory; the rotation evaluates cs*tmp3 first); operand orders and local splits change nothing
int TGMatrix3::QLAlgorithm( float *diag, float *subDiag ) {
	for ( int i0 = 0; i0 < 3; i0++ ) {
		const int maxIter = 32;
		int iter;
		for ( iter = 0; iter < maxIter; iter++ ) {
			int i1;
			for ( i1 = i0; i1 <= 1; i1++ ) {
				float sum = NiAbs( diag[i1] ) + NiAbs( diag[i1 + 1] );
				if ( NiAbs( subDiag[i1] ) + sum == sum ) {
					break;
				}
			}
			if ( i1 == i0 ) {
				break;
			}

			float tmp0 = ( diag[i0 + 1] - diag[i0] ) / ( 2.0f * subDiag[i0] );
			float tmp1 = sqrtf( tmp0 * tmp0 + 1.0f );
			if ( tmp0 < 0.0f ) {
				tmp0 = diag[i1] - diag[i0] + subDiag[i0] / ( tmp0 - tmp1 );
			} else {
				tmp0 = diag[i1] - diag[i0] + subDiag[i0] / ( tmp0 + tmp1 );
			}
			float sn = 1.0f;
			float cs = 1.0f;
			float tmp2 = 0.0f;
			for ( int i2 = i1 - 1; i2 >= i0; i2-- ) {
				float tmp3 = sn * subDiag[i2];
				float tmp4 = cs * subDiag[i2];
				if ( NiAbs( tmp3 ) >= NiAbs( tmp0 ) ) {
					cs = tmp0 / tmp3;
					tmp1 = sqrtf( cs * cs + 1.0f );
					subDiag[i2 + 1] = tmp3 * tmp1;
					sn = 1.0f / tmp1;
					cs *= sn;
				} else {
					sn = tmp3 / tmp0;
					tmp1 = sqrtf( sn * sn + 1.0f );
					subDiag[i2 + 1] = tmp0 * tmp1;
					cs = 1.0f / tmp1;
					sn *= cs;
				}
				tmp0 = diag[i2 + 1] - tmp2;
				tmp1 = ( diag[i2] - tmp0 ) * sn + 2.0f * tmp4 * cs;
				tmp2 = sn * tmp1;
				diag[i2 + 1] = tmp0 + tmp2;
				tmp0 = cs * tmp1 - tmp4;

				for ( int row = 0; row < 3; row++ ) {
					tmp3 = entry[row][i2 + 1];
					entry[row][i2 + 1] = sn * entry[row][i2] + cs * tmp3;
					entry[row][i2] = cs * entry[row][i2] - sn * tmp3;
				}
			}
			diag[i0] -= tmp2;
			subDiag[i0] = tmp0;
			subDiag[i1] = 0.0f;
		}

		if ( iter == maxIter ) {
			// should not happen for a symmetric matrix
			return false;
		}
	}
	return true;
}

/*
================
TGMatrix3::ToEulerAnglesXYZ
================
*/
// FUNCTION: STBC 0x0045ad70
bool TGMatrix3::ToEulerAnglesXYZ( float *xAngle, float *yAngle, float *zAngle ) {
	*yAngle = -NiASin( entry[0][2] );
	if ( *yAngle < NI_HALF_PI ) {
		if ( *yAngle > -NI_HALF_PI ) {
			*xAngle = -NiFastATan2( -entry[1][2], entry[2][2] );
			*zAngle = -NiFastATan2( -entry[0][1], entry[0][0] );
			return true;
		}
		// not unique: any zAngle works with the matching xAngle
		float angle = NiFastATan2( entry[1][0], entry[1][1] );
		*zAngle = 0.0f;
		*xAngle = angle;
		return false;
	}
	float angle = NiFastATan2( entry[1][0], entry[1][1] );
	*zAngle = 0.0f;
	*xAngle = -angle;
	return false;
}

/*
================
TGMatrix3::ToEulerAnglesXZY
================
*/
// FUNCTION: STBC 0x0045ae80
bool TGMatrix3::ToEulerAnglesXZY( float *xAngle, float *zAngle, float *yAngle ) {
	*zAngle = NiASin( entry[0][1] );
	if ( *zAngle < NI_HALF_PI ) {
		if ( *zAngle > -NI_HALF_PI ) {
			*xAngle = -NiFastATan2( entry[2][1], entry[1][1] );
			*yAngle = -NiFastATan2( entry[0][2], entry[0][0] );
			return true;
		}
		// not unique: any yAngle works with the matching xAngle
		float angle = NiFastATan2( -entry[2][0], entry[2][2] );
		*yAngle = 0.0f;
		*xAngle = angle;
		return false;
	}
	float angle = NiFastATan2( -entry[2][0], entry[2][2] );
	*yAngle = 0.0f;
	*xAngle = -angle;
	return false;
}

/*
================
TGMatrix3::ToEulerAnglesYXZ
================
*/
// FUNCTION: STBC 0x0045af80
bool TGMatrix3::ToEulerAnglesYXZ( float *yAngle, float *xAngle, float *zAngle ) {
	*xAngle = NiASin( entry[1][2] );
	if ( *xAngle < NI_HALF_PI ) {
		if ( *xAngle > -NI_HALF_PI ) {
			*yAngle = -NiFastATan2( entry[0][2], entry[2][2] );
			*zAngle = -NiFastATan2( entry[1][0], entry[1][1] );
			return true;
		}
		// not unique: any zAngle works with the matching yAngle
		float angle = NiFastATan2( -entry[0][1], entry[0][0] );
		*zAngle = 0.0f;
		*yAngle = angle;
		return false;
	}
	float angle = NiFastATan2( -entry[0][1], entry[0][0] );
	*zAngle = 0.0f;
	*yAngle = -angle;
	return false;
}

/*
================
TGMatrix3::ToEulerAnglesYZX
================
*/
// FUNCTION: STBC 0x0045b080
bool TGMatrix3::ToEulerAnglesYZX( float *yAngle, float *zAngle, float *xAngle ) {
	*zAngle = -NiASin( entry[1][0] );
	if ( *zAngle < NI_HALF_PI ) {
		if ( *zAngle > -NI_HALF_PI ) {
			*yAngle = -NiFastATan2( -entry[2][0], entry[0][0] );
			*xAngle = -NiFastATan2( -entry[1][2], entry[1][1] );
			return true;
		}
		// not unique: any xAngle works with the matching yAngle
		float angle = NiFastATan2( entry[2][1], entry[2][2] );
		*xAngle = 0.0f;
		*yAngle = angle;
		return false;
	}
	float angle = NiFastATan2( entry[2][1], entry[2][2] );
	*xAngle = 0.0f;
	*yAngle = -angle;
	return false;
}

/*
================
TGMatrix3::ToEulerAnglesZXY
================
*/
// FUNCTION: STBC 0x0045b190
bool TGMatrix3::ToEulerAnglesZXY( float *zAngle, float *xAngle, float *yAngle ) {
	*xAngle = -NiASin( entry[2][1] );
	if ( *xAngle < NI_HALF_PI ) {
		if ( *xAngle > -NI_HALF_PI ) {
			*zAngle = -NiFastATan2( -entry[0][1], entry[1][1] );
			*yAngle = -NiFastATan2( -entry[2][0], entry[2][2] );
			return true;
		}
		// not unique: any yAngle works with the matching zAngle
		float angle = NiFastATan2( entry[0][2], entry[0][0] );
		*yAngle = 0.0f;
		*zAngle = angle;
		return false;
	}
	float angle = NiFastATan2( entry[0][2], entry[0][0] );
	*yAngle = 0.0f;
	*zAngle = -angle;
	return false;
}

/*
================
TGMatrix3::ToEulerAnglesZYX
================
*/
// FUNCTION: STBC 0x0045b2a0
bool TGMatrix3::ToEulerAnglesZYX( float *zAngle, float *yAngle, float *xAngle ) {
	*yAngle = NiASin( entry[2][0] );
	if ( *yAngle < NI_HALF_PI ) {
		if ( *yAngle > -NI_HALF_PI ) {
			*zAngle = -NiFastATan2( entry[1][0], entry[0][0] );
			*xAngle = -NiFastATan2( entry[2][1], entry[2][2] );
			return true;
		}
		// not unique: any xAngle works with the matching zAngle
		float angle = NiFastATan2( -entry[0][1], entry[0][2] );
		*xAngle = 0.0f;
		*zAngle = angle;
		return false;
	}
	float angle = NiFastATan2( -entry[0][1], entry[0][2] );
	*xAngle = 0.0f;
	*zAngle = -angle;
	return false;
}

/*
================
TGMatrix3::FromEulerAnglesXYZ
================
*/
// FUNCTION: STBC 0x0045b3a0
void TGMatrix3::FromEulerAnglesXYZ( float xAngle, float yAngle, float zAngle ) {
	TGMatrix3 rotX, rotY, rotZ;
	rotX.MakeXRotation( xAngle );
	rotY.MakeYRotation( yAngle );
	rotZ.MakeZRotation( zAngle );
	*this = rotX * ( rotY * rotZ );
}

/*
================
TGMatrix3::FromEulerAnglesXZY
================
*/
// FUNCTION: STBC 0x0045b450
void TGMatrix3::FromEulerAnglesXZY( float xAngle, float zAngle, float yAngle ) {
	TGMatrix3 rotX, rotY, rotZ;
	rotX.MakeXRotation( xAngle );
	rotY.MakeYRotation( yAngle );
	rotZ.MakeZRotation( zAngle );
	*this = rotX * ( rotZ * rotY );
}

/*
================
TGMatrix3::FromEulerAnglesYXZ
================
*/
// FUNCTION: STBC 0x0045b500
void TGMatrix3::FromEulerAnglesYXZ( float yAngle, float xAngle, float zAngle ) {
	TGMatrix3 rotX, rotY, rotZ;
	rotX.MakeXRotation( xAngle );
	rotY.MakeYRotation( yAngle );
	rotZ.MakeZRotation( zAngle );
	*this = rotY * ( rotX * rotZ );
}

/*
================
TGMatrix3::FromEulerAnglesYZX
================
*/
// FUNCTION: STBC 0x0045b5b0
void TGMatrix3::FromEulerAnglesYZX( float yAngle, float zAngle, float xAngle ) {
	TGMatrix3 rotX, rotY, rotZ;
	rotX.MakeXRotation( xAngle );
	rotY.MakeYRotation( yAngle );
	rotZ.MakeZRotation( zAngle );
	*this = rotY * ( rotZ * rotX );
}

/*
================
TGMatrix3::FromEulerAnglesZXY
================
*/
// FUNCTION: STBC 0x0045b660
void TGMatrix3::FromEulerAnglesZXY( float zAngle, float xAngle, float yAngle ) {
	TGMatrix3 rotX, rotY, rotZ;
	rotX.MakeXRotation( xAngle );
	rotY.MakeYRotation( yAngle );
	rotZ.MakeZRotation( zAngle );
	*this = rotZ * ( rotX * rotY );
}

/*
================
TGMatrix3::FromEulerAnglesZYX
================
*/
// FUNCTION: STBC 0x0045b710
void TGMatrix3::FromEulerAnglesZYX( float zAngle, float yAngle, float xAngle ) {
	TGMatrix3 rotX, rotY, rotZ;
	rotX.MakeXRotation( xAngle );
	rotY.MakeYRotation( yAngle );
	rotZ.MakeZRotation( zAngle );
	*this = rotZ * ( rotY * rotX );
}
