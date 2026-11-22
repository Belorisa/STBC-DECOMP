/*
===============================================================================

	NiPoint3.h

	A point or vector in 3D space.

	Evidence: NiPoint3.cpp streams three floats and prints them
	("%s = (%f,%f,%f)"); its statics are four constant points (one zero, three
	unit axes) and the fast square root table.

===============================================================================
*/

#ifndef __NIPOINT3_H__
#define __NIPOINT3_H__

#include <math.h>
#include "NiMath.h"

class NiStream;

class NiPoint3 {
public:
	float			x;
	float			y;
	float			z;

					NiPoint3() {}
					NiPoint3( float x_, float y_, float z_ );
	// FUNCTION: STBC 0x00423480 ??0NiPoint3@@QAE@ABV0@@Z @game/TorpedoTube
					NiPoint3( const NiPoint3 &pt ) { x = pt.x; y = pt.y; z = pt.z; }

	// Gamebryo 1.1.2: NiPoint3::operator[] (the components by index)
	float &			operator[]( int i ) const { const float *base = &x; return (float &)base[i]; }

	// FUNCTION: STBC 0x00414560 ??4NiPoint3@@QAEAAV0@ABV0@@Z @netimmerse/NiBound
	NiPoint3 &		operator=( const NiPoint3 &pt ) { x = pt.x; y = pt.y; z = pt.z; return *this; }

	// bool, as Gamebryo 1.1.2 declares them: ShipSubsystem::UpdateDamagePoint tests
	// the out-of-line operator=='s result with test al, al.
	bool			operator==( const NiPoint3 &pt ) const;
	bool			operator!=( const NiPoint3 &pt ) const { return !( *this == pt ); }

	NiPoint3		operator+( const NiPoint3 &pt ) const;
	NiPoint3		operator-( const NiPoint3 &pt ) const;
	// the dot product
	float			operator*( const NiPoint3 &pt ) const;
	NiPoint3		operator*( float scalar ) const;
	NiPoint3		operator/( float scalar ) const;
	NiPoint3		operator-() const;
	friend NiPoint3	operator*( float scalar, const NiPoint3 &pt );

	NiPoint3 &		operator+=( const NiPoint3 &pt );
	NiPoint3 &		operator-=( const NiPoint3 &pt );
	NiPoint3 &		operator*=( float scalar );
	NiPoint3 &		operator/=( float scalar );

	float			Length() const;
	float			SqrLength() const;
	float			Dot( const NiPoint3 &pt ) const;
	float			Unitize();
	NiPoint3		Cross( const NiPoint3 &pt ) const;
	NiPoint3		UnitCross( const NiPoint3 &pt ) const;
	NiPoint3		Perpendicular() const;

	// streaming
	void			LoadBinary( NiStream &stream );
	void			SaveBinary( NiStream &stream );
	char *			GetViewerString( const char *prefix ) const;

	static void		UnitizeVectors( NiPoint3 *vectors, unsigned int count );
	static void		UnitizeVector( NiPoint3 &vector );
	static float	VectorLength( NiPoint3 &vector );

	static const NiPoint3	ZERO;
	static const NiPoint3	UNIT_X;
	static const NiPoint3	UNIT_Y;
	static const NiPoint3	UNIT_Z;

	// The fast square root table: for each of the 128 leading mantissa
	// values, the mantissa of the square root, for even (first half) and odd
	// (second half) exponents.
	static unsigned int *	InitSqrtTable();

	static unsigned int *	sqrtTable;
};

/*
================
NiPoint3 inlines

Defined after the class, as Gamebryo 1.1.2's NiPoint3.inl.
================
*/
// FUNCTION: STBC 0x00414540 ??0NiPoint3@@QAE@MMM@Z @netimmerse/NiBound
inline NiPoint3::NiPoint3( float x_, float y_, float z_ ) {
	x = x_;
	y = y_;
	z = z_;
}

// FUNCTION: STBC 0x0056ce70 ??8NiPoint3@@QBE_NABV0@@Z @game/ShipSubsystem
inline bool NiPoint3::operator==( const NiPoint3 &pt ) const {
	return x == pt.x && y == pt.y && z == pt.z;
}

// FUNCTION: STBC 0x00426c30 ??HNiPoint3@@QBE?AV0@ABV0@@Z @netimmerse/NiBound
inline NiPoint3 NiPoint3::operator+( const NiPoint3 &pt ) const {
	return NiPoint3( x + pt.x, y + pt.y, z + pt.z );
}

// FUNCTION: STBC 0x004695f0 ??GNiPoint3@@QBE?AV0@ABV0@@Z @netimmerse/NiTCBPosKey
inline NiPoint3 NiPoint3::operator-( const NiPoint3 &pt ) const {
	return NiPoint3( x - pt.x, y - pt.y, z - pt.z );
}

// its out-of-line copy is 0x469620
// FUNCTION: STBC 0x469620 ??DNiPoint3@@QBEMABV0@@Z @game/OBBNode
inline float NiPoint3::operator*( const NiPoint3 &pt ) const {
	return x * pt.x + y * pt.y + z * pt.z;
}

// FUNCTION: STBC 0x0041cbd0 ??DNiPoint3@@QBE?AV0@M@Z @netimmerse/NiOBBLeaf
inline NiPoint3 NiPoint3::operator*( float scalar ) const {
	return NiPoint3( scalar * x, scalar * y, scalar * z );
}

// FUNCTION: STBC 0x00414580 ??KNiPoint3@@QBE?AV0@M@Z @game/SetClass
inline NiPoint3 NiPoint3::operator/( float scalar ) const {
	float invScalar = 1.0f / scalar;
	return NiPoint3( invScalar * x, invScalar * y, invScalar * z );
}

// FUNCTION: STBC 0x00580570 ??GNiPoint3@@QBE?AV0@XZ @game/TractorBeamProjector
inline NiPoint3 NiPoint3::operator-() const {
	return NiPoint3( -x, -y, -z );
}

// FUNCTION: STBC 0x004234a0 ??D@YA?AVNiPoint3@@MABV0@@Z @netimmerse/NiBound
inline NiPoint3 operator*( float scalar, const NiPoint3 &pt ) {
	return NiPoint3( scalar * pt.x, scalar * pt.y, scalar * pt.z );
}

// FUNCTION: STBC 0x004234d0 ??YNiPoint3@@QAEAAV0@ABV0@@Z @game/CameraMode
inline NiPoint3 &NiPoint3::operator+=( const NiPoint3 &pt ) {
	x += pt.x;
	y += pt.y;
	z += pt.z;
	return *this;
}

// FUNCTION: STBC 0x00423500 ??ZNiPoint3@@QAEAAV0@ABV0@@Z @game/AnimatedTriShapeParticles
inline NiPoint3 &NiPoint3::operator-=( const NiPoint3 &pt ) {
	x -= pt.x;
	y -= pt.y;
	z -= pt.z;
	return *this;
}

// FUNCTION: STBC 0x00423530 ??XNiPoint3@@QAEAAV0@M@Z @game/CameraMode
inline NiPoint3 &NiPoint3::operator*=( float scalar ) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

inline NiPoint3 &NiPoint3::operator/=( float scalar ) {
	float invScalar = 1.0f / scalar;
	x *= invScalar;
	y *= invScalar;
	z *= invScalar;
	return *this;
}

// FUNCTION: STBC 0x004145b0 ?Length@NiPoint3@@QBEMXZ @netimmerse/NiBound
inline float NiPoint3::Length() const {
	return NiSqrt( x * x + y * y + z * z );
}

// FUNCTION: STBC 0x00427860 ?SqrLength@NiPoint3@@QBEMXZ @game/CameraMode
inline float NiPoint3::SqrLength() const {
	return x * x + y * y + z * z;
}

// FUNCTION: STBC 0x00428160 ?Dot@NiPoint3@@QBEMABV1@@Z @netimmerse/NiTriIntersect
inline float NiPoint3::Dot( const NiPoint3 &pt ) const {
	return x * pt.x + y * pt.y + z * pt.z;
}

/*
================
NiPoint3::Unitize

Scales the point to unit length and returns the old length; a point
shorter than 1e-06 becomes zero and returns zero.

Evidence: inlined with this shape (1e-06 at 0x888B58, the reciprocal
multiplied in, zeros otherwise) in NetImmerse code (0x7E8383) and in
TGPoint3::GetRandomUnitVector (0x45E7B0). Length goes through NiSqrt, as
Gamebryo 1.1.2 writes it.
================
*/
// FUNCTION: STBC 0x00423560 ?Unitize@NiPoint3@@QAEMXZ @game/PulseWeapon
inline float NiPoint3::Unitize() {
	float length = Length();
	if ( length > 1e-06f ) {
		float invLength = 1.0f / length;
		x *= invLength;
		y *= invLength;
		z *= invLength;
	} else {
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		length = 0.0f;
	}
	return length;
}

// FUNCTION: STBC 0x42BC70 ?Cross@NiPoint3@@QBE?AV1@ABV1@@Z
inline NiPoint3 NiPoint3::Cross( const NiPoint3 &pt ) const {
	return NiPoint3( y * pt.z - z * pt.y, z * pt.x - x * pt.z, x * pt.y - y * pt.x );
}

/*
================
NiPoint3::UnitCross

The cross product scaled to unit length, or zero when it is shorter than
1e-06 (NiPlane's three-point constructor, 0x8168F0).
================
*/
// FUNCTION: STBC 0x00422770 ?UnitCross@NiPoint3@@QBE?AV1@ABV1@@Z @game/CameraMode
inline NiPoint3 NiPoint3::UnitCross( const NiPoint3 &pt ) const {
	NiPoint3 cross( y * pt.z - z * pt.y, z * pt.x - x * pt.z, x * pt.y - y * pt.x );
	float length = cross.Length();
	if ( length > 1e-06f ) {
		return cross / length;
	}
	return NiPoint3( 0.0f, 0.0f, 0.0f );
}

/*
================
NiPoint3::Perpendicular

A unit vector perpendicular to the point: the unit cross product with the
axis along which the point is shortest. Gamebryo 1.2.2: NiPoint3::
Perpendicular; the SWIG wrapper NiPoint3_Perpendicular (0x5BE680) runs
this body inline, comparing the fabs results as doubles.
================
*/
inline NiPoint3 NiPoint3::Perpendicular() const {
	if ( fabs( x ) < fabs( y ) ) {
		if ( fabs( z ) < fabs( x ) ) {
			return UnitCross( UNIT_Z );
		} else {
			return UnitCross( UNIT_X );
		}
	} else {
		if ( fabs( z ) < fabs( y ) ) {
			return UnitCross( UNIT_Z );
		} else {
			return UnitCross( UNIT_Y );
		}
	}
}

/*
================
NiPoint3::UnitizeVector

Scales the vector to unit length with the table's square root (see
UnitizeVectors); a zero vector stays zero. Gamebryo 1.2.2: NiPoint3::UnitizeVector.
================
*/
// FUNCTION: STBC 0x80D870 ?UnitizeVector@NiPoint3@@SAXAAV1@@Z @netimmerse/NiTriStripData
inline void NiPoint3::UnitizeVector( NiPoint3 &vector ) {
	float length = vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
	unsigned int *rep = (unsigned int *)&length;
	if ( !( *rep ) ) {
		length = 0.0f;
	} else {
		short exponent = ( *rep >> 23 ) - 127;
		*rep &= 0x7FFFFF;
		if ( exponent & 1 ) {
			*rep |= 0x800000;
		}
		exponent >>= 1;
		*rep = sqrtTable[*rep >> 16] | ( ( exponent + 127 ) << 23 );
		length = 1.0f / length;
	}
	vector.x *= length;
	vector.y *= length;
	vector.z *= length;
}

/*
================
NiPoint3::VectorLength

The vector's length by the table's square root (see UnitizeVectors); 0
for a zero vector. Gamebryo 1.2.2: NiPoint3::VectorLength.
================
*/
inline float NiPoint3::VectorLength( NiPoint3 &vector ) {
	float length = vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
	unsigned int *rep = (unsigned int *)&length;
	if ( !( *rep ) ) {
		return 0.0f;
	} else {
		short exponent = ( *rep >> 23 ) - 127;
		*rep &= 0x7FFFFF;
		if ( exponent & 1 ) {
			*rep |= 0x800000;
		}
		exponent >>= 1;
		*rep = sqrtTable[*rep >> 16] | ( ( exponent + 127 ) << 23 );
		return length;
	}
}

#endif /* !__NIPOINT3_H__ */
